/**
 * @file StationConfigTool.cpp
 * @brief 工位配置工具实现
 */

#include "ui/StationConfigTool.h"
#include "core/MultiStationManager.h"
#include "core/StationConfig.h"
#include "platform/PlatformTypes.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QMessageBox>
#include <QHeaderView>
#include <QApplication>

namespace VisionForge {
namespace UI {

// ============================================================
// 构造与析构
// ============================================================

StationConfigTool::StationConfigTool(QWidget* parent)
    : QDialog(parent)
    , treeWidget_(nullptr)
    , rootItem_(nullptr)
    , settingsGroup_(nullptr)
    , comboPlatformIndex_(nullptr)
    , comboPlatformType_(nullptr)
    , comboCameraCount_(nullptr)
    , checkSharedCamera_(nullptr)
    , comboSceneCount_(nullptr)
    , comboPositionCount_(nullptr)
    , comboDisplayMode_(nullptr)
    , comboProductCount_(nullptr)
    , comboPreloadCount_(nullptr)
    , comboFunctionMode_(nullptr)
    , detectionGroup_(nullptr)
    , spinDetectionConfidence_(nullptr)
    , spinDetectionNms_(nullptr)
    , alignmentGroup_(nullptr)
    , spinAlignToleranceX_(nullptr)
    , spinAlignToleranceY_(nullptr)
    , spinAlignToleranceD_(nullptr)
    , uvwConfigGroup_(nullptr)
    , comboUVWStructure_(nullptr)
    , comboUVWSlider_(nullptr)
    , comboUVWPrecision_(nullptr)
    , spinNode1X_(nullptr)
    , spinNode1Y_(nullptr)
    , spinNode2X_(nullptr)
    , spinNode2Y_(nullptr)
    , spinNode3X_(nullptr)
    , spinNode3Y_(nullptr)
    , spinTravelXY_(nullptr)
    , spinTravelTheta_(nullptr)
    , btnAddPlatform_(nullptr)
    , btnDeletePlatform_(nullptr)
    , btnSaveExit_(nullptr)
    , btnCancelExit_(nullptr)
    , currentPlatformIndex_(-1)
    , hasChanges_(false)
    , saveAndExit_(false)
    , isUpdating_(false)
{
    setWindowTitle(tr("VisionForge 工位配置工具"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumSize(800, 600);
    resize(950, 700);

    setupUI();
    setupConnections();
    loadExistingConfig();

    // 如果没有配置，创建一个默认平台
    if (platforms_.isEmpty()) {
        onAddPlatform();
        hasChanges_ = false;  // 首次创建不算修改
    }

    // 选中第一个平台
    if (!platforms_.isEmpty() && rootItem_ && rootItem_->childCount() > 0) {
        treeWidget_->setCurrentItem(rootItem_->child(0));
        onTreeItemClicked(rootItem_->child(0), 0);
    }
}

StationConfigTool::~StationConfigTool()
{
}

int StationConfigTool::getStationCount() const
{
    return platforms_.size();
}

// ============================================================
// UI 初始化
// ============================================================

void StationConfigTool::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 创建分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // 左侧树形区域
    QWidget* treeArea = createTreeArea();
    splitter->addWidget(treeArea);

    // 右侧设置区域和按钮
    QWidget* rightArea = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);

    // 创建滚动区域包装设置面板
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);

    // 设置面板
    QGroupBox* settingsArea = createSettingsArea();
    scrollArea->setWidget(settingsArea);
    rightLayout->addWidget(scrollArea, 1);  // 给滚动区域更多空间

    // 按钮区域
    QWidget* buttonArea = createButtonArea();
    rightLayout->addWidget(buttonArea);

    // 底部保存/取消按钮
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    btnSaveExit_ = new QPushButton(tr("保存退出"), this);
    btnCancelExit_ = new QPushButton(tr("取消退出"), this);
    btnSaveExit_->setMinimumWidth(80);
    btnCancelExit_->setMinimumWidth(80);
    bottomLayout->addWidget(btnSaveExit_);
    bottomLayout->addWidget(btnCancelExit_);
    rightLayout->addLayout(bottomLayout);

    splitter->addWidget(rightArea);

    // 设置分割比例
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    splitter->setSizes({300, 500});

    mainLayout->addWidget(splitter);
}

QWidget* StationConfigTool::createTreeArea()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    treeWidget_ = new QTreeWidget(widget);
    treeWidget_->setHeaderHidden(true);
    treeWidget_->setIndentation(20);
    treeWidget_->setAnimated(true);

    // 创建根节点
    rootItem_ = new QTreeWidgetItem(treeWidget_);
    rootItem_->setText(0, tr("视觉检测系统"));
    rootItem_->setExpanded(true);
    rootItem_->setFlags(rootItem_->flags() & ~Qt::ItemIsSelectable);

    layout->addWidget(treeWidget_);
    return widget;
}

