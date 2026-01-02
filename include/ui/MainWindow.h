/**
 * @file MainWindow.h
 * @brief 主窗口类定义
 * @details VisionForge Pro视觉检测系统的主窗口界面。
 *          主窗口是应用程序的核心UI容器，负责管理和协调所有子组件，
 *          包括图像显示、工具面板、结果表格、历史记录等功能模块。
 *          采用Qt的信号槽机制实现组件间的解耦通信。
 *
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-12-14
 *
 * @note 主窗口使用QMainWindow作为基类，支持停靠窗口、工具栏、状态栏等标准功能。
 *       根据编译选项，可以使用HalconImageViewer或普通ImageViewer作为图像显示控件。
 *
 * @see OperatorToolBar 操作员工具栏，提供简化的操作界面
 * @see InfoBar 信息栏，显示系统状态信息
 * @see ToolChainPanel 工具链面板，管理视觉检测工具
 * @see ResultTablePanel 结果表格面板，显示检测结果
 * @see HalconImageViewer 基于Halcon的图像查看器
 */

#pragma once

// ============================================================
// 根据是否启用Halcon选择使用哪个ImageViewer
// USE_HALCON宏在CMake中定义，用于切换图像显示引擎
// ============================================================
#ifdef USE_HALCON
#include "ui/HalconImageViewer.h"
#else
#include "ui/ImageViewer.h"
#endif

// ============================================================
// UI组件头文件
// ============================================================
#include "ui/ToolChainPanel.h"          // 工具链面板 - 管理视觉工具列表
#include "ui/ToolParameterPanel.h"      // 工具参数面板 - 配置工具参数
#include "ui/ResultTablePanel.h"        // 结果表格面板 - 显示检测结果
#include "ui/HistoryPanel.h"            // 历史记录面板 - 显示检测历史
#include "ui/RecipeManagerWidget.h"     // 配方管理器 - 管理产品配方
#include "ui/StatisticsPanel.h"         // 统计面板 - 显示检测统计
#include "ui/UIModeManager.h"           // UI模式管理器 - 切换工程师/操作员模式
#include "ui/StationSwitchBar.h"        // 工位切换栏 - 切换多工位
#include "ui/SceneSwitchBar.h"          // 场景切换栏 - 切换检测场景
#include "ui/MultiCameraView.h"         // 多相机视图 - 同时显示多个相机画面
#include "ui/OperatorToolBar.h"         // 操作员工具栏 - 简化操作界面
#include "ui/InfoBar.h"                 // 信息栏 - 显示系统状态

// ============================================================
// 硬件和算法相关头文件
// ============================================================
#include "hal/SimulatedCamera.h"        // 模拟相机 - 用于测试
#include "hal/ICamera.h"                // 相机接口 - 相机抽象基类
#include "algorithm/VisionTool.h"       // 视觉工具基类

// ============================================================
// 核心引擎
// ============================================================
#include "core/VisionEngine.h"          // 视觉引擎 - 核心处理逻辑

// ============================================================
// Qt标准头文件
// ============================================================
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QLabel>
#include <QTimer>
#include <QStackedWidget>
#include <memory>

