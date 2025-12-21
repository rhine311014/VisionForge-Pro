/**
 * @file ShapeMatchToolDialog.cpp
 * @brief 形状匹配工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-15
 */

#include "ui/ShapeMatchToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "ui/ROI.h"
#include "ui/ModelCheckPointDialog.h"
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/ShapeModelManager.h"
#include "ui/ShapeModelLibraryDialog.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QTimer>
#include <QInputDialog>
#include <QDir>
#include <QCoreApplication>
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace UI {

ShapeMatchToolDialog::ShapeMatchToolDialog(Algorithm::ShapeMatchTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentLibraryModelId_()
    , imageViewer_(nullptr)
    , featureViewer_(nullptr)
    , mainSplitter_(nullptr)
    , leftSplitter_(nullptr)
    , paramTabWidget_(nullptr)
    , toolTypeCombo_(nullptr)
    , toolTypePreview_(nullptr)
    , polarCoordCheckBox_(nullptr)
    , drawROIBtn_(nullptr)
    , drawFreehandBtn_(nullptr)
    , clearROIBtn_(nullptr)
    , roiStatusLabel_(nullptr)
    , angleStartSpinBox_(nullptr)
    , angleExtentSpinBox_(nullptr)
    , anglePresetLabel_(nullptr)
    , pyramidLevelSpinBox_(nullptr)
    , minContrastSpinBox_(nullptr)
    , minScoreSpinBox_(nullptr)
    , minScoreSlider_(nullptr)
    , numMatchesSpinBox_(nullptr)
    , overlapSpinBox_(nullptr)
    , subPixelCombo_(nullptr)
    , greedySpinBox_(nullptr)
    , scaleMinSpinBox_(nullptr)
    , scaleMaxSpinBox_(nullptr)
    , matchTypeCombo_(nullptr)
    , getTemplateBtn_(nullptr)
    , trainModelBtn_(nullptr)
    , modelCheckPointBtn_(nullptr)
    , deleteModelBtn_(nullptr)
    , modelLibraryBtn_(nullptr)
    , modelStatusLabel_(nullptr)
    , markShapeTypeCombo_(nullptr)
    , markSize1SpinBox_(nullptr)
    , markSize2SpinBox_(nullptr)
    , markSize3SpinBox_(nullptr)
    , markSize1Label_(nullptr)
    , markSize2Label_(nullptr)
    , markSize3Label_(nullptr)
    , previewMarkBtn_(nullptr)
    , generateMarkContourBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("形状匹配设置");
    setMinimumSize(900, 600);
    resize(1000, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

ShapeMatchToolDialog::~ShapeMatchToolDialog()
{
}

void ShapeMatchToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void ShapeMatchToolDialog::updateUI()
{
    if (!tool_) return;

    // 检查关键控件是否已创建
    if (!matchTypeCombo_ || !minScoreSpinBox_ || !minScoreSlider_ ||
        !numMatchesSpinBox_ || !angleStartSpinBox_ || !angleExtentSpinBox_ ||
        !minContrastSpinBox_ || !scaleMinSpinBox_ || !scaleMaxSpinBox_ ||
        !pyramidLevelSpinBox_ || !modelStatusLabel_) {
        return;
    }

    // 阻塞所有控件信号，避免循环触发
    matchTypeCombo_->blockSignals(true);
    minScoreSpinBox_->blockSignals(true);
    minScoreSlider_->blockSignals(true);
    numMatchesSpinBox_->blockSignals(true);
    angleStartSpinBox_->blockSignals(true);
    angleExtentSpinBox_->blockSignals(true);
    minContrastSpinBox_->blockSignals(true);
    scaleMinSpinBox_->blockSignals(true);
    scaleMaxSpinBox_->blockSignals(true);
    pyramidLevelSpinBox_->blockSignals(true);

    // 更新匹配类型
    matchTypeCombo_->setCurrentIndex(static_cast<int>(tool_->getMatchType()));

    // 更新分数和数量
    minScoreSpinBox_->setValue(tool_->getMinScore());
    minScoreSlider_->setValue(static_cast<int>(tool_->getMinScore() * 100));
    numMatchesSpinBox_->setValue(tool_->getNumMatches());

    // 更新角度范围
    angleStartSpinBox_->setValue(tool_->getAngleStart() * 180.0 / 3.14159265);
    angleExtentSpinBox_->setValue(tool_->getAngleExtent() * 180.0 / 3.14159265);

    // 更新对比度
    minContrastSpinBox_->setValue(tool_->getMinContrast());

    // 更新缩放范围
    scaleMinSpinBox_->setValue(tool_->getScaleMin());
    scaleMaxSpinBox_->setValue(tool_->getScaleMax());

    // 恢复所有控件信号
    matchTypeCombo_->blockSignals(false);
    minScoreSpinBox_->blockSignals(false);
    minScoreSlider_->blockSignals(false);
    numMatchesSpinBox_->blockSignals(false);
    angleStartSpinBox_->blockSignals(false);
    angleExtentSpinBox_->blockSignals(false);
    minContrastSpinBox_->blockSignals(false);
    scaleMinSpinBox_->blockSignals(false);
    scaleMaxSpinBox_->blockSignals(false);
    pyramidLevelSpinBox_->blockSignals(false);

    // 更新模板状态
    if (tool_->hasModel()) {
        modelStatusLabel_->setText("模板状态: 已加载");
        modelStatusLabel_->setStyleSheet("color: #4CAF50;");
    } else {
        modelStatusLabel_->setText("模板状态: 未加载");
        modelStatusLabel_->setStyleSheet("color: #FF9800;");
    }
}

void ShapeMatchToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();

    // 延迟设置图像，等待HalconImageViewer初始化完成
    // HalconImageViewer在showEvent中延迟100ms初始化，所以这里延迟200ms设置图像
    if (currentImage_ && imageViewer_) {
        QTimer::singleShot(200, this, [this]() {
            if (currentImage_ && imageViewer_) {
                imageViewer_->setImage(currentImage_);
                LOG_DEBUG("ShapeMatchToolDialog: 延迟设置图像完成");
            }
        });
    }

    // 延迟更新特征预览
    QTimer::singleShot(300, this, [this]() {
        updateFeaturePreview();
    });
}

void ShapeMatchToolDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
}

void ShapeMatchToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板 - 图像显示
    QWidget* leftPanel = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel);

    // 右侧面板 - 参数设置
    QWidget* rightPanel = new QWidget(mainSplitter_);
    createRightPanel(rightPanel);

    mainSplitter_->addWidget(leftPanel);
    mainSplitter_->addWidget(rightPanel);
    mainSplitter_->setStretchFactor(0, 3);  // 图像区域占比更大
    mainSplitter_->setStretchFactor(1, 2);

    mainLayout->addWidget(mainSplitter_, 1);

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

void ShapeMatchToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 标题
    QLabel* titleLabel = new QLabel("训练图像", parent);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");

    // Halcon图像显示器
    imageViewer_ = new HalconImageViewer(parent);
    imageViewer_->setMinimumSize(400, 300);
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(titleLabel);
    layout->addWidget(imageViewer_, 1);
}

void ShapeMatchToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 顶部：特征预览区域（小窗口）
    QGroupBox* featureGroup = new QGroupBox("特征预览", parent);
    QVBoxLayout* featureLayout = new QVBoxLayout(featureGroup);
    featureLayout->setContentsMargins(4, 4, 4, 4);

    featureViewer_ = new HalconImageViewer(featureGroup);
    featureViewer_->setFixedHeight(120);  // 固定高度，小窗口
    featureViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    featureLayout->addWidget(featureViewer_);
    layout->addWidget(featureGroup);

    // 下方：选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 基本设置
    QWidget* basicTab = new QWidget(paramTabWidget_);
    QVBoxLayout* basicLayout = new QVBoxLayout(basicTab);
    basicLayout->setContentsMargins(8, 8, 8, 8);
    basicLayout->setSpacing(8);
    createToolTypeGroup(basicLayout);
    createROIGroup(basicLayout);
    createAngleGroup(basicLayout);
    basicLayout->addStretch();
    paramTabWidget_->addTab(basicTab, "基本设置");

    // Tab 2: 匹配参数
    QWidget* matchTab = new QWidget(paramTabWidget_);
    QVBoxLayout* matchLayout = new QVBoxLayout(matchTab);
    matchLayout->setContentsMargins(8, 8, 8, 8);
    matchLayout->setSpacing(8);
    createMatchParamGroup(matchLayout);
    matchLayout->addStretch();
    paramTabWidget_->addTab(matchTab, "匹配参数");

    // Tab 3: 模板管理
    QWidget* modelTab = new QWidget(paramTabWidget_);
    QVBoxLayout* modelLayout = new QVBoxLayout(modelTab);
    modelLayout->setContentsMargins(8, 8, 8, 8);
    modelLayout->setSpacing(8);
    createModelGroup(modelLayout);
    modelLayout->addStretch();
    paramTabWidget_->addTab(modelTab, "模板管理");

    layout->addWidget(paramTabWidget_, 1);
}

