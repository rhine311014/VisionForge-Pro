/**
 * @file ThresholdToolDialog.h
 * @brief 二值化工具参数设置对话框
 * @details 提供图像二值化工具的参数配置界面
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSlider>

#include "algorithm/ThresholdTool.h"
#include "base/ImageData.h"

namespace VisionForge {
namespace UI {

/**
 * @class ThresholdToolDialog
 * @brief 二值化工具参数设置对话框
 *
 * 功能：
 * - 设置阈值类型（二值化、反二值化、截断、归零、反归零）
 * - 设置阈值方法（手动、OTSU、Triangle、自适应均值、自适应高斯）
 * - 设置阈值、最大值
 * - 设置自适应参数（块大小、常数C）
 * - 实时预览二值化效果
 */
class ThresholdToolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param tool 二值化工具指针
     * @param parent 父窗口
     */
    explicit ThresholdToolDialog(Algorithm::ThresholdTool* tool, QWidget* parent = nullptr);
    ~ThresholdToolDialog() override;

    /**
     * @brief 设置预览图像
     * @param image 输入图像
     */
    void setPreviewImage(const Base::ImageData::Ptr& image);

signals:
    /**
     * @brief 请求预览信号
     */
    void previewRequested();

    /**
     * @brief 参数已应用信号
     */
    void parametersApplied();

private slots:
    // 参数变更
    void onThresholdTypeChanged(int index);
    void onThresholdMethodChanged(int index);
    void onThresholdChanged(int value);
    void onThresholdSliderChanged(int value);
    void onMaxValueChanged(int value);
    void onBlockSizeChanged(int value);
    void onConstantCChanged(double value);

    // 对话框操作
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createBasicParamsGroup(QVBoxLayout* layout);
    void createManualParamsGroup(QVBoxLayout* layout);
    void createAdaptiveParamsGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);
    void connectSignals();
    void updateUI();
    void loadParameters();
    void applyParameters();

private:
    // 工具指针
    Algorithm::ThresholdTool* tool_;

    // 预览图像
    Base::ImageData::Ptr previewImage_;

    // UI组件 - 基本参数
    QComboBox* thresholdTypeCombo_;
    QComboBox* thresholdMethodCombo_;

    // UI组件 - 手动阈值参数
    QGroupBox* manualGroup_;
    QSpinBox* thresholdSpin_;
    QSlider* thresholdSlider_;
    QSpinBox* maxValueSpin_;
    QLabel* computedThresholdLabel_;

    // UI组件 - 自适应参数
    QGroupBox* adaptiveGroup_;
    QSpinBox* blockSizeSpin_;
    QDoubleSpinBox* constantCSpin_;

    // UI组件 - 按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
