/**
 * @file ErrorRecovery.h
 * @brief 错误恢复机制 - 自动重试、检查点回滚、策略化恢复
 * @author VisionForge Team
 * @date 2025-12-19
 *
 * @details
 * 本文件实现了VisionForge项目的错误恢复机制，提供健壮的错误处理能力。
 *
 * ## 设计模式
 * - **单例模式 (Singleton)**：全局唯一的错误恢复管理器
 * - **策略模式 (Strategy)**：可配置的恢复策略
 * - **命令模式 (Command)**：封装重试操作
 * - **备忘录模式 (Memento)**：检查点状态保存和恢复
 * - **RAII模式**：ScopedCheckpoint自动管理检查点生命周期
 *
 * ## 核心功能
 *
 * ### 错误分类
 * 按严重程度分为四级：
 * - Warning：警告，可继续运行
 * - Error：错误，需处理但可恢复
 * - Critical：严重，可能导致数据丢失
 * - Fatal：致命，必须停止运行
 *
 * ### 恢复策略
 * - Ignore：忽略错误继续运行
 * - Retry：自动重试（支持指数退避）
 * - Rollback：回滚到最近检查点
 * - Skip：跳过当前操作
 * - Notify：通知用户决定
 * - Abort：中止当前任务
 *
 * ### 自动重试
 * - 支持配置最大重试次数
 * - 指数退避算法避免频繁重试
 * - 可自定义延迟策略
 * - 支持进度回调
 *
 * ### 检查点系统
 * - 创建状态快照
 * - 支持多个命名检查点
 * - 自动或手动回滚
 * - RAII风格的ScopedCheckpoint
 *
 * ## 线程安全
 * - recovering_和currentRetry_使用std::atomic
 * - 信号发射线程安全
 * - 检查点操作需要外部同步
 *
 * ## 内存管理
 * - 错误历史限制最大条数，防止内存泄漏
 * - 检查点状态使用QJsonObject，支持序列化
 * - ScopedCheckpoint使用RAII自动清理
 *
 * ## 使用示例
 * @code
 * // 报告错误
 * ERROR_RECOVERY.reportError("CAM_001", "相机连接超时");
 *
 * // 带重试的操作
 * bool success = ERROR_RECOVERY.executeWithRetry([&]() {
 *     return camera->connect();
 * }, "CAM_CONNECT");
 *
 * // 使用检查点
 * {
 *     ScopedCheckpoint cp("before_edit", "编辑前状态", saveState());
 *     // ... 危险操作 ...
 *     cp.commit();  // 成功则不回滚
 * }  // 未commit则自动回滚
 *
 * // 自定义恢复处理器
 * ERROR_RECOVERY.registerRecoveryHandler("PLC_TIMEOUT", [](const ErrorInfo& e) {
 *     return plc->reconnect();
 * });
 * @endcode
 */

#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QJsonObject>
#include <functional>
#include <memory>
#include <chrono>
#include <atomic>

namespace VisionForge {
namespace Base {

/**
 * @brief 错误严重程度
 */
enum class ErrorSeverity {
    Warning,    ///< 警告：可继续运行
    Error,      ///< 错误：需要处理但可恢复
    Critical,   ///< 严重：可能导致数据丢失
    Fatal       ///< 致命：必须停止运行
};

/**
 * @brief 恢复策略
 */
enum class RecoveryStrategy {
    Ignore,         ///< 忽略错误继续运行
    Retry,          ///< 重试操作
    Rollback,       ///< 回滚到上一个检查点
    Skip,           ///< 跳过当前操作
    Notify,         ///< 仅通知用户
    Abort           ///< 中止当前任务
};

/**
 * @brief 错误信息结构
 */
struct ErrorInfo {
    QString code;                       ///< 错误代码
    QString message;                    ///< 错误消息
    QString source;                     ///< 错误来源
    ErrorSeverity severity;             ///< 严重程度
    QVariant context;                   ///< 上下文信息
    std::chrono::system_clock::time_point timestamp;  ///< 发生时间

