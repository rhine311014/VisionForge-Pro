/**
 * @file AIDetectionToolDialog.h
 * @brief AI检测工具参数编辑对话框
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
#include <QCheckBox>
#include <QLineEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QTextEdit>
#include "base/ImageData.h"

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class AIDetectionTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class AIDetectionToolDialog
 * @brief AI检测工具参数编辑对话框
 *
 * 包含左侧图像显示窗口和右侧参数设置面板
 * 支持深度学习模型的目标检测、分类、缺陷检测等任务
 */
class AIDetectionToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AIDetectionToolDialog(Algorithm::AIDetectionTool* tool, QWidget* parent = nullptr);
    ~AIDetectionToolDialog() override;

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
    // 模型管理槽
    void onBrowseModelClicked();
    void onBrowseConfigClicked();
    void onLoadModelClicked();
    void onUnloadModelClicked();

    // 参数槽
    void onTaskTypeChanged(int index);
    void onInferenceEngineChanged(int index);
    void onConfidenceThresholdChanged(double value);
    void onNmsThresholdChanged(double value);
    void onInputSizeChanged();
    void onUseGPUChanged(bool checked);

    // 类别名称管理
    void onLoadClassNamesClicked();
    void onClearClassNamesClicked();

    // 预览按钮槽
    void onPreviewClicked();

    // 对话框按钮槽
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createModelGroup(QVBoxLayout* layout);
    void createInferenceGroup(QVBoxLayout* layout);
    void createDetectionParamGroup(QVBoxLayout* layout);
    void createClassNamesGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateModelStatus();
    void updateResults();

private:
    Algorithm::AIDetectionTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 左侧图像显示
    HalconImageViewer* imageViewer_;
    QSplitter* mainSplitter_;

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 模型管理
    QLineEdit* modelPathEdit_;
    QPushButton* browseModelBtn_;
    QLineEdit* configPathEdit_;
    QPushButton* browseConfigBtn_;
    QPushButton* loadModelBtn_;
    QPushButton* unloadModelBtn_;
    QLabel* modelStatusLabel_;

    // 推理参数
    QComboBox* taskTypeCombo_;
    QComboBox* inferenceEngineCombo_;
    QCheckBox* useGPUCheckBox_;

    // 检测参数
    QDoubleSpinBox* confidenceThresholdSpinBox_;
    QSlider* confidenceSlider_;
    QDoubleSpinBox* nmsThresholdSpinBox_;
    QSpinBox* inputWidthSpinBox_;
    QSpinBox* inputHeightSpinBox_;

    // 类别名称
    QTextEdit* classNamesEdit_;
    QPushButton* loadClassNamesBtn_;
    QPushButton* clearClassNamesBtn_;

    // 结果表格
    QTableWidget* resultsTable_;

    // 预览按钮
    QPushButton* previewBtn_;

    // 对话框按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
