/**
 * @file ThresholdToolDialog.cpp
 * @brief 二值化工具参数设置对话框实现
 */

#include "ui/ThresholdToolDialog.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

ThresholdToolDialog::ThresholdToolDialog(Algorithm::ThresholdTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , previewImage_(nullptr)
    , thresholdTypeCombo_(nullptr)
    , thresholdMethodCombo_(nullptr)
    , manualGroup_(nullptr)
    , thresholdSpin_(nullptr)
    , thresholdSlider_(nullptr)
    , maxValueSpin_(nullptr)
    , computedThresholdLabel_(nullptr)
    , adaptiveGroup_(nullptr)
    , blockSizeSpin_(nullptr)
    , constantCSpin_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("二值化参数设置");
    setMinimumSize(450, 450);
    resize(500, 500);

    createUI();
    connectSignals();
    loadParameters();
    updateUI();
}

ThresholdToolDialog::~ThresholdToolDialog()
{
}

void ThresholdToolDialog::setPreviewImage(const Base::ImageData::Ptr& image)
{
    previewImage_ = image;
}

void ThresholdToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createBasicParamsGroup(mainLayout);
    createManualParamsGroup(mainLayout);
    createAdaptiveParamsGroup(mainLayout);
    createButtons(mainLayout);

    mainLayout->addStretch();
}

void ThresholdToolDialog::createBasicParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("基本参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 阈值类型
    thresholdTypeCombo_ = new QComboBox(group);
    thresholdTypeCombo_->addItem("二值化", static_cast<int>(Algorithm::ThresholdTool::Binary));
    thresholdTypeCombo_->addItem("反二值化", static_cast<int>(Algorithm::ThresholdTool::BinaryInv));
    thresholdTypeCombo_->addItem("截断", static_cast<int>(Algorithm::ThresholdTool::Truncate));
    thresholdTypeCombo_->addItem("归零", static_cast<int>(Algorithm::ThresholdTool::ToZero));
    thresholdTypeCombo_->addItem("反归零", static_cast<int>(Algorithm::ThresholdTool::ToZeroInv));
    formLayout->addRow("阈值类型:", thresholdTypeCombo_);

    // 阈值方法
    thresholdMethodCombo_ = new QComboBox(group);
    thresholdMethodCombo_->addItem("手动设置", static_cast<int>(Algorithm::ThresholdTool::Manual));
    thresholdMethodCombo_->addItem("OTSU自动", static_cast<int>(Algorithm::ThresholdTool::OTSU));
    thresholdMethodCombo_->addItem("Triangle自动", static_cast<int>(Algorithm::ThresholdTool::Triangle));
    thresholdMethodCombo_->addItem("自适应均值", static_cast<int>(Algorithm::ThresholdTool::AdaptiveMean));
    thresholdMethodCombo_->addItem("自适应高斯", static_cast<int>(Algorithm::ThresholdTool::AdaptiveGaussian));
    formLayout->addRow("阈值方法:", thresholdMethodCombo_);

    layout->addWidget(group);
}

void ThresholdToolDialog::createManualParamsGroup(QVBoxLayout* layout)
{
    manualGroup_ = new QGroupBox("阈值参数", this);
    QVBoxLayout* vLayout = new QVBoxLayout(manualGroup_);
    QFormLayout* formLayout = new QFormLayout();

    // 阈值（带滑动条）
    QHBoxLayout* thresholdLayout = new QHBoxLayout();
    thresholdSpin_ = new QSpinBox(manualGroup_);
    thresholdSpin_->setRange(0, 255);
    thresholdSpin_->setValue(128);
    thresholdSpin_->setFixedWidth(80);

    thresholdSlider_ = new QSlider(Qt::Horizontal, manualGroup_);
    thresholdSlider_->setRange(0, 255);
    thresholdSlider_->setValue(128);

    thresholdLayout->addWidget(thresholdSpin_);
    thresholdLayout->addWidget(thresholdSlider_);
    formLayout->addRow("阈值:", thresholdLayout);

    // 最大值
    maxValueSpin_ = new QSpinBox(manualGroup_);
    maxValueSpin_->setRange(0, 255);
    maxValueSpin_->setValue(255);
    formLayout->addRow("最大值:", maxValueSpin_);

    // 计算出的阈值（OTSU/Triangle）
    computedThresholdLabel_ = new QLabel("--", manualGroup_);
    formLayout->addRow("计算阈值:", computedThresholdLabel_);

    vLayout->addLayout(formLayout);

    layout->addWidget(manualGroup_);
}

void ThresholdToolDialog::createAdaptiveParamsGroup(QVBoxLayout* layout)
{
    adaptiveGroup_ = new QGroupBox("自适应参数", this);
    QFormLayout* formLayout = new QFormLayout(adaptiveGroup_);

    // 块大小
    blockSizeSpin_ = new QSpinBox(adaptiveGroup_);
    blockSizeSpin_->setRange(3, 255);
    blockSizeSpin_->setSingleStep(2);  // 奇数步进
    blockSizeSpin_->setValue(11);
    blockSizeSpin_->setToolTip("块大小必须为奇数");
    formLayout->addRow("块大小:", blockSizeSpin_);

    // 常数C
    constantCSpin_ = new QDoubleSpinBox(adaptiveGroup_);
    constantCSpin_->setRange(-50.0, 50.0);
    constantCSpin_->setSingleStep(1.0);
    constantCSpin_->setValue(2.0);
    constantCSpin_->setDecimals(1);
    constantCSpin_->setToolTip("从均值或加权均值中减去的常数");
    formLayout->addRow("常数C:", constantCSpin_);

    layout->addWidget(adaptiveGroup_);
}

void ThresholdToolDialog::createButtons(QVBoxLayout* layout)
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

