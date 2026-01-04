/**
 * @file LineToolDialog.cpp
 * @brief 线检测工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/LineToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/LineTool.h"
#include "base/Logger.h"

#include <opencv2/imgcodecs.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QStyle>
#include <QTimer>

namespace VisionForge {
namespace UI {

LineToolDialog::LineToolDialog(Algorithm::LineTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , paramTabWidget_(nullptr)
    , backendCombo_(nullptr)
    , methodCombo_(nullptr)
    , minLengthSpinBox_(nullptr)
    , maxGapSpinBox_(nullptr)
    , maxCountSpinBox_(nullptr)
    , houghParamGroup_(nullptr)
    , rhoSpinBox_(nullptr)
    , thetaSpinBox_(nullptr)
    , thresholdSpinBox_(nullptr)
    , cannyParamGroup_(nullptr)
    , cannyThreshold1SpinBox_(nullptr)
    , cannyThreshold2SpinBox_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
{
    setWindowTitle("线检测设置");
    setMinimumSize(800, 550);
    resize(900, 600);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

LineToolDialog::~LineToolDialog()
{
}

void LineToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void LineToolDialog::updateUI()
{
    if (!tool_) return;

    if (!backendCombo_ || !methodCombo_ || !minLengthSpinBox_) {
        return;
    }

    // 阻塞信号
    backendCombo_->blockSignals(true);
    methodCombo_->blockSignals(true);
    minLengthSpinBox_->blockSignals(true);
    maxGapSpinBox_->blockSignals(true);
    maxCountSpinBox_->blockSignals(true);
    rhoSpinBox_->blockSignals(true);
    thetaSpinBox_->blockSignals(true);
    thresholdSpinBox_->blockSignals(true);
    cannyThreshold1SpinBox_->blockSignals(true);
    cannyThreshold2SpinBox_->blockSignals(true);

    // 更新值
    backendCombo_->setCurrentIndex(static_cast<int>(tool_->backend()));
    methodCombo_->setCurrentIndex(static_cast<int>(tool_->method()));
    minLengthSpinBox_->setValue(tool_->minLength());
    maxGapSpinBox_->setValue(tool_->maxGap());
    maxCountSpinBox_->setValue(tool_->maxCount());
    rhoSpinBox_->setValue(tool_->rho());
    thetaSpinBox_->setValue(tool_->theta());
    thresholdSpinBox_->setValue(tool_->threshold());
    cannyThreshold1SpinBox_->setValue(tool_->cannyThreshold1());
    cannyThreshold2SpinBox_->setValue(tool_->cannyThreshold2());

    // 恢复信号
    backendCombo_->blockSignals(false);
    methodCombo_->blockSignals(false);
    minLengthSpinBox_->blockSignals(false);
    maxGapSpinBox_->blockSignals(false);
    maxCountSpinBox_->blockSignals(false);
    rhoSpinBox_->blockSignals(false);
    thetaSpinBox_->blockSignals(false);
    thresholdSpinBox_->blockSignals(false);
    cannyThreshold1SpinBox_->blockSignals(false);
    cannyThreshold2SpinBox_->blockSignals(false);

    updateMethodVisibility();
}

void LineToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();

    if (currentImage_ && imageViewer_) {
        QTimer::singleShot(200, this, [this]() {
            if (currentImage_ && imageViewer_) {
                imageViewer_->setImage(currentImage_);
            }
        });
    }
}

void LineToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板
    QWidget* leftPanel = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel);

    // 右侧面板
    QWidget* rightPanel = new QWidget(mainSplitter_);
    createRightPanel(rightPanel);

    mainSplitter_->addWidget(leftPanel);
    mainSplitter_->addWidget(rightPanel);
    mainSplitter_->setStretchFactor(0, 3);
    mainSplitter_->setStretchFactor(1, 2);

    mainLayout->addWidget(mainSplitter_, 1);

    // 创建预览辅助器
    previewHelper_ = new PreviewHelper(this, 200);

    // 预览选项行
    QHBoxLayout* previewLayout = new QHBoxLayout();
    autoPreviewCheck_ = new QCheckBox("实时预览", this);
    autoPreviewCheck_->setChecked(previewHelper_->isAutoPreviewEnabled());
    autoPreviewCheck_->setToolTip("启用后参数修改会自动更新预览");
    previewLayout->addWidget(autoPreviewCheck_);
    previewLayout->addStretch();

    previewBtn_ = new QPushButton("预览", this);
    previewBtn_->setMinimumWidth(80);
    previewLayout->addWidget(previewBtn_);

    mainLayout->addLayout(previewLayout);

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

void LineToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 标题栏布局
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("检测图像", parent);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    // 加载图片按钮
    loadImageBtn_ = new QPushButton(tr("加载图片"), parent);
    loadImageBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    loadImageBtn_->setToolTip(tr("从文件加载图片"));
    loadImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(loadImageBtn_);

    // 采集图像按钮
    captureImageBtn_ = new QPushButton(tr("采集图像"), parent);
    captureImageBtn_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    captureImageBtn_->setToolTip(tr("从相机采集图像"));
    captureImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(captureImageBtn_);

    imageViewer_ = new HalconImageViewer(parent);
    imageViewer_->setMinimumSize(400, 300);
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addLayout(titleLayout);
    layout->addWidget(imageViewer_, 1);
}

void LineToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 基本参数
    QWidget* basicTab = new QWidget(paramTabWidget_);
    QVBoxLayout* basicLayout = new QVBoxLayout(basicTab);
    basicLayout->setContentsMargins(8, 8, 8, 8);
    basicLayout->setSpacing(8);
    createBasicParamGroup(basicLayout);
    basicLayout->addStretch();
    paramTabWidget_->addTab(basicTab, "基本参数");

    // Tab 2: 霍夫参数
    QWidget* houghTab = new QWidget(paramTabWidget_);
    QVBoxLayout* houghLayout = new QVBoxLayout(houghTab);
    houghLayout->setContentsMargins(8, 8, 8, 8);
    houghLayout->setSpacing(8);
    createHoughParamGroup(houghLayout);
    houghLayout->addStretch();
    paramTabWidget_->addTab(houghTab, "霍夫参数");

    // Tab 3: Canny参数
    QWidget* cannyTab = new QWidget(paramTabWidget_);
    QVBoxLayout* cannyLayout = new QVBoxLayout(cannyTab);
    cannyLayout->setContentsMargins(8, 8, 8, 8);
    cannyLayout->setSpacing(8);
    createCannyParamGroup(cannyLayout);
    cannyLayout->addStretch();
    paramTabWidget_->addTab(cannyTab, "边缘检测");

    layout->addWidget(paramTabWidget_, 1);
}

void LineToolDialog::createBasicParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("检测设置", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 后端类型
    groupLayout->addWidget(new QLabel("后端类型:"), row, 0);
    backendCombo_ = new QComboBox(this);
    backendCombo_->addItem("自动选择", static_cast<int>(Algorithm::LineTool::Auto));
    backendCombo_->addItem("OpenCV", static_cast<int>(Algorithm::LineTool::OpenCV));
    backendCombo_->addItem("Halcon", static_cast<int>(Algorithm::LineTool::Halcon));
    groupLayout->addWidget(backendCombo_, row, 1);
    row++;

    // 检测方法
    groupLayout->addWidget(new QLabel("检测方法:"), row, 0);
    methodCombo_ = new QComboBox(this);
    methodCombo_->addItem("概率霍夫变换 (HoughLinesP)", static_cast<int>(Algorithm::LineTool::HoughLinesP));
    methodCombo_->addItem("标准霍夫变换 (HoughLines)", static_cast<int>(Algorithm::LineTool::HoughLines));
    methodCombo_->addItem("轮廓拟合 (ContourFit)", static_cast<int>(Algorithm::LineTool::ContourFit));
    methodCombo_->addItem("边缘拟合 (EdgeFit, Halcon)", static_cast<int>(Algorithm::LineTool::EdgeFit));
    groupLayout->addWidget(methodCombo_, row, 1);
    row++;

    // 最小长度
    groupLayout->addWidget(new QLabel("最小长度:"), row, 0);
    minLengthSpinBox_ = new QDoubleSpinBox(this);
    minLengthSpinBox_->setRange(1, 10000);
    minLengthSpinBox_->setValue(50);
    minLengthSpinBox_->setSuffix(" px");
    minLengthSpinBox_->setToolTip("线段的最小长度");
    groupLayout->addWidget(minLengthSpinBox_, row, 1);
    row++;

    // 最大间隙
    groupLayout->addWidget(new QLabel("最大间隙:"), row, 0);
    maxGapSpinBox_ = new QDoubleSpinBox(this);
    maxGapSpinBox_->setRange(0, 500);
    maxGapSpinBox_->setValue(10);
    maxGapSpinBox_->setSuffix(" px");
    maxGapSpinBox_->setToolTip("同一直线上点之间的最大允许间隙");
    groupLayout->addWidget(maxGapSpinBox_, row, 1);
    row++;

    // 最大检测数量
    groupLayout->addWidget(new QLabel("最大检测数:"), row, 0);
    maxCountSpinBox_ = new QSpinBox(this);
    maxCountSpinBox_->setRange(1, 1000);
    maxCountSpinBox_->setValue(50);
    groupLayout->addWidget(maxCountSpinBox_, row, 1);

    layout->addWidget(group);
}

void LineToolDialog::createHoughParamGroup(QVBoxLayout* layout)
{
    houghParamGroup_ = new QGroupBox("霍夫变换参数", this);
    QGridLayout* groupLayout = new QGridLayout(houghParamGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 距离分辨率
    groupLayout->addWidget(new QLabel("距离分辨率 (rho):"), row, 0);
    rhoSpinBox_ = new QDoubleSpinBox(this);
    rhoSpinBox_->setRange(0.1, 10.0);
    rhoSpinBox_->setSingleStep(0.1);
    rhoSpinBox_->setValue(1.0);
    rhoSpinBox_->setSuffix(" px");
    rhoSpinBox_->setToolTip("累加器的距离分辨率（像素）");
    groupLayout->addWidget(rhoSpinBox_, row, 1);
    row++;

    // 角度分辨率
    groupLayout->addWidget(new QLabel("角度分辨率 (theta):"), row, 0);
    thetaSpinBox_ = new QDoubleSpinBox(this);
    thetaSpinBox_->setRange(0.1, 10.0);
    thetaSpinBox_->setSingleStep(0.1);
    thetaSpinBox_->setValue(1.0);
    thetaSpinBox_->setSuffix("°");
    thetaSpinBox_->setToolTip("累加器的角度分辨率（度）");
    groupLayout->addWidget(thetaSpinBox_, row, 1);
    row++;

    // 累加器阈值
    groupLayout->addWidget(new QLabel("累加器阈值:"), row, 0);
    thresholdSpinBox_ = new QSpinBox(this);
    thresholdSpinBox_->setRange(1, 1000);
    thresholdSpinBox_->setValue(80);
    thresholdSpinBox_->setToolTip("霍夫累加器阈值，值越大检测到的线越少但更可靠");
    groupLayout->addWidget(thresholdSpinBox_, row, 1);

    layout->addWidget(houghParamGroup_);
}

void LineToolDialog::createCannyParamGroup(QVBoxLayout* layout)
{
    cannyParamGroup_ = new QGroupBox("Canny边缘检测参数", this);
    QGridLayout* groupLayout = new QGridLayout(cannyParamGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 低阈值
    groupLayout->addWidget(new QLabel("低阈值:"), row, 0);
    cannyThreshold1SpinBox_ = new QDoubleSpinBox(this);
    cannyThreshold1SpinBox_->setRange(0, 500);
    cannyThreshold1SpinBox_->setValue(50);
    cannyThreshold1SpinBox_->setToolTip("Canny边缘检测的低阈值");
    groupLayout->addWidget(cannyThreshold1SpinBox_, row, 1);
    row++;

    // 高阈值
    groupLayout->addWidget(new QLabel("高阈值:"), row, 0);
    cannyThreshold2SpinBox_ = new QDoubleSpinBox(this);
    cannyThreshold2SpinBox_->setRange(0, 500);
    cannyThreshold2SpinBox_->setValue(150);
    cannyThreshold2SpinBox_->setToolTip("Canny边缘检测的高阈值");
    groupLayout->addWidget(cannyThreshold2SpinBox_, row, 1);

    layout->addWidget(cannyParamGroup_);

    // 添加说明
    QLabel* noteLabel = new QLabel("注意: 高阈值应该大于低阈值，通常比例为2:1或3:1", this);
    noteLabel->setStyleSheet("color: #888; font-size: 11px;");
    noteLabel->setWordWrap(true);
    layout->addWidget(noteLabel);
}

void LineToolDialog::connectSignals()
{
    // 基本参数
    connect(backendCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LineToolDialog::onBackendChanged);
    connect(methodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LineToolDialog::onMethodChanged);
    connect(minLengthSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LineToolDialog::onMinLengthChanged);
    connect(maxGapSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LineToolDialog::onMaxGapChanged);
    connect(maxCountSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &LineToolDialog::onMaxCountChanged);

    // 霍夫参数
    connect(rhoSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LineToolDialog::onRhoChanged);
    connect(thetaSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LineToolDialog::onThetaChanged);
    connect(thresholdSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &LineToolDialog::onThresholdChanged);

    // Canny参数
    connect(cannyThreshold1SpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LineToolDialog::onCannyThreshold1Changed);
    connect(cannyThreshold2SpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LineToolDialog::onCannyThreshold2Changed);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &LineToolDialog::onAutoPreview);

    // 对话框按钮
    connect(previewBtn_, &QPushButton::clicked, this, &LineToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &LineToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &LineToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &LineToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &LineToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &LineToolDialog::onCaptureImageClicked);
}

void LineToolDialog::onBackendChanged(int index)
{
    if (!tool_) return;
    tool_->setBackend(static_cast<Algorithm::LineTool::BackendType>(
        backendCombo_->itemData(index).toInt()));
    updateMethodVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onMethodChanged(int index)
{
    if (!tool_) return;
    tool_->setMethod(static_cast<Algorithm::LineTool::DetectionMethod>(
        methodCombo_->itemData(index).toInt()));
    updateMethodVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onMinLengthChanged(double value)
{
    if (!tool_) return;
    tool_->setMinLength(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onMaxGapChanged(double value)
{
    if (!tool_) return;
    tool_->setMaxGap(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onMaxCountChanged(int value)
{
    if (!tool_) return;
    tool_->setMaxCount(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onRhoChanged(double value)
{
    if (!tool_) return;
    tool_->setRho(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onThetaChanged(double value)
{
    if (!tool_) return;
    tool_->setTheta(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onThresholdChanged(int value)
{
    if (!tool_) return;
    tool_->setThreshold(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onCannyThreshold1Changed(double value)
{
    if (!tool_) return;
    tool_->setCannyThreshold1(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onCannyThreshold2Changed(double value)
{
    if (!tool_) return;
    tool_->setCannyThreshold2(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void LineToolDialog::onPreviewClicked()
{
    if (!tool_ || !currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    applyParameters();

    Algorithm::ToolResult result;
    if (tool_->process(currentImage_, result)) {
        if (result.outputImage) {
            imageViewer_->setImage(result.outputImage);
        }
        LOG_INFO(QString("线检测完成，找到 %1 条线").arg(tool_->lineCount()));
    } else {
        QMessageBox::warning(this, "检测失败", result.errorMessage);
    }
}

void LineToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void LineToolDialog::onCancelClicked()
{
    reject();
}

void LineToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void LineToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setBackend(static_cast<Algorithm::LineTool::BackendType>(
        backendCombo_->currentData().toInt()));
    tool_->setMethod(static_cast<Algorithm::LineTool::DetectionMethod>(
        methodCombo_->currentData().toInt()));
    tool_->setMinLength(minLengthSpinBox_->value());
    tool_->setMaxGap(maxGapSpinBox_->value());
    tool_->setMaxCount(maxCountSpinBox_->value());
    tool_->setRho(rhoSpinBox_->value());
    tool_->setTheta(thetaSpinBox_->value());
    tool_->setThreshold(thresholdSpinBox_->value());
    tool_->setCannyThreshold1(cannyThreshold1SpinBox_->value());
    tool_->setCannyThreshold2(cannyThreshold2SpinBox_->value());

    LOG_INFO("线检测参数已应用");
}

void LineToolDialog::updateMethodVisibility()
{
    // 根据检测方法显示/隐藏相关参数组
    int method = methodCombo_->currentData().toInt();
    bool isHough = (method == static_cast<int>(Algorithm::LineTool::HoughLinesP) ||
                    method == static_cast<int>(Algorithm::LineTool::HoughLines));

    houghParamGroup_->setEnabled(isHough);
}

void LineToolDialog::onAutoPreview()
{
    if (!tool_ || !currentImage_) {
        return;
    }

    applyParameters();

    Algorithm::ToolResult result;
    if (tool_->process(currentImage_, result)) {
        if (result.outputImage && imageViewer_) {
            imageViewer_->setImage(result.outputImage);
        }
    }
}

void LineToolDialog::onLoadImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择图片文件"),
        QString(),
        tr("图片文件 (*.bmp *.png *.jpg *.jpeg *.tiff *.tif);;所有文件 (*.*)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    try {
        cv::Mat mat = cv::imread(filePath.toStdString());
        if (!mat.empty()) {
            Base::ImageData::Ptr image = std::make_shared<Base::ImageData>(mat);
            setImage(image);
            LOG_INFO(QString("加载图片成功: %1").arg(filePath));
        } else {
            QMessageBox::warning(this, tr("加载失败"), tr("无法加载图片文件: %1").arg(filePath));
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, tr("加载失败"), tr("加载图片时发生错误: %1").arg(e.what()));
        LOG_ERROR(QString("加载图片失败: %1").arg(e.what()));
    }
}

void LineToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
