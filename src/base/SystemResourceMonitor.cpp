/**
 * @file SystemResourceMonitor.cpp
 * @brief 系统资源监控器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "base/SystemResourceMonitor.h"
#include "base/Logger.h"
#include <QDateTime>
#include <QElapsedTimer>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
    #include <tlhelp32.h>
    #pragma comment(lib, "psapi.lib")
#elif defined(__linux__)
    #include <unistd.h>
    #include <sys/resource.h>
    #include <fstream>
    #include <string>
#elif defined(__APPLE__)
    #include <mach/mach.h>
    #include <sys/resource.h>
#endif

namespace VisionForge {
namespace Base {

// ========== ResourceSnapshot构造函数 ==========

ResourceSnapshot::ResourceSnapshot()
    : timestamp(0)
    , virtualMemoryMB(0)
    , physicalMemoryMB(0)
    , privateMemoryMB(0)
    , workingSetMB(0)
    , cpuUsagePercent(0.0)
    , cpuUserPercent(0.0)
    , cpuKernelPercent(0.0)
    , threadCount(0)
    , handleCount(0)
    , readBytesSec(0)
    , writeBytesSec(0)
    , totalReadMB(0)
    , totalWriteMB(0)
{
}

// ========== ResourceStats构造函数 ==========

ResourceStats::ResourceStats()
    : memoryInitialMB(0)
    , memoryCurrentMB(0)
    , memoryPeakMB(0)
    , memoryGrowthRateKBh(0.0)
    , memoryGrowthTotalMB(0.0)
    , cpuAvgPercent(0.0)
    , cpuPeakPercent(0.0)
    , cpuMinPercent(0.0)
    , threadCountAvg(0)
    , threadCountPeak(0)
{
}

// ========== SystemResourceMonitor实现 ==========

SystemResourceMonitor::SystemResourceMonitor()
    : timer_(new QTimer(this))
    , maxHistorySize_(3600)  // 默认1小时@1秒采样
    , isMonitoring_(false)
    , startTimestamp_(0)
    , lastCpuTime_(0)
    , lastSystemTime_(0)
{
#ifdef _WIN32
    processHandle_ = GetCurrentProcess();
#endif

    connect(timer_, &QTimer::timeout, this, &SystemResourceMonitor::onMonitorTimer);

    LOG_INFO("SystemResourceMonitor初始化完成");
}

SystemResourceMonitor::~SystemResourceMonitor()
{
    stop();
    LOG_INFO("SystemResourceMonitor销毁");
}

SystemResourceMonitor& SystemResourceMonitor::instance()
{
    static SystemResourceMonitor instance;
    return instance;
}

void SystemResourceMonitor::start(int intervalMs)
{
    if (isMonitoring_.load()) {
        LOG_WARNING("监控已在运行中");
        return;
    }

    QMutexLocker locker(&mutex_);

    // 重置状态
    history_.clear();
    startTimestamp_ = QDateTime::currentMSecsSinceEpoch();
    lastCpuTime_ = 0;
    lastSystemTime_ = 0;

    // 启动定时器
    timer_->start(intervalMs);
    isMonitoring_.store(true);

    LOG_INFO(QString("开始监控: 采样间隔%1ms").arg(intervalMs));

    // 立即采集第一个快照
    collectSnapshot();
}

void SystemResourceMonitor::stop()
{
    if (!isMonitoring_.load()) {
        return;
    }

    timer_->stop();
    isMonitoring_.store(false);

    LOG_INFO("停止监控");
}

void SystemResourceMonitor::reset()
{
    QMutexLocker locker(&mutex_);

    history_.clear();
    startTimestamp_ = QDateTime::currentMSecsSinceEpoch();
    lastCpuTime_ = 0;
    lastSystemTime_ = 0;

    LOG_INFO("监控数据已重置");
}

ResourceSnapshot SystemResourceMonitor::getCurrentSnapshot() const
{
    QMutexLocker locker(&mutex_);

    if (history_.isEmpty()) {
        return ResourceSnapshot();
    }

    return history_.last();
}

QList<ResourceSnapshot> SystemResourceMonitor::getHistory(int seconds) const
{
    QMutexLocker locker(&mutex_);

    if (seconds <= 0) {
        return history_;
    }

    qint64 cutoffTime = QDateTime::currentMSecsSinceEpoch() - (seconds * 1000);

    QList<ResourceSnapshot> recent;
    for (const auto& snapshot : history_) {
        if (snapshot.timestamp >= cutoffTime) {
            recent.append(snapshot);
        }
    }

    return recent;
}

ResourceStats SystemResourceMonitor::getStatistics() const
{
    QMutexLocker locker(&mutex_);
    return calculateStats();
}

bool SystemResourceMonitor::isMemoryLeaking(double thresholdKBs) const
{
    QMutexLocker locker(&mutex_);

    if (history_.size() < 10) {
        return false;  // 数据不足
    }

    double growthRate = getMemoryGrowthRate();
    return (growthRate / 3600.0) > thresholdKBs;  // 转换为KB/s
}

double SystemResourceMonitor::getMemoryGrowthRate() const
{
    // 注意：该方法已在外部加锁（由isMemoryLeaking调用）或不需要加锁（单次调用）

    if (history_.size() < 2) {
        return 0.0;
    }

    // 使用最小二乘法计算线性增长率
    double sumX = 0.0;  // 时间总和
    double sumY = 0.0;  // 内存总和
    double sumXY = 0.0;
    double sumX2 = 0.0;
    int n = history_.size();

    qint64 baseTime = history_.first().timestamp;

    for (const auto& snapshot : history_) {
        double x = (snapshot.timestamp - baseTime) / 1000.0;  // 秒
        double y = snapshot.physicalMemoryMB * 1024.0;        // KB

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

    double slope = (n * sumXY - sumX * sumY) / denominator;  // KB/s

    return slope * 3600.0;  // 转换为KB/h
}

double SystemResourceMonitor::getAvgCpuUsage(int seconds) const
{
    QMutexLocker locker(&mutex_);

    QList<ResourceSnapshot> data = (seconds > 0) ?
        getHistory(seconds) : history_;

    if (data.isEmpty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (const auto& snapshot : data) {
        sum += snapshot.cpuUsagePercent;
    }

    return sum / data.size();
}

bool SystemResourceMonitor::isHighCpuUsage(double thresholdPercent) const
{
    QMutexLocker locker(&mutex_);

    if (history_.isEmpty()) {
        return false;
    }

    return history_.last().cpuUsagePercent > thresholdPercent;
}

QString SystemResourceMonitor::generateReport() const
{
    QMutexLocker locker(&mutex_);

    ResourceStats stats = calculateStats();

    QString report;
    report += "========== 系统资源监控报告 ==========\n\n";

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

    // 内存统计
    report += "【内存统计】\n";
    report += QString("  初始内存: %1 MB\n").arg(stats.memoryInitialMB);
    report += QString("  当前内存: %1 MB\n").arg(stats.memoryCurrentMB);
    report += QString("  峰值内存: %1 MB\n").arg(stats.memoryPeakMB);
    report += QString("  总增长量: %1 MB\n").arg(stats.memoryGrowthTotalMB, 0, 'f', 2);
    report += QString("  增长速率: %1 KB/h\n\n").arg(stats.memoryGrowthRateKBh, 0, 'f', 2);

    // CPU统计
    report += "【CPU统计】\n";
    report += QString("  平均使用率: %1%\n").arg(stats.cpuAvgPercent, 0, 'f', 1);
    report += QString("  峰值使用率: %1%\n").arg(stats.cpuPeakPercent, 0, 'f', 1);
    report += QString("  最低使用率: %1%\n\n").arg(stats.cpuMinPercent, 0, 'f', 1);

    // 线程统计
    report += "【线程统计】\n";
    report += QString("  平均线程数: %1\n").arg(stats.threadCountAvg);
    report += QString("  峰值线程数: %1\n\n").arg(stats.threadCountPeak);

    // 健康评估
    report += "【健康评估】\n";

    if (stats.memoryGrowthRateKBh > 1000.0) {
        report += "  ⚠️ 警告: 检测到显著内存增长，可能存在内存泄漏\n";
    } else if (stats.memoryGrowthRateKBh > 100.0) {
        report += "  ⚠️ 注意: 内存有缓慢增长趋势\n";
    } else {
        report += "  ✅ 内存增长正常\n";
    }

    if (stats.cpuAvgPercent > 80.0) {
        report += "  ⚠️ 警告: CPU使用率持续偏高\n";
    } else if (stats.cpuAvgPercent > 60.0) {
        report += "  ⚠️ 注意: CPU使用率中等偏高\n";
    } else {
        report += "  ✅ CPU使用率正常\n";
    }

    report += "\n========================================\n";

    return report;
}

void SystemResourceMonitor::setMaxHistorySize(int count)
{
    QMutexLocker locker(&mutex_);
    maxHistorySize_ = count;

    // 如果当前历史超过新限制，裁剪掉旧数据
    while (history_.size() > maxHistorySize_) {
        history_.removeFirst();
    }
}

int SystemResourceMonitor::historySize() const
{
    QMutexLocker locker(&mutex_);
    return history_.size();
}

void SystemResourceMonitor::onMonitorTimer()
{
    collectSnapshot();
}

void SystemResourceMonitor::collectSnapshot()
{
    ResourceSnapshot snapshot;
    snapshot.timestamp = QDateTime::currentMSecsSinceEpoch();

    try {
#ifdef _WIN32
        collectWindows(snapshot);
#elif defined(__linux__)
        collectLinux(snapshot);
#elif defined(__APPLE__)
        collectMacOS(snapshot);
#else
        LOG_ERROR("不支持的操作系统平台");
        emit monitorError("不支持的操作系统平台");
        return;
#endif

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
        }

        // 发送信号
        emit snapshotReady(snapshot);

    } catch (const std::exception& e) {
        QString errorMsg = QString("采集快照异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        emit monitorError(errorMsg);
    }
}

void SystemResourceMonitor::analyzeSnapshot(const ResourceSnapshot& snapshot)
{
    // 检测内存泄漏
    if (isMemoryLeaking(1.0)) {  // 阈值: 1 KB/s
        double rateKBs = getMemoryGrowthRate() / 3600.0;
        emit memoryLeakDetected(rateKBs);
    }

    // 检测高CPU使用率
    if (snapshot.cpuUsagePercent > 80.0) {
        emit highCpuUsage(snapshot.cpuUsagePercent);
    }
}

#ifdef _WIN32

void SystemResourceMonitor::collectWindows(ResourceSnapshot& snapshot)
{
    HANDLE hProcess = (HANDLE)processHandle_;

    // ========== 内存信息 ==========
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        snapshot.virtualMemoryMB = pmc.PrivateUsage / (1024 * 1024);
        snapshot.physicalMemoryMB = pmc.WorkingSetSize / (1024 * 1024);
        snapshot.privateMemoryMB = pmc.PrivateUsage / (1024 * 1024);
        snapshot.workingSetMB = pmc.WorkingSetSize / (1024 * 1024);
    }

    // ========== CPU信息 ==========
    FILETIME createTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime)) {
        // 将FILETIME转换为64位整数（100纳秒单位）
        auto fileTimeToInt64 = [](const FILETIME& ft) -> qint64 {
            ULARGE_INTEGER uli;
            uli.LowPart = ft.dwLowDateTime;
            uli.HighPart = ft.dwHighDateTime;
            return uli.QuadPart;
        };

        qint64 currentKernelTime = fileTimeToInt64(kernelTime);
        qint64 currentUserTime = fileTimeToInt64(userTime);
        qint64 currentCpuTime = currentKernelTime + currentUserTime;

        // 获取系统时间
        FILETIME sysIdle, sysKernel, sysUser;
        if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser)) {
            qint64 currentSystemTime = fileTimeToInt64(sysKernel) + fileTimeToInt64(sysUser);

            // 计算CPU使用率（需要有上一次的数据）
            if (lastCpuTime_ > 0 && lastSystemTime_ > 0) {
                qint64 cpuDelta = currentCpuTime - lastCpuTime_;
                qint64 sysDelta = currentSystemTime - lastSystemTime_;

                if (sysDelta > 0) {
                    // 总CPU使用率
                    snapshot.cpuUsagePercent = (cpuDelta * 100.0) / sysDelta;

                    // 用户态CPU
                    qint64 userDelta = currentUserTime - (lastCpuTime_ - (lastCpuTime_ - lastCpuTime_));  // 简化
                    snapshot.cpuUserPercent = (userDelta * 100.0) / sysDelta;

                    // 内核态CPU
                    qint64 kernelDelta = currentKernelTime - (lastCpuTime_ - userDelta);
                    snapshot.cpuKernelPercent = (kernelDelta * 100.0) / sysDelta;
                }
            }

            // 保存当前值供下次使用
            lastCpuTime_ = currentCpuTime;
            lastSystemTime_ = currentSystemTime;
        }
    }

    // ========== 线程和句柄数 ==========
    DWORD handleCount = 0;
    GetProcessHandleCount(hProcess, &handleCount);
    snapshot.handleCount = handleCount;

    // 获取线程数（通过枚举工具帮助库）
    DWORD processId = GetCurrentProcessId();
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap != INVALID_HANDLE_VALUE) {
        THREADENTRY32 te32;
        te32.dwSize = sizeof(THREADENTRY32);

        int threadCount = 0;
        if (Thread32First(hThreadSnap, &te32)) {
            do {
                if (te32.th32OwnerProcessID == processId) {
                    threadCount++;
                }
            } while (Thread32Next(hThreadSnap, &te32));
        }

        snapshot.threadCount = threadCount;
        CloseHandle(hThreadSnap);
    }

    // ========== I/O信息 ==========
    IO_COUNTERS ioCounters;
    if (GetProcessIoCounters(hProcess, &ioCounters)) {
        static qint64 lastReadBytes = 0;
        static qint64 lastWriteBytes = 0;
        static qint64 lastIoTimestamp = 0;

        qint64 currentReadBytes = ioCounters.ReadTransferCount;
        qint64 currentWriteBytes = ioCounters.WriteTransferCount;
        qint64 currentTimestamp = snapshot.timestamp;

        // 计算速率
        if (lastIoTimestamp > 0) {
            qint64 timeDelta = currentTimestamp - lastIoTimestamp;  // ms
            if (timeDelta > 0) {
                snapshot.readBytesSec = ((currentReadBytes - lastReadBytes) * 1000) / timeDelta;
                snapshot.writeBytesSec = ((currentWriteBytes - lastWriteBytes) * 1000) / timeDelta;
            }
        }

        // 累计值
        snapshot.totalReadMB = currentReadBytes / (1024 * 1024);
        snapshot.totalWriteMB = currentWriteBytes / (1024 * 1024);

        // 保存当前值
        lastReadBytes = currentReadBytes;
        lastWriteBytes = currentWriteBytes;
        lastIoTimestamp = currentTimestamp;
    }
}

#elif defined(__linux__)

void SystemResourceMonitor::collectLinux(ResourceSnapshot& snapshot)
{
    // ========== 内存信息 ==========
    std::ifstream statusFile("/proc/self/status");
    std::string line;

    while (std::getline(statusFile, line)) {
        if (line.find("VmSize:") == 0) {
            sscanf(line.c_str(), "VmSize: %lld kB", &snapshot.virtualMemoryMB);
            snapshot.virtualMemoryMB /= 1024;
        } else if (line.find("VmRSS:") == 0) {
            sscanf(line.c_str(), "VmRSS: %lld kB", &snapshot.physicalMemoryMB);
            snapshot.physicalMemoryMB /= 1024;
        }
    }
    statusFile.close();

    // ========== CPU信息 ==========
    std::ifstream statFile("/proc/self/stat");
    long utime, stime;
    if (statFile >> utime >> stime) {
        // 简化实现：需要更复杂的计算来获取准确的CPU使用率
        snapshot.cpuUsagePercent = 0.0;  // TODO: 完整实现
    }
    statFile.close();

    // ========== 线程数 ==========
    std::ifstream taskDir("/proc/self/task");
    // TODO: 枚举task目录获取线程数

    LOG_WARNING("Linux平台监控功能待完善");
}

#elif defined(__APPLE__)

void SystemResourceMonitor::collectMacOS(ResourceSnapshot& snapshot)
{
    // ========== 内存信息 ==========
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);

    if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size) == KERN_SUCCESS) {
        snapshot.virtualMemoryMB = info.virtual_size / (1024 * 1024);
        snapshot.physicalMemoryMB = info.resident_size / (1024 * 1024);
    }

    // ========== CPU信息 ==========
    // TODO: macOS CPU监控实现

    LOG_WARNING("macOS平台监控功能待完善");
}

#endif

ResourceStats SystemResourceMonitor::calculateStats() const
{
    ResourceStats stats;

    if (history_.isEmpty()) {
        return stats;
    }

    // 内存统计
    stats.memoryInitialMB = history_.first().physicalMemoryMB;
    stats.memoryCurrentMB = history_.last().physicalMemoryMB;
    stats.memoryPeakMB = 0;

    for (const auto& snapshot : history_) {
        if (snapshot.physicalMemoryMB > stats.memoryPeakMB) {
            stats.memoryPeakMB = snapshot.physicalMemoryMB;
        }
    }

    stats.memoryGrowthTotalMB = stats.memoryCurrentMB - stats.memoryInitialMB;
    stats.memoryGrowthRateKBh = getMemoryGrowthRate();

    // CPU统计
    double cpuSum = 0.0;
    stats.cpuPeakPercent = 0.0;
    stats.cpuMinPercent = 100.0;

    for (const auto& snapshot : history_) {
        cpuSum += snapshot.cpuUsagePercent;

        if (snapshot.cpuUsagePercent > stats.cpuPeakPercent) {
            stats.cpuPeakPercent = snapshot.cpuUsagePercent;
        }

        if (snapshot.cpuUsagePercent < stats.cpuMinPercent) {
            stats.cpuMinPercent = snapshot.cpuUsagePercent;
        }
    }

    stats.cpuAvgPercent = cpuSum / history_.size();

    // 线程统计
    int threadSum = 0;
    stats.threadCountPeak = 0;

    for (const auto& snapshot : history_) {
        threadSum += snapshot.threadCount;

        if (snapshot.threadCount > stats.threadCountPeak) {
            stats.threadCountPeak = snapshot.threadCount;
        }
    }

    stats.threadCountAvg = threadSum / history_.size();

    return stats;
}

} // namespace Base
} // namespace VisionForge
