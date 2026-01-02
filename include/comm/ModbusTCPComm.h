/**
 * @file ModbusTCPComm.h
 * @brief Modbus TCP通信协议实现头文件
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @details
 * 本文件实现了Modbus TCP通信协议，遵循Modbus协议规范。
 * Modbus是一种开放的工业通信协议，广泛应用于PLC、仪表等工业设备。
 *
 * ## Modbus TCP协议简介
 * Modbus TCP是将Modbus协议封装在TCP/IP协议上的实现。
 * 相比串口版本(RTU/ASCII)，TCP版本更加可靠且易于部署。
 *
 * ## 帧结构
 * Modbus TCP ADU (应用数据单元) 包含:
 * ```
 * +----------------+----------------+----------------+
 * |   MBAP Header  |  Function Code |      Data      |
 * |   (7 bytes)    |    (1 byte)    |   (N bytes)    |
 * +----------------+----------------+----------------+
 * ```
 *
 * ### MBAP头结构 (Modbus Application Protocol Header)
 * | 字节 | 名称 | 说明 |
 * |------|------|------|
 * | 0-1 | Transaction ID | 事务标识符，用于匹配请求和响应 |
 * | 2-3 | Protocol ID | 协议标识符，固定为0x0000 |
 * | 4-5 | Length | 后续字节数 |
 * | 6 | Unit ID | 从站地址 |
 *
 * ## 支持的功能码
 * | 功能码 | 名称 | 说明 |
 * |--------|------|------|
 * | 0x01 | Read Coils | 读取线圈状态 |
 * | 0x02 | Read Discrete Inputs | 读取离散输入 |
 * | 0x03 | Read Holding Registers | 读取保持寄存器 |
 * | 0x04 | Read Input Registers | 读取输入寄存器 |
 * | 0x05 | Write Single Coil | 写单个线圈 |
 * | 0x06 | Write Single Register | 写单个寄存器 |
 * | 0x0F | Write Multiple Coils | 写多个线圈 |
 * | 0x10 | Write Multiple Registers | 写多个寄存器 |
 *
 * ## 寄存器地址映射
 * | 地址范围 | 类型 | 读写权限 |
 * |----------|------|----------|
 * | 00001-09999 | 线圈(Coil) | 读/写 |
 * | 10001-19999 | 离散输入 | 只读 |
 * | 30001-39999 | 输入寄存器 | 只读 |
 * | 40001-49999 | 保持寄存器 | 读/写 |
 *
 * @see PLCComm 基类定义
 * @see https://modbus.org/specs.php Modbus协议规范
 */

#pragma once

#include "comm/PLCComm.h"
#include <QTcpSocket>

