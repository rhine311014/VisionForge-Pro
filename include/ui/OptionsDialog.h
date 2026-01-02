/**
 * @file OptionsDialog.h
 * @brief 选项设置对话框
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
#include <QCheckBox>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTableWidget>
#include <QSlider>
#include <QTabWidget>
#include <QMap>

namespace VisionForge {
namespace UI {

/**
 * @class OptionsDialog
 * @brief 选项设置对话框
 *
 * 功能页面：
 * - 基础设置：语言、图像显示、启动设置、工位名称
 * - 光源控制：光源参数配置
 * - 工位布局：工位布局设置
 */
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget* parent = nullptr);
    ~OptionsDialog() override = default;

private slots:
    void onToolButtonClicked(int index);
    void onOkClicked();
    void onCancelClicked();

    // 光源控制相关槽函数
    void onLightControllerTabChanged(int tabIndex);
    void onLightEnableChanged(bool enabled);
    void onLightTypeChanged(int index);
    void onCommTypeChanged(int index);
    void onOpenLightClicked();
    void onLightSliderValueChanged(int channelIndex, int value);
    void onEnableAllChannelsClicked();
    void onDisableAllChannelsClicked();

private:
    void setupUI();
    void setupToolBar();
    void setupBasicPage();
    void setupLightPage();
    void setupLayoutPage();
    void loadSettings();
    void applySettings();

    // 光源控制辅助函数
    void setupLightControllerPage(int index, QWidget* page);
    void updateLightControllerUI(int index);
    void updateLightConnectionStatus(int index);
    void syncLightConfigFromUI(int index);
    void syncLightConfigToUI(int index);

private:
    // 顶部工具栏
    QWidget* toolBar_ = nullptr;
    QPushButton* btnBasic_ = nullptr;
    QPushButton* btnLight_ = nullptr;
    QPushButton* btnLayout_ = nullptr;
    QButtonGroup* toolButtonGroup_ = nullptr;

    // 页面切换
    QStackedWidget* stackedWidget_ = nullptr;

    // ========== 基础设置页面 ==========
    QWidget* basicPage_ = nullptr;

    // 语言设置
    QGroupBox* languageGroup_ = nullptr;
    QRadioButton* radioChineseSimplified_ = nullptr;
    QRadioButton* radioEnglish_ = nullptr;

    // 图像显示
    QGroupBox* imageDisplayGroup_ = nullptr;
    QSpinBox* frameSkipSpin_ = nullptr;
    QComboBox* displayResolutionCombo_ = nullptr;

    // 启动设置
    QGroupBox* startupGroup_ = nullptr;
    QCheckBox* chkCameraChannel_ = nullptr;
    QCheckBox* chkVirtualKeyboard_ = nullptr;
    QCheckBox* chkCommDisplay_ = nullptr;

    // 工位名称设置
    QGroupBox* stationNameGroup_ = nullptr;
    QTableWidget* stationNameTable_ = nullptr;

    // ========== 光源控制页面 ==========
    QWidget* lightPage_ = nullptr;
    QTabWidget* lightControllerTabs_ = nullptr;
    int currentLightControllerIndex_ = 0;

    // 每个控制器页面的控件结构
    struct LightControllerPageWidgets {
        QCheckBox* chkEnable = nullptr;
        QComboBox* typeCombo = nullptr;
        QComboBox* commTypeCombo = nullptr;
        QComboBox* serialPortCombo = nullptr;
        QComboBox* baudRateCombo = nullptr;
        QPushButton* btnOpen = nullptr;
        QPushButton* btnEnableAll = nullptr;
        QPushButton* btnDisableAll = nullptr;
        QSlider* sliders[8] = {nullptr};
        QLabel* valueLabels[8] = {nullptr};
        QLineEdit* valueEdits[8] = {nullptr};
        QPushButton* channelBtns[8] = {nullptr};
    };

    // 4个控制器页面的控件
    LightControllerPageWidgets lightPageWidgets_[4];

    // 兼容旧代码的指针（指向第一个控制器）
    QCheckBox* chkLightEnable_ = nullptr;
    QComboBox* controllerTypeCombo_ = nullptr;
    QComboBox* commTypeCombo_ = nullptr;
    QComboBox* serialPortCombo_ = nullptr;
    QComboBox* baudRateCombo_ = nullptr;
    QPushButton* btnOpenLight_ = nullptr;

    // 光源通道亮度滑块和输入框（第一个控制器）
    QSlider* lightSliders_[8] = {nullptr};
    QLabel* lightValueLabels_[8] = {nullptr};
    QLineEdit* lightValueEdits_[8] = {nullptr};

    // ========== 工位布局页面 ==========
    QWidget* layoutPage_ = nullptr;

    // ========== 底部按钮 ==========
    QPushButton* okButton_ = nullptr;
    QPushButton* cancelButton_ = nullptr;

    // 工具栏按钮样式
    QString toolBtnNormalStyle_;
    QString toolBtnSelectedStyle_;
};

} // namespace UI
} // namespace VisionForge
