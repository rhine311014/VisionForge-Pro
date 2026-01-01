/**
 * @file Logger.cpp
 * @brief 日志系统实现
 */

#include "base/Logger.h"
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <iostream>

namespace VisionForge {
namespace Base {

Logger::Logger()
    : logLevel_(Info)
    , consoleOutput_(true)
    , fileOutput_(false)
    , maxHistorySize_(10000)  // 默认保留1万条日志
    , unflushedCount_(0)
{
    // logFile_ 和 logStream_ 使用智能指针，自动初始化为nullptr
}

Logger::~Logger()
{
    close();
}

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::log(Level level, const QString& message, const QString& file, int line)
{
    // 检查日志级别
    if (level < logLevel_) {
        return;
    }

    // 格式化消息
    QString formattedMessage = formatMessage(level, message, file, line);
    QDateTime now = QDateTime::currentDateTime();

    QWriteLocker locker(&rwLock_);  // 写入操作使用写锁

    // 存储到历史记录
    LogEntry entry(now, level, message, file, line);
    logHistory_.append(entry);

    // 限制历史记录大小
    if (maxHistorySize_ > 0 && logHistory_.size() > maxHistorySize_) {
        logHistory_.removeFirst();
    }

    // 输出到控制台
    if (consoleOutput_) {
        switch (level) {
        case Debug:
        case Info:
            std::cout << formattedMessage.toStdString() << std::endl;
            break;
        case Warning:
        case Error:
        case Critical:
            std::cerr << formattedMessage.toStdString() << std::endl;
            break;
        }
    }

    // 输出到文件
    if (fileOutput_ && logStream_) {
        writeToFile(formattedMessage);
    }
}

void Logger::setLogLevel(Level level)
{
    logLevel_ = level;
}

bool Logger::setLogFile(const QString& filePath)
{
    QWriteLocker locker(&rwLock_);  // 写入操作使用写锁

    // 关闭已有文件
    close();

    // 创建目录
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            return false;
        }
    }

    // 打开新文件（使用智能指针）
    logFilePath_ = filePath;
    logFile_ = std::make_unique<QFile>(logFilePath_);

    if (!logFile_->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        logFile_.reset();  // 智能指针自动释放
        return false;
    }

    logStream_ = std::make_unique<QTextStream>(logFile_.get());
    logStream_->setEncoding(QStringConverter::Utf8);  // 显式设置UTF-8编码
    fileOutput_ = true;

    return true;
}

void Logger::enableConsoleOutput(bool enabled)
{
    consoleOutput_ = enabled;
}

void Logger::enableFileOutput(bool enabled)
{
    fileOutput_ = enabled;
}

void Logger::flush()
{
    QWriteLocker locker(&rwLock_);  // 文件操作使用写锁

    if (logStream_) {
        logStream_->flush();
    }

    if (logFile_) {
        logFile_->flush();
    }
}

void Logger::close()
{
    // 关闭前确保所有日志已flush
    if (logStream_ && unflushedCount_ > 0) {
        logStream_->flush();
        unflushedCount_ = 0;
    }

    // 先释放流，再释放文件（智能指针自动管理）
    logStream_.reset();

    if (logFile_) {
        logFile_->close();
        logFile_.reset();
    }
}

void Logger::writeToFile(const QString& formattedMessage)
{
    if (logStream_) {
        *logStream_ << formattedMessage << '\n';

        // 批量flush优化：累积一定数量后再flush，减少I/O开销
        unflushedCount_++;
        if (unflushedCount_ >= FLUSH_THRESHOLD) {
            logStream_->flush();
            unflushedCount_ = 0;
        }
    }
}

QString Logger::formatMessage(Level level, const QString& message,
                              const QString& file, int line) const
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString levelStr = levelToString(level);

    QString result = QString("[%1] [%2] %3").arg(timestamp, levelStr, message);

    if (!file.isEmpty() && line > 0) {
        QFileInfo fileInfo(file);
        result += QString(" (%1:%2)").arg(fileInfo.fileName()).arg(line);
    }

    return result;
}

QString Logger::levelToString(Level level)
{
    switch (level) {
    case Debug:    return "DEBUG";
    case Info:     return "INFO ";
    case Warning:  return "WARN ";
    case Error:    return "ERROR";
    case Critical: return "CRIT ";
    default:       return "UNKN ";
    }
}

