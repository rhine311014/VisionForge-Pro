/**
 * @file ImageMemoryPool.cpp
 * @brief 图像内存池实现
 */

#include "base/ImageMemoryPool.h"
#include "base/Logger.h"
#include "base/ConfigManager.h"
#include <opencv2/core.hpp>
#include <QMutexLocker>
#include <algorithm>

#ifdef Q_OS_WIN
#ifndef NOMINMAX
#define NOMINMAX  // 防止 windows.h 定义 min/max 宏
#endif
#include <windows.h>
#elif defined(Q_OS_LINUX)
#include <sys/sysinfo.h>
#elif defined(Q_OS_MAC)
#include <sys/sysctl.h>
#endif

namespace VisionForge {
namespace Base {

// 默认最大池大小
static constexpr size_t DEFAULT_MAX_POOL_SIZE = 10;
static constexpr size_t MIN_MEMORY_LIMIT = 128 * 1024 * 1024;       // 最小128MB
static constexpr size_t MAX_MEMORY_LIMIT = 2048ULL * 1024 * 1024;   // 最大2GB
static constexpr double DEFAULT_MEMORY_PERCENT = 0.10;               // 默认使用系统内存的10%

/**
 * @brief 获取系统总物理内存（字节）
 */
static size_t getSystemTotalMemory()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return static_cast<size_t>(memInfo.ullTotalPhys);
    }
#elif defined(Q_OS_LINUX)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return static_cast<size_t>(info.totalram) * info.mem_unit;
    }
#elif defined(Q_OS_MAC)
    int64_t memsize;
    size_t len = sizeof(memsize);
    if (sysctlbyname("hw.memsize", &memsize, &len, NULL, 0) == 0) {
        return static_cast<size_t>(memsize);
    }
#endif
    // 默认假设8GB
    return 8ULL * 1024 * 1024 * 1024;
}

/**
 * @brief 计算自适应内存限制
 */
static size_t calculateAdaptiveMemoryLimit()
{
    size_t totalMemory = getSystemTotalMemory();
    size_t limit = static_cast<size_t>(totalMemory * DEFAULT_MEMORY_PERCENT);

    // 限制在合理范围内
    limit = std::max(limit, MIN_MEMORY_LIMIT);
    limit = std::min(limit, MAX_MEMORY_LIMIT);

    return limit;
}

/**
 * @brief 解析内存大小字符串（支持KB、MB、GB、%、auto）
 */
static size_t parseMemoryLimit(const QString& sizeStr)
{
    QString str = sizeStr.trimmed().toLower();

    // 自动检测
    if (str == "auto" || str.isEmpty()) {
        return calculateAdaptiveMemoryLimit();
    }

    // 百分比
    if (str.endsWith("%")) {
        double percent = str.chopped(1).toDouble() / 100.0;
        percent = std::max(0.01, std::min(0.5, percent));  // 限制1%-50%
        size_t totalMemory = getSystemTotalMemory();
        size_t limit = static_cast<size_t>(totalMemory * percent);
        return std::max(MIN_MEMORY_LIMIT, std::min(MAX_MEMORY_LIMIT, limit));
    }

    // 绝对值
    size_t multiplier = 1;
    if (str.endsWith("kb")) {
        multiplier = 1024;
        str.chop(2);
    } else if (str.endsWith("mb")) {
        multiplier = 1024 * 1024;
        str.chop(2);
    } else if (str.endsWith("gb")) {
        multiplier = 1024ULL * 1024 * 1024;
        str.chop(2);
    } else if (str.endsWith("b")) {
        str.chop(1);
    }

    bool ok;
    double value = str.toDouble(&ok);
    if (!ok || value <= 0) {
        return calculateAdaptiveMemoryLimit();
    }

    size_t limit = static_cast<size_t>(value * multiplier);
    return std::max(MIN_MEMORY_LIMIT, std::min(MAX_MEMORY_LIMIT, limit));
}

// ============================================================
// PoolKey实现
// ============================================================

size_t ImageMemoryPool::PoolKey::getMemorySize() const
{
    return static_cast<size_t>(width) * height * CV_ELEM_SIZE(type);
}

