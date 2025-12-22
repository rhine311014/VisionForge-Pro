/**
 * @file ApplicationWatchdog.h
 * @brief 应用程序看门狗守护系统
 * @details 监控主应用程序健康状态，实现无人值守产线的自动恢复
 *
 * 功能特性：
 * - 心跳检测：主程序定期发送心跳，超时则触发恢复
 * - 资源监控：内存/CPU超限时触发警告或重启
 * - 崩溃恢复：检测到进程异常退出时自动重启
 * - 日志记录：所有事件记录到独立日志文件
 * - 远程通知：支持通过WebSocket推送告警
 *
 * @author VisionForge Team
 * @date 2025-12-22
 */

#pragma once

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QDateTime>
#include <QMutex>
#include <QSharedMemory>
#include <QFile>
#include <QJsonObject>
#include <memory>
#include <functional>

namespace VisionForge {
namespace Base {

/**
 * @brief Watchdog配置结构
 */
struct WatchdogConfig {
    int heartbeatTimeoutMs = 10000;         ///< 心跳超时时间（毫秒）
    int maxRestartAttempts = 3;             ///< 最大重启尝试次数
    int restartCooldownMs = 60000;          ///< 重启冷却时间（毫秒）
    int memoryLimitMB = 2048;               ///< 内存限制（MB）
    int cpuLimitPercent = 90;               ///< CPU使用率限制（%）
    int resourceCheckIntervalMs = 5000;     ///< 资源检查间隔（毫秒）
    QString applicationPath;                 ///< 被监控应用程序路径
    QStringList applicationArgs;             ///< 应用程序启动参数
    QString logFilePath;                     ///< 日志文件路径
    bool enableRemoteNotification = false;   ///< 启用远程通知
    QString notificationEndpoint;            ///< 通知端点URL

    QJsonObject toJson() const;
    static WatchdogConfig fromJson(const QJsonObject& json);
};

/**
 * @brief Watchdog事件类型
 */
enum class WatchdogEvent {
    Started,                ///< Watchdog启动
    Stopped,                ///< Watchdog停止
    HeartbeatReceived,      ///< 收到心跳
    HeartbeatTimeout,       ///< 心跳超时
    ApplicationStarted,     ///< 应用程序启动
    ApplicationStopped,     ///< 应用程序正常停止
    ApplicationCrashed,     ///< 应用程序崩溃
    ApplicationRestarted,   ///< 应用程序重启
    MemoryWarning,          ///< 内存警告
    MemoryExceeded,         ///< 内存超限
    CpuWarning,             ///< CPU警告
    CpuExceeded,            ///< CPU超限
    MaxRestartsReached,     ///< 达到最大重启次数
    ManualRestart,          ///< 手动重启
    ConfigChanged           ///< 配置变更
};

/**
 * @brief Watchdog状态
 */
enum class WatchdogState {
    Idle,                   ///< 空闲
    Monitoring,             ///< 监控中
    Restarting,             ///< 重启中
    Cooldown,               ///< 冷却中
    Failed                  ///< 失败（达到最大重启次数）
};

/**
 * @brief Watchdog统计信息
 */
struct WatchdogStats {
    QDateTime startTime;                ///< 启动时间
    int totalRestarts = 0;              ///< 总重启次数
    int heartbeatCount = 0;             ///< 心跳计数
    int timeoutCount = 0;               ///< 超时计数
    int crashCount = 0;                 ///< 崩溃计数
    double avgHeartbeatIntervalMs = 0;  ///< 平均心跳间隔
    qint64 totalUptimeMs = 0;           ///< 总运行时间
    QDateTime lastHeartbeat;            ///< 最后心跳时间
    QDateTime lastRestart;              ///< 最后重启时间
    double currentMemoryMB = 0;         ///< 当前内存使用
    double currentCpuPercent = 0;       ///< 当前CPU使用率

    QJsonObject toJson() const;
};

/**
 * @class ApplicationWatchdog
 * @brief 应用程序看门狗守护类
 *
 * 使用方法：
 * 1. 主程序端：定期调用 sendHeartbeat() 发送心跳
 * 2. 守护进程端：启动Watchdog并配置监控参数
 *
 * @note 建议作为独立守护进程运行，而非嵌入主程序
 */
class ApplicationWatchdog : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationWatchdog(QObject* parent = nullptr);
    ~ApplicationWatchdog() override;

    // ========== 配置管理 ==========

    /**
     * @brief 设置配置
     */
    void setConfig(const WatchdogConfig& config);

    /**
     * @brief 获取配置
     */
    WatchdogConfig config() const { return config_; }

    /**
     * @brief 从文件加载配置
     */
    bool loadConfig(const QString& filePath);

