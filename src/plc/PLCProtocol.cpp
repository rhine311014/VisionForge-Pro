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
// ModbusRTUProtocol 实现
// ============================================================

// CRC16查找表 (Modbus多项式 0xA001)
static const quint16 CRC16_TABLE[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

ModbusRTUProtocol::ModbusRTUProtocol(QObject* parent)
    : PLCProtocol(parent)
{
}

ModbusRTUProtocol::~ModbusRTUProtocol()
{
}

quint16 ModbusRTUProtocol::calculateCRC16(const QByteArray& data)
{
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        quint8 byte = static_cast<quint8>(data[i]);
        quint8 index = (crc ^ byte) & 0xFF;
        crc = (crc >> 8) ^ CRC16_TABLE[index];
    }
    return crc;
}

bool ModbusRTUProtocol::verifyCRC16(const QByteArray& frame)
{
    if (frame.size() < 3) {
        return false;
    }

    // 计算除CRC外的数据的CRC
    QByteArray data = frame.left(frame.size() - 2);
    quint16 calculatedCRC = calculateCRC16(data);

    // 获取帧中的CRC (低字节在前)
    quint16 frameCRC = static_cast<uint8_t>(frame[frame.size() - 2]) |
                       (static_cast<uint8_t>(frame[frame.size() - 1]) << 8);

    return calculatedCRC == frameCRC;
}

QByteArray ModbusRTUProtocol::buildRTUFrame(const QByteArray& pdu)
{
    QByteArray frame;
    frame.reserve(pdu.size() + 3);

    // 从站地址
    frame.append(static_cast<char>(config_.slaveId));

    // PDU数据
    frame.append(pdu);

    // 计算CRC
    quint16 crc = calculateCRC16(frame);

    // 添加CRC (低字节在前)
    frame.append(static_cast<char>(crc & 0xFF));
    frame.append(static_cast<char>((crc >> 8) & 0xFF));

    return frame;
}

quint8 ModbusRTUProtocol::getFunctionCodeForRead(RegisterType regType)
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

quint8 ModbusRTUProtocol::getFunctionCodeForWrite(RegisterType regType, bool single)
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

QByteArray ModbusRTUProtocol::packReadRequest(RegisterType regType, int address, int count)
{
    quint8 functionCode = getFunctionCodeForRead(regType);
    lastFunctionCode_ = functionCode;
    lastRequestCount_ = count;

    QByteArray pdu;
    pdu.append(static_cast<char>(functionCode));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));

    return buildRTUFrame(pdu);
}

QByteArray ModbusRTUProtocol::packWriteRequest(RegisterType regType, int address,
                                                const std::vector<uint16_t>& values)
{
    if (values.empty()) {
        return QByteArray();
    }

    if (values.size() == 1) {
        return packWriteSingleRegister(address, values[0]);
    } else {
        return packWriteMultipleRegisters(address, values);
    }
}

QByteArray ModbusRTUProtocol::packWriteBitRequest(RegisterType regType, int address,
                                                   const std::vector<bool>& values)
{
    if (values.empty()) {
        return QByteArray();
    }

    if (values.size() == 1) {
        return packWriteSingleCoil(address, values[0]);
    } else {
        return packWriteMultipleCoils(address, values);
    }
}

QByteArray ModbusRTUProtocol::packReadHoldingRegisters(int address, int count)
{
    lastFunctionCode_ = ModbusFunctionCode::ReadHoldingRegisters;
    lastRequestCount_ = count;

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::ReadHoldingRegisters));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));

    return buildRTUFrame(pdu);
}

QByteArray ModbusRTUProtocol::packWriteSingleRegister(int address, uint16_t value)
{
    lastFunctionCode_ = ModbusFunctionCode::WriteSingleRegister;

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::WriteSingleRegister));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((value >> 8) & 0xFF));
    pdu.append(static_cast<char>(value & 0xFF));

    return buildRTUFrame(pdu);
}

QByteArray ModbusRTUProtocol::packWriteMultipleRegisters(int address,
                                                          const std::vector<uint16_t>& values)
{
    lastFunctionCode_ = ModbusFunctionCode::WriteMultipleRegisters;

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

    return buildRTUFrame(pdu);
}

QByteArray ModbusRTUProtocol::packReadCoils(int address, int count)
{
    lastFunctionCode_ = ModbusFunctionCode::ReadCoils;
    lastRequestCount_ = count;

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::ReadCoils));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));

    return buildRTUFrame(pdu);
}

