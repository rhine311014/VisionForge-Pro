/**
 * @file PLCTypes.cpp
 * @brief PLC通信类型实现
 * @author VisionForge Team
 * @date 2025-12-21
 */

#include "plc/PLCTypes.h"
#include <QDataStream>
#include <QtEndian>
#include <cstring>

namespace VisionForge {
namespace PLC {

// ============================================================
// 字节序转换工具函数
// ============================================================

namespace ByteOrderUtils {

/**
 * @brief 将uint16数组转换为指定字节序的字节数组
 */
QByteArray uint16ToBytes(const std::vector<uint16_t>& values, ByteOrder order) {
    QByteArray result;
    result.reserve(static_cast<int>(values.size() * 2));

    for (uint16_t value : values) {
        switch (order) {
        case ByteOrder::BigEndian:
            result.append(static_cast<char>((value >> 8) & 0xFF));
            result.append(static_cast<char>(value & 0xFF));
            break;
        case ByteOrder::LittleEndian:
            result.append(static_cast<char>(value & 0xFF));
            result.append(static_cast<char>((value >> 8) & 0xFF));
            break;
        default:
            result.append(static_cast<char>((value >> 8) & 0xFF));
            result.append(static_cast<char>(value & 0xFF));
            break;
        }
    }
    return result;
}

/**
 * @brief 将字节数组转换为uint16数组
 */
std::vector<uint16_t> bytesToUint16(const QByteArray& data, ByteOrder order) {
    std::vector<uint16_t> result;
    result.reserve(data.size() / 2);

    for (int i = 0; i + 1 < data.size(); i += 2) {
        uint16_t value;
        switch (order) {
        case ByteOrder::BigEndian:
            value = (static_cast<uint8_t>(data[i]) << 8) |
                    static_cast<uint8_t>(data[i + 1]);
            break;
        case ByteOrder::LittleEndian:
            value = (static_cast<uint8_t>(data[i + 1]) << 8) |
                    static_cast<uint8_t>(data[i]);
            break;
        default:
            value = (static_cast<uint8_t>(data[i]) << 8) |
                    static_cast<uint8_t>(data[i + 1]);
            break;
        }
        result.push_back(value);
    }
    return result;
}

/**
 * @brief 将uint16数组转换为int32数组
 */
std::vector<int32_t> uint16ToInt32(const std::vector<uint16_t>& values, ByteOrder order) {
    std::vector<int32_t> result;
    result.reserve(values.size() / 2);

    for (size_t i = 0; i + 1 < values.size(); i += 2) {
        int32_t value;
        switch (order) {
        case ByteOrder::BigEndian:
            // ABCD -> AB CD
            value = (static_cast<int32_t>(values[i]) << 16) |
                    static_cast<int32_t>(values[i + 1]);
            break;
        case ByteOrder::LittleEndian:
            // DCBA -> CD AB
            value = (static_cast<int32_t>(values[i + 1]) << 16) |
                    static_cast<int32_t>(values[i]);
            break;
        case ByteOrder::BigEndianSwap:
            // CDAB -> CD AB (字交换)
            value = (static_cast<int32_t>(values[i + 1]) << 16) |
                    static_cast<int32_t>(values[i]);
            break;
        case ByteOrder::LittleEndianSwap:
            // BADC -> BA DC (字节交换后字交换)
            value = (static_cast<int32_t>(values[i]) << 16) |
                    static_cast<int32_t>(values[i + 1]);
            break;
        default:
            value = (static_cast<int32_t>(values[i]) << 16) |
                    static_cast<int32_t>(values[i + 1]);
            break;
        }
        result.push_back(value);
    }
    return result;
}

/**
 * @brief 将int32数组转换为uint16数组
 */
std::vector<uint16_t> int32ToUint16(const std::vector<int32_t>& values, ByteOrder order) {
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
        default:
            result.push_back(high);
            result.push_back(low);
            break;
        }
    }
    return result;
}

/**
 * @brief 将uint16数组转换为float数组
 */
std::vector<float> uint16ToFloat(const std::vector<uint16_t>& values, ByteOrder order) {
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
        default:
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

/**
 * @brief 将float数组转换为uint16数组
 */
std::vector<uint16_t> floatToUint16(const std::vector<float>& values, ByteOrder order) {
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
        default:
            result.push_back(high);
            result.push_back(low);
            break;
        }
    }
    return result;
}

/**
 * @brief 将uint16数组转换为double数组
 */
