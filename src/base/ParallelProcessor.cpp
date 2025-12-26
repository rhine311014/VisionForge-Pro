/**
 * @file ParallelProcessor.cpp
 * @brief 并行处理器实现
 */

#include "base/ParallelProcessor.h"
#include "base/Logger.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include <algorithm>
#include <limits>

namespace VisionForge {
namespace Base {

ParallelProcessor::ParallelProcessor()
    : maxThreads_(0)
    , enabled_(true)
{
    // 默认使用系统可用核心数
    maxThreads_ = getAvailableCores();

#ifdef _OPENMP
    omp_set_num_threads(maxThreads_);
    LOG_INFO(QString("并行处理器初始化: %1 线程 (OpenMP已启用)").arg(maxThreads_));
#else
    LOG_INFO("并行处理器初始化: OpenMP未启用，将使用串行处理");
#endif
}

ParallelProcessor& ParallelProcessor::instance()
{
    static ParallelProcessor instance;
    return instance;
}

void ParallelProcessor::setMaxThreads(int numThreads)
{
    if (numThreads <= 0) {
        maxThreads_ = getAvailableCores();
    } else {
        maxThreads_ = numThreads;
    }

#ifdef _OPENMP
    omp_set_num_threads(maxThreads_);
#endif

    LOG_DEBUG(QString("并行处理器线程数设置为: %1").arg(maxThreads_));
}

int ParallelProcessor::getAvailableCores()
{
#ifdef _OPENMP
    return omp_get_num_procs();
#else
    return 1;
#endif
}

// ========== 批量图像处理 ==========

void ParallelProcessor::processBatch(std::vector<ImageData::Ptr>& images,
                                    const ImageProcessor& processor)
{
    if (images.empty()) return;

    totalTasks_++;

    if (!enabled_ || images.size() < 2) {
        serialTasks_++;
        for (auto& image : images) {
            processor(image);
        }
        return;
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, static_cast<int>(images.size()));
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed)
    for (int i = 0; i < static_cast<int>(images.size()); ++i) {
        processor(images[i]);
    }
#else
    for (auto& image : images) {
        processor(image);
    }
#endif
}

void ParallelProcessor::processBatchIndexed(std::vector<ImageData::Ptr>& images,
                                           const std::function<void(ImageData::Ptr&, size_t)>& processor)
{
    if (images.empty()) return;

    totalTasks_++;

    if (!enabled_ || images.size() < 2) {
        serialTasks_++;
        for (size_t i = 0; i < images.size(); ++i) {
            processor(images[i], i);
        }
        return;
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, static_cast<int>(images.size()));
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed)
    for (int i = 0; i < static_cast<int>(images.size()); ++i) {
        processor(images[i], static_cast<size_t>(i));
    }
#else
    for (size_t i = 0; i < images.size(); ++i) {
        processor(images[i], i);
    }
#endif
}

size_t ParallelProcessor::processBatchFiles(
    const QStringList& imageFiles,
    const std::function<BatchProcessResult(const QString&, size_t)>& processor,
    const std::function<void(size_t current, size_t total)>& progressCallback)
{
    if (imageFiles.isEmpty()) return 0;

    totalTasks_++;

    size_t totalFiles = imageFiles.size();
    std::atomic<size_t> successCount{0};
    std::atomic<size_t> processedCount{0};

    // 进度回调节流：计算报告间隔（至少每1%或每10个文件报告一次）
    size_t reportInterval = std::max(totalFiles / 100, static_cast<size_t>(10));
    reportInterval = std::min(reportInterval, totalFiles);  // 确保不超过总数
    std::atomic<size_t> lastReportedProgress{0};

    if (!enabled_ || totalFiles < 2) {
        // 串行处理
        serialTasks_++;
        for (size_t i = 0; i < totalFiles; ++i) {
            auto result = processor(imageFiles[i], i);
            if (result.success) {
                successCount++;
            }
            processedCount++;

            // 节流进度回调
            if (progressCallback) {
                size_t current = processedCount.load();
                size_t lastReported = lastReportedProgress.load();
                if (current - lastReported >= reportInterval || current == totalFiles) {
                    lastReportedProgress.store(current);
                    progressCallback(current, totalFiles);
                }
            }
        }
        return successCount.load();
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, static_cast<int>(totalFiles));
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed) schedule(dynamic)
    for (int i = 0; i < static_cast<int>(totalFiles); ++i) {
        auto result = processor(imageFiles[i], static_cast<size_t>(i));
        if (result.success) {
            successCount++;
        }
        size_t current = processedCount.fetch_add(1, std::memory_order_relaxed) + 1;

        // 节流进度回调：只在达到报告间隔或处理完成时调用
        if (progressCallback) {
            size_t lastReported = lastReportedProgress.load(std::memory_order_relaxed);
            // 检查是否需要报告（使用CAS避免重复报告）
            if ((current - lastReported >= reportInterval || current == totalFiles)) {
                // 尝试更新lastReportedProgress，避免多线程重复调用
                if (lastReportedProgress.compare_exchange_weak(lastReported, current,
                        std::memory_order_relaxed, std::memory_order_relaxed) ||
                    current == totalFiles) {
                    #pragma omp critical(progress_callback)
                    {
                        progressCallback(current, totalFiles);
                    }
                }
            }
        }
    }
