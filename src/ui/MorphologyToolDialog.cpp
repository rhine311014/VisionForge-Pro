/**
 * @file MorphologyToolDialog.cpp
 * @brief 形态学处理工具参数设置对话框实现
 */

#include "ui/MorphologyToolDialog.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

MorphologyToolDialog::MorphologyToolDialog(Algorithm::MorphologyTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , previewImage_(nullptr)
    , morphTypeCombo_(nullptr)
    , iterationsSpin_(nullptr)
    , kernelShapeCombo_(nullptr)
    , kernelWidthSpin_(nullptr)
    , kernelHeightSpin_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("形态学处理参数设置");
    setMinimumSize(400, 350);
    resize(450, 400);

    createUI();
    connectSignals();
    loadParameters();
    updateUI();
}

MorphologyToolDialog::~MorphologyToolDialog()
{
}

void MorphologyToolDialog::setPreviewImage(const Base::ImageData::Ptr& image)
{
    previewImage_ = image;
}

void MorphologyToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createOperationParamsGroup(mainLayout);
    createKernelParamsGroup(mainLayout);
    createButtons(mainLayout);

    mainLayout->addStretch();
}

void MorphologyToolDialog::createOperationParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("操作参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 形态学操作类型
    morphTypeCombo_ = new QComboBox(group);
    morphTypeCombo_->addItem("腐蚀 (Erode)", static_cast<int>(Algorithm::MorphologyTool::Erode));
    morphTypeCombo_->addItem("膨胀 (Dilate)", static_cast<int>(Algorithm::MorphologyTool::Dilate));
    morphTypeCombo_->addItem("开运算 (Open)", static_cast<int>(Algorithm::MorphologyTool::Open));
    morphTypeCombo_->addItem("闭运算 (Close)", static_cast<int>(Algorithm::MorphologyTool::Close));
    morphTypeCombo_->addItem("形态学梯度 (Gradient)", static_cast<int>(Algorithm::MorphologyTool::Gradient));
    morphTypeCombo_->addItem("顶帽变换 (TopHat)", static_cast<int>(Algorithm::MorphologyTool::TopHat));
    morphTypeCombo_->addItem("黑帽变换 (BlackHat)", static_cast<int>(Algorithm::MorphologyTool::BlackHat));
    formLayout->addRow("操作类型:", morphTypeCombo_);

    // 迭代次数
    iterationsSpin_ = new QSpinBox(group);
    iterationsSpin_->setRange(1, 100);
    iterationsSpin_->setValue(1);
    iterationsSpin_->setToolTip("操作重复执行的次数");
    formLayout->addRow("迭代次数:", iterationsSpin_);

    layout->addWidget(group);
}

void MorphologyToolDialog::createKernelParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("结构元素参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 结构元素形状
    kernelShapeCombo_ = new QComboBox(group);
    kernelShapeCombo_->addItem("矩形", static_cast<int>(Algorithm::MorphologyTool::Rect));
    kernelShapeCombo_->addItem("十字形", static_cast<int>(Algorithm::MorphologyTool::Cross));
    kernelShapeCombo_->addItem("椭圆形", static_cast<int>(Algorithm::MorphologyTool::Ellipse));
    formLayout->addRow("形状:", kernelShapeCombo_);

    // 结构元素尺寸
    QHBoxLayout* sizeLayout = new QHBoxLayout();

    kernelWidthSpin_ = new QSpinBox(group);
    kernelWidthSpin_->setRange(1, 99);
    kernelWidthSpin_->setSingleStep(2);  // 奇数步进
    kernelWidthSpin_->setValue(3);
    kernelWidthSpin_->setSuffix(" 像素");
    kernelWidthSpin_->setToolTip("结构元素宽度（建议奇数）");

    kernelHeightSpin_ = new QSpinBox(group);
    kernelHeightSpin_->setRange(1, 99);
    kernelHeightSpin_->setSingleStep(2);  // 奇数步进
    kernelHeightSpin_->setValue(3);
    kernelHeightSpin_->setSuffix(" 像素");
    kernelHeightSpin_->setToolTip("结构元素高度（建议奇数）");

    sizeLayout->addWidget(kernelWidthSpin_);
    sizeLayout->addWidget(new QLabel("x"));
    sizeLayout->addWidget(kernelHeightSpin_);

    formLayout->addRow("尺寸:", sizeLayout);

    layout->addWidget(group);
}

void MorphologyToolDialog::createButtons(QVBoxLayout* layout)
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

void MorphologyToolDialog::connectSignals()
{
    // 参数变更
    connect(morphTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MorphologyToolDialog::onMorphTypeChanged);
    connect(kernelShapeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MorphologyToolDialog::onKernelShapeChanged);
    connect(kernelWidthSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MorphologyToolDialog::onKernelWidthChanged);
    connect(kernelHeightSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MorphologyToolDialog::onKernelHeightChanged);
    connect(iterationsSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MorphologyToolDialog::onIterationsChanged);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &MorphologyToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &MorphologyToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &MorphologyToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &MorphologyToolDialog::onApplyClicked);
}

void MorphologyToolDialog::updateUI()
{
    // 当前无需根据操作类型调整UI
    // 所有参数对所有操作类型都有效
}

void MorphologyToolDialog::loadParameters()
{
    if (!tool_) return;

    // 加载操作类型
    int typeIndex = morphTypeCombo_->findData(static_cast<int>(tool_->morphType()));
    if (typeIndex >= 0) {
        morphTypeCombo_->setCurrentIndex(typeIndex);
    }

    // 加载迭代次数
    iterationsSpin_->setValue(tool_->iterations());

    // 加载结构元素形状
    int shapeIndex = kernelShapeCombo_->findData(static_cast<int>(tool_->kernelShape()));
    if (shapeIndex >= 0) {
        kernelShapeCombo_->setCurrentIndex(shapeIndex);
    }

    // 加载结构元素尺寸
    kernelWidthSpin_->setValue(tool_->kernelWidth());
    kernelHeightSpin_->setValue(tool_->kernelHeight());
}

void MorphologyToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setMorphType(
        static_cast<Algorithm::MorphologyTool::MorphType>(morphTypeCombo_->currentData().toInt()));
    tool_->setKernelShape(
        static_cast<Algorithm::MorphologyTool::KernelShape>(kernelShapeCombo_->currentData().toInt()));
    tool_->setKernelWidth(kernelWidthSpin_->value());
    tool_->setKernelHeight(kernelHeightSpin_->value());
    tool_->setIterations(iterationsSpin_->value());

    LOG_INFO("形态学处理参数已更新");
}

void MorphologyToolDialog::onMorphTypeChanged(int index)
{
    Q_UNUSED(index);
    updateUI();
    applyParameters();
}

void MorphologyToolDialog::onKernelShapeChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

void MorphologyToolDialog::onKernelWidthChanged(int value)
{
    Q_UNUSED(value);
    applyParameters();
}

void MorphologyToolDialog::onKernelHeightChanged(int value)
{
    Q_UNUSED(value);
    applyParameters();
}

void MorphologyToolDialog::onIterationsChanged(int value)
{
    Q_UNUSED(value);
    applyParameters();
}

void MorphologyToolDialog::onPreviewClicked()
{
    applyParameters();
    emit previewRequested();
}

void MorphologyToolDialog::onOkClicked()
{
    applyParameters();
    emit parametersApplied();
    accept();
}

void MorphologyToolDialog::onCancelClicked()
{
    reject();
}

void MorphologyToolDialog::onApplyClicked()
{
    applyParameters();
    emit parametersApplied();
}

} // namespace UI
} // namespace VisionForge
