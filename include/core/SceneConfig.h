/**
 * @file SceneConfig.h
 * @brief 场景配置结构定义
 *
 * 场景用于在同一工位支持多种检测配置，如：
 * - 不同产品型号的检测
 * - 同一产品的正面/背面检测
 * - 不同检测标准的切换
 */

#ifndef VISIONFORGE_CORE_SCENE_CONFIG_H
#define VISIONFORGE_CORE_SCENE_CONFIG_H

#include <QString>
#include <QJsonObject>
#include <QVector>
#include <QMap>

namespace VisionForge {
namespace Core {

/**
 * @brief 场景配置结构
 *
 * 每个场景包含独立的检测参数和工具链配置
 */
struct SceneConfig {
    // ========== 基本信息 ==========
    QString sceneId;            ///< 场景ID (如 "scene0", "scene1")
    QString sceneName;          ///< 场景名称 (如 "正面检测", "背面检测")
    int sceneIndex = 0;         ///< 场景索引 (0-7)
    bool enabled = true;        ///< 是否启用该场景

    // ========== 检测配置 ==========
    QString detectionModelPath;     ///< 该场景使用的检测模型路径
    float confidenceThreshold = 0.5f;   ///< 置信度阈值
    float nmsThreshold = 0.4f;          ///< NMS阈值

    // ========== 模板配置 ==========
    QString templateImagePath;      ///< 模板图像路径
    QString templateConfigPath;     ///< 模板配置文件路径

    // ========== 工具链配置 ==========
    QString toolChainFile;          ///< 该场景的工具链配置文件
    QString recipeFile;             ///< 该场景的配方文件

    // ========== 判定标准 ==========
    int minDetectionCount = 0;      ///< 最少检测数量
    int maxDetectionCount = 100;    ///< 最多检测数量
    double minScore = 0.0;          ///< 最低得分
    double maxScore = 100.0;        ///< 最高得分

    // ========== 位置参数覆盖 ==========
    /// 每个位置的场景特定参数 (positionId -> 参数JSON)
    QMap<QString, QJsonObject> positionOverrides;

    // ========== 构造函数 ==========
    SceneConfig() = default;

    SceneConfig(const QString& id, const QString& name, int index = 0)
        : sceneId(id)
        , sceneName(name)
        , sceneIndex(index)
    {}

    // ========== 序列化 ==========

    /**
     * @brief 转换为JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象创建
     */
    static SceneConfig fromJson(const QJsonObject& json);

    // ========== 辅助方法 ==========

    /**
     * @brief 获取位置的场景特定参数
     */
    QJsonObject getPositionOverride(const QString& positionId) const {
        return positionOverrides.value(positionId, QJsonObject());
    }

    /**
     * @brief 设置位置的场景特定参数
     */
    void setPositionOverride(const QString& positionId, const QJsonObject& params) {
        positionOverrides[positionId] = params;
    }

    /**
     * @brief 比较两个场景是否相同（基于ID）
     */
    bool operator==(const SceneConfig& other) const {
        return sceneId == other.sceneId;
    }
};

/**
 * @brief 场景配置列表类型
 */
using SceneConfigList = QVector<SceneConfig>;

} // namespace Core
} // namespace VisionForge

#endif // VISIONFORGE_CORE_SCENE_CONFIG_H
