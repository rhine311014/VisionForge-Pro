/**
 * @file MultiCameraManagerDialog.cpp
 * @brief 多相机管理对话框实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "ui/MultiCameraManagerDialog.h"
#include "ui/CameraConfigDialog.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QStatusBar>

namespace VisionForge {
namespace UI {

MultiCameraManagerDialog::MultiCameraManagerDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("多相机管理"));
    setMinimumSize(900, 600);

    setupUI();
    connectSignals();
    updateCameraTable();
    updateGroupList();
    updateStatusBar();
}

MultiCameraManagerDialog::~MultiCameraManagerDialog()
{
}

void MultiCameraManagerDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);

    // 标签页
    tabWidget_ = new QTabWidget();

    createCameraListTab();
    createGroupManageTab();
    createSettingsTab();

    tabWidget_->addTab(cameraListTab_, tr("相机列表"));
    tabWidget_->addTab(groupManageTab_, tr("分组管理"));
    tabWidget_->addTab(settingsTab_, tr("设置"));

    mainLayout->addWidget(tabWidget_);

    // 状态栏
    auto* statusLayout = new QHBoxLayout();
    statusLabel_ = new QLabel(tr("就绪"));
    cameraCountLabel_ = new QLabel(tr("相机数: 0"));
    connectedCountLabel_ = new QLabel(tr("已连接: 0"));

    statusLayout->addWidget(statusLabel_);
    statusLayout->addStretch();
    statusLayout->addWidget(cameraCountLabel_);
    statusLayout->addWidget(new QLabel("|"));
    statusLayout->addWidget(connectedCountLabel_);

    mainLayout->addLayout(statusLayout);

    // 底部按钮
    createButtonGroup();
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(saveConfigBtn_);
    btnLayout->addWidget(loadConfigBtn_);
    btnLayout->addStretch();
    btnLayout->addWidget(applyBtn_);
    btnLayout->addWidget(okBtn_);
    btnLayout->addWidget(cancelBtn_);

    mainLayout->addLayout(btnLayout);
}

void MultiCameraManagerDialog::createCameraListTab()
{
    cameraListTab_ = new QWidget();
    auto* layout = new QVBoxLayout(cameraListTab_);

    // 相机表格
    cameraTable_ = new QTableWidget();
    cameraTable_->setColumnCount(7);
    cameraTable_->setHorizontalHeaderLabels({
        tr("ID"), tr("名称"), tr("类型"), tr("序列号"),
        tr("分组"), tr("状态"), tr("启用")
    });
    cameraTable_->horizontalHeader()->setStretchLastSection(true);
    cameraTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    cameraTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    cameraTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cameraTable_->verticalHeader()->setVisible(false);

    connect(cameraTable_, &QTableWidget::itemSelectionChanged,
            this, &MultiCameraManagerDialog::onCameraSelectionChanged);

    layout->addWidget(cameraTable_);

    // 操作按钮
    auto* btnLayout = new QHBoxLayout();

    addCameraBtn_ = new QPushButton(tr("添加相机"));
    removeCameraBtn_ = new QPushButton(tr("删除相机"));
    editCameraBtn_ = new QPushButton(tr("编辑"));
    removeCameraBtn_->setEnabled(false);
    editCameraBtn_->setEnabled(false);

    connect(addCameraBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onAddCamera);
    connect(removeCameraBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onRemoveCamera);
    connect(editCameraBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onEditCamera);

    btnLayout->addWidget(addCameraBtn_);
    btnLayout->addWidget(removeCameraBtn_);
    btnLayout->addWidget(editCameraBtn_);
    btnLayout->addStretch();

    // 连接操作
    connectSelectedBtn_ = new QPushButton(tr("连接选中"));
    disconnectSelectedBtn_ = new QPushButton(tr("断开选中"));
    connectAllBtn_ = new QPushButton(tr("全部连接"));
    disconnectAllBtn_ = new QPushButton(tr("全部断开"));

    connect(connectSelectedBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onConnectSelected);
    connect(disconnectSelectedBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onDisconnectSelected);
    connect(connectAllBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onConnectAll);
    connect(disconnectAllBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onDisconnectAll);

    btnLayout->addWidget(connectSelectedBtn_);
    btnLayout->addWidget(disconnectSelectedBtn_);
    btnLayout->addWidget(connectAllBtn_);
    btnLayout->addWidget(disconnectAllBtn_);

    layout->addLayout(btnLayout);

    // 采集操作
    auto* captureLayout = new QHBoxLayout();
    captureSelectedBtn_ = new QPushButton(tr("采集选中"));
    captureAllBtn_ = new QPushButton(tr("批量采集"));

    connect(captureSelectedBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onCaptureSelected);
    connect(captureAllBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onCaptureAll);

    captureLayout->addWidget(captureSelectedBtn_);
    captureLayout->addWidget(captureAllBtn_);
    captureLayout->addStretch();

    layout->addLayout(captureLayout);
}

void MultiCameraManagerDialog::createGroupManageTab()
{
    groupManageTab_ = new QWidget();
    auto* layout = new QHBoxLayout(groupManageTab_);

    // 左侧：分组列表
    auto* leftWidget = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    auto* groupListGroup = new QGroupBox(tr("分组列表"));
    auto* groupListLayout = new QVBoxLayout(groupListGroup);

    groupList_ = new QListWidget();
    connect(groupList_, &QListWidget::itemSelectionChanged,
            this, &MultiCameraManagerDialog::onGroupSelectionChanged);
    groupListLayout->addWidget(groupList_);

    auto* groupBtnLayout = new QHBoxLayout();
    addGroupBtn_ = new QPushButton(tr("添加"));
    removeGroupBtn_ = new QPushButton(tr("删除"));
    editGroupBtn_ = new QPushButton(tr("编辑"));
    removeGroupBtn_->setEnabled(false);
    editGroupBtn_->setEnabled(false);

    connect(addGroupBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onAddGroup);
    connect(removeGroupBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onRemoveGroup);
    connect(editGroupBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onEditGroup);

    groupBtnLayout->addWidget(addGroupBtn_);
    groupBtnLayout->addWidget(removeGroupBtn_);
    groupBtnLayout->addWidget(editGroupBtn_);
    groupListLayout->addLayout(groupBtnLayout);

    leftLayout->addWidget(groupListGroup);

    // 分组设置
    auto* groupSettingsGroup = new QGroupBox(tr("分组设置"));
    auto* settingsLayout = new QGridLayout(groupSettingsGroup);

    settingsLayout->addWidget(new QLabel(tr("采集模式:")), 0, 0);
    groupCaptureModeCombo_ = new QComboBox();
    groupCaptureModeCombo_->addItem(tr("同时采集"), static_cast<int>(HAL::CaptureMode::Simultaneous));
    groupCaptureModeCombo_->addItem(tr("顺序采集"), static_cast<int>(HAL::CaptureMode::Sequential));
    groupCaptureModeCombo_->addItem(tr("独立采集"), static_cast<int>(HAL::CaptureMode::Independent));
    settingsLayout->addWidget(groupCaptureModeCombo_, 0, 1);

    settingsLayout->addWidget(new QLabel(tr("采集延迟(ms):")), 1, 0);
    groupDelaySpin_ = new QSpinBox();
    groupDelaySpin_->setRange(0, 10000);
    groupDelaySpin_->setValue(0);
    settingsLayout->addWidget(groupDelaySpin_, 1, 1);

    leftLayout->addWidget(groupSettingsGroup);

    layout->addWidget(leftWidget, 1);

    // 右侧：组内相机
    auto* rightWidget = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    auto* groupCameraGroup = new QGroupBox(tr("组内相机"));
    auto* groupCameraLayout = new QVBoxLayout(groupCameraGroup);

    groupCameraTable_ = new QTableWidget();
    groupCameraTable_->setColumnCount(4);
    groupCameraTable_->setHorizontalHeaderLabels({
        tr("ID"), tr("名称"), tr("类型"), tr("状态")
    });
    groupCameraTable_->horizontalHeader()->setStretchLastSection(true);
    groupCameraTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    groupCameraTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    groupCameraTable_->verticalHeader()->setVisible(false);

    groupCameraLayout->addWidget(groupCameraTable_);

    auto* groupCameraBtnLayout = new QHBoxLayout();
    addToGroupBtn_ = new QPushButton(tr("添加到组"));
    removeFromGroupBtn_ = new QPushButton(tr("从组移除"));

    connect(addToGroupBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onAddCameraToGroup);
    connect(removeFromGroupBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onRemoveCameraFromGroup);

    groupCameraBtnLayout->addWidget(addToGroupBtn_);
    groupCameraBtnLayout->addWidget(removeFromGroupBtn_);
    groupCameraBtnLayout->addStretch();
    groupCameraLayout->addLayout(groupCameraBtnLayout);

    rightLayout->addWidget(groupCameraGroup);

    layout->addWidget(rightWidget, 2);
}

void MultiCameraManagerDialog::createSettingsTab()
{
    settingsTab_ = new QWidget();
    auto* layout = new QVBoxLayout(settingsTab_);

    auto* settingsGroup = new QGroupBox(tr("默认设置"));
    auto* settingsLayout = new QGridLayout(settingsGroup);

    int row = 0;

    // 默认采集模式
    settingsLayout->addWidget(new QLabel(tr("默认采集模式:")), row, 0);
    defaultCaptureModeCombo_ = new QComboBox();
    defaultCaptureModeCombo_->addItem(tr("同时采集"), static_cast<int>(HAL::CaptureMode::Simultaneous));
    defaultCaptureModeCombo_->addItem(tr("顺序采集"), static_cast<int>(HAL::CaptureMode::Sequential));
    defaultCaptureModeCombo_->addItem(tr("独立采集"), static_cast<int>(HAL::CaptureMode::Independent));
    settingsLayout->addWidget(defaultCaptureModeCombo_, row, 1);
    row++;

    // 默认超时时间
    settingsLayout->addWidget(new QLabel(tr("采集超时(ms):")), row, 0);
    defaultTimeoutSpin_ = new QSpinBox();
    defaultTimeoutSpin_->setRange(100, 30000);
    defaultTimeoutSpin_->setValue(3000);
    settingsLayout->addWidget(defaultTimeoutSpin_, row, 1);
    row++;

    // 连续采集帧率
    settingsLayout->addWidget(new QLabel(tr("连续采集FPS:")), row, 0);
    continuousFpsSpin_ = new QSpinBox();
    continuousFpsSpin_->setRange(1, 120);
    continuousFpsSpin_->setValue(10);
    settingsLayout->addWidget(continuousFpsSpin_, row, 1);

    layout->addWidget(settingsGroup);
    layout->addStretch();
}

void MultiCameraManagerDialog::createButtonGroup()
{
    saveConfigBtn_ = new QPushButton(tr("保存配置"));
    loadConfigBtn_ = new QPushButton(tr("加载配置"));
    applyBtn_ = new QPushButton(tr("应用"));
    okBtn_ = new QPushButton(tr("确定"));
    cancelBtn_ = new QPushButton(tr("取消"));

    connect(saveConfigBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onSaveConfig);
    connect(loadConfigBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onLoadConfig);
    connect(applyBtn_, &QPushButton::clicked, this, &MultiCameraManagerDialog::onApply);
    connect(okBtn_, &QPushButton::clicked, [this]() {
        onApply();
        accept();
    });
    connect(cancelBtn_, &QPushButton::clicked, this, &QDialog::reject);
}

void MultiCameraManagerDialog::connectSignals()
{
    auto& mgr = HAL::CameraManager::instance();

    connect(&mgr, &HAL::CameraManager::cameraAdded,
            this, &MultiCameraManagerDialog::onCameraAdded);
    connect(&mgr, &HAL::CameraManager::cameraRemoved,
            this, &MultiCameraManagerDialog::onCameraRemoved);
    connect(&mgr, &HAL::CameraManager::cameraConnected,
            this, &MultiCameraManagerDialog::onCameraConnected);
    connect(&mgr, &HAL::CameraManager::cameraDisconnected,
            this, &MultiCameraManagerDialog::onCameraDisconnected);
    connect(&mgr, &HAL::CameraManager::cameraError,
            this, &MultiCameraManagerDialog::onCameraError);
}

void MultiCameraManagerDialog::updateCameraTable()
{
    cameraTable_->setRowCount(0);

    auto& mgr = HAL::CameraManager::instance();
    QStringList cameraIds = mgr.getAllCameraIds();

    for (int i = 0; i < cameraIds.size(); ++i) {
        const QString& id = cameraIds[i];
        HAL::CameraInfo info = mgr.getCameraInfo(id);
        bool connected = mgr.isCameraConnected(id);

        cameraTable_->insertRow(i);
        cameraTable_->setItem(i, 0, new QTableWidgetItem(id));
        cameraTable_->setItem(i, 1, new QTableWidgetItem(info.name));
        cameraTable_->setItem(i, 2, new QTableWidgetItem(getCameraTypeText(info.type)));
        cameraTable_->setItem(i, 3, new QTableWidgetItem(info.deviceInfo.serialNumber));
        cameraTable_->setItem(i, 4, new QTableWidgetItem(info.groupId));
        cameraTable_->setItem(i, 5, new QTableWidgetItem(connected ? tr("已连接") : tr("未连接")));
        cameraTable_->setItem(i, 6, new QTableWidgetItem(info.enabled ? tr("是") : tr("否")));

        // 设置状态颜色
        QTableWidgetItem* statusItem = cameraTable_->item(i, 5);
        if (connected) {
            statusItem->setForeground(QBrush(Qt::green));
        } else {
            statusItem->setForeground(QBrush(Qt::gray));
        }
    }

    cameraTable_->resizeColumnsToContents();
}

void MultiCameraManagerDialog::updateGroupList()
{
    groupList_->clear();

    auto& mgr = HAL::CameraManager::instance();
    QList<HAL::CameraGroup> groups = mgr.getAllGroups();

    for (const auto& group : groups) {
        QListWidgetItem* item = new QListWidgetItem(
            QString("%1 (%2)").arg(group.name).arg(group.cameraIds.size()));
        item->setData(Qt::UserRole, group.id);
        groupList_->addItem(item);
    }
}

void MultiCameraManagerDialog::updateGroupCameraList()
{
    groupCameraTable_->setRowCount(0);

    if (selectedGroupId_.isEmpty()) return;

    auto& mgr = HAL::CameraManager::instance();
    HAL::CameraGroup group = mgr.getGroup(selectedGroupId_);

    for (int i = 0; i < group.cameraIds.size(); ++i) {
        const QString& cameraId = group.cameraIds[i];
        HAL::CameraInfo info = mgr.getCameraInfo(cameraId);
        bool connected = mgr.isCameraConnected(cameraId);

        groupCameraTable_->insertRow(i);
        groupCameraTable_->setItem(i, 0, new QTableWidgetItem(cameraId));
        groupCameraTable_->setItem(i, 1, new QTableWidgetItem(info.name));
        groupCameraTable_->setItem(i, 2, new QTableWidgetItem(getCameraTypeText(info.type)));
        groupCameraTable_->setItem(i, 3, new QTableWidgetItem(connected ? tr("已连接") : tr("未连接")));
    }

    // 更新分组设置显示
    int modeIndex = groupCaptureModeCombo_->findData(static_cast<int>(group.captureMode));
    if (modeIndex >= 0) {
        groupCaptureModeCombo_->setCurrentIndex(modeIndex);
    }
    groupDelaySpin_->setValue(group.captureDelayMs);

    groupCameraTable_->resizeColumnsToContents();
}

void MultiCameraManagerDialog::updateStatusBar()
{
    auto& mgr = HAL::CameraManager::instance();
    int total = mgr.cameraCount();
    int connected = mgr.connectedCameraCount();

    cameraCountLabel_->setText(tr("相机数: %1").arg(total));
    connectedCountLabel_->setText(tr("已连接: %1").arg(connected));
}

QString MultiCameraManagerDialog::getCaptureModeText(HAL::CaptureMode mode) const
{
    switch (mode) {
        case HAL::CaptureMode::Simultaneous: return tr("同时采集");
        case HAL::CaptureMode::Sequential: return tr("顺序采集");
        case HAL::CaptureMode::Independent: return tr("独立采集");
        default: return tr("未知");
    }
}

QString MultiCameraManagerDialog::getCameraTypeText(HAL::CameraFactory::CameraType type) const
{
    switch (type) {
        case HAL::CameraFactory::Simulated: return tr("模拟相机");
        case HAL::CameraFactory::Hikvision: return tr("海康威视");
        case HAL::CameraFactory::Basler: return tr("Basler");
        default: return tr("未知");
    }
}

// ========== 相机管理槽函数 ==========

void MultiCameraManagerDialog::onAddCamera()
{
    CameraConfigDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        HAL::ICamera* camera = dialog.takeCamera();
        if (camera) {
            // 创建相机信息
            HAL::CameraInfo info;
            info.name = camera->deviceName();
            info.type = HAL::CameraFactory::Simulated;  // TODO: 从dialog获取类型
            info.deviceInfo.modelName = camera->deviceName();
            info.config = camera->getConfig();
            info.enabled = true;

            auto& mgr = HAL::CameraManager::instance();
            if (mgr.addCamera(info)) {
                statusLabel_->setText(tr("相机已添加: %1").arg(info.name));
            } else {
                QMessageBox::warning(this, tr("错误"), tr("添加相机失败"));
            }

            delete camera;
        }
    }
}

void MultiCameraManagerDialog::onRemoveCamera()
{
    if (selectedCameraId_.isEmpty()) return;

    int ret = QMessageBox::question(this, tr("确认"),
        tr("确定要删除相机 %1 吗?").arg(selectedCameraId_),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        auto& mgr = HAL::CameraManager::instance();
        if (mgr.removeCamera(selectedCameraId_)) {
            statusLabel_->setText(tr("相机已删除"));
            selectedCameraId_.clear();
        }
    }
}

void MultiCameraManagerDialog::onEditCamera()
{
    if (selectedCameraId_.isEmpty()) return;

    auto& mgr = HAL::CameraManager::instance();
    HAL::CameraInfo info = mgr.getCameraInfo(selectedCameraId_);

    bool ok;
    QString newName = QInputDialog::getText(this, tr("编辑相机"),
        tr("相机名称:"), QLineEdit::Normal, info.name, &ok);

    if (ok && !newName.isEmpty()) {
        info.name = newName;
        mgr.updateCameraInfo(selectedCameraId_, info);
        updateCameraTable();
    }
}

void MultiCameraManagerDialog::onCameraSelectionChanged()
{
    int row = cameraTable_->currentRow();
    bool hasSelection = (row >= 0);

    removeCameraBtn_->setEnabled(hasSelection);
    editCameraBtn_->setEnabled(hasSelection);
    connectSelectedBtn_->setEnabled(hasSelection);
    disconnectSelectedBtn_->setEnabled(hasSelection);
    captureSelectedBtn_->setEnabled(hasSelection);

    if (hasSelection) {
        selectedCameraId_ = cameraTable_->item(row, 0)->text();
    } else {
        selectedCameraId_.clear();
    }
}

void MultiCameraManagerDialog::onConnectSelected()
{
    if (selectedCameraId_.isEmpty()) return;

    auto& mgr = HAL::CameraManager::instance();
    if (mgr.connectCamera(selectedCameraId_)) {
        statusLabel_->setText(tr("相机已连接: %1").arg(selectedCameraId_));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("连接相机失败"));
    }
}

void MultiCameraManagerDialog::onDisconnectSelected()
{
    if (selectedCameraId_.isEmpty()) return;

    auto& mgr = HAL::CameraManager::instance();
    mgr.disconnectCamera(selectedCameraId_);
    statusLabel_->setText(tr("相机已断开: %1").arg(selectedCameraId_));
}

void MultiCameraManagerDialog::onConnectAll()
{
    auto& mgr = HAL::CameraManager::instance();
    mgr.connectAll();
    statusLabel_->setText(tr("已连接所有相机"));
}

void MultiCameraManagerDialog::onDisconnectAll()
{
    auto& mgr = HAL::CameraManager::instance();
    mgr.disconnectAll();
    statusLabel_->setText(tr("已断开所有相机"));
}

void MultiCameraManagerDialog::onCaptureSelected()
{
    if (selectedCameraId_.isEmpty()) return;

    auto& mgr = HAL::CameraManager::instance();
    HAL::CaptureResult result = mgr.capture(selectedCameraId_, defaultTimeoutSpin_->value());

    if (result.success) {
        statusLabel_->setText(tr("采集成功: %1 (耗时: %2ms)")
            .arg(selectedCameraId_).arg(result.captureTime, 0, 'f', 1));
    } else {
        QMessageBox::warning(this, tr("错误"),
            tr("采集失败: %1").arg(result.errorMessage));
    }
}

void MultiCameraManagerDialog::onCaptureAll()
{
    auto& mgr = HAL::CameraManager::instance();
    auto mode = static_cast<HAL::CaptureMode>(defaultCaptureModeCombo_->currentData().toInt());
    HAL::BatchCaptureResult result = mgr.captureAll(mode, defaultTimeoutSpin_->value());

    statusLabel_->setText(tr("批量采集完成: 成功 %1, 失败 %2 (耗时: %3ms)")
        .arg(result.successCount).arg(result.failCount).arg(result.totalTime, 0, 'f', 1));

    if (!result.allSuccess) {
        QString failedList;
        for (auto it = result.results.begin(); it != result.results.end(); ++it) {
            if (!it.value().success) {
                failedList += QString("\n- %1: %2").arg(it.key()).arg(it.value().errorMessage);
            }
        }
        QMessageBox::warning(this, tr("部分失败"),
            tr("以下相机采集失败:") + failedList);
    }
}

// ========== 分组管理槽函数 ==========

void MultiCameraManagerDialog::onAddGroup()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("添加分组"),
        tr("分组名称:"), QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        HAL::CameraGroup group;
        group.name = name;
        group.captureMode = HAL::CaptureMode::Simultaneous;
        group.captureDelayMs = 0;

        auto& mgr = HAL::CameraManager::instance();
        if (mgr.createGroup(group)) {
            updateGroupList();
            statusLabel_->setText(tr("分组已创建: %1").arg(name));
        } else {
            QMessageBox::warning(this, tr("错误"), tr("创建分组失败"));
        }
    }
}

void MultiCameraManagerDialog::onRemoveGroup()
{
    if (selectedGroupId_.isEmpty()) return;

    int ret = QMessageBox::question(this, tr("确认"),
        tr("确定要删除分组吗?"),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        auto& mgr = HAL::CameraManager::instance();
        if (mgr.deleteGroup(selectedGroupId_)) {
            updateGroupList();
            selectedGroupId_.clear();
            groupCameraTable_->setRowCount(0);
            statusLabel_->setText(tr("分组已删除"));
        }
    }
}

void MultiCameraManagerDialog::onEditGroup()
{
    if (selectedGroupId_.isEmpty()) return;

    auto& mgr = HAL::CameraManager::instance();
    HAL::CameraGroup group = mgr.getGroup(selectedGroupId_);

    bool ok;
    QString newName = QInputDialog::getText(this, tr("编辑分组"),
        tr("分组名称:"), QLineEdit::Normal, group.name, &ok);

    if (ok && !newName.isEmpty()) {
        group.name = newName;
        group.captureMode = static_cast<HAL::CaptureMode>(
            groupCaptureModeCombo_->currentData().toInt());
        group.captureDelayMs = groupDelaySpin_->value();

        mgr.updateGroup(selectedGroupId_, group);
        updateGroupList();
    }
}

void MultiCameraManagerDialog::onGroupSelectionChanged()
{
    QListWidgetItem* item = groupList_->currentItem();
    bool hasSelection = (item != nullptr);

    removeGroupBtn_->setEnabled(hasSelection);
    editGroupBtn_->setEnabled(hasSelection);
    addToGroupBtn_->setEnabled(hasSelection);
    removeFromGroupBtn_->setEnabled(hasSelection);

    if (hasSelection) {
        selectedGroupId_ = item->data(Qt::UserRole).toString();
        updateGroupCameraList();
    } else {
        selectedGroupId_.clear();
        groupCameraTable_->setRowCount(0);
    }
}

void MultiCameraManagerDialog::onAddCameraToGroup()
{
    if (selectedGroupId_.isEmpty()) return;

    auto& mgr = HAL::CameraManager::instance();
    QStringList allCameras = mgr.getAllCameraIds();
    HAL::CameraGroup group = mgr.getGroup(selectedGroupId_);

    // 过滤出不在组内的相机
    QStringList availableCameras;
    for (const QString& id : allCameras) {
        if (!group.cameraIds.contains(id)) {
            HAL::CameraInfo info = mgr.getCameraInfo(id);
            availableCameras.append(QString("%1 (%2)").arg(id).arg(info.name));
        }
    }

    if (availableCameras.isEmpty()) {
        QMessageBox::information(this, tr("提示"), tr("没有可添加的相机"));
        return;
    }

    bool ok;
    QString selected = QInputDialog::getItem(this, tr("添加相机到分组"),
        tr("选择相机:"), availableCameras, 0, false, &ok);

    if (ok && !selected.isEmpty()) {
        QString cameraId = selected.split(" ").first();
        if (mgr.addCameraToGroup(cameraId, selectedGroupId_)) {
            updateGroupCameraList();
            updateCameraTable();
            statusLabel_->setText(tr("相机已添加到分组"));
        }
    }
}

void MultiCameraManagerDialog::onRemoveCameraFromGroup()
{
    if (selectedGroupId_.isEmpty()) return;

    int row = groupCameraTable_->currentRow();
    if (row < 0) return;

    QString cameraId = groupCameraTable_->item(row, 0)->text();

    auto& mgr = HAL::CameraManager::instance();
    if (mgr.removeCameraFromGroup(cameraId, selectedGroupId_)) {
        updateGroupCameraList();
        updateCameraTable();
        statusLabel_->setText(tr("相机已从分组移除"));
    }
}

// ========== 配置管理槽函数 ==========

void MultiCameraManagerDialog::onSaveConfig()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("保存配置"), "", tr("JSON文件 (*.json)"));

    if (!filePath.isEmpty()) {
        auto& mgr = HAL::CameraManager::instance();
        if (mgr.saveConfig(filePath)) {
            statusLabel_->setText(tr("配置已保存"));
        } else {
            QMessageBox::warning(this, tr("错误"), tr("保存配置失败"));
        }
    }
}

void MultiCameraManagerDialog::onLoadConfig()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("加载配置"), "", tr("JSON文件 (*.json)"));

    if (!filePath.isEmpty()) {
        auto& mgr = HAL::CameraManager::instance();
        if (mgr.loadConfig(filePath)) {
            updateCameraTable();
            updateGroupList();
            updateStatusBar();
            statusLabel_->setText(tr("配置已加载"));
        } else {
            QMessageBox::warning(this, tr("错误"), tr("加载配置失败"));
        }
    }
}

void MultiCameraManagerDialog::onApply()
{
    // 保存当前分组设置
    if (!selectedGroupId_.isEmpty()) {
        auto& mgr = HAL::CameraManager::instance();
        HAL::CameraGroup group = mgr.getGroup(selectedGroupId_);
        group.captureMode = static_cast<HAL::CaptureMode>(
            groupCaptureModeCombo_->currentData().toInt());
        group.captureDelayMs = groupDelaySpin_->value();
        mgr.updateGroup(selectedGroupId_, group);
    }

    emit configurationChanged();
    statusLabel_->setText(tr("设置已应用"));
}

// ========== 相机管理器信号响应 ==========

void MultiCameraManagerDialog::onCameraAdded(const QString& cameraId)
{
    Q_UNUSED(cameraId)
    updateCameraTable();
    updateStatusBar();
}

void MultiCameraManagerDialog::onCameraRemoved(const QString& cameraId)
{
    Q_UNUSED(cameraId)
    updateCameraTable();
    updateGroupList();
    updateGroupCameraList();
    updateStatusBar();
}

void MultiCameraManagerDialog::onCameraConnected(const QString& cameraId)
{
    Q_UNUSED(cameraId)
    updateCameraTable();
    updateGroupCameraList();
    updateStatusBar();
}

void MultiCameraManagerDialog::onCameraDisconnected(const QString& cameraId)
{
    Q_UNUSED(cameraId)
    updateCameraTable();
    updateGroupCameraList();
    updateStatusBar();
}

void MultiCameraManagerDialog::onCameraError(const QString& cameraId, const QString& error)
{
    statusLabel_->setText(tr("相机错误 [%1]: %2").arg(cameraId).arg(error));
    LOG_ERROR(QString("相机错误 [%1]: %2").arg(cameraId).arg(error));
}

} // namespace UI
} // namespace VisionForge