namespace VisionForge {

// 前向声明
namespace Core {
struct SceneConfig;  // 场景配置结构体
}

namespace UI {

/**
 * @class MainWindow
 * @brief 应用程序主窗口类
 *
 * @details MainWindow是VisionForge Pro视觉检测系统的主界面，承担以下核心职责：
 *
 * @par 主要功能：
 * - 菜单栏和工具栏：提供文件、视图、工具、标定、通信等功能入口
 * - 中央图像显示区域：使用HalconImageViewer或ImageViewer显示采集的图像
 * - 左侧工具链面板：配置和管理视觉检测工具的执行顺序
 * - 右侧工具参数面板：配置当前选中工具的参数
 * - 底部状态栏：显示系统状态、图像信息、鼠标位置等
 * - 图像采集和处理：支持单次采集、连续采集、图片文件夹批处理
 * - ROI绘制功能：支持矩形、圆形、椭圆、多边形、直线等形状
 * - 结果数据显示：以表格形式显示检测结果
 * - 历史记录管理：保存和回放历史检测记录
 *
 * @par 使用场景：
 * - 作为应用程序的主入口窗口
 * - 工程师模式：完整的配置和调试功能
 * - 操作员模式：简化的操作界面
 *
 * @par 多工位支持：
 * - 支持多工位配置和切换
 * - 支持多相机同时显示
 * - 支持多场景切换
 *
 * @par 信号槽关系：
 * - 与VisionEngine连接，响应图像处理完成事件
 * - 与相机管理器连接，响应相机状态变化
 * - 与工具链面板连接，响应工具选择变化
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // ============================================================
    // 构造与析构
    // ============================================================

    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr表示顶层窗口
     *
     * @details 初始化主窗口，创建所有UI组件，建立信号槽连接。
     *          构造函数执行以下初始化步骤：
     *          1. 创建菜单栏（文件、视图、工具、标定、通信、设置、用户、帮助）
     *          2. 创建工具栏（文件、视图、相机、处理、ROI）
     *          3. 创建停靠窗口（工具链、参数、结果、历史、配方、统计）
     *          4. 创建中央图像显示区域
     *          5. 初始化VisionEngine并建立信号连接
     *          6. 创建操作员界面组件
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     *
     * @details 释放所有资源，停止连续采集定时器。
     */
    ~MainWindow() override;

private slots:
    // ============================================================
    // 文件菜单槽函数
    // ============================================================

    /**
     * @brief 打开单张图像文件
     *
     * @details 弹出文件选择对话框，加载单张图像用于调试。
     *          支持的格式：PNG、JPG、BMP、TIFF等常见图像格式。
     */
    void onOpenImage();

    /**
     * @brief 打开图片文件夹
     *
     * @details 弹出文件夹选择对话框，加载整个文件夹的图像序列。
     *          适用于批量测试和离线分析场景。
     */
    void onOpenImageFolder();

    /**
     * @brief 保存当前图像
     *
     * @details 将当前显示的图像（包含检测结果叠加）保存到文件。
     */
    void onSaveImage();

    /**
     * @brief 退出应用程序
     *
     * @details 关闭主窗口，触发应用程序退出流程。
     *          退出前会提示保存未保存的更改。
     */
    void onExit();

    // ============================================================
    // 图片序列导航槽函数
    // ============================================================

    /**
     * @brief 切换到上一张图片
     *
     * @details 在图片序列中向前导航，加载并显示上一张图片。
     *          如果已经是第一张，则不执行操作。
     */
    void onPreviousImage();

    /**
     * @brief 切换到下一张图片
     *
     * @details 在图片序列中向后导航，加载并显示下一张图片。
     *          如果已经是最后一张，则不执行操作。
     */
    void onNextImage();

    /**
     * @brief 批量执行所有图片
     *
     * @details 对文件夹中的所有图片依次执行检测处理。
     *          适用于离线批量分析场景。
     */
    void onRunAllImages();

    // ============================================================
    // 视图菜单槽函数
    // ============================================================

    /**
     * @brief 适应窗口大小
     *
     * @details 调整图像缩放比例，使图像完全适应显示窗口大小。
     */
    void onFitToWindow();

    /**
     * @brief 显示实际大小
     *
     * @details 将图像缩放比例设置为100%，显示图像的实际像素大小。
     */
    void onActualSize();

    /**
     * @brief 放大图像
     *
     * @details 将图像缩放比例增加一个档位（通常为1.2倍）。
     */
    void onZoomIn();

    /**
     * @brief 缩小图像
     *
     * @details 将图像缩放比例减小一个档位（通常为0.8倍）。
     */
    void onZoomOut();

    /**
     * @brief 切换侧边面板显示/隐藏
     *
     * @details 切换左侧工具链面板和右侧参数面板的显示状态。
     *          用于最大化图像显示区域。
     */
    void onTogglePanels();

    // ============================================================
    // 工具菜单槽函数
    // ============================================================

    /**
     * @brief 添加视觉工具
     *
     * @details 弹出工具选择对话框，添加新的视觉检测工具到工具链。
     *          可选工具包括：模板匹配、边缘检测、测量、识别等。
     */
    void onAddTool();

