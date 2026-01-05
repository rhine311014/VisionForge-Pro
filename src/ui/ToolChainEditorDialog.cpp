/**
 * @file ToolChainEditorDialog.cpp
 * @brief 工具链编辑器对话框实现
 * @author VisionForge Team
 * @date 2025-01-04
 */

#include "ui/ToolChainEditorDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/VisionTool.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QSlider>
#include <QButtonGroup>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>

namespace VisionForge {
namespace UI {

ToolChainEditorDialog::ToolChainEditorDialog(QWidget* parent)
    : QDialog(parent)
    , productIndex_(0)
    , sceneIndex_(0)
    , currentToolIndex_(-1)
    , mainLayout_(nullptr)
    , mainSplitter_(nullptr)
    , topWidget_(nullptr)
    , bottomWidget_(nullptr)
    , topSplitter_(nullptr)
    , leftPanel_(nullptr)
    , imageViewer_(nullptr)
    , imageInfoLabel_(nullptr)
    , rightPanel_(nullptr)
    , settingScrollArea_(nullptr)
    , trainSettingGroup_(nullptr)
    , trainAreaLabel_(nullptr)
    , trainAreaBtn_(nullptr)
    , searchAreaLabel_(nullptr)
    , searchAreaBtn_(nullptr)
    , trainBtn_(nullptr)
    , clearTrainBtn_(nullptr)
    , showTrainCheck_(nullptr)
    , showSearchCheck_(nullptr)
    , searchSettingGroup_(nullptr)
    , angleStartSpin_(nullptr)
    , angleEndSpin_(nullptr)
    , angleStepSpin_(nullptr)
    , scoreThresholdSpin_(nullptr)
    , maxCountSpin_(nullptr)
    , pyramidLevelCombo_(nullptr)
    , subPixelCombo_(nullptr)
    , multiMatchCheck_(nullptr)
    , positionSettingGroup_(nullptr)
    , positionModeCombo_(nullptr)
    , offsetXSpin_(nullptr)
    , offsetYSpin_(nullptr)
    , offsetAngleSpin_(nullptr)
    , invertXCheck_(nullptr)
    , invertYCheck_(nullptr)
    , invertAngleCheck_(nullptr)
    , sideBarWidget_(nullptr)
    , captureBtn_(nullptr)
    , cameraParamBtn_(nullptr)
    , lightControlBtn_(nullptr)
    , runToolBtn_(nullptr)
    , runAllBtn_(nullptr)
    , saveConfigBtn_(nullptr)
    , loadConfigBtn_(nullptr)
    , bottomSplitter_(nullptr)
    , categoryWidget_(nullptr)
    , categoryTree_(nullptr)
    , toolListWidget_(nullptr)
    , toolTable_(nullptr)
    , addToolBtn_(nullptr)
    , removeToolBtn_(nullptr)
    , moveUpBtn_(nullptr)
    , moveDownBtn_(nullptr)
    , copyToolBtn_(nullptr)
    , positionWidget_(nullptr)
    , positionImageLabel_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle(tr("工具链编辑器"));
    setMinimumSize(1200, 800);
    resize(1400, 900);

    setupUI();
    initToolCategories();
}

ToolChainEditorDialog::~ToolChainEditorDialog()
{
}

void ToolChainEditorDialog::setupUI()
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setSpacing(6);
    mainLayout_->setContentsMargins(10, 10, 10, 10);

    createMainLayout();

    // 底部按钮
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okBtn_ = new QPushButton(tr("确定"));
    cancelBtn_ = new QPushButton(tr("取消"));
    applyBtn_ = new QPushButton(tr("应用"));

    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    buttonLayout->addWidget(applyBtn_);

    mainLayout_->addLayout(buttonLayout);

    connect(okBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onApplyClicked);
}

