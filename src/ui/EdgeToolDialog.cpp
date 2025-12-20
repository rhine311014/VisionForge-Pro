/**
 * @file EdgeToolDialog.cpp
 * @brief 边缘检测工具参数设置对话框实现
 */

#include "ui/EdgeToolDialog.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

EdgeToolDialog::EdgeToolDialog(Algorithm::EdgeTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , previewImage_(nullptr)
    , edgeTypeCombo_(nullptr)
    , cannyGroup_(nullptr)
    , threshold1Spin_(nullptr)
    , threshold2Spin_(nullptr)
    , l2GradientCheck_(nullptr)
    , sobelGroup_(nullptr)
    , kernelSizeCombo_(nullptr)
    , directionCombo_(nullptr)
    , scaleSpin_(nullptr)
    , deltaSpin_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
{
    setWindowTitle("边缘检测参数设置");
    setMinimumSize(450, 450);
    resize(500, 500);

    createUI();
    connectSignals();
    loadParameters();
    updateUI();
}

EdgeToolDialog::~EdgeToolDialog()
{
}

void EdgeToolDialog::setPreviewImage(const Base::ImageData::Ptr& image)
{
    previewImage_ = image;
}

void EdgeToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createBasicParamsGroup(mainLayout);
    createCannyParamsGroup(mainLayout);
    createSobelParamsGroup(mainLayout);
    createButtons(mainLayout);

    mainLayout->addStretch();
}

void EdgeToolDialog::createBasicParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("基本参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 边缘检测类型
    edgeTypeCombo_ = new QComboBox(group);
    edgeTypeCombo_->addItem("Canny边缘检测", static_cast<int>(Algorithm::EdgeTool::Canny));
    edgeTypeCombo_->addItem("Sobel算子", static_cast<int>(Algorithm::EdgeTool::Sobel));
    edgeTypeCombo_->addItem("Laplacian算子", static_cast<int>(Algorithm::EdgeTool::Laplacian));
    edgeTypeCombo_->addItem("Scharr算子", static_cast<int>(Algorithm::EdgeTool::Scharr));
    formLayout->addRow("检测类型:", edgeTypeCombo_);

    layout->addWidget(group);
}

void EdgeToolDialog::createCannyParamsGroup(QVBoxLayout* layout)
{
    cannyGroup_ = new QGroupBox("Canny参数", this);
    QFormLayout* formLayout = new QFormLayout(cannyGroup_);

    // 低阈值
    threshold1Spin_ = new QDoubleSpinBox(cannyGroup_);
    threshold1Spin_->setRange(0.0, 500.0);
    threshold1Spin_->setSingleStep(10.0);
    threshold1Spin_->setValue(50.0);
    threshold1Spin_->setDecimals(1);
    threshold1Spin_->setToolTip("低阈值：弱边缘判定");
    formLayout->addRow("低阈值:", threshold1Spin_);

    // 高阈值
    threshold2Spin_ = new QDoubleSpinBox(cannyGroup_);
    threshold2Spin_->setRange(0.0, 500.0);
    threshold2Spin_->setSingleStep(10.0);
    threshold2Spin_->setValue(150.0);
    threshold2Spin_->setDecimals(1);
    threshold2Spin_->setToolTip("高阈值：强边缘判定");
    formLayout->addRow("高阈值:", threshold2Spin_);

    // L2梯度
    l2GradientCheck_ = new QCheckBox("使用L2范数", cannyGroup_);
    l2GradientCheck_->setToolTip("使用更精确的L2范数计算梯度幅值");
    formLayout->addRow("", l2GradientCheck_);

    layout->addWidget(cannyGroup_);
}

void EdgeToolDialog::createSobelParamsGroup(QVBoxLayout* layout)
{
    sobelGroup_ = new QGroupBox("Sobel/Laplacian/Scharr参数", this);
    QFormLayout* formLayout = new QFormLayout(sobelGroup_);

    // 核大小
    kernelSizeCombo_ = new QComboBox(sobelGroup_);
    kernelSizeCombo_->addItem("1", 1);
    kernelSizeCombo_->addItem("3", 3);
    kernelSizeCombo_->addItem("5", 5);
    kernelSizeCombo_->addItem("7", 7);
    kernelSizeCombo_->setCurrentIndex(1);  // 默认3
    formLayout->addRow("核大小:", kernelSizeCombo_);

    // 方向（仅Sobel/Scharr）
    directionCombo_ = new QComboBox(sobelGroup_);
    directionCombo_->addItem("X和Y方向", static_cast<int>(Algorithm::EdgeTool::Both));
    directionCombo_->addItem("水平方向 (X)", static_cast<int>(Algorithm::EdgeTool::Horizontal));
    directionCombo_->addItem("垂直方向 (Y)", static_cast<int>(Algorithm::EdgeTool::Vertical));
    formLayout->addRow("检测方向:", directionCombo_);

    // 缩放因子
    scaleSpin_ = new QDoubleSpinBox(sobelGroup_);
    scaleSpin_->setRange(0.0, 100.0);
    scaleSpin_->setSingleStep(0.1);
    scaleSpin_->setValue(1.0);
    scaleSpin_->setDecimals(2);
    scaleSpin_->setToolTip("缩放因子（默认1.0）");
    formLayout->addRow("缩放因子:", scaleSpin_);

    // 偏移量
    deltaSpin_ = new QDoubleSpinBox(sobelGroup_);
    deltaSpin_->setRange(0.0, 255.0);
    deltaSpin_->setSingleStep(1.0);
    deltaSpin_->setValue(0.0);
    deltaSpin_->setDecimals(1);
    deltaSpin_->setToolTip("添加到结果的偏移量");
    formLayout->addRow("偏移量:", deltaSpin_);

    layout->addWidget(sobelGroup_);
}