// ============================================================
// Statistics实现
// ============================================================

QJsonObject ImageMemoryPool::Statistics::toJson() const
{
    QJsonObject json;
    json["totalAllocations"] = static_cast<qint64>(totalAllocations);
    json["cacheHits"] = static_cast<qint64>(cacheHits);
    json["cacheMisses"] = static_cast<qint64>(cacheMisses);
    json["hitRate"] = hitRate;
    json["poolSize"] = static_cast<qint64>(poolSize);
    json["memoryUsage"] = static_cast<qint64>(memoryUsage);
    json["memoryLimit"] = static_cast<qint64>(memoryLimit);
    json["memoryUsageRatio"] = memoryUsageRatio;
    json["cleanupCount"] = static_cast<qint64>(cleanupCount);
    json["evictedCount"] = static_cast<qint64>(evictedCount);
    return json;
}

// ============================================================
// 构造/析构
// ============================================================

ImageMemoryPool::ImageMemoryPool()
    : maxPoolSize_(DEFAULT_MAX_POOL_SIZE)
    , memoryLimit_(calculateAdaptiveMemoryLimit())  // 默认使用自适应内存限制
    , cleanupStrategy_(PoolCleanupStrategy::LRU)
    , autoCleanup_(false)
    , autoCleanupInterval_(5000)
{
    // 从配置文件加载设置
    loadSettings();

    LOG_INFO(QString("图像内存池初始化完成: 内存限制=%1MB, 系统内存=%2GB")
             .arg(memoryLimit_ / 1024 / 1024)
             .arg(getSystemTotalMemory() / 1024 / 1024 / 1024));
}

ImageMemoryPool::~ImageMemoryPool()
{
    clear();
}

ImageMemoryPool& ImageMemoryPool::instance()
{
    static ImageMemoryPool instance;
    return instance;
}

// ============================================================
// 分配/释放
// ============================================================

ImageData::Ptr ImageMemoryPool::allocate(int width, int height, int type)
{
    totalAllocations_++;

    PoolKey key{width, height, type};

    QMutexLocker locker(&mutex_);

    // 尝试从池中获取
    auto it = pool_.find(key);
    if (it != pool_.end() && !it->second.empty()) {
        // 命中缓存
        cacheHits_++;

        ImageData::Ptr image = it->second.back();
        it->second.pop_back();

        // 更新内存计数器（从池中取出，减少内存使用）
        size_t memSize = key.getMemorySize();
        currentMemoryUsage_.fetch_sub(memSize, std::memory_order_relaxed);

        // 更新元数据
        auto metaIt = poolMeta_.find(key);
        if (metaIt != poolMeta_.end()) {
            metaIt->second.lastAccess = std::chrono::steady_clock::now();
            metaIt->second.accessCount++;
        }

        return image;
    }

    // 未命中，创建新对象
    cacheMisses_++;

    // 检查是否需要清理内存（使用O(1)原子计数器）
    size_t currentUsage = currentMemoryUsage_.load(std::memory_order_relaxed);

    if (memoryLimit_ > 0 && currentUsage > memoryLimit_) {
        locker.unlock();
        triggerMemoryPressureCleanup(0.3);
        locker.relock();
    }

    // 释放锁后创建对象（避免锁持有时间过长）
    locker.unlock();

    return std::make_shared<ImageData>(width, height, type);
}

void ImageMemoryPool::release(ImageData::Ptr image)
{
    if (!image || image->isEmpty()) {
        return;
    }

    PoolKey key{image->width(), image->height(), image->mat().type()};

    QMutexLocker locker(&mutex_);

    auto& vec = pool_[key];

    // 检查是否超出限制
    if (vec.size() >= maxPoolSize_) {
        // 池已满，不归还（让shared_ptr自动释放）
        return;
    }

    // 检查内存限制（使用O(1)原子计数器）
    size_t memSize = key.getMemorySize();
    size_t currentUsage = currentMemoryUsage_.load(std::memory_order_relaxed);
    if (memoryLimit_ > 0 && currentUsage + memSize > memoryLimit_) {
        // 超出内存限制，不归还
        return;
    }

    vec.push_back(image);

    // 更新内存计数器（放入池中，增加内存使用）
    currentMemoryUsage_.fetch_add(memSize, std::memory_order_relaxed);

    // 更新元数据
    auto& meta = poolMeta_[key];
    meta.memorySize = memSize;
    meta.lastAccess = std::chrono::steady_clock::now();
}

