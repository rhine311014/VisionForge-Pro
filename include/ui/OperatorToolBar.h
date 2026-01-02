/**
 * @file OperatorToolBar.h
 * @brief 操作员底部工具栏类定义
 * @details 操作员工具栏是为操作员模式设计的简化操作界面，
 *          提供快速访问常用功能的大图标按钮栏，包括相机、通信、产品、系统等功能。
 *          工具栏固定显示在主窗口底部，采用大图标和醒目样式便于触屏快速操作。
 *
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-12-31
 *
 * @note 工具栏设计遵循以下原则：
 *       1. 大图标按钮，便于触屏操作
 *       2. 可配置的按钮显示/隐藏，适应不同场景
 *       3. 支持自定义图标和颜色
 *       4. 状态指示清晰，运行中/停止/激活等状态一目了然
 *
 * @see InfoBar 与操作员工具栏配合使用的顶部信息栏
 * @see MainWindow 主窗口集成操作员界面组件
 * @see UIModeManager UI模式管理器，控制工程师/操作员模式切换
 */

#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMap>

namespace VisionForge {
namespace UI {

/**
 * @class OperatorToolBar
 * @brief 操作员底部工具栏类
 *
 * @details 操作员工具栏提供了生产环境中操作员需要的核心操作入口：
 *
 * @par 主要功能：
 * - 相机按钮：打开相机配置界面
 * - 通信按钮：打开PLC/通信配置界面
 * - 产品按钮：切换产品/配方
 * - 系统按钮：打开系统设置
 * - 选项按钮：打开选项设置
 * - 帧有效/实时显示按钮：切换采集模式
 * - 运行按钮：开始/停止自动运行
 * - 登录按钮：用户登录/注销
 * - 退出按钮：退出程序
 * - 隐藏按钮：隐藏工具栏
 * - 工位按钮：切换工位
 *
 * @par 使用场景：
 * - 操作员模式下显示在主窗口底部
 * - 生产环境中的日常操作
 * - 触屏设备上的快速操作
 *
 * @par 设计特点：
 * - 大尺寸图标按钮，便于触屏操作
 * - 颜色编码区分不同状态和功能
 * - 可配置按钮的显示/隐藏
 * - 支持帧有效/实时显示两种采集模式
 *
 * @par 信号槽关系：
 * - 按钮点击信号用于通知主窗口执行相应操作
 * - 模式切换信号用于通知采集模式变化
 */
class OperatorToolBar : public QWidget
{
    Q_OBJECT

public:
    // ============================================================
    // 类型定义
    // ============================================================

    /**
     * @brief 按钮类型枚举
     *
     * @details 定义工具栏中所有可用的按钮类型，用于统一管理按钮。
     */
    enum ButtonType {
        Camera,         ///< 相机配置按钮
        Communication,  ///< 通信配置按钮
        Product,        ///< 产品/配方切换按钮
        System,         ///< 系统设置按钮
        Options,        ///< 选项设置按钮
        FrameLive,      ///< 帧有效/实时显示按钮（二合一按钮，可切换模式）
        Run,            ///< 运行/停止按钮
        Login,          ///< 登录/注销按钮
        Exit,           ///< 退出程序按钮
        Hide,           ///< 隐藏工具栏按钮
        Station         ///< 工位切换按钮
    };

    /**
     * @brief 帧有效/实时显示模式枚举
     *
     * @details 定义采集模式，用于控制图像采集和显示方式。
     *          - FrameValid：帧有效模式，等待外部触发信号后采集
     *          - LiveDisplay：实时显示模式，连续采集并显示
     */
    enum FrameLiveMode {
        FrameValid,     ///< 帧有效模式：等待触发信号采集
        LiveDisplay     ///< 实时显示模式：连续采集预览
    };

