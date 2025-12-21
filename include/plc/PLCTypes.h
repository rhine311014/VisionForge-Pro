/**
 * @file PLCTypes.h
 * @brief 工业级PLC通信类型定义
 * @author VisionForge Team
 * @date 2025-12-21
 *
 * 本文件定义了PLC通信模块所需的所有类型、枚举和数据结构，
 * 支持Modbus TCP、三菱MC 3E帧、欧姆龙FINS等工业协议。
 */

#pragma once

#include <QString>
#include <QHostAddress>
#include <QByteArray>
#include <QVariant>
#include <QMap>
#include <QMutex>
#include <QDateTime>

#include <cstdint>
#include <vector>
#include <memory>
#include <functional>

namespace VisionForge {
namespace PLC {

// ============================================================
// 前向声明
// ============================================================
class PLCProtocol;
class PLCClient;

// ============================================================
// 通信状态枚举
// ============================================================

/**
 * @brief 通信连接状态
 */
enum class CommStatus {
    Disconnected = 0,   ///< 未连接
    Connecting,         ///< 正在连接
    Connected,          ///< 已连接
    Reconnecting,       ///< 正在重连
    Error               ///< 错误状态
};

/**
 * @brief 获取通信状态名称
 */
inline QString getCommStatusName(CommStatus status) {
    switch (status) {
    case CommStatus::Disconnected:  return QStringLiteral("Disconnected");
    case CommStatus::Connecting:    return QStringLiteral("Connecting");
    case CommStatus::Connected:     return QStringLiteral("Connected");
    case CommStatus::Reconnecting:  return QStringLiteral("Reconnecting");
    case CommStatus::Error:         return QStringLiteral("Error");
    default:                        return QStringLiteral("Unknown");
    }
}

// ============================================================
// 协议类型枚举
// ============================================================

/**
 * @brief 支持的PLC协议类型
 */
enum class ProtocolType {
    Unknown = 0,

    // Modbus协议族
    ModbusTCP = 100,            ///< Modbus TCP协议
    ModbusRTU,                  ///< Modbus RTU协议(串口)
    ModbusASCII,                ///< Modbus ASCII协议(串口)

    // 三菱协议族
    MitsubishiMC3E = 200,       ///< 三菱MC协议3E帧(二进制)
    MitsubishiMC3E_ASCII,       ///< 三菱MC协议3E帧(ASCII)
    MitsubishiMC4E,             ///< 三菱MC协议4E帧
    MitsubishiFX,               ///< 三菱FX系列协议

    // 欧姆龙协议族
    OmronFINS_TCP = 300,        ///< 欧姆龙FINS TCP
    OmronFINS_UDP,              ///< 欧姆龙FINS UDP
    OmronHostLink,              ///< 欧姆龙HostLink协议

    // 西门子协议族
    SiemensS7 = 400,            ///< 西门子S7协议
    SiemensS7_200,              ///< 西门子S7-200协议
    SiemensS7_1200,             ///< 西门子S7-1200协议

