/**
 * @file EdgeToolDialog.h
 * @brief 边缘检测工具参数设置对话框
 * @details 提供边缘检测工具的参数配置界面
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include "algorithm/EdgeTool.h"
#include "base/ImageData.h"

namespace VisionForge {
namespace UI {

/**
 * @class EdgeToolDialog
 * @brief 边缘检测工具参数设置对话框
 *
 * 功能：
 * - 设置边缘检测类型（Canny、Sobel、Laplacian、Scharr）
 * - 设置Canny参数（阈值1、阈值2、L2梯度）
 * - 设置Sobel/Scharr参数（核大小、方向、缩放、偏移）
 * - 设置Laplacian参数（核大小、缩放、偏移）
 * - 实时预览边缘检测效果
 */
class EdgeToolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param tool 边缘检测工具指针
     * @param parent 父窗口
     */
    explicit EdgeToolDialog(Algorithm::EdgeTool* tool, QWidget* parent = nullptr);
    ~EdgeToolDialog() override;

    /**
     * @brief 设置预览图像
     * @param image 输入图像
     */
    void setPreviewImage(const Base::ImageData::Ptr& image);

signals:
    /**
     * @brief 请求预览信号
     */
    void previewRequested();

    /**
     * @brief 参数已应用信号
     */
    void parametersApplied();

private slots:
    // 参数变更
    void onEdgeTypeChanged(int index);
    void onThreshold1Changed(double value);
    void onThreshold2Changed(double value);
    void onKernelSizeChanged(int index);
    void onDirectionChanged(int index);
    void onL2GradientChanged(bool checked);
    void onScaleChanged(double value);
    void onDeltaChanged(double value);

    // 对话框操作
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createBasicParamsGroup(QVBoxLayout* layout);
    void createCannyParamsGroup(QVBoxLayout* layout);
    void createSobelParamsGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);
    void connectSignals();
    void updateUI();
    void loadParameters();
    void applyParameters();

private:
    // 工具指针
    Algorithm::EdgeTool* tool_;

    // 预览图像
    Base::ImageData::Ptr previewImage_;

    // UI组件 - 基本参数
    QComboBox* edgeTypeCombo_;

    // UI组件 - Canny参数
    QGroupBox* cannyGroup_;
    QDoubleSpinBox* threshold1Spin_;
    QDoubleSpinBox* threshold2Spin_;
    QCheckBox* l2GradientCheck_;

    // UI组件 - Sobel/Laplacian参数
    QGroupBox* sobelGroup_;
    QComboBox* kernelSizeCombo_;
    QComboBox* directionCombo_;
    QDoubleSpinBox* scaleSpin_;
    QDoubleSpinBox* deltaSpin_;

    // UI组件 - 按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
