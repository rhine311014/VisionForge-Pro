/**
 * @file ToolParameterPanel.cpp
 * @brief 工具参数面板实现
 */

#include "ui/ToolParameterPanel.h"
#include "algorithm/GrayTool.h"
#include "base/Logger.h"
#include <QPushButton>

namespace VisionForge {
namespace UI {

ToolParameterPanel::ToolParameterPanel(QWidget* parent)
    : QWidget(parent)
    , currentTool_(nullptr)
    , convertModeCombo_(nullptr)
    , channelCombo_(nullptr)
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(5, 5, 5, 5);
    mainLayout_->setSpacing(10);

    // 创建通用参数组
    commonGroup_ = new QGroupBox("通用参数", this);
    QFormLayout* commonLayout = new QFormLayout(commonGroup_);

    enabledCheckBox_ = new QCheckBox(this);
    enabledCheckBox_->setChecked(true);
    connect(enabledCheckBox_, &QCheckBox::checkStateChanged, this, &ToolParameterPanel::onParameterChanged);
    commonLayout->addRow("启用:", enabledCheckBox_);

    displayNameEdit_ = new QLineEdit(this);
    connect(displayNameEdit_, &QLineEdit::textChanged, this, &ToolParameterPanel::onParameterChanged);
    commonLayout->addRow("显示名称:", displayNameEdit_);

    mainLayout_->addWidget(commonGroup_);

    // 创建工具特定参数组
    specificGroup_ = new QGroupBox("工具参数", this);
    specificLayout_ = new QFormLayout(specificGroup_);
    mainLayout_->addWidget(specificGroup_);

    // 添加弹簧
    mainLayout_->addStretch();

    // 默认禁用
    setEnabled(false);
}

void ToolParameterPanel::setTool(Algorithm::VisionTool* tool)
{
    if (currentTool_ == tool) {
        return;
    }

    // 断开旧工具的信号
    if (currentTool_) {
        disconnect(currentTool_, nullptr, this, nullptr);
    }

    currentTool_ = tool;

    if (!currentTool_) {
        clear();
        return;
    }

    // 连接工具的信号
    connect(currentTool_, &Algorithm::VisionTool::paramChanged,
            this, &ToolParameterPanel::refreshUI);

    // 清空旧的特定参数
    QLayoutItem* item;
    while ((item = specificLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    convertModeCombo_ = nullptr;
    channelCombo_ = nullptr;

    // 根据工具类型创建参数界面
    if (currentTool_->toolType() == Algorithm::VisionTool::Gray) {
        createGrayToolParameters();
    }

    // 刷新UI
    refreshUI();
    setEnabled(true);
}

void ToolParameterPanel::clear()
{
    currentTool_ = nullptr;
    enabledCheckBox_->setChecked(true);
    displayNameEdit_->clear();

    // 清空特定参数
    QLayoutItem* item;
    while ((item = specificLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    convertModeCombo_ = nullptr;
    channelCombo_ = nullptr;

    setEnabled(false);
}

void ToolParameterPanel::onParameterChanged()
{
    if (!currentTool_) {
        return;
    }

    updateToolParameters();
    emit parameterChanged();
}

void ToolParameterPanel::createGrayToolParameters()
{
    using namespace Algorithm;

    // 转换模式
    convertModeCombo_ = new QComboBox(this);
    convertModeCombo_->addItem("平均法", static_cast<int>(GrayTool::Average));
    convertModeCombo_->addItem("加权法", static_cast<int>(GrayTool::Weighted));
    convertModeCombo_->addItem("去饱和法", static_cast<int>(GrayTool::Desaturation));
    convertModeCombo_->addItem("单通道法", static_cast<int>(GrayTool::SingleChannel));
    connect(convertModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ToolParameterPanel::onParameterChanged);
    specificLayout_->addRow("转换模式:", convertModeCombo_);

    // 通道选择（仅单通道模式）
    channelCombo_ = new QComboBox(this);
    channelCombo_->addItem("蓝色通道 (B)", static_cast<int>(GrayTool::Blue));
    channelCombo_->addItem("绿色通道 (G)", static_cast<int>(GrayTool::Green));
    channelCombo_->addItem("红色通道 (R)", static_cast<int>(GrayTool::Red));
    connect(channelCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ToolParameterPanel::onParameterChanged);
    specificLayout_->addRow("单通道:", channelCombo_);

    // 根据转换模式显示/隐藏通道选择
    connect(convertModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index) {
                bool isSingleChannel = (convertModeCombo_->itemData(index).toInt()
                                       == static_cast<int>(GrayTool::SingleChannel));
                channelCombo_->setEnabled(isSingleChannel);
            });
}

void ToolParameterPanel::updateToolParameters()
{
    if (!currentTool_) {
        return;
    }

    // 阻塞信号避免循环触发
    currentTool_->blockSignals(true);

    // 更新通用参数
    currentTool_->setEnabled(enabledCheckBox_->isChecked());
    currentTool_->setDisplayName(displayNameEdit_->text());

    // 更新工具特定参数
    if (currentTool_->toolType() == Algorithm::VisionTool::Gray) {
        using namespace Algorithm;
        GrayTool* grayTool = dynamic_cast<GrayTool*>(currentTool_);

        if (grayTool && convertModeCombo_) {
            int modeIndex = convertModeCombo_->currentIndex();
            GrayTool::ConvertMode mode = static_cast<GrayTool::ConvertMode>(
                convertModeCombo_->itemData(modeIndex).toInt());
            grayTool->setConvertMode(mode);
        }

        if (grayTool && channelCombo_) {
            int channelIndex = channelCombo_->currentIndex();
            GrayTool::ChannelType channel = static_cast<GrayTool::ChannelType>(
                channelCombo_->itemData(channelIndex).toInt());
            grayTool->setChannel(channel);
        }
    }

    currentTool_->blockSignals(false);
}

void ToolParameterPanel::refreshUI()
{
    if (!currentTool_) {
        return;
    }

    // 阻塞信号避免循环触发
    blockSignals(true);

    // 刷新通用参数
    enabledCheckBox_->setChecked(currentTool_->isEnabled());
    displayNameEdit_->setText(currentTool_->displayName());

    // 刷新工具特定参数
    if (currentTool_->toolType() == Algorithm::VisionTool::Gray) {
        using namespace Algorithm;
        GrayTool* grayTool = dynamic_cast<GrayTool*>(currentTool_);

        if (grayTool && convertModeCombo_) {
            GrayTool::ConvertMode mode = grayTool->convertMode();
            for (int i = 0; i < convertModeCombo_->count(); ++i) {
                if (convertModeCombo_->itemData(i).toInt() == static_cast<int>(mode)) {
                    convertModeCombo_->setCurrentIndex(i);
                    break;
                }
            }
        }

        if (grayTool && channelCombo_) {
            GrayTool::ChannelType channel = grayTool->channel();
            for (int i = 0; i < channelCombo_->count(); ++i) {
                if (channelCombo_->itemData(i).toInt() == static_cast<int>(channel)) {
                    channelCombo_->setCurrentIndex(i);
                    break;
                }
            }

            // 更新通道选择的启用状态
            bool isSingleChannel = (grayTool->convertMode() == GrayTool::SingleChannel);
            channelCombo_->setEnabled(isSingleChannel);
        }
    }

    blockSignals(false);
}

} // namespace UI
} // namespace VisionForge
