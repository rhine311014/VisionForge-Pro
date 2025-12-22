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
// Modbus RTU协议实现
// ============================================================

/**
 * @class ModbusRTUProtocol
 * @brief Modbus RTU协议实现
 *
 * RTU帧格式：
 * - 从站地址 (1字节)
 * - 功能码 (1字节)
 * - 数据 (N字节)
 * - CRC校验 (2字节, 低字节在前)
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
class ModbusRTUProtocol : public PLCProtocol
{
    Q_OBJECT

public:
    explicit ModbusRTUProtocol(QObject* parent = nullptr);
    ~ModbusRTUProtocol() override;

    // PLCProtocol接口实现
    ProtocolType protocolType() const override { return ProtocolType::ModbusRTU; }
    QString protocolName() const override { return QStringLiteral("Modbus RTU"); }

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
    // Modbus RTU特有方法
    // ============================================================

    /**
     * @brief 打包功能码03读保持寄存器请求
     */
    QByteArray packReadHoldingRegisters(int address, int count);

    /**
     * @brief 打包功能码06写单个寄存器请求
     */
    QByteArray packWriteSingleRegister(int address, uint16_t value);

    /**
     * @brief 打包功能码16(0x10)写多个寄存器请求
     */
    QByteArray packWriteMultipleRegisters(int address, const std::vector<uint16_t>& values);

    /**
     * @brief 打包功能码01读线圈请求
     */
    QByteArray packReadCoils(int address, int count);

    /**
     * @brief 打包功能码05写单个线圈请求
     */
    QByteArray packWriteSingleCoil(int address, bool value);

    /**
     * @brief 打包功能码15(0x0F)写多个线圈请求
     */
    QByteArray packWriteMultipleCoils(int address, const std::vector<bool>& values);

    // ============================================================
    // CRC计算
    // ============================================================

    /**
     * @brief 计算CRC16校验值(Modbus)
     * @param data 数据
     * @return CRC16值
     */
    static quint16 calculateCRC16(const QByteArray& data);

    /**
     * @brief 验证CRC校验
     * @param frame 完整帧(包含CRC)
     * @return 校验通过返回true
     */
    static bool verifyCRC16(const QByteArray& frame);

private:
    /**
     * @brief 添加CRC到帧
     * @param pdu PDU数据(不含CRC)
     * @return 完整RTU帧(含从站地址和CRC)
     */
    QByteArray buildRTUFrame(const QByteArray& pdu);

    /**
     * @brief 获取功能码对应的寄存器类型
     */
    static quint8 getFunctionCodeForRead(RegisterType regType);
    static quint8 getFunctionCodeForWrite(RegisterType regType, bool single);

    /**
     * @brief 获取Modbus异常错误信息
     */
    QString getModbusExceptionMessage(quint8 exceptionCode) const;

    /**
     * @brief 保存最后一次请求的功能码
     */
    quint8 lastFunctionCode_ = 0;

    /**
     * @brief 保存最后一次请求的寄存器数量
     */
    int lastRequestCount_ = 0;
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
// 欧姆龙FINS TCP协议实现
// ============================================================

/**
 * @class OmronFINSProtocol
 * @brief 欧姆龙FINS TCP协议实现
 *
 * FINS (Factory Interface Network Service) 是欧姆龙PLC的通信协议。
 *
 * FINS/TCP帧格式：
 * - FINS TCP头 (16字节):
 *   * 魔术字 "FINS" (4字节)
 *   * 长度 (4字节): 后续数据长度
 *   * 命令 (4字节): 0=握手请求, 1=握手响应, 2=FINS帧
 *   * 错误码 (4字节)
 *
 * - FINS帧头 (10字节):
 *   * ICF (1字节): 信息控制字段 (0x80=命令)
 *   * RSV (1字节): 保留 (0x00)
 *   * GCT (1字节): 网关计数 (0x02)
 *   * DNA (1字节): 目标网络地址
 *   * DA1 (1字节): 目标节点地址
 *   * DA2 (1字节): 目标单元地址
 *   * SNA (1字节): 源网络地址
 *   * SA1 (1字节): 源节点地址
 *   * SA2 (1字节): 源单元地址
 *   * SID (1字节): 服务ID
 *
 * - FINS命令数据:
 *   * 命令码 (2字节): 0x0101=读, 0x0102=写
 *   * 内存区域码 (1字节)
 *   * 起始地址 (3字节)
 *   * 数量 (2字节)
 *   * 数据 (写入时)
 */
class OmronFINSProtocol : public PLCProtocol
{
    Q_OBJECT

public:
    explicit OmronFINSProtocol(QObject* parent = nullptr);
    ~OmronFINSProtocol() override;

    // PLCProtocol接口实现
    ProtocolType protocolType() const override { return ProtocolType::OmronFINS_TCP; }
    QString protocolName() const override { return QStringLiteral("Omron FINS TCP"); }

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
    // FINS特有方法
    // ============================================================

    /**
     * @brief 打包FINS握手请求
     * @return 握手请求数据
     */
    QByteArray packHandshakeRequest();

    /**
     * @brief 解析握手响应
     * @param response 响应数据
     * @return 成功返回true
     */
    bool parseHandshakeResponse(const QByteArray& response);

    /**
     * @brief 打包内存区域读取请求
     * @param areaCode 内存区域代码
     * @param address 起始地址
     * @param count 读取数量
     * @return 请求数据
     */
    QByteArray packMemoryAreaRead(quint8 areaCode, int address, int count);

