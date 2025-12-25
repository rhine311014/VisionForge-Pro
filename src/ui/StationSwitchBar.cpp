/**
 * @file StationSwitchBar.cpp
 * @brief 工位切换工具栏实现
 */

#include "ui/StationSwitchBar.h"
#include "core/MultiStationManager.h"
#include "base/Logger.h"

#include <QShortcut>
#include <QKeySequence>

namespace VisionForge {
namespace UI {

StationSwitchBar::StationSwitchBar(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    refreshStations();
}

StationSwitchBar::~StationSwitchBar()
{
}

void StationSwitchBar::setupUI()
{
    // 创建布局
    layout_ = new QHBoxLayout(this);
    layout_->setSpacing(8);
    layout_->setContentsMargins(4, 4, 4, 4);

    // 创建按钮组
    buttonGroup_ = new QButtonGroup(this);
    buttonGroup_->setExclusive(true);

    // 样式定义
    normalBtnStyle_ = R"(
        QPushButton {
            background-color: #3d3d3d;
            color: #cccccc;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 6px 16px;
            font-size: 13px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #4d4d4d;
            border-color: #666666;
        }
        QPushButton:pressed {
            background-color: #2d2d2d;
        }
    )";

    selectedBtnStyle_ = R"(
        QPushButton {
            background-color: #0078d4;
            color: #ffffff;
            border: 1px solid #0078d4;
            border-radius: 4px;
            padding: 6px 16px;
            font-size: 13px;
            font-weight: bold;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #1084d8;
        }
    )";

    okStatusStyle_ = "QLabel { background-color: #00aa00; border-radius: 6px; min-width: 12px; max-width: 12px; min-height: 12px; max-height: 12px; }";
    ngStatusStyle_ = "QLabel { background-color: #ff0000; border-radius: 6px; min-width: 12px; max-width: 12px; min-height: 12px; max-height: 12px; }";
    unknownStatusStyle_ = "QLabel { background-color: #888888; border-radius: 6px; min-width: 12px; max-width: 12px; min-height: 12px; max-height: 12px; }";

    // 创建"全部"按钮
    btnShowAll_ = new QPushButton("全部", this);
    btnShowAll_->setStyleSheet(normalBtnStyle_);
    btnShowAll_->setToolTip("显示全部工位 (F7)");
    connect(btnShowAll_, &QPushButton::clicked, this, &StationSwitchBar::onShowAllClicked);
    layout_->addWidget(btnShowAll_);

    // 添加分隔线
    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::VLine);
    separator->setStyleSheet("QFrame { color: #555555; }");
    layout_->addWidget(separator);

    // 弹性空间
    layout_->addStretch();
}

QPushButton* StationSwitchBar::createStationButton(int index, const QString& name)
{
    QPushButton* btn = new QPushButton(name, this);
    btn->setStyleSheet(normalBtnStyle_);
    btn->setToolTip(QString("切换到 %1 (F%2)").arg(name).arg(index + 1));
    btn->setCheckable(true);

    buttonGroup_->addButton(btn, index);

    connect(btn, &QPushButton::clicked, [this, index]() {
        onStationButtonClicked(index);
    });

    return btn;
}

QLabel* StationSwitchBar::createStatusLabel()
{
    QLabel* label = new QLabel(this);
    label->setStyleSheet(unknownStatusStyle_);
    label->setFixedSize(12, 12);
    return label;
}