void ToolChainEditorDialog::createMainLayout()
{
    // 主分割器（上下分割）
    mainSplitter_ = new QSplitter(Qt::Vertical);

    // 上部区域
    topWidget_ = new QWidget();
    auto* topLayout = new QHBoxLayout(topWidget_);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(6);

    // 上部分割器（左右分割）
    topSplitter_ = new QSplitter(Qt::Horizontal);

    // 创建左侧面板（图像显示）
    leftPanel_ = new QWidget();
    createLeftPanel(leftPanel_);
    topSplitter_->addWidget(leftPanel_);

    // 创建右侧面板（工具设置）
    rightPanel_ = new QWidget();
    createRightPanel(rightPanel_);
    topSplitter_->addWidget(rightPanel_);

    // 侧边栏
    sideBarWidget_ = new QWidget();
    sideBarWidget_->setFixedWidth(100);
    auto* sideLayout = new QVBoxLayout(sideBarWidget_);
    createSideBar(sideLayout);

    topLayout->addWidget(topSplitter_, 1);
    topLayout->addWidget(sideBarWidget_);

    // 设置上部分割器比例
    topSplitter_->setSizes({600, 400});

    // 下部区域
    bottomWidget_ = new QWidget();
    createBottomPanel(bottomWidget_);

    mainSplitter_->addWidget(topWidget_);
    mainSplitter_->addWidget(bottomWidget_);

    // 设置主分割器比例
    mainSplitter_->setSizes({500, 300});

    mainLayout_->addWidget(mainSplitter_, 1);
}

void ToolChainEditorDialog::createLeftPanel(QWidget* parent)
{
    auto* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 图像显示器
    imageViewer_ = new HalconImageViewer();
    imageViewer_->setMinimumSize(400, 300);
    layout->addWidget(imageViewer_, 1);

    // 图像信息
    imageInfoLabel_ = new QLabel(tr("图像: 无"));
    imageInfoLabel_->setStyleSheet("QLabel { background-color: #3c3c3c; color: #e0e0e0; padding: 4px; }");
    layout->addWidget(imageInfoLabel_);
}

void ToolChainEditorDialog::createRightPanel(QWidget* parent)
{
    auto* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    // 滚动区域
    settingScrollArea_ = new QScrollArea();
    settingScrollArea_->setWidgetResizable(true);
    settingScrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* scrollContent = new QWidget();
    auto* scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setSpacing(10);

    // 创建设置组
    createTrainSettingGroup();
    scrollLayout->addWidget(trainSettingGroup_);

    createSearchSettingGroup();
    scrollLayout->addWidget(searchSettingGroup_);

    createPositionSettingGroup();
    scrollLayout->addWidget(positionSettingGroup_);

    scrollLayout->addStretch();
    settingScrollArea_->setWidget(scrollContent);
    layout->addWidget(settingScrollArea_);
}

void ToolChainEditorDialog::createSideBar(QVBoxLayout* layout)
{
    layout->setSpacing(8);
    layout->setContentsMargins(4, 4, 4, 4);

    // 采集图像
    captureBtn_ = new QPushButton(tr("采集图像"));
    captureBtn_->setFixedHeight(36);
    layout->addWidget(captureBtn_);

    // 相机参数
    cameraParamBtn_ = new QPushButton(tr("相机参数"));
    cameraParamBtn_->setFixedHeight(36);
    layout->addWidget(cameraParamBtn_);

    // 光源控制
    lightControlBtn_ = new QPushButton(tr("光源控制"));
    lightControlBtn_->setFixedHeight(36);
    layout->addWidget(lightControlBtn_);

    layout->addSpacing(20);

    // 运行工具
    runToolBtn_ = new QPushButton(tr("运行工具"));
    runToolBtn_->setFixedHeight(36);
    runToolBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");
    layout->addWidget(runToolBtn_);

    // 运行全部
    runAllBtn_ = new QPushButton(tr("运行全部"));
    runAllBtn_->setFixedHeight(36);
    runAllBtn_->setStyleSheet("QPushButton { background-color: #2196F3; color: white; }");
    layout->addWidget(runAllBtn_);

    layout->addSpacing(20);

    // 保存配置
    saveConfigBtn_ = new QPushButton(tr("保存配置"));
    saveConfigBtn_->setFixedHeight(36);
    layout->addWidget(saveConfigBtn_);

    // 加载配置
    loadConfigBtn_ = new QPushButton(tr("加载配置"));
    loadConfigBtn_->setFixedHeight(36);
    layout->addWidget(loadConfigBtn_);

    layout->addStretch();

    // 连接信号
    connect(captureBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onCaptureImage);
    connect(cameraParamBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onCameraParam);
    connect(lightControlBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onLightControl);
    connect(runToolBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onRunTool);
    connect(runAllBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onRunAll);
    connect(saveConfigBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onSaveConfig);
    connect(loadConfigBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onLoadConfig);
}

