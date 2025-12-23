/**
 * @file CameraConfigDialog.h
 * @brief 相机配置对话框
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QDialog>
#include <QTimer>
#include <QButtonGroup>
#include <opencv2/core.hpp>
#include "hal/ICamera.h"
#include "hal/CameraFactory.h"

class QLabel;
class QComboBox;
class QPushButton;
class QToolButton;
class QDoubleSpinBox;
class QSpinBox;
class QSlider;
class QGroupBox;
class QCheckBox;
class QRadioButton;
class QVBoxLayout;

namespace VisionForge {

/**
 * @brief 相机配置对话框
 *
 * 提供相机参数配置和预览功能
 * 注：设备选择已移至CameraSetupDialog
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
    void onPositionChanged(int id);
    void onStartCapture();
    void onExposureChanged(double value);
    void onGainChanged(double value);
    void onGammaChanged(double value);
    void onGammaEnabled(bool enabled);
    void onLightSettings();
    void onTriggerSettings();
    void onRotationChanged(int id);
    void onCrosslineEnabled(bool enabled);
    void onCrosslineCenterClicked();
    void onPreviewTimer();
    void onCameraError(const QString& error);

private:
    void setupUI();
    void createPreviewArea(QWidget* parent);
    void createControlPanel(QWidget* parent);
    void createPositionGroup(QVBoxLayout* layout);
    void createParameterGroup(QVBoxLayout* layout);
    void createDisplayGroup(QVBoxLayout* layout);
    void createCrosslineGroup(QVBoxLayout* layout);
    void createSideButtons(QWidget* parent);
    void updateParameterRanges();
    void updatePreviewImage(Base::ImageData::Ptr image);
    void setControlsEnabled(bool enabled);
    cv::Mat applyTransform(const cv::Mat& src);
    void drawCrossline(cv::Mat& image);

private:
    // 预览区域
    QLabel* previewLabel_;

    // 位置选择
    QButtonGroup* positionGroup_;
    QRadioButton* positionRadios_[4];

    // 参数设置
    QGroupBox* paramGroup_;
    QSlider* exposureSlider_;
    QDoubleSpinBox* exposureSpin_;
    QToolButton* exposureDecBtn_;
    QToolButton* exposureIncBtn_;

    QSlider* gainSlider_;
    QDoubleSpinBox* gainSpin_;
    QToolButton* gainDecBtn_;
    QToolButton* gainIncBtn_;

    QCheckBox* gammaCheck_;
    QSlider* gammaSlider_;
    QDoubleSpinBox* gammaSpin_;

    QPushButton* lightSettingsBtn_;
    QPushButton* triggerSettingsBtn_;

    // 显示设置
    QGroupBox* displayGroup_;
    QCheckBox* flipHorizontalCheck_;
    QCheckBox* flipVerticalCheck_;
    QButtonGroup* rotationGroup_;
    QRadioButton* rotationRadios_[4];

    // 十字线设置
    QGroupBox* crosslineGroup_;
    QCheckBox* crosslineCheck_;
    QDoubleSpinBox* crosslineXSpin_;
    QDoubleSpinBox* crosslineYSpin_;
    QPushButton* crosslineCenterBtn_;

    // 采集按钮
    QPushButton* startCaptureBtn_;

    // 侧边按钮
    QToolButton* okBtn_;
    QToolButton* cancelBtn_;

    // 相机和状态
    HAL::ICamera* camera_;
    bool ownsCamera_;
    QTimer* previewTimer_;
    int frameCount_;
    qint64 lastFpsTime_;
    bool isCapturing_;

    // 图像变换设置
    int rotationAngle_;
    bool flipHorizontal_;
    bool flipVertical_;

    // 十字线设置
    bool crosslineEnabled_;
    double crosslineX_;
    double crosslineY_;

    // Gamma设置
    bool gammaEnabled_;
    double gammaValue_;

    // 当前位置
    int currentPosition_;
};

} // namespace VisionForge