#else
    for (size_t i = 0; i < totalFiles; ++i) {
        auto result = processor(imageFiles[i], i);
        if (result.success) {
            successCount++;
        }
        processedCount++;

        // 节流进度回调
        if (progressCallback) {
            size_t current = processedCount.load();
            size_t lastReported = lastReportedProgress.load();
            if (current - lastReported >= reportInterval || current == totalFiles) {
                lastReportedProgress.store(current);
                progressCallback(current, totalFiles);
            }
        }
    }
#endif

    return successCount.load();
}

// ========== 图像分块处理 ==========

std::vector<cv::Rect> ParallelProcessor::calculateTiles(const cv::Size& imageSize,
                                                       const cv::Size& tileSize,
                                                       int overlap) const
{
    std::vector<cv::Rect> tiles;

    int stepX = tileSize.width - overlap;
    int stepY = tileSize.height - overlap;

    if (stepX <= 0) stepX = tileSize.width;
    if (stepY <= 0) stepY = tileSize.height;

    for (int y = 0; y < imageSize.height; y += stepY) {
        for (int x = 0; x < imageSize.width; x += stepX) {
            int w = std::min(tileSize.width, imageSize.width - x);
            int h = std::min(tileSize.height, imageSize.height - y);
            tiles.emplace_back(x, y, w, h);
        }
    }

    return tiles;
}

void ParallelProcessor::processTiles(cv::Mat& image,
                                    const cv::Size& tileSize,
                                    int overlap,
                                    const TileProcessor& processor)
{
    if (image.empty()) return;

    totalTasks_++;

    auto tiles = calculateTiles(image.size(), tileSize, overlap);

    if (!enabled_ || tiles.size() < 2) {
        serialTasks_++;
        for (const auto& rect : tiles) {
            cv::Mat tile = image(rect);
            processor(tile, rect);
        }
        return;
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, static_cast<int>(tiles.size()));
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed)
    for (int i = 0; i < static_cast<int>(tiles.size()); ++i) {
        cv::Mat tile = image(tiles[i]);
        processor(tile, tiles[i]);
    }
#else
    for (const auto& rect : tiles) {
        cv::Mat tile = image(rect);
        processor(tile, rect);
    }
#endif
}

void ParallelProcessor::processTilesReadOnly(const cv::Mat& image,
                                            const cv::Size& tileSize,
                                            int overlap,
                                            const std::function<void(const cv::Mat& tile, const cv::Rect& tileRect)>& processor)
{
    if (image.empty()) return;

    totalTasks_++;

    auto tiles = calculateTiles(image.size(), tileSize, overlap);

    if (!enabled_ || tiles.size() < 2) {
        serialTasks_++;
        for (const auto& rect : tiles) {
            const cv::Mat tile = image(rect);
            processor(tile, rect);
        }
        return;
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, static_cast<int>(tiles.size()));
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed)
    for (int i = 0; i < static_cast<int>(tiles.size()); ++i) {
        const cv::Mat tile = image(tiles[i]);
        processor(tile, tiles[i]);
    }
#else
    for (const auto& rect : tiles) {
        const cv::Mat tile = image(rect);
        processor(tile, rect);
    }
#endif
}

// ========== ROI并行处理 ==========

