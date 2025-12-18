/**
 * @file ModbusRTUComm.h
 * @brief Modbus RTU串口通信
 * @details 实现标准Modbus RTU协议，支持通过串口与PLC/设备通信
 * @author VisionForge Team
 * @date 2025-12-18
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
 * 支持的功能码:
 * - 0x01: 读线圈
 * - 0x02: 读离散输入
 * - 0x03: 读保持寄存器
 * - 0x04: 读输入寄存器
 * - 0x05: 写单个线圈
 * - 0x06: 写单个寄存器
 * - 0x0F: 写多个线圈
 * - 0x10: 写多个寄存器
 */
class ModbusRTUComm : public PLCComm
{
    Q_OBJECT

public:
    /**
     * @brief Modbus功能码
     */
    enum FunctionCode {
        ReadCoils = 0x01,               // 读线圈
        ReadDiscreteInputs = 0x02,      // 读离散输入
        ReadHoldingRegisters = 0x03,    // 读保持寄存器
        ReadInputRegisters = 0x04,      // 读输入寄存器
        WriteSingleCoil = 0x05,         // 写单个线圈
        WriteSingleRegister = 0x06,     // 写单个寄存器
        WriteMultipleCoils = 0x0F,      // 写多个线圈
        WriteMultipleRegisters = 0x10   // 写多个寄存器
    };

    explicit ModbusRTUComm(QObject* parent = nullptr);
    ~ModbusRTUComm() override;

    // ========== PLCComm接口实现 ==========

    bool connect(const PLCConfig& config) override;
    void disconnect() override;
    bool isConnected() const override;

    PLCResult readRegisters(int address, int count) override;
    PLCResult writeRegisters(int address, const std::vector<int>& values) override;
    PLCResult readCoils(int address, int count) override;
    PLCResult writeCoils(int address, const std::vector<bool>& values) override;

    // ========== Modbus RTU特有接口 ==========

    /**
     * @brief 读保持寄存器 (功能码03)
     * @param slaveId 从站地址
     * @param address 起始地址
     * @param count 数量
     * @return 结果
     */
    PLCResult readHoldingRegisters(int slaveId, int address, int count);

    /**
     * @brief 读输入寄存器 (功能码04)
     * @param slaveId 从站地址
     * @param address 起始地址
     * @param count 数量
     * @return 结果
     */
    PLCResult readInputRegisters(int slaveId, int address, int count);

    /**
     * @brief 写单个寄存器 (功能码06)
     * @param slaveId 从站地址
     * @param address 地址
     * @param value 值
     * @return 结果
     */
    PLCResult writeSingleRegister(int slaveId, int address, int value);

    /**
     * @brief 写多个寄存器 (功能码10)
     * @param slaveId 从站地址
     * @param address 起始地址
     * @param values 值数组
     * @return 结果
     */
    PLCResult writeMultipleRegisters(int slaveId, int address, const std::vector<int>& values);

    /**
     * @brief 读离散输入 (功能码02)
     * @param slaveId 从站地址
     * @param address 起始地址
     * @param count 数量
     * @return 结果
     */
    PLCResult readDiscreteInputs(int slaveId, int address, int count);

    /**
     * @brief 写单个线圈 (功能码05)
     * @param slaveId 从站地址
     * @param address 地址
     * @param value 值
     * @return 结果
     */
    PLCResult writeSingleCoil(int slaveId, int address, bool value);

    /**
     * @brief 写多个线圈 (功能码0F)
     * @param slaveId 从站地址
     * @param address 起始地址
     * @param values 值数组
     * @return 结果
     */
    PLCResult writeMultipleCoils(int slaveId, int address, const std::vector<bool>& values);

    // ========== 串口配置 ==========

    /**
     * @brief 设置从站地址
     */
    void setSlaveId(int id) { slaveId_ = id; }

    /**
     * @brief 获取从站地址
     */
    int slaveId() const { return slaveId_; }

    /**
     * @brief 获取串口名列表
     */
    static QStringList availablePorts();

private:
    /**
     * @brief 发送并接收数据
     */
    QByteArray sendAndReceive(const QByteArray& request, int expectedLength);

    /**
     * @brief 构建请求帧
     */
    QByteArray buildRequest(int slaveId, FunctionCode func, int address, int countOrValue);

    /**
     * @brief 构建写多个寄存器请求
     */
    QByteArray buildWriteMultipleRegistersRequest(int slaveId, int address, const std::vector<int>& values);

    /**
     * @brief 构建写多个线圈请求
     */
    QByteArray buildWriteMultipleCoilsRequest(int slaveId, int address, const std::vector<bool>& values);

    /**
     * @brief 解析响应帧
     */
    PLCResult parseResponse(const QByteArray& response, FunctionCode expectedFunc);

    /**
     * @brief 计算CRC16
     */
    static quint16 calculateCRC16(const QByteArray& data);

    /**
     * @brief 验证CRC
     */
    static bool verifyCRC(const QByteArray& data);

private:
    QSerialPort* serialPort_;
    int slaveId_;
};

} // namespace Comm
} // namespace VisionForge
