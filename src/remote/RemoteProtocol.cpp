/**
 * @file RemoteProtocol.cpp
 * @brief 远程诊断系统消息协议实现
 */

#include "remote/RemoteProtocol.h"
#include <QJsonDocument>

namespace VisionForge {
namespace Remote {

// ========== 消息创建和解析 ==========

QJsonObject RemoteProtocol::createMessage(MessageType type, const QJsonObject& data)
{
    QJsonObject message;
    message["type"] = messageTypeToString(type);
    message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    if (!data.isEmpty()) {
        message["data"] = data;
    }

    return message;
}

bool RemoteProtocol::parseMessage(const QString& jsonString, MessageType& type, QJsonObject& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (!doc.isObject()) {
        return false;
    }

    QJsonObject message = doc.object();
    if (!message.contains("type")) {
        return false;
    }

    type = stringToMessageType(message["type"].toString());
    data = message["data"].toObject();

    return true;
}

// ========== 握手消息 ==========

QJsonObject RemoteProtocol::createClientHello(const QString& clientId, const QString& version)
{
    QJsonObject data;
    data["client_id"] = clientId;
    data["version"] = version;
    return createMessage(MessageType::ClientHello, data);
}

QJsonObject RemoteProtocol::createServerHello(bool success, const QString& sessionId)
{
    QJsonObject data;
    data["success"] = success;
    data["session_id"] = sessionId;
    data["server_version"] = "1.2.0";
    return createMessage(MessageType::ServerHello, data);
}

// ========== 订阅消息 ==========

QJsonObject RemoteProtocol::createSubscribe(SubscriptionType type)
{
    QJsonObject data;
    data["subscription_type"] = subscriptionTypeToString(type);
    return createMessage(MessageType::Subscribe, data);
}

QJsonObject RemoteProtocol::createUnsubscribe(SubscriptionType type)
{
    QJsonObject data;
    data["subscription_type"] = subscriptionTypeToString(type);
    return createMessage(MessageType::Unsubscribe, data);
}

// ========== 数据推送消息 ==========

QJsonObject RemoteProtocol::createPerformanceUpdate(const PerformanceMetrics& metrics)
{
    return createMessage(MessageType::PerformanceUpdate, metrics.toJson());
}

QJsonObject RemoteProtocol::createLogEntry(const LogEntryData& log)
{
    return createMessage(MessageType::LogEntry, log.toJson());
}

QJsonObject RemoteProtocol::createSystemStatus(const SystemStatusData& status)
{
    return createMessage(MessageType::SystemStatus, status.toJson());
}

// ========== 命令消息 ==========

QJsonObject RemoteProtocol::createCommand(const QString& command, const QJsonObject& params)
{
    QJsonObject data;
    data["command"] = command;
    data["params"] = params;
    return createMessage(MessageType::SendCommand, data);
}

QJsonObject RemoteProtocol::createCommandResult(bool success, const QString& message, const QJsonObject& result)
{
    QJsonObject data;
    data["success"] = success;
    data["message"] = message;
    if (!result.isEmpty()) {
        data["result"] = result;
    }
    return createMessage(MessageType::CommandResult, data);
}

// ========== 查询消息 ==========

QJsonObject RemoteProtocol::createQueryHistory(const QString& dataType, const QDateTime& start, const QDateTime& end)
{
    QJsonObject data;
    data["data_type"] = dataType;
    data["start_time"] = start.toString(Qt::ISODate);
    data["end_time"] = end.toString(Qt::ISODate);
    return createMessage(MessageType::QueryHistory, data);
}

QJsonObject RemoteProtocol::createHistoryData(const QJsonArray& data, int total)
{
    QJsonObject responseData;
    responseData["data"] = data;
    responseData["total"] = total;
    return createMessage(MessageType::HistoryData, responseData);
}

// ========== 心跳消息 ==========

QJsonObject RemoteProtocol::createPing()
{
    return createMessage(MessageType::Ping);
}

QJsonObject RemoteProtocol::createPong()
{
    return createMessage(MessageType::Pong);
}

// ========== 错误消息 ==========

QJsonObject RemoteProtocol::createError(const QString& errorMessage, int errorCode)
{
    QJsonObject data;
    data["error_message"] = errorMessage;
    data["error_code"] = errorCode;
    return createMessage(MessageType::Error, data);
}

// ========== 类型转换 - MessageType ==========

QString RemoteProtocol::messageTypeToString(MessageType type)
{
    switch (type) {
        case MessageType::ClientHello:          return "client_hello";
        case MessageType::Subscribe:            return "subscribe";
        case MessageType::Unsubscribe:          return "unsubscribe";
        case MessageType::QueryHistory:         return "query_history";
        case MessageType::SendCommand:          return "send_command";
        case MessageType::Ping:                 return "ping";
        case MessageType::ServerHello:          return "server_hello";
        case MessageType::PerformanceUpdate:    return "performance_update";
        case MessageType::LogEntry:             return "log_entry";
        case MessageType::SystemStatus:         return "system_status";
        case MessageType::HistoryData:          return "history_data";
        case MessageType::CommandResult:        return "command_result";
        case MessageType::Pong:                 return "pong";
        case MessageType::Error:                return "error";
        default:                                return "unknown";
    }
}

MessageType RemoteProtocol::stringToMessageType(const QString& typeStr)
{
    if (typeStr == "client_hello")          return MessageType::ClientHello;
    if (typeStr == "subscribe")             return MessageType::Subscribe;
    if (typeStr == "unsubscribe")           return MessageType::Unsubscribe;
    if (typeStr == "query_history")         return MessageType::QueryHistory;
    if (typeStr == "send_command")          return MessageType::SendCommand;
    if (typeStr == "ping")                  return MessageType::Ping;
    if (typeStr == "server_hello")          return MessageType::ServerHello;
    if (typeStr == "performance_update")    return MessageType::PerformanceUpdate;
    if (typeStr == "log_entry")             return MessageType::LogEntry;
    if (typeStr == "system_status")         return MessageType::SystemStatus;
    if (typeStr == "history_data")          return MessageType::HistoryData;
    if (typeStr == "command_result")        return MessageType::CommandResult;
    if (typeStr == "pong")                  return MessageType::Pong;
    if (typeStr == "error")                 return MessageType::Error;

    return MessageType::Error;
}

// ========== 类型转换 - SubscriptionType ==========

QString RemoteProtocol::subscriptionTypeToString(SubscriptionType type)
{
    switch (type) {
        case SubscriptionType::Performance:     return "performance";
        case SubscriptionType::Logs:            return "logs";
        case SubscriptionType::SystemStatus:    return "system_status";
        case SubscriptionType::All:             return "all";
        default:                                return "unknown";
    }
}

SubscriptionType RemoteProtocol::stringToSubscriptionType(const QString& typeStr)
{
    if (typeStr == "performance")       return SubscriptionType::Performance;
    if (typeStr == "logs")              return SubscriptionType::Logs;
    if (typeStr == "system_status")     return SubscriptionType::SystemStatus;
    if (typeStr == "all")               return SubscriptionType::All;

    return SubscriptionType::All;
}

// ========== 类型转换 - LogLevel ==========

QString RemoteProtocol::logLevelToString(LogLevel level)
{
    switch (level) {
        case LogLevel::Trace:   return "TRACE";
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warn:    return "WARN";
        case LogLevel::Error:   return "ERROR";
        case LogLevel::Fatal:   return "FATAL";
        default:                return "INFO";
    }
}

LogLevel RemoteProtocol::stringToLogLevel(const QString& levelStr)
{
    QString upper = levelStr.toUpper();
    if (upper == "TRACE")   return LogLevel::Trace;
    if (upper == "DEBUG")   return LogLevel::Debug;
    if (upper == "INFO")    return LogLevel::Info;
    if (upper == "WARN")    return LogLevel::Warn;
    if (upper == "ERROR")   return LogLevel::Error;
    if (upper == "FATAL")   return LogLevel::Fatal;

    return LogLevel::Info;
}

} // namespace Remote
} // namespace VisionForge
