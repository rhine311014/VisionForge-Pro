/**
 * @file ImageData.cpp
 * @brief 图像数据类实现
 */

#include "base/ImageData.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <malloc.h>
#else
#include <cstdlib>
#endif

#ifdef USE_CUDA
#include <cuda_runtime.h>

// CUDA错误检查宏
#define CUDA_CHECK(call) \
    do { \
        cudaError_t error = call; \
        if (error != cudaSuccess) { \
            LOG_ERROR(QString("CUDA错误: %1 (%2:%3)") \
                .arg(cudaGetErrorString(error)) \
                .arg(__FILE__) \
                .arg(__LINE__)); \
            return; \
        } \
    } while(0)

#define CUDA_CHECK_RETURN(call, ret) \
    do { \
        cudaError_t error = call; \
        if (error != cudaSuccess) { \
            LOG_ERROR(QString("CUDA错误: %1 (%2:%3)") \
                .arg(cudaGetErrorString(error)) \
                .arg(__FILE__) \
                .arg(__LINE__)); \
            return ret; \
        } \
    } while(0)
#endif

namespace VisionForge {
namespace Base {

// 默认对齐字节数（32字节，支持AVX2）
static constexpr size_t DEFAULT_ALIGNMENT = 32;

// ============================================================
// 构造函数
// ============================================================

ImageData::ImageData()
    : alignedData_(nullptr)
    , gpuPtr_(nullptr)
    , timestamp_(0)
    , isAligned_(false)
    , alignment_(DEFAULT_ALIGNMENT)
    , customDeleter_(nullptr)
    , ownsMemory_(false)
{
}

ImageData::ImageData(int width, int height, int type)
    : alignedData_(nullptr)
    , gpuPtr_(nullptr)
    , timestamp_(0)
    , isAligned_(true)
    , alignment_(DEFAULT_ALIGNMENT)
    , customDeleter_(nullptr)
    , ownsMemory_(true)
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("ImageData: 图像尺寸必须为正数");
    }

    // 计算所需内存大小
    size_t size = static_cast<size_t>(width) * height * CV_ELEM_SIZE(type);

    // 分配对齐内存
#ifdef _WIN32
    alignedData_ = _aligned_malloc(size, alignment_);
#else
    if (posix_memalign(&alignedData_, alignment_, size) != 0) {
        alignedData_ = nullptr;
    }
#endif

    if (!alignedData_) {
        throw std::bad_alloc();
    }

    // 使用外部内存创建cv::Mat
    mat_ = cv::Mat(height, width, type, alignedData_);
}

ImageData::ImageData(const cv::Mat& mat)
    : alignedData_(nullptr)
    , gpuPtr_(nullptr)
    , timestamp_(0)
    , isAligned_(false)
    , alignment_(DEFAULT_ALIGNMENT)
    , customDeleter_(nullptr)
    , ownsMemory_(false)
{
    // 深拷贝
    mat_ = mat.clone();
}

ImageData::~ImageData()
{
    // 释放GPU内存
    releaseGPU();

    // 释放对齐内存
    if (ownsMemory_ && alignedData_) {
#ifdef _WIN32
        _aligned_free(alignedData_);
#else
        free(alignedData_);
#endif
        alignedData_ = nullptr;
    } else if (customDeleter_ && alignedData_) {
        customDeleter_(alignedData_);
        alignedData_ = nullptr;
    }
}

// ============================================================
// Qt接口
// ============================================================

QImage ImageData::toQImage() const
{
    if (mat_.empty()) {
        return QImage();
    }

    int type = mat_.type();

    // CV_8UC1: 灰度图 - 零拷贝
    if (type == CV_8UC1) {
        // 灰度图可以直接零拷贝，QImage不会修改数据
        return QImage(const_cast<uchar*>(mat_.data), mat_.cols, mat_.rows,
                     static_cast<int>(mat_.step), QImage::Format_Grayscale8,
                     nullptr, nullptr);
    }

    // CV_8UC4: RGBA图 - 零拷贝
    if (type == CV_8UC4) {
        return QImage(const_cast<uchar*>(mat_.data), mat_.cols, mat_.rows,
                     static_cast<int>(mat_.step), QImage::Format_RGBA8888,
                     nullptr, nullptr);
    }

    // CV_8UC3: BGR转RGB - 需要一次拷贝（颜色空间转换）
    if (type == CV_8UC3) {
        // 直接创建QImage并在其内存上进行颜色转换，避免临时Mat
        QImage result(mat_.cols, mat_.rows, QImage::Format_RGB888);
        cv::Mat rgbWrapper(mat_.rows, mat_.cols, CV_8UC3, result.bits(),
                          static_cast<size_t>(result.bytesPerLine()));
        cv::cvtColor(mat_, rgbWrapper, cv::COLOR_BGR2RGB);
        return result;
    }

    // 其他格式：尝试转换
    if (mat_.channels() == 1) {
        // 非8位灰度图，转换为8位
        cv::Mat converted;
        mat_.convertTo(converted, CV_8U);
        return QImage(converted.data, converted.cols, converted.rows,
                     static_cast<int>(converted.step), QImage::Format_Grayscale8).copy();
    } else {
        // 其他多通道格式，转换为RGB
        cv::Mat converted;
        mat_.convertTo(converted, CV_8UC3);
        cv::cvtColor(converted, converted, cv::COLOR_BGR2RGB);
        return QImage(converted.data, converted.cols, converted.rows,
                     static_cast<int>(converted.step), QImage::Format_RGB888).copy();
    }
}

