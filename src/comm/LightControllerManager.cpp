/**
 * @file LightControllerManager.cpp
 * @brief 光源控制器管理器实现文件
 * @details 实现光源控制器管理器的所有功能，包括：
 *          - 配置结构体的JSON序列化/反序列化
 *          - 配置文件的读写操作
 *          - 控制器的创建、打开、关闭管理
 *          - 通道亮度和开关控制
 *          - 信号槽机制的连接和转发
 */

#include "comm/LightControllerManager.h"
#include "base/Logger.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

namespace VisionForge {
namespace Comm {

// ============================================================================
// LightChannelConfig 实现 - 通道配置的JSON序列化
// ============================================================================

/**
 * @brief 将通道配置序列化为JSON对象
 * @return 包含name、brightness、enabled字段的JSON对象
 */
QJsonObject LightChannelConfig::toJson() const
{
    QJsonObject json;
    json["name"] = name;              // 通道名称
    json["brightness"] = brightness;  // 亮度值
    json["enabled"] = enabled;        // 开关状态
    return json;
}

/**
 * @brief 从JSON对象反序列化通道配置
 * @param json 包含通道配置的JSON对象
 * @note 缺失的字段会使用默认值：brightness=0, enabled=false
 */
void LightChannelConfig::fromJson(const QJsonObject& json)
{
    name = json.value("name").toString();
    brightness = json.value("brightness").toInt(0);      // 默认亮度0
    enabled = json.value("enabled").toBool(false);       // 默认关闭
}

// ============================================================================
// LightCtrlConfig 实现 - 控制器配置的JSON序列化
// ============================================================================

/**
 * @brief 将控制器配置序列化为JSON对象
 * @return 包含完整控制器配置的JSON对象
 * @details 序列化内容包括：
 *          - 基本信息：name, used
 *          - 类型信息：type, commType（转为整数存储）
 *          - 串口配置：serialPort, baudRate
 *          - 网络配置：ipAddress, port
 *          - 通道配置：maxChannels, channels数组
 */
QJsonObject LightCtrlConfig::toJson() const
{
    QJsonObject json;

    // 基本信息
    json["name"] = name;
    json["used"] = used;

    // 类型信息（枚举转整数）
    json["type"] = static_cast<int>(type);
    json["commType"] = static_cast<int>(commType);

    // 串口配置
    json["serialPort"] = serialPort;
    json["baudRate"] = baudRate;

    // 网络配置
    json["ipAddress"] = ipAddress;
    json["port"] = port;

    // 通道配置
    json["maxChannels"] = maxChannels;

    // 将所有通道配置序列化为JSON数组
    QJsonArray channelArray;
    for (int i = 0; i < MAX_LIGHT_CHANNELS; ++i) {
        channelArray.append(channels[i].toJson());
    }
    json["channels"] = channelArray;

    return json;
}

/**
 * @brief 从JSON对象反序列化控制器配置
 * @param json 包含控制器配置的JSON对象
 * @note 使用合理的默认值处理缺失字段：
 *       - type默认OPT(30)
 *       - commType默认Serial(0)
 *       - baudRate默认9600
 *       - maxChannels默认8
 */
void LightCtrlConfig::fromJson(const QJsonObject& json)
{
    // 读取基本信息
    name = json.value("name").toString();
    used = json.value("used").toBool(false);

    // 读取类型信息（整数转枚举，默认OPT=30）
    type = static_cast<LightControllerType>(json.value("type").toInt(30));
    commType = static_cast<LightCommType>(json.value("commType").toInt(0));

    // 读取串口配置
    serialPort = json.value("serialPort").toInt(0);
    baudRate = json.value("baudRate").toInt(9600);

    // 读取网络配置
    ipAddress = json.value("ipAddress").toString("192.168.1.100");
    port = json.value("port").toInt(5000);

    // 读取通道配置
    maxChannels = json.value("maxChannels").toInt(8);

    // 从JSON数组读取通道配置
    QJsonArray channelArray = json.value("channels").toArray();
    for (int i = 0; i < MAX_LIGHT_CHANNELS && i < channelArray.size(); ++i) {
        channels[i].fromJson(channelArray[i].toObject());
    }
}

// ============================================================================
// SystemLightConfig 实现 - 系统配置的JSON序列化和文件操作
// ============================================================================

/**
 * @brief 将系统配置序列化为JSON对象
 * @return 包含controllers数组的JSON对象
 */
QJsonObject SystemLightConfig::toJson() const
{
    QJsonObject json;

    // 将所有控制器配置序列化为JSON数组
    QJsonArray ctrlArray;
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS; ++i) {
        ctrlArray.append(controllers[i].toJson());
    }
    json["controllers"] = ctrlArray;

