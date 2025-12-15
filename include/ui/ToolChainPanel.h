/**
 * @file ToolChainPanel.h
 * @brief 工具链面板
 * @details 显示和管理工具链中的所有工具
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QWidget>
#include <QListWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QList>

namespace VisionForge {
namespace UI {

/**
 * @class ToolChainPanel
 * @brief 工具链面板
 *
 * 功能：
 * - 显示工具链中的所有工具
 * - 支持添加/删除工具
 * - 支持选择工具
 * - 支持启用/禁用工具
 * - 显示工具运行状态
 * - 显示工具执行时间统计
 */
class ToolChainPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ToolChainPanel(QWidget* parent = nullptr);
    ~ToolChainPanel() override = default;

    /**
     * @brief 添加工具到工具链
     * @param tool 工具实例
     */
    void addTool(Algorithm::VisionTool* tool);

    /**
     * @brief 移除工具
     * @param tool 工具实例
     */
    void removeTool(Algorithm::VisionTool* tool);

    /**
     * @brief 更新工具执行时间
     * @param tool 工具实例
     * @param executionTime 执行时间(毫秒)
     */
    void updateToolExecutionTime(Algorithm::VisionTool* tool, double executionTime);

    /**
     * @brief 清空工具链
     */
    void clear();

    /**
     * @brief 获取所有工具
     */
    QList<Algorithm::VisionTool*> getTools() const { return tools_; }

    /**
     * @brief 获取当前选中的工具
     */
    Algorithm::VisionTool* getCurrentTool() const;

    /**
     * @brief 设置当前工具
     */
    void setCurrentTool(Algorithm::VisionTool* tool);

signals:
    /**
     * @brief 工具选择改变信号
     * @param tool 当前选中的工具
     */
    void toolSelectionChanged(Algorithm::VisionTool* tool);

    /**
     * @brief 工具双击信号（用于弹出参数编辑对话框）
     * @param tool 双击的工具
     */
    void toolDoubleClicked(Algorithm::VisionTool* tool);

    /**
     * @brief 请求添加工具信号
     */
    void addToolRequested();

    /**
     * @brief 请求删除工具信号
     * @param tool 要删除的工具
     */
    void removeToolRequested(Algorithm::VisionTool* tool);

private slots:
    void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void onAddTool();
    void onRemoveTool();
    void onMoveUp();
    void onMoveDown();
    void onToolParamChanged();

private:
    void updateToolList();
    void updateActions();

private:
    QVBoxLayout* mainLayout_;
    QToolBar* toolBar_;
    QListWidget* toolList_;

    QAction* addAction_;
    QAction* removeAction_;
    QAction* moveUpAction_;
    QAction* moveDownAction_;

    QList<Algorithm::VisionTool*> tools_;
    QMap<Algorithm::VisionTool*, double> toolExecutionTimes_;  // 工具执行时间统计
};

} // namespace UI
} // namespace VisionForge
