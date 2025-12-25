/**
 * @file MultiStationManager.cpp
 * @brief 多工位管理器实现
 */

#include "core/MultiStationManager.h"
#include "base/Logger.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>

namespace VisionForge {
namespace Core {

// ========== 单例实现 ==========

MultiStationManager& MultiStationManager::instance()
{
    static MultiStationManager instance;
    return instance;
}

MultiStationManager::MultiStationManager(QObject* parent)
    : QObject(parent)
{
    // 设置默认配置目录
    // 使用固定路径，确保主程序和配置工具使用相同的配置目录
    // GenericDataLocation 不依赖于应用程序名称
    configDirectory_ = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                       + "/VisionForge Pro/stations";
}

// ========== 工位管理 ==========

QString MultiStationManager::createStation(const QString& name, Platform::PlatformType type)
{
    if (stations_.size() >= MAX_STATION_NUM) {
        LOG_WARNING(QString("无法创建工位: 已达到最大数量 %1").arg(MAX_STATION_NUM));
        return QString();
    }

    QString stationId = generateStationId();
    int index = stations_.size();

    StationConfig config(stationId, name, index);
    config.platformType = type;

    // 创建默认的位置绑定
    for (int i = 0; i < config.positionNum; ++i) {
        PositionCameraBinding binding;
        binding.positionId = QString("pos%1").arg(i);
        binding.positionName = QString("Pos%1-%2")
                               .arg(i)
                               .arg(i < config.positionNum / 2 ? "Target" : "Object");
        binding.cameraIndex = i % config.cameraNum;
        binding.displayIndex = i;
        binding.captureOrder = i;
        binding.positionType = (i < config.positionNum / 2)
                               ? PositionCameraBinding::Target
                               : PositionCameraBinding::Object;

        config.positionBindings.append(binding);
    }

    stations_[stationId] = config;

    LOG_INFO(QString("创建工位: %1 (%2)").arg(name).arg(stationId));
    emit stationCreated(stationId);

    // 如果是第一个工位，设为当前工位
    if (stations_.size() == 1) {
        setCurrentStation(stationId);
    }

    return stationId;
}

bool MultiStationManager::removeStation(const QString& stationId)
{
    if (!stations_.contains(stationId)) {
        return false;
    }

    QString name = stations_[stationId].name;
    stations_.remove(stationId);

    // 重新调整索引
    int idx = 0;
    for (auto& config : stations_) {
        config.index = idx++;
    }

    LOG_INFO(QString("删除工位: %1 (%2)").arg(name).arg(stationId));
    emit stationRemoved(stationId);

    // 如果删除的是当前工位，切换到第一个
    if (currentStationId_ == stationId) {
        if (!stations_.isEmpty()) {
            setCurrentStation(stations_.firstKey());
        } else {
            currentStationId_.clear();
            emit currentStationChanged(QString(), -1);
        }
    }

    return true;
}

StationConfig* MultiStationManager::getStation(const QString& stationId)
{
    auto it = stations_.find(stationId);
    if (it != stations_.end()) {
        return &it.value();
    }
    return nullptr;
}

const StationConfig* MultiStationManager::getStation(const QString& stationId) const
{
    auto it = stations_.find(stationId);
    if (it != stations_.end()) {
        return &it.value();
    }
    return nullptr;
}

StationConfig* MultiStationManager::getStationByIndex(int index)
{
    for (auto& config : stations_) {
        if (config.index == index) {
            return &config;
        }
    }
    return nullptr;
}

const StationConfig* MultiStationManager::getStationByIndex(int index) const
{
    for (const auto& config : stations_) {
        if (config.index == index) {
            return &config;
        }
    }
    return nullptr;
}

QVector<StationConfig*> MultiStationManager::getAllStations()
{
    QVector<StationConfig*> result;
    for (auto& config : stations_) {
        result.append(&config);
    }
    // 按索引排序
    std::sort(result.begin(), result.end(), [](const StationConfig* a, const StationConfig* b) {
        return a->index < b->index;
    });
    return result;
}

QVector<const StationConfig*> MultiStationManager::getAllStations() const
{
    QVector<const StationConfig*> result;
    for (const auto& config : stations_) {
        result.append(&config);
    }
    std::sort(result.begin(), result.end(), [](const StationConfig* a, const StationConfig* b) {
        return a->index < b->index;
    });
    return result;
}

int MultiStationManager::getStationCount() const
{
    return stations_.size();
}

bool MultiStationManager::hasStation(const QString& stationId) const
{
    return stations_.contains(stationId);
}

// ========== 当前工位 ==========

bool MultiStationManager::setCurrentStation(const QString& stationId)
{
    if (!stations_.contains(stationId)) {
        return false;
    }

    if (currentStationId_ != stationId) {
        currentStationId_ = stationId;
        int index = stations_[stationId].index;

        LOG_INFO(QString("切换到工位: %1 (%2)")
                 .arg(stations_[stationId].name)
                 .arg(stationId));

        emit currentStationChanged(stationId, index);
    }

    return true;
}

bool MultiStationManager::setCurrentStationByIndex(int index)
{
    auto* config = getStationByIndex(index);
    if (config) {
        return setCurrentStation(config->id);
    }
    return false;
}

StationConfig* MultiStationManager::currentStation()
{
    return getStation(currentStationId_);
}

const StationConfig* MultiStationManager::currentStation() const
{
    return getStation(currentStationId_);
}

QString MultiStationManager::currentStationId() const
{
    return currentStationId_;
}

int MultiStationManager::currentStationIndex() const
{
    const auto* config = currentStation();
    return config ? config->index : -1;
}

// ========== 位置-相机绑定 ==========

bool MultiStationManager::addPositionBinding(const QString& stationId,
                                             const PositionCameraBinding& binding)
{
    auto* config = getStation(stationId);
    if (!config) return false;

    if (config->addPositionBinding(binding)) {
        emit positionBindingChanged(stationId, binding.positionId);
        emit stationConfigChanged(stationId);
        return true;
    }
    return false;
}

bool MultiStationManager::updatePositionBinding(const QString& stationId,
                                                const PositionCameraBinding& binding)
{
    auto* config = getStation(stationId);
    if (!config) return false;

    if (config->updatePositionBinding(binding)) {
        emit positionBindingChanged(stationId, binding.positionId);
        emit stationConfigChanged(stationId);
        return true;
    }
    return false;
}

bool MultiStationManager::removePositionBinding(const QString& stationId,
                                                const QString& positionId)
{
    auto* config = getStation(stationId);
    if (!config) return false;

    if (config->removePositionBinding(positionId)) {
        emit positionBindingChanged(stationId, positionId);
        emit stationConfigChanged(stationId);
        return true;
    }
    return false;
}

PositionCameraBinding* MultiStationManager::getPositionBinding(const QString& stationId,
                                                               const QString& positionId)
{
    auto* config = getStation(stationId);
    if (!config) return nullptr;

    return config->getPositionBinding(positionId);
}

// ========== 相机共享 ==========

bool MultiStationManager::setSharedCamera(const QString& targetStationId,
                                          const QString& sourceStationId)
{
    auto* target = getStation(targetStationId);
    auto* source = getStation(sourceStationId);

    if (!target || !source) return false;
    if (targetStationId == sourceStationId) return false;

    target->shareCameraSystem = true;
    target->sharedFromStationId = sourceStationId;
    target->cameraIds = source->cameraIds;
    target->cameraNum = source->cameraNum;

    LOG_INFO(QString("工位 %1 共享 %2 的相机系统")
             .arg(target->name).arg(source->name));

    emit stationConfigChanged(targetStationId);
    return true;
}

bool MultiStationManager::clearSharedCamera(const QString& stationId)
{
    auto* config = getStation(stationId);
    if (!config) return false;

    config->shareCameraSystem = false;
    config->sharedFromStationId.clear();

    emit stationConfigChanged(stationId);
    return true;
}

// ========== 配置持久化 ==========

bool MultiStationManager::saveConfig(const QString& configDir)
{
    QString dir = configDir.isEmpty() ? configDirectory_ : configDir;

    // 确保目录存在
    QDir().mkpath(dir);

    // 保存工位索引
    if (!saveStationsIndex(dir)) {
        LOG_ERROR("保存工位索引失败");
        return false;
    }

    // 保存每个工位配置
    for (const auto& config : stations_) {
        QString stationDir = dir + "/" + config.id;
        QDir().mkpath(stationDir);

        QString filePath = stationDir + "/station.json";
        if (!config.saveToFile(filePath)) {
            LOG_ERROR(QString("保存工位配置失败: %1").arg(config.id));
            return false;
        }
    }

    LOG_INFO(QString("工位配置已保存到: %1").arg(dir));
    emit configSaved();
    return true;
}

bool MultiStationManager::loadConfig(const QString& configDir)
{
    QString dir = configDir.isEmpty() ? configDirectory_ : configDir;

    if (!QDir(dir).exists()) {
        LOG_WARNING(QString("配置目录不存在: %1").arg(dir));
        return false;
    }

    // 清空现有配置
    stations_.clear();
    currentStationId_.clear();

    // 加载工位索引
    if (!loadStationsIndex(dir)) {
        LOG_WARNING("加载工位索引失败，尝试扫描目录");

        // 回退：扫描目录中的工位文件夹
        QDir stationsDir(dir);
        QStringList subdirs = stationsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        for (const QString& subdir : subdirs) {
            if (subdir.startsWith("WS")) {
                QString filePath = dir + "/" + subdir + "/station.json";
                if (QFile::exists(filePath)) {
                    bool ok;
                    StationConfig config = StationConfig::loadFromFile(filePath, &ok);
                    if (ok) {
                        stations_[config.id] = config;
                    }
                }
            }
        }
    }

    // 设置当前工位
    if (!stations_.isEmpty()) {
        // 优先使用索引0的工位
        auto* first = getStationByIndex(0);
        if (first) {
            currentStationId_ = first->id;
        } else {
            currentStationId_ = stations_.firstKey();
        }
    }

    LOG_INFO(QString("工位配置已加载: %1 个工位").arg(stations_.size()));
    emit configLoaded();

    if (!currentStationId_.isEmpty()) {
        emit currentStationChanged(currentStationId_, currentStationIndex());
    }

    return true;
}

QString MultiStationManager::getConfigDirectory() const
{
    return configDirectory_;
}

void MultiStationManager::setConfigDirectory(const QString& dir)
{
    configDirectory_ = dir;
}

bool MultiStationManager::saveStationsIndex(const QString& configDir) const
{
    QJsonObject root;
    root["version"] = "1.0";
    root["stationCount"] = stations_.size();
    root["currentStation"] = currentStationId_;

    QJsonArray stationsArray;
    for (const auto& config : stations_) {
        QJsonObject stationRef;
        stationRef["id"] = config.id;
        stationRef["name"] = config.name;
        stationRef["index"] = config.index;
        stationRef["configPath"] = config.id + "/station.json";
        stationsArray.append(stationRef);
    }
    root["stations"] = stationsArray;

    QString filePath = configDir + "/stations.json";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool MultiStationManager::loadStationsIndex(const QString& configDir)
{
    QString filePath = configDir + "/stations.json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError) {
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray stationsArray = root["stations"].toArray();

    for (const auto& stationVal : stationsArray) {
        QJsonObject stationRef = stationVal.toObject();
        QString configPath = configDir + "/" + stationRef["configPath"].toString();

        bool ok;
        StationConfig config = StationConfig::loadFromFile(configPath, &ok);
        if (ok) {
            stations_[config.id] = config;
        }
    }

    // 恢复当前工位
    QString savedCurrent = root["currentStation"].toString();
    if (stations_.contains(savedCurrent)) {
        currentStationId_ = savedCurrent;
    }

    return true;
}

// ========== 与现有模块同步 ==========

void MultiStationManager::syncToWorkStation(const QString& stationId)
{
    // TODO: 实现与 WorkStationManager 的同步
    Q_UNUSED(stationId);
}

void MultiStationManager::syncToCameraManager(const QString& stationId)
{
    // TODO: 实现与 CameraManager 的同步
    Q_UNUSED(stationId);
}

void MultiStationManager::syncAll()
{
    for (const auto& stationId : stations_.keys()) {
        syncToWorkStation(stationId);
        syncToCameraManager(stationId);
    }
}

// ========== 快速配置 ==========

void MultiStationManager::createDefaultConfig(int stationCount,
                                              int camerasPerStation,
                                              int positionsPerStation)
{
    clearAll();

    for (int i = 0; i < stationCount && i < MAX_STATION_NUM; ++i) {
        QString name = QString("工位%1").arg(i + 1);
        QString stationId = createStation(name);

        if (!stationId.isEmpty()) {
            auto* config = getStation(stationId);
            if (config) {
                config->cameraNum = camerasPerStation;
                config->positionNum = positionsPerStation;

                // 重建位置绑定
                config->positionBindings.clear();
                for (int p = 0; p < positionsPerStation; ++p) {
                    PositionCameraBinding binding;
                    binding.positionId = QString("pos%1").arg(p);
                    binding.positionName = QString("Pos%1-%2")
                                           .arg(p)
                                           .arg(p < positionsPerStation / 2 ? "Target" : "Object");
                    binding.cameraIndex = p % camerasPerStation;
                    binding.displayIndex = p;
                    binding.captureOrder = p;
                    binding.positionType = (p < positionsPerStation / 2)
                                           ? PositionCameraBinding::Target
                                           : PositionCameraBinding::Object;

                    config->positionBindings.append(binding);
                }

                // 生成相机ID
                config->cameraIds.clear();
                for (int c = 0; c < camerasPerStation; ++c) {
                    config->cameraIds.append(QString("cam_%1_%2").arg(stationId).arg(c));
                }
            }
        }
    }

    LOG_INFO(QString("创建默认配置: %1 工位, %2 相机/工位, %3 位置/工位")
             .arg(stationCount).arg(camerasPerStation).arg(positionsPerStation));
}

void MultiStationManager::clearAll()
{
    QStringList ids = stations_.keys();
    for (const QString& id : ids) {
        removeStation(id);
    }
}

// ========== 辅助方法 ==========

QString MultiStationManager::generateStationId() const
{
    int maxIndex = -1;
    for (const auto& config : stations_) {
        // 解析 WS0, WS1, WS2...
        if (config.id.startsWith("WS")) {
            bool ok;
            int num = config.id.mid(2).toInt(&ok);
            if (ok && num > maxIndex) {
                maxIndex = num;
            }
        }
    }
    return QString("WS%1").arg(maxIndex + 1);
}

} // namespace Core
} // namespace VisionForge