    return json;
}

/**
 * @brief 从JSON对象反序列化系统配置
 * @param json 包含系统配置的JSON对象
 */
void SystemLightConfig::fromJson(const QJsonObject& json)
{
    // 从JSON数组读取控制器配置
    QJsonArray ctrlArray = json.value("controllers").toArray();
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS && i < ctrlArray.size(); ++i) {
        controllers[i].fromJson(ctrlArray[i].toObject());
    }
}

/**
 * @brief 从文件加载配置
 * @param filePath 配置文件的完整路径
 * @return 加载成功返回true，失败返回false
 * @details 读取JSON格式的配置文件并解析。
 *          失败情况：文件不存在、无法打开、JSON格式错误
 */
bool SystemLightConfig::load(const QString& filePath)
{
    // 打开文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_WARNING(QString("无法打开光源配置文件: %1").arg(filePath));
        return false;
    }

    // 读取并解析JSON
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    // 验证JSON格式
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("光源配置文件格式错误: %1").arg(filePath));
        return false;
    }

    // 反序列化配置
    fromJson(doc.object());
    LOG_INFO(QString("光源配置加载成功: %1").arg(filePath));
    return true;
}

/**
 * @brief 保存配置到文件
 * @param filePath 配置文件的完整路径
 * @return 保存成功返回true，失败返回false
 * @details 将配置序列化为格式化的JSON并写入文件
 */
bool SystemLightConfig::save(const QString& filePath) const
{
    // 打开文件（覆盖模式）
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法保存光源配置文件: %1").arg(filePath));
        return false;
    }

    // 序列化为格式化JSON并写入
    QJsonDocument doc(toJson());
    file.write(doc.toJson(QJsonDocument::Indented));  // 使用缩进格式，便于阅读
    file.close();

    LOG_INFO(QString("光源配置保存成功: %1").arg(filePath));
    return true;
}

// ============================================================================
// LightControllerManager 实现 - 单例模式和生命周期管理
// ============================================================================

/**
 * @brief 获取单例实例
 * @return 管理器的唯一实例引用
 * @details 使用C++11静态局部变量实现线程安全的单例模式
 */
LightControllerManager& LightControllerManager::instance()
{
    static LightControllerManager instance;  // 第一次调用时构造，程序结束时析构
    return instance;
}

/**
 * @brief 构造函数 - 初始化默认配置
 * @details 为所有控制器和通道设置默认名称
 */
LightControllerManager::LightControllerManager()
    : QObject(nullptr)
{
    // 初始化每个控制器的默认配置
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS; ++i) {
        // 设置控制器默认名称
        config_.controllers[i].name = QString("光源控制器%1").arg(i + 1);

        // 设置每个通道的默认名称
        for (int ch = 0; ch < MAX_LIGHT_CHANNELS; ++ch) {
            config_.controllers[i].channels[ch].name = QString("通道%1").arg(ch + 1);
        }
    }
}

/**
 * @brief 析构函数 - 确保资源释放
 */
LightControllerManager::~LightControllerManager()
{
    shutdown();  // 关闭所有控制器并保存配置
}

/**
 * @brief 初始化管理器
 * @param configPath 配置文件路径
 * @return 始终返回true
 * @details 如果指定了配置文件且存在，则加载配置。
 *          重复调用直接返回true，不会重复初始化。
 */
bool LightControllerManager::initialize(const QString& configPath)
{
    // 防止重复初始化
    if (initialized_) {
        return true;
    }

    configPath_ = configPath;

    // 如果指定了配置文件，尝试加载
    if (!configPath_.isEmpty() && QFile::exists(configPath_)) {
        config_.load(configPath_);
    }

    initialized_ = true;
    LOG_INFO("光源控制器管理器初始化完成");
    return true;
}

/**
 * @brief 关闭管理器，释放所有资源
 * @details 依次关闭所有控制器，然后保存配置到文件
 */
void LightControllerManager::shutdown()
{
    // 关闭所有控制器
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS; ++i) {
        closeController(i);
    }

    // 保存配置到文件
    if (!configPath_.isEmpty()) {
        saveConfig();
    }

    initialized_ = false;
    LOG_INFO("光源控制器管理器已关闭");
}

// ============================================================================
// 控制器管理实现 - 打开/关闭/状态查询
// ============================================================================

