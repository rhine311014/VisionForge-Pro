/**
 * @file NinePointCalibDialog.h
 * @brief 九点标定对话框（简化版）
 * @details 参考VisionASM设计，提供向导式九点标定界面
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>

#include "algorithm/NinePointCalibTool.h"
#include "algorithm/CalibrationResult.h"
#include "base/ImageData.h"
#include "ui/ImageViewer.h"

namespace VisionForge {
namespace UI {

/**
 * @class NinePointCalibDialog
 * @brief 简化版九点标定对话框
 *
 * 设计理念（参考VisionASM）：
 * - 向导式三步流程：设置参数 → 采集点位 → 查看结果
 * - 可视化3x3网格，点击采集
 * - 简洁的参数设置
 * - 实时反馈标定状态
 */
class NinePointCalibDialog : public QDialog {
    Q_OBJECT

public:
    explicit NinePointCalibDialog(QWidget* parent = nullptr);
    ~NinePointCalibDialog() override;

    void setCurrentImage(const Base::ImageData::Ptr& image);
    Algorithm::CalibrationResult getCalibrationResult() const;
    bool isCalibrated() const;

signals:
    void calibrationCompleted(const Algorithm::CalibrationResult& result);

private slots:
    // 步骤导航
    void onNextStep();
    void onPrevStep();

    // 网格点击采集
    void onGridPointClicked(int index);
    void onImageClicked(int x, int y);

    // 标定操作
    void onExecuteCalibration();
    void onResetCalibration();

    // 对话框
    void onOkClicked();
    void onCancelClicked();

private:
    void createUI();
    void createStep1_Settings();
    void createStep2_Capture();
    void createStep3_Result();
    void createNavigationButtons();

    void updateStepIndicator();
    void updateGridButtons();
    void updateResultDisplay();
    void drawCalibPoints();
    void goToStep(int step);

    // 生成标准3x3网格物理坐标
    void generateGridCoordinates();

private:
    // 标定工具
    Algorithm::NinePointCalibTool* calibTool_;
    Base::ImageData::Ptr currentImage_;

    // 当前状态
    int currentStep_;           // 0=设置, 1=采集, 2=结果
    int currentPointIndex_;     // 当前采集的点索引 (0-8)

    // 主布局
    QStackedWidget* stackedWidget_;

    // 步骤指示器
    QLabel* stepIndicators_[3];
    QLabel* stepLabels_[3];

    // 步骤1: 参数设置
    QDoubleSpinBox* gridSpacingSpin_;    // 网格间距 (mm)
    QDoubleSpinBox* originXSpin_;        // 原点X (mm)
    QDoubleSpinBox* originYSpin_;        // 原点Y (mm)

    // 步骤2: 点位采集
    ImageViewer* imageViewer_;
    QPushButton* gridButtons_[9];        // 3x3网格按钮
    QLabel* pointStatusLabels_[9];       // 点状态标签
    QLabel* captureInstructionLabel_;    // 采集说明

    // 步骤3: 标定结果
    QLabel* pixelScaleLabel_;            // 像素比例
    QLabel* rotationLabel_;              // 旋转角度
    QLabel* calibErrorLabel_;            // 标定误差
    QLabel* resultStatusLabel_;          // 结果状态

    // 导航按钮
    QPushButton* prevBtn_;
    QPushButton* nextBtn_;
    QPushButton* calibrateBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
};

} // namespace UI
} // namespace VisionForge
