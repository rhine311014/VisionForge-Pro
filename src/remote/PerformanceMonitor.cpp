/**
 * @file PerformanceMonitor.cpp
 * @brief 性能监控器实现
 */

#include "remote/PerformanceMonitor.h"
#include <QDebug>
#include <algorithm>

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#pragma comment(lib, "psapi.lib")
#endif

namespace VisionForge {
namespace Remote {

// ========== 构造和析构 ==========

PerformanceMonitor::PerformanceMonitor(QObject* parent)
    : QObject(parent)
    , samplingTimer_(nullptr)
    , samplingIntervalMs_(1000)
    , statWindowSize_(60)
    , isFrameProcessing_(false)
    , lastCPUTime_(0)
    , lastSystemTime_(0)
    , errorCount_(0)
    , warningCount_(0)
{
}

// ========== 监控控制 ==========

void PerformanceMonitor::start(int intervalMs)
{
    if (isRunning()) {
        qWarning() << "性能监控器已经在运行";
        return;
    }

    samplingIntervalMs_ = intervalMs;

    // 创建并启动定时器
    samplingTimer_ = new QTimer(this);
    connect(samplingTimer_, &QTimer::timeout, this, &PerformanceMonitor::onSamplingTimeout);
    samplingTimer_->start(samplingIntervalMs_);

    // 初始化CPU监控
    lastCPUTime_ = 0;
    lastSystemTime_ = 0;

    qInfo() << QString("性能监控器已启动，采样间隔: %1ms").arg(intervalMs);
}

void PerformanceMonitor::stop()
{
    if (!isRunning()) {
        return;
    }

    samplingTimer_->stop();
    samplingTimer_->deleteLater();
    samplingTimer_ = nullptr;

    qInfo() << "性能监控器已停止";
}

bool PerformanceMonitor::isRunning() const
{
    return samplingTimer_ != nullptr && samplingTimer_->isActive();
}

void PerformanceMonitor::setSamplingInterval(int intervalMs)
{
    samplingIntervalMs_ = intervalMs;
    if (isRunning()) {
        samplingTimer_->setInterval(intervalMs);
    }
}

void PerformanceMonitor::setStatWindowSize(int size)
{
    QMutexLocker locker(&mutex_);
    statWindowSize_ = size;

    // 调整历史记录大小
    while (static_cast<int>(statsHistory_.size()) > statWindowSize_) {
        statsHistory_.pop_front();
    }
}

// ========== 帧处理记录 ==========

void PerformanceMonitor::recordFrameStart()
{
    QMutexLocker locker(&mutex_);
    frameTimer_.start();
    isFrameProcessing_ = true;
}

void PerformanceMonitor::recordFrameEnd(bool success)
{
    QMutexLocker locker(&mutex_);

    if (!isFrameProcessing_) {
        return;
    }

    qint64 elapsedMs = frameTimer_.elapsed();
    isFrameProcessing_ = false;

    // 记录帧处理数据
    FrameRecord record;
    record.timestamp = QDateTime::currentMSecsSinceEpoch();
    record.processingTimeMs = elapsedMs;
    record.success = success;

    frameRecords_.push_back(record);

    // 限制记录数量
    while (frameRecords_.size() > static_cast<size_t>(statWindowSize_)) {
        frameRecords_.pop_front();
    }

    // 更新总帧数
    currentStats_.totalFramesProcessed++;
}

void PerformanceMonitor::incrementErrorCount()
{
    QMutexLocker locker(&mutex_);
    errorCount_++;
}

void PerformanceMonitor::incrementWarningCount()
{
    QMutexLocker locker(&mutex_);
    warningCount_++;
}

void PerformanceMonitor::resetErrorCount()
{
    QMutexLocker locker(&mutex_);
    errorCount_ = 0;
}

void PerformanceMonitor::resetWarningCount()
{
    QMutexLocker locker(&mutex_);
    warningCount_ = 0;
}

// ========== 数据获取 ==========

PerformanceStats PerformanceMonitor::currentStats() const
{
    QMutexLocker locker(&mutex_);
    return currentStats_;
}

PerformanceMetrics PerformanceMonitor::currentMetrics() const
{
    QMutexLocker locker(&mutex_);

    PerformanceMetrics metrics;
    metrics.cpuUsage = currentStats_.cpuUsagePercent;
    metrics.memoryUsedMB = currentStats_.memoryUsedMB;
    metrics.fps = currentStats_.currentFPS;
    metrics.avgLatencyMs = currentStats_.avgLatencyMs;
    metrics.errorCount = currentStats_.errorCount;
    metrics.timestamp = currentStats_.timestamp;

    return metrics;
}

QList<PerformanceStats> PerformanceMonitor::history(int count) const
{
    QMutexLocker locker(&mutex_);

    QList<PerformanceStats> result;
    int start = (std::max)(0, static_cast<int>(statsHistory_.size()) - count);

    for (int i = start; i < static_cast<int>(statsHistory_.size()); ++i) {
        result.append(statsHistory_[i]);
    }

    return result;
}

// ========== 采样 ==========

void PerformanceMonitor::onSamplingTimeout()
{
    QMutexLocker locker(&mutex_);

    // 采集系统资源
    collectSystemResources();

    // 采集处理性能
    collectProcessingPerformance();

    // 计算统计数据
    calculateStatistics();

    // 更新时间戳
    currentStats_.timestamp = QDateTime::currentDateTime();

    // 保存到历史
    statsHistory_.push_back(currentStats_);
    while (statsHistory_.size() > static_cast<size_t>(statWindowSize_)) {
        statsHistory_.pop_front();
    }

    // 检查性能异常
    checkPerformanceIssues(currentStats_);

    // 发送信号
    PerformanceMetrics metrics = currentMetrics();
    emit metricsUpdated(metrics);
    emit statsUpdated(currentStats_);
}

void PerformanceMonitor::collectSystemResources()
{
    // 采集CPU使用率
    currentStats_.cpuUsagePercent = getCPUUsage();

    // 采集内存使用情况
    qint64 usedMB, totalMB;
    getMemoryUsage(usedMB, totalMB);
    currentStats_.memoryUsedMB = usedMB;
    currentStats_.memoryTotalMB = totalMB;

    // 采集线程数
    currentStats_.threadCount = getThreadCount();
}

void PerformanceMonitor::collectProcessingPerformance()
{
    if (frameRecords_.empty()) {
        currentStats_.currentFPS = 0.0;
        currentStats_.currentLatencyMs = 0.0;
        return;
    }

    // 计算当前FPS（最近1秒内的帧数）
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 oneSecondAgo = currentTime - 1000;

    int framesInLastSecond = 0;
    double totalLatency = 0.0;
    int latencyCount = 0;

    for (auto it = frameRecords_.rbegin(); it != frameRecords_.rend(); ++it) {
        if (it->timestamp >= oneSecondAgo) {
            framesInLastSecond++;
            totalLatency += it->processingTimeMs;
            latencyCount++;
        } else {
            break;
        }
    }

    currentStats_.currentFPS = framesInLastSecond;
    currentStats_.currentLatencyMs = (latencyCount > 0) ? (totalLatency / latencyCount) : 0.0;

    // 更新错误计数
    currentStats_.errorCount = errorCount_;
    currentStats_.warningCount = warningCount_;
}

void PerformanceMonitor::calculateStatistics()
{
    if (frameRecords_.empty()) {
        currentStats_.avgFPS = 0.0;
        currentStats_.avgLatencyMs = 0.0;
        return;
    }

    // 计算平均FPS（整个窗口期间）
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 oldestTime = frameRecords_.front().timestamp;
    qint64 timeSpanMs = currentTime - oldestTime;

    if (timeSpanMs > 0) {
        currentStats_.avgFPS = (static_cast<double>(frameRecords_.size()) * 1000.0) / timeSpanMs;
    } else {
        currentStats_.avgFPS = 0.0;
    }

    // 计算平均延迟
    double totalLatency = 0.0;
    for (const auto& record : frameRecords_) {
        totalLatency += record.processingTimeMs;
    }
    currentStats_.avgLatencyMs = totalLatency / static_cast<double>(frameRecords_.size());
}

void PerformanceMonitor::checkPerformanceIssues(const PerformanceStats& stats)
{
    // CPU使用率过高
    if (stats.cpuUsagePercent > 90.0) {
        emit performanceWarning(QString("CPU使用率过高: %1%").arg(stats.cpuUsagePercent, 0, 'f', 1));
    }

    // 内存使用率过高
    if (stats.memoryTotalMB > 0) {
        double memoryUsagePercent = (stats.memoryUsedMB * 100.0) / stats.memoryTotalMB;
        if (memoryUsagePercent > 90.0) {
            emit performanceWarning(QString("内存使用率过高: %1%").arg(memoryUsagePercent, 0, 'f', 1));
        }
    }

    // 处理延迟过高
    if (stats.avgLatencyMs > 100.0) {
        emit performanceWarning(QString("处理延迟过高: %1ms").arg(stats.avgLatencyMs, 0, 'f', 1));
    }

    // 帧率过低
    if (stats.currentFPS > 0 && stats.currentFPS < 10.0) {
        emit performanceWarning(QString("帧率过低: %1 FPS").arg(stats.currentFPS, 0, 'f', 1));
    }
}

// ========== 系统资源采集（Windows实现）==========

#ifdef Q_OS_WIN

double PerformanceMonitor::getCPUUsage()
{
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return 0.0;
    }

