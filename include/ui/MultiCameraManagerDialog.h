/**
 * @file MultiCameraManagerDialog.h
 * @brief 多相机管理对话框
 * @details 提供多相机配置、分组和批量操作界面
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QDialog>
#include <QMap>
#include "hal/CameraManager.h"

class QTableWidget;
class QComboBox;
class QPushButton;
class QSpinBox;
class QGroupBox;
class QLabel;
class QListWidget;
class QTabWidget;
class QLineEdit;

namespace VisionForge {
namespace UI {

/**
 * @class MultiCameraManagerDialog
 * @brief 多相机管理对话框
 *
 * 功能：
 * - 显示和管理多个相机
 * - 相机分组管理
 * - 批量连接/断开/采集
 * - 采集模式配置
 */
class MultiCameraManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiCameraManagerDialog(QWidget* parent = nullptr);
    ~MultiCameraManagerDialog() override;

signals:
    /**
     * @brief 配置已更改信号
     */
    void configurationChanged();

private slots:
    // 相机管理
    void onAddCamera();
    void onRemoveCamera();
    void onEditCamera();
    void onCameraSelectionChanged();

    // 连接操作
    void onConnectSelected();
    void onDisconnectSelected();
    void onConnectAll();
    void onDisconnectAll();

    // 采集操作
    void onCaptureSelected();
    void onCaptureAll();

    // 分组管理
    void onAddGroup();
    void onRemoveGroup();
    void onEditGroup();
    void onGroupSelectionChanged();
    void onAddCameraToGroup();
    void onRemoveCameraFromGroup();

    // 配置
    void onSaveConfig();
    void onLoadConfig();
    void onApply();

    // 相机管理器信号响应
    void onCameraAdded(const QString& cameraId);
    void onCameraRemoved(const QString& cameraId);
    void onCameraConnected(const QString& cameraId);
    void onCameraDisconnected(const QString& cameraId);
    void onCameraError(const QString& cameraId, const QString& error);

private:
    void setupUI();
    void createCameraListTab();
    void createGroupManageTab();
    void createSettingsTab();
    void createButtonGroup();
    void connectSignals();

    void updateCameraTable();
    void updateGroupList();
    void updateGroupCameraList();
    void updateStatusBar();

    QString getCaptureModeText(HAL::CaptureMode mode) const;
    QString getCameraTypeText(HAL::CameraFactory::CameraType type) const;

private:
    // 标签页
    QTabWidget* tabWidget_;

    // 相机列表页
    QWidget* cameraListTab_;
    QTableWidget* cameraTable_;
    QPushButton* addCameraBtn_;
    QPushButton* removeCameraBtn_;
    QPushButton* editCameraBtn_;
    QPushButton* connectSelectedBtn_;
    QPushButton* disconnectSelectedBtn_;
    QPushButton* connectAllBtn_;
    QPushButton* disconnectAllBtn_;
    QPushButton* captureSelectedBtn_;
    QPushButton* captureAllBtn_;

    // 分组管理页
    QWidget* groupManageTab_;
    QListWidget* groupList_;
    QTableWidget* groupCameraTable_;
    QPushButton* addGroupBtn_;
    QPushButton* removeGroupBtn_;
    QPushButton* editGroupBtn_;
    QPushButton* addToGroupBtn_;
    QPushButton* removeFromGroupBtn_;
    QComboBox* groupCaptureModeCombo_;
    QSpinBox* groupDelaySpin_;

    // 设置页
    QWidget* settingsTab_;
    QComboBox* defaultCaptureModeCombo_;
    QSpinBox* defaultTimeoutSpin_;
    QSpinBox* continuousFpsSpin_;

    // 底部按钮
    QPushButton* saveConfigBtn_;
    QPushButton* loadConfigBtn_;
    QPushButton* applyBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;

    // 状态栏
    QLabel* statusLabel_;
    QLabel* cameraCountLabel_;
    QLabel* connectedCountLabel_;

    // 当前选中
    QString selectedCameraId_;
    QString selectedGroupId_;
};

} // namespace UI
} // namespace VisionForge
