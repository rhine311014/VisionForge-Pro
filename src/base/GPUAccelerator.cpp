/**
 * @file GPUAccelerator.cpp
 * @brief GPU加速管理器实现
 */

#include "base/GPUAccelerator.h"
#include "base/ConfigManager.h"
#include "base/Logger.h"
#include <QDebug>

#ifdef USE_CUDA
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>
#endif

namespace VisionForge {
namespace Base {

GPUAccelerator::GPUAccelerator()
    : cudaAvailable_(false)
    , enabled_(true)
    , deviceCount_(0)
    , currentDevice_(-1)
    , accelMode_(GPUAccelMode::Auto)
{
    detectDevices();
    loadSettings();
}

GPUAccelerator& GPUAccelerator::instance()
{
    static GPUAccelerator instance;
    return instance;
}

bool GPUAccelerator::isCudaAvailable() const
{
    return cudaAvailable_;
}

int GPUAccelerator::getDeviceCount() const
{
    return deviceCount_;
}

int GPUAccelerator::getCurrentDevice() const
{
    return currentDevice_;
}

bool GPUAccelerator::setDevice(int deviceId)
{
#ifdef USE_CUDA
    if (!cudaAvailable_) {
        LOG_WARNING("CUDA不可用，无法设置设备");
        return false;
    }

    if (deviceId < 0 || deviceId >= deviceCount_) {
        LOG_ERROR(QString("无效的设备ID: %1").arg(deviceId));
        return false;
    }

    try {
        cv::cuda::setDevice(deviceId);
        currentDevice_ = deviceId;
        LOG_INFO(QString("GPU设备已切换到: %1").arg(deviceId));
        return true;
    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("设置GPU设备失败: %1").arg(e.what()));
        return false;
    }
#else
    Q_UNUSED(deviceId);
    LOG_WARNING("未编译CUDA支持");
    return false;
#endif
}

GPUDeviceInfo GPUAccelerator::getDeviceInfo(int deviceId) const
{
#ifdef USE_CUDA
    if (!cudaAvailable_) {
        return GPUDeviceInfo();
    }

    if (deviceId == -1) {
        deviceId = currentDevice_;
    }

    if (deviceId < 0 || deviceId >= devices_.size()) {
        return GPUDeviceInfo();
    }

    return devices_[deviceId];
#else
    Q_UNUSED(deviceId);
    return GPUDeviceInfo();
#endif
}

QList<GPUDeviceInfo> GPUAccelerator::getAllDevices() const
{
    return devices_;
}

void GPUAccelerator::printDeviceInfo() const
{
    if (!cudaAvailable_) {
        qInfo() << "CUDA不可用";
        return;
    }

    qInfo() << "========================================";
    qInfo() << "GPU设备信息";
    qInfo() << QString("设备数量: %1").arg(deviceCount_);
    qInfo() << QString("当前设备: %1").arg(currentDevice_);
    qInfo() << "========================================";

    for (int i = 0; i < devices_.size(); ++i) {
        const GPUDeviceInfo& info = devices_[i];
        qInfo() << QString("设备 %1: %2").arg(i).arg(info.name);
        qInfo() << QString("  总内存: %1 MB").arg(info.totalMemory / 1024 / 1024);
        qInfo() << QString("  可用内存: %1 MB").arg(info.freeMemory / 1024 / 1024);
        qInfo() << QString("  计算能力: %1").arg(info.computeCapability);
        qInfo() << QString("  多处理器数量: %1").arg(info.multiProcessorCount);
        qInfo() << QString("  状态: %1").arg(info.isAvailable ? "可用" : "不可用");
    }

    qInfo() << "========================================";
}

void GPUAccelerator::detectDevices()
{
#ifdef USE_CUDA
    try {
        deviceCount_ = cv::cuda::getCudaEnabledDeviceCount();

        if (deviceCount_ > 0) {
            cudaAvailable_ = true;
            LOG_INFO(QString("检测到 %1 个CUDA设备").arg(deviceCount_));

            // 获取每个设备的详细信息
            for (int i = 0; i < deviceCount_; ++i) {
                GPUDeviceInfo info;
                info.deviceId = i;
                info.isAvailable = true;

                // 设置当前设备
                cv::cuda::setDevice(i);

                // 获取设备属性
                cv::cuda::DeviceInfo devInfo(i);
                info.name = QString::fromStdString(devInfo.name());
                info.totalMemory = devInfo.totalMemory();
                info.freeMemory = devInfo.freeMemory();
                info.computeCapability = devInfo.majorVersion() * 10 + devInfo.minorVersion();
                info.multiProcessorCount = devInfo.multiProcessorCount();

                devices_.append(info);

                LOG_DEBUG(QString("  设备 %1: %2, 内存=%3MB, 计算能力=%4.%5")
                         .arg(i)
                         .arg(info.name)
                         .arg(info.totalMemory / 1024 / 1024)
                         .arg(devInfo.majorVersion())
                         .arg(devInfo.minorVersion()));
            }

            // 设置默认设备为第一个
            if (deviceCount_ > 0) {
                setDevice(0);
            }
        } else {
            cudaAvailable_ = false;
            LOG_INFO("未检测到CUDA设备");
        }
    } catch (const cv::Exception& e) {
        cudaAvailable_ = false;
        deviceCount_ = 0;
        LOG_WARNING(QString("CUDA检测失败: %1").arg(e.what()));
    }
#else
    cudaAvailable_ = false;
    deviceCount_ = 0;
    LOG_INFO("未编译CUDA支持");
#endif
}

void GPUAccelerator::setAccelMode(GPUAccelMode mode)
{
    accelMode_ = mode;

    // 根据模式更新enabled_状态
    switch (mode) {
        case GPUAccelMode::Disabled:
            enabled_ = false;
            LOG_INFO("GPU加速模式: 禁用");
            break;
        case GPUAccelMode::CUDA:
            enabled_ = cudaAvailable_;
            if (cudaAvailable_) {
                LOG_INFO("GPU加速模式: CUDA");
            } else {
                LOG_WARNING("GPU加速模式: CUDA (但CUDA不可用，将使用CPU)");
            }
            break;
        case GPUAccelMode::Auto:
            enabled_ = cudaAvailable_;
            LOG_INFO(QString("GPU加速模式: 自动 (%1)")
                    .arg(cudaAvailable_ ? "使用GPU" : "使用CPU"));
            break;
    }

    // 保存设置
    saveSettings();
}

bool GPUAccelerator::isUsingGPU() const
{
    switch (accelMode_) {
        case GPUAccelMode::Disabled:
            return false;
        case GPUAccelMode::CUDA:
            return cudaAvailable_;
        case GPUAccelMode::Auto:
            return cudaAvailable_;
        default:
            return false;
    }
}

QString GPUAccelerator::getAccelModeName(GPUAccelMode mode)
{
    switch (mode) {
        case GPUAccelMode::Disabled:
            return "禁用GPU加速";
        case GPUAccelMode::CUDA:
            return "CUDA GPU加速";
        case GPUAccelMode::Auto:
            return "自动选择";
        default:
            return "未知";
    }
}

void GPUAccelerator::saveSettings()
{
    ConfigManager& config = ConfigManager::instance();
    config.setValue("GPU/AccelMode", static_cast<int>(accelMode_));
    config.save();
    LOG_DEBUG(QString("GPU设置已保存: 模式=%1").arg(static_cast<int>(accelMode_)));
}

void GPUAccelerator::loadSettings()
{
    ConfigManager& config = ConfigManager::instance();
    int mode = config.getValue("GPU/AccelMode", static_cast<int>(GPUAccelMode::Auto)).toInt();

    // 验证模式值有效性
    if (mode < 0 || mode > 2) {
        mode = static_cast<int>(GPUAccelMode::Auto);
    }

    accelMode_ = static_cast<GPUAccelMode>(mode);

    // 根据模式更新enabled_状态（不保存，避免循环）
    switch (accelMode_) {
        case GPUAccelMode::Disabled:
            enabled_ = false;
            break;
        case GPUAccelMode::CUDA:
        case GPUAccelMode::Auto:
            enabled_ = cudaAvailable_;
            break;
    }

    LOG_DEBUG(QString("GPU设置已加载: 模式=%1, 实际使用GPU=%2")
             .arg(static_cast<int>(accelMode_))
             .arg(isUsingGPU() ? "是" : "否"));
}

#ifdef USE_CUDA

// ========== GPU加速图像处理实现 ==========

void GPUAccelerator::upload(const cv::Mat& cpuImage, cv::cuda::GpuMat& gpuImage)
{
    gpuImage.upload(cpuImage);
}

void GPUAccelerator::download(const cv::cuda::GpuMat& gpuImage, cv::Mat& cpuImage)
{
    gpuImage.download(cpuImage);
}

void GPUAccelerator::gaussianBlur(const cv::cuda::GpuMat& src,
                                 cv::cuda::GpuMat& dst,
                                 cv::Size ksize,
                                 double sigmaX,
                                 double sigmaY)
{
    auto filter = cv::cuda::createGaussianFilter(
        src.type(),
        dst.type(),
        ksize,
        sigmaX,
        sigmaY
    );
    filter->apply(src, dst);
}

void GPUAccelerator::cvtColor(const cv::cuda::GpuMat& src,
                             cv::cuda::GpuMat& dst,
                             int code)
{
    cv::cuda::cvtColor(src, dst, code);
}

void GPUAccelerator::threshold(const cv::cuda::GpuMat& src,
                              cv::cuda::GpuMat& dst,
                              double thresh,
                              double maxval,
                              int type)
{
    cv::cuda::threshold(src, dst, thresh, maxval, type);
}

void GPUAccelerator::sobel(const cv::cuda::GpuMat& src,
                          cv::cuda::GpuMat& dst,
                          int ddepth,
                          int dx,
                          int dy,
                          int ksize)
{
    auto filter = cv::cuda::createSobelFilter(
        src.type(),
        ddepth,
        dx,
        dy,
        ksize
    );
    filter->apply(src, dst);
}

void GPUAccelerator::canny(const cv::cuda::GpuMat& src,
                          cv::cuda::GpuMat& dst,
                          double low_thresh,
                          double high_thresh,
                          int apperture_size,
                          bool L2gradient)
{
    auto detector = cv::cuda::createCannyEdgeDetector(
        low_thresh,
        high_thresh,
        apperture_size,
        L2gradient
    );
    detector->detect(src, dst);
}

#endif // USE_CUDA

} // namespace Base
} // namespace VisionForge
