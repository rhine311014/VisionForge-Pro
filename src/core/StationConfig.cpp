/**
 * @file StationConfig.cpp
 * @brief 工位配置结构实现文件
 * @details 本文件实现了VisionForge视觉检测系统的工位配置管理功能，包括：
 *          - 工位配置的JSON序列化与反序列化
 *          - 位置-相机绑定关系的管理
 *          - 场景配置的增删改查
 *          - 显示布局的自动计算
 *          - 配置有效性验证
 *          工位是视觉检测系统的物理单元，包含多个相机和检测位置
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-01-02
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 */

#include "core/StationConfig.h"
#include <QJsonDocument>
#include <QFile>
#include <QDir>

namespace VisionForge {
namespace Core {

// ========== 序列化 ==========

/**
 * @brief 将工位配置序列化为JSON对象
 * @return 包含工位完整配置的JSON对象
 * @details 序列化内容包括：
 *          - 基本信息（ID、名称、索引、启用状态）
 *          - 平台配置（类型、运动范围）
 *          - UVW平台特殊配置（仅UVW系列）
 *          - 相机配置（数量、ID列表、共享设置）
 *          - 位置绑定配置
 *          - 功能模式、显示、触发、通信配置
 *          - 场景配置列表
 */
QJsonObject StationConfig::toJson() const
{
    QJsonObject json;

    // 序列化基本信息
    json["id"] = id;
    json["name"] = name;
    json["index"] = index;
    json["enabled"] = enabled;

    // 序列化平台配置
    QJsonObject platformJson;
    platformJson["type"] = static_cast<int>(platformType);
    platformJson["configId"] = platformConfigId;
    platformJson["xRange"] = xRange;      // X轴运动范围（mm）
    platformJson["yRange"] = yRange;      // Y轴运动范围（mm）
    platformJson["dRange"] = dRange;      // 旋转范围（度）
    json["platform"] = platformJson;

    // UVW平台特殊配置 - 仅对UVW系列平台保存
    // UVW平台是一种精密三轴对位平台，用于高精度贴合应用
    if (Platform::platformIsUVW(platformType)) {
        QJsonObject uvwJson;
        uvwJson["structureType"] = uvwStructureType;      // 结构类型
        uvwJson["sliderType"] = uvwSliderType;            // 滑块类型
        uvwJson["precisionGrade"] = uvwPrecisionGrade;    // 精度等级
        uvwJson["node1PosX"] = uvwNode1PosX;              // 节点1 X坐标
        uvwJson["node1PosY"] = uvwNode1PosY;              // 节点1 Y坐标
        uvwJson["node2PosX"] = uvwNode2PosX;              // 节点2 X坐标
        uvwJson["node2PosY"] = uvwNode2PosY;              // 节点2 Y坐标
        uvwJson["node3PosX"] = uvwNode3PosX;              // 节点3 X坐标
        uvwJson["node3PosY"] = uvwNode3PosY;              // 节点3 Y坐标
        uvwJson["travelXY"] = uvwTravelXY;                // XY行程
        uvwJson["travelTheta"] = uvwTravelTheta;          // 角度行程
        json["uvwConfig"] = uvwJson;
    }

    // 序列化相机配置
    QJsonObject camerasJson;
    camerasJson["count"] = cameraNum;
    QJsonArray cameraIdsArray;
    for (const auto& camId : cameraIds) {
        cameraIdsArray.append(camId);
    }
    camerasJson["ids"] = cameraIdsArray;
    camerasJson["shared"] = shareCameraSystem;            // 是否共享相机
    camerasJson["sharedFrom"] = sharedFromStationId;      // 共享来源工位ID
    json["cameras"] = camerasJson;

    // 序列化位置绑定配置
    QJsonObject positionsJson;
    positionsJson["count"] = positionNum;
    QJsonArray bindingsArray;
    for (const auto& binding : positionBindings) {
        bindingsArray.append(binding.toJson());
    }
    positionsJson["bindings"] = bindingsArray;
    json["positions"] = positionsJson;

    // 序列化功能模式配置
    // DetectionOnly: 仅检测，AlignmentOnly: 仅对位，DetectionAndAlignment: 检测+对位
    QString functionModeStr;
    switch (functionMode) {
        case DetectionOnly: functionModeStr = "DetectionOnly"; break;
        case AlignmentOnly: functionModeStr = "AlignmentOnly"; break;
        case DetectionAndAlignment:
        default: functionModeStr = "DetectionAndAlignment"; break;
    }
    json["functionMode"] = functionModeStr;

    // 序列化显示配置
    QJsonObject displayJson;
    displayJson["mode"] = displayMode == ByPosition ? "ByPosition" : "ByCamera";
    displayJson["layout"] = static_cast<int>(displayLayout);
    json["display"] = displayJson;

    // 序列化触发配置
    QJsonObject triggerJson;
    triggerJson["useHardware"] = useHardwareTrigger;      // 是否使用硬件触发
    triggerJson["source"] = triggerSource;                // 触发源
    json["trigger"] = triggerJson;

    // 序列化通信配置
    QJsonObject commJson;
    commJson["plcAddress"] = plcAddress;                  // PLC地址
    commJson["plcPort"] = plcPort;                        // PLC端口
    json["communication"] = commJson;

    // 序列化场景配置列表
    QJsonObject scenesJson;
    scenesJson["count"] = sceneNum;
    scenesJson["currentIndex"] = currentSceneIndex;
    QJsonArray scenesArray;
    for (const auto& scene : scenes) {
        scenesArray.append(scene.toJson());
    }
    scenesJson["list"] = scenesArray;
    json["scenes"] = scenesJson;

    return json;
}

/**
 * @brief 从JSON对象反序列化工位配置
 * @param json 包含工位配置的JSON对象
 * @return 解析后的工位配置结构
 * @details 按照序列化的逆过程恢复配置，包含向后兼容处理
 */
StationConfig StationConfig::fromJson(const QJsonObject& json)
{
    StationConfig config;

    // 解析基本信息
    config.id = json["id"].toString();
    config.name = json["name"].toString();
    config.index = json["index"].toInt(0);
    config.enabled = json["enabled"].toBool(true);

    // 解析平台配置
    QJsonObject platformJson = json["platform"].toObject();
    config.platformType = static_cast<Platform::PlatformType>(platformJson["type"].toInt(0));
    config.platformConfigId = platformJson["configId"].toString();
    config.xRange = platformJson["xRange"].toDouble(500.0);
    config.yRange = platformJson["yRange"].toDouble(500.0);
    config.dRange = platformJson["dRange"].toDouble(360.0);

    // 解析UVW平台特殊配置
    if (json.contains("uvwConfig")) {
        QJsonObject uvwJson = json["uvwConfig"].toObject();
        config.uvwStructureType = uvwJson["structureType"].toInt(0);
        config.uvwSliderType = uvwJson["sliderType"].toInt(0);
        config.uvwPrecisionGrade = uvwJson["precisionGrade"].toInt(0);
        config.uvwNode1PosX = uvwJson["node1PosX"].toDouble(67.5);
        config.uvwNode1PosY = uvwJson["node1PosY"].toDouble(-57.0);
        config.uvwNode2PosX = uvwJson["node2PosX"].toDouble(67.5);
        config.uvwNode2PosY = uvwJson["node2PosY"].toDouble(57.0);
        config.uvwNode3PosX = uvwJson["node3PosX"].toDouble(-57.0);
        config.uvwNode3PosY = uvwJson["node3PosY"].toDouble(67.5);
        config.uvwTravelXY = uvwJson["travelXY"].toDouble(5.0);
        config.uvwTravelTheta = uvwJson["travelTheta"].toDouble(2.0);
    }

    // 解析相机配置
    QJsonObject camerasJson = json["cameras"].toObject();
    config.cameraNum = camerasJson["count"].toInt(2);
    QJsonArray cameraIdsArray = camerasJson["ids"].toArray();
    config.cameraIds.clear();
    for (const auto& id : cameraIdsArray) {
        config.cameraIds.append(id.toString());
    }
    config.shareCameraSystem = camerasJson["shared"].toBool(false);
    config.sharedFromStationId = camerasJson["sharedFrom"].toString();

    // 解析位置绑定配置
    QJsonObject positionsJson = json["positions"].toObject();
    config.positionNum = positionsJson["count"].toInt(2);
    QJsonArray bindingsArray = positionsJson["bindings"].toArray();
    config.positionBindings.clear();
    for (const auto& bindingVal : bindingsArray) {
        config.positionBindings.append(
            PositionCameraBinding::fromJson(bindingVal.toObject())
        );
    }

    // 解析功能模式配置
    QString functionModeStr = json["functionMode"].toString("DetectionAndAlignment");
    if (functionModeStr == "DetectionOnly") {
        config.functionMode = DetectionOnly;
    } else if (functionModeStr == "AlignmentOnly") {
        config.functionMode = AlignmentOnly;
    } else {
        config.functionMode = DetectionAndAlignment;
    }

    // 解析显示配置
    QJsonObject displayJson = json["display"].toObject();
    QString modeStr = displayJson["mode"].toString("ByPosition");
    config.displayMode = (modeStr == "ByCamera") ? ByCamera : ByPosition;
    config.displayLayout = static_cast<LayoutMode>(displayJson["layout"].toInt(0));

    // 解析触发配置
    QJsonObject triggerJson = json["trigger"].toObject();
    config.useHardwareTrigger = triggerJson["useHardware"].toBool(false);
    config.triggerSource = triggerJson["source"].toString("Software");

    // 解析通信配置
    QJsonObject commJson = json["communication"].toObject();
    config.plcAddress = commJson["plcAddress"].toString();
    config.plcPort = commJson["plcPort"].toInt(502);

    // 解析场景配置
    QJsonObject scenesJson = json["scenes"].toObject();
    config.sceneNum = scenesJson["count"].toInt(1);
    config.currentSceneIndex = scenesJson["currentIndex"].toInt(0);
    QJsonArray scenesArray = scenesJson["list"].toArray();
    config.scenes.clear();
    for (const auto& sceneVal : scenesArray) {
        config.scenes.append(SceneConfig::fromJson(sceneVal.toObject()));
    }

    // 向后兼容处理：如果场景数量>0但场景列表为空，自动创建默认场景
    // 这是为了兼容旧版本配置文件
    if (config.scenes.isEmpty() && config.sceneNum > 0) {
        for (int s = 0; s < config.sceneNum; ++s) {
            SceneConfig scene;
            scene.sceneId = QString("scene%1").arg(s);
            scene.sceneName = QString("场景%1").arg(s + 1);
            scene.sceneIndex = s;
            scene.enabled = true;
            config.scenes.append(scene);
        }
    }

    return config;
}

/**
 * @brief 将工位配置保存到文件
 * @param filePath 目标文件路径
 * @return 保存成功返回true，失败返回false
 */
bool StationConfig::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    // 序列化并格式化输出
    QJsonDocument doc(toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

/**
 * @brief 从文件加载工位配置
 * @param filePath 源文件路径
 * @param ok 输出参数，指示加载是否成功
 * @return 解析后的工位配置结构
 */
StationConfig StationConfig::loadFromFile(const QString& filePath, bool* ok)
{
    if (ok) *ok = false;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return StationConfig();
    }

    // 解析JSON文档
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError) {
        return StationConfig();
    }

