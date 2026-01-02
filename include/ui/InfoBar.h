/**
 * @file InfoBar.h
 * @brief 顶部信息栏类定义
 * @details 信息栏是操作员界面顶部的状态显示和控制区域，
 *          提供场景切换、位置选择、检测信息显示、运行状态等功能。
 *          采用标签页和下拉框的方式便于快速切换和查看。
 *
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-12-31
 *
 * @note 信息栏设计遵循以下原则：
 *       1. 信息层次分明，重要信息突出显示
 *       2. 场景切换使用标签页，直观便捷
 *       3. 位置选择使用下拉框，节省空间
 *       4. 检测结果使用颜色编码（绿色-OK，红色-NG）
 *       5. 提供快捷功能按钮入口
 *
 * @see OperatorToolBar 与信息栏配合使用的操作员工具栏
 * @see MainWindow 主窗口集成信息栏组件
 * @see SceneSwitchBar 场景切换栏（另一种场景切换方式）
 */

#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTabBar>

namespace VisionForge {
namespace UI {

/**
 * @class InfoBar
 * @brief 顶部信息栏类
 *
 * @details 信息栏提供了操作员界面顶部的核心功能：
 *
 * @par 主要功能：
 * - 场景标签页切换：使用QTabBar实现多场景快速切换
 * - 位置选择下拉框：选择当前检测位置
 * - 检测信息显示：显示当前检测状态和结果
 * - 运行状态信息：显示系统运行状态
 * - 快捷功能按钮：清除显示、虚拟通信等
 *
 * @par 使用场景：
 * - 操作员模式下显示在主窗口顶部
 * - 多场景多位置的生产检测环境
 * - 需要快速切换和查看状态的场景
 *
 * @par 设计特点：
 * - 紧凑的水平布局，节省垂直空间
 * - 场景使用标签页，位置使用下拉框
 * - 检测结果使用颜色编码
 * - 提供快捷操作按钮
 *
 * @par 信号槽关系：
 * - sceneChanged：场景切换时发射
 * - positionChanged：位置切换时发射
 * - 各按钮点击信号用于通知主窗口执行操作
 */
class InfoBar : public QWidget
{
    Q_OBJECT

public:
    // ============================================================
    // 构造与析构
    // ============================================================

    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     *
     * @details 初始化信息栏，创建场景标签、位置下拉框、信息按钮等组件。
     */
    explicit InfoBar(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~InfoBar() override;

    // ============================================================
    // 场景管理方法
    // ============================================================

    /**
     * @brief 设置场景列表
     * @param scenes 场景名称列表
     *
     * @details 更新场景标签页，每个场景对应一个标签。
     *          清空原有标签并创建新标签。
     *
     * @par 示例：
     * @code
     * infoBar->setScenes({"场景1", "场景2", "场景3"});
     * @endcode
     */
    void setScenes(const QStringList& scenes);

    /**
     * @brief 设置当前场景
     * @param index 场景索引（0-based）
     *
     * @details 切换到指定场景，更新标签页选中状态。
     *          如果索引无效则不执行操作。
     */
    void setCurrentScene(int index);

    /**
     * @brief 获取当前场景索引
     * @return 当前选中的场景索引（0-based）
     */
    int currentScene() const;

    // ============================================================
    // 位置管理方法
    // ============================================================

    /**
     * @brief 设置位置列表
     * @param positions 位置名称列表
     *
     * @details 更新位置下拉框的选项列表。
     *          清空原有选项并添加新选项。
     *
     * @par 示例：
     * @code
     * infoBar->setPositions({"Pos1", "Pos2", "Pos3", "Pos4"});
     * @endcode
     */
    void setPositions(const QStringList& positions);

    /**
     * @brief 设置当前位置
     * @param index 位置索引（0-based）
     *
     * @details 选中指定位置，更新下拉框选中项。
     *          如果索引无效则不执行操作。
     */
    void setCurrentPosition(int index);

    /**
     * @brief 获取当前位置索引
     * @return 当前选中的位置索引（0-based）
     */
    int currentPosition() const;

    // ============================================================
    // 信息显示方法
    // ============================================================

    /**
     * @brief 设置检测信息
     * @param info 检测信息文本
     *
     * @details 更新检测信息按钮的显示文本。
     *          可显示当前检测进度、工具状态等信息。
     *
     * @par 示例：
     * @code
     * infoBar->setDetectionInfo("正在执行: 模板匹配");
     * @endcode
     */
    void setDetectionInfo(const QString& info);

    /**
     * @brief 设置运行信息
     * @param info 运行信息文本
     *
     * @details 更新运行信息按钮的显示文本。
     *          可显示运行时间、检测数量等信息。
     *
     * @par 示例：
     * @code
     * infoBar->setRunInfo("运行: 1h 23m | 检测: 1234");
     * @endcode
     */
    void setRunInfo(const QString& info);

    /**
     * @brief 设置检测结果
     * @param ok true表示OK，false表示NG
     * @param message 结果消息（可选）
     *
     * @details 更新结果标签显示：
     *          - OK：绿色背景显示"OK"
     *          - NG：红色背景显示"NG"
     *          如果提供message，会附加显示结果消息。
     */
    void setDetectionResult(bool ok, const QString& message = QString());

    /**
     * @brief 清除显示信息
     *
     * @details 清除所有临时显示的信息，恢复默认状态。
     *          包括检测信息、运行信息、结果显示等。
     */
    void clearInfo();

signals:
    // ============================================================
    // 信号定义 - 通知外部组件状态变化和用户操作
    // ============================================================

    /**
     * @brief 场景变化信号
     * @param index 新的场景索引
     *
     * @details 当用户点击场景标签切换场景时发射。
     *          外部组件应响应此信号加载对应场景的配置。
     */
    void sceneChanged(int index);

    /**
     * @brief 位置变化信号
     * @param index 新的位置索引
     *
     * @details 当用户选择不同位置时发射。
     *          外部组件应响应此信号更新当前位置。
     */
    void positionChanged(int index);

    /**
     * @brief 清除显示按钮点击信号
     *
     * @details 用户点击清除显示按钮时发射。
     *          通知图像查看器清除叠加显示的结果图形。
     */
    void clearDisplayClicked();

    /**
     * @brief 虚拟通信按钮点击信号
     *
     * @details 用户点击虚拟通信按钮时发射。
     *          用于测试环境模拟PLC通信。
     */
    void virtualCommClicked();

    /**
     * @brief 检测信息按钮点击信号
     *
     * @details 用户点击检测信息按钮时发射。
     *          可用于打开详细检测信息对话框。
     */
    void detectionInfoClicked();

    /**
     * @brief 运行信息按钮点击信号
     *
     * @details 用户点击运行信息按钮时发射。
     *          可用于打开详细运行统计对话框。
     */
    void runInfoClicked();

private:
    // ============================================================
    // UI初始化方法
    // ============================================================

    /**
     * @brief 初始化UI布局
     *
     * @details 创建水平布局，按以下顺序排列组件：
     *          - 左侧：场景标签栏
     *          - 中间：位置下拉框、检测信息按钮、运行信息按钮
     *          - 右侧：结果标签、清除按钮、虚拟通信按钮
     */
    void setupUI();

private:
    // ============================================================
    // 布局组件
    // ============================================================
    QHBoxLayout* layout_;       ///< 主水平布局

    // ============================================================
    // 左侧场景标签组件
    // ============================================================
    QTabBar* sceneTabBar_;      ///< 场景标签栏，每个场景一个标签

    // ============================================================
    // 中部和右侧控件
    // ============================================================
    QComboBox* positionCombo_;      ///< 位置选择下拉框
    QPushButton* btnDetectionInfo_; ///< 检测信息按钮，显示当前检测状态
    QPushButton* btnRunInfo_;       ///< 运行信息按钮，显示运行统计
    QPushButton* btnClearDisplay_;  ///< 清除显示按钮，清除图像叠加
    QPushButton* btnVirtualComm_;   ///< 虚拟通信按钮，模拟PLC触发

    // ============================================================
    // 状态显示组件
    // ============================================================
    QLabel* resultLabel_;       ///< 检测结果标签（OK/NG）
};

} // namespace UI
} // namespace VisionForge
