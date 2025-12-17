/**
 * @file PLCDef.h
 * @brief PLC通信定义
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QString>
#include <QVariant>
#include <vector>

namespace VisionForge {
namespace Comm {

/**
 * @brief 通信类型
 */
enum class CommType {
    Unknown = -1,
    Serial,         // 串口通信
    Socket,         // 网口通信 (TCP/UDP)
    CCLink          // CC-Link通信
};

/**
 * @brief 通信状态
 */
enum class CommStatus {
    Success = 0,    // 成功
    Error,          // 通信错误
    Disconnected,   // 未连接
    Timeout,        // 超时
    InvalidParam    // 参数错误
};

/**
 * @brief PLC协议类型
 */
enum class PLCProtocol {
    Unknown = -1,

    // Modbus协议
    ModbusTCP = 10,             // Modbus TCP
    ModbusRTU,                  // Modbus RTU (串口)
    ModbusASCII,                // Modbus ASCII (串口)

    // 三菱PLC协议
    MitsubishiMC_TCP = 20,      // 三菱MC协议 TCP (Q/L系列)
    MitsubishiMC_UDP,           // 三菱MC协议 UDP
    MitsubishiFX_Serial,        // 三菱FX系列串口

    // 基恩士PLC协议
    KeyencePCLink = 30,         // 基恩士 PC-LINK上位链路协议 (TCP)

    // 欧姆龙PLC协议
    OmronFINS_TCP = 40,         // 欧姆龙 FINS TCP
    OmronFINS_UDP,              // 欧姆龙 FINS UDP
    OmronHostLink,              // 欧姆龙 HostLink

    // 西门子PLC协议
    SiemensS7 = 50,             // 西门子 S7协议

    // 松下PLC协议
    PanasonicMewtocol = 60,     // 松下 Mewtocol协议
};

/**
 * @brief 寄存器数据类型
 */
enum class RegisterType {
    Bit,            // 位 (M, X, Y等)
    Word,           // 字 (D寄存器, 16位)
    DWord,          // 双字 (32位)
    Float,          // 浮点数 (32位)
    Double,         // 双精度浮点 (64位)
    String          // 字符串
};

/**
 * @brief 寄存器区域类型 (三菱)
 */
enum class MitsubishiDeviceType {
    D,      // 数据寄存器
    M,      // 内部继电器
    X,      // 输入
    Y,      // 输出
    W,      // 链接寄存器
    B,      // 链接继电器
    R,      // 文件寄存器
    ZR,     // 文件寄存器
    T,      // 定时器
    C,      // 计数器
    ST,     // 累计定时器
};

/**
 * @brief 通信配置基类
 */
struct CommConfig {
    CommType type = CommType::Unknown;
    int timeout = 3000;         // 超时时间(ms)
    int retryCount = 3;         // 重试次数

    virtual ~CommConfig() = default;
};

/**
 * @brief 串口配置
 */
struct SerialConfig : public CommConfig {
    QString portName;           // 串口名 (COM1, COM2...)
    int baudRate = 9600;        // 波特率
    int dataBits = 8;           // 数据位
    int stopBits = 1;           // 停止位
    int parity = 0;             // 校验 (0=无, 1=奇, 2=偶)

    SerialConfig() { type = CommType::Serial; }
};

/**
 * @brief 网口配置
 */
struct SocketConfig : public CommConfig {
    QString ipAddress;          // IP地址
    quint16 port = 502;         // 端口号
    bool useTcp = true;         // TCP/UDP

    SocketConfig() { type = CommType::Socket; }
};

/**
 * @brief PLC连接配置
 */
struct PLCConfig {
    QString name;                       // 连接名称
    PLCProtocol protocol = PLCProtocol::ModbusTCP;
    std::shared_ptr<CommConfig> commConfig;

    // Modbus特有配置
    int slaveId = 1;                    // 从站地址

    // 三菱MC协议特有配置
    int networkNo = 0;                  // 网络号
    int pcNo = 0xFF;                    // PC号
    int destModuleIo = 0x03FF;          // 目标模块IO
    int destModuleStation = 0;          // 目标模块站号
};

/**
 * @brief 读写结果
 */
struct PLCResult {
    CommStatus status = CommStatus::Success;
    QString errorMessage;
    std::vector<int> intValues;
    std::vector<double> doubleValues;
    std::vector<bool> boolValues;
    QString stringValue;

    bool isSuccess() const { return status == CommStatus::Success; }
};

/**
 * @brief 获取协议名称
 */
inline QString getProtocolName(PLCProtocol protocol) {
    switch (protocol) {
    case PLCProtocol::ModbusTCP: return "Modbus TCP";
    case PLCProtocol::ModbusRTU: return "Modbus RTU";
    case PLCProtocol::ModbusASCII: return "Modbus ASCII";
    case PLCProtocol::MitsubishiMC_TCP: return "三菱MC协议(TCP)";
    case PLCProtocol::MitsubishiMC_UDP: return "三菱MC协议(UDP)";
    case PLCProtocol::MitsubishiFX_Serial: return "三菱FX串口";
    case PLCProtocol::KeyencePCLink: return "基恩士PC-LINK上位链路";
    case PLCProtocol::OmronFINS_TCP: return "欧姆龙FINS(TCP)";
    case PLCProtocol::OmronFINS_UDP: return "欧姆龙FINS(UDP)";
    case PLCProtocol::OmronHostLink: return "欧姆龙HostLink";
    case PLCProtocol::SiemensS7: return "西门子S7";
    case PLCProtocol::PanasonicMewtocol: return "松下Mewtocol";
    default: return "未知协议";
    }
}

} // namespace Comm
} // namespace VisionForge
