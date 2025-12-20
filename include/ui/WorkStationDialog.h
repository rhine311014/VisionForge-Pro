/**
 * @file WorkStationDialog.h
 * @brief 工位配置对话框 - 参考VisionASM设计
 * @details 配置位置、对位参数、补偿等
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QDialog>
#include "core/WorkStation.h"

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
class QListWidget;

namespace VisionForge {
namespace UI {

/**
 * @class WorkStationDialog
 * @brief 工位配置对话框
 *
 * 功能：
 * - 位置管理（Target/Object）
 * - 对位方式配置
 * - 精度要求设置
 * - 补偿参数管理
 */
class WorkStationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkStationDialog(QWidget* parent = nullptr);
    ~WorkStationDialog() override;

    /**
     * @brief 设置工位
     */
    void setWorkStation(Core::WorkStation* ws);

    /**
     * @brief 获取工位
     */
    Core::WorkStation* workStation() const { return workStation_; }

signals:
    void configurationChanged();

private slots:
    // 位置管理
    void onAddTargetPosition();
    void onAddObjectPosition();
    void onRemovePosition();
    void onEditPosition();
    void onPositionSelectionChanged();

    // 对位配置
    void onAlignMethodChanged(int index);
    void onPrecisionChanged();

    // 补偿管理
    void onAddOffset();
    void onRemoveOffset();
    void onEditOffset();
    void onOffsetSelectionChanged();
    void onCurrentOffsetChanged(int index);

    // 操作
    void onApply();
    void onSaveConfig();
    void onLoadConfig();

private:
    void setupUI();
    void createPositionTab();
    void createAlignmentTab();
    void createOffsetTab();
    void createButtonGroup();

    void updatePositionTable();
    void updatePositionDetails();
    void updateOffsetTable();
    void updateFromWorkStation();
    void applyToWorkStation();

    QString getPositionTypeText(Core::PositionType type) const;
    QString getAlignMethodText(Core::AlignmentMethod method) const;

private:
    Core::WorkStation* workStation_;

    // 标签页
    QTabWidget* tabWidget_;

    // ========== 位置配置页 ==========
    QWidget* positionTab_;
    QTableWidget* positionTable_;
    QPushButton* addTargetBtn_;
    QPushButton* addObjectBtn_;
    QPushButton* removePositionBtn_;
    QPushButton* editPositionBtn_;

    // 位置详情
    QGroupBox* positionDetailGroup_;
    QLineEdit* posNameEdit_;
    QComboBox* posTypeCombo_;
    QComboBox* posCameraCombo_;
    QComboBox* posCalibCombo_;
    QSpinBox* posIndexSpin_;
    QCheckBox* posEnabledCheck_;

    // 平台坐标
    QGroupBox* platPosGroup_;
    QDoubleSpinBox* platXSpin_;
    QDoubleSpinBox* platYSpin_;
    QDoubleSpinBox* platDSpin_;

    // ========== 对位配置页 ==========
    QWidget* alignmentTab_;

    // 对位方式
    QGroupBox* alignMethodGroup_;
    QComboBox* alignMethodCombo_;
    QCheckBox* iterativeAlignCheck_;
    QCheckBox* useHomographyCheck_;

    // 精度要求
    QGroupBox* precisionGroup_;
    QDoubleSpinBox* precisionXSpin_;
    QDoubleSpinBox* precisionYSpin_;
    QDoubleSpinBox* precisionDSpin_;
    QSpinBox* maxIterationsSpin_;

    // 位置分配
    QGroupBox* positionAssignGroup_;
    QListWidget* targetPosList_;
    QListWidget* objectPosList_;

    // ========== 补偿配置页 ==========
    QWidget* offsetTab_;
    QTableWidget* offsetTable_;
    QComboBox* currentOffsetCombo_;
    QPushButton* addOffsetBtn_;
    QPushButton* removeOffsetBtn_;
    QPushButton* editOffsetBtn_;

    // 补偿详情
    QGroupBox* offsetDetailGroup_;
    QDoubleSpinBox* offsetXSpin_;
    QDoubleSpinBox* offsetYSpin_;
    QDoubleSpinBox* offsetDSpin_;
    QLineEdit* offsetDescEdit_;

    // 底部按钮
    QPushButton* saveConfigBtn_;
    QPushButton* loadConfigBtn_;
    QPushButton* applyBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;

    // 当前选中
    QString selectedPositionId_;
    int selectedOffsetIndex_;
};

} // namespace UI
} // namespace VisionForge
