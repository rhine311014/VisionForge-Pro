/**
 * @file ParallelProcessor.h
 * @brief 并行处理器 - OpenMP多线程优化
 * @author VisionForge Team
 * @date 2025-12-19
 *
 * @details
 * 本文件实现了VisionForge项目的CPU并行处理功能，基于OpenMP多线程框架。
 *
 * ## 设计模式
 * - **单例模式 (Singleton)**：全局唯一的并行处理器实例
 * - **策略模式 (Strategy)**：根据任务规模自动选择串行或并行
 * - **模板方法模式**：parallelTransform、parallelReduce等通用算法
 *
 * ## 核心功能
 *
 * ### 批量处理
 * - processBatch()：并行处理多张图像
 * - processBatchIndexed()：带索引的批量处理
 * - processBatchFiles()：并行处理文件列表
 *
 * ### 分块处理
 * - processTiles()：将大图分块并行处理
 * - processTilesReadOnly()：只读分块处理
 * - 支持设置重叠区域
 *
 * ### ROI并行
 * - processROIs()：并行处理多个ROI区域
 *
 * ### 像素级并行
 * - forEachPixel()：并行遍历所有像素
 * - forEachRow()：并行处理每一行
 *
 * ### 归约操作
 * - parallelSum()：并行求和
 * - parallelMax/Min()：并行求最值
 * - parallelReduce()：通用归约
 *
 * ### 高级算法
 * - parallelTransform()：并行变换
 * - parallelFilter()：并行过滤
 * - parallelHistogram()：并行直方图
 * - parallelConvolution()：并行卷积
 *
 * ## 条件编译
 * 使用_OPENMP宏控制OpenMP相关代码：
 * - 定义_OPENMP时：使用OpenMP并行
 * - 未定义时：自动回退到串行执行
 *
 * ## 线程安全
 * - 处理函数内部应避免共享状态
 * - 使用reduction子句处理归约操作
 * - 进度回调使用critical section保护
 *
 * ## 性能优化
 *
 * ### 自适应并行
 * - 任务数少于2时自动串行执行
 * - 根据任务数调整实际线程数
 *
 * ### 进度节流
 * processBatchFiles()使用节流机制：
 * - 计算报告间隔（至少1%或10个文件）
 * - 使用atomic+CAS避免重复报告
 *
 * ### 调度策略
 * - 默认使用static调度
 * - 文件处理使用dynamic调度（负载均衡）
 *
 * ## 内存管理
 * - 无动态内存分配（使用栈上对象）
 * - 统计计数器使用std::atomic
 *
 * ## 使用示例
 * @code
 * // 并行处理图像批量
 * std::vector<ImageData::Ptr> images = ...;
 * ParallelProcessor::instance().processBatch(images, [](ImageData::Ptr& img) {
 *     // 处理每张图像
 *     cv::GaussianBlur(img->mat(), img->mat(), cv::Size(5,5), 1.0);
 * });
 *
 * // 分块处理大图
 * ParallelProcessor::instance().processTiles(largeImage, cv::Size(256,256), 10,
 *     [](cv::Mat& tile, const cv::Rect& rect) {
 *         // 处理每个分块
 *     });
 *
 * // 使用便捷宏
 * VF_PARALLEL_FOR
 * for (int i = 0; i < n; ++i) {
 *     // 并行循环体
 * }
 * @endcode
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

    /**
     * @brief 批量图片并行处理（带结果返回）
     * @param imageFiles 图片文件路径列表
     * @param processor 处理函数，返回 {success, errorMsg, elapsedMs}
     * @param progressCallback 进度回调函数
     * @return 成功处理的图片数量
     */
    struct BatchProcessResult {
        bool success;
        QString errorMessage;
        double elapsedMs;
    };

    size_t processBatchFiles(
        const QStringList& imageFiles,
        const std::function<BatchProcessResult(const QString&, size_t)>& processor,
        const std::function<void(size_t current, size_t total)>& progressCallback = nullptr);

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

    // ========== 高级并行算法 ==========

    /**
     * @brief 并行变换（类似std::transform）
     * @param input 输入数组
     * @param output 输出数组
     * @param size 数组大小
     * @param op 变换函数
     */
    template<typename T, typename Op>
    static void parallelTransform(const T* input, T* output, size_t size, Op op);

    /**
     * @brief 并行归约（通用版本）
     * @param data 数据数组
     * @param size 数组大小
     * @param init 初始值
     * @param op 归约操作（如相加、相乘）
     * @return 归约结果
     */
    template<typename T, typename Op>
    static T parallelReduce(const T* data, size_t size, T init, Op op);

    /**
     * @brief 并行过滤（条件筛选）
     * @param input 输入数组
     * @param size 输入大小
     * @param predicate 判断函数
     * @return 符合条件的元素向量
     */
    template<typename T, typename Pred>
    static std::vector<T> parallelFilter(const T* input, size_t size, Pred predicate);

    /**
     * @brief 并行计算直方图
     * @param image 输入图像
     * @param bins 直方图bin数量（默认256）
     * @return 每个通道的直方图
     */
    static std::vector<std::vector<int>> parallelHistogram(
        const cv::Mat& image, int bins = 256);

    /**
     * @brief 并行计算均值和标准差
     * @param image 输入图像
     * @return {mean, stddev}
     */
    static std::pair<cv::Scalar, cv::Scalar> parallelMeanStdDev(const cv::Mat& image);

    /**
     * @brief 并行图像卷积
     * @param input 输入图像
     * @param output 输出图像
     * @param kernel 卷积核
     */
    static void parallelConvolution(const cv::Mat& input, cv::Mat& output, const cv::Mat& kernel);

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

