/**
 * @file OptionsDialog.h
 * @brief 选项设置对话框头文件
 * @details 定义系统选项设置对话框类，提供多页面的设置界面，包括：
 *          - 基础设置：语言、图像显示参数、启动选项、工位名称
 *          - 光源控制：多个光源控制器的配置和实时控制
 *          - 工位布局：工位显示布局的选择
 *          对话框采用工具栏+堆叠页面的导航方式。
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
 * @brief 选项设置对话框类
 * @details 系统的综合设置界面，继承自QDialog，提供模态对话框形式的设置功能。
 *
 * 功能页面说明：
 * - **基础设置页**：
 *   - 语言设置：简体中文/English切换
 *   - 图像显示：抽帧数量、显示分辨率
 *   - 启动设置：相机通道、虚拟键盘、通信显示等开关
 *   - 工位名称：自定义各工位的显示名称
 *
 * - **光源控制页**：
 *   - 支持4个独立的光源控制器配置
 *   - 每个控制器：启用开关、类型选择、通信参数
 *   - 8通道亮度滑块和开关控制
 *   - 实时连接和控制功能
 *
 * - **工位布局页**：
 *   - 提供6种预设布局类型
 *   - 可视化预览布局效果
 *
 * 使用方法：
 * @code
 * OptionsDialog dialog(parentWidget);
 * if (dialog.exec() == QDialog::Accepted) {
 *     // 用户点击确定，设置已保存
 * }
 * @endcode
 */
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     */
    explicit OptionsDialog(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     * @note 使用默认析构，Qt的父子对象机制会自动清理子控件
     */
    ~OptionsDialog() override = default;

private slots:
    // ========== 导航相关槽函数 ==========

    /**
     * @brief 工具栏按钮点击响应
     * @param index 按钮索引（0=基础设置, 1=光源控制, 2=工位布局）
     * @details 切换显示对应的设置页面，并更新按钮选中样式
     */
    void onToolButtonClicked(int index);

    /**
     * @brief 确定按钮点击响应
     * @details 应用所有设置并关闭对话框
     */
    void onOkClicked();

    /**
     * @brief 取消按钮点击响应
     * @details 放弃修改并关闭对话框
     */
    void onCancelClicked();

    // ========== 光源控制相关槽函数 ==========

    /**
     * @brief 光源控制器标签页切换响应
     * @param tabIndex 新的标签页索引（0-3对应4个控制器）
     * @details 切换到指定控制器的配置页面，同步UI显示
     */
    void onLightControllerTabChanged(int tabIndex);

    /**
     * @brief 光源控制器启用状态改变响应
     * @param enabled 新的启用状态
     * @details 更新配置并刷新UI控件的可用状态
     */
    void onLightEnableChanged(bool enabled);

    /**
     * @brief 光源控制器类型改变响应
     * @param index 下拉框选中索引
     * @details 更新控制器类型配置，并调整亮度滑块范围
     */
    void onLightTypeChanged(int index);

    /**
     * @brief 通信类型改变响应
     * @param index 下拉框选中索引（0=串口, 1=网口）
     * @details 切换串口/网口配置区域的显示
     */
    void onCommTypeChanged(int index);

    /**
     * @brief 打开/关闭光源按钮点击响应
     * @details 根据当前连接状态执行打开或关闭操作，更新按钮文字和样式
     */
    void onOpenLightClicked();

    /**
     * @brief 光源通道亮度滑块值改变响应
     * @param channelIndex 通道索引（0-7）
     * @param value 新的亮度值
     * @details 更新配置并实时发送亮度命令到控制器
     */
    void onLightSliderValueChanged(int channelIndex, int value);

    /**
     * @brief 全部开启通道按钮点击响应
     * @details 开启当前控制器的所有通道
     */
    void onEnableAllChannelsClicked();

    /**
     * @brief 全部关闭通道按钮点击响应
     * @details 关闭当前控制器的所有通道
     */
    void onDisableAllChannelsClicked();

private:
    // ========== UI构建函数 ==========

    /**
     * @brief 构建整体UI布局
     * @details 创建主布局、工具栏、页面区域和底部按钮
     */
    void setupUI();

    /**
     * @brief 构建顶部工具栏
     * @details 创建页面导航按钮（基础设置、光源控制、工位布局）
     */
    void setupToolBar();

    /**
     * @brief 构建基础设置页面
     * @details 创建语言、图像显示、启动设置、工位名称等配置区域
     */
    void setupBasicPage();

    /**
     * @brief 构建光源控制页面
     * @details 创建包含4个控制器标签页的TabWidget
     */
    void setupLightPage();

    /**
     * @brief 构建工位布局页面
     * @details 创建6种布局类型的选择界面和预览
     */
    void setupLayoutPage();

    /**
     * @brief 加载已保存的设置
     * @details 从配置文件加载设置并更新UI显示
     */
    void loadSettings();

    /**
     * @brief 应用当前设置
     * @details 将UI上的设置保存到配置文件
     */
    void applySettings();

    // ========== 光源控制辅助函数 ==========

    /**
     * @brief 构建单个光源控制器的配置页面
     * @param index 控制器索引（0-3）
     * @param page 页面容器Widget
     * @details 创建连接设置区域和通道亮度控制区域
     */
    void setupLightControllerPage(int index, QWidget* page);

    /**
     * @brief 更新光源控制器UI的可用状态
     * @param index 控制器索引
     * @details 根据启用状态和连接状态更新各控件的enabled属性
     */
    void updateLightControllerUI(int index);

    /**
     * @brief 更新光源控制器的连接状态显示
     * @param index 控制器索引
     * @details 更新打开/关闭按钮的文字、样式和其他控件状态
     */
    void updateLightConnectionStatus(int index);

    /**
     * @brief 从UI同步配置到管理器
     * @param index 控制器索引
     * @details 读取UI控件的值并更新到LightControllerManager的配置
     */
    void syncLightConfigFromUI(int index);

    /**
     * @brief 从管理器同步配置到UI
     * @param index 控制器索引
     * @details 读取LightControllerManager的配置并更新UI控件显示
     */
    void syncLightConfigToUI(int index);

private:
    // ============================================================================
    // 顶部工具栏相关控件
    // ============================================================================

    QWidget* toolBar_ = nullptr;              ///< 工具栏容器Widget
    QPushButton* btnBasic_ = nullptr;         ///< 基础设置按钮
    QPushButton* btnLight_ = nullptr;         ///< 光源控制按钮
    QPushButton* btnLayout_ = nullptr;        ///< 工位布局按钮
    QButtonGroup* toolButtonGroup_ = nullptr; ///< 工具栏按钮组（互斥选择）

    // ============================================================================
    // 页面切换控件
    // ============================================================================

    QStackedWidget* stackedWidget_ = nullptr; ///< 堆叠页面容器（切换显示不同设置页）

    // ============================================================================
    // 基础设置页面控件
    // ============================================================================

    QWidget* basicPage_ = nullptr;            ///< 基础设置页面容器

    // ----- 语言设置 -----
    QGroupBox* languageGroup_ = nullptr;              ///< 语言设置分组框
    QRadioButton* radioChineseSimplified_ = nullptr;  ///< 简体中文单选按钮
    QRadioButton* radioEnglish_ = nullptr;            ///< English单选按钮

    // ----- 图像显示设置 -----
    QGroupBox* imageDisplayGroup_ = nullptr;          ///< 图像显示分组框
    QSpinBox* frameSkipSpin_ = nullptr;               ///< 抽帧数量输入框（1-100）
    QComboBox* displayResolutionCombo_ = nullptr;     ///< 显示分辨率下拉框

    // ----- 启动设置 -----
    QGroupBox* startupGroup_ = nullptr;               ///< 启动设置分组框
    QCheckBox* chkCameraChannel_ = nullptr;           ///< 相机通道设置复选框
    QCheckBox* chkVirtualKeyboard_ = nullptr;         ///< 虚拟屏幕键盘复选框
    QCheckBox* chkCommDisplay_ = nullptr;             ///< 通信指令显示复选框

    // ----- 工位名称设置 -----
    QGroupBox* stationNameGroup_ = nullptr;           ///< 工位名称设置分组框
    QTableWidget* stationNameTable_ = nullptr;        ///< 工位名称编辑表格（8行2列）

    // ============================================================================
    // 光源控制页面控件
    // ============================================================================

    QWidget* lightPage_ = nullptr;                    ///< 光源控制页面容器
    QTabWidget* lightControllerTabs_ = nullptr;       ///< 控制器标签页容器
    int currentLightControllerIndex_ = 0;             ///< 当前选中的控制器索引

    /**
     * @struct LightControllerPageWidgets
     * @brief 单个光源控制器页面的控件集合
     * @details 存储一个控制器标签页内所有控件的指针，便于统一管理和访问
     */
    struct LightControllerPageWidgets {
        QCheckBox* chkEnable = nullptr;           ///< 启用复选框
        QComboBox* typeCombo = nullptr;           ///< 控制器类型下拉框
        QComboBox* commTypeCombo = nullptr;       ///< 通信类型下拉框（串口/网口）
        QComboBox* serialPortCombo = nullptr;     ///< 串口号下拉框
        QComboBox* baudRateCombo = nullptr;       ///< 波特率下拉框
        QPushButton* btnOpen = nullptr;           ///< 打开/关闭按钮
        QPushButton* btnEnableAll = nullptr;      ///< 全部开启按钮
        QPushButton* btnDisableAll = nullptr;     ///< 全部关闭按钮
        QSlider* sliders[8] = {nullptr};          ///< 8个通道的亮度滑块
        QLabel* valueLabels[8] = {nullptr};       ///< 8个通道的当前值标签
        QLineEdit* valueEdits[8] = {nullptr};     ///< 8个通道的数值输入框
        QPushButton* channelBtns[8] = {nullptr};  ///< 8个通道的开关按钮
    };

    /// 4个控制器页面的控件集合数组
    LightControllerPageWidgets lightPageWidgets_[4];

    // ----- 兼容旧代码的指针（指向第一个控制器的控件） -----
    QCheckBox* chkLightEnable_ = nullptr;             ///< 启用复选框（控制器0）
    QComboBox* controllerTypeCombo_ = nullptr;        ///< 控制器类型下拉框（控制器0）
    QComboBox* commTypeCombo_ = nullptr;              ///< 通信类型下拉框（控制器0）
    QComboBox* serialPortCombo_ = nullptr;            ///< 串口号下拉框（控制器0）
    QComboBox* baudRateCombo_ = nullptr;              ///< 波特率下拉框（控制器0）
    QPushButton* btnOpenLight_ = nullptr;             ///< 打开按钮（控制器0）

    /// 8个通道的亮度滑块（控制器0，兼容旧代码）
    QSlider* lightSliders_[8] = {nullptr};
    /// 8个通道的值标签（控制器0，兼容旧代码）
    QLabel* lightValueLabels_[8] = {nullptr};
    /// 8个通道的值输入框（控制器0，兼容旧代码）
    QLineEdit* lightValueEdits_[8] = {nullptr};

    // ============================================================================
    // 工位布局页面控件
    // ============================================================================

    QWidget* layoutPage_ = nullptr;                   ///< 工位布局页面容器

    // ============================================================================
    // 底部按钮控件
    // ============================================================================

    QPushButton* okButton_ = nullptr;                 ///< 确定按钮
    QPushButton* cancelButton_ = nullptr;             ///< 取消按钮

    // ============================================================================
    // 样式表字符串
    // ============================================================================

    QString toolBtnNormalStyle_;                      ///< 工具栏按钮普通状态样式
    QString toolBtnSelectedStyle_;                    ///< 工具栏按钮选中状态样式
};

} // namespace UI
} // namespace VisionForge
