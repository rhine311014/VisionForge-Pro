/**
 * @file ModbusTCPComm.cpp
 * @brief Modbus TCP通信实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "comm/ModbusTCPComm.h"
#include "base/Logger.h"
#include <QDataStream>

namespace VisionForge {
namespace Comm {

// Modbus功能码定义
namespace ModbusFunction {
    constexpr quint8 ReadCoils = 0x01;              // 读线圈
    constexpr quint8 ReadDiscreteInputs = 0x02;     // 读离散输入
    constexpr quint8 ReadHoldingRegisters = 0x03;   // 读保持寄存器
    constexpr quint8 ReadInputRegisters = 0x04;     // 读输入寄存器
    constexpr quint8 WriteSingleCoil = 0x05;        // 写单个线圈
    constexpr quint8 WriteSingleRegister = 0x06;    // 写单个寄存器
    constexpr quint8 WriteMultipleCoils = 0x0F;     // 写多个线圈
    constexpr quint8 WriteMultipleRegisters = 0x10; // 写多个寄存器
}

ModbusTCPComm::ModbusTCPComm(QObject* parent)
    : PLCComm(parent)
    , socket_(new QTcpSocket(this))
    , transactionId_(0)
    , slaveId_(1)
{
    protocol_ = PLCProtocol::ModbusTCP;

    // 连接信号
    QObject::connect(socket_, &QTcpSocket::connected, this, [this]() {
        LOG_INFO("Modbus TCP已连接");
        emit connectionChanged(true);
    });

    QObject::connect(socket_, &QTcpSocket::disconnected, this, [this]() {
        LOG_INFO("Modbus TCP已断开");
        emit connectionChanged(false);
    });

    QObject::connect(socket_, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        lastError_ = socket_->errorString();
        LOG_ERROR(QString("Modbus TCP错误: %1").arg(lastError_));
        emit errorOccurred(lastError_);
    });
}

ModbusTCPComm::~ModbusTCPComm()
{
    disconnect();
}

bool ModbusTCPComm::connect(const PLCConfig& config)
{
    QMutexLocker locker(&mutex_);

    config_ = config;
    slaveId_ = static_cast<quint8>(config.slaveId);

    auto socketConfig = std::dynamic_pointer_cast<SocketConfig>(config.commConfig);
    if (!socketConfig) {
        lastError_ = "无效的网络配置";
        LOG_ERROR(lastError_);
        return false;
    }

    timeout_ = socketConfig->timeout;

    LOG_INFO(QString("正在连接Modbus TCP: %1:%2").arg(socketConfig->ipAddress).arg(socketConfig->port));

    socket_->connectToHost(socketConfig->ipAddress, socketConfig->port);

    if (!socket_->waitForConnected(timeout_)) {
        lastError_ = QString("连接超时: %1").arg(socket_->errorString());
        LOG_ERROR(lastError_);
        return false;
    }

    LOG_INFO("Modbus TCP连接成功");
    return true;
}

void ModbusTCPComm::disconnect()
{
    QMutexLocker locker(&mutex_);

    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->disconnectFromHost();
        if (socket_->state() != QAbstractSocket::UnconnectedState) {
            socket_->waitForDisconnected(1000);
        }
    }
}

bool ModbusTCPComm::isConnected() const
{
    return socket_->state() == QAbstractSocket::ConnectedState;
}

PLCResult ModbusTCPComm::readRegisters(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 125) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-125)";
        return result;
    }

    // 构建读取保持寄存器请求
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    QByteArray request = buildFrame(ModbusFunction::ReadHoldingRegisters, pduData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, ModbusFunction::ReadHoldingRegisters);
}

PLCResult ModbusTCPComm::writeRegisters(int address, const std::vector<int>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (values.empty() || values.size() > 123) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-123)";
        return result;
    }

    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    if (values.size() == 1) {
        // 写单个寄存器
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values[0] & 0xFFFF);

        QByteArray request = buildFrame(ModbusFunction::WriteSingleRegister, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteSingleRegister);
    } else {
        // 写多个寄存器
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values.size());
        stream << static_cast<quint8>(values.size() * 2);

        for (int value : values) {
            stream << static_cast<quint16>(value & 0xFFFF);
        }

        QByteArray request = buildFrame(ModbusFunction::WriteMultipleRegisters, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteMultipleRegisters);
    }
}

PLCResult ModbusTCPComm::readCoils(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 2000) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-2000)";
        return result;
    }

    // 构建读取线圈请求
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    QByteArray request = buildFrame(ModbusFunction::ReadCoils, pduData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, ModbusFunction::ReadCoils);
}

PLCResult ModbusTCPComm::writeCoils(int address, const std::vector<bool>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (values.empty() || values.size() > 1968) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-1968)";
        return result;
    }

    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    if (values.size() == 1) {
        // 写单个线圈
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values[0] ? 0xFF00 : 0x0000);

        QByteArray request = buildFrame(ModbusFunction::WriteSingleCoil, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteSingleCoil);
    } else {
        // 写多个线圈
        int byteCount = (values.size() + 7) / 8;
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values.size());
        stream << static_cast<quint8>(byteCount);

        // 将布尔值打包为字节
        for (int i = 0; i < byteCount; ++i) {
            quint8 byte = 0;
            for (int j = 0; j < 8 && i * 8 + j < static_cast<int>(values.size()); ++j) {
                if (values[i * 8 + j]) {
                    byte |= (1 << j);
                }
            }
            stream << byte;
        }

        QByteArray request = buildFrame(ModbusFunction::WriteMultipleCoils, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteMultipleCoils);
    }
}

PLCResult ModbusTCPComm::readInputRegisters(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 125) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-125)";
        return result;
    }

    // 构建读取输入寄存器请求
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    QByteArray request = buildFrame(ModbusFunction::ReadInputRegisters, pduData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, ModbusFunction::ReadInputRegisters);
}

PLCResult ModbusTCPComm::readDiscreteInputs(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 2000) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-2000)";
        return result;
    }

    // 构建读取离散输入请求
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    QByteArray request = buildFrame(ModbusFunction::ReadDiscreteInputs, pduData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, ModbusFunction::ReadDiscreteInputs);
}

QByteArray ModbusTCPComm::sendRequest(const QByteArray& request)
{
    if (!socket_->isValid()) {
        return QByteArray();
    }

    // 发送请求
    socket_->write(request);
    if (!socket_->waitForBytesWritten(timeout_)) {
        LOG_ERROR("发送请求超时");
        return QByteArray();
    }

    // 等待响应
    if (!socket_->waitForReadyRead(timeout_)) {
        LOG_ERROR("接收响应超时");
        return QByteArray();
    }

    // 读取响应
    QByteArray response;

    // 首先读取MBAP头 (7字节)
    while (response.size() < 7) {
        if (!socket_->waitForReadyRead(timeout_)) {
            break;
        }
        response.append(socket_->read(7 - response.size()));
    }

    if (response.size() < 7) {
        LOG_ERROR("MBAP头接收不完整");
        return QByteArray();
    }

    // 从MBAP头获取数据长度
    quint16 length = (static_cast<quint8>(response[4]) << 8) | static_cast<quint8>(response[5]);

    // 读取剩余数据
    int remaining = length - 1;  // 减去Unit ID
    while (response.size() < 7 + remaining) {
        if (!socket_->waitForReadyRead(timeout_)) {
            break;
        }
        response.append(socket_->read(7 + remaining - response.size()));
    }

    return response;
}

QByteArray ModbusTCPComm::buildFrame(quint8 functionCode, const QByteArray& data)
{
    QByteArray frame;
    QDataStream stream(&frame, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // MBAP Header
    stream << ++transactionId_;                          // Transaction ID
    stream << static_cast<quint16>(0);                   // Protocol ID (0 for Modbus)
    stream << static_cast<quint16>(data.size() + 2);     // Length (Unit ID + Function Code + Data)
    stream << slaveId_;                                   // Unit ID

    // PDU
    stream << functionCode;
    frame.append(data);

    return frame;
}

PLCResult ModbusTCPComm::parseResponse(const QByteArray& response, quint8 expectedFunctionCode)
{
    PLCResult result;

    if (response.size() < 9) {
        result.status = CommStatus::Error;
        result.errorMessage = "响应数据不完整";
        return result;
    }

    // 检查Transaction ID
    quint16 txId = (static_cast<quint8>(response[0]) << 8) | static_cast<quint8>(response[1]);
    if (txId != transactionId_) {
        result.status = CommStatus::Error;
        result.errorMessage = "Transaction ID不匹配";
        return result;
    }

    // 检查功能码
    quint8 functionCode = static_cast<quint8>(response[7]);

    // 检查异常响应
    if (functionCode & 0x80) {
        quint8 exceptionCode = static_cast<quint8>(response[8]);
        result.status = CommStatus::Error;
        result.errorMessage = getExceptionMessage(exceptionCode);
        return result;
    }

    // 验证功能码
    if (functionCode != expectedFunctionCode) {
        result.status = CommStatus::Error;
        result.errorMessage = QString("功能码不匹配: 期望0x%1, 收到0x%2")
                                  .arg(expectedFunctionCode, 2, 16, QChar('0'))
                                  .arg(functionCode, 2, 16, QChar('0'));
        return result;
    }

    // 解析数据
    result.status = CommStatus::Success;

    switch (functionCode) {
    case ModbusFunction::ReadHoldingRegisters:
    case ModbusFunction::ReadInputRegisters:
    {
        // 字节数在response[8]
        int byteCount = static_cast<quint8>(response[8]);
        int registerCount = byteCount / 2;

        for (int i = 0; i < registerCount; ++i) {
            int offset = 9 + i * 2;
            if (offset + 1 < response.size()) {
                quint16 value = (static_cast<quint8>(response[offset]) << 8) |
                                static_cast<quint8>(response[offset + 1]);
                result.intValues.push_back(static_cast<int>(value));
            }
        }
        break;
    }

    case ModbusFunction::ReadCoils:
    case ModbusFunction::ReadDiscreteInputs:
    {
        int byteCount = static_cast<quint8>(response[8]);

        for (int i = 0; i < byteCount; ++i) {
            int offset = 9 + i;
            if (offset < response.size()) {
                quint8 byte = static_cast<quint8>(response[offset]);
                for (int j = 0; j < 8; ++j) {
                    result.boolValues.push_back((byte >> j) & 0x01);
                }
            }
        }
        break;
    }

    case ModbusFunction::WriteSingleCoil:
    case ModbusFunction::WriteSingleRegister:
    case ModbusFunction::WriteMultipleCoils:
    case ModbusFunction::WriteMultipleRegisters:
        // 写入操作，成功即可
        break;
    }

    return result;
}

QString ModbusTCPComm::getExceptionMessage(quint8 exceptionCode)
{
    switch (exceptionCode) {
    case 0x01: return "非法功能码";
    case 0x02: return "非法数据地址";
    case 0x03: return "非法数据值";
    case 0x04: return "从站设备故障";
    case 0x05: return "确认";
    case 0x06: return "从站设备忙";
    case 0x08: return "存储奇偶性差错";
    case 0x0A: return "网关路径不可用";
    case 0x0B: return "网关目标设备未响应";
    default:   return QString("未知异常码: 0x%1").arg(exceptionCode, 2, 16, QChar('0'));
    }
}

} // namespace Comm
} // namespace VisionForge