    /**
     * @brief 移除视觉工具
     *
     * @details 从工具链中移除当前选中的视觉工具。
     */
    void onRemoveTool();

    /**
     * @brief 执行单个工具
     *
     * @details 仅执行当前选中的视觉工具，用于单独调试。
     */
    void onRunSingle();

    /**
     * @brief 执行所有工具
     *
     * @details 按顺序执行工具链中的所有视觉工具。
     */
    void onRunAll();

    // ============================================================
    // 相机操作槽函数
    // ============================================================

    /**
     * @brief 帧有效切换
     * @param checked 是否选中
     *
     * @details 控制是否启用帧有效信号。当帧有效启用时，
     *          只有收到帧有效信号时才会处理图像。
     */
    void onFrameValidToggled(bool checked);

    /**
     * @brief 实时显示切换
     * @param checked 是否选中
     *
     * @details 控制是否启用实时预览模式。启用后会连续采集并显示相机画面，
     *          但不执行检测处理。
     */
    void onLiveDisplayToggled(bool checked);

    // ============================================================
    // 工具链槽函数
    // ============================================================

    /**
     * @brief 工具选择变化响应
     * @param tool 新选中的视觉工具指针
     *
     * @details 当用户在工具链面板中选择不同工具时调用。
     *          更新参数面板显示对应工具的配置参数。
     */
    void onToolSelectionChanged(Algorithm::VisionTool* tool);

    /**
     * @brief 工具双击响应
     * @param tool 被双击的视觉工具指针
     *
     * @details 当用户双击工具链中的工具时调用。
     *          通常打开工具的配置对话框。
     */
    void onToolDoubleClicked(Algorithm::VisionTool* tool);

    /**
     * @brief 参数变化响应
     *
     * @details 当工具参数发生变化时调用。
     *          标记配置为已修改状态。
     */
    void onParameterChanged();

    // ============================================================
    // 图像查看器槽函数
    // ============================================================

    /**
     * @brief 缩放比例变化响应
     * @param scale 新的缩放比例
     *
     * @details 当图像缩放比例变化时调用，更新状态栏显示。
     */
    void onScaleChanged(double scale);

    /**
     * @brief 鼠标位置变化响应
     * @param x 鼠标X坐标（图像坐标系）
     * @param y 鼠标Y坐标（图像坐标系）
     * @param valid 坐标是否有效（是否在图像范围内）
     *
     * @details 当鼠标在图像上移动时调用，更新状态栏显示坐标和像素值。
     */
    void onMousePositionChanged(int x, int y, bool valid);

    /**
     * @brief ROI创建完成响应
     * @param roi 新创建的ROI形状指针
     *
     * @details 当用户完成ROI绘制时调用。
     *          将ROI应用到当前选中的视觉工具。
     */
    void onROICreated(ROIShapePtr roi);

    /**
     * @brief ROI选择变化响应
     * @param roi 新选中的ROI形状指针
     *
     * @details 当用户选择不同ROI时调用。
     */
    void onROISelectionChanged(ROIShapePtr roi);

    // ============================================================
    // ROI操作槽函数
    // ============================================================

    /**
     * @brief 开始绘制矩形ROI
     *
     * @details 将图像查看器切换到矩形绘制模式。
     */
    void onDrawRectangle();

    /**
     * @brief 开始绘制圆形ROI
     *
     * @details 将图像查看器切换到圆形绘制模式。
     */
    void onDrawCircle();

    /**
     * @brief 开始绘制椭圆ROI
     *
     * @details 将图像查看器切换到椭圆绘制模式。
     */
    void onDrawEllipse();

    /**
     * @brief 开始绘制多边形ROI
     *
     * @details 将图像查看器切换到多边形绘制模式。
     *          用户可以通过点击添加顶点，双击完成绘制。
     */
    void onDrawPolygon();

    /**
     * @brief 开始绘制直线ROI
     *
     * @details 将图像查看器切换到直线绘制模式。
     *          用于定义边缘检测区域等。
     */
    void onDrawLine();

