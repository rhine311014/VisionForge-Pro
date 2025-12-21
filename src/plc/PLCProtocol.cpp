/**
 * @file PLCProtocol.cpp
 * @brief PLC协议实现
 * @author VisionForge Team
 * @date 2025-12-21
 *
 * 实现Modbus TCP和三菱MC 3E帧协议
 */

#include "plc/PLCProtocol.h"
#include <QDataStream>
#include <QtEndian>
#include <cstring>

namespace VisionForge {
namespace PLC {

// ============================================================
// PLCProtocol 基类实现
// ============================================================

PLCProtocol::PLCProtocol(QObject* parent)
    : QObject(parent)
{
}

PLCProtocol::~PLCProtocol()
{
}

void PLCProtocol::setConfig(const PLCConfig& config)
{
    config_ = config;
}

std::vector<int32_t> PLCProtocol::toInt32(const std::vector<uint16_t>& values, ByteOrder order)
{
    std::vector<int32_t> result;
    result.reserve(values.size() / 2);

    for (size_t i = 0; i + 1 < values.size(); i += 2) {
        int32_t value;
        switch (order) {
        case ByteOrder::BigEndian:
            value = (static_cast<int32_t>(values[i]) << 16) |
                    static_cast<int32_t>(values[i + 1]);
            break;
        case ByteOrder::LittleEndian:
            value = (static_cast<int32_t>(values[i + 1]) << 16) |
                    static_cast<int32_t>(values[i]);
            break;
        case ByteOrder::BigEndianSwap:
            value = (static_cast<int32_t>(values[i + 1]) << 16) |
                    static_cast<int32_t>(values[i]);
            break;
        case ByteOrder::LittleEndianSwap:
            value = (static_cast<int32_t>(values[i]) << 16) |
                    static_cast<int32_t>(values[i + 1]);
            break;
        }
        result.push_back(value);
    }
    return result;
}

std::vector<float> PLCProtocol::toFloat(const std::vector<uint16_t>& values, ByteOrder order)
{
    std::vector<float> result;
    result.reserve(values.size() / 2);

    for (size_t i = 0; i + 1 < values.size(); i += 2) {
        uint32_t intValue;
        switch (order) {
        case ByteOrder::BigEndian:
            intValue = (static_cast<uint32_t>(values[i]) << 16) |
                       static_cast<uint32_t>(values[i + 1]);
            break;
        case ByteOrder::LittleEndian:
            intValue = (static_cast<uint32_t>(values[i + 1]) << 16) |
                       static_cast<uint32_t>(values[i]);
            break;
        case ByteOrder::BigEndianSwap:
            intValue = (static_cast<uint32_t>(values[i + 1]) << 16) |
                       static_cast<uint32_t>(values[i]);
            break;
        case ByteOrder::LittleEndianSwap:
            intValue = (static_cast<uint32_t>(values[i]) << 16) |
                       static_cast<uint32_t>(values[i + 1]);
            break;
        }

        float floatValue;
        std::memcpy(&floatValue, &intValue, sizeof(float));
        result.push_back(floatValue);
    }
    return result;
}

std::vector<uint16_t> PLCProtocol::fromInt32(const std::vector<int32_t>& values, ByteOrder order)
{
    std::vector<uint16_t> result;
    result.reserve(values.size() * 2);

    for (int32_t value : values) {
        uint16_t high = static_cast<uint16_t>((value >> 16) & 0xFFFF);
        uint16_t low = static_cast<uint16_t>(value & 0xFFFF);

        switch (order) {
        case ByteOrder::BigEndian:
            result.push_back(high);
            result.push_back(low);
            break;
        case ByteOrder::LittleEndian:
            result.push_back(low);
            result.push_back(high);
            break;
        case ByteOrder::BigEndianSwap:
            result.push_back(low);
            result.push_back(high);
            break;
        case ByteOrder::LittleEndianSwap:
            result.push_back(high);
            result.push_back(low);
            break;
        }
    }
    return result;
}

std::vector<uint16_t> PLCProtocol::fromFloat(const std::vector<float>& values, ByteOrder order)
{
    std::vector<uint16_t> result;
    result.reserve(values.size() * 2);

    for (float value : values) {
        uint32_t intValue;
        std::memcpy(&intValue, &value, sizeof(float));

        uint16_t high = static_cast<uint16_t>((intValue >> 16) & 0xFFFF);
        uint16_t low = static_cast<uint16_t>(intValue & 0xFFFF);

        switch (order) {
        case ByteOrder::BigEndian:
            result.push_back(high);
            result.push_back(low);
            break;
        case ByteOrder::LittleEndian:
            result.push_back(low);
            result.push_back(high);
            break;
        case ByteOrder::BigEndianSwap:
            result.push_back(low);
            result.push_back(high);
            break;
        case ByteOrder::LittleEndianSwap:
            result.push_back(high);
            result.push_back(low);
            break;
        }
    }
    return result;
}

// ============================================================
// ModbusTCPProtocol 实现
// ============================================================

ModbusTCPProtocol::ModbusTCPProtocol(QObject* parent)
    : PLCProtocol(parent)
{
}

ModbusTCPProtocol::~ModbusTCPProtocol()
{
}

QByteArray ModbusTCPProtocol::buildMBAPHeader(int pduLength)
{
    QByteArray header;
    header.reserve(7);

    // 事务标识符 (2字节)
    header.append(static_cast<char>((transactionId_ >> 8) & 0xFF));
    header.append(static_cast<char>(transactionId_ & 0xFF));

    // 协议标识符 (2字节) - Modbus TCP固定为0x0000
    header.append(static_cast<char>(0x00));
    header.append(static_cast<char>(0x00));

    // 长度 (2字节) - 单元标识符(1) + PDU长度
    quint16 length = static_cast<quint16>(pduLength + 1);
    header.append(static_cast<char>((length >> 8) & 0xFF));
    header.append(static_cast<char>(length & 0xFF));

    // 单元标识符 (1字节)
    header.append(static_cast<char>(config_.slaveId));

    return header;
}

bool ModbusTCPProtocol::parseMBAPHeader(const QByteArray& data, quint16& transactionId,
                                         quint16& length, quint8& unitId) const
{
    if (data.size() < 7) {
        return false;
    }

    transactionId = (static_cast<uint8_t>(data[0]) << 8) |
                    static_cast<uint8_t>(data[1]);

    // 检查协议标识符
    if (data[2] != 0x00 || data[3] != 0x00) {
        return false;
    }

    length = (static_cast<uint8_t>(data[4]) << 8) |
             static_cast<uint8_t>(data[5]);

    unitId = static_cast<uint8_t>(data[6]);

    return true;
}

quint8 ModbusTCPProtocol::getFunctionCodeForRead(RegisterType regType)
{
    switch (regType) {
    case RegisterType::Coil:
        return ModbusFunctionCode::ReadCoils;
    case RegisterType::DiscreteInput:
        return ModbusFunctionCode::ReadDiscreteInputs;
    case RegisterType::InputRegister:
        return ModbusFunctionCode::ReadInputRegisters;
    case RegisterType::HoldingRegister:
    default:
        return ModbusFunctionCode::ReadHoldingRegisters;
    }
}

quint8 ModbusTCPProtocol::getFunctionCodeForWrite(RegisterType regType, bool single)
{
    switch (regType) {
    case RegisterType::Coil:
        return single ? ModbusFunctionCode::WriteSingleCoil :
                        ModbusFunctionCode::WriteMultipleCoils;
    case RegisterType::HoldingRegister:
    default:
        return single ? ModbusFunctionCode::WriteSingleRegister :
                        ModbusFunctionCode::WriteMultipleRegisters;
    }
}

QByteArray ModbusTCPProtocol::packReadRequest(RegisterType regType, int address, int count)
{
    nextTransactionId();

    // 确定功能码
    quint8 functionCode = getFunctionCodeForRead(regType);

    // 构建PDU
    QByteArray pdu;
    pdu.append(static_cast<char>(functionCode));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));

