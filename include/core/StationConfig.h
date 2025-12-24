/**
 * @file StationConfig.h
 * @brief 工位配置结构定义
 *
 * 借鉴 VisionASM SysPlatformInfo 设计，集中管理工位的所有配置
 */

#ifndef VISIONFORGE_CORE_STATION_CONFIG_H
#define VISIONFORGE_CORE_STATION_CONFIG_H

#include "core/PositionCameraBinding.h"
#include "core/SceneConfig.h"
#include "platform/PlatformTypes.h"
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

namespace VisionForge {

// 前向声明
namespace HAL {
class ICamera;
}

namespace Core {

/**
 * @brief 工位完整配置
 *
 * 集中管理一个工位的所有配置，包括：
 * - 基本信息（ID、名称、索引）
 * - 平台配置（类型、行程）
 * - 相机配置（数量、ID列表、共享设置）
 * - 位置配置（位置-相机绑定列表）
 * - 显示配置（模式、布局）
 * - 触发配置（硬触发、触发源）
 * - 通信配置（PLC地址）
 */
struct StationConfig {
    // ========== 基本信息 ==========
    QString id;                 ///< 工位ID (如 "WS0", "WS1")
    QString name;               ///< 工位名称 (如 "工位1-上料对位")
    int index = 0;              ///< 工位索引 (0-5)
    bool enabled = true;        ///< 是否启用

    // ========== 平台配置 ==========
    Platform::PlatformType platformType = Platform::PlatformType::XYD;
    QString platformConfigId;   ///< 关联的平台配置ID
    double xRange = 500.0;      ///< X轴行程 (mm)
    double yRange = 500.0;      ///< Y轴行程 (mm)
    double dRange = 360.0;      ///< 旋转行程 (度)

    // ========== 相机配置 ==========
    int cameraNum = 2;          ///< 相机数量 (1-8)
    QVector<QString> cameraIds; ///< 相机ID列表

    // 相机共享配置
    bool shareCameraSystem = false;     ///< 是否与其他工位共享相机
    QString sharedFromStationId;        ///< 共享源工位ID

    // ========== 位置配置 ==========
    int positionNum = 2;        ///< 位置数量 (1-8)
    PositionBindingList positionBindings; ///< 位置-相机绑定列表

    // ========== 场景配置 ==========
    int sceneNum = 1;           ///< 场景数量 (1-8)
    SceneConfigList scenes;     ///< 场景配置列表
    int currentSceneIndex = 0;  ///< 当前激活的场景索引

    // ========== 功能模式配置 ==========
    /**
     * @brief 功能模式枚举
     * 用于区分工位的主要用途：仅检测、仅对位、或检测+对位
     */
    enum FunctionMode {
        DetectionOnly = 0,          ///< 仅检测模式 - 只进行视觉检测，不进行对位
        AlignmentOnly = 1,          ///< 仅对位模式 - 只进行对位校正，不进行检测
        DetectionAndAlignment = 2   ///< 检测+对位模式 - 先检测后对位（默认）
    };
    FunctionMode functionMode = DetectionAndAlignment;  ///< 当前功能模式

    // ========== 显示配置 ==========
    enum DisplayMode {
        ByPosition = 0,     ///< 按位置显示 (每个位置一个窗口)
        ByCamera = 1        ///< 按相机显示 (每个相机一个窗口)
    };
    DisplayMode displayMode = ByPosition;

    enum LayoutMode {
        Layout_Auto = 0,    ///< 自动布局
        Layout_1x1 = 1,     ///< 单画面
        Layout_1x2 = 2,     ///< 1行2列
        Layout_2x2 = 4,     ///< 2行2列
        Layout_2x3 = 6,     ///< 2行3列
        Layout_2x4 = 8      ///< 2行4列
    };
    LayoutMode displayLayout = Layout_Auto;

    // ========== 触发配置 ==========
    bool useHardwareTrigger = false;    ///< 是否使用硬件触发
    QString triggerSource;              ///< 触发源 ("GPIO" / "PLC" / "Software")