void ImageMemoryPool::preallocate(int width, int height, int type, int count)
{
    PoolKey key{width, height, type};

    std::vector<ImageData::Ptr> images;
    images.reserve(count);

    // 在锁外创建对象
    for (int i = 0; i < count; ++i) {
        images.push_back(std::make_shared<ImageData>(width, height, type));
    }

    // 加锁并添加到池中
    QMutexLocker locker(&mutex_);
    auto& vec = pool_[key];
    vec.insert(vec.end(), images.begin(), images.end());

    // 更新内存计数器
    size_t memSize = key.getMemorySize() * count;
    currentMemoryUsage_.fetch_add(memSize, std::memory_order_relaxed);
}

void ImageMemoryPool::clear()
{
    QMutexLocker locker(&mutex_);
    pool_.clear();
    poolMeta_.clear();

    // 重置内存计数器
    currentMemoryUsage_.store(0, std::memory_order_relaxed);
}

// ============================================================
// 池状态
// ============================================================

size_t ImageMemoryPool::size() const
{
    QMutexLocker locker(&mutex_);

    size_t total = 0;
    for (const auto& pair : pool_) {
        total += pair.second.size();
    }

    return total;
}

size_t ImageMemoryPool::size(int width, int height, int type) const
{
    PoolKey key{width, height, type};

    QMutexLocker locker(&mutex_);

    auto it = pool_.find(key);
    if (it != pool_.end()) {
        return it->second.size();
    }

    return 0;
}

size_t ImageMemoryPool::getTotalMemoryUsage() const
{
    // O(1)查询：直接返回原子计数器的值（无需加锁）
    return currentMemoryUsage_.load(std::memory_order_relaxed);
}

// ============================================================
// 统计信息
// ============================================================

ImageMemoryPool::Statistics ImageMemoryPool::getStatistics() const
{
    Statistics stats;

    stats.totalAllocations = totalAllocations_.load();
    stats.cacheHits = cacheHits_.load();
    stats.cacheMisses = cacheMisses_.load();

    if (stats.totalAllocations > 0) {
        stats.hitRate = static_cast<double>(stats.cacheHits) / stats.totalAllocations;
    } else {
        stats.hitRate = 0.0;
    }

    stats.poolSize = size();
    stats.memoryUsage = getTotalMemoryUsage();
    stats.memoryLimit = memoryLimit_;

    if (memoryLimit_ > 0) {
        stats.memoryUsageRatio = static_cast<double>(stats.memoryUsage) / memoryLimit_;
    } else {
        stats.memoryUsageRatio = 0.0;
    }

    stats.cleanupCount = cleanupCount_.load();
    stats.evictedCount = evictedCount_.load();

    return stats;
}

void ImageMemoryPool::resetStatistics()
{
    totalAllocations_.store(0);
    cacheHits_.store(0);
    cacheMisses_.store(0);
    cleanupCount_.store(0);
    evictedCount_.store(0);
}

// ============================================================
// 池配置
// ============================================================

void ImageMemoryPool::setAutoCleanup(bool enabled, int intervalMs)
{
    autoCleanup_ = enabled;
    autoCleanupInterval_ = intervalMs;

    if (enabled) {
        LOG_DEBUG(QString("自动清理已启用，间隔: %1ms").arg(intervalMs));
    } else {
        LOG_DEBUG("自动清理已禁用");
    }
}