// ============================================================
// 模板函数实现
// ============================================================

template<typename T, typename Op>
void ParallelProcessor::parallelTransform(const T* input, T* output, size_t size, Op op)
{
#ifdef _OPENMP
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < static_cast<int>(size); ++i) {
        output[i] = op(input[i]);
    }
#else
    for (size_t i = 0; i < size; ++i) {
        output[i] = op(input[i]);
    }
#endif
}

template<typename T, typename Op>
T ParallelProcessor::parallelReduce(const T* data, size_t size, T init, Op op)
{
    if (size == 0) return init;

    T result = init;

#ifdef _OPENMP
    #pragma omp parallel
    {
        T localResult = init;

        #pragma omp for nowait
        for (int i = 0; i < static_cast<int>(size); ++i) {
            localResult = op(localResult, data[i]);
        }

        #pragma omp critical(reduce_operation)
        {
            result = op(result, localResult);
        }
    }
#else
    for (size_t i = 0; i < size; ++i) {
        result = op(result, data[i]);
    }
#endif

    return result;
}

template<typename T, typename Pred>
std::vector<T> ParallelProcessor::parallelFilter(const T* input, size_t size, Pred predicate)
{
    std::vector<T> result;

    if (size == 0) return result;

#ifdef _OPENMP
    // 每个线程维护自己的局部结果
    std::vector<std::vector<T>> localResults;

    #pragma omp parallel
    {
        std::vector<T> local;

        #pragma omp for nowait schedule(static)
        for (int i = 0; i < static_cast<int>(size); ++i) {
            if (predicate(input[i])) {
                local.push_back(input[i]);
            }
        }

        #pragma omp critical(filter_merge)
        {
            localResults.push_back(std::move(local));
        }
    }

    // 合并所有局部结果
    size_t totalSize = 0;
    for (const auto& local : localResults) {
        totalSize += local.size();
    }
    result.reserve(totalSize);

    for (auto& local : localResults) {
        result.insert(result.end(),
                     std::make_move_iterator(local.begin()),
                     std::make_move_iterator(local.end()));
    }
#else
    for (size_t i = 0; i < size; ++i) {
        if (predicate(input[i])) {
            result.push_back(input[i]);
        }
    }
#endif

    return result;
}

} // namespace Base
} // namespace VisionForge
