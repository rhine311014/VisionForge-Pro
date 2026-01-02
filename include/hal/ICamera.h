/**
 * @file ICamera.h
 * @brief 相机抽象接口定义
 * @details 定义了工业相机的统一抽象接口，支持多种触发模式、硬件同步和Action Command功能。
 *          该接口为所有具体相机实现（海康、Basler、GenTL等）提供了统一的访问方式，
 *          遵循GenICam标准设计，便于系统集成和相机切换。
 * @author VisionForge Team
 * @date 2025-12-14
 * @version 1.0
 *
 * @note 硬件通信协议说明：
 *       - 软件触发：通过SDK API发送TriggerSoftware命令
 *       - 硬件触发：通过外部GPIO信号触发（Line1/Line2等）
 *       - Action Command：通过UDP组播发送GigE Vision Action Command包
 */

#pragma once

#include "base/ImageData.h"
#include <QObject>
#include <QString>

namespace VisionForge {
namespace HAL {

/**
 * @brief 扩展触发模式枚举
 * @details 定义相机支持的各种触发采集模式，包括传统模式和高级硬件同步模式。
 *          扩展模式相比基础触发模式提供了更精细的控制能力。
 */
enum class TriggerModeEx {
    Continuous,         ///< 连续采集模式：相机自由运行，持续输出图像
    Software,           ///< 软件触发模式：通过SDK命令触发单帧采集
    Hardware,           ///< 硬件触发模式：通过外部IO信号触发采集
    ActionCommand,      ///< Action Command模式：用于GigE Vision多相机精确同步触发
    ExternalGPIO        ///< 外部GPIO触发模式：通过GPIO控制器发送触发信号
};

/**
 * @brief Action Command配置结构体
 * @details 用于配置GenICam Action Command多相机同步触发功能。
 *          Action Command是GigE Vision标准定义的一种组播触发机制，
 *          可以实现微秒级的多相机同步采集。
 *
 * @note 硬件通信协议：
 *       Action Command通过UDP组播发送，数据包格式：
 *       - Device Key (4 bytes): 设备密钥，相机必须匹配才响应
 *       - Group Key (4 bytes): 组密钥，用于分组触发
 *       - Group Mask (4 bytes): 组掩码，用于选择性触发
 *       默认组播地址为239.192.0.1:42424
 */
struct ActionCommandConfig {
    uint32_t deviceKey = 0x1;           ///< 设备密钥：相机需要匹配此密钥才会响应触发
    uint32_t groupKey = 0x1;            ///< 组密钥：用于对相机进行分组管理
    uint32_t groupMask = 0xFFFFFFFF;    ///< 组掩码：按位与操作，决定哪些相机响应
    QString multicastAddress = "239.192.0.1";  ///< 组播地址：Action Command的目标地址
    uint16_t multicastPort = 42424;     ///< 组播端口：Action Command的目标端口
    bool enabled = false;               ///< 启用标志：是否启用Action Command功能

    ActionCommandConfig() = default;
};

/**
 * @brief GPIO触发配置结构体
 * @details 配置通过GPIO控制器发送硬件触发信号的参数。
 *          适用于需要精确时序控制的多相机同步采集场景。
 *
 * @note 硬件通信协议：
 *       GPIO触发通过设置引脚电平实现，典型时序：
 *       1. 设置引脚为有效电平（高/低取决于activeHigh）
 *       2. 保持脉冲宽度（pulseWidthUs微秒）
 *       3. 恢复引脚为无效电平
 */
struct GPIOTriggerConfig {
    QString devicePath;                 ///< GPIO设备路径：如"COM3"或"/dev/ttyUSB0"
    int pinNumber = 0;                  ///< 引脚号：用于输出触发信号的GPIO引脚
    int pulseWidthUs = 100;             ///< 脉冲宽度：触发脉冲持续时间，单位微秒
    bool activeHigh = true;             ///< 电平极性：true为高电平有效，false为低电平有效
    int debounceUs = 10;                ///< 去抖延时：消除信号抖动的延时，单位微秒
    bool enabled = false;               ///< 启用标志：是否启用GPIO触发功能