    auto FileTimeToInt64 = [](const FILETIME& ft) -> qint64 {
        ULARGE_INTEGER uli;
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        return uli.QuadPart;
    };

    qint64 idle = FileTimeToInt64(idleTime);
    qint64 kernel = FileTimeToInt64(kernelTime);
    qint64 user = FileTimeToInt64(userTime);
    qint64 system = kernel + user;

    if (lastSystemTime_ == 0) {
        // 首次调用，记录初始值
        lastCPUTime_ = idle;
        lastSystemTime_ = system;
        return 0.0;
    }

    qint64 systemDelta = system - lastSystemTime_;
    qint64 idleDelta = idle - lastCPUTime_;

    lastCPUTime_ = idle;
    lastSystemTime_ = system;

    if (systemDelta == 0) {
        return 0.0;
    }

    double usage = 100.0 * (systemDelta - idleDelta) / systemDelta;
    return (std::max)(0.0, (std::min)(100.0, usage));
}

void PerformanceMonitor::getMemoryUsage(qint64& usedMB, qint64& totalMB)
{
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (GlobalMemoryStatusEx(&memStatus)) {
        totalMB = memStatus.ullTotalPhys / (1024 * 1024);
        usedMB = (memStatus.ullTotalPhys - memStatus.ullAvailPhys) / (1024 * 1024);
    } else {
        totalMB = 0;
        usedMB = 0;
    }
}

int PerformanceMonitor::getThreadCount()
{
    HANDLE hProcess = GetCurrentProcess();
    DWORD threadCount = 0;

    // 获取进程信息
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // 通过枚举线程计数（简化实现）
        HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hThreadSnap != INVALID_HANDLE_VALUE) {
            THREADENTRY32 te32;
            te32.dwSize = sizeof(THREADENTRY32);

            if (Thread32First(hThreadSnap, &te32)) {
                DWORD currentPid = GetCurrentProcessId();
                do {
                    if (te32.th32OwnerProcessID == currentPid) {
                        threadCount++;
                    }
                } while (Thread32Next(hThreadSnap, &te32));
            }
            CloseHandle(hThreadSnap);
        }
    }

    return threadCount;
}

#else
// Linux/macOS实现（简化版）

double PerformanceMonitor::getCPUUsage()
{
    // TODO: 实现Linux/macOS版本
    return 0.0;
}

void PerformanceMonitor::getMemoryUsage(qint64& usedMB, qint64& totalMB)
{
    // TODO: 实现Linux/macOS版本
    usedMB = 0;
    totalMB = 0;
}

int PerformanceMonitor::getThreadCount()
{
    // TODO: 实现Linux/macOS版本
    return 0;
}

#endif

} // namespace Remote
} // namespace VisionForge
