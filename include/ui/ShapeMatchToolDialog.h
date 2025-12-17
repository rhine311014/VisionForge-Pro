/**
 * @file ShapeMatchToolDialog.h
 * @brief 形状匹配工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTabWidget>
#include "base/ImageData.h"

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class ShapeMatchTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class ShapeMatchToolDialog
 * @brief 形状匹配工具参数编辑对话框
 *
 * 包含左侧Halcon图像显示窗口和右侧参数设置面板
 */
class ShapeMatchToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShapeMatchToolDialog(Algorithm::ShapeMatchTool* tool, QWidget* parent = nullptr);
    ~ShapeMatchToolDialog() override;

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
     * @brief 请求训练模板
     */
    void trainModelRequested();

    /**
     * @brief 请求执行匹配
     */
    void executeMatchRequested();

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    // 匹配参数槽
    void onMinScoreChanged(double value);
    void onNumMatchesChanged(int value);
    void onAngleRangeChanged();
    void onContrastChanged(int value);
    void onScaleRangeChanged();
    void onMatchTypeChanged(int index);
    void onPyramidLevelChanged(int value);

    // ROI操作槽
    void onDrawROIClicked();
    void onDrawFreehandClicked();
    void onClearROIClicked();

    // 模板操作槽
    void onGetTemplateImageClicked();
    void onTrainModelClicked();
    void onModelCheckPointClicked();
    void onLoadModelClicked();
    void onSaveModelClicked();
    void onClearModelClicked();
    void onModelLibraryClicked();

    // 标准轮廓生成槽
    void onGenerateMarkContourClicked();
    void onMarkShapeTypeChanged(int index);
    void onPreviewMarkContour();

    // 对话框按钮槽
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createToolTypeGroup(QVBoxLayout* layout);
    void createROIGroup(QVBoxLayout* layout);
    void createAngleGroup(QVBoxLayout* layout);
    void createMatchParamGroup(QVBoxLayout* layout);
    void createModelGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    QString getModelPath(bool forSave);
    void applyParameters();
    void updateFeaturePreview();  // 更新特征预览
    bool saveToModelLibrary();    // 保存到模板库
    bool updateModelInLibrary();  // 更新模板库中的模板

private:
    Algorithm::ShapeMatchTool* tool_;
    Base::ImageData::Ptr currentImage_;
    QString currentLibraryModelId_;  // 当前加载的模板库模型ID（用于直接更新）

    // 左侧图像显示
    HalconImageViewer* imageViewer_;
    HalconImageViewer* featureViewer_;  // 特征预览窗口
    QSplitter* mainSplitter_;
    QSplitter* leftSplitter_;  // 左侧分割器（图像/特征）

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 工具类型
    QComboBox* toolTypeCombo_;
    QLabel* toolTypePreview_;

    // ROI设置
    QCheckBox* polarCoordCheckBox_;
    QPushButton* drawROIBtn_;
    QPushButton* drawFreehandBtn_;
    QPushButton* clearROIBtn_;
    QLabel* roiStatusLabel_;

    // 角度范围
    QDoubleSpinBox* angleStartSpinBox_;
    QDoubleSpinBox* angleExtentSpinBox_;
    QLabel* anglePresetLabel_;

    // 匹配参数
    QSpinBox* pyramidLevelSpinBox_;
    QSpinBox* minContrastSpinBox_;
    QDoubleSpinBox* minScoreSpinBox_;
    QSlider* minScoreSlider_;
    QSpinBox* numMatchesSpinBox_;
    QDoubleSpinBox* overlapSpinBox_;
    QComboBox* subPixelCombo_;
    QDoubleSpinBox* greedySpinBox_;

    // 缩放参数
    QDoubleSpinBox* scaleMinSpinBox_;
    QDoubleSpinBox* scaleMaxSpinBox_;
    QComboBox* matchTypeCombo_;

    // 模板管理
    QPushButton* getTemplateBtn_;
    QPushButton* trainModelBtn_;
    QPushButton* modelCheckPointBtn_;
    QPushButton* deleteModelBtn_;
    QPushButton* modelLibraryBtn_;
    QLabel* modelStatusLabel_;

    // 标准Mark轮廓生成
    QComboBox* markShapeTypeCombo_;           // 形状类型选择
    QDoubleSpinBox* markSize1SpinBox_;        // 主尺寸（臂长/半径/边长）
    QDoubleSpinBox* markSize2SpinBox_;        // 辅尺寸（臂宽/内径/宽度）
    QDoubleSpinBox* markSize3SpinBox_;        // 额外尺寸（线宽等）
    QLabel* markSize1Label_;                  // 尺寸1标签
    QLabel* markSize2Label_;                  // 尺寸2标签
    QLabel* markSize3Label_;                  // 尺寸3标签
    QPushButton* previewMarkBtn_;             // 预览按钮
    QPushButton* generateMarkContourBtn_;     // 生成并训练按钮

    // 对话框按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
