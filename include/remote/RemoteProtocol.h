/**
 * @file RemoteProtocol.h
 * @brief 远程诊断系统消息协议定义
 * @details 定义WebSocket通信的消息格式、类型和辅助函数
 *
 * @section protocol_overview 协议概述
 * 本协议基于JSON格式，通过WebSocket进行双向通信。
 * 所有消息都包含type字段标识消息类型，data字段携带消息内容。
 *
 * @section message_format 消息格式
 * @code{.json}
 * {
 *     "type": "消息类型字符串",
 *     "timestamp": "ISO8601格式时间戳",
 *     "data": {
 *         // 消息具体内容
 *     }
 * }
 * @endcode
 *
 * @section communication_flow 通信流程
 * 1. 客户端发送ClientHello进行握手
 * 2. 服务器响应ServerHello确认连接
 * 3. 客户端发送Subscribe订阅感兴趣的数据类型
 * 4. 服务器推送订阅的数据更新
 * 5. 客户端可随时发送命令或查询
 * 6. 双方通过Ping/Pong维持心跳
 *
 * @section security_note 安全说明
 * - 建议使用WSS（WebSocket Secure）进行加密传输
 * - 所有敏感操作需要有效的认证Token
 * - 命令执行需要相应的权限验证
 *
 * @author VisionForge Team
 * @version 1.6.0
 * @date 2025-12-20
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
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
 * @details 定义所有支持的消息类型，分为客户端发送和服务器发送两类
 *
 * @note 消息类型命名规则：
 * - 客户端->服务器：动词形式（如Subscribe、Query）
 * - 服务器->客户端：名词形式（如Update、Data）
 */
enum class MessageType {
    // ==================== 客户端 → 服务器 ====================

    /**
     * @brief 客户端连接握手
     * @details 建立连接后客户端发送的第一条消息，包含客户端ID和版本信息
     * @see createClientHello()
     */
    ClientHello,

    /**
     * @brief 订阅数据推送
     * @details 客户端请求订阅指定类型的实时数据推送
     * @see SubscriptionType
     */
    Subscribe,

    /**
     * @brief 取消订阅
     * @details 取消之前订阅的数据推送
     */
    Unsubscribe,

    /**
     * @brief 查询历史数据
     * @details 请求指定时间范围内的历史数据
     */
    QueryHistory,

    /**
     * @brief 发送控制命令
     * @details 向服务器发送控制指令，需要相应权限
     */
    SendCommand,

    /**
     * @brief 心跳检测请求
     * @details 客户端定期发送以维持连接活性
     */
    Ping,

    // ==================== 服务器 → 客户端 ====================

    /**
     * @brief 服务器响应握手
     * @details 确认客户端连接，返回会话ID和服务器版本
     */
    ServerHello,

    /**
     * @brief 性能指标更新
     * @details 推送实时性能监控数据
     * @see PerformanceMetrics
     */
    PerformanceUpdate,

    /**
     * @brief 日志条目推送
     * @details 推送新产生的日志记录
     * @see LogEntryData
     */
    LogEntry,

    /**
     * @brief 系统状态更新
     * @details 推送系统运行状态变化
     * @see SystemStatusData
     */
    SystemStatus,

    /**
     * @brief 历史数据响应
     * @details 返回查询的历史数据结果
     */
    HistoryData,

    /**
     * @brief 命令执行结果
     * @details 返回命令执行的成功/失败状态和结果
     */
    CommandResult,

    /**
     * @brief 心跳响应
     * @details 响应客户端的Ping请求
     */
    Pong,

    /**
     * @brief 错误消息
     * @details 通知客户端发生的错误，包含错误码和描述
     */
    Error
};

/**
 * @brief 订阅数据类型枚举
 * @details 客户端可以选择订阅的数据推送类型
 *
 * 各类型推送频率：
 * - Performance: 每秒1次
 * - Logs: 实时推送（有新日志时）
 * - SystemStatus: 状态变化时推送
 */
enum class SubscriptionType {
    Performance,    ///< 性能指标：CPU、内存、帧率等
    Logs,           ///< 日志流：实时日志推送
    SystemStatus,   ///< 系统状态：运行状态、版本等
    All             ///< 全部数据：订阅以上所有类型
};

/**
 * @brief 日志级别枚举
 * @details 定义日志的严重程度等级，数值越大越严重
 *
 * 使用建议：
 * - Trace: 非常详细的调试信息，生产环境通常关闭
 * - Debug: 调试信息，用于开发阶段
 * - Info: 一般信息，正常运行状态
 * - Warn: 警告信息，需要关注但不影响运行
 * - Error: 错误信息，功能受损但系统可运行
 * - Fatal: 致命错误，系统可能无法继续运行
 */
