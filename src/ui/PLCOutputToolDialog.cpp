/**
 * @file PLCOutputToolDialog.cpp
 * @brief PLC输出工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/PLCOutputToolDialog.h"
#include "algorithm/PLCOutputTool.h"
#include "base/Logger.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QHeaderView>

namespace VisionForge {
namespace UI {

PLCOutputToolDialog::PLCOutputToolDialog(Algorithm::PLCOutputTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , mappingTable_(nullptr)
    , addMappingBtn_(nullptr)
    , removeMappingBtn_(nullptr)
    , clearMappingsBtn_(nullptr)
    , outputResultCheck_(nullptr)
    , resultAddressSpin_(nullptr)
    , resultStatusLabel_(nullptr)
    , outputCompleteCheck_(nullptr)
    , completeAddressSpin_(nullptr)
    , completeStatusLabel_(nullptr)
    , testKeyEdit_(nullptr)
    , testValueSpin_(nullptr)
    , testOutputBtn_(nullptr)
    , testResultLabel_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle(tr("PLC输出工具设置"));
    setMinimumSize(600, 550);
    createUI();
    connectSignals();
    updateUI();
}

PLCOutputToolDialog::~PLCOutputToolDialog() = default;

void PLCOutputToolDialog::updateUI()
{
    if (!tool_) return;

    // 更新结果输出设置
    outputResultCheck_->setChecked(tool_->outputResult());
    resultAddressSpin_->setValue(tool_->resultAddress());
    resultAddressSpin_->setEnabled(tool_->outputResult());

    // 更新完成信号设置
    outputCompleteCheck_->setChecked(tool_->outputComplete());
    completeAddressSpin_->setValue(tool_->completeAddress());
    completeAddressSpin_->setEnabled(tool_->outputComplete());

    // 更新映射表格
    updateMappingTable();

    // 更新状态显示
    bool hasPlc = tool_->plcComm() != nullptr;
    resultStatusLabel_->setText(hasPlc ? tr("PLC已连接") : tr("PLC未连接"));
    resultStatusLabel_->setStyleSheet(hasPlc ? "color: green;" : "color: red;");
    completeStatusLabel_->setText(hasPlc ? tr("PLC已连接") : tr("PLC未连接"));
    completeStatusLabel_->setStyleSheet(hasPlc ? "color: green;" : "color: red;");
}

void PLCOutputToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void PLCOutputToolDialog::createUI()
{
    auto* mainLayout = new QVBoxLayout(this);

    // 使用滚动区域
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto* scrollWidget = new QWidget();
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    createMappingGroup(scrollLayout);
    createResultGroup(scrollLayout);
    createCompleteGroup(scrollLayout);
    createTestGroup(scrollLayout);

    scrollLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    createButtonGroup(mainLayout);
}

void PLCOutputToolDialog::createMappingGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox(tr("输出映射配置"), layout->parentWidget());
    auto* groupLayout = new QVBoxLayout(group);

    // 说明标签
    auto* descLabel = new QLabel(tr("配置检测结果数据到PLC地址的映射关系"), group);
    descLabel->setStyleSheet("color: gray;");
    groupLayout->addWidget(descLabel);

    // 映射表格
    mappingTable_ = new QTableWidget(group);
    mappingTable_->setColumnCount(6);
    mappingTable_->setHorizontalHeaderLabels({tr("名称"), tr("数据源"), tr("地址"), tr("位地址"), tr("缩放"), tr("偏移")});
    mappingTable_->horizontalHeader()->setStretchLastSection(true);
    mappingTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    mappingTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    mappingTable_->setMaximumHeight(180);

    // 设置列宽
    mappingTable_->setColumnWidth(0, 80);
    mappingTable_->setColumnWidth(1, 100);
    mappingTable_->setColumnWidth(2, 60);
    mappingTable_->setColumnWidth(3, 50);
    mappingTable_->setColumnWidth(4, 60);
    mappingTable_->setColumnWidth(5, 60);

    groupLayout->addWidget(mappingTable_);

    // 操作按钮
    auto* btnLayout = new QHBoxLayout();
    addMappingBtn_ = new QPushButton(tr("添加映射"), group);
    removeMappingBtn_ = new QPushButton(tr("删除选中"), group);
    clearMappingsBtn_ = new QPushButton(tr("清空全部"), group);

    btnLayout->addWidget(addMappingBtn_);
    btnLayout->addWidget(removeMappingBtn_);
    btnLayout->addWidget(clearMappingsBtn_);
    btnLayout->addStretch();

    groupLayout->addLayout(btnLayout);

    layout->addWidget(group);
}

void PLCOutputToolDialog::createResultGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox(tr("检测结果输出 (OK/NG)"), layout->parentWidget());
    auto* groupLayout = new QFormLayout(group);

    outputResultCheck_ = new QCheckBox(tr("启用结果输出"), group);

    resultAddressSpin_ = new QSpinBox(group);
    resultAddressSpin_->setRange(0, 65535);
    resultAddressSpin_->setValue(0);
    resultAddressSpin_->setPrefix("D");

    resultStatusLabel_ = new QLabel(tr("PLC未连接"), group);
    resultStatusLabel_->setStyleSheet("color: red;");

    groupLayout->addRow("", outputResultCheck_);
    groupLayout->addRow(tr("结果地址:"), resultAddressSpin_);
    groupLayout->addRow(tr("状态:"), resultStatusLabel_);

    // 说明
    auto* noteLabel = new QLabel(tr("OK=1, NG=0 写入指定地址"), group);
    noteLabel->setStyleSheet("color: gray; font-size: 10px;");
    groupLayout->addRow("", noteLabel);

    layout->addWidget(group);
}

void PLCOutputToolDialog::createCompleteGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox(tr("完成信号输出"), layout->parentWidget());
    auto* groupLayout = new QFormLayout(group);

    outputCompleteCheck_ = new QCheckBox(tr("启用完成信号"), group);

    completeAddressSpin_ = new QSpinBox(group);
    completeAddressSpin_->setRange(0, 65535);
    completeAddressSpin_->setValue(1);
    completeAddressSpin_->setPrefix("D");

    completeStatusLabel_ = new QLabel(tr("PLC未连接"), group);
    completeStatusLabel_->setStyleSheet("color: red;");

    groupLayout->addRow("", outputCompleteCheck_);
    groupLayout->addRow(tr("完成地址:"), completeAddressSpin_);
    groupLayout->addRow(tr("状态:"), completeStatusLabel_);

    // 说明
    auto* noteLabel = new QLabel(tr("检测完成后置位，PLC读取后复位"), group);
    noteLabel->setStyleSheet("color: gray; font-size: 10px;");
    groupLayout->addRow("", noteLabel);

    layout->addWidget(group);
}

void PLCOutputToolDialog::createTestGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox(tr("测试输出"), layout->parentWidget());
    auto* groupLayout = new QFormLayout(group);

    testKeyEdit_ = new QLineEdit(group);
    testKeyEdit_->setPlaceholderText(tr("例如: distance, angle"));

    testValueSpin_ = new QDoubleSpinBox(group);
    testValueSpin_->setRange(-999999, 999999);
    testValueSpin_->setDecimals(3);
    testValueSpin_->setValue(0);

    testOutputBtn_ = new QPushButton(tr("测试输出"), group);

    testResultLabel_ = new QLabel("", group);

    groupLayout->addRow(tr("数据键:"), testKeyEdit_);
    groupLayout->addRow(tr("测试值:"), testValueSpin_);
    groupLayout->addRow("", testOutputBtn_);
    groupLayout->addRow(tr("结果:"), testResultLabel_);

    layout->addWidget(group);
}

void PLCOutputToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    auto* buttonLayout = new QHBoxLayout();

    okBtn_ = new QPushButton(tr("确定"), this);
    cancelBtn_ = new QPushButton(tr("取消"), this);
    applyBtn_ = new QPushButton(tr("应用"), this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    buttonLayout->addWidget(applyBtn_);

    layout->addLayout(buttonLayout);
}

void PLCOutputToolDialog::connectSignals()
{
    // 映射操作
    connect(addMappingBtn_, &QPushButton::clicked, this, &PLCOutputToolDialog::onAddMappingClicked);
    connect(removeMappingBtn_, &QPushButton::clicked, this, &PLCOutputToolDialog::onRemoveMappingClicked);
    connect(clearMappingsBtn_, &QPushButton::clicked, this, &PLCOutputToolDialog::onClearMappingsClicked);
    connect(mappingTable_, &QTableWidget::cellClicked, this, &PLCOutputToolDialog::onMappingSelected);
    connect(mappingTable_, &QTableWidget::cellChanged, this, &PLCOutputToolDialog::onMappingCellChanged);

    // 参数变更
    connect(resultAddressSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &PLCOutputToolDialog::onResultAddressChanged);
    connect(completeAddressSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &PLCOutputToolDialog::onCompleteAddressChanged);
    connect(outputResultCheck_, &QCheckBox::toggled, this, &PLCOutputToolDialog::onOutputResultChanged);
    connect(outputCompleteCheck_, &QCheckBox::toggled, this, &PLCOutputToolDialog::onOutputCompleteChanged);

    // 测试
    connect(testOutputBtn_, &QPushButton::clicked, this, &PLCOutputToolDialog::onTestOutputClicked);

    // 对话框按钮
    connect(okBtn_, &QPushButton::clicked, this, &PLCOutputToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &PLCOutputToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &PLCOutputToolDialog::onApplyClicked);
}

void PLCOutputToolDialog::onAddMappingClicked()
{
    if (!tool_) return;

    Algorithm::OutputMapping mapping;
    mapping.name = QString("映射%1").arg(tool_->mappings().size() + 1);
    mapping.sourceKey = "value";
    mapping.address = 100 + static_cast<int>(tool_->mappings().size());
    mapping.isBit = false;
    mapping.scale = 1.0;
    mapping.offset = 0;
    mapping.enabled = true;

    tool_->addMapping(mapping);
    updateMappingTable();
    emit parameterChanged();
}

void PLCOutputToolDialog::onRemoveMappingClicked()
{
    if (!tool_) return;

    int row = mappingTable_->currentRow();
    if (row >= 0) {
        tool_->removeMapping(row);
        updateMappingTable();
        emit parameterChanged();
    }
}

void PLCOutputToolDialog::onClearMappingsClicked()
{
    if (!tool_) return;

    if (QMessageBox::question(this, tr("确认"), tr("确定要清空所有映射吗?")) == QMessageBox::Yes) {
        tool_->clearMappings();
        updateMappingTable();
        emit parameterChanged();
    }
}

void PLCOutputToolDialog::onMappingSelected(int row)
{
    Q_UNUSED(row);
}

void PLCOutputToolDialog::onMappingCellChanged(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    syncMappingsFromTable();
}

void PLCOutputToolDialog::onResultAddressChanged(int value)
{
    if (!tool_) return;
    tool_->setResultAddress(value);
    emit parameterChanged();
}

void PLCOutputToolDialog::onCompleteAddressChanged(int value)
{
    if (!tool_) return;
    tool_->setCompleteAddress(value);
    emit parameterChanged();
}

void PLCOutputToolDialog::onOutputResultChanged(bool checked)
{
    if (!tool_) return;
    tool_->setOutputResult(checked);
    resultAddressSpin_->setEnabled(checked);
    emit parameterChanged();
}

void PLCOutputToolDialog::onOutputCompleteChanged(bool checked)
{
    if (!tool_) return;
    tool_->setOutputComplete(checked);
    completeAddressSpin_->setEnabled(checked);
    emit parameterChanged();
}

void PLCOutputToolDialog::onTestOutputClicked()
{
    if (!tool_) {
        testResultLabel_->setText(tr("工具未初始化"));
        testResultLabel_->setStyleSheet("color: red;");
        return;
    }

    if (!tool_->plcComm()) {
        testResultLabel_->setText(tr("PLC未连接"));
        testResultLabel_->setStyleSheet("color: red;");
        return;
    }

    QString key = testKeyEdit_->text().trimmed();
    if (key.isEmpty()) {
        testResultLabel_->setText(tr("请输入数据键"));
        testResultLabel_->setStyleSheet("color: orange;");
        return;
    }

    double value = testValueSpin_->value();
    tool_->setInputValue(key, value);

    Algorithm::ToolResult result;
    if (tool_->process(nullptr, result)) {
        testResultLabel_->setText(tr("输出成功"));
        testResultLabel_->setStyleSheet("color: green;");
        LOG_INFO(QString("PLC测试输出成功: %1 = %2").arg(key).arg(value));
    } else {
        testResultLabel_->setText(tr("输出失败: ") + result.errorMessage);
        testResultLabel_->setStyleSheet("color: red;");
    }
}

void PLCOutputToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void PLCOutputToolDialog::onCancelClicked()
{
    reject();
}

void PLCOutputToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void PLCOutputToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setOutputResult(outputResultCheck_->isChecked());
    tool_->setResultAddress(resultAddressSpin_->value());
    tool_->setOutputComplete(outputCompleteCheck_->isChecked());
    tool_->setCompleteAddress(completeAddressSpin_->value());

    syncMappingsFromTable();
}

void PLCOutputToolDialog::updateMappingTable()
{
    if (!tool_) return;

    mappingTable_->blockSignals(true);
    mappingTable_->setRowCount(0);

    const auto& mappings = tool_->mappings();
    for (size_t i = 0; i < mappings.size(); ++i) {
        const auto& mapping = mappings[i];
        int row = mappingTable_->rowCount();
        mappingTable_->insertRow(row);

        mappingTable_->setItem(row, 0, new QTableWidgetItem(mapping.name));
        mappingTable_->setItem(row, 1, new QTableWidgetItem(mapping.sourceKey));
        mappingTable_->setItem(row, 2, new QTableWidgetItem(QString::number(mapping.address)));

        auto* bitItem = new QTableWidgetItem();
        bitItem->setCheckState(mapping.isBit ? Qt::Checked : Qt::Unchecked);
        mappingTable_->setItem(row, 3, bitItem);

        mappingTable_->setItem(row, 4, new QTableWidgetItem(QString::number(mapping.scale)));
        mappingTable_->setItem(row, 5, new QTableWidgetItem(QString::number(mapping.offset)));
    }

    mappingTable_->blockSignals(false);
}

void PLCOutputToolDialog::syncMappingsFromTable()
{
    if (!tool_) return;

    tool_->clearMappings();

    for (int row = 0; row < mappingTable_->rowCount(); ++row) {
        Algorithm::OutputMapping mapping;

        if (mappingTable_->item(row, 0))
            mapping.name = mappingTable_->item(row, 0)->text();
        if (mappingTable_->item(row, 1))
            mapping.sourceKey = mappingTable_->item(row, 1)->text();
        if (mappingTable_->item(row, 2))
            mapping.address = mappingTable_->item(row, 2)->text().toInt();
        if (mappingTable_->item(row, 3))
            mapping.isBit = mappingTable_->item(row, 3)->checkState() == Qt::Checked;
        if (mappingTable_->item(row, 4))
            mapping.scale = mappingTable_->item(row, 4)->text().toDouble();
        if (mappingTable_->item(row, 5))
            mapping.offset = mappingTable_->item(row, 5)->text().toInt();

        mapping.enabled = true;
        tool_->addMapping(mapping);
    }
}

} // namespace UI
} // namespace VisionForge
