/**
 * @file RemoteLogCollector.cpp
 * @brief 远程日志收集器实现
 */

#include "remote/RemoteLogCollector.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <algorithm>

namespace VisionForge {
namespace Remote {

// ========== 全局实例 ==========

#ifdef USE_REMOTE_LOG_COLLECTOR
static RemoteLogCollector* g_logCollector = nullptr;

RemoteLogCollector* globalLogCollector()
{
    if (!g_logCollector) {
        g_logCollector = new RemoteLogCollector();
        g_logCollector->initialize();
    }
    return g_logCollector;
}
#endif

// ========== 构造和析构 ==========

RemoteLogCollector::RemoteLogCollector(QObject* parent)
    : QObject(parent)
    , logFile_(nullptr)
    , logStream_(nullptr)
    , currentFileSize_(0)
    , maxFileSize_(10 * 1024 * 1024)  // 10MB
{
    filter_.minLevel = LogLevel::Info;
    filter_.maxCacheSize = 10000;
    filter_.enableFileLog = true;
    filter_.enableRemoteLog = false;
}

RemoteLogCollector::~RemoteLogCollector()
{
    shutdown();
}

// ========== 初始化和关闭 ==========

bool RemoteLogCollector::initialize(const QString& logFilePath)
{
    QMutexLocker locker(&mutex_);

    if (logFilePath.isEmpty()) {
        logFilePath_ = QDir::currentPath() + "/logs/visionforge.log";
    } else {
        logFilePath_ = logFilePath;
    }

    // 创建日志目录
    QFileInfo fileInfo(logFilePath_);
    QDir logDir = fileInfo.absoluteDir();
    if (!logDir.exists()) {
        if (!logDir.mkpath(".")) {
            qCritical() << "无法创建日志目录:" << logDir.absolutePath();
            return false;
        }
    }

    // 打开日志文件
    if (filter_.enableFileLog) {
        logFile_ = new QFile(logFilePath_);
        if (!logFile_->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            qCritical() << "无法打开日志文件:" << logFilePath_;
            delete logFile_;
            logFile_ = nullptr;
            return false;
        }

        logStream_ = new QTextStream(logFile_);
        logStream_->setEncoding(QStringConverter::Utf8);

        currentFileSize_ = logFile_->size();

        qInfo() << "日志收集器已初始化，日志文件:" << logFilePath_;
    }

    return true;
}

void RemoteLogCollector::shutdown()
{
    QMutexLocker locker(&mutex_);

    if (logStream_) {
        logStream_->flush();
        delete logStream_;
        logStream_ = nullptr;
    }

    if (logFile_) {
        logFile_->close();
        delete logFile_;
        logFile_ = nullptr;
    }

    qInfo() << "日志收集器已关闭";
}

void RemoteLogCollector::setFilter(const LogFilter& filter)
{
    QMutexLocker locker(&mutex_);
    filter_ = filter;
}

// ========== 日志记录 ==========

void RemoteLogCollector::addLog(const LogEntryData& entry)
{
    QMutexLocker locker(&mutex_);

    // 检查是否应该记录
    if (!shouldLog(entry)) {
        return;
    }

    // 添加到缓存
    logCache_.push_back(entry);
    limitCacheSize();

    // 更新统计
    logStatistics_[entry.level]++;

    // 写入文件
    if (filter_.enableFileLog) {
        writeToFile(entry);
        checkLogRotation();
    }

    // 发送信号
    emit logAdded(entry);

    // 特殊级别信号
    if (entry.level == LogLevel::Error) {
        emit errorOccurred(entry);
    } else if (entry.level == LogLevel::Fatal) {
        emit fatalError(entry);
    }
}

void RemoteLogCollector::log(LogLevel level, const QString& category, const QString& message,
                              const QString& file, int line)
{
    LogEntryData entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.level = level;
    entry.category = category;
    entry.message = message;
    entry.file = file;
    entry.line = line;

    addLog(entry);
}

void RemoteLogCollector::trace(const QString& category, const QString& message, const QString& file, int line)
{
    log(LogLevel::Trace, category, message, file, line);
}

void RemoteLogCollector::debug(const QString& category, const QString& message, const QString& file, int line)
{
    log(LogLevel::Debug, category, message, file, line);
}

void RemoteLogCollector::info(const QString& category, const QString& message, const QString& file, int line)
{
    log(LogLevel::Info, category, message, file, line);
}

void RemoteLogCollector::warn(const QString& category, const QString& message, const QString& file, int line)
{
    log(LogLevel::Warn, category, message, file, line);
}

void RemoteLogCollector::error(const QString& category, const QString& message, const QString& file, int line)
{
    log(LogLevel::Error, category, message, file, line);
}

void RemoteLogCollector::fatal(const QString& category, const QString& message, const QString& file, int line)
{
    log(LogLevel::Fatal, category, message, file, line);
}

// ========== 日志查询 ==========

QList<LogEntryData> RemoteLogCollector::recentLogs(int count) const
{
    QMutexLocker locker(&mutex_);

    QList<LogEntryData> result;
    int start = (std::max)(0, static_cast<int>(logCache_.size()) - count);

    for (int i = start; i < static_cast<int>(logCache_.size()); ++i) {
        result.append(logCache_[i]);
    }

    return result;
}

QList<LogEntryData> RemoteLogCollector::getLogsByLevel(LogLevel level, int maxCount) const
{
    QMutexLocker locker(&mutex_);

    QList<LogEntryData> result;

    for (auto it = logCache_.rbegin(); it != logCache_.rend() && result.size() < maxCount; ++it) {
        if (it->level == level) {
            result.append(*it);
        }
    }

    return result;
}

QList<LogEntryData> RemoteLogCollector::getLogsByCategory(const QString& category, int maxCount) const
{
    QMutexLocker locker(&mutex_);

    QList<LogEntryData> result;

    for (auto it = logCache_.rbegin(); it != logCache_.rend() && result.size() < maxCount; ++it) {
        if (it->category == category) {
            result.append(*it);
        }
    }

    return result;
}

QList<LogEntryData> RemoteLogCollector::getLogsByTimeRange(const QDateTime& start, const QDateTime& end, int maxCount) const
{
    QMutexLocker locker(&mutex_);

    QList<LogEntryData> result;

    for (auto it = logCache_.rbegin(); it != logCache_.rend() && result.size() < maxCount; ++it) {
        if (it->timestamp >= start && it->timestamp <= end) {
            result.append(*it);
        }
    }

    return result;
}

QList<LogEntryData> RemoteLogCollector::searchLogs(const QString& keyword, int maxCount) const
{
    QMutexLocker locker(&mutex_);

    QList<LogEntryData> result;

    for (auto it = logCache_.rbegin(); it != logCache_.rend() && result.size() < maxCount; ++it) {
        if (it->message.contains(keyword, Qt::CaseInsensitive) ||
            it->category.contains(keyword, Qt::CaseInsensitive)) {
            result.append(*it);
        }
    }

    return result;
}

QMap<LogLevel, int> RemoteLogCollector::getLogStatistics() const
{
    QMutexLocker locker(&mutex_);
    return logStatistics_;
}

// ========== 缓存管理 ==========

int RemoteLogCollector::cacheSize() const
{
    QMutexLocker locker(&mutex_);
    return static_cast<int>(logCache_.size());
}

void RemoteLogCollector::clearCache()
{
    QMutexLocker locker(&mutex_);
    logCache_.clear();
}

void RemoteLogCollector::clearFileLog()
{
    QMutexLocker locker(&mutex_);

    if (logFile_) {
        logFile_->resize(0);
        currentFileSize_ = 0;
    }
}

void RemoteLogCollector::enableRemotePush(bool enable)
{
    QMutexLocker locker(&mutex_);
    filter_.enableRemoteLog = enable;
}

// ========== 私有方法 ==========

bool RemoteLogCollector::shouldLog(const LogEntryData& entry) const
{
    // 检查日志级别
    if (static_cast<int>(entry.level) < static_cast<int>(filter_.minLevel)) {
        return false;
    }

    // 检查分类包含列表
    if (!filter_.includeCategories.isEmpty()) {
        if (!filter_.includeCategories.contains(entry.category)) {
            return false;
        }
    }

    // 检查分类排除列表
    if (filter_.excludeCategories.contains(entry.category)) {
        return false;
    }

    return true;
}

void RemoteLogCollector::writeToFile(const LogEntryData& entry)
{
    if (!logStream_) {
        return;
    }

    QString logLine = formatLogEntry(entry);
    *logStream_ << logLine << "\n";
    logStream_->flush();

    currentFileSize_ += logLine.toUtf8().size() + 1;
}

QString RemoteLogCollector::formatLogEntry(const LogEntryData& entry) const
{
    QString timestamp = entry.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString level = RemoteProtocol::logLevelToString(entry.level);
    QString location;

    if (!entry.file.isEmpty()) {
        location = QString(" [%1:%2]").arg(entry.file).arg(entry.line);
    }

    return QString("[%1] [%2] [%3]%4 %5")
        .arg(timestamp)
        .arg(level)
        .arg(entry.category)
        .arg(location)
        .arg(entry.message);
}

void RemoteLogCollector::checkLogRotation()
{
    if (currentFileSize_ >= maxFileSize_) {
        // 日志轮转：重命名当前文件并创建新文件
        if (logStream_) {
            logStream_->flush();
            delete logStream_;
            logStream_ = nullptr;
        }

        if (logFile_) {
            logFile_->close();

            // 重命名旧文件
            QString oldFilePath = logFilePath_;
            QString newFilePath = logFilePath_ + "." + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
            QFile::rename(oldFilePath, newFilePath);

            delete logFile_;
            logFile_ = nullptr;
        }

        // 创建新文件
        logFile_ = new QFile(logFilePath_);
        if (logFile_->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            logStream_ = new QTextStream(logFile_);
            logStream_->setEncoding(QStringConverter::Utf8);
            currentFileSize_ = 0;
        }
    }
}

void RemoteLogCollector::limitCacheSize()
{
    while (static_cast<int>(logCache_.size()) > filter_.maxCacheSize) {
        logCache_.pop_front();
    }

    if (static_cast<int>(logCache_.size()) >= filter_.maxCacheSize) {
        emit cacheFull();
    }
}

} // namespace Remote
} // namespace VisionForge
