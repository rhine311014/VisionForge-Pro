/**
 * @file ModbusRTUComm.cpp
 * @brief Modbus RTU串口通信实现
 */

#include "comm/ModbusRTUComm.h"
#include "base/Logger.h"
#include <QSerialPortInfo>
#include <QThread>

namespace VisionForge {
namespace Comm {

ModbusRTUComm::ModbusRTUComm(QObject* parent)
    : PLCComm(parent)
    , serialPort_(new QSerialPort(this))
    , slaveId_(1)
{
    protocol_ = PLCProtocol::ModbusRTU;
}

ModbusRTUComm::~ModbusRTUComm()
{
    disconnect();
}

bool ModbusRTUComm::connect(const PLCConfig& config)
{
    if (isConnected()) {
        disconnect();
    }

    // 保存配置
    config_ = config;
    slaveId_ = config.slaveId;

    // 获取串口配置
    auto serialConfig = std::dynamic_pointer_cast<SerialConfig>(config.commConfig);
    if (!serialConfig) {
        LOG_ERROR("无效的串口配置");
        lastError_ = "无效的串口配置";
        return false;
    }

    // 配置串口
    serialPort_->setPortName(serialConfig->portName);
    serialPort_->setBaudRate(serialConfig->baudRate);

    // 设置数据位
    switch (serialConfig->dataBits) {
    case 5: serialPort_->setDataBits(QSerialPort::Data5); break;
    case 6: serialPort_->setDataBits(QSerialPort::Data6); break;
    case 7: serialPort_->setDataBits(QSerialPort::Data7); break;
    case 8:
    default: serialPort_->setDataBits(QSerialPort::Data8); break;
    }

    // 设置校验位 (0=无, 1=奇, 2=偶)
    switch (serialConfig->parity) {
    case 1: serialPort_->setParity(QSerialPort::OddParity); break;
    case 2: serialPort_->setParity(QSerialPort::EvenParity); break;
    case 0:
    default: serialPort_->setParity(QSerialPort::NoParity); break;
    }

    // 设置停止位
    switch (serialConfig->stopBits) {
    case 2: serialPort_->setStopBits(QSerialPort::TwoStop); break;
    case 1:
    default: serialPort_->setStopBits(QSerialPort::OneStop); break;
    }

    // 打开串口
    if (!serialPort_->open(QIODevice::ReadWrite)) {
        lastError_ = QString("打开串口失败: %1 - %2")
                    .arg(serialConfig->portName)
                    .arg(serialPort_->errorString());
        LOG_ERROR(lastError_);
        return false;
    }

    // 清空缓冲区
    serialPort_->clear();

    LOG_INFO(QString("Modbus RTU连接成功: %1 @ %2bps")
            .arg(serialConfig->portName)
            .arg(serialConfig->baudRate));

    emit connectionChanged(true);
    return true;
}

void ModbusRTUComm::disconnect()
{
    if (serialPort_->isOpen()) {
        serialPort_->close();
        LOG_INFO("Modbus RTU断开连接");
        emit connectionChanged(false);
    }
}

bool ModbusRTUComm::isConnected() const
{
    return serialPort_->isOpen();
}

// ========== PLCComm接口实现 ==========

PLCResult ModbusRTUComm::readRegisters(int address, int count)
{
    return readHoldingRegisters(slaveId_, address, count);
}

PLCResult ModbusRTUComm::writeRegisters(int address, const std::vector<int>& values)
{
    if (values.size() == 1) {
        return writeSingleRegister(slaveId_, address, values[0]);
    }
    return writeMultipleRegisters(slaveId_, address, values);
}

PLCResult ModbusRTUComm::readCoils(int address, int count)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    QByteArray request = buildRequest(slaveId_, ReadCoils, address, count);
    int expectedLen = 5 + (count + 7) / 8;  // 从站地址+功能码+字节数+数据+CRC

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, ReadCoils);
}

PLCResult ModbusRTUComm::writeCoils(int address, const std::vector<bool>& values)
{
    if (values.size() == 1) {
        return writeSingleCoil(slaveId_, address, values[0]);
    }
    return writeMultipleCoils(slaveId_, address, values);
}

// ========== Modbus RTU特有接口 ==========

PLCResult ModbusRTUComm::readHoldingRegisters(int slaveId, int address, int count)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    QByteArray request = buildRequest(slaveId, ReadHoldingRegisters, address, count);
    int expectedLen = 5 + count * 2;  // 从站地址+功能码+字节数+数据+CRC

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, ReadHoldingRegisters);
}

