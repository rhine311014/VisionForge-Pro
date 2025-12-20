/**
 * @file RemoteDatabaseManager.cpp
 * @brief 远程诊断数据库管理器实现
 */

#include "remote/RemoteDatabaseManager.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QUuid>

namespace VisionForge {
namespace Remote {

// ========== 构造和析构 ==========

RemoteDatabaseManager::RemoteDatabaseManager(QObject* parent)
    : QObject(parent)
    , cleanupTimer_(nullptr)
{
    // 生成唯一连接名称
    connectionName_ = "RemoteDiagnostics_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
}

RemoteDatabaseManager::~RemoteDatabaseManager()
{
    close();
}

// ========== 初始化和关闭 ==========

bool RemoteDatabaseManager::initialize(const DatabaseConfig& config)
{
    QMutexLocker locker(&mutex_);

    config_ = config;

    // 创建数据库目录
    QFileInfo fileInfo(config_.databasePath);
    QDir dbDir = fileInfo.absoluteDir();
    if (!dbDir.exists()) {
        if (!dbDir.mkpath(".")) {
            qCritical() << "无法创建数据库目录:" << dbDir.absolutePath();
            return false;
        }
    }

    // 创建数据库连接
    db_ = QSqlDatabase::addDatabase("QSQLITE", connectionName_);
    db_.setDatabaseName(config_.databasePath);

    if (!db_.open()) {
        QString error = QString("无法打开数据库: %1").arg(db_.lastError().text());
        qCritical() << error;
        emit databaseError(error);
        return false;
    }

    // 创建表和索引
    if (!createTables()) {
        close();
        return false;
    }

    if (!createIndexes()) {
        close();
        return false;
    }

    // 启用外键约束
    QSqlQuery query(db_);
    query.exec("PRAGMA foreign_keys = ON");

    // 设置WAL模式提升并发性能
    query.exec("PRAGMA journal_mode = WAL");

    // 启动自动清理定时器
    if (config_.enableAutoCleanup) {
        cleanupTimer_ = new QTimer(this);
        connect(cleanupTimer_, &QTimer::timeout, this, &RemoteDatabaseManager::cleanupExpiredData);
        cleanupTimer_->start(config_.cleanupIntervalHours * 3600 * 1000);
    }

    qInfo() << "远程诊断数据库已初始化:" << config_.databasePath;
    return true;
}

void RemoteDatabaseManager::close()
{
    QMutexLocker locker(&mutex_);

    if (cleanupTimer_) {
        cleanupTimer_->stop();
        cleanupTimer_->deleteLater();
        cleanupTimer_ = nullptr;
    }

    if (db_.isOpen()) {
        db_.close();
    }

    QSqlDatabase::removeDatabase(connectionName_);
}

bool RemoteDatabaseManager::isOpen() const
{
    QMutexLocker locker(&mutex_);
    return db_.isOpen();
}

// ========== 表创建 ==========

bool RemoteDatabaseManager::createTables()
{
    return createPerformanceTable() && createLogTable();
}

bool RemoteDatabaseManager::createPerformanceTable()
{
    QSqlQuery query(db_);

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS performance_metrics (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME NOT NULL,
            cpu_usage REAL NOT NULL,
            memory_used_mb INTEGER NOT NULL,
            fps REAL NOT NULL,
            avg_latency_ms REAL NOT NULL,
            error_count INTEGER NOT NULL DEFAULT 0
        )
    )";

    if (!executeQuery(query, "创建性能指标表")) {
        return false;
    }

    return true;
}

bool RemoteDatabaseManager::createLogTable()
{
    QSqlQuery query(db_);

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS log_entries (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME NOT NULL,
            level INTEGER NOT NULL,
            category TEXT NOT NULL,
            message TEXT NOT NULL,
            file TEXT,
            line INTEGER
        )
    )";

    if (!executeQuery(query, "创建日志表")) {
        return false;
    }

    return true;
}

bool RemoteDatabaseManager::createIndexes()
{
    QSqlQuery query(db_);

    // 性能指标表索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_perf_timestamp ON performance_metrics(timestamp DESC)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_perf_cpu ON performance_metrics(cpu_usage)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_perf_fps ON performance_metrics(fps)");

    // 日志表索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_log_timestamp ON log_entries(timestamp DESC)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_log_level ON log_entries(level)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_log_category ON log_entries(category)");

    return true;
}

// ========== 性能指标存储 ==========