QByteArray ModbusRTUProtocol::packWriteSingleCoil(int address, bool value)
{
    lastFunctionCode_ = ModbusFunctionCode::WriteSingleCoil;

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::WriteSingleCoil));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>(value ? 0xFF : 0x00));
    pdu.append(static_cast<char>(0x00));

    return buildRTUFrame(pdu);
}

QByteArray ModbusRTUProtocol::packWriteMultipleCoils(int address, const std::vector<bool>& values)
{
    lastFunctionCode_ = ModbusFunctionCode::WriteMultipleCoils;

    int count = static_cast<int>(values.size());
    int byteCount = (count + 7) / 8;

    QByteArray pdu;
    pdu.append(static_cast<char>(ModbusFunctionCode::WriteMultipleCoils));
    pdu.append(static_cast<char>((address >> 8) & 0xFF));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>((count >> 8) & 0xFF));
    pdu.append(static_cast<char>(count & 0xFF));
    pdu.append(static_cast<char>(byteCount));

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

    return buildRTUFrame(pdu);
}

PLCResult ModbusRTUProtocol::parseResponse(const QByteArray& response,
                                            RegisterType expectedType)
{
    PLCResult result;
    result.timestamp = QDateTime::currentDateTime();
    result.rawData = response;

    // 最小长度检查 (从站地址 + 功能码 + CRC)
    if (response.size() < 5) {
        result.success = false;
        result.errorCode = ErrorCode::DataError;
        result.errorMessage = QStringLiteral("Response too short: %1 bytes").arg(response.size());
        return result;
    }

    // CRC校验
    if (!verifyCRC16(response)) {
        result.success = false;
        result.errorCode = ErrorCode::DataError;
        result.errorMessage = QStringLiteral("CRC check failed");
        return result;
    }

    // 检查从站地址
    quint8 slaveId = static_cast<uint8_t>(response[0]);
    if (slaveId != config_.slaveId) {
        result.success = false;
        result.errorCode = ErrorCode::ProtocolError;
        result.errorMessage = QStringLiteral("Slave ID mismatch: expected %1, got %2")
                                .arg(config_.slaveId).arg(slaveId);
        return result;
    }

    // 获取功能码
    quint8 functionCode = static_cast<uint8_t>(response[1]);

    // 检查异常响应 (功能码最高位为1)
    if (functionCode & 0x80) {
        quint8 exceptionCode = static_cast<uint8_t>(response[2]);
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
        if (response.size() < 5) {
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Invalid response length");
            return result;
        }

        int byteCount = static_cast<uint8_t>(response[2]);
        if (response.size() < 3 + byteCount + 2) {  // 从站地址 + 功能码 + 字节数 + 数据 + CRC
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Incomplete register data");
            return result;
        }

        // 解析寄存器数据
        for (int i = 0; i < byteCount; i += 2) {
            uint16_t value = (static_cast<uint8_t>(response[3 + i]) << 8) |
                             static_cast<uint8_t>(response[3 + i + 1]);
            result.uint16Values.push_back(value);
            result.int16Values.push_back(static_cast<int16_t>(value));
        }
        result.success = true;
        break;
    }

    case ModbusFunctionCode::ReadCoils:
    case ModbusFunctionCode::ReadDiscreteInputs:
    {
        if (response.size() < 5) {
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Invalid response length");
            return result;
        }

        int byteCount = static_cast<uint8_t>(response[2]);
        if (response.size() < 3 + byteCount + 2) {
            result.success = false;
            result.errorCode = ErrorCode::DataError;
            result.errorMessage = QStringLiteral("Incomplete coil data");
            return result;
        }

        // 解析位数据
        for (int i = 0; i < byteCount; ++i) {
            uint8_t byte = static_cast<uint8_t>(response[3 + i]);
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

bool ModbusRTUProtocol::isResponseComplete(const QByteArray& data) const
{
    if (data.size() < 5) {
        return false;
    }

    quint8 functionCode = static_cast<uint8_t>(data[1]);

    // 异常响应固定5字节
    if (functionCode & 0x80) {
        return data.size() >= 5;
    }

    switch (functionCode) {
    case ModbusFunctionCode::ReadHoldingRegisters:
    case ModbusFunctionCode::ReadInputRegisters:
    case ModbusFunctionCode::ReadCoils:
    case ModbusFunctionCode::ReadDiscreteInputs:
    {
        if (data.size() < 3) return false;
        int byteCount = static_cast<uint8_t>(data[2]);
        // 从站地址(1) + 功能码(1) + 字节数(1) + 数据(N) + CRC(2)
        return data.size() >= 3 + byteCount + 2;
    }

    case ModbusFunctionCode::WriteSingleRegister:
    case ModbusFunctionCode::WriteSingleCoil:
    case ModbusFunctionCode::WriteMultipleRegisters:
    case ModbusFunctionCode::WriteMultipleCoils:
        // 写响应固定8字节
        return data.size() >= 8;

    default:
        return false;
    }
}

int ModbusRTUProtocol::getExpectedResponseLength(const QByteArray& request) const
{
    if (request.size() < 8) {
        return -1;
    }

    quint8 functionCode = static_cast<uint8_t>(request[1]);

    switch (functionCode) {
    case ModbusFunctionCode::ReadHoldingRegisters:
    case ModbusFunctionCode::ReadInputRegisters:
    {
        quint16 count = (static_cast<uint8_t>(request[4]) << 8) |
                        static_cast<uint8_t>(request[5]);
        // 从站地址(1) + 功能码(1) + 字节数(1) + 数据(count*2) + CRC(2)
        return 5 + count * 2;
    }

    case ModbusFunctionCode::ReadCoils:
    case ModbusFunctionCode::ReadDiscreteInputs:
    {
        quint16 count = (static_cast<uint8_t>(request[4]) << 8) |
                        static_cast<uint8_t>(request[5]);
        int byteCount = (count + 7) / 8;
        return 5 + byteCount;
    }

    case ModbusFunctionCode::WriteSingleRegister:
    case ModbusFunctionCode::WriteSingleCoil:
    case ModbusFunctionCode::WriteMultipleRegisters:
    case ModbusFunctionCode::WriteMultipleCoils:
        return 8;

    default:
        return -1;
    }
}

QString ModbusRTUProtocol::getModbusExceptionMessage(quint8 exceptionCode) const
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
// OmronFINSProtocol 实现
// ============================================================

OmronFINSProtocol::OmronFINSProtocol(QObject* parent)
    : PLCProtocol(parent)
{
}

OmronFINSProtocol::~OmronFINSProtocol()
{
}

QByteArray OmronFINSProtocol::buildFINSTCPHeader(int finsDataLength, quint32 command)
{
    QByteArray header;
    header.reserve(16);

    // 魔术字 "FINS" (4字节)
    header.append('F');
    header.append('I');
    header.append('N');
    header.append('S');

    // 长度 (4字节, 大端序) - 命令(4) + 错误码(4) + FINS数据
    quint32 length = 8 + finsDataLength;
    header.append(static_cast<char>((length >> 24) & 0xFF));
    header.append(static_cast<char>((length >> 16) & 0xFF));
    header.append(static_cast<char>((length >> 8) & 0xFF));
    header.append(static_cast<char>(length & 0xFF));

    // 命令 (4字节, 大端序)
    header.append(static_cast<char>((command >> 24) & 0xFF));
    header.append(static_cast<char>((command >> 16) & 0xFF));
    header.append(static_cast<char>((command >> 8) & 0xFF));
    header.append(static_cast<char>(command & 0xFF));

    // 错误码 (4字节) - 请求时为0
    header.append(static_cast<char>(0x00));
    header.append(static_cast<char>(0x00));
    header.append(static_cast<char>(0x00));
    header.append(static_cast<char>(0x00));

    return header;
}

QByteArray OmronFINSProtocol::buildFINSHeader()
{
    QByteArray header;
    header.reserve(10);

    // ICF - 信息控制字段 (0x80 = 命令, 需要响应)
    header.append(static_cast<char>(ICF_COMMAND));

    // RSV - 保留
    header.append(static_cast<char>(0x00));

    // GCT - 网关计数 (最多经过的网关数)
    header.append(static_cast<char>(0x02));

    // DNA - 目标网络地址
    header.append(static_cast<char>(destNetwork_));

    // DA1 - 目标节点地址
    header.append(static_cast<char>(destNode_));

    // DA2 - 目标单元地址
    header.append(static_cast<char>(destUnit_));

    // SNA - 源网络地址
    header.append(static_cast<char>(sourceNetwork_));

    // SA1 - 源节点地址
    header.append(static_cast<char>(sourceNode_));

    // SA2 - 源单元地址
    header.append(static_cast<char>(sourceUnit_));

    // SID - 服务ID
    header.append(static_cast<char>(nextSID()));

    return header;
}

QByteArray OmronFINSProtocol::packHandshakeRequest()
{
    // 握手请求帧格式:
    // FINS TCP头 (16字节): 魔术字 + 长度 + 命令(0) + 错误码
    // 客户端节点 (4字节): 0x00000000 (请求自动分配)

    QByteArray request;
    request.reserve(20);

    // 魔术字 "FINS"
    request.append('F');
    request.append('I');
    request.append('N');
    request.append('S');

    // 长度 (4字节) = 命令(4) + 错误码(4) + 客户端节点(4) = 12
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x0C));

    // 命令 (4字节) = 0 (握手请求)
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));

    // 错误码 (4字节) = 0
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));

    // 客户端节点 (4字节) = 0 (请求自动分配)
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));
    request.append(static_cast<char>(0x00));

    return request;
}

