/**
 * @file PLCDef.h
 * @brief PLC通信相关定义和数据结构
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @details
 * 本文件定义了PLC通信模块所需的所有枚举类型、数据结构和配置类。
 *
 * ## 主要内容
 * - 通信类型和状态枚举
 * - PLC协议类型枚举（支持多种品牌PLC）
 * - 寄存器类型和设备类型枚举
 * - 通信配置结构体（串口/网口）
 * - PLC连接配置和读写结果结构体
 *
 * ## 支持的PLC品牌和协议
 * | 品牌 | 协议 | 通信方式 |
 * |------|------|----------|
 * | 通用 | Modbus TCP/RTU/ASCII | 网口/串口 |
 * | 三菱 | MC协议、FX串口 | 网口(TCP/UDP)/串口 |
 * | 基恩士 | PC-LINK上位链路 | 网口 |
 * | 欧姆龙 | FINS、HostLink | 网口/串口 |
 * | 西门子 | S7协议 | 网口 |
 * | 松下 | Mewtocol | 串口 |
 *
 * @see PLCComm PLC通信基类
 * @see PLCManager PLC管理器
 */

#pragma once

#include <QString>
#include <QVariant>
#include <vector>

namespace VisionForge {
namespace Comm {

/**
 * @enum CommType
 * @brief 通信方式类型枚举
 *
 * @details
 * 定义了PLC通信的物理连接方式。
 */
enum class CommType {
    Unknown = -1,   ///< 未知/未设置
    Serial,         ///< 串口通信 (RS232/RS485)
    Socket,         ///< 网口通信 (TCP/UDP)
    CCLink          ///< CC-Link现场总线通信
};

/**
 * @enum CommStatus
 * @brief 通信操作状态枚举
 *
 * @details
 * 表示PLC读写操作的执行结果状态。
 *
 * ## 状态说明
 * - Success: 操作成功完成
 * - Error: 通信过程中发生错误（如协议错误、CRC校验失败等）
 * - Disconnected: 通信链路未建立或已断开
 * - Timeout: 等待响应超时
 * - InvalidParam: 传入的参数无效（如地址越界、数量为0等）
 */
enum class CommStatus {
    Success = 0,    ///< 操作成功
    Error,          ///< 通信错误
    Disconnected,   ///< 未连接
    Timeout,        ///< 通信超时
    InvalidParam    ///< 参数无效
};

/**
 * @enum PLCProtocol
 * @brief PLC通信协议类型枚举
 *
 * @details
 * 定义了系统支持的所有PLC通信协议。
 * 协议编号按品牌分组，每组预留10个编号便于扩展。
 *
 * ## Modbus协议族 (10-19)
 * Modbus是一种开放的工业通信协议，广泛应用于各种PLC和仪表设备。
 * - ModbusTCP: 基于TCP/IP的Modbus协议，端口通常为502
 * - ModbusRTU: 基于串口的二进制Modbus协议
 * - ModbusASCII: 基于串口的ASCII编码Modbus协议
 *
 * ## 三菱PLC协议 (20-29)
 * 用于三菱Q/L系列和FX系列PLC。
 * - MitsubishiMC_TCP: MC协议TCP模式，适用于Q/L系列
 * - MitsubishiMC_UDP: MC协议UDP模式，响应更快
 * - MitsubishiFX_Serial: FX系列专用串口协议
 *
 * ## 基恩士PLC协议 (30-39)
 * - KeyencePCLink: PC-LINK上位链路协议，用于KV系列PLC
 *
 * ## 欧姆龙PLC协议 (40-49)
 * - OmronFINS_TCP: FINS协议TCP模式
 * - OmronFINS_UDP: FINS协议UDP模式
 * - OmronHostLink: HostLink串口协议
 *
 * ## 西门子PLC协议 (50-59)
 * - SiemensS7: S7协议，适用于S7-200/300/400/1200/1500系列
 *
 * ## 松下PLC协议 (60-69)
 * - PanasonicMewtocol: Mewtocol协议，用于FP系列PLC
 */
enum class PLCProtocol {
    Unknown = -1,               ///< 未知协议

