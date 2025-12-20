/**
 * @file UIModeManager.h
 * @brief UI模式管理器 - 简单模式/专业模式切换
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>

namespace VisionForge {
namespace UI {

/**
 * @brief UI显示模式
 */
enum class UIMode {
    Simple,         // 简单模式 - 隐藏高级功能，适合操作员
    Professional    // 专业模式 - 显示所有功能，适合工程师/管理员
};

/**
 * @brief UI组件分组
 */
enum class UIComponent {
    // 菜单相关
    MenuCalibration,        // 标定菜单
    MenuAdvancedTools,      // 高级工具
    MenuSettings,           // 设置菜单
    MenuUserManagement,     // 用户管理

    // 面板相关
    PanelHistory,           // 历史记录面板
    PanelStatistics,        // 统计面板
    PanelRecipe,            // 方案管理面板
    PanelToolParameter,     // 工具参数面板（高级参数）

    // 工具栏相关
    ToolbarROI,             // ROI工具栏
    ToolbarAdvanced,        // 高级工具栏

    // 状态栏相关
    StatusSystemMonitor,    // 系统监控信息

    // 工具相关
    ToolsPreprocessing,     // 预处理工具
    ToolsDetection,         // 检测工具
    ToolsMeasurement,       // 测量工具
    ToolsJudgment,          // 判定工具
    ToolsOutput,            // 输出工具
    ToolsAdvanced           // 高级工具（形状匹配、AI等）
};

/**
 * @class UIModeManager
 * @brief UI模式管理器（单例）
 *
 * 功能：
 * - 管理简单/专业模式
 * - 控制组件可见性
 * - 保存/加载模式配置
 */
class UIModeManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static UIModeManager& instance();

    /**
     * @brief 获取当前模式
     */
    UIMode currentMode() const { return currentMode_; }

    /**
     * @brief 设置UI模式
     */
    void setMode(UIMode mode);

    /**
     * @brief 切换模式
     */
    void toggleMode();

    /**
     * @brief 检查组件在当前模式下是否可见
     * @param component 组件类型
     * @return true如果可见
     */
    bool isComponentVisible(UIComponent component) const;

    /**
     * @brief 设置组件可见性（覆盖默认设置）
     * @param component 组件类型
     * @param visible 是否可见
     */
    void setComponentVisibility(UIComponent component, bool visible);

    /**
     * @brief 重置为默认可见性
     */
    void resetToDefaults();

    /**
     * @brief 获取模式名称
     */
    static QString modeName(UIMode mode);

    /**
     * @brief 获取组件名称
     */
    static QString componentName(UIComponent component);

    /**
     * @brief 保存配置
     */
    void saveConfig();

    /**
     * @brief 加载配置
     */
    void loadConfig();

    /**
     * @brief 获取简单模式下可见的工具类型列表
     */
    QStringList getSimpleModeToolTypes() const;

    /**
     * @brief 获取专业模式下可见的工具类型列表
     */
    QStringList getProfessionalModeToolTypes() const;

signals:
    /**
     * @brief 模式变更信号
     */
    void modeChanged(UIMode newMode);

    /**
     * @brief 组件可见性变更信号
     */
    void componentVisibilityChanged(UIComponent component, bool visible);

private:
    UIModeManager();
    ~UIModeManager() = default;
    UIModeManager(const UIModeManager&) = delete;
    UIModeManager& operator=(const UIModeManager&) = delete;

    void initializeDefaultVisibility();

private:
    UIMode currentMode_;
    QMap<UIComponent, bool> simpleVisibility_;      // 简单模式可见性
    QMap<UIComponent, bool> professionalVisibility_; // 专业模式可见性
    QMap<UIComponent, bool> customVisibility_;      // 用户自定义可见性
    bool useCustomVisibility_;
};

} // namespace UI
} // namespace VisionForge

// 便捷宏
#define UI_MODE VisionForge::UI::UIModeManager::instance()
#define IS_SIMPLE_MODE (VisionForge::UI::UIModeManager::instance().currentMode() == VisionForge::UI::UIMode::Simple)
#define IS_PROFESSIONAL_MODE (VisionForge::UI::UIModeManager::instance().currentMode() == VisionForge::UI::UIMode::Professional)