bool OmronFINSProtocol::parseHandshakeResponse(const QByteArray& response)
{
    // 握手响应格式:
    // FINS TCP头 (16字节)
    // 客户端节点 (4字节)
    // 服务器节点 (4字节)

    if (response.size() < 24) {
        return false;
    }

    // 检查魔术字
    if (response[0] != 'F' || response[1] != 'I' ||
        response[2] != 'N' || response[3] != 'S') {
        return false;
    }

    // 检查命令 (应为1 = 握手响应)
    quint32 command = (static_cast<uint8_t>(response[8]) << 24) |
                      (static_cast<uint8_t>(response[9]) << 16) |
                      (static_cast<uint8_t>(response[10]) << 8) |
                      static_cast<uint8_t>(response[11]);
    if (command != 1) {
        return false;
    }

    // 检查错误码
    quint32 errorCode = (static_cast<uint8_t>(response[12]) << 24) |
                        (static_cast<uint8_t>(response[13]) << 16) |
                        (static_cast<uint8_t>(response[14]) << 8) |
                        static_cast<uint8_t>(response[15]);
    if (errorCode != 0) {
        return false;
    }

    // 获取分配的客户端节点地址
    sourceNode_ = static_cast<uint8_t>(response[19]);

    // 获取服务器节点地址
    destNode_ = static_cast<uint8_t>(response[23]);

    handshakeComplete_ = true;
    return true;
}