    /**
     * @brief 进入ROI选择模式
     *
     * @details 将图像查看器切换到选择模式，可以选择和编辑已有ROI。
     */
    void onSelectROI();

    /**
     * @brief 清除所有ROI
     *
     * @details 删除图像查看器中的所有ROI形状。
     */
    void onClearROIs();

    // ============================================================
    // 历史记录槽函数
    // ============================================================

    /**
     * @brief 历史记录选中响应
     * @param record 选中的历史记录
     *
     * @details 当用户在历史面板中选择记录时调用。
     *          加载并显示对应的历史图像和结果。
     */
    void onHistoryRecordSelected(const HistoryRecord& record);

    // ============================================================
    // 连续采集槽函数
    // ============================================================

    /**
     * @brief 连续采集定时器响应
     *
     * @details 定时器触发时调用，执行一次图像采集和处理。
     *          用于实现连续采集模式。
     */
    void onContinuousTimer();

    // ============================================================
    // 通信配置槽函数
    // ============================================================

    /**
     * @brief 打开PLC配置对话框
     *
     * @details 配置PLC通信参数，包括IP地址、端口、IO映射等。
     */
    void onPLCConfig();

    /**
     * @brief 打开相机配置对话框
     *
     * @details 配置相机参数，包括曝光、增益、触发模式等。
     */
    void onCameraConfig();

    // ============================================================
    // 标定槽函数
    // ============================================================

    /**
     * @brief 相机标定
     *
     * @details 执行相机内参标定，用于畸变校正。
     */
    void onCameraCalibration();

    /**
     * @brief 九点标定
     *
     * @details 执行九点标定，建立像素坐标与物理坐标的映射关系。
     */
    void onNinePointCalibration();

    /**
     * @brief 二维码标定
     *
     * @details 使用二维码进行自动标定。
     */
    void onQRCalibration();

    // ============================================================
    // 多相机对位槽函数
    // ============================================================

    /**
     * @brief 多相机管理
     *
     * @details 打开多相机管理对话框，配置多相机系统。
     */
    void onMultiCameraManager();

    /**
     * @brief 多点对位
     *
     * @details 打开多点对位配置对话框，设置对位参数。
     */
    void onMultiPointAlignment();

    /**
     * @brief 对位输出配置
     *
     * @details 配置对位结果的输出方式。
     */
    void onAlignmentOutput();

    /**
     * @brief 工位配置
     *
     * @details 打开工位配置对话框，配置工位参数。
     */
    void onWorkStationConfig();

    // ============================================================
    // 方案管理槽函数
    // ============================================================

    /**
     * @brief 方案激活响应
     * @param recipe 被激活的方案指针
     *
     * @details 当用户选择激活某个配方时调用。
     *          加载配方中的工具链配置。
     */
    void onRecipeActivated(Core::Recipe* recipe);

    /**
     * @brief 编辑方案请求响应
     * @param recipe 要编辑的方案指针
     *
     * @details 当用户请求编辑配方时调用。
     *          打开配方编辑对话框。
     */
    void onEditRecipeRequested(Core::Recipe* recipe);

    // ============================================================
    // 系统设置槽函数
    // ============================================================

    /**
     * @brief 打开系统设置对话框
     *
     * @details 配置系统级别参数，如保存路径、日志设置等。
     */
    void onSystemSettings();

    /**
     * @brief 打开工位配置工具
     *
     * @details 配置工位参数，包括相机数量、位置数量、功能模式等。
     *          这是一个独立的配置工具对话框。
     */
    void onStationConfigTool();

    // ============================================================
    // 用户管理槽函数
    // ============================================================

    /**
     * @brief 用户登录
     *
     * @details 弹出登录对话框，进行用户身份验证。
     */
    void onLogin();

    /**
     * @brief 用户注销
     *
     * @details 注销当前用户，返回默认权限状态。
     */
    void onLogout();

    /**
     * @brief 用户管理
     *
     * @details 打开用户管理对话框，管理用户账号和权限。
     *          需要管理员权限。
     */
    void onUserManagement();

    /**
     * @brief 更新用户状态显示
     *
     * @details 更新状态栏中的当前用户显示。
     */
    void updateUserStatus();

