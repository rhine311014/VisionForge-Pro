/**
 * @file WebSocketServer.cpp
 * @brief WebSocket服务器实现
 * @details 实现WebSocket服务器的核心功能，包括连接管理、消息处理和心跳检测
 *
 * @section implementation_details 实现细节
 * - 使用QWebSocketServer作为底层WebSocket实现
 * - 支持WS和WSS两种协议
 * - 采用事件驱动的异步处理模式
 * - 所有操作都是线程安全的
 *
 * @author VisionForge Team
 * @version 1.6.0
 * @date 2025-12-20
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 */

#include "remote/WebSocketServer.h"
#include <QJsonDocument>
#include <QUuid>
#include <QDebug>
#include <QSslSocket>

namespace VisionForge {
namespace Remote {

// ==================== 构造和析构 ====================

/**
 * @brief 构造WebSocket服务器
 *
 * 初始化所有成员变量为默认状态
 */
WebSocketServer::WebSocketServer(QObject* parent)
    : QObject(parent)
    , server_(nullptr)
    , heartbeatTimer_(nullptr)
    , sslConfigured_(false)
{
}

/**
 * @brief 析构函数
 *
 * 自动停止服务器并清理所有资源
 */
WebSocketServer::~WebSocketServer()
{
    stop();
}

// ==================== 服务器控制 ====================

/**
 * @brief 设置SSL/TLS配置
 *
 * 必须在调用start()之前设置SSL配置。
 * 如果启用了SSL但未调用此方法，将使用系统默认的SSL配置。
 *
 * @param config Qt SSL配置对象
 */
void WebSocketServer::setSslConfiguration(const QSslConfiguration& config)
{
    sslConfig_ = config;
    sslConfigured_ = true;
    qInfo() << "SSL配置已设置";
}

/**
 * @brief 启动WebSocket服务器
 *
 * 启动流程:
 * 1. 检查服务器是否已在运行
 * 2. 创建QWebSocketServer实例（根据配置选择WS或WSS模式）
 * 3. 配置SSL（如果启用）
 * 4. 开始在指定端口监听
 * 5. 启动心跳检测定时器
 *
 * @param config 服务器配置参数
 * @return 启动成功返回true，失败返回false
 */
bool WebSocketServer::start(const ServerConfig& config)
{
    // 检查是否已在运行
    if (isRunning()) {
        qWarning() << "WebSocket服务器已经在运行";
        return false;
    }

    config_ = config;

    // 创建WebSocket服务器实例
    // 根据配置选择安全模式（WSS）或非安全模式（WS）
    server_ = new QWebSocketServer(
        QStringLiteral("VisionForge Remote Diagnostics"),
        config_.enableSsl ? QWebSocketServer::SecureMode : QWebSocketServer::NonSecureMode,
        this
    );

    // 配置SSL（如果启用）
    if (config_.enableSsl) {
        // 检查系统是否支持SSL
        if (!QSslSocket::supportsSsl()) {
            QString error = "系统不支持SSL";
            qCritical() << error;
            emit serverError(error);
            delete server_;
            server_ = nullptr;
            return false;
        }

        // 应用SSL配置
        if (sslConfigured_) {
            server_->setSslConfiguration(sslConfig_);
            qInfo() << "WSS模式启用，SSL配置已应用";
        } else {
            qWarning() << "启用了SSL但未配置SSL证书，使用默认配置";
        }

        // 连接SSL错误信号
        connect(server_, &QWebSocketServer::sslErrors, this, [this](const QList<QSslError>& errors) {
            qWarning() << "WSS SSL错误:";
            for (const auto& error : errors) {
                qWarning() << "  -" << error.errorString();
            }
        });
    }

    // 连接服务器信号
    connect(server_, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    connect(server_, &QWebSocketServer::serverError, this, [this](QWebSocketProtocol::CloseCode closeCode) {
        Q_UNUSED(closeCode);
        emit serverError(server_->errorString());
    });

    // 开始监听
    if (!server_->listen(QHostAddress::Any, config_.port)) {
        QString error = QString("无法启动WebSocket服务器: %1").arg(server_->errorString());
        qCritical() << error;
        emit serverError(error);
        delete server_;
        server_ = nullptr;
        return false;
    }

    // 启动心跳检测定时器
    heartbeatTimer_ = new QTimer(this);
    connect(heartbeatTimer_, &QTimer::timeout, this, &WebSocketServer::onHeartbeatTimeout);
    heartbeatTimer_->start(config_.heartbeatIntervalMs);

    qInfo() << "WebSocket服务器启动成功，监听端口:" << config_.port;
    emit serverStarted(config_.port);

    return true;
}

/**
 * @brief 停止WebSocket服务器
 *
 * 停止流程:
 * 1. 停止心跳检测定时器
 * 2. 断开所有客户端连接
 * 3. 关闭服务器端口监听
 * 4. 释放所有资源
 */
void WebSocketServer::stop()
{
    if (!isRunning()) {
        return;
    }

    // 停止心跳定时器
    if (heartbeatTimer_) {
        heartbeatTimer_->stop();
        heartbeatTimer_->deleteLater();
        heartbeatTimer_ = nullptr;
    }

    // 断开所有客户端
    disconnectAll("服务器关闭");

    // 关闭服务器
    server_->close();
    server_->deleteLater();
    server_ = nullptr;

    qInfo() << "WebSocket服务器已停止";
    emit serverStopped();
}

// ==================== 客户端信息查询 ====================

/**
 * @brief 获取当前连接的客户端数量
 * @return 已连接的客户端数量
 */
int WebSocketServer::clientCount() const
{
    QMutexLocker locker(&mutex_);
    return clients_.size();
}

/**
 * @brief 获取所有已连接客户端的信息列表
 * @return 客户端信息列表的副本
 */
QList<ClientInfo> WebSocketServer::clients() const
{
    QMutexLocker locker(&mutex_);
    return clients_.values();
}

/**
 * @brief 获取指定会话ID的客户端信息
 * @param sessionId 会话ID
 * @return 客户端信息，不存在时返回空结构
 */
ClientInfo WebSocketServer::clientInfo(const QString& sessionId) const
{
    QMutexLocker locker(&mutex_);
    return clients_.value(sessionId);
}

/**
 * @brief 主动断开指定客户端的连接
 * @param sessionId 要断开的客户端会话ID
 * @param reason 断开原因（可选）
 */
void WebSocketServer::disconnectClient(const QString& sessionId, const QString& reason)
{
    QMutexLocker locker(&mutex_);

    if (!clients_.contains(sessionId)) {
        return;
    }

    ClientInfo& info = clients_[sessionId];
    if (info.socket) {
        info.socket->close(QWebSocketProtocol::CloseCodeNormal, reason);
    }
}

/**
 * @brief 断开所有客户端连接
 * @param reason 断开原因（可选）
 */
void WebSocketServer::disconnectAll(const QString& reason)
{
    QMutexLocker locker(&mutex_);

    for (auto& info : clients_) {
        if (info.socket) {
            info.socket->close(QWebSocketProtocol::CloseCodeNormal, reason);
        }
    }
}

// ==================== 消息发送 ====================

/**
 * @brief 向所有客户端广播消息
 *
 * 遍历所有已连接的客户端，将消息发送给每一个
 *
 * @param message JSON格式的消息对象
 */
void WebSocketServer::broadcast(const QJsonObject& message)
{
    QMutexLocker locker(&mutex_);

    // 将JSON对象转换为紧凑格式的字符串
    QString jsonString = QJsonDocument(message).toJson(QJsonDocument::Compact);

    // 向所有有效连接发送消息
    for (const auto& info : clients_) {
        if (info.socket && info.socket->isValid()) {
            info.socket->sendTextMessage(jsonString);
        }
    }
}

/**
 * @brief 向订阅了特定类型的客户端广播消息
 *
 * 只有订阅了指定类型或订阅了All的客户端才会收到消息
 *
 * @param type 订阅类型
 * @param message JSON格式的消息对象
 */
void WebSocketServer::broadcastToSubscribers(SubscriptionType type, const QJsonObject& message)
{
    QMutexLocker locker(&mutex_);

    QString jsonString = QJsonDocument(message).toJson(QJsonDocument::Compact);

    for (const auto& info : clients_) {
        // 检查客户端是否订阅了该类型或订阅了All
        if ((info.subscriptions.contains(type) || info.subscriptions.contains(SubscriptionType::All)) &&
            info.socket && info.socket->isValid()) {
            info.socket->sendTextMessage(jsonString);
        }
    }
}

/**
 * @brief 向指定会话ID的客户端发送消息
 * @param sessionId 目标客户端的会话ID
 * @param message JSON格式的消息对象
 */
void WebSocketServer::sendTo(const QString& sessionId, const QJsonObject& message)
{
    QMutexLocker locker(&mutex_);

    if (!clients_.contains(sessionId)) {
        return;
    }

    ClientInfo& info = clients_[sessionId];
    if (info.socket && info.socket->isValid()) {
        QString jsonString = QJsonDocument(message).toJson(QJsonDocument::Compact);
        info.socket->sendTextMessage(jsonString);
    }
}

/**
 * @brief 向指定Socket发送消息
 * @param socket 目标WebSocket指针
 * @param message JSON格式的消息对象
 */
void WebSocketServer::sendTo(QWebSocket* socket, const QJsonObject& message)
{
    if (socket && socket->isValid()) {
        QString jsonString = QJsonDocument(message).toJson(QJsonDocument::Compact);
        socket->sendTextMessage(jsonString);
    }
}

// ==================== 数据推送便捷方法 ====================

/**
 * @brief 推送性能指标数据
 *
 * 将性能指标封装为协议消息，推送给订阅了Performance类型的客户端
 *
 * @param metrics 性能指标数据
 */
void WebSocketServer::pushPerformanceMetrics(const PerformanceMetrics& metrics)
{
    QJsonObject message = RemoteProtocol::createPerformanceUpdate(metrics);
    broadcastToSubscribers(SubscriptionType::Performance, message);
}

/**
 * @brief 推送日志条目
 *
 * 将日志条目封装为协议消息，推送给订阅了Logs类型的客户端
 *
 * @param log 日志条目数据
 */
void WebSocketServer::pushLogEntry(const LogEntryData& log)
{
    QJsonObject message = RemoteProtocol::createLogEntry(log);
    broadcastToSubscribers(SubscriptionType::Logs, message);
}

/**
 * @brief 推送系统状态
 *
 * 将系统状态封装为协议消息，推送给订阅了SystemStatus类型的客户端
 *
 * @param status 系统状态数据
 */
void WebSocketServer::pushSystemStatus(const SystemStatusData& status)
{
    QJsonObject message = RemoteProtocol::createSystemStatus(status);
    broadcastToSubscribers(SubscriptionType::SystemStatus, message);
}

// ==================== 连接管理 ====================

/**
 * @brief 处理新的WebSocket连接
 *
 * 当有新客户端连接时:
 * 1. 检查是否超过最大连接数
 * 2. 设置Socket信号连接
 * 3. 等待客户端发送ClientHello完成握手
 */
void WebSocketServer::onNewConnection()
{
    QWebSocket* socket = server_->nextPendingConnection();

    if (!socket) {
        return;
    }

    // 检查最大连接数限制
    if (clients_.size() >= config_.maxConnections) {
        qWarning() << "达到最大连接数限制，拒绝新连接";
        socket->close(QWebSocketProtocol::CloseCodePolicyViolated, "服务器连接数已满");
        return;
    }

    // 连接Socket信号
    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::onSocketDisconnected);
    connect(socket, &QWebSocket::errorOccurred, this, &WebSocketServer::onSocketError);

    qInfo() << "新客户端连接:" << socket->peerAddress().toString();
}

/**
 * @brief 处理Socket断开事件
 *
 * 当客户端断开连接时，注销该客户端并释放资源
 */
void WebSocketServer::onSocketDisconnected()
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    if (!socket) {
        return;
    }

