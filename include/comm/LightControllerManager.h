/**
 * @file LightControllerManager.h
 * @brief 光源控制器管理器头文件
 * @details 本文件定义了光源控制器管理系统的所有数据结构和管理类。
 *          采用单例模式统一管理多个光源控制器，支持多种品牌的光源控制器，
 *          提供串口和网络两种通信方式，实现配置的持久化存储。
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

// ============================================================================
// 系统常量定义
// ============================================================================

/**
 * @brief 系统支持的最大光源控制器数量
 * @details 定义系统可同时管理的光源控制器上限，用于数组大小分配
 */
constexpr int MAX_LIGHT_CONTROLLERS = 8;

/**
 * @brief 每个控制器支持的最大通道数
 * @details 定义单个光源控制器可控制的最大光源通道数量
 */
constexpr int MAX_LIGHT_CHANNELS = 8;

// ============================================================================
// 枚举类型定义
// ============================================================================

/**
 * @enum LightControllerType
 * @brief 光源控制器类型枚举
 * @details 定义系统支持的所有光源控制器品牌和型号。
 *          不同品牌的控制器通信协议可能不同，需要对应的驱动实现。
 *          数值分配规则：
 *          - 0-9: 康视达系列
 *          - 10-19: 锐视系列
 *          - 20-29: 风视系列
 *          - 30-39: 奥普特系列
 *          - 40-49: CCS系列
 *          - 50+: 其他品牌
 */
enum class LightControllerType {
    Unknown = -1,       ///< 未知类型，用于错误处理
    KangShiDa = 0,      ///< 康视达标准型光源控制器
    KangShiDa2 = 1,     ///< 康视达2代光源控制器（增强型）
    RuiShi = 10,        ///< 锐视标准型光源控制器
    RuiShi2 = 11,       ///< 锐视2代光源控制器
    FengShi = 20,       ///< 风视标准型光源控制器
    FengShi2 = 21,      ///< 风视2代光源控制器
    FengShi3 = 22,      ///< 风视3代光源控制器（最新型号）
    OPT = 30,           ///< 奥普特标准型光源控制器（最常用）
    OPT2 = 31,          ///< 奥普特2代光源控制器
    CCS = 40,           ///< CCS（日本品牌）光源控制器
    LKS = 50            ///< 力科胜光源控制器（亮度范围0-1000）
};

/**
 * @enum LightCommType
 * @brief 光源控制器通信类型枚举
 * @details 定义光源控制器与上位机的通信方式
 */
enum class LightCommType {
    Serial = 0,         ///< 串口通信（RS232/RS485），最常用的通信方式
    Network = 1         ///< 网络通信（TCP/IP），适用于远程控制场景
};

// ============================================================================
// 配置结构体定义
// ============================================================================

/**
 * @struct LightChannelConfig
 * @brief 单个光源通道配置结构
 * @details 存储单个光源通道的配置参数，包括名称、亮度和开关状态。
 *          每个光源控制器可有多个通道，每个通道独立控制一路光源。
 */
struct LightChannelConfig {
    QString name;           ///< 通道名称，用于UI显示和日志记录（如"顶光"、"侧光"）
    int brightness = 0;     ///< 亮度值，范围取决于控制器类型（通常0-255或0-1000）
    bool enabled = false;   ///< 通道开关状态，true表示通道已开启

    /**
     * @brief 将通道配置序列化为JSON对象
     * @return 包含通道配置的JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象反序列化通道配置
     * @param json 包含通道配置的JSON对象
     */
    void fromJson(const QJsonObject& json);
};

/**
 * @struct LightCtrlConfig
 * @brief 单个光源控制器配置结构
 * @details 存储单个光源控制器的完整配置信息，包括：
 *          - 基本信息：名称、是否启用
 *          - 设备信息：控制器类型
 *          - 通信参数：通信类型、串口号、波特率或IP地址、端口
 *          - 通道配置：所有通道的配置数组
 */
struct LightCtrlConfig {
    QString name;                       ///< 控制器名称，用于UI显示（如"光源控制器1"）
    bool used = false;                  ///< 是否启用此控制器，false时不参与连接和控制
    LightControllerType type = LightControllerType::OPT;    ///< 控制器类型，默认奥普特
    LightCommType commType = LightCommType::Serial;         ///< 通信类型，默认串口

    // ========== 串口通信配置 ==========
    int serialPort = 0;                 ///< 串口号（0表示COM1, 1表示COM2, 以此类推）
    int baudRate = 9600;                ///< 波特率，常用值：9600, 19200, 38400, 57600, 115200

    // ========== 网络通信配置 ==========
    QString ipAddress = "192.168.1.100";  ///< 控制器IP地址，用于网络通信模式
    int port = 5000;                      ///< 网络端口号，用于网络通信模式

    // ========== 通道配置 ==========
    int maxChannels = 8;                ///< 此控制器的最大通道数（不同型号可能不同）
    LightChannelConfig channels[MAX_LIGHT_CHANNELS];  ///< 所有通道的配置数组

