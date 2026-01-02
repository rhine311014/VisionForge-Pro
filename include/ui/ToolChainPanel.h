/**
 * @file ToolChainPanel.h
 * @brief 工具链面板类定义
 * @details 工具链面板是视觉检测系统的核心配置界面，用于管理和显示检测流程中的所有视觉工具。
 *          支持工具的添加、删除、排序、启用/禁用等操作，并实时显示工具执行状态和时间。
 *          工具链定义了图像处理的执行顺序，是视觉检测流程的核心配置。
 *
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-12-14
 *
 * @note 工具链面板设计遵循以下原则：
 *       1. 直观的列表显示，清晰展示工具执行顺序
 *       2. 支持拖拽排序，便于调整工具顺序
 *       3. 工具状态可视化（运行、成功、失败）
 *       4. 执行时间统计，便于性能分析
 *
 * @see VisionTool 视觉工具基类
 * @see ToolParameterPanel 工具参数面板，配置工具参数
 * @see MainWindow 主窗口集成工具链面板
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
 * @class DraggableToolList
 * @brief 支持拖拽排序的工具列表控件
 *
 * @details 继承自QListWidget，扩展了拖拽排序功能。
 *          用户可以通过拖拽列表项来调整工具的执行顺序。
 *          拖拽完成后会发射toolOrderChanged信号通知外部组件。
 *
 * @par 使用场景：
 * - 工具链面板中的工具列表
 * - 需要拖拽排序功能的列表场景
 *
 * @par 拖拽行为：
 * - 支持列表内部拖拽排序
 * - 拖拽时显示拖动预览
 * - 放置时自动调整位置
 */
class DraggableToolList : public QListWidget
{
    Q_OBJECT

public:
    // ============================================================
    // 构造函数
    // ============================================================

    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     *
     * @details 初始化拖拽列表，配置拖拽模式为InternalMove。
     */
    explicit DraggableToolList(QWidget* parent = nullptr);

signals:
    // ============================================================
    // 信号定义
    // ============================================================

    /**
     * @brief 工具顺序改变信号
     * @param fromIndex 工具原始索引位置
     * @param toIndex 工具目标索引位置
     *
     * @details 当用户通过拖拽改变工具顺序时发射此信号。
     *          外部组件应响应此信号更新工具链的内部顺序。
     */
    void toolOrderChanged(int fromIndex, int toIndex);

protected:
    // ============================================================
    // 拖拽事件处理
    // ============================================================

    /**
     * @brief 拖拽进入事件处理
     * @param event 拖拽事件对象
     *
     * @details 检查拖拽数据是否可接受。
     */
    void dragEnterEvent(QDragEnterEvent* event) override;

    /**
     * @brief 拖拽移动事件处理
     * @param event 拖拽事件对象
     *
     * @details 更新拖拽过程中的视觉反馈。
     */
    void dragMoveEvent(QDragMoveEvent* event) override;

    /**
     * @brief 放置事件处理
     * @param event 放置事件对象
     *
     * @details 完成拖拽操作，发射toolOrderChanged信号。
     */
    void dropEvent(QDropEvent* event) override;

    /**
     * @brief 开始拖拽
     * @param supportedActions 支持的拖拽动作
     *
     * @details 记录拖拽起始位置，创建拖拽预览。
     */
    void startDrag(Qt::DropActions supportedActions) override;

private:
    int dragStartRow_;  ///< 拖拽起始行索引，用于计算移动距离
};

/**
 * @class ToolChainPanel
 * @brief 工具链面板类
 *
 * @details 工具链面板是视觉检测系统的核心配置界面，承担以下职责：
 *
 * @par 主要功能：
 * - 显示工具链中的所有工具：以列表形式展示，支持图标和状态显示
 * - 添加/删除工具：通过工具栏按钮或右键菜单操作
 * - 选择工具：点击选中工具，参数面板显示对应配置
 * - 启用/禁用工具：可临时禁用某些工具用于调试
 * - 拖拽排序：通过拖拽调整工具执行顺序
 * - 显示运行状态：运行中、成功、失败等状态可视化
 * - 执行时间统计：显示每个工具的执行耗时
 *
 * @par 使用场景：
 * - 工程师模式下配置检测流程
 * - 调试和优化检测工具链
 * - 分析各工具的执行性能
 *
 * @par 界面布局：
 * - 顶部工具栏：添加、删除、上移、下移按钮
 * - 主体列表：显示工具名称、状态、执行时间
 *
 * @par 信号槽关系：
 * - toolSelectionChanged：通知参数面板更新显示
 * - addToolRequested：通知主窗口弹出工具选择对话框
 * - removeToolRequested：通知主窗口删除工具
 * - toolOrderChanged：通知系统更新工具执行顺序
 */
