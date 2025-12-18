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
    void onOpenCamera();
    void onCloseCamera();
    void onGrabImage();
    void onContinuousGrab();

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

    // 方案管理
    void onRecipeActivated(Core::Recipe* recipe);
    void onEditRecipeRequested(Core::Recipe* recipe);

    // 系统设置
    void onSystemSettings();

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

    // 面板
    ToolChainPanel* toolChainPanel_;
    ToolParameterPanel* toolParameterPanel_;
    ResultTablePanel* resultTablePanel_;
    HistoryPanel* historyPanel_;
    RecipeManagerWidget* recipeManagerWidget_;

    // 菜单
    QMenu* fileMenu_;
    QMenu* viewMenu_;
    QMenu* toolMenu_;
    QMenu* roiMenu_;
    QMenu* cameraMenu_;
    QMenu* calibMenu_;   // 标定菜单
    QMenu* commMenu_;    // 通信菜单
    QMenu* settingsMenu_;  // 设置菜单
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

    QAction* openCameraAction_;
    QAction* closeCameraAction_;
    QAction* grabImageAction_;
    QAction* continuousGrabAction_;

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

    // 系统设置动作
    QAction* systemSettingsAction_;

    // 状态栏
    QLabel* statusLabel_;
    QLabel* imageInfoLabel_;
    QLabel* scaleLabel_;
    QLabel* positionLabel_;

    // 相机
    HAL::ICamera* camera_;
    QTimer* continuousTimer_;
    bool isContinuousGrabbing_;

    // 当前图像
    Base::ImageData::Ptr currentImage_;

    // 图片序列
    QStringList imageSequence_;       // 图片路径列表
    int currentImageIndex_;           // 当前图片索引

    // 私有辅助方法
    void loadImageAtIndex(int index);         // 加载指定索引的图片
    void updateImageSequenceActions();        // 更新图片序列相关动作状态
    void applyImageTransform(Base::ImageData::Ptr& image);  // 应用图像变换（旋转、镜像）
};

} // namespace UI
} // namespace VisionForge
