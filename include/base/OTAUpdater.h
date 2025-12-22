/**
 * @file OTAUpdater.h
 * @brief OTA远程升级系统
 * @details 实现软件的远程自动更新功能
 *
 * 功能特性：
 * - 版本检查：定期检查服务器上的新版本
 * - 增量更新：支持差分包下载，减少流量
 * - 断点续传：下载中断后可继续
 * - 完整性校验：MD5/SHA256校验确保包完整
 * - 回滚机制：升级失败自动回滚到旧版本
 * - 静默安装：支持后台静默安装
 *
 * @author VisionForge Team
 * @date 2025-12-22
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <QTimer>
#include <QMutex>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <functional>
#include <memory>

namespace VisionForge {
namespace Base {

// ============================================================
// 类型定义
// ============================================================

/**
 * @brief 版本信息
 */
struct VersionInfo {
    QString version;            ///< 版本号 (如 "1.2.3")
    int major = 0;              ///< 主版本号
    int minor = 0;              ///< 次版本号
    int patch = 0;              ///< 修订号
    int build = 0;              ///< 构建号
    QString releaseDate;        ///< 发布日期
    QString releaseNotes;       ///< 发布说明
    QStringList changeLog;      ///< 更新日志
    bool isMandatory = false;   ///< 是否强制更新

    static VersionInfo fromString(const QString& versionStr);
    QString toString() const;
    bool operator>(const VersionInfo& other) const;
    bool operator==(const VersionInfo& other) const;
    bool operator<(const VersionInfo& other) const { return other > *this; }

    QJsonObject toJson() const;
    static VersionInfo fromJson(const QJsonObject& json);
};

/**
 * @brief 更新包信息
 */
struct UpdatePackageInfo {
    QString packageUrl;         ///< 下载地址
    QString fileName;           ///< 文件名
    qint64 fileSize = 0;        ///< 文件大小（字节）
    QString md5Hash;            ///< MD5校验值
    QString sha256Hash;         ///< SHA256校验值
    QString packageType;        ///< 包类型（full/delta）
    QString baseVersion;        ///< 基础版本（增量包）
    QString targetVersion;      ///< 目标版本
    QStringList dependencies;   ///< 依赖组件

    QJsonObject toJson() const;
    static UpdatePackageInfo fromJson(const QJsonObject& json);
};

/**
 * @brief OTA更新状态
 */
enum class OTAState {
    Idle,                   ///< 空闲
    CheckingVersion,        ///< 检查版本中
    UpdateAvailable,        ///< 有新版本
    Downloading,            ///< 下载中
    Verifying,              ///< 校验中
    ReadyToInstall,         ///< 准备安装
    Installing,             ///< 安装中
    Completed,              ///< 完成
    RollingBack,            ///< 回滚中
    Error                   ///< 错误
};

/**
 * @brief OTA配置
 */
struct OTAConfig {
    QString updateServerUrl;            ///< 更新服务器URL
    QString currentVersion;             ///< 当前版本
    QString applicationName;            ///< 应用程序名称
    QString channel = "stable";         ///< 更新通道 (stable/beta/dev)
    QString downloadPath;               ///< 下载目录
    QString backupPath;                 ///< 备份目录
    bool autoCheck = true;              ///< 自动检查更新
    int checkIntervalHours = 24;        ///< 检查间隔（小时）
    bool autoDownload = false;          ///< 自动下载
    bool autoInstall = false;           ///< 自动安装
    bool allowBeta = false;             ///< 允许Beta版本
    int downloadTimeoutSec = 300;       ///< 下载超时（秒）
    int maxRetries = 3;                 ///< 最大重试次数
    bool enableDeltaUpdate = true;      ///< 启用增量更新
    QString proxyServer;                ///< 代理服务器
    int proxyPort = 0;                  ///< 代理端口

    QJsonObject toJson() const;
    static OTAConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 下载进度信息
 */
struct DownloadProgress {
    qint64 bytesReceived = 0;       ///< 已下载字节
    qint64 bytesTotal = 0;          ///< 总字节
    double speedBytesPerSec = 0;    ///< 下载速度
    int progressPercent = 0;        ///< 进度百分比
    int remainingSeconds = 0;       ///< 剩余时间（秒）
    QString currentFile;            ///< 当前文件
};

// ============================================================
// OTAUpdater 类
// ============================================================

/**
 * @class OTAUpdater
 * @brief OTA更新管理器
 *
 * 使用方法：
 * @code
 * OTAUpdater& updater = OTAUpdater::instance();
 *
 * OTAConfig config;
 * config.updateServerUrl = "https://update.example.com/api";
 * config.currentVersion = "1.0.0";
 * config.applicationName = "VisionForge";
 * updater.setConfig(config);
 *
 * // 检查更新
 * updater.checkForUpdate();
 *
 * // 连接信号
 * connect(&updater, &OTAUpdater::updateAvailable, [](const VersionInfo& info) {
 *     qDebug() << "新版本:" << info.version;
 * });
 * @endcode
 */
class OTAUpdater : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static OTAUpdater& instance();