void ShapeMatchToolDialog::createToolTypeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("匹配工具类型", this);
    QHBoxLayout* groupLayout = new QHBoxLayout(group);

    toolTypeCombo_ = new QComboBox(this);
    toolTypeCombo_->addItem("形状匹配工具");
    toolTypeCombo_->addItem("灰度匹配工具");
    toolTypeCombo_->addItem("相关匹配工具");

    // 工具预览图标
    toolTypePreview_ = new QLabel(this);
    toolTypePreview_->setFixedSize(60, 60);
    toolTypePreview_->setStyleSheet("background-color: #333; border: 1px solid #555;");
    toolTypePreview_->setAlignment(Qt::AlignCenter);

    groupLayout->addWidget(toolTypeCombo_, 1);
    groupLayout->addWidget(toolTypePreview_);

    layout->addWidget(group);
}

void ShapeMatchToolDialog::createROIGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("ROI设置", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 极坐标复选框
    polarCoordCheckBox_ = new QCheckBox("使用极坐标", this);

    // ROI操作按钮（第一行：矩形ROI和自由绘制）
    QHBoxLayout* roiButtonLayout1 = new QHBoxLayout();
    drawROIBtn_ = new QPushButton("矩形ROI", this);
    drawROIBtn_->setToolTip("绘制矩形感兴趣区域");
    drawFreehandBtn_ = new QPushButton("自由绘制", this);
    drawFreehandBtn_->setToolTip("自由绘制轮廓（按住鼠标拖动）");
    roiButtonLayout1->addWidget(drawROIBtn_);
    roiButtonLayout1->addWidget(drawFreehandBtn_);

    // ROI操作按钮（第二行：清除）
    QHBoxLayout* roiButtonLayout2 = new QHBoxLayout();
    clearROIBtn_ = new QPushButton("清除ROI", this);
    roiButtonLayout2->addWidget(clearROIBtn_);
    roiButtonLayout2->addStretch();

    // ROI状态
    roiStatusLabel_ = new QLabel("ROI状态: 未设置", this);
    roiStatusLabel_->setStyleSheet("color: #888;");

    groupLayout->addWidget(polarCoordCheckBox_);
    groupLayout->addLayout(roiButtonLayout1);
    groupLayout->addLayout(roiButtonLayout2);
    groupLayout->addWidget(roiStatusLabel_);

    layout->addWidget(group);
}

void ShapeMatchToolDialog::createAngleGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("角度设置", this);
    QGridLayout* groupLayout = new QGridLayout(group);

    groupLayout->addWidget(new QLabel("角度范围:"), 0, 0);

    angleStartSpinBox_ = new QDoubleSpinBox(this);
    angleStartSpinBox_->setRange(-180.0, 180.0);
    angleStartSpinBox_->setValue(-30.0);
    angleStartSpinBox_->setSuffix("°");

    QLabel* toLabel = new QLabel("~");
    toLabel->setAlignment(Qt::AlignCenter);

    angleExtentSpinBox_ = new QDoubleSpinBox(this);
    angleExtentSpinBox_->setRange(0.0, 360.0);
    angleExtentSpinBox_->setValue(60.0);
    angleExtentSpinBox_->setSuffix("°");

    QHBoxLayout* angleLayout = new QHBoxLayout();
    angleLayout->addWidget(angleStartSpinBox_);
    angleLayout->addWidget(toLabel);
    angleLayout->addWidget(angleExtentSpinBox_);

    groupLayout->addLayout(angleLayout, 0, 1);

    layout->addWidget(group);
}

void ShapeMatchToolDialog::createMatchParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("匹配参数", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 匹配模式
    groupLayout->addWidget(new QLabel("匹配模式:"), row, 0);
    matchTypeCombo_ = new QComboBox(this);
    matchTypeCombo_->addItem("标准匹配（仅旋转）");
    matchTypeCombo_->addItem("缩放匹配（旋转+缩放）");
    matchTypeCombo_->addItem("仿射匹配（旋转+各向异性缩放）");
    groupLayout->addWidget(matchTypeCombo_, row, 1);
    row++;

    // 金字塔层数
    groupLayout->addWidget(new QLabel("金字塔层数:"), row, 0);
    pyramidLevelSpinBox_ = new QSpinBox(this);
    pyramidLevelSpinBox_->setRange(1, 10);
    pyramidLevelSpinBox_->setValue(4);
    groupLayout->addWidget(pyramidLevelSpinBox_, row, 1);
    row++;

    // 最小对比度
    groupLayout->addWidget(new QLabel("最小对比度:"), row, 0);
    minContrastSpinBox_ = new QSpinBox(this);
    minContrastSpinBox_->setRange(1, 255);
    minContrastSpinBox_->setValue(30);
    groupLayout->addWidget(minContrastSpinBox_, row, 1);
    row++;

    // 最小分数
    groupLayout->addWidget(new QLabel("最小分数:"), row, 0);
    QHBoxLayout* scoreLayout = new QHBoxLayout();
    minScoreSpinBox_ = new QDoubleSpinBox(this);
    minScoreSpinBox_->setRange(0.0, 1.0);
    minScoreSpinBox_->setSingleStep(0.05);
    minScoreSpinBox_->setValue(0.5);
    minScoreSlider_ = new QSlider(Qt::Horizontal, this);
    minScoreSlider_->setRange(0, 100);
    minScoreSlider_->setValue(50);
    scoreLayout->addWidget(minScoreSpinBox_);
    scoreLayout->addWidget(minScoreSlider_);
    groupLayout->addLayout(scoreLayout, row, 1);
    row++;

    // 最大匹配数量
    groupLayout->addWidget(new QLabel("最大匹配数:"), row, 0);
    numMatchesSpinBox_ = new QSpinBox(this);
    numMatchesSpinBox_->setRange(1, 100);
    numMatchesSpinBox_->setValue(1);
    groupLayout->addWidget(numMatchesSpinBox_, row, 1);
    row++;

    // 最大重叠
    groupLayout->addWidget(new QLabel("最大重叠:"), row, 0);
    overlapSpinBox_ = new QDoubleSpinBox(this);
    overlapSpinBox_->setRange(0.0, 1.0);
    overlapSpinBox_->setSingleStep(0.1);
    overlapSpinBox_->setValue(0.5);
    groupLayout->addWidget(overlapSpinBox_, row, 1);
    row++;

    // 亚像素精度
    groupLayout->addWidget(new QLabel("亚像素精度:"), row, 0);
    subPixelCombo_ = new QComboBox(this);
    subPixelCombo_->addItem("none");
    subPixelCombo_->addItem("interpolation");
    subPixelCombo_->addItem("least_squares");
    subPixelCombo_->addItem("least_squares_high");
    subPixelCombo_->setCurrentIndex(2);
    groupLayout->addWidget(subPixelCombo_, row, 1);
    row++;

    // 贪婪度
    groupLayout->addWidget(new QLabel("贪婪度:"), row, 0);
    greedySpinBox_ = new QDoubleSpinBox(this);
    greedySpinBox_->setRange(0.0, 1.0);
    greedySpinBox_->setSingleStep(0.1);
    greedySpinBox_->setValue(0.9);
    groupLayout->addWidget(greedySpinBox_, row, 1);
    row++;

    // 缩放范围
    groupLayout->addWidget(new QLabel("缩放范围:"), row, 0);
    QHBoxLayout* scaleLayout = new QHBoxLayout();
    scaleMinSpinBox_ = new QDoubleSpinBox(this);
    scaleMinSpinBox_->setRange(0.1, 2.0);
    scaleMinSpinBox_->setSingleStep(0.1);
    scaleMinSpinBox_->setValue(0.8);
    scaleMaxSpinBox_ = new QDoubleSpinBox(this);
    scaleMaxSpinBox_->setRange(0.1, 2.0);
    scaleMaxSpinBox_->setSingleStep(0.1);
    scaleMaxSpinBox_->setValue(1.2);
    scaleLayout->addWidget(scaleMinSpinBox_);
    scaleLayout->addWidget(new QLabel("~"));
    scaleLayout->addWidget(scaleMaxSpinBox_);
    groupLayout->addLayout(scaleLayout, row, 1);

    layout->addWidget(group);
}