/**
 * @brief 打开指定控制器
 * @param index 控制器索引 (0-7)
 * @return 打开成功返回true
 * @details 执行流程：
 *          1. 验证索引有效性和控制器启用状态
 *          2. 如已打开则先关闭
 *          3. 根据类型创建控制器对象
 *          4. 连接控制器信号到管理器信号
 *          5. 使用配置参数建立连接
 *          6. 恢复保存的亮度设置
 */
bool LightControllerManager::openController(int index)
{
    // 验证索引有效性
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        LOG_ERROR(QString("无效的控制器索引: %1").arg(index));
        return false;
    }

    const LightCtrlConfig& cfg = config_.controllers[index];

    // 检查控制器是否启用
    if (!cfg.used) {
        LOG_WARNING(QString("控制器%1未启用").arg(index + 1));
        return false;
    }

    // 如果已经打开，先关闭
    if (controllers_[index] && controllers_[index]->isConnected()) {
        controllers_[index]->disconnect();
    }

    // 根据类型创建控制器对象（工厂模式）
    controllers_[index] = createController(cfg.type);
    if (!controllers_[index]) {
        LOG_ERROR(QString("创建控制器%1失败").arg(index + 1));
        return false;
    }

    // 连接控制器信号到管理器信号（信号转发）
    // 连接状态变化信号
    connect(controllers_[index].get(), &LightController::connectionChanged,
            [this, index](bool connected) {
                emit controllerConnectionChanged(index, connected);
            });

    // 亮度变化信号
    connect(controllers_[index].get(), &LightController::brightnessChanged,
            [this, index](int channel, int brightness) {
                emit channelBrightnessChanged(index, channel, brightness);
            });

    // 通道状态变化信号
    connect(controllers_[index].get(), &LightController::channelStatusChanged,
            [this, index](int channel, bool enabled) {
                emit channelEnabledChanged(index, channel, enabled);
            });

    // 错误信号
    connect(controllers_[index].get(), &LightController::errorOccurred,
            [this, index](const QString& error) {
                emit errorOccurred(index, error);
            });

    // 构建连接配置
    LightControllerConfig connConfig = buildSerialConfig(cfg);

    // 尝试连接控制器
    if (!controllers_[index]->connect(connConfig)) {
        QString error = controllers_[index]->getLastError();
        LOG_ERROR(QString("打开控制器%1失败: %2").arg(index + 1).arg(error));
        emit errorOccurred(index, error);
        return false;
    }

    LOG_INFO(QString("控制器%1打开成功").arg(index + 1));
    emit controllerConnectionChanged(index, true);

    // 恢复保存的亮度设置（只恢复已开启通道的亮度）
    for (int ch = 0; ch < cfg.maxChannels; ++ch) {
        if (cfg.channels[ch].enabled) {
            controllers_[index]->setBrightness(ch, cfg.channels[ch].brightness);
        }
    }

    return true;
}

/**
 * @brief 关闭指定控制器
 * @param index 控制器索引
 * @details 断开连接并释放控制器对象
 */
void LightControllerManager::closeController(int index)
{
    // 验证索引有效性
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return;
    }

    // 如果控制器存在，断开连接并释放
    if (controllers_[index]) {
        controllers_[index]->disconnect();
        controllers_[index].reset();  // 释放智能指针
        LOG_INFO(QString("控制器%1已关闭").arg(index + 1));
        emit controllerConnectionChanged(index, false);
    }
}

/**
 * @brief 检查控制器是否已打开
 * @param index 控制器索引
 * @return 已连接返回true
 */
bool LightControllerManager::isControllerOpen(int index) const
{
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    // 检查控制器对象存在且已连接
    return controllers_[index] && controllers_[index]->isConnected();
}

/**
 * @brief 获取控制器对象指针
 * @param index 控制器索引
 * @return 控制器对象指针，无效时返回nullptr
 */
LightController* LightControllerManager::getController(int index)
{
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return nullptr;
    }

    return controllers_[index].get();
}

// ============================================================================
// 亮度控制实现 - 通道亮度和开关控制
// ============================================================================

/**
 * @brief 设置通道亮度
 * @param ctrlIndex 控制器索引
 * @param channelIndex 通道索引
 * @param brightness 亮度值
 * @return 设置成功返回true
 * @details 同时更新配置缓存和实际硬件（如已连接）
 */