enum class LogLevel {
    Trace = 0,  ///< 跟踪级别 - 最详细的调试信息
    Debug = 1,  ///< 调试级别 - 开发调试信息
    Info = 2,   ///< 信息级别 - 正常运行信息
    Warn = 3,   ///< 警告级别 - 潜在问题警告
    Error = 4,  ///< 错误级别 - 运行时错误
    Fatal = 5   ///< 致命级别 - 系统级严重错误
};

/**
 * @brief 性能指标数据结构
 * @details 包含系统和应用的关键性能指标
 *
 * 采集频率建议：每秒采集一次
 *
 * @note 所有数值均为采样时刻的瞬时值
 */
struct PerformanceMetrics {
    double cpuUsage = 0.0;          ///< CPU使用率 (0-100%)
    qint64 memoryUsedMB = 0;        ///< 已使用内存 (MB)
    double fps = 0.0;               ///< 处理帧率 (帧/秒)
    double avgLatencyMs = 0.0;      ///< 平均处理延迟 (毫秒)
    int errorCount = 0;             ///< 累计错误计数
    QDateTime timestamp;            ///< 采集时间戳

    /**
     * @brief 序列化为JSON对象
     * @return JSON格式的性能指标
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON反序列化
     * @param json JSON对象
     * @return 解析后的性能指标结构
     */
    static PerformanceMetrics fromJson(const QJsonObject& json);
};

/**
 * @brief 日志条目数据结构
 * @details 表示单条日志记录的完整信息
 */
struct LogEntryData {
    QDateTime timestamp;            ///< 日志产生时间
    LogLevel level = LogLevel::Info; ///< 日志级别
    QString category;               ///< 日志分类（如：Vision、Comm、UI）
    QString message;                ///< 日志消息内容
    QString file;                   ///< 源文件名（可选）
    int line = 0;                   ///< 源文件行号（可选）

    /**
     * @brief 序列化为JSON对象
     * @return JSON格式的日志条目
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON反序列化
     * @param json JSON对象
     * @return 解析后的日志条目结构
     */
    static LogEntryData fromJson(const QJsonObject& json);
};

/**
 * @brief 系统状态数据结构
 * @details 描述系统当前的运行状态信息
 */
struct SystemStatusData {
    QString status;                 ///< 运行状态 ("running"/"stopped"/"error"/"paused")
    qint64 uptimeSeconds = 0;       ///< 系统运行时长 (秒)
    QString version;                ///< 软件版本号
    qint64 totalProcessed = 0;      ///< 累计处理数量
    QDateTime timestamp;            ///< 状态更新时间

    /**
     * @brief 序列化为JSON对象
     * @return JSON格式的系统状态
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON反序列化
     * @param json JSON对象
     * @return 解析后的系统状态结构
     */
    static SystemStatusData fromJson(const QJsonObject& json);
};

/**
 * @class RemoteProtocol
 * @brief 远程通信消息协议类
 * @details 提供消息创建、解析和类型转换的静态工具方法
 *
 * 使用示例:
 * @code
 * // 创建握手消息
 * QJsonObject hello = RemoteProtocol::createClientHello("client-001", "1.0.0");
 *
 * // 解析收到的消息
 * MessageType type;
 * QJsonObject data;
 * if (RemoteProtocol::parseMessage(jsonStr, type, data)) {
 *     // 处理消息
 * }
 * @endcode
 */
class RemoteProtocol
{
public:
    // ==================== 消息创建和解析 ====================

    /**
     * @brief 创建标准格式消息
     * @param type 消息类型
     * @param data 消息数据（可选）
     * @return 完整的JSON消息对象
     *
     * 消息格式:
     * @code{.json}
     * {
     *     "type": "message_type",
     *     "timestamp": "2025-12-20T10:30:00Z",
     *     "data": { ... }
     * }
     * @endcode
     */
    static QJsonObject createMessage(MessageType type, const QJsonObject& data = QJsonObject());

    /**
     * @brief 解析JSON消息字符串
     * @param jsonString JSON格式的消息字符串
     * @param[out] type 解析出的消息类型
     * @param[out] data 解析出的消息数据
     * @return 解析是否成功
     */
    static bool parseMessage(const QString& jsonString, MessageType& type, QJsonObject& data);

    // ==================== 握手消息 ====================

    /**
     * @brief 创建客户端握手消息
     * @param clientId 客户端唯一标识
     * @param version 客户端版本号
     * @return ClientHello消息
     *
     * @note 这应该是客户端连接后发送的第一条消息
     */
    static QJsonObject createClientHello(const QString& clientId, const QString& version);

