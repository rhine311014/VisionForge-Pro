/**
 * @file NinePointCalibDialog.h
 * @brief 九点标定对话框
 * @details 提供九点标定的交互界面，支持图像坐标到物理坐标的映射标定
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

#include "algorithm/NinePointCalibTool.h"
#include "algorithm/CalibrationResult.h"
#include "base/ImageData.h"
#include "ui/ImageViewer.h"

namespace VisionForge {
namespace UI {

/**
 * @class NinePointCalibDialog
 * @brief 九点标定对话框
 *
 * 功能：
 * - 设置标定点数量和物理坐标
 * - 在图像上点击采集图像坐标
 * - 执行标定并显示结果
 * - 提供坐标转换测试
 */
class NinePointCalibDialog : public QDialog {
    Q_OBJECT

public:
    explicit NinePointCalibDialog(QWidget* parent = nullptr);
    ~NinePointCalibDialog() override;

    /**
     * @brief 设置当前图像
     */
    void setCurrentImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 获取标定结果
     */
    Algorithm::CalibrationResult getCalibrationResult() const;

    /**
     * @brief 是否已完成标定
     */
    bool isCalibrated() const;

signals:
    /**
     * @brief 标定完成信号
     */
    void calibrationCompleted(const Algorithm::CalibrationResult& result);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 标定点管理
    void onPointCountChanged(int count);
    void onGenerateGrid();
    void onClearPoints();
    void onPointTableCellChanged(int row, int column);

    // 图像点击
    void onImageClicked(int x, int y);

    // 标定执行
    void onExecuteCalibration();
    void onTestTransform();

    // 参数变更
    void onCalibModeChanged(int index);
    void onBackendChanged(int index);

    // 对话框操作
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onSaveResult();
    void onLoadResult();

private:
    void createUI();
    void createImagePreviewArea(QSplitter* splitter);
    void createControlPanel(QSplitter* splitter);
    void createCalibModeGroup(QVBoxLayout* layout);
    void createPointsGroup(QVBoxLayout* layout);
    void createGridGeneratorGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createTestGroup(QVBoxLayout* layout);
    void createButtons(QVBoxLayout* layout);

    void connectSignals();
    void updateUI();
    void updatePointsTable();
    void updateResultDisplay();
    void applyParameters();
    void drawCalibPoints();

    // 当前选中的点索引（用于图像点击赋值）
    int currentPointIndex_;

private:
    // 标定工具
    Algorithm::NinePointCalibTool* calibTool_;

    // 当前图像
    Base::ImageData::Ptr currentImage_;

    // UI组件 - 图像预览
    ImageViewer* imageViewer_;
    QLabel* instructionLabel_;

    // UI组件 - 标定模式
    QComboBox* calibModeCombo_;
    QComboBox* backendCombo_;

    // UI组件 - 标定点
    QSpinBox* pointCountSpin_;
    QTableWidget* pointsTable_;
    QPushButton* clearPointsBtn_;

    // UI组件 - 网格生成器
    QSpinBox* gridRowsSpin_;
    QSpinBox* gridColsSpin_;
    QDoubleSpinBox* gridSpacingXSpin_;
    QDoubleSpinBox* gridSpacingYSpin_;
    QDoubleSpinBox* gridOriginXSpin_;
    QDoubleSpinBox* gridOriginYSpin_;
    QPushButton* generateGridBtn_;

    // UI组件 - 标定结果
    QLabel* pixelScaleXLabel_;
    QLabel* pixelScaleYLabel_;
    QLabel* rotationAngleLabel_;
    QLabel* calibErrorLabel_;
    QPushButton* saveResultBtn_;
    QPushButton* loadResultBtn_;

    // UI组件 - 坐标转换测试
    QDoubleSpinBox* testImageXSpin_;
    QDoubleSpinBox* testImageYSpin_;
    QLabel* testWorldXLabel_;
    QLabel* testWorldYLabel_;
    QPushButton* testTransformBtn_;

    // UI组件 - 对话框按钮
    QPushButton* calibrateBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