    /**
     * @brief 保存配置到文件
     */
    bool saveConfig(const QString& filePath) const;

    // ========== 控制接口 ==========

    /**
     * @brief 启动Watchdog监控
     * @return 是否成功启动
     */
    bool start();

    /**
     * @brief 停止Watchdog监控
     */
    void stop();

    /**
     * @brief 是否正在运行
     */
    bool isRunning() const { return state_ != WatchdogState::Idle; }

    /**
     * @brief 获取当前状态
     */
    WatchdogState state() const { return state_; }

    /**
     * @brief 获取状态字符串
     */
    QString stateString() const;

    /**
     * @brief 手动重启应用程序
     */
    bool restartApplication();

    /**
     * @brief 手动停止应用程序
     */
    bool stopApplication();

    // ========== 心跳接口（主程序调用） ==========

    /**
     * @brief 发送心跳（通过共享内存）
     * @note 供被监控的主程序调用
     */
    static bool sendHeartbeat();

    /**
     * @brief 发送心跳（带附加数据）
     */
    static bool sendHeartbeat(const QJsonObject& data);

    // ========== 统计信息 ==========

    /**
     * @brief 获取统计信息
     */
    WatchdogStats stats() const { return stats_; }

    /**
     * @brief 重置统计信息
     */
    void resetStats();

    // ========== 回调设置 ==========

    using EventCallback = std::function<void(WatchdogEvent, const QString&)>;

    /**
     * @brief 设置事件回调
     */
    void setEventCallback(EventCallback callback) { eventCallback_ = callback; }

signals:
    /**
     * @brief 事件信号
     */
    void eventOccurred(WatchdogEvent event, const QString& message);

    /**
     * @brief 状态变化信号
     */
    void stateChanged(WatchdogState newState);

    /**
     * @brief 统计更新信号
     */
    void statsUpdated(const WatchdogStats& stats);

    /**
     * @brief 应用程序输出信号
     */
    void applicationOutput(const QString& output);

private slots:
    void onHeartbeatCheck();
    void onResourceCheck();
    void onCooldownFinished();
    void onApplicationFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onApplicationError(QProcess::ProcessError error);
    void onApplicationReadyRead();

private:
    void setState(WatchdogState newState);
    void emitEvent(WatchdogEvent event, const QString& message = QString());
    void logEvent(WatchdogEvent event, const QString& message);
    bool startApplication();
    bool checkHeartbeat();
    void checkResources();
    void sendRemoteNotification(WatchdogEvent event, const QString& message);
    void updateStats();
    QString eventToString(WatchdogEvent event) const;

    // 共享内存相关
    bool initSharedMemory();
    void cleanupSharedMemory();

private:
    WatchdogConfig config_;
    WatchdogState state_ = WatchdogState::Idle;
    WatchdogStats stats_;

    QProcess* appProcess_ = nullptr;
    QTimer* heartbeatTimer_ = nullptr;
    QTimer* resourceTimer_ = nullptr;
    QTimer* cooldownTimer_ = nullptr;

    QSharedMemory* sharedMemory_ = nullptr;
    mutable QMutex mutex_;

    int currentRestartAttempts_ = 0;
    QDateTime lastHeartbeatTime_;
    QFile logFile_;

    EventCallback eventCallback_;

    static const QString SHARED_MEMORY_KEY;
};

// ============================================================
// 心跳客户端辅助类（供主程序使用）
// ============================================================

/**
 * @class HeartbeatClient
 * @brief 心跳客户端，供被监控的主程序使用
 *
 * 使用方法：
 * @code
 * HeartbeatClient heartbeat;
 * heartbeat.start(5000);  // 每5秒发送一次心跳
 * @endcode
 */
class HeartbeatClient : public QObject
{
    Q_OBJECT

public:
    explicit HeartbeatClient(QObject* parent = nullptr);
    ~HeartbeatClient() override;

    /**
     * @brief 启动心跳发送
     * @param intervalMs 发送间隔（毫秒）
     */
    void start(int intervalMs = 5000);

    /**
     * @brief 停止心跳发送
     */
    void stop();

    /**
     * @brief 是否正在运行
     */
    bool isRunning() const;

    /**
     * @brief 设置附加数据（每次心跳时发送）
     */
    void setExtraData(const QJsonObject& data) { extraData_ = data; }

    /**
     * @brief 立即发送一次心跳
     */
    bool sendNow();

signals:
    void heartbeatSent();
    void heartbeatFailed(const QString& error);

private slots:
    void onTimerTimeout();

private:
    QTimer* timer_ = nullptr;
    QJsonObject extraData_;
};

} // namespace Base
} // namespace VisionForge
