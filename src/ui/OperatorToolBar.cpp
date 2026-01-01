/**
 * @file OperatorToolBar.cpp
 * @brief 操作员底部工具栏实现
 */

#include "ui/OperatorToolBar.h"
#include "ui/Theme.h"
#include "base/Logger.h"

#include <QApplication>
#include <QStyle>

namespace VisionForge {
namespace UI {

OperatorToolBar::OperatorToolBar(QWidget* parent)
    : QWidget(parent)
    , layout_(nullptr)
    , iconSize_(48)
    , isRunning_(false)
{
    setupStyles();
    setupUI();
}

OperatorToolBar::~OperatorToolBar()
{
}

void OperatorToolBar::setupStyles()
{
    // 普通按钮样式
    normalStyle_ = R"(
        QPushButton {
            background-color: #4a90d9;
            color: white;
            border: 1px solid #3a80c9;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 12px;
            font-weight: bold;
            min-width: 70px;
            min-height: 70px;
        }
        QPushButton:hover {
            background-color: #5aa0e9;
            border-color: #4a90d9;
        }
        QPushButton:pressed {
            background-color: #3a80c9;
        }
        QPushButton:disabled {
            background-color: #666666;
            color: #999999;
            border-color: #555555;
        }
    )";

    // 高亮按钮样式（运行中等状态）
    highlightStyle_ = R"(
        QPushButton {
            background-color: #28a745;
            color: white;
            border: 1px solid #1e7e34;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 12px;
            font-weight: bold;
            min-width: 70px;
            min-height: 70px;
        }
        QPushButton:hover {
            background-color: #34ce57;
            border-color: #28a745;
        }
        QPushButton:pressed {
            background-color: #1e7e34;
        }
    )";

    // 危险操作按钮样式（退出等）
    dangerStyle_ = R"(
        QPushButton {
            background-color: #dc3545;
            color: white;
            border: 1px solid #c82333;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 12px;
            font-weight: bold;
            min-width: 70px;
            min-height: 70px;
        }
        QPushButton:hover {
            background-color: #e4606d;
            border-color: #dc3545;
        }
        QPushButton:pressed {
            background-color: #c82333;
        }
    )";
}

void OperatorToolBar::setupUI()
{
    // 设置工具栏背景
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(60, 60, 60));
    setPalette(pal);

    // 创建布局
    layout_ = new QHBoxLayout(this);
    layout_->setContentsMargins(10, 5, 10, 5);
    layout_->setSpacing(8);

    // 创建按钮（按截图顺序）
    buttons_[Camera] = createToolButton("camera", tr("相机"), Camera);
    buttons_[Communication] = createToolButton("network", tr("通信"), Communication);
    buttons_[Product] = createToolButton("product", tr("产品"), Product);
    buttons_[System] = createToolButton("settings", tr("系统"), System);
    buttons_[Options] = createToolButton("options", tr("选项"), Options);
    buttons_[Preview] = createToolButton("image", tr("图像预览"), Preview);
    buttons_[Run] = createToolButton("play", tr("运行"), Run);
    buttons_[Login] = createToolButton("user", tr("登录"), Login);

    // 添加弹性空间
    layout_->addStretch();

    // 右侧按钮
    buttons_[Exit] = createToolButton("exit", tr("退出"), Exit);
    buttons_[Hide] = createToolButton("hide", tr("隐藏"), Hide);
    buttons_[Station] = createToolButton("station", tr("工位"), Station);

    // 设置退出按钮为危险样式
    buttons_[Exit]->setStyleSheet(dangerStyle_);

    // 连接信号
    connect(buttons_[Camera], &QPushButton::clicked, this, &OperatorToolBar::cameraClicked);
    connect(buttons_[Communication], &QPushButton::clicked, this, &OperatorToolBar::communicationClicked);
    connect(buttons_[Product], &QPushButton::clicked, this, &OperatorToolBar::productClicked);
    connect(buttons_[System], &QPushButton::clicked, this, &OperatorToolBar::systemClicked);
    connect(buttons_[Options], &QPushButton::clicked, this, &OperatorToolBar::optionsClicked);
    connect(buttons_[Preview], &QPushButton::clicked, this, &OperatorToolBar::previewClicked);
    connect(buttons_[Run], &QPushButton::clicked, this, &OperatorToolBar::runClicked);
    connect(buttons_[Login], &QPushButton::clicked, this, &OperatorToolBar::loginClicked);
    connect(buttons_[Exit], &QPushButton::clicked, this, &OperatorToolBar::exitClicked);
    connect(buttons_[Hide], &QPushButton::clicked, this, &OperatorToolBar::hideClicked);
    connect(buttons_[Station], &QPushButton::clicked, this, &OperatorToolBar::stationClicked);

    // 设置固定高度
    setFixedHeight(90);
}

QPushButton* OperatorToolBar::createToolButton(const QString& iconName, const QString& text, ButtonType type)
{
    QPushButton* btn = new QPushButton(this);
    btn->setText(text);
    btn->setStyleSheet(normalStyle_);

    // 尝试加载图标
    QIcon icon = Theme::getIcon(iconName);
    if (!icon.isNull()) {
        btn->setIcon(icon);
        btn->setIconSize(QSize(32, 32));  // 固定图标大小
    }

    // 添加到布局
    layout_->addWidget(btn);

    return btn;
}

void OperatorToolBar::applyButtonStyle(QPushButton* btn, bool highlight)
{
    if (btn) {
        btn->setStyleSheet(highlight ? highlightStyle_ : normalStyle_);
    }
}

void OperatorToolBar::setButtonVisible(ButtonType type, bool visible)
{
    if (buttons_.contains(type)) {
        buttons_[type]->setVisible(visible);
    }
}

void OperatorToolBar::setButtonEnabled(ButtonType type, bool enabled)
{
    if (buttons_.contains(type)) {
        buttons_[type]->setEnabled(enabled);
    }
}

void OperatorToolBar::setButtonText(ButtonType type, const QString& text)
{
    if (buttons_.contains(type)) {
        buttons_[type]->setText(text);
    }
}

void OperatorToolBar::setIconSize(int size)
{
    iconSize_ = size;
    for (auto* btn : buttons_) {
        btn->setIconSize(QSize(size, size));
    }
}

void OperatorToolBar::setRunningState(bool running)
{
    isRunning_ = running;
    if (buttons_.contains(Run)) {
        if (running) {
            buttons_[Run]->setText(tr("停止"));
            buttons_[Run]->setStyleSheet(highlightStyle_);
        } else {
            buttons_[Run]->setText(tr("运行"));
            buttons_[Run]->setStyleSheet(normalStyle_);
        }
    }
}

void OperatorToolBar::setLoginState(bool loggedIn, const QString& username)
{
    if (buttons_.contains(Login)) {
        if (loggedIn && !username.isEmpty()) {
            buttons_[Login]->setText(username);
            buttons_[Login]->setStyleSheet(highlightStyle_);
        } else {
            buttons_[Login]->setText(tr("登录"));
            buttons_[Login]->setStyleSheet(normalStyle_);
        }
    }
}

} // namespace UI
} // namespace VisionForge
