/**
 * @file HikvisionCamera.h
 * @brief 海康威视工业相机封装
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "hal/ICamera.h"
#include <QMutex>

#ifdef USE_HIKVISION_MVS

namespace VisionForge {
namespace HAL {

/**
 * @brief 相机设备信息
 */
struct CameraDeviceInfo {
    QString manufacturer;     // 厂商名称
    QString modelName;        // 型号
    QString serialNumber;     // 序列号
    QString deviceVersion;    // 固件版本
    QString userDefinedName;  // 用户定义名称
    QString ipAddress;        // IP地址 (GigE相机)
    QString macAddress;       // MAC地址
    int deviceType;           // 设备类型 (GigE/USB3等)

    CameraDeviceInfo()
        : deviceType(0)
    {}
};

/**
 * @brief 海康威视相机类
 *
 * 封装海康威视MVS SDK，实现ICamera接口
 * 支持GigE和USB3工业相机
 */
class HikvisionCamera : public ICamera {
    Q_OBJECT

public:
    /**
     * @brief 设备类型
     */
    enum DeviceType {
        Unknown = 0,
        GigE = 1,      // GigE Vision
        USB3 = 2,      // USB3 Vision
        CameraLink = 4 // Camera Link
    };

    explicit HikvisionCamera(QObject* parent = nullptr);
    ~HikvisionCamera() override;

    // ========== 静态方法 ==========

    /**
     * @brief 枚举所有在线设备
     * @return 设备信息列表
     */
    static QList<CameraDeviceInfo> enumerateDevices();

    /**
     * @brief 获取SDK版本
     */
    static QString sdkVersion();

    // ========== 设备选择 ==========

    /**
     * @brief 通过设备索引选择相机
     * @param index 设备索引 (从enumerateDevices获取)
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

    // ========== 海康特有功能 ==========

    /**
     * @brief 获取曝光时间范围
     * @param minVal 最小值 (μs)
     * @param maxVal 最大值 (μs)
     */
    void getExposureRange(double& minVal, double& maxVal) const;

    /**
     * @brief 获取增益范围
     * @param minVal 最小值 (dB)
     * @param maxVal 最大值 (dB)
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
     * @brief 获取当前帧率
     */
    double getFrameRate() const;

    /**
     * @brief 获取设备信息
     */
    CameraDeviceInfo getDeviceInfo() const { return deviceInfo_; }

    /**
     * @brief 保存参数到相机
     */
    bool saveParameters();

    /**
     * @brief 从相机加载参数
     */
    bool loadParameters();

private:
    // 内部辅助方法
    bool createHandle();
    void destroyHandle();
    bool setIntValue(const char* key, int64_t value);
    bool setFloatValue(const char* key, double value);
    bool setEnumValue(const char* key, unsigned int value);
    bool setBoolValue(const char* key, bool value);
    int64_t getIntValue(const char* key) const;
    double getFloatValue(const char* key) const;
    unsigned int getEnumValue(const char* key) const;
    bool getBoolValue(const char* key) const;
    void getIntRange(const char* key, int64_t& minVal, int64_t& maxVal) const;
    void getFloatRange(const char* key, double& minVal, double& maxVal) const;
    QString getLastError() const;

private:
    void* handle_;              // MV_CC_HANDLE_PTR
    bool isOpen_;
    bool isGrabbing_;
    Config config_;
    CameraDeviceInfo deviceInfo_;
    int selectedDeviceIndex_;

    mutable QMutex mutex_;

    // 图像缓冲区
    unsigned char* imageBuffer_;
    unsigned int bufferSize_;
};

} // namespace HAL
} // namespace VisionForge

#endif // USE_HIKVISION_MVS