    // ============================================================
    // UI模式槽函数
    // ============================================================

    /**
     * @brief UI模式变化响应
     * @param mode 新的UI模式（工程师/操作员）
     *
     * @details 当UI模式切换时调用，调整界面布局和可见组件。
     *          工程师模式：显示所有配置面板
     *          操作员模式：简化界面，只显示必要操作按钮
     */
    void onUIModeChanged(VisionForge::UI::UIMode mode);

    // ============================================================
    // 多工位多相机槽函数
    // ============================================================

    /**
     * @brief 工位选择响应
     * @param index 选中的工位索引
     *
     * @details 当用户选择不同工位时调用。
     *          切换到对应工位的配置和显示。
     */
    void onStationSelected(int index);

    /**
     * @brief 显示所有工位
     *
     * @details 在多视图模式下同时显示所有工位的画面。
     */
    void onShowAllStations();

    /**
     * @brief 手动调试中心
     *
     * @details 打开手动调试中心对话框，用于手动控制平台运动和相机采集。
     */
    void onManualDebugCenter();

    /**
     * @brief 标定向导
     *
     * @details 启动多位置标定向导，引导用户完成标定流程。
     */
    void onCalibrationWizard();

    /**
     * @brief 切换多相机视图
     *
     * @details 在单相机视图和多相机视图之间切换。
     */
    void onToggleMultiCameraView();

    /**
     * @brief 多视图选择响应
     * @param index 选中的视图索引
     * @param positionId 对应的位置ID
     *
     * @details 当用户在多相机视图中选择某个视图时调用。
     */
    void onMultiViewSelected(int index, const QString& positionId);

    // ============================================================
    // 场景切换槽函数
    // ============================================================

    /**
     * @brief 场景选择响应
     * @param index 选中的场景索引
     *
     * @details 当用户选择不同场景时调用。
     *          加载对应场景的工具链配置。
     */
    void onSceneSelected(int index);

    /**
     * @brief 请求切换到上一个场景
     *
     * @details 切换到上一个场景。
     */
    void onPreviousSceneRequested();

    /**
     * @brief 请求切换到下一个场景
     *
     * @details 切换到下一个场景。
     */
    void onNextSceneRequested();

    // ============================================================
    // VisionEngine信号响应槽函数
    // ============================================================

    /**
     * @brief 图像更新响应
     * @param image 新的图像数据
     *
     * @details 当VisionEngine加载新图像时调用。
     *          更新图像显示区域。
     */
    void onEngineImageUpdated(Base::ImageData::Ptr image);

    /**
     * @brief 实时帧响应
     * @param image 实时帧图像数据
     *
     * @details 当相机采集到新帧时调用（实时预览模式）。
     *          仅更新显示，不执行处理。
     */
    void onEngineLiveFrame(Base::ImageData::Ptr image);

    /**
     * @brief 处理完成响应
     * @param result 处理结果
     *
     * @details 当VisionEngine完成图像处理时调用。
     *          更新结果表格和历史记录。
     */
    void onEngineProcessCompleted(const Core::ProcessResult& result);

    /**
     * @brief 相机状态变化响应
     * @param connected 是否已连接
     *
     * @details 当相机连接状态变化时调用。
     *          更新相机相关操作按钮的状态。
     */
    void onEngineCameraStatusChanged(bool connected);

    /**
     * @brief 图像序列变化响应
     * @param count 图像总数
     * @param currentIndex 当前索引
     *
     * @details 当图像序列发生变化时调用。
     *          更新导航按钮的状态。
     */
    void onEngineImageSequenceChanged(int count, int currentIndex);

    /**
     * @brief 引擎错误响应
     * @param message 错误消息
     *
     * @details 当VisionEngine发生错误时调用。
     *          显示错误提示对话框。
     */
    void onEngineError(const QString& message);

    /**
     * @brief 引擎状态消息响应
     * @param message 状态消息
     *
     * @details 当VisionEngine发送状态消息时调用。
     *          更新状态栏显示。
     */
    void onEngineStatusMessage(const QString& message);

private:
    // ============================================================
    // UI初始化方法
    // ============================================================

