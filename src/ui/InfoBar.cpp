/**
 * @file InfoBar.cpp
 * @brief 顶部信息栏实现
 */

#include "ui/InfoBar.h"
#include "ui/Theme.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

InfoBar::InfoBar(QWidget* parent)
    : QWidget(parent)
    , layout_(nullptr)
    , sceneTabBar_(nullptr)
    , positionCombo_(nullptr)
    , btnDetectionInfo_(nullptr)
    , btnRunInfo_(nullptr)
    , btnClearDisplay_(nullptr)
    , btnVirtualComm_(nullptr)
    , resultLabel_(nullptr)
{
    setupUI();
}

InfoBar::~InfoBar()
{
}

void InfoBar::setupUI()
{
    // 设置背景
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(45, 45, 45));
    setPalette(pal);

    // 固定高度
    setFixedHeight(32);

    // 创建布局
    layout_ = new QHBoxLayout(this);
    layout_->setContentsMargins(5, 2, 5, 2);
    layout_->setSpacing(5);

    // 场景标签栏
    sceneTabBar_ = new QTabBar(this);
    sceneTabBar_->setExpanding(false);
    sceneTabBar_->setDocumentMode(true);
    sceneTabBar_->setStyleSheet(R"(
        QTabBar::tab {
            background-color: #3d3d3d;
            color: #cccccc;
            border: 1px solid #555555;
            border-bottom: none;
            padding: 4px 12px;
            margin-right: 2px;
            min-width: 60px;
        }
        QTabBar::tab:selected {
            background-color: #0078d4;
            color: white;
        }
        QTabBar::tab:hover:!selected {
            background-color: #4d4d4d;
        }
    )");
    layout_->addWidget(sceneTabBar_);

    // 添加弹性空间
    layout_->addStretch();

    // 位置选择下拉框
    positionCombo_ = new QComboBox(this);
    positionCombo_->setMinimumWidth(80);
    positionCombo_->setStyleSheet(R"(
        QComboBox {
            background-color: #3d3d3d;
            color: white;
            border: 1px solid #555555;
            border-radius: 3px;
            padding: 2px 8px;
        }
        QComboBox:hover {
            border-color: #0078d4;
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        QComboBox QAbstractItemView {
            background-color: #3d3d3d;
            color: white;
            selection-background-color: #0078d4;
        }
    )");
    // 不添加默认位置，由外部通过setPositions()设置
    layout_->addWidget(positionCombo_);

    // 按钮样式
    QString btnStyle = R"(
        QPushButton {
            background-color: #3d3d3d;
            color: #cccccc;
            border: 1px solid #555555;
            border-radius: 3px;
            padding: 3px 10px;
            font-size: 11px;
        }
        QPushButton:hover {
            background-color: #4d4d4d;
            border-color: #0078d4;
        }
        QPushButton:pressed {
            background-color: #2d2d2d;
        }
    )";

    // 检测信息按钮
    btnDetectionInfo_ = new QPushButton(tr("检测信息"), this);
    btnDetectionInfo_->setStyleSheet(btnStyle);
    layout_->addWidget(btnDetectionInfo_);

    // 运行信息按钮
    btnRunInfo_ = new QPushButton(tr("运行信息"), this);
    btnRunInfo_->setStyleSheet(btnStyle);
    layout_->addWidget(btnRunInfo_);

    // GUI清除按钮
    btnClearDisplay_ = new QPushButton(tr("GUI清除"), this);
    btnClearDisplay_->setStyleSheet(btnStyle);
    layout_->addWidget(btnClearDisplay_);

    // 虚拟通信按钮
    btnVirtualComm_ = new QPushButton(tr("虚拟通信"), this);
    btnVirtualComm_->setStyleSheet(btnStyle);
    layout_->addWidget(btnVirtualComm_);

    // 连接信号
    connect(sceneTabBar_, &QTabBar::currentChanged, this, &InfoBar::sceneChanged);
    connect(positionCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InfoBar::positionChanged);
    connect(btnDetectionInfo_, &QPushButton::clicked, this, &InfoBar::detectionInfoClicked);
    connect(btnRunInfo_, &QPushButton::clicked, this, &InfoBar::runInfoClicked);
    connect(btnClearDisplay_, &QPushButton::clicked, this, &InfoBar::clearDisplayClicked);
    connect(btnVirtualComm_, &QPushButton::clicked, this, &InfoBar::virtualCommClicked);
}

void InfoBar::setScenes(const QStringList& scenes)
{
    // 阻止信号发射，避免在初始化时触发不必要的场景切换
    sceneTabBar_->blockSignals(true);

    // 清除现有标签
    while (sceneTabBar_->count() > 0) {
        sceneTabBar_->removeTab(0);
    }

    // 添加新标签
    for (const QString& scene : scenes) {
        sceneTabBar_->addTab(scene);
    }

    // 恢复信号
    sceneTabBar_->blockSignals(false);

    // 不再添加默认场景，场景列表由配置决定
}

void InfoBar::setCurrentScene(int index)
{
    if (index >= 0 && index < sceneTabBar_->count()) {
        sceneTabBar_->setCurrentIndex(index);
    }
}

int InfoBar::currentScene() const
{
    return sceneTabBar_->currentIndex();
}

void InfoBar::setPositions(const QStringList& positions)
{
    // 阻止信号发射，避免在初始化时触发不必要的位置切换
    positionCombo_->blockSignals(true);

    positionCombo_->clear();
    positionCombo_->addItems(positions);

    // 恢复信号
    positionCombo_->blockSignals(false);

    // 不再添加默认位置，位置列表由配置决定
}

void InfoBar::setCurrentPosition(int index)
{
    if (index >= 0 && index < positionCombo_->count()) {
        positionCombo_->setCurrentIndex(index);
    }
}

int InfoBar::currentPosition() const
{
    return positionCombo_->currentIndex();
}

void InfoBar::setDetectionInfo(const QString& info)
{
    btnDetectionInfo_->setToolTip(info);
}

void InfoBar::setRunInfo(const QString& info)
{
    btnRunInfo_->setToolTip(info);
}

void InfoBar::setDetectionResult(bool ok, const QString& message)
{
    // 根据结果设置按钮样式
    QString style;
    if (ok) {
        style = R"(
            QPushButton {
                background-color: #28a745;
                color: white;
                border: 1px solid #1e7e34;
                border-radius: 3px;
                padding: 3px 10px;
                font-size: 11px;
            }
        )";
    } else {
        style = R"(
            QPushButton {
                background-color: #dc3545;
                color: white;
                border: 1px solid #c82333;
                border-radius: 3px;
                padding: 3px 10px;
                font-size: 11px;
            }
        )";
    }
    btnDetectionInfo_->setStyleSheet(style);

    if (!message.isEmpty()) {
        btnDetectionInfo_->setText(message);
    }
}

void InfoBar::clearInfo()
{
    btnDetectionInfo_->setText(tr("检测信息"));
    btnDetectionInfo_->setToolTip(QString());

    // 恢复默认样式
    QString btnStyle = R"(
        QPushButton {
            background-color: #3d3d3d;
            color: #cccccc;
            border: 1px solid #555555;
            border-radius: 3px;
            padding: 3px 10px;
            font-size: 11px;
        }
        QPushButton:hover {
            background-color: #4d4d4d;
            border-color: #0078d4;
        }
    )";
    btnDetectionInfo_->setStyleSheet(btnStyle);
}

} // namespace UI
} // namespace VisionForge