void ShapeMatchToolDialog::createModelGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("模板管理", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 模板状态
    modelStatusLabel_ = new QLabel("模板状态: 未加载", this);
    modelStatusLabel_->setStyleSheet("color: #FF9800;");
    groupLayout->addWidget(modelStatusLabel_);

    // 按钮行1
    QHBoxLayout* btnRow1 = new QHBoxLayout();
    getTemplateBtn_ = new QPushButton("获取模板图像", this);
    trainModelBtn_ = new QPushButton("训练", this);
    btnRow1->addWidget(getTemplateBtn_);
    btnRow1->addWidget(trainModelBtn_);
    groupLayout->addLayout(btnRow1);

    // 按钮行2
    QHBoxLayout* btnRow2 = new QHBoxLayout();
    modelCheckPointBtn_ = new QPushButton("模型校验点", this);
    deleteModelBtn_ = new QPushButton("删除", this);
    btnRow2->addWidget(modelCheckPointBtn_);
    btnRow2->addWidget(deleteModelBtn_);
    groupLayout->addLayout(btnRow2);

    // 模板库按钮
    modelLibraryBtn_ = new QPushButton("模板库...", this);
    groupLayout->addWidget(modelLibraryBtn_);

    // ========== 标准Mark轮廓生成（高噪声场景优化） ==========
    QGroupBox* markGroup = new QGroupBox("标准Mark轮廓（高噪声优化）", this);
    QVBoxLayout* markLayout = new QVBoxLayout(markGroup);

    // 形状类型选择
    QHBoxLayout* shapeTypeLayout = new QHBoxLayout();
    shapeTypeLayout->addWidget(new QLabel("形状类型:"));
    markShapeTypeCombo_ = new QComboBox(this);
    markShapeTypeCombo_->addItem("十字形", static_cast<int>(Algorithm::MarkShapeType::Cross));
    markShapeTypeCombo_->addItem("T字形", static_cast<int>(Algorithm::MarkShapeType::TShape));
    markShapeTypeCombo_->addItem("L字形", static_cast<int>(Algorithm::MarkShapeType::LShape));
    markShapeTypeCombo_->addItem("圆形", static_cast<int>(Algorithm::MarkShapeType::Circle));
    markShapeTypeCombo_->addItem("圆环形", static_cast<int>(Algorithm::MarkShapeType::Ring));
    markShapeTypeCombo_->addItem("矩形", static_cast<int>(Algorithm::MarkShapeType::Rectangle));
    markShapeTypeCombo_->addItem("三角形", static_cast<int>(Algorithm::MarkShapeType::Triangle));
    markShapeTypeCombo_->addItem("菱形", static_cast<int>(Algorithm::MarkShapeType::Diamond));
    markShapeTypeCombo_->addItem("箭头形", static_cast<int>(Algorithm::MarkShapeType::Arrow));
    shapeTypeLayout->addWidget(markShapeTypeCombo_);
    markLayout->addLayout(shapeTypeLayout);

    // 尺寸参数
    QGridLayout* markParamLayout = new QGridLayout();

    markSize1Label_ = new QLabel("臂长(像素):", this);
    markParamLayout->addWidget(markSize1Label_, 0, 0);
    markSize1SpinBox_ = new QDoubleSpinBox(this);
    markSize1SpinBox_->setRange(10, 500);
    markSize1SpinBox_->setValue(100);
    markSize1SpinBox_->setSuffix(" px");
    markParamLayout->addWidget(markSize1SpinBox_, 0, 1);

    markSize2Label_ = new QLabel("臂宽(像素):", this);
    markParamLayout->addWidget(markSize2Label_, 1, 0);
    markSize2SpinBox_ = new QDoubleSpinBox(this);
    markSize2SpinBox_->setRange(5, 200);
    markSize2SpinBox_->setValue(20);
    markSize2SpinBox_->setSuffix(" px");
    markParamLayout->addWidget(markSize2SpinBox_, 1, 1);

    markSize3Label_ = new QLabel("线宽(像素):", this);
    markParamLayout->addWidget(markSize3Label_, 2, 0);
    markSize3SpinBox_ = new QDoubleSpinBox(this);
    markSize3SpinBox_->setRange(5, 100);
    markSize3SpinBox_->setValue(20);
    markSize3SpinBox_->setSuffix(" px");
    markParamLayout->addWidget(markSize3SpinBox_, 2, 1);

    // 默认隐藏第三个参数
    markSize3Label_->setVisible(false);
    markSize3SpinBox_->setVisible(false);

    markLayout->addLayout(markParamLayout);

    // 按钮行
    QHBoxLayout* markBtnLayout = new QHBoxLayout();
    previewMarkBtn_ = new QPushButton("预览轮廓", this);
    previewMarkBtn_->setToolTip("在特征预览窗口中显示当前形状轮廓");
    markBtnLayout->addWidget(previewMarkBtn_);

    generateMarkContourBtn_ = new QPushButton("生成并训练", this);
    generateMarkContourBtn_->setToolTip("使用干净的标准轮廓创建模板，适用于高噪声图像");
    markBtnLayout->addWidget(generateMarkContourBtn_);

    markLayout->addLayout(markBtnLayout);

    groupLayout->addWidget(markGroup);

    layout->addWidget(group);
}

void ShapeMatchToolDialog::connectSignals()
{
    // 匹配参数
    connect(minScoreSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onMinScoreChanged);
    connect(minScoreSlider_, &QSlider::valueChanged, this, [this](int value) {
        minScoreSpinBox_->setValue(value / 100.0);
    });
    connect(numMatchesSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onNumMatchesChanged);
    connect(angleStartSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onAngleRangeChanged);
    connect(angleExtentSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onAngleRangeChanged);
    connect(minContrastSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onContrastChanged);
    connect(scaleMinSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onScaleRangeChanged);
    connect(scaleMaxSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onScaleRangeChanged);
    connect(matchTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ShapeMatchToolDialog::onMatchTypeChanged);
    connect(pyramidLevelSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShapeMatchToolDialog::onPyramidLevelChanged);

    // ROI操作
    connect(drawROIBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onDrawROIClicked);
    connect(drawFreehandBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onDrawFreehandClicked);
    connect(clearROIBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onClearROIClicked);

    // ROI创建完成信号
    if (imageViewer_) {
        connect(imageViewer_, &HalconImageViewer::roiCreated, this, [this](ROIShapePtr roi) {
            Q_UNUSED(roi);
            std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
            roiStatusLabel_->setText(QString("ROI状态: 已设置 (%1个)").arg(rois.size()));
            roiStatusLabel_->setStyleSheet("color: #4CAF50;");
            // 绘制完成后切换回选择模式
            imageViewer_->setInteractionMode(HalconImageViewer::SelectMode);
        });
    }

    // 模板操作
    connect(getTemplateBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onGetTemplateImageClicked);
    connect(trainModelBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onTrainModelClicked);
    connect(modelCheckPointBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onModelCheckPointClicked);
    connect(deleteModelBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onClearModelClicked);
    connect(modelLibraryBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onModelLibraryClicked);

    // 标准Mark轮廓生成
    connect(markShapeTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ShapeMatchToolDialog::onMarkShapeTypeChanged);
    connect(previewMarkBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onPreviewMarkContour);
    connect(generateMarkContourBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onGenerateMarkContourClicked);

    // 对话框按钮
    connect(okBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &ShapeMatchToolDialog::onApplyClicked);
}