void ToolChainEditorDialog::createTrainSettingGroup()
{
    trainSettingGroup_ = new QGroupBox(tr("训练设置"));
    auto* layout = new QGridLayout(trainSettingGroup_);
    layout->setSpacing(8);

    int row = 0;

    // 训练区域
    layout->addWidget(new QLabel(tr("训练区域:")), row, 0);
    trainAreaLabel_ = new QLabel(tr("未设置"));
    trainAreaLabel_->setStyleSheet("QLabel { background-color: #fff; border: 1px solid #ccc; padding: 4px; }");
    layout->addWidget(trainAreaLabel_, row, 1);
    trainAreaBtn_ = new QPushButton(tr("设置"));
    trainAreaBtn_->setFixedWidth(60);
    layout->addWidget(trainAreaBtn_, row, 2);
    row++;

    // 搜索区域
    layout->addWidget(new QLabel(tr("搜索区域:")), row, 0);
    searchAreaLabel_ = new QLabel(tr("未设置"));
    searchAreaLabel_->setStyleSheet("QLabel { background-color: #fff; border: 1px solid #ccc; padding: 4px; }");
    layout->addWidget(searchAreaLabel_, row, 1);
    searchAreaBtn_ = new QPushButton(tr("设置"));
    searchAreaBtn_->setFixedWidth(60);
    layout->addWidget(searchAreaBtn_, row, 2);
    row++;

    // 显示选项
    showTrainCheck_ = new QCheckBox(tr("显示训练区域"));
    showTrainCheck_->setChecked(true);
    layout->addWidget(showTrainCheck_, row, 0, 1, 2);
    row++;

    showSearchCheck_ = new QCheckBox(tr("显示搜索区域"));
    showSearchCheck_->setChecked(true);
    layout->addWidget(showSearchCheck_, row, 0, 1, 2);
    row++;

    // 训练按钮
    auto* btnLayout = new QHBoxLayout();
    trainBtn_ = new QPushButton(tr("训练"));
    trainBtn_->setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-weight: bold; }");
    clearTrainBtn_ = new QPushButton(tr("清除"));
    btnLayout->addWidget(trainBtn_);
    btnLayout->addWidget(clearTrainBtn_);
    layout->addLayout(btnLayout, row, 0, 1, 3);

    connect(trainAreaBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onTrainAreaChanged);
    connect(searchAreaBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onSearchAreaChanged);
    connect(trainBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onTrainClicked);
    connect(clearTrainBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onClearTrainClicked);
}

void ToolChainEditorDialog::createSearchSettingGroup()
{
    searchSettingGroup_ = new QGroupBox(tr("搜索设置"));
    auto* layout = new QGridLayout(searchSettingGroup_);
    layout->setSpacing(8);

    int row = 0;

    // 角度范围
    layout->addWidget(new QLabel(tr("起始角度:")), row, 0);
    angleStartSpin_ = new QDoubleSpinBox();
    angleStartSpin_->setRange(-180, 180);
    angleStartSpin_->setValue(-45);
    angleStartSpin_->setSuffix(tr("°"));
    layout->addWidget(angleStartSpin_, row, 1);

    layout->addWidget(new QLabel(tr("结束角度:")), row, 2);
    angleEndSpin_ = new QDoubleSpinBox();
    angleEndSpin_->setRange(-180, 180);
    angleEndSpin_->setValue(45);
    angleEndSpin_->setSuffix(tr("°"));
    layout->addWidget(angleEndSpin_, row, 3);
    row++;

    // 角度步长
    layout->addWidget(new QLabel(tr("角度步长:")), row, 0);
    angleStepSpin_ = new QDoubleSpinBox();
    angleStepSpin_->setRange(0.1, 10);
    angleStepSpin_->setValue(1);
    angleStepSpin_->setDecimals(1);
    angleStepSpin_->setSuffix(tr("°"));
    layout->addWidget(angleStepSpin_, row, 1);
    row++;

    // 分数阈值
    layout->addWidget(new QLabel(tr("分数阈值:")), row, 0);
    scoreThresholdSpin_ = new QDoubleSpinBox();
    scoreThresholdSpin_->setRange(0, 1);
    scoreThresholdSpin_->setValue(0.7);
    scoreThresholdSpin_->setDecimals(2);
    scoreThresholdSpin_->setSingleStep(0.05);
    layout->addWidget(scoreThresholdSpin_, row, 1);

    // 最大数量
    layout->addWidget(new QLabel(tr("最大数量:")), row, 2);
    maxCountSpin_ = new QSpinBox();
    maxCountSpin_->setRange(1, 100);
    maxCountSpin_->setValue(1);
    layout->addWidget(maxCountSpin_, row, 3);
    row++;

    // 金字塔层数
    layout->addWidget(new QLabel(tr("金字塔层数:")), row, 0);
    pyramidLevelCombo_ = new QComboBox();
    pyramidLevelCombo_->addItems({"自动", "1", "2", "3", "4", "5", "6"});
    layout->addWidget(pyramidLevelCombo_, row, 1);

    // 亚像素模式
    layout->addWidget(new QLabel(tr("亚像素模式:")), row, 2);
    subPixelCombo_ = new QComboBox();
    subPixelCombo_->addItems({tr("无"), tr("最小二乘"), tr("插值")});
    subPixelCombo_->setCurrentIndex(1);
    layout->addWidget(subPixelCombo_, row, 3);
    row++;

    // 多目标匹配
    multiMatchCheck_ = new QCheckBox(tr("多目标匹配"));
    layout->addWidget(multiMatchCheck_, row, 0, 1, 2);

    connect(angleStartSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ToolChainEditorDialog::onAngleRangeChanged);
    connect(angleEndSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ToolChainEditorDialog::onAngleRangeChanged);
    connect(scoreThresholdSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ToolChainEditorDialog::onScoreThresholdChanged);
    connect(maxCountSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ToolChainEditorDialog::onMaxCountChanged);
}

