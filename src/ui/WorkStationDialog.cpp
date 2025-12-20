/**
 * @file WorkStationDialog.cpp
 * @brief 工位配置对话框实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "ui/WorkStationDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QListWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

namespace VisionForge {
namespace UI {

WorkStationDialog::WorkStationDialog(QWidget* parent)
    : QDialog(parent)
    , workStation_(nullptr)
    , selectedOffsetIndex_(-1)
{
    setupUI();
    setWindowTitle(tr("工位配置"));
    resize(900, 700);
}

WorkStationDialog::~WorkStationDialog()
{
}

void WorkStationDialog::setWorkStation(Core::WorkStation* ws)
{
    workStation_ = ws;
    updateFromWorkStation();
}

void WorkStationDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);

    // 创建标签页
    tabWidget_ = new QTabWidget;
    mainLayout->addWidget(tabWidget_);

    createPositionTab();
    createAlignmentTab();
    createOffsetTab();
    createButtonGroup();
}

void WorkStationDialog::createPositionTab()
{
    positionTab_ = new QWidget;
    auto* layout = new QHBoxLayout(positionTab_);

    // 左侧：位置列表
    auto* leftLayout = new QVBoxLayout;

    positionTable_ = new QTableWidget;
    positionTable_->setColumnCount(5);
    positionTable_->setHorizontalHeaderLabels({tr("名称"), tr("类型"), tr("相机"), tr("索引"), tr("启用")});
    positionTable_->horizontalHeader()->setStretchLastSection(true);
    positionTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    positionTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(positionTable_, &QTableWidget::itemSelectionChanged, this, &WorkStationDialog::onPositionSelectionChanged);
    leftLayout->addWidget(positionTable_);

    // 位置操作按钮
    auto* posButtonLayout = new QHBoxLayout;
    addTargetBtn_ = new QPushButton(tr("添加Target"));
    addObjectBtn_ = new QPushButton(tr("添加Object"));
    removePositionBtn_ = new QPushButton(tr("删除"));
    editPositionBtn_ = new QPushButton(tr("编辑"));

    connect(addTargetBtn_, &QPushButton::clicked, this, &WorkStationDialog::onAddTargetPosition);
    connect(addObjectBtn_, &QPushButton::clicked, this, &WorkStationDialog::onAddObjectPosition);
    connect(removePositionBtn_, &QPushButton::clicked, this, &WorkStationDialog::onRemovePosition);
    connect(editPositionBtn_, &QPushButton::clicked, this, &WorkStationDialog::onEditPosition);

    posButtonLayout->addWidget(addTargetBtn_);
    posButtonLayout->addWidget(addObjectBtn_);
    posButtonLayout->addWidget(removePositionBtn_);
    posButtonLayout->addWidget(editPositionBtn_);
    posButtonLayout->addStretch();
    leftLayout->addLayout(posButtonLayout);

    layout->addLayout(leftLayout, 2);

    // 右侧：位置详情
    auto* rightLayout = new QVBoxLayout;

    // 基本信息
    positionDetailGroup_ = new QGroupBox(tr("位置详情"));
    auto* detailForm = new QFormLayout(positionDetailGroup_);

    posNameEdit_ = new QLineEdit;
    posTypeCombo_ = new QComboBox;
    posTypeCombo_->addItem(tr("Target"), static_cast<int>(Core::PositionType::Target));
    posTypeCombo_->addItem(tr("Object"), static_cast<int>(Core::PositionType::Object));
    posCameraCombo_ = new QComboBox;
    posCalibCombo_ = new QComboBox;
    posIndexSpin_ = new QSpinBox;
    posIndexSpin_->setRange(0, 99);
    posEnabledCheck_ = new QCheckBox(tr("启用"));
    posEnabledCheck_->setChecked(true);

    detailForm->addRow(tr("名称:"), posNameEdit_);
    detailForm->addRow(tr("类型:"), posTypeCombo_);
    detailForm->addRow(tr("相机:"), posCameraCombo_);
    detailForm->addRow(tr("标定:"), posCalibCombo_);
    detailForm->addRow(tr("索引:"), posIndexSpin_);
    detailForm->addRow(posEnabledCheck_);

    rightLayout->addWidget(positionDetailGroup_);

    // 平台坐标
    platPosGroup_ = new QGroupBox(tr("平台坐标"));
    auto* platForm = new QFormLayout(platPosGroup_);

    platXSpin_ = new QDoubleSpinBox;
    platXSpin_->setRange(-99999, 99999);
    platXSpin_->setDecimals(3);
    platXSpin_->setSuffix(" mm");

    platYSpin_ = new QDoubleSpinBox;
    platYSpin_->setRange(-99999, 99999);
    platYSpin_->setDecimals(3);
    platYSpin_->setSuffix(" mm");

    platDSpin_ = new QDoubleSpinBox;
    platDSpin_->setRange(-360, 360);
    platDSpin_->setDecimals(3);
    platDSpin_->setSuffix(" °");

    platForm->addRow(tr("X:"), platXSpin_);
    platForm->addRow(tr("Y:"), platYSpin_);
    platForm->addRow(tr("D(角度):"), platDSpin_);

    rightLayout->addWidget(platPosGroup_);
    rightLayout->addStretch();

    layout->addLayout(rightLayout, 1);

    tabWidget_->addTab(positionTab_, tr("位置配置"));
}

void WorkStationDialog::createAlignmentTab()
{
    alignmentTab_ = new QWidget;
    auto* layout = new QVBoxLayout(alignmentTab_);

    // 对位方式
    alignMethodGroup_ = new QGroupBox(tr("对位方式"));
    auto* methodLayout = new QFormLayout(alignMethodGroup_);

    alignMethodCombo_ = new QComboBox;
    alignMethodCombo_->addItem(tr("1点对位 (1VS1)"), static_cast<int>(Core::AlignmentMethod::OnePoint));
    alignMethodCombo_->addItem(tr("2点对位 (2VS2)"), static_cast<int>(Core::AlignmentMethod::TwoPoint));
    alignMethodCombo_->addItem(tr("3点对位 (3VS3)"), static_cast<int>(Core::AlignmentMethod::ThreePoint));
    alignMethodCombo_->addItem(tr("4点对位 (4VS4)"), static_cast<int>(Core::AlignmentMethod::FourPoint));
    alignMethodCombo_->addItem(tr("多点对位"), static_cast<int>(Core::AlignmentMethod::MultiPoint));
    connect(alignMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WorkStationDialog::onAlignMethodChanged);

    iterativeAlignCheck_ = new QCheckBox(tr("启用迭代对位"));
    useHomographyCheck_ = new QCheckBox(tr("使用单应性矩阵"));

    methodLayout->addRow(tr("对位方式:"), alignMethodCombo_);
    methodLayout->addRow(iterativeAlignCheck_);
    methodLayout->addRow(useHomographyCheck_);

    layout->addWidget(alignMethodGroup_);

    // 精度要求
    precisionGroup_ = new QGroupBox(tr("精度要求"));
    auto* precisionLayout = new QFormLayout(precisionGroup_);

    precisionXSpin_ = new QDoubleSpinBox;
    precisionXSpin_->setRange(0, 100);
    precisionXSpin_->setDecimals(4);
    precisionXSpin_->setSuffix(" mm");
    precisionXSpin_->setValue(0.01);

    precisionYSpin_ = new QDoubleSpinBox;
    precisionYSpin_->setRange(0, 100);
    precisionYSpin_->setDecimals(4);
    precisionYSpin_->setSuffix(" mm");
    precisionYSpin_->setValue(0.01);

    precisionDSpin_ = new QDoubleSpinBox;
    precisionDSpin_->setRange(0, 10);
    precisionDSpin_->setDecimals(4);
    precisionDSpin_->setSuffix(" °");
    precisionDSpin_->setValue(0.01);

    maxIterationsSpin_ = new QSpinBox;
    maxIterationsSpin_->setRange(1, 100);
    maxIterationsSpin_->setValue(5);

    connect(precisionXSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &WorkStationDialog::onPrecisionChanged);
    connect(precisionYSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &WorkStationDialog::onPrecisionChanged);
    connect(precisionDSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &WorkStationDialog::onPrecisionChanged);

    precisionLayout->addRow(tr("X精度:"), precisionXSpin_);
    precisionLayout->addRow(tr("Y精度:"), precisionYSpin_);
    precisionLayout->addRow(tr("角度精度:"), precisionDSpin_);
    precisionLayout->addRow(tr("最大迭代次数:"), maxIterationsSpin_);

    layout->addWidget(precisionGroup_);

    // 位置分配
    positionAssignGroup_ = new QGroupBox(tr("位置分配"));
    auto* assignLayout = new QHBoxLayout(positionAssignGroup_);

    auto* targetLayout = new QVBoxLayout;
    targetLayout->addWidget(new QLabel(tr("Target位置:")));
    targetPosList_ = new QListWidget;
    targetPosList_->setSelectionMode(QAbstractItemView::MultiSelection);
    targetLayout->addWidget(targetPosList_);

    auto* objectLayout = new QVBoxLayout;
    objectLayout->addWidget(new QLabel(tr("Object位置:")));
    objectPosList_ = new QListWidget;
    objectPosList_->setSelectionMode(QAbstractItemView::MultiSelection);
    objectLayout->addWidget(objectPosList_);

    assignLayout->addLayout(targetLayout);
    assignLayout->addLayout(objectLayout);

    layout->addWidget(positionAssignGroup_);
    layout->addStretch();

    tabWidget_->addTab(alignmentTab_, tr("对位配置"));
}

void WorkStationDialog::createOffsetTab()
{
    offsetTab_ = new QWidget;
    auto* layout = new QHBoxLayout(offsetTab_);

    // 左侧：补偿列表
    auto* leftLayout = new QVBoxLayout;

    // 当前补偿选择
    auto* currentLayout = new QHBoxLayout;
    currentLayout->addWidget(new QLabel(tr("当前补偿组:")));
    currentOffsetCombo_ = new QComboBox;
    connect(currentOffsetCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WorkStationDialog::onCurrentOffsetChanged);
    currentLayout->addWidget(currentOffsetCombo_, 1);
    leftLayout->addLayout(currentLayout);

    offsetTable_ = new QTableWidget;
    offsetTable_->setColumnCount(4);
    offsetTable_->setHorizontalHeaderLabels({tr("索引"), tr("X偏移"), tr("Y偏移"), tr("D偏移")});
    offsetTable_->horizontalHeader()->setStretchLastSection(true);
    offsetTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    offsetTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(offsetTable_, &QTableWidget::itemSelectionChanged, this, &WorkStationDialog::onOffsetSelectionChanged);
    leftLayout->addWidget(offsetTable_);

    // 补偿操作按钮
    auto* offsetButtonLayout = new QHBoxLayout;
    addOffsetBtn_ = new QPushButton(tr("添加"));
    removeOffsetBtn_ = new QPushButton(tr("删除"));
    editOffsetBtn_ = new QPushButton(tr("编辑"));

    connect(addOffsetBtn_, &QPushButton::clicked, this, &WorkStationDialog::onAddOffset);
    connect(removeOffsetBtn_, &QPushButton::clicked, this, &WorkStationDialog::onRemoveOffset);
    connect(editOffsetBtn_, &QPushButton::clicked, this, &WorkStationDialog::onEditOffset);

    offsetButtonLayout->addWidget(addOffsetBtn_);
    offsetButtonLayout->addWidget(removeOffsetBtn_);
    offsetButtonLayout->addWidget(editOffsetBtn_);
    offsetButtonLayout->addStretch();
    leftLayout->addLayout(offsetButtonLayout);

    layout->addLayout(leftLayout, 2);

    // 右侧：补偿详情
    auto* rightLayout = new QVBoxLayout;

    offsetDetailGroup_ = new QGroupBox(tr("补偿详情"));
    auto* detailForm = new QFormLayout(offsetDetailGroup_);

    offsetXSpin_ = new QDoubleSpinBox;
    offsetXSpin_->setRange(-9999, 9999);
    offsetXSpin_->setDecimals(4);
    offsetXSpin_->setSuffix(" mm");

    offsetYSpin_ = new QDoubleSpinBox;
    offsetYSpin_->setRange(-9999, 9999);
    offsetYSpin_->setDecimals(4);
    offsetYSpin_->setSuffix(" mm");

    offsetDSpin_ = new QDoubleSpinBox;
    offsetDSpin_->setRange(-360, 360);
    offsetDSpin_->setDecimals(4);
    offsetDSpin_->setSuffix(" °");

    offsetDescEdit_ = new QLineEdit;

    detailForm->addRow(tr("X偏移:"), offsetXSpin_);
    detailForm->addRow(tr("Y偏移:"), offsetYSpin_);
    detailForm->addRow(tr("D偏移:"), offsetDSpin_);
    detailForm->addRow(tr("说明:"), offsetDescEdit_);

    rightLayout->addWidget(offsetDetailGroup_);
    rightLayout->addStretch();

    layout->addLayout(rightLayout, 1);

    tabWidget_->addTab(offsetTab_, tr("补偿配置"));
}

void WorkStationDialog::createButtonGroup()
{
    auto* buttonLayout = new QHBoxLayout;

    saveConfigBtn_ = new QPushButton(tr("保存配置"));
    loadConfigBtn_ = new QPushButton(tr("加载配置"));
    applyBtn_ = new QPushButton(tr("应用"));
    okBtn_ = new QPushButton(tr("确定"));
    cancelBtn_ = new QPushButton(tr("取消"));

    connect(saveConfigBtn_, &QPushButton::clicked, this, &WorkStationDialog::onSaveConfig);
    connect(loadConfigBtn_, &QPushButton::clicked, this, &WorkStationDialog::onLoadConfig);
    connect(applyBtn_, &QPushButton::clicked, this, &WorkStationDialog::onApply);
    connect(okBtn_, &QPushButton::clicked, this, [this]() {
        onApply();
        accept();
    });
    connect(cancelBtn_, &QPushButton::clicked, this, &QDialog::reject);

    buttonLayout->addWidget(saveConfigBtn_);
    buttonLayout->addWidget(loadConfigBtn_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyBtn_);
    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);

    static_cast<QVBoxLayout*>(layout())->addLayout(buttonLayout);
}

void WorkStationDialog::updatePositionTable()
{
    positionTable_->setRowCount(0);
    if (!workStation_) return;

    auto positions = workStation_->getAllPositions();
    for (const auto& pos : positions) {
        int row = positionTable_->rowCount();
        positionTable_->insertRow(row);

        auto* nameItem = new QTableWidgetItem(pos.name);
        nameItem->setData(Qt::UserRole, pos.id);
        positionTable_->setItem(row, 0, nameItem);
        positionTable_->setItem(row, 1, new QTableWidgetItem(getPositionTypeText(pos.type)));
        positionTable_->setItem(row, 2, new QTableWidgetItem(pos.cameraId));
        positionTable_->setItem(row, 3, new QTableWidgetItem(QString::number(pos.positionIndex)));

        auto* enableItem = new QTableWidgetItem();
        enableItem->setCheckState(pos.enabled ? Qt::Checked : Qt::Unchecked);
        positionTable_->setItem(row, 4, enableItem);
    }
}

void WorkStationDialog::updatePositionDetails()
{
    if (!workStation_ || selectedPositionId_.isEmpty()) {
        posNameEdit_->clear();
        posTypeCombo_->setCurrentIndex(0);
        posCameraCombo_->setCurrentIndex(-1);
        posCalibCombo_->setCurrentIndex(-1);
        posIndexSpin_->setValue(0);
        posEnabledCheck_->setChecked(true);
        platXSpin_->setValue(0);
        platYSpin_->setValue(0);
        platDSpin_->setValue(0);
        return;
    }

    auto pos = workStation_->getPosition(selectedPositionId_);
    posNameEdit_->setText(pos.name);
    posTypeCombo_->setCurrentIndex(static_cast<int>(pos.type));

    // 设置相机下拉框
    int cameraIndex = posCameraCombo_->findText(pos.cameraId);
    posCameraCombo_->setCurrentIndex(cameraIndex);

    // 设置标定下拉框
    int calibIndex = posCalibCombo_->findText(pos.calibrationId);
    posCalibCombo_->setCurrentIndex(calibIndex);

    posIndexSpin_->setValue(pos.positionIndex);
    posEnabledCheck_->setChecked(pos.enabled);
    platXSpin_->setValue(pos.platX);
    platYSpin_->setValue(pos.platY);
    platDSpin_->setValue(pos.platD);
}

void WorkStationDialog::updateOffsetTable()
{
    offsetTable_->setRowCount(0);
    if (!workStation_) return;

    auto offsets = workStation_->getAlignmentConfig().offsets;
    for (int i = 0; i < offsets.size(); ++i) {
        const auto& offset = offsets[i];
        int row = offsetTable_->rowCount();
        offsetTable_->insertRow(row);

        offsetTable_->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
        offsetTable_->setItem(row, 1, new QTableWidgetItem(QString::number(offset.offsetX, 'f', 4)));
        offsetTable_->setItem(row, 2, new QTableWidgetItem(QString::number(offset.offsetY, 'f', 4)));
        offsetTable_->setItem(row, 3, new QTableWidgetItem(QString::number(offset.offsetD, 'f', 4)));
    }

    // 更新当前补偿组下拉框
    currentOffsetCombo_->clear();
    for (int i = 0; i < offsets.size(); ++i) {
        currentOffsetCombo_->addItem(tr("补偿组 %1").arg(i));
    }
    currentOffsetCombo_->setCurrentIndex(workStation_->getAlignmentConfig().currentOffsetIndex);
}

void WorkStationDialog::updateFromWorkStation()
{
    if (!workStation_) return;

    updatePositionTable();
    updateOffsetTable();

    // 更新对位配置
    auto config = workStation_->getAlignmentConfig();
    alignMethodCombo_->setCurrentIndex(static_cast<int>(config.method));
    iterativeAlignCheck_->setChecked(config.enableIterativeAlign);
    useHomographyCheck_->setChecked(config.useHomography);

    precisionXSpin_->setValue(config.precision.precisionX);
    precisionYSpin_->setValue(config.precision.precisionY);
    precisionDSpin_->setValue(config.precision.precisionD);
    maxIterationsSpin_->setValue(config.precision.maxIterations);

    // 更新位置分配列表
    targetPosList_->clear();
    objectPosList_->clear();

    for (const auto& pos : workStation_->getAllPositions()) {
        if (pos.type == Core::PositionType::Target) {
            auto* item = new QListWidgetItem(pos.name);
            item->setData(Qt::UserRole, pos.id);
            item->setCheckState(config.targetPositionIds.contains(pos.id) ? Qt::Checked : Qt::Unchecked);
            targetPosList_->addItem(item);
        } else {
            auto* item = new QListWidgetItem(pos.name);
            item->setData(Qt::UserRole, pos.id);
            item->setCheckState(config.objectPositionIds.contains(pos.id) ? Qt::Checked : Qt::Unchecked);
            objectPosList_->addItem(item);
        }
    }
}

void WorkStationDialog::applyToWorkStation()
{
    if (!workStation_) return;

    // 更新对位配置
    Core::AlignmentConfig config = workStation_->getAlignmentConfig();
    config.method = static_cast<Core::AlignmentMethod>(
        alignMethodCombo_->currentData().toInt());
    config.enableIterativeAlign = iterativeAlignCheck_->isChecked();
    config.useHomography = useHomographyCheck_->isChecked();

    config.precision.precisionX = precisionXSpin_->value();
    config.precision.precisionY = precisionYSpin_->value();
    config.precision.precisionD = precisionDSpin_->value();
    config.precision.maxIterations = maxIterationsSpin_->value();

    // 收集选中的位置
    config.targetPositionIds.clear();
    for (int i = 0; i < targetPosList_->count(); ++i) {
        auto* item = targetPosList_->item(i);
        if (item->checkState() == Qt::Checked) {
            config.targetPositionIds.append(item->data(Qt::UserRole).toString());
        }
    }

    config.objectPositionIds.clear();
    for (int i = 0; i < objectPosList_->count(); ++i) {
        auto* item = objectPosList_->item(i);
        if (item->checkState() == Qt::Checked) {
            config.objectPositionIds.append(item->data(Qt::UserRole).toString());
        }
    }

    workStation_->setAlignmentConfig(config);

    emit configurationChanged();
}

QString WorkStationDialog::getPositionTypeText(Core::PositionType type) const
{
    switch (type) {
        case Core::PositionType::Target: return tr("Target");
        case Core::PositionType::Object: return tr("Object");
        default: return tr("未知");
    }
}

QString WorkStationDialog::getAlignMethodText(Core::AlignmentMethod method) const
{
    switch (method) {
        case Core::AlignmentMethod::OnePoint: return tr("1点对位");
        case Core::AlignmentMethod::TwoPoint: return tr("2点对位");
        case Core::AlignmentMethod::ThreePoint: return tr("3点对位");
        case Core::AlignmentMethod::FourPoint: return tr("4点对位");
        case Core::AlignmentMethod::MultiPoint: return tr("多点对位");
        default: return tr("未知");
    }
}

// 位置管理槽函数
void WorkStationDialog::onAddTargetPosition()
{
    if (!workStation_) return;

    Core::PositionConfig config;
    config.name = tr("Target_%1").arg(workStation_->positionCount());
    config.type = Core::PositionType::Target;
    config.enabled = true;
    config.positionIndex = workStation_->positionCount();

    workStation_->addPosition(config);
    updatePositionTable();
    updateFromWorkStation();
}

void WorkStationDialog::onAddObjectPosition()
{
    if (!workStation_) return;

    Core::PositionConfig config;
    config.name = tr("Object_%1").arg(workStation_->positionCount());
    config.type = Core::PositionType::Object;
    config.enabled = true;
    config.positionIndex = workStation_->positionCount();

    workStation_->addPosition(config);
    updatePositionTable();
    updateFromWorkStation();
}

void WorkStationDialog::onRemovePosition()
{
    if (!workStation_ || selectedPositionId_.isEmpty()) return;

    auto result = QMessageBox::question(this, tr("确认删除"),
        tr("确定要删除选中的位置吗？"), QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        workStation_->removePosition(selectedPositionId_);
        selectedPositionId_.clear();
        updatePositionTable();
        updateFromWorkStation();
    }
}

void WorkStationDialog::onEditPosition()
{
    if (!workStation_ || selectedPositionId_.isEmpty()) return;

    // 从UI更新位置配置
    Core::PositionConfig config = workStation_->getPosition(selectedPositionId_);
    config.name = posNameEdit_->text();
    config.type = static_cast<Core::PositionType>(posTypeCombo_->currentData().toInt());
    config.cameraId = posCameraCombo_->currentText();
    config.calibrationId = posCalibCombo_->currentText();
    config.positionIndex = posIndexSpin_->value();
    config.enabled = posEnabledCheck_->isChecked();
    config.platX = platXSpin_->value();
    config.platY = platYSpin_->value();
    config.platD = platDSpin_->value();

    workStation_->updatePosition(selectedPositionId_, config);
    updatePositionTable();
    updateFromWorkStation();
}

void WorkStationDialog::onPositionSelectionChanged()
{
    auto items = positionTable_->selectedItems();
    if (items.isEmpty()) {
        selectedPositionId_.clear();
    } else {
        int row = items.first()->row();
        selectedPositionId_ = positionTable_->item(row, 0)->data(Qt::UserRole).toString();
    }
    updatePositionDetails();
}

// 对位配置槽函数
void WorkStationDialog::onAlignMethodChanged(int index)
{
    Q_UNUSED(index);
    // 可以根据对位方法启用/禁用某些选项
}

void WorkStationDialog::onPrecisionChanged()
{
    // 实时更新精度预览或验证
}

// 补偿管理槽函数
void WorkStationDialog::onAddOffset()
{
    if (!workStation_) return;

    Core::AlignmentOffset offset;
    offset.description = tr("补偿组 %1").arg(workStation_->getAlignmentConfig().offsets.size());

    auto config = workStation_->getAlignmentConfig();
    config.offsets.append(offset);
    workStation_->setAlignmentConfig(config);

    updateOffsetTable();
}

void WorkStationDialog::onRemoveOffset()
{
    if (!workStation_ || selectedOffsetIndex_ < 0) return;

    auto result = QMessageBox::question(this, tr("确认删除"),
        tr("确定要删除选中的补偿组吗？"), QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        auto config = workStation_->getAlignmentConfig();
        if (selectedOffsetIndex_ < config.offsets.size()) {
            config.offsets.removeAt(selectedOffsetIndex_);
            if (config.currentOffsetIndex >= config.offsets.size()) {
                config.currentOffsetIndex = config.offsets.size() - 1;
            }
            workStation_->setAlignmentConfig(config);
            selectedOffsetIndex_ = -1;
            updateOffsetTable();
        }
    }
}

void WorkStationDialog::onEditOffset()
{
    if (!workStation_ || selectedOffsetIndex_ < 0) return;

    auto config = workStation_->getAlignmentConfig();
    if (selectedOffsetIndex_ < config.offsets.size()) {
        config.offsets[selectedOffsetIndex_].offsetX = offsetXSpin_->value();
        config.offsets[selectedOffsetIndex_].offsetY = offsetYSpin_->value();
        config.offsets[selectedOffsetIndex_].offsetD = offsetDSpin_->value();
        config.offsets[selectedOffsetIndex_].description = offsetDescEdit_->text();
        workStation_->setAlignmentConfig(config);
        updateOffsetTable();
    }
}

void WorkStationDialog::onOffsetSelectionChanged()
{
    auto items = offsetTable_->selectedItems();
    if (items.isEmpty()) {
        selectedOffsetIndex_ = -1;
        offsetXSpin_->setValue(0);
        offsetYSpin_->setValue(0);
        offsetDSpin_->setValue(0);
        offsetDescEdit_->clear();
    } else {
        selectedOffsetIndex_ = items.first()->row();
        auto config = workStation_->getAlignmentConfig();
        if (selectedOffsetIndex_ < config.offsets.size()) {
            const auto& offset = config.offsets[selectedOffsetIndex_];
            offsetXSpin_->setValue(offset.offsetX);
            offsetYSpin_->setValue(offset.offsetY);
            offsetDSpin_->setValue(offset.offsetD);
            offsetDescEdit_->setText(offset.description);
        }
    }
}

void WorkStationDialog::onCurrentOffsetChanged(int index)
{
    if (!workStation_ || index < 0) return;

    auto config = workStation_->getAlignmentConfig();
    config.currentOffsetIndex = index;
    workStation_->setAlignmentConfig(config);
}

// 操作槽函数
void WorkStationDialog::onApply()
{
    applyToWorkStation();
}

void WorkStationDialog::onSaveConfig()
{
    if (!workStation_) return;

    QString filename = QFileDialog::getSaveFileName(this,
        tr("保存工位配置"), QString(), tr("JSON文件 (*.json)"));

    if (!filename.isEmpty()) {
        if (workStation_->saveToFile(filename)) {
            QMessageBox::information(this, tr("保存成功"), tr("工位配置已保存到文件。"));
        } else {
            QMessageBox::warning(this, tr("保存失败"), tr("无法保存工位配置。"));
        }
    }
}

void WorkStationDialog::onLoadConfig()
{
    if (!workStation_) return;

    QString filename = QFileDialog::getOpenFileName(this,
        tr("加载工位配置"), QString(), tr("JSON文件 (*.json)"));

    if (!filename.isEmpty()) {
        if (workStation_->loadFromFile(filename)) {
            updateFromWorkStation();
            QMessageBox::information(this, tr("加载成功"), tr("工位配置已加载。"));
        } else {
            QMessageBox::warning(this, tr("加载失败"), tr("无法加载工位配置。"));
        }
    }
}

} // namespace UI
} // namespace VisionForge
