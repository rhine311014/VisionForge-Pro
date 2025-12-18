/**
 * @file BlurToolDialog.cpp
 * @brief 图像模糊工具参数设置对话框实现
 */

#include "ui/BlurToolDialog.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

BlurToolDialog::BlurToolDialog(Algorithm::BlurTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , previewImage_(nullptr)
    , blurTypeCombo_(nullptr)
    , kernelSizeSpin_(nullptr)
    , gaussianGroup_(nullptr)
    , sigmaSpin_(nullptr)
    , bilateralGroup_(nullptr)
    , sigmaColorSpin_(nullptr)
    , sigmaSpaceSpin_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("图像模糊参数设置");
    setMinimumSize(400, 350);
    resize(450, 400);

    createUI();
    connectSignals();
    loadParameters();
    updateUI();
}

BlurToolDialog::~BlurToolDialog()
{
}

void BlurToolDialog::setPreviewImage(const Base::ImageData::Ptr& image)
{
    previewImage_ = image;
}

void BlurToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createParamsGroup(mainLayout);
    createGaussianParamsGroup(mainLayout);
    createBilateralParamsGroup(mainLayout);
    createButtons(mainLayout);

    mainLayout->addStretch();
}

void BlurToolDialog::createParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("基本参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 模糊类型
    blurTypeCombo_ = new QComboBox(group);
    blurTypeCombo_->addItem("均值模糊", static_cast<int>(Algorithm::BlurTool::Mean));
    blurTypeCombo_->addItem("高斯模糊", static_cast<int>(Algorithm::BlurTool::Gaussian));
    blurTypeCombo_->addItem("中值模糊", static_cast<int>(Algorithm::BlurTool::Median));
    blurTypeCombo_->addItem("双边滤波", static_cast<int>(Algorithm::BlurTool::Bilateral));
    formLayout->addRow("模糊类型:", blurTypeCombo_);

    // 核大小
    kernelSizeSpin_ = new QSpinBox(group);
    kernelSizeSpin_->setRange(1, 99);
    kernelSizeSpin_->setSingleStep(2);  // 奇数步进
    kernelSizeSpin_->setValue(3);
    kernelSizeSpin_->setSuffix(" 像素");
    kernelSizeSpin_->setToolTip("核大小必须为奇数");
    formLayout->addRow("核大小:", kernelSizeSpin_);

    layout->addWidget(group);
}

void BlurToolDialog::createGaussianParamsGroup(QVBoxLayout* layout)
{
    gaussianGroup_ = new QGroupBox("高斯参数", this);
    QFormLayout* formLayout = new QFormLayout(gaussianGroup_);

    // Sigma值
    sigmaSpin_ = new QDoubleSpinBox(gaussianGroup_);
    sigmaSpin_->setRange(0.0, 100.0);
    sigmaSpin_->setSingleStep(0.1);
    sigmaSpin_->setValue(0.0);
    sigmaSpin_->setDecimals(2);
    sigmaSpin_->setToolTip("0 表示根据核大小自动计算");
    formLayout->addRow("Sigma:", sigmaSpin_);

    layout->addWidget(gaussianGroup_);
}

void BlurToolDialog::createBilateralParamsGroup(QVBoxLayout* layout)
{
    bilateralGroup_ = new QGroupBox("双边滤波参数", this);
    QFormLayout* formLayout = new QFormLayout(bilateralGroup_);

    // 颜色空间sigma
    sigmaColorSpin_ = new QDoubleSpinBox(bilateralGroup_);
    sigmaColorSpin_->setRange(0.0, 500.0);
    sigmaColorSpin_->setSingleStep(10.0);
    sigmaColorSpin_->setValue(75.0);
    sigmaColorSpin_->setDecimals(1);
    sigmaColorSpin_->setToolTip("颜色空间滤波器的sigma值");
    formLayout->addRow("颜色Sigma:", sigmaColorSpin_);

    // 空间sigma
    sigmaSpaceSpin_ = new QDoubleSpinBox(bilateralGroup_);
    sigmaSpaceSpin_->setRange(0.0, 500.0);
    sigmaSpaceSpin_->setSingleStep(10.0);
    sigmaSpaceSpin_->setValue(75.0);
    sigmaSpaceSpin_->setDecimals(1);
    sigmaSpaceSpin_->setToolTip("坐标空间滤波器的sigma值");
    formLayout->addRow("空间Sigma:", sigmaSpaceSpin_);

    layout->addWidget(bilateralGroup_);
}

