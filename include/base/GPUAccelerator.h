/**
 * @file GPUAccelerator.h
 * @brief GPU加速管理器
 * @details 管理GPU设备，提供GPU加速功能
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "base/ImageData.h"
#include <QString>
#include <QStringList>
#include <opencv2/opencv.hpp>

#ifdef USE_CUDA
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#endif

namespace VisionForge {
namespace Base {

/**
 * @brief GPU设备信息
 */
struct GPUDeviceInfo {
    int deviceId;                   // 设备ID
    QString name;                   // 设备名称
    size_t totalMemory;             // 总内存（字节）
    size_t freeMemory;              // 可用内存（字节）
    int computeCapability;          // 计算能力
    int multiProcessorCount;        // 多处理器数量
    bool isAvailable;               // 是否可用

    GPUDeviceInfo()
        : deviceId(-1)
        , totalMemory(0)
        , freeMemory(0)
        , computeCapability(0)
        , multiProcessorCount(0)
        , isAvailable(false)
    {}
};

/**
 * @brief GPU加速器
 *
 * 单例模式，管理GPU设备和加速功能
 */
class GPUAccelerator {
public:
    /**
     * @brief 获取单例实例
     */
    static GPUAccelerator& instance();

    /**
     * @brief 检查CUDA是否可用
     */
    bool isCudaAvailable() const;

    /**
     * @brief 获取GPU设备数量
     */
    int getDeviceCount() const;

    /**
     * @brief 获取当前设备ID
     */
    int getCurrentDevice() const;

    /**
     * @brief 设置当前设备
     * @param deviceId 设备ID
     * @return true如果成功
     */
    bool setDevice(int deviceId);

    /**
     * @brief 获取设备信息
     * @param deviceId 设备ID（-1表示当前设备）
     */
    GPUDeviceInfo getDeviceInfo(int deviceId = -1) const;

    /**
     * @brief 获取所有设备信息
     */
    QList<GPUDeviceInfo> getAllDevices() const;

    /**
     * @brief 打印设备信息
     */
    void printDeviceInfo() const;

    /**
     * @brief 启用/禁用GPU加速
     */
    void setEnabled(bool enabled) { enabled_ = enabled; }

    /**
     * @brief 检查GPU加速是否启用
     */
    bool isEnabled() const { return enabled_ && isCudaAvailable(); }

#ifdef USE_CUDA
    // ========== GPU加速图像处理 ==========

    /**
     * @brief GPU图像上传
     * @param cpuImage CPU图像
     * @param gpuImage GPU图像（输出）
     */
    static void upload(const cv::Mat& cpuImage, cv::cuda::GpuMat& gpuImage);

    /**
     * @brief GPU图像下载
     * @param gpuImage GPU图像
     * @param cpuImage CPU图像（输出）
     */
    static void download(const cv::cuda::GpuMat& gpuImage, cv::Mat& cpuImage);

    /**
     * @brief GPU高斯模糊
     * @param src 源图像
     * @param dst 目标图像
     * @param ksize 核大小
     * @param sigmaX X方向sigma
     * @param sigmaY Y方向sigma
     */
    static void gaussianBlur(const cv::cuda::GpuMat& src,
                            cv::cuda::GpuMat& dst,
                            cv::Size ksize,
                            double sigmaX,
                            double sigmaY = 0);

    /**
     * @brief GPU灰度转换
     * @param src 源图像（BGR）
     * @param dst 目标图像（灰度）
     */
    static void cvtColor(const cv::cuda::GpuMat& src,
                        cv::cuda::GpuMat& dst,
                        int code);

    /**
     * @brief GPU阈值处理
     * @param src 源图像
     * @param dst 目标图像
     * @param thresh 阈值
     * @param maxval 最大值
     * @param type 阈值类型
     */
    static void threshold(const cv::cuda::GpuMat& src,
                         cv::cuda::GpuMat& dst,
                         double thresh,
                         double maxval,
                         int type);

    /**
     * @brief GPU Sobel边缘检测
     * @param src 源图像
     * @param dst 目标图像
     * @param ddepth 输出深度
     * @param dx X方向导数阶数
     * @param dy Y方向导数阶数
     * @param ksize 核大小
     */
    static void sobel(const cv::cuda::GpuMat& src,
                     cv::cuda::GpuMat& dst,
                     int ddepth,
                     int dx,
                     int dy,
                     int ksize = 3);

    /**
     * @brief GPU Canny边缘检测
     * @param src 源图像
     * @param dst 目标图像
     * @param low_thresh 低阈值
     * @param high_thresh 高阈值
     * @param apperture_size 孔径大小
     * @param L2gradient 是否使用L2梯度
     */
    static void canny(const cv::cuda::GpuMat& src,
                     cv::cuda::GpuMat& dst,
                     double low_thresh,
                     double high_thresh,
                     int apperture_size = 3,
                     bool L2gradient = false);

#endif // USE_CUDA

private:
    GPUAccelerator();
    ~GPUAccelerator() = default;

    // 禁止拷贝
    GPUAccelerator(const GPUAccelerator&) = delete;
    GPUAccelerator& operator=(const GPUAccelerator&) = delete;

    void detectDevices();

private:
    bool cudaAvailable_;
    bool enabled_;
    int deviceCount_;
    int currentDevice_;
    QList<GPUDeviceInfo> devices_;
};

} // namespace Base
} // namespace VisionForge
