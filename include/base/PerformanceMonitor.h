/**
 * @file PerformanceMonitor.h
 * @brief 性能监控工具
 * @details 用于性能分析、计时和性能统计
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include <QString>
#include <QMap>
#include <QElapsedTimer>
#include <QMutex>
#include <chrono>

namespace VisionForge {
namespace Base {

/**
 * @brief 性能计时器（RAII风格）
 */
class ScopedTimer {
public:
    explicit ScopedTimer(const QString& name);
    ~ScopedTimer();

    /**
     * @brief 获取已经过的时间（毫秒）
     */
    qint64 elapsed() const;

private:
    QString name_;
    QElapsedTimer timer_;
};

/**
 * @brief 性能统计数据
 */
struct PerformanceStats {
    QString name;               // 统计名称
    qint64 totalTime;          // 总时间（毫秒）
    qint64 minTime;            // 最小时间（毫秒）
    qint64 maxTime;            // 最大时间（毫秒）
    double avgTime;            // 平均时间（毫秒）
    int callCount;             // 调用次数

    PerformanceStats()
        : totalTime(0)
        , minTime(LLONG_MAX)
        , maxTime(0)
        , avgTime(0.0)
        , callCount(0)
    {}
};

/**
 * @brief 性能监控器
 *
 * 单例模式，用于收集和分析性能数据
 */
class PerformanceMonitor {
public:
    /**
     * @brief 获取单例实例
     */
    static PerformanceMonitor& instance();

    /**
     * @brief 记录一次性能数据
     * @param name 统计名称
     * @param elapsedMs 耗时（毫秒）
     */
    void record(const QString& name, qint64 elapsedMs);

    /**
     * @brief 获取性能统计
     * @param name 统计名称
     */
    PerformanceStats getStats(const QString& name) const;

    /**
     * @brief 获取所有统计数据
     */
    QMap<QString, PerformanceStats> getAllStats() const;

    /**
     * @brief 重置所有统计数据
     */
    void reset();

    /**
     * @brief 重置指定统计数据
     */
    void reset(const QString& name);

    /**
     * @brief 打印性能报告
     * @param topN 打印前N个最慢的项（0表示全部）
     */
    void printReport(int topN = 10) const;

    /**
     * @brief 导出性能报告到文件
     * @param filePath 文件路径
     */
    bool exportReport(const QString& filePath) const;

    /**
     * @brief 启用/禁用性能监控
     */
    void setEnabled(bool enabled) { enabled_ = enabled; }

    /**
     * @brief 检查是否启用
     */
    bool isEnabled() const { return enabled_; }

private:
    PerformanceMonitor();
    ~PerformanceMonitor() = default;

    // 禁止拷贝
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    PerformanceMonitor& operator=(const PerformanceMonitor&) = delete;

private:
    mutable QMutex mutex_;
    QMap<QString, PerformanceStats> stats_;
    bool enabled_;
};

/**
 * @brief 性能计时宏
 * 使用RAII方式自动计时并记录
 */
#define PERF_TIMER(name) \
    VisionForge::Base::ScopedTimer __perf_timer__(name)

/**
 * @brief 条件性能计时宏
 */
#define PERF_TIMER_IF(condition, name) \
    VisionForge::Base::ScopedTimer* __perf_timer__ = nullptr; \
    if (condition) __perf_timer__ = new VisionForge::Base::ScopedTimer(name); \
    auto __perf_cleanup__ = [&]() { if (__perf_timer__) delete __perf_timer__; }; \
    (void)__perf_cleanup__

} // namespace Base
} // namespace VisionForge
