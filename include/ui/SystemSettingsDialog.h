/**
 * @file SystemSettingsDialog.h
 * @brief 系统设置对话框
 * @details VisionASM风格的系统配置界面
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QStackedWidget>

namespace VisionForge {
namespace UI {

/**
 * @class SystemSettingsDialog
 * @brief 系统设置对话框 - VisionASM风格
 *
 * 功能页面：
 * - 图像：图像存储设置
 * - 日志：日志配置
 * - 布局：界面布局设置
 * - 平台：平台参数配置
 */
class SystemSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemSettingsDialog(QWidget* parent = nullptr);
    ~SystemSettingsDialog() override = default;

private slots:
    void onToolButtonClicked(int index);
    void onBrowsePath();
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupToolBar();
    void setupImagePage();
    void setupLogPage();
    void setupLayoutPage();
    void setupPlatformPage();
    void loadSettings();
    void applySettings();

private:
    // 顶部工具栏
    QWidget* toolBar_ = nullptr;
    QPushButton* btnImage_ = nullptr;
    QPushButton* btnLog_ = nullptr;
    QPushButton* btnLayout_ = nullptr;
    QPushButton* btnPlatform_ = nullptr;
    QPushButton* btnBianJian_ = nullptr;
    QButtonGroup* toolButtonGroup_ = nullptr;

    // 页面切换
    QStackedWidget* stackedWidget_ = nullptr;

    // ========== 图像页面 ==========
    QWidget* imagePage_ = nullptr;

    // 存储模式
    QGroupBox* storageModeGroup_ = nullptr;
    QRadioButton* radioFlowSave_ = nullptr;
    QRadioButton* radioThreadSave_ = nullptr;
    QSpinBox* diskUsageSpin_ = nullptr;

    // 存图路径
    QGroupBox* savePathGroup_ = nullptr;
    QPushButton* btnBrowsePath_ = nullptr;
    QLineEdit* savePathEdit_ = nullptr;

    // 时刻存图
    QGroupBox* timeSaveGroup_ = nullptr;
    QCheckBox* chkTimeSave_ = nullptr;
    QRadioButton* radioHourSave_ = nullptr;
    QRadioButton* radioDaySave_ = nullptr;
    QSpinBox* dailyStartTimeSpin_ = nullptr;
    QSpinBox* maxFoldersSpin_ = nullptr;

    // 图像文件
    QGroupBox* imageFileGroup_ = nullptr;
    QCheckBox* chkProductIdNaming_ = nullptr;
    QCheckBox* chkSaveFailedImages_ = nullptr;
    QSpinBox* failedImageCountSpin_ = nullptr;
    QCheckBox* chkSaveAllImages_ = nullptr;
    QSpinBox* allImageCountSpin_ = nullptr;
    QComboBox* imageFormatCombo_ = nullptr;

    // VDB文件
    QGroupBox* vdbFileGroup_ = nullptr;
    QCheckBox* chkVdbEnabled_ = nullptr;
    QCheckBox* chkVdbProductIdNaming_ = nullptr;
    QCheckBox* chkVdbSaveFailedImages_ = nullptr;
    QSpinBox* vdbFailedImageCountSpin_ = nullptr;
    QCheckBox* chkVdbSaveAllImages_ = nullptr;
    QSpinBox* vdbAllImageCountSpin_ = nullptr;
    QComboBox* vdbFormatCombo_ = nullptr;
    QSpinBox* vdbCompressionSpin_ = nullptr;

    // 自定义参数
    QGroupBox* customParamsGroup_ = nullptr;
    QCheckBox* chkCustomParams_ = nullptr;
    QSpinBox* lineWidthSpin_ = nullptr;
    QSpinBox* fontSizeSpin_ = nullptr;
    QSpinBox* labelSizeSpin_ = nullptr;

    // ========== 日志页面 ==========
    QWidget* logPage_ = nullptr;

    // ========== 布局页面 ==========
    QWidget* layoutPage_ = nullptr;

    // ========== 平台页面 ==========
    QWidget* platformPage_ = nullptr;

    // ========== 底部按钮 ==========
    QPushButton* okButton_ = nullptr;
    QPushButton* cancelButton_ = nullptr;

    // 工具栏按钮样式
    QString toolBtnNormalStyle_;
    QString toolBtnSelectedStyle_;
};

} // namespace UI
} // namespace VisionForge