    GPIOTriggerConfig() = default;
};

/**
 * @brief 硬件同步配置结构体
 * @details 综合配置相机的硬件同步采集参数，包括触发模式、延时设置等。
 *          用于实现多相机精确同步采集的场景。
 */
struct HardwareSyncConfig {
    TriggerModeEx triggerMode = TriggerModeEx::Software;  ///< 触发模式：选择使用哪种触发方式
    ActionCommandConfig actionCmd;      ///< Action Command配置：当使用Action Command模式时生效
    GPIOTriggerConfig gpioConfig;       ///< GPIO触发配置：当使用GPIO触发模式时生效
    int preTriggerDelayUs = 0;          ///< 预触发延迟：发送触发信号前的等待时间，单位微秒
    int postTriggerDelayUs = 0;         ///< 后触发延迟：发送触发信号后的等待时间，单位微秒
    int exposureDelayUs = 0;            ///< 曝光延迟：触发后到开始曝光的延迟，单位微秒
    bool syncEnabled = false;           ///< 同步启用：是否启用硬件同步功能

    HardwareSyncConfig() = default;
};

/**
 * @class ICamera
 * @brief 相机抽象接口类
 * @details 定义统一的工业相机接口，支持多种工业相机型号。
 *          该接口采用Qt信号槽机制实现异步事件通知，支持：
 *          - 设备管理：打开、关闭、状态查询
 *          - 采集控制：开始/停止采集、软件触发
 *          - 参数配置：曝光、增益、触发模式等
 *          - 硬件同步：Action Command、GPIO触发等
 *
 * @note 使用示例：
 * @code
 * ICamera* camera = CameraFactory::create(CameraType::Hikvision);
 * if (camera->open()) {
 *     camera->setExposure(10000);  // 设置曝光10ms
 *     camera->setTriggerMode(ICamera::Software);
 *     camera->startGrabbing();
 *     camera->trigger();
 *     auto image = camera->grabImage(1000);
 *     camera->stopGrabbing();
 *     camera->close();
 * }
 * @endcode
 */
class ICamera : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 基础触发模式枚举（兼容旧版本）
     * @details 定义相机的基础触发模式，用于向后兼容。
     *          新代码建议使用TriggerModeEx枚举获得更多功能。
     */
    enum TriggerMode {
        Continuous,    ///< 连续采集模式：相机自由运行，持续输出图像帧
        Software,      ///< 软件触发模式：调用trigger()方法触发单帧采集
        Hardware       ///< 硬件触发模式：通过外部IO信号（如Line1）触发采集
    };
    Q_ENUM(TriggerMode)

    /**
     * @brief 相机配置结构体
     * @details 包含相机的主要配置参数，包括图像尺寸、曝光、增益、触发模式和图像变换等。
     */
    struct Config {
        int width;             ///< 图像宽度：采集图像的水平像素数
        int height;            ///< 图像高度：采集图像的垂直像素数
        int offsetX;           ///< X偏移：ROI区域相对于传感器左边缘的偏移像素数
        int offsetY;           ///< Y偏移：ROI区域相对于传感器上边缘的偏移像素数
        double exposure;       ///< 曝光时间：单位微秒(us)，典型范围10-1000000
        double gain;           ///< 增益：单位分贝(dB)，典型范围0-24
        TriggerMode triggerMode;  ///< 触发模式：连续/软件触发/硬件触发

        // 图像变换设置
        int rotationAngle;     ///< 旋转角度：图像顺时针旋转角度，支持0/90/180/270度
        bool flipHorizontal;   ///< 水平镜像：是否沿垂直轴翻转图像
        bool flipVertical;     ///< 垂直镜像：是否沿水平轴翻转图像