    // 其他品牌
    KeyencePCLink = 500,        ///< 基恩士PC-LINK上位链路
    PanasonicMewtocol = 600,    ///< 松下Mewtocol协议
    ABEtherNetIP = 700,         ///< 罗克韦尔EtherNet/IP
};

/**
 * @brief 获取协议类型名称
 */
inline QString getProtocolTypeName(ProtocolType type) {
    switch (type) {
    case ProtocolType::ModbusTCP:           return QStringLiteral("Modbus TCP");
    case ProtocolType::ModbusRTU:           return QStringLiteral("Modbus RTU");
    case ProtocolType::ModbusASCII:         return QStringLiteral("Modbus ASCII");
    case ProtocolType::MitsubishiMC3E:      return QStringLiteral("Mitsubishi MC 3E Binary");
    case ProtocolType::MitsubishiMC3E_ASCII: return QStringLiteral("Mitsubishi MC 3E ASCII");
    case ProtocolType::MitsubishiMC4E:      return QStringLiteral("Mitsubishi MC 4E");
    case ProtocolType::MitsubishiFX:        return QStringLiteral("Mitsubishi FX");
    case ProtocolType::OmronFINS_TCP:       return QStringLiteral("Omron FINS TCP");
    case ProtocolType::OmronFINS_UDP:       return QStringLiteral("Omron FINS UDP");
    case ProtocolType::OmronHostLink:       return QStringLiteral("Omron HostLink");
    case ProtocolType::SiemensS7:           return QStringLiteral("Siemens S7");
    case ProtocolType::SiemensS7_200:       return QStringLiteral("Siemens S7-200");
    case ProtocolType::SiemensS7_1200:      return QStringLiteral("Siemens S7-1200");
    case ProtocolType::KeyencePCLink:       return QStringLiteral("Keyence PC-LINK");
    case ProtocolType::PanasonicMewtocol:   return QStringLiteral("Panasonic Mewtocol");
    case ProtocolType::ABEtherNetIP:        return QStringLiteral("AB EtherNet/IP");
    default:                                return QStringLiteral("Unknown");
    }
}

// ============================================================
// 寄存器类型枚举
// ============================================================

/**
 * @brief 通用寄存器区域类型
 */
enum class RegisterType {
    // 通用类型
    Unknown = 0,

    // 三菱软元件类型
    D = 10,         ///< 数据寄存器 (Data Register)
    M,              ///< 内部继电器 (Internal Relay)
    X,              ///< 输入 (Input)
    Y,              ///< 输出 (Output)
    W,              ///< 链接寄存器 (Link Register)
    B,              ///< 链接继电器 (Link Relay)
    R,              ///< 文件寄存器 (File Register)
    ZR,             ///< 扩展文件寄存器
    T,              ///< 定时器触点 (Timer Contact)
    TN,             ///< 定时器当前值 (Timer Current Value)
    C,              ///< 计数器触点 (Counter Contact)
    CN,             ///< 计数器当前值 (Counter Current Value)
    ST,             ///< 累计定时器 (Retentive Timer)
    STN,            ///< 累计定时器当前值

    // Modbus寄存器类型
    Coil = 100,             ///< 线圈 (0x)
    DiscreteInput,          ///< 离散输入 (1x)
    InputRegister,          ///< 输入寄存器 (3x)
    HoldingRegister,        ///< 保持寄存器 (4x)

    // 欧姆龙寄存器类型
    DM = 200,       ///< 数据存储区
    CIO,            ///< I/O区
    WR,             ///< 作业区
    HR,             ///< 保持区
    AR,             ///< 辅助区
    EM,             ///< 扩展存储区