bool LightControllerManager::setChannelBrightness(int ctrlIndex, int channelIndex, int brightness)
{
    // 验证控制器索引
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    // 验证通道索引
    if (channelIndex < 0 || channelIndex >= MAX_LIGHT_CHANNELS) {
        return false;
    }

    // 更新配置缓存（确保下次打开时能恢复）
    config_.controllers[ctrlIndex].channels[channelIndex].brightness = brightness;

    // 如果控制器已连接，实时更新硬件
    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->setBrightness(channelIndex, brightness);
    }

    return true;
}

/**
 * @brief 获取通道亮度
 * @param ctrlIndex 控制器索引
 * @param channelIndex 通道索引
 * @return 亮度值，参数无效时返回0
 */
int LightControllerManager::getChannelBrightness(int ctrlIndex, int channelIndex) const
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return 0;
    }

    if (channelIndex < 0 || channelIndex >= MAX_LIGHT_CHANNELS) {
        return 0;
    }

    return config_.controllers[ctrlIndex].channels[channelIndex].brightness;
}

/**
 * @brief 设置通道开关状态
 * @param ctrlIndex 控制器索引
 * @param channelIndex 通道索引
 * @param enabled 开关状态
 * @return 设置成功返回true
 */
bool LightControllerManager::setChannelEnabled(int ctrlIndex, int channelIndex, bool enabled)
{
    // 验证索引
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    if (channelIndex < 0 || channelIndex >= MAX_LIGHT_CHANNELS) {
        return false;
    }

    // 更新配置缓存
    config_.controllers[ctrlIndex].channels[channelIndex].enabled = enabled;

    // 如果控制器已连接，实时更新硬件
    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->setChannelEnabled(channelIndex, enabled);
    }

    return true;
}

/**
 * @brief 获取通道开关状态
 * @param ctrlIndex 控制器索引
 * @param channelIndex 通道索引
 * @return 开关状态
 */
bool LightControllerManager::isChannelEnabled(int ctrlIndex, int channelIndex) const
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    if (channelIndex < 0 || channelIndex >= MAX_LIGHT_CHANNELS) {
        return false;
    }

    return config_.controllers[ctrlIndex].channels[channelIndex].enabled;
}

/**
 * @brief 开启控制器的所有通道
 * @param ctrlIndex 控制器索引
 * @return 操作成功返回true
 */
bool LightControllerManager::enableAllChannels(int ctrlIndex)
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    // 获取此控制器的最大通道数
    const int maxCh = config_.controllers[ctrlIndex].maxChannels;

    // 更新所有通道的配置
    for (int ch = 0; ch < maxCh; ++ch) {
        config_.controllers[ctrlIndex].channels[ch].enabled = true;
    }

    // 如果已连接，调用控制器的批量开启功能
    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->enableAll();
    }

    return true;
}

/**
 * @brief 关闭控制器的所有通道
 * @param ctrlIndex 控制器索引
 * @return 操作成功返回true
 */
bool LightControllerManager::disableAllChannels(int ctrlIndex)
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    // 获取此控制器的最大通道数
    const int maxCh = config_.controllers[ctrlIndex].maxChannels;

    // 更新所有通道的配置
    for (int ch = 0; ch < maxCh; ++ch) {
        config_.controllers[ctrlIndex].channels[ch].enabled = false;
    }

    // 如果已连接，调用控制器的批量关闭功能
    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->disableAll();
    }

    return true;
}

// ============================================================================
// 配置管理实现 - 配置访问和文件操作
// ============================================================================

/**
 * @brief 获取指定控制器的配置（可修改版本）
 * @param index 控制器索引
 * @return 控制器配置的引用
 * @note 索引无效时返回静态空对象，避免空指针
 */
LightCtrlConfig& LightControllerManager::controllerConfig(int index)
{
    static LightCtrlConfig dummy;  // 静态空对象，用于无效索引
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return dummy;
    }
    return config_.controllers[index];
}

/**
 * @brief 获取指定控制器的配置（只读版本）
 * @param index 控制器索引
 * @return 控制器配置的常量引用
 */
const LightCtrlConfig& LightControllerManager::controllerConfig(int index) const
{
    static LightCtrlConfig dummy;
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return dummy;
    }
    return config_.controllers[index];
}

/**
 * @brief 保存配置到文件
 * @return 保存成功返回true
 */
bool LightControllerManager::saveConfig()
{
    // 检查配置文件路径
    if (configPath_.isEmpty()) {
        LOG_WARNING("未指定配置文件路径");
        return false;
    }

    // 确保配置文件所在目录存在
    QDir dir = QFileInfo(configPath_).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");  // 创建目录
    }

    return config_.save(configPath_);
}