void ToolChainEditorDialog::createPositionSettingGroup()
{
    positionSettingGroup_ = new QGroupBox(tr("位置设置"));
    auto* layout = new QGridLayout(positionSettingGroup_);
    layout->setSpacing(8);

    int row = 0;

    // 位置模式
    layout->addWidget(new QLabel(tr("位置模式:")), row, 0);
    positionModeCombo_ = new QComboBox();
    positionModeCombo_->addItems({tr("中心点"), tr("左上角"), tr("右上角"), tr("左下角"), tr("右下角")});
    layout->addWidget(positionModeCombo_, row, 1, 1, 3);
    row++;

    // X偏移
    layout->addWidget(new QLabel(tr("X偏移:")), row, 0);
    offsetXSpin_ = new QDoubleSpinBox();
    offsetXSpin_->setRange(-10000, 10000);
    offsetXSpin_->setValue(0);
    offsetXSpin_->setDecimals(3);
    layout->addWidget(offsetXSpin_, row, 1);

    // Y偏移
    layout->addWidget(new QLabel(tr("Y偏移:")), row, 2);
    offsetYSpin_ = new QDoubleSpinBox();
    offsetYSpin_->setRange(-10000, 10000);
    offsetYSpin_->setValue(0);
    offsetYSpin_->setDecimals(3);
    layout->addWidget(offsetYSpin_, row, 3);
    row++;

    // 角度偏移
    layout->addWidget(new QLabel(tr("角度偏移:")), row, 0);
    offsetAngleSpin_ = new QDoubleSpinBox();
    offsetAngleSpin_->setRange(-180, 180);
    offsetAngleSpin_->setValue(0);
    offsetAngleSpin_->setDecimals(2);
    offsetAngleSpin_->setSuffix(tr("°"));
    layout->addWidget(offsetAngleSpin_, row, 1);
    row++;

    // 取反选项
    invertXCheck_ = new QCheckBox(tr("X取反"));
    layout->addWidget(invertXCheck_, row, 0);
    invertYCheck_ = new QCheckBox(tr("Y取反"));
    layout->addWidget(invertYCheck_, row, 1);
    invertAngleCheck_ = new QCheckBox(tr("角度取反"));
    layout->addWidget(invertAngleCheck_, row, 2, 1, 2);

    connect(positionModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ToolChainEditorDialog::onPositionModeChanged);
    connect(offsetXSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ToolChainEditorDialog::onPositionOffsetChanged);
    connect(offsetYSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ToolChainEditorDialog::onPositionOffsetChanged);
}