    // 西门子寄存器类型
    DB = 300,       ///< 数据块
    MB,             ///< 标志位
    IB,             ///< 输入字节
    QB,             ///< 输出字节
    MW,             ///< 标志字
    IW,             ///< 输入字
    QW,             ///< 输出字
};

/**
 * @brief 获取寄存器类型名称
 */
inline QString getRegisterTypeName(RegisterType type) {
    switch (type) {
    case RegisterType::D:               return QStringLiteral("D");
    case RegisterType::M:               return QStringLiteral("M");
    case RegisterType::X:               return QStringLiteral("X");
    case RegisterType::Y:               return QStringLiteral("Y");
    case RegisterType::W:               return QStringLiteral("W");
    case RegisterType::B:               return QStringLiteral("B");
    case RegisterType::R:               return QStringLiteral("R");
    case RegisterType::ZR:              return QStringLiteral("ZR");
    case RegisterType::T:               return QStringLiteral("T");
    case RegisterType::TN:              return QStringLiteral("TN");
    case RegisterType::C:               return QStringLiteral("C");
    case RegisterType::CN:              return QStringLiteral("CN");
    case RegisterType::ST:              return QStringLiteral("ST");
    case RegisterType::STN:             return QStringLiteral("STN");
    case RegisterType::Coil:            return QStringLiteral("Coil");
    case RegisterType::DiscreteInput:   return QStringLiteral("DiscreteInput");
    case RegisterType::InputRegister:   return QStringLiteral("InputRegister");
    case RegisterType::HoldingRegister: return QStringLiteral("HoldingRegister");
    case RegisterType::DM:              return QStringLiteral("DM");
    case RegisterType::CIO:             return QStringLiteral("CIO");
    case RegisterType::WR:              return QStringLiteral("WR");
    case RegisterType::HR:              return QStringLiteral("HR");
    case RegisterType::AR:              return QStringLiteral("AR");
    case RegisterType::EM:              return QStringLiteral("EM");
    case RegisterType::DB:              return QStringLiteral("DB");
    case RegisterType::MB:              return QStringLiteral("MB");
    case RegisterType::IB:              return QStringLiteral("IB");
    case RegisterType::QB:              return QStringLiteral("QB");
    case RegisterType::MW:              return QStringLiteral("MW");
    case RegisterType::IW:              return QStringLiteral("IW");
    case RegisterType::QW:              return QStringLiteral("QW");
    default:                            return QStringLiteral("Unknown");
    }
}

/**
 * @brief 判断是否为位寄存器
 */
inline bool isBitRegister(RegisterType type) {
    switch (type) {
    case RegisterType::M:
    case RegisterType::X:
    case RegisterType::Y:
    case RegisterType::B:
    case RegisterType::T:
    case RegisterType::C:
    case RegisterType::ST:
    case RegisterType::Coil:
    case RegisterType::DiscreteInput:
        return true;
    default:
        return false;
    }
}

// ============================================================
// 数据类型枚举
// ============================================================

/**
 * @brief 数据类型定义
 */
enum class DataType {
    Bit,            ///< 位
    Int16,          ///< 16位有符号整数
    UInt16,         ///< 16位无符号整数
    Int32,          ///< 32位有符号整数
    UInt32,         ///< 32位无符号整数
    Int64,          ///< 64位有符号整数
    UInt64,         ///< 64位无符号整数
    Float32,        ///< 32位浮点数
    Float64,        ///< 64位浮点数
    String,         ///< 字符串
    ByteArray       ///< 字节数组
};

/**
 * @brief 获取数据类型大小(字节)
 */
inline int getDataTypeSize(DataType type) {
    switch (type) {
    case DataType::Bit:      return 1;
    case DataType::Int16:    return 2;
    case DataType::UInt16:   return 2;
    case DataType::Int32:    return 4;
    case DataType::UInt32:   return 4;
    case DataType::Int64:    return 8;
    case DataType::UInt64:   return 8;
    case DataType::Float32:  return 4;
    case DataType::Float64:  return 8;
    default:                 return 0;
    }
}

// ============================================================
// 字节序枚举
// ============================================================

/**
 * @brief 字节序
 */
enum class ByteOrder {
    BigEndian,      ///< 大端序 (Modbus标准)
    LittleEndian,   ///< 小端序
    BigEndianSwap,  ///< 大端序字交换 (CDAB)
    LittleEndianSwap ///< 小端序字交换 (BADC)
};

// ============================================================
// 配置结构体
// ============================================================

/**
 * @brief PLC连接配置
 */
struct PLCConfig {
    // 基本信息
    QString name;                       ///< 连接名称
    ProtocolType protocol = ProtocolType::ModbusTCP;

    // 网络配置
    QString ipAddress = "192.168.1.1";  ///< IP地址
    quint16 port = 502;                 ///< 端口号

    // 通信参数
    int connectTimeout = 3000;          ///< 连接超时(ms)
    int responseTimeout = 1000;         ///< 响应超时(ms)
    int retryCount = 3;                 ///< 重试次数
    int retryInterval = 500;            ///< 重试间隔(ms)

    // 自动重连配置
    bool autoReconnect = true;          ///< 启用自动重连
    int reconnectInterval = 5000;       ///< 重连间隔(ms)
    int maxReconnectAttempts = -1;      ///< 最大重连次数(-1=无限)

