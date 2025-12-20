/**
 * @file GrayToolDialog.h
 * @brief 灰度转换工具参数设置对话框
 * @details 提供灰度转换工具的参数配置界面
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>

#include "algorithm/GrayTool.h"
#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {
namespace UI {

/**
 * @class GrayToolDialog
 * @brief 灰度转换工具参数设置对话框
 *
 * 功能：
 * - 设置转换模式（平均法、加权法、去饱和法、单通道）
 * - 选择提取的通道（蓝、绿、红）
 * - 实时预览转换效果
 */
class GrayToolDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param tool 灰度转换工具指针
     * @param parent 父窗口
     */
    explicit GrayToolDialog(Algorithm::GrayTool* tool, QWidget* parent = nullptr);
    ~GrayToolDialog() override;

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
    void onConvertModeChanged(int index);
    void onChannelChanged(int index);

    // 对话框操作
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onAutoPreview();

private:
    void createUI();
    void createParamsGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);
    void connectSignals();
    void updateUI();
    void loadParameters();
    void applyParameters();

private:
    // 工具指针
    Algorithm::GrayTool* tool_;

    // 预览图像
    Base::ImageData::Ptr previewImage_;

    // UI组件 - 参数
    QComboBox* convertModeCombo_;
    QComboBox* channelCombo_;
    QLabel* channelLabel_;

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