QGroupBox* StationConfigTool::createSettingsArea()
{
    settingsGroup_ = new QGroupBox(tr("平台设置"), this);
    QGridLayout* grid = new QGridLayout(settingsGroup_);
    grid->setContentsMargins(15, 20, 15, 15);
    grid->setHorizontalSpacing(15);
    grid->setVerticalSpacing(12);

    int row = 0;

    // 平台序
    QLabel* labelPlatformIndex = new QLabel(tr("平台序:"), settingsGroup_);
    comboPlatformIndex_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 6; ++i) {
        comboPlatformIndex_->addItem(QString::number(i), i);
    }
    comboPlatformIndex_->setMinimumWidth(120);
    grid->addWidget(labelPlatformIndex, row, 0);
    grid->addWidget(comboPlatformIndex_, row, 1);
    ++row;

    // 平台类型
    QLabel* labelPlatformType = new QLabel(tr("平台类型:"), settingsGroup_);
    comboPlatformType_ = new QComboBox(settingsGroup_);
    // 添加常用平台类型
    comboPlatformType_->addItem(tr("XYD (标准三轴)"), static_cast<int>(Platform::PlatformType::XYD));
    comboPlatformType_->addItem(tr("XY (双轴)"), static_cast<int>(Platform::PlatformType::XY));
    comboPlatformType_->addItem(tr("X1X2Y (龙门双X)"), static_cast<int>(Platform::PlatformType::X1X2Y));
    comboPlatformType_->addItem(tr("XY1Y2 (双Y平台)"), static_cast<int>(Platform::PlatformType::XY1Y2));
    comboPlatformType_->addItem(tr("DXY (旋转优先)"), static_cast<int>(Platform::PlatformType::DXY));
    comboPlatformType_->addItem(tr("XD (X轴+旋转)"), static_cast<int>(Platform::PlatformType::XD));
    comboPlatformType_->addItem(tr("YD (Y轴+旋转)"), static_cast<int>(Platform::PlatformType::YD));
    comboPlatformType_->addItem(tr("XYDP (四轴)"), static_cast<int>(Platform::PlatformType::XYDP));
    // UVW精密对位平台系列
    comboPlatformType_->addItem(tr("UVW (精密对位)"), static_cast<int>(Platform::PlatformType::UVW));
    comboPlatformType_->addItem(tr("UVW-XXY (双X单Y)"), static_cast<int>(Platform::PlatformType::UVW_XXY));
    comboPlatformType_->addItem(tr("UVW-XYY (单X双Y)"), static_cast<int>(Platform::PlatformType::UVW_XYY));
    comboPlatformType_->addItem(tr("UVW-XXYY (四轴)"), static_cast<int>(Platform::PlatformType::UVW_XXYY));
    comboPlatformType_->addItem(tr("UVW-中空 (光学)"), static_cast<int>(Platform::PlatformType::UVW_HOLLOW));
    comboPlatformType_->addItem(tr("UVW-高精度 (2μm)"), static_cast<int>(Platform::PlatformType::UVW_HIGH_PRECISION));
    comboPlatformType_->setMinimumWidth(150);
    comboPlatformType_->setToolTip(tr("选择平台运动轴配置类型\nUVW系列适用于曝光机、贴合机、丝印机等精密对位场景"));
    grid->addWidget(labelPlatformType, row, 0);
    grid->addWidget(comboPlatformType_, row, 1);
    ++row;

    // 相机数
    QLabel* labelCameraCount = new QLabel(tr("相机数:"), settingsGroup_);
    comboCameraCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 8; ++i) {
        comboCameraCount_->addItem(QString::number(i), i);
    }
    comboCameraCount_->setMinimumWidth(120);
    checkSharedCamera_ = new QCheckBox(tr("相机是否共享系统"), settingsGroup_);
    grid->addWidget(labelCameraCount, row, 0);
    grid->addWidget(comboCameraCount_, row, 1);
    grid->addWidget(checkSharedCamera_, row, 2);
    ++row;

    // 场景数量
    QLabel* labelSceneCount = new QLabel(tr("场景数量:"), settingsGroup_);
    comboSceneCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 8; ++i) {
        comboSceneCount_->addItem(QString::number(i), i);
    }
    comboSceneCount_->setMinimumWidth(120);
    grid->addWidget(labelSceneCount, row, 0);
    grid->addWidget(comboSceneCount_, row, 1);
    ++row;

    // 位置数量
    QLabel* labelPositionCount = new QLabel(tr("位置数量:"), settingsGroup_);
    comboPositionCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 8; ++i) {
        comboPositionCount_->addItem(QString::number(i), i);
    }
    comboPositionCount_->setMinimumWidth(120);
    grid->addWidget(labelPositionCount, row, 0);
    grid->addWidget(comboPositionCount_, row, 1);
    ++row;

    // 显示模式
    QLabel* labelDisplayMode = new QLabel(tr("显示模式:"), settingsGroup_);
    comboDisplayMode_ = new QComboBox(settingsGroup_);
    comboDisplayMode_->addItem(tr("按照位置显示"), 0);
    comboDisplayMode_->addItem(tr("按照相机显示"), 1);
    comboDisplayMode_->setMinimumWidth(120);
    grid->addWidget(labelDisplayMode, row, 0);
    grid->addWidget(comboDisplayMode_, row, 1);
    ++row;

    // 产品数量
    QLabel* labelProductCount = new QLabel(tr("产品数量:"), settingsGroup_);
    comboProductCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 20; ++i) {
        comboProductCount_->addItem(QString::number(i), i);
    }
    comboProductCount_->setMinimumWidth(120);
    grid->addWidget(labelProductCount, row, 0);
    grid->addWidget(comboProductCount_, row, 1);
    ++row;

    // 产品预加载数量
    QLabel* labelPreloadCount = new QLabel(tr("产品预加载数量:"), settingsGroup_);
    comboPreloadCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 5; ++i) {
        comboPreloadCount_->addItem(QString::number(i), i);
    }
    comboPreloadCount_->setMinimumWidth(120);
    grid->addWidget(labelPreloadCount, row, 0);
    grid->addWidget(comboPreloadCount_, row, 1);
    ++row;

    // ========== 功能模式选择 ==========
    QLabel* labelFunctionMode = new QLabel(tr("功能模式:"), settingsGroup_);
    comboFunctionMode_ = new QComboBox(settingsGroup_);
    comboFunctionMode_->addItem(tr("仅检测"), 0);
    comboFunctionMode_->addItem(tr("仅对位"), 1);
    comboFunctionMode_->addItem(tr("检测+对位"), 2);
    comboFunctionMode_->setCurrentIndex(2);  // 默认选择检测+对位
    comboFunctionMode_->setMinimumWidth(120);
    grid->addWidget(labelFunctionMode, row, 0);
    grid->addWidget(comboFunctionMode_, row, 1);
    ++row;

    // ========== 检测参数组 ==========
    detectionGroup_ = new QGroupBox(tr("检测参数"), settingsGroup_);
    QGridLayout* detectionLayout = new QGridLayout(detectionGroup_);
    detectionLayout->setContentsMargins(10, 15, 10, 10);
    detectionLayout->setHorizontalSpacing(10);
    detectionLayout->setVerticalSpacing(8);

    QLabel* labelConfidence = new QLabel(tr("置信度阈值:"), detectionGroup_);
    spinDetectionConfidence_ = new QDoubleSpinBox(detectionGroup_);
    spinDetectionConfidence_->setRange(0.0, 1.0);
    spinDetectionConfidence_->setSingleStep(0.05);
    spinDetectionConfidence_->setDecimals(2);
    spinDetectionConfidence_->setValue(0.5);
    spinDetectionConfidence_->setMinimumWidth(100);
    detectionLayout->addWidget(labelConfidence, 0, 0);
    detectionLayout->addWidget(spinDetectionConfidence_, 0, 1);

    QLabel* labelNms = new QLabel(tr("NMS阈值:"), detectionGroup_);
    spinDetectionNms_ = new QDoubleSpinBox(detectionGroup_);
    spinDetectionNms_->setRange(0.0, 1.0);
    spinDetectionNms_->setSingleStep(0.05);
    spinDetectionNms_->setDecimals(2);
    spinDetectionNms_->setValue(0.4);
    spinDetectionNms_->setMinimumWidth(100);
    detectionLayout->addWidget(labelNms, 1, 0);
    detectionLayout->addWidget(spinDetectionNms_, 1, 1);

    grid->addWidget(detectionGroup_, row, 0, 1, 3);
    ++row;

    // ========== 对位参数组 ==========
    alignmentGroup_ = new QGroupBox(tr("对位参数"), settingsGroup_);
    QGridLayout* alignmentLayout = new QGridLayout(alignmentGroup_);
    alignmentLayout->setContentsMargins(10, 15, 10, 10);
    alignmentLayout->setHorizontalSpacing(10);
    alignmentLayout->setVerticalSpacing(8);

    QLabel* labelToleranceX = new QLabel(tr("X容差 (mm):"), alignmentGroup_);
    spinAlignToleranceX_ = new QDoubleSpinBox(alignmentGroup_);
    spinAlignToleranceX_->setRange(0.001, 10.0);
    spinAlignToleranceX_->setSingleStep(0.01);
    spinAlignToleranceX_->setDecimals(3);
    spinAlignToleranceX_->setValue(0.1);
    spinAlignToleranceX_->setMinimumWidth(100);
    alignmentLayout->addWidget(labelToleranceX, 0, 0);
    alignmentLayout->addWidget(spinAlignToleranceX_, 0, 1);

    QLabel* labelToleranceY = new QLabel(tr("Y容差 (mm):"), alignmentGroup_);
    spinAlignToleranceY_ = new QDoubleSpinBox(alignmentGroup_);
    spinAlignToleranceY_->setRange(0.001, 10.0);
    spinAlignToleranceY_->setSingleStep(0.01);
    spinAlignToleranceY_->setDecimals(3);
    spinAlignToleranceY_->setValue(0.1);
    spinAlignToleranceY_->setMinimumWidth(100);
    alignmentLayout->addWidget(labelToleranceY, 1, 0);
    alignmentLayout->addWidget(spinAlignToleranceY_, 1, 1);

    QLabel* labelToleranceD = new QLabel(tr("角度容差 (°):"), alignmentGroup_);
    spinAlignToleranceD_ = new QDoubleSpinBox(alignmentGroup_);
    spinAlignToleranceD_->setRange(0.001, 10.0);
    spinAlignToleranceD_->setSingleStep(0.01);
    spinAlignToleranceD_->setDecimals(3);
    spinAlignToleranceD_->setValue(0.1);
    spinAlignToleranceD_->setMinimumWidth(100);
    alignmentLayout->addWidget(labelToleranceD, 2, 0);
    alignmentLayout->addWidget(spinAlignToleranceD_, 2, 1);

    grid->addWidget(alignmentGroup_, row, 0, 1, 3);
    ++row;

    // ========== UVW节点配置组 ==========
    uvwConfigGroup_ = new QGroupBox(tr("UVW平台节点配置"), settingsGroup_);
    QGridLayout* uvwLayout = new QGridLayout(uvwConfigGroup_);
    uvwLayout->setContentsMargins(10, 15, 10, 10);
    uvwLayout->setHorizontalSpacing(10);
    uvwLayout->setVerticalSpacing(8);

    int uvwRow = 0;

    // 结构类型
    QLabel* labelStructure = new QLabel(tr("结构类型:"), uvwConfigGroup_);
    comboUVWStructure_ = new QComboBox(uvwConfigGroup_);
    comboUVWStructure_->addItem(tr("XXY (双X单Y)"), 0);
    comboUVWStructure_->addItem(tr("XYY (单X双Y)"), 1);
    comboUVWStructure_->setMinimumWidth(120);
    comboUVWStructure_->setToolTip(tr("选择UVW平台的机械结构类型\nXXY: 两个X轴+一个Y轴\nXYY: 一个X轴+两个Y轴"));
    uvwLayout->addWidget(labelStructure, uvwRow, 0);
    uvwLayout->addWidget(comboUVWStructure_, uvwRow, 1);
    ++uvwRow;

    // 滑轨类型
    QLabel* labelSlider = new QLabel(tr("滑轨类型:"), uvwConfigGroup_);
    comboUVWSlider_ = new QComboBox(uvwConfigGroup_);
    comboUVWSlider_->addItem(tr("固定滑轨"), 0);
    comboUVWSlider_->addItem(tr("移动滑轨"), 1);
    comboUVWSlider_->setMinimumWidth(120);
    comboUVWSlider_->setToolTip(tr("固定滑轨: 旋转轴连接点固定\n移动滑轨: 旋转轴连接点可移动"));
    uvwLayout->addWidget(labelSlider, uvwRow, 0);
    uvwLayout->addWidget(comboUVWSlider_, uvwRow, 1);
    ++uvwRow;

    // 精度等级
    QLabel* labelPrecision = new QLabel(tr("精度等级:"), uvwConfigGroup_);
    comboUVWPrecision_ = new QComboBox(uvwConfigGroup_);
    comboUVWPrecision_->addItem(tr("P级 (10μm)"), 0);
    comboUVWPrecision_->addItem(tr("B级 (5μm)"), 1);
    comboUVWPrecision_->addItem(tr("G级 (2μm)"), 2);
    comboUVWPrecision_->setMinimumWidth(120);
    uvwLayout->addWidget(labelPrecision, uvwRow, 0);
    uvwLayout->addWidget(comboUVWPrecision_, uvwRow, 1);
    ++uvwRow;

    // 分隔线
    QFrame* separator = new QFrame(uvwConfigGroup_);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("QFrame { color: #555555; }");
    uvwLayout->addWidget(separator, uvwRow, 0, 1, 4);
    ++uvwRow;

    // 节点位置标题
    QLabel* labelNodes = new QLabel(tr("节点位置 (mm):"), uvwConfigGroup_);
    labelNodes->setStyleSheet("font-weight: bold;");
    uvwLayout->addWidget(labelNodes, uvwRow, 0, 1, 4);
    ++uvwRow;

    // 节点1
    QLabel* labelNode1 = new QLabel(tr("节点1 (X1/Y1):"), uvwConfigGroup_);
    spinNode1X_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinNode1X_->setRange(-500.0, 500.0);
    spinNode1X_->setSingleStep(0.5);
    spinNode1X_->setDecimals(1);
    spinNode1X_->setValue(67.5);
    spinNode1X_->setPrefix("X: ");
    spinNode1X_->setMinimumWidth(90);
    spinNode1Y_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinNode1Y_->setRange(-500.0, 500.0);
    spinNode1Y_->setSingleStep(0.5);
    spinNode1Y_->setDecimals(1);
    spinNode1Y_->setValue(-57.0);
    spinNode1Y_->setPrefix("Y: ");
    spinNode1Y_->setMinimumWidth(90);
    uvwLayout->addWidget(labelNode1, uvwRow, 0);
    uvwLayout->addWidget(spinNode1X_, uvwRow, 1);
    uvwLayout->addWidget(spinNode1Y_, uvwRow, 2);
    ++uvwRow;

    // 节点2
    QLabel* labelNode2 = new QLabel(tr("节点2 (X2/Y2):"), uvwConfigGroup_);
    spinNode2X_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinNode2X_->setRange(-500.0, 500.0);
    spinNode2X_->setSingleStep(0.5);
    spinNode2X_->setDecimals(1);
    spinNode2X_->setValue(67.5);
    spinNode2X_->setPrefix("X: ");
    spinNode2X_->setMinimumWidth(90);
    spinNode2Y_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinNode2Y_->setRange(-500.0, 500.0);
    spinNode2Y_->setSingleStep(0.5);
    spinNode2Y_->setDecimals(1);
    spinNode2Y_->setValue(57.0);
    spinNode2Y_->setPrefix("Y: ");
    spinNode2Y_->setMinimumWidth(90);
    uvwLayout->addWidget(labelNode2, uvwRow, 0);
    uvwLayout->addWidget(spinNode2X_, uvwRow, 1);
    uvwLayout->addWidget(spinNode2Y_, uvwRow, 2);
    ++uvwRow;

    // 节点3
    QLabel* labelNode3 = new QLabel(tr("节点3 (Y/X):"), uvwConfigGroup_);
    spinNode3X_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinNode3X_->setRange(-500.0, 500.0);
    spinNode3X_->setSingleStep(0.5);
    spinNode3X_->setDecimals(1);
    spinNode3X_->setValue(-57.0);
    spinNode3X_->setPrefix("X: ");
    spinNode3X_->setMinimumWidth(90);
    spinNode3Y_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinNode3Y_->setRange(-500.0, 500.0);
    spinNode3Y_->setSingleStep(0.5);
    spinNode3Y_->setDecimals(1);
    spinNode3Y_->setValue(67.5);
    spinNode3Y_->setPrefix("Y: ");
    spinNode3Y_->setMinimumWidth(90);
    uvwLayout->addWidget(labelNode3, uvwRow, 0);
    uvwLayout->addWidget(spinNode3X_, uvwRow, 1);
    uvwLayout->addWidget(spinNode3Y_, uvwRow, 2);
    ++uvwRow;

    // 行程范围
    QLabel* labelTravel = new QLabel(tr("行程范围:"), uvwConfigGroup_);
    spinTravelXY_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinTravelXY_->setRange(0.1, 50.0);
    spinTravelXY_->setSingleStep(0.5);
    spinTravelXY_->setDecimals(1);
    spinTravelXY_->setValue(5.0);
    spinTravelXY_->setSuffix(" mm");
    spinTravelXY_->setToolTip(tr("XY方向行程范围 (±mm)"));
    spinTravelXY_->setMinimumWidth(90);
    spinTravelTheta_ = new QDoubleSpinBox(uvwConfigGroup_);
    spinTravelTheta_->setRange(0.1, 10.0);
    spinTravelTheta_->setSingleStep(0.1);
    spinTravelTheta_->setDecimals(1);
    spinTravelTheta_->setValue(2.0);
    spinTravelTheta_->setSuffix(" °");
    spinTravelTheta_->setToolTip(tr("θ旋转范围 (±度)"));
    spinTravelTheta_->setMinimumWidth(90);
    uvwLayout->addWidget(labelTravel, uvwRow, 0);
    uvwLayout->addWidget(spinTravelXY_, uvwRow, 1);
    uvwLayout->addWidget(spinTravelTheta_, uvwRow, 2);

    grid->addWidget(uvwConfigGroup_, row, 0, 1, 3);
    uvwConfigGroup_->setVisible(false);  // 默认隐藏，仅UVW平台显示
    ++row;

    // 添加弹性空间
    grid->setRowStretch(row, 1);

    return settingsGroup_;
}

