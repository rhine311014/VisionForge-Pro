/**
 * @file BlurToolDialog.h
 * @brief 图像模糊工具参数设置对话框
 * @details 提供图像模糊工具的参数配置界面
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

#include "algorithm/BlurTool.h"
#include "base/ImageData.h"

namespace VisionForge {
namespace UI {

/**
 * @class BlurToolDialog
 * @brief 图像模糊工具参数设置对话框
 *
 * 功能：
 * - 设置模糊类型（均值、高斯、中值、双边滤波）
 * - 设置核大小（奇数）
 * - 设置高斯sigma值
 * - 设置双边滤波参数（sigmaColor, sigmaSpace）
 * - 实时预览模糊效果
 */
class BlurToolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param tool 模糊工具指针
     * @param parent 父窗口
     */
    explicit BlurToolDialog(Algorithm::BlurTool* tool, QWidget* parent = nullptr);
    ~BlurToolDialog() override;

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
    void onBlurTypeChanged(int index);
    void onKernelSizeChanged(int value);
    void onSigmaChanged(double value);
    void onSigmaColorChanged(double value);
    void onSigmaSpaceChanged(double value);

    // 对话框操作
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createParamsGroup(QVBoxLayout* layout);
    void createGaussianParamsGroup(QVBoxLayout* layout);
    void createBilateralParamsGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);
    void connectSignals();
    void updateUI();
    void loadParameters();
    void applyParameters();

private:
    // 工具指针
    Algorithm::BlurTool* tool_;

    // 预览图像
    Base::ImageData::Ptr previewImage_;

    // UI组件 - 基本参数
    QComboBox* blurTypeCombo_;
    QSpinBox* kernelSizeSpin_;

    // UI组件 - 高斯参数
    QGroupBox* gaussianGroup_;
    QDoubleSpinBox* sigmaSpin_;

    // UI组件 - 双边滤波参数
    QGroupBox* bilateralGroup_;
    QDoubleSpinBox* sigmaColorSpin_;
    QDoubleSpinBox* sigmaSpaceSpin_;

    // UI组件 - 按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