    // 心跳配置
    bool enableHeartbeat = true;        ///< 启用心跳
    int heartbeatInterval = 5000;       ///< 心跳间隔(ms)
    RegisterType heartbeatRegister = RegisterType::D;
    int heartbeatAddress = 0;           ///< 心跳寄存器地址

    // Modbus特有配置
    int slaveId = 1;                    ///< 从站地址

    // 三菱MC协议特有配置
    quint8 networkNo = 0;               ///< 网络号
    quint8 pcNo = 0xFF;                 ///< PC号
    quint16 destModuleIo = 0x03FF;      ///< 目标模块IO号
    quint8 destModuleStation = 0;       ///< 目标模块站号
    quint16 monitoringTimer = 10;       ///< 监视定时器(250ms单位)

    // 欧姆龙FINS特有配置
    quint8 finsSourceNode = 0;          ///< FINS源节点地址
    quint8 finsDestNode = 0;            ///< FINS目标节点地址
    quint8 finsSourceUnit = 0;          ///< FINS源单元地址
    quint8 finsDestUnit = 0;            ///< FINS目标单元地址

    // 数据格式
    ByteOrder byteOrder = ByteOrder::BigEndian;

    // 日志配置
    bool enableLogging = false;         ///< 启用通信日志
    QString logPath;                    ///< 日志路径

    /**
     * @brief 验证配置有效性
     */
    bool isValid() const {
        if (ipAddress.isEmpty()) return false;
        if (port == 0) return false;
        if (connectTimeout <= 0) return false;
        if (responseTimeout <= 0) return false;
        return true;
    }

    /**
     * @brief 转换为JSON
     */
    QVariantMap toVariantMap() const {
        QVariantMap map;
        map["name"] = name;
        map["protocol"] = static_cast<int>(protocol);
        map["ipAddress"] = ipAddress;
        map["port"] = port;
        map["connectTimeout"] = connectTimeout;
        map["responseTimeout"] = responseTimeout;
        map["retryCount"] = retryCount;
        map["retryInterval"] = retryInterval;
        map["autoReconnect"] = autoReconnect;
        map["reconnectInterval"] = reconnectInterval;
        map["maxReconnectAttempts"] = maxReconnectAttempts;
        map["enableHeartbeat"] = enableHeartbeat;
        map["heartbeatInterval"] = heartbeatInterval;
        map["slaveId"] = slaveId;
        map["networkNo"] = networkNo;
        map["pcNo"] = pcNo;
        map["byteOrder"] = static_cast<int>(byteOrder);
        return map;
    }

    /**
     * @brief 从JSON加载
     */
    static PLCConfig fromVariantMap(const QVariantMap& map) {
        PLCConfig config;
        config.name = map.value("name").toString();
        config.protocol = static_cast<ProtocolType>(map.value("protocol").toInt());
        config.ipAddress = map.value("ipAddress", "192.168.1.1").toString();
        config.port = static_cast<quint16>(map.value("port", 502).toInt());
        config.connectTimeout = map.value("connectTimeout", 3000).toInt();
        config.responseTimeout = map.value("responseTimeout", 1000).toInt();
        config.retryCount = map.value("retryCount", 3).toInt();
        config.retryInterval = map.value("retryInterval", 500).toInt();
        config.autoReconnect = map.value("autoReconnect", true).toBool();
        config.reconnectInterval = map.value("reconnectInterval", 5000).toInt();
        config.maxReconnectAttempts = map.value("maxReconnectAttempts", -1).toInt();
        config.enableHeartbeat = map.value("enableHeartbeat", true).toBool();
        config.heartbeatInterval = map.value("heartbeatInterval", 5000).toInt();
        config.slaveId = map.value("slaveId", 1).toInt();
        config.networkNo = static_cast<quint8>(map.value("networkNo", 0).toInt());
        config.pcNo = static_cast<quint8>(map.value("pcNo", 0xFF).toInt());
        config.byteOrder = static_cast<ByteOrder>(map.value("byteOrder", 0).toInt());
        return config;
    }
};

/**
 * @brief 寄存器映射结构
 * 用于视觉系统与PLC的数据交换
 */
struct RegisterMap {
    // 触发信号
    RegisterType triggerRegister = RegisterType::M;     ///< 触发信号寄存器类型
    int triggerAddress = 100;                           ///< 触发信号地址
    int triggerBit = 0;                                 ///< 触发信号位编号