QWidget* StationConfigTool::createButtonArea()
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    btnAddPlatform_ = new QPushButton(tr("新增平台"), widget);
    btnDeletePlatform_ = new QPushButton(tr("删除平台"), widget);

    btnAddPlatform_->setMinimumWidth(80);
    btnDeletePlatform_->setMinimumWidth(80);

    layout->addWidget(btnAddPlatform_);
    layout->addWidget(btnDeletePlatform_);
    layout->addStretch();

    return widget;
}

void StationConfigTool::setupConnections()
{
    // 树形控件
    connect(treeWidget_, &QTreeWidget::itemClicked,
            this, &StationConfigTool::onTreeItemClicked);

    // 参数变更
    connect(comboPlatformIndex_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onPlatformIndexChanged);
    connect(comboPlatformType_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onPlatformTypeChanged);
    connect(comboCameraCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onCameraCountChanged);
    connect(checkSharedCamera_, &QCheckBox::stateChanged,
            this, &StationConfigTool::onSharedCameraChanged);
    connect(comboSceneCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onSceneCountChanged);
    connect(comboPositionCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onPositionCountChanged);
    connect(comboDisplayMode_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onDisplayModeChanged);
    connect(comboProductCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onProductCountChanged);
    connect(comboPreloadCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onPreloadCountChanged);
    connect(comboFunctionMode_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onFunctionModeChanged);

    // 检测参数变更
    connect(spinDetectionConfidence_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });
    connect(spinDetectionNms_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });

    // 对位参数变更
    connect(spinAlignToleranceX_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });
    connect(spinAlignToleranceY_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });
    connect(spinAlignToleranceD_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });

    // UVW配置参数变更 - 同步值到platforms_
    connect(comboUVWStructure_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].uvwStructureType = value;
                    markAsChanged();
                }
            });
    connect(comboUVWSlider_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].uvwSliderType = value;
                    markAsChanged();
                }
            });
    connect(comboUVWPrecision_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].uvwPrecisionGrade = value;
                    markAsChanged();
                }
            });
    connect(spinNode1X_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].node1PosX = value;
                    markAsChanged();
                }
            });
    connect(spinNode1Y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].node1PosY = value;
                    markAsChanged();
                }
            });
    connect(spinNode2X_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].node2PosX = value;
                    markAsChanged();
                }
            });
    connect(spinNode2Y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].node2PosY = value;
                    markAsChanged();
                }
            });
    connect(spinNode3X_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].node3PosX = value;
                    markAsChanged();
                }
            });
    connect(spinNode3Y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].node3PosY = value;
                    markAsChanged();
                }
            });
    connect(spinTravelXY_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].travelXY = value;
                    markAsChanged();
                }
            });
    connect(spinTravelTheta_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                if (!isUpdating_ && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
                    platforms_[currentPlatformIndex_].travelTheta = value;
                    markAsChanged();
                }
            });

    // 按钮
    connect(btnAddPlatform_, &QPushButton::clicked,
            this, &StationConfigTool::onAddPlatform);
    connect(btnDeletePlatform_, &QPushButton::clicked,
            this, &StationConfigTool::onDeletePlatform);
    connect(btnSaveExit_, &QPushButton::clicked,
            this, &StationConfigTool::onSaveAndExit);
    connect(btnCancelExit_, &QPushButton::clicked,
            this, &StationConfigTool::onCancelExit);
}

