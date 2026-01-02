/**
 * @file Logger.h
 * @brief 日志系统 - 多级别日志记录与导出
 * @author VisionForge Team
 * @date 2025-12-14
 *
 * @details
 * 本文件实现了VisionForge项目的核心日志系统，提供统一的日志记录接口。
 *
 * ## 设计模式
 * - **单例模式 (Singleton)**：全局唯一的日志实例，通过instance()获取
 * - **模板方法模式**：统一的日志格式化和输出流程
 *
 * ## 核心功能
 * - 多级别日志：Debug、Info、Warning、Error、Critical
 * - 多输出目标：控制台输出、文件输出
 * - 日志历史：内存中保留可配置数量的日志记录
 * - 多格式导出：支持纯文本、CSV、JSON、HTML格式
 * - 日志筛选：按级别、时间、关键词筛选日志
 *
 * ## 线程安全
 * - 使用QReadWriteLock实现读写锁，优化读多写少场景
 * - 写操作（记录日志）使用写锁
 * - 读操作（查询历史、导出）使用读锁
 * - 多线程环境下可安全使用
 *
 * ## 内存管理
 * - 日志历史使用QVector存储，自动管理内存
 * - 文件句柄使用std::unique_ptr智能指针管理
 * - 支持设置最大历史条数，防止内存无限增长
 *
 * ## 性能优化
 * - 批量flush机制：累积多条日志后统一写入磁盘
 * - 日志级别过滤：低于设定级别的日志直接跳过
 *
 * ## 使用示例
 * @code
 * // 使用便捷宏记录日志
 * LOG_INFO("应用程序启动");
 * LOG_DEBUG("调试信息");
 * LOG_WARNING("警告消息");
 * LOG_ERROR("错误信息");
 *
 * // 配置日志
 * Logger::instance().setLogFile("app.log");
 * Logger::instance().setLogLevel(Logger::Debug);
 *
 * // 导出日志
 * Logger::instance().exportLogs("export.html", LogExportFormat::HTML);
 * @endcode
 */

#pragma once

#include <QString>
#include <QStringList>
#include <QMutex>
#include <QReadWriteLock>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QVector>
#include <QMap>
#include <memory>

namespace VisionForge {
namespace Base {

/**
 * @brief 日志条目结构
 */
struct LogEntry {
    QDateTime timestamp;    // 时间戳
    int level;              // 日志级别
    QString message;        // 日志消息
    QString file;           // 源文件
    int line;               // 行号

    LogEntry() : level(0), line(0) {}
    LogEntry(const QDateTime& t, int l, const QString& m, const QString& f = "", int ln = 0)
        : timestamp(t), level(l), message(m), file(f), line(ln) {}
};

/**
 * @brief 日志导出格式
 */
enum class LogExportFormat {
    PlainText,  // 纯文本 (.txt)
    CSV,        // CSV格式 (.csv)
    JSON,       // JSON格式 (.json)
    HTML        // HTML格式 (.html)
};

/**
 * @brief 日志系统（单例）
 *
 * 核心功能：
 * - 多级别日志：Debug, Info, Warning, Error, Critical
 * - 文件输出：支持日志文件输出
 * - 控制台输出：支持控制台输出
 * - 线程安全：使用互斥锁保护
 * - 日志导出：支持多种格式导出
 */
class Logger {
public:
    /**
     * @brief 日志级别
     */
    enum Level {
        Debug,      // 调试信息
        Info,       // 一般信息
        Warning,    // 警告
        Error,      // 错误
        Critical    // 严重错误
    };

    /**
     * @brief 获取单例实例
     */
    static Logger& instance();

    /**
     * @brief 记录日志
     * @param level 日志级别
     * @param message 日志消息
     * @param file 源文件名
     * @param line 源文件行号
     */
    void log(Level level, const QString& message,
            const QString& file = "", int line = 0);

    /**
     * @brief 设置日志级别
     * @param level 最低日志级别
     */
    void setLogLevel(Level level);

    /**
     * @brief 获取当前日志级别
     */
    Level logLevel() const { return logLevel_; }

    /**
     * @brief 设置日志文件路径
     * @param filePath 日志文件路径
     * @return 是否成功
     */
    bool setLogFile(const QString& filePath);

    /**
     * @brief 启用/禁用控制台输出
     * @param enabled true启用，false禁用
     */
    void enableConsoleOutput(bool enabled);

    /**
     * @brief 是否启用控制台输出
     */
    bool isConsoleOutputEnabled() const { return consoleOutput_; }

    /**
     * @brief 启用/禁用文件输出
     * @param enabled true启用，false禁用
     */
    void enableFileOutput(bool enabled);

    /**
     * @brief 是否启用文件输出
     */
    bool isFileOutputEnabled() const { return fileOutput_; }