    // 响应信号
    RegisterType responseRegister = RegisterType::M;    ///< 响应信号寄存器类型
    int responseAddress = 101;                          ///< 响应信号地址
    int responseBit = 0;                                ///< 响应信号位编号

    // 检测结果
    RegisterType resultRegister = RegisterType::D;      ///< 结果寄存器类型
    int resultAddress = 100;                            ///< 结果起始地址
    int resultCount = 10;                               ///< 结果寄存器数量

    // 偏移量输出
    RegisterType offsetXRegister = RegisterType::D;     ///< X偏移量寄存器类型
    int offsetXAddress = 110;                           ///< X偏移量地址
    RegisterType offsetYRegister = RegisterType::D;     ///< Y偏移量寄存器类型
    int offsetYAddress = 112;                           ///< Y偏移量地址
    RegisterType offsetAngleRegister = RegisterType::D; ///< 角度偏移寄存器类型
    int offsetAngleAddress = 114;                       ///< 角度偏移地址

    // OK/NG信号
    RegisterType okNgRegister = RegisterType::M;        ///< OK/NG信号寄存器类型
    int okAddress = 200;                                ///< OK信号地址
    int ngAddress = 201;                                ///< NG信号地址

    // 心跳地址
    RegisterType heartbeatRegister = RegisterType::D;   ///< 心跳寄存器类型
    int heartbeatAddress = 0;                           ///< 心跳地址

    /**
     * @brief 转换为JSON
     */
    QVariantMap toVariantMap() const {
        QVariantMap map;
        map["triggerRegister"] = static_cast<int>(triggerRegister);
        map["triggerAddress"] = triggerAddress;
        map["triggerBit"] = triggerBit;
        map["responseRegister"] = static_cast<int>(responseRegister);
        map["responseAddress"] = responseAddress;
        map["responseBit"] = responseBit;
        map["resultRegister"] = static_cast<int>(resultRegister);
        map["resultAddress"] = resultAddress;
        map["resultCount"] = resultCount;
        map["offsetXAddress"] = offsetXAddress;
        map["offsetYAddress"] = offsetYAddress;
        map["offsetAngleAddress"] = offsetAngleAddress;
        map["okAddress"] = okAddress;
        map["ngAddress"] = ngAddress;
        return map;
    }
};

// ============================================================
// 操作结果结构体
// ============================================================

/**
 * @brief 通信操作结果
 */
struct PLCResult {
    bool success = false;               ///< 操作是否成功
    int errorCode = 0;                  ///< 错误码
    QString errorMessage;               ///< 错误信息
    QByteArray rawData;                 ///< 原始响应数据

    // 读取结果数据
    std::vector<bool> bitValues;        ///< 位值列表
    std::vector<int16_t> int16Values;   ///< 16位有符号整数列表
    std::vector<uint16_t> uint16Values; ///< 16位无符号整数列表
    std::vector<int32_t> int32Values;   ///< 32位有符号整数列表
    std::vector<uint32_t> uint32Values; ///< 32位无符号整数列表
    std::vector<float> floatValues;     ///< 浮点数列表
    std::vector<double> doubleValues;   ///< 双精度浮点数列表
    QString stringValue;                ///< 字符串值

    // 时间戳
    QDateTime timestamp;                ///< 操作时间戳
    qint64 responseTime = 0;            ///< 响应时间(ms)

    /**
     * @brief 构造成功结果
     */
    static PLCResult Success() {
        PLCResult result;
        result.success = true;
        result.timestamp = QDateTime::currentDateTime();
        return result;
    }

    /**
     * @brief 构造失败结果
     */
    static PLCResult Error(int code, const QString& message) {
        PLCResult result;
        result.success = false;
        result.errorCode = code;
        result.errorMessage = message;
        result.timestamp = QDateTime::currentDateTime();
        return result;
    }