namespace VisionForge {
namespace Comm {

/**
 * @class ModbusTCPComm
 * @brief Modbus TCP通信类
 *
 * @details
 * 实现了完整的Modbus TCP客户端功能，支持:
 * - 读取线圈 (0x01)
 * - 读取离散输入 (0x02)
 * - 读取保持寄存器 (0x03)
 * - 读取输入寄存器 (0x04)
 * - 写单个线圈 (0x05)
 * - 写单个寄存器 (0x06)
 * - 写多个线圈 (0x0F)
 * - 写多个寄存器 (0x10)
 *
 * ## 使用示例
 * @code
 * ModbusTCPComm modbus;
 *
 * // 配置连接
 * PLCConfig config;
 * config.slaveId = 1;
 * auto socketConfig = std::make_shared<SocketConfig>();
 * socketConfig->ipAddress = "192.168.1.100";
 * socketConfig->port = 502;
 * config.commConfig = socketConfig;
 *
 * // 连接
 * if (modbus.connect(config)) {
 *     // 读取10个保持寄存器
 *     PLCResult result = modbus.readRegisters(0, 10);
 *     if (result.isSuccess()) {
 *         for (int value : result.intValues) {
 *             qDebug() << "寄存器值:" << value;
 *         }
 *     }
 *
 *     // 写入寄存器
 *     modbus.writeRegisters(100, {1234, 5678});
 *
 *     // 断开连接
 *     modbus.disconnect();
 * }
 * @endcode
 *
 * ## 错误处理
 * 当PLC返回异常响应时，errorMessage会包含对应的异常描述:
 * - 0x01: 非法功能码
 * - 0x02: 非法数据地址
 * - 0x03: 非法数据值
 * - 0x04: 从站设备故障
 * 等等
 */
class ModbusTCPComm : public PLCComm
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     *
     * @details
     * 初始化TCP套接字并连接相关信号。
     * 默认协议类型设置为PLCProtocol::ModbusTCP。
     */
    explicit ModbusTCPComm(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 自动断开连接并释放资源
     */
    ~ModbusTCPComm() override;

    /**
     * @brief 连接到Modbus TCP服务器
     * @param config PLC配置，需包含SocketConfig
     * @return 连接成功返回true
     *
     * @details
     * 从config中提取IP地址、端口号和从站地址，建立TCP连接。
     *
     * ## 配置要求
     * - config.commConfig: 必须是SocketConfig类型
     * - config.slaveId: Modbus从站地址 (1-247)
     *
     * @note 使用阻塞方式等待连接，超时时间由config.commConfig->timeout指定
     */
    bool connect(const PLCConfig& config) override;

    /**
     * @brief 断开连接
     * @details 关闭TCP连接，等待断开完成
     */
    void disconnect() override;

    /**
     * @brief 检查连接状态
     * @return TCP连接已建立返回true
     */
    bool isConnected() const override;

    /**
     * @brief 读取保持寄存器 (功能码03)
     * @param address 起始寄存器地址 (0-65535)
     * @param count 读取的寄存器数量 (1-125)
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 使用功能码0x03读取保持寄存器(Holding Registers)。
     * 保持寄存器是可读写的16位寄存器，对应Modbus地址4xxxx。
     *
     * ## 地址示例
     * - 地址0 -> Modbus地址40001
     * - 地址99 -> Modbus地址40100
     *
     * @note 单次最多读取125个寄存器
     */
    PLCResult readRegisters(int address, int count) override;

    /**
     * @brief 写入保持寄存器
     * @param address 起始寄存器地址
     * @param values 要写入的值数组
     * @return PLCResult包含写入状态
     *
     * @details
     * - 单个值时使用功能码0x06 (Write Single Register)
     * - 多个值时使用功能码0x10 (Write Multiple Registers)
     *
     * ## 值范围
     * 每个值被截断为16位 (0-65535或-32768 to 32767)
     *
     * @note 单次最多写入123个寄存器
     */
    PLCResult writeRegisters(int address, const std::vector<int>& values) override;

    /**
     * @brief 读取线圈 (功能码01)
     * @param address 起始线圈地址 (0-65535)
     * @param count 读取的线圈数量 (1-2000)
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 使用功能码0x01读取线圈(Coils)状态。
     * 线圈是可读写的位元件，对应Modbus地址0xxxx。
     *
     * ## 数据格式
     * 返回的boolValues数组包含每个线圈的ON/OFF状态。
     *
     * @note 单次最多读取2000个线圈
     */
    PLCResult readCoils(int address, int count) override;

    /**
     * @brief 写入线圈
     * @param address 起始线圈地址
     * @param values 要写入的布尔值数组
     * @return PLCResult包含写入状态
     *
     * @details
     * - 单个值时使用功能码0x05 (Write Single Coil)
     *   - ON: 发送0xFF00
     *   - OFF: 发送0x0000
     * - 多个值时使用功能码0x0F (Write Multiple Coils)
     *   - 布尔值打包为字节，低位在前
     *
     * @note 单次最多写入1968个线圈
     */
    PLCResult writeCoils(int address, const std::vector<bool>& values) override;

    /**
     * @brief 读取输入寄存器 (功能码04)
     * @param address 起始寄存器地址 (0-65535)
     * @param count 读取的寄存器数量 (1-125)
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 使用功能码0x04读取输入寄存器(Input Registers)。
     * 输入寄存器是只读的16位寄存器，对应Modbus地址3xxxx。
     *
     * ## 典型应用
     * 输入寄存器通常用于读取模拟量输入、传感器数据等。
     */
    PLCResult readInputRegisters(int address, int count);

    /**
     * @brief 读取离散输入 (功能码02)
     * @param address 起始输入地址 (0-65535)
     * @param count 读取的输入数量 (1-2000)
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 使用功能码0x02读取离散输入(Discrete Inputs)。
     * 离散输入是只读的位元件，对应Modbus地址1xxxx。
     *
     * ## 典型应用
     * 离散输入通常连接开关、传感器等数字输入信号。
     */
    PLCResult readDiscreteInputs(int address, int count);

private:
    /**
     * @brief 发送Modbus请求并接收响应
     * @param request 完整的Modbus TCP请求帧
     * @return 接收到的响应数据，失败返回空数组
     *
     * @details
     * 实现流程:
     * 1. 通过TCP发送请求数据
     * 2. 等待并读取MBAP头(7字节)
     * 3. 根据MBAP头中的长度字段读取剩余数据
     *
     * @note 使用同步方式发送和接收，阻塞直到完成或超时
     */
    QByteArray sendRequest(const QByteArray& request);

    /**
     * @brief 构建Modbus TCP请求帧
     * @param functionCode 功能码 (0x01-0x10)
     * @param data PDU数据部分
     * @return 完整的Modbus TCP ADU帧
     *
     * @details
     * 构建的帧结构:
     * ```
     * | Transaction ID (2B) | Protocol ID (2B) | Length (2B) | Unit ID (1B) | Function (1B) | Data |
     * ```
     *
     * - Transaction ID自动递增，用于匹配请求和响应
     * - Protocol ID固定为0x0000
     * - Length = Unit ID + Function Code + Data
     */
    QByteArray buildFrame(quint8 functionCode, const QByteArray& data);

    /**
     * @brief 解析Modbus响应帧
     * @param response 接收到的响应数据
     * @param expectedFunctionCode 期望的功能码
     * @return PLCResult包含解析结果
     *
     * @details
     * 解析流程:
     * 1. 验证响应长度
     * 2. 检查Transaction ID匹配
     * 3. 检查是否为异常响应(功能码最高位为1)
     * 4. 验证功能码匹配
     * 5. 根据功能码类型解析数据
     *
     * ## 数据解析
     * - 寄存器读取: 提取16位值到intValues
     * - 线圈读取: 解包位数据到boolValues
     * - 写入操作: 仅验证成功
     */
    PLCResult parseResponse(const QByteArray& response, quint8 expectedFunctionCode);

    /**
     * @brief 获取Modbus异常码的错误描述
     * @param exceptionCode 异常码 (0x01-0x0B)
     * @return 中文错误描述
     *
     * @details
     * ## 异常码对照表
     * | 代码 | 含义 |
     * |------|------|
     * | 0x01 | 非法功能码 |
     * | 0x02 | 非法数据地址 |
     * | 0x03 | 非法数据值 |
     * | 0x04 | 从站设备故障 |
     * | 0x05 | 确认(处理中) |
     * | 0x06 | 从站设备忙 |
     * | 0x08 | 存储奇偶性差错 |
     * | 0x0A | 网关路径不可用 |
     * | 0x0B | 网关目标设备未响应 |
     */
    QString getExceptionMessage(quint8 exceptionCode);

private:
    QTcpSocket* socket_;        ///< TCP套接字对象
    quint16 transactionId_;     ///< 事务ID计数器，每次请求自增
    quint8 slaveId_;            ///< Modbus从站地址
};

} // namespace Comm
} // namespace VisionForge