    unregisterClient(socket);
    socket->deleteLater();
}

/**
 * @brief 处理Socket错误
 * @param error 错误类型
 */
void WebSocketServer::onSocketError(QAbstractSocket::SocketError error)
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    if (!socket) {
        return;
    }

    qWarning() << "Socket错误:" << error << socket->errorString();
}

// ==================== 消息处理 ====================

/**
 * @brief 处理收到的文本消息
 *
 * 消息处理流程:
 * 1. 解析JSON消息
 * 2. 验证消息格式
 * 3. 根据消息类型分发处理
 *
 * @param message 收到的消息内容
 */
void WebSocketServer::onTextMessageReceived(const QString& message)
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    if (!socket) {
        return;
    }

    // 解析消息
    MessageType type;
    QJsonObject data;

    if (!RemoteProtocol::parseMessage(message, type, data)) {
        qWarning() << "无效的消息格式";
        sendTo(socket, RemoteProtocol::createError("无效的消息格式", 400));
        return;
    }

    // 分发处理
    handleClientMessage(socket, type, data);
}

/**
 * @brief 消息分发处理
 *
 * 根据消息类型将消息分发到对应的处理方法
 *
 * @param socket 消息来源Socket
 * @param type 消息类型
 * @param data 消息数据
 */
void WebSocketServer::handleClientMessage(QWebSocket* socket, MessageType type, const QJsonObject& data)
{
    switch (type) {
        case MessageType::ClientHello:
            handleClientHello(socket, data);
            break;

        case MessageType::Subscribe:
            handleSubscribe(socket, data);
            break;

        case MessageType::Unsubscribe:
            handleUnsubscribe(socket, data);
            break;

        case MessageType::SendCommand:
            handleCommand(socket, data);
            break;

        case MessageType::QueryHistory:
            handleQueryHistory(socket, data);
            break;

        case MessageType::Ping:
            handlePing(socket);
            break;

        default:
            qWarning() << "未知的消息类型:" << static_cast<int>(type);
            sendTo(socket, RemoteProtocol::createError("未知的消息类型", 400));
            break;
    }
}