    /**
     * @brief 创建菜单栏
     *
     * @details 创建所有菜单及其子菜单项：
     *          - 文件菜单：打开、保存、退出
     *          - 视图菜单：缩放、面板显示
     *          - 工具菜单：添加/移除工具、执行
     *          - ROI菜单：绘制各种ROI形状
     *          - 相机菜单：相机控制
     *          - 标定菜单：各种标定功能
     *          - 通信菜单：PLC配置
     *          - 设置菜单：系统设置
     *          - 用户菜单：登录/注销/用户管理
     *          - 帮助菜单：关于、帮助文档
     */
    void createMenus();

    /**
     * @brief 创建工具栏
     *
     * @details 创建各功能工具栏：
     *          - 文件工具栏：打开、保存
     *          - 视图工具栏：缩放控制
     *          - 相机工具栏：采集控制
     *          - 处理工具栏：执行控制
     *          - ROI工具栏：ROI绘制工具
     */
    void createToolBars();

    /**
     * @brief 创建停靠窗口
     *
     * @details 创建可停靠的功能面板：
     *          - 工具链面板（左侧）
     *          - 工具参数面板（右侧）
     *          - 结果表格面板（底部）
     *          - 历史记录面板
     *          - 配方管理面板
     *          - 统计面板
     */
    void createDockWindows();

    /**
     * @brief 创建状态栏
     *
     * @details 创建底部状态栏，包含：
     *          - 状态信息标签
     *          - 图像信息标签
     *          - 缩放比例标签
     *          - 鼠标位置标签
     *          - 当前用户标签
     *          - CPU使用率标签
     *          - 内存使用率标签
     */
    void createStatusBar();

    /**
     * @brief 创建操作员界面组件
     *
     * @details 创建操作员模式专用的界面组件：
     *          - 顶部信息栏：显示工位、产品、结果等信息
     *          - 底部操作工具栏：开始、停止、复位等按钮
     */
    void createOperatorUI();

    /**
     * @brief 将停靠窗口添加到视图菜单
     *
     * @details 为每个停靠窗口在视图菜单中添加显示/隐藏切换选项。
     */
    void addDockWidgetsToViewMenu();

    /**
     * @brief 建立信号槽连接
     *
     * @details 连接各组件之间的信号和槽，实现事件通信：
     *          - 菜单动作与槽函数的连接
     *          - 工具栏动作与槽函数的连接
     *          - 面板与主窗口的信号连接
     *          - 图像查看器事件连接
     */
    void connectSignals();

    /**
     * @brief 连接操作员界面信号
     *
     * @details 连接操作员工具栏和信息栏的信号。
     */
    void connectOperatorSignals();

    /**
     * @brief 更新动作状态
     *
     * @details 根据当前系统状态更新菜单项和工具栏按钮的启用/禁用状态。
     */
    void updateActions();

    /**
     * @brief 更新状态栏
     *
     * @details 更新状态栏中的各项信息显示。
     */
    void updateStatusBar();

    /**
     * @brief 处理图像
     * @param image 要处理的图像数据
     *
     * @details 对图像执行完整的处理流程，包括执行所有工具。
     */
    void processImage(Base::ImageData::Ptr image);

    /**
     * @brief 仅显示图像
     * @param image 要显示的图像数据
     *
     * @details 只更新图像显示，不执行工具链处理。
     *          用于实时预览模式。
     */
    void displayImage(Base::ImageData::Ptr image);

    /**
     * @brief 显示添加工具对话框
     *
     * @details 弹出工具选择对话框，让用户选择要添加的视觉工具类型。
     */
    void showAddToolDialog();

    /**
     * @brief 加载场景工具链
     * @param scene 场景配置
     *
     * @details 根据场景配置加载对应的工具链，切换当前检测场景。
     */
    void loadSceneToolChain(const Core::SceneConfig* scene);

    /**
     * @brief 连接VisionEngine信号
     *
     * @details 建立与VisionEngine单例的信号连接。
     */
    void connectEngineSignals();

