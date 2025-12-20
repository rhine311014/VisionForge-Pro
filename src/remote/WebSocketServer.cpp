/**
 * @file WebSocketServer.cpp
 * @brief WebSocket服务器实现
 */

#include "remote/WebSocketServer.h"
#include <QJsonDocument>
#include <QUuid>
#include <QDebug>
#include <QSslSocket>

namespace VisionForge {
namespace Remote {

// ========== 构造和析构 ==========

WebSocketServer::WebSocketServer(QObject* parent)
    : QObject(parent)
    , server_(nullptr)
    , heartbeatTimer_(nullptr)
    , sslConfigured_(false)
{
}

WebSocketServer::~WebSocketServer()
{
    stop();
}

// ========== 服务器控制 ==========

void WebSocketServer::setSslConfiguration(const QSslConfiguration& config)
{
    sslConfig_ = config;
    sslConfigured_ = true;
    qInfo() << "SSL配置已设置";
}

bool WebSocketServer::start(const ServerConfig& config)
{
    if (isRunning()) {
        qWarning() << "WebSocket服务器已经在运行";
        return false;
    }

    config_ = config;

    // 创建WebSocket服务器
    server_ = new QWebSocketServer(
        QStringLiteral("VisionForge Remote Diagnostics"),
        config_.enableSsl ? QWebSocketServer::SecureMode : QWebSocketServer::NonSecureMode,
        this
    );

    // 如果启用SSL，配置SSL
    if (config_.enableSsl) {
        if (!QSslSocket::supportsSsl()) {
            QString error = "系统不支持SSL";
            qCritical() << error;
            emit serverError(error);
            delete server_;
            server_ = nullptr;
            return false;
        }

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

    // 连接信号
    connect(server_, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    connect(server_, &QWebSocketServer::serverError, this, [this](QWebSocketProtocol::CloseCode closeCode) {
        Q_UNUSED(closeCode);
        emit serverError(server_->errorString());
    });

    // 启动监听
    if (!server_->listen(QHostAddress::Any, config_.port)) {
        QString error = QString("无法启动WebSocket服务器: %1").arg(server_->errorString());
        qCritical() << error;
        emit serverError(error);
        delete server_;
        server_ = nullptr;
        return false;
    }

    // 启动心跳定时器
    heartbeatTimer_ = new QTimer(this);
    connect(heartbeatTimer_, &QTimer::timeout, this, &WebSocketServer::onHeartbeatTimeout);
    heartbeatTimer_->start(config_.heartbeatIntervalMs);

    qInfo() << "WebSocket服务器启动成功，监听端口:" << config_.port;
    emit serverStarted(config_.port);

    return true;
}

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

// ========== 客户端管理 ==========

int WebSocketServer::clientCount() const
{
    QMutexLocker locker(&mutex_);
    return clients_.size();
}

QList<ClientInfo> WebSocketServer::clients() const
{
    QMutexLocker locker(&mutex_);
    return clients_.values();
}

ClientInfo WebSocketServer::clientInfo(const QString& sessionId) const
{
    QMutexLocker locker(&mutex_);
    return clients_.value(sessionId);
}

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

void WebSocketServer::disconnectAll(const QString& reason)
{
    QMutexLocker locker(&mutex_);

    for (auto& info : clients_) {
        if (info.socket) {
            info.socket->close(QWebSocketProtocol::CloseCodeNormal, reason);
        }
    }
}

// ========== 消息发送 ==========

void WebSocketServer::broadcast(const QJsonObject& message)
{
    QMutexLocker locker(&mutex_);

    QString jsonString = QJsonDocument(message).toJson(QJsonDocument::Compact);

    for (const auto& info : clients_) {
        if (info.socket && info.socket->isValid()) {
            info.socket->sendTextMessage(jsonString);
        }
    }
}

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

void WebSocketServer::sendTo(QWebSocket* socket, const QJsonObject& message)
{
    if (socket && socket->isValid()) {
        QString jsonString = QJsonDocument(message).toJson(QJsonDocument::Compact);
        socket->sendTextMessage(jsonString);
    }
}

// ========== 便捷推送方法 ==========

void WebSocketServer::pushPerformanceMetrics(const PerformanceMetrics& metrics)
{
    QJsonObject message = RemoteProtocol::createPerformanceUpdate(metrics);
    broadcastToSubscribers(SubscriptionType::Performance, message);
}

void WebSocketServer::pushLogEntry(const LogEntryData& log)
{
    QJsonObject message = RemoteProtocol::createLogEntry(log);
    broadcastToSubscribers(SubscriptionType::Logs, message);
}

void WebSocketServer::pushSystemStatus(const SystemStatusData& status)
{
    QJsonObject message = RemoteProtocol::createSystemStatus(status);
    broadcastToSubscribers(SubscriptionType::SystemStatus, message);
}

// ========== 连接管理 ==========

void WebSocketServer::onNewConnection()
{
    QWebSocket* socket = server_->nextPendingConnection();

    if (!socket) {
        return;
    }

    // 检查最大连接数
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

void WebSocketServer::onSocketDisconnected()
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    if (!socket) {
        return;
    }

    unregisterClient(socket);
    socket->deleteLater();
}

void WebSocketServer::onSocketError(QAbstractSocket::SocketError error)
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    if (!socket) {
        return;
    }

    qWarning() << "Socket错误:" << error << socket->errorString();
}

// ========== 消息处理 ==========

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

    // 处理消息
    handleClientMessage(socket, type, data);
}

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

// ========== 具体消息处理 ==========

void WebSocketServer::handleClientHello(QWebSocket* socket, const QJsonObject& data)
{
    QString clientId = data["client_id"].toString();
    QString version = data["version"].toString();

    if (clientId.isEmpty()) {
        sendTo(socket, RemoteProtocol::createError("缺少client_id", 400));
        return;
    }

    // 注册客户端
    QString sessionId = registerClient(socket, clientId);

    // 发送响应
    QJsonObject response = RemoteProtocol::createServerHello(true, sessionId);
    sendTo(socket, response);

    qInfo() << QString("客户端握手成功: %1 (session: %2)").arg(clientId, sessionId);
    emit clientConnected(sessionId, clientId);
}

void WebSocketServer::handleSubscribe(QWebSocket* socket, const QJsonObject& data)
{
    ClientInfo* info = findClientBySocket(socket);
    if (!info) {
        sendTo(socket, RemoteProtocol::createError("未认证的连接", 401));
        return;
    }

    QString typeStr = data["subscription_type"].toString();
    SubscriptionType type = RemoteProtocol::stringToSubscriptionType(typeStr);

    info->subscriptions.insert(type);

    qInfo() << QString("客户端 %1 订阅了 %2").arg(info->sessionId, typeStr);
    emit clientSubscribed(info->sessionId, type);
}

void WebSocketServer::handleUnsubscribe(QWebSocket* socket, const QJsonObject& data)
{
    ClientInfo* info = findClientBySocket(socket);
    if (!info) {
        return;
    }

    QString typeStr = data["subscription_type"].toString();
    SubscriptionType type = RemoteProtocol::stringToSubscriptionType(typeStr);

    info->subscriptions.remove(type);

    qInfo() << QString("客户端 %1 取消订阅 %2").arg(info->sessionId, typeStr);
    emit clientUnsubscribed(info->sessionId, type);
}

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

