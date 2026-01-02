/**
 * @file PLCManager.h
 * @brief PLC通信管理器头文件
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @details
 * 本文件定义了PLC通信管理器类，采用单例模式统一管理所有PLC连接。
 *
 * ## 功能概述
 * - 集中管理多个PLC连接
 * - 提供连接/断开/读写的统一接口
 * - 支持配置的持久化存储和加载
 * - 自动处理连接状态和错误通知
 *
 * ## 设计模式
 * - 单例模式：确保全局只有一个PLC管理器实例
 * - 外观模式：为复杂的PLC通信提供简化接口
 *
 * ## 线程安全
 * 所有公共方法都是线程安全的，内部使用互斥锁保护共享资源。
 *
 * ## 配置持久化
 * 支持将PLC配置保存为JSON格式文件，并可从文件恢复配置。
 *
 * @see PLCComm PLC通信基类
 * @see PLCConfig PLC配置结构体
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
 * @brief PLC通信管理器（单例模式）
 *
 * @details
 * PLCManager是整个系统中PLC通信的核心管理类，负责：
 * 1. 创建和管理多个PLC连接实例
 * 2. 提供统一的读写接口
 * 3. 监控连接状态并发出信号通知
 * 4. 配置的序列化和反序列化
 *
 * ## 使用示例
 * @code
 * // 获取管理器实例
 * PLCManager& mgr = PLCManager::instance();
 *
 * // 添加PLC连接
 * PLCConfig config;
 * config.protocol = PLCProtocol::ModbusTCP;
 * auto socketConfig = std::make_shared<SocketConfig>();
 * socketConfig->ipAddress = "192.168.1.100";
 * socketConfig->port = 502;
 * config.commConfig = socketConfig;
 *
 * mgr.addConnection("主PLC", config);
 *
 * // 连接PLC
 * if (mgr.connectPLC("主PLC")) {
 *     // 读取寄存器
 *     PLCResult result = mgr.readRegisters("主PLC", 0, 10);
 *     if (result.isSuccess()) {
 *         // 处理数据...
 *     }
 * }
 *
 * // 保存配置
 * mgr.saveConfig("plc_config.json");
 * @endcode
 *
 * ## 信号连接
 * @code
 * // 监控连接状态
 * connect(&PLCManager::instance(), &PLCManager::connectionStatusChanged,
 *         this, [](const QString& name, bool connected) {
 *             qDebug() << name << (connected ? "已连接" : "已断开");
 *         });
 *
 * // 监控错误
 * connect(&PLCManager::instance(), &PLCManager::errorOccurred,
 *         this, [](const QString& name, const QString& error) {
 *             qWarning() << "PLC错误:" << name << error;
 *         });
 * @endcode
 */
class PLCManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取PLCManager单例实例
     * @return PLCManager的引用
     *
     * @details
     * 使用Meyers单例模式，确保线程安全的延迟初始化。
     * 首次调用时创建实例，之后返回同一实例。
     *
     * @code
     * PLCManager& mgr = PLCManager::instance();
     * @endcode
     */
    static PLCManager& instance();

    /**
     * @brief 添加PLC连接
     * @param name 连接的唯一标识名称
     * @param config PLC配置参数
     * @return 添加成功返回true，失败返回false
     *
     * @details
     * 根据配置创建相应协议的通信实例并添加到管理器。
     *
     * ## 失败情况
     * - 连接名称为空
     * - 不支持的协议类型
     *
     * ## 覆盖行为
     * 如果指定名称的连接已存在，会先断开并移除旧连接，然后添加新连接。
     *
     * @note 添加成功后会发出connectionsChanged信号
     */
    bool addConnection(const QString& name, const PLCConfig& config);

    /**
     * @brief 移除PLC连接
     * @param name 要移除的连接名称
     * @return 移除成功返回true，连接不存在返回false
     *
     * @details
     * 断开指定连接并从管理器中移除。
     *
     * @note 移除成功后会发出connectionsChanged信号
     */
    bool removeConnection(const QString& name);

    /**
     * @brief 获取PLC通信对象
     * @param name 连接名称
     * @return PLC通信对象的智能指针，不存在则返回nullptr
     *
     * @details
     * 获取底层的PLCComm对象，可用于直接进行更复杂的操作。
     *
     * @warning 请谨慎使用返回的对象，避免在持有指针时连接被移除
     */
    std::shared_ptr<PLCComm> getConnection(const QString& name);

    /**
     * @brief 获取所有连接的名称列表
     * @return 连接名称的字符串列表
     */
    QStringList getConnectionNames() const;

    /**
     * @brief 获取指定连接的配置
     * @param name 连接名称
     * @return PLC配置，不存在则返回默认配置
     */
    PLCConfig getConfig(const QString& name) const;

    /**
     * @brief 连接指定的PLC
     * @param name 连接名称
     * @return 连接成功返回true，失败返回false
     *
     * @details
     * 使用已配置的参数连接到PLC。连接状态改变时会发出connectionStatusChanged信号。
     */
    bool connectPLC(const QString& name);

    /**
     * @brief 断开指定的PLC连接
     * @param name 连接名称
     *
     * @details
     * 断开与PLC的通信连接。如果连接不存在，则不执行任何操作。
     */
    void disconnectPLC(const QString& name);

    /**
     * @brief 连接所有已配置的PLC
     *
     * @details
     * 遍历所有已添加的连接，依次尝试连接。
     * 单个连接失败不影响其他连接的尝试。
     */
    void connectAll();

    /**
     * @brief 断开所有PLC连接
     *
     * @details
     * 遍历所有连接并断开。
     * 通常在程序退出或需要重新配置时调用。
     */
    void disconnectAll();

    /**
     * @brief 检查指定PLC是否已连接
     * @param name 连接名称
     * @return 已连接返回true，未连接或不存在返回false
     */
    bool isConnected(const QString& name) const;

    /**
     * @brief 通过管理器读取寄存器
     * @param connectionName 连接名称
     * @param address 起始寄存器地址
     * @param count 读取数量
     * @return PLCResult包含读取状态和数据
     *
     * @details
     * 便捷方法，直接通过连接名称读取寄存器，无需先获取PLCComm对象。
     *
     * @note 连接不存在时返回Disconnected状态
     */
    PLCResult readRegisters(const QString& connectionName, int address, int count);

    /**
     * @brief 通过管理器写入寄存器
     * @param connectionName 连接名称
     * @param address 起始寄存器地址
     * @param values 要写入的值
     * @return PLCResult包含写入状态
     *
     * @details
     * 便捷方法，直接通过连接名称写入寄存器。
     */
    PLCResult writeRegisters(const QString& connectionName, int address, const std::vector<int>& values);

    /**
     * @brief 通过管理器读取线圈
     * @param connectionName 连接名称
     * @param address 起始线圈地址
     * @param count 读取数量
     * @return PLCResult包含读取状态和数据
     */
    PLCResult readCoils(const QString& connectionName, int address, int count);

    /**
     * @brief 通过管理器写入线圈
     * @param connectionName 连接名称
     * @param address 起始线圈地址
     * @param values 要写入的布尔值
     * @return PLCResult包含写入状态
     */
    PLCResult writeCoils(const QString& connectionName, int address, const std::vector<bool>& values);

    /**
     * @brief 保存所有PLC配置到文件
     * @param filePath 保存的文件路径
     * @return 保存成功返回true，失败返回false
     *
     * @details
     * 将所有连接的配置序列化为JSON格式并保存到文件。
     *
     * ## JSON格式示例
     * @code{.json}
     * {
     *   "connections": [
     *     {
     *       "name": "主PLC",
     *       "protocol": 0,
     *       "slaveId": 1,
     *       "commConfig": {
     *         "type": 0,
     *         "ipAddress": "192.168.1.100",
     *         "port": 502
     *       }
     *     }
     *   ]
     * }
     * @endcode
     */
    bool saveConfig(const QString& filePath);

    /**
     * @brief 从文件加载PLC配置
     * @param filePath 配置文件路径
     * @return 加载成功返回true，失败返回false
     *
     * @details
     * 从JSON文件读取配置并重建所有连接。
     *
     * @warning 加载前会清除所有现有连接
     * @note 加载不会自动连接PLC，需要手动调用connectAll()或connectPLC()
     */
    bool loadConfig(const QString& filePath);

signals:
    /**
     * @brief 连接状态改变信号
     * @param name 连接名称
     * @param connected true表示已连接，false表示已断开
     *
     * @details
     * 当某个PLC的连接状态发生变化时发出。
     * 可用于更新UI状态指示器。
     */
    void connectionStatusChanged(const QString& name, bool connected);

    /**
     * @brief 通信错误信号
     * @param name 发生错误的连接名称
     * @param error 错误描述信息
     *
     * @details
     * 当通信过程中发生错误时发出。
     * 可用于显示错误提示或记录日志。
     */
    void errorOccurred(const QString& name, const QString& error);

    /**
     * @brief 连接列表改变信号
     *
     * @details
     * 当添加或移除连接时发出。
     * 可用于刷新连接列表的UI显示。
     */
    void connectionsChanged();

private:
    /**
     * @brief 私有构造函数（单例模式）
     * @param parent 父对象指针
     */
    PLCManager(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 断开所有连接并释放资源
     */
    ~PLCManager();

    /// @brief 禁用拷贝构造函数
    PLCManager(const PLCManager&) = delete;

    /// @brief 禁用赋值运算符
    PLCManager& operator=(const PLCManager&) = delete;

    /**
     * @brief 序列化PLC配置为QVariantMap
     * @param config 要序列化的配置
     * @return 序列化后的QVariantMap
     *
     * @details
     * 将PLCConfig结构体转换为可存储的键值对格式。
     * 根据通信类型（Socket/Serial）序列化不同的参数。
     */
    QVariantMap serializeConfig(const PLCConfig& config) const;

    /**
     * @brief 从QVariantMap反序列化PLC配置
     * @param map 序列化的配置数据
     * @return 反序列化后的PLCConfig
     *
     * @details
     * 从键值对格式还原PLCConfig结构体。
     * 根据通信类型创建相应的SocketConfig或SerialConfig。
     */
    PLCConfig deserializeConfig(const QVariantMap& map) const;

private:
    QMap<QString, std::shared_ptr<PLCComm>> connections_;  ///< 连接名称到通信对象的映射
    QMap<QString, PLCConfig> configs_;                      ///< 连接名称到配置的映射
    mutable QMutex mutex_;                                  ///< 线程同步互斥锁
};

} // namespace Comm
} // namespace VisionForge