    /**
     * @brief 将控制器配置序列化为JSON对象
     * @return 包含控制器配置的JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象反序列化控制器配置
     * @param json 包含控制器配置的JSON对象
     */
    void fromJson(const QJsonObject& json);
};

/**
 * @struct SystemLightConfig
 * @brief 系统光源配置结构
 * @details 存储整个系统的光源配置，包含所有光源控制器的配置数组。
 *          提供配置的文件读写功能，支持JSON格式的配置文件。
 */
struct SystemLightConfig {
    LightCtrlConfig controllers[MAX_LIGHT_CONTROLLERS];  ///< 所有控制器的配置数组

    /**
     * @brief 将系统配置序列化为JSON对象
     * @return 包含系统配置的JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象反序列化系统配置
     * @param json 包含系统配置的JSON对象
     */
    void fromJson(const QJsonObject& json);

    /**
     * @brief 从文件加载配置
     * @param filePath 配置文件的完整路径
     * @return 加载成功返回true，失败返回false
     */
    bool load(const QString& filePath);

    /**
     * @brief 保存配置到文件
     * @param filePath 配置文件的完整路径
     * @return 保存成功返回true，失败返回false
     */
    bool save(const QString& filePath) const;
};

// ============================================================================
// 光源控制器管理器类
// ============================================================================

/**
 * @class LightControllerManager
 * @brief 光源控制器管理器（单例模式）
 * @details 系统核心管理类，负责：
 *          - 统一管理多个光源控制器的生命周期
 *          - 提供控制器的打开、关闭、状态查询接口
 *          - 提供通道亮度和开关的控制接口
 *          - 管理配置的加载、保存和同步
 *          - 发送状态变化的信号通知
 *
 * 使用方法：
 * @code
 * // 获取单例实例
 * auto& mgr = LightControllerManager::instance();
 *
 * // 初始化（通常在程序启动时调用）
 * mgr.initialize("config/light_config.json");
 *
 * // 打开控制器
 * mgr.openController(0);
 *
 * // 设置亮度
 * mgr.setChannelBrightness(0, 0, 128);  // 控制器0，通道0，亮度128
 *
 * // 关闭时保存配置
 * mgr.shutdown();
 * @endcode
 */
class LightControllerManager : public QObject
{
    Q_OBJECT

public:
    // ========== 单例访问 ==========

    /**
     * @brief 获取单例实例
     * @return 管理器的唯一实例引用
     * @note 线程安全，采用C++11静态局部变量实现
     */
    static LightControllerManager& instance();

    /**
     * @brief 初始化管理器
     * @param configPath 配置文件路径，为空则不加载配置文件
     * @return 初始化成功返回true
     * @note 应在程序启动时调用一次，重复调用直接返回true
     */
    bool initialize(const QString& configPath = QString());

    /**
     * @brief 关闭所有控制器并释放资源
     * @note 会自动保存配置到文件，应在程序退出时调用
     */
    void shutdown();

    // ========== 控制器管理接口 ==========

    /**
     * @brief 打开指定控制器
     * @param index 控制器索引，范围 [0, MAX_LIGHT_CONTROLLERS-1]
     * @return 打开成功返回true，失败返回false
     * @note 会根据配置自动选择通信方式和参数。
     *       如果控制器已打开，会先关闭再重新打开。
     *       打开成功后会自动恢复保存的亮度设置。
     */
    bool openController(int index);

    /**
     * @brief 关闭指定控制器
     * @param index 控制器索引，范围 [0, MAX_LIGHT_CONTROLLERS-1]
     * @note 会断开通信连接并释放控制器对象
     */
    void closeController(int index);

    /**
     * @brief 检查控制器是否已打开（连接）
     * @param index 控制器索引
     * @return 已连接返回true，未连接或索引无效返回false
     */
    bool isControllerOpen(int index) const;

    /**
     * @brief 获取控制器对象指针
     * @param index 控制器索引
     * @return 控制器对象指针，索引无效或未创建时返回nullptr
     * @note 返回的指针由管理器管理，调用者不应delete
     */
    LightController* getController(int index);

    // ========== 亮度控制接口 ==========

    /**
     * @brief 设置通道亮度
     * @param ctrlIndex 控制器索引
     * @param channelIndex 通道索引，范围 [0, MAX_LIGHT_CHANNELS-1]
     * @param brightness 亮度值（范围取决于控制器类型）
     * @return 设置成功返回true
     * @note 会同时更新配置缓存和实际硬件（如已连接）
     */
    bool setChannelBrightness(int ctrlIndex, int channelIndex, int brightness);

    /**
     * @brief 获取通道亮度
     * @param ctrlIndex 控制器索引
     * @param channelIndex 通道索引
     * @return 亮度值，参数无效时返回0
     */
    int getChannelBrightness(int ctrlIndex, int channelIndex) const;