// ============================================================
// 配置加载与保存
// ============================================================

void StationConfigTool::loadExistingConfig()
{
    auto& manager = Core::MultiStationManager::instance();

    // 尝试加载现有配置
    manager.loadConfig();

    // 转换为内部格式
    auto stations = manager.getAllStations();
    platforms_.clear();

    for (const auto* station : stations) {
        PlatformConfig config;
        config.index = station->index + 1;  // 内部0-based，显示1-based
        config.platformType = static_cast<int>(station->platformType);
        config.cameraCount = station->cameraNum;
        config.sharedCamera = station->shareCameraSystem;
        config.sceneCount = station->sceneNum > 0 ? station->sceneNum : 1;  // 从配置读取
        config.positionCount = station->positionNum;
        config.displayMode = station->displayMode;
        config.productCount = 5;  // 默认值
        config.preloadCount = 1;  // 默认值
        config.functionMode = static_cast<int>(station->functionMode);

        // 从第一个位置绑定获取检测/对位参数（如有）
        if (!station->positionBindings.isEmpty()) {
            const auto& binding = station->positionBindings.first();
            config.detectionConfidence = binding.detectionConfidenceThreshold;
            config.detectionNms = binding.detectionNmsThreshold;
            config.alignmentToleranceX = binding.alignmentToleranceX;
            config.alignmentToleranceY = binding.alignmentToleranceY;
            config.alignmentToleranceD = binding.alignmentToleranceD;
        }

        // UVW平台配置
        config.uvwStructureType = station->uvwStructureType;
        config.uvwSliderType = station->uvwSliderType;
        config.uvwPrecisionGrade = station->uvwPrecisionGrade;
        config.node1PosX = station->uvwNode1PosX;
        config.node1PosY = station->uvwNode1PosY;
        config.node2PosX = station->uvwNode2PosX;
        config.node2PosY = station->uvwNode2PosY;
        config.node3PosX = station->uvwNode3PosX;
        config.node3PosY = station->uvwNode3PosY;
        config.travelXY = station->uvwTravelXY;
        config.travelTheta = station->uvwTravelTheta;

        platforms_.append(config);
    }

    refreshTree();
}

