/**
 * @file StationConfig.cpp
 * @brief 工位配置结构实现
 */

#include "core/StationConfig.h"
#include <QJsonDocument>
#include <QFile>
#include <QDir>

namespace VisionForge {
namespace Core {

// ========== 序列化 ==========

QJsonObject StationConfig::toJson() const
{
    QJsonObject json;

    // 基本信息
    json["id"] = id;
    json["name"] = name;
    json["index"] = index;
    json["enabled"] = enabled;

    // 平台配置
    QJsonObject platformJson;
    platformJson["type"] = static_cast<int>(platformType);
    platformJson["configId"] = platformConfigId;
    platformJson["xRange"] = xRange;
    platformJson["yRange"] = yRange;
    platformJson["dRange"] = dRange;
    json["platform"] = platformJson;

    // 相机配置
    QJsonObject camerasJson;
    camerasJson["count"] = cameraNum;
    QJsonArray cameraIdsArray;
    for (const auto& camId : cameraIds) {
        cameraIdsArray.append(camId);
    }
    camerasJson["ids"] = cameraIdsArray;
    camerasJson["shared"] = shareCameraSystem;
    camerasJson["sharedFrom"] = sharedFromStationId;
    json["cameras"] = camerasJson;

    // 位置配置
    QJsonObject positionsJson;
    positionsJson["count"] = positionNum;
    QJsonArray bindingsArray;
    for (const auto& binding : positionBindings) {
        bindingsArray.append(binding.toJson());
    }
    positionsJson["bindings"] = bindingsArray;
    json["positions"] = positionsJson;

    // 功能模式配置
    QString functionModeStr;
    switch (functionMode) {
        case DetectionOnly: functionModeStr = "DetectionOnly"; break;
        case AlignmentOnly: functionModeStr = "AlignmentOnly"; break;
        case DetectionAndAlignment:
        default: functionModeStr = "DetectionAndAlignment"; break;
    }
    json["functionMode"] = functionModeStr;

    // 显示配置
    QJsonObject displayJson;
    displayJson["mode"] = displayMode == ByPosition ? "ByPosition" : "ByCamera";
    displayJson["layout"] = static_cast<int>(displayLayout);
    json["display"] = displayJson;

    // 触发配置
    QJsonObject triggerJson;
    triggerJson["useHardware"] = useHardwareTrigger;
    triggerJson["source"] = triggerSource;
    json["trigger"] = triggerJson;

    // 通信配置
    QJsonObject commJson;
    commJson["plcAddress"] = plcAddress;
    commJson["plcPort"] = plcPort;
    json["communication"] = commJson;

    return json;
}

StationConfig StationConfig::fromJson(const QJsonObject& json)
{
    StationConfig config;

    // 基本信息
    config.id = json["id"].toString();
    config.name = json["name"].toString();
    config.index = json["index"].toInt(0);
    config.enabled = json["enabled"].toBool(true);

    // 平台配置
    QJsonObject platformJson = json["platform"].toObject();
    config.platformType = static_cast<Platform::PlatformType>(platformJson["type"].toInt(0));
    config.platformConfigId = platformJson["configId"].toString();
    config.xRange = platformJson["xRange"].toDouble(500.0);
    config.yRange = platformJson["yRange"].toDouble(500.0);
    config.dRange = platformJson["dRange"].toDouble(360.0);

    // 相机配置
    QJsonObject camerasJson = json["cameras"].toObject();
    config.cameraNum = camerasJson["count"].toInt(2);
    QJsonArray cameraIdsArray = camerasJson["ids"].toArray();
    config.cameraIds.clear();
    for (const auto& id : cameraIdsArray) {
        config.cameraIds.append(id.toString());
    }
    config.shareCameraSystem = camerasJson["shared"].toBool(false);
    config.sharedFromStationId = camerasJson["sharedFrom"].toString();

    // 位置配置
    QJsonObject positionsJson = json["positions"].toObject();
    config.positionNum = positionsJson["count"].toInt(2);
    QJsonArray bindingsArray = positionsJson["bindings"].toArray();
    config.positionBindings.clear();
    for (const auto& bindingVal : bindingsArray) {
        config.positionBindings.append(
            PositionCameraBinding::fromJson(bindingVal.toObject())
        );
    }

    // 功能模式配置
    QString functionModeStr = json["functionMode"].toString("DetectionAndAlignment");
    if (functionModeStr == "DetectionOnly") {
        config.functionMode = DetectionOnly;
    } else if (functionModeStr == "AlignmentOnly") {
        config.functionMode = AlignmentOnly;
    } else {
        config.functionMode = DetectionAndAlignment;
    }

    // 显示配置
    QJsonObject displayJson = json["display"].toObject();
    QString modeStr = displayJson["mode"].toString("ByPosition");
    config.displayMode = (modeStr == "ByCamera") ? ByCamera : ByPosition;
    config.displayLayout = static_cast<LayoutMode>(displayJson["layout"].toInt(0));

    // 触发配置
    QJsonObject triggerJson = json["trigger"].toObject();
    config.useHardwareTrigger = triggerJson["useHardware"].toBool(false);
    config.triggerSource = triggerJson["source"].toString("Software");

    // 通信配置
    QJsonObject commJson = json["communication"].toObject();
    config.plcAddress = commJson["plcAddress"].toString();
    config.plcPort = commJson["plcPort"].toInt(502);

    return config;
}

bool StationConfig::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QJsonDocument doc(toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

StationConfig StationConfig::loadFromFile(const QString& filePath, bool* ok)
{
    if (ok) *ok = false;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return StationConfig();
    }

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

bool StationConfig::addPositionBinding(const PositionCameraBinding& binding)
{
    // 检查是否已存在
    for (const auto& existing : positionBindings) {
        if (existing.positionId == binding.positionId) {
            return false;
        }
    }

    positionBindings.append(binding);
    positionNum = positionBindings.size();
    return true;
}

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

PositionCameraBinding* StationConfig::getPositionBinding(const QString& positionId)
{
    for (auto& binding : positionBindings) {
        if (binding.positionId == positionId) {
            return &binding;
        }
    }
    return nullptr;
}

const PositionCameraBinding* StationConfig::getPositionBinding(const QString& positionId) const
{
    for (const auto& binding : positionBindings) {
        if (binding.positionId == positionId) {
            return &binding;
        }
    }
    return nullptr;
}

PositionCameraBinding* StationConfig::getPositionBindingByIndex(int index)
{
    if (index >= 0 && index < positionBindings.size()) {
        return &positionBindings[index];
    }
    return nullptr;
}

const PositionCameraBinding* StationConfig::getPositionBindingByIndex(int index) const
{
    if (index >= 0 && index < positionBindings.size()) {
        return &positionBindings[index];
    }
    return nullptr;
}

// ========== 相机关联查询 ==========

QString StationConfig::getCameraIdForPosition(int posIndex) const
{
    const auto* binding = getPositionBindingByIndex(posIndex);
    if (binding) {
        return binding->cameraId;
    }
    return QString();
}

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

// ========== 布局辅助 ==========

StationConfig::LayoutMode StationConfig::getAutoLayout() const
{
    int count = displayMode == ByPosition ? positionNum : cameraNum;

    if (count <= 1) return Layout_1x1;
    if (count <= 2) return Layout_1x2;
    if (count <= 4) return Layout_2x2;
    if (count <= 6) return Layout_2x3;
    return Layout_2x4;
}

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

bool StationConfig::isValid() const
{
    if (id.isEmpty()) return false;
    if (name.isEmpty()) return false;
    if (index < 0 || index > 5) return false;
    if (cameraNum < 1 || cameraNum > 8) return false;
    if (positionNum < 1 || positionNum > 8) return false;

    // 检查每个位置绑定
    for (const auto& binding : positionBindings) {
        if (binding.positionId.isEmpty()) return false;
        if (binding.cameraIndex < 0 || binding.cameraIndex >= cameraNum) return false;
    }

    return true;
}

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
