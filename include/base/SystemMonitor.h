/**
 * @file SystemMonitor.h
 * @brief 系统资源监控器
 * @details 监控CPU、内存、磁盘等系统资源使用情况
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QDateTime>

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#endif

namespace VisionForge {
namespace Base {

/**
 * @brief 系统资源快照
 */
struct SystemSnapshot {
    QDateTime timestamp;        // 时间戳

    // CPU信息
    double cpuUsage;            // CPU使用率 (0-100%)
    int cpuCoreCount;           // CPU核心数

    // 内存信息
    qint64 totalMemory;         // 总内存 (bytes)
    qint64 usedMemory;          // 已用内存 (bytes)
    qint64 availableMemory;     // 可用内存 (bytes)
    double memoryUsage;         // 内存使用率 (0-100%)

    // 进程信息
    qint64 processMemory;       // 当前进程内存 (bytes)
    double processCpuUsage;     // 当前进程CPU使用率

    // 磁盘信息（程序所在磁盘）
    qint64 totalDisk;           // 总磁盘空间 (bytes)
    qint64 freeDisk;            // 可用磁盘空间 (bytes)
    double diskUsage;           // 磁盘使用率 (0-100%)

    SystemSnapshot()
        : cpuUsage(0.0)
        , cpuCoreCount(1)
        , totalMemory(0)
        , usedMemory(0)
        , availableMemory(0)
        , memoryUsage(0.0)
        , processMemory(0)
        , processCpuUsage(0.0)
        , totalDisk(0)
        , freeDisk(0)
        , diskUsage(0.0)
    {
        timestamp = QDateTime::currentDateTime();
    }
};

/**
 * @brief 资源警告级别
 */
enum class ResourceWarningLevel {
    Normal,     // 正常
    Low,        // 低警告
    Medium,     // 中警告
    High,       // 高警告
    Critical    // 危险
};

/**
 * @brief 资源阈值配置
 */
struct ResourceThresholds {
    double cpuWarning;          // CPU警告阈值 (%)
    double cpuCritical;         // CPU危险阈值 (%)
    double memoryWarning;       // 内存警告阈值 (%)
    double memoryCritical;      // 内存危险阈值 (%)
    double diskWarning;         // 磁盘警告阈值 (%)
    double diskCritical;        // 磁盘危险阈值 (%)

    ResourceThresholds()
        : cpuWarning(70.0)
        , cpuCritical(90.0)
        , memoryWarning(75.0)
        , memoryCritical(90.0)
        , diskWarning(80.0)
        , diskCritical(95.0)
    {}
};

/**
 * @class SystemMonitor
 * @brief 系统资源监控器（单例）
 *
 * 功能：
 * - 实时监控系统资源
 * - 资源使用率警告
 * - 历史数据记录
 * - 资源趋势分析
 */
class SystemMonitor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static SystemMonitor& instance();

    /**
     * @brief 启动监控
     * @param intervalMs 监控间隔（毫秒）
     */
    void start(int intervalMs = 1000);

    /**
     * @brief 停止监控
     */
    void stop();

    /**
     * @brief 检查是否正在运行
     */
    bool isRunning() const { return timer_ && timer_->isActive(); }

    // ==================== 资源获取 ====================

    /**
     * @brief 获取当前系统快照
     */
    SystemSnapshot currentSnapshot() const;

    /**
     * @brief 立即刷新系统快照
     */
    SystemSnapshot refresh();

    /**
     * @brief 获取历史快照
     * @param maxCount 最大数量（0为全部）
     */
    QVector<SystemSnapshot> history(int maxCount = 0) const;

    /**
     * @brief 设置历史记录最大数量
     */
    void setMaxHistorySize(int maxSize);

    // ==================== 便捷方法 ====================

    /**
     * @brief 获取CPU使用率
     */
    double cpuUsage() const { return currentSnapshot_.cpuUsage; }

    /**
     * @brief 获取内存使用率
     */
    double memoryUsage() const { return currentSnapshot_.memoryUsage; }

    /**
     * @brief 获取磁盘使用率
     */
    double diskUsage() const { return currentSnapshot_.diskUsage; }

    /**
     * @brief 获取进程内存使用（MB）
     */
    double processMemoryMB() const { return currentSnapshot_.processMemory / (1024.0 * 1024.0); }

    // ==================== 警告系统 ====================

    /**
     * @brief 设置资源阈值
     */
    void setThresholds(const ResourceThresholds& thresholds);

    /**
     * @brief 获取资源阈值
     */
    ResourceThresholds thresholds() const { return thresholds_; }

    /**
     * @brief 获取CPU警告级别
     */
    ResourceWarningLevel cpuWarningLevel() const;

    /**
     * @brief 获取内存警告级别
     */
    ResourceWarningLevel memoryWarningLevel() const;

    /**
     * @brief 获取磁盘警告级别
     */
    ResourceWarningLevel diskWarningLevel() const;

    /**
     * @brief 获取总体警告级别（取最高）
     */
    ResourceWarningLevel overallWarningLevel() const;

    // ==================== 格式化输出 ====================

    /**
     * @brief 格式化字节数
     */
    static QString formatBytes(qint64 bytes);

    /**
     * @brief 获取警告级别名称
     */
    static QString warningLevelName(ResourceWarningLevel level);

signals:
    /**
     * @brief 资源更新信号
     */
    void resourceUpdated(const SystemSnapshot& snapshot);

    /**
     * @brief CPU警告信号
     */
    void cpuWarning(double usage, ResourceWarningLevel level);

    /**
     * @brief 内存警告信号
     */
    void memoryWarning(double usage, ResourceWarningLevel level);

    /**
     * @brief 磁盘警告信号
     */
    void diskWarning(double usage, ResourceWarningLevel level);

private slots:
    void onTimer();

private:
    SystemMonitor();
    ~SystemMonitor();
    SystemMonitor(const SystemMonitor&) = delete;
    SystemMonitor& operator=(const SystemMonitor&) = delete;

    void updateSnapshot();
    void updateCpuUsage();
    void updateMemoryUsage();
    void updateDiskUsage();
    void updateProcessInfo();
    void checkWarnings();

    ResourceWarningLevel calculateLevel(double usage, double warning, double critical) const;

private:
    QTimer* timer_;
    SystemSnapshot currentSnapshot_;
    QVector<SystemSnapshot> history_;
    int maxHistorySize_;
    ResourceThresholds thresholds_;

    // CPU计算辅助变量
#ifdef Q_OS_WIN
    ULARGE_INTEGER lastCpuIdle_;
    ULARGE_INTEGER lastCpuKernel_;
    ULARGE_INTEGER lastCpuUser_;
    FILETIME lastProcessKernel_;
    FILETIME lastProcessUser_;
    QDateTime lastProcessTime_;
#endif

    ResourceWarningLevel lastCpuLevel_;
    ResourceWarningLevel lastMemoryLevel_;
    ResourceWarningLevel lastDiskLevel_;
};

} // namespace Base
} // namespace VisionForge

// 便捷宏
#define SYSTEM_MONITOR VisionForge::Base::SystemMonitor::instance()
