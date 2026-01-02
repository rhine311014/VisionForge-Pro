/**
 * @file RemoteLogCollector.h
 * @brief 远程日志收集器 - 统一收集和管理应用日志
 * @details 支持日志分级、过滤、本地缓存和远程上传
 *
 * @section log_overview 日志系统概述
 * RemoteLogCollector提供统一的日志管理功能，支持:
 * - 六级日志分类: Trace/Debug/Info/Warn/Error/Fatal
 * - 分类过滤: 按模块筛选日志
 * - 本地存储: 文件日志持久化
 * - 内存缓存: 快速日志查询
 * - 远程推送: 实时日志上报
 *
 * @section log_levels 日志级别说明
 * | 级别 | 数值 | 用途 | 示例 |
 * |------|------|------|------|
 * | Trace | 0 | 详细调试 | 函数进入/退出 |
 * | Debug | 1 | 开发调试 | 变量值、状态 |
 * | Info | 2 | 运行信息 | 启动、配置加载 |
 * | Warn | 3 | 警告信息 | 配置缺失、性能 |
 * | Error | 4 | 错误信息 | 操作失败、异常 |
 * | Fatal | 5 | 致命错误 | 系统崩溃、无法恢复 |
 *
 * @section log_format 日志格式
 * @code
 * [2025-12-20 10:30:45.123] [INFO] [Vision] [MainWindow.cpp:156] 检测完成
 * @endcode
 *
 * @section log_rotation 日志轮转
 * - 单文件最大10MB
 * - 超过大小自动重命名为: visionforge.log.20251220_103045
 * - 创建新的日志文件继续记录
 *
 * @section usage_example 使用示例
 * @code
 * // 方式1: 直接使用实例
 * RemoteLogCollector logger;
 * logger.initialize("./logs/app.log");
 * logger.info("Vision", "检测完成，耗时100ms");
 * logger.error("Comm", "连接失败", __FILE__, __LINE__);
 *
 * // 方式2: 使用全局宏（需定义USE_REMOTE_LOG_COLLECTOR）
 * VFLOG_INFO("Vision", "开始检测");
 * VFLOG_ERROR("Comm", "连接超时");
 * @endcode
 *
 * @author VisionForge Team
 * @version 1.6.0
 * @date 2025-12-20
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 */

#pragma once

#include "remote/RemoteProtocol.h"
#include <QObject>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <deque>
#include <memory>

namespace VisionForge {
namespace Remote {

/**
 * @brief 日志过滤器配置
 */
struct LogFilter {
    LogLevel minLevel = LogLevel::Info;     // 最低日志级别
    QStringList includeCategories;          // 包含的分类
    QStringList excludeCategories;          // 排除的分类
    int maxCacheSize = 10000;               // 最大缓存条数
    bool enableFileLog = true;              // 启用文件日志
    bool enableRemoteLog = false;           // 启用远程日志
};

/**
 * @class RemoteLogCollector
 * @brief 远程日志收集器
 *
 * 功能特性:
 * - 日志分级管理 (Trace/Debug/Info/Warn/Error/Fatal)
 * - 日志分类过滤
 * - 本地文件存储
 * - 内存缓存队列
 * - 远程推送功能
 * - 日志搜索和查询
 * - 自动日志轮转
 */
class RemoteLogCollector : public QObject
{
    Q_OBJECT

public:
    explicit RemoteLogCollector(QObject* parent = nullptr);
    ~RemoteLogCollector() override;

    /**
     * @brief 初始化日志收集器
     */
    bool initialize(const QString& logFilePath = QString());

    /**
     * @brief 关闭日志收集器
     */
    void shutdown();

    /**
     * @brief 设置日志过滤器
     */
    void setFilter(const LogFilter& filter);
    LogFilter filter() const { return filter_; }

    // ========== 日志记录 ==========

    /**
     * @brief 添加日志条目
     */
    void addLog(const LogEntryData& entry);

    /**
     * @brief 便捷日志记录方法
     */
    void log(LogLevel level, const QString& category, const QString& message,
             const QString& file = QString(), int line = 0);

    void trace(const QString& category, const QString& message, const QString& file = QString(), int line = 0);
    void debug(const QString& category, const QString& message, const QString& file = QString(), int line = 0);
    void info(const QString& category, const QString& message, const QString& file = QString(), int line = 0);
    void warn(const QString& category, const QString& message, const QString& file = QString(), int line = 0);
    void error(const QString& category, const QString& message, const QString& file = QString(), int line = 0);
    void fatal(const QString& category, const QString& message, const QString& file = QString(), int line = 0);

