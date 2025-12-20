/**
 * @file WorkStation.cpp
 * @brief 工位管理实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "core/WorkStation.h"
#include "hal/IMotionController.h"
#include "base/Logger.h"
#include "base/ImageData.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QPointF>
#include <QElapsedTimer>
#include <QThread>
#include <cmath>

namespace VisionForge {
namespace Core {

// ========== PositionConfig ==========

QJsonObject PositionConfig::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["name"] = name;
    json["type"] = static_cast<int>(type);
    json["enabled"] = enabled;
    json["cameraId"] = cameraId;
    json["calibrationId"] = calibrationId;
    json["positionIndex"] = positionIndex;
    json["platX"] = platX;
    json["platY"] = platY;
    json["platD"] = platD;
    json["searchToolId"] = searchToolId;

    QJsonArray auxArray;
    for (const auto& auxId : auxiliaryToolIds) {
        auxArray.append(auxId);
    }
    json["auxiliaryToolIds"] = auxArray;

    return json;
}

PositionConfig PositionConfig::fromJson(const QJsonObject& json)
{
    PositionConfig config;
    config.id = json["id"].toString();
    config.name = json["name"].toString();
    config.type = static_cast<PositionType>(json["type"].toInt());
    config.enabled = json["enabled"].toBool(true);
    config.cameraId = json["cameraId"].toString();
    config.calibrationId = json["calibrationId"].toString();
    config.positionIndex = json["positionIndex"].toInt();
    config.platX = json["platX"].toDouble();
    config.platY = json["platY"].toDouble();
    config.platD = json["platD"].toDouble();
    config.searchToolId = json["searchToolId"].toString();

    QJsonArray auxArray = json["auxiliaryToolIds"].toArray();
    for (const auto& val : auxArray) {
        config.auxiliaryToolIds.append(val.toString());
    }

    return config;
}

// ========== AlignmentOffset ==========

QJsonObject AlignmentOffset::toJson() const
{
    QJsonObject json;
    json["offsetX"] = offsetX;
    json["offsetY"] = offsetY;
    json["offsetD"] = offsetD;
    json["description"] = description;
    return json;
}

AlignmentOffset AlignmentOffset::fromJson(const QJsonObject& json)
{
    AlignmentOffset offset;
    offset.offsetX = json["offsetX"].toDouble();
    offset.offsetY = json["offsetY"].toDouble();
    offset.offsetD = json["offsetD"].toDouble();
    offset.description = json["description"].toString();
    return offset;
}

// ========== AlignmentPrecision ==========

QJsonObject AlignmentPrecision::toJson() const
{
    QJsonObject json;
    json["precisionX"] = precisionX;
    json["precisionY"] = precisionY;
    json["precisionD"] = precisionD;
    json["maxIterations"] = maxIterations;
    return json;
}

AlignmentPrecision AlignmentPrecision::fromJson(const QJsonObject& json)
{
    AlignmentPrecision precision;
    precision.precisionX = json["precisionX"].toDouble(0.01);
    precision.precisionY = json["precisionY"].toDouble(0.01);
    precision.precisionD = json["precisionD"].toDouble(0.01);
    precision.maxIterations = json["maxIterations"].toInt(3);
    return precision;
}

// ========== IterativeAlignConfig ==========

QJsonObject IterativeAlignConfig::toJson() const
{
    QJsonObject json;
    json["enabled"] = enabled;
    json["maxIterations"] = maxIterations;
    json["convergenceThresholdX"] = convergenceThresholdX;
    json["convergenceThresholdY"] = convergenceThresholdY;
    json["convergenceThresholdD"] = convergenceThresholdD;
    json["motionControllerId"] = motionControllerId;
    json["plcConnectionName"] = plcConnectionName;
    json["compensationFactorX"] = compensationFactorX;
    json["compensationFactorY"] = compensationFactorY;
    json["compensationFactorD"] = compensationFactorD;
    json["motionSettleTimeMs"] = motionSettleTimeMs;
    json["captureDelayMs"] = captureDelayMs;
    json["useIncrementalCompensation"] = useIncrementalCompensation;
    return json;
}

IterativeAlignConfig IterativeAlignConfig::fromJson(const QJsonObject& json)
{
    IterativeAlignConfig config;
    config.enabled = json["enabled"].toBool(false);
    config.maxIterations = json["maxIterations"].toInt(3);
    config.convergenceThresholdX = json["convergenceThresholdX"].toDouble(0.01);
    config.convergenceThresholdY = json["convergenceThresholdY"].toDouble(0.01);
    config.convergenceThresholdD = json["convergenceThresholdD"].toDouble(0.01);
    config.motionControllerId = json["motionControllerId"].toString();
    config.plcConnectionName = json["plcConnectionName"].toString();
    config.compensationFactorX = json["compensationFactorX"].toDouble(1.0);
    config.compensationFactorY = json["compensationFactorY"].toDouble(1.0);
    config.compensationFactorD = json["compensationFactorD"].toDouble(1.0);
    config.motionSettleTimeMs = json["motionSettleTimeMs"].toInt(100);
    config.captureDelayMs = json["captureDelayMs"].toInt(50);
    config.useIncrementalCompensation = json["useIncrementalCompensation"].toBool(true);
    return config;
}

// ========== AlignmentConfig ==========

QJsonObject AlignmentConfig::toJson() const
{
    QJsonObject json;
    json["method"] = static_cast<int>(method);
    json["precision"] = precision.toJson();
    json["currentOffsetIndex"] = currentOffsetIndex;
    json["enableIterativeAlign"] = enableIterativeAlign;
    json["useHomography"] = useHomography;
    json["iterativeConfig"] = iterativeConfig.toJson();

    QJsonArray offsetsArray;
    for (const auto& offset : offsets) {
        offsetsArray.append(offset.toJson());
    }
    json["offsets"] = offsetsArray;

    QJsonArray targetArray;
    for (const auto& id : targetPositionIds) {
        targetArray.append(id);
    }
    json["targetPositionIds"] = targetArray;

    QJsonArray objectArray;
    for (const auto& id : objectPositionIds) {
        objectArray.append(id);
    }
    json["objectPositionIds"] = objectArray;

    return json;
}

AlignmentConfig AlignmentConfig::fromJson(const QJsonObject& json)
{
    AlignmentConfig config;
    config.method = static_cast<AlignmentMethod>(json["method"].toInt());
    config.precision = AlignmentPrecision::fromJson(json["precision"].toObject());
    config.currentOffsetIndex = json["currentOffsetIndex"].toInt();
    config.enableIterativeAlign = json["enableIterativeAlign"].toBool();
    config.useHomography = json["useHomography"].toBool();
    config.iterativeConfig = IterativeAlignConfig::fromJson(json["iterativeConfig"].toObject());

    QJsonArray offsetsArray = json["offsets"].toArray();
    for (const auto& val : offsetsArray) {
        config.offsets.append(AlignmentOffset::fromJson(val.toObject()));
    }

    QJsonArray targetArray = json["targetPositionIds"].toArray();
    for (const auto& val : targetArray) {
        config.targetPositionIds.append(val.toString());
    }

    QJsonArray objectArray = json["objectPositionIds"].toArray();
    for (const auto& val : objectArray) {
        config.objectPositionIds.append(val.toString());
    }

    return config;
}

// ========== SearchResult ==========

QJsonObject SearchResult::toJson() const
{
    QJsonObject json;
    json["positionId"] = positionId;
    json["valid"] = valid;
    json["imageX"] = imageX;
    json["imageY"] = imageY;
    json["imageAngle"] = imageAngle;
    json["score"] = score;
    json["platX"] = platX;
    json["platY"] = platY;
    json["timestamp"] = timestamp;
    json["processTime"] = processTime;

    QJsonArray auxArray;
    for (const auto& pt : auxiliaryPoints) {
        QJsonObject ptObj;
        ptObj["x"] = pt.x();
        ptObj["y"] = pt.y();
        auxArray.append(ptObj);
    }
    json["auxiliaryPoints"] = auxArray;

    return json;
}

// ========== WorkStationAlignResult ==========

QJsonObject WorkStationAlignResult::toJson() const
{
    QJsonObject json;
    json["success"] = success;
    json["offsetX"] = offsetX;
    json["offsetY"] = offsetY;
    json["offsetD"] = offsetD;
    json["outputX"] = outputX;
    json["outputY"] = outputY;
    json["outputD"] = outputD;
    json["withinTolerance"] = withinTolerance;
    json["alignmentCount"] = alignmentCount;
    json["residualError"] = residualError;
    json["timestamp"] = timestamp;
    json["totalTime"] = totalTime;

    QJsonObject targetObj;
    for (auto it = targetResults.begin(); it != targetResults.end(); ++it) {
        targetObj[it.key()] = it.value().toJson();
    }
    json["targetResults"] = targetObj;

    QJsonObject objectObj;
    for (auto it = objectResults.begin(); it != objectResults.end(); ++it) {
        objectObj[it.key()] = it.value().toJson();
    }
    json["objectResults"] = objectObj;

    return json;
}

// ========== WorkStation ==========

WorkStation::WorkStation(const QString& id, QObject* parent)
    : QObject(parent)
    , id_(id)
    , enabled_(true)
    , positionIdCounter_(0)
{
    name_ = QString("WorkStation_%1").arg(id);
}

WorkStation::~WorkStation()
{
}

QString WorkStation::generatePositionId()
{
    return QString("pos_%1").arg(++positionIdCounter_);
}

QString WorkStation::addPosition(const PositionConfig& config)
{
    PositionConfig newConfig = config;
    if (newConfig.id.isEmpty()) {
        newConfig.id = generatePositionId();
    }

    positions_[newConfig.id] = newConfig;
    emit positionAdded(newConfig.id);

    LOG_INFO(QString("工位 %1 添加位置: %2 (%3)")
        .arg(id_).arg(newConfig.name).arg(newConfig.id));

    return newConfig.id;
}

bool WorkStation::removePosition(const QString& positionId)
{
    if (!positions_.contains(positionId)) {
        return false;
    }

    positions_.remove(positionId);
    searchResults_.remove(positionId);

    // 从对位配置中移除
    alignmentConfig_.targetPositionIds.removeAll(positionId);
    alignmentConfig_.objectPositionIds.removeAll(positionId);

    emit positionRemoved(positionId);
    LOG_INFO(QString("工位 %1 移除位置: %2").arg(id_).arg(positionId));

    return true;
}

bool WorkStation::updatePosition(const QString& positionId, const PositionConfig& config)
{
    if (!positions_.contains(positionId)) {
        return false;
    }

    PositionConfig updatedConfig = config;
    updatedConfig.id = positionId;  // 保持ID不变
    positions_[positionId] = updatedConfig;

    emit positionChanged(positionId);
    return true;
}

PositionConfig WorkStation::getPosition(const QString& positionId) const
{
    return positions_.value(positionId);
}

QVector<PositionConfig> WorkStation::getAllPositions() const
{
    QVector<PositionConfig> result;
    for (const auto& config : positions_) {
        result.append(config);
    }
    return result;
}

QVector<PositionConfig> WorkStation::getTargetPositions() const
{
    QVector<PositionConfig> result;
    for (const auto& config : positions_) {
        if (config.type == PositionType::Target && config.enabled) {
            result.append(config);
        }
    }
    return result;
}

QVector<PositionConfig> WorkStation::getObjectPositions() const
{
    QVector<PositionConfig> result;
    for (const auto& config : positions_) {
        if (config.type == PositionType::Object && config.enabled) {
            result.append(config);
        }
    }
    return result;
}

void WorkStation::setAlignmentConfig(const AlignmentConfig& config)
{
    alignmentConfig_ = config;
    emit alignmentConfigChanged();
}

int WorkStation::addAlignmentOffset(const AlignmentOffset& offset)
{
    alignmentConfig_.offsets.append(offset);
    return alignmentConfig_.offsets.size() - 1;
}

void WorkStation::setCurrentOffsetIndex(int index)
{
    if (index >= 0 && index < alignmentConfig_.offsets.size()) {
        alignmentConfig_.currentOffsetIndex = index;
    }
}

AlignmentOffset WorkStation::getCurrentOffset() const
{
    if (alignmentConfig_.currentOffsetIndex >= 0 &&
        alignmentConfig_.currentOffsetIndex < alignmentConfig_.offsets.size()) {
        return alignmentConfig_.offsets[alignmentConfig_.currentOffsetIndex];
    }
    return AlignmentOffset();
}

void WorkStation::setSearchResult(const QString& positionId, const SearchResult& result)
{
    searchResults_[positionId] = result;
    emit searchResultUpdated(positionId);
}

SearchResult WorkStation::getSearchResult(const QString& positionId) const
{
    return searchResults_.value(positionId);
}

void WorkStation::clearSearchResults()
{
    searchResults_.clear();
}

QJsonObject WorkStation::toJson() const
{
    QJsonObject json;
    json["id"] = id_;
    json["name"] = name_;
    json["enabled"] = enabled_;
    json["positionIdCounter"] = positionIdCounter_;

    // 位置配置
    QJsonArray positionsArray;
    for (const auto& pos : positions_) {
        positionsArray.append(pos.toJson());
    }
    json["positions"] = positionsArray;

    // 对位配置
    json["alignmentConfig"] = alignmentConfig_.toJson();

    return json;
}

std::shared_ptr<WorkStation> WorkStation::fromJson(const QJsonObject& json, QObject* parent)
{
    QString id = json["id"].toString();
    auto ws = std::make_shared<WorkStation>(id, parent);

    ws->name_ = json["name"].toString();
    ws->enabled_ = json["enabled"].toBool(true);
    ws->positionIdCounter_ = json["positionIdCounter"].toInt();

    // 加载位置配置
    QJsonArray positionsArray = json["positions"].toArray();
    for (const auto& val : positionsArray) {
        PositionConfig config = PositionConfig::fromJson(val.toObject());
        ws->positions_[config.id] = config;
    }

    // 加载对位配置
    ws->alignmentConfig_ = AlignmentConfig::fromJson(json["alignmentConfig"].toObject());

    return ws;
}

bool WorkStation::saveToFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法保存工位配置: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("工位配置已保存: %1").arg(filePath));
    return true;
}

bool WorkStation::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法加载工位配置: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        LOG_ERROR("工位配置JSON解析失败");
        return false;
    }

    QJsonObject json = doc.object();

    // 加载数据
    name_ = json["name"].toString();
    enabled_ = json["enabled"].toBool(true);
    positionIdCounter_ = json["positionIdCounter"].toInt();

    positions_.clear();
    QJsonArray positionsArray = json["positions"].toArray();
    for (const auto& val : positionsArray) {
        PositionConfig config = PositionConfig::fromJson(val.toObject());
        positions_[config.id] = config;
    }

    alignmentConfig_ = AlignmentConfig::fromJson(json["alignmentConfig"].toObject());

    LOG_INFO(QString("工位配置已加载: %1").arg(filePath));
    return true;
}

// ========== 迭代对位实现 ==========

void WorkStation::setMotionController(std::shared_ptr<HAL::IMotionController> controller)
{
    motionController_ = controller;
    LOG_INFO(QString("工位 %1 设置运动控制器").arg(id_));
}

void WorkStation::setImageCaptureCallback(ImageCaptureCallback callback)
{
    imageCaptureCallback_ = callback;
}

void WorkStation::setAlignmentCallback(AlignmentCallback callback)
{
    alignmentCallback_ = callback;
}

void WorkStation::setPLCOutputCallback(PLCOutputCallback callback)
{
    plcOutputCallback_ = callback;
}

IterativeAlignResult WorkStation::executeIterativeAlignment(
    const QMap<QString, std::shared_ptr<Base::ImageData>>& initialImages)
{
    IterativeAlignResult result;
    QElapsedTimer timer;
    timer.start();

    const auto& iterConfig = alignmentConfig_.iterativeConfig;

    if (!iterConfig.enabled) {
        result.errorMessage = "迭代对位未启用";
        LOG_WARNING(result.errorMessage);
        return result;
    }

    if (!alignmentCallback_) {
        result.errorMessage = "未设置对位计算回调";
        LOG_ERROR(result.errorMessage);
        return result;
    }

    LOG_INFO(QString("工位 %1 开始迭代对位，最大迭代次数: %2")
             .arg(id_).arg(iterConfig.maxIterations));

    // 第一次对位使用传入的图像或自动采集
    QMap<QString, std::shared_ptr<Base::ImageData>> currentImages = initialImages;

    for (int iteration = 1; iteration <= iterConfig.maxIterations; ++iteration) {
        emit iterationStarted(iteration);

        LOG_INFO(QString("迭代对位 - 第 %1 次迭代").arg(iteration));

        // 1. 如果没有图像，执行采集
        if (currentImages.isEmpty() && imageCaptureCallback_) {
            if (iterConfig.captureDelayMs > 0) {
                QThread::msleep(iterConfig.captureDelayMs);
            }
            currentImages = imageCaptureCallback_();

            if (currentImages.isEmpty()) {
                result.errorMessage = QString("第 %1 次迭代：图像采集失败").arg(iteration);
                LOG_ERROR(result.errorMessage);
                break;
            }
        }

        // 2. 执行对位计算
        WorkStationAlignResult alignResult = alignmentCallback_(currentImages);
        result.iterationResults.append(alignResult);

        emit iterationCompleted(iteration, alignResult);

        if (!alignResult.success) {
            result.errorMessage = QString("第 %1 次迭代：对位计算失败").arg(iteration);
            LOG_ERROR(result.errorMessage);
            break;
        }

        // 3. 检查是否收敛
        if (checkConvergence(alignResult)) {
            result.success = true;
            result.converged = true;
            result.iterationCount = iteration;
            result.finalErrorX = alignResult.offsetX;
            result.finalErrorY = alignResult.offsetY;
            result.finalErrorD = alignResult.offsetD;
            result.residualError = alignResult.residualError;

            LOG_INFO(QString("迭代对位收敛，迭代次数: %1，残差: X=%.4f Y=%.4f D=%.4f")
                    .arg(iteration)
                    .arg(result.finalErrorX)
                    .arg(result.finalErrorY)
                    .arg(result.finalErrorD));
            break;
        }

        // 4. 如果未收敛且未到最大迭代次数，执行补偿运动
        if (iteration < iterConfig.maxIterations) {
            double moveX, moveY, moveTheta;
            calculateCompensation(alignResult, moveX, moveY, moveTheta);

            // 4a. 输出到PLC
            if (plcOutputCallback_) {
                if (!plcOutputCallback_(moveX, moveY, moveTheta)) {
                    result.errorMessage = QString("第 %1 次迭代：PLC输出失败").arg(iteration);
                    LOG_ERROR(result.errorMessage);
                    break;
                }
            }

            // 4b. 执行运动
            if (motionController_ && motionController_->isConnected()) {
                bool moveSuccess = false;

                if (iterConfig.useIncrementalCompensation) {
                    // 相对运动
                    moveSuccess = motionController_->moveXYTheta(
                        moveX, moveY, moveTheta, HAL::MoveMode::Relative);
                } else {
                    // 绝对运动
                    double currentX, currentY, currentTheta;
                    motionController_->getCurrentPosition(currentX, currentY, currentTheta);
                    moveSuccess = motionController_->moveXYTheta(
                        currentX + moveX, currentY + moveY, currentTheta + moveTheta,
                        HAL::MoveMode::Absolute);
                }

                if (!moveSuccess) {
                    result.errorMessage = QString("第 %1 次迭代：运动命令发送失败").arg(iteration);
                    LOG_ERROR(result.errorMessage);
                    break;
                }

                // 等待运动完成
                if (!motionController_->waitMotionComplete(5000)) {
                    result.errorMessage = QString("第 %1 次迭代：运动超时").arg(iteration);
                    LOG_ERROR(result.errorMessage);
                    break;
                }

                // 等待稳定
                if (iterConfig.motionSettleTimeMs > 0) {
                    motionController_->waitSettle(iterConfig.motionSettleTimeMs);
                }
            }

            // 清空当前图像，下次迭代重新采集
            currentImages.clear();
        }

        result.iterationCount = iteration;
    }

    // 如果达到最大迭代次数但未收敛
    if (!result.converged && result.iterationCount >= iterConfig.maxIterations) {
        result.success = false;
        result.errorMessage = QString("达到最大迭代次数 %1 次，未收敛").arg(iterConfig.maxIterations);

        // 记录最终误差
        if (!result.iterationResults.isEmpty()) {
            const auto& lastResult = result.iterationResults.last();
            result.finalErrorX = lastResult.offsetX;
            result.finalErrorY = lastResult.offsetY;
            result.finalErrorD = lastResult.offsetD;
            result.residualError = lastResult.residualError;
        }

        LOG_WARNING(result.errorMessage);
    }

    result.totalTime = timer.elapsed();
    lastIterativeResult_ = result;

    emit iterativeAlignmentCompleted(result);

    LOG_INFO(QString("迭代对位完成，成功: %1，迭代次数: %2，耗时: %.1f ms")
             .arg(result.success ? "是" : "否")
             .arg(result.iterationCount)
             .arg(result.totalTime));

    return result;
}

bool WorkStation::checkConvergence(const WorkStationAlignResult& result) const
{
    const auto& iterConfig = alignmentConfig_.iterativeConfig;

    double absX = std::abs(result.offsetX);
    double absY = std::abs(result.offsetY);
    double absD = std::abs(result.offsetD);

    bool xOK = absX <= iterConfig.convergenceThresholdX;
    bool yOK = absY <= iterConfig.convergenceThresholdY;
    bool dOK = absD <= iterConfig.convergenceThresholdD;

    return xOK && yOK && dOK;
}

void WorkStation::calculateCompensation(const WorkStationAlignResult& result,
                                         double& moveX, double& moveY, double& moveTheta)
{
    const auto& iterConfig = alignmentConfig_.iterativeConfig;

    // 应用补偿系数
    // 负号：偏差为正时需要向负方向补偿
    moveX = -result.offsetX * iterConfig.compensationFactorX;
    moveY = -result.offsetY * iterConfig.compensationFactorY;
    moveTheta = -result.offsetD * iterConfig.compensationFactorD;

    LOG_DEBUG(QString("计算补偿运动: X=%.4f Y=%.4f Theta=%.4f")
             .arg(moveX).arg(moveY).arg(moveTheta));
}

// ========== WorkStationManager ==========

WorkStationManager& WorkStationManager::instance()
{
    static WorkStationManager instance;
    return instance;
}

WorkStationManager::WorkStationManager()
    : wsIdCounter_(0)
{
}

WorkStationManager::~WorkStationManager()
{
}

QString WorkStationManager::createWorkStation(const QString& name)
{
    QString wsId = QString("WS%1").arg(wsIdCounter_++);

    auto ws = std::make_shared<WorkStation>(wsId, this);
    ws->setName(name.isEmpty() ? wsId : name);

    workStations_[wsId] = ws;

    if (currentWsId_.isEmpty()) {
        currentWsId_ = wsId;
    }

    emit workStationCreated(wsId);
    LOG_INFO(QString("创建工位: %1 (%2)").arg(ws->name()).arg(wsId));

    return wsId;
}

bool WorkStationManager::removeWorkStation(const QString& wsId)
{
    if (!workStations_.contains(wsId)) {
        return false;
    }

    workStations_.remove(wsId);

    if (currentWsId_ == wsId) {
        currentWsId_ = workStations_.isEmpty() ? "" : workStations_.firstKey();
        emit currentWorkStationChanged(currentWsId_);
    }

    emit workStationRemoved(wsId);
    LOG_INFO(QString("移除工位: %1").arg(wsId));

    return true;
}

WorkStation* WorkStationManager::getWorkStation(const QString& wsId)
{
    auto it = workStations_.find(wsId);
    if (it != workStations_.end()) {
        return it.value().get();
    }
    return nullptr;
}

QStringList WorkStationManager::getAllWorkStationIds() const
{
    return workStations_.keys();
}

int WorkStationManager::workStationCount() const
{
    return workStations_.size();
}

void WorkStationManager::setCurrentWorkStation(const QString& wsId)
{
    if (workStations_.contains(wsId) && currentWsId_ != wsId) {
        currentWsId_ = wsId;
        emit currentWorkStationChanged(wsId);
    }
}

WorkStation* WorkStationManager::currentWorkStation()
{
    return getWorkStation(currentWsId_);
}

bool WorkStationManager::saveAllConfig(const QString& dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 保存每个工位
    for (auto it = workStations_.begin(); it != workStations_.end(); ++it) {
        QString wsDir = dir.filePath(it.key());
        QDir().mkpath(wsDir);

        QString configPath = QDir(wsDir).filePath("WorkStationConfig.json");
        it.value()->saveToFile(configPath);
    }

    // 保存管理器配置
    QJsonObject managerJson;
    managerJson["currentWsId"] = currentWsId_;
    managerJson["wsIdCounter"] = wsIdCounter_;

    QJsonArray wsArray;
    for (const auto& wsId : workStations_.keys()) {
        wsArray.append(wsId);
    }
    managerJson["workStations"] = wsArray;

    QFile file(dir.filePath("WorkStationManager.json"));
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(managerJson);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }

    LOG_INFO(QString("所有工位配置已保存到: %1").arg(dirPath));
    return true;
}

bool WorkStationManager::loadAllConfig(const QString& dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        LOG_WARNING(QString("工位配置目录不存在: %1").arg(dirPath));
        return false;
    }

    // 加载管理器配置
    QFile file(dir.filePath("WorkStationManager.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject managerJson = doc.object();
    wsIdCounter_ = managerJson["wsIdCounter"].toInt();

    // 清空现有工位
    workStations_.clear();

    // 加载每个工位
    QJsonArray wsArray = managerJson["workStations"].toArray();
    for (const auto& val : wsArray) {
        QString wsId = val.toString();
        QString wsDir = dir.filePath(wsId);
        QString configPath = QDir(wsDir).filePath("WorkStationConfig.json");

        if (QFile::exists(configPath)) {
            auto ws = std::make_shared<WorkStation>(wsId, this);
            if (ws->loadFromFile(configPath)) {
                workStations_[wsId] = ws;
            }
        }
    }

    currentWsId_ = managerJson["currentWsId"].toString();
    if (!workStations_.contains(currentWsId_) && !workStations_.isEmpty()) {
        currentWsId_ = workStations_.firstKey();
    }

    LOG_INFO(QString("已加载 %1 个工位配置").arg(workStations_.size()));
    return true;
}

} // namespace Core
} // namespace VisionForge
