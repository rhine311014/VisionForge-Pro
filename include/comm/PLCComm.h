/**
 * @file PLCComm.h
 * @brief PLC通信基类头文件
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @details
 * 本文件定义了PLC通信的抽象基类，为不同PLC协议提供统一的通信接口。
 *
 * ## 功能概述
 * - 提供PLC连接/断开的统一接口
 * - 定义寄存器读写的抽象方法
 * - 定义线圈/位读写的抽象方法
 * - 支持多种数据类型：字(16位)、双字(32位)、浮点数
 *
 * ## 支持的PLC协议
 * - Modbus TCP/RTU
 * - 三菱MC协议 (TCP/UDP)
 * - 基恩士PC-Link协议
 * - OPC UA
 *
 * ## 数据格式说明
 * - 字寄存器：16位有符号/无符号整数
 * - 双字寄存器：32位整数，由2个连续字寄存器组成（低字在前，高字在后）
 * - 浮点数：IEEE 754单精度格式，占用2个字寄存器
 * - 线圈/位：布尔值
 *
 * ## 线程安全
 * 所有读写操作都通过互斥锁保护，支持多线程并发访问。
 *
 * @see PLCDef.h 查看PLC相关数据结构定义
 * @see ModbusTCPComm 查看Modbus TCP协议实现
 * @see MitsubishiMCComm 查看三菱MC协议实现
 * @see KeyencePCLinkComm 查看基恩士PC-Link协议实现
 */

#pragma once

#include "comm/PLCDef.h"
#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <memory>