std::vector<double> uint16ToDouble(const std::vector<uint16_t>& values, ByteOrder order) {
    std::vector<double> result;
    result.reserve(values.size() / 4);

    for (size_t i = 0; i + 3 < values.size(); i += 4) {
        uint64_t intValue;
        switch (order) {
        case ByteOrder::BigEndian:
            intValue = (static_cast<uint64_t>(values[i]) << 48) |
                       (static_cast<uint64_t>(values[i + 1]) << 32) |
                       (static_cast<uint64_t>(values[i + 2]) << 16) |
                       static_cast<uint64_t>(values[i + 3]);
            break;
        case ByteOrder::LittleEndian:
            intValue = (static_cast<uint64_t>(values[i + 3]) << 48) |
                       (static_cast<uint64_t>(values[i + 2]) << 32) |
                       (static_cast<uint64_t>(values[i + 1]) << 16) |
                       static_cast<uint64_t>(values[i]);
            break;
        default:
            intValue = (static_cast<uint64_t>(values[i]) << 48) |
                       (static_cast<uint64_t>(values[i + 1]) << 32) |
                       (static_cast<uint64_t>(values[i + 2]) << 16) |
                       static_cast<uint64_t>(values[i + 3]);
            break;
        }

        double doubleValue;
        std::memcpy(&doubleValue, &intValue, sizeof(double));
        result.push_back(doubleValue);
    }
    return result;
}

/**
 * @brief 将double数组转换为uint16数组
 */
std::vector<uint16_t> doubleToUint16(const std::vector<double>& values, ByteOrder order) {
    std::vector<uint16_t> result;
    result.reserve(values.size() * 4);

    for (double value : values) {
        uint64_t intValue;
        std::memcpy(&intValue, &value, sizeof(double));

        uint16_t word0 = static_cast<uint16_t>((intValue >> 48) & 0xFFFF);
        uint16_t word1 = static_cast<uint16_t>((intValue >> 32) & 0xFFFF);
        uint16_t word2 = static_cast<uint16_t>((intValue >> 16) & 0xFFFF);
        uint16_t word3 = static_cast<uint16_t>(intValue & 0xFFFF);

        switch (order) {
        case ByteOrder::BigEndian:
            result.push_back(word0);
            result.push_back(word1);
            result.push_back(word2);
            result.push_back(word3);
            break;
        case ByteOrder::LittleEndian:
            result.push_back(word3);
            result.push_back(word2);
            result.push_back(word1);
            result.push_back(word0);
            break;
        default:
            result.push_back(word0);
            result.push_back(word1);
            result.push_back(word2);
            result.push_back(word3);
            break;
        }
    }
    return result;
}

/**
 * @brief 字符串转换为uint16数组
 */
std::vector<uint16_t> stringToUint16(const QString& str, ByteOrder order) {
    QByteArray bytes = str.toLatin1();
    if (bytes.size() % 2 != 0) {
        bytes.append('\0');  // 补零对齐
    }

    std::vector<uint16_t> result;
    result.reserve(bytes.size() / 2);

    for (int i = 0; i < bytes.size(); i += 2) {
        uint16_t value;
        switch (order) {
        case ByteOrder::BigEndian:
            value = (static_cast<uint8_t>(bytes[i]) << 8) |
                    static_cast<uint8_t>(bytes[i + 1]);
            break;
        case ByteOrder::LittleEndian:
            value = (static_cast<uint8_t>(bytes[i + 1]) << 8) |
                    static_cast<uint8_t>(bytes[i]);
            break;
        default:
            value = (static_cast<uint8_t>(bytes[i]) << 8) |
                    static_cast<uint8_t>(bytes[i + 1]);
            break;
        }
        result.push_back(value);
    }
    return result;
}

/**
 * @brief uint16数组转换为字符串
 */
QString uint16ToString(const std::vector<uint16_t>& values, ByteOrder order) {
    QByteArray bytes;
    bytes.reserve(static_cast<int>(values.size() * 2));

    for (uint16_t value : values) {
        switch (order) {
        case ByteOrder::BigEndian:
            bytes.append(static_cast<char>((value >> 8) & 0xFF));
            bytes.append(static_cast<char>(value & 0xFF));
            break;
        case ByteOrder::LittleEndian:
            bytes.append(static_cast<char>(value & 0xFF));
            bytes.append(static_cast<char>((value >> 8) & 0xFF));
            break;
        default:
            bytes.append(static_cast<char>((value >> 8) & 0xFF));
            bytes.append(static_cast<char>(value & 0xFF));
            break;
        }
    }

    // 移除尾部的空字符
    while (!bytes.isEmpty() && bytes.back() == '\0') {
        bytes.chop(1);
    }

    return QString::fromLatin1(bytes);
}

} // namespace ByteOrderUtils

