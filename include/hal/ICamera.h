/**
 * @file ICamera.h
 * @brief 相机抽象接口
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "base/ImageData.h"
#include <QObject>
#include <QString>

namespace VisionForge {
namespace HAL {

/**
 * @brief 相机抽象接口
 *
 * 定义统一的相机接口，支持多种工业相机
 */
class ICamera : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 触发模式
     */
    enum TriggerMode {
        Continuous,    // 连续采集
        Software,      // 软件触发
        Hardware       // 硬件触发
    };
    Q_ENUM(TriggerMode)

    /**
     * @brief 相机配置
     */
    struct Config {
        int width;             // 图像宽度
        int height;            // 图像高度
        int offsetX;           // X偏移
        int offsetY;           // Y偏移
        double exposure;       // 曝光时间(μs)
        double gain;           // 增益(dB)
        TriggerMode triggerMode;  // 触发模式

        Config()
            : width(1920), height(1080)
            , offsetX(0), offsetY(0)
            , exposure(10000.0), gain(0.0)
            , triggerMode(Software)
        {}
    };

    virtual ~ICamera() = default;

    // ========== 设备管理 ==========

    /**
     * @brief 打开相机
     * @return true如果成功
     */
    virtual bool open() = 0;

    /**
     * @brief 关闭相机
     */
    virtual void close() = 0;

    /**
     * @brief 检查相机是否打开
     */
    virtual bool isOpen() const = 0;

    /**
     * @brief 获取设备名称
     */
    virtual QString deviceName() const = 0;

    /**
     * @brief 获取序列号
     */
    virtual QString serialNumber() const = 0;

    // ========== 采集控制 ==========

    /**
     * @brief 开始采集
     * @return true如果成功
     */
    virtual bool startGrabbing() = 0;

    /**
     * @brief 停止采集
     */
    virtual void stopGrabbing() = 0;

    /**
     * @brief 检查是否正在采集
     */
    virtual bool isGrabbing() const = 0;

    /**
     * @brief 采集一帧图像
     * @param timeoutMs 超时时间(毫秒)
     * @return 图像数据，失败返回nullptr
     */
    virtual Base::ImageData::Ptr grabImage(int timeoutMs = 1000) = 0;

    /**
     * @brief 软件触发
     * @return true如果成功
     */
    virtual bool trigger() = 0;

    // ========== 参数配置 ==========

    /**
     * @brief 设置配置
     */
    virtual void setConfig(const Config& config) = 0;

    /**
     * @brief 获取配置
     */
    virtual Config getConfig() const = 0;

    /**
     * @brief 设置曝光时间
     * @param exposure 曝光时间(μs)
     */
    virtual void setExposure(double exposure) = 0;

    /**
     * @brief 获取曝光时间
     */
    virtual double getExposure() const = 0;

    /**
     * @brief 设置增益
     * @param gain 增益(dB)
     */
    virtual void setGain(double gain) = 0;

    /**
     * @brief 获取增益
     */
    virtual double getGain() const = 0;

    /**
     * @brief 设置触发模式
     */
    virtual void setTriggerMode(TriggerMode mode) = 0;

    /**
     * @brief 获取触发模式
     */
    virtual TriggerMode getTriggerMode() const = 0;

signals:
    /**
     * @brief 图像采集完成信号
     * @param image 采集的图像
     */
    void imageGrabbed(Base::ImageData::Ptr image);

    /**
     * @brief 错误发生信号
     * @param error 错误信息
     */
    void errorOccurred(const QString& error);

protected:
    explicit ICamera(QObject* parent = nullptr) : QObject(parent) {}
};

} // namespace HAL
} // namespace VisionForge