void ToolChainEditorDialog::createBottomPanel(QWidget* parent)
{
    auto* layout = new QHBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    // 底部分割器
    bottomSplitter_ = new QSplitter(Qt::Horizontal);

    // 工具类别树
    categoryWidget_ = new QWidget();
    createCategoryTree();
    bottomSplitter_->addWidget(categoryWidget_);

    // 工具列表
    toolListWidget_ = new QWidget();
    createToolTable();
    bottomSplitter_->addWidget(toolListWidget_);

    // 位置检测设置
    positionWidget_ = new QWidget();
    auto* posLayout = new QVBoxLayout(positionWidget_);
    posLayout->setContentsMargins(4, 4, 4, 4);

    auto* posGroup = new QGroupBox(tr("位置检测"));
    auto* posGroupLayout = new QVBoxLayout(posGroup);

    positionImageLabel_ = new QLabel();
    positionImageLabel_->setMinimumSize(150, 150);
    positionImageLabel_->setAlignment(Qt::AlignCenter);
    positionImageLabel_->setStyleSheet("QLabel { background-color: #3c3c3c; color: #e0e0e0; border: 1px solid #555; }");
    positionImageLabel_->setText(tr("位置示意图"));
    posGroupLayout->addWidget(positionImageLabel_);

    posLayout->addWidget(posGroup);
    posLayout->addStretch();
    bottomSplitter_->addWidget(positionWidget_);

    // 设置分割比例
    bottomSplitter_->setSizes({200, 500, 200});

    layout->addWidget(bottomSplitter_);
}

void ToolChainEditorDialog::createCategoryTree()
{
    auto* layout = new QVBoxLayout(categoryWidget_);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);

    auto* label = new QLabel(tr("工具类别"));
    label->setStyleSheet("QLabel { font-weight: bold; padding: 4px; background-color: #3c3c3c; color: #e0e0e0; }");
    layout->addWidget(label);

    categoryTree_ = new QTreeWidget();
    categoryTree_->setHeaderHidden(true);
    categoryTree_->setRootIsDecorated(true);
    categoryTree_->setIndentation(20);
    layout->addWidget(categoryTree_);

    connect(categoryTree_, &QTreeWidget::itemClicked,
            this, &ToolChainEditorDialog::onCategoryItemClicked);
    connect(categoryTree_, &QTreeWidget::itemDoubleClicked,
            this, &ToolChainEditorDialog::onCategoryItemDoubleClicked);
}

void ToolChainEditorDialog::createToolTable()
{
    auto* layout = new QVBoxLayout(toolListWidget_);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);

    // 标题和管理按钮
    auto* headerLayout = new QHBoxLayout();

    auto* label = new QLabel(tr("工具列表"));
    label->setStyleSheet("QLabel { font-weight: bold; padding: 4px; background-color: #3c3c3c; color: #e0e0e0; }");
    headerLayout->addWidget(label);
    headerLayout->addStretch();

    createToolManageButtons();
    headerLayout->addWidget(addToolBtn_);
    headerLayout->addWidget(removeToolBtn_);
    headerLayout->addWidget(moveUpBtn_);
    headerLayout->addWidget(moveDownBtn_);
    headerLayout->addWidget(copyToolBtn_);

    layout->addLayout(headerLayout);

    // 工具表格
    toolTable_ = new QTableWidget();
    toolTable_->setColumnCount(5);
    toolTable_->setHorizontalHeaderLabels({tr("序号"), tr("名称"), tr("类型"), tr("时间(ms)"), tr("结果")});
    toolTable_->horizontalHeader()->setStretchLastSection(true);
    toolTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    toolTable_->setColumnWidth(0, 50);
    toolTable_->setColumnWidth(1, 120);
    toolTable_->setColumnWidth(2, 100);
    toolTable_->setColumnWidth(3, 80);
    toolTable_->setColumnWidth(4, 60);
    toolTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    toolTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    toolTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    toolTable_->verticalHeader()->setVisible(false);
    layout->addWidget(toolTable_);

    connect(toolTable_, &QTableWidget::itemSelectionChanged,
            this, &ToolChainEditorDialog::onToolSelectionChanged);
    connect(toolTable_, &QTableWidget::cellDoubleClicked,
            this, &ToolChainEditorDialog::onToolDoubleClicked);
}