    ErrorInfo()
        : severity(ErrorSeverity::Error)
        , timestamp(std::chrono::system_clock::now()) {}
};

/**
 * @brief 检查点数据
 */
struct Checkpoint {
    QString id;                         ///< 检查点ID
    QString description;                ///< 描述
    QJsonObject state;                  ///< 状态数据
    std::chrono::system_clock::time_point timestamp;  ///< 创建时间

    Checkpoint() : timestamp(std::chrono::system_clock::now()) {}
};

/**
 * @brief 重试策略配置
 */
struct RetryConfig {
    int maxRetries;             ///< 最大重试次数
    int initialDelayMs;         ///< 初始延迟(毫秒)
    int maxDelayMs;             ///< 最大延迟(毫秒)
    double backoffMultiplier;   ///< 退避乘数
    bool exponentialBackoff;    ///< 是否指数退避

    RetryConfig()
        : maxRetries(3)
        , initialDelayMs(100)
        , maxDelayMs(5000)
        , backoffMultiplier(2.0)
        , exponentialBackoff(true) {}
};

/**
 * @brief 恢复处理器函数类型
 */
using RecoveryHandler = std::function<bool(const ErrorInfo&)>;

/**
 * @class ErrorRecovery
 * @brief 错误恢复管理器（单例）
 *
 * 功能：
 * - 错误捕获和分类
 * - 自动重试逻辑
 * - 检查点和回滚
 * - 自定义恢复策略
 */
class ErrorRecovery : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static ErrorRecovery& instance();

    // ========== 错误报告 ==========

    /**
     * @brief 报告错误
     * @param info 错误信息
     * @return 选择的恢复策略
     */
    RecoveryStrategy reportError(const ErrorInfo& info);

    /**
     * @brief 报告错误（便捷方法）
     */
    RecoveryStrategy reportError(const QString& code,
                                 const QString& message,
                                 ErrorSeverity severity = ErrorSeverity::Error,
                                 const QString& source = QString());

    // ========== 恢复策略 ==========

    /**
     * @brief 设置错误的默认恢复策略
     * @param code 错误代码（空字符串表示全局默认）
     * @param strategy 恢复策略
     */
    void setRecoveryStrategy(const QString& code, RecoveryStrategy strategy);

    /**
     * @brief 获取错误的恢复策略
     * @param code 错误代码
     */
    RecoveryStrategy getRecoveryStrategy(const QString& code) const;

    /**
     * @brief 注册自定义恢复处理器
     * @param code 错误代码
     * @param handler 处理函数
     */
    void registerRecoveryHandler(const QString& code, RecoveryHandler handler);

    // ========== 重试机制 ==========

    /**
     * @brief 设置重试配置
     * @param config 重试配置
     */
    void setRetryConfig(const RetryConfig& config) { retryConfig_ = config; }

    /**
     * @brief 获取重试配置
     */
    const RetryConfig& retryConfig() const { return retryConfig_; }

    /**
     * @brief 执行带重试的操作
     * @param operation 操作函数（返回true表示成功）
     * @param errorCode 失败时的错误代码
     * @return true如果操作成功
     */
    bool executeWithRetry(std::function<bool()> operation,
                          const QString& errorCode = "OPERATION_FAILED");

    /**
     * @brief 执行带重试的操作（带进度回调）
     */
    bool executeWithRetry(std::function<bool()> operation,
                          std::function<void(int attempt, int maxAttempts)> progressCallback,
                          const QString& errorCode = "OPERATION_FAILED");

    // ========== 检查点 ==========

    /**
     * @brief 创建检查点
     * @param id 检查点ID
     * @param description 描述
     * @param state 状态数据
     */
    void createCheckpoint(const QString& id,
                          const QString& description,
                          const QJsonObject& state);

    /**
     * @brief 恢复到检查点
     * @param id 检查点ID
     * @return 检查点状态数据（如果存在）
     */
    QJsonObject restoreCheckpoint(const QString& id);

    /**
     * @brief 删除检查点
     * @param id 检查点ID
     */
    void removeCheckpoint(const QString& id);