quint8 OmronFINSProtocol::getMemoryAreaCode(RegisterType regType)
{
    switch (regType) {
    case RegisterType::DM:  return AREA_DM_WORD;
    case RegisterType::CIO: return AREA_CIO_WORD;
    case RegisterType::WR:  return AREA_WR_WORD;
    case RegisterType::HR:  return AREA_HR_WORD;
    case RegisterType::AR:  return AREA_AR_WORD;
    case RegisterType::EM:  return AREA_EM_WORD;
    // Modbus映射
    case RegisterType::HoldingRegister: return AREA_DM_WORD;
    case RegisterType::InputRegister:   return AREA_DM_WORD;
    case RegisterType::Coil:            return AREA_CIO_BIT;
    default: return AREA_DM_WORD;
    }
}

bool OmronFINSProtocol::isBitArea(RegisterType regType)
{
    switch (regType) {
    case RegisterType::Coil:
    case RegisterType::DiscreteInput:
        return true;
    default:
        return false;
    }
}

QByteArray OmronFINSProtocol::packMemoryAreaRead(quint8 areaCode, int address, int count)
{
    lastCommand_ = CMD_MEMORY_AREA_READ;
    lastRequestCount_ = count;

    // FINS命令数据:
    // 命令码 (2字节): 0x01 0x01
    // 内存区域码 (1字节)
    // 起始地址 (3字节): 高字节 + 低字节 + 位地址
    // 数量 (2字节)

    QByteArray cmdData;
    cmdData.reserve(8);

    // 命令码 - 0x0101 (Memory Area Read)
    cmdData.append(static_cast<char>(0x01));
    cmdData.append(static_cast<char>(0x01));

    // 内存区域码
    cmdData.append(static_cast<char>(areaCode));

    // 起始地址 (高字节)
    cmdData.append(static_cast<char>((address >> 8) & 0xFF));
    // 起始地址 (低字节)
    cmdData.append(static_cast<char>(address & 0xFF));
    // 位地址 (字操作时为0)
    cmdData.append(static_cast<char>(0x00));

    // 读取数量
    cmdData.append(static_cast<char>((count >> 8) & 0xFF));
    cmdData.append(static_cast<char>(count & 0xFF));

    // 构建完整帧
    QByteArray finsHeader = buildFINSHeader();
    int finsDataLength = finsHeader.size() + cmdData.size();

    return buildFINSTCPHeader(finsDataLength) + finsHeader + cmdData;
}