    /**
     * @brief 更新图片序列相关动作状态
     *
     * @details 根据当前图片序列状态更新导航按钮的启用状态。
     */
    void updateImageSequenceActions();

    /**
     * @brief 尝试自动连接相机
     * @return 连接是否成功
     *
     * @details 尝试自动连接上次使用的相机配置。
     */
    bool tryAutoConnectCamera();

    /**
     * @brief 启动时显示相机配置对话框
     *
     * @details 如果没有保存的相机配置，在启动时显示配置对话框。
     */
    void showCameraConfigOnStartup();

private:
    // ============================================================
    // 中央显示组件
    // ============================================================
#ifdef USE_HALCON
    HalconImageViewer* imageViewer_;    ///< 基于Halcon的图像查看器
#else
    ImageViewer* imageViewer_;          ///< 标准图像查看器
#endif

    // ============================================================
    // 停靠窗口容器
    // ============================================================
    QDockWidget* toolChainDock_;        ///< 工具链面板停靠窗口
    QDockWidget* toolParameterDock_;    ///< 工具参数面板停靠窗口
    QDockWidget* resultTableDock_;      ///< 结果表格停靠窗口
    QDockWidget* historyDock_;          ///< 历史记录停靠窗口
    QDockWidget* recipeDock_;           ///< 配方管理停靠窗口
    QDockWidget* statisticsDock_;       ///< 统计面板停靠窗口

    // ============================================================
    // 功能面板组件
    // ============================================================
    ToolChainPanel* toolChainPanel_;            ///< 工具链面板
    ToolParameterPanel* toolParameterPanel_;    ///< 工具参数面板
    ResultTablePanel* resultTablePanel_;        ///< 结果表格面板
    HistoryPanel* historyPanel_;                ///< 历史记录面板
    RecipeManagerWidget* recipeManagerWidget_;  ///< 配方管理器
    StatisticsPanel* statisticsPanel_;          ///< 统计面板

    // ============================================================
    // 菜单组件
    // ============================================================
    QMenu* fileMenu_;       ///< 文件菜单
    QMenu* viewMenu_;       ///< 视图菜单
    QMenu* toolMenu_;       ///< 工具菜单
    QMenu* roiMenu_;        ///< ROI菜单
    QMenu* cameraMenu_;     ///< 相机菜单
    QMenu* calibMenu_;      ///< 标定菜单
    QMenu* commMenu_;       ///< 通信菜单
    QMenu* settingsMenu_;   ///< 设置菜单
    QMenu* userMenu_;       ///< 用户菜单
    QMenu* helpMenu_;       ///< 帮助菜单

    // ============================================================
    // 工具栏组件
    // ============================================================
    QToolBar* fileToolBar_;     ///< 文件工具栏
    QToolBar* viewToolBar_;     ///< 视图工具栏
    QToolBar* cameraToolBar_;   ///< 相机工具栏
    QToolBar* processToolBar_;  ///< 处理工具栏
    QToolBar* roiToolBar_;      ///< ROI工具栏

    // ============================================================
    // 文件菜单动作
    // ============================================================
    QAction* openImageAction_;          ///< 打开图像动作
    QAction* openImageFolderAction_;    ///< 打开图片文件夹动作
    QAction* saveImageAction_;          ///< 保存图像动作
    QAction* exitAction_;               ///< 退出动作

    // ============================================================
    // 图片序列导航动作
    // ============================================================
    QAction* previousImageAction_;      ///< 上一张动作
    QAction* nextImageAction_;          ///< 下一张动作
    QAction* runAllImagesAction_;       ///< 批量执行动作

    // ============================================================
    // 视图菜单动作
    // ============================================================
    QAction* fitToWindowAction_;    ///< 适应窗口动作
    QAction* actualSizeAction_;     ///< 实际大小动作
    QAction* zoomInAction_;         ///< 放大动作
    QAction* zoomOutAction_;        ///< 缩小动作

    // ============================================================
    // 工具菜单动作
    // ============================================================
    QAction* addToolAction_;        ///< 添加工具动作
    QAction* removeToolAction_;     ///< 移除工具动作
    QAction* runSingleAction_;      ///< 执行单个工具动作
    QAction* runAllAction_;         ///< 执行所有工具动作

