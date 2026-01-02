/**
 * @file MultiStationManager.cpp
 * @brief 多工位管理器实现文件
 * @details 本文件实现了VisionForge视觉检测系统的多工位统一管理功能，包括：
 *          - 工位的创建、删除、查询等生命周期管理
 *          - 当前工位的切换与状态追踪
 *          - 位置-相机绑定关系的管理
 *          - 相机共享配置的管理
 *          - 工位配置的持久化存储与加载
 *          - 快速默认配置的批量生成
 *          采用单例模式确保全局唯一的工位管理实例
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-01-02
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
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

/**
 * @brief 获取多工位管理器单例实例
 * @return 管理器单例引用
 * @details 使用静态局部变量实现线程安全的单例模式
 */
MultiStationManager& MultiStationManager::instance()
{
    static MultiStationManager instance;
    return instance;
}

/**
 * @brief 构造函数 - 初始化多工位管理器
 * @param parent 父对象指针
 * @details 设置默认配置目录，使用系统通用数据位置确保
 *          主程序和配置工具可以共享相同的配置文件
 */
MultiStationManager::MultiStationManager(QObject* parent)
    : QObject(parent)
{
    // 设置默认配置目录
    // 使用固定路径，确保主程序和配置工具使用相同的配置目录
    // GenericDataLocation 不依赖于应用程序名称，适合多程序共享
    configDirectory_ = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                       + "/VisionForge Pro/stations";
}

// ========== 工位管理 ==========

/**
 * @brief 创建新工位
 * @param name 工位名称
 * @param type 平台类型
 * @return 新创建工位的ID，失败返回空字符串
 * @details 执行以下步骤：
 *          1. 检查是否达到最大工位数限制
 *          2. 生成唯一工位ID
 *          3. 创建默认位置绑定配置
 *          4. 如果是第一个工位，自动设为当前工位
 */
QString MultiStationManager::createStation(const QString& name, Platform::PlatformType type)
{
    // 检查工位数量限制
    if (stations_.size() >= MAX_STATION_NUM) {
        LOG_WARNING(QString("无法创建工位: 已达到最大数量 %1").arg(MAX_STATION_NUM));
        return QString();
    }

    // 生成唯一工位ID并确定索引
    QString stationId = generateStationId();
    int index = stations_.size();

    // 创建工位配置并设置平台类型
    StationConfig config(stationId, name, index);
    config.platformType = type;

    // 创建默认的位置-相机绑定关系
    // 前半位置为Target类型（目标），后半为Object类型（物料）
    for (int i = 0; i < config.positionNum; ++i) {
        PositionCameraBinding binding;
        binding.positionId = QString("pos%1").arg(i);
        binding.positionName = QString("Pos%1-%2")
                               .arg(i)
                               .arg(i < config.positionNum / 2 ? "Target" : "Object");
        binding.cameraIndex = i % config.cameraNum;  // 循环分配相机
        binding.displayIndex = i;
        binding.captureOrder = i;
        binding.positionType = (i < config.positionNum / 2)
                               ? PositionCameraBinding::Target
                               : PositionCameraBinding::Object;

        config.positionBindings.append(binding);
    }

    // 添加到工位集合
    stations_[stationId] = config;

    LOG_INFO(QString("创建工位: %1 (%2)").arg(name).arg(stationId));
    emit stationCreated(stationId);

    // 如果是第一个工位，自动设为当前工位
    if (stations_.size() == 1) {
        setCurrentStation(stationId);
    }

    return stationId;
}

/**
 * @brief 删除工位
 * @param stationId 要删除的工位ID
 * @return 删除成功返回true，工位不存在返回false
 * @details 删除后会重新调整所有工位的索引，保持连续
 */