QByteArray OmronFINSProtocol::packMemoryAreaWrite(quint8 areaCode, int address,
                                                    const std::vector<uint16_t>& values)
{
    lastCommand_ = CMD_MEMORY_AREA_WRITE;
    lastRequestCount_ = static_cast<int>(values.size());

    QByteArray cmdData;
    cmdData.reserve(8 + values.size() * 2);

    // 命令码 - 0x0102 (Memory Area Write)
    cmdData.append(static_cast<char>(0x01));
    cmdData.append(static_cast<char>(0x02));

    // 内存区域码
    cmdData.append(static_cast<char>(areaCode));

    // 起始地址
    cmdData.append(static_cast<char>((address >> 8) & 0xFF));
    cmdData.append(static_cast<char>(address & 0xFF));
    cmdData.append(static_cast<char>(0x00));  // 位地址

    // 写入数量
    int count = static_cast<int>(values.size());
    cmdData.append(static_cast<char>((count >> 8) & 0xFF));
    cmdData.append(static_cast<char>(count & 0xFF));

    // 写入数据 (大端序)
    for (uint16_t value : values) {
        cmdData.append(static_cast<char>((value >> 8) & 0xFF));
        cmdData.append(static_cast<char>(value & 0xFF));
    }

    // 构建完整帧
    QByteArray finsHeader = buildFINSHeader();
    int finsDataLength = finsHeader.size() + cmdData.size();

    return buildFINSTCPHeader(finsDataLength) + finsHeader + cmdData;
}

QByteArray OmronFINSProtocol::packReadRequest(RegisterType regType, int address, int count)
{
    quint8 areaCode = getMemoryAreaCode(regType);
    return packMemoryAreaRead(areaCode, address, count);
}

QByteArray OmronFINSProtocol::packWriteRequest(RegisterType regType, int address,
                                                 const std::vector<uint16_t>& values)
{
    if (values.empty()) {
        return QByteArray();
    }

    quint8 areaCode = getMemoryAreaCode(regType);
    return packMemoryAreaWrite(areaCode, address, values);
}

QByteArray OmronFINSProtocol::packWriteBitRequest(RegisterType regType, int address,
                                                    const std::vector<bool>& values)
{
    if (values.empty()) {
        return QByteArray();
    }

    // 将位值转换为字值 (每16位一个字)
    std::vector<uint16_t> wordValues;
    int wordCount = (static_cast<int>(values.size()) + 15) / 16;
    wordValues.reserve(wordCount);

    for (int w = 0; w < wordCount; ++w) {
        uint16_t word = 0;
        for (int b = 0; b < 16; ++b) {
            int index = w * 16 + b;
            if (index < static_cast<int>(values.size()) && values[index]) {
                word |= (1 << b);
            }
        }
        wordValues.push_back(word);
    }

    quint8 areaCode = getMemoryAreaCode(regType);
    return packMemoryAreaWrite(areaCode, address, wordValues);
}

bool OmronFINSProtocol::parseFINSResponse(const QByteArray& response, PLCResult& result)
{
    // 检查最小长度: TCP头(16) + FINS头(10) + 响应码(2)
    if (response.size() < 28) {
        result.success = false;
        result.errorCode = ErrorCode::DataError;
        result.errorMessage = QStringLiteral("Response too short");
        return false;
    }

    // 提取FINS响应码
    int finsDataOffset = FINS_TCP_HEADER_SIZE + FINS_FRAME_HEADER_SIZE;
    quint8 mainCode = static_cast<uint8_t>(response[finsDataOffset]);
    quint8 subCode = static_cast<uint8_t>(response[finsDataOffset + 1]);

    // 检查响应码
    if (mainCode != 0 || subCode != 0) {
        result.success = false;
        if (mainCode >= 0x01 && mainCode <= 0x03) {
            result.errorCode = ErrorCode::FINSLocalNodeError;
        } else if (mainCode >= 0x04 && mainCode <= 0x08) {
            result.errorCode = ErrorCode::FINSDestNodeError;
        } else if (mainCode >= 0x10 && mainCode <= 0x15) {
            result.errorCode = ErrorCode::FINSControllerError;
        } else {
            result.errorCode = ErrorCode::ProtocolError;
        }
        result.errorMessage = getFINSErrorMessage(mainCode, subCode);
        return false;
    }

    // 解析数据
    int dataOffset = finsDataOffset + 2;  // 跳过响应码
    int dataSize = response.size() - dataOffset;

    if (dataSize > 0 && lastCommand_ == CMD_MEMORY_AREA_READ) {
        // 解析读取的数据 (大端序)
        for (int i = 0; i + 1 < dataSize; i += 2) {
            uint16_t value = (static_cast<uint8_t>(response[dataOffset + i]) << 8) |
                             static_cast<uint8_t>(response[dataOffset + i + 1]);
            result.uint16Values.push_back(value);
            result.int16Values.push_back(static_cast<int16_t>(value));
        }
    }

    result.success = true;
    return true;
}