// ==================== 具体消息处理 ====================

/**
 * @brief 处理客户端握手消息
 *
 * 握手处理流程:
 * 1. 验证clientId是否存在
 * 2. 注册客户端并分配sessionId
 * 3. 发送握手成功响应
 *
 * @param socket 消息来源Socket
 * @param data 握手数据（包含client_id和version）
 */
void WebSocketServer::handleClientHello(QWebSocket* socket, const QJsonObject& data)
{
    QString clientId = data["client_id"].toString();
    QString version = data["version"].toString();

    // 验证必要字段
    if (clientId.isEmpty()) {
        sendTo(socket, RemoteProtocol::createError("缺少client_id", 400));
        return;
    }

    // 注册客户端
    QString sessionId = registerClient(socket, clientId);

    // 发送握手成功响应
    QJsonObject response = RemoteProtocol::createServerHello(true, sessionId);
    sendTo(socket, response);

    qInfo() << QString("客户端握手成功: %1 (session: %2)").arg(clientId, sessionId);
    emit clientConnected(sessionId, clientId);
}

/**
 * @brief 处理订阅请求
 *
 * 将指定的数据类型添加到客户端的订阅列表中
 *
 * @param socket 消息来源Socket
 * @param data 订阅数据（包含subscription_type）
 */