    // ========== 通信配置 ==========
    QString plcAddress;         ///< PLC地址
    int plcPort = 502;          ///< PLC端口 (Modbus默认502)

    // ========== 构造函数 ==========
    StationConfig() = default;

    StationConfig(const QString& stationId, const QString& stationName, int stationIndex = 0)
        : id(stationId)
        , name(stationName)
        , index(stationIndex)
    {}

    // ========== 序列化 ==========

    /**
     * @brief 转换为JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象创建
     */
    static StationConfig fromJson(const QJsonObject& json);

    /**
     * @brief 保存到文件
     */
    bool saveToFile(const QString& filePath) const;

    /**
     * @brief 从文件加载
     */
    static StationConfig loadFromFile(const QString& filePath, bool* ok = nullptr);

    // ========== 位置管理 ==========

    /**
     * @brief 添加位置绑定
     */
    bool addPositionBinding(const PositionCameraBinding& binding);

    /**
     * @brief 移除位置绑定
     */
    bool removePositionBinding(const QString& positionId);

    /**
     * @brief 更新位置绑定
     */
    bool updatePositionBinding(const PositionCameraBinding& binding);

    /**
     * @brief 获取位置绑定
     */
    PositionCameraBinding* getPositionBinding(const QString& positionId);
    const PositionCameraBinding* getPositionBinding(const QString& positionId) const;

    /**
     * @brief 根据索引获取位置绑定
     */
    PositionCameraBinding* getPositionBindingByIndex(int index);
    const PositionCameraBinding* getPositionBindingByIndex(int index) const;

    // ========== 相机关联查询 ==========

    /**
     * @brief 获取指定位置使用的相机ID
     */
    QString getCameraIdForPosition(int posIndex) const;

    /**
     * @brief 获取指定相机服务的所有位置索引
     */
    QVector<int> getPositionsForCamera(const QString& cameraId) const;

    /**
     * @brief 获取所有目标位置
     */
    PositionBindingList getTargetPositions() const;

    /**
     * @brief 获取所有对象位置
     */
    PositionBindingList getObjectPositions() const;

    // ========== 场景管理 ==========

    /**
     * @brief 添加场景配置
     */
    bool addScene(const SceneConfig& scene);

    /**
     * @brief 移除场景配置
     */
    bool removeScene(const QString& sceneId);

    /**
     * @brief 更新场景配置
     */
    bool updateScene(const SceneConfig& scene);

    /**
     * @brief 获取场景配置
     */
    SceneConfig* getScene(const QString& sceneId);
    const SceneConfig* getScene(const QString& sceneId) const;

    /**
     * @brief 根据索引获取场景配置
     */
    SceneConfig* getSceneByIndex(int index);
    const SceneConfig* getSceneByIndex(int index) const;

    /**
     * @brief 获取当前激活的场景
     */
    SceneConfig* getCurrentScene();
    const SceneConfig* getCurrentScene() const;

    /**
     * @brief 切换到指定场景
     * @return 是否切换成功
     */
    bool switchToScene(int sceneIndex);
    bool switchToScene(const QString& sceneId);

    // ========== 布局辅助 ==========

    /**
     * @brief 根据位置数量自动选择布局
     */
    LayoutMode getAutoLayout() const;

    /**
     * @brief 获取实际使用的布局
     */
    LayoutMode getEffectiveLayout() const {
        return displayLayout == Layout_Auto ? getAutoLayout() : displayLayout;
    }

    /**
     * @brief 获取布局的行列数
     */
    void getLayoutDimensions(int& rows, int& cols) const;

    // ========== 验证 ==========

    /**
     * @brief 验证配置是否有效
     */
    bool isValid() const;

    /**
     * @brief 获取验证错误信息
     */
    QString getValidationError() const;
};

/**
 * @brief 工位配置列表类型
 */
using StationConfigList = QVector<StationConfig>;

} // namespace Core
} // namespace VisionForge

#endif // VISIONFORGE_CORE_STATION_CONFIG_H
