/**
 * @file FindEdgeToolDialog.h
 * @brief 边缘查找工具参数编辑对话框
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
#include <QSplitter>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class FindEdgeTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class FindEdgeToolDialog
 * @brief 边缘查找工具参数编辑对话框
 *
 * 包含左侧图像显示窗口和右侧参数设置面板
 * 支持线性搜索和矩形区域搜索两种模式
 */
class FindEdgeToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindEdgeToolDialog(Algorithm::FindEdgeTool* tool, QWidget* parent = nullptr);
    ~FindEdgeToolDialog() override;

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
    // 参数槽
    void onSearchModeChanged(int index);
    void onEdgeTypeChanged(int index);
    void onSelectModeChanged(int index);
    void onMinAmplitudeChanged(double value);
    void onSigmaChanged(double value);
    void onSearchWidthChanged(int value);

    // 搜索区域设置槽
    void onDrawSearchLineClicked();
    void onDrawSearchRectClicked();
    void onClearSearchRegionClicked();

    // 预览按钮槽
    void onPreviewClicked();
    void onAutoPreview();

    // 对话框按钮槽
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createSearchModeGroup(QVBoxLayout* layout);
    void createSearchRegionGroup(QVBoxLayout* layout);
    void createEdgeParamGroup(QVBoxLayout* layout);
    void createFilterParamGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateSearchModeVisibility();

private:
    Algorithm::FindEdgeTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 左侧图像显示
    HalconImageViewer* imageViewer_;
    QSplitter* mainSplitter_;

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 搜索模式
    QComboBox* searchModeCombo_;

    // 搜索区域设置
    QGroupBox* lineSearchGroup_;
    QPushButton* drawSearchLineBtn_;
    QSpinBox* searchWidthSpinBox_;
    QLabel* lineStatusLabel_;

    QGroupBox* rectSearchGroup_;
    QPushButton* drawSearchRectBtn_;
    QLabel* rectStatusLabel_;
    QDoubleSpinBox* rectAngleSpinBox_;

    QPushButton* clearSearchRegionBtn_;

    // 边缘检测参数
    QComboBox* edgeTypeCombo_;
    QComboBox* selectModeCombo_;
    QDoubleSpinBox* minAmplitudeSpinBox_;

    // 滤波参数
    QDoubleSpinBox* sigmaSpinBox_;

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