void BlurToolDialog::createButtons(QVBoxLayout* layout)
{
    // 预览按钮
    QHBoxLayout* previewLayout = new QHBoxLayout();
    previewBtn_ = new QPushButton("预览效果", this);
    previewBtn_->setMinimumHeight(35);
    previewLayout->addWidget(previewBtn_);
    layout->addLayout(previewLayout);

    // 对话框按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    okBtn_ = new QPushButton("确定", this);
    cancelBtn_ = new QPushButton("取消", this);
    applyBtn_ = new QPushButton("应用", this);

    btnLayout->addWidget(okBtn_);
    btnLayout->addWidget(cancelBtn_);
    btnLayout->addWidget(applyBtn_);

    layout->addLayout(btnLayout);
}

void BlurToolDialog::connectSignals()
{
    // 参数变更
    connect(blurTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BlurToolDialog::onBlurTypeChanged);
    connect(kernelSizeSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &BlurToolDialog::onKernelSizeChanged);
    connect(sigmaSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BlurToolDialog::onSigmaChanged);
    connect(sigmaColorSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BlurToolDialog::onSigmaColorChanged);
    connect(sigmaSpaceSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BlurToolDialog::onSigmaSpaceChanged);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &BlurToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &BlurToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &BlurToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &BlurToolDialog::onApplyClicked);
}

void BlurToolDialog::updateUI()
{
    int blurType = blurTypeCombo_->currentData().toInt();

    // 高斯参数仅在高斯模糊模式下可用
    bool isGaussian = (blurType == static_cast<int>(Algorithm::BlurTool::Gaussian));
    gaussianGroup_->setVisible(isGaussian);

    // 双边滤波参数仅在双边滤波模式下可用
    bool isBilateral = (blurType == static_cast<int>(Algorithm::BlurTool::Bilateral));
    bilateralGroup_->setVisible(isBilateral);

    // 调整窗口大小
    adjustSize();
}

void BlurToolDialog::loadParameters()
{
    if (!tool_) return;

    // 加载模糊类型
    int typeIndex = blurTypeCombo_->findData(static_cast<int>(tool_->blurType()));
    if (typeIndex >= 0) {
        blurTypeCombo_->setCurrentIndex(typeIndex);
    }

    // 加载核大小
    kernelSizeSpin_->setValue(tool_->kernelSize());

    // 加载sigma
    sigmaSpin_->setValue(tool_->sigma());

    // 加载双边滤波参数
    sigmaColorSpin_->setValue(tool_->sigmaColor());
    sigmaSpaceSpin_->setValue(tool_->sigmaSpace());
}

void BlurToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setBlurType(
        static_cast<Algorithm::BlurTool::BlurType>(blurTypeCombo_->currentData().toInt()));

    // 确保核大小为奇数
    int kernelSize = kernelSizeSpin_->value();
    if (kernelSize % 2 == 0) {
        kernelSize += 1;
        kernelSizeSpin_->setValue(kernelSize);
    }
    tool_->setKernelSize(kernelSize);

    tool_->setSigma(sigmaSpin_->value());
    tool_->setSigmaColor(sigmaColorSpin_->value());
    tool_->setSigmaSpace(sigmaSpaceSpin_->value());

    LOG_INFO("图像模糊参数已更新");
}

void BlurToolDialog::onBlurTypeChanged(int index)
{
    Q_UNUSED(index);
    updateUI();
    applyParameters();
}

void BlurToolDialog::onKernelSizeChanged(int value)
{
    // 确保核大小为奇数
    if (value % 2 == 0) {
        kernelSizeSpin_->setValue(value + 1);
        return;
    }
    applyParameters();
}

void BlurToolDialog::onSigmaChanged(double value)
{
    Q_UNUSED(value);
    applyParameters();
}

void BlurToolDialog::onSigmaColorChanged(double value)
{
    Q_UNUSED(value);
    applyParameters();
}

void BlurToolDialog::onSigmaSpaceChanged(double value)
{
    Q_UNUSED(value);
    applyParameters();
}

void BlurToolDialog::onPreviewClicked()
{
    applyParameters();
    emit previewRequested();
}

void BlurToolDialog::onOkClicked()
{
    applyParameters();
    emit parametersApplied();
    accept();
}

void BlurToolDialog::onCancelClicked()
{
    reject();
}

void BlurToolDialog::onApplyClicked()
{
    applyParameters();
    emit parametersApplied();
}

} // namespace UI
} // namespace VisionForge
