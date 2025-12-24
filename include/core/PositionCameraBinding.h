/**
 * @file PositionCameraBinding.h
 * @brief 位置-相机绑定结构定义
 *
 * 借鉴 VisionASM CPosInfo 设计，简洁高效地将位置与相机关联
 */

#ifndef VISIONFORGE_CORE_POSITION_CAMERA_BINDING_H
#define VISIONFORGE_CORE_POSITION_CAMERA_BINDING_H

#include <QString>
#include <QJsonObject>
#include <QVector>

namespace VisionForge {
namespace Core {

/**
 * @brief 位置-相机绑定信息
 *
 * 用于描述一个拍照位置与相机的关联关系。
 * 支持一个相机服务多个位置（如上下两个拍照点共用一个相机）。
 */
struct PositionCameraBinding {
    // ========== 基本信息 ==========
    QString positionId;         ///< 位置ID (如 "pos0", "pos1")
    QString positionName;       ///< 位置名称 (如 "Pos0-Target上", "Pos1-Object下")
    bool enabled = true;        ///< 是否启用该位置

    // ========== 相机关联 ==========
    int cameraIndex = 0;        ///< 相机索引 (0-7)
    QString cameraId;           ///< 相机ID (CameraManager中的唯一ID)

    // ========== 显示配置 ==========
    int displayIndex = -1;      ///< 在多画面显示中的位置 (-1=自动分配)

    // ========== 采集配置 ==========
    int captureOrder = 0;       ///< 顺序采集时的顺序号
    bool useHardwareTrigger = false; ///< 是否使用硬件触发

    // ========== 工具链配置 ==========
    QString toolChainFile;      ///< 工具链配置文件路径 (相对于工位目录)
    QString recipeFile;         ///< 配方文件路径 (相对于工位目录)

    // ========== 检测功能配置 ==========
    bool enableDetection = true;            ///< 是否启用检测功能
    QString detectionModelPath;             ///< 检测模型路径
    float detectionConfidenceThreshold = 0.5f;  ///< 检测置信度阈值
    float detectionNmsThreshold = 0.4f;     ///< 检测NMS阈值

    // ========== 对位功能配置 ==========
    bool enableAlignment = true;            ///< 是否启用对位功能
    double alignmentTargetX = 0.0;          ///< 对位目标X坐标
    double alignmentTargetY = 0.0;          ///< 对位目标Y坐标
    double alignmentTargetD = 0.0;          ///< 对位目标角度
    double alignmentToleranceX = 0.1;       ///< 对位X方向容差 (mm)
    double alignmentToleranceY = 0.1;       ///< 对位Y方向容差 (mm)
    double alignmentToleranceD = 0.1;       ///< 对位角度容差 (度)

    // ========== 平台坐标 (拍照位置) ==========
    double platX = 0.0;         ///< 平台X坐标
    double platY = 0.0;         ///< 平台Y坐标
    double platD = 0.0;         ///< 平台旋转角度

    // ========== 位置类型 ==========
    enum PositionType {
        Target = 0,     ///< 目标位置（基准）
        Object = 1      ///< 对象位置（待对位）
    };
    PositionType positionType = Target;

    // ========== 构造函数 ==========
    PositionCameraBinding() = default;

    PositionCameraBinding(const QString& id, const QString& name, int camIndex = 0)
        : positionId(id)
        , positionName(name)
        , cameraIndex(camIndex)
    {}

    // ========== 序列化 ==========

    /**
     * @brief 转换为JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象创建
     */
    static PositionCameraBinding fromJson(const QJsonObject& json);

    // ========== 辅助方法 ==========

    /**
     * @brief 是否为目标位置
     */
    bool isTarget() const { return positionType == Target; }

    /**
     * @brief 是否为对象位置
     */
    bool isObject() const { return positionType == Object; }

    /**
     * @brief 获取位置类型字符串
     */
    QString positionTypeString() const {
        return positionType == Target ? "Target" : "Object";
    }

    /**
     * @brief 比较两个绑定是否相同（基于ID）
     */
    bool operator==(const PositionCameraBinding& other) const {
        return positionId == other.positionId;
    }
};

/**
 * @brief 位置绑定列表类型
 */
using PositionBindingList = QVector<PositionCameraBinding>;

} // namespace Core
} // namespace VisionForge

#endif // VISIONFORGE_CORE_POSITION_CAMERA_BINDING_H
