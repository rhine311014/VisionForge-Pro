/**
 * @file ToolChainPanel.cpp
 * @brief 工具链面板实现
 */

#include "ui/ToolChainPanel.h"
#include "ui/Theme.h"
#include "base/Logger.h"
#include <QIcon>
#include <QListWidgetItem>

namespace VisionForge {
namespace UI {

ToolChainPanel::ToolChainPanel(QWidget* parent)
    : QWidget(parent)
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0);
    mainLayout_->setSpacing(0);

    // 工具栏
    toolBar_ = new QToolBar(this);
    toolBar_->setIconSize(QSize(20, 20));
    toolBar_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    addAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_ADD), "添加");
    addAction_->setToolTip("添加工具到工具链");
    connect(addAction_, &QAction::triggered, this, &ToolChainPanel::onAddTool);

    removeAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_REMOVE), "删除");
    removeAction_->setToolTip("删除选中的工具");
    connect(removeAction_, &QAction::triggered, this, &ToolChainPanel::onRemoveTool);

    toolBar_->addSeparator();

    moveUpAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_UP), "上移");
    moveUpAction_->setToolTip("将工具上移");
    connect(moveUpAction_, &QAction::triggered, this, &ToolChainPanel::onMoveUp);

    moveDownAction_ = toolBar_->addAction(Theme::getIcon(Icons::TOOL_DOWN), "下移");
    moveDownAction_->setToolTip("将工具下移");
    connect(moveDownAction_, &QAction::triggered, this, &ToolChainPanel::onMoveDown);

    mainLayout_->addWidget(toolBar_);

    // 工具列表
    toolList_ = new QListWidget(this);
    toolList_->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(toolList_, &QListWidget::currentItemChanged,
            this, &ToolChainPanel::onCurrentItemChanged);

    // 双击打开参数编辑对话框
    connect(toolList_, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem* item) {
                Q_UNUSED(item);
                Algorithm::VisionTool* tool = getCurrentTool();
                if (tool) {
                    emit toolDoubleClicked(tool);
                }
            });

    mainLayout_->addWidget(toolList_);

    updateActions();
}

void ToolChainPanel::addTool(Algorithm::VisionTool* tool)
{
    if (!tool || tools_.contains(tool)) {
        return;
    }

    tools_.append(tool);

    // 连接工具信号
    connect(tool, &Algorithm::VisionTool::paramChanged,
            this, &ToolChainPanel::onToolParamChanged);

    updateToolList();
    updateActions();

    LOG_DEBUG(QString("添加工具到工具链: %1").arg(tool->displayName()));
}

void ToolChainPanel::removeTool(Algorithm::VisionTool* tool)
{
    if (!tool || !tools_.contains(tool)) {
        return;
    }

    tools_.removeOne(tool);
    toolExecutionTimes_.remove(tool);  // 移除执行时间记录

    // 断开信号
    disconnect(tool, nullptr, this, nullptr);

    updateToolList();
    updateActions();

    LOG_DEBUG(QString("从工具链移除工具: %1").arg(tool->displayName()));
}

void ToolChainPanel::updateToolExecutionTime(Algorithm::VisionTool* tool, double executionTime)
{
    if (tool && tools_.contains(tool)) {
        toolExecutionTimes_[tool] = executionTime;
        updateToolList();
    }
}

void ToolChainPanel::clear()
{
    // 断开所有工具的信号
    for (Algorithm::VisionTool* tool : tools_) {
        disconnect(tool, nullptr, this, nullptr);
    }

    tools_.clear();
    toolList_->clear();
    updateActions();
}

Algorithm::VisionTool* ToolChainPanel::getCurrentTool() const
{
    int currentRow = toolList_->currentRow();
    if (currentRow >= 0 && currentRow < tools_.size()) {
        return tools_[currentRow];
    }
    return nullptr;
}

void ToolChainPanel::setCurrentTool(Algorithm::VisionTool* tool)
{
    int index = tools_.indexOf(tool);
    if (index >= 0) {
        toolList_->setCurrentRow(index);
    }
}

void ToolChainPanel::onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    Q_UNUSED(previous);

    Algorithm::VisionTool* tool = getCurrentTool();
    emit toolSelectionChanged(tool);

    updateActions();
}

void ToolChainPanel::onAddTool()
{
    emit addToolRequested();
}

void ToolChainPanel::onRemoveTool()
{
    Algorithm::VisionTool* tool = getCurrentTool();
    if (tool) {
        emit removeToolRequested(tool);
    }
}

void ToolChainPanel::onMoveUp()
{
    int currentRow = toolList_->currentRow();
    if (currentRow > 0) {
        // 交换工具
        tools_.swapItemsAt(currentRow, currentRow - 1);
        updateToolList();
        toolList_->setCurrentRow(currentRow - 1);
    }
}

void ToolChainPanel::onMoveDown()
{
    int currentRow = toolList_->currentRow();
    if (currentRow >= 0 && currentRow < tools_.size() - 1) {
        // 交换工具
        tools_.swapItemsAt(currentRow, currentRow + 1);
        updateToolList();
        toolList_->setCurrentRow(currentRow + 1);
    }
}

void ToolChainPanel::onToolParamChanged()
{
    // 工具参数改变，更新显示
    updateToolList();
}

void ToolChainPanel::updateToolList()
{
    // 保存当前选中项
    int currentRow = toolList_->currentRow();

    toolList_->clear();

    for (int i = 0; i < tools_.size(); ++i) {
        Algorithm::VisionTool* tool = tools_[i];

        QString text = QString("%1. %2").arg(i + 1).arg(tool->displayName());

        // 显示执行时间
        if (toolExecutionTimes_.contains(tool)) {
            double time = toolExecutionTimes_[tool];
            text += QString(" [%1 ms]").arg(time, 0, 'f', 2);
        }

        if (!tool->isEnabled()) {
            text += " (已禁用)";
        }

        QListWidgetItem* item = new QListWidgetItem(text);

        // 根据启用状态设置颜色
        if (!tool->isEnabled()) {
            item->setForeground(Qt::gray);
        }

        toolList_->addItem(item);
    }

    // 恢复选中项
    if (currentRow >= 0 && currentRow < tools_.size()) {
        toolList_->setCurrentRow(currentRow);
    }
}

void ToolChainPanel::updateActions()
{
    int currentRow = toolList_->currentRow();
    bool hasSelection = (currentRow >= 0);
    bool hasTool = !tools_.isEmpty();

    removeAction_->setEnabled(hasSelection);
    moveUpAction_->setEnabled(hasSelection && currentRow > 0);
    moveDownAction_->setEnabled(hasSelection && currentRow < tools_.size() - 1);
}

} // namespace UI
} // namespace VisionForge