void WebSocketServer::handleSubscribe(QWebSocket* socket, const QJsonObject& data)
{
    ClientInfo* info = findClientBySocket(socket);
    if (!info) {
        sendTo(socket, RemoteProtocol::createError("未认证的连接", 401));
        return;
    }

    QString typeStr = data["subscription_type"].toString();
    SubscriptionType type = RemoteProtocol::stringToSubscriptionType(typeStr);

    // 添加到订阅集合
    info->subscriptions.insert(type);

    qInfo() << QString("客户端 %1 订阅了 %2").arg(info->sessionId, typeStr);
    emit clientSubscribed(info->sessionId, type);
}

/**
 * @brief 处理取消订阅请求
 *
 * 从客户端的订阅列表中移除指定的数据类型
 *
 * @param socket 消息来源Socket
 * @param data 取消订阅数据（包含subscription_type）
 */
void WebSocketServer::handleUnsubscribe(QWebSocket* socket, const QJsonObject& data)
{
    ClientInfo* info = findClientBySocket(socket);
    if (!info) {
        return;
    }

    QString typeStr = data["subscription_type"].toString();
    SubscriptionType type = RemoteProtocol::stringToSubscriptionType(typeStr);

    // 从订阅集合移除
    info->subscriptions.remove(type);

    qInfo() << QString("客户端 %1 取消订阅 %2").arg(info->sessionId, typeStr);
    emit clientUnsubscribed(info->sessionId, type);
}

