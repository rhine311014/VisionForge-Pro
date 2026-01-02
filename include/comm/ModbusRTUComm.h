/**
 * @file ModbusRTUComm.h
 * @brief Modbus RTU串口通信协议实现头文件
 * @author VisionForge Team
 * @date 2025-12-18
 *
 * @details
 * 本文件实现了Modbus RTU串口通信协议，用于通过RS232/RS485与PLC及其他设备通信。
 *
 * ## Modbus RTU协议简介
 * Modbus RTU是一种紧凑的二进制协议，通过串口进行通信。
 * 与ASCII模式相比，RTU模式具有更高的传输效率。
 *
 * ## 帧结构
 * Modbus RTU帧格式：
 * ```
 * +----------+---------------+--------+----------+
 * | 从站地址  |    功能码     |  数据   | CRC校验  |
 * | 1 byte   |    1 byte     | N bytes| 2 bytes  |
 * +----------+---------------+--------+----------+
 * ```
 *
 * ## CRC校验
 * 使用CRC-16算法（多项式0xA001），低字节在前。
 * CRC计算覆盖从站地址到数据的所有字节。
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
 * ## 通信时序
 * - 帧间隔：至少3.5个字符时间
 * - 字符间隔：不超过1.5个字符时间
 *
 * @see PLCComm 基类定义
 * @see ModbusTCPComm Modbus TCP实现
 */

#pragma once

#include "comm/PLCComm.h"
#include <QSerialPort>
#include <QByteArray>

namespace VisionForge {
namespace Comm {

/**
 * @class ModbusRTUComm
 * @brief Modbus RTU串口通信类
 *
 * @details
 * 实现了完整的Modbus RTU客户端功能，支持通过串口与从站设备通信。
 *
 * ## 主要特性
 * - 支持所有标准Modbus功能码（0x01-0x10）
 * - 自动计算和验证CRC校验
 * - 支持自定义串口参数（波特率、数据位、校验位、停止位）
 * - 线程安全的读写操作
 *
 * ## 使用示例
 * @code
 * ModbusRTUComm modbus;
 *
 * // 配置连接
 * PLCConfig config;
 * config.slaveId = 1;
 * auto serialConfig = std::make_shared<SerialConfig>();
 * serialConfig->portName = "COM1";
 * serialConfig->baudRate = 9600;
 * serialConfig->dataBits = 8;
 * serialConfig->parity = 0;  // 无校验
 * serialConfig->stopBits = 1;
 * config.commConfig = serialConfig;
 *
 * // 连接
 * if (modbus.connect(config)) {
 *     // 读取寄存器
 *     PLCResult result = modbus.readRegisters(0, 10);
 *     if (result.isSuccess()) {
 *         for (int value : result.intValues) {
 *             qDebug() << "寄存器值:" << value;
 *         }
 *     }
 *
 *     modbus.disconnect();
 * }
 * @endcode
 *
 * ## 常用串口配置
 * - 9600, 8, N, 1 (标准配置)
 * - 9600, 8, E, 1 (偶校验，Modbus推荐)
 * - 19200, 8, N, 1 (较高速率)
 */
class ModbusRTUComm : public PLCComm
{
    Q_OBJECT

public:
    /**
     * @enum FunctionCode
     * @brief Modbus功能码枚举
     *
     * @details
     * 定义了Modbus协议支持的标准功能码。
     */
    enum FunctionCode {
        ReadCoils = 0x01,               ///< 读线圈（输出状态）
        ReadDiscreteInputs = 0x02,      ///< 读离散输入（输入状态）
        ReadHoldingRegisters = 0x03,    ///< 读保持寄存器
        ReadInputRegisters = 0x04,      ///< 读输入寄存器
        WriteSingleCoil = 0x05,         ///< 写单个线圈
        WriteSingleRegister = 0x06,     ///< 写单个寄存器
        WriteMultipleCoils = 0x0F,      ///< 写多个线圈
        WriteMultipleRegisters = 0x10   ///< 写多个寄存器
    };