PLCResult OmronFINSProtocol::parseResponse(const QByteArray& response, RegisterType expectedType)
{
    PLCResult result;
    result.timestamp = QDateTime::currentDateTime();
    result.rawData = response;

    // 最小长度检查
    if (response.size() < static_cast<int>(FINS_TCP_HEADER_SIZE)) {
        result.success = false;
        result.errorCode = ErrorCode::DataError;
        result.errorMessage = QStringLiteral("Response too short: %1 bytes").arg(response.size());
        return result;
    }

    // 检查魔术字
    if (response[0] != 'F' || response[1] != 'I' ||
        response[2] != 'N' || response[3] != 'S') {
        result.success = false;
        result.errorCode = ErrorCode::ProtocolError;
        result.errorMessage = QStringLiteral("Invalid FINS magic");
        return result;
    }

    // 获取命令类型
    quint32 command = (static_cast<uint8_t>(response[8]) << 24) |
                      (static_cast<uint8_t>(response[9]) << 16) |
                      (static_cast<uint8_t>(response[10]) << 8) |
                      static_cast<uint8_t>(response[11]);

    // 获取错误码
    quint32 tcpErrorCode = (static_cast<uint8_t>(response[12]) << 24) |
                           (static_cast<uint8_t>(response[13]) << 16) |
                           (static_cast<uint8_t>(response[14]) << 8) |
                           static_cast<uint8_t>(response[15]);

    if (tcpErrorCode != 0) {
        result.success = false;
        result.errorCode = ErrorCode::ConnectionFailed;
        result.errorMessage = QStringLiteral("FINS TCP error: 0x%1").arg(tcpErrorCode, 8, 16, QChar('0'));
        return result;
    }

    // 如果是握手响应
    if (command == 1) {
        if (parseHandshakeResponse(response)) {
            result.success = true;
        } else {
            result.success = false;
            result.errorCode = ErrorCode::ProtocolError;
            result.errorMessage = QStringLiteral("Handshake failed");
        }
        return result;
    }

    // FINS帧响应
    if (command == 2) {
        parseFINSResponse(response, result);
    } else {
        result.success = false;
        result.errorCode = ErrorCode::ProtocolError;
        result.errorMessage = QStringLiteral("Unknown command: %1").arg(command);
    }

    return result;
}

bool OmronFINSProtocol::isResponseComplete(const QByteArray& data) const
{
    if (data.size() < static_cast<int>(FINS_TCP_HEADER_SIZE)) {
        return false;
    }

    // 从TCP头获取长度
    quint32 length = (static_cast<uint8_t>(data[4]) << 24) |
                     (static_cast<uint8_t>(data[5]) << 16) |
                     (static_cast<uint8_t>(data[6]) << 8) |
                     static_cast<uint8_t>(data[7]);

    // 完整帧长度 = 魔术字(4) + 长度字段(4) + 数据长度
    return data.size() >= static_cast<int>(8 + length);
}

