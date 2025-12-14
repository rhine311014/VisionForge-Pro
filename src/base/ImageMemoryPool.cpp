/**
 * @file ImageMemoryPool.cpp
 * @brief 图像内存池实现
 */

#include "base/ImageMemoryPool.h"
#include <opencv2/core.hpp>
#include <QMutexLocker>

namespace VisionForge {
namespace Base {

// 默认最大池大小
static constexpr size_t DEFAULT_MAX_POOL_SIZE = 10;

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
    return json;
}

// ============================================================
// 构造/析构
// ============================================================

ImageMemoryPool::ImageMemoryPool()
    : maxPoolSize_(DEFAULT_MAX_POOL_SIZE)
{
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

        return image;
    }

    // 未命中，创建新对象
    cacheMisses_++;

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

    vec.push_back(image);
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
}

void ImageMemoryPool::clear()
{
    QMutexLocker locker(&mutex_);
    pool_.clear();
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
    QMutexLocker locker(&mutex_);

    size_t total = 0;
    for (const auto& pair : pool_) {
        size_t count = pair.second.size();
        size_t memPerImage = pair.first.getMemorySize();
        total += count * memPerImage;
    }

    return total;
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

    return stats;
}

void ImageMemoryPool::resetStatistics()
{
    totalAllocations_.store(0);
    cacheHits_.store(0);
    cacheMisses_.store(0);
}

// ============================================================
// 内部辅助函数
// ============================================================

void ImageMemoryPool::evictExcess()
{
    // 清理超出限制的对象
    QMutexLocker locker(&mutex_);

    for (auto& pair : pool_) {
        auto& vec = pair.second;
        if (vec.size() > maxPoolSize_) {
            vec.resize(maxPoolSize_);
        }
    }
}

} // namespace Base
} // namespace VisionForge
