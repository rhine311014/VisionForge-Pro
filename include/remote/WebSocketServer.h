/**
 * @file WebSocketServer.h
 * @brief WebSocket服务器 - 远程诊断系统核心通信组件
 * @details 提供实时双向通信能力，支持多客户端连接和消息广播
 *
 * @section server_overview 服务器概述
 * 本WebSocket服务器是远程诊断系统的核心网络组件，负责:
 * - 管理多个客户端连接
 * - 处理客户端认证和会话管理
 * - 实现消息的单播和广播
 * - 支持订阅/发布模式的数据推送
 * - 维护心跳检测保持连接活性
 *
 * @section connection_lifecycle 连接生命周期
 * 1. 客户端发起WebSocket连接
 * 2. 连接建立后，客户端发送ClientHello握手
 * 3. 服务器验证并分配SessionId
 * 4. 客户端发送Subscribe订阅数据
 * 5. 服务器推送订阅的实时数据
 * 6. 双方通过Ping/Pong维持连接
 * 7. 连接断开时自动清理资源
 *
 * @section security_features 安全特性
 * - 支持WSS (WebSocket Secure) 加密传输
 * - 可配置的最大连接数限制
 * - 心跳超时检测和自动断开
 * - 支持与TokenAuthManager集成进行认证
 *
 * @section usage_example 使用示例
 * @code
 * WebSocketServer server;
 *
 * // 配置SSL (可选)
 * QSslConfiguration sslConfig;
 * // ... 配置SSL证书 ...
 * server.setSslConfiguration(sslConfig);
 *
 * // 配置服务器参数
 * ServerConfig config;
 * config.port = 8765;
 * config.maxConnections = 100;
 * config.heartbeatIntervalMs = 10000;
 *
 * // 启动服务器
 * if (server.start(config)) {
 *     qInfo() << "服务器启动成功";
 * }
 *
 * // 推送性能数据给订阅者
 * PerformanceMetrics metrics;
 * server.pushPerformanceMetrics(metrics);
 * @endcode
 *
 * @author VisionForge Team
 * @version 1.6.0
 * @date 2025-12-20
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 */

#pragma once

#include "remote/RemoteProtocol.h"
#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QMap>
#include <QSet>
#include <QTimer>
#include <QMutex>
#include <QSslConfiguration>
#include <memory>

namespace VisionForge {
namespace Remote {

/**
 * @brief 客户端连接信息结构
 * @details 记录每个已连接客户端的完整状态信息
 */
struct ClientInfo {
    QString clientId;                               ///< 客户端唯一标识（由客户端提供）
    QString sessionId;                              ///< 会话ID（由服务器分配）
    QWebSocket* socket = nullptr;                   ///< WebSocket连接指针
    QDateTime connectTime;                          ///< 连接建立时间
    QSet<SubscriptionType> subscriptions;           ///< 已订阅的数据类型集合
    qint64 lastPingTime = 0;                        ///< 最后心跳时间戳（毫秒）
    int pingFailCount = 0;                          ///< 连续心跳失败次数

    /**
     * @brief 检查连接是否存活
     * @return 30秒内有心跳返回true，否则返回false
     */
    bool isAlive() const;
};

/**
 * @brief WebSocket服务器配置结构
 * @details 包含服务器运行所需的所有配置参数
 */
struct ServerConfig {
    quint16 port = 8765;                            ///< 监听端口号
    int maxConnections = 100;                       ///< 最大并发连接数
    int heartbeatIntervalMs = 10000;                ///< 心跳检测间隔（毫秒）
    int heartbeatTimeoutMs = 30000;                 ///< 心跳超时时间（毫秒）
    int maxPingFailCount = 3;                       ///< 最大允许心跳失败次数
    bool enableSsl = false;                         ///< 是否启用SSL/TLS加密
    bool requireAuth = true;                        ///< 是否需要客户端认证
};

/**
 * @class WebSocketServer
 * @brief WebSocket服务器核心类
 *
 * @details 主要功能包括:
 * - 多客户端连接管理：支持多个客户端同时连接
 * - 消息广播和单播：可向所有客户端或指定客户端发送消息
 * - 自动心跳检测：定期检测连接活性，自动清理断开的连接
 * - 订阅/取消订阅机制：客户端可选择性接收特定类型的数据
 * - 连接状态监控：实时监控所有连接的状态
 *
 * @note 线程安全：所有公共方法都是线程安全的
 */
class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    explicit WebSocketServer(QObject* parent = nullptr);

    /**
     * @brief 析构函数，自动清理所有资源
     */
    ~WebSocketServer() override;

    // ==================== 服务器控制 ====================

