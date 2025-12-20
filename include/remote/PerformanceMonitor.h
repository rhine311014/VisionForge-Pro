/**
 * @file PerformanceMonitor.h
 * @brief 性能监控器 - 实时采集系统和应用性能指标
 * @details 采集CPU、内存、帧率、延迟等关键性能指标
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "remote/RemoteProtocol.h"
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QMutex>
#include <deque>

namespace VisionForge {
namespace Remote {

/**
 * @brief 性能统计数据
 */
struct PerformanceStats {
    // CPU和内存
    double cpuUsagePercent = 0.0;       // CPU使用率 (%)
    qint64 memoryUsedMB = 0;            // 已用内存 (MB)
    qint64 memoryTotalMB = 0;           // 总内存 (MB)
    int threadCount = 0;                // 线程数

    // 处理性能
    double currentFPS = 0.0;            // 当前帧率
    double avgFPS = 0.0;                // 平均帧率
    double currentLatencyMs = 0.0;      // 当前延迟 (ms)
    double avgLatencyMs = 0.0;          // 平均延迟 (ms)
    qint64 totalFramesProcessed = 0;    // 总处理帧数

    // 错误统计
    int errorCount = 0;                 // 错误计数
    int warningCount = 0;               // 警告计数

    QDateTime timestamp;                // 时间戳
};

/**
 * @brief 帧处理记录
 */
struct FrameRecord {
    qint64 timestamp;                   // 时间戳 (毫秒)
    double processingTimeMs;            // 处理时长 (毫秒)
    bool success;                       // 是否成功
};

/**
 * @class PerformanceMonitor
 * @brief 性能监控器
 *
 * 功能特性:
 * - 自动采集系统资源使用情况
 * - 监控处理帧率和延迟
 * - 统计错误和警告数量
 * - 计算移动平均值
 * - 定时推送性能指标
 */
class PerformanceMonitor : public QObject
{
    Q_OBJECT

public:
    explicit PerformanceMonitor(QObject* parent = nullptr);
    ~PerformanceMonitor() override = default;

    /**
     * @brief 启动监控
     * @param intervalMs 采样间隔 (毫秒)
     */
    void start(int intervalMs = 1000);

    /**
     * @brief 停止监控
     */
    void stop();

    /**
     * @brief 是否正在运行
     */
    bool isRunning() const;

    /**
     * @brief 设置采样间隔
     */
    void setSamplingInterval(int intervalMs);
    int samplingInterval() const { return samplingIntervalMs_; }

    /**
     * @brief 设置统计窗口大小 (用于计算移动平均)
     */
    void setStatWindowSize(int size);
    int statWindowSize() const { return statWindowSize_; }

    // ========== 帧处理记录 ==========

    /**
     * @brief 记录帧处理开始
     */
    void recordFrameStart();

    /**
     * @brief 记录帧处理结束
     * @param success 是否成功
     */
    void recordFrameEnd(bool success = true);

    /**
     * @brief 增加错误计数
     */
    void incrementErrorCount();

    /**
     * @brief 增加警告计数
     */
    void incrementWarningCount();

    /**
     * @brief 重置错误计数
     */
    void resetErrorCount();

    /**
     * @brief 重置警告计数
     */
    void resetWarningCount();

    // ========== 数据获取 ==========

    /**
     * @brief 获取当前性能统计
     */
    PerformanceStats currentStats() const;

    /**
     * @brief 获取性能指标（RemoteProtocol格式）
     */
    PerformanceMetrics currentMetrics() const;

    /**
     * @brief 获取历史记录
     * @param count 获取最近N条记录
     */
    QList<PerformanceStats> history(int count = 60) const;

signals:
    /**
     * @brief 性能指标更新
     */
    void metricsUpdated(const PerformanceMetrics& metrics);

    /**
     * @brief 性能统计更新
     */
    void statsUpdated(const PerformanceStats& stats);

    /**
     * @brief 性能异常告警
     */
    void performanceWarning(const QString& message);

private slots:
    /**
     * @brief 采样定时器超时
     */
    void onSamplingTimeout();

private:
    /**
     * @brief 采集系统资源
     */
    void collectSystemResources();

    /**
     * @brief 采集处理性能
     */
    void collectProcessingPerformance();

    /**
     * @brief 计算统计数据
     */
    void calculateStatistics();

    /**
     * @brief 检查性能异常
     */
    void checkPerformanceIssues(const PerformanceStats& stats);

    /**
     * @brief 获取CPU使用率
     */
    double getCPUUsage();

    /**
     * @brief 获取内存使用情况
     */
    void getMemoryUsage(qint64& usedMB, qint64& totalMB);

    /**
     * @brief 获取线程数
     */
    int getThreadCount();

private:
    QTimer* samplingTimer_;                 // 采样定时器
    int samplingIntervalMs_;                // 采样间隔 (毫秒)
    int statWindowSize_;                    // 统计窗口大小

    // 当前统计
    PerformanceStats currentStats_;

    // 帧处理记录
    std::deque<FrameRecord> frameRecords_;  // 帧记录队列
    QElapsedTimer frameTimer_;              // 帧计时器
    bool isFrameProcessing_;                // 是否正在处理帧

    // 历史数据
    std::deque<PerformanceStats> statsHistory_;

    // CPU监控（Windows特定）
    qint64 lastCPUTime_;                    // 上次CPU时间
    qint64 lastSystemTime_;                 // 上次系统时间

    // 错误计数
    int errorCount_;
    int warningCount_;

    mutable QMutex mutex_;                  // 线程安全锁
};

} // namespace Remote
} // namespace VisionForge
