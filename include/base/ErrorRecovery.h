/**
 * @file ErrorRecovery.h
 * @brief 错误恢复机制
 * @author VisionForge Team
 * @date 2025-12-19
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
