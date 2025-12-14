/**
 * @file ImageData.cpp
 * @brief 图像数据类实现
 */

#include "base/ImageData.h"
#include <opencv2/imgproc.hpp>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <malloc.h>
#else
#include <cstdlib>
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

    QImage::Format format;
    int type = mat_.type();

    // 确定QImage格式
    if (type == CV_8UC1) {
        format = QImage::Format_Grayscale8;
    } else if (type == CV_8UC3) {
        format = QImage::Format_RGB888;
    } else if (type == CV_8UC4) {
        format = QImage::Format_RGBA8888;
    } else {
        // 不支持的格式，需要转换
        cv::Mat converted;
        if (mat_.channels() == 1) {
            converted = mat_;
            format = QImage::Format_Grayscale8;
        } else {
            cv::cvtColor(mat_, converted, cv::COLOR_BGR2RGB);
            format = QImage::Format_RGB888;
        }
        return QImage(converted.data, converted.cols, converted.rows,
                     static_cast<int>(converted.step), format).copy();
    }

    // 零拷贝创建QImage
    // 注意：OpenCV使用BGR，Qt使用RGB，需要转换
    if (type == CV_8UC3) {
        // 创建临时RGB图像
        cv::Mat rgb;
        cv::cvtColor(mat_, rgb, cv::COLOR_BGR2RGB);

        // 拷贝到新的QImage（这里无法完全零拷贝，因为需要颜色空间转换）
        return QImage(rgb.data, rgb.cols, rgb.rows,
                     static_cast<int>(rgb.step), format).copy();
    } else {
        // 灰度或RGBA可以零拷贝
        // 使用const_cast是安全的，因为QImage不会修改数据
        return QImage(const_cast<uchar*>(mat_.data), mat_.cols, mat_.rows,
                     static_cast<int>(mat_.step), format,
                     nullptr, nullptr);
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
// GPU内存管理（占位实现）
// ============================================================

void ImageData::allocateGPU()
{
    // TODO: 实现CUDA内存分配
    // cudaMalloc(&gpuPtr_, totalBytes());
}

void ImageData::uploadToGPU()
{
    // TODO: 实现CUDA内存上传
    // if (gpuPtr_) {
    //     cudaMemcpy(gpuPtr_, mat_.data, totalBytes(), cudaMemcpyHostToDevice);
    // }
}

void ImageData::downloadFromGPU()
{
    // TODO: 实现CUDA内存下载
    // if (gpuPtr_) {
    //     cudaMemcpy(mat_.data, gpuPtr_, totalBytes(), cudaMemcpyDeviceToHost);
    // }
}

void ImageData::releaseGPU()
{
    // TODO: 实现CUDA内存释放
    // if (gpuPtr_) {
    //     cudaFree(gpuPtr_);
    //     gpuPtr_ = nullptr;
    // }
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
