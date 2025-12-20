/**
 * @file MultiPointAlignmentDialog.h
 * @brief 多点对位配置对话框
 * @details 配置多点对位检测参数、ROI、偏差计算模式等
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QDialog>
#include <QMap>
#include "algorithm/MultiPointAlignmentTool.h"

class QTableWidget;
class QComboBox;
class QPushButton;
class QDoubleSpinBox;
class QSpinBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QTabWidget;

namespace VisionForge {
namespace UI {

/**
 * @class MultiPointAlignmentDialog
 * @brief 多点对位配置对话框
 *
 * 功能：
 * - 对位点列表管理（添加/删除/编辑）
 * - 检测类型配置
 * - ROI设置
 * - 标准位置设置
 * - 偏差计算模式选择
 * - 坐标转换设置
 */
class MultiPointAlignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiPointAlignmentDialog(QWidget* parent = nullptr);
    ~MultiPointAlignmentDialog() override;

    /**
     * @brief 设置对位工具
     */
    void setAlignmentTool(Algorithm::MultiPointAlignmentTool* tool);

    /**
     * @brief 获取对位工具
     */
    Algorithm::MultiPointAlignmentTool* getAlignmentTool() const;

signals:
    /**
     * @brief 配置已更改信号
     */
    void configurationChanged();

    /**
     * @brief 请求设置ROI
     */
    void requestSetROI(const QString& pointId);

    /**
     * @brief 请求创建模板
     */
    void requestCreateTemplate(const QString& pointId);

private slots:
    // 对位点管理
    void onAddPoint();
    void onRemovePoint();
    void onEditPoint();
    void onPointSelectionChanged();
    void onMovePointUp();
    void onMovePointDown();

    // ROI和模板
    void onSetROI();
    void onCreateTemplate();
    void onClearTemplate();

    // 偏差计算
    void onDeviationModeChanged(int index);
    void onAutoRotationCenterChanged(bool checked);
    void onRotationCenterChanged();

    // 坐标转换
    void onCoordTransformEnabledChanged(bool enabled);
    void onLoadCalibration();

    // 测试
    void onTestDetection();

    // 配置
    void onApply();

private:
    void setupUI();
    void createPointListTab();
    void createDeviationSettingsTab();
    void createCoordinateTab();
    void createButtonGroup();

    void updatePointTable();
    void updatePointDetails();
    void updateFromTool();
    void applyToTool();

    QString getPointTypeText(Algorithm::AlignmentPointType type) const;
    QString getDeviationModeText(Algorithm::DeviationCalcMode mode) const;

private:
    Algorithm::MultiPointAlignmentTool* alignmentTool_;

    // 标签页
    QTabWidget* tabWidget_;

    // 对位点列表页
    QWidget* pointListTab_;
    QTableWidget* pointTable_;
    QPushButton* addPointBtn_;
    QPushButton* removePointBtn_;
    QPushButton* editPointBtn_;
    QPushButton* moveUpBtn_;
    QPushButton* moveDownBtn_;

    // 点详情
    QGroupBox* pointDetailGroup_;
    QLineEdit* pointNameEdit_;
    QComboBox* pointTypeCombo_;
    QComboBox* pointCameraCombo_;
    QCheckBox* pointEnabledCheck_;

    // ROI设置
    QGroupBox* roiGroup_;
    QDoubleSpinBox* roiXSpin_;
    QDoubleSpinBox* roiYSpin_;
    QDoubleSpinBox* roiWidthSpin_;
    QDoubleSpinBox* roiHeightSpin_;
    QPushButton* setROIBtn_;

    // 标准位置
    QGroupBox* standardPosGroup_;
    QDoubleSpinBox* stdPosXSpin_;
    QDoubleSpinBox* stdPosYSpin_;
    QDoubleSpinBox* stdAngleSpin_;

    // 模板
    QGroupBox* templateGroup_;
    QLabel* templatePreviewLabel_;
    QPushButton* createTemplateBtn_;
    QPushButton* clearTemplateBtn_;

    // 偏差计算设置页
    QWidget* deviationTab_;
    QComboBox* deviationModeCombo_;
    QCheckBox* autoRotationCenterCheck_;
    QDoubleSpinBox* rotationCenterXSpin_;
    QDoubleSpinBox* rotationCenterYSpin_;

    // 坐标转换页
    QWidget* coordinateTab_;
    QCheckBox* coordTransformEnabledCheck_;
    QComboBox* calibrationCombo_;
    QPushButton* loadCalibrationBtn_;
    QLabel* calibrationInfoLabel_;

    // 底部按钮
    QPushButton* testBtn_;
    QPushButton* applyBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;

    // 当前选中
    QString selectedPointId_;
};

} // namespace UI
} // namespace VisionForge