bool StationConfigTool::saveConfig()
{
    auto& manager = Core::MultiStationManager::instance();

    // 清空现有配置
    manager.clearAll();

    // 创建新配置
    for (int i = 0; i < platforms_.size(); ++i) {
        const auto& config = platforms_[i];

        QString stationId = manager.createStation(
            QString("平台%1").arg(config.index),
            static_cast<Platform::PlatformType>(config.platformType)
        );

        if (!stationId.isEmpty()) {
            auto* station = manager.getStation(stationId);
            if (station) {
                station->index = config.index - 1;  // 显示1-based，内部0-based
                station->platformType = static_cast<Platform::PlatformType>(config.platformType);
                station->cameraNum = config.cameraCount;
                station->shareCameraSystem = config.sharedCamera;
                station->positionNum = config.positionCount;
                station->displayMode = static_cast<Core::StationConfig::DisplayMode>(config.displayMode);
                station->functionMode = static_cast<Core::StationConfig::FunctionMode>(config.functionMode);

                // 生成相机ID
                station->cameraIds.clear();
                for (int c = 0; c < config.cameraCount; ++c) {
                    station->cameraIds.append(QString("cam_%1_%2").arg(stationId).arg(c));
                }

                // 根据功能模式设置启用状态
                bool enableDetection = (config.functionMode == 0 || config.functionMode == 2);
                bool enableAlignment = (config.functionMode == 1 || config.functionMode == 2);

                // 生成位置绑定
                station->positionBindings.clear();
                for (int p = 0; p < config.positionCount; ++p) {
                    Core::PositionCameraBinding binding;
                    binding.positionId = QString("pos%1").arg(p);
                    binding.positionName = QString("Pos%1").arg(p + 1);
                    binding.cameraIndex = p % config.cameraCount;
                    binding.displayIndex = p;
                    binding.captureOrder = p;
                    binding.positionType = (p < config.positionCount / 2)
                                           ? Core::PositionCameraBinding::Target
                                           : Core::PositionCameraBinding::Object;

                    // 设置检测参数
                    binding.enableDetection = enableDetection;
                    binding.detectionConfidenceThreshold = config.detectionConfidence;
                    binding.detectionNmsThreshold = config.detectionNms;

                    // 设置对位参数
                    binding.enableAlignment = enableAlignment;
                    binding.alignmentToleranceX = config.alignmentToleranceX;
                    binding.alignmentToleranceY = config.alignmentToleranceY;
                    binding.alignmentToleranceD = config.alignmentToleranceD;

                    station->positionBindings.append(binding);
                }

                // 生成场景配置
                station->scenes.clear();
                station->sceneNum = config.sceneCount;
                for (int s = 0; s < config.sceneCount; ++s) {
                    Core::SceneConfig scene;
                    scene.sceneId = QString("scene%1").arg(s);
                    scene.sceneName = QString("场景%1").arg(s + 1);
                    scene.sceneIndex = s;
                    scene.enabled = true;
                    station->scenes.append(scene);
                }
                station->currentSceneIndex = 0;

                // UVW平台配置
                station->uvwStructureType = config.uvwStructureType;
                station->uvwSliderType = config.uvwSliderType;
                station->uvwPrecisionGrade = config.uvwPrecisionGrade;
                station->uvwNode1PosX = config.node1PosX;
                station->uvwNode1PosY = config.node1PosY;
                station->uvwNode2PosX = config.node2PosX;
                station->uvwNode2PosY = config.node2PosY;
                station->uvwNode3PosX = config.node3PosX;
                station->uvwNode3PosY = config.node3PosY;
                station->uvwTravelXY = config.travelXY;
                station->uvwTravelTheta = config.travelTheta;
            }
        }
    }

    // 保存到文件
    if (manager.saveConfig()) {
        LOG_INFO(QString("工位配置已保存: %1 个平台").arg(platforms_.size()));
        return true;
    }

    LOG_ERROR("保存工位配置失败");
    return false;
}