// ============================================================
// 静态工厂方法
// ============================================================

ImageData::Ptr ImageData::fromAlignedMemory(void* data, int width, int height, int type,
                                            std::function<void(void*)> deleter)
{
    if (!data) {
        throw std::invalid_argument("ImageData::fromAlignedMemory: 数据指针为空");
    }

    auto image = std::make_shared<ImageData>();
    image->alignedData_ = data;
    image->mat_ = cv::Mat(height, width, type, data);
    image->isAligned_ = true;
    image->customDeleter_ = deleter;
    image->ownsMemory_ = false;

    return image;
}

// ============================================================
// GPU内存管理
// ============================================================

void ImageData::allocateGPU()
{
#ifdef USE_CUDA
    if (mat_.empty()) {
        LOG_WARNING("ImageData::allocateGPU: 图像为空，无法分配GPU内存");
        return;
    }

    // 如果已有GPU内存，先释放
    if (gpuPtr_) {
        releaseGPU();
    }

    size_t bytes = totalBytes();
    if (bytes == 0) {
        LOG_WARNING("ImageData::allocateGPU: 图像大小为0");
        return;
    }

    // 分配GPU内存
    CUDA_CHECK(cudaMalloc(&gpuPtr_, bytes));

    LOG_DEBUG(QString("GPU内存已分配: %1 字节").arg(bytes));
#else
    LOG_WARNING("ImageData::allocateGPU: CUDA未启用");
#endif
}

void ImageData::uploadToGPU()
{
#ifdef USE_CUDA
    if (mat_.empty()) {
        LOG_WARNING("ImageData::uploadToGPU: 图像为空");
        return;
    }

    // 如果没有GPU内存，先分配
    if (!gpuPtr_) {
        allocateGPU();
    }

    if (!gpuPtr_) {
        LOG_ERROR("ImageData::uploadToGPU: GPU内存分配失败");
        return;
    }

    size_t bytes = totalBytes();

    // 确保Mat数据是连续的
    cv::Mat continuous = mat_.isContinuous() ? mat_ : mat_.clone();

    // 上传数据到GPU
    CUDA_CHECK(cudaMemcpy(gpuPtr_, continuous.data, bytes, cudaMemcpyHostToDevice));

    LOG_DEBUG(QString("数据已上传到GPU: %1 字节").arg(bytes));
#else
    LOG_WARNING("ImageData::uploadToGPU: CUDA未启用");
#endif
}

void ImageData::downloadFromGPU()
{
#ifdef USE_CUDA
    if (!gpuPtr_) {
        LOG_WARNING("ImageData::downloadFromGPU: 无GPU内存");
        return;
    }

    if (mat_.empty()) {
        LOG_ERROR("ImageData::downloadFromGPU: CPU端图像为空");
        return;
    }

    size_t bytes = totalBytes();

    // 确保Mat数据是连续的
    if (!mat_.isContinuous()) {
        mat_ = mat_.clone();
    }

    // 从GPU下载数据
    CUDA_CHECK(cudaMemcpy(mat_.data, gpuPtr_, bytes, cudaMemcpyDeviceToHost));

    LOG_DEBUG(QString("数据已从GPU下载: %1 字节").arg(bytes));
#else
    LOG_WARNING("ImageData::downloadFromGPU: CUDA未启用");
#endif
}

void ImageData::releaseGPU()
{
#ifdef USE_CUDA
    if (gpuPtr_) {
        cudaError_t error = cudaFree(gpuPtr_);
        if (error != cudaSuccess) {
            LOG_ERROR(QString("释放GPU内存失败: %1").arg(cudaGetErrorString(error)));
        } else {
            LOG_DEBUG("GPU内存已释放");
        }
        gpuPtr_ = nullptr;
    }
#endif
}

// ============================================================
// 拷贝操作
// ============================================================

ImageData::Ptr ImageData::clone() const
{
    // 浅拷贝：共享数据
    auto image = std::make_shared<ImageData>();
    image->mat_ = mat_;  // cv::Mat的赋值是浅拷贝
    image->timestamp_ = timestamp_;
    image->isAligned_ = isAligned_;
    image->alignment_ = alignment_;
    image->ownsMemory_ = false;  // 不拥有内存

    return image;
}

ImageData::Ptr ImageData::deepCopy() const
{
    // 深拷贝：复制数据
    auto image = std::make_shared<ImageData>(width(), height(), mat_.type());
    mat_.copyTo(image->mat_);
    image->timestamp_ = timestamp_;

    return image;
}

// ============================================================
// 静态辅助函数
// ============================================================

void ImageData::cleanupAlignedMemory(void* ptr)
{
    if (ptr) {
#ifdef _WIN32
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }
}

void ImageData::qImageCleanup(void* info)
{
    // QImage清理回调（可用于引用计数管理）
    // 当前实现为空
}

} // namespace Base
} // namespace VisionForge