/**
 * @brief 处理客户端命令
 *
 * 接收客户端发送的命令并转发给命令处理模块
 *
 * @param socket 消息来源Socket
 * @param data 命令数据（包含command和params）
 */
void WebSocketServer::handleCommand(QWebSocket* socket, const QJsonObject& data)
{
    ClientInfo* info = findClientBySocket(socket);
    if (!info) {
        sendTo(socket, RemoteProtocol::createError("未认证的连接", 401));
        return;
    }

    QString command = data["command"].toString();
    QJsonObject params = data["params"].toObject();

    qInfo() << QString("收到客户端 %1 的命令: %2").arg(info->sessionId, command);
    emit commandReceived(info->sessionId, command, params);
}

/**
 * @brief 处理历史数据查询请求
 *
 * 目前返回空数据，实际应用中应连接到数据库管理器
 *
 * @param socket 消息来源Socket
 * @param data 查询参数（包含data_type, start_time, end_time）
 */
void WebSocketServer::handleQueryHistory(QWebSocket* socket, const QJsonObject& data)
{
    ClientInfo* info = findClientBySocket(socket);
    if (!info) {
        sendTo(socket, RemoteProtocol::createError("未认证的连接", 401));
        return;
    }

    QString dataType = data["data_type"].toString();
    QDateTime startTime = QDateTime::fromString(data["start_time"].toString(), Qt::ISODate);
    QDateTime endTime = QDateTime::fromString(data["end_time"].toString(), Qt::ISODate);

    qInfo() << QString("客户端 %1 查询历史数据: %2 (%3 - %4)")
               .arg(info->sessionId, dataType, startTime.toString(), endTime.toString());

    // TODO: 连接数据库管理器查询历史数据
    // 目前返回空数据
    QJsonObject response = RemoteProtocol::createHistoryData(QJsonArray(), 0);
    sendTo(socket, response);
}

/**
 * @brief 处理心跳请求
 *
 * 更新客户端的最后心跳时间，并发送Pong响应
 *
 * @param socket 消息来源Socket
 */
void WebSocketServer::handlePing(QWebSocket* socket)
{
    ClientInfo* info = findClientBySocket(socket);
    if (info) {
        // 更新心跳时间和重置失败计数
        info->lastPingTime = QDateTime::currentMSecsSinceEpoch();
        info->pingFailCount = 0;
    }

    // 发送Pong响应
    sendPong(socket);
}

// ==================== 客户端注册/注销 ====================

/**
 * @brief 注册新客户端
 *
 * 创建客户端信息记录并分配会话ID
 *
 * @param socket 客户端Socket
 * @param clientId 客户端提供的ID
 * @return 分配的会话ID
 */
