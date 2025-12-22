/**
 * @file ApplicationWatchdog.cpp
 * @brief 应用程序看门狗守护系统实现
 * @author VisionForge Team
 * @date 2025-12-22
 */

#include "base/ApplicationWatchdog.h"
#include "base/Logger.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <QCoreApplication>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#endif

namespace VisionForge {
namespace Base {

const QString ApplicationWatchdog::SHARED_MEMORY_KEY = "VisionForge_Watchdog_Heartbeat";

// ============================================================
// WatchdogConfig 实现
// ============================================================

QJsonObject WatchdogConfig::toJson() const
{
    QJsonObject json;
    json["heartbeatTimeoutMs"] = heartbeatTimeoutMs;
    json["maxRestartAttempts"] = maxRestartAttempts;
    json["restartCooldownMs"] = restartCooldownMs;
    json["memoryLimitMB"] = memoryLimitMB;
    json["cpuLimitPercent"] = cpuLimitPercent;
    json["resourceCheckIntervalMs"] = resourceCheckIntervalMs;
    json["applicationPath"] = applicationPath;
    json["applicationArgs"] = QJsonArray::fromStringList(applicationArgs);
    json["logFilePath"] = logFilePath;
    json["enableRemoteNotification"] = enableRemoteNotification;
    json["notificationEndpoint"] = notificationEndpoint;
    return json;
}

WatchdogConfig WatchdogConfig::fromJson(const QJsonObject& json)
{
    WatchdogConfig config;
    config.heartbeatTimeoutMs = json.value("heartbeatTimeoutMs").toInt(10000);
    config.maxRestartAttempts = json.value("maxRestartAttempts").toInt(3);
    config.restartCooldownMs = json.value("restartCooldownMs").toInt(60000);
    config.memoryLimitMB = json.value("memoryLimitMB").toInt(2048);
    config.cpuLimitPercent = json.value("cpuLimitPercent").toInt(90);
    config.resourceCheckIntervalMs = json.value("resourceCheckIntervalMs").toInt(5000);
    config.applicationPath = json.value("applicationPath").toString();

    QJsonArray argsArray = json.value("applicationArgs").toArray();
    for (const auto& arg : argsArray) {
        config.applicationArgs << arg.toString();
    }

    config.logFilePath = json.value("logFilePath").toString();
    config.enableRemoteNotification = json.value("enableRemoteNotification").toBool(false);
    config.notificationEndpoint = json.value("notificationEndpoint").toString();
    return config;
}

// ============================================================
// WatchdogStats 实现
// ============================================================

QJsonObject WatchdogStats::toJson() const
{
    QJsonObject json;
    json["startTime"] = startTime.toString(Qt::ISODate);
    json["totalRestarts"] = totalRestarts;
    json["heartbeatCount"] = heartbeatCount;
    json["timeoutCount"] = timeoutCount;
    json["crashCount"] = crashCount;
    json["avgHeartbeatIntervalMs"] = avgHeartbeatIntervalMs;
    json["totalUptimeMs"] = totalUptimeMs;
    json["lastHeartbeat"] = lastHeartbeat.toString(Qt::ISODate);
    json["lastRestart"] = lastRestart.toString(Qt::ISODate);
    json["currentMemoryMB"] = currentMemoryMB;
    json["currentCpuPercent"] = currentCpuPercent;
    return json;
}

// ============================================================
// ApplicationWatchdog 实现
// ============================================================

ApplicationWatchdog::ApplicationWatchdog(QObject* parent)
    : QObject(parent)
{
    heartbeatTimer_ = new QTimer(this);
    resourceTimer_ = new QTimer(this);
    cooldownTimer_ = new QTimer(this);

    cooldownTimer_->setSingleShot(true);

    connect(heartbeatTimer_, &QTimer::timeout, this, &ApplicationWatchdog::onHeartbeatCheck);
    connect(resourceTimer_, &QTimer::timeout, this, &ApplicationWatchdog::onResourceCheck);
    connect(cooldownTimer_, &QTimer::timeout, this, &ApplicationWatchdog::onCooldownFinished);
}

ApplicationWatchdog::~ApplicationWatchdog()
{
    stop();
    cleanupSharedMemory();
}

void ApplicationWatchdog::setConfig(const WatchdogConfig& config)
{
    QMutexLocker locker(&mutex_);
    config_ = config;
    emitEvent(WatchdogEvent::ConfigChanged, "配置已更新");
}

bool ApplicationWatchdog::loadConfig(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开Watchdog配置文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR("Watchdog配置文件格式错误");
        return false;
    }

