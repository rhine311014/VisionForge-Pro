/**
 * @file CircleToolDialog.cpp
 * @brief 圆检测工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/CircleToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/CircleTool.h"
#include "base/Logger.h"

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

CircleToolDialog::CircleToolDialog(Algorithm::CircleTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , paramTabWidget_(nullptr)
    , backendCombo_(nullptr)
    , methodCombo_(nullptr)
    , minRadiusSpinBox_(nullptr)
    , maxRadiusSpinBox_(nullptr)
    , maxCountSpinBox_(nullptr)
    , houghParamGroup_(nullptr)
    , houghDpSpinBox_(nullptr)
    , minDistSpinBox_(nullptr)
    , cannyThresholdSpinBox_(nullptr)
    , accumThresholdSpinBox_(nullptr)
    , filterParamGroup_(nullptr)
    , minCircularitySpinBox_(nullptr)
    , minAreaSpinBox_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
{
    setWindowTitle("圆检测设置");
    setMinimumSize(800, 550);
    resize(900, 600);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

CircleToolDialog::~CircleToolDialog()
{
}

void CircleToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void CircleToolDialog::updateUI()
{
    if (!tool_) return;

    // 检查关键控件
    if (!backendCombo_ || !methodCombo_ || !minRadiusSpinBox_) {
        return;
    }

    // 阻塞信号
    backendCombo_->blockSignals(true);
    methodCombo_->blockSignals(true);
    minRadiusSpinBox_->blockSignals(true);
    maxRadiusSpinBox_->blockSignals(true);
    maxCountSpinBox_->blockSignals(true);
    houghDpSpinBox_->blockSignals(true);
    minDistSpinBox_->blockSignals(true);
    cannyThresholdSpinBox_->blockSignals(true);
    accumThresholdSpinBox_->blockSignals(true);
    minCircularitySpinBox_->blockSignals(true);
    minAreaSpinBox_->blockSignals(true);

    // 更新值
    backendCombo_->setCurrentIndex(static_cast<int>(tool_->backend()));
    methodCombo_->setCurrentIndex(static_cast<int>(tool_->method()));
    minRadiusSpinBox_->setValue(tool_->minRadius());
    maxRadiusSpinBox_->setValue(tool_->maxRadius());
    maxCountSpinBox_->setValue(tool_->maxCount());
    houghDpSpinBox_->setValue(tool_->houghDp());
    minDistSpinBox_->setValue(tool_->minDist());
    cannyThresholdSpinBox_->setValue(tool_->cannyThreshold());
    accumThresholdSpinBox_->setValue(tool_->accumThreshold());
    minCircularitySpinBox_->setValue(tool_->minCircularity());
    minAreaSpinBox_->setValue(tool_->minArea());

    // 恢复信号
    backendCombo_->blockSignals(false);
    methodCombo_->blockSignals(false);
    minRadiusSpinBox_->blockSignals(false);
    maxRadiusSpinBox_->blockSignals(false);
    maxCountSpinBox_->blockSignals(false);
    houghDpSpinBox_->blockSignals(false);
    minDistSpinBox_->blockSignals(false);
    cannyThresholdSpinBox_->blockSignals(false);
    accumThresholdSpinBox_->blockSignals(false);
    minCircularitySpinBox_->blockSignals(false);
    minAreaSpinBox_->blockSignals(false);

    updateMethodVisibility();
}

void CircleToolDialog::showEvent(QShowEvent* event)
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

void CircleToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板
    leftPanel_ = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel_);

    // 右侧面板
    QWidget* rightPanel = new QWidget(mainSplitter_);
    createRightPanel(rightPanel);

    mainSplitter_->addWidget(leftPanel_);
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

void CircleToolDialog::createLeftPanel(QWidget* parent)
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

void CircleToolDialog::createRightPanel(QWidget* parent)
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

    // Tab 3: 过滤参数
    QWidget* filterTab = new QWidget(paramTabWidget_);
    QVBoxLayout* filterLayout = new QVBoxLayout(filterTab);
    filterLayout->setContentsMargins(8, 8, 8, 8);
    filterLayout->setSpacing(8);
    createFilterParamGroup(filterLayout);
    filterLayout->addStretch();
    paramTabWidget_->addTab(filterTab, "过滤参数");

    layout->addWidget(paramTabWidget_, 1);
}

void CircleToolDialog::createBasicParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("检测设置", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 后端类型
    groupLayout->addWidget(new QLabel("后端类型:"), row, 0);
    backendCombo_ = new QComboBox(this);
    backendCombo_->addItem("自动选择", static_cast<int>(Algorithm::CircleTool::Auto));
    backendCombo_->addItem("OpenCV", static_cast<int>(Algorithm::CircleTool::OpenCV));
    backendCombo_->addItem("Halcon", static_cast<int>(Algorithm::CircleTool::Halcon));
    groupLayout->addWidget(backendCombo_, row, 1);
    row++;

    // 检测方法
    groupLayout->addWidget(new QLabel("检测方法:"), row, 0);
    methodCombo_ = new QComboBox(this);
    methodCombo_->addItem("霍夫圆检测", static_cast<int>(Algorithm::CircleTool::HoughCircle));
    methodCombo_->addItem("轮廓拟合", static_cast<int>(Algorithm::CircleTool::ContourFit));
    methodCombo_->addItem("边缘拟合 (Halcon)", static_cast<int>(Algorithm::CircleTool::EdgeFit));
    methodCombo_->addItem("Blob拟合 (Halcon)", static_cast<int>(Algorithm::CircleTool::BlobFit));
    groupLayout->addWidget(methodCombo_, row, 1);
    row++;

    // 半径范围
    groupLayout->addWidget(new QLabel("半径范围:"), row, 0);
    QHBoxLayout* radiusLayout = new QHBoxLayout();
    minRadiusSpinBox_ = new QDoubleSpinBox(this);
    minRadiusSpinBox_->setRange(1, 10000);
    minRadiusSpinBox_->setValue(10);
    minRadiusSpinBox_->setSuffix(" px");
    radiusLayout->addWidget(minRadiusSpinBox_);
    radiusLayout->addWidget(new QLabel("~"));
    maxRadiusSpinBox_ = new QDoubleSpinBox(this);
    maxRadiusSpinBox_->setRange(1, 10000);
    maxRadiusSpinBox_->setValue(200);
    maxRadiusSpinBox_->setSuffix(" px");
    radiusLayout->addWidget(maxRadiusSpinBox_);
    groupLayout->addLayout(radiusLayout, row, 1);
    row++;

    // 最大检测数量
    groupLayout->addWidget(new QLabel("最大检测数:"), row, 0);
    maxCountSpinBox_ = new QSpinBox(this);
    maxCountSpinBox_->setRange(1, 100);
    maxCountSpinBox_->setValue(10);
    groupLayout->addWidget(maxCountSpinBox_, row, 1);

    layout->addWidget(group);
}

void CircleToolDialog::createHoughParamGroup(QVBoxLayout* layout)
{
    houghParamGroup_ = new QGroupBox("霍夫圆检测参数", this);
    QGridLayout* groupLayout = new QGridLayout(houghParamGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // dp参数
    groupLayout->addWidget(new QLabel("分辨率比例 (dp):"), row, 0);
    houghDpSpinBox_ = new QDoubleSpinBox(this);
    houghDpSpinBox_->setRange(0.5, 5.0);
    houghDpSpinBox_->setSingleStep(0.1);
    houghDpSpinBox_->setValue(1.0);
    houghDpSpinBox_->setToolTip("累加器分辨率与图像分辨率的比值，1表示相同分辨率");
    groupLayout->addWidget(houghDpSpinBox_, row, 1);
    row++;

    // 最小圆心距离
    groupLayout->addWidget(new QLabel("最小圆心距:"), row, 0);
    minDistSpinBox_ = new QDoubleSpinBox(this);
    minDistSpinBox_->setRange(1, 1000);
    minDistSpinBox_->setValue(50);
    minDistSpinBox_->setSuffix(" px");
    minDistSpinBox_->setToolTip("检测到的圆心之间的最小距离");
    groupLayout->addWidget(minDistSpinBox_, row, 1);
    row++;

    // Canny阈值
    groupLayout->addWidget(new QLabel("Canny高阈值:"), row, 0);
    cannyThresholdSpinBox_ = new QDoubleSpinBox(this);
    cannyThresholdSpinBox_->setRange(1, 500);
    cannyThresholdSpinBox_->setValue(100);
    cannyThresholdSpinBox_->setToolTip("Canny边缘检测的高阈值，低阈值为此值的一半");
    groupLayout->addWidget(cannyThresholdSpinBox_, row, 1);
    row++;

    // 累加器阈值
    groupLayout->addWidget(new QLabel("累加器阈值:"), row, 0);
    accumThresholdSpinBox_ = new QDoubleSpinBox(this);
    accumThresholdSpinBox_->setRange(1, 500);
    accumThresholdSpinBox_->setValue(30);
    accumThresholdSpinBox_->setToolTip("圆心检测的累加器阈值，值越小检测到的圆越多");
    groupLayout->addWidget(accumThresholdSpinBox_, row, 1);

    layout->addWidget(houghParamGroup_);
}

void CircleToolDialog::createFilterParamGroup(QVBoxLayout* layout)
{
    filterParamGroup_ = new QGroupBox("圆度/面积过滤", this);
    QGridLayout* groupLayout = new QGridLayout(filterParamGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 最小圆度
    groupLayout->addWidget(new QLabel("最小圆度:"), row, 0);
    minCircularitySpinBox_ = new QDoubleSpinBox(this);
    minCircularitySpinBox_->setRange(0.0, 1.0);
    minCircularitySpinBox_->setSingleStep(0.05);
    minCircularitySpinBox_->setValue(0.5);
    minCircularitySpinBox_->setToolTip("圆度范围0-1，1表示完美圆形");
    groupLayout->addWidget(minCircularitySpinBox_, row, 1);
    row++;

    // 最小面积
    groupLayout->addWidget(new QLabel("最小面积:"), row, 0);
    minAreaSpinBox_ = new QDoubleSpinBox(this);
    minAreaSpinBox_->setRange(0, 1000000);
    minAreaSpinBox_->setValue(100);
    minAreaSpinBox_->setSuffix(" px2");
    minAreaSpinBox_->setToolTip("检测圆的最小面积（像素平方）");
    groupLayout->addWidget(minAreaSpinBox_, row, 1);

    layout->addWidget(filterParamGroup_);
}

void CircleToolDialog::connectSignals()
{
    // 基本参数
    connect(backendCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircleToolDialog::onBackendChanged);
    connect(methodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircleToolDialog::onMethodChanged);
    connect(minRadiusSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onRadiusRangeChanged);
    connect(maxRadiusSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onRadiusRangeChanged);
    connect(maxCountSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CircleToolDialog::onMaxCountChanged);

    // 霍夫参数
    connect(houghDpSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onHoughDpChanged);
    connect(minDistSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onMinDistChanged);
    connect(cannyThresholdSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onCannyThresholdChanged);
    connect(accumThresholdSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onAccumThresholdChanged);

    // 过滤参数
    connect(minCircularitySpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onMinCircularityChanged);
    connect(minAreaSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CircleToolDialog::onMinAreaChanged);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &CircleToolDialog::onAutoPreview);

    // 对话框按钮
    connect(previewBtn_, &QPushButton::clicked, this, &CircleToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &CircleToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &CircleToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &CircleToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &CircleToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &CircleToolDialog::onCaptureImageClicked);
}

void CircleToolDialog::onBackendChanged(int index)
{
    if (!tool_) return;
    tool_->setBackend(static_cast<Algorithm::CircleTool::BackendType>(
        backendCombo_->itemData(index).toInt()));
    updateMethodVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onMethodChanged(int index)
{
    if (!tool_) return;
    tool_->setMethod(static_cast<Algorithm::CircleTool::DetectionMethod>(
        methodCombo_->itemData(index).toInt()));
    updateMethodVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onRadiusRangeChanged()
{
    if (!tool_) return;
    tool_->setMinRadius(minRadiusSpinBox_->value());
    tool_->setMaxRadius(maxRadiusSpinBox_->value());
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onMaxCountChanged(int value)
{
    if (!tool_) return;
    tool_->setMaxCount(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onHoughDpChanged(double value)
{
    if (!tool_) return;
    tool_->setHoughDp(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onMinDistChanged(double value)
{
    if (!tool_) return;
    tool_->setMinDist(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onCannyThresholdChanged(double value)
{
    if (!tool_) return;
    tool_->setCannyThreshold(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onAccumThresholdChanged(double value)
{
    if (!tool_) return;
    tool_->setAccumThreshold(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onMinCircularityChanged(double value)
{
    if (!tool_) return;
    tool_->setMinCircularity(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onMinAreaChanged(double value)
{
    if (!tool_) return;
    tool_->setMinArea(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CircleToolDialog::onPreviewClicked()
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
        LOG_INFO(QString("圆检测完成，找到 %1 个圆").arg(tool_->circleCount()));
    } else {
        QMessageBox::warning(this, "检测失败", result.errorMessage);
    }
}

void CircleToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void CircleToolDialog::onCancelClicked()
{
    reject();
}

void CircleToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void CircleToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setBackend(static_cast<Algorithm::CircleTool::BackendType>(
        backendCombo_->currentData().toInt()));
    tool_->setMethod(static_cast<Algorithm::CircleTool::DetectionMethod>(
        methodCombo_->currentData().toInt()));
    tool_->setMinRadius(minRadiusSpinBox_->value());
    tool_->setMaxRadius(maxRadiusSpinBox_->value());
    tool_->setMaxCount(maxCountSpinBox_->value());
    tool_->setHoughDp(houghDpSpinBox_->value());
    tool_->setMinDist(minDistSpinBox_->value());
    tool_->setCannyThreshold(cannyThresholdSpinBox_->value());
    tool_->setAccumThreshold(accumThresholdSpinBox_->value());
    tool_->setMinCircularity(minCircularitySpinBox_->value());
    tool_->setMinArea(minAreaSpinBox_->value());

    LOG_INFO("圆检测参数已应用");
}

void CircleToolDialog::updateMethodVisibility()
{
    // 根据检测方法显示/隐藏相关参数组
    int method = methodCombo_->currentData().toInt();
    bool isHough = (method == static_cast<int>(Algorithm::CircleTool::HoughCircle));

    houghParamGroup_->setEnabled(isHough);
    filterParamGroup_->setEnabled(!isHough);
}

void CircleToolDialog::onAutoPreview()
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

void CircleToolDialog::onLoadImageClicked()
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

    Base::ImageData::Ptr image = Base::ImageData::loadFromFile(filePath);
    if (image) {
        setImage(image);
        LOG_INFO(QString("加载图片成功: %1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("加载失败"), tr("无法加载图片文件: %1").arg(filePath));
        LOG_ERROR(QString("加载图片失败: %1").arg(filePath));
    }
}

void CircleToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

void CircleToolDialog::setEmbeddedMode(bool embedded)
{
    embeddedMode_ = embedded;
    if (embedded) {
        if (leftPanel_) leftPanel_->hide();
        if (okBtn_) okBtn_->hide();
        if (cancelBtn_) cancelBtn_->hide();
        if (applyBtn_) applyBtn_->hide();
        if (previewBtn_) previewBtn_->hide();
        if (autoPreviewCheck_) autoPreviewCheck_->hide();
        if (mainSplitter_) mainSplitter_->setSizes({0, 1});
        setMinimumSize(0, 0);
        resize(400, 500);
    } else {
        if (leftPanel_) leftPanel_->show();
        if (okBtn_) okBtn_->show();
        if (cancelBtn_) cancelBtn_->show();
        if (applyBtn_) applyBtn_->show();
        if (previewBtn_) previewBtn_->show();
        if (autoPreviewCheck_) autoPreviewCheck_->show();
        if (mainSplitter_) mainSplitter_->setSizes({600, 400});
    }
}

} // namespace UI
} // namespace VisionForge
