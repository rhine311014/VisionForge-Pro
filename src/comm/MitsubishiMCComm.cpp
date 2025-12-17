/**
 * @file MitsubishiMCComm.cpp
 * @brief 三菱MC协议通信实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "comm/MitsubishiMCComm.h"
#include "base/Logger.h"
#include <QDataStream>

namespace VisionForge {
namespace Comm {

// MC协议命令码
namespace MCCommand {
    constexpr quint16 BatchRead = 0x0401;     // 批量读取
    constexpr quint16 BatchWrite = 0x1401;    // 批量写入
    constexpr quint16 RandomRead = 0x0403;    // 随机读取
    constexpr quint16 RandomWrite = 0x1402;   // 随机写入
}

// 3E帧头标识
constexpr quint16 FRAME_3E_SUBHEADER = 0x5000;  // 请求子头
constexpr quint16 FRAME_3E_RESPONSE = 0xD000;   // 响应子头

MitsubishiMCComm::MitsubishiMCComm(QObject* parent)
    : PLCComm(parent)
    , socket_(new QTcpSocket(this))
    , useTcp_(true)
    , networkNo_(0)
    , pcNo_(0xFF)
    , destModuleIo_(0x03FF)
    , destModuleStation_(0)
    , monitoringTimer_(0x0010)  // 默认16 * 250ms = 4秒
    , defaultWordDevice_(MitsubishiDeviceType::D)
    , defaultBitDevice_(MitsubishiDeviceType::M)
{
    protocol_ = PLCProtocol::MitsubishiMC_TCP;

    // 连接信号
    QObject::connect(socket_, &QTcpSocket::connected, this, [this]() {
        LOG_INFO("三菱MC协议已连接");
        emit connectionChanged(true);
    });

    QObject::connect(socket_, &QTcpSocket::disconnected, this, [this]() {
        LOG_INFO("三菱MC协议已断开");
        emit connectionChanged(false);
    });

    QObject::connect(socket_, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        lastError_ = socket_->errorString();
        LOG_ERROR(QString("三菱MC协议错误: %1").arg(lastError_));
        emit errorOccurred(lastError_);
    });
}

MitsubishiMCComm::~MitsubishiMCComm()
{
    disconnect();
}

bool MitsubishiMCComm::connect(const PLCConfig& config)
{
    QMutexLocker locker(&mutex_);

    config_ = config;

    // 获取MC协议特有参数
    networkNo_ = static_cast<quint8>(config.networkNo);
    pcNo_ = static_cast<quint8>(config.pcNo);
    destModuleIo_ = static_cast<quint16>(config.destModuleIo);
    destModuleStation_ = static_cast<quint8>(config.destModuleStation);

    auto socketConfig = std::dynamic_pointer_cast<SocketConfig>(config.commConfig);
    if (!socketConfig) {
        lastError_ = "无效的网络配置";
        LOG_ERROR(lastError_);
        return false;
    }

    timeout_ = socketConfig->timeout;
    useTcp_ = socketConfig->useTcp;

    LOG_INFO(QString("正在连接三菱PLC: %1:%2").arg(socketConfig->ipAddress).arg(socketConfig->port));

    socket_->connectToHost(socketConfig->ipAddress, socketConfig->port);

    if (!socket_->waitForConnected(timeout_)) {
        lastError_ = QString("连接超时: %1").arg(socket_->errorString());
        LOG_ERROR(lastError_);
        return false;
    }

    LOG_INFO("三菱MC协议连接成功");
    return true;
}

void MitsubishiMCComm::disconnect()
{
    QMutexLocker locker(&mutex_);

    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->disconnectFromHost();
        if (socket_->state() != QAbstractSocket::UnconnectedState) {
            socket_->waitForDisconnected(1000);
        }
    }
}

bool MitsubishiMCComm::isConnected() const
{
    return socket_->state() == QAbstractSocket::ConnectedState;
}

PLCResult MitsubishiMCComm::readRegisters(int address, int count)
{
    // 默认读取D寄存器
    return readDevice(defaultWordDevice_, address, count);
}

PLCResult MitsubishiMCComm::writeRegisters(int address, const std::vector<int>& values)
{
    // 默认写入D寄存器
    return writeDevice(defaultWordDevice_, address, values);
}

PLCResult MitsubishiMCComm::readCoils(int address, int count)
{
    // 默认读取M继电器
    return readBitDevice(defaultBitDevice_, address, count);
}

PLCResult MitsubishiMCComm::writeCoils(int address, const std::vector<bool>& values)
{
    // 默认写入M继电器
    return writeBitDevice(defaultBitDevice_, address, values);
}

PLCResult MitsubishiMCComm::readDevice(MitsubishiDeviceType deviceType, int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 960) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-960)";
        return result;
    }

    // 构建请求数据
    QByteArray requestData;

    // 软元件地址 (3字节) + 软元件代码 (1字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 软元件点数 (2字节，小端序)
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    // 构建3E帧 (字软元件使用子命令0x0000)
    QByteArray request = build3EFrame(MCCommand::BatchRead, MCSubCommand::Word, requestData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, false);
}

PLCResult MitsubishiMCComm::writeDevice(MitsubishiDeviceType deviceType, int address, const std::vector<int>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (values.empty() || values.size() > 960) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-960)";
        return result;
    }

    // 构建请求数据
    QByteArray requestData;

    // 软元件地址 (3字节) + 软元件代码 (1字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 软元件点数 (2字节，小端序)
    quint16 count = static_cast<quint16>(values.size());
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    // 写入数据 (每个值2字节，小端序)
    for (int value : values) {
        requestData.append(static_cast<char>(value & 0xFF));
        requestData.append(static_cast<char>((value >> 8) & 0xFF));
    }

    // 构建3E帧 (字软元件使用子命令0x0000)
    QByteArray request = build3EFrame(MCCommand::BatchWrite, MCSubCommand::Word, requestData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, false);
}

PLCResult MitsubishiMCComm::readBitDevice(MitsubishiDeviceType deviceType, int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 7168) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-7168)";
        return result;
    }

    // 构建请求数据
    QByteArray requestData;

    // 软元件地址 (3字节) + 软元件代码 (1字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 软元件点数 (2字节，小端序)
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    // 构建3E帧 (位软元件使用子命令0x0001)
    QByteArray request = build3EFrame(MCCommand::BatchRead, MCSubCommand::Bit, requestData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, true);
}

PLCResult MitsubishiMCComm::writeBitDevice(MitsubishiDeviceType deviceType, int address, const std::vector<bool>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (values.empty() || values.size() > 7168) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-7168)";
        return result;
    }

    // 构建请求数据
    QByteArray requestData;

    // 软元件地址 (3字节) + 软元件代码 (1字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 软元件点数 (2字节，小端序)
    quint16 count = static_cast<quint16>(values.size());
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    // 位数据 (每个位占4位，2个位占1字节)
    for (size_t i = 0; i < values.size(); i += 2) {
        quint8 byte = values[i] ? 0x10 : 0x00;
        if (i + 1 < values.size()) {
            byte |= values[i + 1] ? 0x01 : 0x00;
        }
        requestData.append(static_cast<char>(byte));
    }

    // 构建3E帧 (位软元件使用子命令0x0001)
    QByteArray request = build3EFrame(MCCommand::BatchWrite, MCSubCommand::Bit, requestData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, true);
}

QByteArray MitsubishiMCComm::sendRequest(const QByteArray& request)
{
    if (!socket_->isValid()) {
        return QByteArray();
    }

    // 调试：打印发送的数据
    LOG_INFO(QString("发送数据(%1字节): %2").arg(request.size()).arg(QString(request.toHex(' ').toUpper())));

    // 发送请求
    socket_->write(request);
    if (!socket_->waitForBytesWritten(timeout_)) {
        LOG_ERROR("发送请求超时");
        return QByteArray();
    }

    // 等待响应 (额外等待时间处理模拟器延迟)
    int waitTime = timeout_ + 2000;  // 额外增加2秒等待
    if (!socket_->waitForReadyRead(waitTime)) {
        LOG_ERROR(QString("接收响应超时(等待%1ms)").arg(waitTime));
        return QByteArray();
    }

    // 读取所有可用数据
    QByteArray response = socket_->readAll();

    // 如果数据不完整，继续等待
    while (response.size() < 11) {
        if (!socket_->waitForReadyRead(timeout_)) {
            break;
        }
        response.append(socket_->readAll());
    }

    if (response.size() < 11) {
        LOG_ERROR(QString("响应头接收不完整，收到%1字节").arg(response.size()));
        return QByteArray();
    }

    // 从响应头获取数据长度
    quint16 dataLength = (static_cast<quint8>(response[7]) | (static_cast<quint8>(response[8]) << 8));
    int totalExpected = 9 + dataLength;  // 头部9字节 + 数据长度

    // 如果还需要更多数据
    while (response.size() < totalExpected) {
        if (!socket_->waitForReadyRead(timeout_)) {
            break;
        }
        response.append(socket_->readAll());
    }

    // 调试：打印接收的数据
    LOG_INFO(QString("接收数据(%1字节): %2").arg(response.size()).arg(QString(response.toHex(' ').toUpper())));

    return response;
}

QByteArray MitsubishiMCComm::build3EFrame(quint16 command, quint16 subCommand, const QByteArray& data)
{
    QByteArray frame;

    // 子头 (2字节) - 大端序: 0x50 0x00
    frame.append(static_cast<char>(0x50));
    frame.append(static_cast<char>(0x00));

    // 网络号 (1字节)
    frame.append(static_cast<char>(networkNo_));

    // PC号 (1字节)
    frame.append(static_cast<char>(pcNo_));

    // 请求目标模块IO号 (2字节) - 小端序
    frame.append(static_cast<char>(destModuleIo_ & 0xFF));
    frame.append(static_cast<char>((destModuleIo_ >> 8) & 0xFF));

    // 请求目标模块站号 (1字节)
    frame.append(static_cast<char>(destModuleStation_));

    // 请求数据长度 (2字节) - 小端序
    // = 监视定时器(2) + 命令(2) + 子命令(2) + 数据
    quint16 requestLength = 2 + 2 + 2 + data.size();
    frame.append(static_cast<char>(requestLength & 0xFF));
    frame.append(static_cast<char>((requestLength >> 8) & 0xFF));

    // 监视定时器 (2字节) - 小端序
    frame.append(static_cast<char>(monitoringTimer_ & 0xFF));
    frame.append(static_cast<char>((monitoringTimer_ >> 8) & 0xFF));

    // 命令 (2字节) - 小端序
    frame.append(static_cast<char>(command & 0xFF));
    frame.append(static_cast<char>((command >> 8) & 0xFF));

    // 子命令 (2字节) - 小端序
    frame.append(static_cast<char>(subCommand & 0xFF));
    frame.append(static_cast<char>((subCommand >> 8) & 0xFF));

    // 数据部分
    frame.append(data);

    return frame;
}

PLCResult MitsubishiMCComm::parseResponse(const QByteArray& response, bool isBitDevice)
{
    PLCResult result;

    if (response.size() < 11) {
        result.status = CommStatus::Error;
        result.errorMessage = "响应数据不完整";
        return result;
    }

    // 检查子头 (大端序: 0xD0 0x00)
    if (static_cast<quint8>(response[0]) != 0xD0 || static_cast<quint8>(response[1]) != 0x00) {
        result.status = CommStatus::Error;
        result.errorMessage = QString("无效的响应子头: 0x%1%2")
            .arg(static_cast<quint8>(response[0]), 2, 16, QChar('0'))
            .arg(static_cast<quint8>(response[1]), 2, 16, QChar('0'));
        return result;
    }

    // 获取结束代码 (偏移9-10)
    quint16 endCode = static_cast<quint8>(response[9]) | (static_cast<quint8>(response[10]) << 8);

    if (endCode != 0) {
        result.status = CommStatus::Error;
        result.errorMessage = QString("PLC错误代码: 0x%1").arg(endCode, 4, 16, QChar('0'));

        // 常见错误代码解释
        switch (endCode) {
        case 0xC050: result.errorMessage += " (软元件指定错误)"; break;
        case 0xC051: result.errorMessage += " (软元件数量超出范围)"; break;
        case 0xC052: result.errorMessage += " (软元件范围超出)"; break;
        case 0xC053: result.errorMessage += " (软元件数量为0)"; break;
        case 0xC054: result.errorMessage += " (软元件编号超出范围)"; break;
        case 0xC056: result.errorMessage += " (软元件指定不正确)"; break;
        case 0xC059: result.errorMessage += " (无法执行命令)"; break;
        case 0xC05B: result.errorMessage += " (CPU模块错误)"; break;
        case 0xC05C: result.errorMessage += " (CPU模块繁忙)"; break;
        case 0xC061: result.errorMessage += " (请求数据长度错误)"; break;
        }

        return result;
    }

    result.status = CommStatus::Success;

    // 解析数据 (从偏移11开始)
    if (response.size() > 11) {
        if (isBitDevice) {
            // 位数据解析 (每个字节包含2个位)
            for (int i = 11; i < response.size(); ++i) {
                quint8 byte = static_cast<quint8>(response[i]);
                result.boolValues.push_back((byte & 0x10) != 0);
                result.boolValues.push_back((byte & 0x01) != 0);
            }
        } else {
            // 字数据解析 (每2字节一个值，小端序)
            for (int i = 11; i + 1 < response.size(); i += 2) {
                quint16 value = static_cast<quint8>(response[i]) |
                                (static_cast<quint8>(response[i + 1]) << 8);
                result.intValues.push_back(static_cast<int>(value));
            }
        }
    }

    return result;
}

quint8 MitsubishiMCComm::getDeviceCode(MitsubishiDeviceType deviceType) const
{
    switch (deviceType) {
    case MitsubishiDeviceType::D:  return MCDeviceCode::D;
    case MitsubishiDeviceType::M:  return MCDeviceCode::M;
    case MitsubishiDeviceType::X:  return MCDeviceCode::X;
    case MitsubishiDeviceType::Y:  return MCDeviceCode::Y;
    case MitsubishiDeviceType::W:  return MCDeviceCode::W;
    case MitsubishiDeviceType::B:  return MCDeviceCode::B;
    case MitsubishiDeviceType::R:  return MCDeviceCode::R;
    case MitsubishiDeviceType::ZR: return MCDeviceCode::ZR;
    case MitsubishiDeviceType::T:  return MCDeviceCode::TN;
    case MitsubishiDeviceType::C:  return MCDeviceCode::CN;
    case MitsubishiDeviceType::ST: return MCDeviceCode::STN;
    default: return MCDeviceCode::D;
    }
}

bool MitsubishiMCComm::isBitDevice(MitsubishiDeviceType deviceType) const
{
    switch (deviceType) {
    case MitsubishiDeviceType::M:
    case MitsubishiDeviceType::X:
    case MitsubishiDeviceType::Y:
    case MitsubishiDeviceType::B:
        return true;
    default:
        return false;
    }
}

QByteArray MitsubishiMCComm::buildDeviceAddress(MitsubishiDeviceType deviceType, int address)
{
    QByteArray data;

    // 软元件地址 (3字节，小端序)
    data.append(static_cast<char>(address & 0xFF));
    data.append(static_cast<char>((address >> 8) & 0xFF));
    data.append(static_cast<char>((address >> 16) & 0xFF));

    // 软元件代码 (1字节)
    data.append(static_cast<char>(getDeviceCode(deviceType)));

    return data;
}

} // namespace Comm
} // namespace VisionForge