void ShapeMatchToolDialog::onMinScoreChanged(double value)
{
    if (!tool_) return;
    // 阻塞tool信号避免触发外部组件更新
    tool_->blockSignals(true);
    tool_->setMinScore(value);
    tool_->blockSignals(false);
    minScoreSlider_->blockSignals(true);
    minScoreSlider_->setValue(static_cast<int>(value * 100));
    minScoreSlider_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchToolDialog::onNumMatchesChanged(int value)
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setNumMatches(value);
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchToolDialog::onAngleRangeChanged()
{
    if (!tool_) return;
    double startDeg = angleStartSpinBox_->value();
    double extentDeg = angleExtentSpinBox_->value();
    tool_->blockSignals(true);
    tool_->setAngleStart(startDeg * 3.14159265 / 180.0);
    tool_->setAngleExtent(extentDeg * 3.14159265 / 180.0);
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchToolDialog::onContrastChanged(int value)
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setMinContrast(value);
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchToolDialog::onScaleRangeChanged()
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setScaleMin(scaleMinSpinBox_->value());
    tool_->setScaleMax(scaleMaxSpinBox_->value());
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchToolDialog::onMatchTypeChanged(int index)
{
    if (!tool_) return;
    if (index < 0 || index > 2) return;

    tool_->blockSignals(true);
    tool_->setMatchType(static_cast<Algorithm::ShapeMatchTool::MatchType>(index));
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchToolDialog::onPyramidLevelChanged(int value)
{
    Q_UNUSED(value);
    // 金字塔层数暂不支持动态设置
    emit parameterChanged();
}

void ShapeMatchToolDialog::onDrawROIClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    if (imageViewer_) {
        imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
        roiStatusLabel_->setText("ROI状态: 正在绘制... (拖动鼠标绘制矩形)");
        roiStatusLabel_->setStyleSheet("color: #2196F3;");
    }
}

void ShapeMatchToolDialog::onDrawFreehandClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    if (imageViewer_) {
        imageViewer_->setInteractionMode(HalconImageViewer::DrawFreehand);
        roiStatusLabel_->setText("ROI状态: 自由绘制中... (按住鼠标拖动绘制轮廓)");
        roiStatusLabel_->setStyleSheet("color: #E91E63;");  // 使用不同颜色区分
    }
}

void ShapeMatchToolDialog::onClearROIClicked()
{
    if (imageViewer_) {
        imageViewer_->clearROIs();
        roiStatusLabel_->setText("ROI状态: 未设置");
    }
}

void ShapeMatchToolDialog::onGetTemplateImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "选择模板图像",
        QString(),
        "图像文件 (*.png *.jpg *.bmp *.tif);;所有文件 (*)");

    if (!filePath.isEmpty()) {
        // 使用Qt读取文件（支持中文路径），然后用OpenCV解码
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "错误", "无法打开图像文件");
            return;
        }

        QByteArray fileData = file.readAll();
        file.close();

        // 使用OpenCV解码图像数据
        std::vector<uchar> buffer(fileData.begin(), fileData.end());
        cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

        if (!mat.empty()) {
            // 使用内存池分配图像
            Base::ImageData::Ptr image = Base::ImageMemoryPool::instance().allocate(
                mat.cols, mat.rows, mat.type());
            if (image) {
                mat.copyTo(image->mat());
                setImage(image);
                LOG_INFO(QString("已加载模板图像: %1").arg(filePath));
            }
        } else {
            QMessageBox::warning(this, "错误", "无法解码图像文件");
        }
    }
}

void ShapeMatchToolDialog::onTrainModelClicked()
{
    if (!tool_ || !currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    // 获取ROI
    std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
    if (rois.empty()) {
        QMessageBox::warning(this, "警告", "请先绘制ROI区域");
        return;
    }

    // 获取第一个ROI
    ROIShapePtr roi = rois.front();

#ifdef _WIN32
    // 检查是否是自由绘制轮廓
    if (roi->getType() == ROIType::Freehand) {
        auto freehand = std::dynamic_pointer_cast<ROIFreehand>(roi);
        if (freehand && freehand->pointCount() >= 3) {
            LOG_INFO("从自由绘制轮廓训练模板...");

            // 将ROIFreehand转换为HXLDCont
            QVector<QPoint> points = freehand->getPoints();
            HTuple rows, cols;
            for (const QPoint& pt : points) {
                rows.Append(static_cast<double>(pt.y()));
                cols.Append(static_cast<double>(pt.x()));
            }

            // 生成XLD轮廓
            HXLDCont contour;
            GenContourPolygonXld(&contour, rows, cols);

            // 从轮廓训练模板
            if (tool_->trainModelFromContour(contour, currentImage_)) {
                LOG_INFO("从自由绘制轮廓训练模板成功");
                currentLibraryModelId_.clear();  // 训练新模板，清除模板库ID
                updateUI();
                updateFeaturePreview();
                // 询问是否保存到模板库
                saveToModelLibrary();
            } else {
                QMessageBox::warning(this, "训练失败", "从自由绘制轮廓训练模板失败");
            }
            return;
        }
    }
#endif

    // 原有逻辑：从图像区域训练
    QRect rect = roi->boundingRect();

    LOG_DEBUG(QString("开始训练模板: ROI区域 top=%1, left=%2, bottom=%3, right=%4")
        .arg(rect.top()).arg(rect.left()).arg(rect.bottom()).arg(rect.right()));

    // 直接在对话框内执行训练
    if (tool_->trainModel(currentImage_,
        rect.top(),
        rect.left(),
        rect.bottom(),
        rect.right())) {
        LOG_INFO("模板训练成功");
        currentLibraryModelId_.clear();  // 训练新模板，清除模板库ID
        updateUI();
        updateFeaturePreview();  // 更新特征预览
        // 询问是否保存到模板库
        saveToModelLibrary();
    } else {
        QMessageBox::warning(this, "训练失败", "模板训练失败，请检查ROI区域是否有效");
    }
}

void ShapeMatchToolDialog::onModelCheckPointClicked()
{
    if (!tool_ || !tool_->hasModel()) {
        QMessageBox::warning(this, "警告", "没有有效的模板，请先训练或加载模板");
        return;
    }

    // 创建并显示校验点编辑对话框
    ModelCheckPointDialog dialog(tool_, this);

    // 设置当前图像
    if (currentImage_) {
        dialog.setImage(currentImage_);
    }

    // 如果有匹配结果，设置匹配位置用于校验点显示
    // 这里暂时使用图像中心作为参考点
    if (currentImage_) {
        double centerRow = currentImage_->height() / 2.0;
        double centerCol = currentImage_->width() / 2.0;
        dialog.setMatchResult(centerRow, centerCol, 0);
    }

    // 连接信号
    connect(&dialog, &ModelCheckPointDialog::checkPointsChanged, this, [this]() {
        emit parameterChanged();
        LOG_INFO("校验点已更新");
    });

    dialog.exec();
}

void ShapeMatchToolDialog::onLoadModelClicked()
{
    QString filePath = getModelPath(false);
    if (filePath.isEmpty()) return;

    if (tool_ && tool_->loadModel(filePath)) {
        currentLibraryModelId_.clear();  // 从文件加载，不是模板库模板
        updateUI();
        updateFeaturePreview();  // 更新特征预览
        LOG_INFO(QString("已加载模板: %1").arg(filePath));
    } else {
        QMessageBox::warning(this, "错误", "无法加载模板文件");
    }
}

void ShapeMatchToolDialog::onSaveModelClicked()
{
    if (!tool_ || !tool_->hasModel()) {
        QMessageBox::warning(this, "警告", "没有有效的模板可保存");
        return;
    }

    QString filePath = getModelPath(true);
    if (filePath.isEmpty()) return;

    if (tool_->saveModel(filePath)) {
        LOG_INFO(QString("已保存模板: %1").arg(filePath));
    } else {
        QMessageBox::warning(this, "错误", "无法保存模板文件");
    }
}

void ShapeMatchToolDialog::onClearModelClicked()
{
    if (!tool_) return;

    int ret = QMessageBox::question(this, "确认",
                                    "确定要删除当前模板吗？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        tool_->clearModel();
        currentLibraryModelId_.clear();  // 清除模板库ID
        updateUI();
        updateFeaturePreview();  // 清除特征预览
        LOG_INFO("模板已删除");
    }
}

void ShapeMatchToolDialog::onModelLibraryClicked()
{
    ShapeModelLibraryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString selectedPath = dialog.getSelectedModelPath();
        QString selectedId = dialog.getSelectedModelId();
        if (!selectedPath.isEmpty() && tool_) {
            if (tool_->loadModel(selectedPath)) {
                // 保存模板库模型ID，用于后续直接更新
                currentLibraryModelId_ = selectedId;

                // 从模板库恢复保存的参数
                Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
                auto info = manager.getModel(selectedId);
                if (info) {
                    // 恢复匹配参数到UI控件
                    minScoreSpinBox_->blockSignals(true);
                    minScoreSlider_->blockSignals(true);
                    numMatchesSpinBox_->blockSignals(true);
                    overlapSpinBox_->blockSignals(true);
                    greedySpinBox_->blockSignals(true);
                    scaleMinSpinBox_->blockSignals(true);
                    scaleMaxSpinBox_->blockSignals(true);
                    matchTypeCombo_->blockSignals(true);
                    angleStartSpinBox_->blockSignals(true);
                    angleExtentSpinBox_->blockSignals(true);
                    minContrastSpinBox_->blockSignals(true);

                    minScoreSpinBox_->setValue(info->minScore);
                    minScoreSlider_->setValue(static_cast<int>(info->minScore * 100));
                    numMatchesSpinBox_->setValue(info->numMatches);
                    overlapSpinBox_->setValue(info->maxOverlap);
                    greedySpinBox_->setValue(info->greediness);
                    scaleMinSpinBox_->setValue(info->scaleMin);
                    scaleMaxSpinBox_->setValue(info->scaleMax);
                    matchTypeCombo_->setCurrentIndex(info->matchType);
                    angleStartSpinBox_->setValue(info->angleStart * 180.0 / 3.14159265);
                    angleExtentSpinBox_->setValue(info->angleExtent * 180.0 / 3.14159265);
                    minContrastSpinBox_->setValue(info->minContrast);

                    // 恢复亚像素精度选项
                    int subPixelIndex = subPixelCombo_->findText(info->subPixel);
                    if (subPixelIndex >= 0) {
                        subPixelCombo_->blockSignals(true);
                        subPixelCombo_->setCurrentIndex(subPixelIndex);
                        subPixelCombo_->blockSignals(false);
                    }

                    minScoreSpinBox_->blockSignals(false);
                    minScoreSlider_->blockSignals(false);
                    numMatchesSpinBox_->blockSignals(false);
                    overlapSpinBox_->blockSignals(false);
                    greedySpinBox_->blockSignals(false);
                    scaleMinSpinBox_->blockSignals(false);
                    scaleMaxSpinBox_->blockSignals(false);
                    matchTypeCombo_->blockSignals(false);
                    angleStartSpinBox_->blockSignals(false);
                    angleExtentSpinBox_->blockSignals(false);
                    minContrastSpinBox_->blockSignals(false);

                    // 同步参数到工具
                    tool_->setMinScore(info->minScore);
                    tool_->setNumMatches(info->numMatches);
                    tool_->setScaleMin(info->scaleMin);
                    tool_->setScaleMax(info->scaleMax);
                    tool_->setMatchType(static_cast<Algorithm::ShapeMatchTool::MatchType>(info->matchType));

                    LOG_INFO(QString("已恢复模板参数: minScore=%1, numMatches=%2, scaleRange=[%3, %4]")
                        .arg(info->minScore).arg(info->numMatches)
                        .arg(info->scaleMin).arg(info->scaleMax));
                }

                updateUI();
                updateFeaturePreview();  // 更新特征预览
                LOG_INFO(QString("从模板库加载模板: %1 (ID: %2)").arg(selectedPath).arg(selectedId));
            }
        }
    }
}

void ShapeMatchToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void ShapeMatchToolDialog::onCancelClicked()
{
    reject();
}

void ShapeMatchToolDialog::onApplyClicked()
{
    applyParameters();

    // 如果是从模板库加载的模板，询问用户是否更新模板库
    if (!currentLibraryModelId_.isEmpty() && tool_ && tool_->hasModel()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "更新模板库",
            "是否将修改后的参数更新到模板库？",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);

        if (reply == QMessageBox::Yes) {
            if (updateModelInLibrary()) {
                QMessageBox::information(this, "成功", "模板库已更新！");
            }
        }
    }

    emit parameterChanged();
}

QString ShapeMatchToolDialog::getModelPath(bool forSave)
{
    if (forSave) {
        return QFileDialog::getSaveFileName(
            this, "保存模板",
            QString(),
            "Halcon模板 (*.shm);;所有文件 (*)");
    } else {
        return QFileDialog::getOpenFileName(
            this, "加载模板",
            QString(),
            "Halcon模板 (*.shm);;所有文件 (*)");
    }
}

void ShapeMatchToolDialog::applyParameters()
{
    if (!tool_) return;

    // 应用所有参数到工具
    tool_->setMinScore(minScoreSpinBox_->value());
    tool_->setNumMatches(numMatchesSpinBox_->value());
    tool_->setAngleStart(angleStartSpinBox_->value() * 3.14159265 / 180.0);
    tool_->setAngleExtent(angleExtentSpinBox_->value() * 3.14159265 / 180.0);
    tool_->setMinContrast(minContrastSpinBox_->value());
    tool_->setScaleMin(scaleMinSpinBox_->value());
    tool_->setScaleMax(scaleMaxSpinBox_->value());
    tool_->setMatchType(static_cast<Algorithm::ShapeMatchTool::MatchType>(matchTypeCombo_->currentIndex()));
    // 金字塔层数暂不支持动态设置

    LOG_INFO("参数已应用");
}