    // ===== Modbus协议族 (10-19) =====
    ModbusTCP = 10,             ///< Modbus TCP协议 (网口，端口502)
    ModbusRTU,                  ///< Modbus RTU协议 (串口，二进制格式)
    ModbusASCII,                ///< Modbus ASCII协议 (串口，ASCII格式)

    // ===== 三菱PLC协议 (20-29) =====
    MitsubishiMC_TCP = 20,      ///< 三菱MC协议 TCP模式 (Q/L系列)
    MitsubishiMC_UDP,           ///< 三菱MC协议 UDP模式
    MitsubishiFX_Serial,        ///< 三菱FX系列串口协议

    // ===== 基恩士PLC协议 (30-39) =====
    KeyencePCLink = 30,         ///< 基恩士PC-LINK上位链路协议

    // ===== 欧姆龙PLC协议 (40-49) =====
    OmronFINS_TCP = 40,         ///< 欧姆龙FINS协议 TCP模式
    OmronFINS_UDP,              ///< 欧姆龙FINS协议 UDP模式
    OmronHostLink,              ///< 欧姆龙HostLink串口协议

    // ===== 西门子PLC协议 (50-59) =====
    SiemensS7 = 50,             ///< 西门子S7协议

    // ===== 松下PLC协议 (60-69) =====
    PanasonicMewtocol = 60,     ///< 松下Mewtocol协议
};

/**
 * @enum RegisterType
 * @brief 寄存器数据类型枚举
 *
 * @details
 * 定义了PLC寄存器中存储的数据类型。
 *
 * ## 类型说明
 * | 类型 | 大小 | 占用寄存器数 | 说明 |
 * |------|------|------------|------|
 * | Bit | 1位 | - | 继电器、线圈等位元件 |
 * | Word | 16位 | 1 | 标准字寄存器 |
 * | DWord | 32位 | 2 | 双字，用于大数值 |
 * | Float | 32位 | 2 | IEEE 754单精度浮点 |
 * | Double | 64位 | 4 | IEEE 754双精度浮点 |
 * | String | 可变 | 可变 | ASCII字符串 |
 */
enum class RegisterType {
    Bit,            ///< 位元件 (如M、X、Y继电器)
    Word,           ///< 字 (16位整数，占1个寄存器)
    DWord,          ///< 双字 (32位整数，占2个寄存器)
    Float,          ///< 单精度浮点 (32位，占2个寄存器)
    Double,         ///< 双精度浮点 (64位，占4个寄存器)
    String          ///< 字符串 (长度可变)
};

/**
 * @enum MitsubishiDeviceType
 * @brief 三菱PLC设备类型枚举
 *
 * @details
 * 定义了三菱PLC中各种软元件（设备）的类型代码。
 *
 * ## 软元件说明
 * | 类型 | 名称 | 用途 |
 * |------|------|------|
 * | D | 数据寄存器 | 存储数值数据，16位 |
 * | M | 内部继电器 | 程序内部标志位 |
 * | X | 输入继电器 | 连接外部输入信号 |
 * | Y | 输出继电器 | 控制外部输出 |
 * | W | 链接寄存器 | 网络通信数据 |
 * | B | 链接继电器 | 网络通信标志 |
 * | R | 文件寄存器 | 大容量数据存储 |
 * | ZR | 扩展文件寄存器 | 更大容量存储 |
 * | T | 定时器 | 延时控制 |
 * | C | 计数器 | 计数控制 |
 * | ST | 累计定时器 | 累计时间测量 |
 *
 * ## 地址格式示例
 * - D0: 数据寄存器0号
 * - M100: 内部继电器100号
 * - X0: 输入X0
 * - Y10: 输出Y10（八进制地址）
 */
enum class MitsubishiDeviceType {
    D,      ///< 数据寄存器 (Data Register)
    M,      ///< 内部继电器 (Internal Relay)
    X,      ///< 输入继电器 (Input)
    Y,      ///< 输出继电器 (Output)
    W,      ///< 链接寄存器 (Link Register)
    B,      ///< 链接继电器 (Link Relay)
    R,      ///< 文件寄存器 (File Register)
    ZR,     ///< 扩展文件寄存器 (Extended File Register)
    T,      ///< 定时器 (Timer)
    C,      ///< 计数器 (Counter)
    ST,     ///< 累计定时器 (Retentive Timer)
};

/**
 * @struct CommConfig
 * @brief 通信配置基类
 *
 * @details
 * 所有通信配置的基类，包含通用的超时和重试设置。
 * 派生类包括SerialConfig（串口）和SocketConfig（网口）。
 *
 * @note 使用虚析构函数以支持多态
 */
struct CommConfig {
    CommType type = CommType::Unknown;  ///< 通信方式类型
    int timeout = 3000;                 ///< 超时时间（毫秒），默认3秒
    int retryCount = 3;                 ///< 失败重试次数，默认3次

