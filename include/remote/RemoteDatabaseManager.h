/**
 * @file RemoteDatabaseManager.h
 * @brief 远程诊断数据库管理器 - SQLite数据持久化
 * @details 存储和查询性能指标、日志等历史数据
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "remote/RemoteProtocol.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>
#include <QDateTime>

namespace VisionForge {
namespace Remote {

/**
 * @brief 数据库配置
 */
struct DatabaseConfig {
    QString databasePath = "./data/diagnostics.db";  // 数据库文件路径
    int maxRecordAge = 30;                           // 最大记录保留天数
    bool enableAutoCleanup = true;                   // 启用自动清理
    int cleanupIntervalHours = 24;                   // 清理间隔（小时）
};

/**
 * @brief 性能指标查询条件
 */
struct PerformanceQuery {
    QDateTime startTime;
    QDateTime endTime;
    int limit = 1000;                                // 最大返回条数
    int minFps = -1;                                 // 最小FPS过滤（-1表示不过滤）
    double maxCpuUsage = -1;                         // 最大CPU使用率过滤
};

/**
 * @brief 日志查询条件
 */
struct LogQuery {
    QDateTime startTime;
    QDateTime endTime;
    LogLevel minLevel = LogLevel::Trace;             // 最低日志级别
    QString category;                                // 分类过滤
    QString keyword;                                 // 关键字搜索
    int limit = 1000;                                // 最大返回条数
};

/**
 * @brief 统计数据
 */
struct StatisticsData {
    qint64 totalPerformanceRecords = 0;              // 性能记录总数
    qint64 totalLogRecords = 0;                      // 日志记录总数
    qint64 databaseSizeBytes = 0;                    // 数据库文件大小
    QDateTime oldestRecordTime;                      // 最早记录时间
    QDateTime newestRecordTime;                      // 最新记录时间
    QMap<LogLevel, int> logLevelDistribution;        // 日志级别分布
};

/**
 * @class RemoteDatabaseManager
 * @brief 远程诊断数据库管理器
 *
 * 功能特性:
 * - SQLite数据库管理
 * - 性能指标历史存储
 * - 日志历史存储
 * - 高效查询和索引
 * - 自动数据清理
 * - 数据导出（CSV/JSON）
 */
class RemoteDatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit RemoteDatabaseManager(QObject* parent = nullptr);
    ~RemoteDatabaseManager() override;

    /**
     * @brief 初始化数据库
     */
    bool initialize(const DatabaseConfig& config = DatabaseConfig());

    /**
     * @brief 关闭数据库
     */
    void close();

    /**
     * @brief 数据库是否已打开
     */
    bool isOpen() const;

    // ========== 性能指标存储 ==========

    /**
     * @brief 插入性能指标记录
     */
    bool insertPerformanceMetrics(const PerformanceMetrics& metrics);

    /**
     * @brief 批量插入性能指标
     */
    bool insertPerformanceMetricsBatch(const QList<PerformanceMetrics>& metricsList);

    /**
     * @brief 查询性能指标
     */
    QList<PerformanceMetrics> queryPerformanceMetrics(const PerformanceQuery& query);

    /**
     * @brief 获取最新的性能指标
     */
    QList<PerformanceMetrics> getLatestPerformanceMetrics(int count = 100);

    /**
     * @brief 删除指定时间之前的性能记录
     */
    bool deletePerformanceMetricsBefore(const QDateTime& beforeTime);

    // ========== 日志存储 ==========

    /**
     * @brief 插入日志记录
     */
    bool insertLogEntry(const LogEntryData& log);

    /**
     * @brief 批量插入日志
     */
    bool insertLogEntriesBatch(const QList<LogEntryData>& logs);

    /**
     * @brief 查询日志
     */
    QList<LogEntryData> queryLogEntries(const LogQuery& query);

    /**
     * @brief 获取最新的日志
     */
    QList<LogEntryData> getLatestLogEntries(int count = 100);

    /**
     * @brief 删除指定时间之前的日志记录
     */
    bool deleteLogEntriesBefore(const QDateTime& beforeTime);

    // ========== 统计和分析 ==========

    /**
     * @brief 获取统计数据
     */
    StatisticsData getStatistics();

    /**
     * @brief 获取性能指标平均值（指定时间范围）
     */
    PerformanceMetrics getAveragePerformance(const QDateTime& start, const QDateTime& end);

    /**
     * @brief 获取日志级别分布
     */
    QMap<LogLevel, int> getLogLevelDistribution(const QDateTime& start, const QDateTime& end);

    // ========== 数据导出 ==========

    /**
     * @brief 导出性能指标到CSV
     */
    bool exportPerformanceToCSV(const QString& filePath, const PerformanceQuery& query);

    /**
     * @brief 导出日志到CSV
     */
    bool exportLogsToCSV(const QString& filePath, const LogQuery& query);

    /**
     * @brief 导出数据到JSON
     */
    bool exportToJSON(const QString& filePath, const QDateTime& start, const QDateTime& end);

    // ========== 维护 ==========

    /**
     * @brief 清理过期数据
     */
    bool cleanupExpiredData();

    /**
     * @brief 优化数据库（VACUUM）
     */
    bool optimizeDatabase();

    /**
     * @brief 获取数据库大小（字节）
     */
    qint64 getDatabaseSize() const;

signals:
    /**
     * @brief 数据库错误
     */
    void databaseError(const QString& error);

    /**
     * @brief 清理完成
     */
    void cleanupCompleted(int recordsDeleted);

private:
    /**
     * @brief 创建数据库表
     */
    bool createTables();

    /**
     * @brief 创建性能指标表
     */
    bool createPerformanceTable();

    /**
     * @brief 创建日志表
     */
    bool createLogTable();

    /**
     * @brief 创建索引
     */
    bool createIndexes();

    /**
     * @brief 执行查询并处理错误
     */
    bool executeQuery(QSqlQuery& query, const QString& errorContext);

    /**
     * @brief 开始事务
     */
    bool beginTransaction();

    /**
     * @brief 提交事务
     */
    bool commitTransaction();

    /**
     * @brief 回滚事务
     */
    void rollbackTransaction();

private:
    QSqlDatabase db_;                       // 数据库连接
    DatabaseConfig config_;                 // 数据库配置
    QString connectionName_;                // 连接名称
    mutable QMutex mutex_;                  // 线程安全锁
    QTimer* cleanupTimer_;                  // 自动清理定时器
};

} // namespace Remote
} // namespace VisionForge
