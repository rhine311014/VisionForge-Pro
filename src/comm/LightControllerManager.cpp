/**
 * @file LightControllerManager.cpp
 * @brief 光源控制器管理器实现
 */

#include "comm/LightControllerManager.h"
#include "base/Logger.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

namespace VisionForge {
namespace Comm {

// ========== LightChannelConfig ==========

QJsonObject LightChannelConfig::toJson() const
{
    QJsonObject json;
    json["name"] = name;
    json["brightness"] = brightness;
    json["enabled"] = enabled;
    return json;
}

void LightChannelConfig::fromJson(const QJsonObject& json)
{
    name = json.value("name").toString();
    brightness = json.value("brightness").toInt(0);
    enabled = json.value("enabled").toBool(false);
}

// ========== LightCtrlConfig ==========

QJsonObject LightCtrlConfig::toJson() const
{
    QJsonObject json;
    json["name"] = name;
    json["used"] = used;
    json["type"] = static_cast<int>(type);
    json["commType"] = static_cast<int>(commType);
    json["serialPort"] = serialPort;
    json["baudRate"] = baudRate;
    json["ipAddress"] = ipAddress;
    json["port"] = port;
    json["maxChannels"] = maxChannels;

    QJsonArray channelArray;
    for (int i = 0; i < MAX_LIGHT_CHANNELS; ++i) {
        channelArray.append(channels[i].toJson());
    }
    json["channels"] = channelArray;

    return json;
}

void LightCtrlConfig::fromJson(const QJsonObject& json)
{
    name = json.value("name").toString();
    used = json.value("used").toBool(false);
    type = static_cast<LightControllerType>(json.value("type").toInt(30));
    commType = static_cast<LightCommType>(json.value("commType").toInt(0));
    serialPort = json.value("serialPort").toInt(0);
    baudRate = json.value("baudRate").toInt(9600);
    ipAddress = json.value("ipAddress").toString("192.168.1.100");
    port = json.value("port").toInt(5000);
    maxChannels = json.value("maxChannels").toInt(8);

    QJsonArray channelArray = json.value("channels").toArray();
    for (int i = 0; i < MAX_LIGHT_CHANNELS && i < channelArray.size(); ++i) {
        channels[i].fromJson(channelArray[i].toObject());
    }
}

// ========== SystemLightConfig ==========

QJsonObject SystemLightConfig::toJson() const
{
    QJsonObject json;
    QJsonArray ctrlArray;
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS; ++i) {
        ctrlArray.append(controllers[i].toJson());
    }
    json["controllers"] = ctrlArray;
    return json;
}

void SystemLightConfig::fromJson(const QJsonObject& json)
{
    QJsonArray ctrlArray = json.value("controllers").toArray();
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS && i < ctrlArray.size(); ++i) {
        controllers[i].fromJson(ctrlArray[i].toObject());
    }
}

bool SystemLightConfig::load(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_WARNING(QString("无法打开光源配置文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("光源配置文件格式错误: %1").arg(filePath));
        return false;
    }

    fromJson(doc.object());
    LOG_INFO(QString("光源配置加载成功: %1").arg(filePath));
    return true;
}

bool SystemLightConfig::save(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法保存光源配置文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("光源配置保存成功: %1").arg(filePath));
    return true;
}

// ========== LightControllerManager ==========

LightControllerManager& LightControllerManager::instance()
{
    static LightControllerManager instance;
    return instance;
}

LightControllerManager::LightControllerManager()
    : QObject(nullptr)
{
    // 初始化默认配置
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS; ++i) {
        config_.controllers[i].name = QString("光源控制器%1").arg(i + 1);
        for (int ch = 0; ch < MAX_LIGHT_CHANNELS; ++ch) {
            config_.controllers[i].channels[ch].name = QString("通道%1").arg(ch + 1);
        }
    }
}

LightControllerManager::~LightControllerManager()
{
    shutdown();
}

bool LightControllerManager::initialize(const QString& configPath)
{
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

void LightControllerManager::shutdown()
{
    // 关闭所有控制器
    for (int i = 0; i < MAX_LIGHT_CONTROLLERS; ++i) {
        closeController(i);
    }

    // 保存配置
    if (!configPath_.isEmpty()) {
        saveConfig();
    }

    initialized_ = false;
    LOG_INFO("光源控制器管理器已关闭");
}

bool LightControllerManager::openController(int index)
{
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        LOG_ERROR(QString("无效的控制器索引: %1").arg(index));
        return false;
    }

    const LightCtrlConfig& cfg = config_.controllers[index];

    if (!cfg.used) {
        LOG_WARNING(QString("控制器%1未启用").arg(index + 1));
        return false;
    }

    // 如果已经打开，先关闭
    if (controllers_[index] && controllers_[index]->isConnected()) {
        controllers_[index]->disconnect();
    }

    // 创建控制器
    controllers_[index] = createController(cfg.type);
    if (!controllers_[index]) {
        LOG_ERROR(QString("创建控制器%1失败").arg(index + 1));
        return false;
    }

    // 连接信号
    connect(controllers_[index].get(), &LightController::connectionChanged,
            [this, index](bool connected) {
                emit controllerConnectionChanged(index, connected);
            });

    connect(controllers_[index].get(), &LightController::brightnessChanged,
            [this, index](int channel, int brightness) {
                emit channelBrightnessChanged(index, channel, brightness);
            });

    connect(controllers_[index].get(), &LightController::channelStatusChanged,
            [this, index](int channel, bool enabled) {
                emit channelEnabledChanged(index, channel, enabled);
            });

    connect(controllers_[index].get(), &LightController::errorOccurred,
            [this, index](const QString& error) {
                emit errorOccurred(index, error);
            });

    // 构建配置
    LightControllerConfig connConfig = buildSerialConfig(cfg);

    // 连接
    if (!controllers_[index]->connect(connConfig)) {
        QString error = controllers_[index]->getLastError();
        LOG_ERROR(QString("打开控制器%1失败: %2").arg(index + 1).arg(error));
        emit errorOccurred(index, error);
        return false;
    }

    LOG_INFO(QString("控制器%1打开成功").arg(index + 1));
    emit controllerConnectionChanged(index, true);

    // 恢复保存的亮度设置
    for (int ch = 0; ch < cfg.maxChannels; ++ch) {
        if (cfg.channels[ch].enabled) {
            controllers_[index]->setBrightness(ch, cfg.channels[ch].brightness);
        }
    }

    return true;
}

