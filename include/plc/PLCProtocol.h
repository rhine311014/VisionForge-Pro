/**
 * @file PLCProtocol.h
 * @brief PLC协议抽象基类和协议实现
 * @author VisionForge Team
 * @date 2025-12-21
 *
 * 本文件定义了PLC协议的抽象接口和具体实现：
 * - PLCProtocol: 协议抽象基类
 * - ModbusTCPProtocol: Modbus TCP协议实现
 * - MitsubishiMCProtocol: 三菱MC 3E帧协议实现
 */

#pragma once

#include "plc/PLCTypes.h"
#include <QObject>
#include <QByteArray>
#include <memory>

namespace VisionForge {
namespace PLC {

// ============================================================
// 协议抽象基类
// ============================================================

/**
 * @class PLCProtocol
 * @brief PLC协议抽象基类
 *
 * 定义了协议打包和解析的通用接口，
 * 所有具体协议实现都需要继承此类。
 */
class PLCProtocol : public QObject
{
    Q_OBJECT

public:
    explicit PLCProtocol(QObject* parent = nullptr);
    virtual ~PLCProtocol();

    /**
     * @brief 获取协议类型
     */
    virtual ProtocolType protocolType() const = 0;

    /**
     * @brief 获取协议名称
     */
    virtual QString protocolName() const = 0;

    /**
     * @brief 设置配置
     */
    virtual void setConfig(const PLCConfig& config);

    /**
     * @brief 获取配置
     */
    const PLCConfig& config() const { return config_; }

    // ============================================================
    // 请求打包接口
    // ============================================================

    /**
     * @brief 打包读取请求
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param count 读取数量
     * @return 打包后的请求数据
     */
    virtual QByteArray packReadRequest(RegisterType regType, int address, int count) = 0;

    /**
     * @brief 打包写入请求 (16位值)
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param values 写入值列表
     * @return 打包后的请求数据
     */
    virtual QByteArray packWriteRequest(RegisterType regType, int address,
                                        const std::vector<uint16_t>& values) = 0;

    /**
     * @brief 打包位写入请求
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param values 位值列表
     * @return 打包后的请求数据
     */
    virtual QByteArray packWriteBitRequest(RegisterType regType, int address,
                                           const std::vector<bool>& values) = 0;

    // ============================================================
    // 响应解析接口
    // ============================================================

    /**
     * @brief 解析响应数据
     * @param response 原始响应数据
     * @param expectedType 期望的寄存器类型
     * @return 解析结果
     */
    virtual PLCResult parseResponse(const QByteArray& response,
                                    RegisterType expectedType = RegisterType::Unknown) = 0;

    /**
     * @brief 检查响应是否完整
     * @param data 接收到的数据
     * @return 完整返回true
     */
    virtual bool isResponseComplete(const QByteArray& data) const = 0;

    /**
     * @brief 获取期望的响应长度
     * @param request 请求数据
     * @return 期望的响应长度，-1表示无法确定
     */
    virtual int getExpectedResponseLength(const QByteArray& request) const = 0;

    // ============================================================
    // 辅助方法
    // ============================================================

    /**
     * @brief 将uint16数组转换为int32数组
     */
    static std::vector<int32_t> toInt32(const std::vector<uint16_t>& values, ByteOrder order);

    /**
     * @brief 将uint16数组转换为float数组
     */
    static std::vector<float> toFloat(const std::vector<uint16_t>& values, ByteOrder order);

    /**
     * @brief 将int32数组转换为uint16数组
     */
    static std::vector<uint16_t> fromInt32(const std::vector<int32_t>& values, ByteOrder order);

    /**
     * @brief 将float数组转换为uint16数组
     */
    static std::vector<uint16_t> fromFloat(const std::vector<float>& values, ByteOrder order);

protected:
    PLCConfig config_;
};

// ============================================================
// Modbus TCP协议实现
// ============================================================

/**
 * @class ModbusTCPProtocol
 * @brief Modbus TCP协议实现
 *
 * 实现Modbus TCP协议的打包和解析：
 * - MBAP头 (7字节): 事务标识符(2) + 协议标识符(2) + 长度(2) + 单元标识符(1)
 * - PDU: 功能码(1) + 数据(N)
 *
 * 支持的功能码：
 * - 01: 读线圈
 * - 02: 读离散输入
 * - 03: 读保持寄存器
 * - 04: 读输入寄存器
 * - 05: 写单个线圈
 * - 06: 写单个寄存器
 * - 0F: 写多个线圈
 * - 10: 写多个寄存器
 */
class ModbusTCPProtocol : public PLCProtocol
{
    Q_OBJECT

public:
    explicit ModbusTCPProtocol(QObject* parent = nullptr);
    ~ModbusTCPProtocol() override;

    // PLCProtocol接口实现
    ProtocolType protocolType() const override { return ProtocolType::ModbusTCP; }
    QString protocolName() const override { return QStringLiteral("Modbus TCP"); }