void ImageMemoryPool::triggerMemoryPressureCleanup(double targetReduction)
{
    cleanupCount_++;

    size_t currentUsage = currentMemoryUsage_.load(std::memory_order_relaxed);
    size_t targetUsage = static_cast<size_t>(currentUsage * (1.0 - targetReduction));

    LOG_DEBUG(QString("触发内存压力清理: 当前=%1MB, 目标=%2MB")
             .arg(currentUsage / 1024 / 1024)
             .arg(targetUsage / 1024 / 1024));

    QMutexLocker locker(&mutex_);

    // 根据策略选择要清理的键
    auto keysToClean = selectKeysForCleanup(pool_.size());

    size_t evicted = 0;
    size_t freedMemory = 0;
    for (const auto& key : keysToClean) {
        auto it = pool_.find(key);
        if (it != pool_.end()) {
            size_t beforeSize = it->second.size();
            size_t memPerItem = key.getMemorySize();

            // 清理一半
            size_t toRemove = beforeSize / 2;
            if (toRemove > 0) {
                it->second.resize(beforeSize - toRemove);
                evicted += toRemove;
                freedMemory += toRemove * memPerItem;
                currentUsage -= toRemove * memPerItem;
            }

            if (currentUsage <= targetUsage) {
                break;
            }
        }
    }

    // 更新内存计数器
    currentMemoryUsage_.fetch_sub(freedMemory, std::memory_order_relaxed);

    evictedCount_ += evicted;
    LOG_DEBUG(QString("内存压力清理完成: 清理了 %1 个对象, 释放 %2MB")
             .arg(evicted).arg(freedMemory / 1024 / 1024));
}

void ImageMemoryPool::shrinkTo(size_t targetSize)
{
    QMutexLocker locker(&mutex_);

    size_t currentSize = 0;
    for (const auto& pair : pool_) {
        currentSize += pair.second.size();
    }

    if (currentSize <= targetSize) {
        return;
    }

    size_t toRemove = currentSize - targetSize;
    auto keysToClean = selectKeysForCleanup(pool_.size());

    size_t removed = 0;
    size_t freedMemory = 0;
    for (const auto& key : keysToClean) {
        auto it = pool_.find(key);
        if (it != pool_.end() && !it->second.empty()) {
            size_t canRemove = std::min(it->second.size(), toRemove - removed);
            it->second.resize(it->second.size() - canRemove);
            removed += canRemove;
            freedMemory += canRemove * key.getMemorySize();
            evictedCount_ += canRemove;

            if (removed >= toRemove) {
                break;
            }
        }
    }

    // 更新内存计数器
    currentMemoryUsage_.fetch_sub(freedMemory, std::memory_order_relaxed);

    LOG_DEBUG(QString("池收缩完成: 移除了 %1 个对象, 释放 %2MB")
             .arg(removed).arg(freedMemory / 1024 / 1024));
}

// ============================================================
// 内部辅助函数
// ============================================================

void ImageMemoryPool::evictExcess()
{
    // 清理超出限制的对象
    QMutexLocker locker(&mutex_);

    size_t freedMemory = 0;
    for (auto& pair : pool_) {
        auto& vec = pair.second;
        if (vec.size() > maxPoolSize_) {
            size_t toRemove = vec.size() - maxPoolSize_;
            vec.resize(maxPoolSize_);
            freedMemory += toRemove * pair.first.getMemorySize();
            evictedCount_ += toRemove;
        }
    }

    // 更新内存计数器
    if (freedMemory > 0) {
        currentMemoryUsage_.fetch_sub(freedMemory, std::memory_order_relaxed);
    }
}

void ImageMemoryPool::performAutoCleanup()
{
    if (!autoCleanup_) {
        return;
    }

    size_t currentUsage = getTotalMemoryUsage();
    if (memoryLimit_ > 0 && currentUsage > memoryLimit_ * 0.8) {
        // 内存使用超过80%限制，触发清理
        triggerMemoryPressureCleanup(0.3);
    }
}

