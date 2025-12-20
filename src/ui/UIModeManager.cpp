/**
 * @file UIModeManager.cpp
 * @brief UI模式管理器实现
 */

#include "ui/UIModeManager.h"
#include "base/ConfigManager.h"
#include "base/Logger.h"

#include <QMap>

namespace VisionForge {
namespace UI {

UIModeManager& UIModeManager::instance()
{
    static UIModeManager instance;
    return instance;
}

UIModeManager::UIModeManager()
    : QObject(nullptr)
    , currentMode_(UIMode::Professional)  // 默认专业模式
    , useCustomVisibility_(false)
{
    initializeDefaultVisibility();
    loadConfig();
}

void UIModeManager::initializeDefaultVisibility()
{
    // 简单模式 - 隐藏高级功能
    simpleVisibility_[UIComponent::MenuCalibration] = false;
    simpleVisibility_[UIComponent::MenuAdvancedTools] = false;
    simpleVisibility_[UIComponent::MenuSettings] = false;
    simpleVisibility_[UIComponent::MenuUserManagement] = false;

    simpleVisibility_[UIComponent::PanelHistory] = false;
    simpleVisibility_[UIComponent::PanelStatistics] = false;
    simpleVisibility_[UIComponent::PanelRecipe] = false;
    simpleVisibility_[UIComponent::PanelToolParameter] = true;  // 基本参数可见

    simpleVisibility_[UIComponent::ToolbarROI] = true;
    simpleVisibility_[UIComponent::ToolbarAdvanced] = false;

    simpleVisibility_[UIComponent::StatusSystemMonitor] = false;

    simpleVisibility_[UIComponent::ToolsPreprocessing] = true;
    simpleVisibility_[UIComponent::ToolsDetection] = true;
    simpleVisibility_[UIComponent::ToolsMeasurement] = true;
    simpleVisibility_[UIComponent::ToolsJudgment] = true;
    simpleVisibility_[UIComponent::ToolsOutput] = true;
    simpleVisibility_[UIComponent::ToolsAdvanced] = false;

    // 专业模式 - 显示所有功能
    professionalVisibility_[UIComponent::MenuCalibration] = true;
    professionalVisibility_[UIComponent::MenuAdvancedTools] = true;
    professionalVisibility_[UIComponent::MenuSettings] = true;
    professionalVisibility_[UIComponent::MenuUserManagement] = true;

    professionalVisibility_[UIComponent::PanelHistory] = true;
    professionalVisibility_[UIComponent::PanelStatistics] = true;
    professionalVisibility_[UIComponent::PanelRecipe] = true;
    professionalVisibility_[UIComponent::PanelToolParameter] = true;

    professionalVisibility_[UIComponent::ToolbarROI] = true;
    professionalVisibility_[UIComponent::ToolbarAdvanced] = true;

    professionalVisibility_[UIComponent::StatusSystemMonitor] = true;

    professionalVisibility_[UIComponent::ToolsPreprocessing] = true;
    professionalVisibility_[UIComponent::ToolsDetection] = true;
    professionalVisibility_[UIComponent::ToolsMeasurement] = true;
    professionalVisibility_[UIComponent::ToolsJudgment] = true;
    professionalVisibility_[UIComponent::ToolsOutput] = true;
    professionalVisibility_[UIComponent::ToolsAdvanced] = true;
}

void UIModeManager::setMode(UIMode mode)
{
    if (currentMode_ != mode) {
        currentMode_ = mode;
        LOG_INFO(QString("UI模式切换: %1").arg(modeName(mode)));
        saveConfig();
        emit modeChanged(mode);
    }
}

void UIModeManager::toggleMode()
{
    if (currentMode_ == UIMode::Simple) {
        setMode(UIMode::Professional);
    } else {
        setMode(UIMode::Simple);
    }
}

bool UIModeManager::isComponentVisible(UIComponent component) const
{
    // 如果使用自定义可见性，优先返回自定义设置
    if (useCustomVisibility_ && customVisibility_.contains(component)) {
        return customVisibility_[component];
    }

    // 否则根据当前模式返回默认可见性
    if (currentMode_ == UIMode::Simple) {
        return simpleVisibility_.value(component, false);
    } else {
        return professionalVisibility_.value(component, true);
    }
}

void UIModeManager::setComponentVisibility(UIComponent component, bool visible)
{
    useCustomVisibility_ = true;
    customVisibility_[component] = visible;
    emit componentVisibilityChanged(component, visible);
}

void UIModeManager::resetToDefaults()
{
    useCustomVisibility_ = false;
    customVisibility_.clear();
    emit modeChanged(currentMode_);
}

QString UIModeManager::modeName(UIMode mode)
{
    switch (mode) {
        case UIMode::Simple: return QObject::tr("简单模式");
        case UIMode::Professional: return QObject::tr("专业模式");
        default: return QObject::tr("未知模式");
    }
}

QString UIModeManager::componentName(UIComponent component)
{
    switch (component) {
        case UIComponent::MenuCalibration: return QObject::tr("标定菜单");
        case UIComponent::MenuAdvancedTools: return QObject::tr("高级工具");
        case UIComponent::MenuSettings: return QObject::tr("设置菜单");
        case UIComponent::MenuUserManagement: return QObject::tr("用户管理");
        case UIComponent::PanelHistory: return QObject::tr("历史记录面板");
        case UIComponent::PanelStatistics: return QObject::tr("统计面板");
        case UIComponent::PanelRecipe: return QObject::tr("方案管理面板");
        case UIComponent::PanelToolParameter: return QObject::tr("工具参数面板");
        case UIComponent::ToolbarROI: return QObject::tr("ROI工具栏");
        case UIComponent::ToolbarAdvanced: return QObject::tr("高级工具栏");
        case UIComponent::StatusSystemMonitor: return QObject::tr("系统监控");
        case UIComponent::ToolsPreprocessing: return QObject::tr("预处理工具");
        case UIComponent::ToolsDetection: return QObject::tr("检测工具");
        case UIComponent::ToolsMeasurement: return QObject::tr("测量工具");
        case UIComponent::ToolsJudgment: return QObject::tr("判定工具");
        case UIComponent::ToolsOutput: return QObject::tr("输出工具");
        case UIComponent::ToolsAdvanced: return QObject::tr("高级工具");
        default: return QObject::tr("未知组件");
    }
}

void UIModeManager::saveConfig()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();
    config.setValue("UI/Mode", static_cast<int>(currentMode_));
    config.setValue("UI/UseCustomVisibility", useCustomVisibility_);
}

void UIModeManager::loadConfig()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();
    int modeValue = config.getValue("UI/Mode", static_cast<int>(UIMode::Professional)).toInt();
    currentMode_ = static_cast<UIMode>(modeValue);
    useCustomVisibility_ = config.getValue("UI/UseCustomVisibility", false).toBool();
}