    // 构建完整帧
    return buildMBAPHeader(pdu.size()) + pdu;
}

QByteArray ModbusTCPProtocol::packWriteRequest(RegisterType regType, int address,
                                                const std::vector<uint16_t>& values)
{
    if (values.empty()) {
        return QByteArray();
    }

    nextTransactionId();

    if (values.size() == 1) {
        // 功能码06 - 写单个寄存器
        return packWriteSingleRegister(address, values[0]);
    } else {
        // 功能码16 - 写多个寄存器
        return packWriteMultipleRegisters(address, values);
    }
}

QByteArray ModbusTCPProtocol::packWriteBitRequest(RegisterType regType, int address,
                                                   const std::vector<bool>& values)
{
    if (values.empty()) {
        return QByteArray();
    }

    nextTransactionId();

    if (values.size() == 1) {
        return packWriteSingleCoil(address, values[0]);
    } else {
        return packWriteMultipleCoils(address, values);
    }
}

QByteArray ModbusTCPProtocol::packReadHoldingRegisters(int address, int count)
{
    nextTransactionId();

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::ReadHoldingRegisters));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));

    return buildMBAPHeader(pdu.size()) + pdu;
}

QByteArray ModbusTCPProtocol::packWriteSingleRegister(int address, uint16_t value)
{
    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::WriteSingleRegister));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((value >> 8) & 0xFF));
    pdu.append(static_cast<char>(value & 0xFF));

    return buildMBAPHeader(pdu.size()) + pdu;
}

