/**
 * @file ImageMemoryPool.h
 * @brief 图像内存池 - 预分配、零拷贝
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "base/ImageData.h"
#include <QMutex>
#include <QJsonObject>
#include <map>
#include <vector>
#include <atomic>

namespace VisionForge {
namespace Base {

/**
 * @brief 图像内存池（单例）
 *
 * 核心功能：
 * - 对象池：预分配ImageData对象，避免频繁malloc/free
 * - 性能统计：记录分配次数、命中率等
 * - 自动管理：超出限制时自动清理
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
     * @brief 设置最大池大小
     */
    void setMaxPoolSize(size_t maxSize) { maxPoolSize_ = maxSize; }

    /**
     * @brief 获取最大池大小
     */
    size_t getMaxPoolSize() const { return maxPoolSize_; }

    // ========== 统计信息 ==========

    /**
     * @brief 统计信息结构
     */
    struct Statistics {
        size_t totalAllocations;      // 总分配次数
        size_t cacheHits;              // 缓存命中次数
        size_t cacheMisses;            // 缓存未命中次数
        double hitRate;                // 命中率
        size_t poolSize;               // 当前池大小
        size_t memoryUsage;            // 内存使用量（字节）

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

private:
    std::map<PoolKey, std::vector<ImageData::Ptr>> pool_;
    mutable QMutex mutex_;

    size_t maxPoolSize_;               // 最大池大小（默认10）

    // 统计信息
    mutable std::atomic<size_t> totalAllocations_{0};
    mutable std::atomic<size_t> cacheHits_{0};
    mutable std::atomic<size_t> cacheMisses_{0};
};

} // namespace Base
} // namespace VisionForge