    // ========== 日志查询 ==========

    /**
     * @brief 获取最近的日志条目
     */
    QList<LogEntryData> recentLogs(int count = 100) const;

    /**
     * @brief 根据级别获取日志
     */
    QList<LogEntryData> getLogsByLevel(LogLevel level, int maxCount = 100) const;

    /**
     * @brief 根据分类获取日志
     */
    QList<LogEntryData> getLogsByCategory(const QString& category, int maxCount = 100) const;

    /**
     * @brief 根据时间范围获取日志
     */
    QList<LogEntryData> getLogsByTimeRange(const QDateTime& start, const QDateTime& end, int maxCount = 100) const;

    /**
     * @brief 搜索日志
     */
    QList<LogEntryData> searchLogs(const QString& keyword, int maxCount = 100) const;

    /**
     * @brief 获取日志统计
     */
    QMap<LogLevel, int> getLogStatistics() const;

    // ========== 缓存管理 ==========

    /**
     * @brief 获取缓存大小
     */
    int cacheSize() const;

    /**
     * @brief 清空缓存
     */
    void clearCache();

    /**
     * @brief 清空文件日志
     */
    void clearFileLog();

    // ========== 远程推送 ==========

    /**
     * @brief 启用远程推送
     */
    void enableRemotePush(bool enable);

    /**
     * @brief 是否启用远程推送
     */
    bool isRemotePushEnabled() const { return filter_.enableRemoteLog; }

signals:
    /**
     * @brief 新日志添加
     */
    void logAdded(const LogEntryData& entry);

    /**
     * @brief 错误日志
     */
    void errorOccurred(const LogEntryData& entry);

    /**
     * @brief 严重错误日志
     */
    void fatalError(const LogEntryData& entry);

    /**
     * @brief 缓存已满警告
     */
    void cacheFull();

private:
    /**
     * @brief 检查日志是否应该记录
     */
    bool shouldLog(const LogEntryData& entry) const;

    /**
     * @brief 写入文件日志
     */
    void writeToFile(const LogEntryData& entry);

    /**
     * @brief 格式化日志条目
     */
    QString formatLogEntry(const LogEntryData& entry) const;

    /**
     * @brief 检查并轮转日志文件
     */
    void checkLogRotation();

    /**
     * @brief 限制缓存大小
     */
    void limitCacheSize();

private:
    LogFilter filter_;                      // 日志过滤器配置
    std::deque<LogEntryData> logCache_;     // 日志缓存队列

    // 文件日志
    QString logFilePath_;                   // 日志文件路径
    QFile* logFile_;                        // 日志文件
    QTextStream* logStream_;                // 日志流
    qint64 currentFileSize_;                // 当前文件大小
    qint64 maxFileSize_;                    // 最大文件大小 (10MB)

    // 统计信息
    QMap<LogLevel, int> logStatistics_;     // 日志统计

    mutable QMutex mutex_;                  // 线程安全锁
};

// ========== 全局日志宏 ==========

#ifdef USE_REMOTE_LOG_COLLECTOR

// 全局日志收集器实例
RemoteLogCollector* globalLogCollector();

// 便捷日志宏
#define VFLOG_TRACE(category, message) \
    globalLogCollector()->trace(category, message, __FILE__, __LINE__)

#define VFLOG_DEBUG(category, message) \
    globalLogCollector()->debug(category, message, __FILE__, __LINE__)

#define VFLOG_INFO(category, message) \
    globalLogCollector()->info(category, message, __FILE__, __LINE__)

#define VFLOG_WARN(category, message) \
    globalLogCollector()->warn(category, message, __FILE__, __LINE__)

#define VFLOG_ERROR(category, message) \
    globalLogCollector()->error(category, message, __FILE__, __LINE__)

#define VFLOG_FATAL(category, message) \
    globalLogCollector()->fatal(category, message, __FILE__, __LINE__)

#else

// 空宏定义（不使用远程日志时）
#define VFLOG_TRACE(category, message) do {} while(0)
#define VFLOG_DEBUG(category, message) do {} while(0)
#define VFLOG_INFO(category, message) do {} while(0)
#define VFLOG_WARN(category, message) do {} while(0)
#define VFLOG_ERROR(category, message) do {} while(0)
#define VFLOG_FATAL(category, message) do {} while(0)

#endif

} // namespace Remote
} // namespace VisionForge