QByteArray ModbusTCPProtocol::packWriteMultipleRegisters(int address,
                                                          const std::vector<uint16_t>& values)
{
    int count = static_cast<int>(values.size());
    int byteCount = count * 2;

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::WriteMultipleRegisters));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));
    pdu.append(static_cast<char>(byteCount));

    for (uint16_t value : values) {
        pdu.append(static_cast<char>((value >> 8) & 0xFF));
        pdu.append(static_cast<char>(value & 0xFF));
    }

    return buildMBAPHeader(pdu.size()) + pdu;
}

QByteArray ModbusTCPProtocol::packReadCoils(int address, int count)
{
    nextTransactionId();

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::ReadCoils));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));

    return buildMBAPHeader(pdu.size()) + pdu;
}

QByteArray ModbusTCPProtocol::packWriteSingleCoil(int address, bool value)
{
    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::WriteSingleCoil));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>(value ? 0xFF : 0x00));
    pdu.append(static_cast<char>(0x00));

    return buildMBAPHeader(pdu.size()) + pdu;
}

QByteArray ModbusTCPProtocol::packWriteMultipleCoils(int address, const std::vector<bool>& values)
{
    int count = static_cast<int>(values.size());
    int byteCount = (count + 7) / 8;

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::WriteMultipleCoils));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));
    pdu.append(static_cast<char>(byteCount));

    // 打包位数据
    for (int i = 0; i < byteCount; ++i) {
        uint8_t byte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            int index = i * 8 + bit;
            if (index < count && values[index]) {
                byte |= (1 << bit);
            }
        }
        pdu.append(static_cast<char>(byte));
    }

    return buildMBAPHeader(pdu.size()) + pdu;
}