PLCResult ModbusRTUComm::readInputRegisters(int slaveId, int address, int count)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    QByteArray request = buildRequest(slaveId, ReadInputRegisters, address, count);
    int expectedLen = 5 + count * 2;

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, ReadInputRegisters);
}

PLCResult ModbusRTUComm::writeSingleRegister(int slaveId, int address, int value)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    QByteArray request = buildRequest(slaveId, WriteSingleRegister, address, value);
    int expectedLen = 8;  // 固定8字节响应

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, WriteSingleRegister);
}

PLCResult ModbusRTUComm::writeMultipleRegisters(int slaveId, int address, const std::vector<int>& values)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    QByteArray request = buildWriteMultipleRegistersRequest(slaveId, address, values);
    int expectedLen = 8;  // 固定8字节响应

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, WriteMultipleRegisters);
}

PLCResult ModbusRTUComm::readDiscreteInputs(int slaveId, int address, int count)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    QByteArray request = buildRequest(slaveId, ReadDiscreteInputs, address, count);
    int expectedLen = 5 + (count + 7) / 8;

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, ReadDiscreteInputs);
}

PLCResult ModbusRTUComm::writeSingleCoil(int slaveId, int address, bool value)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    // Modbus中ON=0xFF00, OFF=0x0000
    int coilValue = value ? 0xFF00 : 0x0000;
    QByteArray request = buildRequest(slaveId, WriteSingleCoil, address, coilValue);
    int expectedLen = 8;

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, WriteSingleCoil);
}

PLCResult ModbusRTUComm::writeMultipleCoils(int slaveId, int address, const std::vector<bool>& values)
{
    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    QByteArray request = buildWriteMultipleCoilsRequest(slaveId, address, values);
    int expectedLen = 8;

    QByteArray response = sendAndReceive(request, expectedLen);
    return parseResponse(response, WriteMultipleCoils);
}

QStringList ModbusRTUComm::availablePorts()
{
    QStringList ports;
    const auto portInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : portInfos) {
        ports.append(info.portName());
    }
    return ports;
}

// ========== 私有方法 ==========

QByteArray ModbusRTUComm::sendAndReceive(const QByteArray& request, int expectedLength)
{
    QMutexLocker locker(&mutex_);

    // 清空接收缓冲区
    serialPort_->clear();

    // 发送请求
    qint64 written = serialPort_->write(request);
    if (written != request.size()) {
        LOG_ERROR("Modbus发送失败");
        return QByteArray();
    }

    // 等待数据发送完成
    if (!serialPort_->waitForBytesWritten(timeout_)) {
        LOG_ERROR("Modbus发送超时");
        return QByteArray();
    }

    // 等待响应
    QByteArray response;
    int elapsed = 0;
    const int interval = 10;

    while (response.size() < expectedLength && elapsed < timeout_) {
        if (serialPort_->waitForReadyRead(interval)) {
            response.append(serialPort_->readAll());
        }
        elapsed += interval;
    }

    if (response.size() < expectedLength) {
        LOG_WARNING(QString("Modbus响应不完整: 期望%1字节，收到%2字节")
                   .arg(expectedLength).arg(response.size()));
    }

    return response;
}

QByteArray ModbusRTUComm::buildRequest(int slaveId, FunctionCode func, int address, int countOrValue)
{
    QByteArray request;
    request.append(static_cast<char>(slaveId));
    request.append(static_cast<char>(func));
    request.append(static_cast<char>((address >> 8) & 0xFF));
    request.append(static_cast<char>(address & 0xFF));
    request.append(static_cast<char>((countOrValue >> 8) & 0xFF));
    request.append(static_cast<char>(countOrValue & 0xFF));

    // 添加CRC
    quint16 crc = calculateCRC16(request);
    request.append(static_cast<char>(crc & 0xFF));        // CRC低字节
    request.append(static_cast<char>((crc >> 8) & 0xFF)); // CRC高字节

    return request;
}

QByteArray ModbusRTUComm::buildWriteMultipleRegistersRequest(int slaveId, int address, const std::vector<int>& values)
{
    QByteArray request;
    request.append(static_cast<char>(slaveId));
    request.append(static_cast<char>(WriteMultipleRegisters));
    request.append(static_cast<char>((address >> 8) & 0xFF));
    request.append(static_cast<char>(address & 0xFF));

    int count = static_cast<int>(values.size());
    request.append(static_cast<char>((count >> 8) & 0xFF));
    request.append(static_cast<char>(count & 0xFF));

    int byteCount = count * 2;
    request.append(static_cast<char>(byteCount));

    // 添加寄存器值
    for (int value : values) {
        request.append(static_cast<char>((value >> 8) & 0xFF));
        request.append(static_cast<char>(value & 0xFF));
    }

    // 添加CRC
    quint16 crc = calculateCRC16(request);
    request.append(static_cast<char>(crc & 0xFF));
    request.append(static_cast<char>((crc >> 8) & 0xFF));

    return request;
}