    /**
     * @brief 刷新日志缓冲区
     */
    void flush();

    /**
     * @brief 关闭日志文件
     */
    void close();

    // ==================== 日志导出功能 ====================

    /**
     * @brief 获取日志历史记录
     * @param maxEntries 最大条目数（0表示全部）
     * @return 日志条目列表
     */
    QVector<LogEntry> getLogHistory(int maxEntries = 0) const;

    /**
     * @brief 按条件筛选日志
     * @param minLevel 最低级别
     * @param startTime 开始时间（可选）
     * @param endTime 结束时间（可选）
     * @param keyword 关键词筛选（可选）
     * @return 筛选后的日志列表
     */
    QVector<LogEntry> filterLogs(Level minLevel = Debug,
                                 const QDateTime& startTime = QDateTime(),
                                 const QDateTime& endTime = QDateTime(),
                                 const QString& keyword = QString()) const;

    /**
     * @brief 导出日志到文件
     * @param filePath 导出文件路径
     * @param format 导出格式
     * @param entries 要导出的日志（空则导出全部）
     * @return true如果导出成功
     */
    bool exportLogs(const QString& filePath,
                   LogExportFormat format = LogExportFormat::PlainText,
                   const QVector<LogEntry>& entries = QVector<LogEntry>());

    /**
     * @brief 导出日志到字符串
     * @param format 导出格式
     * @param entries 要导出的日志（空则导出全部）
     * @return 导出的字符串内容
     */
    QString exportLogsToString(LogExportFormat format = LogExportFormat::PlainText,
                              const QVector<LogEntry>& entries = QVector<LogEntry>()) const;

    /**
     * @brief 清空日志历史
     */
    void clearHistory();

    /**
     * @brief 设置历史记录最大条数
     * @param maxSize 最大条数（0表示无限制）
     */
    void setMaxHistorySize(int maxSize);

    /**
     * @brief 获取历史记录条数
     */
    int historySize() const;

    /**
     * @brief 获取按级别统计的日志数量
     * @return 各级别的日志数量
     */
    QMap<Level, int> getLogStatistics() const;

private:
    // 私有构造函数（单例）
    Logger();
    ~Logger();

    // 禁止拷贝
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief 写入日志文件
     * @param formattedMessage 格式化的日志消息
     */
    void writeToFile(const QString& formattedMessage);

    /**
     * @brief 格式化日志消息
     * @param level 日志级别
     * @param message 日志消息
     * @param file 源文件名
     * @param line 源文件行号
     * @return 格式化后的消息
     */
    QString formatMessage(Level level, const QString& message,
                         const QString& file, int line) const;

    /**
     * @brief 获取日志级别字符串
     * @param level 日志级别
     * @return 级别字符串
     */
    static QString levelToString(Level level);

    // 导出辅助方法
    QString exportToPlainText(const QVector<LogEntry>& entries) const;
    QString exportToCSV(const QVector<LogEntry>& entries) const;
    QString exportToJSON(const QVector<LogEntry>& entries) const;
    QString exportToHTML(const QVector<LogEntry>& entries) const;

private:
    Level logLevel_;            // 当前日志级别
    QString logFilePath_;       // 日志文件路径
    bool consoleOutput_;        // 是否输出到控制台
    bool fileOutput_;           // 是否输出到文件
    mutable QReadWriteLock rwLock_;  // 读写锁（读多写少场景优化）
    std::unique_ptr<QFile> logFile_;        // 日志文件（智能指针）
    std::unique_ptr<QTextStream> logStream_; // 日志流（智能指针）

    // 日志历史记录
    QVector<LogEntry> logHistory_;  // 日志历史
    int maxHistorySize_;            // 最大历史条数

    // 批量flush优化
    int unflushedCount_;                        // 未flush的日志计数
    static constexpr int FLUSH_THRESHOLD = 10;  // 每10条flush一次
};

} // namespace Base
} // namespace VisionForge

// ========== 便捷宏 ==========

#define LOG_DEBUG(msg) \
    VisionForge::Base::Logger::instance().log(VisionForge::Base::Logger::Debug, msg, __FILE__, __LINE__)

#define LOG_INFO(msg) \
    VisionForge::Base::Logger::instance().log(VisionForge::Base::Logger::Info, msg, __FILE__, __LINE__)

#define LOG_WARNING(msg) \
    VisionForge::Base::Logger::instance().log(VisionForge::Base::Logger::Warning, msg, __FILE__, __LINE__)

#define LOG_ERROR(msg) \
    VisionForge::Base::Logger::instance().log(VisionForge::Base::Logger::Error, msg, __FILE__, __LINE__)

#define LOG_CRITICAL(msg) \
    VisionForge::Base::Logger::instance().log(VisionForge::Base::Logger::Critical, msg, __FILE__, __LINE__)
