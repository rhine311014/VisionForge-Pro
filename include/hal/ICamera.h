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
 * @brief 触发模式枚举（扩展版）
 */
enum class TriggerModeEx {
    Continuous,         ///< 连续采集
    Software,           ///< 软件触发（原有）
    Hardware,           ///< 硬件触发（新增）
    ActionCommand,      ///< Action Command（新增，用于多相机同步）
    ExternalGPIO        ///< 外部GPIO（新增）
};

/**
 * @brief Action Command配置
 * 用于GenICam Action Command多相机同步触发
 */
struct ActionCommandConfig {
    uint32_t deviceKey = 0x1;           ///< 设备密钥
    uint32_t groupKey = 0x1;            ///< 组密钥
    uint32_t groupMask = 0xFFFFFFFF;    ///< 组掩码
    QString multicastAddress = "239.192.0.1";  ///< 组播地址
    uint16_t multicastPort = 42424;     ///< 组播端口
    bool enabled = false;               ///< 是否启用

    ActionCommandConfig() = default;
};

/**
 * @brief GPIO触发配置
 */
struct GPIOTriggerConfig {
    QString devicePath;                 ///< GPIO设备路径
    int pinNumber = 0;                  ///< 引脚号
    int pulseWidthUs = 100;             ///< 脉冲宽度（微秒）
    bool activeHigh = true;             ///< 高电平有效
    int debounceUs = 10;                ///< 去抖延时（微秒）
    bool enabled = false;               ///< 是否启用

    GPIOTriggerConfig() = default;
};

/**
 * @brief 硬件同步配置
 */
struct HardwareSyncConfig {
    TriggerModeEx triggerMode = TriggerModeEx::Software;  ///< 触发模式
    ActionCommandConfig actionCmd;      ///< Action Command配置
    GPIOTriggerConfig gpioConfig;       ///< GPIO触发配置
    int preTriggerDelayUs = 0;          ///< 预触发延迟（微秒）
    int postTriggerDelayUs = 0;         ///< 后触发延迟（微秒）
    int exposureDelayUs = 0;            ///< 曝光延迟（微秒）
    bool syncEnabled = false;           ///< 是否启用同步

    HardwareSyncConfig() = default;
};

/**
 * @brief 相机抽象接口
 *
 * 定义统一的相机接口，支持多种工业相机
 */
class ICamera : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 触发模式（兼容旧版本）
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

        // 图像变换设置
        int rotationAngle;     // 旋转角度: 0, 90, 180, 270
        bool flipHorizontal;   // 水平镜像
        bool flipVertical;     // 垂直镜像

        Config()
            : width(1920), height(1080)
            , offsetX(0), offsetY(0)
            , exposure(10000.0), gain(0.0)
            , triggerMode(Software)
            , rotationAngle(0)
            , flipHorizontal(false)
            , flipVertical(false)
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

    // ========== 扩展触发模式接口 ==========

    /**
     * @brief 设置扩展触发模式
     */
    virtual void setTriggerModeEx(TriggerModeEx mode) {
        // 默认实现：转换为基础触发模式
        switch (mode) {
        case TriggerModeEx::Continuous:
            setTriggerMode(Continuous);
            break;
        case TriggerModeEx::Software:
            setTriggerMode(Software);
            break;
        case TriggerModeEx::Hardware:
        case TriggerModeEx::ActionCommand:
        case TriggerModeEx::ExternalGPIO:
            setTriggerMode(Hardware);
            break;
        }
    }

    /**
     * @brief 获取扩展触发模式
     */
    virtual TriggerModeEx getTriggerModeEx() const {
        switch (getTriggerMode()) {
        case Continuous: return TriggerModeEx::Continuous;
        case Software: return TriggerModeEx::Software;
        case Hardware: return TriggerModeEx::Hardware;
        default: return TriggerModeEx::Software;
        }
    }

    /**
     * @brief 配置Action Command
     */
    virtual bool configureActionCommand(const ActionCommandConfig& config) {
        Q_UNUSED(config);
        return false;  // 默认不支持
    }

    /**
     * @brief 获取Action Command配置
     */
    virtual ActionCommandConfig getActionCommandConfig() const {
        return ActionCommandConfig();
    }

    /**
     * @brief 检查是否支持Action Command
     */
    virtual bool supportsActionCommand() const { return false; }

    /**
     * @brief 配置硬件同步
     */
    virtual bool configureHardwareSync(const HardwareSyncConfig& config) {
        Q_UNUSED(config);
        return false;  // 默认不支持
    }

    /**
     * @brief 获取硬件同步配置
     */
    virtual HardwareSyncConfig getHardwareSyncConfig() const {
        return HardwareSyncConfig();
    }

    /**
     * @brief 检查是否支持硬件同步
     */
    virtual bool supportsHardwareSync() const { return false; }

    /**
     * @brief 等待Action Command触发
     * @param timeoutMs 超时时间
     * @return 是否成功接收到触发
     */
    virtual bool waitForActionTrigger(int timeoutMs = 5000) {
        Q_UNUSED(timeoutMs);
        return false;
    }

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
