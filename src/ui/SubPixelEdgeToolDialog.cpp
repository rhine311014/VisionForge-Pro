/**
 * @file SubPixelEdgeToolDialog.cpp
 * @brief 亚像素边缘检测工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-22
 */

#include "ui/SubPixelEdgeToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/SubPixelEdgeTool.h"
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

SubPixelEdgeToolDialog::SubPixelEdgeToolDialog(Algorithm::SubPixelEdgeTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , paramTabWidget_(nullptr)
    , methodCombo_(nullptr)
    , methodDescLabel_(nullptr)
    , gradientThresholdSpinBox_(nullptr)
    , windowSizeSpinBox_(nullptr)
    , useSubPixelCheck_(nullptr)
    , sigmaSpinBox_(nullptr)
    , cannyLowSpinBox_(nullptr)
    , cannyHighSpinBox_(nullptr)
    , edgePointCountLabel_(nullptr)
    , avgSubPixelShiftLabel_(nullptr)
    , maxGradientLabel_(nullptr)
    , processTimeLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
{
    setWindowTitle("亚像素边缘检测设置");
    setMinimumSize(850, 600);
    resize(950, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

SubPixelEdgeToolDialog::~SubPixelEdgeToolDialog()
{
}

void SubPixelEdgeToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void SubPixelEdgeToolDialog::updateUI()
{
    if (!tool_) return;

    if (!methodCombo_ || !gradientThresholdSpinBox_ || !windowSizeSpinBox_) {
        return;
    }

    // 阻塞信号
    methodCombo_->blockSignals(true);
    gradientThresholdSpinBox_->blockSignals(true);
    windowSizeSpinBox_->blockSignals(true);
    useSubPixelCheck_->blockSignals(true);
    sigmaSpinBox_->blockSignals(true);
    cannyLowSpinBox_->blockSignals(true);
    cannyHighSpinBox_->blockSignals(true);

    // 更新值
    auto config = tool_->getConfig();
    methodCombo_->setCurrentIndex(static_cast<int>(config.method));
    gradientThresholdSpinBox_->setValue(config.gradientThreshold);
    windowSizeSpinBox_->setValue(config.windowSize);
    useSubPixelCheck_->setChecked(config.useSubPixel);
    sigmaSpinBox_->setValue(config.sigma);
    cannyLowSpinBox_->setValue(config.cannyLow);
    cannyHighSpinBox_->setValue(config.cannyHigh);

    // 恢复信号
    methodCombo_->blockSignals(false);
    gradientThresholdSpinBox_->blockSignals(false);
    windowSizeSpinBox_->blockSignals(false);
    useSubPixelCheck_->blockSignals(false);
    sigmaSpinBox_->blockSignals(false);
    cannyLowSpinBox_->blockSignals(false);
    cannyHighSpinBox_->blockSignals(false);

    updateMethodDescription();
}

void SubPixelEdgeToolDialog::showEvent(QShowEvent* event)
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

void SubPixelEdgeToolDialog::createUI()
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

void SubPixelEdgeToolDialog::createLeftPanel(QWidget* parent)
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
    imageViewer_->setMinimumSize(450, 350);
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addLayout(titleLayout);
    layout->addWidget(imageViewer_, 1);
}

void SubPixelEdgeToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 亚像素方法
    QWidget* methodTab = new QWidget(paramTabWidget_);
    QVBoxLayout* methodLayout = new QVBoxLayout(methodTab);
    methodLayout->setContentsMargins(8, 8, 8, 8);
    methodLayout->setSpacing(8);
    createMethodGroup(methodLayout);
    methodLayout->addStretch();
    paramTabWidget_->addTab(methodTab, "检测方法");

    // Tab 2: 边缘参数
    QWidget* edgeTab = new QWidget(paramTabWidget_);
    QVBoxLayout* edgeLayout = new QVBoxLayout(edgeTab);
    edgeLayout->setContentsMargins(8, 8, 8, 8);
    edgeLayout->setSpacing(8);
    createEdgeParamGroup(edgeLayout);
    createFilterParamGroup(edgeLayout);
    edgeLayout->addStretch();
    paramTabWidget_->addTab(edgeTab, "参数设置");

    // Tab 3: 检测结果
    QWidget* resultTab = new QWidget(paramTabWidget_);
    QVBoxLayout* resultLayout = new QVBoxLayout(resultTab);
    resultLayout->setContentsMargins(8, 8, 8, 8);
    resultLayout->setSpacing(8);
    createResultGroup(resultLayout);
    resultLayout->addStretch();
    paramTabWidget_->addTab(resultTab, "检测结果");

    layout->addWidget(paramTabWidget_, 1);
}

void SubPixelEdgeToolDialog::createMethodGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("亚像素检测方法", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QHBoxLayout* methodLayout = new QHBoxLayout();
    methodLayout->addWidget(new QLabel("方法:"));

    methodCombo_ = new QComboBox(this);
    methodCombo_->addItem("Sobel梯度插值", static_cast<int>(Algorithm::SubPixelMethod::SobelInterpolation));
    methodCombo_->addItem("Zernike矩", static_cast<int>(Algorithm::SubPixelMethod::ZernikeMoment));
    methodCombo_->addItem("二次曲线拟合", static_cast<int>(Algorithm::SubPixelMethod::QuadraticFit));
    methodCombo_->addItem("一维高斯拟合", static_cast<int>(Algorithm::SubPixelMethod::Gaussian1D));
    methodCombo_->addItem("ESR拟合", static_cast<int>(Algorithm::SubPixelMethod::ESRFit));
    methodLayout->addWidget(methodCombo_, 1);

    groupLayout->addLayout(methodLayout);

    // 方法说明
    methodDescLabel_ = new QLabel(this);
    methodDescLabel_->setWordWrap(true);
    methodDescLabel_->setStyleSheet("color: #666; font-size: 11px; padding: 8px; "
                                     "background-color: #f5f5f5; border-radius: 4px;");
    groupLayout->addWidget(methodDescLabel_);

    layout->addWidget(group);

    // 亚像素开关
    QGroupBox* optionGroup = new QGroupBox("选项", this);
    QVBoxLayout* optionLayout = new QVBoxLayout(optionGroup);

    useSubPixelCheck_ = new QCheckBox("启用亚像素精化", this);
    useSubPixelCheck_->setChecked(true);
    useSubPixelCheck_->setToolTip("关闭后仅返回整像素边缘位置");
    optionLayout->addWidget(useSubPixelCheck_);

    layout->addWidget(optionGroup);
}

void SubPixelEdgeToolDialog::createEdgeParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("边缘检测参数", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 梯度阈值
    groupLayout->addWidget(new QLabel("梯度阈值:"), row, 0);
    gradientThresholdSpinBox_ = new QDoubleSpinBox(this);
    gradientThresholdSpinBox_->setRange(1.0, 255.0);
    gradientThresholdSpinBox_->setValue(20.0);
    gradientThresholdSpinBox_->setSingleStep(1.0);
    gradientThresholdSpinBox_->setToolTip("边缘梯度的最小强度阈值，值越大检测的边缘越少但越可靠");
    groupLayout->addWidget(gradientThresholdSpinBox_, row, 1);
    row++;

    // 窗口大小
    groupLayout->addWidget(new QLabel("窗口大小:"), row, 0);
    windowSizeSpinBox_ = new QSpinBox(this);
    windowSizeSpinBox_->setRange(3, 15);
    windowSizeSpinBox_->setSingleStep(2);
    windowSizeSpinBox_->setValue(5);
    windowSizeSpinBox_->setSuffix(" px");
    windowSizeSpinBox_->setToolTip("亚像素精化的采样窗口大小（奇数）");
    groupLayout->addWidget(windowSizeSpinBox_, row, 1);

    layout->addWidget(group);
}

void SubPixelEdgeToolDialog::createFilterParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("滤波参数", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 高斯Sigma
    groupLayout->addWidget(new QLabel("高斯Sigma:"), row, 0);
    sigmaSpinBox_ = new QDoubleSpinBox(this);
    sigmaSpinBox_->setRange(0.1, 5.0);
    sigmaSpinBox_->setSingleStep(0.1);
    sigmaSpinBox_->setValue(1.0);
    sigmaSpinBox_->setToolTip("高斯平滑的sigma值，较大值可减少噪声但会降低定位精度");
    groupLayout->addWidget(sigmaSpinBox_, row, 1);
    row++;

    // Canny低阈值
    groupLayout->addWidget(new QLabel("Canny低阈值:"), row, 0);
    cannyLowSpinBox_ = new QSpinBox(this);
    cannyLowSpinBox_->setRange(1, 255);
    cannyLowSpinBox_->setValue(50);
    cannyLowSpinBox_->setToolTip("Canny边缘检测的低阈值（可选预处理）");
    groupLayout->addWidget(cannyLowSpinBox_, row, 1);
    row++;

    // Canny高阈值
    groupLayout->addWidget(new QLabel("Canny高阈值:"), row, 0);
    cannyHighSpinBox_ = new QSpinBox(this);
    cannyHighSpinBox_->setRange(1, 255);
    cannyHighSpinBox_->setValue(150);
    cannyHighSpinBox_->setToolTip("Canny边缘检测的高阈值（可选预处理）");
    groupLayout->addWidget(cannyHighSpinBox_, row, 1);

    layout->addWidget(group);

    // 添加说明
    QLabel* noteLabel = new QLabel(
        "提示:\n"
        "- 梯度阈值控制边缘检测灵敏度\n"
        "- 窗口大小影响亚像素精化范围\n"
        "- Sigma值越大噪声越少但精度降低", this);
    noteLabel->setStyleSheet("color: #888; font-size: 11px;");
    noteLabel->setWordWrap(true);
    layout->addWidget(noteLabel);
}

void SubPixelEdgeToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("检测结果统计", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(8);

    int row = 0;

    // 边缘点数量
    groupLayout->addWidget(new QLabel("边缘点数量:"), row, 0);
    edgePointCountLabel_ = new QLabel("--", this);
    edgePointCountLabel_->setStyleSheet("font-weight: bold; color: #2196F3;");
    groupLayout->addWidget(edgePointCountLabel_, row, 1);
    row++;

    // 平均亚像素偏移
    groupLayout->addWidget(new QLabel("平均亚像素偏移:"), row, 0);
    avgSubPixelShiftLabel_ = new QLabel("-- px", this);
    avgSubPixelShiftLabel_->setStyleSheet("font-weight: bold; color: #4CAF50;");
    groupLayout->addWidget(avgSubPixelShiftLabel_, row, 1);
    row++;

    // 最大梯度
    groupLayout->addWidget(new QLabel("最大梯度值:"), row, 0);
    maxGradientLabel_ = new QLabel("--", this);
    groupLayout->addWidget(maxGradientLabel_, row, 1);
    row++;

    // 处理时间
    groupLayout->addWidget(new QLabel("处理时间:"), row, 0);
    processTimeLabel_ = new QLabel("-- ms", this);
    groupLayout->addWidget(processTimeLabel_, row, 1);

    layout->addWidget(group);

    // 精度说明
    QGroupBox* precisionGroup = new QGroupBox("精度说明", this);
    QVBoxLayout* precisionLayout = new QVBoxLayout(precisionGroup);

    QLabel* precisionLabel = new QLabel(
        "各方法理论精度：\n"
        "• Sobel梯度插值: ~0.1像素\n"
        "• Zernike矩: ~0.05像素\n"
        "• 二次曲线拟合: ~0.1像素\n"
        "• 一维高斯拟合: ~0.1像素\n"
        "• ESR拟合: ~0.05像素", this);
    precisionLabel->setStyleSheet("color: #666; font-size: 11px;");
    precisionLayout->addWidget(precisionLabel);

    layout->addWidget(precisionGroup);
}

void SubPixelEdgeToolDialog::connectSignals()
{
    // 方法选择
    connect(methodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SubPixelEdgeToolDialog::onMethodChanged);

    // 参数改变
    connect(gradientThresholdSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SubPixelEdgeToolDialog::onGradientThresholdChanged);
    connect(windowSizeSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SubPixelEdgeToolDialog::onWindowSizeChanged);
    connect(useSubPixelCheck_, &QCheckBox::toggled,
            this, &SubPixelEdgeToolDialog::onUseSubPixelChanged);
    connect(sigmaSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SubPixelEdgeToolDialog::onSigmaChanged);
    connect(cannyLowSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SubPixelEdgeToolDialog::onCannyLowChanged);
    connect(cannyHighSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SubPixelEdgeToolDialog::onCannyHighChanged);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &SubPixelEdgeToolDialog::onAutoPreview);

    // 对话框按钮
    connect(previewBtn_, &QPushButton::clicked, this, &SubPixelEdgeToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &SubPixelEdgeToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &SubPixelEdgeToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &SubPixelEdgeToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &SubPixelEdgeToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &SubPixelEdgeToolDialog::onCaptureImageClicked);
}