    // 这里应该查询数据库，返回历史数据
    // 暂时返回空数据
    QJsonObject response = RemoteProtocol::createHistoryData(QJsonArray(), 0);
    sendTo(socket, response);
}

void WebSocketServer::handlePing(QWebSocket* socket)
{
    ClientInfo* info = findClientBySocket(socket);
    if (info) {
        info->lastPingTime = QDateTime::currentMSecsSinceEpoch();
        info->pingFailCount = 0;
    }

    sendPong(socket);
}

// ========== 客户端注册/注销 ==========

QString WebSocketServer::registerClient(QWebSocket* socket, const QString& clientId)
{
    QMutexLocker locker(&mutex_);

    QString sessionId = generateSessionId();

    ClientInfo info;
    info.clientId = clientId;
    info.sessionId = sessionId;
    info.socket = socket;
    info.connectTime = QDateTime::currentDateTime();
    info.lastPingTime = QDateTime::currentMSecsSinceEpoch();
    info.pingFailCount = 0;

    clients_[sessionId] = info;
    socketToSession_[socket] = sessionId;

    return sessionId;
}

void WebSocketServer::unregisterClient(QWebSocket* socket)
{
    QMutexLocker locker(&mutex_);

    if (!socketToSession_.contains(socket)) {
        return;
    }

    QString sessionId = socketToSession_[socket];
    QString clientId = clients_[sessionId].clientId;

    clients_.remove(sessionId);
    socketToSession_.remove(socket);

    qInfo() << QString("客户端断开: %1 (session: %2)").arg(clientId, sessionId);
    emit clientDisconnected(sessionId, "客户端断开连接");
}

// ========== 辅助方法 ==========

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

ClientInfo* WebSocketServer::findClientBySessionId(const QString& sessionId)
{
    if (!clients_.contains(sessionId)) {
        return nullptr;
    }

    return &clients_[sessionId];
}

QString WebSocketServer::generateSessionId() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void WebSocketServer::sendPong(QWebSocket* socket)
{
    sendTo(socket, RemoteProtocol::createPong());
}

// ========== 心跳检测 ==========

void WebSocketServer::onHeartbeatTimeout()
{
    checkHeartbeats();
}

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