    setConfig(WatchdogConfig::fromJson(doc.object()));
    return true;
}

bool ApplicationWatchdog::saveConfig(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法保存Watchdog配置文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(config_.toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool ApplicationWatchdog::start()
{
    QMutexLocker locker(&mutex_);

    if (state_ != WatchdogState::Idle) {
        LOG_WARNING("Watchdog已在运行中");
        return false;
    }

    // 初始化共享内存
    if (!initSharedMemory()) {
        LOG_ERROR("无法初始化共享内存");
        return false;
    }

    // 初始化日志文件
    if (!config_.logFilePath.isEmpty()) {
        logFile_.setFileName(config_.logFilePath);
        if (!logFile_.open(QIODevice::Append | QIODevice::Text)) {
            LOG_WARNING(QString("无法打开Watchdog日志文件: %1").arg(config_.logFilePath));
        }
    }

    // 初始化统计
    stats_ = WatchdogStats();
    stats_.startTime = QDateTime::currentDateTime();

    // 启动应用程序
    if (!startApplication()) {
        LOG_ERROR("无法启动被监控应用程序");
        cleanupSharedMemory();
        return false;
    }

    // 启动定时器
    heartbeatTimer_->start(config_.heartbeatTimeoutMs / 2);
    resourceTimer_->start(config_.resourceCheckIntervalMs);

    setState(WatchdogState::Monitoring);
    emitEvent(WatchdogEvent::Started, "Watchdog已启动");

    LOG_INFO("ApplicationWatchdog 已启动监控");
    return true;
}

void ApplicationWatchdog::stop()
{
    QMutexLocker locker(&mutex_);

    heartbeatTimer_->stop();
    resourceTimer_->stop();
    cooldownTimer_->stop();

    if (appProcess_ && appProcess_->state() != QProcess::NotRunning) {
        appProcess_->terminate();
        if (!appProcess_->waitForFinished(5000)) {
            appProcess_->kill();
        }
    }

    if (logFile_.isOpen()) {
        logFile_.close();
    }

    cleanupSharedMemory();
    setState(WatchdogState::Idle);
    emitEvent(WatchdogEvent::Stopped, "Watchdog已停止");

    LOG_INFO("ApplicationWatchdog 已停止");
}

QString ApplicationWatchdog::stateString() const
{
    switch (state_) {
    case WatchdogState::Idle: return "空闲";
    case WatchdogState::Monitoring: return "监控中";
    case WatchdogState::Restarting: return "重启中";
    case WatchdogState::Cooldown: return "冷却中";
    case WatchdogState::Failed: return "失败";
    default: return "未知";
    }
}

bool ApplicationWatchdog::restartApplication()
{
    QMutexLocker locker(&mutex_);

    emitEvent(WatchdogEvent::ManualRestart, "手动触发重启");
    currentRestartAttempts_ = 0;  // 手动重启不计入重启次数

    if (appProcess_ && appProcess_->state() != QProcess::NotRunning) {
        appProcess_->terminate();
        if (!appProcess_->waitForFinished(5000)) {
            appProcess_->kill();
        }
    }

    return startApplication();
}

bool ApplicationWatchdog::stopApplication()
{
    QMutexLocker locker(&mutex_);

    if (!appProcess_ || appProcess_->state() == QProcess::NotRunning) {
        return true;
    }

    appProcess_->terminate();
    if (!appProcess_->waitForFinished(5000)) {
        appProcess_->kill();
        appProcess_->waitForFinished(1000);
    }

    emitEvent(WatchdogEvent::ApplicationStopped, "应用程序已停止");
    return true;
}

bool ApplicationWatchdog::sendHeartbeat()
{
    QSharedMemory shm(SHARED_MEMORY_KEY);
    if (!shm.attach()) {
        return false;
    }

    shm.lock();
    char* data = static_cast<char*>(shm.data());
    if (data) {
        // 写入时间戳
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        memcpy(data, &timestamp, sizeof(qint64));
    }
    shm.unlock();
    shm.detach();

    return true;
}

bool ApplicationWatchdog::sendHeartbeat(const QJsonObject& extraData)
{
    QSharedMemory shm(SHARED_MEMORY_KEY);
    if (!shm.attach()) {
        return false;
    }

    shm.lock();
    char* data = static_cast<char*>(shm.data());
    if (data) {
        // 写入时间戳
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        memcpy(data, &timestamp, sizeof(qint64));

        // 写入附加数据（如果有空间）
        if (shm.size() > sizeof(qint64) + 4) {
            QByteArray jsonData = QJsonDocument(extraData).toJson(QJsonDocument::Compact);
            int jsonSize = qMin(jsonData.size(), shm.size() - static_cast<int>(sizeof(qint64)) - 4);
            memcpy(data + sizeof(qint64), &jsonSize, sizeof(int));
            memcpy(data + sizeof(qint64) + sizeof(int), jsonData.data(), jsonSize);
        }
    }
    shm.unlock();
    shm.detach();

    return true;
}

void ApplicationWatchdog::resetStats()
{
    QMutexLocker locker(&mutex_);
    stats_ = WatchdogStats();
    stats_.startTime = QDateTime::currentDateTime();
    emit statsUpdated(stats_);
}

void ApplicationWatchdog::onHeartbeatCheck()
{
    if (!checkHeartbeat()) {
        stats_.timeoutCount++;

        if (state_ == WatchdogState::Monitoring) {
            emitEvent(WatchdogEvent::HeartbeatTimeout,
                     QString("心跳超时，上次心跳: %1")
                     .arg(lastHeartbeatTime_.toString("hh:mm:ss.zzz")));

            // 尝试重启
            if (currentRestartAttempts_ < config_.maxRestartAttempts) {
                setState(WatchdogState::Restarting);
                currentRestartAttempts_++;
                stats_.totalRestarts++;

                if (appProcess_ && appProcess_->state() != QProcess::NotRunning) {
                    appProcess_->terminate();
                    if (!appProcess_->waitForFinished(3000)) {
                        appProcess_->kill();
                    }
                }

                if (startApplication()) {
                    emitEvent(WatchdogEvent::ApplicationRestarted,
                             QString("应用程序已重启 (尝试 %1/%2)")
                             .arg(currentRestartAttempts_)
                             .arg(config_.maxRestartAttempts));
                    stats_.lastRestart = QDateTime::currentDateTime();

                    // 进入冷却期
                    setState(WatchdogState::Cooldown);
                    cooldownTimer_->start(config_.restartCooldownMs);
                }
            } else {
                setState(WatchdogState::Failed);
                emitEvent(WatchdogEvent::MaxRestartsReached,
                         QString("已达到最大重启次数 (%1)")
                         .arg(config_.maxRestartAttempts));
            }
        }
    } else {
        stats_.heartbeatCount++;
        stats_.lastHeartbeat = lastHeartbeatTime_;

        // 重置重启计数（应用程序正常运行）
        if (state_ == WatchdogState::Monitoring) {
            currentRestartAttempts_ = 0;
        }

        emitEvent(WatchdogEvent::HeartbeatReceived);
    }

    updateStats();
}

void ApplicationWatchdog::onResourceCheck()
{
    checkResources();
    updateStats();
}

void ApplicationWatchdog::onCooldownFinished()
{
    QMutexLocker locker(&mutex_);

    if (state_ == WatchdogState::Cooldown) {
        setState(WatchdogState::Monitoring);
        LOG_INFO("Watchdog冷却期结束，恢复监控");
    }
}

void ApplicationWatchdog::onApplicationFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QMutexLocker locker(&mutex_);

    if (exitStatus == QProcess::CrashExit) {
        stats_.crashCount++;
        emitEvent(WatchdogEvent::ApplicationCrashed,
                 QString("应用程序崩溃，退出代码: %1").arg(exitCode));
    } else {
        emitEvent(WatchdogEvent::ApplicationStopped,
                 QString("应用程序退出，退出代码: %1").arg(exitCode));
    }

    // 如果处于监控状态，尝试重启
    if (state_ == WatchdogState::Monitoring || state_ == WatchdogState::Cooldown) {
        if (currentRestartAttempts_ < config_.maxRestartAttempts) {
            currentRestartAttempts_++;
            stats_.totalRestarts++;

            QTimer::singleShot(1000, this, [this]() {
                if (startApplication()) {
                    emitEvent(WatchdogEvent::ApplicationRestarted,
                             QString("应用程序已重启 (尝试 %1/%2)")
                             .arg(currentRestartAttempts_)
                             .arg(config_.maxRestartAttempts));
                    stats_.lastRestart = QDateTime::currentDateTime();
                }
            });
        } else {
            setState(WatchdogState::Failed);
            emitEvent(WatchdogEvent::MaxRestartsReached,
                     QString("已达到最大重启次数 (%1)")
                     .arg(config_.maxRestartAttempts));
        }
    }
}

void ApplicationWatchdog::onApplicationError(QProcess::ProcessError error)
{
    QString errorMsg;
    switch (error) {
    case QProcess::FailedToStart:
        errorMsg = "启动失败";
        break;
    case QProcess::Crashed:
        errorMsg = "崩溃";
        break;
    case QProcess::Timedout:
        errorMsg = "超时";
        break;
    case QProcess::WriteError:
        errorMsg = "写入错误";
        break;
    case QProcess::ReadError:
        errorMsg = "读取错误";
        break;
    default:
        errorMsg = "未知错误";
    }

    LOG_ERROR(QString("应用程序进程错误: %1").arg(errorMsg));
}

void ApplicationWatchdog::onApplicationReadyRead()
{
    if (appProcess_) {
        QString output = QString::fromLocal8Bit(appProcess_->readAllStandardOutput());
        QString error = QString::fromLocal8Bit(appProcess_->readAllStandardError());

        if (!output.isEmpty()) {
            emit applicationOutput(output);
        }
        if (!error.isEmpty()) {
            emit applicationOutput("[ERROR] " + error);
        }
    }
}

void ApplicationWatchdog::setState(WatchdogState newState)
{
    if (state_ != newState) {
        state_ = newState;
        emit stateChanged(newState);
    }
}

void ApplicationWatchdog::emitEvent(WatchdogEvent event, const QString& message)
{
    logEvent(event, message);
    emit eventOccurred(event, message);

    if (eventCallback_) {
        eventCallback_(event, message);
    }

    if (config_.enableRemoteNotification) {
        sendRemoteNotification(event, message);
    }
}

void ApplicationWatchdog::logEvent(WatchdogEvent event, const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString eventStr = eventToString(event);
    QString logLine = QString("[%1] [%2] %3\n").arg(timestamp, eventStr, message);

    if (logFile_.isOpen()) {
        logFile_.write(logLine.toUtf8());
        logFile_.flush();
    }

    // 同时输出到系统日志
    switch (event) {
    case WatchdogEvent::ApplicationCrashed:
    case WatchdogEvent::MaxRestartsReached:
    case WatchdogEvent::MemoryExceeded:
    case WatchdogEvent::CpuExceeded:
        LOG_ERROR(QString("[Watchdog] %1: %2").arg(eventStr, message));
        break;
    case WatchdogEvent::HeartbeatTimeout:
    case WatchdogEvent::MemoryWarning:
    case WatchdogEvent::CpuWarning:
        LOG_WARNING(QString("[Watchdog] %1: %2").arg(eventStr, message));
        break;
    default:
        LOG_INFO(QString("[Watchdog] %1: %2").arg(eventStr, message));
    }
}

bool ApplicationWatchdog::startApplication()
{
    if (config_.applicationPath.isEmpty()) {
        LOG_ERROR("未配置应用程序路径");
        return false;
    }

    if (!QFile::exists(config_.applicationPath)) {
        LOG_ERROR(QString("应用程序不存在: %1").arg(config_.applicationPath));
        return false;
    }

    if (!appProcess_) {
        appProcess_ = new QProcess(this);
        connect(appProcess_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &ApplicationWatchdog::onApplicationFinished);
        connect(appProcess_, &QProcess::errorOccurred,
                this, &ApplicationWatchdog::onApplicationError);
        connect(appProcess_, &QProcess::readyReadStandardOutput,
                this, &ApplicationWatchdog::onApplicationReadyRead);
        connect(appProcess_, &QProcess::readyReadStandardError,
                this, &ApplicationWatchdog::onApplicationReadyRead);
    }

    // 设置工作目录
    QFileInfo appInfo(config_.applicationPath);
    appProcess_->setWorkingDirectory(appInfo.absolutePath());

    // 启动应用程序
    appProcess_->start(config_.applicationPath, config_.applicationArgs);

    if (!appProcess_->waitForStarted(5000)) {
        LOG_ERROR(QString("应用程序启动超时: %1").arg(config_.applicationPath));
        return false;
    }

    lastHeartbeatTime_ = QDateTime::currentDateTime();
    emitEvent(WatchdogEvent::ApplicationStarted,
             QString("应用程序已启动 (PID: %1)").arg(appProcess_->processId()));

    return true;
}

bool ApplicationWatchdog::checkHeartbeat()
{
    if (!sharedMemory_ || !sharedMemory_->isAttached()) {
        return false;
    }

    sharedMemory_->lock();
    const char* data = static_cast<const char*>(sharedMemory_->constData());
    qint64 timestamp = 0;
    if (data) {
        memcpy(&timestamp, data, sizeof(qint64));
    }
    sharedMemory_->unlock();

    if (timestamp > 0) {
        QDateTime heartbeatTime = QDateTime::fromMSecsSinceEpoch(timestamp);
        qint64 elapsed = lastHeartbeatTime_.msecsTo(heartbeatTime);

        if (elapsed > 0) {
            lastHeartbeatTime_ = heartbeatTime;
            return true;
        }

        // 检查是否超时
        qint64 sinceLastHeartbeat = heartbeatTime.msecsTo(QDateTime::currentDateTime());
        return sinceLastHeartbeat < config_.heartbeatTimeoutMs;
    }

    return false;
}

void ApplicationWatchdog::checkResources()
{
#ifdef Q_OS_WIN
    if (!appProcess_ || appProcess_->state() != QProcess::Running) {
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                   FALSE, appProcess_->processId());
    if (!hProcess) {
        return;
    }

    // 获取内存信息
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(hProcess, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
        stats_.currentMemoryMB = pmc.WorkingSetSize / (1024.0 * 1024.0);

        if (stats_.currentMemoryMB > config_.memoryLimitMB) {
            emitEvent(WatchdogEvent::MemoryExceeded,
                     QString("内存使用超限: %.2f MB > %d MB")
                     .arg(stats_.currentMemoryMB)
                     .arg(config_.memoryLimitMB));
        } else if (stats_.currentMemoryMB > config_.memoryLimitMB * 0.9) {
            emitEvent(WatchdogEvent::MemoryWarning,
                     QString("内存使用警告: %.2f MB (限制: %d MB)")
                     .arg(stats_.currentMemoryMB)
                     .arg(config_.memoryLimitMB));
        }
    }

    CloseHandle(hProcess);
#endif
}

void ApplicationWatchdog::sendRemoteNotification(WatchdogEvent event, const QString& message)
{
    if (config_.notificationEndpoint.isEmpty()) {
        return;
    }

    // 构建通知数据
    QJsonObject notification;
    notification["event"] = eventToString(event);
    notification["message"] = message;
    notification["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    notification["stats"] = stats_.toJson();

    // 发送HTTP请求
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(config_.notificationEndpoint));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = manager->post(request, QJsonDocument(notification).toJson());
    connect(reply, &QNetworkReply::finished, [reply, manager]() {
        reply->deleteLater();
        manager->deleteLater();
    });
}

void ApplicationWatchdog::updateStats()
{
    if (stats_.startTime.isValid()) {
        stats_.totalUptimeMs = stats_.startTime.msecsTo(QDateTime::currentDateTime());
    }

    if (stats_.heartbeatCount > 1 && stats_.totalUptimeMs > 0) {
        stats_.avgHeartbeatIntervalMs = static_cast<double>(stats_.totalUptimeMs) / stats_.heartbeatCount;
    }

    emit statsUpdated(stats_);
}

QString ApplicationWatchdog::eventToString(WatchdogEvent event) const
{
    switch (event) {
    case WatchdogEvent::Started: return "STARTED";
    case WatchdogEvent::Stopped: return "STOPPED";
    case WatchdogEvent::HeartbeatReceived: return "HEARTBEAT";
    case WatchdogEvent::HeartbeatTimeout: return "TIMEOUT";
    case WatchdogEvent::ApplicationStarted: return "APP_STARTED";
    case WatchdogEvent::ApplicationStopped: return "APP_STOPPED";
    case WatchdogEvent::ApplicationCrashed: return "APP_CRASHED";
    case WatchdogEvent::ApplicationRestarted: return "APP_RESTARTED";
    case WatchdogEvent::MemoryWarning: return "MEM_WARNING";
    case WatchdogEvent::MemoryExceeded: return "MEM_EXCEEDED";
    case WatchdogEvent::CpuWarning: return "CPU_WARNING";
    case WatchdogEvent::CpuExceeded: return "CPU_EXCEEDED";
    case WatchdogEvent::MaxRestartsReached: return "MAX_RESTARTS";
    case WatchdogEvent::ManualRestart: return "MANUAL_RESTART";
    case WatchdogEvent::ConfigChanged: return "CONFIG_CHANGED";
    default: return "UNKNOWN";
    }
}

bool ApplicationWatchdog::initSharedMemory()
{
    cleanupSharedMemory();

    sharedMemory_ = new QSharedMemory(SHARED_MEMORY_KEY, this);

    // 尝试创建共享内存（1KB）
    if (!sharedMemory_->create(1024)) {
        // 如果已存在，尝试附加
        if (sharedMemory_->error() == QSharedMemory::AlreadyExists) {
            if (!sharedMemory_->attach()) {
                LOG_ERROR(QString("无法附加到共享内存: %1").arg(sharedMemory_->errorString()));
                return false;
            }
        } else {
            LOG_ERROR(QString("无法创建共享内存: %1").arg(sharedMemory_->errorString()));
            return false;
        }
    }

    // 初始化共享内存内容
    sharedMemory_->lock();
    memset(sharedMemory_->data(), 0, sharedMemory_->size());
    sharedMemory_->unlock();

    return true;
}

void ApplicationWatchdog::cleanupSharedMemory()
{
    if (sharedMemory_) {
        if (sharedMemory_->isAttached()) {
            sharedMemory_->detach();
        }
        delete sharedMemory_;
        sharedMemory_ = nullptr;
    }
}

// ============================================================
// HeartbeatClient 实现
// ============================================================

HeartbeatClient::HeartbeatClient(QObject* parent)
    : QObject(parent)
{
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &HeartbeatClient::onTimerTimeout);
}

HeartbeatClient::~HeartbeatClient()
{
    stop();
}

void HeartbeatClient::start(int intervalMs)
{
    timer_->start(intervalMs);
    LOG_INFO(QString("心跳客户端已启动，间隔: %1ms").arg(intervalMs));
}

void HeartbeatClient::stop()
{
    timer_->stop();
}

bool HeartbeatClient::isRunning() const
{
    return timer_->isActive();
}

bool HeartbeatClient::sendNow()
{
    bool success;
    if (extraData_.isEmpty()) {
        success = ApplicationWatchdog::sendHeartbeat();
    } else {
        success = ApplicationWatchdog::sendHeartbeat(extraData_);
    }

    if (success) {
        emit heartbeatSent();
    } else {
        emit heartbeatFailed("发送心跳失败");
    }

    return success;
}

void HeartbeatClient::onTimerTimeout()
{
    sendNow();
}

} // namespace Base
} // namespace VisionForge
