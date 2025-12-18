/**
 * @file MorphologyToolDialog.h
 * @brief 形态学处理工具参数设置对话框
 * @details 提供形态学处理工具的参数配置界面
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include "algorithm/MorphologyTool.h"
#include "base/ImageData.h"

namespace VisionForge {
namespace UI {

/**
 * @class MorphologyToolDialog
 * @brief 形态学处理工具参数设置对话框
 *
 * 功能：
 * - 设置形态学操作类型（腐蚀、膨胀、开运算、闭运算、梯度、顶帽、黑帽）
 * - 设置结构元素形状（矩形、十字形、椭圆形）
 * - 设置结构元素尺寸（宽、高）
 * - 设置迭代次数
 * - 实时预览形态学处理效果
 */
class MorphologyToolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param tool 形态学处理工具指针
     * @param parent 父窗口
     */
    explicit MorphologyToolDialog(Algorithm::MorphologyTool* tool, QWidget* parent = nullptr);
    ~MorphologyToolDialog() override;

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
    void onMorphTypeChanged(int index);
    void onKernelShapeChanged(int index);
    void onKernelWidthChanged(int value);
    void onKernelHeightChanged(int value);
    void onIterationsChanged(int value);

    // 对话框操作
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createOperationParamsGroup(QVBoxLayout* layout);
    void createKernelParamsGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);
    void connectSignals();
    void updateUI();
    void loadParameters();
    void applyParameters();

private:
    // 工具指针
    Algorithm::MorphologyTool* tool_;

    // 预览图像
    Base::ImageData::Ptr previewImage_;

    // UI组件 - 操作参数
    QComboBox* morphTypeCombo_;
    QSpinBox* iterationsSpin_;

    // UI组件 - 结构元素参数
    QComboBox* kernelShapeCombo_;
    QSpinBox* kernelWidthSpin_;
    QSpinBox* kernelHeightSpin_;

    // UI组件 - 按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