// ==================== 日志导出功能实现 ====================

QVector<LogEntry> Logger::getLogHistory(int maxEntries) const
{
    QReadLocker locker(&rwLock_);  // 读取操作使用读锁

    if (maxEntries <= 0 || maxEntries >= logHistory_.size()) {
        return logHistory_;
    }

    // 返回最近的N条记录
    return logHistory_.mid(logHistory_.size() - maxEntries);
}

QVector<LogEntry> Logger::filterLogs(Level minLevel,
                                     const QDateTime& startTime,
                                     const QDateTime& endTime,
                                     const QString& keyword) const
{
    QReadLocker locker(&rwLock_);  // 读取操作使用读锁

    QVector<LogEntry> result;
    for (const auto& entry : logHistory_) {
        // 级别筛选
        if (entry.level < minLevel) continue;

        // 时间筛选
        if (startTime.isValid() && entry.timestamp < startTime) continue;
        if (endTime.isValid() && entry.timestamp > endTime) continue;

        // 关键词筛选
        if (!keyword.isEmpty() && !entry.message.contains(keyword, Qt::CaseInsensitive)) continue;

        result.append(entry);
    }

    return result;
}

bool Logger::exportLogs(const QString& filePath,
                       LogExportFormat format,
                       const QVector<LogEntry>& entries)
{
    QString content = exportLogsToString(format, entries);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << content;
    file.close();

    return true;
}

QString Logger::exportLogsToString(LogExportFormat format,
                                  const QVector<LogEntry>& entries) const
{
    const QVector<LogEntry>& logs = entries.isEmpty() ? logHistory_ : entries;

    switch (format) {
    case LogExportFormat::PlainText:
        return exportToPlainText(logs);
    case LogExportFormat::CSV:
        return exportToCSV(logs);
    case LogExportFormat::JSON:
        return exportToJSON(logs);
    case LogExportFormat::HTML:
        return exportToHTML(logs);
    default:
        return exportToPlainText(logs);
    }
}

void Logger::clearHistory()
{
    QWriteLocker locker(&rwLock_);  // 写入操作使用写锁
    logHistory_.clear();
}

void Logger::setMaxHistorySize(int maxSize)
{
    QWriteLocker locker(&rwLock_);  // 写入操作使用写锁
    maxHistorySize_ = maxSize;

    // 如果需要，裁剪现有历史
    if (maxSize > 0 && logHistory_.size() > maxSize) {
        logHistory_ = logHistory_.mid(logHistory_.size() - maxSize);
    }
}

int Logger::historySize() const
{
    QReadLocker locker(&rwLock_);  // 读取操作使用读锁
    return logHistory_.size();
}

QMap<Logger::Level, int> Logger::getLogStatistics() const
{
    QReadLocker locker(&rwLock_);  // 读取操作使用读锁

    QMap<Level, int> stats;
    stats[Debug] = 0;
    stats[Info] = 0;
    stats[Warning] = 0;
    stats[Error] = 0;
    stats[Critical] = 0;

    for (const auto& entry : logHistory_) {
        stats[static_cast<Level>(entry.level)]++;
    }

    return stats;
}

// ==================== 导出辅助方法 ====================