bool RemoteDatabaseManager::insertPerformanceMetrics(const PerformanceMetrics& metrics)
{
    QMutexLocker locker(&mutex_);

    QSqlQuery query(db_);
    query.prepare(R"(
        INSERT INTO performance_metrics
        (timestamp, cpu_usage, memory_used_mb, fps, avg_latency_ms, error_count)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(metrics.timestamp);
    query.addBindValue(metrics.cpuUsage);
    query.addBindValue(metrics.memoryUsedMB);
    query.addBindValue(metrics.fps);
    query.addBindValue(metrics.avgLatencyMs);
    query.addBindValue(metrics.errorCount);

    return executeQuery(query, "插入性能指标");
}

bool RemoteDatabaseManager::insertPerformanceMetricsBatch(const QList<PerformanceMetrics>& metricsList)
{
    if (metricsList.isEmpty()) {
        return true;
    }

    QMutexLocker locker(&mutex_);

    if (!beginTransaction()) {
        return false;
    }

    QSqlQuery query(db_);
    query.prepare(R"(
        INSERT INTO performance_metrics
        (timestamp, cpu_usage, memory_used_mb, fps, avg_latency_ms, error_count)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    for (const auto& metrics : metricsList) {
        query.addBindValue(metrics.timestamp);
        query.addBindValue(metrics.cpuUsage);
        query.addBindValue(metrics.memoryUsedMB);
        query.addBindValue(metrics.fps);
        query.addBindValue(metrics.avgLatencyMs);
        query.addBindValue(metrics.errorCount);

        if (!query.exec()) {
            rollbackTransaction();
            qWarning() << "批量插入性能指标失败:" << query.lastError().text();
            return false;
        }
    }

    return commitTransaction();
}

QList<PerformanceMetrics> RemoteDatabaseManager::queryPerformanceMetrics(const PerformanceQuery& query)
{
    QMutexLocker locker(&mutex_);

    QList<PerformanceMetrics> result;

    QString sql = "SELECT timestamp, cpu_usage, memory_used_mb, fps, avg_latency_ms, error_count "
                  "FROM performance_metrics WHERE 1=1";

    QSqlQuery sqlQuery(db_);

    // 添加时间范围条件
    if (query.startTime.isValid()) {
        sql += " AND timestamp >= ?";
    }
    if (query.endTime.isValid()) {
        sql += " AND timestamp <= ?";
    }

    // 添加FPS过滤
    if (query.minFps >= 0) {
        sql += " AND fps >= ?";
    }

    // 添加CPU过滤
    if (query.maxCpuUsage >= 0) {
        sql += " AND cpu_usage <= ?";
    }

    sql += " ORDER BY timestamp DESC LIMIT ?";

    sqlQuery.prepare(sql);

    // 绑定参数
    int bindIndex = 0;
    if (query.startTime.isValid()) {
        sqlQuery.bindValue(bindIndex++, query.startTime);
    }
    if (query.endTime.isValid()) {
        sqlQuery.bindValue(bindIndex++, query.endTime);
    }
    if (query.minFps >= 0) {
        sqlQuery.bindValue(bindIndex++, query.minFps);
    }
    if (query.maxCpuUsage >= 0) {
        sqlQuery.bindValue(bindIndex++, query.maxCpuUsage);
    }
    sqlQuery.bindValue(bindIndex++, query.limit);

    if (!sqlQuery.exec()) {
        qWarning() << "查询性能指标失败:" << sqlQuery.lastError().text();
        return result;
    }

    while (sqlQuery.next()) {
        PerformanceMetrics metrics;
        metrics.timestamp = sqlQuery.value(0).toDateTime();
        metrics.cpuUsage = sqlQuery.value(1).toDouble();
        metrics.memoryUsedMB = sqlQuery.value(2).toLongLong();
        metrics.fps = sqlQuery.value(3).toDouble();
        metrics.avgLatencyMs = sqlQuery.value(4).toDouble();
        metrics.errorCount = sqlQuery.value(5).toInt();

        result.append(metrics);
    }

    return result;
}

QList<PerformanceMetrics> RemoteDatabaseManager::getLatestPerformanceMetrics(int count)
{
    PerformanceQuery query;
    query.limit = count;
    return queryPerformanceMetrics(query);
}

bool RemoteDatabaseManager::deletePerformanceMetricsBefore(const QDateTime& beforeTime)
{
    QMutexLocker locker(&mutex_);

    QSqlQuery query(db_);
    query.prepare("DELETE FROM performance_metrics WHERE timestamp < ?");
    query.addBindValue(beforeTime);

    return executeQuery(query, "删除过期性能指标");
}

// ========== 日志存储 ==========

bool RemoteDatabaseManager::insertLogEntry(const LogEntryData& log)
{
    QMutexLocker locker(&mutex_);

    QSqlQuery query(db_);
    query.prepare(R"(
        INSERT INTO log_entries
        (timestamp, level, category, message, file, line)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(log.timestamp);
    query.addBindValue(static_cast<int>(log.level));
    query.addBindValue(log.category);
    query.addBindValue(log.message);
    query.addBindValue(log.file);
    query.addBindValue(log.line);

    return executeQuery(query, "插入日志记录");
}

bool RemoteDatabaseManager::insertLogEntriesBatch(const QList<LogEntryData>& logs)
{
    if (logs.isEmpty()) {
        return true;
    }

    QMutexLocker locker(&mutex_);

    if (!beginTransaction()) {
        return false;
    }

    QSqlQuery query(db_);
    query.prepare(R"(
        INSERT INTO log_entries
        (timestamp, level, category, message, file, line)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    for (const auto& log : logs) {
        query.addBindValue(log.timestamp);
        query.addBindValue(static_cast<int>(log.level));
        query.addBindValue(log.category);
        query.addBindValue(log.message);
        query.addBindValue(log.file);
        query.addBindValue(log.line);

        if (!query.exec()) {
            rollbackTransaction();
            qWarning() << "批量插入日志失败:" << query.lastError().text();
            return false;
        }
    }

    return commitTransaction();
}

QList<LogEntryData> RemoteDatabaseManager::queryLogEntries(const LogQuery& query)
{
    QMutexLocker locker(&mutex_);

    QList<LogEntryData> result;

    QString sql = "SELECT timestamp, level, category, message, file, line "
                  "FROM log_entries WHERE 1=1";

    QSqlQuery sqlQuery(db_);

    // 添加时间范围条件
    if (query.startTime.isValid()) {
        sql += " AND timestamp >= ?";
    }
    if (query.endTime.isValid()) {
        sql += " AND timestamp <= ?";
    }

    // 添加日志级别过滤
    sql += " AND level >= ?";

    // 添加分类过滤
    if (!query.category.isEmpty()) {
        sql += " AND category = ?";
    }

    // 添加关键字搜索
    if (!query.keyword.isEmpty()) {
        sql += " AND message LIKE ?";
    }

    sql += " ORDER BY timestamp DESC LIMIT ?";

    sqlQuery.prepare(sql);

    // 绑定参数
    int bindIndex = 0;
    if (query.startTime.isValid()) {
        sqlQuery.bindValue(bindIndex++, query.startTime);
    }
    if (query.endTime.isValid()) {
        sqlQuery.bindValue(bindIndex++, query.endTime);
    }
    sqlQuery.bindValue(bindIndex++, static_cast<int>(query.minLevel));
    if (!query.category.isEmpty()) {
        sqlQuery.bindValue(bindIndex++, query.category);
    }
    if (!query.keyword.isEmpty()) {
        sqlQuery.bindValue(bindIndex++, "%" + query.keyword + "%");
    }
    sqlQuery.bindValue(bindIndex++, query.limit);

    if (!sqlQuery.exec()) {
        qWarning() << "查询日志失败:" << sqlQuery.lastError().text();
        return result;
    }

    while (sqlQuery.next()) {
        LogEntryData log;
        log.timestamp = sqlQuery.value(0).toDateTime();
        log.level = static_cast<LogLevel>(sqlQuery.value(1).toInt());
        log.category = sqlQuery.value(2).toString();
        log.message = sqlQuery.value(3).toString();
        log.file = sqlQuery.value(4).toString();
        log.line = sqlQuery.value(5).toInt();

        result.append(log);
    }

    return result;
}

QList<LogEntryData> RemoteDatabaseManager::getLatestLogEntries(int count)
{
    LogQuery query;
    query.limit = count;
    return queryLogEntries(query);
}

bool RemoteDatabaseManager::deleteLogEntriesBefore(const QDateTime& beforeTime)
{
    QMutexLocker locker(&mutex_);

    QSqlQuery query(db_);
    query.prepare("DELETE FROM log_entries WHERE timestamp < ?");
    query.addBindValue(beforeTime);

    return executeQuery(query, "删除过期日志");
}

// ========== 统计和分析 ==========

StatisticsData RemoteDatabaseManager::getStatistics()
{
    QMutexLocker locker(&mutex_);

    StatisticsData stats;

    QSqlQuery query(db_);

    // 性能记录统计
    if (query.exec("SELECT COUNT(*), MIN(timestamp), MAX(timestamp) FROM performance_metrics")) {
        if (query.next()) {
            stats.totalPerformanceRecords = query.value(0).toLongLong();
            stats.oldestRecordTime = query.value(1).toDateTime();
            stats.newestRecordTime = query.value(2).toDateTime();
        }
    }

    // 日志记录统计
    if (query.exec("SELECT COUNT(*) FROM log_entries")) {
        if (query.next()) {
            stats.totalLogRecords = query.value(0).toLongLong();
        }
    }

    // 日志级别分布
    if (query.exec("SELECT level, COUNT(*) FROM log_entries GROUP BY level")) {
        while (query.next()) {
            LogLevel level = static_cast<LogLevel>(query.value(0).toInt());
            int count = query.value(1).toInt();
            stats.logLevelDistribution[level] = count;
        }
    }

    // 数据库大小
    stats.databaseSizeBytes = getDatabaseSize();

    return stats;
}

PerformanceMetrics RemoteDatabaseManager::getAveragePerformance(const QDateTime& start, const QDateTime& end)
{
    QMutexLocker locker(&mutex_);

    PerformanceMetrics avgMetrics;

    QSqlQuery query(db_);
    query.prepare(R"(
        SELECT
            AVG(cpu_usage),
            AVG(memory_used_mb),
            AVG(fps),
            AVG(avg_latency_ms),
            SUM(error_count)
        FROM performance_metrics
        WHERE timestamp >= ? AND timestamp <= ?
    )");

    query.addBindValue(start);
    query.addBindValue(end);

    if (query.exec() && query.next()) {
        avgMetrics.cpuUsage = query.value(0).toDouble();
        avgMetrics.memoryUsedMB = query.value(1).toLongLong();
        avgMetrics.fps = query.value(2).toDouble();
        avgMetrics.avgLatencyMs = query.value(3).toDouble();
        avgMetrics.errorCount = query.value(4).toInt();
        avgMetrics.timestamp = QDateTime::currentDateTime();
    }

    return avgMetrics;
}

QMap<LogLevel, int> RemoteDatabaseManager::getLogLevelDistribution(const QDateTime& start, const QDateTime& end)
{
    QMutexLocker locker(&mutex_);

    QMap<LogLevel, int> distribution;

    QSqlQuery query(db_);
    query.prepare("SELECT level, COUNT(*) FROM log_entries WHERE timestamp >= ? AND timestamp <= ? GROUP BY level");
    query.addBindValue(start);
    query.addBindValue(end);

    if (query.exec()) {
        while (query.next()) {
            LogLevel level = static_cast<LogLevel>(query.value(0).toInt());
            int count = query.value(1).toInt();
            distribution[level] = count;
        }
    }

    return distribution;
}

// ========== 数据导出 ==========

bool RemoteDatabaseManager::exportPerformanceToCSV(const QString& filePath, const PerformanceQuery& query)
{
    QList<PerformanceMetrics> metrics = queryPerformanceMetrics(query);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法创建CSV文件:" << filePath;
        return false;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    // 写入CSV头
    stream << "Timestamp,CPU Usage (%),Memory (MB),FPS,Avg Latency (ms),Error Count\n";

    // 写入数据
    for (const auto& m : metrics) {
        stream << m.timestamp.toString(Qt::ISODate) << ","
               << m.cpuUsage << ","
               << m.memoryUsedMB << ","
               << m.fps << ","
               << m.avgLatencyMs << ","
               << m.errorCount << "\n";
    }

    file.close();
    qInfo() << "性能指标已导出到:" << filePath;
    return true;
}

bool RemoteDatabaseManager::exportLogsToCSV(const QString& filePath, const LogQuery& query)
{
    QList<LogEntryData> logs = queryLogEntries(query);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法创建CSV文件:" << filePath;
        return false;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    // 写入CSV头
    stream << "Timestamp,Level,Category,Message,File,Line\n";

    // 写入数据
    for (const auto& log : logs) {
        // 转义消息中的引号
        QString escapedMessage = log.message;
        escapedMessage.replace("\"", "\"\"");

        stream << log.timestamp.toString(Qt::ISODate) << ","
               << RemoteProtocol::logLevelToString(log.level) << ","
               << log.category << ","
               << "\"" << escapedMessage << "\","
               << log.file << ","
               << log.line << "\n";
    }

    file.close();
    qInfo() << "日志已导出到:" << filePath;
    return true;
}

bool RemoteDatabaseManager::exportToJSON(const QString& filePath, const QDateTime& start, const QDateTime& end)
{
    // 查询数据
    PerformanceQuery perfQuery;
    perfQuery.startTime = start;
    perfQuery.endTime = end;
    perfQuery.limit = 10000;

    LogQuery logQuery;
    logQuery.startTime = start;
    logQuery.endTime = end;
    logQuery.limit = 10000;

    QList<PerformanceMetrics> perfMetrics = queryPerformanceMetrics(perfQuery);
    QList<LogEntryData> logs = queryLogEntries(logQuery);

    // 构建JSON
    QJsonObject root;

    // 性能指标数组
    QJsonArray perfArray;
    for (const auto& m : perfMetrics) {
        perfArray.append(m.toJson());
    }
    root["performance_metrics"] = perfArray;

    // 日志数组
    QJsonArray logArray;
    for (const auto& log : logs) {
        logArray.append(log.toJson());
    }
    root["log_entries"] = logArray;

    // 元数据
    QJsonObject metadata;
    metadata["export_time"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    metadata["start_time"] = start.toString(Qt::ISODate);
    metadata["end_time"] = end.toString(Qt::ISODate);
    metadata["performance_count"] = perfMetrics.size();
    metadata["log_count"] = logs.size();
    root["metadata"] = metadata;

    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法创建JSON文件:" << filePath;
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qInfo() << "数据已导出到JSON:" << filePath;
    return true;
}

// ========== 维护 ==========

bool RemoteDatabaseManager::cleanupExpiredData()
{
    QDateTime cutoffTime = QDateTime::currentDateTime().addDays(-config_.maxRecordAge);

    int deletedPerf = 0;
    int deletedLogs = 0;

    // 删除过期性能记录
    QSqlQuery query(db_);
    query.prepare("DELETE FROM performance_metrics WHERE timestamp < ?");
    query.addBindValue(cutoffTime);
    if (query.exec()) {
        deletedPerf = query.numRowsAffected();
    }

    // 删除过期日志
    query.prepare("DELETE FROM log_entries WHERE timestamp < ?");
    query.addBindValue(cutoffTime);
    if (query.exec()) {
        deletedLogs = query.numRowsAffected();
    }

    int totalDeleted = deletedPerf + deletedLogs;
    qInfo() << QString("清理过期数据完成: 性能记录 %1 条, 日志 %2 条")
               .arg(deletedPerf).arg(deletedLogs);

    emit cleanupCompleted(totalDeleted);
    return true;
}

bool RemoteDatabaseManager::optimizeDatabase()
{
    QMutexLocker locker(&mutex_);

    QSqlQuery query(db_);
    if (query.exec("VACUUM")) {
        qInfo() << "数据库优化完成";
        return true;
    } else {
        qWarning() << "数据库优化失败:" << query.lastError().text();
        return false;
    }
}

qint64 RemoteDatabaseManager::getDatabaseSize() const
{
    QFileInfo fileInfo(config_.databasePath);
    return fileInfo.size();
}

// ========== 辅助方法 ==========

bool RemoteDatabaseManager::executeQuery(QSqlQuery& query, const QString& errorContext)
{
    if (!query.exec()) {
        QString error = QString("%1 失败: %2").arg(errorContext, query.lastError().text());
        qWarning() << error;
        emit databaseError(error);
        return false;
    }
    return true;
}

bool RemoteDatabaseManager::beginTransaction()
{
    if (!db_.transaction()) {
        qWarning() << "开始事务失败:" << db_.lastError().text();
        return false;
    }
    return true;
}

bool RemoteDatabaseManager::commitTransaction()
{
    if (!db_.commit()) {
        qWarning() << "提交事务失败:" << db_.lastError().text();
        rollbackTransaction();
        return false;
    }
    return true;
}

void RemoteDatabaseManager::rollbackTransaction()
{
    db_.rollback();
}

} // namespace Remote
} // namespace VisionForge