void ShapeMatchToolDialog::updateFeaturePreview()
{
#ifdef _WIN32
    if (!tool_ || !featureViewer_) return;

    if (!tool_->hasModel()) {
        // 清除特征预览
        featureViewer_->clearImage();
        return;
    }

    try {
        // 获取模型轮廓
        HXLDCont contours = tool_->getModelContours();
        if (contours.CountObj() == 0) {
            LOG_WARNING("模型轮廓为空");
            return;
        }

        // 获取轮廓的边界框（可能返回多个值，需要计算整体边界）
        HTuple minRowArr, maxRowArr, minColArr, maxColArr;
        SmallestRectangle1Xld(contours, &minRowArr, &maxRowArr, &minColArr, &maxColArr);

        // 计算所有轮廓的整体边界
        double overallMinRow = 1e9, overallMaxRow = -1e9;
        double overallMinCol = 1e9, overallMaxCol = -1e9;

        for (Hlong i = 0; i < minRowArr.Length(); ++i) {
            if (minRowArr[i].D() < overallMinRow) overallMinRow = minRowArr[i].D();
            if (maxRowArr[i].D() > overallMaxRow) overallMaxRow = maxRowArr[i].D();
            if (minColArr[i].D() < overallMinCol) overallMinCol = minColArr[i].D();
            if (maxColArr[i].D() > overallMaxCol) overallMaxCol = maxColArr[i].D();
        }

        LOG_DEBUG(QString("轮廓边界: Row[%1, %2], Col[%3, %4]")
            .arg(overallMinRow, 0, 'f', 1).arg(overallMaxRow, 0, 'f', 1)
            .arg(overallMinCol, 0, 'f', 1).arg(overallMaxCol, 0, 'f', 1));

        // 计算轮廓实际尺寸
        double contourWidth = overallMaxCol - overallMinCol;
        double contourHeight = overallMaxRow - overallMinRow;

        // 计算画布大小（添加边距，确保足够大）
        int margin = 40;
        int width = static_cast<int>(contourWidth) + margin * 2;
        int height = static_cast<int>(contourHeight) + margin * 2;

        // 确保画布有最小尺寸
        if (width < 200) width = 200;
        if (height < 150) height = 150;

        // 创建黑色背景图像
        cv::Mat canvas(height, width, CV_8UC3, cv::Scalar(30, 30, 30));

        // 计算轮廓中心
        double contourCenterRow = (overallMinRow + overallMaxRow) / 2.0;
        double contourCenterCol = (overallMinCol + overallMaxCol) / 2.0;

        // 计算画布中心
        double canvasCenterRow = height / 2.0;
        double canvasCenterCol = width / 2.0;

        // 平移轮廓到画布中心
        double offsetRow = canvasCenterRow - contourCenterRow;
        double offsetCol = canvasCenterCol - contourCenterCol;

        LOG_DEBUG(QString("画布大小: %1x%2, 轮廓中心: (%3, %4), 偏移: (%5, %6)")
            .arg(width).arg(height)
            .arg(contourCenterRow, 0, 'f', 1).arg(contourCenterCol, 0, 'f', 1)
            .arg(offsetRow, 0, 'f', 1).arg(offsetCol, 0, 'f', 1));

        // 创建平移矩阵
        HTuple homMat2D;
        HomMat2dIdentity(&homMat2D);
        HomMat2dTranslate(homMat2D, offsetRow, offsetCol, &homMat2D);

        // 变换轮廓
        HXLDCont transformedContours;
        AffineTransContourXld(contours, &transformedContours, homMat2D);

        // 绘制轮廓到OpenCV图像
        for (Hlong i = 1; i <= transformedContours.CountObj(); ++i) {
            HXLDCont singleContour = transformedContours.SelectObj(i);
            HTuple rows, cols;
            singleContour.GetContourXld(&rows, &cols);

            if (rows.Length() > 1) {
                std::vector<cv::Point> points;
                for (Hlong j = 0; j < rows.Length(); ++j) {
                    int x = static_cast<int>(cols[j].D() + 0.5);
                    int y = static_cast<int>(rows[j].D() + 0.5);
                    // 确保点在画布范围内（使用括号避免Windows宏冲突）
                    x = (std::max)(0, (std::min)(x, width - 1));
                    y = (std::max)(0, (std::min)(y, height - 1));
                    points.push_back(cv::Point(x, y));
                }
                // 使用闭合多边形绘制（十字轮廓是闭合的）
                cv::polylines(canvas, points, true, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
            }
        }

        // 在中心绘制参考点标记
        cv::circle(canvas, cv::Point(width / 2, height / 2), 3, cv::Scalar(0, 0, 255), -1);
        cv::line(canvas, cv::Point(width / 2 - 8, height / 2), cv::Point(width / 2 + 8, height / 2), cv::Scalar(0, 0, 255), 1);
        cv::line(canvas, cv::Point(width / 2, height / 2 - 8), cv::Point(width / 2, height / 2 + 8), cv::Scalar(0, 0, 255), 1);

        // 添加标注文字
        cv::putText(canvas, "Model Contours", cv::Point(10, height - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(128, 128, 128), 1);

        // 使用内存池分配预览图像
        auto previewImage = Base::ImageMemoryPool::instance().allocate(
            canvas.cols, canvas.rows, canvas.type());
        if (previewImage) {
            canvas.copyTo(previewImage->mat());
            featureViewer_->setImage(previewImage);
        }

        // 延迟调用fitToWindow，确保图像完全显示
        QTimer::singleShot(100, this, [this]() {
            if (featureViewer_) {
                featureViewer_->fitToWindow();
            }
        });

        LOG_DEBUG(QString("特征预览已更新，轮廓数量: %1").arg(transformedContours.CountObj()));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("更新特征预览失败: %1").arg(e.ErrorMessage().Text()));
    }
#endif
}

void ShapeMatchToolDialog::onMarkShapeTypeChanged(int index)
{
    // 根据选择的形状类型更新参数标签和可见性
    Algorithm::MarkShapeType shapeType = static_cast<Algorithm::MarkShapeType>(
        markShapeTypeCombo_->currentData().toInt());

    switch (shapeType) {
        case Algorithm::MarkShapeType::Cross:
            markSize1Label_->setText("臂长(像素):");
            markSize2Label_->setText("臂宽(像素):");
            markSize3Label_->setVisible(false);
            markSize3SpinBox_->setVisible(false);
            markSize2Label_->setVisible(true);
            markSize2SpinBox_->setVisible(true);
            break;

        case Algorithm::MarkShapeType::TShape:
        case Algorithm::MarkShapeType::LShape:
            markSize1Label_->setText("宽度(像素):");
            markSize2Label_->setText("高度(像素):");
            markSize3Label_->setText("线宽(像素):");
            markSize2Label_->setVisible(true);
            markSize2SpinBox_->setVisible(true);
            markSize3Label_->setVisible(true);
            markSize3SpinBox_->setVisible(true);
            break;

        case Algorithm::MarkShapeType::Circle:
            markSize1Label_->setText("半径(像素):");
            markSize2Label_->setVisible(false);
            markSize2SpinBox_->setVisible(false);
            markSize3Label_->setVisible(false);
            markSize3SpinBox_->setVisible(false);
            break;

        case Algorithm::MarkShapeType::Ring:
            markSize1Label_->setText("外径(像素):");
            markSize2Label_->setText("内径(像素):");
            markSize2Label_->setVisible(true);
            markSize2SpinBox_->setVisible(true);
            markSize3Label_->setVisible(false);
            markSize3SpinBox_->setVisible(false);
            break;

        case Algorithm::MarkShapeType::Rectangle:
        case Algorithm::MarkShapeType::Diamond:
            markSize1Label_->setText("宽度(像素):");
            markSize2Label_->setText("高度(像素):");
            markSize2Label_->setVisible(true);
            markSize2SpinBox_->setVisible(true);
            markSize3Label_->setVisible(false);
            markSize3SpinBox_->setVisible(false);
            break;

        case Algorithm::MarkShapeType::Triangle:
            markSize1Label_->setText("边长(像素):");
            markSize2Label_->setVisible(false);
            markSize2SpinBox_->setVisible(false);
            markSize3Label_->setVisible(false);
            markSize3SpinBox_->setVisible(false);
            break;

        case Algorithm::MarkShapeType::Arrow:
            markSize1Label_->setText("长度(像素):");
            markSize2Label_->setText("头宽(像素):");
            markSize3Label_->setText("杆宽(像素):");
            markSize2Label_->setVisible(true);
            markSize2SpinBox_->setVisible(true);
            markSize3Label_->setVisible(true);
            markSize3SpinBox_->setVisible(true);
            break;

        default:
            break;
    }

    LOG_DEBUG(QString("Mark形状类型已更改为: %1").arg(Algorithm::MarkShapeParams::getShapeName(shapeType)));
}

void ShapeMatchToolDialog::onPreviewMarkContour()
{
#ifdef _WIN32
    try {
        // 构建形状参数
        Algorithm::MarkShapeParams params;
        params.type = static_cast<Algorithm::MarkShapeType>(markShapeTypeCombo_->currentData().toInt());
        params.size1 = markSize1SpinBox_->value();
        params.size2 = markSize2SpinBox_->value();
        params.size3 = markSize3SpinBox_->value();

        // 生成轮廓
        HXLDCont contour = Algorithm::ShapeMatchTool::generateMarkContour(params);

        if (contour.CountObj() == 0) {
            QMessageBox::warning(this, "预览失败", "无法生成轮廓");
            return;
        }

        // 获取轮廓边界
        HTuple minRow, maxRow, minCol, maxCol;
        SmallestRectangle1Xld(contour, &minRow, &maxRow, &minCol, &maxCol);

        // 计算画布大小
        double contourWidth = maxCol.D() - minCol.D();
        double contourHeight = maxRow.D() - minRow.D();
        int margin = 40;
        int width = static_cast<int>(contourWidth) + margin * 2;
        int height = static_cast<int>(contourHeight) + margin * 2;
        if (width < 200) width = 200;
        if (height < 150) height = 150;

        // 创建画布
        cv::Mat canvas(height, width, CV_8UC3, cv::Scalar(30, 30, 30));

        // 平移轮廓到画布中心
        double contourCenterRow = (minRow.D() + maxRow.D()) / 2.0;
        double contourCenterCol = (minCol.D() + maxCol.D()) / 2.0;
        double offsetRow = height / 2.0 - contourCenterRow;
        double offsetCol = width / 2.0 - contourCenterCol;

        HTuple homMat2D;
        HomMat2dIdentity(&homMat2D);
        HomMat2dTranslate(homMat2D, offsetRow, offsetCol, &homMat2D);

        HXLDCont transformedContour;
        AffineTransContourXld(contour, &transformedContour, homMat2D);

        // 绘制轮廓
        HTuple numContours;
        CountObj(transformedContour, &numContours);
        for (int i = 1; i <= numContours.I(); ++i) {
            HXLDCont singleContour;
            SelectObj(transformedContour, &singleContour, i);

            HTuple rows, cols;
            GetContourXld(singleContour, &rows, &cols);

            std::vector<cv::Point> points;
            for (Hlong j = 0; j < rows.Length(); ++j) {
                int x = static_cast<int>(cols[j].D() + 0.5);
                int y = static_cast<int>(rows[j].D() + 0.5);
                x = (std::max)(0, (std::min)(x, width - 1));
                y = (std::max)(0, (std::min)(y, height - 1));
                points.push_back(cv::Point(x, y));
            }
            cv::polylines(canvas, points, true, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        }

        // 绘制中心参考点
        cv::circle(canvas, cv::Point(width / 2, height / 2), 3, cv::Scalar(0, 0, 255), -1);
        cv::line(canvas, cv::Point(width / 2 - 8, height / 2), cv::Point(width / 2 + 8, height / 2), cv::Scalar(0, 0, 255), 1);
        cv::line(canvas, cv::Point(width / 2, height / 2 - 8), cv::Point(width / 2, height / 2 + 8), cv::Scalar(0, 0, 255), 1);

        // 添加形状名称标注
        QString shapeName = Algorithm::MarkShapeParams::getShapeName(params.type);
        cv::putText(canvas, shapeName.toStdString(), cv::Point(10, height - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(128, 128, 128), 1);

        // 使用内存池分配预览图像
        auto previewImage = Base::ImageMemoryPool::instance().allocate(
            canvas.cols, canvas.rows, canvas.type());
        if (previewImage) {
            canvas.copyTo(previewImage->mat());
            featureViewer_->setImage(previewImage);
        }

        LOG_INFO(QString("预览Mark轮廓: %1").arg(shapeName));
    }
    catch (const HException& e) {
        LOG_ERROR(QString("预览Mark轮廓失败: %1").arg(e.ErrorMessage().Text()));
        QMessageBox::warning(this, "预览失败", QString("生成轮廓失败:\n%1").arg(e.ErrorMessage().Text()));
    }
#else
    QMessageBox::warning(this, "不支持", "此功能需要Halcon支持");
#endif
}

void ShapeMatchToolDialog::onGenerateMarkContourClicked()
{
#ifdef _WIN32
    if (!tool_) return;

    // 构建形状参数
    Algorithm::MarkShapeParams params;
    params.type = static_cast<Algorithm::MarkShapeType>(markShapeTypeCombo_->currentData().toInt());
    params.size1 = markSize1SpinBox_->value();
    params.size2 = markSize2SpinBox_->value();
    params.size3 = markSize3SpinBox_->value();

    QString shapeName = Algorithm::MarkShapeParams::getShapeName(params.type);
    LOG_INFO(QString("生成标准%1轮廓: size1=%2, size2=%3, size3=%4")
        .arg(shapeName).arg(params.size1).arg(params.size2).arg(params.size3));

    try {
        // 生成Mark轮廓
        HXLDCont markContour = Algorithm::ShapeMatchTool::generateMarkContour(params);

        if (markContour.CountObj() == 0) {
            QMessageBox::warning(this, "生成失败", "无法生成轮廓");
            return;
        }

        // 检查是否有高噪声样本图像和ROI区域
        Base::ImageData::Ptr sampleImage = nullptr;
        int sampleRow1 = 0, sampleCol1 = 0, sampleRow2 = 0, sampleCol2 = 0;

        if (currentImage_ && imageViewer_) {
            std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
            if (!rois.empty()) {
                // 有ROI时使用图像作为高噪声样本
                sampleImage = currentImage_;
                QRect rect = rois.front()->boundingRect();
                sampleRow1 = rect.top();
                sampleCol1 = rect.left();
                sampleRow2 = rect.bottom();
                sampleCol2 = rect.right();
                LOG_DEBUG(QString("使用高噪声样本优化: ROI(%1,%2)-(%3,%4)")
                    .arg(sampleRow1).arg(sampleCol1).arg(sampleRow2).arg(sampleCol2));
            }
        }

        // 从轮廓训练模板
        bool success = tool_->trainModelFromContour(markContour, sampleImage,
                                                     sampleRow1, sampleCol1,
                                                     sampleRow2, sampleCol2);

        if (success) {
            LOG_INFO(QString("标准%1轮廓模板训练成功").arg(shapeName));
            currentLibraryModelId_.clear();  // 训练新模板，清除模板库ID
            updateUI();
            updateFeaturePreview();
            // 询问是否保存到模板库
            saveToModelLibrary();
        } else {
            QMessageBox::warning(this, "训练失败", QString("从标准%1轮廓训练模板失败").arg(shapeName));
        }
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成标准%1轮廓失败: %2").arg(shapeName).arg(e.ErrorMessage().Text()));
        QMessageBox::critical(this, "错误",
            QString("生成标准%1轮廓失败:\n%2").arg(shapeName).arg(e.ErrorMessage().Text()));
    }
#else
    QMessageBox::warning(this, "不支持", "此功能需要Halcon支持");
#endif
}

bool ShapeMatchToolDialog::updateModelInLibrary()
{
    if (!tool_ || !tool_->hasModel()) {
        return false;
    }

    if (currentLibraryModelId_.isEmpty()) {
        return false;
    }

    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    auto info = manager.getModel(currentLibraryModelId_);
    if (!info) {
        LOG_WARNING(QString("无法找到模板库中的模板: %1").arg(currentLibraryModelId_));
        currentLibraryModelId_.clear();
        return false;
    }

    // 更新模板参数
    info->angleStart = tool_->getAngleStart();
    info->angleExtent = tool_->getAngleExtent();
    info->minContrast = tool_->getMinContrast();
    info->modifyTime = QDateTime::currentDateTime();

    // 更新匹配参数
    info->minScore = minScoreSpinBox_->value();
    info->numMatches = numMatchesSpinBox_->value();
    info->maxOverlap = overlapSpinBox_->value();
    info->subPixel = subPixelCombo_->currentText();
    info->greediness = greedySpinBox_->value();
    info->scaleMin = scaleMinSpinBox_->value();
    info->scaleMax = scaleMaxSpinBox_->value();
    info->matchType = matchTypeCombo_->currentIndex();

    // 保存模板文件（覆盖原文件）
    if (!info->filePath.isEmpty() && QFile::exists(info->filePath)) {
        if (!tool_->saveModel(info->filePath)) {
            LOG_ERROR(QString("更新模板文件失败: %1").arg(info->filePath));
            QMessageBox::warning(this, "更新失败",
                QString("无法更新模板文件:\n%1").arg(info->filePath));
            return false;
        }
        LOG_INFO(QString("模板文件已更新: %1").arg(info->filePath));
    }

    // 更新缩略图
    if (featureViewer_ && featureViewer_->getImage()) {
        cv::Mat mat = featureViewer_->getImage()->mat();
        if (!mat.empty()) {
            cv::Mat rgb;
            if (mat.channels() == 1) {
                cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
            } else if (mat.channels() == 3) {
                cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
            } else {
                rgb = mat.clone();
            }
            QImage thumbnail = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();

            // 保存缩略图
            if (!info->thumbnailPath.isEmpty()) {
                thumbnail.save(info->thumbnailPath);
            }
        }
    }

    // 更新模板库
    if (manager.updateModel(info)) {
        manager.saveLibrary();
        LOG_INFO(QString("模板库已更新: %1 (ID: %2)").arg(info->name).arg(currentLibraryModelId_));
        return true;
    }

    return false;
}

bool ShapeMatchToolDialog::saveToModelLibrary()
{
    if (!tool_ || !tool_->hasModel()) {
        return false;
    }

    // 确保模板库路径存在并初始化
    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    QString libraryPath = manager.getLibraryPath();
    if (libraryPath.isEmpty()) {
        // 使用默认路径
        libraryPath = QCoreApplication::applicationDirPath() + "/model_library";
    }

    // 创建目录
    QDir dir(libraryPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::warning(this, "错误",
                QString("无法创建模板库目录:\n%1").arg(libraryPath));
            return false;
        }
    }

    // 加载/初始化模板库（确保库已初始化）
    if (!manager.loadLibrary(libraryPath)) {
        QMessageBox::warning(this, "错误",
            QString("无法初始化模板库:\n%1").arg(libraryPath));
        return false;
    }
    LOG_DEBUG(QString("模板库已初始化: %1").arg(libraryPath));

    // 弹出模板库对话框，让用户选择覆盖现有模板或创建新模板
    ShapeModelLibraryDialog dialog(this);

    // 自定义对话框，添加"新建"按钮
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("保存到模板库");
    msgBox.setText("请选择保存方式：");
    msgBox.setInformativeText("选择\"覆盖现有\"可以在模板库中选择一个模板进行覆盖。\n选择\"新建模板\"创建一个新的模板。");
    QPushButton* overwriteBtn = msgBox.addButton("覆盖现有", QMessageBox::ActionRole);
    QPushButton* newBtn = msgBox.addButton("新建模板", QMessageBox::ActionRole);
    QPushButton* cancelBtn = msgBox.addButton("取消", QMessageBox::RejectRole);
    msgBox.setDefaultButton(newBtn);
    msgBox.exec();

    if (msgBox.clickedButton() == cancelBtn) {
        return false;
    }

    bool overwriteExisting = (msgBox.clickedButton() == overwriteBtn);

    // 生成缩略图（从特征预览获取）
    QImage thumbnail;
    if (featureViewer_ && featureViewer_->getImage()) {
        cv::Mat mat = featureViewer_->getImage()->mat();
        if (!mat.empty()) {
            cv::Mat rgb;
            if (mat.channels() == 1) {
                cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
            } else if (mat.channels() == 3) {
                cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
            } else {
                rgb = mat.clone();
            }
            thumbnail = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
        }
    }

    if (overwriteExisting) {
        // 打开模板库对话框让用户选择要覆盖的模板
        if (dialog.exec() == QDialog::Accepted) {
            QString selectedId = dialog.getSelectedModelId();
            if (selectedId.isEmpty()) {
                QMessageBox::warning(this, "错误", "请选择要覆盖的模板");
                return false;
            }

            auto existingInfo = manager.getModel(selectedId);
            if (!existingInfo) {
                QMessageBox::warning(this, "错误", "无法找到选中的模板");
                return false;
            }

            // 确认覆盖
            QMessageBox::StandardButton confirm = QMessageBox::question(
                this, "确认覆盖",
                QString("确定要覆盖模板\"%1\"吗？\n此操作不可撤销。").arg(existingInfo->name),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);

            if (confirm != QMessageBox::Yes) {
                return false;
            }

            // 更新模板信息
            existingInfo->modifyTime = QDateTime::currentDateTime();
            existingInfo->angleStart = tool_->getAngleStart();
            existingInfo->angleExtent = tool_->getAngleExtent();
            existingInfo->minContrast = tool_->getMinContrast();

            // 更新匹配参数
            existingInfo->minScore = minScoreSpinBox_->value();
            existingInfo->numMatches = numMatchesSpinBox_->value();
            existingInfo->maxOverlap = overlapSpinBox_->value();
            existingInfo->subPixel = subPixelCombo_->currentText();
            existingInfo->greediness = greedySpinBox_->value();
            existingInfo->scaleMin = scaleMinSpinBox_->value();
            existingInfo->scaleMax = scaleMaxSpinBox_->value();
            existingInfo->matchType = matchTypeCombo_->currentIndex();

            // 覆盖模板文件
            if (!existingInfo->filePath.isEmpty()) {
                if (!tool_->saveModel(existingInfo->filePath)) {
                    QMessageBox::warning(this, "覆盖失败",
                        QString("无法覆盖模板文件:\n%1").arg(existingInfo->filePath));
                    return false;
                }
                LOG_INFO(QString("模板文件已覆盖: %1").arg(existingInfo->filePath));
            }

            // 更新缩略图
            if (!thumbnail.isNull() && !existingInfo->thumbnailPath.isEmpty()) {
                thumbnail.save(existingInfo->thumbnailPath);
            }

            // 更新模板库
            if (manager.updateModel(existingInfo)) {
                manager.saveLibrary();
                // 设置当前模板库ID
                currentLibraryModelId_ = selectedId;
                QMessageBox::information(this, "成功",
                    QString("模板\"%1\"已覆盖更新！").arg(existingInfo->name));
                return true;
            } else {
                QMessageBox::warning(this, "覆盖失败", "无法更新模板库");
                return false;
            }
        }
        return false;
    }

    // 创建新模板
    bool ok;
    QString modelName = QInputDialog::getText(
        this, "模板名称",
        "请输入模板名称:",
        QLineEdit::Normal,
        QString("自由绘制模板_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")),
        &ok);

    if (!ok || modelName.isEmpty()) {
        return false;
    }

    // 获取模板描述
    QString description = QInputDialog::getText(
        this, "模板描述",
        "请输入模板描述（可选）:",
        QLineEdit::Normal,
        QString(),
        &ok);

    // 生成临时文件路径（使用UUID避免中文路径问题）
    QString tempDir = QDir::tempPath();
    QString tempFileName = QString("vf_model_%1.shm").arg(
        QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz"));
    QString tempModelPath = tempDir + "/" + tempFileName;
    LOG_DEBUG(QString("保存模板到临时路径: %1").arg(tempModelPath));

    // 保存模板到文件
    if (!tool_->saveModel(tempModelPath)) {
        QMessageBox::warning(this, "保存失败",
            QString("无法保存模板文件到:\n%1\n\n请检查日志了解详细错误信息。").arg(tempModelPath));
        return false;
    }
    LOG_INFO(QString("模板文件已保存: %1").arg(tempModelPath));

    // 创建模板信息
    auto info = std::make_shared<Algorithm::ShapeModelInfo>();
    info->name = modelName;
    info->description = description;
    info->createTime = QDateTime::currentDateTime();
    info->modifyTime = info->createTime;
    info->angleStart = tool_->getAngleStart();
    info->angleExtent = tool_->getAngleExtent();
    info->minContrast = tool_->getMinContrast();
    info->numLevels = 0;
    info->usageCount = 0;

    // 保存匹配参数
    info->minScore = minScoreSpinBox_->value();
    info->numMatches = numMatchesSpinBox_->value();
    info->maxOverlap = overlapSpinBox_->value();
    info->subPixel = subPixelCombo_->currentText();
    info->greediness = greedySpinBox_->value();
    info->scaleMin = scaleMinSpinBox_->value();
    info->scaleMax = scaleMaxSpinBox_->value();
    info->matchType = matchTypeCombo_->currentIndex();

    // 添加到模板库
    if (manager.addModel(info, tempModelPath, thumbnail)) {
        manager.saveLibrary();
        // 删除临时文件
        QFile::remove(tempModelPath);
        // 设置当前模板库ID
        currentLibraryModelId_ = info->id;
        QMessageBox::information(this, "成功",
            QString("模板已保存到模板库！\n名称: %1").arg(modelName));
        return true;
    } else {
        // 删除临时文件
        QFile::remove(tempModelPath);
        QMessageBox::warning(this, "添加失败", "无法添加模板到模板库");
        return false;
    }
}

} // namespace UI
} // namespace VisionForge