PLCResult ModbusTCPProtocol::parseResponse(const QByteArray& response,
                                            RegisterType expectedType)
{
    PLCResult result;
    result.timestamp = QDateTime::currentDateTime();
    result.rawData = response;

    // 最小长度检查
    if (response.size() < 9) {
        result.success = false;
        result.errorCode = ErrorCode::DataError;
        result.errorMessage = QStringLiteral("Response too short: %1 bytes").arg(response.size());
        return result;
    }

    // 解析MBAP头
    quint16 transactionId, length;
    quint8 unitId;
    if (!parseMBAPHeader(response, transactionId, length, unitId)) {
        result.success = false;
        result.errorCode = ErrorCode::ProtocolError;
        result.errorMessage = QStringLiteral("Invalid MBAP header");
        return result;
    }

    // 检查事务ID
    if (transactionId != transactionId_) {
        result.success = false;
        result.errorCode = ErrorCode::ProtocolError;
        result.errorMessage = QStringLiteral("Transaction ID mismatch: expected %1, got %2")
                                .arg(transactionId_).arg(transactionId);
        return result;
    }

    // 获取功能码
    quint8 functionCode = static_cast<uint8_t>(response[7]);

    // 检查异常响应 (功能码最高位为1)
    if (functionCode & 0x80) {
        quint8 exceptionCode = static_cast<uint8_t>(response[8]);
        result.success = false;
        result.errorCode = ErrorCode::ModbusIllegalFunction + exceptionCode - 1;
        result.errorMessage = getModbusExceptionMessage(exceptionCode);
        return result;
    }

    // 根据功能码解析数据
    switch (functionCode) {
    case ModbusFunctionCode::ReadHoldingRegisters:
    case ModbusFunctionCode::ReadInputRegisters:
    {
        if (response.size() < 9) {
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Invalid response length");
            return result;
        }

        int byteCount = static_cast<uint8_t>(response[8]);
        if (response.size() < 9 + byteCount) {
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Incomplete register data");
            return result;
        }

        // 解析寄存器数据
        for (int i = 0; i < byteCount; i += 2) {
            uint16_t value = (static_cast<uint8_t>(response[9 + i]) << 8) |
                             static_cast<uint8_t>(response[9 + i + 1]);
            result.uint16Values.push_back(value);
            result.int16Values.push_back(static_cast<int16_t>(value));
        }
        result.success = true;
        break;
    }

    case ModbusFunctionCode::ReadCoils:
    case ModbusFunctionCode::ReadDiscreteInputs:
    {
        if (response.size() < 9) {
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Invalid response length");
            return result;
        }

        int byteCount = static_cast<uint8_t>(response[8]);
        if (response.size() < 9 + byteCount) {
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Incomplete coil data");
            return result;
        }

        // 解析位数据
        for (int i = 0; i < byteCount; ++i) {
            uint8_t byte = static_cast<uint8_t>(response[9 + i]);
            for (int bit = 0; bit < 8; ++bit) {
                result.bitValues.push_back((byte >> bit) & 0x01);
            }
        }
        result.success = true;
        break;
    }

    case ModbusFunctionCode::WriteSingleRegister:
    case ModbusFunctionCode::WriteMultipleRegisters:
    case ModbusFunctionCode::WriteSingleCoil:
    case ModbusFunctionCode::WriteMultipleCoils:
        // 写入响应 - 成功
        result.success = true;
        break;

    default:
        result.success = false;
        result.errorCode = ErrorCode::ProtocolError;
        result.errorMessage = QStringLiteral("Unknown function code: 0x%1")
                                .arg(functionCode, 2, 16, QChar('0'));
        break;
    }

    return result;
}

bool ModbusTCPProtocol::isResponseComplete(const QByteArray& data) const
{
    if (data.size() < 7) {
        return false;
    }

    // 从MBAP头获取长度
    quint16 length = (static_cast<uint8_t>(data[4]) << 8) |
                     static_cast<uint8_t>(data[5]);

    // 完整帧长度 = MBAP头(6字节) + 单元标识符(1字节) + PDU数据(length-1字节)
    return data.size() >= 6 + length;
}