    /// @brief 虚析构函数
    virtual ~CommConfig() = default;
};

/**
 * @struct SerialConfig
 * @brief 串口通信配置
 *
 * @details
 * 继承自CommConfig，包含串口通信所需的全部参数。
 *
 * ## 常用配置组合
 * - 标准: 9600, 8, N, 1 (9600波特率, 8数据位, 无校验, 1停止位)
 * - 高速: 115200, 8, N, 1
 * - Modbus RTU常用: 9600/19200, 8, E, 1 (偶校验)
 *
 * ## 校验位说明
 * - 0: 无校验 (None)
 * - 1: 奇校验 (Odd)
 * - 2: 偶校验 (Even)
 */
struct SerialConfig : public CommConfig {
    QString portName;           ///< 串口名称 (如 "COM1", "COM2", "/dev/ttyUSB0")
    int baudRate = 9600;        ///< 波特率 (300/1200/2400/4800/9600/19200/38400/57600/115200)
    int dataBits = 8;           ///< 数据位 (5/6/7/8)
    int stopBits = 1;           ///< 停止位 (1/2)
    int parity = 0;             ///< 校验位 (0=无校验, 1=奇校验, 2=偶校验)

    /// @brief 构造函数，自动设置通信类型为Serial
    SerialConfig() { type = CommType::Serial; }
};

/**
 * @struct SocketConfig
 * @brief 网口通信配置
 *
 * @details
 * 继承自CommConfig，包含TCP/UDP网络通信所需的参数。
 *
 * ## 常用端口号
 * - Modbus TCP: 502
 * - 三菱MC协议: 5000-5002 (可配置)
 * - 基恩士PC-Link: 8501
 * - 欧姆龙FINS: 9600
 *
 * ## TCP vs UDP
 * - TCP: 可靠传输，有连接状态，适合大多数应用
 * - UDP: 无连接，速度快，适合实时性要求高的场景
 */
struct SocketConfig : public CommConfig {
    QString ipAddress;          ///< IP地址 (如 "192.168.1.100")
    quint16 port = 502;         ///< 端口号 (1-65535)
    bool useTcp = true;         ///< true=TCP, false=UDP

    /// @brief 构造函数，自动设置通信类型为Socket
    SocketConfig() { type = CommType::Socket; }
};

/**
 * @struct PLCConfig
 * @brief PLC连接配置
 *
 * @details
 * 完整的PLC连接配置，包含协议选择、通信配置和协议特定参数。
 *
 * ## 配置项说明
 *
 * ### 通用配置
 * - name: 连接的唯一标识名称
 * - protocol: 使用的通信协议
 * - commConfig: 底层通信配置（串口或网口）
 *
 * ### Modbus特有配置
 * - slaveId: 从站地址 (1-247)
 *
 * ### 三菱MC协议特有配置
 * - networkNo: 网络号 (0-255)
 * - pcNo: PC号，通常为0xFF表示自动
 * - destModuleIo: 目标模块I/O号，Q系列通常为0x03FF
 * - destModuleStation: 目标模块站号
 *
 * ## 使用示例
 * @code
 * // Modbus TCP配置
 * PLCConfig config;
 * config.name = "生产线PLC";
 * config.protocol = PLCProtocol::ModbusTCP;
 * config.slaveId = 1;
 * auto socketConfig = std::make_shared<SocketConfig>();
 * socketConfig->ipAddress = "192.168.1.100";
 * socketConfig->port = 502;
 * config.commConfig = socketConfig;
 * @endcode
 */
struct PLCConfig {
    QString name;                               ///< 连接名称（用于标识和管理）
    PLCProtocol protocol = PLCProtocol::ModbusTCP;  ///< 通信协议类型
    std::shared_ptr<CommConfig> commConfig;     ///< 底层通信配置