    /**
     * @brief 是否成功
     */
    bool isSuccess() const { return success; }

    /**
     * @brief 获取第一个int16值
     */
    int16_t getInt16(size_t index = 0) const {
        return (index < int16Values.size()) ? int16Values[index] : 0;
    }

    /**
     * @brief 获取第一个int32值
     */
    int32_t getInt32(size_t index = 0) const {
        return (index < int32Values.size()) ? int32Values[index] : 0;
    }

    /**
     * @brief 获取第一个float值
     */
    float getFloat(size_t index = 0) const {
        return (index < floatValues.size()) ? floatValues[index] : 0.0f;
    }

    /**
     * @brief 获取第一个bit值
     */
    bool getBit(size_t index = 0) const {
        return (index < bitValues.size()) ? bitValues[index] : false;
    }
};

// ============================================================
// 错误码定义
// ============================================================

namespace ErrorCode {
    // 通用错误 (0-99)
    constexpr int Success = 0;
    constexpr int Unknown = 1;
    constexpr int InvalidParameter = 2;
    constexpr int NotConnected = 3;
    constexpr int Timeout = 4;
    constexpr int ConnectionFailed = 5;
    constexpr int ConnectionLost = 6;
    constexpr int ProtocolError = 7;
    constexpr int DataError = 8;
    constexpr int BufferOverflow = 9;

    // Modbus错误 (100-199)
    constexpr int ModbusIllegalFunction = 101;
    constexpr int ModbusIllegalDataAddress = 102;
    constexpr int ModbusIllegalDataValue = 103;
    constexpr int ModbusSlaveDeviceFailure = 104;
    constexpr int ModbusAcknowledge = 105;
    constexpr int ModbusSlaveDeviceBusy = 106;
    constexpr int ModbusMemoryParityError = 108;
    constexpr int ModbusGatewayPathUnavailable = 110;
    constexpr int ModbusGatewayTargetFailed = 111;

    // 三菱MC错误 (200-299)
    constexpr int MCCommandError = 201;
    constexpr int MCDeviceError = 202;
    constexpr int MCAccessError = 203;
    constexpr int MCDataError = 204;