    /**
     * @brief 设置SSL配置
     * @param config SSL配置对象
     * @warning 必须在start()之前调用
     *
     * 使用示例:
     * @code
     * QSslConfiguration sslConfig;
     * sslConfig.setLocalCertificate(cert);
     * sslConfig.setPrivateKey(key);
     * server.setSslConfiguration(sslConfig);
     * @endcode
     */
    void setSslConfiguration(const QSslConfiguration& config);

    /**
     * @brief 启动WebSocket服务器
     * @param config 服务器配置
     * @return 启动成功返回true，失败返回false
     *
     * 启动流程:
     * 1. 创建QWebSocketServer实例
     * 2. 配置SSL（如启用）
     * 3. 绑定端口开始监听
     * 4. 启动心跳检测定时器
     */
    bool start(const ServerConfig& config = ServerConfig());

    /**
     * @brief 停止服务器
     * @details 断开所有客户端连接，释放所有资源
     */
    void stop();

    /**
     * @brief 检查服务器是否正在运行
     * @return 服务器正在监听返回true
     */
    bool isRunning() const { return server_ && server_->isListening(); }

    /**
     * @brief 获取当前服务器配置
     * @return 配置结构的常量引用
     */
    const ServerConfig& config() const { return config_; }

    // ==================== 客户端信息查询 ====================

    /**
     * @brief 获取当前连接的客户端数量
     * @return 已连接的客户端数
     */
    int clientCount() const;

    /**
     * @brief 获取所有已连接客户端的信息列表
     * @return 客户端信息列表
     */
    QList<ClientInfo> clients() const;

    /**
     * @brief 获取指定会话ID的客户端信息
     * @param sessionId 会话ID
     * @return 客户端信息，不存在时返回空结构
     */
    ClientInfo clientInfo(const QString& sessionId) const;

    // ==================== 消息发送接口 ====================

    /**
     * @brief 向所有已连接客户端广播消息
     * @param message JSON格式的消息对象
     *
     * @note 广播会发送给所有已认证的客户端，无论其订阅状态
     */
    void broadcast(const QJsonObject& message);

    /**
     * @brief 向订阅了指定类型的客户端广播消息
     * @param type 订阅类型
     * @param message JSON格式的消息对象
     *
     * @note 仅发送给订阅了该类型或订阅了All的客户端
     */
    void broadcastToSubscribers(SubscriptionType type, const QJsonObject& message);

    /**
     * @brief 向指定会话ID的客户端发送消息
     * @param sessionId 目标客户端的会话ID
     * @param message JSON格式的消息对象
     */
    void sendTo(const QString& sessionId, const QJsonObject& message);

    /**
     * @brief 向指定Socket发送消息
     * @param socket 目标WebSocket指针
     * @param message JSON格式的消息对象
     */
    void sendTo(QWebSocket* socket, const QJsonObject& message);

    // ==================== 数据推送便捷方法 ====================

    /**
     * @brief 推送性能指标给订阅了Performance的客户端
     * @param metrics 性能指标数据
     */
    void pushPerformanceMetrics(const PerformanceMetrics& metrics);

    /**
     * @brief 推送日志条目给订阅了Logs的客户端
     * @param log 日志条目数据
     */
    void pushLogEntry(const LogEntryData& log);

    /**
     * @brief 推送系统状态给订阅了SystemStatus的客户端
     * @param status 系统状态数据
     */
    void pushSystemStatus(const SystemStatusData& status);

    // ==================== 客户端管理 ====================

    /**
     * @brief 主动断开指定客户端的连接
     * @param sessionId 要断开的客户端会话ID
     * @param reason 断开原因描述（可选）
     */
    void disconnectClient(const QString& sessionId, const QString& reason = QString());

    /**
     * @brief 断开所有客户端连接
     * @param reason 断开原因描述（可选）
     */
    void disconnectAll(const QString& reason = QString());

signals:
    // ==================== 服务器状态信号 ====================

    /**
     * @brief 服务器成功启动信号
     * @param port 监听的端口号
     */
    void serverStarted(quint16 port);

    /**
     * @brief 服务器停止信号
     */
    void serverStopped();

    /**
     * @brief 服务器错误信号
     * @param error 错误描述
     */
    void serverError(const QString& error);

    // ==================== 客户端连接信号 ====================

    /**
     * @brief 新客户端连接成功信号
     * @param sessionId 分配的会话ID
     * @param clientId 客户端提供的ID
     */
    void clientConnected(const QString& sessionId, const QString& clientId);

    /**
     * @brief 客户端断开连接信号
     * @param sessionId 断开的会话ID
     * @param reason 断开原因
     */
    void clientDisconnected(const QString& sessionId, const QString& reason);

    // ==================== 消息处理信号 ====================