int ModbusTCPProtocol::getExpectedResponseLength(const QByteArray& request) const
{
    if (request.size() < 12) {
        return -1;
    }

    quint8 functionCode = static_cast<uint8_t>(request[7]);

    switch (functionCode) {
    case ModbusFunctionCode::ReadHoldingRegisters:
    case ModbusFunctionCode::ReadInputRegisters:
    {
        // 读取的寄存器数量
        quint16 count = (static_cast<uint8_t>(request[10]) << 8) |
                        static_cast<uint8_t>(request[11]);
        // MBAP头(7) + 功能码(1) + 字节数(1) + 数据(count*2)
        return 9 + count * 2;
    }

    case ModbusFunctionCode::ReadCoils:
    case ModbusFunctionCode::ReadDiscreteInputs:
    {
        quint16 count = (static_cast<uint8_t>(request[10]) << 8) |
                        static_cast<uint8_t>(request[11]);
        int byteCount = (count + 7) / 8;
        return 9 + byteCount;
    }

    case ModbusFunctionCode::WriteSingleRegister:
    case ModbusFunctionCode::WriteSingleCoil:
        // 回显请求
        return 12;

    case ModbusFunctionCode::WriteMultipleRegisters:
    case ModbusFunctionCode::WriteMultipleCoils:
        // MBAP头(7) + 功能码(1) + 地址(2) + 数量(2)
        return 12;

    default:
        return -1;
    }
}

QString ModbusTCPProtocol::getModbusExceptionMessage(quint8 exceptionCode) const
{
    switch (exceptionCode) {
    case 0x01: return QStringLiteral("Illegal function");
    case 0x02: return QStringLiteral("Illegal data address");
    case 0x03: return QStringLiteral("Illegal data value");
    case 0x04: return QStringLiteral("Slave device failure");
    case 0x05: return QStringLiteral("Acknowledge");
    case 0x06: return QStringLiteral("Slave device busy");
    case 0x08: return QStringLiteral("Memory parity error");
    case 0x0A: return QStringLiteral("Gateway path unavailable");
    case 0x0B: return QStringLiteral("Gateway target device failed to respond");
    default:   return QStringLiteral("Unknown exception code: 0x%1").arg(exceptionCode, 2, 16, QChar('0'));
    }
}

// ============================================================
// MitsubishiMCProtocol 实现
// ============================================================

MitsubishiMCProtocol::MitsubishiMCProtocol(QObject* parent)
    : PLCProtocol(parent)
{
}

MitsubishiMCProtocol::~MitsubishiMCProtocol()
{
}

QByteArray MitsubishiMCProtocol::build3EHeader(int dataLength)
{
    QByteArray header;
    header.reserve(HEADER_LENGTH);

    // 子头 (2字节) - 请求为 0x50 0x00
    header.append(static_cast<char>(REQUEST_SUBHEADER_1));
    header.append(static_cast<char>(REQUEST_SUBHEADER_2));

    // 网络号 (1字节)
    header.append(static_cast<char>(networkNo_));

    // PC号 (1字节)
    header.append(static_cast<char>(pcNo_));

    // 请求目标模块IO编号 (2字节, 小端序)
    header.append(static_cast<char>(destModuleIO_ & 0xFF));
    header.append(static_cast<char>((destModuleIO_ >> 8) & 0xFF));

    // 请求目标模块站号 (1字节)
    header.append(static_cast<char>(destModuleStation_));

    // 请求数据长度 (2字节, 小端序) - 监视定时器(2) + 请求内容长度
    quint16 requestLength = static_cast<quint16>(2 + dataLength);
    header.append(static_cast<char>(requestLength & 0xFF));
    header.append(static_cast<char>((requestLength >> 8) & 0xFF));

    // 监视定时器 (2字节, 小端序)
    header.append(static_cast<char>(monitoringTimer_ & 0xFF));
    header.append(static_cast<char>((monitoringTimer_ >> 8) & 0xFF));

    return header;
}

QByteArray MitsubishiMCProtocol::buildDeviceAddress(RegisterType deviceType, int address)
{
    QByteArray data;
    data.reserve(4);

    // 起始地址 (3字节, 小端序)
    data.append(static_cast<char>(address & 0xFF));
    data.append(static_cast<char>((address >> 8) & 0xFF));
    data.append(static_cast<char>((address >> 16) & 0xFF));

    // 软元件代码 (1字节)
    data.append(static_cast<char>(getDeviceCode(deviceType)));

    return data;
}