void EdgeToolDialog::createButtons(QVBoxLayout* layout)
{
    // 创建预览辅助器
    previewHelper_ = new PreviewHelper(this, 150);

    // 预览选项
    QHBoxLayout* previewLayout = new QHBoxLayout();

    autoPreviewCheck_ = new QCheckBox("实时预览", this);
    autoPreviewCheck_->setChecked(previewHelper_->isAutoPreviewEnabled());
    autoPreviewCheck_->setToolTip("启用后参数修改会自动更新预览");
    previewLayout->addWidget(autoPreviewCheck_);

    previewLayout->addStretch();

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

void EdgeToolDialog::connectSignals()
{
    // 参数变更
    connect(edgeTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EdgeToolDialog::onEdgeTypeChanged);
    connect(threshold1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EdgeToolDialog::onThreshold1Changed);
    connect(threshold2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EdgeToolDialog::onThreshold2Changed);
    connect(kernelSizeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EdgeToolDialog::onKernelSizeChanged);
    connect(directionCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EdgeToolDialog::onDirectionChanged);
    connect(l2GradientCheck_, &QCheckBox::toggled,
            this, &EdgeToolDialog::onL2GradientChanged);
    connect(scaleSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EdgeToolDialog::onScaleChanged);
    connect(deltaSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EdgeToolDialog::onDeltaChanged);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &EdgeToolDialog::onAutoPreview);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &EdgeToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &EdgeToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &EdgeToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &EdgeToolDialog::onApplyClicked);
}

void EdgeToolDialog::updateUI()
{
    int edgeType = edgeTypeCombo_->currentData().toInt();

    bool isCanny = (edgeType == static_cast<int>(Algorithm::EdgeTool::Canny));
    bool isSobel = (edgeType == static_cast<int>(Algorithm::EdgeTool::Sobel));
    bool isLaplacian = (edgeType == static_cast<int>(Algorithm::EdgeTool::Laplacian));
    bool isScharr = (edgeType == static_cast<int>(Algorithm::EdgeTool::Scharr));

    // Canny参数
    cannyGroup_->setVisible(isCanny);

    // Sobel/Laplacian/Scharr参数
    sobelGroup_->setVisible(!isCanny);

    // 方向选择（仅Sobel/Scharr）
    directionCombo_->setEnabled(isSobel || isScharr);

    // Scharr核大小固定为3
    kernelSizeCombo_->setEnabled(!isScharr);
    if (isScharr) {
        kernelSizeCombo_->setCurrentIndex(1);  // 3
    }

    // 调整窗口大小
    adjustSize();
}

void EdgeToolDialog::loadParameters()
{
    if (!tool_) return;

    // 加载边缘类型
    int typeIndex = edgeTypeCombo_->findData(static_cast<int>(tool_->edgeType()));
    if (typeIndex >= 0) {
        edgeTypeCombo_->setCurrentIndex(typeIndex);
    }

    // 加载Canny参数
    threshold1Spin_->setValue(tool_->threshold1());
    threshold2Spin_->setValue(tool_->threshold2());
    l2GradientCheck_->setChecked(tool_->l2Gradient());

    // 加载核大小
    int kernelIndex = kernelSizeCombo_->findData(tool_->kernelSize());
    if (kernelIndex >= 0) {
        kernelSizeCombo_->setCurrentIndex(kernelIndex);
    }

    // 加载方向
    int dirIndex = directionCombo_->findData(static_cast<int>(tool_->direction()));
    if (dirIndex >= 0) {
        directionCombo_->setCurrentIndex(dirIndex);
    }

    // 加载缩放和偏移
    scaleSpin_->setValue(tool_->scale());
    deltaSpin_->setValue(tool_->delta());
}

void EdgeToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setEdgeType(
        static_cast<Algorithm::EdgeTool::EdgeType>(edgeTypeCombo_->currentData().toInt()));
    tool_->setThreshold1(threshold1Spin_->value());
    tool_->setThreshold2(threshold2Spin_->value());
    tool_->setKernelSize(kernelSizeCombo_->currentData().toInt());
    tool_->setDirection(
        static_cast<Algorithm::EdgeTool::Direction>(directionCombo_->currentData().toInt()));
    tool_->setL2Gradient(l2GradientCheck_->isChecked());
    tool_->setScale(scaleSpin_->value());
    tool_->setDelta(deltaSpin_->value());

    LOG_INFO("边缘检测参数已更新");
}

void EdgeToolDialog::onEdgeTypeChanged(int index)
{
    Q_UNUSED(index);
    updateUI();
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onThreshold1Changed(double value)
{
    Q_UNUSED(value);
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onThreshold2Changed(double value)
{
    Q_UNUSED(value);
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onKernelSizeChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onDirectionChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onL2GradientChanged(bool checked)
{
    Q_UNUSED(checked);
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onScaleChanged(double value)
{
    Q_UNUSED(value);
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onDeltaChanged(double value)
{
    Q_UNUSED(value);
    applyParameters();
    previewHelper_->requestPreview();
}

void EdgeToolDialog::onPreviewClicked()
{
    applyParameters();
    emit previewRequested();
}

void EdgeToolDialog::onOkClicked()
{
    applyParameters();
    emit parametersApplied();
    accept();
}

void EdgeToolDialog::onCancelClicked()
{
    reject();
}

void EdgeToolDialog::onApplyClicked()
{
    applyParameters();
    emit parametersApplied();
}

void EdgeToolDialog::onAutoPreview()
{
    // 自动预览触发时发射预览请求信号
    emit previewRequested();
}

} // namespace UI
} // namespace VisionForge