int OmronFINSProtocol::getExpectedResponseLength(const QByteArray& request) const
{
    if (request.size() < static_cast<int>(FINS_TCP_HEADER_SIZE + FINS_FRAME_HEADER_SIZE + 2)) {
        return -1;
    }

    // 获取命令类型
    quint32 command = (static_cast<uint8_t>(request[8]) << 24) |
                      (static_cast<uint8_t>(request[9]) << 16) |
                      (static_cast<uint8_t>(request[10]) << 8) |
                      static_cast<uint8_t>(request[11]);

    // 握手响应
    if (command == 0) {
        return 24;  // TCP头(16) + 客户端节点(4) + 服务器节点(4)
    }

    // FINS帧响应
    if (command == 2) {
        int cmdDataOffset = FINS_TCP_HEADER_SIZE + FINS_FRAME_HEADER_SIZE;
        quint8 cmd1 = static_cast<uint8_t>(request[cmdDataOffset]);
        quint8 cmd2 = static_cast<uint8_t>(request[cmdDataOffset + 1]);
        quint16 cmdCode = (cmd1 << 8) | cmd2;

        if (cmdCode == CMD_MEMORY_AREA_READ) {
            // 获取读取数量
            quint16 count = (static_cast<uint8_t>(request[cmdDataOffset + 6]) << 8) |
                            static_cast<uint8_t>(request[cmdDataOffset + 7]);
            // TCP头(16) + FINS头(10) + 响应码(2) + 数据(count*2)
            return FINS_TCP_HEADER_SIZE + FINS_FRAME_HEADER_SIZE + 2 + count * 2;
        } else if (cmdCode == CMD_MEMORY_AREA_WRITE) {
            // 写入响应只有响应码
            return FINS_TCP_HEADER_SIZE + FINS_FRAME_HEADER_SIZE + 2;
        }
    }

    return -1;
}