    /**
     * @brief 收到客户端消息信号
     * @param sessionId 发送消息的会话ID
     * @param type 消息类型
     * @param data 消息数据
     */
    void messageReceived(const QString& sessionId, MessageType type, const QJsonObject& data);

    /**
     * @brief 客户端订阅数据信号
     * @param sessionId 订阅的会话ID
     * @param type 订阅的数据类型
     */
    void clientSubscribed(const QString& sessionId, SubscriptionType type);

    /**
     * @brief 客户端取消订阅信号
     * @param sessionId 取消订阅的会话ID
     * @param type 取消的数据类型
     */
    void clientUnsubscribed(const QString& sessionId, SubscriptionType type);

    /**
     * @brief 收到客户端命令信号
     * @param sessionId 发送命令的会话ID
     * @param command 命令名称
     * @param params 命令参数
     */
    void commandReceived(const QString& sessionId, const QString& command, const QJsonObject& params);

private slots:
    /**
     * @brief 处理新的WebSocket连接
     */
    void onNewConnection();

    /**
     * @brief 处理收到的文本消息
     * @param message 消息内容
     */
    void onTextMessageReceived(const QString& message);

    /**
     * @brief 处理Socket断开事件
     */
    void onSocketDisconnected();

    /**
     * @brief 处理Socket错误
     * @param error 错误类型
     */
    void onSocketError(QAbstractSocket::SocketError error);

    /**
     * @brief 心跳定时器超时处理
     */
    void onHeartbeatTimeout();

private:
    // ==================== 消息处理方法 ====================

    /**
     * @brief 分发处理客户端消息
     * @param socket 消息来源Socket
     * @param type 消息类型
     * @param data 消息数据
     */
    void handleClientMessage(QWebSocket* socket, MessageType type, const QJsonObject& data);

    /**
     * @brief 处理客户端握手消息
     * @param socket 消息来源Socket
     * @param data 握手数据
     */
    void handleClientHello(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理订阅请求
     * @param socket 消息来源Socket
     * @param data 订阅数据
     */
    void handleSubscribe(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理取消订阅请求
     * @param socket 消息来源Socket
     * @param data 取消订阅数据
     */
    void handleUnsubscribe(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理客户端命令
     * @param socket 消息来源Socket
     * @param data 命令数据
     */
    void handleCommand(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理历史数据查询请求
     * @param socket 消息来源Socket
     * @param data 查询参数
     */
    void handleQueryHistory(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理心跳请求
     * @param socket 消息来源Socket
     */
    void handlePing(QWebSocket* socket);

    // ==================== 客户端管理方法 ====================

    /**
     * @brief 注册新客户端
     * @param socket 客户端Socket
     * @param clientId 客户端ID
     * @return 分配的会话ID
     */
    QString registerClient(QWebSocket* socket, const QString& clientId);

    /**
     * @brief 注销客户端
     * @param socket 客户端Socket
     */
    void unregisterClient(QWebSocket* socket);

    /**
     * @brief 通过Socket查找客户端信息
     * @param socket 目标Socket
     * @return 客户端信息指针，不存在返回nullptr
     */
    ClientInfo* findClientBySocket(QWebSocket* socket);

    /**
     * @brief 通过会话ID查找客户端信息
     * @param sessionId 目标会话ID
     * @return 客户端信息指针，不存在返回nullptr
     */
    ClientInfo* findClientBySessionId(const QString& sessionId);

    /**
     * @brief 生成唯一的会话ID
     * @return UUID格式的会话ID
     */
    QString generateSessionId() const;

    // ==================== 心跳管理方法 ====================

    /**
     * @brief 检查所有客户端的心跳状态
     */
    void checkHeartbeats();

    /**
     * @brief 发送Pong响应
     * @param socket 目标Socket
     */
    void sendPong(QWebSocket* socket);

private:
    QWebSocketServer* server_;                      ///< WebSocket服务器实例
    ServerConfig config_;                           ///< 服务器配置
    QMap<QString, ClientInfo> clients_;             ///< 客户端映射 (sessionId -> ClientInfo)
    QMap<QWebSocket*, QString> socketToSession_;    ///< Socket到会话ID的映射
    QTimer* heartbeatTimer_;                        ///< 心跳检测定时器
    QSslConfiguration sslConfig_;                   ///< SSL配置
    bool sslConfigured_;                            ///< SSL是否已配置标志
    mutable QMutex mutex_;                          ///< 线程安全互斥锁
};

// ==================== Inline实现 ====================

inline bool ClientInfo::isAlive() const
{
    // 检查最后心跳时间是否在30秒内
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    return (currentTime - lastPingTime) < 30000;
}

} // namespace Remote
} // namespace VisionForge
