/**
 * @file ShapeMatchToolDialog.cpp
 * @brief 形状匹配工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-15
 */

#include "ui/ShapeMatchToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "ui/ROI.h"
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/ShapeModelManager.h"
#include "ui/ShapeModelLibraryDialog.h"
#include "base/Logger.h"

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
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace UI {

ShapeMatchToolDialog::ShapeMatchToolDialog(Algorithm::ShapeMatchTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , toolTypeCombo_(nullptr)
    , toolTypePreview_(nullptr)
    , polarCoordCheckBox_(nullptr)
    , drawROIBtn_(nullptr)
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

    // 坐标显示
    QLabel* coordLabel = new QLabel("(0, 0)", parent);
    coordLabel->setAlignment(Qt::AlignRight);
    coordLabel->setStyleSheet("color: #888;");

    layout->addWidget(titleLabel);
    layout->addWidget(imageViewer_, 1);
    layout->addWidget(coordLabel);
}

void ShapeMatchToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 创建滚动区域内容
    createToolTypeGroup(layout);
    createROIGroup(layout);
    createAngleGroup(layout);
    createMatchParamGroup(layout);
    createModelGroup(layout);

    layout->addStretch();
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
    QGroupBox* group = new QGroupBox("极坐标设置", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 极坐标复选框
    polarCoordCheckBox_ = new QCheckBox("使用极坐标", this);

    // ROI操作按钮
    QHBoxLayout* roiButtonLayout = new QHBoxLayout();
    drawROIBtn_ = new QPushButton("绘制ROI", this);
    clearROIBtn_ = new QPushButton("清除ROI", this);
    roiButtonLayout->addWidget(drawROIBtn_);
    roiButtonLayout->addWidget(clearROIBtn_);

    // ROI状态
    roiStatusLabel_ = new QLabel("ROI状态: 未设置", this);
    roiStatusLabel_->setStyleSheet("color: #888;");

    groupLayout->addWidget(polarCoordCheckBox_);
    groupLayout->addLayout(roiButtonLayout);
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
            Base::ImageData::Ptr image = std::make_shared<Base::ImageData>(mat);
            setImage(image);
            LOG_INFO(QString("已加载模板图像: %1").arg(filePath));
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

    // 获取第一个ROI的边界矩形
    ROIShapePtr roi = rois.front();
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
        QMessageBox::information(this, "成功", "模板训练成功！");
        updateUI();
    } else {
        QMessageBox::warning(this, "训练失败", "模板训练失败，请检查ROI区域是否有效");
    }
}

void ShapeMatchToolDialog::onModelCheckPointClicked()
{
    if (!tool_ || !tool_->hasModel()) {
        QMessageBox::warning(this, "警告", "没有有效的模板");
        return;
    }

    // TODO: 实现模型校验点功能
    QMessageBox::information(this, "提示", "模型校验点功能待实现");
}

void ShapeMatchToolDialog::onLoadModelClicked()
{
    QString filePath = getModelPath(false);
    if (filePath.isEmpty()) return;

    if (tool_ && tool_->loadModel(filePath)) {
        updateUI();
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
        updateUI();
        LOG_INFO("模板已删除");
    }
}

void ShapeMatchToolDialog::onModelLibraryClicked()
{
    ShapeModelLibraryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString selectedPath = dialog.getSelectedModelPath();
        if (!selectedPath.isEmpty() && tool_) {
            if (tool_->loadModel(selectedPath)) {
                updateUI();
                LOG_INFO(QString("从模板库加载模板: %1").arg(selectedPath));
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

} // namespace UI
} // namespace VisionForge