    // 欧姆龙FINS错误 (300-399)
    constexpr int FINSLocalNodeError = 301;
    constexpr int FINSDestNodeError = 302;
    constexpr int FINSControllerError = 303;
    constexpr int FINSServiceUnsupported = 304;
    constexpr int FINSRoutingError = 305;
    constexpr int FINSCommandFormatError = 306;
    constexpr int FINSParameterError = 307;
    constexpr int FINSReadNotPossible = 308;
    constexpr int FINSWriteNotPossible = 309;
    constexpr int FINSModeError = 310;
    constexpr int FINSDeviceNotFound = 311;
    constexpr int FINSAccessRightError = 312;
}

/**
 * @brief 获取错误码描述
 */
inline QString getErrorMessage(int errorCode) {
    switch (errorCode) {
    case ErrorCode::Success:                    return QStringLiteral("Success");
    case ErrorCode::Unknown:                    return QStringLiteral("Unknown error");
    case ErrorCode::InvalidParameter:           return QStringLiteral("Invalid parameter");
    case ErrorCode::NotConnected:               return QStringLiteral("Not connected");
    case ErrorCode::Timeout:                    return QStringLiteral("Timeout");
    case ErrorCode::ConnectionFailed:           return QStringLiteral("Connection failed");
    case ErrorCode::ConnectionLost:             return QStringLiteral("Connection lost");
    case ErrorCode::ProtocolError:              return QStringLiteral("Protocol error");
    case ErrorCode::DataError:                  return QStringLiteral("Data error");
    case ErrorCode::BufferOverflow:             return QStringLiteral("Buffer overflow");
    case ErrorCode::ModbusIllegalFunction:      return QStringLiteral("Modbus: Illegal function");
    case ErrorCode::ModbusIllegalDataAddress:   return QStringLiteral("Modbus: Illegal data address");
    case ErrorCode::ModbusIllegalDataValue:     return QStringLiteral("Modbus: Illegal data value");
    case ErrorCode::ModbusSlaveDeviceFailure:   return QStringLiteral("Modbus: Slave device failure");
    case ErrorCode::ModbusSlaveDeviceBusy:      return QStringLiteral("Modbus: Slave device busy");
    case ErrorCode::MCCommandError:             return QStringLiteral("MC: Command error");
    case ErrorCode::MCDeviceError:              return QStringLiteral("MC: Device error");
    case ErrorCode::MCAccessError:              return QStringLiteral("MC: Access error");
    case ErrorCode::FINSLocalNodeError:         return QStringLiteral("FINS: Local node error");
    case ErrorCode::FINSDestNodeError:          return QStringLiteral("FINS: Destination node error");
    default:                                    return QStringLiteral("Error code: %1").arg(errorCode);
    }
}

// ============================================================
// Modbus功能码定义
// ============================================================

namespace ModbusFunctionCode {
    constexpr quint8 ReadCoils = 0x01;              ///< 读线圈
    constexpr quint8 ReadDiscreteInputs = 0x02;     ///< 读离散输入
    constexpr quint8 ReadHoldingRegisters = 0x03;   ///< 读保持寄存器
    constexpr quint8 ReadInputRegisters = 0x04;     ///< 读输入寄存器
    constexpr quint8 WriteSingleCoil = 0x05;        ///< 写单个线圈
    constexpr quint8 WriteSingleRegister = 0x06;    ///< 写单个寄存器
    constexpr quint8 WriteMultipleCoils = 0x0F;     ///< 写多个线圈
    constexpr quint8 WriteMultipleRegisters = 0x10; ///< 写多个寄存器
    constexpr quint8 ReadWriteMultipleRegisters = 0x17; ///< 读写多个寄存器
}

// ============================================================
// 三菱MC协议命令定义
// ============================================================

namespace MCCommand {
    // 批量读取命令
    constexpr quint16 BatchRead = 0x0401;       ///< 批量读取
    constexpr quint16 BatchWrite = 0x1401;      ///< 批量写入

    // 随机读取命令
    constexpr quint16 RandomRead = 0x0403;      ///< 随机读取
    constexpr quint16 RandomWrite = 0x1402;     ///< 随机写入

    // 监视命令
    constexpr quint16 MonitorRegister = 0x0801; ///< 监视登录
    constexpr quint16 Monitor = 0x0802;         ///< 监视读取

    // 子命令 - 位软元件/字软元件区分
    constexpr quint16 SubCmdWord = 0x0000;      ///< 字软元件操作
    constexpr quint16 SubCmdBit = 0x0001;       ///< 位软元件操作
}

// ============================================================
// 三菱软元件代码定义
// ============================================================

namespace MCDeviceCode {
    constexpr quint8 D = 0xA8;      ///< 数据寄存器
    constexpr quint8 M = 0x90;      ///< 内部继电器
    constexpr quint8 X = 0x9C;      ///< 输入
    constexpr quint8 Y = 0x9D;      ///< 输出
    constexpr quint8 W = 0xB4;      ///< 链接寄存器
    constexpr quint8 B = 0xA0;      ///< 链接继电器
    constexpr quint8 R = 0xAF;      ///< 文件寄存器
    constexpr quint8 ZR = 0xB0;     ///< 扩展文件寄存器
    constexpr quint8 T = 0xC1;      ///< 定时器触点
    constexpr quint8 TN = 0xC2;     ///< 定时器当前值
    constexpr quint8 C = 0xC3;      ///< 计数器触点
    constexpr quint8 CN = 0xC5;     ///< 计数器当前值
    constexpr quint8 ST = 0xC8;     ///< 累计定时器触点
    constexpr quint8 STN = 0xC9;    ///< 累计定时器当前值
}

/**
 * @brief 获取三菱软元件代码
 */
inline quint8 getMCDeviceCode(RegisterType type) {
    switch (type) {
    case RegisterType::D:   return MCDeviceCode::D;
    case RegisterType::M:   return MCDeviceCode::M;
    case RegisterType::X:   return MCDeviceCode::X;
    case RegisterType::Y:   return MCDeviceCode::Y;
    case RegisterType::W:   return MCDeviceCode::W;
    case RegisterType::B:   return MCDeviceCode::B;
    case RegisterType::R:   return MCDeviceCode::R;
    case RegisterType::ZR:  return MCDeviceCode::ZR;
    case RegisterType::T:   return MCDeviceCode::T;
    case RegisterType::TN:  return MCDeviceCode::TN;
    case RegisterType::C:   return MCDeviceCode::C;
    case RegisterType::CN:  return MCDeviceCode::CN;
    case RegisterType::ST:  return MCDeviceCode::ST;
    case RegisterType::STN: return MCDeviceCode::STN;
    default:                return MCDeviceCode::D;
    }
}

// ============================================================
// 统计信息结构
// ============================================================

/**
 * @brief 通信统计信息
 */
struct CommStatistics {
    // 连接统计
    int totalConnections = 0;           ///< 总连接次数
    int successfulConnections = 0;      ///< 成功连接次数
    int failedConnections = 0;          ///< 失败连接次数
    int reconnectCount = 0;             ///< 重连次数

