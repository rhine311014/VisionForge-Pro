/**
 * @file ShapeMatchParamPanel.h
 * @brief 形状匹配参数面板
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class ShapeMatchTool;
}

namespace UI {

/**
 * @class ShapeMatchParamPanel
 * @brief 形状匹配工具参数设置面板
 */
class ShapeMatchParamPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ShapeMatchParamPanel(Algorithm::ShapeMatchTool* tool, QWidget* parent = nullptr);
    ~ShapeMatchParamPanel() override = default;

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

private slots:
    void onMinScoreChanged(double value);
    void onNumMatchesChanged(int value);
    void onAngleRangeChanged();
    void onContrastChanged(int value);
    void onScaleRangeChanged();
    void onMatchTypeChanged(int index);
    void onScaleStepRowChanged(double value);
    void onScaleStepColChanged(double value);
    void onUseXLDDisplayChanged(int state);
    void onLoadModelClicked();
    void onSaveModelClicked();
    void onTrainModelClicked();
    void onClearModelClicked();
    void onModelLibraryClicked();

private:
    void createUI();
    void connectSignals();
    QString getModelPath(bool forSave);

private:
    Algorithm::ShapeMatchTool* tool_;

    // 匹配参数
    QComboBox* matchTypeCombo_;
    QDoubleSpinBox* minScoreSpinBox_;
    QSlider* minScoreSlider_;
    QSpinBox* numMatchesSpinBox_;
    QDoubleSpinBox* angleStartSpinBox_;
    QDoubleSpinBox* angleExtentSpinBox_;
    QSpinBox* minContrastSpinBox_;
    QDoubleSpinBox* scaleMinSpinBox_;
    QDoubleSpinBox* scaleMaxSpinBox_;
    QLabel* scaleStepRowLabel_;
    QDoubleSpinBox* scaleStepRowSpinBox_;
    QLabel* scaleStepColLabel_;
    QDoubleSpinBox* scaleStepColSpinBox_;

    // 显示选项
    QCheckBox* useXLDDisplayCheckBox_;

    // 模板管理
    QLineEdit* modelPathEdit_;
    QPushButton* loadModelBtn_;
    QPushButton* saveModelBtn_;
    QPushButton* trainModelBtn_;
    QPushButton* clearModelBtn_;
    QPushButton* modelLibraryBtn_;
    QLabel* modelStatusLabel_;
};

} // namespace UI
} // namespace VisionForge
