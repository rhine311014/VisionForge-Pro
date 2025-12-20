/**
 * @file BlobToolDialog.h
 * @brief Blob分析工具参数编辑对话框
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
#include <QSplitter>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableWidget>
#include "base/ImageData.h"
#include "ui/PreviewHelper.h"

namespace VisionForge {

// 前向声明
namespace Algorithm {
    class BlobTool;
}

namespace UI {

class HalconImageViewer;

/**
 * @class BlobToolDialog
 * @brief Blob分析工具参数编辑对话框
 *
 * 包含左侧图像显示窗口和右侧参数设置面板
 * 支持连通域分析、特征提取和过滤
 */
class BlobToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlobToolDialog(Algorithm::BlobTool* tool, QWidget* parent = nullptr);
    ~BlobToolDialog() override;

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
    // 基本参数槽
    void onBackendChanged(int index);
    void onConnectivityChanged(int index);
    void onPolarityChanged(int index);
    void onSortByChanged(int index);
    void onSortDescendingChanged(bool checked);
    void onMaxCountChanged(int value);

    // 二值化参数槽
    void onThresholdChanged(int value);
    void onAutoThresholdChanged(bool checked);

    // 过滤参数槽
    void onAreaRangeChanged();
    void onCircularityRangeChanged();

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
    void createBasicParamGroup(QVBoxLayout* layout);
    void createThresholdGroup(QVBoxLayout* layout);
    void createFilterGroup(QVBoxLayout* layout);
    void createSortGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateResults();

private:
    Algorithm::BlobTool* tool_;
    Base::ImageData::Ptr currentImage_;

    // 左侧图像显示
    HalconImageViewer* imageViewer_;
    QSplitter* mainSplitter_;

    // 右侧选项卡
    QTabWidget* paramTabWidget_;

    // 基本参数
    QComboBox* backendCombo_;
    QComboBox* connectivityCombo_;
    QComboBox* polarityCombo_;

    // 二值化参数
    QGroupBox* thresholdGroup_;
    QCheckBox* autoThresholdCheckBox_;
    QSpinBox* thresholdSpinBox_;
    QSlider* thresholdSlider_;

    // 过滤参数
    QDoubleSpinBox* minAreaSpinBox_;
    QDoubleSpinBox* maxAreaSpinBox_;
    QDoubleSpinBox* minCircularitySpinBox_;
    QDoubleSpinBox* maxCircularitySpinBox_;

    // 排序参数
    QComboBox* sortByCombo_;
    QCheckBox* sortDescendingCheckBox_;
    QSpinBox* maxCountSpinBox_;

    // 结果表格
    QTableWidget* resultsTable_;

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