    // 读写统计
    qint64 totalReadCount = 0;          ///< 总读取次数
    qint64 totalWriteCount = 0;         ///< 总写入次数
    qint64 successReadCount = 0;        ///< 成功读取次数
    qint64 successWriteCount = 0;       ///< 成功写入次数
    qint64 failedReadCount = 0;         ///< 失败读取次数
    qint64 failedWriteCount = 0;        ///< 失败写入次数

    // 数据量统计
    qint64 bytesSent = 0;               ///< 发送字节数
    qint64 bytesReceived = 0;           ///< 接收字节数

    // 时间统计
    qint64 totalResponseTime = 0;       ///< 总响应时间(ms)
    qint64 minResponseTime = INT64_MAX; ///< 最小响应时间(ms)
    qint64 maxResponseTime = 0;         ///< 最大响应时间(ms)
    QDateTime lastSuccessTime;          ///< 最后成功时间
    QDateTime lastErrorTime;            ///< 最后错误时间
    QString lastError;                  ///< 最后错误信息

    // 运行时间
    QDateTime startTime;                ///< 启动时间

    /**
     * @brief 获取平均响应时间
     */
    double getAverageResponseTime() const {
        qint64 totalOps = successReadCount + successWriteCount;
        return (totalOps > 0) ? static_cast<double>(totalResponseTime) / totalOps : 0.0;
    }

    /**
     * @brief 获取读取成功率
     */
    double getReadSuccessRate() const {
        return (totalReadCount > 0) ?
            static_cast<double>(successReadCount) / totalReadCount * 100.0 : 0.0;
    }

    /**
     * @brief 获取写入成功率
     */
    double getWriteSuccessRate() const {
        return (totalWriteCount > 0) ?
            static_cast<double>(successWriteCount) / totalWriteCount * 100.0 : 0.0;
    }

    /**
     * @brief 重置统计
     */
    void reset() {
        *this = CommStatistics();
        startTime = QDateTime::currentDateTime();
    }
};

// ============================================================
// 类型别名
// ============================================================

using PLCConfigPtr = std::shared_ptr<PLCConfig>;
using RegisterMapPtr = std::shared_ptr<RegisterMap>;
using PLCResultPtr = std::shared_ptr<PLCResult>;

// ============================================================
// 回调函数类型
// ============================================================

/// 连接状态变化回调
using ConnectionCallback = std::function<void(CommStatus status)>;

/// 数据接收回调
using DataReceivedCallback = std::function<void(const PLCResult& result)>;

/// 错误回调
using ErrorCallback = std::function<void(int errorCode, const QString& message)>;

/// 心跳回调
using HeartbeatCallback = std::function<void(bool alive)>;

} // namespace PLC
} // namespace VisionForge
