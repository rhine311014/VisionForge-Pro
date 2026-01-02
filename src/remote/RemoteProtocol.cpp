/**
 * @file RemoteProtocol.cpp
 * @brief 远程诊断系统消息协议实现
 * @details 实现消息创建、解析和类型转换的具体逻辑
 *
 * @section implementation_notes 实现说明
 * - 所有消息使用JSON格式进行序列化
 * - 时间戳统一使用ISO8601格式
 * - 消息类型和订阅类型使用字符串形式传输，便于调试和扩展
 *
 * @author VisionForge Team
 * @version 1.6.0
 * @date 2025-12-20
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 */

#include "remote/RemoteProtocol.h"
#include <QJsonDocument>

namespace VisionForge {
namespace Remote {

// ==================== 消息创建和解析 ====================

/**
 * @brief 创建标准格式的协议消息
 *
 * 消息结构说明:
 * - type: 消息类型的字符串标识
 * - timestamp: ISO8601格式的消息创建时间
 * - data: 消息携带的具体数据（可选）
 *
 * @param type 消息类型枚举
 * @param data 消息数据，空对象时不包含在消息中
 * @return 完整的JSON消息对象
 */
QJsonObject RemoteProtocol::createMessage(MessageType type, const QJsonObject& data)
{
    QJsonObject message;

    // 设置消息类型
    message["type"] = messageTypeToString(type);

    // 添加时间戳，使用ISO8601标准格式
    message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // 仅在数据非空时添加data字段
    if (!data.isEmpty()) {
        message["data"] = data;
    }

    return message;
}

/**
 * @brief 解析JSON格式的消息字符串
 *
 * 解析流程:
 * 1. 将JSON字符串转换为QJsonDocument
 * 2. 验证是否为有效的JSON对象
 * 3. 提取type字段并转换为枚举
 * 4. 提取data字段（如果存在）
 *
 * @param jsonString 待解析的JSON字符串
 * @param[out] type 解析出的消息类型
 * @param[out] data 解析出的消息数据
 * @return 解析成功返回true，失败返回false
 */
bool RemoteProtocol::parseMessage(const QString& jsonString, MessageType& type, QJsonObject& data)
{
    // 解析JSON字符串
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());

    // 验证是否为有效的JSON对象
    if (!doc.isObject()) {
        return false;
    }

    QJsonObject message = doc.object();

    // 验证必要字段是否存在
    if (!message.contains("type")) {
        return false;
    }

    // 提取消息类型
    type = stringToMessageType(message["type"].toString());

    // 提取消息数据（可选字段）
    data = message["data"].toObject();

