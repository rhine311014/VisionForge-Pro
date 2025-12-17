/**
 * @file CameraConfigDialog.h
 * @brief 相机配置对话框
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QDialog>
#include <QTimer>
#include <opencv2/core.hpp>
#include "hal/ICamera.h"
#include "hal/CameraFactory.h"

class QLabel;
class QComboBox;
class QPushButton;
class QDoubleSpinBox;
class QSpinBox;
class QSlider;
class QGroupBox;
class QTableWidget;
class QCheckBox;

namespace VisionForge {

/**
 * @brief 相机配置对话框
 *
 * 提供相机选择、参数配置和预览功能
 */
class CameraConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit CameraConfigDialog(QWidget* parent = nullptr);
    ~CameraConfigDialog() override;

    /**
     * @brief 获取选择的相机
     * @return 相机指针（所有权转移给调用者）
     */
    HAL::ICamera* takeCamera();

    /**
     * @brief 设置当前相机
     * @param camera 相机指针
     */
    void setCamera(HAL::ICamera* camera);

signals:
    /**
     * @brief 相机已选择信号
     * @param camera 相机指针
     */
    void cameraSelected(HAL::ICamera* camera);

private slots:
    void onRefreshDevices();
    void onDeviceSelectionChanged();
    void onConnectCamera();
    void onDisconnectCamera();
    void onStartPreview();
    void onStopPreview();
    void onExposureChanged(double value);
    void onGainChanged(double value);
    void onTriggerModeChanged(int index);
    void onPreviewTimer();
    void onApplySettings();
    void onCameraError(const QString& error);

private:
    void setupUI();
    void createDeviceListGroup();
    void createParameterGroup();
    void createTransformGroup();
    void createPreviewGroup();
    void createButtonGroup();
    void updateDeviceList();
    void updateParameterRanges();
    void updatePreviewImage(Base::ImageData::Ptr image);
    void setControlsEnabled(bool enabled);
    cv::Mat applyTransform(const cv::Mat& src);

private:
    // 设备列表
    QGroupBox* deviceGroup_;
    QTableWidget* deviceTable_;
    QPushButton* refreshBtn_;
    QComboBox* cameraTypeCombo_;

    // 参数配置
    QGroupBox* paramGroup_;
    QDoubleSpinBox* exposureSpin_;
    QSlider* exposureSlider_;
    QDoubleSpinBox* gainSpin_;
    QSlider* gainSlider_;
    QComboBox* triggerModeCombo_;
    QSpinBox* widthSpin_;
    QSpinBox* heightSpin_;
    QSpinBox* offsetXSpin_;
    QSpinBox* offsetYSpin_;

    // 图像变换
    QGroupBox* transformGroup_;
    QComboBox* rotationCombo_;
    QCheckBox* flipHorizontalCheck_;
    QCheckBox* flipVerticalCheck_;

    // 预览
    QGroupBox* previewGroup_;
    QLabel* previewLabel_;
    QPushButton* startPreviewBtn_;
    QPushButton* stopPreviewBtn_;
    QLabel* fpsLabel_;
    QCheckBox* autoExposureCheck_;

    // 按钮
    QPushButton* connectBtn_;
    QPushButton* disconnectBtn_;
    QPushButton* applyBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;

    // 相机和状态
    HAL::ICamera* camera_;
    bool ownsCamera_;
    QTimer* previewTimer_;
    int frameCount_;
    qint64 lastFpsTime_;

    // 设备信息缓存
    QList<HAL::GenericDeviceInfo> deviceList_;

    // 图像变换设置
    int rotationAngle_;         // 旋转角度: 0, 90, 180, 270
    bool flipHorizontal_;       // 水平镜像
    bool flipVertical_;         // 垂直镜像
};

} // namespace VisionForge
