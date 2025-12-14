/**
 * @file ToolParameterPanel.h
 * @brief 工具参数面板
 * @details 显示和编辑当前选中工具的参数
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

namespace VisionForge {
namespace UI {

/**
 * @class ToolParameterPanel
 * @brief 工具参数面板
 *
 * 功能：
 * - 显示当前工具的所有参数
 * - 支持参数编辑
 * - 参数改变时通知工具
 * - 支持不同类型的工具参数
 */
class ToolParameterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ToolParameterPanel(QWidget* parent = nullptr);
    ~ToolParameterPanel() override = default;

    /**
     * @brief 设置当前工具
     * @param tool 工具实例
     */
    void setTool(Algorithm::VisionTool* tool);

    /**
     * @brief 清空面板
     */
    void clear();

signals:
    /**
     * @brief 参数改变信号
     */
    void parameterChanged();

private slots:
    void onParameterChanged();

private:
    /**
     * @brief 创建通用参数界面
     */
    void createCommonParameters();

    /**
     * @brief 创建灰度工具参数界面
     */
    void createGrayToolParameters();

    /**
     * @brief 更新工具参数（从UI到工具）
     */
    void updateToolParameters();

    /**
     * @brief 刷新UI（从工具到UI）
     */
    void refreshUI();

private:
    Algorithm::VisionTool* currentTool_;    // 当前工具

    // 主布局
    QVBoxLayout* mainLayout_;

    // 通用参数组
    QGroupBox* commonGroup_;
    QCheckBox* enabledCheckBox_;
    QLineEdit* displayNameEdit_;

    // 工具特定参数组
    QGroupBox* specificGroup_;
    QFormLayout* specificLayout_;

    // GrayTool参数（示例）
    QComboBox* convertModeCombo_;
    QComboBox* channelCombo_;
};

} // namespace UI
} // namespace VisionForge
