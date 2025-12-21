/**
 * @file PoolPerformanceMonitor.h
 * @brief 内存池性能监控器 - 用于稳定性测试
 * @details 监控ImageMemoryPool的命中率、效率、内存使用等性能指标
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include <QObject>
#include <QTimer>
#include <QList>
#include <QMutex>
#include <atomic>

namespace VisionForge {
namespace Base {

/**
 * @brief 池性能快照
 */
struct PoolSnapshot {
    qint64 timestamp;               ///< 时间戳(ms)

    // 基础统计
    size_t totalAllocations;        ///< 累计分配次数
    size_t poolHits;                ///< 池命中次数
    size_t poolMisses;              ///< 池未命中次数
    double hitRate;                 ///< 命中率(%)

    // 池状态
    size_t poolSize;                ///< 池中对象数量
    size_t poolCapacity;            ///< 池容量
    size_t inUseCount;              ///< 正在使用的对象数量

    // 内存统计
    size_t memoryUsageMB;           ///< 内存使用量(MB)
    size_t memoryPeakMB;            ///< 峰值内存(MB)

    // 性能指标
    double avgAllocTimeUs;          ///< 平均分配耗时(微秒)
    double avgReleaseTimeUs;        ///< 平均释放耗时(微秒)

    PoolSnapshot();
};

/**
 * @brief 池性能统计摘要
 */
struct PoolPerformanceStats {
    // 效率统计
    double avgHitRate;              ///< 平均命中率(%)
    double minHitRate;              ///< 最低命中率(%)
    double currentHitRate;          ///< 当前命中率(%)

    // 池使用统计
    size_t avgPoolSize;             ///< 平均池大小
    size_t peakPoolSize;            ///< 峰值池大小
    size_t avgInUse;                ///< 平均使用中对象数

    // 内存统计
    size_t avgMemoryMB;             ///< 平均内存使用(MB)
    size_t peakMemoryMB;            ///< 峰值内存(MB)

    // 性能统计
    double avgAllocTimeUs;          ///< 平均分配时间(微秒)
    double maxAllocTimeUs;          ///< 最大分配时间(微秒)

    PoolPerformanceStats();
};

/**
 * @class PoolPerformanceMonitor
 * @brief 内存池性能监控器(单例)
 *
 * **功能**:
 * - 实时监控ImageMemoryPool性能指标
 * - 记录历史数据用于趋势分析
 * - 检测池效率退化
 * - 生成性能报告
 *
 * **使用示例**:
 * @code
 * auto& monitor = PoolPerformanceMonitor::instance();
 * monitor.start(1000);  // 每秒采样一次
 *
 * // 获取当前快照
 * auto snapshot = monitor.getCurrentSnapshot();
 * qDebug() << "命中率:" << snapshot.hitRate << "%";
 *
 * // 获取统计
 * auto stats = monitor.getStatistics();
 * qDebug() << "平均命中率:" << stats.avgHitRate << "%";
 *
 * // 检测效率退化
 * if (monitor.isEfficiencyDegraded()) {
 *     qWarning() << "池效率退化，命中率过低!";
 * }
 * @endcode
 */
class PoolPerformanceMonitor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static PoolPerformanceMonitor& instance();

    // ========== 监控控制 ==========

    /**
     * @brief 启动监控
     * @param intervalMs 采样间隔(毫秒)
     */
    void start(int intervalMs = 1000);

    /**
     * @brief 停止监控
     */
    void stop();

    /**
     * @brief 是否正在监控
     */
    bool isMonitoring() const { return isMonitoring_.load(); }

    /**
     * @brief 重置监控数据
     */
    void reset();

    // ========== 数据获取 ==========

    /**
     * @brief 获取当前快照
     */
    PoolSnapshot getCurrentSnapshot() const;

    /**
     * @brief 获取历史快照
     * @param seconds 获取最近N秒的数据(0=全部)
     * @return 快照列表
     */
    QList<PoolSnapshot> getHistory(int seconds = 0) const;

    /**
     * @brief 获取统计摘要
     */
    PoolPerformanceStats getStatistics() const;

    // ========== 分析功能 ==========

    /**
     * @brief 检测池效率是否退化
     * @param thresholdPercent 命中率阈值(%)
     * @return true=效率退化
     */
    bool isEfficiencyDegraded(double thresholdPercent = 75.0) const;

    /**
     * @brief 检测池是否接近容量上限
     * @param thresholdPercent 容量使用率阈值(%)
     * @return true=接近上限
     */
    bool isNearCapacity(double thresholdPercent = 90.0) const;

    /**
     * @brief 获取池命中率趋势
     * @param seconds 计算最近N秒的趋势(0=全部)
     * @return 趋势值（正数=上升，负数=下降）
     */
    double getHitRateTrend(int seconds = 0) const;

    /**
     * @brief 生成文本报告
     */
    QString generateReport() const;

    // ========== 配置 ==========

    /**
     * @brief 设置最大历史记录数
     * @param count 记录数(默认3600=1小时@1秒采样)
     */
    void setMaxHistorySize(int count);

    /**
     * @brief 获取历史记录数量
     */
    int historySize() const;

signals:
    /**
     * @brief 新快照可用
     */
    void snapshotReady(const PoolSnapshot& snapshot);

    /**
     * @brief 检测到效率退化
     * @param hitRate 当前命中率(%)
     */
    void efficiencyDegraded(double hitRate);

    /**
     * @brief 检测到接近容量上限
     * @param usagePercent 容量使用率(%)
     */
    void nearCapacity(double usagePercent);

    /**
     * @brief 监控错误
     */
    void monitorError(const QString& error);

private:
    PoolPerformanceMonitor();
    ~PoolPerformanceMonitor();
    PoolPerformanceMonitor(const PoolPerformanceMonitor&) = delete;
    PoolPerformanceMonitor& operator=(const PoolPerformanceMonitor&) = delete;

private slots:
    void onMonitorTimer();

private:
    void collectSnapshot();
    void analyzeSnapshot(const PoolSnapshot& snapshot);
    PoolPerformanceStats calculateStats() const;

private:
    QTimer* timer_;
    mutable QMutex mutex_;
    QList<PoolSnapshot> history_;
    int maxHistorySize_;
    std::atomic<bool> isMonitoring_;

    qint64 startTimestamp_;
    PoolSnapshot lastSnapshot_;  ///< 上一次快照（用于计算增量）
};

} // namespace Base
} // namespace VisionForge
