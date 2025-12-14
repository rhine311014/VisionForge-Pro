/**
 * @file ImageData.h
 * @brief 图像数据类 - 支持零拷贝、内存对齐、GPU加速
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include <opencv2/core.hpp>
#include <QImage>
#include <memory>
#include <functional>
#include <cstddef>

namespace VisionForge {
namespace Base {

/**
 * @brief 图像数据类
 *
 * 核心特性：
 * - 零拷贝：使用shared_ptr管理内存，避免不必要的数据复制
 * - 内存对齐：默认32字节对齐，支持AVX2 SIMD指令集
 * - GPU支持：可选的GPU内存管理
 * - Qt集成：零拷贝转换到QImage
 */
class ImageData {
public:
    using Ptr = std::shared_ptr<ImageData>;

    /**
     * @brief 默认构造函数
     */
    ImageData();

    /**
     * @brief 构造函数（分配对齐内存）
     * @param width 图像宽度
     * @param height 图像高度
     * @param type OpenCV图像类型（如CV_8UC3）
     */
    ImageData(int width, int height, int type);

    /**
     * @brief 从cv::Mat构造（深拷贝）
     * @param mat OpenCV矩阵
     */
    explicit ImageData(const cv::Mat& mat);

    /**
     * @brief 析构函数
     */
    ~ImageData();

    // ========== OpenCV接口 ==========

    /**
     * @brief 获取OpenCV矩阵（零拷贝访问）
     */
    cv::Mat& mat() { return mat_; }
    const cv::Mat& mat() const { return mat_; }

    // ========== Qt接口 ==========

    /**
     * @brief 转换为QImage（零拷贝）
     * @return QImage对象，与ImageData共享内存
     */
    QImage toQImage() const;

    // ========== 静态工厂方法 ==========

    /**
     * @brief 从外部对齐内存构造（零拷贝）
     * @param data 对齐的内存指针
     * @param width 图像宽度
     * @param height 图像高度
     * @param type OpenCV图像类型
     * @param deleter 自定义析构函数
     * @return ImageData智能指针
     */
    static Ptr fromAlignedMemory(void* data, int width, int height, int type,
                                 std::function<void(void*)> deleter = nullptr);

    // ========== GPU内存管理 ==========

    /**
     * @brief 获取GPU内存指针
     */
    void* gpuPtr() const { return gpuPtr_; }

    /**
     * @brief 是否有GPU内存
     */
    bool hasGPUMemory() const { return gpuPtr_ != nullptr; }

    /**
     * @brief 分配GPU内存
     */
    void allocateGPU();

    /**
     * @brief 上传到GPU
     */
    void uploadToGPU();

    /**
     * @brief 从GPU下载
     */
    void downloadFromGPU();

    /**
     * @brief 释放GPU内存
     */
    void releaseGPU();

    // ========== 内存对齐 ==========

    /**
     * @brief 检查内存是否对齐
     */
    bool isMemoryAligned() const { return isAligned_; }

    /**
     * @brief 获取对齐字节数
     */
    size_t getAlignment() const { return alignment_; }

    // ========== 元数据 ==========

    /**
     * @brief 获取时间戳
     */
    size_t timestamp() const { return timestamp_; }

    /**
     * @brief 设置时间戳
     */
    void setTimestamp(size_t ts) { timestamp_ = ts; }

    /**
     * @brief 获取图像宽度
     */
    int width() const { return mat_.cols; }

    /**
     * @brief 获取图像高度
     */
    int height() const { return mat_.rows; }

    /**
     * @brief 获取通道数
     */
    int channels() const { return mat_.channels(); }

    /**
     * @brief 获取总字节数
     */
    size_t totalBytes() const { return mat_.total() * mat_.elemSize(); }

    /**
     * @brief 检查图像是否为空
     */
    bool isEmpty() const { return mat_.empty(); }

    // ========== 拷贝操作 ==========

    /**
     * @brief 浅拷贝（共享数据）
     * @return 新的ImageData智能指针，共享相同的数据
     */
    Ptr clone() const;

    /**
     * @brief 深拷贝（复制数据）
     * @return 新的ImageData智能指针，包含数据副本
     */
    Ptr deepCopy() const;

    // 禁止拷贝构造和赋值
    ImageData(const ImageData&) = delete;
    ImageData& operator=(const ImageData&) = delete;

private:
    /**
     * @brief 清理对齐内存的回调函数
     */
    static void cleanupAlignedMemory(void* ptr);

    /**
     * @brief QImage清理回调函数
     */
    static void qImageCleanup(void* info);

private:
    cv::Mat mat_;                              // OpenCV矩阵
    void* alignedData_;                        // 对齐的内存指针
    void* gpuPtr_;                             // GPU内存指针
    size_t timestamp_;                         // 时间戳
    bool isAligned_;                           // 是否内存对齐
    size_t alignment_;                         // 对齐字节数（默认32）
    std::function<void(void*)> customDeleter_; // 自定义析构函数
    bool ownsMemory_;                          // 是否拥有内存所有权
};

} // namespace Base
} // namespace VisionForge