    /**
     * @brief 构造函数
     * @param parent 父对象指针
     *
     * @details
     * 初始化串口对象，默认从站地址为1。
     */
    explicit ModbusRTUComm(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 自动关闭串口连接
     */
    ~ModbusRTUComm() override;

    // ========== PLCComm接口实现 ==========

    /**
     * @brief 连接串口
     * @param config PLC配置，需包含SerialConfig
     * @return 连接成功返回true
     *
     * @details
     * 配置并打开串口。配置参数包括：
     * - 串口名称（如COM1、COM2）
     * - 波特率（9600、19200、38400等）
     * - 数据位（5、6、7、8）
     * - 校验位（无、奇、偶）
     * - 停止位（1、2）
     */
    bool connect(const PLCConfig& config) override;

    /**
     * @brief 断开串口连接
     */
    void disconnect() override;

    /**
     * @brief 检查串口是否已打开
     * @return 串口已打开返回true
     */
    bool isConnected() const override;

    /**
     * @brief 读取保持寄存器（使用默认从站地址）
     * @param address 起始寄存器地址
     * @param count 读取的寄存器数量
     * @return PLCResult包含读取状态和数据
     */
    PLCResult readRegisters(int address, int count) override;

    /**
     * @brief 写入保持寄存器（使用默认从站地址）
     * @param address 起始寄存器地址
     * @param values 要写入的值数组
     * @return PLCResult包含写入状态
     *
     * @details
     * - 单个值时使用功能码0x06
     * - 多个值时使用功能码0x10
     */
    PLCResult writeRegisters(int address, const std::vector<int>& values) override;

    /**
     * @brief 读取线圈（使用默认从站地址）
     * @param address 起始线圈地址
     * @param count 读取的线圈数量
     * @return PLCResult包含读取状态和数据
     */
    PLCResult readCoils(int address, int count) override;

    /**
     * @brief 写入线圈（使用默认从站地址）
     * @param address 起始线圈地址
     * @param values 要写入的布尔值数组
     * @return PLCResult包含写入状态
     *
     * @details
     * - 单个值时使用功能码0x05
     * - 多个值时使用功能码0x0F
     */
    PLCResult writeCoils(int address, const std::vector<bool>& values) override;

    // ========== Modbus RTU特有接口 ==========

    /**
     * @brief 读取保持寄存器（指定从站地址）
     * @param slaveId 从站地址 (1-247)
     * @param address 起始寄存器地址
     * @param count 读取数量
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 使用功能码0x03读取保持寄存器。
     * 允许指定从站地址，适用于一个串口总线上有多个从站的情况。
     */
    PLCResult readHoldingRegisters(int slaveId, int address, int count);

    /**
     * @brief 读取输入寄存器（指定从站地址）
     * @param slaveId 从站地址
     * @param address 起始寄存器地址
     * @param count 读取数量
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 使用功能码0x04读取输入寄存器。
     * 输入寄存器通常用于模拟量输入、传感器数据等只读数据。
     */
    PLCResult readInputRegisters(int slaveId, int address, int count);

    /**
     * @brief 写入单个寄存器（指定从站地址）
     * @param slaveId 从站地址
     * @param address 寄存器地址
     * @param value 要写入的值
     * @return PLCResult包含写入状态
     *
     * @details
     * 使用功能码0x06写入单个保持寄存器。
     */
    PLCResult writeSingleRegister(int slaveId, int address, int value);

    /**
     * @brief 写入多个寄存器（指定从站地址）
     * @param slaveId 从站地址
     * @param address 起始寄存器地址
     * @param values 要写入的值数组
     * @return PLCResult包含写入状态
     *
     * @details
     * 使用功能码0x10写入多个保持寄存器。
     */
    PLCResult writeMultipleRegisters(int slaveId, int address, const std::vector<int>& values);

    /**
     * @brief 读取离散输入（指定从站地址）
     * @param slaveId 从站地址
     * @param address 起始输入地址
     * @param count 读取数量
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 使用功能码0x02读取离散输入。
     * 离散输入通常连接外部开关、传感器等数字输入信号。
     */
    PLCResult readDiscreteInputs(int slaveId, int address, int count);

    /**
     * @brief 写入单个线圈（指定从站地址）
     * @param slaveId 从站地址
     * @param address 线圈地址
     * @param value 要写入的值
     * @return PLCResult包含写入状态
     *
     * @details
     * 使用功能码0x05写入单个线圈。
     * - ON: 发送0xFF00
     * - OFF: 发送0x0000
     */
    PLCResult writeSingleCoil(int slaveId, int address, bool value);

    /**
     * @brief 写入多个线圈（指定从站地址）
     * @param slaveId 从站地址
     * @param address 起始线圈地址
     * @param values 要写入的布尔值数组
     * @return PLCResult包含写入状态
     *
     * @details
     * 使用功能码0x0F写入多个线圈。
     * 布尔值打包为字节，低位在前。
     */
    PLCResult writeMultipleCoils(int slaveId, int address, const std::vector<bool>& values);

    // ========== 串口配置 ==========

    /**
     * @brief 设置默认从站地址
     * @param id 从站地址 (1-247)
     */
    void setSlaveId(int id) { slaveId_ = id; }

    /**
     * @brief 获取当前默认从站地址
     * @return 从站地址
     */
    int slaveId() const { return slaveId_; }

    /**
     * @brief 获取系统可用的串口列表
     * @return 串口名称列表（如"COM1", "COM2"...）
     *
     * @details
     * 静态方法，无需创建对象即可调用。
     * 可用于在UI中显示可选择的串口列表。
     */
    static QStringList availablePorts();

private:
    /**
     * @brief 发送请求并接收响应
     * @param request 请求帧数据
     * @param expectedLength 期望的响应长度
     * @return 接收到的响应数据
     *
     * @details
     * 实现串口通信的核心方法：
     * 1. 清空接收缓冲区
     * 2. 发送请求数据
     * 3. 等待并接收响应
     * 4. 返回完整响应帧
     */
    QByteArray sendAndReceive(const QByteArray& request, int expectedLength);

    /**
     * @brief 构建标准Modbus请求帧
     * @param slaveId 从站地址
     * @param func 功能码
     * @param address 寄存器/线圈地址
     * @param countOrValue 数量或值
     * @return 完整的请求帧（包含CRC）
     *
     * @details
     * 帧格式：从站地址(1) + 功能码(1) + 地址(2) + 数量/值(2) + CRC(2)
     */
    QByteArray buildRequest(int slaveId, FunctionCode func, int address, int countOrValue);

    /**
     * @brief 构建写多个寄存器请求帧
     * @param slaveId 从站地址
     * @param address 起始地址
     * @param values 要写入的值数组
     * @return 完整的请求帧
     *
     * @details
     * 帧格式：从站地址(1) + 功能码(1) + 地址(2) + 数量(2) + 字节数(1) + 数据(N) + CRC(2)
     */
    QByteArray buildWriteMultipleRegistersRequest(int slaveId, int address, const std::vector<int>& values);

    /**
     * @brief 构建写多个线圈请求帧
     * @param slaveId 从站地址
     * @param address 起始地址
     * @param values 要写入的布尔值数组
     * @return 完整的请求帧
     *
     * @details
     * 布尔值打包为字节，低位在前。
     */
    QByteArray buildWriteMultipleCoilsRequest(int slaveId, int address, const std::vector<bool>& values);

    /**
     * @brief 解析Modbus响应帧
     * @param response 接收到的响应数据
     * @param expectedFunc 期望的功能码
     * @return PLCResult包含解析结果
     *
     * @details
     * 解析流程：
     * 1. 验证CRC校验
     * 2. 检查是否为异常响应
     * 3. 根据功能码类型解析数据
     */
    PLCResult parseResponse(const QByteArray& response, FunctionCode expectedFunc);

    /**
     * @brief 计算CRC-16校验码
     * @param data 要计算校验的数据
     * @return 16位CRC校验码
     *
     * @details
     * 使用Modbus标准CRC-16算法：
     * - 初始值：0xFFFF
     * - 多项式：0xA001（反转的0x8005）
     * - 字节顺序：低字节在前
     */
    static quint16 calculateCRC16(const QByteArray& data);

    /**
     * @brief 验证响应帧的CRC校验
     * @param data 完整的响应帧
     * @return CRC正确返回true
     *
     * @details
     * 比较响应帧中的CRC与重新计算的CRC是否一致。
     */
    static bool verifyCRC(const QByteArray& data);

private:
    QSerialPort* serialPort_;   ///< 串口对象
    int slaveId_;               ///< 默认从站地址
};

} // namespace Comm
} // namespace VisionForge