    if (ok) *ok = true;
    return fromJson(doc.object());
}

// ========== 位置管理 ==========

/**
 * @brief 添加位置-相机绑定关系
 * @param binding 要添加的绑定关系
 * @return 添加成功返回true，已存在同ID则返回false
 */
bool StationConfig::addPositionBinding(const PositionCameraBinding& binding)
{
    // 检查是否已存在相同ID的绑定
    for (const auto& existing : positionBindings) {
        if (existing.positionId == binding.positionId) {
            return false;
        }
    }

    // 添加新绑定并更新计数
    positionBindings.append(binding);
    positionNum = positionBindings.size();
    return true;
}

/**
 * @brief 移除位置-相机绑定关系
 * @param positionId 要移除的位置ID
 * @return 移除成功返回true，不存在则返回false
 */
bool StationConfig::removePositionBinding(const QString& positionId)
{
    for (int i = 0; i < positionBindings.size(); ++i) {
        if (positionBindings[i].positionId == positionId) {
            positionBindings.removeAt(i);
            positionNum = positionBindings.size();
            return true;
        }
    }
    return false;
}

/**
 * @brief 更新位置-相机绑定关系
 * @param binding 更新后的绑定关系
 * @return 更新成功返回true，不存在则返回false
 */
bool StationConfig::updatePositionBinding(const PositionCameraBinding& binding)
{
    for (auto& existing : positionBindings) {
        if (existing.positionId == binding.positionId) {
            existing = binding;
            return true;
        }
    }
    return false;
}

/**
 * @brief 按位置ID获取绑定关系（可修改）
 * @param positionId 位置ID
 * @return 绑定关系指针，不存在返回nullptr
 */
PositionCameraBinding* StationConfig::getPositionBinding(const QString& positionId)
{
    for (auto& binding : positionBindings) {
        if (binding.positionId == positionId) {
            return &binding;
        }
    }
    return nullptr;
}

/**
 * @brief 按位置ID获取绑定关系（只读）
 * @param positionId 位置ID
 * @return 绑定关系指针，不存在返回nullptr
 */
const PositionCameraBinding* StationConfig::getPositionBinding(const QString& positionId) const
{
    for (const auto& binding : positionBindings) {
        if (binding.positionId == positionId) {
            return &binding;
        }
    }
    return nullptr;
}

/**
 * @brief 按索引获取绑定关系（可修改）
 * @param index 位置索引
 * @return 绑定关系指针，索引无效返回nullptr
 */
PositionCameraBinding* StationConfig::getPositionBindingByIndex(int index)
{
    if (index >= 0 && index < positionBindings.size()) {
        return &positionBindings[index];
    }
    return nullptr;
}

/**
 * @brief 按索引获取绑定关系（只读）
 * @param index 位置索引
 * @return 绑定关系指针，索引无效返回nullptr
 */
const PositionCameraBinding* StationConfig::getPositionBindingByIndex(int index) const
{
    if (index >= 0 && index < positionBindings.size()) {
        return &positionBindings[index];
    }
    return nullptr;
}

// ========== 相机关联查询 ==========

/**
 * @brief 获取指定位置关联的相机ID
 * @param posIndex 位置索引
 * @return 相机ID，位置无效返回空字符串
 */
QString StationConfig::getCameraIdForPosition(int posIndex) const
{
    const auto* binding = getPositionBindingByIndex(posIndex);
    if (binding) {
        return binding->cameraId;
    }
    return QString();
}

/**
 * @brief 获取指定相机关联的所有位置索引
 * @param cameraId 相机ID
 * @return 关联该相机的位置索引列表
 * @details 一个相机可能被多个位置共享使用
 */
QVector<int> StationConfig::getPositionsForCamera(const QString& cameraId) const
{
    QVector<int> positions;
    for (int i = 0; i < positionBindings.size(); ++i) {
        if (positionBindings[i].cameraId == cameraId) {
            positions.append(i);
        }
    }
    return positions;
}

/**
 * @brief 获取所有目标位置的绑定列表
 * @return 类型为Target的位置绑定列表
 * @details Target位置用于拍摄贴装目标（如PCB Mark点）
 */
PositionBindingList StationConfig::getTargetPositions() const
{
    PositionBindingList targets;
    for (const auto& binding : positionBindings) {
        if (binding.isTarget()) {
            targets.append(binding);
        }
    }
    return targets;
}

/**
 * @brief 获取所有物料位置的绑定列表
 * @return 类型为Object的位置绑定列表
 * @details Object位置用于拍摄待贴装物料（如芯片）
 */
PositionBindingList StationConfig::getObjectPositions() const
{
    PositionBindingList objects;
    for (const auto& binding : positionBindings) {
        if (binding.isObject()) {
            objects.append(binding);
        }
    }
    return objects;
}

// ========== 场景管理 ==========

/**
 * @brief 添加场景配置
 * @param scene 要添加的场景配置
 * @return 添加成功返回true，已存在同ID则返回false
 */
bool StationConfig::addScene(const SceneConfig& scene)
{
    // 检查是否已存在相同ID的场景
    for (const auto& existing : scenes) {
        if (existing.sceneId == scene.sceneId) {
            return false;
        }
    }

    scenes.append(scene);
    sceneNum = scenes.size();
    return true;
}

/**
 * @brief 移除场景配置
 * @param sceneId 要移除的场景ID
 * @return 移除成功返回true，不存在则返回false
 * @details 移除后会自动调整当前场景索引
 */
bool StationConfig::removeScene(const QString& sceneId)
{
    for (int i = 0; i < scenes.size(); ++i) {
        if (scenes[i].sceneId == sceneId) {
            scenes.removeAt(i);
            sceneNum = scenes.size();

            // 调整当前场景索引，确保不越界
            if (currentSceneIndex >= sceneNum) {
                currentSceneIndex = qMax(0, sceneNum - 1);
            }
            return true;
        }
    }
    return false;
}

/**
 * @brief 更新场景配置
 * @param scene 更新后的场景配置
 * @return 更新成功返回true，不存在则返回false
 */
bool StationConfig::updateScene(const SceneConfig& scene)
{
    for (auto& existing : scenes) {
        if (existing.sceneId == scene.sceneId) {
            existing = scene;
            return true;
        }
    }
    return false;
}

/**
 * @brief 按场景ID获取场景配置（可修改）
 * @param sceneId 场景ID
 * @return 场景配置指针，不存在返回nullptr
 */
SceneConfig* StationConfig::getScene(const QString& sceneId)
{
    for (auto& scene : scenes) {
        if (scene.sceneId == sceneId) {
            return &scene;
        }
    }
    return nullptr;
}

/**
 * @brief 按场景ID获取场景配置（只读）
 * @param sceneId 场景ID
 * @return 场景配置指针，不存在返回nullptr
 */
const SceneConfig* StationConfig::getScene(const QString& sceneId) const
{
    for (const auto& scene : scenes) {
        if (scene.sceneId == sceneId) {
            return &scene;
        }
    }
    return nullptr;
}

/**
 * @brief 按索引获取场景配置（可修改）
 * @param index 场景索引
 * @return 场景配置指针，索引无效返回nullptr
 */
SceneConfig* StationConfig::getSceneByIndex(int index)
{
    if (index >= 0 && index < scenes.size()) {
        return &scenes[index];
    }
    return nullptr;
}

/**
 * @brief 按索引获取场景配置（只读）
 * @param index 场景索引
 * @return 场景配置指针，索引无效返回nullptr
 */
const SceneConfig* StationConfig::getSceneByIndex(int index) const
{
    if (index >= 0 && index < scenes.size()) {
        return &scenes[index];
    }
    return nullptr;
}

/**
 * @brief 获取当前场景配置（可修改）
 * @return 当前场景配置指针
 */
SceneConfig* StationConfig::getCurrentScene()
{
    return getSceneByIndex(currentSceneIndex);
}

/**
 * @brief 获取当前场景配置（只读）
 * @return 当前场景配置指针
 */
const SceneConfig* StationConfig::getCurrentScene() const
{
    return getSceneByIndex(currentSceneIndex);
}

/**
 * @brief 按索引切换当前场景
 * @param sceneIndex 目标场景索引
 * @return 切换成功返回true，场景禁用或索引无效返回false
 */
bool StationConfig::switchToScene(int sceneIndex)
{
    if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
        // 只能切换到启用的场景
        if (scenes[sceneIndex].enabled) {
            currentSceneIndex = sceneIndex;
            return true;
        }
    }
    return false;
}

