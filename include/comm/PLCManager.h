/**
 * @file PLCManager.h
 * @brief PLC通信管理器
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "comm/PLCComm.h"
#include <QObject>
#include <QMap>
#include <QSettings>
#include <memory>

namespace VisionForge {
namespace Comm {

/**
 * @class PLCManager
 * @brief PLC通信管理器
 *
 * 单例模式，管理所有PLC连接
 */
class PLCManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static PLCManager& instance();

    /**
     * @brief 添加PLC连接
     * @param name 连接名称
     * @param config PLC配置
     * @return 是否添加成功
     */
    bool addConnection(const QString& name, const PLCConfig& config);

    /**
     * @brief 移除PLC连接
     * @param name 连接名称
     * @return 是否移除成功
     */
    bool removeConnection(const QString& name);

    /**
     * @brief 获取PLC连接
     * @param name 连接名称
     * @return PLC通信对象
     */
    std::shared_ptr<PLCComm> getConnection(const QString& name);

    /**
     * @brief 获取所有连接名称
     * @return 连接名称列表
     */
    QStringList getConnectionNames() const;

    /**
     * @brief 获取连接配置
     * @param name 连接名称
     * @return PLC配置
     */
    PLCConfig getConfig(const QString& name) const;

    /**
     * @brief 连接指定PLC
     * @param name 连接名称
     * @return 是否连接成功
     */
    bool connectPLC(const QString& name);

    /**
     * @brief 断开指定PLC
     * @param name 连接名称
     */
    void disconnectPLC(const QString& name);

    /**
     * @brief 连接所有PLC
     */
    void connectAll();

    /**
     * @brief 断开所有PLC
     */
    void disconnectAll();

    /**
     * @brief 检查指定PLC是否已连接
     * @param name 连接名称
     * @return 是否已连接
     */
    bool isConnected(const QString& name) const;

    /**
     * @brief 读取寄存器
     * @param connectionName 连接名称
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readRegisters(const QString& connectionName, int address, int count);

    /**
     * @brief 写入寄存器
     * @param connectionName 连接名称
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    PLCResult writeRegisters(const QString& connectionName, int address, const std::vector<int>& values);

    /**
     * @brief 读取线圈
     * @param connectionName 连接名称
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readCoils(const QString& connectionName, int address, int count);

    /**
     * @brief 写入线圈
     * @param connectionName 连接名称
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    PLCResult writeCoils(const QString& connectionName, int address, const std::vector<bool>& values);

    /**
     * @brief 保存配置到文件
     * @param filePath 文件路径
     * @return 是否保存成功
     */
    bool saveConfig(const QString& filePath);

    /**
     * @brief 从文件加载配置
     * @param filePath 文件路径
     * @return 是否加载成功
     */
    bool loadConfig(const QString& filePath);

signals:
    /**
     * @brief 连接状态改变
     * @param name 连接名称
     * @param connected 是否已连接
     */
    void connectionStatusChanged(const QString& name, bool connected);

    /**
     * @brief 通信错误
     * @param name 连接名称
     * @param error 错误信息
     */
    void errorOccurred(const QString& name, const QString& error);

    /**
     * @brief 连接列表改变
     */
    void connectionsChanged();

private:
    PLCManager(QObject* parent = nullptr);
    ~PLCManager();
    PLCManager(const PLCManager&) = delete;
    PLCManager& operator=(const PLCManager&) = delete;

    /**
     * @brief 序列化配置
     */
    QVariantMap serializeConfig(const PLCConfig& config) const;

    /**
     * @brief 反序列化配置
     */
    PLCConfig deserializeConfig(const QVariantMap& map) const;

private:
    QMap<QString, std::shared_ptr<PLCComm>> connections_;
    QMap<QString, PLCConfig> configs_;
    mutable QMutex mutex_;
};

} // namespace Comm
} // namespace VisionForge