bool MultiStationManager::removeStation(const QString& stationId)
{
    if (!stations_.contains(stationId)) {
        return false;
    }

    QString name = stations_[stationId].name;
    stations_.remove(stationId);

    // 重新调整剩余工位的索引，保持连续
    int idx = 0;
    for (auto& config : stations_) {
        config.index = idx++;
    }

    LOG_INFO(QString("删除工位: %1 (%2)").arg(name).arg(stationId));
    emit stationRemoved(stationId);

    // 如果删除的是当前工位，切换到第一个工位
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

/**
 * @brief 按ID获取工位配置（可修改）
 * @param stationId 工位ID
 * @return 工位配置指针，不存在返回nullptr
 */
StationConfig* MultiStationManager::getStation(const QString& stationId)
{
    auto it = stations_.find(stationId);
    if (it != stations_.end()) {
        return &it.value();
    }
    return nullptr;
}

/**
 * @brief 按ID获取工位配置（只读）
 * @param stationId 工位ID
 * @return 工位配置指针，不存在返回nullptr
 */
const StationConfig* MultiStationManager::getStation(const QString& stationId) const
{
    auto it = stations_.find(stationId);
    if (it != stations_.end()) {
        return &it.value();
    }
    return nullptr;
}

/**
 * @brief 按索引获取工位配置（可修改）
 * @param index 工位索引
 * @return 工位配置指针，索引无效返回nullptr
 */
StationConfig* MultiStationManager::getStationByIndex(int index)
{
    for (auto& config : stations_) {
        if (config.index == index) {
            return &config;
        }
    }
    return nullptr;
}

/**
 * @brief 按索引获取工位配置（只读）
 * @param index 工位索引
 * @return 工位配置指针，索引无效返回nullptr
 */
const StationConfig* MultiStationManager::getStationByIndex(int index) const
{
    for (const auto& config : stations_) {
        if (config.index == index) {
            return &config;
        }
    }
    return nullptr;
}

/**
 * @brief 获取所有工位配置列表（可修改）
 * @return 按索引排序的工位配置指针列表
 */
QVector<StationConfig*> MultiStationManager::getAllStations()
{
    QVector<StationConfig*> result;
    for (auto& config : stations_) {
        result.append(&config);
    }
    // 按索引排序确保顺序一致
    std::sort(result.begin(), result.end(), [](const StationConfig* a, const StationConfig* b) {
        return a->index < b->index;
    });
    return result;
}

/**
 * @brief 获取所有工位配置列表（只读）
 * @return 按索引排序的工位配置指针列表
 */
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

/**
 * @brief 获取工位总数
 * @return 当前管理的工位数量
 */
int MultiStationManager::getStationCount() const
{
    return stations_.size();
}

/**
 * @brief 检查工位是否存在
 * @param stationId 工位ID
 * @return 存在返回true，否则返回false
 */
bool MultiStationManager::hasStation(const QString& stationId) const
{
    return stations_.contains(stationId);
}

// ========== 当前工位 ==========

/**
 * @brief 设置当前工位
 * @param stationId 目标工位ID
 * @return 切换成功返回true，工位不存在返回false
 * @details 切换成功后发送currentStationChanged信号
 */
bool MultiStationManager::setCurrentStation(const QString& stationId)
{
    if (!stations_.contains(stationId)) {
        return false;
    }

    // 仅在工位变化时执行切换
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

/**
 * @brief 按索引设置当前工位
 * @param index 目标工位索引
 * @return 切换成功返回true，索引无效返回false
 */
bool MultiStationManager::setCurrentStationByIndex(int index)
{
    auto* config = getStationByIndex(index);
    if (config) {
        return setCurrentStation(config->id);
    }
    return false;
}

/**
 * @brief 获取当前工位配置（可修改）
 * @return 当前工位配置指针，无当前工位返回nullptr
 */
StationConfig* MultiStationManager::currentStation()
{
    return getStation(currentStationId_);
}

/**
 * @brief 获取当前工位配置（只读）
 * @return 当前工位配置指针，无当前工位返回nullptr
 */
const StationConfig* MultiStationManager::currentStation() const
{
    return getStation(currentStationId_);
}

/**
 * @brief 获取当前工位ID
 * @return 当前工位ID，无当前工位返回空字符串
 */
QString MultiStationManager::currentStationId() const
{
    return currentStationId_;
}

/**
 * @brief 获取当前工位索引
 * @return 当前工位索引，无当前工位返回-1
 */
int MultiStationManager::currentStationIndex() const
{
    const auto* config = currentStation();
    return config ? config->index : -1;
}

// ========== 位置-相机绑定 ==========

/**
 * @brief 添加位置-相机绑定关系
 * @param stationId 工位ID
 * @param binding 绑定关系配置
 * @return 添加成功返回true，失败返回false
 */
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

/**
 * @brief 更新位置-相机绑定关系
 * @param stationId 工位ID
 * @param binding 更新后的绑定关系配置
 * @return 更新成功返回true，失败返回false
 */
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

/**
 * @brief 移除位置-相机绑定关系
 * @param stationId 工位ID
 * @param positionId 位置ID
 * @return 移除成功返回true，失败返回false
 */
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

/**
 * @brief 获取位置-相机绑定关系
 * @param stationId 工位ID
 * @param positionId 位置ID
 * @return 绑定关系指针，不存在返回nullptr
 */
PositionCameraBinding* MultiStationManager::getPositionBinding(const QString& stationId,
                                                               const QString& positionId)
{
    auto* config = getStation(stationId);
    if (!config) return nullptr;

    return config->getPositionBinding(positionId);
}

// ========== 相机共享 ==========

/**
 * @brief 设置相机共享关系
 * @param targetStationId 目标工位ID（使用共享相机的工位）
 * @param sourceStationId 源工位ID（提供相机的工位）
 * @return 设置成功返回true，失败返回false
 * @details 目标工位将使用源工位的相机配置，实现相机资源共享
 */
bool MultiStationManager::setSharedCamera(const QString& targetStationId,
                                          const QString& sourceStationId)
{
    auto* target = getStation(targetStationId);
    auto* source = getStation(sourceStationId);

    // 验证参数有效性
    if (!target || !source) return false;
    if (targetStationId == sourceStationId) return false;  // 不能共享自己

    // 设置共享关系，复制相机配置
    target->shareCameraSystem = true;
    target->sharedFromStationId = sourceStationId;
    target->cameraIds = source->cameraIds;
    target->cameraNum = source->cameraNum;

    LOG_INFO(QString("工位 %1 共享 %2 的相机系统")
             .arg(target->name).arg(source->name));

    emit stationConfigChanged(targetStationId);
    return true;
}

/**
 * @brief 清除相机共享关系
 * @param stationId 工位ID
 * @return 清除成功返回true，失败返回false
 */
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

/**
 * @brief 保存所有工位配置
 * @param configDir 配置目录路径，为空则使用默认目录
 * @return 保存成功返回true，失败返回false
 * @details 保存结构：
 *          - stations.json: 工位索引文件
 *          - {工位ID}/station.json: 各工位配置文件
 */
bool MultiStationManager::saveConfig(const QString& configDir)
{
    QString dir = configDir.isEmpty() ? configDirectory_ : configDir;

    // 确保目录存在
    QDir().mkpath(dir);

    // 保存工位索引文件
    if (!saveStationsIndex(dir)) {
        LOG_ERROR("保存工位索引失败");
        return false;
    }

    // 保存每个工位的独立配置文件
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

/**
 * @brief 加载工位配置
 * @param configDir 配置目录路径，为空则使用默认目录
 * @return 加载成功返回true，失败返回false
 * @details 支持两种加载方式：
 *          1. 从索引文件加载（优先）
 *          2. 扫描目录自动发现（回退）
 */
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

    // 尝试从索引文件加载
    if (!loadStationsIndex(dir)) {
        LOG_WARNING("加载工位索引失败，尝试扫描目录");

        // 回退方案：扫描目录中以"WS"开头的子目录
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

    // 设置当前工位：优先使用索引0的工位
    if (!stations_.isEmpty()) {
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

/**
 * @brief 获取配置目录路径
 * @return 当前配置目录路径
 */
QString MultiStationManager::getConfigDirectory() const
{
    return configDirectory_;
}

/**
 * @brief 设置配置目录路径
 * @param dir 新的配置目录路径
 */
void MultiStationManager::setConfigDirectory(const QString& dir)
{
    configDirectory_ = dir;
}

/**
 * @brief 保存工位索引文件
 * @param configDir 配置目录路径
 * @return 保存成功返回true，失败返回false
 * @details 索引文件记录所有工位的基本信息和配置文件路径
 */
bool MultiStationManager::saveStationsIndex(const QString& configDir) const
{
    QJsonObject root;
    root["version"] = "1.0";
    root["stationCount"] = stations_.size();
    root["currentStation"] = currentStationId_;

    // 构建工位引用数组
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

    // 写入索引文件
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

/**
 * @brief 加载工位索引文件
 * @param configDir 配置目录路径
 * @return 加载成功返回true，失败返回false
 */
bool MultiStationManager::loadStationsIndex(const QString& configDir)
{
    QString filePath = configDir + "/stations.json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    // 解析索引文件
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError) {
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray stationsArray = root["stations"].toArray();

    // 按索引文件中的引用加载各工位配置
    for (const auto& stationVal : stationsArray) {
        QJsonObject stationRef = stationVal.toObject();
        QString configPath = configDir + "/" + stationRef["configPath"].toString();

        bool ok;
        StationConfig config = StationConfig::loadFromFile(configPath, &ok);
        if (ok) {
            stations_[config.id] = config;
        }
    }

    // 恢复当前工位设置
    QString savedCurrent = root["currentStation"].toString();
    if (stations_.contains(savedCurrent)) {
        currentStationId_ = savedCurrent;
    }

    return true;
}

// ========== 与现有模块同步 ==========

/**
 * @brief 同步配置到WorkStation模块
 * @param stationId 工位ID
 * @details TODO: 实现与WorkStationManager的配置同步
 */
void MultiStationManager::syncToWorkStation(const QString& stationId)
{
    // TODO: 实现与 WorkStationManager 的同步
    Q_UNUSED(stationId);
}

/**
 * @brief 同步配置到CameraManager模块
 * @param stationId 工位ID
 * @details TODO: 实现与CameraManager的配置同步
 */
void MultiStationManager::syncToCameraManager(const QString& stationId)
{
    // TODO: 实现与 CameraManager 的同步
    Q_UNUSED(stationId);
}

/**
 * @brief 同步所有工位配置到其他模块
 */
void MultiStationManager::syncAll()
{
    for (const auto& stationId : stations_.keys()) {
        syncToWorkStation(stationId);
        syncToCameraManager(stationId);
    }
}

// ========== 快速配置 ==========

/**
 * @brief 创建默认配置（批量）
 * @param stationCount 工位数量
 * @param camerasPerStation 每工位相机数量
 * @param positionsPerStation 每工位位置数量
 * @details 快速创建标准化的多工位配置，适用于初始化新系统
 */
void MultiStationManager::createDefaultConfig(int stationCount,
                                              int camerasPerStation,
                                              int positionsPerStation)
{
    // 清除现有配置
    clearAll();

    // 批量创建工位
    for (int i = 0; i < stationCount && i < MAX_STATION_NUM; ++i) {
        QString name = QString("工位%1").arg(i + 1);
        QString stationId = createStation(name);

        if (!stationId.isEmpty()) {
            auto* config = getStation(stationId);
            if (config) {
                // 设置相机和位置数量
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

/**
 * @brief 清除所有工位配置
 */
void MultiStationManager::clearAll()
{
    QStringList ids = stations_.keys();
    for (const QString& id : ids) {
        removeStation(id);
    }
}

// ========== 辅助方法 ==========

/**
 * @brief 生成唯一工位ID
 * @return 格式为 "WSN" 的唯一ID字符串
 * @details 扫描现有工位ID，生成下一个可用的编号
 */
QString MultiStationManager::generateStationId() const
{
    int maxIndex = -1;

    // 查找现有最大编号
    for (const auto& config : stations_) {
        // 解析 WS0, WS1, WS2... 格式的ID
        if (config.id.startsWith("WS")) {
            bool ok;
            int num = config.id.mid(2).toInt(&ok);
            if (ok && num > maxIndex) {
                maxIndex = num;
            }
        }
    }

    // 返回下一个编号
    return QString("WS%1").arg(maxIndex + 1);
}

} // namespace Core
} // namespace VisionForge