void ToolChainEditorDialog::createToolManageButtons()
{
    addToolBtn_ = new QPushButton(tr("添加"));
    addToolBtn_->setFixedWidth(60);
    removeToolBtn_ = new QPushButton(tr("删除"));
    removeToolBtn_->setFixedWidth(60);
    moveUpBtn_ = new QPushButton(tr("上移"));
    moveUpBtn_->setFixedWidth(60);
    moveDownBtn_ = new QPushButton(tr("下移"));
    moveDownBtn_->setFixedWidth(60);
    copyToolBtn_ = new QPushButton(tr("复制"));
    copyToolBtn_->setFixedWidth(60);

    connect(addToolBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onAddTool);
    connect(removeToolBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onRemoveTool);
    connect(moveUpBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onMoveToolUp);
    connect(moveDownBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onMoveToolDown);
    connect(copyToolBtn_, &QPushButton::clicked, this, &ToolChainEditorDialog::onCopyTool);
}

void ToolChainEditorDialog::initToolCategories()
{
    categoryTree_->clear();

    // 图像预处理
    auto* preprocessItem = new QTreeWidgetItem(categoryTree_, {tr("图像预处理")});
    preprocessItem->setData(0, Qt::UserRole, static_cast<int>(ToolCategory::ImagePreprocess));
    new QTreeWidgetItem(preprocessItem, {tr("灰度转换")});
    new QTreeWidgetItem(preprocessItem, {tr("滤波")});
    new QTreeWidgetItem(preprocessItem, {tr("二值化")});
    new QTreeWidgetItem(preprocessItem, {tr("边缘检测")});
    new QTreeWidgetItem(preprocessItem, {tr("形态学")});
    new QTreeWidgetItem(preprocessItem, {tr("颜色转换")});
    new QTreeWidgetItem(preprocessItem, {tr("ROI")});

    // 位置检测
    auto* detectItem = new QTreeWidgetItem(categoryTree_, {tr("位置检测")});
    detectItem->setData(0, Qt::UserRole, static_cast<int>(ToolCategory::PositionDetect));
    new QTreeWidgetItem(detectItem, {tr("模板匹配")});
    new QTreeWidgetItem(detectItem, {tr("形状匹配")});
    new QTreeWidgetItem(detectItem, {tr("快速搜索")});
    new QTreeWidgetItem(detectItem, {tr("图案搜索")});
    new QTreeWidgetItem(detectItem, {tr("角点搜索")});
    new QTreeWidgetItem(detectItem, {tr("找圆")});
    new QTreeWidgetItem(detectItem, {tr("找线")});
    new QTreeWidgetItem(detectItem, {tr("找边")});
    new QTreeWidgetItem(detectItem, {tr("Blob分析")});

    // 测量
    auto* measureItem = new QTreeWidgetItem(categoryTree_, {tr("测量")});
    measureItem->setData(0, Qt::UserRole, static_cast<int>(ToolCategory::Measurement));
    new QTreeWidgetItem(measureItem, {tr("测量距离")});
    new QTreeWidgetItem(measureItem, {tr("测量角度")});
    new QTreeWidgetItem(measureItem, {tr("测量面积")});
    new QTreeWidgetItem(measureItem, {tr("计算中心")});
    new QTreeWidgetItem(measureItem, {tr("计算方向")});

    // 判定
    auto* judgeItem = new QTreeWidgetItem(categoryTree_, {tr("判定")});
    judgeItem->setData(0, Qt::UserRole, static_cast<int>(ToolCategory::Judgment));
    new QTreeWidgetItem(judgeItem, {tr("范围判定")});
    new QTreeWidgetItem(judgeItem, {tr("逻辑运算")});

    // 识别
    auto* codeItem = new QTreeWidgetItem(categoryTree_, {tr("识别")});
    codeItem->setData(0, Qt::UserRole, static_cast<int>(ToolCategory::CodeRead));
    new QTreeWidgetItem(codeItem, {tr("二维码/条码")});

    // 标定
    auto* calibItem = new QTreeWidgetItem(categoryTree_, {tr("标定")});
    calibItem->setData(0, Qt::UserRole, static_cast<int>(ToolCategory::Calibration));
    new QTreeWidgetItem(calibItem, {tr("相机标定")});
    new QTreeWidgetItem(calibItem, {tr("九点标定")});
    new QTreeWidgetItem(calibItem, {tr("多点对位")});

    // 输出
    auto* outputItem = new QTreeWidgetItem(categoryTree_, {tr("输出")});
    outputItem->setData(0, Qt::UserRole, static_cast<int>(ToolCategory::Output));
    new QTreeWidgetItem(outputItem, {tr("PLC输出")});
    new QTreeWidgetItem(outputItem, {tr("保存图像")});
    new QTreeWidgetItem(outputItem, {tr("对位输出")});

    categoryTree_->expandAll();
}

