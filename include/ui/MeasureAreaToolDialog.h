/**
 * @file MeasureAreaToolDialog.h
 * @brief 面积测量工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QVBoxLayout>

#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {

namespace Algorithm {
    class MeasureAreaTool;
}

namespace UI {

class ImageViewer;

/**
 * @class MeasureAreaToolDialog
 * @brief 面积测量工具参数编辑对话框
 *
 * 支持的测量模式:
 * - 多边形面积
 * - 轮廓面积(二值图)
 * - 圆形面积
 * - 椭圆面积
 */
class MeasureAreaToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeasureAreaToolDialog(Algorithm::MeasureAreaTool* tool, QWidget* parent = nullptr);
    ~MeasureAreaToolDialog() override;

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
     * @brief 请求执行测量
     */
    void executeMeasureRequested();

    /**
     * @brief 请求采集图像信号
     */
    void captureImageRequested();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onMeasureModeChanged(int index);
    void onPixelToMmChanged(double value);
    void onThresholdChanged(int value);
    void onMinAreaChanged(double value);
    void onInputSourceChanged(int index);
    void onCircleParamsChanged();
    void onEllipseParamsChanged();

    // 对话框按钮槽
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onAutoCalc();

    // 图像操作槽
    void onLoadImageClicked();
    void onCaptureImageClicked();

private:
    void createUI();
    void createMeasureModeGroup(QVBoxLayout* layout);
    void createInputSourceGroup(QVBoxLayout* layout);
    void createContourParamsGroup(QVBoxLayout* layout);
    void createCircleParamsGroup(QVBoxLayout* layout);
    void createEllipseParamsGroup(QVBoxLayout* layout);
    void createUnitConversionGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateInputVisibility();
    void updateResultDisplay();

private:
    Algorithm::MeasureAreaTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 图像显示
    ImageViewer* imageViewer_;

    // 测量模式
    QComboBox* measureModeCombo_;

    // 输入源选择
    QComboBox* inputSourceCombo_;

    // 轮廓参数
    QGroupBox* contourParamsGroup_;
    QSpinBox* thresholdSpin_;
    QDoubleSpinBox* minAreaSpin_;
    QCheckBox* invertCheck_;

    // 圆形参数
    QGroupBox* circleParamsGroup_;
    QDoubleSpinBox* circleCenterXSpin_;
    QDoubleSpinBox* circleCenterYSpin_;
    QDoubleSpinBox* circleRadiusSpin_;

    // 椭圆参数
    QGroupBox* ellipseParamsGroup_;
    QDoubleSpinBox* ellipseCenterXSpin_;
    QDoubleSpinBox* ellipseCenterYSpin_;
    QDoubleSpinBox* ellipseASpin_;
    QDoubleSpinBox* ellipseBSpin_;
    QDoubleSpinBox* ellipseAngleSpin_;

    // 单位转换
    QDoubleSpinBox* pixelToMmSpin_;

    // 结果显示
    QLabel* areaPixelLabel_;
    QLabel* areaMmLabel_;
    QLabel* perimeterLabel_;
    QLabel* centroidLabel_;
    QLabel* circularityLabel_;
    QTableWidget* resultsTable_;

    // 对话框按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;

    // 图像操作按钮
    QPushButton* loadImageBtn_;
    QPushButton* captureImageBtn_;

    // 实时计算
    PreviewHelper* previewHelper_;
    QCheckBox* autoCalcCheck_;
};

} // namespace UI
} // namespace VisionForge