    // ============================================================
    // 相机控制动作
    // ============================================================
    QAction* frameValidAction_;     ///< 帧有效切换动作
    QAction* liveDisplayAction_;    ///< 实时显示切换动作

    // ============================================================
    // ROI操作动作
    // ============================================================
    QAction* drawRectangleAction_;  ///< 绘制矩形动作
    QAction* drawCircleAction_;     ///< 绘制圆形动作
    QAction* drawEllipseAction_;    ///< 绘制椭圆动作
    QAction* drawPolygonAction_;    ///< 绘制多边形动作
    QAction* drawLineAction_;       ///< 绘制直线动作
    QAction* selectROIAction_;      ///< 选择ROI动作
    QAction* clearROIsAction_;      ///< 清除ROI动作

    // ============================================================
    // 通信配置动作
    // ============================================================
    QAction* plcConfigAction_;      ///< PLC配置动作
    QAction* cameraConfigAction_;   ///< 相机配置动作

    // ============================================================
    // 标定动作
    // ============================================================
    QAction* cameraCalibAction_;    ///< 相机标定动作
    QAction* ninePointCalibAction_; ///< 九点标定动作
    QAction* qrCalibAction_;        ///< 二维码标定动作

    // ============================================================
    // 对位菜单和动作
    // ============================================================
    QMenu* alignmentMenu_;                  ///< 对位菜单
    QAction* multiCameraManagerAction_;     ///< 多相机管理动作
    QAction* multiPointAlignmentAction_;    ///< 多点对位动作
    QAction* alignmentOutputAction_;        ///< 对位输出动作
    QAction* workStationConfigAction_;      ///< 工位配置动作

    // ============================================================
    // 多工位相关动作
    // ============================================================
    QAction* manualDebugAction_;        ///< 手动调试动作
    QAction* calibWizardAction_;        ///< 标定向导动作
    QAction* toggleMultiViewAction_;    ///< 切换多视图动作

    // ============================================================
    // 多工位相关组件
    // ============================================================
    StationSwitchBar* stationSwitchBar_;    ///< 工位切换栏
    SceneSwitchBar* sceneSwitchBar_;        ///< 场景切换栏
    MultiCameraView* multiCameraView_;      ///< 多相机视图
    QStackedWidget* centralStack_;          ///< 中央堆叠容器（切换单视图/多视图）
    bool isMultiViewMode_;                  ///< 是否多视图模式

    // ============================================================
    // 操作员界面组件
    // ============================================================
    OperatorToolBar* operatorToolBar_;      ///< 底部操作员工具栏
    InfoBar* infoBar_;                      ///< 顶部信息栏

    // ============================================================
    // 系统设置动作
    // ============================================================
    QAction* systemSettingsAction_;         ///< 系统设置动作
    QAction* stationConfigToolAction_;      ///< 工位配置工具动作

    // ============================================================
    // 用户管理动作
    // ============================================================
    QAction* loginAction_;              ///< 登录动作
    QAction* logoutAction_;             ///< 注销动作
    QAction* userManagementAction_;     ///< 用户管理动作

    // ============================================================
    // 状态栏组件
    // ============================================================
    QLabel* statusLabel_;       ///< 状态信息标签
    QLabel* imageInfoLabel_;    ///< 图像信息标签
    QLabel* scaleLabel_;        ///< 缩放比例标签
    QLabel* positionLabel_;     ///< 鼠标位置标签
    QLabel* userLabel_;         ///< 当前用户标签
    QLabel* cpuLabel_;          ///< CPU使用率标签
    QLabel* memoryLabel_;       ///< 内存使用率标签

    // ============================================================
    // 核心引擎引用
    // ============================================================
    Core::VisionEngine& engine_;    ///< VisionEngine单例引用

    // ============================================================
    // UI状态变量
    // ============================================================
    bool isFrameValid_;     ///< 帧有效状态（是否启用帧有效信号）
    bool isLiveDisplay_;    ///< 实时显示状态（是否处于实时预览模式）
};

} // namespace UI
} // namespace VisionForge
