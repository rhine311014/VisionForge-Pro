/**
 * @file QRCalibDialog.h
 * @brief QR码/DM码自动标定对话框
 * @details 基于2D码的自动视觉标定界面
 * @author VisionForge Team
 * @date 2025
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QTableWidget>
#include <QProgressBar>

#include "calibration/QRCodeCalibration.h"
#include "algorithm/CalibrationResult.h"
#include "base/ImageData.h"
#include "ui/ImageViewer.h"

namespace VisionForge {
namespace UI {

/**
 * @class QRCalibDialog
 * @brief QR码/DM码自动标定对话框
 *
 * 功能：
 * - 自动检测图像中的QR码/DataMatrix码
 * - 提取码的角点作为标定点
 * - 一键执行标定计算
 * - 显示标定结果和误差
 */
class QRCalibDialog : public QDialog {
    Q_OBJECT

public:
    explicit QRCalibDialog(QWidget* parent = nullptr);
    ~QRCalibDialog() override;

    void setCurrentImage(const Base::ImageData::Ptr& image);
    Algorithm::CalibrationResult getCalibrationResult() const;
    bool isCalibrated() const;

signals:
    void calibrationCompleted(const Algorithm::CalibrationResult& result);

private slots:
    // 图像操作
    void onLoadImageClicked();
    void onCaptureImageClicked();

    // 检测操作
    void onDetectClicked();
    void onCalibrateClicked();
    void onResetClicked();

    // 配置变更
    void onCodeTypeChanged(int index);
    void onBoardLayoutChanged(int index);

    // 对话框按钮
    void onOkClicked();
    void onCancelClicked();

    // 进度更新
    void onProgressUpdated(int progress, const QString& message);

private:
    void createUI();
    void createConfigGroup();
    void createPreviewGroup();
    void createResultGroup();
    void createButtons();

    void updateDetectionResults();
    void updateCalibrationResult();
    void drawDetectedCodes();

private:
    // 标定工具
    Calibration::QRCodeCalibration* qrCalibrator_;
    Base::ImageData::Ptr currentImage_;

    // 配置区域
    QGroupBox* configGroup_;
    QComboBox* codeTypeCombo_;          // 码类型
    QComboBox* boardLayoutCombo_;       // 标定板布局
    QDoubleSpinBox* codeSizeSpin_;      // 码尺寸 (mm)
    QDoubleSpinBox* codeSpacingXSpin_;  // X间距 (mm)
    QDoubleSpinBox* codeSpacingYSpin_;  // Y间距 (mm)

    // 预览区域
    QGroupBox* previewGroup_;
    ImageViewer* imageViewer_;
    QLabel* detectionStatusLabel_;
    QPushButton* loadImageBtn_;      // 加载图像按钮
    QPushButton* captureImageBtn_;   // 采集图像按钮

    // 结果区域
    QGroupBox* resultGroup_;
    QTableWidget* pointsTable_;
    QLabel* pixelScaleLabel_;
    QLabel* rotationLabel_;
    QLabel* calibErrorLabel_;
    QProgressBar* progressBar_;
    QLabel* statusLabel_;

    // 按钮
    QPushButton* detectBtn_;
    QPushButton* calibrateBtn_;
    QPushButton* resetBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;

    // 状态
    bool calibrated_;
};

} // namespace UI
} // namespace VisionForge
