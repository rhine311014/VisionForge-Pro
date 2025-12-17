/**
 * @file BaslerCamera.h
 * @brief Basler工业相机封装 (预留框架)
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "hal/ICamera.h"
#include <QMutex>

#ifdef USE_BASLER_PYLON

namespace VisionForge {
namespace HAL {

/**
 * @brief Basler相机设备信息
 */
struct BaslerDeviceInfo {
    QString modelName;        // 型号
    QString serialNumber;     // 序列号
    QString ipAddress;        // IP地址 (GigE相机)
    QString interfaceType;    // 接口类型 (GigE/USB3)
    QString vendorName;       // 厂商名称

    BaslerDeviceInfo()
    {}
};

/**
 * @brief Basler相机类
 *
 * 封装Basler Pylon SDK，实现ICamera接口
 * 支持GigE和USB3工业相机
 *
 * @note 此为预留框架，需要安装Basler Pylon SDK后实现
 */
class BaslerCamera : public ICamera {
    Q_OBJECT

public:
    explicit BaslerCamera(QObject* parent = nullptr);
    ~BaslerCamera() override;

    // ========== 静态方法 ==========

    /**
     * @brief 枚举所有在线设备
     * @return 设备信息列表
     */
    static QList<BaslerDeviceInfo> enumerateDevices();

    /**
     * @brief 获取SDK版本
     */
    static QString sdkVersion();

    // ========== 设备选择 ==========

    /**
     * @brief 通过设备索引选择相机
     * @param index 设备索引
     */
    void selectDevice(int index);

    /**
     * @brief 通过序列号选择相机
     * @param serialNumber 序列号
     * @return true如果找到设备
     */
    bool selectBySerialNumber(const QString& serialNumber);

    /**
     * @brief 通过IP地址选择相机 (GigE)
     * @param ipAddress IP地址
     * @return true如果找到设备
     */
    bool selectByIP(const QString& ipAddress);

    // ========== ICamera接口实现 ==========

    bool open() override;
    void close() override;
    bool isOpen() const override { return isOpen_; }

    QString deviceName() const override { return deviceInfo_.modelName; }
    QString serialNumber() const override { return deviceInfo_.serialNumber; }

    bool startGrabbing() override;
    void stopGrabbing() override;
    bool isGrabbing() const override { return isGrabbing_; }

    Base::ImageData::Ptr grabImage(int timeoutMs = 1000) override;
    bool trigger() override;

    void setConfig(const Config& config) override;
    Config getConfig() const override { return config_; }

    void setExposure(double exposure) override;
    double getExposure() const override;

    void setGain(double gain) override;
    double getGain() const override;

    void setTriggerMode(TriggerMode mode) override;
    TriggerMode getTriggerMode() const override { return config_.triggerMode; }

    // ========== Basler特有功能 ==========

    /**
     * @brief 获取曝光时间范围
     */
    void getExposureRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取增益范围
     */
    void getGainRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取图像宽度范围
     */
    void getWidthRange(int& minVal, int& maxVal) const;

    /**
     * @brief 获取图像高度范围
     */
    void getHeightRange(int& minVal, int& maxVal) const;

    /**
     * @brief 设置ROI区域
     */
    bool setROI(int offsetX, int offsetY, int width, int height);

    /**
     * @brief 获取设备信息
     */
    BaslerDeviceInfo getDeviceInfo() const { return deviceInfo_; }

private:
    bool isOpen_;
    bool isGrabbing_;
    Config config_;
    BaslerDeviceInfo deviceInfo_;
    int selectedDeviceIndex_;

    mutable QMutex mutex_;

    // Pylon相机对象 (使用void*避免头文件依赖)
    void* camera_;  // Pylon::CInstantCamera*
};

} // namespace HAL
} // namespace VisionForge

#endif // USE_BASLER_PYLON
