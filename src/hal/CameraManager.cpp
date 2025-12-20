/**
 * @file CameraManager.cpp
 * @brief 多相机管理器实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "hal/CameraManager.h"
#include "base/Logger.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QUuid>
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

namespace VisionForge {
namespace HAL {

// ============================================================
// 单例实现
// ============================================================

CameraManager& CameraManager::instance()
{
    static CameraManager instance;
    return instance;
}

CameraManager::CameraManager()
    : QObject(nullptr)
    , cameraIdCounter_(0)
    , groupIdCounter_(0)
{
    // 创建Action Command UDP套接字
    actionCommandSocket_ = std::make_unique<QUdpSocket>(this);

    LOG_INFO("CameraManager初始化");
}

CameraManager::~CameraManager()
{
    // 停止所有连续采集
    for (auto it = continuousTimers_.begin(); it != continuousTimers_.end(); ++it) {
        if (it.value()) {
            it.value()->stop();
            delete it.value();
        }
    }
    continuousTimers_.clear();

    // 断开所有相机
    disconnectAll();

    // 清空相机实例
    cameras_.clear();

    LOG_INFO("CameraManager已销毁");
}

// ============================================================
// 相机管理
// ============================================================

bool CameraManager::addCamera(const CameraInfo& info)
{
    QMutexLocker locker(&mutex_);

    QString cameraId = info.id.isEmpty() ? generateCameraId() : info.id;

    if (cameras_.contains(cameraId)) {
        LOG_WARNING(QString("相机ID已存在: %1").arg(cameraId));
        return false;
    }

    // 创建相机实例
    ICamera* camera = createCameraInstance(info);
    if (!camera) {
        LOG_ERROR(QString("创建相机实例失败: %1").arg(cameraId));
        return false;
    }

    // 存储相机信息
    CameraInfo storedInfo = info;
    storedInfo.id = cameraId;

    cameras_[cameraId] = std::shared_ptr<ICamera>(camera);
    cameraInfos_[cameraId] = storedInfo;

    LOG_INFO(QString("添加相机: %1 (%2)").arg(storedInfo.name).arg(cameraId));

    emit cameraAdded(cameraId);
    return true;
}

bool CameraManager::removeCamera(const QString& cameraId)
{
    QMutexLocker locker(&mutex_);

    if (!cameras_.contains(cameraId)) {
        LOG_WARNING(QString("相机不存在: %1").arg(cameraId));
        return false;
    }

    // 断开相机连接
    if (cameras_[cameraId]->isOpen()) {
        cameras_[cameraId]->close();
    }

    // 从所有组中移除
    for (auto& group : groups_) {
        group.cameraIds.removeAll(cameraId);
    }

    // 移除相机
    cameras_.remove(cameraId);
    cameraInfos_.remove(cameraId);

    LOG_INFO(QString("移除相机: %1").arg(cameraId));

    emit cameraRemoved(cameraId);
    return true;
}

ICamera* CameraManager::getCamera(const QString& cameraId)
{
    QMutexLocker locker(&mutex_);

    if (cameras_.contains(cameraId)) {
        return cameras_[cameraId].get();
    }
    return nullptr;
}

CameraInfo CameraManager::getCameraInfo(const QString& cameraId) const
{
    QMutexLocker locker(&mutex_);

    if (cameraInfos_.contains(cameraId)) {
        return cameraInfos_[cameraId];
    }
    return CameraInfo();
}

bool CameraManager::updateCameraInfo(const QString& cameraId, const CameraInfo& info)
{
    QMutexLocker locker(&mutex_);

    if (!cameraInfos_.contains(cameraId)) {
        return false;
    }

    CameraInfo updated = info;
    updated.id = cameraId; // 保持ID不变

    cameraInfos_[cameraId] = updated;

    // 如果相机已连接，更新配置
    if (cameras_.contains(cameraId) && cameras_[cameraId]->isOpen()) {
        cameras_[cameraId]->setConfig(updated.config);
    }

    emit cameraInfoChanged(cameraId);
    return true;
}

QStringList CameraManager::getAllCameraIds() const
{
    QMutexLocker locker(&mutex_);
    return cameras_.keys();
}

int CameraManager::cameraCount() const
{
    QMutexLocker locker(&mutex_);
    return cameras_.size();
}

bool CameraManager::hasCamera(const QString& cameraId) const
{
    QMutexLocker locker(&mutex_);
    return cameras_.contains(cameraId);
}

// ============================================================
// 相机分组
// ============================================================

bool CameraManager::createGroup(const CameraGroup& group)
{
    QMutexLocker locker(&mutex_);

    QString groupId = group.id.isEmpty() ? generateGroupId() : group.id;

    if (groups_.contains(groupId)) {
        LOG_WARNING(QString("相机组ID已存在: %1").arg(groupId));
        return false;
    }

    CameraGroup storedGroup = group;
    storedGroup.id = groupId;

    groups_[groupId] = storedGroup;

    LOG_INFO(QString("创建相机组: %1 (%2)").arg(storedGroup.name).arg(groupId));

    emit groupCreated(groupId);
    return true;
}

bool CameraManager::deleteGroup(const QString& groupId)
{
    QMutexLocker locker(&mutex_);

    if (!groups_.contains(groupId)) {
        LOG_WARNING(QString("相机组不存在: %1").arg(groupId));
        return false;
    }

    // 停止连续采集
    if (continuousTimers_.contains(groupId)) {
        continuousTimers_[groupId]->stop();
        delete continuousTimers_[groupId];
        continuousTimers_.remove(groupId);
    }

    groups_.remove(groupId);

    LOG_INFO(QString("删除相机组: %1").arg(groupId));

    emit groupDeleted(groupId);
    return true;
}

CameraGroup CameraManager::getGroup(const QString& groupId) const
{
    QMutexLocker locker(&mutex_);

    if (groups_.contains(groupId)) {
        return groups_[groupId];
    }
    return CameraGroup();
}

bool CameraManager::updateGroup(const QString& groupId, const CameraGroup& group)
{
    QMutexLocker locker(&mutex_);

    if (!groups_.contains(groupId)) {
        return false;
    }

    CameraGroup updated = group;
    updated.id = groupId; // 保持ID不变

    groups_[groupId] = updated;

    emit groupChanged(groupId);
    return true;
}

QList<CameraGroup> CameraManager::getAllGroups() const
{
    QMutexLocker locker(&mutex_);
    return groups_.values();
}

QStringList CameraManager::getAllGroupIds() const
{
    QMutexLocker locker(&mutex_);
    return groups_.keys();
}

bool CameraManager::addCameraToGroup(const QString& cameraId, const QString& groupId)
{
    QMutexLocker locker(&mutex_);

    if (!cameras_.contains(cameraId)) {
        LOG_WARNING(QString("相机不存在: %1").arg(cameraId));
        return false;
    }

    if (!groups_.contains(groupId)) {
        LOG_WARNING(QString("相机组不存在: %1").arg(groupId));
        return false;
    }

    // 检查是否已在组中
    if (groups_[groupId].cameraIds.contains(cameraId)) {
        return true; // 已经在组中
    }

    groups_[groupId].cameraIds.append(cameraId);

    // 更新相机信息中的组ID
    if (cameraInfos_.contains(cameraId)) {
        cameraInfos_[cameraId].groupId = groupId;
    }

    emit groupChanged(groupId);
    return true;
}

bool CameraManager::removeCameraFromGroup(const QString& cameraId, const QString& groupId)
{
    QMutexLocker locker(&mutex_);

    if (!groups_.contains(groupId)) {
        return false;
    }

    groups_[groupId].cameraIds.removeAll(cameraId);

    // 清除相机信息中的组ID
    if (cameraInfos_.contains(cameraId) && cameraInfos_[cameraId].groupId == groupId) {
        cameraInfos_[cameraId].groupId.clear();
    }

    emit groupChanged(groupId);
    return true;
}

bool CameraManager::hasGroup(const QString& groupId) const
{
    QMutexLocker locker(&mutex_);
    return groups_.contains(groupId);
}

// ============================================================
// 连接管理
// ============================================================

bool CameraManager::connectCamera(const QString& cameraId)
{
    QMutexLocker locker(&mutex_);

    if (!cameras_.contains(cameraId)) {
        LOG_WARNING(QString("相机不存在: %1").arg(cameraId));
        return false;
    }

    ICamera* camera = cameras_[cameraId].get();

    if (camera->isOpen()) {
        return true; // 已经连接
    }

    if (!camera->open()) {
        LOG_ERROR(QString("连接相机失败: %1").arg(cameraId));
        emit cameraError(cameraId, "连接失败");
        return false;
    }

    // 应用配置
    if (cameraInfos_.contains(cameraId)) {
        camera->setConfig(cameraInfos_[cameraId].config);
    }

    LOG_INFO(QString("连接相机成功: %1").arg(cameraId));
    emit cameraConnected(cameraId);
    return true;
}

void CameraManager::disconnectCamera(const QString& cameraId)
{
    QMutexLocker locker(&mutex_);

    if (!cameras_.contains(cameraId)) {
        return;
    }

    ICamera* camera = cameras_[cameraId].get();

    if (!camera->isOpen()) {
        return; // 未连接
    }

    camera->stopGrabbing();
    camera->close();

    LOG_INFO(QString("断开相机: %1").arg(cameraId));
    emit cameraDisconnected(cameraId);
}

bool CameraManager::connectGroup(const QString& groupId)
{
    QMutexLocker locker(&mutex_);

    if (!groups_.contains(groupId)) {
        return false;
    }

    const CameraGroup& group = groups_[groupId];
    bool allSuccess = true;

    for (const QString& cameraId : group.cameraIds) {
        // 临时解锁以避免死锁
        locker.unlock();
        if (!connectCamera(cameraId)) {
            allSuccess = false;
        }
        locker.relock();
    }

    return allSuccess;
}

void CameraManager::disconnectGroup(const QString& groupId)
{
    QMutexLocker locker(&mutex_);

    if (!groups_.contains(groupId)) {
        return;
    }

    const CameraGroup& group = groups_[groupId];

    for (const QString& cameraId : group.cameraIds) {
        locker.unlock();
        disconnectCamera(cameraId);
        locker.relock();
    }
}

void CameraManager::connectAll()
{
    QStringList ids = getAllCameraIds();
    for (const QString& id : ids) {
        connectCamera(id);
    }
}

void CameraManager::disconnectAll()
{
    QStringList ids = getAllCameraIds();
    for (const QString& id : ids) {
        disconnectCamera(id);
    }
}

bool CameraManager::isCameraConnected(const QString& cameraId) const
{
    QMutexLocker locker(&mutex_);

    if (!cameras_.contains(cameraId)) {
        return false;
    }

    return cameras_[cameraId]->isOpen();
}

int CameraManager::connectedCameraCount() const
{
    QMutexLocker locker(&mutex_);

    int count = 0;
    for (auto it = cameras_.begin(); it != cameras_.end(); ++it) {
        if (it.value()->isOpen()) {
            ++count;
        }
    }
    return count;
}

// ============================================================
// 采集控制
// ============================================================

CaptureResult CameraManager::capture(const QString& cameraId, int timeoutMs)
{
    CaptureResult result;
    result.cameraId = cameraId;
    result.timestamp = QDateTime::currentMSecsSinceEpoch();

    QMutexLocker locker(&mutex_);

    if (!cameras_.contains(cameraId)) {
        result.success = false;
        result.errorMessage = "相机不存在";
        return result;
    }

    ICamera* camera = cameras_[cameraId].get();

    if (!camera->isOpen()) {
        result.success = false;
        result.errorMessage = "相机未连接";
        return result;
    }

    // 解锁以避免阻塞其他操作
    locker.unlock();

    emit captureStarted(cameraId);

    QElapsedTimer timer;
    timer.start();

    // 如果是软件触发模式，先触发
    if (camera->getTriggerMode() == ICamera::Software) {
        if (!camera->startGrabbing()) {
            result.success = false;
            result.errorMessage = "启动采集失败";
            emit captureFailed(cameraId, result.errorMessage);
            return result;
        }
        camera->trigger();
    }

    // 采集图像
    Base::ImageData::Ptr image = camera->grabImage(timeoutMs);

    result.captureTime = timer.elapsed();

    if (image && !image->isEmpty()) {
        result.success = true;
        result.image = image;
        emit captureCompleted(cameraId, image);
    } else {
        result.success = false;
        result.errorMessage = "采集超时或失败";
        emit captureFailed(cameraId, result.errorMessage);
    }

    // 如果是软件触发模式，停止采集
    if (camera->getTriggerMode() == ICamera::Software) {
        camera->stopGrabbing();
    }

    return result;
}

BatchCaptureResult CameraManager::captureGroup(const QString& groupId, int timeoutMs)
{
    BatchCaptureResult result;

    CameraGroup group = getGroup(groupId);
    if (group.id.isEmpty()) {
        result.allSuccess = false;
        return result;
    }

    switch (group.captureMode) {
    case CaptureMode::Simultaneous:
        result = captureSimultaneous(group.cameraIds, timeoutMs);
        break;
    case CaptureMode::Sequential:
        result = captureSequential(group.cameraIds, group.captureDelayMs, timeoutMs);
        break;
    case CaptureMode::Independent:
        // 独立模式：按顺序采集，但不等待
        result = captureSequential(group.cameraIds, 0, timeoutMs);
        break;
    }

    emit batchCaptureCompleted(result);
    return result;
}

BatchCaptureResult CameraManager::captureSimultaneous(const QStringList& cameraIds, int timeoutMs)
{
    BatchCaptureResult result;

    QElapsedTimer timer;
    timer.start();

    // 使用QtConcurrent并行采集
    QList<QFuture<CaptureResult>> futures;

    for (const QString& cameraId : cameraIds) {
        QFuture<CaptureResult> future = QtConcurrent::run([this, cameraId, timeoutMs]() {
            return capture(cameraId, timeoutMs);
        });
        futures.append(future);
    }

    // 等待所有采集完成
    for (int i = 0; i < futures.size(); ++i) {
        futures[i].waitForFinished();
        CaptureResult captureResult = futures[i].result();
        result.results[captureResult.cameraId] = captureResult;

        if (captureResult.success) {
            result.successCount++;
        } else {
            result.failCount++;
        }
    }

    result.totalTime = timer.elapsed();
    result.allSuccess = (result.failCount == 0) && (result.successCount > 0);

    return result;
}

BatchCaptureResult CameraManager::captureSequential(const QStringList& cameraIds,
                                                     int delayMs,
                                                     int timeoutMs)
{
    BatchCaptureResult result;

    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < cameraIds.size(); ++i) {
        const QString& cameraId = cameraIds[i];

        // 延迟（除了第一个相机）
        if (i > 0 && delayMs > 0) {
            QThread::msleep(delayMs);
        }

        CaptureResult captureResult = capture(cameraId, timeoutMs);
        result.results[cameraId] = captureResult;

        if (captureResult.success) {
            result.successCount++;
        } else {
            result.failCount++;
        }
    }

    result.totalTime = timer.elapsed();
    result.allSuccess = (result.failCount == 0) && (result.successCount > 0);

    return result;
}

BatchCaptureResult CameraManager::captureAll(CaptureMode mode, int timeoutMs)
{
    QStringList allIds = getAllCameraIds();

    switch (mode) {
    case CaptureMode::Simultaneous:
        return captureSimultaneous(allIds, timeoutMs);
    case CaptureMode::Sequential:
        return captureSequential(allIds, 0, timeoutMs);
    case CaptureMode::Independent:
    default:
        return captureSequential(allIds, 0, timeoutMs);
    }
}

// ============================================================
// 连续采集
// ============================================================

bool CameraManager::startContinuousCapture(const QString& groupId, double fps)
{
    QMutexLocker locker(&mutex_);

    if (!groups_.contains(groupId)) {
        LOG_WARNING(QString("相机组不存在: %1").arg(groupId));
        return false;
    }

    // 如果已经在连续采集，先停止
    if (continuousTimers_.contains(groupId)) {
        continuousTimers_[groupId]->stop();
        delete continuousTimers_[groupId];
        continuousTimers_.remove(groupId);
    }

    // 计算定时器间隔
    int intervalMs = static_cast<int>(1000.0 / fps);
    if (intervalMs < 1) intervalMs = 1;

    // 创建定时器
    QTimer* timer = new QTimer(this);
    timer->setProperty("groupId", groupId);
    connect(timer, &QTimer::timeout, this, &CameraManager::onContinuousCaptureTimer);

    continuousTimers_[groupId] = timer;
    timer->start(intervalMs);

    LOG_INFO(QString("开始连续采集: 组=%1, FPS=%2").arg(groupId).arg(fps));
    emit continuousCaptureStarted(groupId);

    return true;
}

void CameraManager::stopContinuousCapture(const QString& groupId)
{
    QMutexLocker locker(&mutex_);

    if (!continuousTimers_.contains(groupId)) {
        return;
    }

    continuousTimers_[groupId]->stop();
    delete continuousTimers_[groupId];
    continuousTimers_.remove(groupId);

    LOG_INFO(QString("停止连续采集: 组=%1").arg(groupId));
    emit continuousCaptureStopped(groupId);
}

bool CameraManager::isContinuousCapturing(const QString& groupId) const
{
    QMutexLocker locker(&mutex_);
    return continuousTimers_.contains(groupId) && continuousTimers_[groupId]->isActive();
}

void CameraManager::onContinuousCaptureTimer()
{
    QTimer* timer = qobject_cast<QTimer*>(sender());
    if (!timer) return;

    QString groupId = timer->property("groupId").toString();
    if (groupId.isEmpty()) return;

    // 执行组采集
    captureGroup(groupId);
}

// ============================================================
// 配置管理
// ============================================================

bool CameraManager::saveConfig(const QString& filePath)
{
    QMutexLocker locker(&mutex_);

    QJsonObject root;

    // 保存相机信息
    QJsonArray camerasArray;
    for (auto it = cameraInfos_.begin(); it != cameraInfos_.end(); ++it) {
        const CameraInfo& info = it.value();
        QJsonObject cameraObj;
        cameraObj["id"] = info.id;
        cameraObj["name"] = info.name;
        cameraObj["groupId"] = info.groupId;
        cameraObj["type"] = static_cast<int>(info.type);
        cameraObj["enabled"] = info.enabled;

        // 设备信息
        QJsonObject deviceObj;
        deviceObj["manufacturer"] = info.deviceInfo.manufacturer;
        deviceObj["modelName"] = info.deviceInfo.modelName;
        deviceObj["serialNumber"] = info.deviceInfo.serialNumber;
        deviceObj["ipAddress"] = info.deviceInfo.ipAddress;
        deviceObj["interfaceType"] = info.deviceInfo.interfaceType;
        cameraObj["deviceInfo"] = deviceObj;

        // 相机配置
        QJsonObject configObj;
        configObj["width"] = info.config.width;
        configObj["height"] = info.config.height;
        configObj["offsetX"] = info.config.offsetX;
        configObj["offsetY"] = info.config.offsetY;
        configObj["exposure"] = info.config.exposure;
        configObj["gain"] = info.config.gain;
        configObj["triggerMode"] = static_cast<int>(info.config.triggerMode);
        configObj["rotationAngle"] = info.config.rotationAngle;
        configObj["flipHorizontal"] = info.config.flipHorizontal;
        configObj["flipVertical"] = info.config.flipVertical;
        cameraObj["config"] = configObj;

        camerasArray.append(cameraObj);
    }
    root["cameras"] = camerasArray;

    // 保存分组信息
    QJsonArray groupsArray;
    for (auto it = groups_.begin(); it != groups_.end(); ++it) {
        const CameraGroup& group = it.value();
        QJsonObject groupObj;
        groupObj["id"] = group.id;
        groupObj["name"] = group.name;
        groupObj["captureMode"] = static_cast<int>(group.captureMode);
        groupObj["captureDelayMs"] = group.captureDelayMs;

        QJsonArray cameraIdsArray;
        for (const QString& cameraId : group.cameraIds) {
            cameraIdsArray.append(cameraId);
        }
        groupObj["cameraIds"] = cameraIdsArray;

        groupsArray.append(groupObj);
    }
    root["groups"] = groupsArray;

    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法打开配置文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("保存相机配置: %1").arg(filePath));
    return true;
}

bool CameraManager::loadConfig(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开配置文件: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        LOG_ERROR(QString("解析配置文件失败: %1").arg(error.errorString()));
        return false;
    }

    QMutexLocker locker(&mutex_);

    // 清空现有配置
    clear();

    QJsonObject root = doc.object();

    // 加载相机信息
    QJsonArray camerasArray = root["cameras"].toArray();
    for (const QJsonValue& value : camerasArray) {
        QJsonObject cameraObj = value.toObject();

        CameraInfo info;
        info.id = cameraObj["id"].toString();
        info.name = cameraObj["name"].toString();
        info.groupId = cameraObj["groupId"].toString();
        info.type = static_cast<CameraFactory::CameraType>(cameraObj["type"].toInt());
        info.enabled = cameraObj["enabled"].toBool(true);

        // 设备信息
        QJsonObject deviceObj = cameraObj["deviceInfo"].toObject();
        info.deviceInfo.manufacturer = deviceObj["manufacturer"].toString();
        info.deviceInfo.modelName = deviceObj["modelName"].toString();
        info.deviceInfo.serialNumber = deviceObj["serialNumber"].toString();
        info.deviceInfo.ipAddress = deviceObj["ipAddress"].toString();
        info.deviceInfo.interfaceType = deviceObj["interfaceType"].toString();

        // 相机配置
        QJsonObject configObj = cameraObj["config"].toObject();
        info.config.width = configObj["width"].toInt(1920);
        info.config.height = configObj["height"].toInt(1080);
        info.config.offsetX = configObj["offsetX"].toInt(0);
        info.config.offsetY = configObj["offsetY"].toInt(0);
        info.config.exposure = configObj["exposure"].toDouble(10000.0);
        info.config.gain = configObj["gain"].toDouble(0.0);
        info.config.triggerMode = static_cast<ICamera::TriggerMode>(configObj["triggerMode"].toInt());
        info.config.rotationAngle = configObj["rotationAngle"].toInt(0);
        info.config.flipHorizontal = configObj["flipHorizontal"].toBool(false);
        info.config.flipVertical = configObj["flipVertical"].toBool(false);

        // 临时解锁添加相机
        locker.unlock();
        addCamera(info);
        locker.relock();
    }

    // 加载分组信息
    QJsonArray groupsArray = root["groups"].toArray();
    for (const QJsonValue& value : groupsArray) {
        QJsonObject groupObj = value.toObject();

        CameraGroup group;
        group.id = groupObj["id"].toString();
        group.name = groupObj["name"].toString();
        group.captureMode = static_cast<CaptureMode>(groupObj["captureMode"].toInt());
        group.captureDelayMs = groupObj["captureDelayMs"].toInt(0);

        QJsonArray cameraIdsArray = groupObj["cameraIds"].toArray();
        for (const QJsonValue& idValue : cameraIdsArray) {
            group.cameraIds.append(idValue.toString());
        }

        locker.unlock();
        createGroup(group);
        locker.relock();
    }

    LOG_INFO(QString("加载相机配置: %1, 相机数=%2, 分组数=%3")
             .arg(filePath).arg(cameras_.size()).arg(groups_.size()));

    return true;
}

void CameraManager::clear()
{
    // 停止所有连续采集
    QStringList groupIds = continuousTimers_.keys();
    for (const QString& groupId : groupIds) {
        stopContinuousCapture(groupId);
    }

    // 断开所有相机
    disconnectAll();

    QMutexLocker locker(&mutex_);

    cameras_.clear();
    cameraInfos_.clear();
    groups_.clear();
    groupSyncConfigs_.clear();

    LOG_INFO("清空所有相机和分组");
}

// ============================================================
// 硬件同步采集
// ============================================================

BatchCaptureResult CameraManager::captureWithHardwareSync(
    const QStringList& cameraIds,
    const HardwareSyncCaptureConfig& syncConfig)
{
    BatchCaptureResult result;
    QElapsedTimer timer;
    timer.start();

    if (cameraIds.isEmpty()) {
        LOG_WARNING("硬件同步采集: 相机列表为空");
        return result;
    }

    LOG_INFO(QString("开始硬件同步采集, 相机数: %1, 触发模式: %2")
             .arg(cameraIds.size())
             .arg(static_cast<int>(syncConfig.triggerMode)));

    // 1. 配置所有相机为硬件触发模式
    for (const QString& cameraId : cameraIds) {
        ICamera* camera = getCamera(cameraId);
        if (camera && camera->isOpen()) {
            // 配置扩展触发模式
            camera->setTriggerModeEx(syncConfig.triggerMode);

            // 如果是Action Command模式，配置Action Command
            if (syncConfig.triggerMode == TriggerModeEx::ActionCommand) {
                camera->configureActionCommand(syncConfig.actionCmd);
            }

            // 开始采集
            camera->startGrabbing();
        }
    }

    // 2. 预触发延迟
    if (syncConfig.preTriggerDelayUs > 0) {
        GPIOController::delayMicroseconds(syncConfig.preTriggerDelayUs);
    }

    // 3. 发送触发信号
    bool triggerSuccess = false;
    qint64 triggerTimestamp = QDateTime::currentMSecsSinceEpoch();

    switch (syncConfig.triggerMode) {
    case TriggerModeEx::ActionCommand:
        triggerSuccess = sendActionCommand(syncConfig.actionCmd);
        break;

    case TriggerModeEx::ExternalGPIO:
        if (gpioController_ && gpioController_->isOpen()) {
            triggerSuccess = gpioController_->outputPulse(
                syncConfig.gpioConfig.pinNumber,
                syncConfig.gpioConfig.pulseWidthUs,
                syncConfig.gpioConfig.activeHigh);
        }
        break;

    case TriggerModeEx::Hardware:
        // 硬件触发由外部信号控制，这里只等待
        triggerSuccess = true;
        break;

    case TriggerModeEx::Software:
        // 软件触发模式下，直接触发所有相机
        for (const QString& cameraId : cameraIds) {
            ICamera* camera = getCamera(cameraId);
            if (camera && camera->isOpen()) {
                camera->trigger();
            }
        }
        triggerSuccess = true;
        break;

    default:
        break;
    }

    if (!triggerSuccess) {
        LOG_ERROR("硬件同步采集: 发送触发信号失败");
        result.allSuccess = false;
        return result;
    }

    // 4. 后触发延迟
    if (syncConfig.postTriggerDelayUs > 0) {
        GPIOController::delayMicroseconds(syncConfig.postTriggerDelayUs);
    }

    // 5. 并行采集图像
    QList<QFuture<CaptureResult>> futures;

    for (const QString& cameraId : cameraIds) {
        QFuture<CaptureResult> future = QtConcurrent::run([this, cameraId, syncConfig, triggerTimestamp]() {
            CaptureResult captureResult;
            captureResult.cameraId = cameraId;
            captureResult.timestamp = triggerTimestamp;

            ICamera* camera = getCamera(cameraId);
            if (!camera || !camera->isOpen()) {
                captureResult.success = false;
                captureResult.errorMessage = "相机未连接";
                return captureResult;
            }

            QElapsedTimer captureTimer;
            captureTimer.start();

            Base::ImageData::Ptr image = camera->grabImage(syncConfig.timeoutMs);

            captureResult.captureTime = captureTimer.elapsed();

            if (image && !image->isEmpty()) {
                captureResult.success = true;
                captureResult.image = image;
            } else {
                captureResult.success = false;
                captureResult.errorMessage = "采集超时或失败";
            }

            return captureResult;
        });
        futures.append(future);
    }

    // 6. 等待所有采集完成
    for (int i = 0; i < futures.size(); ++i) {
        futures[i].waitForFinished();
        CaptureResult captureResult = futures[i].result();
        result.results[captureResult.cameraId] = captureResult;

        if (captureResult.success) {
            result.successCount++;
        } else {
            result.failCount++;
        }
    }

    // 7. 停止采集
    for (const QString& cameraId : cameraIds) {
        ICamera* camera = getCamera(cameraId);
        if (camera && camera->isGrabbing()) {
            camera->stopGrabbing();
        }
    }

    result.totalTime = timer.elapsed();
    result.allSuccess = (result.failCount == 0) && (result.successCount > 0);

    // 8. 验证同步精度
    if (syncConfig.validateSyncAccuracy && result.allSuccess) {
        result.syncAccuracyUs = validateSyncAccuracy(result);

        if (result.syncAccuracyUs > syncConfig.maxSyncErrorUs) {
            LOG_WARNING(QString("硬件同步精度超出允许范围: %.2f us > %d us")
                       .arg(result.syncAccuracyUs)
                       .arg(syncConfig.maxSyncErrorUs));
        }
    }

    LOG_INFO(QString("硬件同步采集完成, 成功: %1/%2, 耗时: %.1f ms, 同步精度: %.2f us")
             .arg(result.successCount)
             .arg(cameraIds.size())
             .arg(result.totalTime)
             .arg(result.syncAccuracyUs));

    emit batchCaptureCompleted(result);
    return result;
}

bool CameraManager::sendActionCommand(const ActionCommandConfig& config)
{
    if (!config.enabled) {
        LOG_WARNING("Action Command未启用");
        return false;
    }

    // 构建Action Command数据包 (GigE Vision规范)
    // Action Command包结构:
    // - Device Key (4 bytes)
    // - Group Key (4 bytes)
    // - Group Mask (4 bytes)
    // - 保留字段

    QByteArray packet;
    packet.resize(16);

    // 大端序
    packet[0] = (config.deviceKey >> 24) & 0xFF;
    packet[1] = (config.deviceKey >> 16) & 0xFF;
    packet[2] = (config.deviceKey >> 8) & 0xFF;
    packet[3] = config.deviceKey & 0xFF;

    packet[4] = (config.groupKey >> 24) & 0xFF;
    packet[5] = (config.groupKey >> 16) & 0xFF;
    packet[6] = (config.groupKey >> 8) & 0xFF;
    packet[7] = config.groupKey & 0xFF;

    packet[8] = (config.groupMask >> 24) & 0xFF;
    packet[9] = (config.groupMask >> 16) & 0xFF;
    packet[10] = (config.groupMask >> 8) & 0xFF;
    packet[11] = config.groupMask & 0xFF;

    // 保留字段
    packet[12] = 0;
    packet[13] = 0;
    packet[14] = 0;
    packet[15] = 0;

    // 发送组播
    QHostAddress multicastAddr(config.multicastAddress);
    qint64 bytesSent = actionCommandSocket_->writeDatagram(
        packet, multicastAddr, config.multicastPort);

    if (bytesSent != packet.size()) {
        LOG_ERROR(QString("发送Action Command失败: %1")
                 .arg(actionCommandSocket_->errorString()));
        return false;
    }

    LOG_DEBUG(QString("发送Action Command: DeviceKey=0x%1, GroupKey=0x%2, GroupMask=0x%3")
             .arg(config.deviceKey, 8, 16, QChar('0'))
             .arg(config.groupKey, 8, 16, QChar('0'))
             .arg(config.groupMask, 8, 16, QChar('0')));

    return true;
}

bool CameraManager::configureGroupSync(const QString& groupId, const HardwareSyncCaptureConfig& syncConfig)
{
    QMutexLocker locker(&mutex_);

    if (!groups_.contains(groupId)) {
        LOG_WARNING(QString("相机组不存在: %1").arg(groupId));
        return false;
    }

    groupSyncConfigs_[groupId] = syncConfig;

    // 配置组内所有相机
    const CameraGroup& group = groups_[groupId];
    for (const QString& cameraId : group.cameraIds) {
        if (cameras_.contains(cameraId)) {
            ICamera* camera = cameras_[cameraId].get();

            if (syncConfig.triggerMode == TriggerModeEx::ActionCommand) {
                camera->configureActionCommand(syncConfig.actionCmd);
            }

            camera->setTriggerModeEx(syncConfig.triggerMode);
        }
    }

    LOG_INFO(QString("配置相机组硬件同步: %1, 模式: %2")
             .arg(groupId)
             .arg(static_cast<int>(syncConfig.triggerMode)));

    return true;
}

double CameraManager::validateSyncAccuracy(const BatchCaptureResult& result)
{
    if (result.results.size() < 2) {
        return 0.0;
    }

    // 计算时间戳差异
    QList<qint64> timestamps;
    for (const auto& captureResult : result.results) {
        if (captureResult.success) {
            timestamps.append(captureResult.timestamp);
        }
    }

    if (timestamps.size() < 2) {
        return 0.0;
    }

    // 计算最大时间差
    qint64 minTs = *std::min_element(timestamps.begin(), timestamps.end());
    qint64 maxTs = *std::max_element(timestamps.begin(), timestamps.end());

    // 转换为微秒
    double syncErrorUs = (maxTs - minTs) * 1000.0;

    return syncErrorUs;
}

void CameraManager::setGPIOController(std::shared_ptr<GPIOController> gpio)
{
    gpioController_ = gpio;
    LOG_INFO("设置GPIO控制器");
}

// ============================================================
// 私有辅助方法
// ============================================================

QString CameraManager::generateCameraId()
{
    return QString("CAM_%1").arg(++cameraIdCounter_, 4, 10, QChar('0'));
}

QString CameraManager::generateGroupId()
{
    return QString("GRP_%1").arg(++groupIdCounter_, 4, 10, QChar('0'));
}

ICamera* CameraManager::createCameraInstance(const CameraInfo& info)
{
    // 使用CameraFactory创建相机实例
    ICamera* camera = CameraFactory::create(info.type, nullptr);

    if (camera) {
        // 连接错误信号
        connect(camera, &ICamera::errorOccurred, this, [this, id = info.id](const QString& error) {
            emit cameraError(id, error);
        });
    }

    return camera;
}

} // namespace HAL
} // namespace VisionForge
