/**
 * @file PositionCameraBinding.cpp
 * @brief 位置-相机绑定结构实现
 */

#include "core/PositionCameraBinding.h"
#include <QJsonArray>

namespace VisionForge {
namespace Core {

QJsonObject PositionCameraBinding::toJson() const
{
    QJsonObject json;

    // 基本信息
    json["positionId"] = positionId;
    json["positionName"] = positionName;
    json["enabled"] = enabled;

    // 相机关联
    json["cameraIndex"] = cameraIndex;
    json["cameraId"] = cameraId;

    // 显示配置
    json["displayIndex"] = displayIndex;

    // 采集配置
    json["captureOrder"] = captureOrder;
    json["useHardwareTrigger"] = useHardwareTrigger;

    // 工具链配置
    json["toolChainFile"] = toolChainFile;
    json["recipeFile"] = recipeFile;

    // 检测功能配置
    json["enableDetection"] = enableDetection;
    json["detectionModelPath"] = detectionModelPath;
    json["detectionConfidenceThreshold"] = static_cast<double>(detectionConfidenceThreshold);
    json["detectionNmsThreshold"] = static_cast<double>(detectionNmsThreshold);

    // 对位功能配置
    json["enableAlignment"] = enableAlignment;
    json["alignmentTargetX"] = alignmentTargetX;
    json["alignmentTargetY"] = alignmentTargetY;
    json["alignmentTargetD"] = alignmentTargetD;
    json["alignmentToleranceX"] = alignmentToleranceX;
    json["alignmentToleranceY"] = alignmentToleranceY;
    json["alignmentToleranceD"] = alignmentToleranceD;

    // 平台坐标
    json["platX"] = platX;
    json["platY"] = platY;
    json["platD"] = platD;

    // 位置类型
    json["positionType"] = static_cast<int>(positionType);

    return json;
}

PositionCameraBinding PositionCameraBinding::fromJson(const QJsonObject& json)
{
    PositionCameraBinding binding;

    // 基本信息
    binding.positionId = json["positionId"].toString();
    binding.positionName = json["positionName"].toString();
    binding.enabled = json["enabled"].toBool(true);

    // 相机关联
    binding.cameraIndex = json["cameraIndex"].toInt(0);
    binding.cameraId = json["cameraId"].toString();

    // 显示配置
    binding.displayIndex = json["displayIndex"].toInt(-1);

    // 采集配置
    binding.captureOrder = json["captureOrder"].toInt(0);
    binding.useHardwareTrigger = json["useHardwareTrigger"].toBool(false);

    // 工具链配置
    binding.toolChainFile = json["toolChainFile"].toString();
    binding.recipeFile = json["recipeFile"].toString();

    // 检测功能配置
    binding.enableDetection = json["enableDetection"].toBool(true);
    binding.detectionModelPath = json["detectionModelPath"].toString();
    binding.detectionConfidenceThreshold = static_cast<float>(json["detectionConfidenceThreshold"].toDouble(0.5));
    binding.detectionNmsThreshold = static_cast<float>(json["detectionNmsThreshold"].toDouble(0.4));

    // 对位功能配置
    binding.enableAlignment = json["enableAlignment"].toBool(true);
    binding.alignmentTargetX = json["alignmentTargetX"].toDouble(0.0);
    binding.alignmentTargetY = json["alignmentTargetY"].toDouble(0.0);
    binding.alignmentTargetD = json["alignmentTargetD"].toDouble(0.0);
    binding.alignmentToleranceX = json["alignmentToleranceX"].toDouble(0.1);
    binding.alignmentToleranceY = json["alignmentToleranceY"].toDouble(0.1);
    binding.alignmentToleranceD = json["alignmentToleranceD"].toDouble(0.1);

    // 平台坐标
    binding.platX = json["platX"].toDouble(0.0);
    binding.platY = json["platY"].toDouble(0.0);
    binding.platD = json["platD"].toDouble(0.0);

    // 位置类型
    binding.positionType = static_cast<PositionType>(json["positionType"].toInt(0));

    return binding;
}

} // namespace Core
} // namespace VisionForge