/**
 * @brief 按场景ID切换当前场景
 * @param sceneId 目标场景ID
 * @return 切换成功返回true，场景不存在或禁用返回false
 */
bool StationConfig::switchToScene(const QString& sceneId)
{
    for (int i = 0; i < scenes.size(); ++i) {
        if (scenes[i].sceneId == sceneId) {
            return switchToScene(i);
        }
    }
    return false;
}

// ========== 布局辅助 ==========

/**
 * @brief 自动计算最佳布局模式
 * @return 根据显示项数量推荐的布局模式
 * @details 根据位置数或相机数自动选择合适的网格布局：
 *          - 1个: 1x1
 *          - 2个: 1x2
 *          - 3-4个: 2x2
 *          - 5-6个: 2x3
 *          - 7-8个: 2x4
 */
StationConfig::LayoutMode StationConfig::getAutoLayout() const
{
    // 根据显示模式确定显示项数量
    int count = displayMode == ByPosition ? positionNum : cameraNum;

    if (count <= 1) return Layout_1x1;
    if (count <= 2) return Layout_1x2;
    if (count <= 4) return Layout_2x2;
    if (count <= 6) return Layout_2x3;
    return Layout_2x4;
}

/**
 * @brief 获取布局的行列数
 * @param rows 输出行数
 * @param cols 输出列数
 * @details 根据当前有效布局模式计算网格尺寸
 */