void SubPixelEdgeToolDialog::onMethodChanged(int index)
{
    if (!tool_) return;

    auto method = static_cast<Algorithm::SubPixelMethod>(methodCombo_->itemData(index).toInt());
    tool_->setMethod(method);

    updateMethodDescription();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void SubPixelEdgeToolDialog::onGradientThresholdChanged(double value)
{
    if (!tool_) return;
    tool_->setGradientThreshold(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void SubPixelEdgeToolDialog::onWindowSizeChanged(int value)
{
    if (!tool_) return;
    tool_->setWindowSize(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void SubPixelEdgeToolDialog::onUseSubPixelChanged(bool checked)
{
    if (!tool_) return;
    tool_->setUseSubPixel(checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void SubPixelEdgeToolDialog::onSigmaChanged(double value)
{
    if (!tool_) return;
    tool_->setSigma(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void SubPixelEdgeToolDialog::onCannyLowChanged(int value)
{
    if (!tool_) return;

    auto config = tool_->getConfig();
    config.cannyLow = value;
    tool_->setConfig(config);

    emit parameterChanged();
    previewHelper_->requestPreview();
}

void SubPixelEdgeToolDialog::onCannyHighChanged(int value)
{
    if (!tool_) return;

    auto config = tool_->getConfig();
    config.cannyHigh = value;
    tool_->setConfig(config);

    emit parameterChanged();
    previewHelper_->requestPreview();
}

void SubPixelEdgeToolDialog::onPreviewClicked()
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

        // 更新结果显示
        auto lastResult = tool_->lastResult();
        edgePointCountLabel_->setText(QString::number(lastResult.validPointCount));
        avgSubPixelShiftLabel_->setText(QString("%1 px").arg(lastResult.averageSubPixelShift, 0, 'f', 4));
        maxGradientLabel_->setText(QString::number(lastResult.maxGradient, 'f', 2));
        processTimeLabel_->setText(QString("%1 ms").arg(lastResult.processTime, 0, 'f', 2));

        LOG_INFO(QString("亚像素边缘检测完成，找到 %1 个边缘点，平均偏移: %2像素")
                .arg(lastResult.validPointCount)
                .arg(lastResult.averageSubPixelShift, 0, 'f', 4));
    } else {
        QMessageBox::warning(this, "检测失败", result.errorMessage);
    }
}

void SubPixelEdgeToolDialog::onAutoPreview()
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

        // 更新结果显示
        auto lastResult = tool_->lastResult();
        edgePointCountLabel_->setText(QString::number(lastResult.validPointCount));
        avgSubPixelShiftLabel_->setText(QString("%1 px").arg(lastResult.averageSubPixelShift, 0, 'f', 4));
        maxGradientLabel_->setText(QString::number(lastResult.maxGradient, 'f', 2));
        processTimeLabel_->setText(QString("%1 ms").arg(lastResult.processTime, 0, 'f', 2));
    }
}

void SubPixelEdgeToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void SubPixelEdgeToolDialog::onCancelClicked()
{
    reject();
}

void SubPixelEdgeToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void SubPixelEdgeToolDialog::applyParameters()
{
    if (!tool_) return;

    Algorithm::SubPixelEdgeConfig config;
    config.method = static_cast<Algorithm::SubPixelMethod>(methodCombo_->currentData().toInt());
    config.gradientThreshold = gradientThresholdSpinBox_->value();
    config.windowSize = windowSizeSpinBox_->value();
    config.useSubPixel = useSubPixelCheck_->isChecked();
    config.sigma = sigmaSpinBox_->value();
    config.cannyLow = cannyLowSpinBox_->value();
    config.cannyHigh = cannyHighSpinBox_->value();

    tool_->setConfig(config);

    LOG_INFO("亚像素边缘检测参数已应用");
}

void SubPixelEdgeToolDialog::updateMethodDescription()
{
    if (!methodCombo_ || !methodDescLabel_) return;

    int methodIndex = methodCombo_->currentData().toInt();
    QString desc;

    switch (static_cast<Algorithm::SubPixelMethod>(methodIndex)) {
    case Algorithm::SubPixelMethod::SobelInterpolation:
        desc = "Sobel梯度插值\n"
               "基于Sobel算子计算梯度，沿梯度方向进行插值定位。\n"
               "优点：速度快，计算简单\n"
               "精度：约0.1像素";
        break;
    case Algorithm::SubPixelMethod::ZernikeMoment:
        desc = "Zernike矩\n"
               "使用Zernike正交矩进行边缘定位，具有旋转不变性。\n"
               "优点：精度最高，抗噪声能力强\n"
               "精度：约0.05像素";
        break;
    case Algorithm::SubPixelMethod::QuadraticFit:
        desc = "二次曲线拟合\n"
               "在梯度极值点附近拟合抛物线，求极值点位置。\n"
               "优点：速度与精度平衡\n"
               "精度：约0.1像素";
        break;
    case Algorithm::SubPixelMethod::Gaussian1D:
        desc = "一维高斯拟合\n"
               "沿梯度方向拟合一维高斯分布，适合单边缘场景。\n"
               "优点：适合边缘清晰的场景\n"
               "精度：约0.1像素";
        break;
    case Algorithm::SubPixelMethod::ESRFit:
        desc = "ESR拟合 (Error Surface Regression)\n"
               "在邻域内拟合误差曲面，求解极值点。\n"
               "优点：适合复杂边缘，鲁棒性强\n"
               "精度：约0.05像素";
        break;
    }

    methodDescLabel_->setText(desc);
}

void SubPixelEdgeToolDialog::onLoadImageClicked()
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
    }
}

void SubPixelEdgeToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