    QByteArray packReadRequest(RegisterType regType, int address, int count) override;
    QByteArray packWriteRequest(RegisterType regType, int address,
                                const std::vector<uint16_t>& values) override;
    QByteArray packWriteBitRequest(RegisterType regType, int address,
                                   const std::vector<bool>& values) override;
    PLCResult parseResponse(const QByteArray& response,
                            RegisterType expectedType = RegisterType::Unknown) override;
    bool isResponseComplete(const QByteArray& data) const override;
    int getExpectedResponseLength(const QByteArray& request) const override;

    // ============================================================
    // Modbus特有方法
    // ============================================================

    /**
     * @brief 打包功能码03读保持寄存器请求
     * @param address 起始地址
     * @param count 寄存器数量
     * @return 请求数据
     */
    QByteArray packReadHoldingRegisters(int address, int count);

    /**
     * @brief 打包功能码06写单个寄存器请求
     * @param address 地址
     * @param value 值
     * @return 请求数据
     */
    QByteArray packWriteSingleRegister(int address, uint16_t value);

    /**
     * @brief 打包功能码16(0x10)写多个寄存器请求
     * @param address 起始地址
     * @param values 值列表
     * @return 请求数据
     */
    QByteArray packWriteMultipleRegisters(int address, const std::vector<uint16_t>& values);

    /**
     * @brief 打包功能码01读线圈请求
     * @param address 起始地址
     * @param count 线圈数量
     * @return 请求数据
     */
    QByteArray packReadCoils(int address, int count);

    /**
     * @brief 打包功能码05写单个线圈请求
     * @param address 地址
     * @param value 值
     * @return 请求数据
     */
    QByteArray packWriteSingleCoil(int address, bool value);

    /**
     * @brief 打包功能码15(0x0F)写多个线圈请求
     * @param address 起始地址
     * @param values 值列表
     * @return 请求数据
     */
    QByteArray packWriteMultipleCoils(int address, const std::vector<bool>& values);

    /**
     * @brief 设置事务ID
     */
    void setTransactionId(quint16 id) { transactionId_ = id; }

    /**
     * @brief 获取当前事务ID
     */
    quint16 transactionId() const { return transactionId_; }

    /**
     * @brief 递增并获取下一个事务ID
     */
    quint16 nextTransactionId() { return ++transactionId_; }

private:
    /**
     * @brief 构建MBAP头
     * @param pduLength PDU长度
     * @return MBAP头数据
     */
    QByteArray buildMBAPHeader(int pduLength);

    /**
     * @brief 解析MBAP头
     * @param data 数据
     * @param transactionId 输出事务ID
     * @param length 输出长度
     * @param unitId 输出单元ID
     * @return 成功返回true
     */
    bool parseMBAPHeader(const QByteArray& data, quint16& transactionId,
                         quint16& length, quint8& unitId) const;

    /**
     * @brief 获取Modbus异常错误信息
     */
    QString getModbusExceptionMessage(quint8 exceptionCode) const;

    /**
     * @brief 获取功能码对应的寄存器类型
     */
    static quint8 getFunctionCodeForRead(RegisterType regType);
    static quint8 getFunctionCodeForWrite(RegisterType regType, bool single);

private:
    quint16 transactionId_ = 0;     ///< 事务标识符
    static constexpr quint16 PROTOCOL_ID = 0x0000;  ///< Modbus协议标识符
};

// ============================================================
// 三菱MC 3E帧协议实现
// ============================================================

/**
 * @class MitsubishiMCProtocol
 * @brief 三菱MC协议3E帧实现
 *
 * 3E帧二进制格式：
 * 请求帧：
 * - 子头 (2字节): 0x50 0x00 (请求)
 * - 网络号 (1字节)
 * - PC号 (1字节)
 * - 请求目标模块IO编号 (2字节)
 * - 请求目标模块站号 (1字节)
 * - 请求数据长度 (2字节)
 * - 监视定时器 (2字节)
 * - 请求内容：命令(2) + 子命令(2) + 起始地址(3) + 软元件代码(1) + 点数(2) [+ 数据]
 *
 * 响应帧：
 * - 子头 (2字节): 0xD0 0x00 (响应)
 * - 网络号 (1字节)
 * - PC号 (1字节)
 * - 请求目标模块IO编号 (2字节)
 * - 请求目标模块站号 (1字节)
 * - 响应数据长度 (2字节)
 * - 完成代码 (2字节): 0x0000表示成功
 * - 响应数据 (N字节)
 */
class MitsubishiMCProtocol : public PLCProtocol
{
    Q_OBJECT

public:
    explicit MitsubishiMCProtocol(QObject* parent = nullptr);
    ~MitsubishiMCProtocol() override;

    // PLCProtocol接口实现
    ProtocolType protocolType() const override { return ProtocolType::MitsubishiMC3E; }
    QString protocolName() const override { return QStringLiteral("Mitsubishi MC 3E Binary"); }

