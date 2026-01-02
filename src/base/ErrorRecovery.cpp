/**
 * @file ErrorRecovery.cpp
 * @brief 错误恢复机制实现
 * @author VisionForge Team
 * @date 2025-12-19
 *
 * @details
 * 本文件实现ErrorRecovery类及相关辅助类的所有成员函数。
 *
 * ## 实现细节
 *
 * ### 指数退避算法
 * calculateDelay()实现指数退避：
 * delay = min(initialDelay * multiplier^attempt, maxDelay)
 *
 * 默认配置：
 * - 初始延迟：100ms
 * - 最大延迟：5000ms
 * - 退避乘数：2.0
 *
 * ### 策略确定逻辑
 * determineStrategy()按以下优先级：
 * 1. Fatal错误 -> 强制Abort
 * 2. 特定错误码策略
 * 3. 根据严重程度的默认策略
 *
 * ### 错误历史管理
 * - 使用QVector存储，FIFO方式
 * - 超过maxErrorHistory_时自动清理最旧记录
 * - 默认保留100条历史
 *
 * ### ScopedCheckpoint工作原理
 * - 构造时创建检查点
 * - commit()标记成功，阻止回滚
 * - 析构时：未commit则回滚，然后删除检查点
 */

#include "base/ErrorRecovery.h"
#include "base/Logger.h"
#include <QThread>
#include <QCoreApplication>
#include <algorithm>

