/**
 * @file MeasureAngleToolDialog.h
 * @brief 角度测量工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QVBoxLayout>

#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {

namespace Algorithm {
    class MeasureAngleTool;
}

namespace UI {

class ImageViewer;

/**
 * @class MeasureAngleToolDialog
 * @brief 角度测量工具参数编辑对话框
 *
 * 支持的测量模式:
 * - 两条线夹角
 * - 三点角度
 * - 线与水平/垂直方向夹角
 */
class MeasureAngleToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeasureAngleToolDialog(Algorithm::MeasureAngleTool* tool, QWidget* parent = nullptr);
    ~MeasureAngleToolDialog() override;

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

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onMeasureModeChanged(int index);
    void onAngleRangeChanged(int index);
    void onInputSourceChanged(int index);
    void onLine1Changed();
    void onLine2Changed();
    void onVertexChanged();
    void onPoint1Changed();
    void onPoint2Changed();

    // 对话框按钮槽
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onAutoCalc();

private:
    void createUI();
    void createMeasureModeGroup(QVBoxLayout* layout);
    void createAngleFormatGroup(QVBoxLayout* layout);
    void createInputSourceGroup(QVBoxLayout* layout);
    void createLineInputGroup(QVBoxLayout* layout);
    void createPointInputGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateInputVisibility();
    void updateResultDisplay();

private:
    Algorithm::MeasureAngleTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 图像显示
    ImageViewer* imageViewer_;

    // 测量模式
    QComboBox* measureModeCombo_;

    // 角度格式
    QComboBox* angleRangeCombo_;
    QComboBox* angleFormatCombo_;

    // 输入源选择
    QComboBox* inputSourceCombo_;

    // 线输入
    QGroupBox* lineInputGroup_;
    QDoubleSpinBox* line1X1Spin_;
    QDoubleSpinBox* line1Y1Spin_;
    QDoubleSpinBox* line1X2Spin_;
    QDoubleSpinBox* line1Y2Spin_;
    QDoubleSpinBox* line2X1Spin_;
    QDoubleSpinBox* line2Y1Spin_;
    QDoubleSpinBox* line2X2Spin_;
    QDoubleSpinBox* line2Y2Spin_;

    // 三点输入
    QGroupBox* pointInputGroup_;
    QDoubleSpinBox* vertexXSpin_;
    QDoubleSpinBox* vertexYSpin_;
    QDoubleSpinBox* point1XSpin_;
    QDoubleSpinBox* point1YSpin_;
    QDoubleSpinBox* point2XSpin_;
    QDoubleSpinBox* point2YSpin_;

    // 结果显示
    QLabel* angleDegLabel_;
    QLabel* angleRadLabel_;
    QLabel* vertexResultLabel_;

    // 对话框按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;

    // 实时计算
    PreviewHelper* previewHelper_;
    QCheckBox* autoCalcCheck_;
};

} // namespace UI
} // namespace VisionForge
