/**
 * @file ModelCheckPointDialog.h
 * @brief 模型校验点编辑对话框
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include "base/ImageData.h"

namespace VisionForge {

namespace Algorithm {
    class ShapeMatchTool;
    struct ModelCheckPoint;
}

namespace UI {

class HalconImageViewer;

/**
 * @class ModelCheckPointDialog
 * @brief 模型校验点编辑对话框
 *
 * 功能:
 * - 添加、删除、编辑校验点
 * - 在图像上可视化校验点位置
 * - 从图像上交互式选取校验点
 * - 测试校验点验证
 */
class ModelCheckPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModelCheckPointDialog(Algorithm::ShapeMatchTool* tool, QWidget* parent = nullptr);
    ~ModelCheckPointDialog() override;

    /**
     * @brief 设置显示图像
     */
    void setImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 设置匹配结果位置（用于显示校验点的绝对位置）
     */
    void setMatchResult(double row, double col, double angle);

signals:
    void checkPointsChanged();

private slots:
    void onAddCheckPointClicked();
    void onRemoveCheckPointClicked();
    void onClearAllClicked();
    void onPickFromImageClicked();
    void onTestValidateClicked();
    void onEnableCheckPointsChanged(bool enabled);
    void onTableSelectionChanged();
    void onTableCellChanged(int row, int column);
    void onImageClicked(double row, double col);
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createImagePanel(QVBoxLayout* layout);
    void createCheckPointTable(QVBoxLayout* layout);
    void createEditPanel(QVBoxLayout* layout);
    void createButtonPanel(QVBoxLayout* layout);
    void connectSignals();
    void updateTable();
    void updateVisualization();
    void applyChanges();
    void loadCheckPoints();

private:
    Algorithm::ShapeMatchTool* tool_;
    Base::ImageData::Ptr currentImage_;
    std::vector<Algorithm::ModelCheckPoint> checkPoints_;  // 临时编辑的校验点列表

    // 匹配结果位置
    double matchRow_;
    double matchCol_;
    double matchAngle_;
    bool hasMatchResult_;

    // 交互状态
    bool pickingMode_;  // 是否处于从图像选取校验点模式
    int lastMouseRow_;  // 上次鼠标位置
    int lastMouseCol_;

    // UI组件
    HalconImageViewer* imageViewer_;
    QTableWidget* checkPointTable_;
    QCheckBox* enableCheckPointsCheckBox_;

    // 编辑面板
    QLineEdit* nameEdit_;
    QDoubleSpinBox* relRowSpinBox_;
    QDoubleSpinBox* relColSpinBox_;
    QSpinBox* expectedGraySpinBox_;
    QSpinBox* toleranceSpinBox_;
    QCheckBox* pointEnabledCheckBox_;

    // 按钮
    QPushButton* addBtn_;
    QPushButton* removeBtn_;
    QPushButton* clearAllBtn_;
    QPushButton* pickFromImageBtn_;
    QPushButton* testValidateBtn_;

    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
