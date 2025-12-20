/**
 * @file LineToolDialog.h
 * @brief 线检测工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class LineTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class LineToolDialog
 * @brief 线检测工具参数编辑对话框
 *
 * 包含左侧图像显示窗口和右侧参数设置面板
 * 支持霍夫线检测、轮廓拟合等多种检测方法
 */
class LineToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LineToolDialog(Algorithm::LineTool* tool, QWidget* parent = nullptr);
    ~LineToolDialog() override;

    /**
     * @brief 设置显示图像
     */
    void setImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 更新UI显示
     */
    void updateUI();

signals:
    /**
     * @brief 参数已改变
     */
    void parameterChanged();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onBackendChanged(int index);
    void onMethodChanged(int index);
    void onMinLengthChanged(double value);
    void onMaxGapChanged(double value);
    void onMaxCountChanged(int value);

    // 霍夫参数槽
    void onRhoChanged(double value);
    void onThetaChanged(double value);
    void onThresholdChanged(int value);

    // Canny参数槽
    void onCannyThreshold1Changed(double value);
    void onCannyThreshold2Changed(double value);

    // 预览按钮槽
    void onPreviewClicked();
    void onAutoPreview();

    // 对话框按钮槽
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createBasicParamGroup(QVBoxLayout* layout);
    void createHoughParamGroup(QVBoxLayout* layout);
    void createCannyParamGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateMethodVisibility();

private:
    Algorithm::LineTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 左侧图像显示
    HalconImageViewer* imageViewer_;
    QSplitter* mainSplitter_;

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 基本参数
    QComboBox* backendCombo_;
    QComboBox* methodCombo_;
    QDoubleSpinBox* minLengthSpinBox_;
    QDoubleSpinBox* maxGapSpinBox_;
    QSpinBox* maxCountSpinBox_;

    // 霍夫参数
    QGroupBox* houghParamGroup_;
    QDoubleSpinBox* rhoSpinBox_;
    QDoubleSpinBox* thetaSpinBox_;
    QSpinBox* thresholdSpinBox_;

    // Canny参数
    QGroupBox* cannyParamGroup_;
    QDoubleSpinBox* cannyThreshold1SpinBox_;
    QDoubleSpinBox* cannyThreshold2SpinBox_;

    // 预览按钮
    QPushButton* previewBtn_;

    // 对话框按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;

    // 实时预览
    PreviewHelper* previewHelper_;
    QCheckBox* autoPreviewCheck_;
};

} // namespace UI
} // namespace VisionForge