QString Logger::exportToPlainText(const QVector<LogEntry>& entries) const
{
    QStringList lines;
    lines << QString("VisionForge Pro 日志导出");
    lines << QString("导出时间: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    lines << QString("日志条数: %1").arg(entries.size());
    lines << QString("================================================================================");
    lines << "";

    for (const auto& entry : entries) {
        QString line = QString("[%1] [%2] %3")
            .arg(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
            .arg(levelToString(static_cast<Level>(entry.level)))
            .arg(entry.message);

        if (!entry.file.isEmpty() && entry.line > 0) {
            QFileInfo fileInfo(entry.file);
            line += QString(" (%1:%2)").arg(fileInfo.fileName()).arg(entry.line);
        }

        lines << line;
    }

    return lines.join("\n");
}

QString Logger::exportToCSV(const QVector<LogEntry>& entries) const
{
    QStringList lines;
    // CSV头
    lines << "Timestamp,Level,Message,File,Line";

    for (const auto& entry : entries) {
        // 转义CSV中的特殊字符
        QString message = entry.message;
        message.replace("\"", "\"\"");
        if (message.contains(",") || message.contains("\"") || message.contains("\n")) {
            message = "\"" + message + "\"";
        }

        QString line = QString("%1,%2,%3,%4,%5")
            .arg(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
            .arg(levelToString(static_cast<Level>(entry.level)).trimmed())
            .arg(message)
            .arg(entry.file.isEmpty() ? "" : QFileInfo(entry.file).fileName())
            .arg(entry.line);

        lines << line;
    }

    return lines.join("\n");
}

QString Logger::exportToJSON(const QVector<LogEntry>& entries) const
{
    QStringList items;

    for (const auto& entry : entries) {
        // 转义JSON中的特殊字符
        QString message = entry.message;
        message.replace("\\", "\\\\");
        message.replace("\"", "\\\"");
        message.replace("\n", "\\n");
        message.replace("\r", "\\r");
        message.replace("\t", "\\t");

        QString item = QString(
            "  {\n"
            "    \"timestamp\": \"%1\",\n"
            "    \"level\": \"%2\",\n"
            "    \"message\": \"%3\",\n"
            "    \"file\": \"%4\",\n"
            "    \"line\": %5\n"
            "  }")
            .arg(entry.timestamp.toString(Qt::ISODate))
            .arg(levelToString(static_cast<Level>(entry.level)).trimmed())
            .arg(message)
            .arg(entry.file.isEmpty() ? "" : QFileInfo(entry.file).fileName())
            .arg(entry.line);

        items << item;
    }

    return QString("{\n  \"exportTime\": \"%1\",\n  \"count\": %2,\n  \"logs\": [\n%3\n  ]\n}")
        .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
        .arg(entries.size())
        .arg(items.join(",\n"));
}

QString Logger::exportToHTML(const QVector<LogEntry>& entries) const
{
    QString html;
    html += "<!DOCTYPE html>\n";
    html += "<html>\n<head>\n";
    html += "<meta charset=\"UTF-8\">\n";
    html += "<title>VisionForge Pro - 日志导出</title>\n";
    html += "<style>\n";
    html += "body { font-family: 'Microsoft YaHei', Arial, sans-serif; margin: 20px; }\n";
    html += "h1 { color: #2196F3; }\n";
    html += "table { border-collapse: collapse; width: 100%; margin-top: 20px; }\n";
    html += "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n";
    html += "th { background-color: #2196F3; color: white; }\n";
    html += "tr:nth-child(even) { background-color: #f9f9f9; }\n";
    html += ".debug { color: #9E9E9E; }\n";
    html += ".info { color: #2196F3; }\n";
    html += ".warn { color: #FF9800; }\n";
    html += ".error { color: #F44336; }\n";
    html += ".crit { color: #B71C1C; font-weight: bold; }\n";
    html += "</style>\n";
    html += "</head>\n<body>\n";
    html += "<h1>VisionForge Pro 日志导出</h1>\n";
    html += QString("<p>导出时间: %1</p>\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    html += QString("<p>日志条数: %1</p>\n").arg(entries.size());
    html += "<table>\n";
    html += "<tr><th>时间</th><th>级别</th><th>消息</th><th>位置</th></tr>\n";

    for (const auto& entry : entries) {
        QString levelClass;
        switch (entry.level) {
        case Debug:    levelClass = "debug"; break;
        case Info:     levelClass = "info"; break;
        case Warning:  levelClass = "warn"; break;
        case Error:    levelClass = "error"; break;
        case Critical: levelClass = "crit"; break;
        }

        QString location;
        if (!entry.file.isEmpty() && entry.line > 0) {
            location = QString("%1:%2").arg(QFileInfo(entry.file).fileName()).arg(entry.line);
        }

        // HTML转义
        QString message = entry.message;
        message.replace("&", "&amp;");
        message.replace("<", "&lt;");
        message.replace(">", "&gt;");

        html += QString("<tr><td>%1</td><td class=\"%2\">%3</td><td>%4</td><td>%5</td></tr>\n")
            .arg(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
            .arg(levelClass)
            .arg(levelToString(static_cast<Level>(entry.level)).trimmed())
            .arg(message)
            .arg(location);
    }

    html += "</table>\n";
    html += "</body>\n</html>";

    return html;
}

} // namespace Base
} // namespace VisionForge