    // ============================================================
    // 构造与析构
    // ============================================================

    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     *
     * @details 初始化操作员工具栏，创建所有按钮并设置样式。
     *          默认显示所有按钮，可通过setButtonVisible()控制显示。
     */
    explicit OperatorToolBar(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~OperatorToolBar() override;

    // ============================================================
    // 按钮配置方法
    // ============================================================

    /**
     * @brief 设置按钮可见性
     * @param type 按钮类型
     * @param visible 是否可见
     *
     * @details 控制指定按钮的显示/隐藏状态。
     *          可用于根据用户权限或场景需求定制工具栏。
     */
    void setButtonVisible(ButtonType type, bool visible);

    /**
     * @brief 设置按钮启用状态
     * @param type 按钮类型
     * @param enabled 是否启用
     *
     * @details 控制指定按钮的启用/禁用状态。
     *          禁用的按钮会显示为灰色且不响应点击。
     */
    void setButtonEnabled(ButtonType type, bool enabled);

    /**
     * @brief 设置按钮文本
     * @param type 按钮类型
     * @param text 按钮文本
     *
     * @details 自定义指定按钮显示的文本内容。
     */
    void setButtonText(ButtonType type, const QString& text);

    /**
     * @brief 设置按钮图标大小
     * @param size 图标尺寸（像素）
     *
     * @details 统一设置所有按钮的图标大小。
     *          建议范围：32-64像素，默认48像素。
     */
    void setIconSize(int size);

    /**
     * @brief 获取当前图标大小
     * @return 图标尺寸（像素）
     */
    int iconSize() const { return iconSize_; }

    // ============================================================
    // 状态控制方法
    // ============================================================

    /**
     * @brief 设置运行状态
     * @param running true表示运行中，false表示已停止
     *
     * @details 更新运行按钮的显示状态：
     *          - 运行中：显示"停止"，红色高亮
     *          - 已停止：显示"运行"，正常样式
     */
    void setRunningState(bool running);

    /**
     * @brief 设置登录状态
     * @param loggedIn true表示已登录，false表示未登录
     * @param username 已登录用户名（可选）
     *
     * @details 更新登录按钮的显示状态：
     *          - 已登录：显示用户名或"注销"
     *          - 未登录：显示"登录"
     */
    void setLoginState(bool loggedIn, const QString& username = QString());

    /**
     * @brief 设置帧有效/实时显示模式
     * @param mode 采集模式
     *
     * @details 切换帧有效/实时显示按钮的当前模式。
     *          模式切换会更新按钮图标和文本。
     */
    void setFrameLiveMode(FrameLiveMode mode);

    /**
     * @brief 获取当前帧有效/实时显示模式
     * @return 当前采集模式
     */
    FrameLiveMode frameLiveMode() const { return frameLiveMode_; }

    /**
     * @brief 设置实时显示激活状态
     * @param active true表示实时显示已激活
     *
     * @details 当实时显示模式激活时，按钮会显示特殊样式。
     *          用于指示当前正在进行实时预览。
     */
    void setLiveDisplayActive(bool active);

signals:
    // ============================================================
    // 按钮点击信号 - 通知外部组件用户操作
    // ============================================================

    /**
     * @brief 相机按钮点击信号
     *
     * @details 用户点击相机按钮时发射，通知主窗口打开相机配置对话框。
     */
    void cameraClicked();

    /**
     * @brief 通信按钮点击信号
     *
     * @details 用户点击通信按钮时发射，通知主窗口打开PLC/通信配置对话框。
     */
    void communicationClicked();

    /**
     * @brief 产品按钮点击信号
     *
     * @details 用户点击产品按钮时发射，通知主窗口打开产品/配方选择对话框。
     */
    void productClicked();

    /**
     * @brief 系统按钮点击信号
     *
     * @details 用户点击系统按钮时发射，通知主窗口打开系统设置对话框。
     */
    void systemClicked();

    /**
     * @brief 选项按钮点击信号
     *
     * @details 用户点击选项按钮时发射，通知主窗口打开选项设置对话框。
     */
    void optionsClicked();

    /**
     * @brief 帧有效/实时显示按钮点击信号
     *
     * @details 用户点击帧有效/实时显示按钮时发射。
     *          具体行为取决于当前模式和激活状态。
     */
    void frameLiveClicked();

    /**
     * @brief 模式切换信号
     * @param mode 新的采集模式
     *
     * @details 当采集模式发生切换时发射（通过右键或长按切换）。
     */
    void frameLiveModeChanged(FrameLiveMode mode);

    /**
     * @brief 帧有效触发信号
     *
     * @details 在帧有效模式下点击按钮时发射。
     *          通知系统执行一次触发采集。
     */
    void frameValidTriggered();

    /**
     * @brief 实时显示切换信号
     * @param enabled true表示开启实时显示
     *
     * @details 在实时显示模式下点击按钮时发射。
     *          通知系统开始或停止连续采集预览。
     */
    void liveDisplayToggled(bool enabled);

    /**
     * @brief 运行按钮点击信号
     *
     * @details 用户点击运行按钮时发射。
     *          通知系统开始或停止自动运行。
     */
    void runClicked();

    /**
     * @brief 登录按钮点击信号
     *
     * @details 用户点击登录按钮时发射。
     *          通知系统打开登录对话框或执行注销。
     */
    void loginClicked();

    /**
     * @brief 退出按钮点击信号
     *
     * @details 用户点击退出按钮时发射。
     *          通知系统退出程序。
     */
    void exitClicked();

    /**
     * @brief 隐藏按钮点击信号
     *
     * @details 用户点击隐藏按钮时发射。
     *          通知主窗口隐藏操作员工具栏。
     */
    void hideClicked();

    /**
     * @brief 工位按钮点击信号
     *
     * @details 用户点击工位按钮时发射。
     *          通知系统打开工位切换对话框。
     */
    void stationClicked();

private:
    // ============================================================
    // UI初始化方法
    // ============================================================

    /**
     * @brief 初始化UI布局
     *
     * @details 创建水平布局，添加所有工具按钮。
     *          按钮按功能分组排列，使用分隔符区分不同区域。
     */
    void setupUI();

    /**
     * @brief 初始化按钮样式
     *
     * @details 设置各种状态的按钮样式：
     *          - normalStyle_：正常状态
     *          - highlightStyle_：高亮状态
     *          - dangerStyle_：危险/警告状态
     *          - activeStyle_：激活状态
     */
    void setupStyles();

    /**
     * @brief 创建工具按钮
     * @param iconName 图标名称
     * @param text 按钮文本
     * @param type 按钮类型
     * @return 创建的按钮指针
     *
     * @details 创建统一样式的工具按钮，并添加到buttons_映射表。
     */
    QPushButton* createToolButton(const QString& iconName, const QString& text, ButtonType type);

    /**
     * @brief 应用按钮样式
     * @param btn 目标按钮
     * @param highlight 是否使用高亮样式
     *
     * @details 为指定按钮应用样式表。
     */
    void applyButtonStyle(QPushButton* btn, bool highlight = false);

private slots:
    // ============================================================
    // 私有槽函数
    // ============================================================

    /**
     * @brief 帧有效/实时显示按钮点击响应
     *
     * @details 根据当前模式和状态决定触发的行为：
     *          - 帧有效模式：发射frameValidTriggered信号
     *          - 实时显示模式：切换激活状态，发射liveDisplayToggled信号
     */
    void onFrameLiveButtonClicked();

    /**
     * @brief 更新帧有效/实时显示按钮状态
     *
     * @details 根据当前模式和激活状态更新按钮图标、文本和样式。
     */
    void updateFrameLiveButton();

private:
    // ============================================================
    // UI组件
    // ============================================================
    QHBoxLayout* layout_;                       ///< 主水平布局
    QMap<ButtonType, QPushButton*> buttons_;    ///< 按钮类型到指针的映射表

    // ============================================================
    // 配置参数
    // ============================================================
    int iconSize_;          ///< 按钮图标大小（像素）

    // ============================================================
    // 状态变量
    // ============================================================
    bool isRunning_;            ///< 当前运行状态
    FrameLiveMode frameLiveMode_;   ///< 当前帧有效/实时显示模式
    bool isLiveDisplayActive_;  ///< 实时显示是否激活

    // ============================================================
    // 样式字符串
    // ============================================================
    QString normalStyle_;       ///< 正常状态样式
    QString highlightStyle_;    ///< 高亮状态样式（运行中等）
    QString dangerStyle_;       ///< 危险/警告状态样式（停止、退出等）
    QString activeStyle_;       ///< 激活状态样式（实时显示激活等）
};

} // namespace UI
} // namespace VisionForge