QString OmronFINSProtocol::getFINSErrorMessage(quint8 mainCode, quint8 subCode)
{
    // FINS错误码解析
    switch (mainCode) {
    case 0x00:
        return QStringLiteral("Success");

    case 0x01:
        switch (subCode) {
        case 0x01: return QStringLiteral("Local node not in network");
        case 0x02: return QStringLiteral("Token timeout");
        case 0x03: return QStringLiteral("Retries failed");
        case 0x04: return QStringLiteral("Too many send frames");
        case 0x05: return QStringLiteral("Node address range error");
        case 0x06: return QStringLiteral("Node address duplication");
        default:   return QStringLiteral("Local node error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x02:
        switch (subCode) {
        case 0x01: return QStringLiteral("Destination node not in network");
        case 0x02: return QStringLiteral("Unit missing");
        case 0x03: return QStringLiteral("Third node missing");
        case 0x04: return QStringLiteral("Destination node busy");
        case 0x05: return QStringLiteral("Response timeout");
        default:   return QStringLiteral("Destination node error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x03:
        switch (subCode) {
        case 0x01: return QStringLiteral("Communications controller error");
        case 0x02: return QStringLiteral("CPU unit error");
        case 0x03: return QStringLiteral("Controller error");
        case 0x04: return QStringLiteral("Unit number error");
        default:   return QStringLiteral("Controller error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x04:
        switch (subCode) {
        case 0x01: return QStringLiteral("Undefined command");
        case 0x02: return QStringLiteral("Not supported by model/version");
        default:   return QStringLiteral("Service unsupported error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x05:
        switch (subCode) {
        case 0x01: return QStringLiteral("Destination address setting error");
        case 0x02: return QStringLiteral("No routing tables");
        case 0x03: return QStringLiteral("Routing table error");
        case 0x04: return QStringLiteral("Too many relays");
        default:   return QStringLiteral("Routing table error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x10:
        switch (subCode) {
        case 0x01: return QStringLiteral("Command too long");
        case 0x02: return QStringLiteral("Command too short");
        case 0x03: return QStringLiteral("Elements/data don't match");
        case 0x04: return QStringLiteral("Command format error");
        case 0x05: return QStringLiteral("Header error");
        default:   return QStringLiteral("Command format error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x11:
        switch (subCode) {
        case 0x01: return QStringLiteral("Area classification missing");
        case 0x02: return QStringLiteral("Access size error");
        case 0x03: return QStringLiteral("Address range error");
        case 0x04: return QStringLiteral("Address range exceeded");
        case 0x06: return QStringLiteral("Program missing");
        case 0x09: return QStringLiteral("Relational error");
        case 0x0A: return QStringLiteral("Duplicate data access");
        case 0x0B: return QStringLiteral("Response too long");
        case 0x0C: return QStringLiteral("Parameter error");
        default:   return QStringLiteral("Parameter error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x20:
        switch (subCode) {
        case 0x02: return QStringLiteral("Protected");
        case 0x03: return QStringLiteral("Table missing");
        case 0x04: return QStringLiteral("Data missing");
        case 0x05: return QStringLiteral("Program missing");
        case 0x06: return QStringLiteral("File missing");
        case 0x07: return QStringLiteral("Data mismatch");
        default:   return QStringLiteral("Read not possible (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x21:
        switch (subCode) {
        case 0x01: return QStringLiteral("Read only");
        case 0x02: return QStringLiteral("Protected, cannot write during execution");
        case 0x03: return QStringLiteral("Cannot register");
        case 0x05: return QStringLiteral("Program missing");
        case 0x06: return QStringLiteral("File missing");
        case 0x07: return QStringLiteral("File name already exists");
        case 0x08: return QStringLiteral("Cannot change");
        default:   return QStringLiteral("Write not possible (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x22:
        switch (subCode) {
        case 0x01: return QStringLiteral("Not possible during execution");
        case 0x02: return QStringLiteral("Not possible while running");
        case 0x03: return QStringLiteral("Wrong PLC mode");
        case 0x04: return QStringLiteral("Wrong PLC mode");
        case 0x05: return QStringLiteral("Wrong PLC mode");
        case 0x06: return QStringLiteral("Wrong PLC mode");
        case 0x07: return QStringLiteral("Specified node not polling node");
        case 0x08: return QStringLiteral("Step cannot be executed");
        default:   return QStringLiteral("Not executable in current mode (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x23:
        switch (subCode) {
        case 0x01: return QStringLiteral("File device missing");
        case 0x02: return QStringLiteral("Memory missing");
        case 0x03: return QStringLiteral("Clock missing");
        default:   return QStringLiteral("No such device (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x25:
        switch (subCode) {
        case 0x02: return QStringLiteral("Memory error");
        case 0x03: return QStringLiteral("I/O setting error");
        case 0x04: return QStringLiteral("Too many I/O points");
        case 0x05: return QStringLiteral("CPU bus error");
        case 0x06: return QStringLiteral("I/O duplication");
        case 0x07: return QStringLiteral("I/O bus error");
        case 0x09: return QStringLiteral("SYSMAC BUS/2 error");
        case 0x0A: return QStringLiteral("CPU bus unit error");
        case 0x0D: return QStringLiteral("SYSMAC BUS no. duplication");
        case 0x0F: return QStringLiteral("Memory error");
        case 0x10: return QStringLiteral("SYSMAC BUS terminator missing");
        default:   return QStringLiteral("Cannot start/stop (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    case 0x26:
        switch (subCode) {
        case 0x01: return QStringLiteral("No protection");
        case 0x02: return QStringLiteral("Incorrect password");
        case 0x04: return QStringLiteral("Protected");
        case 0x05: return QStringLiteral("Service already executing");
        case 0x06: return QStringLiteral("Service stopped");
        case 0x07: return QStringLiteral("No execution right");
        case 0x08: return QStringLiteral("Settings not complete");
        case 0x09: return QStringLiteral("Necessary items not set");
        case 0x0A: return QStringLiteral("Number already defined");
        case 0x0B: return QStringLiteral("Error will not clear");
        default:   return QStringLiteral("Access right error (0x%1%2)")
                          .arg(mainCode, 2, 16, QChar('0'))
                          .arg(subCode, 2, 16, QChar('0'));
        }

    default:
        return QStringLiteral("FINS error code: 0x%1%2")
               .arg(mainCode, 2, 16, QChar('0'))
               .arg(subCode, 2, 16, QChar('0'));
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

    case ProtocolType::ModbusRTU:
        return std::make_unique<ModbusRTUProtocol>(parent);

    case ProtocolType::MitsubishiMC3E:
        return std::make_unique<MitsubishiMCProtocol>(parent);

    case ProtocolType::OmronFINS_TCP:
        return std::make_unique<OmronFINSProtocol>(parent);

    // 其他协议可以在此扩展
    default:
        return nullptr;
    }
}

std::vector<ProtocolType> PLCProtocolFactory::supportedProtocols()
{
    return {
        ProtocolType::ModbusTCP,
        ProtocolType::ModbusRTU,
        ProtocolType::MitsubishiMC3E,
        ProtocolType::OmronFINS_TCP,
    };
}

bool PLCProtocolFactory::isSupported(ProtocolType type)
{
    auto supported = supportedProtocols();
    return std::find(supported.begin(), supported.end(), type) != supported.end();
}

} // namespace PLC
} // namespace VisionForge