void ParallelProcessor::processROIs(cv::Mat& image,
                                   const std::vector<cv::Rect>& rois,
                                   const ROIProcessor& processor)
{
    if (image.empty() || rois.empty()) return;

    totalTasks_++;

    if (!enabled_ || rois.size() < 2) {
        serialTasks_++;
        for (size_t i = 0; i < rois.size(); ++i) {
            cv::Rect safeRect = rois[i] & cv::Rect(0, 0, image.cols, image.rows);
            if (safeRect.area() > 0) {
                cv::Mat roi = image(safeRect);
                processor(roi, static_cast<int>(i));
            }
        }
        return;
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, static_cast<int>(rois.size()));
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed)
    for (int i = 0; i < static_cast<int>(rois.size()); ++i) {
        cv::Rect safeRect = rois[i] & cv::Rect(0, 0, image.cols, image.rows);
        if (safeRect.area() > 0) {
            cv::Mat roi = image(safeRect);
            processor(roi, i);
        }
    }
#else
    for (size_t i = 0; i < rois.size(); ++i) {
        cv::Rect safeRect = rois[i] & cv::Rect(0, 0, image.cols, image.rows);
        if (safeRect.area() > 0) {
            cv::Mat roi = image(safeRect);
            processor(roi, static_cast<int>(i));
        }
    }
#endif
}

// ========== 像素级并行处理 ==========

void ParallelProcessor::forEachPixel(cv::Mat& image, const PixelProcessor& processor)
{
    if (image.empty()) return;

    totalTasks_++;

    int rows = image.rows;
    int cols = image.cols;
    int channels = image.channels();

    if (!enabled_) {
        serialTasks_++;
        for (int y = 0; y < rows; ++y) {
            uchar* rowPtr = image.ptr<uchar>(y);
            for (int x = 0; x < cols; ++x) {
                processor(rowPtr + x * channels, x, y);
            }
        }
        return;
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, rows);
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed)
    for (int y = 0; y < rows; ++y) {
        uchar* rowPtr = image.ptr<uchar>(y);
        for (int x = 0; x < cols; ++x) {
            processor(rowPtr + x * channels, x, y);
        }
    }
#else
    for (int y = 0; y < rows; ++y) {
        uchar* rowPtr = image.ptr<uchar>(y);
        for (int x = 0; x < cols; ++x) {
            processor(rowPtr + x * channels, x, y);
        }
    }
#endif
}

void ParallelProcessor::forEachRow(cv::Mat& image,
                                  const std::function<void(uchar* rowPtr, int y, int cols, int channels)>& processor)
{
    if (image.empty()) return;

    totalTasks_++;

    int rows = image.rows;
    int cols = image.cols;
    int channels = image.channels();

    if (!enabled_) {
        serialTasks_++;
        for (int y = 0; y < rows; ++y) {
            processor(image.ptr<uchar>(y), y, cols, channels);
        }
        return;
    }

    parallelTasks_++;

#ifdef _OPENMP
    int threadsUsed = std::min(maxThreads_, rows);
    if (threadsUsed > maxThreadsUsed_.load()) {
        maxThreadsUsed_.store(threadsUsed);
    }

    #pragma omp parallel for num_threads(threadsUsed)
    for (int y = 0; y < rows; ++y) {
        processor(image.ptr<uchar>(y), y, cols, channels);
    }
#else
    for (int y = 0; y < rows; ++y) {
        processor(image.ptr<uchar>(y), y, cols, channels);
    }
#endif
}

// ========== 归约操作 ==========

double ParallelProcessor::parallelSum(const double* data, size_t size)
{
    double sum = 0.0;

#ifdef _OPENMP
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < static_cast<int>(size); ++i) {
        sum += data[i];
    }
#else
    for (size_t i = 0; i < size; ++i) {
        sum += data[i];
    }
#endif

    return sum;
}

