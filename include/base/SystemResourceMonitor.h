/**
 * @file SystemResourceMonitor.h
 * @brief 系统资源监控器 - 用于稳定性测试
 * @details 监控进程内存、CPU、线程、I/O等系统资源
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
 * @brief 资源快照结构
 */
struct ResourceSnapshot {
    qint64 timestamp;               ///< 时间戳(ms)

    // 内存指标
    qint64 virtualMemoryMB;         ///< 虚拟内存(MB)
    qint64 physicalMemoryMB;        ///< 物理内存(MB)
    qint64 privateMemoryMB;         ///< 私有内存(MB)
    qint64 workingSetMB;            ///< 工作集(MB)

    // CPU指标
    double cpuUsagePercent;         ///< 总CPU使用率(%)
    double cpuUserPercent;          ///< 用户态CPU(%)
    double cpuKernelPercent;        ///< 内核态CPU(%)

    // 线程/句柄
    int threadCount;                ///< 线程数量
    int handleCount;                ///< 句柄数量(Windows)

    // I/O指标
    qint64 readBytesSec;            ///< 读取速率(B/s)
    qint64 writeBytesSec;           ///< 写入速率(B/s)
    qint64 totalReadMB;             ///< 累计读取(MB)
    qint64 totalWriteMB;            ///< 累计写入(MB)

    ResourceSnapshot();
};

/**
 * @brief 统计摘要结构
 */
struct ResourceStats {
    // 内存统计
    qint64 memoryInitialMB;         ///< 初始内存(MB)
    qint64 memoryCurrentMB;         ///< 当前内存(MB)
    qint64 memoryPeakMB;            ///< 峰值内存(MB)
    double memoryGrowthRateKBh;     ///< 内存增长率(KB/h)
    double memoryGrowthTotalMB;     ///< 总增长(MB)

    // CPU统计
    double cpuAvgPercent;           ///< 平均CPU(%)
    double cpuPeakPercent;          ///< 峰值CPU(%)
    double cpuMinPercent;           ///< 最低CPU(%)

    // 线程统计
    int threadCountAvg;             ///< 平均线程数
    int threadCountPeak;            ///< 峰值线程数

    ResourceStats();
};

/**
 * @class SystemResourceMonitor
 * @brief 系统资源监控器(单例)
 *
 * **功能**:
 * - 实时监控进程资源使用
 * - 记录历史数据用于趋势分析
 * - 检测内存泄漏和性能退化
 * - 生成统计报告
 *
 * **使用示例**:
 * @code
 * auto& monitor = SystemResourceMonitor::instance();
 * monitor.start(1000);  // 每秒采样一次
 *
 * // 获取当前快照
 * auto snapshot = monitor.getCurrentSnapshot();
 * qDebug() << "内存:" << snapshot.physicalMemoryMB << "MB";
 *
 * // 获取统计
 * auto stats = monitor.getStatistics();
 * qDebug() << "内存增长率:" << stats.memoryGrowthRateKBh << "KB/h";
 *
 * // 检测内存泄漏
 * if (monitor.isMemoryLeaking()) {
 *     qWarning() << "检测到内存泄漏!";
 * }
 * @endcode
 */
class SystemResourceMonitor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static SystemResourceMonitor& instance();

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
    ResourceSnapshot getCurrentSnapshot() const;

    /**
     * @brief 获取历史快照
     * @param seconds 获取最近N秒的数据(0=全部)
     * @return 快照列表
     */
    QList<ResourceSnapshot> getHistory(int seconds = 0) const;

    /**
     * @brief 获取统计摘要
     */
    ResourceStats getStatistics() const;

    // ========== 分析功能 ==========

    /**
     * @brief 检测是否存在内存泄漏
     * @param thresholdKBs 阈值(KB/s)
     * @return true=检测到泄漏
     */
    bool isMemoryLeaking(double thresholdKBs = 1.0) const;

    /**
     * @brief 获取内存增长率
     * @return 增长率(KB/h)
     */
    double getMemoryGrowthRate() const;

    /**
     * @brief 获取平均CPU使用率
     * @param seconds 计算最近N秒(0=全部)
     */
    double getAvgCpuUsage(int seconds = 0) const;

    /**
     * @brief 检测CPU使用率是否异常高
     * @param thresholdPercent 阈值(%)
     */
    bool isHighCpuUsage(double thresholdPercent = 80.0) const;

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
    void snapshotReady(const ResourceSnapshot& snapshot);

    /**
     * @brief 检测到内存泄漏
     * @param rateKBs 泄漏率(KB/s)
     */
    void memoryLeakDetected(double rateKBs);

    /**
     * @brief 检测到高CPU使用率
     * @param percent CPU使用率(%)
     */
    void highCpuUsage(double percent);

    /**
     * @brief 监控错误
     */
    void monitorError(const QString& error);

private:
    SystemResourceMonitor();
    ~SystemResourceMonitor();
    SystemResourceMonitor(const SystemResourceMonitor&) = delete;
    SystemResourceMonitor& operator=(const SystemResourceMonitor&) = delete;

private slots:
    void onMonitorTimer();

private:
    void collectSnapshot();
    void analyzeSnapshot(const ResourceSnapshot& snapshot);

#ifdef _WIN32
    void collectWindows(ResourceSnapshot& snapshot);
#elif defined(__linux__)
    void collectLinux(ResourceSnapshot& snapshot);
#elif defined(__APPLE__)
    void collectMacOS(ResourceSnapshot& snapshot);
#endif

    ResourceStats calculateStats() const;

private:
    QTimer* timer_;
    mutable QMutex mutex_;
    QList<ResourceSnapshot> history_;
    int maxHistorySize_;
    std::atomic<bool> isMonitoring_;

    qint64 startTimestamp_;
    qint64 lastCpuTime_;
    qint64 lastSystemTime_;

#ifdef _WIN32
    void* processHandle_;           ///< Windows进程句柄
#endif
};

} // namespace Base
} // namespace VisionForge
