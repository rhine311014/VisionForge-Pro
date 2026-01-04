/**
 * @file TemplateMatchToolDialog.h
 * @brief 模板匹配工具参数编辑对话框 (OpenCV版本)
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTabWidget>
#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class TemplateMatchTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class TemplateMatchToolDialog
 * @brief 模板匹配工具参数编辑对话框 (OpenCV版本)
 *
 * 包含左侧图像显示窗口和右侧参数设置面板
 * 支持多种OpenCV模板匹配方法
 */
class TemplateMatchToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TemplateMatchToolDialog(Algorithm::TemplateMatchTool* tool, QWidget* parent = nullptr);
    ~TemplateMatchToolDialog() override;

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
    // 匹配参数槽
    void onMatchMethodChanged(int index);
    void onThresholdChanged(double value);
    void onMaxMatchesChanged(int value);
    void onPyramidLevelsChanged(int value);
    void onUsePyramidChanged(bool checked);
    void onEnableAngleSearchChanged(bool checked);
    void onAngleRangeChanged();
    void onNmsDistanceChanged(double value);

    // 模板操作槽
    void onSelectTemplateClicked();
    void onCreateTemplateFromROIClicked();
    void onLoadTemplateClicked();
    void onSaveTemplateClicked();
    void onClearTemplateClicked();

    // ROI操作槽
    void onDrawROIClicked();
    void onClearROIClicked();

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
    void createMatchMethodGroup(QVBoxLayout* layout);
    void createMatchParamGroup(QVBoxLayout* layout);
    void createAngleSearchGroup(QVBoxLayout* layout);
    void createTemplateGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateTemplatePreview();

private:
    Algorithm::TemplateMatchTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 左侧图像显示
    HalconImageViewer* imageViewer_;
    HalconImageViewer* templateViewer_;  // 模板预览窗口
    QSplitter* mainSplitter_;

    // 图像操作按钮
    QPushButton* loadImageBtn_;
    QPushButton* captureImageBtn_;

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 匹配方法
    QComboBox* matchMethodCombo_;
    QLabel* matchMethodDesc_;

    // 匹配参数
    QDoubleSpinBox* thresholdSpinBox_;
    QSlider* thresholdSlider_;
    QSpinBox* maxMatchesSpinBox_;
    QCheckBox* usePyramidCheckBox_;
    QSpinBox* pyramidLevelsSpinBox_;
    QDoubleSpinBox* nmsDistanceSpinBox_;

    // 角度搜索
    QCheckBox* enableAngleSearchCheckBox_;
    QDoubleSpinBox* angleMinSpinBox_;
    QDoubleSpinBox* angleMaxSpinBox_;
    QDoubleSpinBox* angleStepSpinBox_;
    QWidget* angleSearchWidget_;

    // 模板管理
    QPushButton* selectTemplateBtn_;
    QPushButton* createFromROIBtn_;
    QPushButton* loadTemplateBtn_;
    QPushButton* saveTemplateBtn_;
    QPushButton* clearTemplateBtn_;
    QLabel* templateStatusLabel_;

    // ROI操作
    QPushButton* drawROIBtn_;
    QPushButton* clearROIBtn_;
    QLabel* roiStatusLabel_;

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
