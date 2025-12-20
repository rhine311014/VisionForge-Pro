/**
 * @file GrayToolDialog.cpp
 * @brief 灰度转换工具参数设置对话框实现
 */

#include "ui/GrayToolDialog.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

GrayToolDialog::GrayToolDialog(Algorithm::GrayTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , previewImage_(nullptr)
    , convertModeCombo_(nullptr)
    , channelCombo_(nullptr)
    , channelLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
{
    setWindowTitle("灰度转换参数设置");
    setMinimumSize(350, 200);
    resize(400, 250);

    createUI();
    connectSignals();
    loadParameters();
    updateUI();
}

GrayToolDialog::~GrayToolDialog()
{
}

void GrayToolDialog::setPreviewImage(const Base::ImageData::Ptr& image)
{
    previewImage_ = image;
}

void GrayToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createParamsGroup(mainLayout);
    createButtons(mainLayout);

    mainLayout->addStretch();
}

void GrayToolDialog::createParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("转换参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 转换模式
    convertModeCombo_ = new QComboBox(group);
    convertModeCombo_->addItem("平均法", static_cast<int>(Algorithm::GrayTool::Average));
    convertModeCombo_->addItem("加权法 (推荐)", static_cast<int>(Algorithm::GrayTool::Weighted));
    convertModeCombo_->addItem("去饱和法", static_cast<int>(Algorithm::GrayTool::Desaturation));
    convertModeCombo_->addItem("单通道提取", static_cast<int>(Algorithm::GrayTool::SingleChannel));
    formLayout->addRow("转换模式:", convertModeCombo_);

    // 通道选择（仅单通道模式可用）
    channelLabel_ = new QLabel("提取通道:", group);
    channelCombo_ = new QComboBox(group);
    channelCombo_->addItem("蓝色通道 (B)", static_cast<int>(Algorithm::GrayTool::Blue));
    channelCombo_->addItem("绿色通道 (G)", static_cast<int>(Algorithm::GrayTool::Green));
    channelCombo_->addItem("红色通道 (R)", static_cast<int>(Algorithm::GrayTool::Red));
    formLayout->addRow(channelLabel_, channelCombo_);

    layout->addWidget(group);
}

void GrayToolDialog::createButtons(QVBoxLayout* layout)
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

void GrayToolDialog::connectSignals()
{
    // 参数变更
    connect(convertModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GrayToolDialog::onConvertModeChanged);
    connect(channelCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GrayToolDialog::onChannelChanged);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &GrayToolDialog::onAutoPreview);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &GrayToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &GrayToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &GrayToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &GrayToolDialog::onApplyClicked);
}

void GrayToolDialog::updateUI()
{
    // 通道选择仅在单通道模式下可用
    bool singleChannel = (convertModeCombo_->currentData().toInt() ==
                          static_cast<int>(Algorithm::GrayTool::SingleChannel));
    channelLabel_->setEnabled(singleChannel);
    channelCombo_->setEnabled(singleChannel);
}

void GrayToolDialog::loadParameters()
{
    if (!tool_) return;

    // 加载转换模式
    int modeIndex = convertModeCombo_->findData(static_cast<int>(tool_->convertMode()));
    if (modeIndex >= 0) {
        convertModeCombo_->setCurrentIndex(modeIndex);
    }

    // 加载通道
    int channelIndex = channelCombo_->findData(static_cast<int>(tool_->channel()));
    if (channelIndex >= 0) {
        channelCombo_->setCurrentIndex(channelIndex);
    }
}

void GrayToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setConvertMode(
        static_cast<Algorithm::GrayTool::ConvertMode>(convertModeCombo_->currentData().toInt()));
    tool_->setChannel(
        static_cast<Algorithm::GrayTool::ChannelType>(channelCombo_->currentData().toInt()));

    LOG_INFO("灰度转换参数已更新");
}

void GrayToolDialog::onConvertModeChanged(int index)
{
    Q_UNUSED(index);
    updateUI();
    applyParameters();
    previewHelper_->requestPreview();
}

void GrayToolDialog::onChannelChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
    previewHelper_->requestPreview();
}

void GrayToolDialog::onPreviewClicked()
{
    applyParameters();
    emit previewRequested();
}

void GrayToolDialog::onOkClicked()
{
    applyParameters();
    emit parametersApplied();
    accept();
}

void GrayToolDialog::onCancelClicked()
{
    reject();
}

void GrayToolDialog::onApplyClicked()
{
    applyParameters();
    emit parametersApplied();
}

void GrayToolDialog::onAutoPreview()
{
    // 自动预览触发时发射预览请求信号
    emit previewRequested();
}

} // namespace UI
} // namespace VisionForge