    /**
     * @brief 打包内存区域写入请求
     * @param areaCode 内存区域代码
     * @param address 起始地址
     * @param values 写入值
     * @return 请求数据
     */
    QByteArray packMemoryAreaWrite(quint8 areaCode, int address,
                                   const std::vector<uint16_t>& values);

    /**
     * @brief 设置节点地址参数
     */
    void setSourceNode(quint8 node) { sourceNode_ = node; }
    void setDestNode(quint8 node) { destNode_ = node; }
    void setSourceUnit(quint8 unit) { sourceUnit_ = unit; }
    void setDestUnit(quint8 unit) { destUnit_ = unit; }
    void setSourceNetwork(quint8 network) { sourceNetwork_ = network; }
    void setDestNetwork(quint8 network) { destNetwork_ = network; }

    /**
     * @brief 获取节点地址参数
     */
    quint8 sourceNode() const { return sourceNode_; }
    quint8 destNode() const { return destNode_; }

    /**
     * @brief 检查是否已完成握手
     */
    bool isHandshakeComplete() const { return handshakeComplete_; }

    /**
     * @brief 获取内存区域代码
     * @param regType 寄存器类型
     * @return 内存区域代码
     */
    static quint8 getMemoryAreaCode(RegisterType regType);

    /**
     * @brief 判断是否为位区域
     */
    static bool isBitArea(RegisterType regType);

    /**
     * @brief 获取FINS错误信息
     */
    static QString getFINSErrorMessage(quint8 mainCode, quint8 subCode);

    // ============================================================
    // FINS命令码定义
    // ============================================================
    static constexpr quint16 CMD_MEMORY_AREA_READ = 0x0101;     ///< 内存区域读取
    static constexpr quint16 CMD_MEMORY_AREA_WRITE = 0x0102;    ///< 内存区域写入
    static constexpr quint16 CMD_MEMORY_AREA_FILL = 0x0103;     ///< 内存区域填充
    static constexpr quint16 CMD_MULTIPLE_AREA_READ = 0x0104;   ///< 多区域读取
    static constexpr quint16 CMD_MEMORY_AREA_TRANSFER = 0x0105; ///< 内存区域传输

    // ============================================================
    // FINS内存区域代码
    // ============================================================
    static constexpr quint8 AREA_CIO_BIT = 0x30;    ///< CIO位区域
    static constexpr quint8 AREA_CIO_WORD = 0xB0;   ///< CIO字区域
    static constexpr quint8 AREA_WR_BIT = 0x31;     ///< WR位区域
    static constexpr quint8 AREA_WR_WORD = 0xB1;    ///< WR字区域
    static constexpr quint8 AREA_HR_BIT = 0x32;     ///< HR位区域
    static constexpr quint8 AREA_HR_WORD = 0xB2;    ///< HR字区域
    static constexpr quint8 AREA_AR_BIT = 0x33;     ///< AR位区域
    static constexpr quint8 AREA_AR_WORD = 0xB3;    ///< AR字区域
    static constexpr quint8 AREA_DM_BIT = 0x02;     ///< DM位区域
    static constexpr quint8 AREA_DM_WORD = 0x82;    ///< DM字区域
    static constexpr quint8 AREA_EM_WORD = 0x98;    ///< EM字区域 (扩展存储)

private:
    /**
     * @brief 构建FINS/TCP帧头
     * @param finsDataLength FINS数据长度
     * @param command TCP命令 (0=握手请求, 2=FINS帧)
     * @return TCP头数据
     */
    QByteArray buildFINSTCPHeader(int finsDataLength, quint32 command = 2);

    /**
     * @brief 构建FINS帧头
     * @return FINS帧头数据
     */
    QByteArray buildFINSHeader();

    /**
     * @brief 解析FINS响应
     * @param response 完整响应数据
     * @param result 输出结果
     * @return 成功返回true
     */
    bool parseFINSResponse(const QByteArray& response, PLCResult& result);

    /**
     * @brief 获取下一个服务ID
     */
    quint8 nextSID() { return ++serviceId_; }

private:
    // 节点地址配置
    quint8 sourceNetwork_ = 0;      ///< 源网络地址
    quint8 sourceNode_ = 0;         ///< 源节点地址
    quint8 sourceUnit_ = 0;         ///< 源单元地址
    quint8 destNetwork_ = 0;        ///< 目标网络地址
    quint8 destNode_ = 0;           ///< 目标节点地址
    quint8 destUnit_ = 0;           ///< 目标单元地址

    // 服务ID
    quint8 serviceId_ = 0;          ///< 服务ID计数器

    // 握手状态
    bool handshakeComplete_ = false; ///< 握手完成标志

    // 最后请求信息
    quint16 lastCommand_ = 0;       ///< 最后命令码
    int lastRequestCount_ = 0;      ///< 最后请求数量

    // FINS/TCP常量
    static constexpr quint32 FINS_TCP_HEADER_SIZE = 16;     ///< TCP头大小
    static constexpr quint32 FINS_FRAME_HEADER_SIZE = 10;   ///< FINS帧头大小
    static constexpr quint8 ICF_COMMAND = 0x80;             ///< ICF命令标志
    static constexpr quint8 ICF_RESPONSE = 0xC0;            ///< ICF响应标志
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
