/**
 * @file CalcOrientationToolDialog.h
 * @brief 方向计算工具参数编辑对话框
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
    class CalcOrientationTool;
}

namespace UI {

class ImageViewer;

/**
 * @class CalcOrientationToolDialog
 * @brief 方向计算工具参数编辑对话框
 *
 * 支持的计算方法:
 * - 主成分分析(PCA)
 * - 最小外接矩形
 * - 惯性矩(二阶矩)
 * - 拟合椭圆
 */
class CalcOrientationToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalcOrientationToolDialog(Algorithm::CalcOrientationTool* tool, QWidget* parent = nullptr);
    ~CalcOrientationToolDialog() override;

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
     * @brief 请求执行计算
     */
    void executeCalcRequested();

    /**
     * @brief 请求采集图像
     */
    void captureImageRequested();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onCalcMethodChanged(int index);
    void onSourceTypeChanged(int index);
    void onAngleRangeChanged(int index);
    void onThresholdChanged(int value);
    void onInvertedChanged(bool checked);

    // 点列表操作
    void onAddPoint();
    void onRemovePoint();
    void onClearPoints();

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
    void createCalcMethodGroup(QVBoxLayout* layout);
    void createSourceTypeGroup(QVBoxLayout* layout);
    void createAngleRangeGroup(QVBoxLayout* layout);
    void createImageParamsGroup(QVBoxLayout* layout);
    void createPointsInputGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateInputVisibility();
    void updateResultDisplay();
    void updatePointsTable();

private:
    Algorithm::CalcOrientationTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 图像显示
    ImageViewer* imageViewer_;

    // 计算方法
    QComboBox* calcMethodCombo_;

    // 数据源类型
    QComboBox* sourceTypeCombo_;

    // 角度范围
    QComboBox* angleRangeCombo_;

    // 图像参数
    QGroupBox* imageParamsGroup_;
    QSpinBox* thresholdSpin_;
    QCheckBox* invertCheck_;

    // 点列表输入
    QGroupBox* pointsInputGroup_;
    QTableWidget* pointsTable_;
    QDoubleSpinBox* newPointXSpin_;
    QDoubleSpinBox* newPointYSpin_;
    QPushButton* addPointBtn_;
    QPushButton* removePointBtn_;
    QPushButton* clearPointsBtn_;

    // 结果显示
    QLabel* angleLabel_;
    QLabel* centerLabel_;
    QLabel* majorAxisLabel_;
    QLabel* minorAxisLabel_;

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
