/**
 * @file LightController.cpp
 * @brief 光源控制器实现
 */

#include "comm/LightController.h"
#include "base/Logger.h"
#include <QSerialPortInfo>
#include <QThread>
#include <algorithm>

namespace VisionForge {
namespace Comm {

// ========== LightController ==========

LightController::LightController(QObject* parent)
    : QObject(parent)
    , serialPort_(new QSerialPort(this))
    , channelCount_(4)
    , triggerMode_(LightTriggerMode::Continuous)
{
    // 初始化通道
    for (int i = 0; i < channelCount_; ++i) {
        channels_.emplace_back(i, 0, false);
    }
}

LightController::~LightController()
{
    disconnect();
}

bool LightController::connect(const LightControllerConfig& config)
{
    QMutexLocker locker(&mutex_);

    if (isConnected()) {
        disconnect();
    }

    config_ = config;
    channelCount_ = config.channelCount;

    // 重新初始化通道
    channels_.clear();
    for (int i = 0; i < channelCount_; ++i) {
        channels_.emplace_back(i, 0, false);
    }

    // 配置串口
    serialPort_->setPortName(config.portName);
    serialPort_->setBaudRate(config.baudRate);

    // 设置数据位
    switch (config.dataBits) {
    case 5: serialPort_->setDataBits(QSerialPort::Data5); break;
    case 6: serialPort_->setDataBits(QSerialPort::Data6); break;
    case 7: serialPort_->setDataBits(QSerialPort::Data7); break;
    case 8:
    default: serialPort_->setDataBits(QSerialPort::Data8); break;
    }

    // 设置校验位
    switch (config.parity) {
    case 1: serialPort_->setParity(QSerialPort::OddParity); break;
    case 2: serialPort_->setParity(QSerialPort::EvenParity); break;
    case 0:
    default: serialPort_->setParity(QSerialPort::NoParity); break;
    }

    // 设置停止位
    switch (config.stopBits) {
    case 2: serialPort_->setStopBits(QSerialPort::TwoStop); break;
    case 1:
    default: serialPort_->setStopBits(QSerialPort::OneStop); break;
    }

    // 打开串口
    if (!serialPort_->open(QIODevice::ReadWrite)) {
        lastError_ = QString("打开串口失败: %1 - %2")
                    .arg(config.portName)
                    .arg(serialPort_->errorString());
        LOG_ERROR(lastError_);
        emit errorOccurred(lastError_);
        return false;
    }

    // 清空缓冲区
    serialPort_->clear();

    LOG_INFO(QString("光源控制器连接成功: %1 @ %2bps, %3通道")
            .arg(config.portName)
            .arg(config.baudRate)
            .arg(channelCount_));

    emit connectionChanged(true);
    return true;
}

void LightController::disconnect()
{
    QMutexLocker locker(&mutex_);

    if (serialPort_->isOpen()) {
        // 关闭所有通道
        for (auto& ch : channels_) {
            ch.enabled = false;
            ch.brightness = 0;
        }

        serialPort_->close();
        LOG_INFO("光源控制器断开连接");
        emit connectionChanged(false);
    }
}

bool LightController::isConnected() const
{
    return serialPort_->isOpen();
}

bool LightController::setBrightness(int channel, int brightness)
{
    if (!isConnected()) {
        lastError_ = "未连接";
        return false;
    }

    if (channel < 0 || channel >= channelCount_) {
        lastError_ = QString("无效的通道号: %1").arg(channel);
        return false;
    }

    brightness = std::clamp(brightness, 0, 255);

    QByteArray command = buildSetBrightnessCommand(channel, brightness);
    QByteArray response = sendCommand(command, 0);

    // 更新本地状态
    channels_[channel].brightness = brightness;
    channels_[channel].enabled = (brightness > 0);

    LOG_DEBUG(QString("设置光源通道%1亮度: %2").arg(channel).arg(brightness));
    emit brightnessChanged(channel, brightness);

    return true;
}

int LightController::getBrightness(int channel) const
{
    if (channel < 0 || channel >= channelCount_) {
        return 0;
    }
    return channels_[channel].brightness;
}

bool LightController::setAllBrightness(int brightness)
{
    bool success = true;
    brightness = std::clamp(brightness, 0, 255);

    for (int i = 0; i < channelCount_; ++i) {
        if (!setBrightness(i, brightness)) {
            success = false;
        }
    }

    return success;
}

bool LightController::setMultiBrightness(const std::vector<int>& brightnesses)
{
    bool success = true;
    int count = std::min(static_cast<int>(brightnesses.size()), channelCount_);

    for (int i = 0; i < count; ++i) {
        if (!setBrightness(i, brightnesses[i])) {
            success = false;
        }
    }

    return success;
}

bool LightController::setChannelEnabled(int channel, bool enabled)
{
    if (!isConnected()) {
        lastError_ = "未连接";
        return false;
    }

    if (channel < 0 || channel >= channelCount_) {
        lastError_ = QString("无效的通道号: %1").arg(channel);
        return false;
    }

    // 使能通道时保持原亮度，禁用时设为0
    int brightness = enabled ? channels_[channel].brightness : 0;
    if (enabled && brightness == 0) {
        brightness = 128;  // 默认中等亮度
    }

    bool result = setBrightness(channel, brightness);

    channels_[channel].enabled = enabled;
    emit channelStatusChanged(channel, enabled);

    return result;
}

bool LightController::enableAll()
{
    if (!isConnected()) {
        lastError_ = "未连接";
        return false;
    }

    QByteArray command = buildEnableAllCommand();
    sendCommand(command, 0);

    for (auto& ch : channels_) {
        ch.enabled = true;
        if (ch.brightness == 0) {
            ch.brightness = 128;
        }
        emit channelStatusChanged(ch.channelId, true);
    }

    LOG_DEBUG("启用所有光源通道");
    return true;
}

bool LightController::disableAll()
{
    if (!isConnected()) {
        lastError_ = "未连接";
        return false;
    }

    QByteArray command = buildDisableAllCommand();
    sendCommand(command, 0);

    for (auto& ch : channels_) {
        ch.enabled = false;
        emit channelStatusChanged(ch.channelId, false);
    }

    LOG_DEBUG("禁用所有光源通道");
    return true;
}

LightChannel LightController::getChannelStatus(int channel) const
{
    if (channel < 0 || channel >= channelCount_) {
        return LightChannel();
    }
    return channels_[channel];
}

std::vector<LightChannel> LightController::getAllChannelStatus() const
{
    return channels_;
}

bool LightController::setTriggerMode(LightTriggerMode mode)
{
    if (!isConnected()) {
        lastError_ = "未连接";
        return false;
    }

    QByteArray command = buildSetTriggerModeCommand(mode);
    sendCommand(command, 0);

    triggerMode_ = mode;

    LOG_DEBUG(QString("设置光源触发模式: %1")
             .arg(mode == LightTriggerMode::Continuous ? "常亮" :
                  mode == LightTriggerMode::Flash ? "闪光" : "频闪"));

    return true;
}

bool LightController::triggerFlash(int durationMs)
{
    if (!isConnected()) {
        lastError_ = "未连接";
        return false;
    }

    QByteArray command = buildTriggerFlashCommand(durationMs);
    sendCommand(command, 0);

    LOG_DEBUG(QString("触发闪光: %1ms").arg(durationMs));
    return true;
}

QStringList LightController::availablePorts()
{
    QStringList ports;
    const auto portInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : portInfos) {
        ports.append(info.portName());
    }
    return ports;
}

// ========== 默认协议命令构建 ==========

QByteArray LightController::buildSetBrightnessCommand(int channel, int brightness)
{
    // 默认协议: "CH<通道>B<亮度>\r\n"
    return QString("CH%1B%2\r\n")
           .arg(channel)
           .arg(brightness, 3, 10, QChar('0'))
           .toLatin1();
}

QByteArray LightController::buildEnableAllCommand()
{
    return QByteArray("ON\r\n");
}

QByteArray LightController::buildDisableAllCommand()
{
    return QByteArray("OFF\r\n");
}

QByteArray LightController::buildSetTriggerModeCommand(LightTriggerMode mode)
{
    switch (mode) {
    case LightTriggerMode::Continuous:
        return QByteArray("MODE:CONT\r\n");
    case LightTriggerMode::Flash:
        return QByteArray("MODE:FLASH\r\n");
    case LightTriggerMode::Strobe:
        return QByteArray("MODE:STROBE\r\n");
    }
    return QByteArray();
}

QByteArray LightController::buildTriggerFlashCommand(int durationMs)
{
    return QString("FLASH:%1\r\n").arg(durationMs).toLatin1();
}

QByteArray LightController::sendCommand(const QByteArray& command, int expectedResponseSize)
{
    QMutexLocker locker(&mutex_);

    if (!serialPort_->isOpen()) {
        return QByteArray();
    }

    // 清空接收缓冲区
    serialPort_->clear();

    // 发送命令
    qint64 written = serialPort_->write(command);
    if (written != command.size()) {
        lastError_ = "发送命令失败";
        LOG_ERROR(lastError_);
        return QByteArray();
    }

    // 等待数据发送完成
    if (!serialPort_->waitForBytesWritten(config_.timeout)) {
        lastError_ = "发送超时";
        LOG_ERROR(lastError_);
        return QByteArray();
    }

    // 如果不需要响应，直接返回
    if (expectedResponseSize <= 0) {
        return QByteArray();
    }

    // 等待响应
    QByteArray response;
    int elapsed = 0;
    const int interval = 10;

    while (response.size() < expectedResponseSize && elapsed < config_.timeout) {
        if (serialPort_->waitForReadyRead(interval)) {
            response.append(serialPort_->readAll());
        }
        elapsed += interval;
    }

    return response;
}

bool LightController::parseResponse(const QByteArray& response)
{
    // 默认实现：检查是否包含"OK"
    return response.contains("OK");
}

// ========== OPTLightController ==========

OPTLightController::OPTLightController(QObject* parent)
    : LightController(parent)
{
}

QByteArray OPTLightController::buildSetBrightnessCommand(int channel, int brightness)
{
    // OPT协议: "#<通道><亮度>\r" (通道A-D, 亮度000-255)
    char channelChar = 'A' + channel;
    return QString("#%1%2\r")
           .arg(channelChar)
           .arg(brightness, 3, 10, QChar('0'))
           .toLatin1();
}

QByteArray OPTLightController::buildEnableAllCommand()
{
    // OPT: 设置所有通道为之前的亮度
    QByteArray commands;
    for (int i = 0; i < channelCount_; ++i) {
        int bri = channels_[i].brightness;
        if (bri == 0) bri = 128;
        commands.append(buildSetBrightnessCommand(i, bri));
    }
    return commands;
}

QByteArray OPTLightController::buildDisableAllCommand()
{
    // OPT: 设置所有通道亮度为0
    QByteArray commands;
    for (int i = 0; i < channelCount_; ++i) {
        commands.append(buildSetBrightnessCommand(i, 0));
    }
    return commands;
}

// ========== CCSLightController ==========

CCSLightController::CCSLightController(QObject* parent)
    : LightController(parent)
{
}

QByteArray CCSLightController::buildSetBrightnessCommand(int channel, int brightness)
{
    // CCS协议示例: "L<通道><亮度>\r\n"
    return QString("L%1%2\r\n")
           .arg(channel)
           .arg(brightness, 3, 10, QChar('0'))
           .toLatin1();
}

} // namespace Comm
} // namespace VisionForge