    /**
     * @brief 清除所有检查点
     */
    void clearCheckpoints();

    /**
     * @brief 获取所有检查点ID列表
     */
    QStringList checkpointIds() const;

    /**
     * @brief 检查检查点是否存在
     */
    bool hasCheckpoint(const QString& id) const;

    /**
     * @brief 获取最近的检查点ID
     */
    QString lastCheckpointId() const;

    // ========== 错误历史 ==========

    /**
     * @brief 获取错误历史
     * @param maxCount 最大数量（0表示全部）
     */
    QVector<ErrorInfo> errorHistory(int maxCount = 0) const;

    /**
     * @brief 清除错误历史
     */
    void clearErrorHistory();

    /**
     * @brief 设置最大错误历史数量
     */
    void setMaxErrorHistory(int maxCount) { maxErrorHistory_ = maxCount; }

    // ========== 状态 ==========

    /**
     * @brief 是否正在恢复中
     */
    bool isRecovering() const { return recovering_.load(); }

    /**
     * @brief 获取当前重试次数
     */
    int currentRetryCount() const { return currentRetry_.load(); }

signals:
    /**
     * @brief 错误发生信号
     */
    void errorOccurred(const ErrorInfo& info);

    /**
     * @brief 恢复开始信号
     */
    void recoveryStarted(const QString& errorCode, RecoveryStrategy strategy);

    /**
     * @brief 恢复完成信号
     */
    void recoveryCompleted(const QString& errorCode, bool success);

    /**
     * @brief 检查点创建信号
     */
    void checkpointCreated(const QString& id);

    /**
     * @brief 检查点恢复信号
     */
    void checkpointRestored(const QString& id);

    /**
     * @brief 重试进度信号
     */
    void retryProgress(int attempt, int maxAttempts);

private:
    ErrorRecovery();
    ~ErrorRecovery() = default;
    ErrorRecovery(const ErrorRecovery&) = delete;
    ErrorRecovery& operator=(const ErrorRecovery&) = delete;

    RecoveryStrategy determineStrategy(const ErrorInfo& info) const;
    bool executeRecovery(const ErrorInfo& info, RecoveryStrategy strategy);
    int calculateDelay(int attempt) const;
    void addToHistory(const ErrorInfo& info);

private:
    QMap<QString, RecoveryStrategy> strategyMap_;
    QMap<QString, RecoveryHandler> handlers_;
    QMap<QString, Checkpoint> checkpoints_;
    QVector<ErrorInfo> errorHistory_;

    RetryConfig retryConfig_;
    int maxErrorHistory_;
    QString lastCheckpointId_;

    std::atomic<bool> recovering_;
    std::atomic<int> currentRetry_;
};

/**
 * @class ScopedCheckpoint
 * @brief RAII检查点（自动创建和清理）
 */
class ScopedCheckpoint
{
public:
    ScopedCheckpoint(const QString& id, const QString& description, const QJsonObject& state);
    ~ScopedCheckpoint();

    void commit();   ///< 提交（不回滚）
    void rollback(); ///< 手动回滚

private:
    QString id_;
    bool committed_;
};

/**
 * @class RetryScope
 * @brief RAII重试作用域
 */
class RetryScope
{
public:
    RetryScope(const RetryConfig& config);
    ~RetryScope();

private:
    RetryConfig previousConfig_;
};

} // namespace Base
} // namespace VisionForge

// 便捷宏
#define ERROR_RECOVERY VisionForge::Base::ErrorRecovery::instance()
#define REPORT_ERROR(code, msg) VisionForge::Base::ErrorRecovery::instance().reportError(code, msg)
#define REPORT_WARNING(code, msg) VisionForge::Base::ErrorRecovery::instance().reportError(code, msg, VisionForge::Base::ErrorSeverity::Warning)
#define REPORT_CRITICAL(code, msg) VisionForge::Base::ErrorRecovery::instance().reportError(code, msg, VisionForge::Base::ErrorSeverity::Critical)
