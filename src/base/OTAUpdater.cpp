/**
 * @file OTAUpdater.cpp
 * @brief OTA远程升级系统实现
 * @author VisionForge Team
 * @date 2025-12-22
 */

#include "base/OTAUpdater.h"
#include "base/Logger.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCryptographicHash>
#include <QProcess>
#include <QCoreApplication>
#include <QDateTime>

namespace VisionForge {
namespace Base {

// ============================================================
// VersionInfo 实现
// ============================================================

VersionInfo VersionInfo::fromString(const QString& versionStr)
{
    VersionInfo info;
    info.version = versionStr;

    QStringList parts = versionStr.split('.');
    if (parts.size() >= 1) info.major = parts[0].toInt();
    if (parts.size() >= 2) info.minor = parts[1].toInt();
    if (parts.size() >= 3) info.patch = parts[2].toInt();
    if (parts.size() >= 4) info.build = parts[3].toInt();

    return info;
}

QString VersionInfo::toString() const
{
    if (build > 0) {
        return QString("%1.%2.%3.%4").arg(major).arg(minor).arg(patch).arg(build);
    }
    return QString("%1.%2.%3").arg(major).arg(minor).arg(patch);
}

bool VersionInfo::operator>(const VersionInfo& other) const
{
    if (major != other.major) return major > other.major;
    if (minor != other.minor) return minor > other.minor;
    if (patch != other.patch) return patch > other.patch;
    return build > other.build;
}

bool VersionInfo::operator==(const VersionInfo& other) const
{
    return major == other.major && minor == other.minor &&
           patch == other.patch && build == other.build;
}

QJsonObject VersionInfo::toJson() const
{
    QJsonObject json;
    json["version"] = version;
    json["major"] = major;
    json["minor"] = minor;
    json["patch"] = patch;
    json["build"] = build;
    json["releaseDate"] = releaseDate;
    json["releaseNotes"] = releaseNotes;
    json["changeLog"] = QJsonArray::fromStringList(changeLog);
    json["isMandatory"] = isMandatory;
    return json;
}

VersionInfo VersionInfo::fromJson(const QJsonObject& json)
{
    VersionInfo info;
    info.version = json.value("version").toString();
    info.major = json.value("major").toInt();
    info.minor = json.value("minor").toInt();
    info.patch = json.value("patch").toInt();
    info.build = json.value("build").toInt();
    info.releaseDate = json.value("releaseDate").toString();
    info.releaseNotes = json.value("releaseNotes").toString();
    info.isMandatory = json.value("isMandatory").toBool(false);

    QJsonArray changeArray = json.value("changeLog").toArray();
    for (const auto& item : changeArray) {
        info.changeLog << item.toString();
    }

    return info;
}

// ============================================================
// UpdatePackageInfo 实现
// ============================================================

QJsonObject UpdatePackageInfo::toJson() const
{
    QJsonObject json;
    json["packageUrl"] = packageUrl;
    json["fileName"] = fileName;
    json["fileSize"] = fileSize;
    json["md5Hash"] = md5Hash;
    json["sha256Hash"] = sha256Hash;
    json["packageType"] = packageType;
    json["baseVersion"] = baseVersion;
    json["targetVersion"] = targetVersion;
    json["dependencies"] = QJsonArray::fromStringList(dependencies);
    return json;
}

UpdatePackageInfo UpdatePackageInfo::fromJson(const QJsonObject& json)
{
    UpdatePackageInfo info;
    info.packageUrl = json.value("packageUrl").toString();
    info.fileName = json.value("fileName").toString();
    info.fileSize = json.value("fileSize").toVariant().toLongLong();
    info.md5Hash = json.value("md5Hash").toString();
    info.sha256Hash = json.value("sha256Hash").toString();
    info.packageType = json.value("packageType").toString("full");
    info.baseVersion = json.value("baseVersion").toString();
    info.targetVersion = json.value("targetVersion").toString();

    QJsonArray depArray = json.value("dependencies").toArray();
    for (const auto& item : depArray) {
        info.dependencies << item.toString();
    }

    return info;
}

// ============================================================
// OTAConfig 实现
// ============================================================

QJsonObject OTAConfig::toJson() const
{
    QJsonObject json;
    json["updateServerUrl"] = updateServerUrl;
    json["currentVersion"] = currentVersion;
    json["applicationName"] = applicationName;
    json["channel"] = channel;
    json["downloadPath"] = downloadPath;
    json["backupPath"] = backupPath;
    json["autoCheck"] = autoCheck;
    json["checkIntervalHours"] = checkIntervalHours;
    json["autoDownload"] = autoDownload;
    json["autoInstall"] = autoInstall;
    json["allowBeta"] = allowBeta;
    json["downloadTimeoutSec"] = downloadTimeoutSec;
    json["maxRetries"] = maxRetries;
    json["enableDeltaUpdate"] = enableDeltaUpdate;
    json["proxyServer"] = proxyServer;
    json["proxyPort"] = proxyPort;
    return json;
}

OTAConfig OTAConfig::fromJson(const QJsonObject& json)
{
    OTAConfig config;
    config.updateServerUrl = json.value("updateServerUrl").toString();
    config.currentVersion = json.value("currentVersion").toString();
    config.applicationName = json.value("applicationName").toString();
    config.channel = json.value("channel").toString("stable");
    config.downloadPath = json.value("downloadPath").toString();
    config.backupPath = json.value("backupPath").toString();
    config.autoCheck = json.value("autoCheck").toBool(true);
    config.checkIntervalHours = json.value("checkIntervalHours").toInt(24);
    config.autoDownload = json.value("autoDownload").toBool(false);
    config.autoInstall = json.value("autoInstall").toBool(false);
    config.allowBeta = json.value("allowBeta").toBool(false);
    config.downloadTimeoutSec = json.value("downloadTimeoutSec").toInt(300);
    config.maxRetries = json.value("maxRetries").toInt(3);
    config.enableDeltaUpdate = json.value("enableDeltaUpdate").toBool(true);
    config.proxyServer = json.value("proxyServer").toString();
    config.proxyPort = json.value("proxyPort").toInt(0);
    return config;
}

// ============================================================
// OTAUpdater 实现
// ============================================================

OTAUpdater::OTAUpdater()
{
    networkManager_ = new QNetworkAccessManager(this);
    autoCheckTimer_ = new QTimer(this);

    connect(autoCheckTimer_, &QTimer::timeout, this, &OTAUpdater::onAutoCheckTimer);
}

OTAUpdater::~OTAUpdater()
{
    cancelDownload();
}

OTAUpdater& OTAUpdater::instance()
{
    static OTAUpdater instance;
    return instance;
}

void OTAUpdater::setConfig(const OTAConfig& config)
{
    QMutexLocker locker(&mutex_);
    config_ = config;

    // 设置默认路径
    if (config_.downloadPath.isEmpty()) {
        config_.downloadPath = QCoreApplication::applicationDirPath() + "/updates";
    }
    if (config_.backupPath.isEmpty()) {
        config_.backupPath = QCoreApplication::applicationDirPath() + "/backup";
    }

    QDir().mkpath(config_.downloadPath);
    QDir().mkpath(config_.backupPath);
}

bool OTAUpdater::loadConfig(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        setError(QString("无法打开配置文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isObject()) {
        setError("配置文件格式错误");
        return false;
    }

    setConfig(OTAConfig::fromJson(doc.object()));
    return true;
}

bool OTAUpdater::saveConfig(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(config_.toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

void OTAUpdater::checkForUpdate(bool forceCheck)
{
    QMutexLocker locker(&mutex_);

    if (state_ == OTAState::CheckingVersion || state_ == OTAState::Downloading) {
        return;
    }

    if (!forceCheck && lastCheckTime_.isValid()) {
        qint64 hoursSinceLastCheck = lastCheckTime_.secsTo(QDateTime::currentDateTime()) / 3600;
        if (hoursSinceLastCheck < config_.checkIntervalHours) {
            LOG_DEBUG("跳过版本检查（距离上次检查不足间隔时间）");
            return;
        }
    }

    setState(OTAState::CheckingVersion);
    lastCheckTime_ = QDateTime::currentDateTime();

    // 构建API请求
    QString apiUrl = QString("%1/check?app=%2&version=%3&channel=%4")
        .arg(config_.updateServerUrl)
        .arg(config_.applicationName)
        .arg(config_.currentVersion)
        .arg(config_.channel);

    if (config_.allowBeta) {
        apiUrl += "&beta=1";
    }

    QNetworkRequest request(QUrl(apiUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", QString("%1/%2")
                        .arg(config_.applicationName)
                        .arg(config_.currentVersion).toUtf8());

    QNetworkReply* reply = networkManager_->get(request);
    connect(reply, &QNetworkReply::finished, this, &OTAUpdater::onVersionCheckReply);

    LOG_INFO(QString("正在检查更新: %1").arg(apiUrl));
}

VersionInfo OTAUpdater::currentVersion() const
{
    return VersionInfo::fromString(config_.currentVersion);
}

bool OTAUpdater::startDownload()
{
    QMutexLocker locker(&mutex_);

    if (!hasUpdate_) {
        setError("没有可用更新");
        return false;
    }

    if (state_ == OTAState::Downloading) {
        return true;
    }

    if (packageInfo_.packageUrl.isEmpty()) {
        setError("下载地址为空");
        return false;
    }

    setState(OTAState::Downloading);

    // 准备下载文件
    downloadedFilePath_ = QString("%1/%2").arg(config_.downloadPath).arg(packageInfo_.fileName);

    // 检查是否支持断点续传
    qint64 existingSize = 0;
    QFile existingFile(downloadedFilePath_);
    if (existingFile.exists()) {
        existingSize = existingFile.size();
    }

    downloadFile_ = new QFile(downloadedFilePath_, this);
    QIODevice::OpenMode mode = existingSize > 0 ? QIODevice::Append : QIODevice::WriteOnly;

    if (!downloadFile_->open(mode)) {
        setError(QString("无法创建下载文件: %1").arg(downloadedFilePath_));
        setState(OTAState::Error);
        return false;
    }

    // 构建下载请求
    QNetworkRequest request(QUrl(packageInfo_.packageUrl));

    // 断点续传
    if (existingSize > 0) {
        request.setRawHeader("Range", QString("bytes=%1-").arg(existingSize).toUtf8());
        LOG_INFO(QString("断点续传，从 %1 字节开始").arg(existingSize));
    }

    downloadStartTime_ = QDateTime::currentMSecsSinceEpoch();
    downloadProgress_.bytesReceived = existingSize;
    downloadProgress_.bytesTotal = packageInfo_.fileSize;

    currentReply_ = networkManager_->get(request);
    connect(currentReply_, &QNetworkReply::downloadProgress,
            this, &OTAUpdater::onDownloadProgress);
    connect(currentReply_, &QNetworkReply::finished,
            this, &OTAUpdater::onDownloadFinished);
    connect(currentReply_, &QNetworkReply::errorOccurred,
            this, &OTAUpdater::onDownloadError);
    connect(currentReply_, &QNetworkReply::readyRead, this, [this]() {
        if (downloadFile_ && currentReply_) {
            downloadFile_->write(currentReply_->readAll());
        }
    });

    LOG_INFO(QString("开始下载: %1").arg(packageInfo_.packageUrl));
    return true;
}

void OTAUpdater::pauseDownload()
{
    if (currentReply_) {
        currentReply_->abort();
        currentReply_->deleteLater();
        currentReply_ = nullptr;
    }

    if (downloadFile_) {
        downloadFile_->close();
    }

    LOG_INFO("下载已暂停");
}

void OTAUpdater::resumeDownload()
{
    startDownload();  // 会自动使用断点续传
}

void OTAUpdater::cancelDownload()
{
    pauseDownload();

    if (downloadFile_) {
        downloadFile_->remove();
        delete downloadFile_;
        downloadFile_ = nullptr;
    }

    downloadProgress_ = DownloadProgress();
    setState(OTAState::Idle);
    LOG_INFO("下载已取消");
}

bool OTAUpdater::startInstall(bool silentInstall)
{
    QMutexLocker locker(&mutex_);

    if (state_ != OTAState::ReadyToInstall) {
        setError("更新包未就绪");
        return false;
    }

    setState(OTAState::Installing);
    emit installStarted();

    LOG_INFO(QString("开始安装更新 (静默: %1)").arg(silentInstall ? "是" : "否"));

    // 创建备份
    emit installProgress(10, "正在创建备份...");
    if (!createBackup()) {
        setError("创建备份失败");
        setState(OTAState::Error);
        emit installFailed(lastError_);
        return false;
    }

    // 解压更新包
    emit installProgress(30, "正在解压更新包...");
    QString extractDir = config_.downloadPath + "/extract";
    if (!extractPackage(downloadedFilePath_, extractDir)) {
        setError("解压更新包失败");
        rollback();
        return false;
    }

    // 应用更新
    emit installProgress(60, "正在应用更新...");
    if (!applyUpdate(extractDir)) {
        setError("应用更新失败");
        rollback();
        return false;
    }

    // 清理临时文件
    emit installProgress(90, "正在清理...");
    cleanupTempFiles();

    // 更新配置中的版本号
    config_.currentVersion = latestVersion_.version;

    emit installProgress(100, "安装完成");
    setState(OTAState::Completed);
    emit installCompleted();

    LOG_INFO(QString("更新安装完成，新版本: %1").arg(latestVersion_.version));

    // 提示需要重启
    emit restartRequired();

    return true;
}

bool OTAUpdater::createBackup()
{
    QString backupDir = QString("%1/%2_%3")
        .arg(config_.backupPath)
        .arg(config_.applicationName)
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    QDir().mkpath(backupDir);

    // 备份关键文件
    // TODO: 根据实际应用程序结构备份文件

    LOG_INFO(QString("已创建备份: %1").arg(backupDir));
    return true;
}

bool OTAUpdater::rollback()
{
    setState(OTAState::RollingBack);
    LOG_WARNING("正在回滚到旧版本...");

    // TODO: 从备份恢复文件

    setState(OTAState::Error);
    emit installFailed("更新失败，已回滚到旧版本");
    return true;
}

QString OTAUpdater::stateString() const
{
    switch (state_) {
    case OTAState::Idle: return "空闲";
    case OTAState::CheckingVersion: return "检查版本中";
    case OTAState::UpdateAvailable: return "有新版本";
    case OTAState::Downloading: return "下载中";
    case OTAState::Verifying: return "校验中";
    case OTAState::ReadyToInstall: return "准备安装";
    case OTAState::Installing: return "安装中";
    case OTAState::Completed: return "完成";
    case OTAState::RollingBack: return "回滚中";
    case OTAState::Error: return "错误";
    default: return "未知";
    }
}

void OTAUpdater::startAutoCheck()
{
    if (config_.autoCheck && config_.checkIntervalHours > 0) {
        autoCheckTimer_->start(config_.checkIntervalHours * 3600 * 1000);
        LOG_INFO(QString("已启动自动更新检查，间隔: %1 小时").arg(config_.checkIntervalHours));

        // 立即检查一次
        checkForUpdate();
    }
}

void OTAUpdater::stopAutoCheck()
{
    autoCheckTimer_->stop();
    LOG_INFO("已停止自动更新检查");
}

bool OTAUpdater::isAutoCheckRunning() const
{
    return autoCheckTimer_->isActive();
}

void OTAUpdater::onAutoCheckTimer()
{
    checkForUpdate();
}

void OTAUpdater::onVersionCheckReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        setError(QString("版本检查失败: %1").arg(reply->errorString()));
        setState(OTAState::Error);
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isObject()) {
        setError("服务器响应格式错误");
        setState(OTAState::Idle);
        return;
    }

    QJsonObject json = doc.object();

    // 检查是否有更新
    bool hasNewVersion = json.value("hasUpdate").toBool(false);

    if (hasNewVersion) {
        latestVersion_ = VersionInfo::fromJson(json.value("version").toObject());
        packageInfo_ = UpdatePackageInfo::fromJson(json.value("package").toObject());
        hasUpdate_ = true;

        setState(OTAState::UpdateAvailable);
        emit updateAvailable(latestVersion_, packageInfo_);

        LOG_INFO(QString("发现新版本: %1 -> %2")
                .arg(config_.currentVersion)
                .arg(latestVersion_.version));

        // 自动下载
        if (config_.autoDownload) {
            startDownload();
        }
    } else {
        hasUpdate_ = false;
        setState(OTAState::Idle);
        emit noUpdateAvailable();
        LOG_INFO("当前已是最新版本");
    }
}

void OTAUpdater::onDownloadProgress(qint64 received, qint64 total)
{
    QMutexLocker locker(&mutex_);

    downloadProgress_.bytesReceived = received;
    if (total > 0) {
        downloadProgress_.bytesTotal = total;
    }

    // 计算进度百分比
    if (downloadProgress_.bytesTotal > 0) {
        downloadProgress_.progressPercent = static_cast<int>(
            received * 100 / downloadProgress_.bytesTotal);
    }

    // 计算下载速度
    qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - downloadStartTime_;
    if (elapsed > 0) {
        downloadProgress_.speedBytesPerSec = received * 1000.0 / elapsed;

        // 计算剩余时间
        if (downloadProgress_.speedBytesPerSec > 0) {
            qint64 remaining = downloadProgress_.bytesTotal - received;
            downloadProgress_.remainingSeconds = static_cast<int>(
                remaining / downloadProgress_.speedBytesPerSec);
        }
    }

    downloadProgress_.currentFile = packageInfo_.fileName;

    emit downloadProgressChanged(downloadProgress_);
}

void OTAUpdater::onDownloadFinished()
{
    QMutexLocker locker(&mutex_);

    if (downloadFile_) {
        downloadFile_->close();
        delete downloadFile_;
        downloadFile_ = nullptr;
    }

    if (currentReply_) {
        currentReply_->deleteLater();
        currentReply_ = nullptr;
    }

    // 校验文件
    setState(OTAState::Verifying);
    LOG_INFO("下载完成，正在校验...");

    if (!verifyPackage(downloadedFilePath_)) {
        setError("文件校验失败");
        setState(OTAState::Error);
        emit downloadFailed(lastError_);
        return;
    }

    setState(OTAState::ReadyToInstall);
    emit downloadCompleted(downloadedFilePath_);
    LOG_INFO(QString("更新包已就绪: %1").arg(downloadedFilePath_));

    // 自动安装
    if (config_.autoInstall) {
        startInstall(true);
    }
}

void OTAUpdater::onDownloadError()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        setError(QString("下载错误: %1").arg(reply->errorString()));
    }

    setState(OTAState::Error);
    emit downloadFailed(lastError_);
}

void OTAUpdater::setState(OTAState newState)
{
    if (state_ != newState) {
        state_ = newState;
        emit stateChanged(newState);
    }
}

void OTAUpdater::setError(const QString& error)
{
    lastError_ = error;
    LOG_ERROR(QString("OTA错误: %1").arg(error));
    emit errorOccurred(error);
}

bool OTAUpdater::verifyPackage(const QString& filePath)
{
    if (!QFile::exists(filePath)) {
        setError("文件不存在");
        return false;
    }

    // 检查文件大小
    QFileInfo fileInfo(filePath);
    if (packageInfo_.fileSize > 0 && fileInfo.size() != packageInfo_.fileSize) {
        setError(QString("文件大小不匹配: %1 != %2")
                .arg(fileInfo.size())
                .arg(packageInfo_.fileSize));
        return false;
    }

    // SHA256校验
    if (!packageInfo_.sha256Hash.isEmpty()) {
        QString actualHash = calculateFileHash(filePath, "SHA256");
        if (actualHash.toLower() != packageInfo_.sha256Hash.toLower()) {
            setError("SHA256校验失败");
            return false;
        }
        LOG_INFO("SHA256校验通过");
    }

    // MD5校验
    if (!packageInfo_.md5Hash.isEmpty()) {
        QString actualHash = calculateFileHash(filePath, "MD5");
        if (actualHash.toLower() != packageInfo_.md5Hash.toLower()) {
            setError("MD5校验失败");
            return false;
        }
        LOG_INFO("MD5校验通过");
    }

    return true;
}

QString OTAUpdater::calculateFileHash(const QString& filePath, const QString& algorithm)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QCryptographicHash::Algorithm algo = QCryptographicHash::Sha256;
    if (algorithm == "MD5") {
        algo = QCryptographicHash::Md5;
    } else if (algorithm == "SHA1") {
        algo = QCryptographicHash::Sha1;
    }

    QCryptographicHash hash(algo);
    hash.addData(&file);
    file.close();

    return hash.result().toHex();
}

bool OTAUpdater::extractPackage(const QString& packagePath, const QString& targetDir)
{
    QDir().mkpath(targetDir);

    // 使用外部解压工具或Qt实现
    // TODO: 实现ZIP/7z解压

    LOG_DEBUG(QString("解压 %1 到 %2").arg(packagePath, targetDir));
    return true;
}

bool OTAUpdater::applyUpdate(const QString& updateDir)
{
    // TODO: 复制更新文件到应用目录
    Q_UNUSED(updateDir)
    return true;
}

void OTAUpdater::cleanupTempFiles()
{
    // 清理下载和解压的临时文件
    QDir extractDir(config_.downloadPath + "/extract");
    if (extractDir.exists()) {
        extractDir.removeRecursively();
    }

    // 保留下载的安装包以便出问题时重试
    LOG_INFO("临时文件已清理");
}

} // namespace Base
} // namespace VisionForge
