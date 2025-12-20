/**
 * @file RemoteProtocol.h
 * @brief 远程诊断系统消息协议定义
 * @details 定义WebSocket通信的消息格式、类型和辅助函数
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QDateTime>

namespace VisionForge {
namespace Remote {

/**
 * @brief 消息类型枚举
 */
enum class MessageType {
    // ========== 客户端 → 服务器 ==========
    ClientHello,            // 客户端连接握手
    Subscribe,              // 订阅数据推送
    Unsubscribe,            // 取消订阅
    QueryHistory,           // 查询历史数据
    SendCommand,            // 发送控制命令
    Ping,                   // 心跳检测

    // ========== 服务器 → 客户端 ==========
    ServerHello,            // 服务器响应握手
    PerformanceUpdate,      // 性能指标更新
    LogEntry,               // 日志条目推送
    SystemStatus,           // 系统状态更新
    HistoryData,            // 历史数据响应
    CommandResult,          // 命令执行结果
    Pong,                   // 心跳响应
    Error                   // 错误消息
};

/**
 * @brief 订阅数据类型
 */
enum class SubscriptionType {
    Performance,    // 性能指标
    Logs,           // 日志流
    SystemStatus,   // 系统状态
    All             // 全部数据
};

/**
 * @brief 日志级别
 */
enum class LogLevel {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Fatal = 5
};

/**
 * @brief 性能指标数据
 */
struct PerformanceMetrics {
    double cpuUsage = 0.0;          // CPU使用率 (%)
    qint64 memoryUsedMB = 0;        // 内存使用 (MB)
    double fps = 0.0;               // 处理帧率
    double avgLatencyMs = 0.0;      // 平均延迟 (ms)
    int errorCount = 0;             // 错误计数
    QDateTime timestamp;            // 时间戳

    QJsonObject toJson() const;
    static PerformanceMetrics fromJson(const QJsonObject& json);
};

/**
 * @brief 日志条目数据
 */
struct LogEntryData {
    QDateTime timestamp;
    LogLevel level = LogLevel::Info;
    QString category;
    QString message;
    QString file;
    int line = 0;

    QJsonObject toJson() const;
    static LogEntryData fromJson(const QJsonObject& json);
};

/**
 * @brief 系统状态数据
 */
struct SystemStatusData {
    QString status;                 // 运行状态 (running/stopped/error)
    qint64 uptimeSeconds = 0;       // 运行时长 (秒)
    QString version;                // 版本号
    qint64 totalProcessed = 0;      // 总处理数量
    QDateTime timestamp;

    QJsonObject toJson() const;
    static SystemStatusData fromJson(const QJsonObject& json);
};

/**
 * @brief 消息协议类
 */
class RemoteProtocol
{
public:
    /**
     * @brief 创建消息
     */
    static QJsonObject createMessage(MessageType type, const QJsonObject& data = QJsonObject());

    /**
     * @brief 解析消息
     */
    static bool parseMessage(const QString& jsonString, MessageType& type, QJsonObject& data);

    /**
     * @brief 创建握手消息
     */
    static QJsonObject createClientHello(const QString& clientId, const QString& version);
    static QJsonObject createServerHello(bool success, const QString& sessionId);

    /**
     * @brief 创建订阅消息
     */
    static QJsonObject createSubscribe(SubscriptionType type);
    static QJsonObject createUnsubscribe(SubscriptionType type);

    /**
     * @brief 创建数据推送消息
     */
    static QJsonObject createPerformanceUpdate(const PerformanceMetrics& metrics);
    static QJsonObject createLogEntry(const LogEntryData& log);
    static QJsonObject createSystemStatus(const SystemStatusData& status);

    /**
     * @brief 创建命令消息
     */
    static QJsonObject createCommand(const QString& command, const QJsonObject& params = QJsonObject());
    static QJsonObject createCommandResult(bool success, const QString& message, const QJsonObject& result = QJsonObject());

    /**
     * @brief 创建查询消息
     */
    static QJsonObject createQueryHistory(const QString& dataType, const QDateTime& start, const QDateTime& end);
    static QJsonObject createHistoryData(const QJsonArray& data, int total);

    /**
     * @brief 创建心跳消息
     */
    static QJsonObject createPing();
    static QJsonObject createPong();

    /**
     * @brief 创建错误消息
     */
    static QJsonObject createError(const QString& errorMessage, int errorCode = 0);

    /**
     * @brief 消息类型转换
     */
    static QString messageTypeToString(MessageType type);
    static MessageType stringToMessageType(const QString& typeStr);

    /**
     * @brief 订阅类型转换
     */
    static QString subscriptionTypeToString(SubscriptionType type);
    static SubscriptionType stringToSubscriptionType(const QString& typeStr);

    /**
     * @brief 日志级别转换
     */
    static QString logLevelToString(LogLevel level);
    static LogLevel stringToLogLevel(const QString& levelStr);
};

// ========== Inline实现 ==========

inline QJsonObject PerformanceMetrics::toJson() const
{
    QJsonObject json;
    json["cpu_usage"] = cpuUsage;
    json["memory_used_mb"] = memoryUsedMB;
    json["fps"] = fps;
    json["avg_latency_ms"] = avgLatencyMs;
    json["error_count"] = errorCount;
    json["timestamp"] = timestamp.toString(Qt::ISODate);
    return json;
}

inline PerformanceMetrics PerformanceMetrics::fromJson(const QJsonObject& json)
{
    PerformanceMetrics metrics;
    metrics.cpuUsage = json["cpu_usage"].toDouble();
    metrics.memoryUsedMB = json["memory_used_mb"].toInteger();
    metrics.fps = json["fps"].toDouble();
    metrics.avgLatencyMs = json["avg_latency_ms"].toDouble();
    metrics.errorCount = json["error_count"].toInt();
    metrics.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    return metrics;
}

inline QJsonObject LogEntryData::toJson() const
{
    QJsonObject json;
    json["timestamp"] = timestamp.toString(Qt::ISODate);
    json["level"] = RemoteProtocol::logLevelToString(level);
    json["category"] = category;
    json["message"] = message;
    json["file"] = file;
    json["line"] = line;
    return json;
}

inline LogEntryData LogEntryData::fromJson(const QJsonObject& json)
{
    LogEntryData log;
    log.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    log.level = RemoteProtocol::stringToLogLevel(json["level"].toString());
    log.category = json["category"].toString();
    log.message = json["message"].toString();
    log.file = json["file"].toString();
    log.line = json["line"].toInt();
    return log;
}

inline QJsonObject SystemStatusData::toJson() const
{
    QJsonObject json;
    json["status"] = status;
    json["uptime_seconds"] = uptimeSeconds;
    json["version"] = version;
    json["total_processed"] = totalProcessed;
    json["timestamp"] = timestamp.toString(Qt::ISODate);
    return json;
}

inline SystemStatusData SystemStatusData::fromJson(const QJsonObject& json)
{
    SystemStatusData data;
    data.status = json["status"].toString();
    data.uptimeSeconds = json["uptime_seconds"].toInteger();
    data.version = json["version"].toString();
    data.totalProcessed = json["total_processed"].toInteger();
    data.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    return data;
}

} // namespace Remote
} // namespace VisionForge