void ToolChainEditorDialog::setWorkStationId(const QString& wsId)
{
    workStationId_ = wsId;
}

void ToolChainEditorDialog::setProductIndex(int index)
{
    productIndex_ = index;
}

void ToolChainEditorDialog::setSceneIndex(int index)
{
    sceneIndex_ = index;
}

int ToolChainEditorDialog::toolCount() const
{
    return tools_.size();
}

Algorithm::VisionTool* ToolChainEditorDialog::getTool(int index) const
{
    if (index >= 0 && index < tools_.size()) {
        return tools_[index];
    }
    return nullptr;
}

int ToolChainEditorDialog::addTool(Algorithm::VisionTool* tool)
{
    if (!tool) return -1;

    tools_.append(tool);
    updateToolTable();
    return tools_.size() - 1;
}

bool ToolChainEditorDialog::removeTool(int index)
{
    if (index >= 0 && index < tools_.size()) {
        tools_.removeAt(index);
        updateToolTable();
        return true;
    }
    return false;
}

bool ToolChainEditorDialog::loadConfig(const QString& configPath)
{
    Q_UNUSED(configPath);
    // TODO: 实现配置加载
    return true;
}

bool ToolChainEditorDialog::saveConfig(const QString& configPath)
{
    Q_UNUSED(configPath);
    // TODO: 实现配置保存
    return true;
}

void ToolChainEditorDialog::updateToolTable()
{
    toolTable_->setRowCount(tools_.size());

    for (int i = 0; i < tools_.size(); ++i) {
        auto* tool = tools_[i];

        toolTable_->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        toolTable_->setItem(i, 1, new QTableWidgetItem(tool ? tool->toolName() : tr("未知")));
        toolTable_->setItem(i, 2, new QTableWidgetItem(tool ? tool->toolName() : tr("未知")));
        toolTable_->setItem(i, 3, new QTableWidgetItem("0.0"));
        toolTable_->setItem(i, 4, new QTableWidgetItem("OK"));

        // 设置居中对齐
        for (int j = 0; j < 5; ++j) {
            if (auto* item = toolTable_->item(i, j)) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

void ToolChainEditorDialog::updateToolSettings()
{
    if (currentToolIndex_ < 0 || currentToolIndex_ >= tools_.size()) {
        return;
    }

    // TODO: 根据当前选中的工具更新设置面板
}

void ToolChainEditorDialog::updateCategoryTree()
{
    // TODO: 更新类别树状态
}

QString ToolChainEditorDialog::getCategoryName(ToolCategory category)
{
    switch (category) {
        case ToolCategory::ImagePreprocess: return tr("图像预处理");
        case ToolCategory::PositionDetect: return tr("位置检测");
        case ToolCategory::Measurement: return tr("测量");
        case ToolCategory::Judgment: return tr("判定");
        case ToolCategory::CodeRead: return tr("识别");
        case ToolCategory::Calibration: return tr("标定");
        case ToolCategory::Output: return tr("输出");
        default: return tr("未知");
    }
}

QIcon ToolChainEditorDialog::getCategoryIcon(ToolCategory category)
{
    Q_UNUSED(category);
    return QIcon();
}

QIcon ToolChainEditorDialog::getToolIcon(const QString& toolType)
{
    Q_UNUSED(toolType);
    return QIcon();
}

// ========== 槽函数实现 ==========

void ToolChainEditorDialog::onToolSelectionChanged()
{
    int row = toolTable_->currentRow();
    if (row >= 0 && row < tools_.size()) {
        currentToolIndex_ = row;
        updateToolSettings();
        emit toolSelected(row);
    }
}

void ToolChainEditorDialog::onToolDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row >= 0 && row < tools_.size()) {
        // TODO: 打开工具参数编辑对话框
    }
}

void ToolChainEditorDialog::onAddTool()
{
    // TODO: 打开工具选择对话框
    QMessageBox::information(this, tr("添加工具"), tr("请从左侧工具类别树中双击选择要添加的工具"));
}

void ToolChainEditorDialog::onRemoveTool()
{
    int row = toolTable_->currentRow();
    if (row >= 0 && row < tools_.size()) {
        int ret = QMessageBox::question(this, tr("删除工具"),
                                        tr("确定要删除工具 \"%1\" 吗？").arg(tools_[row]->toolName()),
                                        QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            removeTool(row);
        }
    }
}

void ToolChainEditorDialog::onMoveToolUp()
{
    int row = toolTable_->currentRow();
    if (row > 0 && row < tools_.size()) {
        tools_.swapItemsAt(row, row - 1);
        updateToolTable();
        toolTable_->selectRow(row - 1);
    }
}

void ToolChainEditorDialog::onMoveToolDown()
{
    int row = toolTable_->currentRow();
    if (row >= 0 && row < tools_.size() - 1) {
        tools_.swapItemsAt(row, row + 1);
        updateToolTable();
        toolTable_->selectRow(row + 1);
    }
}

void ToolChainEditorDialog::onCopyTool()
{
    // TODO: 实现工具复制
    QMessageBox::information(this, tr("复制工具"), tr("功能开发中"));
}

void ToolChainEditorDialog::onCategoryItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    Q_UNUSED(item);
    // 单击选择类别
}