quint8 MitsubishiMCProtocol::getDeviceCode(RegisterType deviceType) const
{
    return getMCDeviceCode(deviceType);
}

bool MitsubishiMCProtocol::isBitDevice(RegisterType deviceType) const
{
    return isBitRegister(deviceType);
}

QByteArray MitsubishiMCProtocol::packReadRequest(RegisterType regType, int address, int count)
{
    if (isBitDevice(regType)) {
        return packBatchReadBit(regType, address, count);
    } else {
        return packBatchReadWord(regType, address, count);
    }
}

QByteArray MitsubishiMCProtocol::packWriteRequest(RegisterType regType, int address,
                                                   const std::vector<uint16_t>& values)
{
    return packBatchWriteWord(regType, address, values);
}

QByteArray MitsubishiMCProtocol::packWriteBitRequest(RegisterType regType, int address,
                                                      const std::vector<bool>& values)
{
    return packBatchWriteBit(regType, address, values);
}

QByteArray MitsubishiMCProtocol::packBatchReadWord(RegisterType deviceType, int address, int count)
{
    QByteArray requestData;

    // 命令 (2字节, 小端序) - 批量读取 0x0401
    requestData.append(static_cast<char>(MCCommand::BatchRead & 0xFF));
    requestData.append(static_cast<char>((MCCommand::BatchRead >> 8) & 0xFF));

    // 子命令 (2字节, 小端序) - 字软元件 0x0000
    requestData.append(static_cast<char>(MCCommand::SubCmdWord & 0xFF));
    requestData.append(static_cast<char>((MCCommand::SubCmdWord >> 8) & 0xFF));

    // 软元件地址 (4字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 点数 (2字节, 小端序)
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    // 构建完整帧
    return build3EHeader(requestData.size()) + requestData;
}

QByteArray MitsubishiMCProtocol::packBatchReadBit(RegisterType deviceType, int address, int count)
{
    QByteArray requestData;

    // 命令 (2字节, 小端序) - 批量读取 0x0401
    requestData.append(static_cast<char>(MCCommand::BatchRead & 0xFF));
    requestData.append(static_cast<char>((MCCommand::BatchRead >> 8) & 0xFF));

    // 子命令 (2字节, 小端序) - 位软元件 0x0001
    requestData.append(static_cast<char>(MCCommand::SubCmdBit & 0xFF));
    requestData.append(static_cast<char>((MCCommand::SubCmdBit >> 8) & 0xFF));

    // 软元件地址 (4字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 点数 (2字节, 小端序)
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    return build3EHeader(requestData.size()) + requestData;
}

QByteArray MitsubishiMCProtocol::packBatchWriteWord(RegisterType deviceType, int address,
                                                     const std::vector<uint16_t>& values)
{
    QByteArray requestData;

    // 命令 (2字节, 小端序) - 批量写入 0x1401
    requestData.append(static_cast<char>(MCCommand::BatchWrite & 0xFF));
    requestData.append(static_cast<char>((MCCommand::BatchWrite >> 8) & 0xFF));

    // 子命令 (2字节, 小端序) - 字软元件 0x0000
    requestData.append(static_cast<char>(MCCommand::SubCmdWord & 0xFF));
    requestData.append(static_cast<char>((MCCommand::SubCmdWord >> 8) & 0xFF));

    // 软元件地址 (4字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 点数 (2字节, 小端序)
    int count = static_cast<int>(values.size());
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    // 写入数据 (每个值2字节, 小端序)
    for (uint16_t value : values) {
        requestData.append(static_cast<char>(value & 0xFF));
        requestData.append(static_cast<char>((value >> 8) & 0xFF));
    }

    return build3EHeader(requestData.size()) + requestData;
}

