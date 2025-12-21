/**
 * @file PoolPerformanceMonitor.cpp
 * @brief 内存池性能监控器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "base/PoolPerformanceMonitor.h"
#include "base/ImageMemoryPool.h"
#include "base/Logger.h"
#include <QDateTime>
#include <QtMath>

namespace VisionForge {
namespace Base {

// ========== PoolSnapshot构造函数 ==========

PoolSnapshot::PoolSnapshot()
    : timestamp(0)
    , totalAllocations(0)
    , poolHits(0)
    , poolMisses(0)
    , hitRate(0.0)
    , poolSize(0)
    , poolCapacity(0)
    , inUseCount(0)
    , memoryUsageMB(0)
    , memoryPeakMB(0)
    , avgAllocTimeUs(0.0)
    , avgReleaseTimeUs(0.0)
{
}

// ========== PoolPerformanceStats构造函数 ==========

PoolPerformanceStats::PoolPerformanceStats()
    : avgHitRate(0.0)
    , minHitRate(0.0)
    , currentHitRate(0.0)
    , avgPoolSize(0)
    , peakPoolSize(0)
    , avgInUse(0)
    , avgMemoryMB(0)
    , peakMemoryMB(0)
    , avgAllocTimeUs(0.0)
    , maxAllocTimeUs(0.0)
{
}

// ========== PoolPerformanceMonitor实现 ==========

PoolPerformanceMonitor::PoolPerformanceMonitor()
    : timer_(new QTimer(this))
    , maxHistorySize_(3600)  // 默认1小时@1秒采样
    , isMonitoring_(false)
    , startTimestamp_(0)
{
    connect(timer_, &QTimer::timeout, this, &PoolPerformanceMonitor::onMonitorTimer);

    LOG_INFO("PoolPerformanceMonitor初始化完成");
}

PoolPerformanceMonitor::~PoolPerformanceMonitor()
{
    stop();
    LOG_INFO("PoolPerformanceMonitor销毁");
}

PoolPerformanceMonitor& PoolPerformanceMonitor::instance()
{
    static PoolPerformanceMonitor instance;
    return instance;
}

void PoolPerformanceMonitor::start(int intervalMs)
{
    if (isMonitoring_.load()) {
        LOG_WARNING("内存池监控已在运行中");
        return;
    }

    QMutexLocker locker(&mutex_);

    // 重置状态
    history_.clear();
    startTimestamp_ = QDateTime::currentMSecsSinceEpoch();
    lastSnapshot_ = PoolSnapshot();

    // 启动定时器
    timer_->start(intervalMs);
    isMonitoring_.store(true);

    LOG_INFO(QString("开始监控内存池性能: 采样间隔%1ms").arg(intervalMs));

    // 立即采集第一个快照
    collectSnapshot();
}

void PoolPerformanceMonitor::stop()
{
    if (!isMonitoring_.load()) {
        return;
    }

    timer_->stop();
    isMonitoring_.store(false);

    LOG_INFO("停止内存池性能监控");
}

void PoolPerformanceMonitor::reset()
{
    QMutexLocker locker(&mutex_);

    history_.clear();
    startTimestamp_ = QDateTime::currentMSecsSinceEpoch();
    lastSnapshot_ = PoolSnapshot();

    LOG_INFO("内存池监控数据已重置");
}

PoolSnapshot PoolPerformanceMonitor::getCurrentSnapshot() const
{
    QMutexLocker locker(&mutex_);

    if (history_.isEmpty()) {
        return PoolSnapshot();
    }

    return history_.last();
}

QList<PoolSnapshot> PoolPerformanceMonitor::getHistory(int seconds) const
{
    QMutexLocker locker(&mutex_);

    if (seconds <= 0) {
        return history_;
    }

    qint64 cutoffTime = QDateTime::currentMSecsSinceEpoch() - (seconds * 1000);

    QList<PoolSnapshot> recent;
    for (const auto& snapshot : history_) {
        if (snapshot.timestamp >= cutoffTime) {
            recent.append(snapshot);
        }
    }

    return recent;
}

PoolPerformanceStats PoolPerformanceMonitor::getStatistics() const
{
    QMutexLocker locker(&mutex_);
    return calculateStats();
}

bool PoolPerformanceMonitor::isEfficiencyDegraded(double thresholdPercent) const
{
    QMutexLocker locker(&mutex_);

    if (history_.isEmpty()) {
        return false;
    }

    return history_.last().hitRate < thresholdPercent;
}

bool PoolPerformanceMonitor::isNearCapacity(double thresholdPercent) const
{
    QMutexLocker locker(&mutex_);

    if (history_.isEmpty()) {
        return false;
    }

    const auto& snapshot = history_.last();

    if (snapshot.poolCapacity == 0) {
        return false;
    }

    double usagePercent = (snapshot.poolSize * 100.0) / snapshot.poolCapacity;
    return usagePercent > thresholdPercent;
}

double PoolPerformanceMonitor::getHitRateTrend(int seconds) const
{
    QMutexLocker locker(&mutex_);

    QList<PoolSnapshot> data = (seconds > 0) ?
        getHistory(seconds) : history_;

    if (data.size() < 2) {
        return 0.0;
    }

    // 使用最小二乘法计算线性趋势
    double sumX = 0.0;  // 时间总和
    double sumY = 0.0;  // 命中率总和
    double sumXY = 0.0;
    double sumX2 = 0.0;
    int n = data.size();

    qint64 baseTime = data.first().timestamp;

    for (const auto& snapshot : data) {
        double x = (snapshot.timestamp - baseTime) / 1000.0;  // 秒
        double y = snapshot.hitRate;                          // %

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    // 斜率 = (n*sumXY - sumX*sumY) / (n*sumX2 - sumX*sumX)
    double denominator = n * sumX2 - sumX * sumX;
    if (qAbs(denominator) < 1e-6) {
        return 0.0;
    }

    double slope = (n * sumXY - sumX * sumY) / denominator;  // %/s

    return slope * 3600.0;  // 转换为%/h
}

QString PoolPerformanceMonitor::generateReport() const
{
    QMutexLocker locker(&mutex_);

    PoolPerformanceStats stats = calculateStats();

    QString report;
    report += "========== 内存池性能监控报告 ==========\n\n";

    // 监控时长
    if (!history_.isEmpty()) {
        qint64 duration = (history_.last().timestamp - history_.first().timestamp) / 1000;
        int hours = duration / 3600;
        int minutes = (duration % 3600) / 60;
        int seconds = duration % 60;
        report += QString("监控时长: %1时%2分%3秒\n")
                  .arg(hours).arg(minutes).arg(seconds);
    }

    report += QString("采样数量: %1\n\n").arg(history_.size());

    // 命中率统计
    report += "【命中率统计】\n";
    report += QString("  当前命中率: %1%\n").arg(stats.currentHitRate, 0, 'f', 1);
    report += QString("  平均命中率: %1%\n").arg(stats.avgHitRate, 0, 'f', 1);
    report += QString("  最低命中率: %1%\n\n").arg(stats.minHitRate, 0, 'f', 1);

    // 池使用统计
    report += "【池使用统计】\n";
    report += QString("  平均池大小: %1 对象\n").arg(stats.avgPoolSize);
    report += QString("  峰值池大小: %1 对象\n").arg(stats.peakPoolSize);
    report += QString("  平均使用中: %1 对象\n\n").arg(stats.avgInUse);

    // 内存统计
    report += "【内存统计】\n";
    report += QString("  平均内存使用: %1 MB\n").arg(stats.avgMemoryMB);
    report += QString("  峰值内存使用: %1 MB\n\n").arg(stats.peakMemoryMB);

    // 性能统计
    report += "【性能统计】\n";
    report += QString("  平均分配时间: %1 微秒\n").arg(stats.avgAllocTimeUs, 0, 'f', 2);
    report += QString("  最大分配时间: %1 微秒\n\n").arg(stats.maxAllocTimeUs, 0, 'f', 2);

    // 健康评估
    report += "【健康评估】\n";

    if (stats.currentHitRate < 50.0) {
        report += "  ⚠️ 严重警告: 命中率严重偏低，池效率极差\n";
    } else if (stats.currentHitRate < 75.0) {
        report += "  ⚠️ 警告: 命中率偏低，池效率不佳\n";
    } else if (stats.currentHitRate > 90.0) {
        report += "  ✅ 优秀: 命中率很高，池效率出色\n";
    } else {
        report += "  ✅ 良好: 命中率正常\n";
    }

    // 趋势分析
    double trend = getHitRateTrend(300);  // 最近5分钟趋势
    if (trend < -5.0) {
        report += "  ⚠️ 注意: 命中率呈下降趋势\n";
    } else if (trend > 5.0) {
        report += "  ✅ 命中率呈上升趋势\n";
    } else {
        report += "  ✅ 命中率稳定\n";
    }

    report += "\n========================================\n";

    return report;
}

void PoolPerformanceMonitor::setMaxHistorySize(int count)
{
    QMutexLocker locker(&mutex_);
    maxHistorySize_ = count;

    // 如果当前历史超过新限制，裁剪掉旧数据
    while (history_.size() > maxHistorySize_) {
        history_.removeFirst();
    }
}

int PoolPerformanceMonitor::historySize() const
{
    QMutexLocker locker(&mutex_);
    return history_.size();
}

void PoolPerformanceMonitor::onMonitorTimer()
{
    collectSnapshot();
}

void PoolPerformanceMonitor::collectSnapshot()
{
    PoolSnapshot snapshot;
    snapshot.timestamp = QDateTime::currentMSecsSinceEpoch();

    try {
        // 从ImageMemoryPool获取统计信息
        auto& pool = ImageMemoryPool::instance();
        auto poolStats = pool.getStatistics();

        // 填充快照数据
        snapshot.totalAllocations = poolStats.totalAllocations;
        snapshot.poolHits = poolStats.cacheHits;          // 使用cacheHits
        snapshot.poolMisses = poolStats.cacheMisses;      // 使用cacheMisses
        snapshot.hitRate = poolStats.hitRate * 100.0;     // 转换为百分比

        snapshot.poolSize = poolStats.poolSize;
        snapshot.poolCapacity = poolStats.memoryLimit / (1920 * 1080 * 3);  // 估算容量（假设1080p图像）
        snapshot.inUseCount = 0;  // TODO: 池统计不支持inUseCount，暂时设为0

        snapshot.memoryUsageMB = poolStats.memoryUsage / (1024 * 1024);
        snapshot.memoryPeakMB = snapshot.memoryUsageMB;  // TODO: 池统计不支持峰值，使用当前值

        snapshot.avgAllocTimeUs = 0.0;  // TODO: 池统计不支持分配时间
        snapshot.avgReleaseTimeUs = 0.0;  // TODO: 池统计不支持释放时间

        // 分析快照
        analyzeSnapshot(snapshot);

        // 添加到历史记录
        {
            QMutexLocker locker(&mutex_);
            history_.append(snapshot);

            // 限制历史记录大小
            if (history_.size() > maxHistorySize_) {
                history_.removeFirst();
            }

            // 保存为上一次快照
            lastSnapshot_ = snapshot;
        }

        // 发送信号
        emit snapshotReady(snapshot);

    } catch (const std::exception& e) {
        QString errorMsg = QString("采集池快照异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        emit monitorError(errorMsg);
    }
}

void PoolPerformanceMonitor::analyzeSnapshot(const PoolSnapshot& snapshot)
{
    // 检测效率退化
    if (snapshot.hitRate < 75.0) {
        emit efficiencyDegraded(snapshot.hitRate);
    }

    // 检测接近容量上限
    if (snapshot.poolCapacity > 0) {
        double usagePercent = (snapshot.poolSize * 100.0) / snapshot.poolCapacity;
        if (usagePercent > 90.0) {
            emit nearCapacity(usagePercent);
        }
    }
}

PoolPerformanceStats PoolPerformanceMonitor::calculateStats() const
{
    PoolPerformanceStats stats;

    if (history_.isEmpty()) {
        return stats;
    }

    // 命中率统计
    double hitRateSum = 0.0;
    stats.currentHitRate = history_.last().hitRate;
    stats.minHitRate = 100.0;

    for (const auto& snapshot : history_) {
        hitRateSum += snapshot.hitRate;

        if (snapshot.hitRate < stats.minHitRate) {
            stats.minHitRate = snapshot.hitRate;
        }
    }

    stats.avgHitRate = hitRateSum / history_.size();

    // 池使用统计
    size_t poolSizeSum = 0;
    size_t inUseSum = 0;
    stats.peakPoolSize = 0;

    for (const auto& snapshot : history_) {
        poolSizeSum += snapshot.poolSize;
        inUseSum += snapshot.inUseCount;

        if (snapshot.poolSize > stats.peakPoolSize) {
            stats.peakPoolSize = snapshot.poolSize;
        }
    }

    stats.avgPoolSize = poolSizeSum / history_.size();
    stats.avgInUse = inUseSum / history_.size();

    // 内存统计
    size_t memorySum = 0;
    stats.peakMemoryMB = 0;

    for (const auto& snapshot : history_) {
        memorySum += snapshot.memoryUsageMB;

        if (snapshot.memoryUsageMB > stats.peakMemoryMB) {
            stats.peakMemoryMB = snapshot.memoryUsageMB;
        }
    }

    stats.avgMemoryMB = memorySum / history_.size();

    // 性能统计
    double allocTimeSum = 0.0;
    stats.maxAllocTimeUs = 0.0;

    for (const auto& snapshot : history_) {
        allocTimeSum += snapshot.avgAllocTimeUs;

        if (snapshot.avgAllocTimeUs > stats.maxAllocTimeUs) {
            stats.maxAllocTimeUs = snapshot.avgAllocTimeUs;
        }
    }

    stats.avgAllocTimeUs = allocTimeSum / history_.size();

    return stats;
}

} // namespace Base
} // namespace VisionForge
