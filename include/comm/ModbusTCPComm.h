/**
 * @file ModbusTCPComm.h
 * @brief Modbus TCP通信实现
 * @author VisionForge Team
 * @date 2025-12-17
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
 * 实现Modbus TCP协议，支持读写保持寄存器和线圈
 */
class ModbusTCPComm : public PLCComm
{
    Q_OBJECT

public:
    explicit ModbusTCPComm(QObject* parent = nullptr);
    ~ModbusTCPComm() override;

    // 实现基类接口
    bool connect(const PLCConfig& config) override;
    void disconnect() override;
    bool isConnected() const override;

    PLCResult readRegisters(int address, int count) override;
    PLCResult writeRegisters(int address, const std::vector<int>& values) override;
    PLCResult readCoils(int address, int count) override;
    PLCResult writeCoils(int address, const std::vector<bool>& values) override;

    /**
     * @brief 读取输入寄存器 (功能码04)
     */
    PLCResult readInputRegisters(int address, int count);

    /**
     * @brief 读取离散输入 (功能码02)
     */
    PLCResult readDiscreteInputs(int address, int count);

private:
    /**
     * @brief 发送Modbus请求并接收响应
     * @param request 请求数据
     * @return 响应数据
     */
    QByteArray sendRequest(const QByteArray& request);

    /**
     * @brief 构建Modbus TCP帧
     * @param functionCode 功能码
     * @param data PDU数据
     * @return 完整的Modbus TCP帧
     */
    QByteArray buildFrame(quint8 functionCode, const QByteArray& data);

    /**
     * @brief 解析响应帧
     * @param response 响应数据
     * @param expectedFunctionCode 期望的功能码
     * @return 解析结果
     */
    PLCResult parseResponse(const QByteArray& response, quint8 expectedFunctionCode);

    /**
     * @brief 检查Modbus异常响应
     * @param functionCode 功能码
     * @param exceptionCode 异常码
     * @return 错误信息
     */
    QString getExceptionMessage(quint8 exceptionCode);

private:
    QTcpSocket* socket_;
    quint16 transactionId_;
    quint8 slaveId_;
};

} // namespace Comm
} // namespace VisionForge