/**
 * @brief 重新从文件加载配置
 * @return 加载成功返回true
 */
bool LightControllerManager::reloadConfig()
{
    if (configPath_.isEmpty()) {
        return false;
    }

    return config_.load(configPath_);
}

// ============================================================================
// 静态辅助函数实现
// ============================================================================

/**
 * @brief 获取指定控制器类型的亮度范围
 * @param type 控制器类型
 * @param minVal [out] 最小亮度值
 * @param maxVal [out] 最大亮度值
 * @details 不同品牌的控制器可能有不同的亮度范围：
 *          - 大多数控制器：0-255
 *          - 力科胜(LKS)：0-1000（高精度控制）
 */
void LightControllerManager::getBrightnessRange(LightControllerType type, int& minVal, int& maxVal)
{
    switch (type) {
        case LightControllerType::OPT:
        case LightControllerType::OPT2:
            minVal = 0;
            maxVal = 255;
            break;
        case LightControllerType::CCS:
            minVal = 0;
            maxVal = 255;
            break;
        case LightControllerType::KangShiDa:
        case LightControllerType::KangShiDa2:
            minVal = 0;
            maxVal = 255;
            break;
        case LightControllerType::LKS:
            // 力科胜支持更高精度的亮度控制
            minVal = 0;
            maxVal = 1000;
            break;
        default:
            // 默认使用标准范围
            minVal = 0;
            maxVal = 255;
            break;
    }
}

/**
 * @brief 获取控制器类型的中文名称
 * @param type 控制器类型
 * @return 类型对应的中文名称
 */
QString LightControllerManager::getTypeName(LightControllerType type)
{
    switch (type) {
        case LightControllerType::KangShiDa:    return "康视达";
        case LightControllerType::KangShiDa2:   return "康视达2";
        case LightControllerType::RuiShi:       return "锐视";
        case LightControllerType::RuiShi2:      return "锐视2";
        case LightControllerType::FengShi:      return "风视";
        case LightControllerType::FengShi2:     return "风视2";
        case LightControllerType::FengShi3:     return "风视3";
        case LightControllerType::OPT:          return "奥普特";
        case LightControllerType::OPT2:         return "奥普特2";
        case LightControllerType::CCS:          return "CCS";
        case LightControllerType::LKS:          return "力科胜";
        default:                                return "未知";
    }
}

// ============================================================================
// 私有辅助函数实现
// ============================================================================

/**
 * @brief 根据类型创建对应的控制器对象
 * @param type 控制器类型
 * @return 创建的控制器对象智能指针
 * @details 使用工厂模式，根据控制器类型创建对应的子类实例。
 *          不同类型的控制器有不同的通信协议实现。
 */
std::unique_ptr<LightController> LightControllerManager::createController(LightControllerType type)
{
    switch (type) {
        case LightControllerType::OPT:
        case LightControllerType::OPT2:
            // 奥普特系列使用专用驱动
            return std::make_unique<OPTLightController>();

        case LightControllerType::CCS:
            // CCS系列使用专用驱动
            return std::make_unique<CCSLightController>();

        default:
            // 其他类型使用基类（通用协议）
            return std::make_unique<LightController>();
    }
}

/**
 * @brief 根据配置构建串口连接参数
 * @param cfg 控制器配置
 * @return 构建好的连接配置结构
 * @details 将LightCtrlConfig转换为LightControllerConfig：
 *          - 串口模式：设置串口名和波特率
 *          - 网络模式：暂不支持，使用默认串口参数
 *          - 通用参数：8数据位、1停止位、无校验、1000ms超时
 */
LightControllerConfig LightControllerManager::buildSerialConfig(const LightCtrlConfig& cfg)
{
    LightControllerConfig config;

    if (cfg.commType == LightCommType::Serial) {
        // 串口通信模式：构建串口名（COM1, COM2, ...）
        config.portName = QString("COM%1").arg(cfg.serialPort + 1);
        config.baudRate = cfg.baudRate;
    } else {
        // 网络模式 - 暂时不支持，使用默认串口参数
        // TODO: 实现网络通信支持
        config.portName = QString("COM%1").arg(cfg.serialPort + 1);
        config.baudRate = cfg.baudRate;
    }

    // 设置通用参数
    config.channelCount = cfg.maxChannels;  // 通道数
    config.dataBits = 8;                    // 8数据位
    config.stopBits = 1;                    // 1停止位
    config.parity = 0;                      // 无校验
    config.timeout = 1000;                  // 1秒超时

    return config;
}

} // namespace Comm
} // namespace VisionForge
