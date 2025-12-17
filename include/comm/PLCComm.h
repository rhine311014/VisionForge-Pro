/**
 * @file PLCComm.h
 * @brief PLC通信基类
 * @author VisionForge Team
 * @date 2025-12-17
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
 * @brief PLC通信基类
 *
 * 提供PLC通信的基本接口，具体协议由子类实现
 */
class PLCComm : public QObject
{
    Q_OBJECT

public:
    explicit PLCComm(QObject* parent = nullptr);
    virtual ~PLCComm();

    /**
     * @brief 连接PLC
     * @param config PLC配置
     * @return 是否连接成功
     */
    virtual bool connect(const PLCConfig& config) = 0;

    /**
     * @brief 断开连接
     */
    virtual void disconnect() = 0;

    /**
     * @brief 是否已连接
     */
    virtual bool isConnected() const = 0;

    /**
     * @brief 读取寄存器 (字)
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    virtual PLCResult readRegisters(int address, int count) = 0;

    /**
     * @brief 写入寄存器 (字)
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    virtual PLCResult writeRegisters(int address, const std::vector<int>& values) = 0;

    /**
     * @brief 读取线圈/位
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    virtual PLCResult readCoils(int address, int count) = 0;

    /**
     * @brief 写入线圈/位
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    virtual PLCResult writeCoils(int address, const std::vector<bool>& values) = 0;

    /**
     * @brief 读取双字寄存器 (32位整数)
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    virtual PLCResult readDWords(int address, int count);

    /**
     * @brief 写入双字寄存器 (32位整数)
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    virtual PLCResult writeDWords(int address, const std::vector<int>& values);

    /**
     * @brief 读取浮点数寄存器
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    virtual PLCResult readFloats(int address, int count);

    /**
     * @brief 写入浮点数寄存器
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    virtual PLCResult writeFloats(int address, const std::vector<double>& values);

    /**
     * @brief 获取协议类型
     */
    PLCProtocol getProtocol() const { return protocol_; }

    /**
     * @brief 获取最后错误信息
     */
    QString getLastError() const { return lastError_; }

    /**
     * @brief 设置超时时间
     */
    void setTimeout(int ms) { timeout_ = ms; }

    /**
     * @brief 获取超时时间
     */
    int getTimeout() const { return timeout_; }

signals:
    /**
     * @brief 连接状态改变
     */
    void connectionChanged(bool connected);

    /**
     * @brief 通信错误
     */
    void errorOccurred(const QString& error);

protected:
    PLCProtocol protocol_ = PLCProtocol::Unknown;
    PLCConfig config_;
    QString lastError_;
    int timeout_ = 3000;
    mutable QMutex mutex_;
};

/**
 * @brief 创建PLC通信实例
 * @param protocol 协议类型
 * @return PLC通信实例
 */
std::shared_ptr<PLCComm> createPLCComm(PLCProtocol protocol);

} // namespace Comm
} // namespace VisionForge