QByteArray MitsubishiMCProtocol::packBatchWriteBit(RegisterType deviceType, int address,
                                                    const std::vector<bool>& values)
{
    QByteArray requestData;

    // 命令 (2字节, 小端序) - 批量写入 0x1401
    requestData.append(static_cast<char>(MCCommand::BatchWrite & 0xFF));
    requestData.append(static_cast<char>((MCCommand::BatchWrite >> 8) & 0xFF));

    // 子命令 (2字节, 小端序) - 位软元件 0x0001
    requestData.append(static_cast<char>(MCCommand::SubCmdBit & 0xFF));
    requestData.append(static_cast<char>((MCCommand::SubCmdBit >> 8) & 0xFF));

    // 软元件地址 (4字节)
    requestData.append(buildDeviceAddress(deviceType, address));

    // 点数 (2字节, 小端序)
    int count = static_cast<int>(values.size());
    requestData.append(static_cast<char>(count & 0xFF));
    requestData.append(static_cast<char>((count >> 8) & 0xFF));

    // 写入数据 (每个位占半字节, 0x10=ON, 0x00=OFF)
    for (size_t i = 0; i < values.size(); i += 2) {
        uint8_t byte = 0;
        // 低半字节
        byte |= values[i] ? 0x10 : 0x00;
        // 高半字节 (如果存在)
        if (i + 1 < values.size()) {
            byte |= values[i + 1] ? 0x01 : 0x00;
        }
        requestData.append(static_cast<char>(byte));
    }

    return build3EHeader(requestData.size()) + requestData;
}

PLCResult MitsubishiMCProtocol::parseResponse(const QByteArray& response,
                                               RegisterType expectedType)
{
    PLCResult result;
    result.timestamp = QDateTime::currentDateTime();
    result.rawData = response;

    // 最小长度检查
    if (response.size() < MIN_RESPONSE_LENGTH) {
        result.success = false;
        result.errorCode = ErrorCode::DataError;
        result.errorMessage = QStringLiteral("Response too short: %1 bytes").arg(response.size());
        return result;
    }

    // 检查响应子头
    if (static_cast<uint8_t>(response[0]) != RESPONSE_SUBHEADER_1 ||
        static_cast<uint8_t>(response[1]) != RESPONSE_SUBHEADER_2) {
        result.success = false;
        result.errorCode = ErrorCode::ProtocolError;
        result.errorMessage = QStringLiteral("Invalid response subheader");
        return result;
    }

    // 获取响应数据长度 (偏移7-8, 小端序)
    quint16 dataLength = static_cast<uint8_t>(response[7]) |
                         (static_cast<uint8_t>(response[8]) << 8);

    // 检查完整性
    if (response.size() < 9 + dataLength) {
        result.success = false;
        result.errorCode = ErrorCode::DataError;
        result.errorMessage = QStringLiteral("Incomplete response data");
        return result;
    }

    // 获取完成代码 (偏移9-10, 小端序)
    quint16 completionCode = static_cast<uint8_t>(response[9]) |
                             (static_cast<uint8_t>(response[10]) << 8);

    if (completionCode != 0x0000) {
        result.success = false;
        result.errorCode = ErrorCode::MCCommandError;
        result.errorMessage = getCompletionCodeMessage(completionCode);
        return result;
    }

    // 解析响应数据 (从偏移11开始)
    int dataOffset = 11;
    int dataSize = response.size() - dataOffset;

    if (dataSize > 0) {
        // 判断是位数据还是字数据
        if (isBitDevice(expectedType)) {
            // 位数据 - 每半字节一个位
            for (int i = 0; i < dataSize; ++i) {
                uint8_t byte = static_cast<uint8_t>(response[dataOffset + i]);
                // 低半字节
                result.bitValues.push_back((byte & 0x10) != 0);
                // 高半字节
                result.bitValues.push_back((byte & 0x01) != 0);
            }
        } else {
            // 字数据 (小端序)
            for (int i = 0; i + 1 < dataSize; i += 2) {
                uint16_t value = static_cast<uint8_t>(response[dataOffset + i]) |
                                 (static_cast<uint8_t>(response[dataOffset + i + 1]) << 8);
                result.uint16Values.push_back(value);
                result.int16Values.push_back(static_cast<int16_t>(value));
            }
        }
    }

    result.success = true;
    return result;
}