    // ===== Modbus协议特有配置 =====
    int slaveId = 1;                            ///< Modbus从站地址 (1-247)

    // ===== 三菱MC协议特有配置 =====
    int networkNo = 0;                          ///< 网络号 (0表示本地网络)
    int pcNo = 0xFF;                            ///< PC号 (0xFF=广播/自动)
    int destModuleIo = 0x03FF;                  ///< 目标模块I/O号
    int destModuleStation = 0;                  ///< 目标模块站号
};

/**
 * @struct PLCResult
 * @brief PLC读写操作结果
 *
 * @details
 * 封装了PLC读写操作的返回结果，包括状态和数据。
 *
 * ## 结果检查
 * 首先检查status或调用isSuccess()确认操作成功，然后访问相应的数据数组。
 *
 * ## 数据存储
 * - intValues: 整数值数组（用于字/双字寄存器）
 * - doubleValues: 浮点值数组（用于浮点寄存器）
 * - boolValues: 布尔值数组（用于线圈/位元件）
 * - stringValue: 字符串值（用于字符串数据）
 *
 * ## 使用示例
 * @code
 * PLCResult result = plc->readRegisters(0, 10);
 * if (result.isSuccess()) {
 *     for (int value : result.intValues) {
 *         qDebug() << "读取值:" << value;
 *     }
 * } else {
 *     qWarning() << "读取失败:" << result.errorMessage;
 * }
 * @endcode
 */
struct PLCResult {
    CommStatus status = CommStatus::Success;    ///< 操作状态
    QString errorMessage;                       ///< 错误描述（status非Success时有效）
    std::vector<int> intValues;                 ///< 整数结果数组
    std::vector<double> doubleValues;           ///< 浮点数结果数组
    std::vector<bool> boolValues;               ///< 布尔值结果数组
    QString stringValue;                        ///< 字符串结果

    /**
     * @brief 检查操作是否成功
     * @return 成功返回true，失败返回false
     */
    bool isSuccess() const { return status == CommStatus::Success; }
};

/**
 * @brief 获取PLC协议的显示名称
 * @param protocol 协议类型枚举值
 * @return 协议的中文名称字符串
 *
 * @details
 * 将PLCProtocol枚举值转换为用户友好的中文名称，
 * 用于UI显示和日志输出。
 *
 * @code
 * QString name = getProtocolName(PLCProtocol::ModbusTCP);
 * // 返回 "Modbus TCP"
 * @endcode
 */
inline QString getProtocolName(PLCProtocol protocol) {
    switch (protocol) {
    // Modbus协议
    case PLCProtocol::ModbusTCP: return "Modbus TCP";
    case PLCProtocol::ModbusRTU: return "Modbus RTU";
    case PLCProtocol::ModbusASCII: return "Modbus ASCII";

    // 三菱PLC协议
    case PLCProtocol::MitsubishiMC_TCP: return "三菱MC协议(TCP)";
    case PLCProtocol::MitsubishiMC_UDP: return "三菱MC协议(UDP)";
    case PLCProtocol::MitsubishiFX_Serial: return "三菱FX串口";

    // 基恩士PLC协议
    case PLCProtocol::KeyencePCLink: return "基恩士PC-LINK上位链路";

    // 欧姆龙PLC协议
    case PLCProtocol::OmronFINS_TCP: return "欧姆龙FINS(TCP)";
    case PLCProtocol::OmronFINS_UDP: return "欧姆龙FINS(UDP)";
    case PLCProtocol::OmronHostLink: return "欧姆龙HostLink";

    // 西门子PLC协议
    case PLCProtocol::SiemensS7: return "西门子S7";

    // 松下PLC协议
    case PLCProtocol::PanasonicMewtocol: return "松下Mewtocol";

    default: return "未知协议";
    }
}

} // namespace Comm
} // namespace VisionForge
