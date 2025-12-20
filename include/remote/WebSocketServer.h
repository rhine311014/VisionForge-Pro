/**
 * @file WebSocketServer.h
 * @brief WebSocket服务器 - 远程诊断系统核心通信组件
 * @details 提供实时双向通信能力，支持多客户端连接和消息广播
 * @author VisionForge Team
 * @date 2025-12-20
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
 * @brief 客户端信息
 */
struct ClientInfo {
    QString clientId;                               // 客户端ID
    QString sessionId;                              // 会话ID
    QWebSocket* socket = nullptr;                   // Socket指针
    QDateTime connectTime;                          // 连接时间
    QSet<SubscriptionType> subscriptions;           // 订阅的数据类型
    qint64 lastPingTime = 0;                        // 最后心跳时间
    int pingFailCount = 0;                          // 心跳失败次数

    bool isAlive() const;
};

/**
 * @brief WebSocket服务器配置
 */
struct ServerConfig {
    quint16 port = 8765;                            // 监听端口
    int maxConnections = 100;                       // 最大连接数
    int heartbeatIntervalMs = 10000;                // 心跳间隔 (ms)
    int heartbeatTimeoutMs = 30000;                 // 心跳超时 (ms)
    int maxPingFailCount = 3;                       // 最大心跳失败次数
    bool enableSsl = false;                         // 是否启用SSL
    bool requireAuth = true;                        // 是否需要认证
};

/**
 * @class WebSocketServer
 * @brief WebSocket服务器
 *
 * 功能特性:
 * - 多客户端连接管理
 * - 消息广播和单播
 * - 自动心跳检测
 * - 订阅/取消订阅机制
 * - 连接状态监控
 */
class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketServer(QObject* parent = nullptr);
    ~WebSocketServer() override;

    /**
     * @brief 设置SSL配置（必须在start之前调用）
     */
    void setSslConfiguration(const QSslConfiguration& config);

    /**
     * @brief 启动服务器
     * @param config 服务器配置
     * @return 是否成功启动
     */
    bool start(const ServerConfig& config = ServerConfig());

    /**
     * @brief 停止服务器
     */
    void stop();

    /**
     * @brief 服务器是否正在运行
     */
    bool isRunning() const { return server_ && server_->isListening(); }

    /**
     * @brief 获取服务器配置
     */
    const ServerConfig& config() const { return config_; }

    /**
     * @brief 获取连接的客户端数量
     */
    int clientCount() const;

    /**
     * @brief 获取所有客户端信息
     */
    QList<ClientInfo> clients() const;

    /**
     * @brief 获取特定客户端信息
     */
    ClientInfo clientInfo(const QString& sessionId) const;

    // ========== 消息发送 ==========

    /**
     * @brief 广播消息给所有客户端
     */
    void broadcast(const QJsonObject& message);

    /**
     * @brief 广播消息给订阅了特定类型的客户端
     */
    void broadcastToSubscribers(SubscriptionType type, const QJsonObject& message);

    /**
     * @brief 发送消息给特定客户端
     */
    void sendTo(const QString& sessionId, const QJsonObject& message);

    /**
     * @brief 发送消息给特定Socket
     */
    void sendTo(QWebSocket* socket, const QJsonObject& message);

    // ========== 便捷方法 ==========

    /**
     * @brief 推送性能指标
     */
    void pushPerformanceMetrics(const PerformanceMetrics& metrics);

    /**
     * @brief 推送日志条目
     */
    void pushLogEntry(const LogEntryData& log);

    /**
     * @brief 推送系统状态
     */
    void pushSystemStatus(const SystemStatusData& status);

    // ========== 客户端管理 ==========

    /**
     * @brief 断开指定客户端
     */
    void disconnectClient(const QString& sessionId, const QString& reason = QString());

    /**
     * @brief 断开所有客户端
     */
    void disconnectAll(const QString& reason = QString());

signals:
    /**
     * @brief 服务器启动成功
     */
    void serverStarted(quint16 port);

    /**
     * @brief 服务器停止
     */
    void serverStopped();

    /**
     * @brief 服务器启动失败
     */
    void serverError(const QString& error);

    /**
     * @brief 新客户端连接
     */
    void clientConnected(const QString& sessionId, const QString& clientId);

    /**
     * @brief 客户端断开
     */
    void clientDisconnected(const QString& sessionId, const QString& reason);

    /**
     * @brief 收到客户端消息
     */
    void messageReceived(const QString& sessionId, MessageType type, const QJsonObject& data);

    /**
     * @brief 客户端订阅数据
     */
    void clientSubscribed(const QString& sessionId, SubscriptionType type);

    /**
     * @brief 客户端取消订阅
     */
    void clientUnsubscribed(const QString& sessionId, SubscriptionType type);

    /**
     * @brief 收到客户端命令
     */
    void commandReceived(const QString& sessionId, const QString& command, const QJsonObject& params);

private slots:
    /**
     * @brief 新连接建立
     */
    void onNewConnection();

    /**
     * @brief 收到文本消息
     */
    void onTextMessageReceived(const QString& message);

    /**
     * @brief Socket断开
     */
    void onSocketDisconnected();

    /**
     * @brief Socket错误
     */
    void onSocketError(QAbstractSocket::SocketError error);

    /**
     * @brief 心跳定时器超时
     */
    void onHeartbeatTimeout();

private:
    /**
     * @brief 处理客户端消息
     */
    void handleClientMessage(QWebSocket* socket, MessageType type, const QJsonObject& data);

    /**
     * @brief 处理握手消息
     */
    void handleClientHello(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理订阅消息
     */
    void handleSubscribe(QWebSocket* socket, const QJsonObject& data);
    void handleUnsubscribe(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理命令消息
     */
    void handleCommand(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理查询消息
     */
    void handleQueryHistory(QWebSocket* socket, const QJsonObject& data);

    /**
     * @brief 处理心跳消息
     */
    void handlePing(QWebSocket* socket);

    /**
     * @brief 注册新客户端
     */
    QString registerClient(QWebSocket* socket, const QString& clientId);

    /**
     * @brief 注销客户端
     */
    void unregisterClient(QWebSocket* socket);

    /**
     * @brief 查找客户端信息
     */
    ClientInfo* findClientBySocket(QWebSocket* socket);
    ClientInfo* findClientBySessionId(const QString& sessionId);

    /**
     * @brief 生成会话ID
     */
    QString generateSessionId() const;

    /**
     * @brief 检查心跳超时
     */
    void checkHeartbeats();

    /**
     * @brief 发送Pong响应
     */
    void sendPong(QWebSocket* socket);

private:
    QWebSocketServer* server_;                      // WebSocket服务器
    ServerConfig config_;                           // 服务器配置
    QMap<QString, ClientInfo> clients_;             // 客户端映射 (sessionId -> ClientInfo)
    QMap<QWebSocket*, QString> socketToSession_;    // Socket到SessionId的映射
    QTimer* heartbeatTimer_;                        // 心跳定时器
    QSslConfiguration sslConfig_;                   // SSL配置
    bool sslConfigured_;                            // SSL是否已配置
    mutable QMutex mutex_;                          // 线程安全锁
};

// ========== Inline实现 ==========

inline bool ClientInfo::isAlive() const
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    return (currentTime - lastPingTime) < 30000; // 30秒内有心跳
}

} // namespace Remote
} // namespace VisionForge
