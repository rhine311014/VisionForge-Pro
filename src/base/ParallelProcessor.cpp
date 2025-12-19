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
    // 使用临界区代替reduction(max:)，兼容MSVC OpenMP 2.0
    #pragma omp parallel
    {
        double localMax = data[0];
        #pragma omp for nowait
        for (int i = 1; i < static_cast<int>(size); ++i) {
            if (data[i] > localMax) {
                localMax = data[i];
            }
        }
        #pragma omp critical
        {
            if (localMax > maxVal) {
                maxVal = localMax;
            }
        }
    }
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
    // 使用临界区代替reduction(min:)，兼容MSVC OpenMP 2.0
    #pragma omp parallel
    {
        double localMin = data[0];
        #pragma omp for nowait
        for (int i = 1; i < static_cast<int>(size); ++i) {
            if (data[i] < localMin) {
                localMin = data[i];
            }
        }
        #pragma omp critical
        {
            if (localMin < minVal) {
                minVal = localMin;
            }
        }
    }
#else
    for (size_t i = 1; i < size; ++i) {
        if (data[i] < minVal) {
            minVal = data[i];
        }
    }
#endif

    return minVal;
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
