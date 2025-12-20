/**
 * @file RangeJudgeToolDialog.cpp
 * @brief 范围判定工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/RangeJudgeToolDialog.h"
#include "algorithm/RangeJudgeTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>

namespace VisionForge {
namespace UI {

RangeJudgeToolDialog::RangeJudgeToolDialog(Algorithm::RangeJudgeTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , combineLogicCombo_(nullptr)
    , conditionsTable_(nullptr)
    , addConditionBtn_(nullptr)
    , removeConditionBtn_(nullptr)
    , clearConditionsBtn_(nullptr)
    , testKeyEdit_(nullptr)
    , testValueSpin_(nullptr)
    , testBtn_(nullptr)
    , overallResultLabel_(nullptr)
    , passCountLabel_(nullptr)
    , failCountLabel_(nullptr)
    , detailsTable_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoCalcCheck_(nullptr)
{
    setWindowTitle("范围判定设置");
    setMinimumSize(700, 600);
    resize(800, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

RangeJudgeToolDialog::~RangeJudgeToolDialog()
{
}

void RangeJudgeToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    combineLogicCombo_->blockSignals(true);

    // 更新组合逻辑
    combineLogicCombo_->setCurrentIndex(static_cast<int>(tool_->combineLogic()));

    // 恢复信号
    combineLogicCombo_->blockSignals(false);

    updateConditionsTable();
    updateResultDisplay();
}

void RangeJudgeToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void RangeJudgeToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createCombineLogicGroup(mainLayout);
    createConditionsGroup(mainLayout);
    createTestGroup(mainLayout);
    createResultGroup(mainLayout);
    createButtonGroup(mainLayout);

    // 创建实时计算辅助器
    previewHelper_ = new PreviewHelper(this, 100);

    // 实时计算选项
    QHBoxLayout* autoCalcLayout = new QHBoxLayout();
    autoCalcCheck_ = new QCheckBox("实时计算", this);
    autoCalcCheck_->setChecked(true);
    autoCalcCheck_->setToolTip("启用后参数修改会自动更新判定结果");
    autoCalcLayout->addWidget(autoCalcCheck_);
    autoCalcLayout->addStretch();
    mainLayout->addLayout(autoCalcLayout);

    // 底部按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okBtn_ = new QPushButton("确定", this);
    cancelBtn_ = new QPushButton("取消", this);
    applyBtn_ = new QPushButton("应用", this);

    okBtn_->setMinimumWidth(80);
    cancelBtn_->setMinimumWidth(80);
    applyBtn_->setMinimumWidth(80);

    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    buttonLayout->addWidget(applyBtn_);

    mainLayout->addLayout(buttonLayout);
}

void RangeJudgeToolDialog::createCombineLogicGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("判定逻辑", this);
    QFormLayout* formLayout = new QFormLayout(group);

    combineLogicCombo_ = new QComboBox(group);
    combineLogicCombo_->addItem("与 (AND) - 所有条件都满足", static_cast<int>(Algorithm::RangeJudgeTool::LogicAND));
    combineLogicCombo_->addItem("或 (OR) - 任一条件满足", static_cast<int>(Algorithm::RangeJudgeTool::LogicOR));

    formLayout->addRow("组合逻辑:", combineLogicCombo_);

    layout->addWidget(group);
}

void RangeJudgeToolDialog::createConditionsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("判定条件", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 条件表格
    conditionsTable_ = new QTableWidget(group);
    conditionsTable_->setColumnCount(6);
    conditionsTable_->setHorizontalHeaderLabels({"启用", "名称", "数据源", "最小值", "最大值", "结果"});
    conditionsTable_->horizontalHeader()->setStretchLastSection(true);
    conditionsTable_->setMinimumHeight(150);
    conditionsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    groupLayout->addWidget(conditionsTable_);

    // 操作按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addConditionBtn_ = new QPushButton("添加条件", group);
    removeConditionBtn_ = new QPushButton("删除选中", group);
    clearConditionsBtn_ = new QPushButton("清空全部", group);
    btnLayout->addWidget(addConditionBtn_);
    btnLayout->addWidget(removeConditionBtn_);
    btnLayout->addWidget(clearConditionsBtn_);
    btnLayout->addStretch();
    groupLayout->addLayout(btnLayout);

    layout->addWidget(group);
}

void RangeJudgeToolDialog::createTestGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("测试输入", this);
    QHBoxLayout* groupLayout = new QHBoxLayout(group);

    groupLayout->addWidget(new QLabel("键名:"));
    testKeyEdit_ = new QLineEdit(group);
    testKeyEdit_->setPlaceholderText("输入值键名");
    testKeyEdit_->setMaximumWidth(150);
    groupLayout->addWidget(testKeyEdit_);

    groupLayout->addWidget(new QLabel("值:"));
    testValueSpin_ = new QDoubleSpinBox(group);
    testValueSpin_->setRange(-1e10, 1e10);
    testValueSpin_->setDecimals(4);
    groupLayout->addWidget(testValueSpin_);

    testBtn_ = new QPushButton("设置测试值", group);
    groupLayout->addWidget(testBtn_);
    groupLayout->addStretch();

    layout->addWidget(group);
}

void RangeJudgeToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("判定结果", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QHBoxLayout* summaryLayout = new QHBoxLayout();

    overallResultLabel_ = new QLabel("--", group);
    overallResultLabel_->setStyleSheet("font-size: 16px; font-weight: bold;");
    summaryLayout->addWidget(new QLabel("总体结果:"));
    summaryLayout->addWidget(overallResultLabel_);
    summaryLayout->addSpacing(20);

    passCountLabel_ = new QLabel("0", group);
    passCountLabel_->setStyleSheet("color: green;");
    summaryLayout->addWidget(new QLabel("通过:"));
    summaryLayout->addWidget(passCountLabel_);
    summaryLayout->addSpacing(10);

    failCountLabel_ = new QLabel("0", group);
    failCountLabel_->setStyleSheet("color: red;");
    summaryLayout->addWidget(new QLabel("失败:"));
    summaryLayout->addWidget(failCountLabel_);
    summaryLayout->addStretch();

    groupLayout->addLayout(summaryLayout);

    // 详细结果表格
    detailsTable_ = new QTableWidget(group);
    detailsTable_->setColumnCount(4);
    detailsTable_->setHorizontalHeaderLabels({"条件名称", "当前值", "范围", "结果"});
    detailsTable_->horizontalHeader()->setStretchLastSection(true);
    detailsTable_->setMinimumHeight(100);
    groupLayout->addWidget(detailsTable_);

    layout->addWidget(group);
}

void RangeJudgeToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("执行判定", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void RangeJudgeToolDialog::connectSignals()
{
    // 参数变化
    connect(combineLogicCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RangeJudgeToolDialog::onCombineLogicChanged);

    // 条件操作
    connect(addConditionBtn_, &QPushButton::clicked, this, &RangeJudgeToolDialog::onAddCondition);
    connect(removeConditionBtn_, &QPushButton::clicked, this, &RangeJudgeToolDialog::onRemoveCondition);
    connect(clearConditionsBtn_, &QPushButton::clicked, this, &RangeJudgeToolDialog::onClearConditions);
    connect(conditionsTable_, &QTableWidget::cellChanged, this, &RangeJudgeToolDialog::onConditionCellChanged);

    // 测试
    connect(testValueSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RangeJudgeToolDialog::onTestValueChanged);
    connect(testBtn_, &QPushButton::clicked, this, [this]() {
        if (!tool_) return;
        QString key = testKeyEdit_->text();
        if (!key.isEmpty()) {
            tool_->setInputValue(key, testValueSpin_->value());
            LOG_INFO(QString("设置测试值: %1 = %2").arg(key).arg(testValueSpin_->value()));
            previewHelper_->requestPreview();
        }
    });

    // 实时计算
    connect(autoCalcCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &RangeJudgeToolDialog::onAutoCalc);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &RangeJudgeToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &RangeJudgeToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &RangeJudgeToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &RangeJudgeToolDialog::onApplyClicked);
}

void RangeJudgeToolDialog::onCombineLogicChanged(int index)
{
    Q_UNUSED(index);
    if (!tool_) return;

    auto logic = static_cast<Algorithm::RangeJudgeTool::CombineLogic>(
        combineLogicCombo_->currentData().toInt());
    tool_->setCombineLogic(logic);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void RangeJudgeToolDialog::onAddCondition()
{
    if (!tool_) return;

    Algorithm::JudgeCondition condition;
    condition.name = QString("条件%1").arg(tool_->conditions().size() + 1);
    condition.minValue = 0.0;
    condition.maxValue = 100.0;
    condition.enabled = true;

    tool_->addCondition(condition);
    updateConditionsTable();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void RangeJudgeToolDialog::onRemoveCondition()
{
    if (!tool_) return;

    int row = conditionsTable_->currentRow();
    if (row >= 0) {
        tool_->removeCondition(row);
        updateConditionsTable();
        emit parameterChanged();
        previewHelper_->requestPreview();
    }
}

void RangeJudgeToolDialog::onClearConditions()
{
    if (!tool_) return;

    if (QMessageBox::question(this, "确认", "确定清空所有条件吗?") == QMessageBox::Yes) {
        tool_->clearConditions();
        updateConditionsTable();
        emit parameterChanged();
    }
}

void RangeJudgeToolDialog::onConditionCellChanged(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    syncConditionsFromTable();
    previewHelper_->requestPreview();
}

void RangeJudgeToolDialog::onTestValueChanged(double value)
{
    // 可选: 自动更新测试值
}

void RangeJudgeToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeJudgeRequested();
    updateResultDisplay();
}

void RangeJudgeToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void RangeJudgeToolDialog::onCancelClicked()
{
    reject();
}

void RangeJudgeToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void RangeJudgeToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setCombineLogic(static_cast<Algorithm::RangeJudgeTool::CombineLogic>(
        combineLogicCombo_->currentData().toInt()));

    syncConditionsFromTable();

    LOG_INFO("范围判定参数已应用");
}

void RangeJudgeToolDialog::updateConditionsTable()
{
    if (!tool_) return;

    conditionsTable_->blockSignals(true);

    const auto& conditions = tool_->conditions();
    conditionsTable_->setRowCount(conditions.size());

    for (size_t i = 0; i < conditions.size(); ++i) {
        const auto& cond = conditions[i];

        // 启用复选框
        QTableWidgetItem* enableItem = new QTableWidgetItem();
        enableItem->setCheckState(cond.enabled ? Qt::Checked : Qt::Unchecked);
        conditionsTable_->setItem(i, 0, enableItem);

        // 名称
        conditionsTable_->setItem(i, 1, new QTableWidgetItem(cond.name));

        // 数据源
        conditionsTable_->setItem(i, 2, new QTableWidgetItem(cond.sourceToolId));

        // 最小值
        conditionsTable_->setItem(i, 3, new QTableWidgetItem(QString::number(cond.minValue)));

        // 最大值
        conditionsTable_->setItem(i, 4, new QTableWidgetItem(QString::number(cond.maxValue)));

        // 结果
        QTableWidgetItem* resultItem = new QTableWidgetItem(cond.result ? "PASS" : "FAIL");
        resultItem->setForeground(cond.result ? QColor(0, 128, 0) : QColor(255, 0, 0));
        conditionsTable_->setItem(i, 5, resultItem);
    }

    conditionsTable_->blockSignals(false);
}

void RangeJudgeToolDialog::updateResultDisplay()
{
    if (!tool_) return;

    const auto& result = tool_->result();

    // 更新总体结果
    if (result.overallResult) {
        overallResultLabel_->setText("OK");
        overallResultLabel_->setStyleSheet("font-size: 16px; font-weight: bold; color: green;");
    } else {
        overallResultLabel_->setText("NG");
        overallResultLabel_->setStyleSheet("font-size: 16px; font-weight: bold; color: red;");
    }

    passCountLabel_->setText(QString::number(result.passCount));
    failCountLabel_->setText(QString::number(result.failCount));

    // 更新详细结果表格
    detailsTable_->setRowCount(result.details.size());
    for (size_t i = 0; i < result.details.size(); ++i) {
        const auto& detail = result.details[i];
        detailsTable_->setItem(i, 0, new QTableWidgetItem(detail.name));
        detailsTable_->setItem(i, 1, new QTableWidgetItem("--")); // 当前值
        detailsTable_->setItem(i, 2, new QTableWidgetItem(
            QString("[%1, %2]").arg(detail.minValue).arg(detail.maxValue)));

        QTableWidgetItem* resultItem = new QTableWidgetItem(detail.result ? "PASS" : "FAIL");
        resultItem->setForeground(detail.result ? QColor(0, 128, 0) : QColor(255, 0, 0));
        detailsTable_->setItem(i, 3, resultItem);
    }

    // 更新条件表格的结果列
    updateConditionsTable();
}

void RangeJudgeToolDialog::syncConditionsFromTable()
{
    if (!tool_) return;

    conditionsTable_->blockSignals(true);

    for (int i = 0; i < conditionsTable_->rowCount(); ++i) {
        Algorithm::JudgeCondition condition;

        auto enableItem = conditionsTable_->item(i, 0);
        if (enableItem) {
            condition.enabled = (enableItem->checkState() == Qt::Checked);
        }

        auto nameItem = conditionsTable_->item(i, 1);
        if (nameItem) {
            condition.name = nameItem->text();
        }

        auto sourceItem = conditionsTable_->item(i, 2);
        if (sourceItem) {
            condition.sourceToolId = sourceItem->text();
        }

        auto minItem = conditionsTable_->item(i, 3);
        if (minItem) {
            condition.minValue = minItem->text().toDouble();
        }

        auto maxItem = conditionsTable_->item(i, 4);
        if (maxItem) {
            condition.maxValue = maxItem->text().toDouble();
        }

        tool_->setCondition(i, condition);
    }

    conditionsTable_->blockSignals(false);
}

void RangeJudgeToolDialog::onAutoCalc()
{
    if (!tool_) return;

    applyParameters();

    // 执行判定
    Algorithm::ToolResult result;
    tool_->process(nullptr, result);
    updateResultDisplay();
}

} // namespace UI
} // namespace VisionForge