double ParallelProcessor::parallelMax(const double* data, size_t size)
{
    if (size == 0) return 0.0;

    double maxVal = data[0];

#ifdef _OPENMP
    // OpenMP 4.0+ 支持 max reduction，完全消除 critical section
    #if _OPENMP >= 201307  // OpenMP 4.0+
        #pragma omp parallel for reduction(max:maxVal)
        for (int i = 0; i < static_cast<int>(size); ++i) {
            if (data[i] > maxVal) {
                maxVal = data[i];
            }
        }
    #else
        // OpenMP 2.0 fallback: 使用线程局部变量 + 命名critical section优化
        #pragma omp parallel
        {
            double localMax = -std::numeric_limits<double>::infinity();

            #pragma omp for nowait
            for (int i = 0; i < static_cast<int>(size); ++i) {
                if (data[i] > localMax) {
                    localMax = data[i];
                }
            }

            // 命名critical section，避免与其他锁冲突
            #pragma omp critical(max_reduction)
            {
                if (localMax > maxVal) {
                    maxVal = localMax;
                }
            }
        }
    #endif
#else
    for (size_t i = 1; i < size; ++i) {
        if (data[i] > maxVal) {
            maxVal = data[i];
        }
    }
#endif

    return maxVal;
}

double ParallelProcessor::parallelMin(const double* data, size_t size)
{
    if (size == 0) return 0.0;

    double minVal = data[0];

#ifdef _OPENMP
    // OpenMP 4.0+ 支持 min reduction，完全消除 critical section
    #if _OPENMP >= 201307  // OpenMP 4.0+
        #pragma omp parallel for reduction(min:minVal)
        for (int i = 0; i < static_cast<int>(size); ++i) {
            if (data[i] < minVal) {
                minVal = data[i];
            }
        }
    #else
        // OpenMP 2.0 fallback: 使用线程局部变量 + 命名critical section优化
        #pragma omp parallel
        {
            double localMin = std::numeric_limits<double>::infinity();

            #pragma omp for nowait
            for (int i = 0; i < static_cast<int>(size); ++i) {
                if (data[i] < localMin) {
                    localMin = data[i];
                }
            }

            // 命名critical section，避免与其他锁冲突
            #pragma omp critical(min_reduction)
            {
                if (localMin < minVal) {
                    minVal = localMin;
                }
            }
        }
    #endif
#else
    for (size_t i = 1; i < size; ++i) {
        if (data[i] < minVal) {
            minVal = data[i];
        }
    }
#endif

    return minVal;
}

// ========== 高级并行算法实现 ==========

std::vector<std::vector<int>> ParallelProcessor::parallelHistogram(const cv::Mat& image, int bins)
{
    if (image.empty() || bins <= 0) {
        return {};
    }

    int channels = image.channels();
    std::vector<std::vector<int>> histograms(channels, std::vector<int>(bins, 0));

    // 计算bin大小
    double binSize = 256.0 / bins;

#ifdef _OPENMP
    // 为每个线程创建局部直方图
    int numThreads = omp_get_max_threads();
    std::vector<std::vector<std::vector<int>>> localHistograms(
        numThreads, std::vector<std::vector<int>>(channels, std::vector<int>(bins, 0)));

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        auto& localHist = localHistograms[tid];

        #pragma omp for
        for (int y = 0; y < image.rows; ++y) {
            const uchar* rowPtr = image.ptr<uchar>(y);
            for (int x = 0; x < image.cols; ++x) {
                for (int c = 0; c < channels; ++c) {
                    int value = rowPtr[x * channels + c];
                    int binIdx = static_cast<int>(value / binSize);
                    binIdx = std::min(binIdx, bins - 1);
                    localHist[c][binIdx]++;
                }
            }
        }
    }

    // 合并所有局部直方图
    for (const auto& localHist : localHistograms) {
        for (int c = 0; c < channels; ++c) {
            for (int b = 0; b < bins; ++b) {
                histograms[c][b] += localHist[c][b];
            }
        }
    }
#else
    // 串行版本
    for (int y = 0; y < image.rows; ++y) {
        const uchar* rowPtr = image.ptr<uchar>(y);
        for (int x = 0; x < image.cols; ++x) {
            for (int c = 0; c < channels; ++c) {
                int value = rowPtr[x * channels + c];
                int binIdx = static_cast<int>(value / binSize);
                binIdx = std::min(binIdx, bins - 1);
                histograms[c][binIdx]++;
            }
        }
    }
#endif

    return histograms;
}

