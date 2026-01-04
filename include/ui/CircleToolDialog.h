/**
 * @file CircleToolDialog.h
 * @brief 圆检测工具参数编辑对话框
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
    class CircleTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class CircleToolDialog
 * @brief 圆检测工具参数编辑对话框
 *
 * 包含左侧图像显示窗口和右侧参数设置面板
 * 支持霍夫圆检测和轮廓拟合等多种检测方法
 */
class CircleToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CircleToolDialog(Algorithm::CircleTool* tool, QWidget* parent = nullptr);
    ~CircleToolDialog() override;

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

    /**
     * @brief 请求采集图像信号
     */
    void captureImageRequested();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onBackendChanged(int index);
    void onMethodChanged(int index);
    void onRadiusRangeChanged();
    void onMaxCountChanged(int value);

    // 霍夫参数槽
    void onHoughDpChanged(double value);
    void onMinDistChanged(double value);
    void onCannyThresholdChanged(double value);
    void onAccumThresholdChanged(double value);

    // 轮廓拟合参数槽
    void onMinCircularityChanged(double value);
    void onMinAreaChanged(double value);

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
    void createBasicParamGroup(QVBoxLayout* layout);
    void createHoughParamGroup(QVBoxLayout* layout);
    void createFilterParamGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateMethodVisibility();

private:
    Algorithm::CircleTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 左侧图像显示
    HalconImageViewer* imageViewer_;
    QSplitter* mainSplitter_;

    // 图像操作按钮
    QPushButton* loadImageBtn_;
    QPushButton* captureImageBtn_;

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 基本参数
    QComboBox* backendCombo_;
    QComboBox* methodCombo_;
    QDoubleSpinBox* minRadiusSpinBox_;
    QDoubleSpinBox* maxRadiusSpinBox_;
    QSpinBox* maxCountSpinBox_;

    // 霍夫参数
    QGroupBox* houghParamGroup_;
    QDoubleSpinBox* houghDpSpinBox_;
    QDoubleSpinBox* minDistSpinBox_;
    QDoubleSpinBox* cannyThresholdSpinBox_;
    QDoubleSpinBox* accumThresholdSpinBox_;

    // 轮廓拟合/过滤参数
    QGroupBox* filterParamGroup_;
    QDoubleSpinBox* minCircularitySpinBox_;
    QDoubleSpinBox* minAreaSpinBox_;

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