QStringList UIModeManager::getSimpleModeToolTypes() const
{
    // 简单模式下显示的工具类型
    QStringList tools;

    // 预处理工具
    tools << "GrayTool" << "BlurTool" << "ThresholdTool";

    // 检测工具
    tools << "TemplateMatchTool" << "CircleTool" << "LineTool" << "BlobTool";

    // 测量工具
    tools << "MeasureDistanceTool" << "MeasureAngleTool" << "MeasureAreaTool";

    // 判定工具
    tools << "RangeJudgeTool";

    // 输出工具
    tools << "SaveImageTool" << "PLCOutputTool";

    return tools;
}

QStringList UIModeManager::getProfessionalModeToolTypes() const
{
    // 专业模式下显示所有工具
    QStringList tools;

    // 预处理工具
    tools << "GrayTool" << "BlurTool" << "ThresholdTool"
          << "ColorConvertTool" << "EdgeTool" << "MorphologyTool";

    // 检测工具
    tools << "TemplateMatchTool" << "CircleTool" << "LineTool"
          << "FindEdgeTool" << "BlobTool" << "AIDetectionTool";

    // 测量工具
    tools << "MeasureDistanceTool" << "MeasureAngleTool" << "MeasureAreaTool"
          << "CalcCenterTool" << "CalcOrientationTool";

    // 判定工具
    tools << "RangeJudgeTool" << "LogicOperationTool";

    // 输出工具
    tools << "SaveImageTool" << "PLCOutputTool" << "ROITool";

#ifdef USE_HALCON
    // Halcon高级工具
    tools << "ShapeMatchTool";
#endif

    return tools;
}

} // namespace UI
} // namespace VisionForge
