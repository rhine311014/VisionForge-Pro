/**
 * @file ColorConvertToolDialog.h
 * @brief 颜色转换工具参数设置对话框
 * @details 提供颜色空间转换工具的参数配置界面
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include "algorithm/ColorConvertTool.h"
#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {
namespace UI {

/**
 * @class ColorConvertToolDialog
 * @brief 颜色转换工具参数设置对话框
 *
 * 功能：
 * - 设置源颜色空间（BGR、RGB、HSV、HLS、Lab、YCrCb、XYZ、Grayscale）
 * - 设置目标颜色空间
 * - 设置是否提取单通道
 * - 选择要提取的通道
 * - 实时预览转换效果
 */
class ColorConvertToolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param tool 颜色转换工具指针
     * @param parent 父窗口
     */
    explicit ColorConvertToolDialog(Algorithm::ColorConvertTool* tool, QWidget* parent = nullptr);
    ~ColorConvertToolDialog() override;

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
    void onSourceSpaceChanged(int index);
    void onTargetSpaceChanged(int index);
    void onExtractChannelChanged(bool checked);
    void onChannelIndexChanged(int index);

    // 对话框操作
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onAutoPreview();

private:
    void createUI();
    void createColorSpaceGroup(QVBoxLayout* layout);
    void createChannelExtractGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);
    void connectSignals();
    void updateUI();
    void updateChannelCombo();
    void loadParameters();
    void applyParameters();

private:
    // 工具指针
    Algorithm::ColorConvertTool* tool_;

    // 预览图像
    Base::ImageData::Ptr previewImage_;

    // UI组件 - 颜色空间
    QComboBox* sourceSpaceCombo_;
    QComboBox* targetSpaceCombo_;

    // UI组件 - 通道提取
    QGroupBox* channelExtractGroup_;
    QCheckBox* extractChannelCheck_;
    QComboBox* channelIndexCombo_;

    // UI组件 - 按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;

    // 实时预览
    PreviewHelper* previewHelper_;
    QCheckBox* autoPreviewCheck_;
};

} // namespace UI
} // namespace VisionForge