namespace VisionForge {
namespace Comm {

/**
 * @class PLCComm
 * @brief PLC通信抽象基类
 *
 * @details
 * 本类是所有PLC通信类的基类，定义了通用的通信接口。
 * 子类需要实现以下纯虚函数：
 * - connect(): 建立PLC连接
 * - disconnect(): 断开PLC连接
 * - isConnected(): 获取连接状态
 * - readRegisters(): 读取字寄存器
 * - writeRegisters(): 写入字寄存器
 * - readCoils(): 读取线圈/位
 * - writeCoils(): 写入线圈/位
 *
 * 基类提供了双字和浮点数读写的默认实现，子类可根据需要重写这些方法。
 *
 * @note 所有读写操作应在连接成功后调用，否则会返回错误状态。
 *
 * ## 使用示例
 * @code
 * // 创建PLC通信实例（使用工厂函数）
 * auto plc = createPLCComm(PLCProtocol::ModbusTCP);
 *
 * // 配置连接参数
 * PLCConfig config;
 * config.ip = "192.168.1.100";
 * config.port = 502;
 *
 * // 连接PLC
 * if (plc->connect(config)) {
 *     // 读取寄存器
 *     PLCResult result = plc->readRegisters(0, 10);
 *     if (result.isSuccess()) {
 *         for (int value : result.intValues) {
 *             qDebug() << "值:" << value;
 *         }
 *     }
 *
 *     // 写入寄存器
 *     plc->writeRegisters(100, {1, 2, 3});
 *
 *     // 断开连接
 *     plc->disconnect();
 * }
 * @endcode
 */
class PLCComm : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，用于Qt对象树管理
     */
    explicit PLCComm(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 虚析构函数，确保派生类正确析构
     */
    virtual ~PLCComm();

    /**
     * @brief 连接PLC
     * @param config PLC配置参数，包含IP地址、端口号等信息
     * @return 连接成功返回true，失败返回false
     *
     * @details
     * 纯虚函数，由子类根据具体协议实现连接逻辑。
     * 连接失败时可通过getLastError()获取错误信息。
     *
     * @note 调用此方法前请确保已正确配置config参数
     * @see PLCConfig 查看配置参数详情
     */
    virtual bool connect(const PLCConfig& config) = 0;

    /**
     * @brief 断开PLC连接
     *
     * @details
     * 纯虚函数，由子类实现断开连接的逻辑。
     * 调用后isConnected()应返回false。
     */
    virtual void disconnect() = 0;

    /**
     * @brief 检查是否已连接到PLC
     * @return 已连接返回true，未连接返回false
     */
    virtual bool isConnected() const = 0;

    /**
     * @brief 读取字寄存器（16位）
     * @param address 起始寄存器地址
     * @param count 读取的寄存器数量
     * @return PLCResult结构体，包含读取状态和数据
     *
     * @details
     * 纯虚函数，由子类根据具体协议实现。
     *
     * ## 地址说明
     * - Modbus: 地址范围0-65535，对应保持寄存器(4xxxx)
     * - 三菱MC: D寄存器地址，如D0-D8191
     * - 基恩士: DM寄存器地址
     *
     * @note 返回的intValues数组长度等于count
     */
    virtual PLCResult readRegisters(int address, int count) = 0;

    /**
     * @brief 写入字寄存器（16位）
     * @param address 起始寄存器地址
     * @param values 要写入的值数组
     * @return PLCResult结构体，包含写入状态
     *
     * @details
     * 纯虚函数，由子类根据具体协议实现。
     *
     * ## 值范围
     * - 无符号：0-65535
     * - 有符号：-32768 至 32767
     */
    virtual PLCResult writeRegisters(int address, const std::vector<int>& values) = 0;

    /**
     * @brief 读取线圈/位（布尔值）
     * @param address 起始线圈地址
     * @param count 读取的线圈数量
     * @return PLCResult结构体，包含读取状态和数据
     *
     * @details
     * 纯虚函数，由子类根据具体协议实现。
     *
     * ## 地址说明
     * - Modbus: 线圈地址0-65535，对应输出线圈(0xxxx)
     * - 三菱MC: M继电器、X/Y端子等
     * - 基恩士: R继电器等
     *
     * @note 返回的boolValues数组长度等于count
     */
    virtual PLCResult readCoils(int address, int count) = 0;

    /**
     * @brief 写入线圈/位（布尔值）
     * @param address 起始线圈地址
     * @param values 要写入的布尔值数组
     * @return PLCResult结构体，包含写入状态
     */
    virtual PLCResult writeCoils(int address, const std::vector<bool>& values) = 0;

    /**
     * @brief 读取双字寄存器（32位整数）
     * @param address 起始寄存器地址
     * @param count 读取的双字数量
     * @return PLCResult结构体，包含读取状态和数据
     *
     * @details
     * 默认实现：读取2*count个字寄存器，然后组合成双字。
     * 数据格式：低字在前（地址N），高字在后（地址N+1）。
     *
     * ## 内存布局示例
     * 地址N:   低16位 (bits 0-15)
     * 地址N+1: 高16位 (bits 16-31)
     * 组合后: (高字 << 16) | 低字
     *
     * @note 子类可根据PLC的具体字节序重写此方法
     */
    virtual PLCResult readDWords(int address, int count);

    /**
     * @brief 写入双字寄存器（32位整数）
     * @param address 起始寄存器地址
     * @param values 要写入的32位整数数组
     * @return PLCResult结构体，包含写入状态
     *
     * @details
     * 默认实现：将32位值拆分为两个16位寄存器后写入。
     * 拆分规则：低16位写入地址N，高16位写入地址N+1。
     */
    virtual PLCResult writeDWords(int address, const std::vector<int>& values);

    /**
     * @brief 读取浮点数寄存器（IEEE 754单精度）
     * @param address 起始寄存器地址
     * @param count 读取的浮点数数量
     * @return PLCResult结构体，包含读取状态和数据
     *
     * @details
     * 默认实现：读取2*count个字寄存器，然后按IEEE 754格式转换为浮点数。
     *
     * ## IEEE 754单精度格式
     * - 符号位：1位
     * - 指数：8位
     * - 尾数：23位
     * - 总共：32位（2个字寄存器）
     *
     * @note 返回的doubleValues数组包含转换后的浮点数
     */
    virtual PLCResult readFloats(int address, int count);

    /**
     * @brief 写入浮点数寄存器
     * @param address 起始寄存器地址
     * @param values 要写入的浮点数数组
     * @return PLCResult结构体，包含写入状态
     *
     * @details
     * 默认实现：将double转换为float，再按IEEE 754格式拆分为两个字寄存器写入。
     *
     * @warning 由于精度转换，double值超出float范围可能导致精度损失
     */
    virtual PLCResult writeFloats(int address, const std::vector<double>& values);

    /**
     * @brief 获取当前使用的PLC协议类型
     * @return PLCProtocol枚举值
     */
    PLCProtocol getProtocol() const { return protocol_; }

    /**
     * @brief 获取最后一次操作的错误信息
     * @return 错误描述字符串，无错误时返回空字符串
     *
     * @details
     * 当connect()、readXXX()、writeXXX()等方法失败时，
     * 可调用此方法获取详细的错误描述。
     */
    QString getLastError() const { return lastError_; }

    /**
     * @brief 设置通信超时时间
     * @param ms 超时时间（毫秒）
     *
     * @details
     * 设置读写操作的超时时间，超过此时间未响应将返回超时错误。
     * 默认超时时间为3000毫秒。
     */
    void setTimeout(int ms) { timeout_ = ms; }

    /**
     * @brief 获取当前超时时间设置
     * @return 超时时间（毫秒）
     */
    int getTimeout() const { return timeout_; }

signals:
    /**
     * @brief 连接状态改变信号
     * @param connected true表示已连接，false表示已断开
     *
     * @details
     * 当PLC连接状态发生变化时发出此信号。
     * 可用于更新UI显示或触发其他业务逻辑。
     */
    void connectionChanged(bool connected);

    /**
     * @brief 通信错误信号
     * @param error 错误描述信息
     *
     * @details
     * 当通信过程中发生错误时发出此信号。
     * 典型错误包括：连接断开、超时、协议错误等。
     */
    void errorOccurred(const QString& error);

protected:
    PLCProtocol protocol_ = PLCProtocol::Unknown;  ///< 当前PLC协议类型
    PLCConfig config_;                              ///< PLC连接配置参数
    QString lastError_;                             ///< 最后一次错误信息
    int timeout_ = 3000;                            ///< 通信超时时间（毫秒）
    mutable QMutex mutex_;                          ///< 线程安全互斥锁
};

/**
 * @brief PLC通信实例工厂函数
 * @param protocol 要创建的PLC协议类型
 * @return 对应协议的PLCComm智能指针，不支持的协议返回nullptr
 *
 * @details
 * 根据指定的协议类型创建相应的PLC通信实例。
 *
 * ## 支持的协议类型
 * - PLCProtocol::ModbusTCP -> ModbusTCPComm
 * - PLCProtocol::MitsubishiMC_TCP -> MitsubishiMCComm
 * - PLCProtocol::MitsubishiMC_UDP -> MitsubishiMCComm
 * - PLCProtocol::KeyencePCLink -> KeyencePCLinkComm
 *
 * @code
 * // 使用示例
 * auto modbus = createPLCComm(PLCProtocol::ModbusTCP);
 * auto mitsubishi = createPLCComm(PLCProtocol::MitsubishiMC_TCP);
 * @endcode
 */
std::shared_ptr<PLCComm> createPLCComm(PLCProtocol protocol);

} // namespace Comm
} // namespace VisionForge
