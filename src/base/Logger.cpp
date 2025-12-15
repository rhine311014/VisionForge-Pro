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
    , logFile_(nullptr)
    , logStream_(nullptr)
{
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

    QMutexLocker locker(&mutex_);

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
    QMutexLocker locker(&mutex_);

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

    // 打开新文件
    logFilePath_ = filePath;
    logFile_ = new QFile(logFilePath_);

    if (!logFile_->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        delete logFile_;
        logFile_ = nullptr;
        return false;
    }

    logStream_ = new QTextStream(logFile_);
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
    QMutexLocker locker(&mutex_);

    if (logStream_) {
        logStream_->flush();
    }

    if (logFile_) {
        logFile_->flush();
    }
}

void Logger::close()
{
    if (logStream_) {
        delete logStream_;
        logStream_ = nullptr;
    }

    if (logFile_) {
        logFile_->close();
        delete logFile_;
        logFile_ = nullptr;
    }
}

void Logger::writeToFile(const QString& formattedMessage)
{
    if (logStream_) {
        *logStream_ << formattedMessage << '\n';
        logStream_->flush();
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

} // namespace Base
} // namespace VisionForge