namespace VisionForge {
namespace Base {

ErrorRecovery& ErrorRecovery::instance()
{
    static ErrorRecovery instance;
    return instance;
}

ErrorRecovery::ErrorRecovery()
    : QObject(nullptr)
    , maxErrorHistory_(100)
    , recovering_(false)
    , currentRetry_(0)
{
    // 设置默认恢复策略
    strategyMap_[""] = RecoveryStrategy::Notify;  // 全局默认

    LOG_DEBUG("错误恢复系统初始化完成");
}

// ========== 错误报告 ==========

RecoveryStrategy ErrorRecovery::reportError(const ErrorInfo& info)
{
    // 记录到日志
    switch (info.severity) {
        case ErrorSeverity::Warning:
            LOG_WARNING(QString("[%1] %2 - %3").arg(info.code).arg(info.source).arg(info.message));
            break;
        case ErrorSeverity::Error:
            LOG_ERROR(QString("[%1] %2 - %3").arg(info.code).arg(info.source).arg(info.message));
            break;
        case ErrorSeverity::Critical:
        case ErrorSeverity::Fatal:
            LOG_ERROR(QString("[CRITICAL][%1] %2 - %3").arg(info.code).arg(info.source).arg(info.message));
            break;
    }

    // 添加到历史
    addToHistory(info);

    // 发送信号
    emit errorOccurred(info);

    // 确定恢复策略
    RecoveryStrategy strategy = determineStrategy(info);

    // 执行恢复
    if (strategy != RecoveryStrategy::Ignore && strategy != RecoveryStrategy::Notify) {
        emit recoveryStarted(info.code, strategy);
        bool success = executeRecovery(info, strategy);
        emit recoveryCompleted(info.code, success);
    }

    return strategy;
}

RecoveryStrategy ErrorRecovery::reportError(const QString& code,
                                            const QString& message,
                                            ErrorSeverity severity,
                                            const QString& source)
{
    ErrorInfo info;
    info.code = code;
    info.message = message;
    info.severity = severity;
    info.source = source;
    return reportError(info);
}

// ========== 恢复策略 ==========

void ErrorRecovery::setRecoveryStrategy(const QString& code, RecoveryStrategy strategy)
{
    strategyMap_[code] = strategy;
}

RecoveryStrategy ErrorRecovery::getRecoveryStrategy(const QString& code) const
{
    if (strategyMap_.contains(code)) {
        return strategyMap_[code];
    }
    return strategyMap_.value("", RecoveryStrategy::Notify);
}

void ErrorRecovery::registerRecoveryHandler(const QString& code, RecoveryHandler handler)
{
    handlers_[code] = handler;
}

RecoveryStrategy ErrorRecovery::determineStrategy(const ErrorInfo& info) const
{
    // 致命错误必须中止
    if (info.severity == ErrorSeverity::Fatal) {
        return RecoveryStrategy::Abort;
    }

    // 查找特定错误代码的策略
    if (strategyMap_.contains(info.code)) {
        return strategyMap_[info.code];
    }

    // 根据严重程度返回默认策略
    switch (info.severity) {
        case ErrorSeverity::Warning:
            return RecoveryStrategy::Ignore;
        case ErrorSeverity::Error:
            return RecoveryStrategy::Retry;
        case ErrorSeverity::Critical:
            return RecoveryStrategy::Rollback;
        default:
            return RecoveryStrategy::Notify;
    }
}

bool ErrorRecovery::executeRecovery(const ErrorInfo& info, RecoveryStrategy strategy)
{
    recovering_.store(true);

    bool success = false;

    // 首先检查是否有自定义处理器
    if (handlers_.contains(info.code)) {
        try {
            success = handlers_[info.code](info);
        } catch (const std::exception& e) {
            LOG_ERROR(QString("恢复处理器异常: %1").arg(e.what()));
            success = false;
        }
    } else {
        // 使用默认恢复逻辑
        switch (strategy) {
            case RecoveryStrategy::Retry:
                // 重试逻辑由 executeWithRetry 处理
                success = true;
                break;

            case RecoveryStrategy::Rollback:
                if (!lastCheckpointId_.isEmpty()) {
                    QJsonObject state = restoreCheckpoint(lastCheckpointId_);
                    success = !state.isEmpty();
                    if (success) {
                        LOG_INFO(QString("已回滚到检查点: %1").arg(lastCheckpointId_));
                    }
                } else {
                    LOG_WARNING("没有可用的检查点进行回滚");
                    success = false;
                }
                break;

            case RecoveryStrategy::Skip:
                LOG_INFO(QString("跳过错误操作: %1").arg(info.code));
                success = true;
                break;

            case RecoveryStrategy::Abort:
                LOG_ERROR(QString("中止操作: %1").arg(info.code));
                success = false;
                break;

            default:
                success = true;
                break;
        }
    }

    recovering_.store(false);
    return success;
}

// ========== 重试机制 ==========

bool ErrorRecovery::executeWithRetry(std::function<bool()> operation,
                                     const QString& errorCode)
{
    return executeWithRetry(operation, nullptr, errorCode);
}

bool ErrorRecovery::executeWithRetry(std::function<bool()> operation,
                                     std::function<void(int, int)> progressCallback,
                                     const QString& errorCode)
{
    int attempt = 0;
    int delayMs = retryConfig_.initialDelayMs;

    while (attempt <= retryConfig_.maxRetries) {
        currentRetry_.store(attempt);

        if (progressCallback) {
            progressCallback(attempt, retryConfig_.maxRetries);
        }
        emit retryProgress(attempt, retryConfig_.maxRetries);

        try {
            if (operation()) {
                currentRetry_.store(0);
                return true;
            }
        } catch (const std::exception& e) {
            LOG_WARNING(QString("重试 %1/%2 失败: %3")
                       .arg(attempt + 1)
                       .arg(retryConfig_.maxRetries + 1)
                       .arg(e.what()));
        }

        if (attempt < retryConfig_.maxRetries) {
            LOG_DEBUG(QString("等待 %1ms 后重试...").arg(delayMs));
            QThread::msleep(delayMs);

            // 计算下次延迟
            delayMs = calculateDelay(attempt);
        }

        attempt++;
    }

    currentRetry_.store(0);

    // 所有重试都失败
    reportError(errorCode,
               QString("操作在 %1 次重试后仍然失败").arg(retryConfig_.maxRetries + 1),
               ErrorSeverity::Error);

    return false;
}

int ErrorRecovery::calculateDelay(int attempt) const
{
    int delay;

    if (retryConfig_.exponentialBackoff) {
        delay = static_cast<int>(retryConfig_.initialDelayMs *
                                 std::pow(retryConfig_.backoffMultiplier, attempt));
    } else {
        delay = retryConfig_.initialDelayMs;
    }

    return std::min(delay, retryConfig_.maxDelayMs);
}

// ========== 检查点 ==========

void ErrorRecovery::createCheckpoint(const QString& id,
                                     const QString& description,
                                     const QJsonObject& state)
{
    Checkpoint cp;
    cp.id = id;
    cp.description = description;
    cp.state = state;
    cp.timestamp = std::chrono::system_clock::now();

    checkpoints_[id] = cp;
    lastCheckpointId_ = id;

    LOG_DEBUG(QString("创建检查点: %1 - %2").arg(id).arg(description));
    emit checkpointCreated(id);
}

QJsonObject ErrorRecovery::restoreCheckpoint(const QString& id)
{
    if (!checkpoints_.contains(id)) {
        LOG_WARNING(QString("检查点不存在: %1").arg(id));
        return QJsonObject();
    }

    const Checkpoint& cp = checkpoints_[id];
    LOG_INFO(QString("恢复检查点: %1 - %2").arg(id).arg(cp.description));

    emit checkpointRestored(id);
    return cp.state;
}

void ErrorRecovery::removeCheckpoint(const QString& id)
{
    checkpoints_.remove(id);
    if (lastCheckpointId_ == id) {
        // 更新最后检查点
        if (!checkpoints_.isEmpty()) {
            lastCheckpointId_ = checkpoints_.lastKey();
        } else {
            lastCheckpointId_.clear();
        }
    }
}

void ErrorRecovery::clearCheckpoints()
{
    checkpoints_.clear();
    lastCheckpointId_.clear();
}

QStringList ErrorRecovery::checkpointIds() const
{
    return checkpoints_.keys();
}

bool ErrorRecovery::hasCheckpoint(const QString& id) const
{
    return checkpoints_.contains(id);
}

QString ErrorRecovery::lastCheckpointId() const
{
    return lastCheckpointId_;
}

// ========== 错误历史 ==========

QVector<ErrorInfo> ErrorRecovery::errorHistory(int maxCount) const
{
    if (maxCount <= 0 || maxCount >= errorHistory_.size()) {
        return errorHistory_;
    }

    // 返回最近的maxCount条记录
    return QVector<ErrorInfo>(errorHistory_.end() - maxCount, errorHistory_.end());
}

void ErrorRecovery::clearErrorHistory()
{
    errorHistory_.clear();
}

void ErrorRecovery::addToHistory(const ErrorInfo& info)
{
    errorHistory_.append(info);

    // 限制历史大小
    while (errorHistory_.size() > maxErrorHistory_) {
        errorHistory_.removeFirst();
    }
}

// ============================================================
// ScopedCheckpoint
// ============================================================

ScopedCheckpoint::ScopedCheckpoint(const QString& id,
                                   const QString& description,
                                   const QJsonObject& state)
    : id_(id)
    , committed_(false)
{
    ErrorRecovery::instance().createCheckpoint(id, description, state);
}

ScopedCheckpoint::~ScopedCheckpoint()
{
    if (!committed_) {
        // 未提交，执行回滚
        rollback();
    }
    // 清理检查点
    ErrorRecovery::instance().removeCheckpoint(id_);
}

void ScopedCheckpoint::commit()
{
    committed_ = true;
}

void ScopedCheckpoint::rollback()
{
    if (!committed_) {
        ErrorRecovery::instance().restoreCheckpoint(id_);
    }
}

// ============================================================
// RetryScope
// ============================================================

RetryScope::RetryScope(const RetryConfig& config)
    : previousConfig_(ErrorRecovery::instance().retryConfig())
{
    ErrorRecovery::instance().setRetryConfig(config);
}

RetryScope::~RetryScope()
{
    ErrorRecovery::instance().setRetryConfig(previousConfig_);
}

} // namespace Base
} // namespace VisionForge
