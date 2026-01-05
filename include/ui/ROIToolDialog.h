/**
 * @file ROIToolDialog.h
 * @brief ROI区域工具参数编辑对话框
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
#include <QTableWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "base/ImageData.h"

namespace VisionForge {

namespace Algorithm {
    class ROITool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class ROIToolDialog
 * @brief ROI区域工具参数编辑对话框
 *
 * 支持的ROI类型:
 * - 矩形
 * - 圆形
 * - 椭圆
 * - 多边形
 * - 旋转矩形
 */
class ROIToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ROIToolDialog(Algorithm::ROITool* tool, QWidget* parent = nullptr);
    ~ROIToolDialog() override;

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
    // ROI类型选择
    void onROITypeChanged(int index);

    // ROI操作
    void onAddROIClicked();
    void onRemoveROIClicked();
    void onClearROIsClicked();
    void onROISelected(int row);

    // 矩形参数
    void onRectParamsChanged();

    // 圆形参数
    void onCircleParamsChanged();

    // 椭圆参数
    void onEllipseParamsChanged();

    // 旋转矩形参数
    void onRotatedRectParamsChanged();

    // 组合模式
    void onCombineModeChanged(int index);
    void onInvertMaskChanged(bool checked);
    void onCropOutputChanged(bool checked);

    // 预览按钮槽
    void onPreviewClicked();

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
    void createROITypeGroup(QVBoxLayout* layout);
    void createROIParamsGroup(QVBoxLayout* layout);
    void createRectParamsPage();
    void createCircleParamsPage();
    void createEllipseParamsPage();
    void createPolygonParamsPage();
    void createRotatedRectParamsPage();
    void createROIListGroup(QVBoxLayout* layout);
    void createCombineGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateROIList();
    void updateParamsFromSelection();

private:
    Algorithm::ROITool* tool_;
    Base::ImageData::Ptr currentImage_;
    bool embeddedMode_ = false;

    // 左侧图像显示
    QWidget* leftPanel_ = nullptr;
    HalconImageViewer* imageViewer_;
    QSplitter* mainSplitter_;

    // 图像操作按钮
    QPushButton* loadImageBtn_;
    QPushButton* captureImageBtn_;

    // ROI类型选择
    QComboBox* roiTypeCombo_;

    // ROI参数页面
    QStackedWidget* paramsStack_;

    // 矩形参数
    QWidget* rectParamsPage_;
    QDoubleSpinBox* rectXSpin_;
    QDoubleSpinBox* rectYSpin_;
    QDoubleSpinBox* rectWidthSpin_;
    QDoubleSpinBox* rectHeightSpin_;

    // 圆形参数
    QWidget* circleParamsPage_;
    QDoubleSpinBox* circleCXSpin_;
    QDoubleSpinBox* circleCYSpin_;
    QDoubleSpinBox* circleRadiusSpin_;

    // 椭圆参数
    QWidget* ellipseParamsPage_;
    QDoubleSpinBox* ellipseCXSpin_;
    QDoubleSpinBox* ellipseCYSpin_;
    QDoubleSpinBox* ellipseRXSpin_;
    QDoubleSpinBox* ellipseRYSpin_;
    QDoubleSpinBox* ellipseAngleSpin_;

    // 多边形参数
    QWidget* polygonParamsPage_;
    QTableWidget* polygonPointsTable_;
    QPushButton* addPolygonPointBtn_;
    QPushButton* removePolygonPointBtn_;

    // 旋转矩形参数
    QWidget* rotatedRectParamsPage_;
    QDoubleSpinBox* rotRectCXSpin_;
    QDoubleSpinBox* rotRectCYSpin_;
    QDoubleSpinBox* rotRectWidthSpin_;
    QDoubleSpinBox* rotRectHeightSpin_;
    QDoubleSpinBox* rotRectAngleSpin_;

    // ROI列表
    QTableWidget* roiListTable_;
    QPushButton* addROIBtn_;
    QPushButton* removeROIBtn_;
    QPushButton* clearROIsBtn_;

    // 组合模式
    QComboBox* combineModeCombo_;
    QCheckBox* invertMaskCheck_;
    QCheckBox* cropOutputCheck_;

    // 预览按钮
    QPushButton* previewBtn_;

    // 对话框按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
