/**
 * @file SystemMonitor.cpp
 * @brief 系统资源监控器实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "base/SystemMonitor.h"
#include "base/Logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QStorageInfo>
#include <QThread>

#ifdef Q_OS_WIN
#pragma comment(lib, "psapi.lib")
#endif

namespace VisionForge {
namespace Base {

SystemMonitor::SystemMonitor()
    : QObject(nullptr)
    , timer_(nullptr)
    , maxHistorySize_(3600)  // 默认保留1小时数据（每秒一条）
    , lastCpuLevel_(ResourceWarningLevel::Normal)
    , lastMemoryLevel_(ResourceWarningLevel::Normal)
    , lastDiskLevel_(ResourceWarningLevel::Normal)
{
    // 初始化CPU计时器
#ifdef Q_OS_WIN
    lastCpuIdle_.QuadPart = 0;
    lastCpuKernel_.QuadPart = 0;
    lastCpuUser_.QuadPart = 0;
    memset(&lastProcessKernel_, 0, sizeof(lastProcessKernel_));
    memset(&lastProcessUser_, 0, sizeof(lastProcessUser_));
#endif

    // 获取CPU核心数
    currentSnapshot_.cpuCoreCount = QThread::idealThreadCount();
}

SystemMonitor::~SystemMonitor()
{
    stop();
}

SystemMonitor& SystemMonitor::instance()
{
    static SystemMonitor instance;
    return instance;
}

void SystemMonitor::start(int intervalMs)
{
    if (timer_) {
        timer_->stop();
        delete timer_;
    }

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &SystemMonitor::onTimer);
    timer_->start(intervalMs);

    // 立即获取一次
    refresh();

    LOG_INFO(tr("系统监控已启动，间隔: %1ms").arg(intervalMs));
}

void SystemMonitor::stop()
{
    if (timer_) {
        timer_->stop();
        delete timer_;
        timer_ = nullptr;
    }

    LOG_INFO(tr("系统监控已停止"));
}

SystemSnapshot SystemMonitor::currentSnapshot() const
{
    return currentSnapshot_;
}

SystemSnapshot SystemMonitor::refresh()
{
    updateSnapshot();
    return currentSnapshot_;
}

QVector<SystemSnapshot> SystemMonitor::history(int maxCount) const
{
    if (maxCount <= 0 || maxCount >= history_.size()) {
        return history_;
    }
    return history_.mid(history_.size() - maxCount);
}

void SystemMonitor::setMaxHistorySize(int maxSize)
{
    maxHistorySize_ = maxSize;
    if (maxSize > 0 && history_.size() > maxSize) {
        history_ = history_.mid(history_.size() - maxSize);
    }
}

void SystemMonitor::setThresholds(const ResourceThresholds& thresholds)
{
    thresholds_ = thresholds;
}

ResourceWarningLevel SystemMonitor::cpuWarningLevel() const
{
    return calculateLevel(currentSnapshot_.cpuUsage, thresholds_.cpuWarning, thresholds_.cpuCritical);
}

ResourceWarningLevel SystemMonitor::memoryWarningLevel() const
{
    return calculateLevel(currentSnapshot_.memoryUsage, thresholds_.memoryWarning, thresholds_.memoryCritical);
}

ResourceWarningLevel SystemMonitor::diskWarningLevel() const
{
    return calculateLevel(currentSnapshot_.diskUsage, thresholds_.diskWarning, thresholds_.diskCritical);
}

ResourceWarningLevel SystemMonitor::overallWarningLevel() const
{
    ResourceWarningLevel cpu = cpuWarningLevel();
    ResourceWarningLevel mem = memoryWarningLevel();
    ResourceWarningLevel disk = diskWarningLevel();

    // 返回最高级别
    int maxLevel = qMax(qMax(static_cast<int>(cpu), static_cast<int>(mem)), static_cast<int>(disk));
    return static_cast<ResourceWarningLevel>(maxLevel);
}

QString SystemMonitor::formatBytes(qint64 bytes)
{
    if (bytes < 1024) {
        return QString("%1 B").arg(bytes);
    } else if (bytes < 1024 * 1024) {
        return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    } else if (bytes < 1024 * 1024 * 1024) {
        return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    } else {
        return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
}

QString SystemMonitor::warningLevelName(ResourceWarningLevel level)
{
    switch (level) {
    case ResourceWarningLevel::Normal:   return tr("正常");
    case ResourceWarningLevel::Low:      return tr("低警告");
    case ResourceWarningLevel::Medium:   return tr("中警告");
    case ResourceWarningLevel::High:     return tr("高警告");
    case ResourceWarningLevel::Critical: return tr("危险");
    default:                             return tr("未知");
    }
}

void SystemMonitor::onTimer()
{
    updateSnapshot();
}

void SystemMonitor::updateSnapshot()
{
    currentSnapshot_.timestamp = QDateTime::currentDateTime();

    updateCpuUsage();
    updateMemoryUsage();
    updateDiskUsage();
    updateProcessInfo();

    // 保存历史
    history_.append(currentSnapshot_);
    if (maxHistorySize_ > 0 && history_.size() > maxHistorySize_) {
        history_.removeFirst();
    }

    // 检查警告
    checkWarnings();

    emit resourceUpdated(currentSnapshot_);
}

void SystemMonitor::updateCpuUsage()
{
#ifdef Q_OS_WIN
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        ULARGE_INTEGER idle, kernel, user;
        idle.LowPart = idleTime.dwLowDateTime;
        idle.HighPart = idleTime.dwHighDateTime;
        kernel.LowPart = kernelTime.dwLowDateTime;
        kernel.HighPart = kernelTime.dwHighDateTime;
        user.LowPart = userTime.dwLowDateTime;
        user.HighPart = userTime.dwHighDateTime;

        if (lastCpuIdle_.QuadPart != 0) {
            ULONGLONG idleDiff = idle.QuadPart - lastCpuIdle_.QuadPart;
            ULONGLONG kernelDiff = kernel.QuadPart - lastCpuKernel_.QuadPart;
            ULONGLONG userDiff = user.QuadPart - lastCpuUser_.QuadPart;
            ULONGLONG totalDiff = kernelDiff + userDiff;

            if (totalDiff > 0) {
                currentSnapshot_.cpuUsage = 100.0 * (totalDiff - idleDiff) / totalDiff;
            }
        }

        lastCpuIdle_ = idle;
        lastCpuKernel_ = kernel;
        lastCpuUser_ = user;
    }
#else
    // Linux/macOS实现可以通过读取/proc/stat或sysctl
    currentSnapshot_.cpuUsage = 0.0;
#endif
}

void SystemMonitor::updateMemoryUsage()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        currentSnapshot_.totalMemory = memStatus.ullTotalPhys;
        currentSnapshot_.availableMemory = memStatus.ullAvailPhys;
        currentSnapshot_.usedMemory = memStatus.ullTotalPhys - memStatus.ullAvailPhys;
        currentSnapshot_.memoryUsage = memStatus.dwMemoryLoad;
    }
#else
    // Linux/macOS实现
    currentSnapshot_.totalMemory = 0;
    currentSnapshot_.availableMemory = 0;
    currentSnapshot_.usedMemory = 0;
    currentSnapshot_.memoryUsage = 0.0;
#endif
}

void SystemMonitor::updateDiskUsage()
{
    // 获取程序所在磁盘信息
    QString appPath = QCoreApplication::applicationDirPath();
    QStorageInfo storage(appPath);

    if (storage.isValid()) {
        currentSnapshot_.totalDisk = storage.bytesTotal();
        currentSnapshot_.freeDisk = storage.bytesAvailable();

        if (currentSnapshot_.totalDisk > 0) {
            currentSnapshot_.diskUsage = 100.0 *
                (currentSnapshot_.totalDisk - currentSnapshot_.freeDisk) /
                currentSnapshot_.totalDisk;
        }
    }
}

void SystemMonitor::updateProcessInfo()
{
#ifdef Q_OS_WIN
    HANDLE hProcess = GetCurrentProcess();

    // 获取进程内存
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        currentSnapshot_.processMemory = pmc.WorkingSetSize;
    }

    // 获取进程CPU
    FILETIME createTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime)) {
        QDateTime now = QDateTime::currentDateTime();

        if (lastProcessTime_.isValid()) {
            ULARGE_INTEGER kernel, user;
            kernel.LowPart = kernelTime.dwLowDateTime;
            kernel.HighPart = kernelTime.dwHighDateTime;
            user.LowPart = userTime.dwLowDateTime;
            user.HighPart = userTime.dwHighDateTime;

            ULARGE_INTEGER lastKernel, lastUser;
            lastKernel.LowPart = lastProcessKernel_.dwLowDateTime;
            lastKernel.HighPart = lastProcessKernel_.dwHighDateTime;
            lastUser.LowPart = lastProcessUser_.dwLowDateTime;
            lastUser.HighPart = lastProcessUser_.dwHighDateTime;

            ULONGLONG kernelDiff = kernel.QuadPart - lastKernel.QuadPart;
            ULONGLONG userDiff = user.QuadPart - lastUser.QuadPart;
            ULONGLONG totalCpu = kernelDiff + userDiff;

            // 计算时间差（100纳秒单位）
            qint64 timeDiffMs = lastProcessTime_.msecsTo(now);
            if (timeDiffMs > 0) {
                double cpuTimeMs = totalCpu / 10000.0;  // 转换为毫秒
                currentSnapshot_.processCpuUsage = 100.0 * cpuTimeMs / timeDiffMs / currentSnapshot_.cpuCoreCount;
            }
        }

        lastProcessKernel_ = kernelTime;
        lastProcessUser_ = userTime;
        lastProcessTime_ = now;
    }
#else
    currentSnapshot_.processMemory = 0;
    currentSnapshot_.processCpuUsage = 0.0;
#endif
}

void SystemMonitor::checkWarnings()
{
    // CPU警告
    ResourceWarningLevel cpuLevel = cpuWarningLevel();
    if (cpuLevel != lastCpuLevel_ && cpuLevel >= ResourceWarningLevel::Medium) {
        emit cpuWarning(currentSnapshot_.cpuUsage, cpuLevel);
        if (cpuLevel >= ResourceWarningLevel::High) {
            LOG_WARNING(tr("CPU使用率过高: %1%").arg(currentSnapshot_.cpuUsage, 0, 'f', 1));
        }
    }
    lastCpuLevel_ = cpuLevel;

    // 内存警告
    ResourceWarningLevel memLevel = memoryWarningLevel();
    if (memLevel != lastMemoryLevel_ && memLevel >= ResourceWarningLevel::Medium) {
        emit memoryWarning(currentSnapshot_.memoryUsage, memLevel);
        if (memLevel >= ResourceWarningLevel::High) {
            LOG_WARNING(tr("内存使用率过高: %1%").arg(currentSnapshot_.memoryUsage, 0, 'f', 1));
        }
    }
    lastMemoryLevel_ = memLevel;

    // 磁盘警告
    ResourceWarningLevel diskLevel = diskWarningLevel();
    if (diskLevel != lastDiskLevel_ && diskLevel >= ResourceWarningLevel::Medium) {
        emit diskWarning(currentSnapshot_.diskUsage, diskLevel);
        if (diskLevel >= ResourceWarningLevel::High) {
            LOG_WARNING(tr("磁盘使用率过高: %1%").arg(currentSnapshot_.diskUsage, 0, 'f', 1));
        }
    }
    lastDiskLevel_ = diskLevel;
}

ResourceWarningLevel SystemMonitor::calculateLevel(double usage, double warning, double critical) const
{
    if (usage >= critical) {
        return ResourceWarningLevel::Critical;
    } else if (usage >= (warning + critical) / 2) {
        return ResourceWarningLevel::High;
    } else if (usage >= warning) {
        return ResourceWarningLevel::Medium;
    } else if (usage >= warning * 0.8) {
        return ResourceWarningLevel::Low;
    }
    return ResourceWarningLevel::Normal;
}

} // namespace Base
} // namespace VisionForge