        /**
         * @brief 默认构造函数
         * @details 初始化为常用的默认配置：1920x1080分辨率，10ms曝光，软件触发
         */
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

    /**
     * @brief 虚析构函数
     * @details 确保派生类正确释放资源
     */
    virtual ~ICamera() = default;

    // ========== 设备管理接口 ==========

    /**
     * @brief 打开相机设备
     * @details 建立与相机的连接，初始化相机资源。
     *          调用此方法前需要先选择具体的设备（通过序列号或索引）。
     * @return true表示成功打开，false表示打开失败（设备不存在、被占用等）
     * @note 打开相机后需要调用close()释放资源
     */
    virtual bool open() = 0;

    /**
     * @brief 关闭相机设备
     * @details 断开与相机的连接，释放所有相机资源。
     *          如果正在采集，会自动停止采集。
     */
    virtual void close() = 0;

    /**
     * @brief 检查相机是否已打开
     * @return true表示相机已打开且可用，false表示未打开
     */
    virtual bool isOpen() const = 0;

    /**
     * @brief 获取设备名称
     * @return 相机的型号名称，如"MV-CE050-10GM"
     */
    virtual QString deviceName() const = 0;

    /**
     * @brief 获取设备序列号
     * @return 相机的唯一序列号，用于设备识别
     */
    virtual QString serialNumber() const = 0;

    // ========== 采集控制接口 ==========

    /**
     * @brief 开始图像采集
     * @details 启动相机的图像采集流程。在连续模式下，相机开始持续输出图像；
     *          在触发模式下，相机进入待触发状态。
     * @return true表示成功开始采集，false表示启动失败
     * @note 需要先调用open()打开相机
     */
    virtual bool startGrabbing() = 0;

    /**
     * @brief 停止图像采集
     * @details 停止相机的图像采集流程，释放采集资源。
     */
    virtual void stopGrabbing() = 0;

    /**
     * @brief 检查是否正在采集
     * @return true表示正在采集中，false表示未采集
     */
    virtual bool isGrabbing() const = 0;

    /**
     * @brief 采集一帧图像
     * @details 从相机获取一帧图像数据。该方法会阻塞直到获取到图像或超时。
     *          在软件触发模式下，需要先调用trigger()发送触发命令。
     * @param timeoutMs 超时时间，单位毫秒，默认1000ms
     * @return 图像数据智能指针，采集失败返回nullptr
     * @note 返回的图像数据使用智能指针管理，无需手动释放
     */
    virtual Base::ImageData::Ptr grabImage(int timeoutMs = 1000) = 0;

    /**
     * @brief 发送软件触发命令
     * @details 在软件触发模式下，发送触发命令使相机采集一帧图像。
     *          触发后需要调用grabImage()获取采集的图像。
     * @return true表示触发成功，false表示触发失败
     * @note 仅在软件触发模式下有效
     */
    virtual bool trigger() = 0;

    // ========== 参数配置接口 ==========

    /**
     * @brief 设置相机配置
     * @details 批量设置相机的各项参数，包括分辨率、曝光、增益、触发模式等。
     * @param config 相机配置结构体
     */
    virtual void setConfig(const Config& config) = 0;

    /**
     * @brief 获取当前相机配置
     * @return 当前的相机配置结构体
     */
    virtual Config getConfig() const = 0;

    /**
     * @brief 设置曝光时间
     * @details 设置相机的曝光时间，影响图像亮度和运动模糊。
     * @param exposure 曝光时间，单位微秒(us)
     * @note 曝光时间受相机帧率限制，过长的曝光会降低帧率
     */
    virtual void setExposure(double exposure) = 0;

    /**
     * @brief 获取当前曝光时间
     * @return 当前曝光时间，单位微秒(us)
     */
    virtual double getExposure() const = 0;