// ============================================================
// 树形控件操作
// ============================================================

void StationConfigTool::refreshTree()
{
    if (!rootItem_) return;

    // 清空子节点
    while (rootItem_->childCount() > 0) {
        delete rootItem_->takeChild(0);
    }

    // 添加平台节点
    for (int i = 0; i < platforms_.size(); ++i) {
        QTreeWidgetItem* platformItem = new QTreeWidgetItem(rootItem_);
        platformItem->setData(0, Qt::UserRole, i);  // 存储索引
        updateTreeItem(platformItem, i);
    }

    rootItem_->setExpanded(true);
}

void StationConfigTool::updateTreeItem(QTreeWidgetItem* item, int platformIndex)
{
    if (!item || platformIndex < 0 || platformIndex >= platforms_.size()) return;

    const auto& config = platforms_[platformIndex];

    // 设置平台名称
    item->setText(0, QString("平台%1").arg(config.index));

    // 清空子节点
    while (item->childCount() > 0) {
        delete item->takeChild(0);
    }

    // 添加详细信息子节点
    auto addInfoItem = [item](const QString& text) {
        QTreeWidgetItem* child = new QTreeWidgetItem(item);
        child->setText(0, text);
        child->setFlags(child->flags() & ~Qt::ItemIsSelectable);
    };

    addInfoItem(QString("平台类型: %1").arg(getPlatformTypeText(config.platformType)));
    addInfoItem(QString("相机数量: %1").arg(config.cameraCount));
    addInfoItem(QString("场景数量: %1").arg(config.sceneCount));
    addInfoItem(QString("位置数量: %1").arg(config.positionCount));
    addInfoItem(QString("视窗显示模式: %1").arg(getDisplayModeText(config.displayMode)));
    addInfoItem(QString("功能模式: %1").arg(getFunctionModeText(config.functionMode)));
    addInfoItem(QString("产品数量: %1").arg(config.productCount));
    addInfoItem(QString("产品预加载数量: %1").arg(config.preloadCount));
    addInfoItem(QString("是否为共享相机: %1").arg(config.sharedCamera ? "是" : "否"));

    item->setExpanded(true);
}

void StationConfigTool::onTreeItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);

    if (!item || item == rootItem_) {
        setSettingsPanelEnabled(false);
        currentPlatformIndex_ = -1;
        return;
    }

    // 检查是否是平台节点（有UserRole数据）
    QVariant data = item->data(0, Qt::UserRole);
    if (!data.isValid()) {
        // 可能是子节点，获取父节点
        QTreeWidgetItem* parent = item->parent();
        if (parent && parent != rootItem_) {
            data = parent->data(0, Qt::UserRole);
        }
    }

    if (data.isValid()) {
        int index = data.toInt();
        if (index >= 0 && index < platforms_.size()) {
            currentPlatformIndex_ = index;
            updateSettingsPanel(index);
            setSettingsPanelEnabled(true);
        }
    }
}

void StationConfigTool::onTreeItemChanged(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
}

// ============================================================
// 设置面板操作
// ============================================================

