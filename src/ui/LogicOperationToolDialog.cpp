/**
 * @file LogicOperationToolDialog.cpp
 * @brief 逻辑运算工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/LogicOperationToolDialog.h"
#include "algorithm/LogicOperationTool.h"
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

LogicOperationToolDialog::LogicOperationToolDialog(Algorithm::LogicOperationTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , logicTypeCombo_(nullptr)
    , inputsTable_(nullptr)
    , addInputBtn_(nullptr)
    , removeInputBtn_(nullptr)
    , clearInputsBtn_(nullptr)
    , outputNameEdit_(nullptr)
    , resultLabel_(nullptr)
    , expressionLabel_(nullptr)
    , trueCountLabel_(nullptr)
    , falseCountLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("逻辑运算设置");
    setMinimumSize(700, 550);
    resize(750, 600);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

LogicOperationToolDialog::~LogicOperationToolDialog()
{
}

void LogicOperationToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    logicTypeCombo_->blockSignals(true);

    // 更新运算类型
    logicTypeCombo_->setCurrentIndex(static_cast<int>(tool_->logicType()));

    // 恢复信号
    logicTypeCombo_->blockSignals(false);

    updateInputsTable();
    updateResultDisplay();
}

void LogicOperationToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void LogicOperationToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createLogicTypeGroup(mainLayout);
    createInputsGroup(mainLayout);
    createOutputGroup(mainLayout);
    createResultGroup(mainLayout);
    createButtonGroup(mainLayout);

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

void LogicOperationToolDialog::createLogicTypeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("运算类型", this);
    QFormLayout* formLayout = new QFormLayout(group);

    logicTypeCombo_ = new QComboBox(group);
    logicTypeCombo_->addItem("AND (与运算) - 所有输入为真则结果为真", static_cast<int>(Algorithm::LogicType::AND));
    logicTypeCombo_->addItem("OR (或运算) - 任一输入为真则结果为真", static_cast<int>(Algorithm::LogicType::OR));
    logicTypeCombo_->addItem("NOT (非运算) - 单输入取反", static_cast<int>(Algorithm::LogicType::NOT));
    logicTypeCombo_->addItem("XOR (异或运算) - 奇数个输入为真则结果为真", static_cast<int>(Algorithm::LogicType::XOR));
    logicTypeCombo_->addItem("NAND (与非运算) - AND的取反", static_cast<int>(Algorithm::LogicType::NAND));
    logicTypeCombo_->addItem("NOR (或非运算) - OR的取反", static_cast<int>(Algorithm::LogicType::NOR));

    formLayout->addRow("运算类型:", logicTypeCombo_);

    layout->addWidget(group);
}

void LogicOperationToolDialog::createInputsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("输入源列表", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 输入表格
    inputsTable_ = new QTableWidget(group);
    inputsTable_->setColumnCount(5);
    inputsTable_->setHorizontalHeaderLabels({"启用", "名称", "来源工具ID", "取反", "当前值"});
    inputsTable_->horizontalHeader()->setStretchLastSection(true);
    inputsTable_->setMinimumHeight(150);
    inputsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    groupLayout->addWidget(inputsTable_);

    // 操作按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    addInputBtn_ = new QPushButton("添加输入", group);
    removeInputBtn_ = new QPushButton("删除选中", group);
    clearInputsBtn_ = new QPushButton("清空全部", group);
    btnLayout->addWidget(addInputBtn_);
    btnLayout->addWidget(removeInputBtn_);
    btnLayout->addWidget(clearInputsBtn_);
    btnLayout->addStretch();
    groupLayout->addLayout(btnLayout);

    layout->addWidget(group);
}

void LogicOperationToolDialog::createOutputGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("输出设置", this);
    QFormLayout* formLayout = new QFormLayout(group);

    outputNameEdit_ = new QLineEdit(group);
    outputNameEdit_->setPlaceholderText("输出结果名称");
    outputNameEdit_->setText("LogicResult");

    formLayout->addRow("输出名称:", outputNameEdit_);

    layout->addWidget(group);
}

void LogicOperationToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("运算结果", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QHBoxLayout* resultLayout = new QHBoxLayout();

    resultLabel_ = new QLabel("--", group);
    resultLabel_->setStyleSheet("font-size: 18px; font-weight: bold;");
    resultLayout->addWidget(new QLabel("结果:"));
    resultLayout->addWidget(resultLabel_);
    resultLayout->addSpacing(30);

    trueCountLabel_ = new QLabel("0", group);
    trueCountLabel_->setStyleSheet("color: green;");
    resultLayout->addWidget(new QLabel("真值数:"));
    resultLayout->addWidget(trueCountLabel_);
    resultLayout->addSpacing(10);

    falseCountLabel_ = new QLabel("0", group);
    falseCountLabel_->setStyleSheet("color: red;");
    resultLayout->addWidget(new QLabel("假值数:"));
    resultLayout->addWidget(falseCountLabel_);
    resultLayout->addStretch();

    groupLayout->addLayout(resultLayout);

    // 表达式
    QHBoxLayout* exprLayout = new QHBoxLayout();
    exprLayout->addWidget(new QLabel("表达式:"));
    expressionLabel_ = new QLabel("--", group);
    expressionLabel_->setStyleSheet("font-family: monospace; color: #666;");
    exprLayout->addWidget(expressionLabel_);
    exprLayout->addStretch();
    groupLayout->addLayout(exprLayout);

    layout->addWidget(group);
}

void LogicOperationToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("执行运算", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #9C27B0; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void LogicOperationToolDialog::connectSignals()
{
    // 参数变化
    connect(logicTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LogicOperationToolDialog::onLogicTypeChanged);

    // 输入操作
    connect(addInputBtn_, &QPushButton::clicked, this, &LogicOperationToolDialog::onAddInput);
    connect(removeInputBtn_, &QPushButton::clicked, this, &LogicOperationToolDialog::onRemoveInput);
    connect(clearInputsBtn_, &QPushButton::clicked, this, &LogicOperationToolDialog::onClearInputs);
    connect(inputsTable_, &QTableWidget::cellChanged, this, &LogicOperationToolDialog::onInputCellChanged);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &LogicOperationToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &LogicOperationToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &LogicOperationToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &LogicOperationToolDialog::onApplyClicked);
}

void LogicOperationToolDialog::onLogicTypeChanged(int index)
{
    if (!tool_) return;

    auto type = static_cast<Algorithm::LogicType>(logicTypeCombo_->currentData().toInt());
    tool_->setLogicType(type);
    emit parameterChanged();
}

void LogicOperationToolDialog::onAddInput()
{
    if (!tool_) return;

    Algorithm::LogicInput input;
    input.name = QString("输入%1").arg(tool_->inputs().size() + 1);
    input.enabled = true;
    input.inverted = false;

    tool_->addInput(input);
    updateInputsTable();
    emit parameterChanged();
}

void LogicOperationToolDialog::onRemoveInput()
{
    if (!tool_) return;

    int row = inputsTable_->currentRow();
    if (row >= 0) {
        tool_->removeInput(row);
        updateInputsTable();
        emit parameterChanged();
    }
}

void LogicOperationToolDialog::onClearInputs()
{
    if (!tool_) return;

    if (QMessageBox::question(this, "确认", "确定清空所有输入吗?") == QMessageBox::Yes) {
        tool_->clearInputs();
        updateInputsTable();
        emit parameterChanged();
    }
}

void LogicOperationToolDialog::onInputCellChanged(int row, int column)
{
    syncInputsFromTable();
}

void LogicOperationToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeOperationRequested();
    updateResultDisplay();
}

void LogicOperationToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void LogicOperationToolDialog::onCancelClicked()
{
    reject();
}

void LogicOperationToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void LogicOperationToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setLogicType(static_cast<Algorithm::LogicType>(
        logicTypeCombo_->currentData().toInt()));

    syncInputsFromTable();

    LOG_INFO("逻辑运算参数已应用");
}

void LogicOperationToolDialog::updateInputsTable()
{
    if (!tool_) return;

    inputsTable_->blockSignals(true);

    const auto& inputs = tool_->inputs();
    inputsTable_->setRowCount(inputs.size());

    for (size_t i = 0; i < inputs.size(); ++i) {
        const auto& input = inputs[i];

        // 启用复选框
        QTableWidgetItem* enableItem = new QTableWidgetItem();
        enableItem->setCheckState(input.enabled ? Qt::Checked : Qt::Unchecked);
        inputsTable_->setItem(i, 0, enableItem);

        // 名称
        inputsTable_->setItem(i, 1, new QTableWidgetItem(input.name));

        // 来源工具ID
        inputsTable_->setItem(i, 2, new QTableWidgetItem(input.sourceToolId));

        // 取反复选框
        QTableWidgetItem* invertItem = new QTableWidgetItem();
        invertItem->setCheckState(input.inverted ? Qt::Checked : Qt::Unchecked);
        inputsTable_->setItem(i, 3, invertItem);

        // 当前值
        QTableWidgetItem* valueItem = new QTableWidgetItem(input.currentValue ? "TRUE" : "FALSE");
        valueItem->setForeground(input.currentValue ? QColor(0, 128, 0) : QColor(255, 0, 0));
        inputsTable_->setItem(i, 4, valueItem);
    }

    inputsTable_->blockSignals(false);
}

void LogicOperationToolDialog::updateResultDisplay()
{
    if (!tool_) return;

    const auto& result = tool_->result();

    // 更新结果
    if (result.result) {
        resultLabel_->setText("TRUE");
        resultLabel_->setStyleSheet("font-size: 18px; font-weight: bold; color: green;");
    } else {
        resultLabel_->setText("FALSE");
        resultLabel_->setStyleSheet("font-size: 18px; font-weight: bold; color: red;");
    }

    trueCountLabel_->setText(QString::number(result.trueCount));
    falseCountLabel_->setText(QString::number(result.falseCount));
    expressionLabel_->setText(result.expression);

    // 更新输入表格的当前值列
    updateInputsTable();
}

void LogicOperationToolDialog::syncInputsFromTable()
{
    if (!tool_) return;

    inputsTable_->blockSignals(true);

    for (int i = 0; i < inputsTable_->rowCount(); ++i) {
        Algorithm::LogicInput input;

        auto enableItem = inputsTable_->item(i, 0);
        if (enableItem) {
            input.enabled = (enableItem->checkState() == Qt::Checked);
        }

        auto nameItem = inputsTable_->item(i, 1);
        if (nameItem) {
            input.name = nameItem->text();
        }

        auto sourceItem = inputsTable_->item(i, 2);
        if (sourceItem) {
            input.sourceToolId = sourceItem->text();
        }

        auto invertItem = inputsTable_->item(i, 3);
        if (invertItem) {
            input.inverted = (invertItem->checkState() == Qt::Checked);
        }

        tool_->setInput(i, input);
    }

    inputsTable_->blockSignals(false);
}

} // namespace UI
} // namespace VisionForge
