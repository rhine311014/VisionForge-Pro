/**
 * @file ParallelProcessor.h
 * @brief 并行处理器 - OpenMP多线程优化
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include "base/ImageData.h"
#include <QVector>
#include <QRect>
#include <functional>
#include <vector>
#include <opencv2/opencv.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace VisionForge {
namespace Base {

/**
 * @brief 并行处理器
 *
 * 提供OpenMP多线程并行处理功能：
 * - 批量图像处理
 * - 图像分块并行处理
 * - 并行ROI处理
 * - 线程池管理
 */
class ParallelProcessor {
public:
    /**
     * @brief 获取单例实例
     */
    static ParallelProcessor& instance();

    // ========== 线程配置 ==========

    /**
     * @brief 设置最大线程数
     * @param numThreads 线程数（0表示自动检测）
     */
    void setMaxThreads(int numThreads);

    /**
     * @brief 获取最大线程数
     */
    int getMaxThreads() const { return maxThreads_; }

    /**
     * @brief 获取可用处理器核心数
     */
    static int getAvailableCores();

    /**
     * @brief 启用/禁用并行处理
     */
    void setEnabled(bool enabled) { enabled_ = enabled; }

    /**
     * @brief 检查并行处理是否启用
     */
    bool isEnabled() const { return enabled_; }

    // ========== 批量图像处理 ==========

    /**
     * @brief 图像处理函数类型
     */
    using ImageProcessor = std::function<void(ImageData::Ptr&)>;

    /**
     * @brief 并行处理多张图像
     * @param images 图像列表
     * @param processor 处理函数
     */
    void processBatch(std::vector<ImageData::Ptr>& images,
                     const ImageProcessor& processor);

    /**
     * @brief 并行处理多张图像（带索引）
     * @param images 图像列表
     * @param processor 处理函数（带索引）
     */
    void processBatchIndexed(std::vector<ImageData::Ptr>& images,
                            const std::function<void(ImageData::Ptr&, size_t)>& processor);

    // ========== 图像分块处理 ==========

    /**
     * @brief 分块处理函数类型
     * @param tile 分块Mat引用
     * @param tileRect 分块在原图中的位置
     */
    using TileProcessor = std::function<void(cv::Mat& tile, const cv::Rect& tileRect)>;

    /**
     * @brief 分块并行处理图像
     * @param image 输入图像
     * @param tileSize 分块大小
     * @param overlap 重叠像素
     * @param processor 处理函数
     */
    void processTiles(cv::Mat& image,
                     const cv::Size& tileSize,
                     int overlap,
                     const TileProcessor& processor);

    /**
     * @brief 分块并行处理图像（只读）
     * @param image 输入图像
     * @param tileSize 分块大小
     * @param overlap 重叠像素
     * @param processor 处理函数
     */
    void processTilesReadOnly(const cv::Mat& image,
                             const cv::Size& tileSize,
                             int overlap,
                             const std::function<void(const cv::Mat& tile, const cv::Rect& tileRect)>& processor);

    // ========== ROI并行处理 ==========

    /**
     * @brief ROI处理函数类型
     */
    using ROIProcessor = std::function<void(cv::Mat& roi, int roiIndex)>;

    /**
     * @brief 并行处理多个ROI
     * @param image 输入图像
     * @param rois ROI列表
     * @param processor 处理函数
     */
    void processROIs(cv::Mat& image,
                    const std::vector<cv::Rect>& rois,
                    const ROIProcessor& processor);

    // ========== 像素级并行处理 ==========

    /**
     * @brief 像素处理函数类型
     * @param pixel 像素值引用
     * @param x X坐标
     * @param y Y坐标
     */
    using PixelProcessor = std::function<void(uchar* pixel, int x, int y)>;

    /**
     * @brief 并行遍历所有像素
     * @param image 输入图像
     * @param processor 像素处理函数
     */
    void forEachPixel(cv::Mat& image, const PixelProcessor& processor);

    /**
     * @brief 并行行处理
     * @param image 输入图像
     * @param processor 行处理函数
     */
    void forEachRow(cv::Mat& image,
                   const std::function<void(uchar* rowPtr, int y, int cols, int channels)>& processor);

    // ========== 归约操作 ==========

    /**
     * @brief 并行求和（double）
     * @param data 数据数组
     * @param size 数组大小
     * @return 求和结果
     */
    static double parallelSum(const double* data, size_t size);

    /**
     * @brief 并行求最大值
     * @param data 数据数组
     * @param size 数组大小
     * @return 最大值
     */
    static double parallelMax(const double* data, size_t size);

    /**
     * @brief 并行求最小值
     * @param data 数据数组
     * @param size 数组大小
     * @return 最小值
     */
    static double parallelMin(const double* data, size_t size);

    // ========== 统计信息 ==========

    /**
     * @brief 统计信息
     */
    struct Statistics {
        size_t totalTasks;          ///< 总任务数
        size_t parallelTasks;       ///< 并行执行的任务数
        size_t serialTasks;         ///< 串行执行的任务数
        double avgSpeedup;          ///< 平均加速比
        int maxThreadsUsed;         ///< 使用的最大线程数
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
    ParallelProcessor();
    ~ParallelProcessor() = default;

    ParallelProcessor(const ParallelProcessor&) = delete;
    ParallelProcessor& operator=(const ParallelProcessor&) = delete;

    /**
     * @brief 计算分块数量
     */
    std::vector<cv::Rect> calculateTiles(const cv::Size& imageSize,
                                        const cv::Size& tileSize,
                                        int overlap) const;

private:
    int maxThreads_;
    bool enabled_;

    // 统计
    mutable std::atomic<size_t> totalTasks_{0};
    mutable std::atomic<size_t> parallelTasks_{0};
    mutable std::atomic<size_t> serialTasks_{0};
    mutable std::atomic<int> maxThreadsUsed_{0};
};

// ============================================================
// 便捷宏
// ============================================================

/**
 * @brief OpenMP并行for循环宏
 */
#ifdef _OPENMP
    #define VF_PARALLEL_FOR _Pragma("omp parallel for")
    #define VF_PARALLEL_FOR_COLLAPSE(n) _Pragma("omp parallel for collapse(" #n ")")
    #define VF_PARALLEL_REDUCE(op, var) _Pragma("omp parallel for reduction(" #op ":" #var ")")
#else
    #define VF_PARALLEL_FOR
    #define VF_PARALLEL_FOR_COLLAPSE(n)
    #define VF_PARALLEL_REDUCE(op, var)
#endif

} // namespace Base
} // namespace VisionForge
