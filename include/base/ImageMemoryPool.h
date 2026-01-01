/**
 * @file ImageMemoryPool.h
 * @brief 图像内存池 - 预分配、零拷贝、内存压力管理
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "base/ImageData.h"
#include <QMutex>
#include <QRecursiveMutex>
#include <QJsonObject>
#include <QTimer>
#include <map>
#include <vector>
#include <atomic>
#include <chrono>
#include <functional>

namespace VisionForge {
namespace Base {

/**
 * @brief 内存池清理策略
 */
enum class PoolCleanupStrategy {
    LRU,        ///< 最近最少使用
    LFU,        ///< 最不经常使用
    FIFO,       ///< 先进先出
    Size        ///< 优先清理大内存块
};

/**
 * @brief 图像内存池（单例）
 *
 * 核心功能：
 * - 对象池：预分配ImageData对象，避免频繁malloc/free
 * - 性能统计：记录分配次数、命中率等
 * - 自动管理：超出限制时自动清理
 * - 内存压力监控：自动根据系统内存压力调整池大小
 * - 多种清理策略：LRU、LFU、FIFO、Size
 */
class ImageMemoryPool {
public:
    /**
     * @brief 获取单例实例
     */
    static ImageMemoryPool& instance();

    /**
     * @brief 分配图像（从池中获取或新建）
     * @param width 图像宽度
     * @param height 图像高度
     * @param type OpenCV图像类型
     * @return ImageData智能指针
     */
    ImageData::Ptr allocate(int width, int height, int type);

    /**
     * @brief 归还图像到池中
     * @param image 图像智能指针
     * @note 通常不需要手动调用，shared_ptr会自动管理
     */
    void release(ImageData::Ptr image);

    /**
     * @brief 预分配指定规格的图像
     * @param width 图像宽度
     * @param height 图像高度
     * @param type OpenCV图像类型
     * @param count 预分配数量
     */
    void preallocate(int width, int height, int type, int count);

    /**
     * @brief 清空池
     */
    void clear();

    // ========== 池状态 ==========

    /**
     * @brief 池中总对象数量
     */
    size_t size() const;

    /**
     * @brief 指定规格的对象数量
     */
    size_t size(int width, int height, int type) const;

    /**
     * @brief 总内存使用量（字节）
     */
    size_t getTotalMemoryUsage() const;

    // ========== 池配置 ==========

    /**
     * @brief 设置最大池大小（每种规格）
     */
    void setMaxPoolSize(size_t maxSize) { maxPoolSize_ = maxSize; }

    /**
     * @brief 获取最大池大小
     */
    size_t getMaxPoolSize() const { return maxPoolSize_; }

    /**
     * @brief 设置总内存限制（字节）
     * @param limit 内存限制（0表示无限制）
     */
    void setMemoryLimit(size_t limit) { memoryLimit_ = limit; }

    /**
     * @brief 获取总内存限制
     */
    size_t getMemoryLimit() const { return memoryLimit_; }

    /**
     * @brief 设置清理策略
     */
    void setCleanupStrategy(PoolCleanupStrategy strategy) { cleanupStrategy_ = strategy; }

    /**
     * @brief 获取清理策略
     */
    PoolCleanupStrategy getCleanupStrategy() const { return cleanupStrategy_; }

    /**
     * @brief 启用/禁用自动清理
     * @param enabled 是否启用
     * @param intervalMs 检查间隔（毫秒）
     */
    void setAutoCleanup(bool enabled, int intervalMs = 5000);

    /**
     * @brief 检查是否启用自动清理
     */
    bool isAutoCleanupEnabled() const { return autoCleanup_; }

    /**
     * @brief 触发内存压力清理
     * @param targetReduction 目标减少的内存比例 (0.0-1.0)
     */
    void triggerMemoryPressureCleanup(double targetReduction = 0.5);

    /**
     * @brief 收缩池到目标大小
     * @param targetSize 目标大小
     */
    void shrinkTo(size_t targetSize);

