/**
 * @file SceneSwitchBar.cpp
 * @brief 场景切换工具栏实现
 */

#include "ui/SceneSwitchBar.h"
#include "core/SceneManager.h"
#include "core/StationConfig.h"
#include "base/Logger.h"

namespace VisionForge {
namespace UI {

SceneSwitchBar::SceneSwitchBar(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

SceneSwitchBar::~SceneSwitchBar()
{
}

void SceneSwitchBar::setupUI()
{
    layout_ = new QHBoxLayout(this);
    layout_->setContentsMargins(5, 2, 5, 2);
    layout_->setSpacing(5);

    // 场景标签
    labelScene_ = new QLabel("场景:", this);
    layout_->addWidget(labelScene_);

    // 上一个按钮
    btnPrevious_ = new QPushButton("<", this);
    btnPrevious_->setFixedWidth(30);
    btnPrevious_->setToolTip("上一个场景");
    layout_->addWidget(btnPrevious_);

    // 场景下拉框
    comboScene_ = new QComboBox(this);
    comboScene_->setMinimumWidth(120);
    comboScene_->setToolTip("选择检测场景");
    layout_->addWidget(comboScene_);

    // 下一个按钮
    btnNext_ = new QPushButton(">", this);
    btnNext_->setFixedWidth(30);
    btnNext_->setToolTip("下一个场景");
    layout_->addWidget(btnNext_);

    // 状态标签
    labelStatus_ = new QLabel(this);
    labelStatus_->setMinimumWidth(100);
    layout_->addWidget(labelStatus_);

    // 弹性空间
    layout_->addStretch();

    // 连接信号
    connect(btnPrevious_, &QPushButton::clicked, this, &SceneSwitchBar::onPreviousClicked);
    connect(btnNext_, &QPushButton::clicked, this, &SceneSwitchBar::onNextClicked);
    connect(comboScene_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SceneSwitchBar::onSceneComboChanged);

    updateButtonStates();
}

void SceneSwitchBar::setSceneManager(Core::SceneManager* manager)
{
    // 断开旧的连接
    if (sceneManager_) {
        disconnect(sceneManager_, nullptr, this, nullptr);
    }

    sceneManager_ = manager;

    if (sceneManager_) {
        connect(sceneManager_, &Core::SceneManager::sceneChanged,
                this, &SceneSwitchBar::onSceneChanged);
        connect(sceneManager_, &Core::SceneManager::sceneLoadProgress,
                this, &SceneSwitchBar::onSceneLoadProgress);
    }
}

void SceneSwitchBar::refreshScenes(Core::StationConfig* stationConfig)
{
    isUpdating_ = true;

    sceneIds_.clear();
    sceneNames_.clear();
    comboScene_->clear();

    if (!stationConfig) {
        isUpdating_ = false;
        updateButtonStates();
        return;
    }

    // 获取场景列表
    for (const auto& scene : stationConfig->scenes) {
        sceneIds_.append(scene.sceneId);
        sceneNames_.append(scene.sceneName);

        QString displayText = scene.sceneName;
        if (!scene.enabled) {
            displayText += " (禁用)";
        }
        comboScene_->addItem(displayText);
    }

    // 设置当前场景
    currentIndex_ = stationConfig->currentSceneIndex;
    if (currentIndex_ >= 0 && currentIndex_ < comboScene_->count()) {
        comboScene_->setCurrentIndex(currentIndex_);
    }

    isUpdating_ = false;
    updateButtonStates();

    LOG_DEBUG(QString("[SceneSwitchBar] 刷新场景列表, 共 %1 个场景").arg(sceneNames_.size()));
}

void SceneSwitchBar::setCurrentScene(int index)
{
    if (index < 0 || index >= sceneNames_.size()) {
        return;
    }

    isUpdating_ = true;
    currentIndex_ = index;
    comboScene_->setCurrentIndex(index);
    isUpdating_ = false;

    updateButtonStates();
}

void SceneSwitchBar::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
    comboScene_->setEnabled(enabled);
    btnPrevious_->setEnabled(enabled && currentIndex_ > 0);
    btnNext_->setEnabled(enabled && currentIndex_ < sceneNames_.size() - 1);
}

void SceneSwitchBar::onSceneComboChanged(int index)
{
    if (isUpdating_ || index < 0 || index >= sceneIds_.size()) {
        return;
    }

    currentIndex_ = index;
    updateButtonStates();

    emit sceneSelected(index);
    emit sceneSelectedById(sceneIds_[index]);

    LOG_DEBUG(QString("[SceneSwitchBar] 场景选择变更: %1 %2").arg(index).arg(sceneNames_[index]));
}

void SceneSwitchBar::onPreviousClicked()
{
    if (currentIndex_ > 0) {
        emit previousSceneRequested();
    }
}

void SceneSwitchBar::onNextClicked()
{
    if (currentIndex_ < sceneNames_.size() - 1) {
        emit nextSceneRequested();
    }
}

void SceneSwitchBar::onSceneChanged(int sceneIndex, const QString& sceneName)
{
    setCurrentScene(sceneIndex);
    labelStatus_->setText(QString("当前: %1").arg(sceneName));
    labelStatus_->setStyleSheet("color: green;");
}

void SceneSwitchBar::onSceneLoadProgress(int progress, const QString& message)
{
    if (progress < 100) {
        labelStatus_->setText(QString("%1 (%2%)").arg(message).arg(progress));
        labelStatus_->setStyleSheet("color: orange;");
    }
}

void SceneSwitchBar::updateButtonStates()
{
    int count = sceneNames_.size();
    btnPrevious_->setEnabled(count > 1 && currentIndex_ > 0);
    btnNext_->setEnabled(count > 1 && currentIndex_ < count - 1);
    comboScene_->setEnabled(count > 0);

    if (count == 0) {
        labelStatus_->setText("无场景");
        labelStatus_->setStyleSheet("color: gray;");
    }
}

} // namespace UI
} // namespace VisionForge