    // ========== 配置管理 ==========

    /**
     * @brief 设置配置
     */
    void setConfig(const OTAConfig& config);

    /**
     * @brief 获取配置
     */
    OTAConfig config() const { return config_; }

    /**
     * @brief 从文件加载配置
     */
    bool loadConfig(const QString& filePath);

    /**
     * @brief 保存配置到文件
     */
    bool saveConfig(const QString& filePath) const;

    // ========== 版本检查 ==========

    /**
     * @brief 检查更新
     * @param forceCheck 强制检查（忽略缓存）
     */
    void checkForUpdate(bool forceCheck = false);

    /**
     * @brief 获取当前版本
     */
    VersionInfo currentVersion() const;

    /**
     * @brief 获取最新版本信息
     */
    VersionInfo latestVersion() const { return latestVersion_; }

    /**
     * @brief 是否有可用更新
     */
    bool hasUpdate() const { return hasUpdate_; }

    // ========== 下载控制 ==========

    /**
     * @brief 开始下载更新
     */
    bool startDownload();

    /**
     * @brief 暂停下载
     */
    void pauseDownload();

    /**
     * @brief 继续下载
     */
    void resumeDownload();

    /**
     * @brief 取消下载
     */
    void cancelDownload();

    /**
     * @brief 获取下载进度
     */
    DownloadProgress downloadProgress() const { return downloadProgress_; }

    // ========== 安装控制 ==========

    /**
     * @brief 开始安装
     * @param silentInstall 静默安装
     */
    bool startInstall(bool silentInstall = false);

    /**
     * @brief 创建备份
     */
    bool createBackup();

    /**
     * @brief 回滚到旧版本
     */
    bool rollback();

    // ========== 状态查询 ==========

    /**
     * @brief 获取当前状态
     */
    OTAState state() const { return state_; }

    /**
     * @brief 获取状态字符串
     */
    QString stateString() const;

    /**
     * @brief 获取最后错误
     */
    QString lastError() const { return lastError_; }

    // ========== 自动更新控制 ==========

    /**
     * @brief 启动自动检查
     */
    void startAutoCheck();

    /**
     * @brief 停止自动检查
     */
    void stopAutoCheck();

    /**
     * @brief 是否正在自动检查
     */
    bool isAutoCheckRunning() const;

signals:
    /**
     * @brief 状态变化
     */
    void stateChanged(OTAState newState);

    /**
     * @brief 有可用更新
     */
    void updateAvailable(const VersionInfo& version, const UpdatePackageInfo& package);

    /**
     * @brief 无可用更新
     */
    void noUpdateAvailable();

    /**
     * @brief 下载进度
     */
    void downloadProgressChanged(const DownloadProgress& progress);

    /**
     * @brief 下载完成
     */
    void downloadCompleted(const QString& filePath);

    /**
     * @brief 下载失败
     */
    void downloadFailed(const QString& error);

    /**
     * @brief 安装开始
     */
    void installStarted();

    /**
     * @brief 安装进度
     */
    void installProgress(int percent, const QString& status);

    /**
     * @brief 安装完成
     */
    void installCompleted();

    /**
     * @brief 安装失败
     */
    void installFailed(const QString& error);

    /**
     * @brief 需要重启
     */
    void restartRequired();

    /**
     * @brief 错误发生
     */
    void errorOccurred(const QString& error);

private slots:
    void onAutoCheckTimer();
    void onVersionCheckReply();
    void onDownloadProgress(qint64 received, qint64 total);
    void onDownloadFinished();
    void onDownloadError();

private:
    OTAUpdater();
    ~OTAUpdater();
    OTAUpdater(const OTAUpdater&) = delete;
    OTAUpdater& operator=(const OTAUpdater&) = delete;

    void setState(OTAState newState);
    void setError(const QString& error);
    bool verifyPackage(const QString& filePath);
    QString calculateFileHash(const QString& filePath, const QString& algorithm);
    bool extractPackage(const QString& packagePath, const QString& targetDir);
    bool applyUpdate(const QString& updateDir);
    void cleanupTempFiles();

private:
    OTAConfig config_;
    OTAState state_ = OTAState::Idle;
    QString lastError_;

    VersionInfo latestVersion_;
    UpdatePackageInfo packageInfo_;
    bool hasUpdate_ = false;

    QNetworkAccessManager* networkManager_ = nullptr;
    QNetworkReply* currentReply_ = nullptr;
    QFile* downloadFile_ = nullptr;
    DownloadProgress downloadProgress_;

    QTimer* autoCheckTimer_ = nullptr;
    QDateTime lastCheckTime_;

    qint64 downloadStartTime_ = 0;
    QString downloadedFilePath_;

    mutable QMutex mutex_;
};

} // namespace Base
} // namespace VisionForge
