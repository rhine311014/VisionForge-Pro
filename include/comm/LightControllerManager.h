/**
 * @file LightControllerManager.h
 * @brief 光源控制器管理器
 * @details 单例模式管理多个光源控制器
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "comm/LightController.h"
#include <QObject>
#include <QJsonObject>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Comm {

// 最大光源控制器数量
constexpr int MAX_LIGHT_CONTROLLERS = 8;
// 每个控制器最大通道数
constexpr int MAX_LIGHT_CHANNELS = 8;

/**
 * @brief 光源控制器类型
 */
enum class LightControllerType {
    Unknown = -1,
    KangShiDa = 0,      // 康视达
    KangShiDa2 = 1,     // 康视达2
    RuiShi = 10,        // 锐视
    RuiShi2 = 11,       // 锐视2
    FengShi = 20,       // 风视
    FengShi2 = 21,      // 风视2
    FengShi3 = 22,      // 风视3
    OPT = 30,           // 奥普特
    OPT2 = 31,          // 奥普特2
    CCS = 40,           // CCS
    LKS = 50            // 力科胜
};

/**
 * @brief 通信类型
 */
enum class LightCommType {
    Serial = 0,         // 串口
    Network = 1         // 网络
};

/**
 * @brief 单个通道配置
 */
struct LightChannelConfig {
    QString name;           // 通道名称
    int brightness = 0;     // 亮度值
    bool enabled = false;   // 是否开启

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

/**
 * @brief 单个光源控制器配置
 */
struct LightCtrlConfig {
    QString name;                       // 控制器名称
    bool used = false;                  // 是否启用
    LightControllerType type = LightControllerType::OPT;    // 控制器类型
    LightCommType commType = LightCommType::Serial;         // 通信类型

    // 串口配置
    int serialPort = 0;                 // 串口号 (0=COM1, 1=COM2, ...)
    int baudRate = 9600;                // 波特率

    // 网络配置
    QString ipAddress = "192.168.1.100";
    int port = 5000;

    // 通道配置
    int maxChannels = 8;                // 最大通道数
    LightChannelConfig channels[MAX_LIGHT_CHANNELS];

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

/**
 * @brief 系统光源配置
 */
struct SystemLightConfig {
    LightCtrlConfig controllers[MAX_LIGHT_CONTROLLERS];

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);

    // 从文件加载/保存
    bool load(const QString& filePath);
    bool save(const QString& filePath) const;
};

/**
 * @class LightControllerManager
 * @brief 光源控制器管理器（单例）
 *
 * 功能：
 * - 管理多个光源控制器
 * - 提供统一的控制接口
 * - 配置持久化
 */
class LightControllerManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static LightControllerManager& instance();

    /**
     * @brief 初始化管理器
     * @param configPath 配置文件路径
     */
    bool initialize(const QString& configPath = QString());

    /**
     * @brief 关闭所有控制器
     */
    void shutdown();

    // ========== 控制器管理 ==========

    /**
     * @brief 打开指定控制器
     * @param index 控制器索引 (0-7)
     * @return 是否成功
     */
    bool openController(int index);

    /**
     * @brief 关闭指定控制器
     */
    void closeController(int index);

    /**
     * @brief 检查控制器是否打开
     */
    bool isControllerOpen(int index) const;

    /**
     * @brief 获取控制器对象
     */
    LightController* getController(int index);

    // ========== 亮度控制 ==========

    /**
     * @brief 设置通道亮度
     * @param ctrlIndex 控制器索引
     * @param channelIndex 通道索引
     * @param brightness 亮度值 (0-255)
     */
    bool setChannelBrightness(int ctrlIndex, int channelIndex, int brightness);

    /**
     * @brief 获取通道亮度
     */
    int getChannelBrightness(int ctrlIndex, int channelIndex) const;

    /**
     * @brief 开启/关闭通道
     */
    bool setChannelEnabled(int ctrlIndex, int channelIndex, bool enabled);

    /**
     * @brief 获取通道状态
     */
    bool isChannelEnabled(int ctrlIndex, int channelIndex) const;

    /**
     * @brief 开启控制器所有通道
     */
    bool enableAllChannels(int ctrlIndex);

    /**
     * @brief 关闭控制器所有通道
     */
    bool disableAllChannels(int ctrlIndex);

    // ========== 配置管理 ==========

    /**
     * @brief 获取系统光源配置
     */
    SystemLightConfig& config() { return config_; }
    const SystemLightConfig& config() const { return config_; }

    /**
     * @brief 获取控制器配置
     */
    LightCtrlConfig& controllerConfig(int index);
    const LightCtrlConfig& controllerConfig(int index) const;

    /**
     * @brief 保存配置
     */
    bool saveConfig();

    /**
     * @brief 重新加载配置
     */
    bool reloadConfig();

    /**
     * @brief 获取亮度范围
     */
    static void getBrightnessRange(LightControllerType type, int& minVal, int& maxVal);

    /**
     * @brief 获取控制器类型名称
     */
    static QString getTypeName(LightControllerType type);

signals:
    /**
     * @brief 控制器连接状态改变
     */
    void controllerConnectionChanged(int index, bool connected);

    /**
     * @brief 通道亮度改变
     */
    void channelBrightnessChanged(int ctrlIndex, int channelIndex, int brightness);

    /**
     * @brief 通道状态改变
     */
    void channelEnabledChanged(int ctrlIndex, int channelIndex, bool enabled);

    /**
     * @brief 错误发生
     */
    void errorOccurred(int ctrlIndex, const QString& error);

private:
    LightControllerManager();
    ~LightControllerManager();

    // 禁止拷贝和移动
    LightControllerManager(const LightControllerManager&) = delete;
    LightControllerManager& operator=(const LightControllerManager&) = delete;

    /**
     * @brief 创建指定类型的控制器
     */
    std::unique_ptr<LightController> createController(LightControllerType type);

    /**
     * @brief 配置串口参数
     */
    LightControllerConfig buildSerialConfig(const LightCtrlConfig& cfg);

private:
    QString configPath_;
    SystemLightConfig config_;
    std::unique_ptr<LightController> controllers_[MAX_LIGHT_CONTROLLERS];
    bool initialized_ = false;
};

} // namespace Comm
} // namespace VisionForge
