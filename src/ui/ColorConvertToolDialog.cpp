/**
 * @file ColorConvertToolDialog.cpp
 * @brief 颜色转换工具参数设置对话框实现
 */

#include "ui/ColorConvertToolDialog.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

ColorConvertToolDialog::ColorConvertToolDialog(Algorithm::ColorConvertTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , previewImage_(nullptr)
    , sourceSpaceCombo_(nullptr)
    , targetSpaceCombo_(nullptr)
    , channelExtractGroup_(nullptr)
    , extractChannelCheck_(nullptr)
    , channelIndexCombo_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("颜色转换参数设置");
    setMinimumSize(400, 350);
    resize(450, 400);

    createUI();
    connectSignals();
    loadParameters();
    updateUI();
}

ColorConvertToolDialog::~ColorConvertToolDialog()
{
}

void ColorConvertToolDialog::setPreviewImage(const Base::ImageData::Ptr& image)
{
    previewImage_ = image;
}

void ColorConvertToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createColorSpaceGroup(mainLayout);
    createChannelExtractGroup(mainLayout);
    createButtons(mainLayout);

    mainLayout->addStretch();
}

void ColorConvertToolDialog::createColorSpaceGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("颜色空间转换", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 源颜色空间
    sourceSpaceCombo_ = new QComboBox(group);
    sourceSpaceCombo_->addItem("BGR (OpenCV默认)", static_cast<int>(Algorithm::ColorConvertTool::BGR));
    sourceSpaceCombo_->addItem("RGB", static_cast<int>(Algorithm::ColorConvertTool::RGB));
    sourceSpaceCombo_->addItem("HSV", static_cast<int>(Algorithm::ColorConvertTool::HSV));
    sourceSpaceCombo_->addItem("HLS", static_cast<int>(Algorithm::ColorConvertTool::HLS));
    sourceSpaceCombo_->addItem("Lab", static_cast<int>(Algorithm::ColorConvertTool::Lab));
    sourceSpaceCombo_->addItem("YCrCb", static_cast<int>(Algorithm::ColorConvertTool::YCrCb));
    sourceSpaceCombo_->addItem("XYZ", static_cast<int>(Algorithm::ColorConvertTool::XYZ));
    sourceSpaceCombo_->addItem("灰度", static_cast<int>(Algorithm::ColorConvertTool::Grayscale));
    formLayout->addRow("源颜色空间:", sourceSpaceCombo_);

    // 目标颜色空间
    targetSpaceCombo_ = new QComboBox(group);
    targetSpaceCombo_->addItem("BGR (OpenCV默认)", static_cast<int>(Algorithm::ColorConvertTool::BGR));
    targetSpaceCombo_->addItem("RGB", static_cast<int>(Algorithm::ColorConvertTool::RGB));
    targetSpaceCombo_->addItem("HSV", static_cast<int>(Algorithm::ColorConvertTool::HSV));
    targetSpaceCombo_->addItem("HLS", static_cast<int>(Algorithm::ColorConvertTool::HLS));
    targetSpaceCombo_->addItem("Lab", static_cast<int>(Algorithm::ColorConvertTool::Lab));
    targetSpaceCombo_->addItem("YCrCb", static_cast<int>(Algorithm::ColorConvertTool::YCrCb));
    targetSpaceCombo_->addItem("XYZ", static_cast<int>(Algorithm::ColorConvertTool::XYZ));
    targetSpaceCombo_->addItem("灰度", static_cast<int>(Algorithm::ColorConvertTool::Grayscale));
    formLayout->addRow("目标颜色空间:", targetSpaceCombo_);

    layout->addWidget(group);
}

void ColorConvertToolDialog::createChannelExtractGroup(QVBoxLayout* layout)
{
    channelExtractGroup_ = new QGroupBox("通道提取", this);
    QVBoxLayout* vLayout = new QVBoxLayout(channelExtractGroup_);

    // 是否提取单通道
    extractChannelCheck_ = new QCheckBox("提取单通道", channelExtractGroup_);
    extractChannelCheck_->setToolTip("从目标颜色空间提取单个通道");
    vLayout->addWidget(extractChannelCheck_);

    // 通道选择
    QHBoxLayout* channelLayout = new QHBoxLayout();
    channelLayout->addWidget(new QLabel("通道:"));

    channelIndexCombo_ = new QComboBox(channelExtractGroup_);
    channelLayout->addWidget(channelIndexCombo_);
    channelLayout->addStretch();

    vLayout->addLayout(channelLayout);

    layout->addWidget(channelExtractGroup_);
}