void StationConfigTool::updateSettingsPanel(int platformIndex)
{
    if (platformIndex < 0 || platformIndex >= platforms_.size()) return;

    isUpdating_ = true;

    const auto& config = platforms_[platformIndex];

    comboPlatformIndex_->setCurrentIndex(config.index - 1);
    // 设置平台类型（根据数据值找到对应的combo index）
    int platformTypeIndex = comboPlatformType_->findData(config.platformType);
    if (platformTypeIndex >= 0) {
        comboPlatformType_->setCurrentIndex(platformTypeIndex);
    }
    comboCameraCount_->setCurrentIndex(config.cameraCount - 1);
    checkSharedCamera_->setChecked(config.sharedCamera);
    comboSceneCount_->setCurrentIndex(config.sceneCount - 1);
    comboPositionCount_->setCurrentIndex(config.positionCount - 1);
    comboDisplayMode_->setCurrentIndex(config.displayMode);
    comboProductCount_->setCurrentIndex(config.productCount - 1);
    comboPreloadCount_->setCurrentIndex(config.preloadCount - 1);
    comboFunctionMode_->setCurrentIndex(config.functionMode);

    // 检测参数
    spinDetectionConfidence_->setValue(config.detectionConfidence);
    spinDetectionNms_->setValue(config.detectionNms);

    // 对位参数
    spinAlignToleranceX_->setValue(config.alignmentToleranceX);
    spinAlignToleranceY_->setValue(config.alignmentToleranceY);
    spinAlignToleranceD_->setValue(config.alignmentToleranceD);

    // 根据功能模式更新UI可见性
    updateFunctionModeUI(config.functionMode);

    // UVW节点配置参数
    if (comboUVWStructure_) {
        comboUVWStructure_->setCurrentIndex(config.uvwStructureType);
    }
    if (comboUVWSlider_) {
        comboUVWSlider_->setCurrentIndex(config.uvwSliderType);
    }
    if (comboUVWPrecision_) {
        comboUVWPrecision_->setCurrentIndex(config.uvwPrecisionGrade);
    }
    if (spinNode1X_) {
        spinNode1X_->setValue(config.node1PosX);
    }
    if (spinNode1Y_) {
        spinNode1Y_->setValue(config.node1PosY);
    }
    if (spinNode2X_) {
        spinNode2X_->setValue(config.node2PosX);
    }
    if (spinNode2Y_) {
        spinNode2Y_->setValue(config.node2PosY);
    }
    if (spinNode3X_) {
        spinNode3X_->setValue(config.node3PosX);
    }
    if (spinNode3Y_) {
        spinNode3Y_->setValue(config.node3PosY);
    }
    if (spinTravelXY_) {
        spinTravelXY_->setValue(config.travelXY);
    }
    if (spinTravelTheta_) {
        spinTravelTheta_->setValue(config.travelTheta);
    }

    // 根据平台类型更新UVW配置组可见性
    updateUVWConfigVisibility(config.platformType);

    isUpdating_ = false;
}

void StationConfigTool::setSettingsPanelEnabled(bool enabled)
{
    comboPlatformIndex_->setEnabled(enabled);
    comboPlatformType_->setEnabled(enabled);
    comboCameraCount_->setEnabled(enabled);
    checkSharedCamera_->setEnabled(enabled);
    comboSceneCount_->setEnabled(enabled);
    comboPositionCount_->setEnabled(enabled);
    comboDisplayMode_->setEnabled(enabled);
    comboProductCount_->setEnabled(enabled);
    comboPreloadCount_->setEnabled(enabled);
    comboFunctionMode_->setEnabled(enabled);
    detectionGroup_->setEnabled(enabled);
    alignmentGroup_->setEnabled(enabled);
    if (uvwConfigGroup_) {
        uvwConfigGroup_->setEnabled(enabled);
    }
    btnDeletePlatform_->setEnabled(enabled && platforms_.size() > 1);

    // 如果启用，根据功能模式更新参数组可见性
    if (enabled && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
        updateFunctionModeUI(platforms_[currentPlatformIndex_].functionMode);
        // 根据平台类型更新UVW配置组可见性
        updateUVWConfigVisibility(platforms_[currentPlatformIndex_].platformType);
    }
}

void StationConfigTool::markAsChanged()
{
    if (!isUpdating_) {
        hasChanges_ = true;
    }
}

QString StationConfigTool::getDisplayModeText(int mode) const
{
    switch (mode) {
    case 0: return tr("按位置数显示");
    case 1: return tr("按相机数显示");
    default: return tr("未知");
    }
}

QString StationConfigTool::getFunctionModeText(int mode) const
{
    switch (mode) {
    case 0: return tr("仅检测");
    case 1: return tr("仅对位");
    case 2: return tr("检测+对位");
    default: return tr("未知");
    }
}

QString StationConfigTool::getPlatformTypeText(int type) const
{
    switch (type) {
    case static_cast<int>(Platform::PlatformType::XYD): return tr("XYD (标准三轴)");
    case static_cast<int>(Platform::PlatformType::XY): return tr("XY (双轴)");
    case static_cast<int>(Platform::PlatformType::X1X2Y): return tr("X1X2Y (龙门双X)");
    case static_cast<int>(Platform::PlatformType::XY1Y2): return tr("XY1Y2 (双Y平台)");
    case static_cast<int>(Platform::PlatformType::DXY): return tr("DXY (旋转优先)");
    case static_cast<int>(Platform::PlatformType::XD): return tr("XD (X轴+旋转)");
    case static_cast<int>(Platform::PlatformType::YD): return tr("YD (Y轴+旋转)");
    case static_cast<int>(Platform::PlatformType::XYDP): return tr("XYDP (四轴)");
    // UVW精密对位平台系列
    case static_cast<int>(Platform::PlatformType::UVW): return tr("UVW (精密对位)");
    case static_cast<int>(Platform::PlatformType::UVW_XXY): return tr("UVW-XXY (双X单Y)");
    case static_cast<int>(Platform::PlatformType::UVW_XYY): return tr("UVW-XYY (单X双Y)");
    case static_cast<int>(Platform::PlatformType::UVW_XXYY): return tr("UVW-XXYY (四轴)");
    case static_cast<int>(Platform::PlatformType::UVW_HOLLOW): return tr("UVW-中空 (光学)");
    case static_cast<int>(Platform::PlatformType::UVW_HIGH_PRECISION): return tr("UVW-高精度 (2μm)");
    default: return tr("未知");
    }
}

void StationConfigTool::updateFunctionModeUI(int mode)
{
    // 根据功能模式显示/隐藏参数组
    // mode: 0=仅检测, 1=仅对位, 2=检测+对位
    bool showDetection = (mode == 0 || mode == 2);
    bool showAlignment = (mode == 1 || mode == 2);

    detectionGroup_->setVisible(showDetection);
    alignmentGroup_->setVisible(showAlignment);
}