    /**
     * @brief 开启或关闭通道
     * @param ctrlIndex 控制器索引
     * @param channelIndex 通道索引
     * @param enabled true开启通道，false关闭通道
     * @return 操作成功返回true
     */
    bool setChannelEnabled(int ctrlIndex, int channelIndex, bool enabled);

    /**
     * @brief 获取通道开关状态
     * @param ctrlIndex 控制器索引
     * @param channelIndex 通道索引
     * @return true表示通道已开启，false表示关闭或参数无效
     */
    bool isChannelEnabled(int ctrlIndex, int channelIndex) const;

    /**
     * @brief 开启控制器的所有通道
     * @param ctrlIndex 控制器索引
     * @return 操作成功返回true
     */
    bool enableAllChannels(int ctrlIndex);

    /**
     * @brief 关闭控制器的所有通道
     * @param ctrlIndex 控制器索引
     * @return 操作成功返回true
     */
    bool disableAllChannels(int ctrlIndex);

    // ========== 配置管理接口 ==========

    /**
     * @brief 获取系统光源配置（可修改）
     * @return 系统配置的引用
     */
    SystemLightConfig& config() { return config_; }

    /**
     * @brief 获取系统光源配置（只读）
     * @return 系统配置的常量引用
     */
    const SystemLightConfig& config() const { return config_; }

    /**
     * @brief 获取指定控制器的配置（可修改）
     * @param index 控制器索引
     * @return 控制器配置的引用，索引无效时返回静态空对象
     */
    LightCtrlConfig& controllerConfig(int index);

    /**
     * @brief 获取指定控制器的配置（只读）
     * @param index 控制器索引
     * @return 控制器配置的常量引用
     */
    const LightCtrlConfig& controllerConfig(int index) const;

    /**
     * @brief 保存配置到文件
     * @return 保存成功返回true
     * @note 使用初始化时指定的配置文件路径
     */
    bool saveConfig();

    /**
     * @brief 重新从文件加载配置
     * @return 加载成功返回true
     * @note 会覆盖当前内存中的配置
     */
    bool reloadConfig();

    // ========== 静态辅助函数 ==========

    /**
     * @brief 获取指定控制器类型的亮度范围
     * @param type 控制器类型
     * @param minVal [out] 最小亮度值
     * @param maxVal [out] 最大亮度值
     * @note 大多数控制器范围是0-255，力科胜是0-1000
     */
    static void getBrightnessRange(LightControllerType type, int& minVal, int& maxVal);

    /**
     * @brief 获取控制器类型的中文名称
     * @param type 控制器类型
     * @return 类型对应的中文名称字符串
     */
    static QString getTypeName(LightControllerType type);

signals:
    // ========== 信号定义 ==========

    /**
     * @brief 控制器连接状态改变信号
     * @param index 控制器索引
     * @param connected true表示已连接，false表示已断开
     */
    void controllerConnectionChanged(int index, bool connected);

    /**
     * @brief 通道亮度改变信号
     * @param ctrlIndex 控制器索引
     * @param channelIndex 通道索引
     * @param brightness 新的亮度值
     */
    void channelBrightnessChanged(int ctrlIndex, int channelIndex, int brightness);

    /**
     * @brief 通道开关状态改变信号
     * @param ctrlIndex 控制器索引
     * @param channelIndex 通道索引
     * @param enabled 新的开关状态
     */
    void channelEnabledChanged(int ctrlIndex, int channelIndex, bool enabled);

    /**
     * @brief 错误发生信号
     * @param ctrlIndex 控制器索引
     * @param error 错误信息描述
     */
    void errorOccurred(int ctrlIndex, const QString& error);

private:
    // ========== 私有构造/析构（单例模式） ==========

    /**
     * @brief 私有构造函数
     * @note 单例模式，禁止外部直接构造
     */
    LightControllerManager();

    /**
     * @brief 私有析构函数
     * @note 会调用shutdown()确保资源释放
     */
    ~LightControllerManager();

    // 禁止拷贝和移动（单例模式）
    LightControllerManager(const LightControllerManager&) = delete;
    LightControllerManager& operator=(const LightControllerManager&) = delete;

    // ========== 私有辅助函数 ==========

    /**
     * @brief 根据类型创建对应的控制器对象
     * @param type 控制器类型
     * @return 创建的控制器对象智能指针
     * @note 使用工厂模式，根据类型创建不同子类的实例
     */
    std::unique_ptr<LightController> createController(LightControllerType type);

    /**
     * @brief 根据配置构建串口连接参数
     * @param cfg 控制器配置
     * @return 构建好的连接配置结构
     */
    LightControllerConfig buildSerialConfig(const LightCtrlConfig& cfg);

private:
    // ========== 私有成员变量 ==========

    QString configPath_;                ///< 配置文件完整路径
    SystemLightConfig config_;          ///< 系统光源配置数据
    std::unique_ptr<LightController> controllers_[MAX_LIGHT_CONTROLLERS];  ///< 控制器对象数组
    bool initialized_ = false;          ///< 初始化标志，防止重复初始化
};

} // namespace Comm
} // namespace VisionForge