std::vector<ImageMemoryPool::PoolKey> ImageMemoryPool::selectKeysForCleanup(size_t targetCount) const
{
    std::vector<std::pair<PoolKey, PoolItemMeta>> items;

    for (const auto& pair : poolMeta_) {
        items.emplace_back(pair.first, pair.second);
    }

    switch (cleanupStrategy_) {
        case PoolCleanupStrategy::LRU:
            // 按最后访问时间排序（最旧的在前）
            std::sort(items.begin(), items.end(),
                [](const auto& a, const auto& b) {
                    return a.second.lastAccess < b.second.lastAccess;
                });
            break;

        case PoolCleanupStrategy::LFU:
            // 按访问次数排序（最少的在前）
            std::sort(items.begin(), items.end(),
                [](const auto& a, const auto& b) {
                    return a.second.accessCount < b.second.accessCount;
                });
            break;

        case PoolCleanupStrategy::FIFO:
            // FIFO不需要排序，使用插入顺序
            break;

        case PoolCleanupStrategy::Size:
            // 按内存大小排序（最大的在前）
            std::sort(items.begin(), items.end(),
                [](const auto& a, const auto& b) {
                    return a.second.memorySize > b.second.memorySize;
                });
            break;
    }

    std::vector<PoolKey> result;
    size_t count = std::min(targetCount, items.size());
    for (size_t i = 0; i < count; ++i) {
        result.push_back(items[i].first);
    }

    return result;
}

// ============================================================
// 配置持久化
// ============================================================

void ImageMemoryPool::loadSettings()
{
    ConfigManager& config = ConfigManager::instance();

    // 加载内存限制配置
    // 支持格式：
    // - "auto"：自动检测系统内存的10%
    // - "25%"：系统内存的25%
    // - "512MB"：绝对值512MB
    // - "1GB"：绝对值1GB
    QString memLimitStr = config.getValue("MemoryPool/MemoryLimit", "auto").toString();
    memoryLimit_ = parseMemoryLimit(memLimitStr);

    // 加载其他配置
    maxPoolSize_ = config.getValue("MemoryPool/MaxPoolSize",
                                   static_cast<int>(DEFAULT_MAX_POOL_SIZE)).toInt();

    QString strategyStr = config.getValue("MemoryPool/CleanupStrategy", "LRU").toString();
    if (strategyStr == "LFU") {
        cleanupStrategy_ = PoolCleanupStrategy::LFU;
    } else if (strategyStr == "FIFO") {
        cleanupStrategy_ = PoolCleanupStrategy::FIFO;
    } else if (strategyStr == "Size") {
        cleanupStrategy_ = PoolCleanupStrategy::Size;
    } else {
        cleanupStrategy_ = PoolCleanupStrategy::LRU;
    }

    autoCleanup_ = config.getValue("MemoryPool/AutoCleanup", false).toBool();
    autoCleanupInterval_ = config.getValue("MemoryPool/AutoCleanupInterval", 5000).toInt();

    LOG_DEBUG(QString("内存池配置已加载: 限制=%1MB, 策略=%2, 自动清理=%3")
             .arg(memoryLimit_ / 1024 / 1024)
             .arg(strategyStr)
             .arg(autoCleanup_ ? "是" : "否"));
}

void ImageMemoryPool::saveSettings()
{
    ConfigManager& config = ConfigManager::instance();

    // 保存内存限制（保存当前值，单位MB）
    config.setValue("MemoryPool/MemoryLimit", QString("%1MB").arg(memoryLimit_ / 1024 / 1024));
    config.setValue("MemoryPool/MaxPoolSize", static_cast<int>(maxPoolSize_));

    QString strategyStr;
    switch (cleanupStrategy_) {
        case PoolCleanupStrategy::LRU:  strategyStr = "LRU"; break;
        case PoolCleanupStrategy::LFU:  strategyStr = "LFU"; break;
        case PoolCleanupStrategy::FIFO: strategyStr = "FIFO"; break;
        case PoolCleanupStrategy::Size: strategyStr = "Size"; break;
    }
    config.setValue("MemoryPool/CleanupStrategy", strategyStr);

    config.setValue("MemoryPool/AutoCleanup", autoCleanup_);
    config.setValue("MemoryPool/AutoCleanupInterval", autoCleanupInterval_);

    config.save();

    LOG_DEBUG("内存池配置已保存");
}

} // namespace Base
} // namespace VisionForge
