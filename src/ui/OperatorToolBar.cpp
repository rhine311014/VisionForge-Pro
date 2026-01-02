/**
 * @file OperatorToolBar.cpp
 * @brief 操作员底部工具栏实现
 */

#include "ui/OperatorToolBar.h"
#include "ui/Theme.h"
#include "base/Logger.h"

#include <QApplication>
#include <QStyle>
#include <QMenu>
#include <QAction>

namespace VisionForge {
namespace UI {

OperatorToolBar::OperatorToolBar(QWidget* parent)
    : QWidget(parent)
    , layout_(nullptr)
    , iconSize_(48)
    , isRunning_(false)
    , frameLiveMode_(FrameValid)
    , isLiveDisplayActive_(false)
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

    // 激活状态样式（实时显示激活时）
    activeStyle_ = R"(
        QPushButton {
            background-color: #ff9800;
            color: white;
            border: 2px solid #f57c00;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 12px;
            font-weight: bold;
            min-width: 70px;
            min-height: 70px;
        }
        QPushButton:hover {
            background-color: #ffb74d;
            border-color: #ff9800;
        }
        QPushButton:pressed {
            background-color: #f57c00;
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
    buttons_[FrameLive] = createToolButton("image", tr("帧有效"), FrameLive);  // 帧有效/实时显示二合一
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
    connect(buttons_[FrameLive], &QPushButton::clicked, this, &OperatorToolBar::onFrameLiveButtonClicked);

    // 帧有效/实时显示按钮右键菜单 - 切换模式
    buttons_[FrameLive]->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(buttons_[FrameLive], &QPushButton::customContextMenuRequested, [this](const QPoint& pos) {
        QMenu menu(this);
        QAction* frameValidAction = menu.addAction(tr("帧有效模式"));
        frameValidAction->setCheckable(true);
        frameValidAction->setChecked(frameLiveMode_ == FrameValid);

        QAction* liveDisplayAction = menu.addAction(tr("实时显示模式"));
        liveDisplayAction->setCheckable(true);
        liveDisplayAction->setChecked(frameLiveMode_ == LiveDisplay);

        connect(frameValidAction, &QAction::triggered, [this]() {
            setFrameLiveMode(FrameValid);
        });
        connect(liveDisplayAction, &QAction::triggered, [this]() {
            setFrameLiveMode(LiveDisplay);
        });

        menu.exec(buttons_[FrameLive]->mapToGlobal(pos));
    });

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

void OperatorToolBar::setFrameLiveMode(FrameLiveMode mode)
{
    if (frameLiveMode_ != mode) {
        frameLiveMode_ = mode;
        updateFrameLiveButton();
        emit frameLiveModeChanged(mode);
    }
}

void OperatorToolBar::setLiveDisplayActive(bool active)
{
    if (isLiveDisplayActive_ != active) {
        isLiveDisplayActive_ = active;
        updateFrameLiveButton();
    }
}

void OperatorToolBar::onFrameLiveButtonClicked()
{
    emit frameLiveClicked();

    if (frameLiveMode_ == FrameValid) {
        // 帧有效模式：单击触发一次帧采集
        emit frameValidTriggered();
    } else {
        // 实时显示模式：切换实时显示开关
        isLiveDisplayActive_ = !isLiveDisplayActive_;
        updateFrameLiveButton();
        emit liveDisplayToggled(isLiveDisplayActive_);
    }
}

void OperatorToolBar::updateFrameLiveButton()
{
    if (!buttons_.contains(FrameLive)) {
        return;
    }

    QPushButton* btn = buttons_[FrameLive];

    if (frameLiveMode_ == FrameValid) {
        // 帧有效模式
        btn->setText(tr("帧有效"));
        btn->setStyleSheet(normalStyle_);
    } else {
        // 实时显示模式
        if (isLiveDisplayActive_) {
            btn->setText(tr("实时显示\n(运行中)"));
            btn->setStyleSheet(activeStyle_);
        } else {
            btn->setText(tr("实时显示"));
            btn->setStyleSheet(normalStyle_);
        }
    }
}

} // namespace UI
} // namespace VisionForge
