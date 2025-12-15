/**
 * @file PerformanceMonitor.cpp
 * @brief 性能监控工具实现
 */

#include "base/PerformanceMonitor.h"
#include "base/Logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <algorithm>

namespace VisionForge {
namespace Base {

// ========== ScopedTimer实现 ==========

ScopedTimer::ScopedTimer(const QString& name)
    : name_(name)
{
    timer_.start();
}

ScopedTimer::~ScopedTimer()
{
    qint64 elapsed = timer_.elapsed();

    if (PerformanceMonitor::instance().isEnabled()) {
        PerformanceMonitor::instance().record(name_, elapsed);
    }
}

qint64 ScopedTimer::elapsed() const
{
    return timer_.elapsed();
}

// ========== PerformanceMonitor实现 ==========

PerformanceMonitor::PerformanceMonitor()
    : enabled_(true)
{
}

PerformanceMonitor& PerformanceMonitor::instance()
{
    static PerformanceMonitor instance;
    return instance;
}

void PerformanceMonitor::record(const QString& name, qint64 elapsedMs)
{
    if (!enabled_) {
        return;
    }

    QMutexLocker locker(&mutex_);

    PerformanceStats& stats = stats_[name];
    stats.name = name;
    stats.totalTime += elapsedMs;
    stats.minTime = std::min(stats.minTime, elapsedMs);
    stats.maxTime = std::max(stats.maxTime, elapsedMs);
    stats.callCount++;
    stats.avgTime = static_cast<double>(stats.totalTime) / stats.callCount;
}

PerformanceStats PerformanceMonitor::getStats(const QString& name) const
{
    QMutexLocker locker(&mutex_);

    if (stats_.contains(name)) {
        return stats_[name];
    }

    return PerformanceStats();
}

QMap<QString, PerformanceStats> PerformanceMonitor::getAllStats() const
{
    QMutexLocker locker(&mutex_);
    return stats_;
}

void PerformanceMonitor::reset()
{
    QMutexLocker locker(&mutex_);
    stats_.clear();
    LOG_INFO("性能统计已重置");
}

void PerformanceMonitor::reset(const QString& name)
{
    QMutexLocker locker(&mutex_);

    if (stats_.contains(name)) {
        stats_.remove(name);
        LOG_INFO(QString("性能统计已重置: %1").arg(name));
    }
}

void PerformanceMonitor::printReport(int topN) const
{
    QMutexLocker locker(&mutex_);

    if (stats_.isEmpty()) {
        qInfo() << "没有性能统计数据";
        return;
    }

    // 转换为列表并按平均时间排序
    QList<PerformanceStats> statsList;
    for (const PerformanceStats& stats : stats_) {
        statsList.append(stats);
    }

    std::sort(statsList.begin(), statsList.end(),
             [](const PerformanceStats& a, const PerformanceStats& b) {
        return a.avgTime > b.avgTime;
    });

    // 打印报告
    qInfo() << "========================================";
    qInfo() << "性能分析报告";
    qInfo() << QString("生成时间: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    qInfo() << QString("总项目数: %1").arg(statsList.size());
    qInfo() << "========================================";
    qInfo() << QString("%1  %2  %3  %4  %5  %6")
               .arg("名称", -30)
               .arg("调用次数", -10)
               .arg("总时间(ms)", -12)
               .arg("平均(ms)", -12)
               .arg("最小(ms)", -12)
               .arg("最大(ms)", -12);
    qInfo() << "----------------------------------------";

    int count = (topN > 0) ? std::min(topN, static_cast<int>(statsList.size())) : statsList.size();

    for (int i = 0; i < count; ++i) {
        const PerformanceStats& stats = statsList[i];
        qInfo() << QString("%1  %2  %3  %4  %5  %6")
                   .arg(stats.name, -30)
                   .arg(stats.callCount, -10)
                   .arg(stats.totalTime, -12)
                   .arg(stats.avgTime, -12, 'f', 2)
                   .arg(stats.minTime, -12)
                   .arg(stats.maxTime, -12);
    }

    qInfo() << "========================================";
}

bool PerformanceMonitor::exportReport(const QString& filePath) const
{
    QMutexLocker locker(&mutex_);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR(QString("导出性能报告失败：无法打开文件 %1").arg(filePath));
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 转换为列表并按平均时间排序
    QList<PerformanceStats> statsList;
    for (const PerformanceStats& stats : stats_) {
        statsList.append(stats);
    }

    std::sort(statsList.begin(), statsList.end(),
             [](const PerformanceStats& a, const PerformanceStats& b) {
        return a.avgTime > b.avgTime;
    });

    // 写入报告
    out << "========================================\n";
    out << "性能分析报告\n";
    out << QString("生成时间: %1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    out << QString("总项目数: %1\n").arg(statsList.size());
    out << "========================================\n";
    out << QString("%1,%2,%3,%4,%5,%6\n")
           .arg("名称")
           .arg("调用次数")
           .arg("总时间(ms)")
           .arg("平均时间(ms)")
           .arg("最小时间(ms)")
           .arg("最大时间(ms)");

    for (const PerformanceStats& stats : statsList) {
        out << QString("%1,%2,%3,%4,%5,%6\n")
               .arg(stats.name)
               .arg(stats.callCount)
               .arg(stats.totalTime)
               .arg(stats.avgTime, 0, 'f', 2)
               .arg(stats.minTime)
               .arg(stats.maxTime);
    }

    file.close();

    LOG_INFO(QString("性能报告已导出到: %1").arg(filePath));
    return true;
}

} // namespace Base
} // namespace VisionForge