    QByteArray packReadRequest(RegisterType regType, int address, int count) override;
    QByteArray packWriteRequest(RegisterType regType, int address,
                                const std::vector<uint16_t>& values) override;
    QByteArray packWriteBitRequest(RegisterType regType, int address,
                                   const std::vector<bool>& values) override;
    PLCResult parseResponse(const QByteArray& response,
                            RegisterType expectedType = RegisterType::Unknown) override;
    bool isResponseComplete(const QByteArray& data) const override;
    int getExpectedResponseLength(const QByteArray& request) const override;

    // ============================================================
    // MC协议特有方法
    // ============================================================

    /**
     * @brief 批量读取字软元件
     * @param deviceType 软元件类型 (D, W, R等)
     * @param address 起始地址
     * @param count 读取点数
     * @return 请求数据
     */
    QByteArray packBatchReadWord(RegisterType deviceType, int address, int count);

    /**
     * @brief 批量读取位软元件
     * @param deviceType 软元件类型 (M, X, Y, B等)
     * @param address 起始地址
     * @param count 读取点数
     * @return 请求数据
     */
    QByteArray packBatchReadBit(RegisterType deviceType, int address, int count);

    /**
     * @brief 批量写入字软元件
     * @param deviceType 软元件类型
     * @param address 起始地址
     * @param values 写入值列表
     * @return 请求数据
     */
    QByteArray packBatchWriteWord(RegisterType deviceType, int address,
                                  const std::vector<uint16_t>& values);

    /**
     * @brief 批量写入位软元件
     * @param deviceType 软元件类型
     * @param address 起始地址
     * @param values 位值列表
     * @return 请求数据
     */
    QByteArray packBatchWriteBit(RegisterType deviceType, int address,
                                 const std::vector<bool>& values);

    /**
     * @brief 设置MC协议参数
     */
    void setNetworkNo(quint8 no) { networkNo_ = no; }
    void setPCNo(quint8 no) { pcNo_ = no; }
    void setDestModuleIO(quint16 io) { destModuleIO_ = io; }
    void setDestModuleStation(quint8 station) { destModuleStation_ = station; }
    void setMonitoringTimer(quint16 timer) { monitoringTimer_ = timer; }

private:
    /**
     * @brief 构建3E帧头
     * @param dataLength 请求数据长度
     * @return 帧头数据
     */
    QByteArray build3EHeader(int dataLength);

    /**
     * @brief 构建软元件地址数据
     * @param deviceType 软元件类型
     * @param address 地址
     * @return 地址数据 (3字节地址 + 1字节软元件代码)
     */
    QByteArray buildDeviceAddress(RegisterType deviceType, int address);

    /**
     * @brief 获取软元件代码
     */
    quint8 getDeviceCode(RegisterType deviceType) const;

    /**
     * @brief 判断是否为位软元件
     */
    bool isBitDevice(RegisterType deviceType) const;

    /**
     * @brief 解析完成代码错误信息
     */
    QString getCompletionCodeMessage(quint16 code) const;

private:
    // MC协议参数
    quint8 networkNo_ = 0;              ///< 网络号
    quint8 pcNo_ = 0xFF;                ///< PC号
    quint16 destModuleIO_ = 0x03FF;     ///< 目标模块IO号
    quint8 destModuleStation_ = 0;      ///< 目标模块站号
    quint16 monitoringTimer_ = 10;      ///< 监视定时器 (250ms单位，默认2.5秒)

    // 3E帧常量
    static constexpr quint8 REQUEST_SUBHEADER_1 = 0x50;     ///< 请求子头1
    static constexpr quint8 REQUEST_SUBHEADER_2 = 0x00;     ///< 请求子头2
    static constexpr quint8 RESPONSE_SUBHEADER_1 = 0xD0;    ///< 响应子头1
    static constexpr quint8 RESPONSE_SUBHEADER_2 = 0x00;    ///< 响应子头2
    static constexpr int HEADER_LENGTH = 9;                  ///< 子头到请求数据长度的长度
    static constexpr int MIN_RESPONSE_LENGTH = 11;           ///< 最小响应长度
};

// ============================================================
// 协议工厂
// ============================================================

/**
 * @class PLCProtocolFactory
 * @brief 协议工厂类
 */
class PLCProtocolFactory
{
public:
    /**
     * @brief 创建协议实例
     * @param type 协议类型
     * @param parent 父对象
     * @return 协议实例指针
     */
    static std::unique_ptr<PLCProtocol> create(ProtocolType type, QObject* parent = nullptr);

    /**
     * @brief 获取支持的协议类型列表
     */
    static std::vector<ProtocolType> supportedProtocols();

    /**
     * @brief 判断协议是否支持
     */
    static bool isSupported(ProtocolType type);
};

} // namespace PLC
} // namespace VisionForge