    // ========== 统计信息 ==========

    /**
     * @brief 统计信息结构
     */
    struct Statistics {
        size_t totalAllocations;       ///< 总分配次数
        size_t cacheHits;              ///< 缓存命中次数
        size_t cacheMisses;            ///< 缓存未命中次数
        double hitRate;                ///< 命中率
        size_t poolSize;               ///< 当前池大小
        size_t memoryUsage;            ///< 内存使用量（字节）
        size_t memoryLimit;            ///< 内存限制（字节）
        double memoryUsageRatio;       ///< 内存使用率
        size_t cleanupCount;           ///< 清理次数
        size_t evictedCount;           ///< 被清理的对象数

        QJsonObject toJson() const;
    };

    /**
     * @brief 获取统计信息
     */
    Statistics getStatistics() const;

    /**
     * @brief 重置统计信息
     */
    void resetStatistics();

    // ========== 配置持久化 ==========

    /**
     * @brief 从配置文件加载设置
     * @note 支持的格式: "auto", "25%", "512MB", "1GB" 等
     */
    void loadSettings();

    /**
     * @brief 保存设置到配置文件
     */
    void saveSettings();

private:
    // 私有构造函数（单例）
    ImageMemoryPool();
    ~ImageMemoryPool();

    // 禁止拷贝
    ImageMemoryPool(const ImageMemoryPool&) = delete;
    ImageMemoryPool& operator=(const ImageMemoryPool&) = delete;

    // 池键定义
    struct PoolKey {
        int width;
        int height;
        int type;

        bool operator<(const PoolKey& other) const
        {
            if (width != other.width) return width < other.width;
            if (height != other.height) return height < other.height;
            return type < other.type;
        }

        size_t getMemorySize() const;
    };

    // 清理超出限制的池对象
    void evictExcess();

    /**
     * @brief 池项元数据（用于LRU/LFU策略）
     */
    struct PoolItemMeta {
        std::chrono::steady_clock::time_point lastAccess;  ///< 最后访问时间
        size_t accessCount;                                 ///< 访问计数
        size_t memorySize;                                  ///< 内存大小

        PoolItemMeta() : accessCount(0), memorySize(0) {
            lastAccess = std::chrono::steady_clock::now();
        }
    };

    /**
     * @brief 执行自动清理检查
     */
    void performAutoCleanup();

    /**
     * @brief 根据策略选择要清理的键
     */
    std::vector<PoolKey> selectKeysForCleanup(size_t targetCount) const;

private:
    std::map<PoolKey, std::vector<ImageData::Ptr>> pool_;
    std::map<PoolKey, PoolItemMeta> poolMeta_;  ///< 池项元数据
    mutable QRecursiveMutex mutex_;  ///< 递归互斥锁（避免嵌套调用死锁）

    size_t maxPoolSize_;               ///< 每种规格的最大池大小（默认10）
    size_t memoryLimit_;               ///< 总内存限制（字节，0表示无限制）
    PoolCleanupStrategy cleanupStrategy_;  ///< 清理策略
    bool autoCleanup_;                 ///< 是否启用自动清理
    int autoCleanupInterval_;          ///< 自动清理检查间隔（毫秒）

    // 统计信息
    mutable std::atomic<size_t> totalAllocations_{0};
    mutable std::atomic<size_t> cacheHits_{0};
    mutable std::atomic<size_t> cacheMisses_{0};
    mutable std::atomic<size_t> cleanupCount_{0};
    mutable std::atomic<size_t> evictedCount_{0};

    // 内存使用跟踪（原子计数器，O(1)查询）
    mutable std::atomic<size_t> currentMemoryUsage_{0};

    // 清理节流控制
    mutable std::chrono::steady_clock::time_point lastCleanupTime_;
    static constexpr std::chrono::milliseconds CLEANUP_COOLDOWN{1000};  // 1秒冷却期
};

} // namespace Base
} // namespace VisionForge