void ToolChainEditorDialog::onCategoryItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);

    if (!item || item->parent() == nullptr) {
        // 是父节点，展开/折叠
        return;
    }

    // 是子节点，添加工具
    QString toolName = item->text(0);
    QMessageBox::information(this, tr("添加工具"),
                            tr("添加工具: %1\n(功能开发中)").arg(toolName));
}

void ToolChainEditorDialog::onCaptureImage()
{
    emit captureImageRequested();
}

void ToolChainEditorDialog::onCameraParam()
{
    emit cameraParamRequested();
}

void ToolChainEditorDialog::onLightControl()
{
    emit lightControlRequested();
}

void ToolChainEditorDialog::onRunTool()
{
    if (currentToolIndex_ >= 0 && currentToolIndex_ < tools_.size()) {
        emit executeToolRequested(currentToolIndex_);
    }
}

void ToolChainEditorDialog::onRunAll()
{
    emit executeAllRequested();
}

void ToolChainEditorDialog::onSaveConfig()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("保存配置"),
                                                    QString(), tr("配置文件 (*.json)"));
    if (!filePath.isEmpty()) {
        if (saveConfig(filePath)) {
            QMessageBox::information(this, tr("保存成功"), tr("配置已保存"));
        } else {
            QMessageBox::warning(this, tr("保存失败"), tr("无法保存配置"));
        }
    }
}

void ToolChainEditorDialog::onLoadConfig()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("加载配置"),
                                                    QString(), tr("配置文件 (*.json)"));
    if (!filePath.isEmpty()) {
        if (loadConfig(filePath)) {
            QMessageBox::information(this, tr("加载成功"), tr("配置已加载"));
        } else {
            QMessageBox::warning(this, tr("加载失败"), tr("无法加载配置"));
        }
    }
}

void ToolChainEditorDialog::onTrainAreaChanged()
{
    // TODO: 进入训练区域绘制模式
    imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
}

void ToolChainEditorDialog::onSearchAreaChanged()
{
    // TODO: 进入搜索区域绘制模式
    imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
}

void ToolChainEditorDialog::onTrainClicked()
{
    // TODO: 执行训练
    QMessageBox::information(this, tr("训练"), tr("开始训练... (功能开发中)"));
}

void ToolChainEditorDialog::onClearTrainClicked()
{
    // TODO: 清除训练
    trainAreaLabel_->setText(tr("未设置"));
    searchAreaLabel_->setText(tr("未设置"));
}

void ToolChainEditorDialog::onAngleRangeChanged()
{
    // TODO: 更新角度范围
}

void ToolChainEditorDialog::onScoreThresholdChanged()
{
    // TODO: 更新分数阈值
}

void ToolChainEditorDialog::onMaxCountChanged()
{
    // TODO: 更新最大数量
}

void ToolChainEditorDialog::onPositionModeChanged()
{
    // TODO: 更新位置模式
}

void ToolChainEditorDialog::onPositionOffsetChanged()
{
    // TODO: 更新位置偏移
}

void ToolChainEditorDialog::onOkClicked()
{
    accept();
}

void ToolChainEditorDialog::onCancelClicked()
{
    reject();
}

void ToolChainEditorDialog::onApplyClicked()
{
    // TODO: 应用配置
    QMessageBox::information(this, tr("应用"), tr("配置已应用"));
}

} // namespace UI
} // namespace VisionForge