void StationSwitchBar::refreshStations()
{
    // 清除现有按钮（保留"全部"按钮和分隔线）
    for (auto* btn : stationBtns_) {
        layout_->removeWidget(btn);
        buttonGroup_->removeButton(btn);
        btn->deleteLater();
    }
    stationBtns_.clear();

    for (auto* label : statusLabels_) {
        layout_->removeWidget(label);
        label->deleteLater();
    }
    statusLabels_.clear();
    stationIds_.clear();

    // 获取工位列表
    auto& manager = Core::MultiStationManager::instance();
    auto stations = manager.getAllStations();

    // 移除末尾的弹性空间
    QLayoutItem* stretch = layout_->takeAt(layout_->count() - 1);
    delete stretch;

    // 创建工位按钮
    for (int i = 0; i < stations.size(); ++i) {
        const auto* station = stations[i];

        // 创建水平布局容器（按钮+状态灯）
        QWidget* container = new QWidget(this);
        QHBoxLayout* containerLayout = new QHBoxLayout(container);
        containerLayout->setSpacing(4);
        containerLayout->setContentsMargins(0, 0, 0, 0);

        // 创建按钮
        QPushButton* btn = createStationButton(i, station->name);
        stationBtns_.append(btn);
        stationIds_.append(station->id);
        containerLayout->addWidget(btn);

        // 创建状态指示灯
        if (showStatus_) {
            QLabel* statusLabel = createStatusLabel();
            statusLabels_.append(statusLabel);
            containerLayout->addWidget(statusLabel);
        }

        layout_->addWidget(container);
    }

    // 重新添加弹性空间
    layout_->addStretch();

    // 更新当前选中状态
    if (!stationBtns_.isEmpty()) {
        int currentIdx = manager.currentStationIndex();
        if (currentIdx >= 0 && currentIdx < stationBtns_.size()) {
            currentIndex_ = currentIdx;
        } else {
            currentIndex_ = 0;
        }
        updateButtonStates();
    }

    LOG_DEBUG(QString("StationSwitchBar: 刷新工位按钮，共 %1 个").arg(stations.size()));
}

void StationSwitchBar::setCurrentStation(int index)
{
    if (index >= 0 && index < stationBtns_.size() && index != currentIndex_) {
        currentIndex_ = index;
        updateButtonStates();

        emit stationSelected(index);
        if (index < stationIds_.size()) {
            emit stationSelectedById(stationIds_[index]);
        }
    }
}

void StationSwitchBar::setCurrentStation(const QString& stationId)
{
    int index = stationIds_.indexOf(stationId);
    if (index >= 0) {
        setCurrentStation(index);
    }
}

void StationSwitchBar::setShowAllButton(bool show)
{
    showAllButton_ = show;
    btnShowAll_->setVisible(show);
}

void StationSwitchBar::setShowStationStatus(bool show)
{
    showStatus_ = show;
    for (auto* label : statusLabels_) {
        label->setVisible(show);
    }
}

void StationSwitchBar::setStationStatus(int index, bool ok)
{
    if (index >= 0 && index < statusLabels_.size()) {
        statusLabels_[index]->setStyleSheet(ok ? okStatusStyle_ : ngStatusStyle_);
    }
}

void StationSwitchBar::clearAllStatus()
{
    for (auto* label : statusLabels_) {
        label->setStyleSheet(unknownStatusStyle_);
    }
}

void StationSwitchBar::installShortcuts(QWidget* parent)
{
    // F1-F6 切换工位
    for (int i = 0; i < 6; ++i) {
        QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_F1 + i), parent);
        connect(shortcut, &QShortcut::activated, [this, i]() {
            if (i < stationBtns_.size()) {
                setCurrentStation(i);
            }
        });
    }

    // F7 显示全部
    QShortcut* shortcutAll = new QShortcut(QKeySequence(Qt::Key_F7), parent);
    connect(shortcutAll, &QShortcut::activated, this, &StationSwitchBar::showAllStations);
}

void StationSwitchBar::onStationButtonClicked(int index)
{
    setCurrentStation(index);
}

void StationSwitchBar::onShowAllClicked()
{
    // 取消所有按钮的选中状态
    buttonGroup_->setExclusive(false);
    for (auto* btn : stationBtns_) {
        btn->setChecked(false);
        btn->setStyleSheet(normalBtnStyle_);
    }
    buttonGroup_->setExclusive(true);

    emit showAllStations();
}

void StationSwitchBar::updateButtonStates()
{
    for (int i = 0; i < stationBtns_.size(); ++i) {
        if (i == currentIndex_) {
            stationBtns_[i]->setStyleSheet(selectedBtnStyle_);
            stationBtns_[i]->setChecked(true);
        } else {
            stationBtns_[i]->setStyleSheet(normalBtnStyle_);
            stationBtns_[i]->setChecked(false);
        }
    }
}

} // namespace UI
} // namespace VisionForge