QByteArray ModbusRTUComm::buildWriteMultipleCoilsRequest(int slaveId, int address, const std::vector<bool>& values)
{
    QByteArray request;
    request.append(static_cast<char>(slaveId));
    request.append(static_cast<char>(WriteMultipleCoils));
    request.append(static_cast<char>((address >> 8) & 0xFF));
    request.append(static_cast<char>(address & 0xFF));

    int count = static_cast<int>(values.size());
    request.append(static_cast<char>((count >> 8) & 0xFF));
    request.append(static_cast<char>(count & 0xFF));

    int byteCount = (count + 7) / 8;
    request.append(static_cast<char>(byteCount));

    // 打包线圈值
    for (int i = 0; i < byteCount; ++i) {
        char byte = 0;
        for (int j = 0; j < 8 && (i * 8 + j) < count; ++j) {
            if (values[i * 8 + j]) {
                byte |= (1 << j);
            }
        }
        request.append(byte);
    }

    // 添加CRC
    quint16 crc = calculateCRC16(request);
    request.append(static_cast<char>(crc & 0xFF));
    request.append(static_cast<char>((crc >> 8) & 0xFF));

    return request;
}

PLCResult ModbusRTUComm::parseResponse(const QByteArray& response, FunctionCode expectedFunc)
{
    PLCResult result;

    if (response.isEmpty()) {
        result.status = CommStatus::Timeout;
        result.errorMessage = "无响应";
        return result;
    }

    // 验证CRC
    if (!verifyCRC(response)) {
        result.status = CommStatus::Error;
        result.errorMessage = "CRC校验失败";
        return result;
    }

    // 检查异常响应
    quint8 funcCode = static_cast<quint8>(response[1]);
    if (funcCode & 0x80) {
        result.status = CommStatus::Error;
        quint8 exceptionCode = static_cast<quint8>(response[2]);
        switch (exceptionCode) {
        case 0x01: result.errorMessage = "非法功能码"; break;
        case 0x02: result.errorMessage = "非法数据地址"; break;
        case 0x03: result.errorMessage = "非法数据值"; break;
        case 0x04: result.errorMessage = "从站设备故障"; break;
        default: result.errorMessage = QString("未知异常: 0x%1").arg(exceptionCode, 2, 16, QChar('0')); break;
        }
        return result;
    }

    // 解析成功响应
    result.status = CommStatus::Success;

    switch (expectedFunc) {
    case ReadCoils:
    case ReadDiscreteInputs: {
        // 解析线圈/离散输入
        quint8 byteCount = static_cast<quint8>(response[2]);
        for (int i = 0; i < byteCount; ++i) {
            quint8 dataByte = static_cast<quint8>(response[3 + i]);
            for (int j = 0; j < 8; ++j) {
                result.boolValues.push_back((dataByte >> j) & 0x01);
            }
        }
        break;
    }

    case ReadHoldingRegisters:
    case ReadInputRegisters: {
        // 解析寄存器
        quint8 byteCount = static_cast<quint8>(response[2]);
        int registerCount = byteCount / 2;
        for (int i = 0; i < registerCount; ++i) {
            int high = static_cast<quint8>(response[3 + i * 2]);
            int low = static_cast<quint8>(response[4 + i * 2]);
            result.intValues.push_back((high << 8) | low);
        }
        break;
    }

    case WriteSingleCoil:
    case WriteSingleRegister:
    case WriteMultipleCoils:
    case WriteMultipleRegisters:
        // 写操作成功
        break;
    }

    return result;
}

quint16 ModbusRTUComm::calculateCRC16(const QByteArray& data)
{
    quint16 crc = 0xFFFF;

    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data[i]);

        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }

    return crc;
}

bool ModbusRTUComm::verifyCRC(const QByteArray& data)
{
    if (data.size() < 4) {
        return false;
    }

    // 提取接收到的CRC（低字节在前）
    quint8 crcLow = static_cast<quint8>(data[data.size() - 2]);
    quint8 crcHigh = static_cast<quint8>(data[data.size() - 1]);
    quint16 receivedCRC = (crcHigh << 8) | crcLow;

    // 计算数据部分的CRC
    QByteArray dataWithoutCRC = data.left(data.size() - 2);
    quint16 calculatedCRC = calculateCRC16(dataWithoutCRC);

    return receivedCRC == calculatedCRC;
}

} // namespace Comm
} // namespace VisionForge