    return true;
}

// ==================== 握手消息 ====================

/**
 * @brief 创建客户端握手消息
 *
 * 握手流程:
 * 1. 客户端连接WebSocket后立即发送此消息
 * 2. 包含客户端唯一标识和版本信息
 * 3. 服务器据此进行认证和版本兼容性检查
 *
 * @param clientId 客户端唯一标识（如设备ID或UUID）
 * @param version 客户端软件版本号
 * @return ClientHello类型的消息对象
 */
QJsonObject RemoteProtocol::createClientHello(const QString& clientId, const QString& version)
{
    QJsonObject data;
    data["client_id"] = clientId;   // 客户端唯一标识
    data["version"] = version;       // 客户端版本号
    return createMessage(MessageType::ClientHello, data);
}

/**
 * @brief 创建服务器握手响应消息
 *
 * 响应内容:
 * - success: 握手是否成功
 * - session_id: 分配的会话标识（用于后续通信）
 * - server_version: 服务器版本号
 *
 * @param success 握手是否成功
 * @param sessionId 服务器分配的会话ID
 * @return ServerHello类型的消息对象
 */
QJsonObject RemoteProtocol::createServerHello(bool success, const QString& sessionId)
{
    QJsonObject data;
    data["success"] = success;              // 握手结果
    data["session_id"] = sessionId;         // 会话标识
    data["server_version"] = "1.6.0";       // 服务器版本
    return createMessage(MessageType::ServerHello, data);
}

// ==================== 订阅消息 ====================

/**
 * @brief 创建数据订阅请求消息
 *
 * 订阅机制说明:
 * - 客户端可订阅一种或多种数据类型
 * - 订阅All将接收所有类型的数据推送
 * - 重复订阅同一类型会被忽略
 *
 * @param type 要订阅的数据类型
 * @return Subscribe类型的消息对象
 */
QJsonObject RemoteProtocol::createSubscribe(SubscriptionType type)
{
    QJsonObject data;
    data["subscription_type"] = subscriptionTypeToString(type);
    return createMessage(MessageType::Subscribe, data);
}

/**
 * @brief 创建取消订阅消息
 *
 * @param type 要取消订阅的数据类型
 * @return Unsubscribe类型的消息对象
 */
QJsonObject RemoteProtocol::createUnsubscribe(SubscriptionType type)
{
    QJsonObject data;
    data["subscription_type"] = subscriptionTypeToString(type);
    return createMessage(MessageType::Unsubscribe, data);
}

// ==================== 数据推送消息 ====================

/**
 * @brief 创建性能指标更新消息
 *
 * 推送频率: 建议每秒推送一次
 * 数据内容: CPU使用率、内存占用、帧率、延迟等
 *
 * @param metrics 性能指标数据结构
 * @return PerformanceUpdate类型的消息对象
 */
QJsonObject RemoteProtocol::createPerformanceUpdate(const PerformanceMetrics& metrics)
{
    return createMessage(MessageType::PerformanceUpdate, metrics.toJson());
}

/**
 * @brief 创建日志条目推送消息
 *
 * 推送时机: 新日志产生时实时推送
 * 数据内容: 时间戳、级别、分类、消息内容等
 *
 * @param log 日志条目数据结构
 * @return LogEntry类型的消息对象
 */
QJsonObject RemoteProtocol::createLogEntry(const LogEntryData& log)
{
    return createMessage(MessageType::LogEntry, log.toJson());
}

/**
 * @brief 创建系统状态更新消息
 *
 * 推送时机: 系统状态发生变化时
 * 数据内容: 运行状态、运行时长、版本信息等
 *
 * @param status 系统状态数据结构
 * @return SystemStatus类型的消息对象
 */
QJsonObject RemoteProtocol::createSystemStatus(const SystemStatusData& status)
{
    return createMessage(MessageType::SystemStatus, status.toJson());
}

// ==================== 命令消息 ====================

/**
 * @brief 创建控制命令消息
 *
 * 权限要求: 执行命令需要相应的用户权限
 * 支持的命令:
 * - start: 启动服务
 * - stop: 停止服务
 * - restart: 重启服务
 * - clear_logs: 清除日志
 * - export_data: 导出数据
 *
 * @param command 命令名称
 * @param params 命令参数（可选）
 * @return SendCommand类型的消息对象
 */
QJsonObject RemoteProtocol::createCommand(const QString& command, const QJsonObject& params)
{
    QJsonObject data;
    data["command"] = command;      // 命令名称
    data["params"] = params;        // 命令参数
    return createMessage(MessageType::SendCommand, data);
}

/**
 * @brief 创建命令执行结果消息
 *
 * 结果内容:
 * - success: 命令是否执行成功
 * - message: 结果描述或错误信息
 * - result: 命令返回的数据（可选）
 *
 * @param success 执行是否成功
 * @param message 结果描述信息
 * @param result 返回数据（可选）
 * @return CommandResult类型的消息对象
 */
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

// ==================== 查询消息 ====================

/**
 * @brief 创建历史数据查询消息
 *
 * 查询参数:
 * - data_type: 数据类型 ("performance" / "logs")
 * - start_time: 查询起始时间 (ISO8601格式)
 * - end_time: 查询结束时间 (ISO8601格式)
 *
 * @param dataType 数据类型
 * @param start 起始时间
 * @param end 结束时间
 * @return QueryHistory类型的消息对象
 */
QJsonObject RemoteProtocol::createQueryHistory(const QString& dataType, const QDateTime& start, const QDateTime& end)
{
    QJsonObject data;
    data["data_type"] = dataType;
    data["start_time"] = start.toString(Qt::ISODate);
    data["end_time"] = end.toString(Qt::ISODate);
    return createMessage(MessageType::QueryHistory, data);
}

/**
 * @brief 创建历史数据响应消息
 *
 * 响应内容:
 * - data: 查询结果数组
 * - total: 总记录数（用于分页）
 *
 * @param data 查询结果数组
 * @param total 总记录数
 * @return HistoryData类型的消息对象
 */
QJsonObject RemoteProtocol::createHistoryData(const QJsonArray& data, int total)
{
    QJsonObject responseData;
    responseData["data"] = data;
    responseData["total"] = total;
    return createMessage(MessageType::HistoryData, responseData);
}

// ==================== 心跳消息 ====================

/**
 * @brief 创建心跳请求消息（Ping）
 *
 * 心跳机制说明:
 * - 客户端定期发送Ping以维持连接
 * - 建议间隔: 10-30秒
 * - 服务器收到后应立即响应Pong
 * - 多次无响应可判定连接断开
 *
 * @return Ping类型的消息对象
 */
QJsonObject RemoteProtocol::createPing()
{
    return createMessage(MessageType::Ping);
}

/**
 * @brief 创建心跳响应消息（Pong）
 *
 * @return Pong类型的消息对象
 */
QJsonObject RemoteProtocol::createPong()
{
    return createMessage(MessageType::Pong);
}

// ==================== 错误消息 ====================

/**
 * @brief 创建错误通知消息
 *
 * 错误码规范:
 * - 0: 通用错误
 * - 400: 请求格式错误
 * - 401: 未认证/认证失败
 * - 403: 权限不足
 * - 404: 资源不存在
 * - 500: 服务器内部错误
 *
 * @param errorMessage 错误描述
 * @param errorCode 错误码
 * @return Error类型的消息对象
 */
QJsonObject RemoteProtocol::createError(const QString& errorMessage, int errorCode)
{
    QJsonObject data;
    data["error_message"] = errorMessage;
    data["error_code"] = errorCode;
    return createMessage(MessageType::Error, data);
}

// ==================== 类型转换 - MessageType ====================

/**
 * @brief 将消息类型枚举转换为字符串
 *
 * 字符串格式采用snake_case命名规范，便于跨平台解析
 *
 * @param type 消息类型枚举值
 * @return 消息类型的字符串表示
 */
QString RemoteProtocol::messageTypeToString(MessageType type)
{
    switch (type) {
        // 客户端 -> 服务器
        case MessageType::ClientHello:          return "client_hello";
        case MessageType::Subscribe:            return "subscribe";
        case MessageType::Unsubscribe:          return "unsubscribe";
        case MessageType::QueryHistory:         return "query_history";
        case MessageType::SendCommand:          return "send_command";
        case MessageType::Ping:                 return "ping";

        // 服务器 -> 客户端
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

/**
 * @brief 将字符串转换为消息类型枚举
 *
 * @param typeStr 消息类型字符串
 * @return 对应的消息类型枚举值，无法识别时返回Error
 */
MessageType RemoteProtocol::stringToMessageType(const QString& typeStr)
{
    // 客户端 -> 服务器
    if (typeStr == "client_hello")          return MessageType::ClientHello;
    if (typeStr == "subscribe")             return MessageType::Subscribe;
    if (typeStr == "unsubscribe")           return MessageType::Unsubscribe;
    if (typeStr == "query_history")         return MessageType::QueryHistory;
    if (typeStr == "send_command")          return MessageType::SendCommand;
    if (typeStr == "ping")                  return MessageType::Ping;

    // 服务器 -> 客户端
    if (typeStr == "server_hello")          return MessageType::ServerHello;
    if (typeStr == "performance_update")    return MessageType::PerformanceUpdate;
    if (typeStr == "log_entry")             return MessageType::LogEntry;
    if (typeStr == "system_status")         return MessageType::SystemStatus;
    if (typeStr == "history_data")          return MessageType::HistoryData;
    if (typeStr == "command_result")        return MessageType::CommandResult;
    if (typeStr == "pong")                  return MessageType::Pong;
    if (typeStr == "error")                 return MessageType::Error;

    // 无法识别的类型默认返回Error
    return MessageType::Error;
}

// ==================== 类型转换 - SubscriptionType ====================

/**
 * @brief 将订阅类型枚举转换为字符串
 *
 * @param type 订阅类型枚举值
 * @return 订阅类型的字符串表示
 */
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

/**
 * @brief 将字符串转换为订阅类型枚举
 *
 * @param typeStr 订阅类型字符串
 * @return 对应的订阅类型枚举值，无法识别时返回All
 */
SubscriptionType RemoteProtocol::stringToSubscriptionType(const QString& typeStr)
{
    if (typeStr == "performance")       return SubscriptionType::Performance;
    if (typeStr == "logs")              return SubscriptionType::Logs;
    if (typeStr == "system_status")     return SubscriptionType::SystemStatus;
    if (typeStr == "all")               return SubscriptionType::All;

    // 无法识别时默认订阅全部
    return SubscriptionType::All;
}

// ==================== 类型转换 - LogLevel ====================

/**
 * @brief 将日志级别枚举转换为字符串
 *
 * 返回大写格式的级别名称，符合通用日志规范
 *
 * @param level 日志级别枚举值
 * @return 日志级别的字符串表示（大写）
 */
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

/**
 * @brief 将字符串转换为日志级别枚举
 *
 * 支持大小写不敏感的匹配
 *
 * @param levelStr 日志级别字符串
 * @return 对应的日志级别枚举值，无法识别时返回Info
 */
LogLevel RemoteProtocol::stringToLogLevel(const QString& levelStr)
{
    // 转换为大写以支持大小写不敏感匹配
    QString upper = levelStr.toUpper();

    if (upper == "TRACE")   return LogLevel::Trace;
    if (upper == "DEBUG")   return LogLevel::Debug;
    if (upper == "INFO")    return LogLevel::Info;
    if (upper == "WARN")    return LogLevel::Warn;
    if (upper == "ERROR")   return LogLevel::Error;
    if (upper == "FATAL")   return LogLevel::Fatal;

    // 无法识别时默认为Info级别
    return LogLevel::Info;
}

} // namespace Remote
} // namespace VisionForge
