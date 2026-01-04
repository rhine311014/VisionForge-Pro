/**
 * @file ImageData.h
 * @brief 图像数据类 - 支持零拷贝、内存对齐、GPU加速
 * @author VisionForge Team
 * @date 2025-12-14
 *
 * @details
 * 本文件定义了VisionForge项目的核心图像数据结构，封装了OpenCV的cv::Mat，
 * 并提供了内存对齐、GPU加速和Qt集成等高级功能。
 *
 * ## 设计模式
 * - **工厂模式 (Factory)**：通过静态方法fromAlignedMemory创建对象
 * - **RAII模式**：自动管理内存生命周期
 * - **享元模式 (Flyweight)**：配合ImageMemoryPool实现对象复用
 *
 * ## 核心特性
 *
 * ### 零拷贝设计
 * - 使用shared_ptr进行引用计数，避免不必要的数据复制
 * - toQImage()在灰度图和RGBA图场景下实现零拷贝
 * - clone()方法返回共享数据的浅拷贝
 * - deepCopy()方法在需要时创建完整副本
 *
 * ### 内存对齐
 * - 默认32字节对齐，支持AVX2 SIMD指令集
 * - 使用平台特定API：Windows使用_aligned_malloc，POSIX使用posix_memalign
 * - 对齐内存可显著提升SIMD运算性能（约20-30%）
 *
 * ### GPU内存管理
 * - 支持CUDA GPU内存分配和传输
 * - allocateGPU()/releaseGPU()：显式管理GPU内存
 * - uploadToGPU()/downloadFromGPU()：CPU与GPU间数据传输
 * - 使用USE_CUDA宏条件编译
 *
 * ## 线程安全
 * - ImageData对象本身不是线程安全的
 * - 多线程访问同一ImageData需要外部同步
 * - 建议每个线程使用独立的ImageData实例
 *
 * ## 内存管理策略
 * - ownsMemory_标志：区分是否拥有内存所有权
 * - customDeleter_：支持自定义析构函数
 * - 禁止拷贝构造和赋值，防止意外的浅拷贝
 *
 * ## 使用示例
 * @code
 * // 创建图像
 * auto image = std::make_shared<ImageData>(1920, 1080, CV_8UC3);
 *
 * // 访问OpenCV Mat
 * cv::Mat& mat = image->mat();
 *
 * // 转换为QImage显示
 * QImage qimg = image->toQImage();
 *
 * // GPU加速处理
 * #ifdef USE_CUDA
 * image->uploadToGPU();
 * // ... GPU处理 ...
 * image->downloadFromGPU();
 * #endif
 * @endcode
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

    /**
     * @brief 从文件路径加载图像（支持中文路径）
     * @param filePath 图像文件路径（支持Unicode/中文路径）
     * @return ImageData智能指针，加载失败返回nullptr
     * @details 使用Qt的QFile读取文件内容，再用OpenCV解码
     *          支持的格式：BMP, PNG, JPG, JPEG, TIFF等
     */
    static Ptr loadFromFile(const QString& filePath);

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
