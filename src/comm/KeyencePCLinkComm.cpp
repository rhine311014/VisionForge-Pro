/**
 * @file KeyencePCLinkComm.cpp
 * @brief 基恩士PC-LINK上位链路协议通信实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "comm/KeyencePCLinkComm.h"
#include "base/Logger.h"
#include <QRegularExpression>

namespace VisionForge {
namespace Comm {

// PC-LINK协议命令
namespace PCLinkCommand {
    const QString READ = "RD";        // 读取命令
    const QString WRITE = "WR";       // 写入命令
    const QString READ_CONT = "RDS";  // 连续读取
    const QString WRITE_CONT = "WRS"; // 连续写入
}

// 命令终止符
const char CMD_TERMINATOR = '\r';
const QString RESPONSE_TERMINATOR = "\r\n";

KeyencePCLinkComm::KeyencePCLinkComm(QObject* parent)
    : PLCComm(parent)
    , socket_(new QTcpSocket(this))
    , defaultWordDevice_(KeyenceDeviceType::DM)
    , defaultBitDevice_(KeyenceDeviceType::MR)
{
    protocol_ = PLCProtocol::KeyencePCLink;

    // 连接信号
    QObject::connect(socket_, &QTcpSocket::connected, this, [this]() {
        LOG_INFO("基恩士PC-LINK已连接");
        emit connectionChanged(true);
    });

    QObject::connect(socket_, &QTcpSocket::disconnected, this, [this]() {
        LOG_INFO("基恩士PC-LINK已断开");
        emit connectionChanged(false);
    });

    QObject::connect(socket_, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        lastError_ = socket_->errorString();
        LOG_ERROR(QString("基恩士PC-LINK错误: %1").arg(lastError_));
        emit errorOccurred(lastError_);
    });
}

KeyencePCLinkComm::~KeyencePCLinkComm()
{
    disconnect();
}

bool KeyencePCLinkComm::connect(const PLCConfig& config)
{
    QMutexLocker locker(&mutex_);

    config_ = config;

    auto socketConfig = std::dynamic_pointer_cast<SocketConfig>(config.commConfig);
    if (!socketConfig) {
        lastError_ = "无效的网络配置";
        LOG_ERROR(lastError_);
        return false;
    }

    timeout_ = socketConfig->timeout;

    LOG_INFO(QString("正在连接基恩士PLC: %1:%2").arg(socketConfig->ipAddress).arg(socketConfig->port));

    socket_->connectToHost(socketConfig->ipAddress, socketConfig->port);

    if (!socket_->waitForConnected(timeout_)) {
        lastError_ = QString("连接超时: %1").arg(socket_->errorString());
        LOG_ERROR(lastError_);
        return false;
    }

    LOG_INFO("基恩士PC-LINK连接成功");
    return true;
}

void KeyencePCLinkComm::disconnect()
{
    QMutexLocker locker(&mutex_);

    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->disconnectFromHost();
        if (socket_->state() != QAbstractSocket::UnconnectedState) {
            socket_->waitForDisconnected(1000);
        }
    }
}

bool KeyencePCLinkComm::isConnected() const
{
    return socket_->state() == QAbstractSocket::ConnectedState;
}

PLCResult KeyencePCLinkComm::readRegisters(int address, int count)
{
    // 默认读取DM寄存器
    return readDevice(defaultWordDevice_, address, count);
}

PLCResult KeyencePCLinkComm::writeRegisters(int address, const std::vector<int>& values)
{
    // 默认写入DM寄存器
    return writeDevice(defaultWordDevice_, address, values);
}

PLCResult KeyencePCLinkComm::readCoils(int address, int count)
{
    // 默认读取MR继电器
    return readBitDevice(defaultBitDevice_, address, count);
}

PLCResult KeyencePCLinkComm::writeCoils(int address, const std::vector<bool>& values)
{
    // 默认写入MR继电器
    return writeBitDevice(defaultBitDevice_, address, values);
}

PLCResult KeyencePCLinkComm::readDevice(KeyenceDeviceType deviceType, int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 256) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-256)";
        return result;
    }

    // 构建读取命令: RDS DM00000.H 0010\r
    // 格式: RDS <设备><5位地址>.H <4位数量>
    QString deviceCode = getDeviceCode(deviceType);
    QString command = QString("%1 %2%3.H %4%5")
        .arg(PCLinkCommand::READ_CONT)
        .arg(deviceCode)
        .arg(address, 5, 10, QChar('0'))
        .arg(count, 4, 10, QChar('0'))
        .arg(CMD_TERMINATOR);

    QString response = sendCommand(command);
    return parseResponse(response, false);
}

PLCResult KeyencePCLinkComm::writeDevice(KeyenceDeviceType deviceType, int address, const std::vector<int>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (values.empty() || values.size() > 256) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-256)";
        return result;
    }

    // 构建写入命令: WRS DM00000.H 0010 xxxx xxxx...\r
    // 格式: WRS <设备><5位地址>.H <4位数量> <4位hex值>...
    QString deviceCode = getDeviceCode(deviceType);
    QString command = QString("%1 %2%3.H %4")
        .arg(PCLinkCommand::WRITE_CONT)
        .arg(deviceCode)
        .arg(address, 5, 10, QChar('0'))
        .arg(static_cast<int>(values.size()), 4, 10, QChar('0'));

    // 添加数据值 (4位十六进制)
    for (int value : values) {
        command += QString(" %1").arg(value & 0xFFFF, 4, 16, QChar('0')).toUpper();
    }
    command += CMD_TERMINATOR;

    QString response = sendCommand(command);

    QString errorMsg;
    if (checkResponseOK(response, errorMsg)) {
        result.status = CommStatus::Success;
    } else {
        result.status = CommStatus::Error;
        result.errorMessage = errorMsg;
    }

    return result;
}

PLCResult KeyencePCLinkComm::readBitDevice(KeyenceDeviceType deviceType, int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 256) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-256)";
        return result;
    }

    // 对于位软元件，逐个读取或使用连续读取
    QString deviceCode = getDeviceCode(deviceType);

    if (count == 1) {
        // 单点读取: RD MR0\r
        QString command = QString("%1 %2%3%4")
            .arg(PCLinkCommand::READ)
            .arg(deviceCode)
            .arg(address)
            .arg(CMD_TERMINATOR);

        QString response = sendCommand(command);
        return parseResponse(response, true);
    } else {
        // 连续读取: RDS MR0 10\r
        QString command = QString("%1 %2%3 %4%5")
            .arg(PCLinkCommand::READ_CONT)
            .arg(deviceCode)
            .arg(address)
            .arg(count)
            .arg(CMD_TERMINATOR);

        QString response = sendCommand(command);
        return parseResponse(response, true);
    }
}

PLCResult KeyencePCLinkComm::writeBitDevice(KeyenceDeviceType deviceType, int address, const std::vector<bool>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (values.empty() || values.size() > 256) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-256)";
        return result;
    }

    QString deviceCode = getDeviceCode(deviceType);

    if (values.size() == 1) {
        // 单点写入: WR MR0 1\r
        QString command = QString("%1 %2%3 %4%5")
            .arg(PCLinkCommand::WRITE)
            .arg(deviceCode)
            .arg(address)
            .arg(values[0] ? "1" : "0")
            .arg(CMD_TERMINATOR);

        QString response = sendCommand(command);

        QString errorMsg;
        if (checkResponseOK(response, errorMsg)) {
            result.status = CommStatus::Success;
        } else {
            result.status = CommStatus::Error;
            result.errorMessage = errorMsg;
        }
    } else {
        // 连续写入: WRS MR0 10 1 0 1 0...\r
        QString command = QString("%1 %2%3 %4")
            .arg(PCLinkCommand::WRITE_CONT)
            .arg(deviceCode)
            .arg(address)
            .arg(values.size());

        for (bool value : values) {
            command += QString(" %1").arg(value ? "1" : "0");
        }
        command += CMD_TERMINATOR;

        QString response = sendCommand(command);

        QString errorMsg;
        if (checkResponseOK(response, errorMsg)) {
            result.status = CommStatus::Success;
        } else {
            result.status = CommStatus::Error;
            result.errorMessage = errorMsg;
        }
    }

    return result;
}

QString KeyencePCLinkComm::sendCommand(const QString& command)
{
    if (!socket_->isValid()) {
        return QString();
    }

    // 转换为字节并发送
    QByteArray data = command.toLatin1();

    LOG_INFO(QString("发送命令: %1").arg(command.trimmed()));

    socket_->write(data);
    if (!socket_->waitForBytesWritten(timeout_)) {
        LOG_ERROR("发送命令超时");
        return QString();
    }

    // 等待响应
    if (!socket_->waitForReadyRead(timeout_)) {
        LOG_ERROR("接收响应超时");
        return QString();
    }

    // 读取响应直到收到完整响应
    QString response;
    while (!response.contains(RESPONSE_TERMINATOR)) {
        QByteArray chunk = socket_->readAll();
        response += QString::fromLatin1(chunk);

        if (!response.contains(RESPONSE_TERMINATOR)) {
            if (!socket_->waitForReadyRead(timeout_)) {
                break;
            }
        }
    }

    LOG_INFO(QString("接收响应: %1").arg(response.trimmed()));

    return response;
}

QString KeyencePCLinkComm::getDeviceCode(KeyenceDeviceType deviceType) const
{
    switch (deviceType) {
    case KeyenceDeviceType::DM:  return KeyenceDeviceCode::DM;
    case KeyenceDeviceType::CM:  return KeyenceDeviceCode::CM;
    case KeyenceDeviceType::TM:  return KeyenceDeviceCode::TM;
    case KeyenceDeviceType::EM:  return KeyenceDeviceCode::EM;
    case KeyenceDeviceType::FM:  return KeyenceDeviceCode::FM;
    case KeyenceDeviceType::ZF:  return KeyenceDeviceCode::ZF;
    case KeyenceDeviceType::R:   return KeyenceDeviceCode::R;
    case KeyenceDeviceType::B:   return KeyenceDeviceCode::B;
    case KeyenceDeviceType::MR:  return KeyenceDeviceCode::MR;
    case KeyenceDeviceType::LR:  return KeyenceDeviceCode::LR;
    case KeyenceDeviceType::CR:  return KeyenceDeviceCode::CR;
    case KeyenceDeviceType::T:   return KeyenceDeviceCode::T;
    case KeyenceDeviceType::TC:  return KeyenceDeviceCode::TC;
    case KeyenceDeviceType::C:   return KeyenceDeviceCode::C;
    case KeyenceDeviceType::CC:  return KeyenceDeviceCode::CC;
    default: return KeyenceDeviceCode::DM;
    }
}

bool KeyencePCLinkComm::isBitDevice(KeyenceDeviceType deviceType) const
{
    switch (deviceType) {
    case KeyenceDeviceType::R:
    case KeyenceDeviceType::B:
    case KeyenceDeviceType::MR:
    case KeyenceDeviceType::LR:
    case KeyenceDeviceType::CR:
    case KeyenceDeviceType::TC:
    case KeyenceDeviceType::CC:
        return true;
    default:
        return false;
    }
}

PLCResult KeyencePCLinkComm::parseResponse(const QString& response, bool isBitDevice)
{
    PLCResult result;

    if (response.isEmpty()) {
        result.status = CommStatus::Timeout;
        result.errorMessage = "响应超时";
        return result;
    }

    // 检查错误响应
    QString errorMsg;
    if (!checkResponseOK(response, errorMsg)) {
        result.status = CommStatus::Error;
        result.errorMessage = errorMsg;
        return result;
    }

    // 去除终止符
    QString data = response.trimmed();

    // 如果只是OK响应，说明是写入成功
    if (data == "OK") {
        result.status = CommStatus::Success;
        return result;
    }

    // 解析数据值 (十六进制格式，如 "0064 00C8")
    result.status = CommStatus::Success;
    QStringList values = data.split(' ', Qt::SkipEmptyParts);

    if (isBitDevice) {
        for (const QString& val : values) {
            result.boolValues.push_back(val != "0" && val != "0000");
        }
    } else {
        for (const QString& val : values) {
            bool ok;
            // 先尝试十六进制解析
            int intVal = val.toInt(&ok, 16);
            if (!ok) {
                // 如果失败，尝试十进制
                intVal = val.toInt(&ok, 10);
            }
            if (ok) {
                result.intValues.push_back(intVal);
            }
        }
    }

    return result;
}

bool KeyencePCLinkComm::checkResponseOK(const QString& response, QString& errorMsg)
{
    QString trimmed = response.trimmed();

    // 检查错误代码
    if (trimmed.startsWith("E0")) {
        errorMsg = "命令错误 (E0)";
        return false;
    }
    if (trimmed.startsWith("E1")) {
        errorMsg = "数据错误 (E1)";
        return false;
    }
    if (trimmed.startsWith("E2")) {
        errorMsg = "通信错误 (E2)";
        return false;
    }
    if (trimmed.startsWith("E")) {
        errorMsg = QString("未知错误: %1").arg(trimmed);
        return false;
    }

    return true;
}

} // namespace Comm
} // namespace VisionForge