QString WebSocketServer::registerClient(QWebSocket* socket, const QString& clientId)
{
    QMutexLocker locker(&mutex_);

    // 生成唯一的会话ID
    QString sessionId = generateSessionId();

    // 创建客户端信息
    ClientInfo info;
    info.clientId = clientId;
    info.sessionId = sessionId;
    info.socket = socket;
    info.connectTime = QDateTime::currentDateTime();
    info.lastPingTime = QDateTime::currentMSecsSinceEpoch();
    info.pingFailCount = 0;

    // 添加到映射表
    clients_[sessionId] = info;
    socketToSession_[socket] = sessionId;

    return sessionId;
}

/**
 * @brief 注销客户端
 *
 * 从映射表中移除客户端信息，并发送断开信号
 *
 * @param socket 客户端Socket
 */
void WebSocketServer::unregisterClient(QWebSocket* socket)
{
    QMutexLocker locker(&mutex_);

    if (!socketToSession_.contains(socket)) {
        return;
    }

    QString sessionId = socketToSession_[socket];
    QString clientId = clients_[sessionId].clientId;

    // 从映射表移除
    clients_.remove(sessionId);
    socketToSession_.remove(socket);

    qInfo() << QString("客户端断开: %1 (session: %2)").arg(clientId, sessionId);
    emit clientDisconnected(sessionId, "客户端断开连接");
}

// ==================== 辅助方法 ====================

/**
 * @brief 通过Socket查找客户端信息
 * @param socket 目标Socket
 * @return 客户端信息指针，不存在返回nullptr
 */
ClientInfo* WebSocketServer::findClientBySocket(QWebSocket* socket)
{
    if (!socketToSession_.contains(socket)) {
        return nullptr;
    }

    QString sessionId = socketToSession_[socket];
    if (!clients_.contains(sessionId)) {
        return nullptr;
    }

    return &clients_[sessionId];
}

/**
 * @brief 通过会话ID查找客户端信息
 * @param sessionId 目标会话ID
 * @return 客户端信息指针，不存在返回nullptr
 */
ClientInfo* WebSocketServer::findClientBySessionId(const QString& sessionId)
{
    if (!clients_.contains(sessionId)) {
        return nullptr;
    }

    return &clients_[sessionId];
}

/**
 * @brief 生成唯一的会话ID
 *
 * 使用UUID生成不带花括号的唯一标识符
 *
 * @return UUID格式的会话ID
 */
QString WebSocketServer::generateSessionId() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

/**
 * @brief 发送Pong心跳响应
 * @param socket 目标Socket
 */
void WebSocketServer::sendPong(QWebSocket* socket)
{
    sendTo(socket, RemoteProtocol::createPong());
}

// ==================== 心跳检测 ====================

/**
 * @brief 心跳定时器超时处理
 *
 * 定期调用以检查所有客户端的心跳状态
 */
void WebSocketServer::onHeartbeatTimeout()
{
    checkHeartbeats();
}

/**
 * @brief 检查所有客户端的心跳状态
 *
 * 心跳检测逻辑:
 * 1. 遍历所有客户端
 * 2. 检查最后心跳时间是否超时
 * 3. 超时则增加失败计数
 * 4. 失败次数超过阈值则断开连接
 */
void WebSocketServer::checkHeartbeats()
{
    QMutexLocker locker(&mutex_);

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    QList<QString> toDisconnect;

    for (auto& info : clients_) {
        qint64 elapsed = currentTime - info.lastPingTime;

        // 检查是否超时
        if (elapsed > config_.heartbeatTimeoutMs) {
            info.pingFailCount++;

            // 超过最大失败次数则断开
            if (info.pingFailCount >= config_.maxPingFailCount) {
                qWarning() << QString("客户端 %1 心跳超时，准备断开").arg(info.sessionId);
                toDisconnect.append(info.sessionId);
            }
        }
    }

    // 断开超时的客户端
    for (const QString& sessionId : toDisconnect) {
        if (clients_.contains(sessionId) && clients_[sessionId].socket) {
            clients_[sessionId].socket->close(QWebSocketProtocol::CloseCodeGoingAway, "心跳超时");
        }
    }
}

} // namespace Remote
} // namespace VisionForge