    /**
     * @brief 设置增益
     * @details 设置相机的模拟增益，提高暗环境下的图像亮度。
     *          增益过高会增加图像噪声。
     * @param gain 增益值，单位分贝(dB)
     */
    virtual void setGain(double gain) = 0;

    /**
     * @brief 获取当前增益
     * @return 当前增益值，单位分贝(dB)
     */
    virtual double getGain() const = 0;

    /**
     * @brief 设置触发模式
     * @details 设置相机的触发采集模式。
     * @param mode 触发模式枚举值
     */
    virtual void setTriggerMode(TriggerMode mode) = 0;

    /**
     * @brief 获取当前触发模式
     * @return 当前触发模式枚举值
     */
    virtual TriggerMode getTriggerMode() const = 0;

    // ========== 扩展触发模式接口 ==========

    /**
     * @brief 设置扩展触发模式
     * @details 设置相机的扩展触发模式，支持更多触发方式。
     *          默认实现将扩展模式转换为基础触发模式。
     * @param mode 扩展触发模式枚举值
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
     * @details 获取当前的扩展触发模式。
     *          默认实现根据基础触发模式返回对应的扩展模式。
     * @return 扩展触发模式枚举值
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
     * @brief 配置Action Command参数
     * @details 配置GigE Vision Action Command多相机同步触发功能。
     *          需要相机支持Action Command功能（通常是GigE相机）。
     * @param config Action Command配置参数
     * @return true表示配置成功，false表示相机不支持或配置失败
     */
    virtual bool configureActionCommand(const ActionCommandConfig& config) {
        Q_UNUSED(config);
        return false;  // 默认不支持Action Command
    }

    /**
     * @brief 获取Action Command配置
     * @return 当前的Action Command配置参数
     */
    virtual ActionCommandConfig getActionCommandConfig() const {
        return ActionCommandConfig();
    }

    /**
     * @brief 检查是否支持Action Command
     * @return true表示支持，false表示不支持
     */
    virtual bool supportsActionCommand() const { return false; }

    /**
     * @brief 配置硬件同步参数
     * @details 配置多相机硬件同步采集的各项参数。
     * @param config 硬件同步配置参数
     * @return true表示配置成功，false表示配置失败
     */
    virtual bool configureHardwareSync(const HardwareSyncConfig& config) {
        Q_UNUSED(config);
        return false;  // 默认不支持硬件同步
    }

    /**
     * @brief 获取硬件同步配置
     * @return 当前的硬件同步配置参数
     */
    virtual HardwareSyncConfig getHardwareSyncConfig() const {
        return HardwareSyncConfig();
    }

    /**
     * @brief 检查是否支持硬件同步
     * @return true表示支持，false表示不支持
     */
    virtual bool supportsHardwareSync() const { return false; }

    /**
     * @brief 等待Action Command触发
     * @details 阻塞等待Action Command触发信号。用于确认相机已接收到触发。
     * @param timeoutMs 超时时间，单位毫秒，默认5000ms
     * @return true表示成功接收到触发，false表示超时或失败
     */
    virtual bool waitForActionTrigger(int timeoutMs = 5000) {
        Q_UNUSED(timeoutMs);
        return false;
    }

signals:
    /**
     * @brief 图像采集完成信号
     * @details 当相机成功采集一帧图像时发出此信号。
     *          连接此信号可以实现异步图像处理。
     * @param image 采集到的图像数据智能指针
     */
    void imageGrabbed(Base::ImageData::Ptr image);

    /**
     * @brief 错误发生信号
     * @details 当相机操作发生错误时发出此信号。
     *          可用于错误日志记录和用户界面提示。
     * @param error 错误信息描述
     */
    void errorOccurred(const QString& error);

protected:
    /**
     * @brief 受保护的构造函数
     * @details 只能通过派生类或工厂方法创建实例
     * @param parent Qt父对象指针
     */
    explicit ICamera(QObject* parent = nullptr) : QObject(parent) {}
};

} // namespace HAL
} // namespace VisionForge