bool StationConfigTool::isUVWPlatformType(int platformType) const
{
    // 检查是否为UVW系列平台类型
    return platformType == static_cast<int>(Platform::PlatformType::UVW) ||
           platformType == static_cast<int>(Platform::PlatformType::UVW_XXY) ||
           platformType == static_cast<int>(Platform::PlatformType::UVW_XYY) ||
           platformType == static_cast<int>(Platform::PlatformType::UVW_XXYY) ||
           platformType == static_cast<int>(Platform::PlatformType::UVW_HOLLOW) ||
           platformType == static_cast<int>(Platform::PlatformType::UVW_HIGH_PRECISION);
}

void StationConfigTool::updateUVWConfigVisibility(int platformType)
{
    // 根据平台类型显示/隐藏UVW配置组
    bool isUVW = isUVWPlatformType(platformType);
    if (uvwConfigGroup_) {
        uvwConfigGroup_->setVisible(isUVW);
    }
}

// ============================================================
// 参数变更槽函数
// ============================================================

void StationConfigTool::onPlatformIndexChanged(int index)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    int newIndex = index + 1;

    // 检查是否与其他平台冲突
    for (int i = 0; i < platforms_.size(); ++i) {
        if (i != currentPlatformIndex_ && platforms_[i].index == newIndex) {
            QMessageBox::warning(this, tr("警告"),
                tr("平台序 %1 已被使用，请选择其他序号").arg(newIndex));
            isUpdating_ = true;
            comboPlatformIndex_->setCurrentIndex(platforms_[currentPlatformIndex_].index - 1);
            isUpdating_ = false;
            return;
        }
    }

    platforms_[currentPlatformIndex_].index = newIndex;
    markAsChanged();

    // 更新树形显示
    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onPlatformTypeChanged(int index)
{
    Q_UNUSED(index);
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    int platformType = comboPlatformType_->currentData().toInt();
    platforms_[currentPlatformIndex_].platformType = platformType;
    markAsChanged();

    // 更新UVW配置组可见性
    updateUVWConfigVisibility(platformType);

    // 更新树形显示
    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onCameraCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].cameraCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onSharedCameraChanged(int state)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].sharedCamera = (state == Qt::Checked);
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onSceneCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].sceneCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onPositionCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].positionCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onDisplayModeChanged(int index)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].displayMode = index;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onProductCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].productCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onPreloadCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].preloadCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onFunctionModeChanged(int index)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].functionMode = index;

    // 同步更新当前spinbox的值到platforms数据
    platforms_[currentPlatformIndex_].detectionConfidence = static_cast<float>(spinDetectionConfidence_->value());
    platforms_[currentPlatformIndex_].detectionNms = static_cast<float>(spinDetectionNms_->value());
    platforms_[currentPlatformIndex_].alignmentToleranceX = spinAlignToleranceX_->value();
    platforms_[currentPlatformIndex_].alignmentToleranceY = spinAlignToleranceY_->value();
    platforms_[currentPlatformIndex_].alignmentToleranceD = spinAlignToleranceD_->value();

    markAsChanged();

    // 更新参数组可见性
    updateFunctionModeUI(index);

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

// ============================================================
// 按钮操作
// ============================================================

void StationConfigTool::onAddPlatform()
{
    if (platforms_.size() >= 6) {
        QMessageBox::warning(this, tr("警告"), tr("最多只能添加6个平台"));
        return;
    }

    // 查找可用的平台序号
    int newIndex = 1;
    QSet<int> usedIndices;
    for (const auto& p : platforms_) {
        usedIndices.insert(p.index);
    }
    while (usedIndices.contains(newIndex) && newIndex <= 6) {
        ++newIndex;
    }

    if (newIndex > 6) {
        QMessageBox::warning(this, tr("警告"), tr("没有可用的平台序号"));
        return;
    }

    // 创建新平台配置（默认简单检测模式）
    PlatformConfig config;
    config.index = newIndex;
    config.cameraCount = 1;
    config.sharedCamera = false;
    config.sceneCount = 1;      // 默认1个场景（简单检测模式）
    config.positionCount = 1;   // 默认1个位置（简单检测模式）
    config.displayMode = 0;
    config.productCount = 5;
    config.preloadCount = 1;

    platforms_.append(config);
    markAsChanged();

    // 刷新树形显示
    refreshTree();

    // 选中新添加的平台
    if (rootItem_ && rootItem_->childCount() > 0) {
        QTreeWidgetItem* newItem = rootItem_->child(rootItem_->childCount() - 1);
        treeWidget_->setCurrentItem(newItem);
        onTreeItemClicked(newItem, 0);
    }

    LOG_INFO(QString("添加平台%1").arg(newIndex));
}

void StationConfigTool::onDeletePlatform()
{
    if (currentPlatformIndex_ < 0 || currentPlatformIndex_ >= platforms_.size()) {
        return;
    }

    if (platforms_.size() <= 1) {
        QMessageBox::warning(this, tr("警告"), tr("至少需要保留一个平台"));
        return;
    }

    int platformIndex = platforms_[currentPlatformIndex_].index;

    int ret = QMessageBox::question(this, tr("确认删除"),
        tr("确定要删除平台%1吗？").arg(platformIndex),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret != QMessageBox::Yes) {
        return;
    }

    platforms_.removeAt(currentPlatformIndex_);
    markAsChanged();

    // 刷新树形显示
    refreshTree();

    // 选中第一个平台
    currentPlatformIndex_ = -1;
    if (!platforms_.isEmpty() && rootItem_ && rootItem_->childCount() > 0) {
        treeWidget_->setCurrentItem(rootItem_->child(0));
        onTreeItemClicked(rootItem_->child(0), 0);
    } else {
        setSettingsPanelEnabled(false);
    }

    LOG_INFO(QString("删除平台%1").arg(platformIndex));
}

void StationConfigTool::onSaveAndExit()
{
    if (saveConfig()) {
        saveAndExit_ = true;
        accept();
    } else {
        QMessageBox::critical(this, tr("错误"), tr("保存配置失败"));
    }
}

void StationConfigTool::onCancelExit()
{
    if (hasChanges_) {
        int ret = QMessageBox::question(this, tr("确认"),
            tr("配置已修改，确定要放弃更改吗？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (ret != QMessageBox::Yes) {
            return;
        }
    }

    saveAndExit_ = false;
    reject();
}

} // namespace UI
} // namespace VisionForge