class ToolChainPanel : public QWidget
{
    Q_OBJECT

public:
    // ============================================================
    // 构造与析构
    // ============================================================

    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     *
     * @details 初始化工具链面板，创建工具栏和列表控件。
     */
    explicit ToolChainPanel(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ToolChainPanel() override = default;

    // ============================================================
    // 工具管理方法
    // ============================================================

    /**
     * @brief 添加工具到工具链
     * @param tool 要添加的视觉工具实例
     *
     * @details 将工具添加到工具链末尾，并在列表中显示。
     *          自动连接工具的状态变化信号。
     *
     * @note 工具的所有权不转移，调用者负责管理工具的生命周期。
     */
    void addTool(Algorithm::VisionTool* tool);

    /**
     * @brief 从工具链移除工具
     * @param tool 要移除的工具实例
     *
     * @details 从工具链和列表中移除指定工具。
     *          断开与工具的信号连接。
     *
     * @note 此方法不会删除工具对象本身。
     */
    void removeTool(Algorithm::VisionTool* tool);

    /**
     * @brief 更新工具执行时间
     * @param tool 工具实例
     * @param executionTime 执行时间（毫秒）
     *
     * @details 更新指定工具的执行时间显示。
     *          用于性能分析和优化。
     *
     * @par 示例：
     * @code
     * panel->updateToolExecutionTime(tool, 15.5); // 显示 "15.5ms"
     * @endcode
     */
    void updateToolExecutionTime(Algorithm::VisionTool* tool, double executionTime);

    /**
     * @brief 清空工具链
     *
     * @details 移除工具链中的所有工具，清空列表显示。
     *          通常在加载新项目或重置时调用。
     *
     * @note 此方法不会删除工具对象本身。
     */
    void clear();

    /**
     * @brief 获取所有工具
     * @return 工具列表（按执行顺序）
     *
     * @details 返回当前工具链中所有工具的列表。
     */
    QList<Algorithm::VisionTool*> getTools() const { return tools_; }

    /**
     * @brief 获取当前选中的工具
     * @return 当前选中的工具指针，如果没有选中则返回nullptr
     */
    Algorithm::VisionTool* getCurrentTool() const;

    /**
     * @brief 设置当前选中的工具
     * @param tool 要选中的工具
     *
     * @details 选中指定工具，高亮显示对应列表项。
     *          如果tool为nullptr或不在列表中，则取消选择。
     */
    void setCurrentTool(Algorithm::VisionTool* tool);

signals:
    // ============================================================
    // 信号定义 - 通知外部组件状态变化和用户操作
    // ============================================================

    /**
     * @brief 工具选择改变信号
     * @param tool 当前选中的工具（可能为nullptr）
     *
     * @details 当用户点击选择不同工具时发射。
     *          参数面板应响应此信号更新显示。
     */
    void toolSelectionChanged(Algorithm::VisionTool* tool);

    /**
     * @brief 工具双击信号
     * @param tool 被双击的工具
     *
     * @details 当用户双击工具项时发射。
     *          通常用于弹出工具的详细参数编辑对话框。
     */
    void toolDoubleClicked(Algorithm::VisionTool* tool);

    /**
     * @brief 请求添加工具信号
     *
     * @details 当用户点击添加按钮时发射。
     *          主窗口应响应此信号显示工具选择对话框。
     */
    void addToolRequested();

    /**
     * @brief 请求删除工具信号
     * @param tool 要删除的工具
     *
     * @details 当用户请求删除工具时发射。
     *          主窗口应响应此信号执行删除操作。
     */
    void removeToolRequested(Algorithm::VisionTool* tool);

    /**
     * @brief 工具顺序改变信号
     *
     * @details 当用户通过拖拽或按钮改变工具顺序时发射。
     *          系统应响应此信号更新工具链的执行顺序。
     */
    void toolOrderChanged();

private slots:
    // ============================================================
    // 私有槽函数
    // ============================================================

    /**
     * @brief 列表选择改变响应
     * @param current 当前选中项
     * @param previous 之前选中项
     *
     * @details 更新选择状态，发射toolSelectionChanged信号。
     */
    void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

    /**
     * @brief 添加工具按钮响应
     *
     * @details 发射addToolRequested信号。
     */
    void onAddTool();

    /**
     * @brief 移除工具按钮响应
     *
     * @details 发射removeToolRequested信号（如果有选中的工具）。
     */
    void onRemoveTool();

    /**
     * @brief 上移按钮响应
     *
     * @details 将当前选中的工具上移一位。
     */
    void onMoveUp();

    /**
     * @brief 下移按钮响应
     *
     * @details 将当前选中的工具下移一位。
     */
    void onMoveDown();

    /**
     * @brief 工具参数变化响应
     *
     * @details 当工具参数发生变化时更新列表显示。
     */
    void onToolParamChanged();

    /**
     * @brief 拖拽排序响应
     * @param fromIndex 原始位置
     * @param toIndex 目标位置
     *
     * @details 更新内部工具列表顺序，发射toolOrderChanged信号。
     */
    void onToolDragDrop(int fromIndex, int toIndex);

private:
    // ============================================================
    // 私有方法
    // ============================================================

    /**
     * @brief 更新工具列表显示
     *
     * @details 根据内部工具列表刷新UI显示。
     */
    void updateToolList();

    /**
     * @brief 更新动作状态
     *
     * @details 根据当前选择状态更新工具栏按钮的启用/禁用状态。
     */
    void updateActions();

private:
    // ============================================================
    // 布局组件
    // ============================================================
    QVBoxLayout* mainLayout_;       ///< 主垂直布局
    QToolBar* toolBar_;             ///< 顶部工具栏
    DraggableToolList* toolList_;   ///< 工具列表（支持拖拽）

    // ============================================================
    // 工具栏动作
    // ============================================================
    QAction* addAction_;        ///< 添加工具动作
    QAction* removeAction_;     ///< 删除工具动作
    QAction* moveUpAction_;     ///< 上移动作
    QAction* moveDownAction_;   ///< 下移动作

    // ============================================================
    // 数据成员
    // ============================================================
    QList<Algorithm::VisionTool*> tools_;               ///< 工具列表（按执行顺序）
    QMap<Algorithm::VisionTool*, double> toolExecutionTimes_;  ///< 工具执行时间统计（毫秒）
};

} // namespace UI
} // namespace VisionForge