void ThresholdToolDialog::connectSignals()
{
    // 参数变更
    connect(thresholdTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ThresholdToolDialog::onThresholdTypeChanged);
    connect(thresholdMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ThresholdToolDialog::onThresholdMethodChanged);
    connect(thresholdSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ThresholdToolDialog::onThresholdChanged);
    connect(thresholdSlider_, &QSlider::valueChanged,
            this, &ThresholdToolDialog::onThresholdSliderChanged);
    connect(maxValueSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ThresholdToolDialog::onMaxValueChanged);
    connect(blockSizeSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ThresholdToolDialog::onBlockSizeChanged);
    connect(constantCSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ThresholdToolDialog::onConstantCChanged);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &ThresholdToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &ThresholdToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &ThresholdToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &ThresholdToolDialog::onApplyClicked);
}

void ThresholdToolDialog::updateUI()
{
    int method = thresholdMethodCombo_->currentData().toInt();

    bool isManual = (method == static_cast<int>(Algorithm::ThresholdTool::Manual));
    bool isAuto = (method == static_cast<int>(Algorithm::ThresholdTool::OTSU) ||
                   method == static_cast<int>(Algorithm::ThresholdTool::Triangle));
    bool isAdaptive = (method == static_cast<int>(Algorithm::ThresholdTool::AdaptiveMean) ||
                       method == static_cast<int>(Algorithm::ThresholdTool::AdaptiveGaussian));

    // 手动阈值参数
    thresholdSpin_->setEnabled(isManual);
    thresholdSlider_->setEnabled(isManual);

    // 计算阈值标签（OTSU/Triangle）
    computedThresholdLabel_->setVisible(isAuto);

    // 自适应参数
    adaptiveGroup_->setVisible(isAdaptive);

    // 更新计算出的阈值显示
    if (isAuto && tool_) {
        int computed = tool_->getComputedThreshold();
        if (computed >= 0) {
            computedThresholdLabel_->setText(QString::number(computed));
        }
    }

    // 调整窗口大小
    adjustSize();
}

void ThresholdToolDialog::loadParameters()
{
    if (!tool_) return;

    // 加载阈值类型
    int typeIndex = thresholdTypeCombo_->findData(static_cast<int>(tool_->thresholdType()));
    if (typeIndex >= 0) {
        thresholdTypeCombo_->setCurrentIndex(typeIndex);
    }

    // 加载阈值方法
    int methodIndex = thresholdMethodCombo_->findData(static_cast<int>(tool_->thresholdMethod()));
    if (methodIndex >= 0) {
        thresholdMethodCombo_->setCurrentIndex(methodIndex);
    }

    // 加载阈值
    thresholdSpin_->setValue(tool_->threshold());
    thresholdSlider_->setValue(tool_->threshold());

    // 加载最大值
    maxValueSpin_->setValue(tool_->maxValue());

    // 加载自适应参数
    blockSizeSpin_->setValue(tool_->blockSize());
    constantCSpin_->setValue(tool_->constantC());
}

void ThresholdToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setThresholdType(
        static_cast<Algorithm::ThresholdTool::ThresholdType>(thresholdTypeCombo_->currentData().toInt()));
    tool_->setThresholdMethod(
        static_cast<Algorithm::ThresholdTool::ThresholdMethod>(thresholdMethodCombo_->currentData().toInt()));
    tool_->setThreshold(thresholdSpin_->value());
    tool_->setMaxValue(maxValueSpin_->value());

    // 确保块大小为奇数
    int blockSize = blockSizeSpin_->value();
    if (blockSize % 2 == 0) {
        blockSize += 1;
        blockSizeSpin_->setValue(blockSize);
    }
    tool_->setBlockSize(blockSize);
    tool_->setConstantC(constantCSpin_->value());

    LOG_INFO("二值化参数已更新");
}

void ThresholdToolDialog::onThresholdTypeChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

void ThresholdToolDialog::onThresholdMethodChanged(int index)
{
    Q_UNUSED(index);
    updateUI();
    applyParameters();
}

void ThresholdToolDialog::onThresholdChanged(int value)
{
    // 同步滑动条
    thresholdSlider_->blockSignals(true);
    thresholdSlider_->setValue(value);
    thresholdSlider_->blockSignals(false);
    applyParameters();
}

void ThresholdToolDialog::onThresholdSliderChanged(int value)
{
    // 同步数值框
    thresholdSpin_->blockSignals(true);
    thresholdSpin_->setValue(value);
    thresholdSpin_->blockSignals(false);
    applyParameters();
}

void ThresholdToolDialog::onMaxValueChanged(int value)
{
    Q_UNUSED(value);
    applyParameters();
}

void ThresholdToolDialog::onBlockSizeChanged(int value)
{
    // 确保块大小为奇数
    if (value % 2 == 0) {
        blockSizeSpin_->setValue(value + 1);
        return;
    }
    applyParameters();
}

void ThresholdToolDialog::onConstantCChanged(double value)
{
    Q_UNUSED(value);
    applyParameters();
}

void ThresholdToolDialog::onPreviewClicked()
{
    applyParameters();
    emit previewRequested();

    // 更新计算阈值显示
    if (tool_) {
        int computed = tool_->getComputedThreshold();
        if (computed >= 0) {
            computedThresholdLabel_->setText(QString::number(computed));
        }
    }
}

void ThresholdToolDialog::onOkClicked()
{
    applyParameters();
    emit parametersApplied();
    accept();
}

void ThresholdToolDialog::onCancelClicked()
{
    reject();
}

void ThresholdToolDialog::onApplyClicked()
{
    applyParameters();
    emit parametersApplied();
}

} // namespace UI
} // namespace VisionForge