void ColorConvertToolDialog::createButtons(QVBoxLayout* layout)
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

void ColorConvertToolDialog::connectSignals()
{
    // 参数变更
    connect(sourceSpaceCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ColorConvertToolDialog::onSourceSpaceChanged);
    connect(targetSpaceCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ColorConvertToolDialog::onTargetSpaceChanged);
    connect(extractChannelCheck_, &QCheckBox::toggled,
            this, &ColorConvertToolDialog::onExtractChannelChanged);
    connect(channelIndexCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ColorConvertToolDialog::onChannelIndexChanged);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &ColorConvertToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &ColorConvertToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &ColorConvertToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &ColorConvertToolDialog::onApplyClicked);
}

void ColorConvertToolDialog::updateUI()
{
    // 通道提取仅当目标不是灰度时可用
    int targetSpace = targetSpaceCombo_->currentData().toInt();
    bool isGrayscale = (targetSpace == static_cast<int>(Algorithm::ColorConvertTool::Grayscale));

    channelExtractGroup_->setEnabled(!isGrayscale);

    // 通道选择仅当启用提取时可用
    channelIndexCombo_->setEnabled(extractChannelCheck_->isChecked() && !isGrayscale);

    // 更新通道下拉框内容
    updateChannelCombo();
}

void ColorConvertToolDialog::updateChannelCombo()
{
    channelIndexCombo_->blockSignals(true);
    channelIndexCombo_->clear();

    int targetSpace = targetSpaceCombo_->currentData().toInt();
    auto colorSpace = static_cast<Algorithm::ColorConvertTool::ColorSpace>(targetSpace);

    // 根据目标颜色空间添加通道选项
    int channelCount = Algorithm::ColorConvertTool::channelCount(colorSpace);

    for (int i = 0; i < channelCount; ++i) {
        QString channelName = Algorithm::ColorConvertTool::channelName(colorSpace, i);
        channelIndexCombo_->addItem(QString("通道 %1 (%2)").arg(i).arg(channelName), i);
    }

    channelIndexCombo_->blockSignals(false);
}

void ColorConvertToolDialog::loadParameters()
{
    if (!tool_) return;

    // 加载源颜色空间
    int sourceIndex = sourceSpaceCombo_->findData(static_cast<int>(tool_->sourceSpace()));
    if (sourceIndex >= 0) {
        sourceSpaceCombo_->setCurrentIndex(sourceIndex);
    }

    // 加载目标颜色空间
    int targetIndex = targetSpaceCombo_->findData(static_cast<int>(tool_->targetSpace()));
    if (targetIndex >= 0) {
        targetSpaceCombo_->setCurrentIndex(targetIndex);
    }

    // 加载通道提取设置
    extractChannelCheck_->setChecked(tool_->extractChannel());

    // 更新通道下拉框后设置通道索引
    updateChannelCombo();
    int channelIndex = channelIndexCombo_->findData(tool_->channelIndex());
    if (channelIndex >= 0) {
        channelIndexCombo_->setCurrentIndex(channelIndex);
    }
}

void ColorConvertToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setSourceSpace(
        static_cast<Algorithm::ColorConvertTool::ColorSpace>(sourceSpaceCombo_->currentData().toInt()));
    tool_->setTargetSpace(
        static_cast<Algorithm::ColorConvertTool::ColorSpace>(targetSpaceCombo_->currentData().toInt()));
    tool_->setExtractChannel(extractChannelCheck_->isChecked());
    tool_->setChannelIndex(channelIndexCombo_->currentData().toInt());

    LOG_INFO("颜色转换参数已更新");
}

void ColorConvertToolDialog::onSourceSpaceChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

void ColorConvertToolDialog::onTargetSpaceChanged(int index)
{
    Q_UNUSED(index);
    updateUI();
    applyParameters();
}

void ColorConvertToolDialog::onExtractChannelChanged(bool checked)
{
    Q_UNUSED(checked);
    updateUI();
    applyParameters();
}

void ColorConvertToolDialog::onChannelIndexChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

void ColorConvertToolDialog::onPreviewClicked()
{
    applyParameters();
    emit previewRequested();
}

void ColorConvertToolDialog::onOkClicked()
{
    applyParameters();
    emit parametersApplied();
    accept();
}

void ColorConvertToolDialog::onCancelClicked()
{
    reject();
}

void ColorConvertToolDialog::onApplyClicked()
{
    applyParameters();
    emit parametersApplied();
}

} // namespace UI
} // namespace VisionForge