void StationConfig::getLayoutDimensions(int& rows, int& cols) const
{
    LayoutMode layout = getEffectiveLayout();

    switch (layout) {
    case Layout_1x1:
        rows = 1; cols = 1;
        break;
    case Layout_1x2:
        rows = 1; cols = 2;
        break;
    case Layout_2x2:
        rows = 2; cols = 2;
        break;
    case Layout_2x3:
        rows = 2; cols = 3;
        break;
    case Layout_2x4:
        rows = 2; cols = 4;
        break;
    default:
        rows = 2; cols = 2;
        break;
    }
}

// ========== 验证 ==========

/**
 * @brief 验证配置有效性
 * @return 配置有效返回true，无效返回false
 * @details 检查项：
 *          - ID和名称非空
 *          - 索引范围有效（0-5）
 *          - 相机和位置数量范围有效（1-8）
 *          - 所有位置绑定的相机索引有效
 */
bool StationConfig::isValid() const
{
    if (id.isEmpty()) return false;
    if (name.isEmpty()) return false;
    if (index < 0 || index > 5) return false;
    if (cameraNum < 1 || cameraNum > 8) return false;
    if (positionNum < 1 || positionNum > 8) return false;

    // 检查每个位置绑定的有效性
    for (const auto& binding : positionBindings) {
        if (binding.positionId.isEmpty()) return false;
        if (binding.cameraIndex < 0 || binding.cameraIndex >= cameraNum) return false;
    }

    return true;
}

/**
 * @brief 获取配置验证错误信息
 * @return 详细的错误描述字符串，配置有效则返回空字符串
 */
QString StationConfig::getValidationError() const
{
    if (id.isEmpty()) return "工位ID不能为空";
    if (name.isEmpty()) return "工位名称不能为空";
    if (index < 0 || index > 5) return "工位索引必须在0-5之间";
    if (cameraNum < 1 || cameraNum > 8) return "相机数量必须在1-8之间";
    if (positionNum < 1 || positionNum > 8) return "位置数量必须在1-8之间";

    for (int i = 0; i < positionBindings.size(); ++i) {
        const auto& binding = positionBindings[i];
        if (binding.positionId.isEmpty()) {
            return QString("位置%1的ID不能为空").arg(i);
        }
        if (binding.cameraIndex < 0 || binding.cameraIndex >= cameraNum) {
            return QString("位置%1的相机索引(%2)超出范围(0-%3)")
                   .arg(i).arg(binding.cameraIndex).arg(cameraNum - 1);
        }
    }

    return QString();
}

} // namespace Core
} // namespace VisionForge
