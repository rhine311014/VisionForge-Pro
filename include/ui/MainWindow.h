/**
 * @file MainWindow.h
 * @brief 主窗口
 * @details VisionForge Pro的主窗口界面
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

// 根据是否启用Halcon选择使用哪个ImageViewer
#ifdef USE_HALCON
#include "ui/HalconImageViewer.h"
#else
#include "ui/ImageViewer.h"
#endif

#include "ui/ToolChainPanel.h"
#include "ui/ToolParameterPanel.h"
#include "ui/ResultTablePanel.h"
#include "ui/HistoryPanel.h"
#include "ui/RecipeManagerWidget.h"
#include "ui/StatisticsPanel.h"
#include "ui/UIModeManager.h"
#include "ui/StationSwitchBar.h"
#include "ui/SceneSwitchBar.h"
#include "ui/MultiCameraView.h"
#include "hal/SimulatedCamera.h"
#include "hal/ICamera.h"
#include "algorithm/VisionTool.h"
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
namespace UI {

/**
 * @class MainWindow
 * @brief 主窗口类
 *
 * 功能：
 * - 菜单栏和工具栏
 * - 中央图像显示区域
 * - 左侧工具链面板
 * - 右侧工具参数面板
 * - 底部状态栏
 * - 图像采集和处理
 * - ROI绘制功能
 * - 结果数据显示
 * - 历史记录管理
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    // 文件菜单
    void onOpenImage();
    void onOpenImageFolder();  // 打开图片文件夹
    void onSaveImage();
    void onExit();

    // 图片序列导航
    void onPreviousImage();    // 上一张图片
    void onNextImage();        // 下一张图片
    void onRunAllImages();     // 批量执行所有图片

    // 视图菜单
    void onFitToWindow();
    void onActualSize();
    void onZoomIn();
    void onZoomOut();
    void onTogglePanels();  // 切换侧边面板显示/隐藏

    // 工具菜单
    void onAddTool();
    void onRemoveTool();
    void onRunSingle();
    void onRunAll();

    // 相机操作
    void onFrameValidToggled(bool checked);   // 帧有效切换
    void onLiveDisplayToggled(bool checked);  // 实时显示切换

    // 工具链
    void onToolSelectionChanged(Algorithm::VisionTool* tool);
    void onToolDoubleClicked(Algorithm::VisionTool* tool);
    void onParameterChanged();

    // 图像查看器
    void onScaleChanged(double scale);
    void onMousePositionChanged(int x, int y, bool valid);
    void onROICreated(ROIShapePtr roi);
    void onROISelectionChanged(ROIShapePtr roi);

    // ROI操作
    void onDrawRectangle();
    void onDrawCircle();
    void onDrawEllipse();
    void onDrawPolygon();
    void onDrawLine();
    void onSelectROI();
    void onClearROIs();

    // 历史记录
    void onHistoryRecordSelected(const HistoryRecord& record);

    // 连续采集
    void onContinuousTimer();

    // PLC配置
    void onPLCConfig();

    // 相机配置
    void onCameraConfig();

    // 标定
    void onCameraCalibration();
    void onNinePointCalibration();
    void onQRCalibration();

    // 多相机多位置对位
    void onMultiCameraManager();
    void onMultiPointAlignment();
    void onAlignmentOutput();
    void onWorkStationConfig();

    // 方案管理
    void onRecipeActivated(Core::Recipe* recipe);
    void onEditRecipeRequested(Core::Recipe* recipe);

    // 系统设置
    void onSystemSettings();

    // 用户管理
    void onLogin();
    void onLogout();
    void onUserManagement();
    void updateUserStatus();

    // UI模式
    void onUIModeChanged(VisionForge::UI::UIMode mode);

    // 多工位多相机
    void onStationSelected(int index);
    void onShowAllStations();
    void onManualDebugCenter();
    void onCalibrationWizard();
    void onToggleMultiCameraView();
    void onMultiViewSelected(int index, const QString& positionId);

    // 场景切换
    void onSceneSelected(int index);
    void onSceneSwitchRequested();

private:
    void createMenus();
    void createToolBars();
    void createDockWindows();
    void createStatusBar();
    void addDockWidgetsToViewMenu();
    void connectSignals();

    void updateActions();
    void updateStatusBar();

    void processImage(Base::ImageData::Ptr image);
    void displayImage(Base::ImageData::Ptr image);  // 只显示图像，不运行工具
    void showAddToolDialog();

private:
    // 中央显示
#ifdef USE_HALCON
    HalconImageViewer* imageViewer_;
#else
    ImageViewer* imageViewer_;
#endif

    // 停靠窗口
    QDockWidget* toolChainDock_;
    QDockWidget* toolParameterDock_;
    QDockWidget* resultTableDock_;
    QDockWidget* historyDock_;
    QDockWidget* recipeDock_;
    QDockWidget* statisticsDock_;

    // 面板
    ToolChainPanel* toolChainPanel_;
    ToolParameterPanel* toolParameterPanel_;
    ResultTablePanel* resultTablePanel_;
    HistoryPanel* historyPanel_;
    RecipeManagerWidget* recipeManagerWidget_;
    StatisticsPanel* statisticsPanel_;

    // 菜单
    QMenu* fileMenu_;
    QMenu* viewMenu_;
    QMenu* toolMenu_;
    QMenu* roiMenu_;
    QMenu* cameraMenu_;
    QMenu* calibMenu_;   // 标定菜单
    QMenu* commMenu_;    // 通信菜单
    QMenu* settingsMenu_;  // 设置菜单
    QMenu* userMenu_;      // 用户菜单
    QMenu* helpMenu_;

    // 工具栏
    QToolBar* fileToolBar_;
    QToolBar* viewToolBar_;
    QToolBar* cameraToolBar_;
    QToolBar* processToolBar_;
    QToolBar* roiToolBar_;

    // 动作
    QAction* openImageAction_;
    QAction* openImageFolderAction_;  // 打开图片文件夹
    QAction* saveImageAction_;
    QAction* exitAction_;

    // 图片序列导航动作
    QAction* previousImageAction_;    // 上一张
    QAction* nextImageAction_;        // 下一张
    QAction* runAllImagesAction_;     // 批量执行所有图片

    QAction* fitToWindowAction_;
    QAction* actualSizeAction_;
    QAction* zoomInAction_;
    QAction* zoomOutAction_;

    QAction* addToolAction_;
    QAction* removeToolAction_;
    QAction* runSingleAction_;
    QAction* runAllAction_;

    QAction* frameValidAction_;       // 帧有效切换
    QAction* liveDisplayAction_;      // 实时显示切换

    // ROI操作动作
    QAction* drawRectangleAction_;
    QAction* drawCircleAction_;
    QAction* drawEllipseAction_;
    QAction* drawPolygonAction_;
    QAction* drawLineAction_;
    QAction* selectROIAction_;
    QAction* clearROIsAction_;

    // PLC通信动作
    QAction* plcConfigAction_;

    // 相机配置动作
    QAction* cameraConfigAction_;

    // 标定动作
    QAction* cameraCalibAction_;
    QAction* ninePointCalibAction_;
    QAction* qrCalibAction_;

    // 对位菜单和动作
    QMenu* alignmentMenu_;
    QAction* multiCameraManagerAction_;
    QAction* multiPointAlignmentAction_;
    QAction* alignmentOutputAction_;
    QAction* workStationConfigAction_;

    // 多工位相关动作
    QAction* manualDebugAction_;
    QAction* calibWizardAction_;
    QAction* toggleMultiViewAction_;

    // 多工位相关组件
    StationSwitchBar* stationSwitchBar_;
    SceneSwitchBar* sceneSwitchBar_;    // 场景切换栏
    MultiCameraView* multiCameraView_;
    QStackedWidget* centralStack_;      // 中央堆叠容器（切换单视图/多视图）
    bool isMultiViewMode_;              // 是否多视图模式

    // 系统设置动作
    QAction* systemSettingsAction_;

    // 用户动作
    QAction* loginAction_;
    QAction* logoutAction_;
    QAction* userManagementAction_;

    // 状态栏
    QLabel* statusLabel_;
    QLabel* imageInfoLabel_;
    QLabel* scaleLabel_;
    QLabel* positionLabel_;
    QLabel* userLabel_;          // 当前用户
    QLabel* cpuLabel_;           // CPU使用率
    QLabel* memoryLabel_;        // 内存使用率

    // 相机 - 使用unique_ptr管理生命周期，避免内存泄漏
    std::unique_ptr<HAL::ICamera> camera_;
    QTimer* continuousTimer_;
    bool isContinuousGrabbing_;
    bool isFrameValid_;           // 帧有效状态
    bool isLiveDisplay_;          // 实时显示状态

    // 当前图像
    Base::ImageData::Ptr currentImage_;

    // 图片序列
    QStringList imageSequence_;       // 图片路径列表
    int currentImageIndex_;           // 当前图片索引

    // 私有辅助方法
    void loadImageAtIndex(int index);         // 加载指定索引的图片
    void updateImageSequenceActions();        // 更新图片序列相关动作状态
    void applyImageTransform(Base::ImageData::Ptr& image);  // 应用图像变换（旋转、镜像）
    bool tryAutoConnectCamera();              // 尝试自动连接已保存的相机
    void showCameraConfigOnStartup();         // 启动时显示相机配置对话框
};

} // namespace UI
} // namespace VisionForge