void LightControllerManager::closeController(int index)
{
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return;
    }

    if (controllers_[index]) {
        controllers_[index]->disconnect();
        controllers_[index].reset();
        LOG_INFO(QString("控制器%1已关闭").arg(index + 1));
        emit controllerConnectionChanged(index, false);
    }
}

bool LightControllerManager::isControllerOpen(int index) const
{
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    return controllers_[index] && controllers_[index]->isConnected();
}

LightController* LightControllerManager::getController(int index)
{
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return nullptr;
    }

    return controllers_[index].get();
}

bool LightControllerManager::setChannelBrightness(int ctrlIndex, int channelIndex, int brightness)
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    if (channelIndex < 0 || channelIndex >= MAX_LIGHT_CHANNELS) {
        return false;
    }

    // 更新配置
    config_.controllers[ctrlIndex].channels[channelIndex].brightness = brightness;

    // 如果控制器已打开，实时更新
    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->setBrightness(channelIndex, brightness);
    }

    return true;
}

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

bool LightControllerManager::setChannelEnabled(int ctrlIndex, int channelIndex, bool enabled)
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    if (channelIndex < 0 || channelIndex >= MAX_LIGHT_CHANNELS) {
        return false;
    }

    // 更新配置
    config_.controllers[ctrlIndex].channels[channelIndex].enabled = enabled;

    // 如果控制器已打开，实时更新
    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->setChannelEnabled(channelIndex, enabled);
    }

    return true;
}

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

bool LightControllerManager::enableAllChannels(int ctrlIndex)
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    const int maxCh = config_.controllers[ctrlIndex].maxChannels;
    for (int ch = 0; ch < maxCh; ++ch) {
        config_.controllers[ctrlIndex].channels[ch].enabled = true;
    }

    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->enableAll();
    }

    return true;
}

bool LightControllerManager::disableAllChannels(int ctrlIndex)
{
    if (ctrlIndex < 0 || ctrlIndex >= MAX_LIGHT_CONTROLLERS) {
        return false;
    }

    const int maxCh = config_.controllers[ctrlIndex].maxChannels;
    for (int ch = 0; ch < maxCh; ++ch) {
        config_.controllers[ctrlIndex].channels[ch].enabled = false;
    }

    if (controllers_[ctrlIndex] && controllers_[ctrlIndex]->isConnected()) {
        return controllers_[ctrlIndex]->disableAll();
    }

    return true;
}

LightCtrlConfig& LightControllerManager::controllerConfig(int index)
{
    static LightCtrlConfig dummy;
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return dummy;
    }
    return config_.controllers[index];
}

const LightCtrlConfig& LightControllerManager::controllerConfig(int index) const
{
    static LightCtrlConfig dummy;
    if (index < 0 || index >= MAX_LIGHT_CONTROLLERS) {
        return dummy;
    }
    return config_.controllers[index];
}

bool LightControllerManager::saveConfig()
{
    if (configPath_.isEmpty()) {
        LOG_WARNING("未指定配置文件路径");
        return false;
    }

    // 确保目录存在
    QDir dir = QFileInfo(configPath_).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return config_.save(configPath_);
}

bool LightControllerManager::reloadConfig()
{
    if (configPath_.isEmpty()) {
        return false;
    }

    return config_.load(configPath_);
}

void LightControllerManager::getBrightnessRange(LightControllerType type, int& minVal, int& maxVal)
{
    // 不同类型的控制器可能有不同的亮度范围
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
            minVal = 0;
            maxVal = 1000;
            break;
        default:
            minVal = 0;
            maxVal = 255;
            break;
    }
}

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

std::unique_ptr<LightController> LightControllerManager::createController(LightControllerType type)
{
    switch (type) {
        case LightControllerType::OPT:
        case LightControllerType::OPT2:
            return std::make_unique<OPTLightController>();

        case LightControllerType::CCS:
            return std::make_unique<CCSLightController>();

        default:
            // 默认使用基类
            return std::make_unique<LightController>();
    }
}

LightControllerConfig LightControllerManager::buildSerialConfig(const LightCtrlConfig& cfg)
{
    LightControllerConfig config;

    if (cfg.commType == LightCommType::Serial) {
        config.portName = QString("COM%1").arg(cfg.serialPort + 1);
        config.baudRate = cfg.baudRate;
    } else {
        // 网络模式 - 暂时不支持，使用默认串口
        config.portName = QString("COM%1").arg(cfg.serialPort + 1);
        config.baudRate = cfg.baudRate;
    }

    config.channelCount = cfg.maxChannels;
    config.dataBits = 8;
    config.stopBits = 1;
    config.parity = 0;
    config.timeout = 1000;

    return config;
}

} // namespace Comm
} // namespace VisionForge