// ============================================================
// CRC计算工具
// ============================================================

namespace CRCUtils {

/**
 * @brief Modbus CRC16计算表
 */
static const uint16_t CRC16_TABLE[256] = {
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

/**
 * @brief 计算Modbus CRC16
 */
uint16_t calculateCRC16(const QByteArray& data) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        uint8_t index = static_cast<uint8_t>(crc ^ static_cast<uint8_t>(data[i]));
        crc = (crc >> 8) ^ CRC16_TABLE[index];
    }
    return crc;
}

/**
 * @brief 验证Modbus CRC16
 */
bool verifyCRC16(const QByteArray& data) {
    if (data.size() < 2) return false;

    QByteArray payload = data.left(data.size() - 2);
    uint16_t calculatedCRC = calculateCRC16(payload);

    uint16_t receivedCRC = static_cast<uint8_t>(data[data.size() - 2]) |
                           (static_cast<uint8_t>(data[data.size() - 1]) << 8);

    return calculatedCRC == receivedCRC;
}

/**
 * @brief 计算LRC (Modbus ASCII)
 */
uint8_t calculateLRC(const QByteArray& data) {
    uint8_t lrc = 0;
    for (int i = 0; i < data.size(); ++i) {
        lrc += static_cast<uint8_t>(data[i]);
    }
    return static_cast<uint8_t>(-static_cast<int8_t>(lrc));
}

} // namespace CRCUtils

// ============================================================
// 辅助工具函数
// ============================================================

namespace PLCUtils {

/**
 * @brief 将十六进制字符串转换为字节数组
 */
QByteArray hexStringToBytes(const QString& hexString) {
    QString cleaned = hexString;
    cleaned.remove(' ');
    cleaned.remove('-');

    QByteArray result;
    for (int i = 0; i + 1 < cleaned.size(); i += 2) {
        bool ok;
        uint8_t byte = cleaned.mid(i, 2).toUInt(&ok, 16);
        if (ok) {
            result.append(static_cast<char>(byte));
        }
    }
    return result;
}

/**
 * @brief 将字节数组转换为十六进制字符串
 */
QString bytesToHexString(const QByteArray& data, const QString& separator) {
    QStringList hexList;
    for (int i = 0; i < data.size(); ++i) {
        hexList.append(QString("%1").arg(static_cast<uint8_t>(data[i]), 2, 16, QChar('0')).toUpper());
    }
    return hexList.join(separator);
}

/**
 * @brief 格式化字节数组为可读格式
 */
QString formatByteArray(const QByteArray& data, int bytesPerLine) {
    QString result;
    for (int i = 0; i < data.size(); ++i) {
        result += QString("%1 ").arg(static_cast<uint8_t>(data[i]), 2, 16, QChar('0')).toUpper();
        if ((i + 1) % bytesPerLine == 0 && i < data.size() - 1) {
            result += "\n";
        }
    }
    return result.trimmed();
}

/**
 * @brief 解析地址字符串 (如 "D100", "M200")
 */
bool parseAddressString(const QString& addressStr, RegisterType& regType, int& address) {
    QString upper = addressStr.toUpper().trimmed();

    // 尝试解析三菱格式
    static const QMap<QString, RegisterType> prefixMap = {
        {"D", RegisterType::D},
        {"M", RegisterType::M},
        {"X", RegisterType::X},
        {"Y", RegisterType::Y},
        {"W", RegisterType::W},
        {"B", RegisterType::B},
        {"R", RegisterType::R},
        {"ZR", RegisterType::ZR},
        {"T", RegisterType::T},
        {"TN", RegisterType::TN},
        {"C", RegisterType::C},
        {"CN", RegisterType::CN},
        {"ST", RegisterType::ST},
        {"STN", RegisterType::STN},
    };

    for (auto it = prefixMap.constBegin(); it != prefixMap.constEnd(); ++it) {
        if (upper.startsWith(it.key())) {
            QString numPart = upper.mid(it.key().length());
            bool ok;
            address = numPart.toInt(&ok);
            if (ok) {
                regType = it.value();
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief 生成地址字符串
 */
QString formatAddressString(RegisterType regType, int address) {
    return QString("%1%2").arg(getRegisterTypeName(regType)).arg(address);
}

} // namespace PLCUtils

} // namespace PLC
} // namespace VisionForge