    /**
     * @brief 创建服务器握手响应
     * @param success 握手是否成功
     * @param sessionId 分配给客户端的会话ID
     * @return ServerHello消息
     */
    static QJsonObject createServerHello(bool success, const QString& sessionId);

    // ==================== 订阅消息 ====================

    /**
     * @brief 创建订阅请求消息
     * @param type 要订阅的数据类型
     * @return Subscribe消息
     */
    static QJsonObject createSubscribe(SubscriptionType type);

    /**
     * @brief 创建取消订阅消息
     * @param type 要取消订阅的数据类型
     * @return Unsubscribe消息
     */
    static QJsonObject createUnsubscribe(SubscriptionType type);

    // ==================== 数据推送消息 ====================

    /**
     * @brief 创建性能指标更新消息
     * @param metrics 性能指标数据
     * @return PerformanceUpdate消息
     */
    static QJsonObject createPerformanceUpdate(const PerformanceMetrics& metrics);

    /**
     * @brief 创建日志条目推送消息
     * @param log 日志条目数据
     * @return LogEntry消息
     */
    static QJsonObject createLogEntry(const LogEntryData& log);

    /**
     * @brief 创建系统状态更新消息
     * @param status 系统状态数据
     * @return SystemStatus消息
     */
    static QJsonObject createSystemStatus(const SystemStatusData& status);

    // ==================== 命令消息 ====================

    /**
     * @brief 创建命令请求消息
     * @param command 命令名称（如："start", "stop", "restart"）
     * @param params 命令参数（可选）
     * @return SendCommand消息
     *
     * 支持的命令:
     * - "start": 启动服务
     * - "stop": 停止服务
     * - "restart": 重启服务
     * - "clear_logs": 清除日志
     */
    static QJsonObject createCommand(const QString& command, const QJsonObject& params = QJsonObject());

    /**
     * @brief 创建命令执行结果消息
     * @param success 命令是否执行成功
     * @param message 结果描述信息
     * @param result 返回数据（可选）
     * @return CommandResult消息
     */
    static QJsonObject createCommandResult(bool success, const QString& message, const QJsonObject& result = QJsonObject());

    // ==================== 查询消息 ====================

    /**
     * @brief 创建历史数据查询消息
     * @param dataType 数据类型 ("performance" / "logs")
     * @param start 查询起始时间
     * @param end 查询结束时间
     * @return QueryHistory消息
     */
    static QJsonObject createQueryHistory(const QString& dataType, const QDateTime& start, const QDateTime& end);

    /**
     * @brief 创建历史数据响应消息
     * @param data 查询结果数组
     * @param total 总记录数
     * @return HistoryData消息
     */
    static QJsonObject createHistoryData(const QJsonArray& data, int total);

    // ==================== 心跳消息 ====================

    /**
     * @brief 创建心跳请求消息
     * @return Ping消息
     * @note 建议每10-30秒发送一次
     */
    static QJsonObject createPing();

    /**
     * @brief 创建心跳响应消息
     * @return Pong消息
     */
    static QJsonObject createPong();

    // ==================== 错误消息 ====================

    /**
     * @brief 创建错误消息
     * @param errorMessage 错误描述
     * @param errorCode 错误码（0表示通用错误）
     * @return Error消息
     *
     * 常见错误码:
     * - 400: 请求格式错误
     * - 401: 未认证
     * - 403: 权限不足
     * - 404: 资源不存在
     * - 500: 服务器内部错误
     */
    static QJsonObject createError(const QString& errorMessage, int errorCode = 0);

    // ==================== 类型转换工具 ====================

    /**
     * @brief 消息类型枚举转字符串
     * @param type 消息类型
     * @return 消息类型的字符串表示
     */
    static QString messageTypeToString(MessageType type);

    /**
     * @brief 字符串转消息类型枚举
     * @param typeStr 消息类型字符串
     * @return 对应的消息类型枚举值
     */
    static MessageType stringToMessageType(const QString& typeStr);

    /**
     * @brief 订阅类型枚举转字符串
     * @param type 订阅类型
     * @return 订阅类型的字符串表示
     */
    static QString subscriptionTypeToString(SubscriptionType type);

    /**
     * @brief 字符串转订阅类型枚举
     * @param typeStr 订阅类型字符串
     * @return 对应的订阅类型枚举值
     */
    static SubscriptionType stringToSubscriptionType(const QString& typeStr);

    /**
     * @brief 日志级别枚举转字符串
     * @param level 日志级别
     * @return 日志级别的字符串表示（大写）
     */
    static QString logLevelToString(LogLevel level);

    /**
     * @brief 字符串转日志级别枚举
     * @param levelStr 日志级别字符串
     * @return 对应的日志级别枚举值
     */
    static LogLevel stringToLogLevel(const QString& levelStr);
};

// ==================== Inline实现 ====================

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
