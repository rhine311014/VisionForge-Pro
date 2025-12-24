/**
 * @file SceneConfig.cpp
 * @brief 场景配置结构实现
 */

#include "core/SceneConfig.h"
#include <QJsonArray>

namespace VisionForge {
namespace Core {

QJsonObject SceneConfig::toJson() const
{
    QJsonObject json;

    // 基本信息
    json["sceneId"] = sceneId;
    json["sceneName"] = sceneName;
    json["sceneIndex"] = sceneIndex;
    json["enabled"] = enabled;

    // 检测配置
    json["detectionModelPath"] = detectionModelPath;
    json["confidenceThreshold"] = static_cast<double>(confidenceThreshold);
    json["nmsThreshold"] = static_cast<double>(nmsThreshold);

    // 模板配置
    json["templateImagePath"] = templateImagePath;
    json["templateConfigPath"] = templateConfigPath;

    // 工具链配置
    json["toolChainFile"] = toolChainFile;
    json["recipeFile"] = recipeFile;

    // 判定标准
    json["minDetectionCount"] = minDetectionCount;
    json["maxDetectionCount"] = maxDetectionCount;
    json["minScore"] = minScore;
    json["maxScore"] = maxScore;

    // 位置参数覆盖
    QJsonObject overridesJson;
    for (auto it = positionOverrides.begin(); it != positionOverrides.end(); ++it) {
        overridesJson[it.key()] = it.value();
    }
    json["positionOverrides"] = overridesJson;

    return json;
}

SceneConfig SceneConfig::fromJson(const QJsonObject& json)
{
    SceneConfig config;

    // 基本信息
    config.sceneId = json["sceneId"].toString();
    config.sceneName = json["sceneName"].toString();
    config.sceneIndex = json["sceneIndex"].toInt(0);
    config.enabled = json["enabled"].toBool(true);

    // 检测配置
    config.detectionModelPath = json["detectionModelPath"].toString();
    config.confidenceThreshold = static_cast<float>(json["confidenceThreshold"].toDouble(0.5));
    config.nmsThreshold = static_cast<float>(json["nmsThreshold"].toDouble(0.4));

    // 模板配置
    config.templateImagePath = json["templateImagePath"].toString();
    config.templateConfigPath = json["templateConfigPath"].toString();

    // 工具链配置
    config.toolChainFile = json["toolChainFile"].toString();
    config.recipeFile = json["recipeFile"].toString();

    // 判定标准
    config.minDetectionCount = json["minDetectionCount"].toInt(0);
    config.maxDetectionCount = json["maxDetectionCount"].toInt(100);
    config.minScore = json["minScore"].toDouble(0.0);
    config.maxScore = json["maxScore"].toDouble(100.0);

    // 位置参数覆盖
    QJsonObject overridesJson = json["positionOverrides"].toObject();
    for (auto it = overridesJson.begin(); it != overridesJson.end(); ++it) {
        config.positionOverrides[it.key()] = it.value().toObject();
    }

    return config;
}

} // namespace Core
} // namespace VisionForge