std::pair<cv::Scalar, cv::Scalar> ParallelProcessor::parallelMeanStdDev(const cv::Mat& image)
{
    if (image.empty()) {
        return {cv::Scalar(), cv::Scalar()};
    }

    int channels = image.channels();
    cv::Scalar mean, stddev;

    // 计算每个通道的均值和标准差
    for (int c = 0; c < channels; ++c) {
        double sum = 0.0;
        double sumSq = 0.0;
        int totalPixels = image.rows * image.cols;

#ifdef _OPENMP
        #pragma omp parallel for reduction(+:sum,sumSq)
        for (int y = 0; y < image.rows; ++y) {
            const uchar* rowPtr = image.ptr<uchar>(y);
            for (int x = 0; x < image.cols; ++x) {
                double value = rowPtr[x * channels + c];
                sum += value;
                sumSq += value * value;
            }
        }
#else
        for (int y = 0; y < image.rows; ++y) {
            const uchar* rowPtr = image.ptr<uchar>(y);
            for (int x = 0; x < image.cols; ++x) {
                double value = rowPtr[x * channels + c];
                sum += value;
                sumSq += value * value;
            }
        }
#endif

        mean[c] = sum / totalPixels;
        double variance = (sumSq / totalPixels) - (mean[c] * mean[c]);
        stddev[c] = std::sqrt(std::max(0.0, variance));
    }

    return {mean, stddev};
}

void ParallelProcessor::parallelConvolution(const cv::Mat& input, cv::Mat& output, const cv::Mat& kernel)
{
    if (input.empty() || kernel.empty()) {
        return;
    }

    // 确保输出图像大小正确
    output.create(input.size(), input.type());

    int kRows = kernel.rows;
    int kCols = kernel.cols;
    int kCenterX = kCols / 2;
    int kCenterY = kRows / 2;

    // 转换kernel为double类型以提高精度
    cv::Mat kernelDouble;
    kernel.convertTo(kernelDouble, CV_64F);

#ifdef _OPENMP
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < input.rows; ++y) {
        for (int x = 0; x < input.cols; ++x) {
            double sum = 0.0;

            // 卷积操作
            for (int ky = 0; ky < kRows; ++ky) {
                for (int kx = 0; kx < kCols; ++kx) {
                    int iy = y + ky - kCenterY;
                    int ix = x + kx - kCenterX;

                    // 边界处理（镜像）
                    iy = std::abs(iy);
                    ix = std::abs(ix);
                    if (iy >= input.rows) iy = 2 * input.rows - iy - 2;
                    if (ix >= input.cols) ix = 2 * input.cols - ix - 2;

                    double pixelValue = input.at<uchar>(iy, ix);
                    double kernelValue = kernelDouble.at<double>(ky, kx);
                    sum += pixelValue * kernelValue;
                }
            }

            output.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
        }
    }
#else
    // 串行版本
    for (int y = 0; y < input.rows; ++y) {
        for (int x = 0; x < input.cols; ++x) {
            double sum = 0.0;

            for (int ky = 0; ky < kRows; ++ky) {
                for (int kx = 0; kx < kCols; ++kx) {
                    int iy = y + ky - kCenterY;
                    int ix = x + kx - kCenterX;

                    iy = std::abs(iy);
                    ix = std::abs(ix);
                    if (iy >= input.rows) iy = 2 * input.rows - iy - 2;
                    if (ix >= input.cols) ix = 2 * input.cols - ix - 2;

                    double pixelValue = input.at<uchar>(iy, ix);
                    double kernelValue = kernelDouble.at<double>(ky, kx);
                    sum += pixelValue * kernelValue;
                }
            }

            output.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
        }
    }
#endif
}

// ========== 统计信息 ==========

ParallelProcessor::Statistics ParallelProcessor::getStatistics() const
{
    Statistics stats;
    stats.totalTasks = totalTasks_.load();
    stats.parallelTasks = parallelTasks_.load();
    stats.serialTasks = serialTasks_.load();
    stats.maxThreadsUsed = maxThreadsUsed_.load();

    if (stats.serialTasks > 0 && stats.parallelTasks > 0) {
        // 估算加速比（假设并行任务使用了最大线程数）
        stats.avgSpeedup = static_cast<double>(stats.maxThreadsUsed) * 0.7; // 保守估计70%效率
    } else {
        stats.avgSpeedup = 1.0;
    }

    return stats;
}

void ParallelProcessor::resetStatistics()
{
    totalTasks_.store(0);
    parallelTasks_.store(0);
    serialTasks_.store(0);
    maxThreadsUsed_.store(0);
}

} // namespace Base
} // namespace VisionForge
