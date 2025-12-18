/**
 * @file CameraCalibDialog.h
 * @brief 相机标定对话框
 * @details 提供相机标定的交互界面，支持棋盘格/圆点阵检测和标定
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QProgressBar>

#include "algorithm/CameraCalibTool.h"
#include "algorithm/CalibrationResult.h"
#include "base/ImageData.h"
#include "ui/ImageViewer.h"

namespace VisionForge {
namespace UI {

/**
 * @class CameraCalibDialog
 * @brief 相机标定对话框
 *
 * 功能：
 * - 设置标定板参数（类型、尺寸、格子大小）
 * - 采集和管理标定图像
 * - 检测角点并预览
 * - 执行标定并显示结果
 */
class CameraCalibDialog : public QDialog {
    Q_OBJECT

public:
    explicit CameraCalibDialog(QWidget* parent = nullptr);
    ~CameraCalibDialog() override;

    /**
     * @brief 设置当前图像
     * @param image 图像数据
     */
    void setCurrentImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 获取标定结果
     */
    Algorithm::CalibrationResult getCalibrationResult() const;

    /**
     * @brief 是否已完成标定
     */
    bool isCalibrated() const;

signals:
    /**
     * @brief 请求采集图像
     */
    void captureRequested();

    /**
     * @brief 标定完成信号
     */
    void calibrationCompleted(const Algorithm::CalibrationResult& result);

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    // 图像管理
    void onAddCurrentImage();
    void onLoadImagesFromFolder();
    void onRemoveSelectedImages();
    void onClearAllImages();
    void onImageSelected(int index);

    // 检测与标定
    void onDetectCorners();
    void onExecuteCalibration();

    // 参数变更
    void onPatternTypeChanged(int index);
    void onBoardSizeChanged();
    void onSquareSizeChanged(double value);
    void onBackendChanged(int index);

    // 对话框操作
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onSaveResult();

private:
    void createUI();
    void createImagePreviewArea(QSplitter* splitter);
    void createControlPanel(QSplitter* splitter);
    void createPatternParamsGroup(QVBoxLayout* layout);
    void createDetectionParamsGroup(QVBoxLayout* layout);
    void createImageListGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);

    void connectSignals();
    void updateUI();
    void updateImageList();
    void updateResultDisplay();
    void applyParameters();

    // 在预览图像上绘制角点
    void drawCornersOnPreview(const cv::Mat& image,
                               const std::vector<cv::Point2f>& corners,
                               bool found);

private:
    // 标定工具
    Algorithm::CameraCalibTool* calibTool_;

    // 当前图像
    Base::ImageData::Ptr currentImage_;

    // UI组件 - 图像预览
    ImageViewer* imageViewer_;

    // UI组件 - 标定板参数
    QComboBox* patternTypeCombo_;
    QSpinBox* boardWidthSpin_;
    QSpinBox* boardHeightSpin_;
    QDoubleSpinBox* squareSizeSpin_;
    QComboBox* backendCombo_;

    // UI组件 - 检测参数
    QCheckBox* subPixelCheck_;
    QCheckBox* adaptiveThreshCheck_;
    QCheckBox* fastCheckCheck_;
    QSpinBox* minImageCountSpin_;

    // UI组件 - 图像列表
    QListWidget* imageList_;
    QLabel* imageCountLabel_;
    QPushButton* addImageBtn_;
    QPushButton* loadFolderBtn_;
    QPushButton* removeImageBtn_;
    QPushButton* clearAllBtn_;

    // UI组件 - 检测与标定
    QPushButton* detectBtn_;
    QPushButton* calibrateBtn_;
    QProgressBar* progressBar_;

    // UI组件 - 结果显示
    QLabel* rmsErrorLabel_;
    QLabel* focalLengthLabel_;
    QLabel* principalPointLabel_;
    QLabel* imagesSizeLabel_;
    QPushButton* saveResultBtn_;

    // UI组件 - 对话框按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
