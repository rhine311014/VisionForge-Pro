/**
 * @file SubPixelEdgeToolDialog.h
 * @brief 亚像素边缘检测工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-22
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
    class SubPixelEdgeTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class SubPixelEdgeToolDialog
 * @brief 亚像素边缘检测工具参数编辑对话框
 *
 * 支持多种亚像素边缘检测算法：
 * - Sobel梯度插值：速度快，精度0.1像素
 * - Zernike矩：精度高，0.05像素
 * - 二次曲线拟合：平衡型
 * - 一维高斯拟合：适合单边缘
 * - ESR拟合：复杂边缘
 */
class SubPixelEdgeToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubPixelEdgeToolDialog(Algorithm::SubPixelEdgeTool* tool, QWidget* parent = nullptr);
    ~SubPixelEdgeToolDialog() override;

    /**
     * @brief 设置显示图像
     */
    void setImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 更新UI显示
     */
    void updateUI();

    /**
     * @brief 设置嵌入模式
     */
    Q_INVOKABLE void setEmbeddedMode(bool embedded);

signals:
    /**
     * @brief 参数已改变
     */
    void parameterChanged();

    /**
     * @brief 请求采集图像信号
     */
    void captureImageRequested();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onMethodChanged(int index);
    void onGradientThresholdChanged(double value);
    void onWindowSizeChanged(int value);
    void onUseSubPixelChanged(bool checked);
    void onSigmaChanged(double value);
    void onCannyLowChanged(int value);
    void onCannyHighChanged(int value);

    // 预览按钮槽
    void onPreviewClicked();
    void onAutoPreview();

    // 对话框按钮槽
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

    // 图像操作槽
    void onLoadImageClicked();
    void onCaptureImageClicked();

private:
    void createUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createMethodGroup(QVBoxLayout* layout);
    void createEdgeParamGroup(QVBoxLayout* layout);
    void createFilterParamGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateMethodDescription();

private:
    Algorithm::SubPixelEdgeTool* tool_;
    Base::ImageData::Ptr currentImage_;
    bool embeddedMode_ = false;

    // 左侧图像显示
    QWidget* leftPanel_ = nullptr;
    HalconImageViewer* imageViewer_;
    QSplitter* mainSplitter_;

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 亚像素方法选择
    QComboBox* methodCombo_;
    QLabel* methodDescLabel_;

    // 边缘检测参数
    QDoubleSpinBox* gradientThresholdSpinBox_;
    QSpinBox* windowSizeSpinBox_;
    QCheckBox* useSubPixelCheck_;

    // 滤波参数
    QDoubleSpinBox* sigmaSpinBox_;
    QSpinBox* cannyLowSpinBox_;
    QSpinBox* cannyHighSpinBox_;

    // 结果显示
    QLabel* edgePointCountLabel_;
    QLabel* avgSubPixelShiftLabel_;
    QLabel* maxGradientLabel_;
    QLabel* processTimeLabel_;

    // 预览按钮
    QPushButton* previewBtn_;

    // 对话框按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;

    // 图像操作按钮
    QPushButton* loadImageBtn_;
    QPushButton* captureImageBtn_;

    // 实时预览
    PreviewHelper* previewHelper_;
    QCheckBox* autoPreviewCheck_;
};

} // namespace UI
} // namespace VisionForge