bool MitsubishiMCProtocol::isResponseComplete(const QByteArray& data) const
{
    if (data.size() < MIN_RESPONSE_LENGTH) {
        return false;
    }

    // 获取响应数据长度
    quint16 dataLength = static_cast<uint8_t>(data[7]) |
                         (static_cast<uint8_t>(data[8]) << 8);

    // 完整帧长度 = 子头(2) + 网络号(1) + PC号(1) + 模块IO(2) + 站号(1) + 长度(2) + 数据
    return data.size() >= 9 + dataLength;
}

int MitsubishiMCProtocol::getExpectedResponseLength(const QByteArray& request) const
{
    if (request.size() < 21) {
        return -1;
    }

    // 获取命令
    quint16 command = static_cast<uint8_t>(request[11]) |
                      (static_cast<uint8_t>(request[12]) << 8);

    // 获取点数
    quint16 pointCount = static_cast<uint8_t>(request[19]) |
                         (static_cast<uint8_t>(request[20]) << 8);

    // 获取子命令判断是位还是字
    quint16 subCommand = static_cast<uint8_t>(request[13]) |
                         (static_cast<uint8_t>(request[14]) << 8);

    int dataSize = 0;
    if (command == MCCommand::BatchRead) {
        if (subCommand == MCCommand::SubCmdWord) {
            dataSize = pointCount * 2;  // 字数据
        } else {
            dataSize = (pointCount + 1) / 2;  // 位数据
        }
    }
    // 写入响应只有完成代码

    // 子头(2) + 网络号(1) + PC号(1) + 模块IO(2) + 站号(1) + 长度(2) + 完成代码(2) + 数据
    return 11 + dataSize;
}

QString MitsubishiMCProtocol::getCompletionCodeMessage(quint16 code) const
{
    switch (code) {
    case 0x0000: return QStringLiteral("Success");
    case 0xC050: return QStringLiteral("Wrong command");
    case 0xC051: return QStringLiteral("Wrong subcommand");
    case 0xC052: return QStringLiteral("Wrong parameter");
    case 0xC053: return QStringLiteral("Wrong device type");
    case 0xC054: return QStringLiteral("Wrong data");
    case 0xC055: return QStringLiteral("Wrong register");
    case 0xC056: return QStringLiteral("Request data length error");
    case 0xC059: return QStringLiteral("MC protocol error");
    case 0xC05B: return QStringLiteral("CPU unit error");
    case 0xC05C: return QStringLiteral("Encryption error");
    case 0xC05F: return QStringLiteral("Request cannot be executed");
    case 0xC060: return QStringLiteral("Wrong request target");
    case 0xC061: return QStringLiteral("Routing error");
    case 0xC06F: return QStringLiteral("CPU timeout");
    default:
        return QStringLiteral("Unknown error code: 0x%1").arg(code, 4, 16, QChar('0'));
    }
}

// ============================================================
// PLCProtocolFactory 实现
// ============================================================

std::unique_ptr<PLCProtocol> PLCProtocolFactory::create(ProtocolType type, QObject* parent)
{
    switch (type) {
    case ProtocolType::ModbusTCP:
        return std::make_unique<ModbusTCPProtocol>(parent);

    case ProtocolType::MitsubishiMC3E:
        return std::make_unique<MitsubishiMCProtocol>(parent);

    // 其他协议可以在此扩展
    default:
        return nullptr;
    }
}

std::vector<ProtocolType> PLCProtocolFactory::supportedProtocols()
{
    return {
        ProtocolType::ModbusTCP,
        ProtocolType::MitsubishiMC3E,
    };
}

bool PLCProtocolFactory::isSupported(ProtocolType type)
{
    auto supported = supportedProtocols();
    return std::find(supported.begin(), supported.end(), type) != supported.end();
}

} // namespace PLC
} // namespace VisionForge
