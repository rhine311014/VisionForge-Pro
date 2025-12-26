/**
 * @file MainWindow.cpp
 * @brief 主窗口实现
 */

#include "ui/MainWindow.h"
#include "ui/BatchProcessWorker.h"
#include "ui/Theme.h"
#include "algorithm/ToolFactory.h"
#include "base/Logger.h"
#include "base/ParallelProcessor.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QInputDialog>
#include <QSplitter>
#include <QActionGroup>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <chrono>

#ifdef USE_HALCON
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/HalconObjectWrapper.h"
#include "ui/ShapeMatchToolDialog.h"
#endif

#include "ui/PLCConfigDialog.h"
#include "ui/CameraConfigDialog.h"
#include "ui/CameraSetupDialog.h"
#include "ui/RecipeEditorDialog.h"
#include "ui/CameraCalibDialog.h"
#include "ui/NinePointCalibDialog.h"
#include "ui/QRCalibDialog.h"
#include "ui/SystemSettingsDialog.h"
// 工具对话框工厂
#include "ui/ToolDialogFactory.h"
// 系统对话框
#include "ui/LoginDialog.h"
#include "ui/StatisticsPanel.h"
#include "ui/UserManagementDialog.h"
#include "ui/UIModeManager.h"
// 多相机多位置对位对话框
#include "ui/MultiCameraManagerDialog.h"
#include "ui/MultiPointAlignmentDialog.h"
#include "ui/AlignmentOutputDialog.h"
#include "ui/WorkStationDialog.h"
#include "core/WorkStation.h"
// 多工位多相机组件
#include "ui/ManualDebugDialog.h"
#include "ui/CalibrationWizard.h"
#include "core/MultiStationManager.h"
#include "core/PositionToolChainManager.h"
#include "core/SceneManager.h"
#include "core/SceneConfig.h"
#include "core/ToolChain.h"
#include "ui/SceneSwitchBar.h"
// 基础设施
#include "base/PermissionManager.h"
#include "base/SystemMonitor.h"
#include "base/DataExporter.h"
// 算法工具头文件
#include "algorithm/GrayTool.h"
#include "algorithm/BlurTool.h"
#include "algorithm/ThresholdTool.h"
#include "algorithm/ColorConvertTool.h"
#include "algorithm/EdgeTool.h"
#include "algorithm/MorphologyTool.h"
#include "algorithm/TemplateMatchTool.h"
#include "algorithm/CircleTool.h"
#include "algorithm/LineTool.h"
#include "algorithm/FindEdgeTool.h"
#include "algorithm/BlobTool.h"
#include "algorithm/AIDetectionTool.h"
#include "algorithm/MeasureDistanceTool.h"
#include "algorithm/MeasureAngleTool.h"
#include "algorithm/MeasureAreaTool.h"
#include "algorithm/CalcCenterTool.h"
#include "algorithm/CalcOrientationTool.h"
#include "algorithm/RangeJudgeTool.h"
#include "algorithm/LogicOperationTool.h"
#include "algorithm/SaveImageTool.h"
#include "algorithm/ROITool.h"
#include "algorithm/PLCOutputTool.h"
#include "hal/CameraFactory.h"
#include "core/RecipeManager.h"
#include "algorithm/CalibrationManager.h"
#include "base/ConfigManager.h"

#ifdef USE_HIKVISION_MVS
#include "hal/HikvisionCamera.h"
#endif

// 定义ImageViewer类型宏，根据USE_HALCON选择使用哪个ImageViewer
#ifdef USE_HALCON
#define IMAGEVIEWER_CLASS HalconImageViewer
#else
#define IMAGEVIEWER_CLASS ImageViewer
#endif

namespace VisionForge {
namespace UI {

// InspectionWorker已被BatchProcessWorker替代，参见include/ui/BatchProcessWorker.h

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , imageViewer_(nullptr)
    , toolChainDock_(nullptr)
    , toolParameterDock_(nullptr)
    , resultTableDock_(nullptr)
    , historyDock_(nullptr)
    , recipeDock_(nullptr)
    , statisticsDock_(nullptr)
    , toolChainPanel_(nullptr)
    , toolParameterPanel_(nullptr)
    , resultTablePanel_(nullptr)
    , historyPanel_(nullptr)
    , recipeManagerWidget_(nullptr)
    , statisticsPanel_(nullptr)
    , stationSwitchBar_(nullptr)
    , sceneSwitchBar_(nullptr)
    , multiCameraView_(nullptr)
    , centralStack_(nullptr)
    , isMultiViewMode_(false)
    , engine_(Core::VisionEngine::instance())
    , isFrameValid_(true)
    , isLiveDisplay_(true)
{
    // 初始化工具对话框工厂（设置 VisionTool 的对话框创建器回调）
    ToolDialogFactory::instance();

    setWindowTitle("VisionForge Pro - 机器视觉检测平台");

    // 设置主窗口样式，移除所有内边距和边距
    setStyleSheet("QMainWindow { padding: 0px; margin: 0px; } "
                  "QMainWindow::separator { width: 0px; height: 0px; }");

    // 创建中央堆叠容器（用于切换单视图/多视图）
    centralStack_ = new QStackedWidget(this);
    centralStack_->setContentsMargins(0, 0, 0, 0);

    // 创建中央图像查看器
#ifdef USE_HALCON
    imageViewer_ = new HalconImageViewer(this);
#else
    imageViewer_ = new ImageViewer(this);
#endif

    // 将图像查看器添加到堆叠容器
    centralStack_->addWidget(imageViewer_);

    // 设置堆叠容器为中央窗口
    setCentralWidget(centralStack_);

    // 移除所有可能的边距
    centralWidget()->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    // 设置主窗口最大化显示，以获得最大的Halcon窗口尺寸
    showMaximized();

    // 创建停靠窗口和面板
    createDockWindows();

    // 创建菜单和工具栏
    createMenus();
    createToolBars();
    createStatusBar();

    // 在视图菜单中添加停靠窗口切换选项（必须在createDockWindows和createMenus之后）
    addDockWidgetsToViewMenu();

    // 连接信号
    connectSignals();

    // 连接VisionEngine信号
    connectEngineSignals();

    // 创建模拟相机作为默认相机（备用）
    // 注意：不传递parent，让unique_ptr完全管理生命周期，避免双重删除
    HAL::SimulatedCamera* simCamera = new HAL::SimulatedCamera(nullptr);
    simCamera->useTestPattern(0);  // 使用渐变测试图案
    engine_.setCamera(std::unique_ptr<HAL::ICamera>(simCamera));

    // 初始化配方管理器（扫描目录并加载上次使用的配方）
    Core::RecipeManager::instance().initialize();

    // 初始化多工位管理器
    Core::MultiStationManager::instance().loadConfig();

    // 工位和场景切换功能（通过StationConfigTool.exe配置，主界面不显示切换栏）
#ifdef USE_HALCON
    stationSwitchBar_ = new StationSwitchBar(this);
    stationSwitchBar_->setVisible(false);  // 隐藏，配置通过StationConfigTool完成

    // 连接工位切换信号（保留功能，但不显示UI）
    connect(stationSwitchBar_, &StationSwitchBar::stationSelected,
            this, &MainWindow::onStationSelected);
    connect(stationSwitchBar_, &StationSwitchBar::showAllStations,
            this, &MainWindow::onShowAllStations);

    // 场景切换栏（隐藏，配置通过StationConfigTool完成）
    sceneSwitchBar_ = new SceneSwitchBar(this);
    sceneSwitchBar_->setVisible(false);  // 隐藏，配置通过StationConfigTool完成

    // 连接场景切换信号
    connect(sceneSwitchBar_, &SceneSwitchBar::sceneSelected,
            this, &MainWindow::onSceneSelected);
    connect(sceneSwitchBar_, &SceneSwitchBar::previousSceneRequested,
            this, &MainWindow::onPreviousSceneRequested);
    connect(sceneSwitchBar_, &SceneSwitchBar::nextSceneRequested,
            this, &MainWindow::onNextSceneRequested);

    // 创建多相机视图并添加到堆叠容器
    multiCameraView_ = new MultiCameraView(this);
    centralStack_->addWidget(multiCameraView_);

    // 连接多相机视图信号
    connect(multiCameraView_, &MultiCameraView::viewSelected,
            this, &MainWindow::onMultiViewSelected);

    // 根据工位配置初始化界面
    auto& stationManager = Core::MultiStationManager::instance();
    auto& toolChainManager = Core::PositionToolChainManager::instance();

    int stationCount = stationManager.getStationCount();
    if (stationCount > 0) {
        // 刷新工位切换栏（内部功能保留，UI隐藏）
        stationSwitchBar_->refreshStations();

        // 获取当前工位配置
        auto* currentStation = stationManager.currentStation();
        if (currentStation) {
            // 初始化工位的工具链管理
            toolChainManager.initializeStation(currentStation);

            // 设置多相机视图
            multiCameraView_->setStation(currentStation);

            // 刷新场景切换栏（内部功能保留，UI隐藏）
            if (sceneSwitchBar_) {
                sceneSwitchBar_->refreshScenes(currentStation);
            }

            // 如果位置数量大于1，默认显示多相机视图
            if (currentStation->positionNum > 1) {
                // 切换到多相机视图模式（使用堆叠容器切换）
                isMultiViewMode_ = true;
                centralStack_->setCurrentWidget(multiCameraView_);

                // 设置当前位置为第一个位置，并初始化工具链
                if (!currentStation->positionBindings.isEmpty()) {
                    QString firstPosId = currentStation->positionBindings.first().positionId;
                    toolChainManager.setCurrentPosition(currentStation->id, firstPosId);

                    // 更新工具链面板显示当前位置的工具
                    auto tools = toolChainManager.currentTools();
                    toolChainPanel_->clear();
                    for (auto* tool : tools) {
                        toolChainPanel_->addTool(tool);
                    }
                }

                LOG_INFO(QString("多工位模式: %1 个工位, 当前工位 %2 个位置")
                         .arg(stationCount).arg(currentStation->positionNum));
            }
        }
    }
#endif

    // 尝试自动连接已保存的相机配置
    if (tryAutoConnectCamera()) {
        LOG_INFO("已自动连接保存的相机配置");
    } else {
        LOG_DEBUG("未能自动连接相机，将弹出相机配置对话框");
        // 延迟弹出相机配置对话框，确保主窗口构造完成后再弹出
        QTimer::singleShot(100, this, [this]() {
            showCameraConfigOnStartup();
        });
    }

    // 更新界面状态
    updateActions();
    updateStatusBar();

    LOG_INFO("主窗口初始化完成");
}

MainWindow::~MainWindow()
{
    // 停止连续采集
    engine_.stopLiveGrab();

    // 清理工具链中的工具
    QList<Algorithm::VisionTool*> tools = toolChainPanel_->getTools();
    for (Algorithm::VisionTool* tool : tools) {
        delete tool;
    }
}

// ========== 文件菜单 ==========

void MainWindow::onOpenImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "打开图像", "",
        "图像文件 (*.png *.jpg *.jpeg *.bmp *.tiff);;所有文件 (*.*)");

    if (fileName.isEmpty()) {
        return;
    }

    // 使用VisionEngine加载图像
    if (engine_.loadImage(fileName)) {
        updateActions();
        updateStatusBar();
    } else {
        QMessageBox::warning(this, "错误", "无法加载图像文件");
    }
}

void MainWindow::onOpenImageFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
        "选择图片文件夹", "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (folderPath.isEmpty()) {
        return;
    }

    // 使用VisionEngine加载图片文件夹
    int count = engine_.loadImageFolder(folderPath);

    if (count == 0) {
        QMessageBox::information(this, "提示", "所选文件夹中没有找到图片文件");
    }
    // 状态更新由VisionEngine的信号处理
}

void MainWindow::onPreviousImage()
{
    // 使用VisionEngine切换到上一张图片
    engine_.previousImage();
    // 状态更新由VisionEngine的信号处理
}

void MainWindow::onNextImage()
{
    // 使用VisionEngine切换到下一张图片
    engine_.nextImage();
    // 状态更新由VisionEngine的信号处理
}

void MainWindow::onRunAllImages()
{
    // 从VisionEngine获取图像序列
    const QStringList& imageSequence = engine_.imageSequence();

    if (imageSequence.isEmpty()) {
        QMessageBox::information(this, "提示", "请先加载图片文件夹");
        return;
    }

    QList<Algorithm::VisionTool*> tools = toolChainPanel_->getTools();
    if (tools.isEmpty()) {
        QMessageBox::information(this, "提示", "请先添加处理工具");
        return;
    }

    int totalImages = imageSequence.size();

    statusLabel_->setText(QString("正在批量处理 %1 张图片...").arg(totalImages));

    // 禁用UI控件，防止在处理期间进行其他操作
    setEnabled(false);
    statusLabel_->setText(QString("批量处理中: 0/%1").arg(totalImages));

    // 创建工作线程（使用BatchProcessWorker替代InspectionWorker）
    auto worker = new BatchProcessWorker(imageSequence, tools, this);

    // 连接进度信号（新增百分比参数）
    connect(worker, &BatchProcessWorker::progress, this,
            [this](int current, int total, double percentage) {
        statusLabel_->setText(QString("批量处理中: %1/%2 (%3%)")
            .arg(current).arg(total).arg(percentage, 0, 'f', 1));
    });

    // 连接单张图片处理完成信号（使用FileProcessResult结构）
    connect(worker, &BatchProcessWorker::fileProcessed, this,
            [this](const FileProcessResult& result) {
        if (result.success) {
            LOG_DEBUG(QString("图片 %1 处理成功，耗时: %2 ms")
                     .arg(QFileInfo(result.filePath).fileName())
                     .arg(result.processTime, 0, 'f', 2));
        } else {
            LOG_WARNING(QString("图片 %1 处理失败: %2")
                       .arg(QFileInfo(result.filePath).fileName())
                       .arg(result.errorMessage));
        }
    });

    // 连接完成信号（使用BatchProcessResult结构）
    connect(worker, &BatchProcessWorker::finished, this,
            [this, worker](const BatchProcessResult& result) {
        // 重新启用UI
        setEnabled(true);

        // 显示完成信息
        QString resultMsg = QString("批量处理完成!\n"
                                    "成功: %1 张\n"
                                    "失败: %2 张\n"
                                    "总耗时: %3 ms\n"
                                    "平均耗时: %4 ms/张")
            .arg(result.successCount)
            .arg(result.failedCount)
            .arg(result.totalTime, 0, 'f', 2)
            .arg(result.avgTime, 0, 'f', 2);

        if (!result.success && !result.errorMessage.isEmpty()) {
            resultMsg += QString("\n\n错误: %1").arg(result.errorMessage);
        }

        QMessageBox::information(this, "批量处理完成", resultMsg);
        statusLabel_->setText(QString("批量处理完成: 成功%1张, 失败%2张, 耗时%.2f ms")
            .arg(result.successCount).arg(result.failedCount).arg(result.totalTime));

        updateImageSequenceActions();

        LOG_INFO(QString("批量处理完成: 成功%1张, 失败%2张, 总耗时%3ms")
            .arg(result.successCount).arg(result.failedCount).arg(result.totalTime, 0, 'f', 2));

        // 自动删除工作线程
        worker->deleteLater();
    });

    // 启动工作线程
    worker->start();
}

void MainWindow::onSaveImage()
{
    // 使用VisionEngine保存图像
    QString fileName = QFileDialog::getSaveFileName(this,
        "保存图像", "",
        "PNG图像 (*.png);;JPEG图像 (*.jpg *.jpeg);;BMP图像 (*.bmp)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!engine_.saveImage(fileName)) {
        QMessageBox::warning(this, "错误", "保存图像失败");
    }
    // 状态更新由VisionEngine的信号处理
}

void MainWindow::onExit()
{
    close();
}

// ========== 视图菜单 ==========

void MainWindow::onFitToWindow()
{
    imageViewer_->fitToWindow();
}

void MainWindow::onActualSize()
{
    imageViewer_->actualSize();
}

void MainWindow::onZoomIn()
{
    imageViewer_->zoomIn();
}

void MainWindow::onZoomOut()
{
    imageViewer_->zoomOut();
}

void MainWindow::onTogglePanels()
{
    // 检查当前面板是否全部隐藏
    bool allHidden = toolChainDock_->isHidden() &&
                     toolParameterDock_->isHidden() &&
                     resultTableDock_->isHidden();

    // 切换显示状态
    if (allHidden) {
        // 全部隐藏时，恢复显示（历史面板保持隐藏）
        toolChainDock_->show();
        toolParameterDock_->show();
        resultTableDock_->show();
        statusLabel_->setText("已显示侧边面板");
        LOG_DEBUG("侧边面板已显示");
    } else {
        // 隐藏所有侧边面板，最大化Halcon图像窗口
        toolChainDock_->hide();
        toolParameterDock_->hide();
        resultTableDock_->hide();
        historyDock_->hide();
        statusLabel_->setText("已隐藏侧边面板 - Halcon窗口最大化");
        LOG_DEBUG("侧边面板已隐藏");
    }
}

// ========== 工具菜单 ==========

void MainWindow::onAddTool()
{
    showAddToolDialog();
}

void MainWindow::onRemoveTool()
{
    Algorithm::VisionTool* tool = toolChainPanel_->getCurrentTool();
    if (!tool) {
        return;
    }

    int ret = QMessageBox::question(this, "确认删除",
        QString("确定要删除工具 \"%1\" 吗？").arg(tool->displayName()),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        toolChainPanel_->removeTool(tool);

        // 同步到PositionToolChainManager (多工位模式)
        // 注意：removeTool会删除工具，所以这里需要在删除前从Manager中移除
#ifdef USE_HALCON
        if (isMultiViewMode_) {
            auto& toolChainManager = Core::PositionToolChainManager::instance();
            // 标记当前工具链已修改（工具已从面板移除）
            toolChainManager.markCurrentAsModified();
        }
#endif

        delete tool;

        statusLabel_->setText("已删除工具");
        updateActions();
    }
}

void MainWindow::onRunSingle()
{
    Algorithm::VisionTool* tool = toolChainPanel_->getCurrentTool();
    if (!tool || !engine_.currentImage()) {
        return;
    }

    if (!tool->isEnabled()) {
        QMessageBox::information(this, "提示", "当前工具已禁用");
        return;
    }

    // 使用异步执行，避免阻塞UI
    statusLabel_->setText(QString("正在运行工具: %1...").arg(tool->displayName()));
    engine_.executeToolAsync(tool);
}

void MainWindow::onRunAll()
{
    if (!engine_.currentImage()) {
        QMessageBox::information(this, "提示", "请先加载或采集图像");
        return;
    }

    QList<Algorithm::VisionTool*> tools = toolChainPanel_->getTools();
    if (tools.isEmpty()) {
        QMessageBox::information(this, "提示", "工具链为空");
        return;
    }

    statusLabel_->setText("正在运行工具链...");
    engine_.executeToolChainAsync(tools);
}

// ========== 相机操作 ==========

void MainWindow::onFrameValidToggled(bool checked)
{
    isFrameValid_ = checked;

    if (checked) {
        // 帧有效：触发单帧采集
        if (engine_.camera() && engine_.camera()->isOpen()) {
            bool wasGrabbing = engine_.camera()->isGrabbing();

            // 如果相机未在采集，先启动
            if (!wasGrabbing) {
                if (!engine_.camera()->startGrabbing()) {
                    LOG_ERROR("启动相机采集失败");
                    statusLabel_->setText("帧有效: 采集失败");
                    frameValidAction_->setChecked(false);
                    return;
                }
            }

            // 触发采集（软触发模式）
            if (engine_.camera()->getConfig().triggerMode == HAL::ICamera::Software) {
                engine_.camera()->trigger();
            }

            // 获取图像
            Base::ImageData::Ptr image = engine_.camera()->grabImage(1000);

            // 如果之前未在连续采集，停止采集
            if (!wasGrabbing && !engine_.isLiveGrabbing()) {
                engine_.camera()->stopGrabbing();
            }

            if (image) {
                // 应用图像变换（旋转、镜像）
                engine_.applyImageTransform(image);
                
                // 设置为当前图像
                engine_.setCurrentImage(image);

                // 异步执行工具链
                QList<Algorithm::VisionTool*> tools = toolChainPanel_->getTools();
                if (!tools.isEmpty()) {
                    engine_.executeToolChainAsync(tools);
                }

                statusLabel_->setText("帧有效: 采集成功");
                LOG_INFO("帧有效采集成功");
            } else {
                statusLabel_->setText("帧有效: 获取图像失败");
                LOG_WARNING("帧有效获取图像失败");
            }
        } else {
            LOG_WARNING("相机未连接或未打开");
            statusLabel_->setText("帧有效: 相机未就绪");
        }

        // 重置为未选中状态（单次触发）
        frameValidAction_->setChecked(false);
    }
}

void MainWindow::onLiveDisplayToggled(bool checked)
{
    isLiveDisplay_ = checked;

    if (checked) {
        // 启用实时显示 - 开始连续采集
        if (engine_.camera() && engine_.camera()->isOpen()) {
            if (!engine_.isLiveGrabbing()) {
                // 先启动相机采集
                if (!engine_.camera()->isGrabbing()) {
                    if (!engine_.camera()->startGrabbing()) {
                        LOG_ERROR("启动相机采集失败");
                        statusLabel_->setText("实时显示: 启动失败");
                        liveDisplayAction_->setChecked(false);
                        return;
                    }
                }
                engine_.startLiveGrab(150);  // 约6-7 FPS，减轻CPU负担
            }
        } else {
            LOG_WARNING("相机未连接或未打开，无法启用实时显示");
            statusLabel_->setText("实时显示: 相机未就绪");
            liveDisplayAction_->setChecked(false);
            return;
        }
        statusLabel_->setText("实时显示: 已启用");
        LOG_INFO("实时显示已启用");
    } else {
        // 禁用实时显示 - 停止连续采集
        if (engine_.isLiveGrabbing()) {
            engine_.stopLiveGrab();
            // 停止相机采集
            if (engine_.camera() && engine_.camera()->isGrabbing()) {
                engine_.camera()->stopGrabbing();
            }
        }
        statusLabel_->setText("实时显示: 已禁用");
        LOG_INFO("实时显示已禁用");
    }
}

// ========== 工具链 ==========

void MainWindow::onToolSelectionChanged(Algorithm::VisionTool* tool)
{
    toolParameterPanel_->setTool(tool);
    updateActions();
}

void MainWindow::onToolDoubleClicked(Algorithm::VisionTool* tool)
{
    if (!tool) return;

#ifdef USE_HALCON
    // 形状匹配工具需要特殊处理（连接训练信号）
    if (Algorithm::ShapeMatchTool* shapeMatchTool =
        dynamic_cast<Algorithm::ShapeMatchTool*>(tool)) {
        ShapeMatchToolDialog dialog(shapeMatchTool, this);

        if (engine_.currentImage()) {
            dialog.setImage(engine_.currentImage());
        }

        // 连接训练请求信号
        connect(&dialog, &ShapeMatchToolDialog::trainModelRequested, this, [this, shapeMatchTool]() {
            std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
            if (!rois.empty() && engine_.currentImage()) {
                ROIShapePtr roi = rois.front();
                QRectF rect = roi->boundingRect();

                if (shapeMatchTool->trainModel(engine_.currentImage(),
                    static_cast<int>(rect.top()),
                    static_cast<int>(rect.left()),
                    static_cast<int>(rect.bottom()),
                    static_cast<int>(rect.right()))) {
                    LOG_INFO("模板训练成功");
                    statusLabel_->setText("模板训练成功");
                } else {
                    QMessageBox::warning(this, "训练失败", "模板训练失败，请检查ROI区域");
                }
            }
        });

        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("工具参数已更新");
            updateActions();
        }
        return;
    }
#endif

    // 使用工具的 createConfigDialog 方法创建对话框
    // 回调由 ToolDialogFactory 在初始化时设置
    QDialog* dialog = tool->createConfigDialog(this, engine_.currentImage());

    if (dialog) {
        // 尝试连接预览信号（如果对话框支持）
        // 使用旧式连接语法，若信号不存在会静默失败
        QObject::connect(dialog, SIGNAL(previewRequested()),
                        this, SLOT(onRunSingle()));

        if (dialog->exec() == QDialog::Accepted) {
            statusLabel_->setText(QString("工具 %1 参数已更新").arg(tool->displayName()));
        }

        delete dialog;
    } else {
        // 工具没有注册对话框，提示使用参数面板
        QMessageBox::information(this, "提示",
            QString("工具 \"%1\" 的参数可在右侧参数面板中编辑").arg(tool->displayName()));
    }
}

void MainWindow::onParameterChanged()
{
    // 参数改变，可以选择自动重新处理
    // statusLabel_->setText("工具参数已改变");
}

// ========== 图像查看器 ==========

void MainWindow::onScaleChanged(double scale)
{
    scaleLabel_->setText(QString("缩放: %1%").arg(static_cast<int>(scale * 100)));
}

void MainWindow::onMousePositionChanged(int x, int y, bool valid)
{
    if (valid) {
        positionLabel_->setText(QString("位置: (%1, %2)").arg(x).arg(y));
    } else {
        positionLabel_->setText("位置: --");
    }
}

// ========== 连续采集 ==========

void MainWindow::onContinuousTimer()
{
    // 防止重入
    static bool isGrabbing = false;
    static int consecutiveFailures = 0;  // 连续失败次数
    static int successCount = 0;         // 成功计数

    if (isGrabbing) {
        return;
    }

    if (!engine_.camera() || !engine_.camera()->isOpen()) {
        engine_.stopLiveGrab();
        liveDisplayAction_->setChecked(false);
        consecutiveFailures = 0;
        successCount = 0;
        return;
    }

    // 检查是否需要显示（实时显示开启时才更新画面）
    if (!isLiveDisplay_) {
        return;
    }

    isGrabbing = true;

    try {
        // 连续采集模式下不需要trigger，直接获取图像
        // 如果是软触发模式才需要调用trigger
        if (engine_.camera()->getConfig().triggerMode == HAL::ICamera::Software) {
            engine_.camera()->trigger();
        }

        // 使用较长超时避免超时错误，连续采集模式下相机应该持续输出帧
        Base::ImageData::Ptr image = engine_.camera()->grabImage(1000);

        if (image) {
            // 重置失败计数
            consecutiveFailures = 0;
            successCount++;

            // 应用图像变换（旋转、镜像）
            engine_.applyImageTransform(image);

            // 实时显示：只显示图像，不运行工具
            displayImage(image);
        } else {
            // 图像为空（可能是临时超时）
            consecutiveFailures++;

            // 只在首次失败或每隔10次失败时记录日志，避免刷屏
            if (consecutiveFailures == 1 || consecutiveFailures % 10 == 0) {
                LOG_WARNING(QString("连续采集：获取图像失败（第%1次）").arg(consecutiveFailures));
            }

            // 如果连续失败超过30次（约30秒），尝试重启采集
            if (consecutiveFailures >= 30) {
                LOG_WARNING("连续采集失败次数过多，尝试重启采集");
                engine_.camera()->stopGrabbing();
                QThread::msleep(100);
                if (engine_.camera()->startGrabbing()) {
                    consecutiveFailures = 0;
                    LOG_INFO("重启采集成功");
                } else {
                    LOG_ERROR("重启采集失败，停止实时显示");
                    engine_.stopLiveGrab();
                    liveDisplayAction_->setChecked(false);
                }
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR(QString("连续采集异常: %1").arg(e.what()));
        consecutiveFailures++;
    } catch (...) {
        LOG_ERROR("连续采集未知异常");
        consecutiveFailures++;
    }

    isGrabbing = false;

    // 让出CPU时间给UI，避免界面卡顿
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

// ========== 私有方法 ==========

void MainWindow::createMenus()
{
    // 文件菜单
    fileMenu_ = menuBar()->addMenu("文件(&F)");

    openImageAction_ = new QAction(Theme::getIcon(Icons::FILE_OPEN), "打开图像(&O)...", this);
    openImageAction_->setShortcut(QKeySequence::Open);
    openImageAction_->setStatusTip("打开图像文件");
    connect(openImageAction_, &QAction::triggered, this, &MainWindow::onOpenImage);
    fileMenu_->addAction(openImageAction_);

    openImageFolderAction_ = new QAction(Theme::getIcon(Icons::FILE_OPEN), "打开图片文件夹(&D)...", this);
    openImageFolderAction_->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_O);
    openImageFolderAction_->setStatusTip("打开图片文件夹，加载多张图片");
    connect(openImageFolderAction_, &QAction::triggered, this, &MainWindow::onOpenImageFolder);
    fileMenu_->addAction(openImageFolderAction_);

    fileMenu_->addSeparator();

    // 图片序列导航菜单
    previousImageAction_ = new QAction(Theme::getIcon(Icons::TOOL_STOP), "上一张图片(&P)", this);
    previousImageAction_->setShortcut(Qt::Key_Left);
    previousImageAction_->setStatusTip("显示上一张图片");
    previousImageAction_->setEnabled(false);
    connect(previousImageAction_, &QAction::triggered, this, &MainWindow::onPreviousImage);
    fileMenu_->addAction(previousImageAction_);

    nextImageAction_ = new QAction(Theme::getIcon(Icons::TOOL_RUN), "下一张图片(&N)", this);
    nextImageAction_->setShortcut(Qt::Key_Right);
    nextImageAction_->setStatusTip("显示下一张图片");
    nextImageAction_->setEnabled(false);
    connect(nextImageAction_, &QAction::triggered, this, &MainWindow::onNextImage);
    fileMenu_->addAction(nextImageAction_);

    runAllImagesAction_ = new QAction(Theme::getIcon(Icons::TOOL_RUN), "批量执行所有图片(&B)", this);
    runAllImagesAction_->setShortcut(Qt::Key_F8);
    runAllImagesAction_->setStatusTip("依次执行所有图片的工具处理（F8）");
    runAllImagesAction_->setEnabled(false);
    connect(runAllImagesAction_, &QAction::triggered, this, &MainWindow::onRunAllImages);
    fileMenu_->addAction(runAllImagesAction_);

    fileMenu_->addSeparator();

    saveImageAction_ = new QAction(Theme::getIcon(Icons::FILE_SAVE), "保存图像(&S)...", this);
    saveImageAction_->setShortcut(QKeySequence::Save);
    saveImageAction_->setStatusTip("保存当前图像");
    connect(saveImageAction_, &QAction::triggered, this, &MainWindow::onSaveImage);
    fileMenu_->addAction(saveImageAction_);

    fileMenu_->addSeparator();

    exitAction_ = new QAction(Theme::getIcon(Icons::EXIT), "退出(&X)", this);
    exitAction_->setShortcut(QKeySequence::Quit);
    exitAction_->setStatusTip("退出应用程序");
    connect(exitAction_, &QAction::triggered, this, &MainWindow::onExit);
    fileMenu_->addAction(exitAction_);

    // 视图菜单
    viewMenu_ = menuBar()->addMenu("视图(&V)");

    fitToWindowAction_ = new QAction(Theme::getIcon(Icons::ZOOM_FIT), "适应窗口(&F)", this);
    fitToWindowAction_->setStatusTip("缩放图像以适应窗口");
    connect(fitToWindowAction_, &QAction::triggered, this, &MainWindow::onFitToWindow);
    viewMenu_->addAction(fitToWindowAction_);

    actualSizeAction_ = new QAction(Theme::getIcon(Icons::ZOOM_ORIGINAL), "实际大小(&A)", this);
    actualSizeAction_->setStatusTip("显示图像原始大小（100%）");
    connect(actualSizeAction_, &QAction::triggered, this, &MainWindow::onActualSize);
    viewMenu_->addAction(actualSizeAction_);

    viewMenu_->addSeparator();

    zoomInAction_ = new QAction(Theme::getIcon(Icons::ZOOM_IN), "放大(&I)", this);
    zoomInAction_->setShortcut(QKeySequence::ZoomIn);
    zoomInAction_->setStatusTip("放大图像");
    connect(zoomInAction_, &QAction::triggered, this, &MainWindow::onZoomIn);
    viewMenu_->addAction(zoomInAction_);

    zoomOutAction_ = new QAction(Theme::getIcon(Icons::ZOOM_OUT), "缩小(&O)", this);
    zoomOutAction_->setShortcut(QKeySequence::ZoomOut);
    zoomOutAction_->setStatusTip("缩小图像");
    connect(zoomOutAction_, &QAction::triggered, this, &MainWindow::onZoomOut);
    viewMenu_->addAction(zoomOutAction_);

    viewMenu_->addSeparator();

    // 添加切换面板动作（F11快捷键）
    QAction* togglePanelsAction = new QAction("切换侧边面板(&P)", this);
    togglePanelsAction->setShortcut(Qt::Key_F11);
    togglePanelsAction->setStatusTip("显示/隐藏所有侧边面板，最大化Halcon窗口（F11）");
    togglePanelsAction->setCheckable(false);
    connect(togglePanelsAction, &QAction::triggered, this, &MainWindow::onTogglePanels);
    viewMenu_->addAction(togglePanelsAction);

    // 工具菜单
    toolMenu_ = menuBar()->addMenu("工具(&T)");

    addToolAction_ = new QAction(Theme::getIcon(Icons::TOOL_ADD), tr("添加工具(&A)..."), this);
    addToolAction_->setShortcut(Qt::CTRL | Qt::Key_T);
    addToolAction_->setStatusTip(tr("添加新的视觉处理工具 (Ctrl+T)"));
    connect(addToolAction_, &QAction::triggered, this, &MainWindow::onAddTool);
    toolMenu_->addAction(addToolAction_);

    removeToolAction_ = new QAction(Theme::getIcon(Icons::TOOL_REMOVE), tr("删除工具(&R)"), this);
    removeToolAction_->setShortcut(Qt::Key_Delete);
    removeToolAction_->setStatusTip(tr("删除选中的工具 (Delete)"));
    connect(removeToolAction_, &QAction::triggered, this, &MainWindow::onRemoveTool);
    toolMenu_->addAction(removeToolAction_);

    toolMenu_->addSeparator();

    runSingleAction_ = new QAction(Theme::getIcon(Icons::TOOL_RUN), "运行当前工具(&S)", this);
    runSingleAction_->setShortcut(Qt::Key_F5);
    runSingleAction_->setStatusTip("运行当前选中的工具（F5）");
    connect(runSingleAction_, &QAction::triggered, this, &MainWindow::onRunSingle);
    toolMenu_->addAction(runSingleAction_);

    runAllAction_ = new QAction(Theme::getIcon(Icons::TOOL_RUN), "运行所有工具(&A)", this);
    runAllAction_->setShortcut(Qt::Key_F6);
    runAllAction_->setStatusTip("运行工具链中的所有工具（F6）");
    connect(runAllAction_, &QAction::triggered, this, &MainWindow::onRunAll);
    toolMenu_->addAction(runAllAction_);

    // 相机菜单
    cameraMenu_ = menuBar()->addMenu("相机(&C)");

    // 帧有效切换
    frameValidAction_ = new QAction(Theme::getIcon(Icons::TOOL_RUN), "帧有效(&F)", this);
    frameValidAction_->setCheckable(true);
    frameValidAction_->setChecked(true);
    frameValidAction_->setStatusTip("切换帧有效状态");
    connect(frameValidAction_, &QAction::toggled, this, &MainWindow::onFrameValidToggled);
    cameraMenu_->addAction(frameValidAction_);

    // 实时显示切换
    liveDisplayAction_ = new QAction(Theme::getIcon(Icons::CAMERA_VIDEO), "实时显示(&L)", this);
    liveDisplayAction_->setCheckable(true);
    liveDisplayAction_->setChecked(true);
    liveDisplayAction_->setStatusTip("切换实时显示状态");
    connect(liveDisplayAction_, &QAction::toggled, this, &MainWindow::onLiveDisplayToggled);
    cameraMenu_->addAction(liveDisplayAction_);

    cameraMenu_->addSeparator();

    cameraConfigAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "相机配置(&S)...", this);
    cameraConfigAction_->setStatusTip("配置相机设备和参数");
    connect(cameraConfigAction_, &QAction::triggered, this, &MainWindow::onCameraConfig);
    cameraMenu_->addAction(cameraConfigAction_);

    // 标定菜单
    calibMenu_ = menuBar()->addMenu("标定(&B)");

    cameraCalibAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "相机标定(&C)...", this);
    cameraCalibAction_->setStatusTip("相机内参和畸变校正标定");
    connect(cameraCalibAction_, &QAction::triggered, this, &MainWindow::onCameraCalibration);
    calibMenu_->addAction(cameraCalibAction_);

    ninePointCalibAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "九点标定(&N)...", this);
    ninePointCalibAction_->setStatusTip("图像坐标到物理坐标映射标定");
    connect(ninePointCalibAction_, &QAction::triggered, this, &MainWindow::onNinePointCalibration);
    calibMenu_->addAction(ninePointCalibAction_);

    qrCalibAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "QR码标定(&Q)...", this);
    qrCalibAction_->setStatusTip("基于QR码/DM码的自动标定");
    connect(qrCalibAction_, &QAction::triggered, this, &MainWindow::onQRCalibration);
    calibMenu_->addAction(qrCalibAction_);

    // 对位菜单
    alignmentMenu_ = menuBar()->addMenu("对位(&A)");

    multiCameraManagerAction_ = new QAction(Theme::getIcon(Icons::CAMERA_PHOTO), "多相机管理(&M)...", this);
    multiCameraManagerAction_->setStatusTip("管理多个相机和分组采集");
    connect(multiCameraManagerAction_, &QAction::triggered, this, &MainWindow::onMultiCameraManager);
    alignmentMenu_->addAction(multiCameraManagerAction_);

    multiPointAlignmentAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "多点对位配置(&P)...", this);
    multiPointAlignmentAction_->setStatusTip("配置多点对位检测参数");
    connect(multiPointAlignmentAction_, &QAction::triggered, this, &MainWindow::onMultiPointAlignment);
    alignmentMenu_->addAction(multiPointAlignmentAction_);

    workStationConfigAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "工位配置(&W)...", this);
    workStationConfigAction_->setStatusTip("配置工位位置、对位方式和补偿参数");
    connect(workStationConfigAction_, &QAction::triggered, this, &MainWindow::onWorkStationConfig);
    alignmentMenu_->addAction(workStationConfigAction_);

    alignmentMenu_->addSeparator();

    alignmentOutputAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "对位输出配置(&O)...", this);
    alignmentOutputAction_->setStatusTip("配置对位结果输出（PLC/TCP/串口）");
    connect(alignmentOutputAction_, &QAction::triggered, this, &MainWindow::onAlignmentOutput);
    alignmentMenu_->addAction(alignmentOutputAction_);

    alignmentMenu_->addSeparator();

    // 多工位调试功能
#ifdef USE_HALCON
    manualDebugAction_ = new QAction(Theme::getIcon(Icons::TOOL_RUN), "手动调试中心(&D)...", this);
    manualDebugAction_->setShortcut(Qt::Key_F8);
    manualDebugAction_->setStatusTip("打开多工位手动调试中心（F8）");
    connect(manualDebugAction_, &QAction::triggered, this, &MainWindow::onManualDebugCenter);
    alignmentMenu_->addAction(manualDebugAction_);

    calibWizardAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "标定向导(&W)...", this);
    calibWizardAction_->setStatusTip("多位置标定向导");
    connect(calibWizardAction_, &QAction::triggered, this, &MainWindow::onCalibrationWizard);
    alignmentMenu_->addAction(calibWizardAction_);

    alignmentMenu_->addSeparator();

    toggleMultiViewAction_ = new QAction(Theme::getIcon(Icons::CAMERA_VIDEO), "切换多相机视图(&V)", this);
    toggleMultiViewAction_->setShortcut(Qt::Key_F9);
    toggleMultiViewAction_->setCheckable(true);
    toggleMultiViewAction_->setChecked(false);
    toggleMultiViewAction_->setStatusTip("切换单相机/多相机显示模式（F9）");
    connect(toggleMultiViewAction_, &QAction::triggered, this, &MainWindow::onToggleMultiCameraView);
    alignmentMenu_->addAction(toggleMultiViewAction_);
#endif

    // 默认隐藏对位菜单（检测模式不需要）
    alignmentMenu_->menuAction()->setVisible(false);

    // 通信菜单
    commMenu_ = menuBar()->addMenu("通信(&M)");

    plcConfigAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "PLC配置(&P)...", this);
    plcConfigAction_->setStatusTip("配置PLC通信连接");
    connect(plcConfigAction_, &QAction::triggered, this, &MainWindow::onPLCConfig);
    commMenu_->addAction(plcConfigAction_);

    // 设置菜单
    settingsMenu_ = menuBar()->addMenu("设置(&S)");

    systemSettingsAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), "系统设置(&S)...", this);
    systemSettingsAction_->setStatusTip("配置系统设置（GPU加速等）");
    connect(systemSettingsAction_, &QAction::triggered, this, &MainWindow::onSystemSettings);
    settingsMenu_->addAction(systemSettingsAction_);

    settingsMenu_->addSeparator();

    // 主题子菜单
    QMenu* themeMenu = settingsMenu_->addMenu(tr("主题(&T)"));

    QActionGroup* themeGroup = new QActionGroup(this);
    themeGroup->setExclusive(true);

    QAction* industrialDarkAction = new QAction("工业深色 (蓝色)", this);
    industrialDarkAction->setCheckable(true);
    industrialDarkAction->setChecked(true);  // 默认主题
    industrialDarkAction->setData(static_cast<int>(Theme::IndustrialDark));
    themeGroup->addAction(industrialDarkAction);
    themeMenu->addAction(industrialDarkAction);

    QAction* industrialLightAction = new QAction("工业浅色 (蓝色)", this);
    industrialLightAction->setCheckable(true);
    industrialLightAction->setData(static_cast<int>(Theme::IndustrialLight));
    themeGroup->addAction(industrialLightAction);
    themeMenu->addAction(industrialLightAction);

    themeMenu->addSeparator();

    QAction* classicDarkAction = new QAction("经典深色 (绿色)", this);
    classicDarkAction->setCheckable(true);
    classicDarkAction->setData(static_cast<int>(Theme::ClassicDark));
    themeGroup->addAction(classicDarkAction);
    themeMenu->addAction(classicDarkAction);

    QAction* classicLightAction = new QAction("经典浅色 (绿色)", this);
    classicLightAction->setCheckable(true);
    classicLightAction->setData(static_cast<int>(Theme::ClassicLight));
    themeGroup->addAction(classicLightAction);
    themeMenu->addAction(classicLightAction);

    // 连接主题切换信号
    connect(themeGroup, &QActionGroup::triggered, this, [](QAction* action) {
        Theme::ThemeType type = static_cast<Theme::ThemeType>(action->data().toInt());
        Theme::applyTheme(type);
    });

    // UI模式子菜单
    QMenu* modeMenu = settingsMenu_->addMenu(tr("界面模式(&M)"));

    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);

    QAction* simpleModeAction = new QAction(tr("简单模式 - 操作员"), this);
    simpleModeAction->setCheckable(true);
    simpleModeAction->setData(static_cast<int>(UIMode::Simple));
    simpleModeAction->setStatusTip(tr("简化界面，隐藏高级功能，适合日常操作"));
    modeGroup->addAction(simpleModeAction);
    modeMenu->addAction(simpleModeAction);

    QAction* professionalModeAction = new QAction(tr("专业模式 - 工程师"), this);
    professionalModeAction->setCheckable(true);
    professionalModeAction->setData(static_cast<int>(UIMode::Professional));
    professionalModeAction->setStatusTip(tr("显示所有功能，适合开发和调试"));
    modeGroup->addAction(professionalModeAction);
    modeMenu->addAction(professionalModeAction);

    // 根据当前模式设置选中状态
    if (UIModeManager::instance().currentMode() == UIMode::Simple) {
        simpleModeAction->setChecked(true);
    } else {
        professionalModeAction->setChecked(true);
    }

    // 连接模式切换信号
    connect(modeGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        UIMode mode = static_cast<UIMode>(action->data().toInt());
        UIModeManager::instance().setMode(mode);
    });

    // 连接UIModeManager的模式变更信号
    connect(&UIModeManager::instance(), &UIModeManager::modeChanged,
            this, &MainWindow::onUIModeChanged);

    // 添加模式切换快捷键 (Ctrl+M)
    QAction* toggleModeAction = new QAction(tr("切换界面模式"), this);
    toggleModeAction->setShortcut(Qt::CTRL | Qt::Key_M);
    connect(toggleModeAction, &QAction::triggered, this, []() {
        UIModeManager::instance().toggleMode();
    });
    addAction(toggleModeAction);  // 添加到主窗口以启用快捷键

    // 添加面板快捷键
    // Ctrl+1: 显示/隐藏工具链面板
    QAction* toggleToolChainAction = new QAction(tr("工具链面板"), this);
    toggleToolChainAction->setShortcut(Qt::CTRL | Qt::Key_1);
    connect(toggleToolChainAction, &QAction::triggered, this, [this]() {
        toolChainDock_->setVisible(!toolChainDock_->isVisible());
    });
    addAction(toggleToolChainAction);

    // Ctrl+2: 显示/隐藏参数面板
    QAction* toggleParamAction = new QAction(tr("参数面板"), this);
    toggleParamAction->setShortcut(Qt::CTRL | Qt::Key_2);
    connect(toggleParamAction, &QAction::triggered, this, [this]() {
        toolParameterDock_->setVisible(!toolParameterDock_->isVisible());
    });
    addAction(toggleParamAction);

    // Ctrl+3: 显示/隐藏结果面板
    QAction* toggleResultAction = new QAction(tr("结果面板"), this);
    toggleResultAction->setShortcut(Qt::CTRL | Qt::Key_3);
    connect(toggleResultAction, &QAction::triggered, this, [this]() {
        resultTableDock_->setVisible(!resultTableDock_->isVisible());
    });
    addAction(toggleResultAction);

    // Escape: 取消当前ROI绘制
    QAction* cancelAction = new QAction(tr("取消"), this);
    cancelAction->setShortcut(Qt::Key_Escape);
    connect(cancelAction, &QAction::triggered, this, [this]() {
        imageViewer_->setInteractionMode(IMAGEVIEWER_CLASS::SelectMode);
        statusLabel_->setText(tr("就绪"));
    });
    addAction(cancelAction);

    // 用户菜单
    userMenu_ = menuBar()->addMenu(tr("用户(&U)"));

    loginAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), tr("登录(&L)..."), this);
    loginAction_->setStatusTip(tr("用户登录"));
    connect(loginAction_, &QAction::triggered, this, &MainWindow::onLogin);
    userMenu_->addAction(loginAction_);

    logoutAction_ = new QAction(Theme::getIcon(Icons::EXIT), tr("注销(&O)"), this);
    logoutAction_->setStatusTip(tr("注销当前用户"));
    logoutAction_->setEnabled(false);  // 初始状态未登录
    connect(logoutAction_, &QAction::triggered, this, &MainWindow::onLogout);
    userMenu_->addAction(logoutAction_);

    userMenu_->addSeparator();

    userManagementAction_ = new QAction(Theme::getIcon(Icons::APP_SETTINGS), tr("用户管理(&M)..."), this);
    userManagementAction_->setStatusTip(tr("管理系统用户（需要管理员权限）"));
    connect(userManagementAction_, &QAction::triggered, this, &MainWindow::onUserManagement);
    userMenu_->addAction(userManagementAction_);

    // 帮助菜单
    helpMenu_ = menuBar()->addMenu(tr("帮助(&H)"));

    QAction* aboutAction = new QAction(Theme::getIcon(Icons::APP_ABOUT), "关于(&A)", this);
    aboutAction->setStatusTip("关于VisionForge Pro");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "关于VisionForge Pro",
            QString("<h3>VisionForge Pro v%1</h3>"
                   "<p>通用工业视觉检测平台</p>"
                   "<p>Copyright © 2025 VisionForge Team</p>"
                   "<p>基于Qt %2和OpenCV %3开发</p>")
            .arg(VISIONFORGE_VERSION)
            .arg(QT_VERSION_STR)
            .arg(CV_VERSION));
    });
    helpMenu_->addAction(aboutAction);
}

void MainWindow::createToolBars()
{
    // ========== 主工具栏（精简：文件+处理合并）==========
    fileToolBar_ = addToolBar("主工具栏");
    fileToolBar_->setObjectName("mainToolBar");
    fileToolBar_->addAction(openImageFolderAction_);  // 打开文件夹
    fileToolBar_->addSeparator();
    fileToolBar_->addAction(runSingleAction_);        // 运行单张
    fileToolBar_->addAction(runAllAction_);           // 运行所有
    fileToolBar_->setToolButtonStyle(Qt::ToolButtonIconOnly);

    // ========== 视图工具栏（只保留适应窗口）==========
    viewToolBar_ = addToolBar("视图");
    viewToolBar_->setObjectName("viewToolBar");
    viewToolBar_->addAction(fitToWindowAction_);
    viewToolBar_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // ========== 相机工具栏（默认隐藏）==========
    cameraToolBar_ = addToolBar("相机");
    cameraToolBar_->setObjectName("cameraToolBar");
    cameraToolBar_->addAction(frameValidAction_);
    cameraToolBar_->addAction(liveDisplayAction_);
    cameraToolBar_->setVisible(false);  // 默认隐藏
    cameraToolBar_->setToolButtonStyle(Qt::ToolButtonIconOnly);

    // 处理工具栏指针指向主工具栏（兼容性）
    processToolBar_ = fileToolBar_;
}

void MainWindow::createDockWindows()
{
    // 工具链面板（左上）
    toolChainDock_ = new QDockWidget("工具链", this);
    toolChainDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toolChainDock_->setMinimumWidth(200);
    toolChainDock_->setMaximumWidth(350);

    toolChainPanel_ = new ToolChainPanel(this);
    toolChainDock_->setWidget(toolChainPanel_);

    addDockWidget(Qt::LeftDockWidgetArea, toolChainDock_);

    // 工具参数面板（右上）
    toolParameterDock_ = new QDockWidget("工具参数", this);
    toolParameterDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toolParameterDock_->setMinimumWidth(200);
    toolParameterDock_->setMaximumWidth(350);

    toolParameterPanel_ = new ToolParameterPanel(this);
    toolParameterDock_->setWidget(toolParameterPanel_);

    // 设置相机指针（用于工具对话框中的采集功能）
    if (engine_.camera()) {
        toolParameterPanel_->setCamera(engine_.camera());
    }

    addDockWidget(Qt::RightDockWidgetArea, toolParameterDock_);

    // 结果数据面板（右下）
    resultTableDock_ = new QDockWidget("结果数据", this);
    resultTableDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    resultTableDock_->setMinimumWidth(200);
    resultTableDock_->setMaximumWidth(350);
    resultTableDock_->setMinimumHeight(150);  // 限制最小高度
    resultTableDock_->setMaximumHeight(300);  // 限制最大高度

    resultTablePanel_ = new ResultTablePanel(this);
    resultTableDock_->setWidget(resultTablePanel_);

    addDockWidget(Qt::RightDockWidgetArea, resultTableDock_);
    splitDockWidget(toolParameterDock_, resultTableDock_, Qt::Vertical);

    // 历史记录面板（左下）- 默认隐藏以节省空间
    historyDock_ = new QDockWidget("历史记录", this);
    historyDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    historyDock_->setMinimumWidth(200);
    historyDock_->setMaximumWidth(350);

    historyPanel_ = new HistoryPanel(this);
    historyDock_->setWidget(historyPanel_);

    addDockWidget(Qt::LeftDockWidgetArea, historyDock_);
    splitDockWidget(toolChainDock_, historyDock_, Qt::Vertical);

    // 方案管理面板（右侧）
    recipeDock_ = new QDockWidget("方案管理", this);
    recipeDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    recipeDock_->setMinimumWidth(250);
    recipeDock_->setMaximumWidth(400);

    recipeManagerWidget_ = new RecipeManagerWidget(this);
    recipeDock_->setWidget(recipeManagerWidget_);

    addDockWidget(Qt::RightDockWidgetArea, recipeDock_);

    // 统计面板（底部）
    statisticsDock_ = new QDockWidget(tr("运行统计"), this);
    statisticsDock_->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    statisticsDock_->setMinimumHeight(200);
    statisticsDock_->setMaximumHeight(400);

    statisticsPanel_ = new StatisticsPanel(this);
    statisticsDock_->setWidget(statisticsPanel_);

    addDockWidget(Qt::BottomDockWidgetArea, statisticsDock_);

    // 默认隐藏所有侧边面板，最大化Halcon窗口显示空间
    toolChainDock_->hide();
    toolParameterDock_->hide();
    resultTableDock_->hide();
    historyDock_->hide();
    recipeDock_->hide();
    statisticsDock_->hide();

    // 设置停靠窗口的初始大小（当显示时使用）
    resizeDocks({toolChainDock_}, {250}, Qt::Horizontal);
    resizeDocks({toolParameterDock_}, {250}, Qt::Horizontal);
    resizeDocks({resultTableDock_}, {200}, Qt::Vertical);
}

void MainWindow::createStatusBar()
{
    statusLabel_ = new QLabel(tr("就绪"), this);
    statusBar()->addWidget(statusLabel_, 1);

    imageInfoLabel_ = new QLabel(tr("图像: --"), this);
    statusBar()->addPermanentWidget(imageInfoLabel_);

    scaleLabel_ = new QLabel(tr("缩放: 100%"), this);
    statusBar()->addPermanentWidget(scaleLabel_);

    positionLabel_ = new QLabel(tr("位置: --"), this);
    statusBar()->addPermanentWidget(positionLabel_);

    // 分隔符
    QLabel* separator1 = new QLabel(" | ", this);
    statusBar()->addPermanentWidget(separator1);

    // 用户信息
    userLabel_ = new QLabel(tr("用户: 访客"), this);
    userLabel_->setToolTip(tr("当前登录用户"));
    statusBar()->addPermanentWidget(userLabel_);

    // 分隔符
    QLabel* separator2 = new QLabel(" | ", this);
    statusBar()->addPermanentWidget(separator2);

    // CPU使用率
    cpuLabel_ = new QLabel(tr("CPU: --%"), this);
    cpuLabel_->setToolTip(tr("CPU使用率"));
    cpuLabel_->setMinimumWidth(80);
    statusBar()->addPermanentWidget(cpuLabel_);

    // 内存使用率
    memoryLabel_ = new QLabel(tr("内存: --%"), this);
    memoryLabel_->setToolTip(tr("内存使用率"));
    memoryLabel_->setMinimumWidth(80);
    statusBar()->addPermanentWidget(memoryLabel_);

    // 启动系统监控定时更新
    QTimer* monitorTimer = new QTimer(this);
    connect(monitorTimer, &QTimer::timeout, this, [this]() {
        Base::SystemMonitor& monitor = Base::SystemMonitor::instance();

        // 更新CPU和内存显示
        double cpu = monitor.cpuUsage();
        double mem = monitor.memoryUsage();
        cpuLabel_->setText(QString("CPU: %1%").arg(static_cast<int>(cpu)));
        memoryLabel_->setText(QString(tr("内存: %1%")).arg(static_cast<int>(mem)));

        // 根据使用率设置颜色
        if (cpu > 80) {
            cpuLabel_->setStyleSheet("color: red;");
        } else if (cpu > 60) {
            cpuLabel_->setStyleSheet("color: orange;");
        } else {
            cpuLabel_->setStyleSheet("");
        }

        if (mem > 80) {
            memoryLabel_->setStyleSheet("color: red;");
        } else if (mem > 60) {
            memoryLabel_->setStyleSheet("color: orange;");
        } else {
            memoryLabel_->setStyleSheet("");
        }
    });
    monitorTimer->start(2000);  // 每2秒更新一次
}

void MainWindow::addDockWidgetsToViewMenu()
{
    // 在视图菜单中添加分隔符和面板显示选项
    viewMenu_->addSeparator();

    // 添加停靠窗口的显示/隐藏切换选项
    viewMenu_->addAction(toolChainDock_->toggleViewAction());
    viewMenu_->addAction(toolParameterDock_->toggleViewAction());
    viewMenu_->addAction(resultTableDock_->toggleViewAction());
    viewMenu_->addAction(historyDock_->toggleViewAction());
    viewMenu_->addAction(recipeDock_->toggleViewAction());
    viewMenu_->addAction(statisticsDock_->toggleViewAction());
}

void MainWindow::connectSignals()
{
    // 工具链信号
    connect(toolChainPanel_, &ToolChainPanel::toolSelectionChanged,
            this, &MainWindow::onToolSelectionChanged);
    connect(toolChainPanel_, &ToolChainPanel::toolDoubleClicked,
            this, &MainWindow::onToolDoubleClicked);
    connect(toolChainPanel_, &ToolChainPanel::addToolRequested,
            this, &MainWindow::onAddTool);
    connect(toolChainPanel_, &ToolChainPanel::removeToolRequested,
            [this](Algorithm::VisionTool* tool) {
                toolChainPanel_->removeTool(tool);
                delete tool;
            });

    // 工具参数信号
    connect(toolParameterPanel_, &ToolParameterPanel::parameterChanged,
            this, &MainWindow::onParameterChanged);

    // 图像查看器信号
    connect(imageViewer_, &IMAGEVIEWER_CLASS::scaleChanged,
            this, &MainWindow::onScaleChanged);
    connect(imageViewer_, &IMAGEVIEWER_CLASS::mousePositionChanged,
            this, &MainWindow::onMousePositionChanged);

    // ROI信号
    connect(imageViewer_, &IMAGEVIEWER_CLASS::roiCreated,
            this, &MainWindow::onROICreated);
    connect(imageViewer_, &IMAGEVIEWER_CLASS::roiSelectionChanged,
            this, &MainWindow::onROISelectionChanged);

    // 历史记录信号
    connect(historyPanel_, &HistoryPanel::recordSelected,
            this, &MainWindow::onHistoryRecordSelected);

    // 方案管理信号
    connect(recipeManagerWidget_, &RecipeManagerWidget::recipeActivated,
            this, &MainWindow::onRecipeActivated);
    connect(recipeManagerWidget_, &RecipeManagerWidget::editRecipeRequested,
            this, &MainWindow::onEditRecipeRequested);
}

void MainWindow::updateActions()
{
    bool hasImage = (engine_.currentImage() != nullptr);
    bool hasTool = (toolChainPanel_ && toolChainPanel_->getCurrentTool() != nullptr);

    if (saveImageAction_) saveImageAction_->setEnabled(hasImage);
    if (runSingleAction_) runSingleAction_->setEnabled(hasImage && hasTool);
    if (runAllAction_) runAllAction_->setEnabled(hasImage);
    if (removeToolAction_) removeToolAction_->setEnabled(hasTool);

    // 帧有效和实时显示始终可用
    if (frameValidAction_) frameValidAction_->setEnabled(true);
    if (liveDisplayAction_) liveDisplayAction_->setEnabled(true);
}

void MainWindow::updateStatusBar()
{
    if (!imageInfoLabel_) return;

    if (engine_.currentImage()) {
        imageInfoLabel_->setText(QString("图像: %1x%2, %3通道")
            .arg(engine_.currentImage()->width())
            .arg(engine_.currentImage()->height())
            .arg(engine_.currentImage()->channels()));
    } else {
        imageInfoLabel_->setText("图像: --");
    }
}

void MainWindow::processImage(Base::ImageData::Ptr image)
{
    if (!image) {
        return;
    }

    // 安全检查：确保所有面板已初始化
    if (!toolChainPanel_ || !resultTablePanel_ || !historyPanel_ || !imageViewer_) {
        LOG_ERROR("processImage: 面板未初始化");
        return;
    }

    Base::ImageData::Ptr result = image;
    QList<Algorithm::VisionTool*> tools = toolChainPanel_->getTools();

    // 清除之前的结果
    resultTablePanel_->clearResults();

    double totalExecutionTime = 0.0;
    QString toolChainDesc;
    QVariantMap lastDisplayObjects;  // 保存最后一个工具的displayObjects

    for (Algorithm::VisionTool* tool : tools) {
        if (!tool) {
            LOG_WARNING("processImage: 工具列表中存在空指针");
            continue;
        }
        if (!tool->isEnabled()) {
            continue;
        }

        // 记录开始时间
        auto startTime = std::chrono::high_resolution_clock::now();

        Algorithm::ToolResult toolResult;
        bool success = tool->process(result, toolResult);

        // 计算执行时间
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = endTime - startTime;
        toolResult.executionTime = duration.count();
        totalExecutionTime += toolResult.executionTime;

        // 更新工具链面板中的执行时间
        toolChainPanel_->updateToolExecutionTime(tool, toolResult.executionTime);

        // 添加结果到结果面板
        resultTablePanel_->addResult(tool->displayName(), toolResult);

        // 构建工具链描述
        if (!toolChainDesc.isEmpty()) {
            toolChainDesc += " → ";
        }
        toolChainDesc += tool->displayName();

        if (!success) {
            statusLabel_->setText(QString("工具 \"%1\" 处理失败: %2")
                .arg(tool->displayName()).arg(toolResult.errorMessage));
            LOG_WARNING(QString("工具处理失败: %1 - %2")
                .arg(tool->displayName()).arg(toolResult.errorMessage));
            return;
        }

        if (toolResult.outputImage) {
            result = toolResult.outputImage;
        }

        // 保存displayObjects（如果有）
        if (!toolResult.displayObjects.isEmpty()) {
            lastDisplayObjects = toolResult.displayObjects;
        }
    }

    // 更新当前图像
    engine_.setCurrentImage(result);

    // 根据当前模式显示图像
    if (isMultiViewMode_ && multiCameraView_ && multiCameraView_->isVisible()) {
        // 多视图模式：只更新当前选中的视图位置（每个位置有独立的工具链）
        int selectedView = multiCameraView_->selectedView();
        if (selectedView >= 0) {
            multiCameraView_->updateImage(selectedView, engine_.currentImage());
        }

        // 同时更新隐藏的单图像查看器（用于其他功能）
        imageViewer_->setImage(engine_.currentImage());
    } else {
        // 单视图模式
        imageViewer_->setImage(engine_.currentImage());
    }

    // 处理displayObjects - XLD轮廓显示
#ifdef USE_HALCON
    if (!lastDisplayObjects.isEmpty() &&
        lastDisplayObjects.contains("match_contours")) {
        QVariant objVariant = lastDisplayObjects["match_contours"];
        if (objVariant.canConvert<Algorithm::HalconObjectPtr>()) {
            Algorithm::HalconObjectPtr objPtr =
                objVariant.value<Algorithm::HalconObjectPtr>();

            if (objPtr && objPtr->type() == Algorithm::HalconObjectWrapper::XLD_Contour) {
                Algorithm::XLDContourPtr xldPtr =
                    qSharedPointerCast<Algorithm::XLDContourWrapper>(objPtr);

                QList<HXLDCont> contours;
                contours.append(xldPtr->contours());

                // 在多视图模式下也需要更新对应视图的XLD
                if (isMultiViewMode_ && multiCameraView_ && multiCameraView_->isVisible()) {
                    HalconImageViewer* viewer = multiCameraView_->getSelectedViewer();
                    if (viewer) {
                        viewer->setXLDContours(contours);
                    }
                }
                imageViewer_->setXLDContours(contours);
            }
        }
    } else {
        // 清除之前的XLD显示
        if (isMultiViewMode_ && multiCameraView_ && multiCameraView_->isVisible()) {
            HalconImageViewer* viewer = multiCameraView_->getSelectedViewer();
            if (viewer) {
                viewer->clearXLDContours();
            }
        }
        imageViewer_->clearXLDContours();
    }
#endif

    // 添加到历史记录
    QString description = toolChainDesc.isEmpty() ? "原始图像" : QString("处理结果 (%1)").arg(toolChainDesc);
    historyPanel_->addRecord(engine_.currentImage(), description, toolChainDesc, totalExecutionTime);

    // 更新状态栏
    statusLabel_->setText(QString("处理完成 - 总耗时: %1 ms").arg(totalExecutionTime, 0, 'f', 2));
    updateStatusBar();

    LOG_INFO(QString("图像处理完成 - 工具链: %1, 总耗时: %2 ms")
        .arg(toolChainDesc)
        .arg(totalExecutionTime, 0, 'f', 2));
}

void MainWindow::displayImage(Base::ImageData::Ptr image)
{
    if (!image) {
        return;
    }

    // 更新当前图像
    engine_.setCurrentImage(image);

    // 根据当前模式显示图像
    if (isMultiViewMode_ && multiCameraView_ && multiCameraView_->isVisible()) {
        // 多视图模式：更新所有视图位置
        int viewCount = multiCameraView_->viewerCount();
        for (int i = 0; i < viewCount; ++i) {
            multiCameraView_->updateImage(i, engine_.currentImage());
        }
        // 同时更新隐藏的单图像查看器
        if (imageViewer_) {
            imageViewer_->setImage(engine_.currentImage());
        }
    } else {
        // 单视图模式
        if (imageViewer_) {
            imageViewer_->setImage(engine_.currentImage());
        }
    }

    // 更新状态栏显示图像信息
    cv::Mat mat = image->mat();
    if (!mat.empty()) {
        statusLabel_->setText(QString("图像: %1x%2, %3通道")
            .arg(mat.cols).arg(mat.rows).arg(mat.channels()));
    }
}

void MainWindow::showAddToolDialog()
{
    using namespace Algorithm;

    ToolFactory& factory = ToolFactory::instance();
    QList<ToolFactory::ToolInfo> allTools = factory.getAllTools();

    if (allTools.isEmpty()) {
        QMessageBox::information(this, "提示", "没有可用的工具");
        return;
    }

    QStringList toolNames;
    for (const auto& info : allTools) {
        toolNames.append(info.name);
    }

    bool ok;
    QString selectedName = QInputDialog::getItem(this, "添加工具",
        "选择要添加的工具:", toolNames, 0, false, &ok);

    if (ok && !selectedName.isEmpty()) {
        VisionTool* tool = factory.createTool(selectedName);
        if (tool) {
            toolChainPanel_->addTool(tool);

            // 同步到PositionToolChainManager (多工位模式)
#ifdef USE_HALCON
            if (isMultiViewMode_) {
                auto& toolChainManager = Core::PositionToolChainManager::instance();
                toolChainManager.addTool(tool);
            }
#endif

            statusLabel_->setText(QString("已添加工具: %1").arg(selectedName));
            updateActions();
        } else {
            QMessageBox::warning(this, "错误", "创建工具失败");
        }
    }
}

// ========== ROI操作槽函数 ==========

void MainWindow::onROICreated(ROIShapePtr roi)
{
    Q_UNUSED(roi);
    statusLabel_->setText("已创建ROI");
}

void MainWindow::onROISelectionChanged(ROIShapePtr roi)
{
    if (roi) {
        statusLabel_->setText("已选择ROI");
    } else {
        statusLabel_->setText("");
    }
}

void MainWindow::onDrawRectangle()
{
    imageViewer_->setInteractionMode(IMAGEVIEWER_CLASS::DrawRectangle);
    statusLabel_->setText("绘制矩形ROI - 拖动鼠标绘制");
}

void MainWindow::onDrawCircle()
{
    imageViewer_->setInteractionMode(IMAGEVIEWER_CLASS::DrawCircle);
    statusLabel_->setText("绘制圆形ROI - 拖动鼠标绘制");
}

void MainWindow::onDrawEllipse()
{
    imageViewer_->setInteractionMode(IMAGEVIEWER_CLASS::DrawEllipse);
    statusLabel_->setText("绘制椭圆ROI - 拖动鼠标绘制");
}

void MainWindow::onDrawPolygon()
{
    imageViewer_->setInteractionMode(IMAGEVIEWER_CLASS::DrawPolygon);
    statusLabel_->setText("绘制多边形ROI - 单击添加点，右键完成");
}

void MainWindow::onDrawLine()
{
    imageViewer_->setInteractionMode(IMAGEVIEWER_CLASS::DrawLine);
    statusLabel_->setText("绘制直线ROI - 拖动鼠标绘制");
}

void MainWindow::onSelectROI()
{
    imageViewer_->setInteractionMode(IMAGEVIEWER_CLASS::SelectMode);
    statusLabel_->setText("选择ROI - 点击ROI选中");
}

void MainWindow::onClearROIs()
{
    imageViewer_->clearROIs();
    statusLabel_->setText("已清除所有ROI");
}

// ========== 历史记录槽函数 ==========

void MainWindow::onHistoryRecordSelected(const HistoryRecord& record)
{
    if (record.image) {
        engine_.setCurrentImage(record.image);
        imageViewer_->setImage(engine_.currentImage());
        statusLabel_->setText(QString("已加载历史记录: %1").arg(record.description));
        updateStatusBar();
    }
}

// ========== PLC通信槽函数 ==========

void MainWindow::onPLCConfig()
{
    PLCConfigDialog dialog(this);
    dialog.exec();
}

// ========== 相机配置槽函数 ==========

void MainWindow::onCameraConfig()
{
    // 保存当前连续采集状态并暂停
    bool wasGrabbing = engine_.isLiveGrabbing();
    if (engine_.isLiveGrabbing()) {
        engine_.stopLiveGrab();
        // 停止相机采集以释放资源给配置对话框
        if (engine_.camera() && engine_.camera()->isGrabbing()) {
            engine_.camera()->stopGrabbing();
        }
    }

    CameraConfigDialog dialog(this);

    // 如果有当前相机，传递给对话框
    if (engine_.camera()) {
        dialog.setCamera(engine_.camera());
    }

    if (dialog.exec() == QDialog::Accepted) {
        // 获取选择的相机
        HAL::ICamera* newCamera = dialog.takeCamera();
        if (newCamera && newCamera != engine_.camera()) {
            // 使用VisionEngine设置新相机（自动关闭旧相机并转移所有权）
            engine_.setCamera(std::unique_ptr<HAL::ICamera>(newCamera));
            LOG_INFO(QString("相机已切换: %1").arg(engine_.camera()->deviceName()));

            // 更新工具参数面板的相机指针
            if (toolParameterPanel_) {
                toolParameterPanel_->setCamera(engine_.camera());
            }
        }

        // 保存相机显示设置到配置文件
        if (engine_.camera() && engine_.camera()->isOpen()) {
            HAL::ICamera::Config cameraConfig = engine_.camera()->getConfig();
            Base::ConfigManager& config = Base::ConfigManager::instance();
            config.setValue("Camera/RotationAngle", cameraConfig.rotationAngle);
            config.setValue("Camera/FlipHorizontal", cameraConfig.flipHorizontal);
            config.setValue("Camera/FlipVertical", cameraConfig.flipVertical);
            LOG_INFO(QString("保存相机显示设置: 旋转=%1°, 水平镜像=%2, 垂直镜像=%3")
                .arg(cameraConfig.rotationAngle)
                .arg(cameraConfig.flipHorizontal ? "是" : "否")
                .arg(cameraConfig.flipVertical ? "是" : "否"));
        }

        updateActions();
        LOG_INFO("相机配置已更新");
    }

    // 恢复连续采集状态
    if (wasGrabbing && engine_.camera() && engine_.camera()->isOpen() && isLiveDisplay_) {
        if (engine_.camera()->startGrabbing()) {
            engine_.startLiveGrab(150);
        }
    }
}

// ========== 方案管理 ==========

void MainWindow::onRecipeActivated(Core::Recipe* recipe)
{
    if (!recipe) {
        return;
    }

    LOG_INFO(QString("应用方案: %1").arg(recipe->name()));

    // 将方案的工具链配置应用到当前工具链
    // 这里简化处理：清除现有工具链并从方案中加载
    // 实际应用中可能需要更复杂的合并逻辑

    // 显示确认对话框
    int ret = QMessageBox::question(this, "应用方案",
        QString("确定要将方案 \"%1\" 应用到当前工具链吗？\n这将替换现有的工具链配置。").arg(recipe->name()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret != QMessageBox::Yes) {
        return;
    }

    // 清除现有工具
    QList<Algorithm::VisionTool*> tools = toolChainPanel_->getTools();
    for (Algorithm::VisionTool* tool : tools) {
        toolChainPanel_->removeTool(tool);
        delete tool;
    }

    // 从方案加载工具链
    if (recipe->toolChain()) {
        Core::ToolChain* srcChain = recipe->toolChain();
        for (int i = 0; i < srcChain->toolCount(); ++i) {
            const Core::ToolChainNode* node = srcChain->getNode(i);
            if (node && node->tool) {
                // 创建工具的副本
                Algorithm::VisionTool* newTool = Algorithm::ToolFactory::instance().createTool(node->tool->toolType());
                if (newTool) {
                    // 复制参数
                    QJsonObject params = node->tool->serializeParams();
                    newTool->deserializeParams(params);
                    newTool->setEnabled(node->enabled);
                    if (!node->name.isEmpty()) {
                        newTool->setDisplayName(node->name);
                    }

                    toolChainPanel_->addTool(newTool);
                }
            }
        }
    }

    statusLabel_->setText(QString("已应用方案: %1").arg(recipe->name()));
}

void MainWindow::onEditRecipeRequested(Core::Recipe* recipe)
{
    if (!recipe) {
        return;
    }

    RecipeEditorDialog dialog(recipe, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 保存方案
        Core::RecipeManager::instance().saveRecipe(recipe);
        recipeManagerWidget_->refreshRecipeList();
        LOG_INFO(QString("方案已编辑: %1").arg(recipe->name()));
    }
}

// ========== 标定 ==========

void MainWindow::onCameraCalibration()
{
    CameraCalibDialog dialog(this);

    // 设置当前图像
    if (engine_.currentImage()) {
        dialog.setCurrentImage(engine_.currentImage());
    }

    // 连接标定完成信号
    connect(&dialog, &CameraCalibDialog::calibrationCompleted,
            this, [this](const Algorithm::CalibrationResult& result) {
        // 保存标定结果到CalibrationManager
        Algorithm::CalibrationManager::instance().setCameraCalibResult(result);
        LOG_INFO("相机标定结果已保存到CalibrationManager");
        statusLabel_->setText("相机标定完成");
    });

    dialog.exec();
}

void MainWindow::onNinePointCalibration()
{
    NinePointCalibDialog dialog(this);

    // 设置当前图像
    if (engine_.currentImage()) {
        dialog.setCurrentImage(engine_.currentImage());
    }

    // 连接标定完成信号
    connect(&dialog, &NinePointCalibDialog::calibrationCompleted,
            this, [this](const Algorithm::CalibrationResult& result) {
        // 保存标定结果到CalibrationManager
        Algorithm::CalibrationManager::instance().setNinePointCalibResult(result);
        LOG_INFO("九点标定结果已保存到CalibrationManager");
        statusLabel_->setText("九点标定完成");
    });

    dialog.exec();
}

void MainWindow::onQRCalibration()
{
    QRCalibDialog dialog(this);

    // 设置当前图像
    if (engine_.currentImage()) {
        dialog.setCurrentImage(engine_.currentImage());
    }

    // 连接标定完成信号
    connect(&dialog, &QRCalibDialog::calibrationCompleted,
            this, [this](const Algorithm::CalibrationResult& result) {
        // 保存标定结果到CalibrationManager
        Algorithm::CalibrationManager::instance().setNinePointCalibResult(result);
        LOG_INFO("QR码标定结果已保存到CalibrationManager");
        statusLabel_->setText("QR码标定完成");
    });

    dialog.exec();
}

// ========== 多相机多位置对位 ==========

void MainWindow::onMultiCameraManager()
{
    MultiCameraManagerDialog dialog(this);
    dialog.exec();
}

void MainWindow::onMultiPointAlignment()
{
    MultiPointAlignmentDialog dialog(this);

    // TODO: 设置对位工具实例
    // dialog.setAlignmentTool(alignmentTool_);

    dialog.exec();
}

void MainWindow::onAlignmentOutput()
{
    AlignmentOutputDialog dialog(this);

    // TODO: 设置输出工具实例
    // dialog.setOutputTool(outputTool_);

    dialog.exec();
}

void MainWindow::onWorkStationConfig()
{
    WorkStationDialog dialog(this);

    // 获取或创建当前工位
    auto& wsManager = Core::WorkStationManager::instance();
    if (wsManager.workStationCount() == 0) {
        wsManager.createWorkStation("默认工位");
    }

    Core::WorkStation* ws = wsManager.currentWorkStation();
    if (ws) {
        dialog.setWorkStation(ws);
    }

    dialog.exec();
}

// ========== 系统设置 ==========

void MainWindow::onSystemSettings()
{
    SystemSettingsDialog dialog(this);
    dialog.exec();
}

// ========== 用户登录/注销 ==========

void MainWindow::onLogin()
{
    LoginDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // 登录成功，更新用户状态
        updateUserStatus();
        const Base::UserInfo* user = Base::PermissionManager::instance().currentUser();
        if (user) {
            LOG_INFO(QString("用户登录: %1").arg(user->username));
        }
    }
}

void MainWindow::onLogout()
{
    int ret = QMessageBox::question(this, tr("确认注销"),
        tr("确定要注销当前用户吗？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        const Base::UserInfo* user = Base::PermissionManager::instance().currentUser();
        QString username = user ? user->username : tr("未知");
        Base::PermissionManager::instance().logout();
        updateUserStatus();
        statusLabel_->setText(tr("用户已注销"));
        LOG_INFO(QString("用户注销: %1").arg(username));
    }
}

void MainWindow::onUserManagement()
{
    // 检查权限
    if (!Base::PermissionManager::instance().hasPermission(Base::Permission::UserManagement)) {
        QMessageBox::warning(this, tr("权限不足"),
            tr("您没有用户管理权限。\n需要管理员权限才能管理用户。"));
        return;
    }

    // 打开用户管理对话框
    UserManagementDialog dialog(this);
    dialog.exec();

    // 刷新当前用户状态（可能被修改）
    updateUserStatus();
}

void MainWindow::updateUserStatus()
{
    Base::PermissionManager& pm = Base::PermissionManager::instance();

    if (pm.isLoggedIn()) {
        const Base::UserInfo* user = pm.currentUser();
        if (user) {
            QString roleStr;
            switch (user->role) {
                case Base::UserRole::Guest:
                    roleStr = tr("访客");
                    break;
                case Base::UserRole::Operator:
                    roleStr = tr("操作员");
                    break;
                case Base::UserRole::Engineer:
                    roleStr = tr("工程师");
                    break;
                case Base::UserRole::Administrator:
                    roleStr = tr("管理员");
                    break;
            }
            userLabel_->setText(QString("%1 (%2)").arg(user->displayName).arg(roleStr));
            userLabel_->setToolTip(tr("用户名: %1\n角色: %2").arg(user->username).arg(roleStr));
        }
        loginAction_->setEnabled(false);
        logoutAction_->setEnabled(true);
    } else {
        userLabel_->setText(tr("用户: 访客"));
        userLabel_->setToolTip(tr("未登录"));

        loginAction_->setEnabled(true);
        logoutAction_->setEnabled(false);
    }
}

// ========== UI模式 ==========

void MainWindow::onUIModeChanged(UIMode mode)
{
    // 根据模式更新界面组件可见性
    UIModeManager& mgr = UIModeManager::instance();

    // 更新菜单可见性
    calibMenu_->menuAction()->setVisible(mgr.isComponentVisible(UIComponent::MenuCalibration));
    settingsMenu_->menuAction()->setVisible(mgr.isComponentVisible(UIComponent::MenuSettings));

    // 更新停靠面板可见性（仅当面板存在时）
    if (historyDock_) {
        // 在简单模式下隐藏历史面板
        if (!mgr.isComponentVisible(UIComponent::PanelHistory)) {
            historyDock_->hide();
        }
    }

    if (statisticsDock_) {
        // 在简单模式下隐藏统计面板
        if (!mgr.isComponentVisible(UIComponent::PanelStatistics)) {
            statisticsDock_->hide();
        }
    }

    if (recipeDock_) {
        // 在简单模式下隐藏方案面板
        if (!mgr.isComponentVisible(UIComponent::PanelRecipe)) {
            recipeDock_->hide();
        }
    }

    // 更新状态栏显示
    QString modeStr = UIModeManager::modeName(mode);
    statusLabel_->setText(tr("已切换到%1").arg(modeStr));

    // 记录日志
    LOG_INFO(QString("UI模式已切换: %1").arg(modeStr));
}

// ========== 相机自动连接 ==========

bool MainWindow::tryAutoConnectCamera()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();

    // 读取保存的相机配置
    int cameraType = config.getValue("Camera/Type", -1).toInt();
    QString serialNumber = config.getValue("Camera/SerialNumber", "").toString();
    QString ipAddress = config.getValue("Camera/IPAddress", "").toString();
    QString modelName = config.getValue("Camera/ModelName", "").toString();

    // 检查是否有有效的配置
    if (cameraType < 0 || (serialNumber.isEmpty() && ipAddress.isEmpty())) {
        LOG_DEBUG("没有找到保存的相机配置，跳过自动连接");
        return false;
    }

    LOG_INFO(QString("尝试自动连接相机: 类型=%1, 序列号=%2, IP=%3")
        .arg(cameraType).arg(serialNumber).arg(ipAddress));

    // 枚举可用相机
    QList<HAL::GenericDeviceInfo> cameras = HAL::CameraFactory::enumerateAllDevices();

    if (cameras.isEmpty()) {
        LOG_WARNING("没有找到可用相机，无法自动连接");
        return false;
    }

    // 查找匹配的相机
    HAL::GenericDeviceInfo matchedCamera;
    bool found = false;

    for (const HAL::GenericDeviceInfo& cam : cameras) {
        // 优先匹配序列号（更精确）
        if (!serialNumber.isEmpty() && cam.serialNumber == serialNumber) {
            matchedCamera = cam;
            found = true;
            LOG_INFO(QString("通过序列号匹配到相机: %1").arg(serialNumber));
            break;
        }
        // 其次匹配IP地址
        if (!ipAddress.isEmpty() && cam.ipAddress == ipAddress) {
            matchedCamera = cam;
            found = true;
            LOG_INFO(QString("通过IP地址匹配到相机: %1").arg(ipAddress));
            break;
        }
    }

    if (!found) {
        LOG_WARNING(QString("未找到匹配的相机 (序列号=%1, IP=%2)")
            .arg(serialNumber).arg(ipAddress));
        return false;
    }

    // 创建并连接相机
    HAL::ICamera* newCamera = HAL::CameraFactory::create(
        static_cast<HAL::CameraFactory::CameraType>(matchedCamera.cameraType), this);
    if (!newCamera) {
        LOG_ERROR("创建相机实例失败");
        return false;
    }

    // 根据相机类型选择设备
    bool deviceSelected = false;
#ifdef USE_HIKVISION_MVS
    if (matchedCamera.cameraType == HAL::CameraFactory::Hikvision) {
        HAL::HikvisionCamera* hikCamera = dynamic_cast<HAL::HikvisionCamera*>(newCamera);
        if (hikCamera) {
            // 优先使用序列号选择设备
            if (!serialNumber.isEmpty()) {
                deviceSelected = hikCamera->selectBySerialNumber(serialNumber);
            }
            // 其次使用IP地址
            if (!deviceSelected && !ipAddress.isEmpty()) {
                deviceSelected = hikCamera->selectByIP(ipAddress);
            }
        }
    }
#endif

    // 非海康相机或选择失败，直接尝试打开
    if (!deviceSelected && matchedCamera.cameraType != HAL::CameraFactory::Hikvision) {
        deviceSelected = true;  // 模拟相机等不需要选择设备
    }

    if (!deviceSelected) {
        LOG_ERROR("选择相机设备失败");
        delete newCamera;
        return false;
    }

    // 尝试打开相机
    if (!newCamera->open()) {
        LOG_ERROR("打开相机失败");
        delete newCamera;
        return false;
    }

    // 成功连接，使用VisionEngine设置新相机（自动关闭旧相机并转移所有权）
    engine_.setCamera(std::unique_ptr<HAL::ICamera>(newCamera));

    // 加载并应用显示设置（旋转、镜像）
    HAL::ICamera::Config cameraConfig = engine_.camera()->getConfig();
    cameraConfig.rotationAngle = config.getValue("Camera/RotationAngle", 0).toInt();
    cameraConfig.flipHorizontal = config.getValue("Camera/FlipHorizontal", false).toBool();
    cameraConfig.flipVertical = config.getValue("Camera/FlipVertical", false).toBool();
    engine_.camera()->setConfig(cameraConfig);
    LOG_INFO(QString("加载相机显示设置: 旋转=%1°, 水平镜像=%2, 垂直镜像=%3")
        .arg(cameraConfig.rotationAngle)
        .arg(cameraConfig.flipHorizontal ? "是" : "否")
        .arg(cameraConfig.flipVertical ? "是" : "否"));

    // 更新工具参数面板的相机指针
    if (toolParameterPanel_) {
        toolParameterPanel_->setCamera(engine_.camera());
    }

    // 开始采集（必须在获取图像之前调用）
    if (!engine_.camera()->startGrabbing()) {
        LOG_ERROR("启动相机采集失败");
        statusLabel_->setText("相机采集启动失败");
        return false;
    }

    statusLabel_->setText(QString("已自动连接相机: %1").arg(matchedCamera.modelName));
    updateActions();

    LOG_INFO(QString("相机自动连接成功: %1 (%2)")
        .arg(matchedCamera.modelName).arg(matchedCamera.serialNumber));

    // 如果实时显示已启用，自动开始连续采集
    if (isLiveDisplay_ && liveDisplayAction_->isChecked()) {
        if (!engine_.isLiveGrabbing()) {
            // 等待相机稳定后再开始连续采集
            // 给相机一些时间来采集第一帧
            QTimer::singleShot(500, this, [this]() {
                if (engine_.camera() && engine_.camera()->isOpen() && isLiveDisplay_) {
                    engine_.startLiveGrab(150);  // 10 FPS
                    LOG_INFO("相机连接成功，自动开始实时显示");
                }
            });
        }
    }

    return true;
}

void MainWindow::showCameraConfigOnStartup()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();

    // 检查是否有配置的相机
    int cameraType = config.getValue("Camera/Type", -1).toInt();
    QString serialNumber = config.getValue("Camera/SerialNumber", "").toString();
    QString ipAddress = config.getValue("Camera/IPAddress", "").toString();

    QString reason;
    if (cameraType < 0 || (serialNumber.isEmpty() && ipAddress.isEmpty())) {
        reason = tr("未检测到相机配置，请选择相机设备");
    } else {
        reason = tr("配置的相机未找到或已断开，请重新选择");
    }

    // 弹出消息框提示用户
    QMessageBox::information(this, tr("相机设置"), reason);

    // 打开相机设置对话框（用于选择相机设备）
    CameraSetupDialog setupDialog(this);

    if (setupDialog.exec() == QDialog::Accepted) {
        // 用户保存了配置，尝试重新连接相机
        if (setupDialog.isSaveAndExit()) {
            // 尝试自动连接已保存的相机配置
            if (tryAutoConnectCamera()) {
                if (engine_.camera()) {
                    QString modelName = engine_.camera()->deviceName();
                    statusLabel_->setText(tr("已连接相机: %1").arg(modelName));
                    LOG_INFO(QString("启动时配置相机成功: %1").arg(modelName));

                    // 更新工具参数面板的相机指针
                    if (toolParameterPanel_) {
                        toolParameterPanel_->setCamera(engine_.camera());
                    }

                    // 如果实时显示已启用，开始连续采集
                    if (isLiveDisplay_ && liveDisplayAction_ && liveDisplayAction_->isChecked()) {
                        if (!engine_.isLiveGrabbing()) {
                            QTimer::singleShot(500, this, [this]() {
                                if (engine_.camera() && engine_.camera()->isOpen() && isLiveDisplay_) {
                                    engine_.startLiveGrab(150);
                                    LOG_INFO("启动时配置相机后，自动开始实时显示");
                                }
                            });
                        }
                    }
                }
            } else {
                // 连接失败，继续使用模拟相机
                statusLabel_->setText(tr("相机连接失败，使用模拟相机"));
                LOG_WARNING("启动时相机配置失败，继续使用模拟相机");
            }
        }
    } else if (setupDialog.isSkipped()) {
        // 用户跳过了配置，继续使用模拟相机
        statusLabel_->setText(tr("使用模拟相机"));
        LOG_INFO("用户跳过相机配置，使用模拟相机");
    } else {
        // 用户取消了配置，继续使用模拟相机
        statusLabel_->setText(tr("使用模拟相机"));
        LOG_INFO("用户取消相机配置，使用模拟相机");
    }

    updateActions();
}

// ========== 多工位多相机 ==========

void MainWindow::onStationSelected(int index)
{
#ifdef USE_HALCON
    auto& manager = Core::MultiStationManager::instance();
    if (manager.setCurrentStationByIndex(index)) {
        auto* station = manager.currentStation();
        if (station && multiCameraView_) {
            multiCameraView_->setStation(station);
        }

        // 刷新场景切换栏
        if (sceneSwitchBar_ && station) {
            sceneSwitchBar_->refreshScenes(station);
        }

        statusLabel_->setText(QString("切换到工位: %1").arg(station ? station->name : ""));
        LOG_INFO(QString("切换到工位索引 %1").arg(index));
    }
#else
    Q_UNUSED(index);
#endif
}

void MainWindow::onShowAllStations()
{
#ifdef USE_HALCON
    statusLabel_->setText("显示所有工位");
    LOG_INFO("显示所有工位");
#endif
}

void MainWindow::onManualDebugCenter()
{
#ifdef USE_HALCON
    auto& manager = Core::MultiStationManager::instance();
    auto* station = manager.currentStation();

    ManualDebugDialog dialog(this);
    if (station) {
        dialog.setStation(station);
    }
    dialog.exec();
#endif
}

void MainWindow::onCalibrationWizard()
{
#ifdef USE_HALCON
    auto& manager = Core::MultiStationManager::instance();
    auto* station = manager.currentStation();

    if (!station) {
        QMessageBox::warning(this, tr("警告"), tr("请先配置工位"));
        return;
    }

    CalibrationWizard wizard(station, this);
    wizard.exec();
#endif
}

void MainWindow::onToggleMultiCameraView()
{
#ifdef USE_HALCON
    isMultiViewMode_ = !isMultiViewMode_;

    if (isMultiViewMode_) {
        // 切换到多相机视图（使用堆叠容器）
        centralStack_->setCurrentWidget(multiCameraView_);

        // 显示工位切换栏
        stationSwitchBar_->setVisible(true);
        stationSwitchBar_->refreshStations();

        // 加载当前工位配置
        auto& manager = Core::MultiStationManager::instance();
        auto* station = manager.currentStation();
        if (station) {
            multiCameraView_->setStation(station);
        }

        statusLabel_->setText("多相机视图模式");
    } else {
        // 切换回单相机视图（使用堆叠容器）
        centralStack_->setCurrentWidget(imageViewer_);

        // 隐藏工位切换栏
        stationSwitchBar_->setVisible(false);

        statusLabel_->setText("单相机视图模式");
    }

    toggleMultiViewAction_->setChecked(isMultiViewMode_);
#endif
}

void MainWindow::onMultiViewSelected(int index, const QString& positionId)
{
#ifdef USE_HALCON
    Q_UNUSED(index);

    auto& stationManager = Core::MultiStationManager::instance();
    auto& toolChainManager = Core::PositionToolChainManager::instance();

    QString stationId = stationManager.currentStationId();

    // 先保存当前工具链面板的工具到PositionToolChainManager
    // 避免切换位置时丢失当前位置的工具
    if (toolChainPanel_) {
        QList<Algorithm::VisionTool*> currentPanelTools = toolChainPanel_->getTools();
        toolChainManager.setCurrentTools(currentPanelTools);

        // 保存到文件（持久化）
        toolChainManager.saveCurrentToolChain();
    }

    // 切换到选中位置的工具链
    if (toolChainManager.setCurrentPosition(stationId, positionId)) {
        // 获取该位置的工具列表
        auto tools = toolChainManager.currentTools();

        // 更新工具链面板
        toolChainPanel_->clear();
        for (auto* tool : tools) {
            toolChainPanel_->addTool(tool);
        }

        // 获取位置名称
        auto* station = stationManager.currentStation();
        QString positionName = positionId;
        if (station) {
            auto* binding = station->getPositionBinding(positionId);
            if (binding) {
                positionName = binding->positionName;
            }
        }

        statusLabel_->setText(QString("当前位置: %1 (%2)").arg(positionName).arg(positionId));
        LOG_INFO(QString("切换到位置: %1/%2").arg(stationId).arg(positionId));
    }
#else
    Q_UNUSED(index);
    Q_UNUSED(positionId);
#endif
}

void MainWindow::onSceneSelected(int index)
{
#ifdef USE_HALCON
    // 获取当前工位
    auto& stationManager = Core::MultiStationManager::instance();
    auto* station = stationManager.currentStation();

    if (!station) {
        LOG_WARNING("onSceneSelected: 没有当前工位");
        return;
    }

    // 检查场景索引有效性
    if (index < 0 || index >= station->scenes.size()) {
        LOG_WARNING(QString("onSceneSelected: 场景索引无效 %1").arg(index));
        return;
    }

    // 切换场景
    if (station->switchToScene(index)) {
        const Core::SceneConfig* scene = station->getCurrentScene();
        QString sceneName = scene ? scene->sceneName : QString("场景%1").arg(index + 1);
        statusLabel_->setText(QString("切换到场景: %1").arg(sceneName));
        LOG_INFO(QString("切换到场景索引 %1: %2").arg(index).arg(sceneName));

        // 更新场景切换栏
        if (sceneSwitchBar_) {
            sceneSwitchBar_->setCurrentScene(index);
        }

        // 加载场景对应的工具链
        loadSceneToolChain(scene);
    } else {
        LOG_ERROR(QString("场景切换失败: 索引 %1").arg(index));
        statusLabel_->setText(QString("场景切换失败"));
    }
#else
    Q_UNUSED(index);
#endif
}

void MainWindow::onPreviousSceneRequested()
{
#ifdef USE_HALCON
    // 获取当前工位
    auto& stationManager = Core::MultiStationManager::instance();
    auto* station = stationManager.currentStation();

    if (!station) {
        LOG_WARNING("onPreviousSceneRequested: 没有当前工位");
        return;
    }

    int sceneCount = station->scenes.size();
    if (sceneCount <= 1) {
        statusLabel_->setText("只有一个场景，无法切换");
        return;
    }

    // 获取当前索引
    int currentIndex = station->currentSceneIndex;

    // 切换到上一个场景（循环）
    int prevIndex = (currentIndex - 1 + sceneCount) % sceneCount;

    if (station->switchToScene(prevIndex)) {
        const Core::SceneConfig* scene = station->getCurrentScene();
        QString sceneName = scene ? scene->sceneName : QString("场景%1").arg(prevIndex + 1);
        statusLabel_->setText(QString("切换到场景: %1").arg(sceneName));
        LOG_INFO(QString("切换到上一个场景: %1").arg(sceneName));

        // 更新场景切换栏
        if (sceneSwitchBar_) {
            sceneSwitchBar_->setCurrentScene(prevIndex);
        }

        // 加载场景对应的工具链
        loadSceneToolChain(scene);
    } else {
        LOG_ERROR(QString("场景切换失败: 索引 %1").arg(prevIndex));
        statusLabel_->setText(QString("场景切换失败"));
    }
#endif
}

void MainWindow::onNextSceneRequested()
{
#ifdef USE_HALCON
    // 获取当前工位
    auto& stationManager = Core::MultiStationManager::instance();
    auto* station = stationManager.currentStation();

    if (!station) {
        LOG_WARNING("onNextSceneRequested: 没有当前工位");
        return;
    }

    int sceneCount = station->scenes.size();
    if (sceneCount <= 1) {
        statusLabel_->setText("只有一个场景，无法切换");
        return;
    }

    // 获取当前索引
    int currentIndex = station->currentSceneIndex;

    // 切换到下一个场景（循环）
    int nextIndex = (currentIndex + 1) % sceneCount;

    if (station->switchToScene(nextIndex)) {
        const Core::SceneConfig* scene = station->getCurrentScene();
        QString sceneName = scene ? scene->sceneName : QString("场景%1").arg(nextIndex + 1);
        statusLabel_->setText(QString("切换到场景: %1").arg(sceneName));
        LOG_INFO(QString("切换到下一个场景: %1").arg(sceneName));

        // 更新场景切换栏
        if (sceneSwitchBar_) {
            sceneSwitchBar_->setCurrentScene(nextIndex);
        }

        // 加载场景对应的工具链
        loadSceneToolChain(scene);
    } else {
        LOG_ERROR(QString("场景切换失败: 索引 %1").arg(nextIndex));
        statusLabel_->setText(QString("场景切换失败"));
    }
#endif
}

void MainWindow::loadSceneToolChain(const Core::SceneConfig* scene)
{
    if (!scene) {
        LOG_WARNING("loadSceneToolChain: 场景配置为空");
        return;
    }

    if (!toolChainPanel_) {
        LOG_WARNING("loadSceneToolChain: 工具链面板未初始化");
        return;
    }

    // 检查场景是否有工具链配置文件
    if (scene->toolChainFile.isEmpty()) {
        LOG_DEBUG(QString("场景 '%1' 没有工具链配置文件，清空当前工具链").arg(scene->sceneName));
        // 清空当前工具链面板
        QList<Algorithm::VisionTool*> currentTools = toolChainPanel_->getTools();
        for (auto* tool : currentTools) {
            toolChainPanel_->removeTool(tool);
            delete tool;
        }
        return;
    }

    // 检查文件是否存在
    if (!QFile::exists(scene->toolChainFile)) {
        LOG_WARNING(QString("场景 '%1' 的工具链文件不存在: %2")
                   .arg(scene->sceneName).arg(scene->toolChainFile));
        return;
    }

    // 使用 ToolChain 加载工具链配置
    Core::ToolChain tempToolChain;
    if (!tempToolChain.loadFromFile(scene->toolChainFile)) {
        LOG_ERROR(QString("加载场景工具链失败: %1").arg(scene->toolChainFile));
        return;
    }

    LOG_INFO(QString("加载场景 '%1' 的工具链: %2, 工具数: %3")
            .arg(scene->sceneName)
            .arg(scene->toolChainFile)
            .arg(tempToolChain.toolCount()));

    // 清空当前工具链面板的工具（并释放内存）
    QList<Algorithm::VisionTool*> currentTools = toolChainPanel_->getTools();
    for (auto* tool : currentTools) {
        toolChainPanel_->removeTool(tool);
        delete tool;
    }

    // 从加载的工具链中复制工具到面板
    // 注意：ToolChain 使用 unique_ptr 管理工具，我们需要重新创建工具
    // 由于 ToolChain 不提供直接获取工具的方法，这里需要特殊处理
    // 我们直接将 ToolChain 的工具移动到面板（需要 ToolChain 提供相应接口）

    // 目前的设计限制：ToolChain 使用 unique_ptr，而 ToolChainPanel 使用裸指针
    // 作为临时方案，我们将工具从 ToolChain 中提取出来（需要修改 ToolChain 类）
    // 或者我们可以通过序列化/反序列化来传递工具配置

    // 简化实现：使用 ToolChain 的 serialize/deserialize 机制
    // 重新从文件读取配置，然后用 ToolFactory 创建工具
    QFile file(scene->toolChainFile);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开工具链文件: %1").arg(scene->toolChainFile));
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        LOG_ERROR(QString("工具链文件格式错误: %1").arg(scene->toolChainFile));
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonArray toolsArray = rootObj["tools"].toArray();

    for (const auto& toolVal : toolsArray) {
        QJsonObject toolObj = toolVal.toObject();

        // 获取工具类型（整数枚举值）
        int toolType = toolObj["toolType"].toInt(-1);
        QString toolName = toolObj["name"].toString();
        bool enabled = toolObj["enabled"].toBool(true);
        QJsonObject params = toolObj["params"].toObject();

        // 使用工厂单例创建工具
        Algorithm::VisionTool* tool = Algorithm::ToolFactory::instance().createTool(
            static_cast<Algorithm::VisionTool::ToolType>(toolType)
        );
        if (tool) {
            tool->setDisplayName(toolName);
            tool->setEnabled(enabled);
            // 加载工具参数
            tool->deserializeParams(params);
            toolChainPanel_->addTool(tool);
            LOG_DEBUG(QString("  添加工具: %1 (type=%2)").arg(toolName).arg(toolType));
        } else {
            LOG_WARNING(QString("  无法创建工具: %1 (type=%2)").arg(toolName).arg(toolType));
        }
    }

    statusLabel_->setText(QString("已加载场景 '%1' 的工具链 (%2 个工具)")
                         .arg(scene->sceneName).arg(toolChainPanel_->getTools().size()));
}

// ========== VisionEngine 信号连接 ==========

void MainWindow::connectEngineSignals()
{
    // 连接图像更新信号
    connect(&engine_, &Core::VisionEngine::imageUpdated,
            this, &MainWindow::onEngineImageUpdated);

    // 连接连续采集帧信号
    connect(&engine_, &Core::VisionEngine::liveFrameArrived,
            this, &MainWindow::onEngineLiveFrame);

    // 连接处理完成信号
    connect(&engine_, &Core::VisionEngine::processCompleted,
            this, &MainWindow::onEngineProcessCompleted);

    // 连接相机状态变化信号
    connect(&engine_, &Core::VisionEngine::cameraStatusChanged,
            this, &MainWindow::onEngineCameraStatusChanged);

    // 连接图像序列变化信号
    connect(&engine_, &Core::VisionEngine::imageSequenceChanged,
            this, &MainWindow::onEngineImageSequenceChanged);

    // 连接错误信号
    connect(&engine_, &Core::VisionEngine::errorOccurred,
            this, &MainWindow::onEngineError);

    // 连接状态消息信号
    connect(&engine_, &Core::VisionEngine::statusMessage,
            this, &MainWindow::onEngineStatusMessage);
}

void MainWindow::onEngineImageUpdated(Base::ImageData::Ptr image)
{
    if (!image) {
        return;
    }

    // 根据当前模式显示图像
    if (isMultiViewMode_ && multiCameraView_ && multiCameraView_->isVisible()) {
        // 多视图模式：更新当前选中的视图
        int selectedView = multiCameraView_->selectedView();
        if (selectedView >= 0) {
            multiCameraView_->updateImage(selectedView, image);
        }
        // 同时更新隐藏的单图像查看器
        if (imageViewer_) {
            imageViewer_->setImage(image);
        }
    } else {
        // 单视图模式
        if (imageViewer_) {
            imageViewer_->setImage(image);
        }
    }

    updateActions();
    updateStatusBar();
}

void MainWindow::onEngineLiveFrame(Base::ImageData::Ptr image)
{
    if (!image || !isLiveDisplay_) {
        return;
    }

    // 实时显示帧
    if (isMultiViewMode_ && multiCameraView_ && multiCameraView_->isVisible()) {
        int selectedView = multiCameraView_->selectedView();
        if (selectedView >= 0) {
            multiCameraView_->updateImage(selectedView, image);
        }
    } else if (imageViewer_) {
        imageViewer_->setImage(image);
    }
}

void MainWindow::onEngineProcessCompleted(const Core::ProcessResult& result)
{
    if (result.success) {
        // 更新结果表
        for (const auto& toolResult : result.toolResults) {
            if (resultTablePanel_) {
                // 从结果数据中获取工具名称（如果有）
                QString toolName = toolResult.data.value("toolName").toString();
                if (toolName.isEmpty()) {
                    toolName = "工具";
                }
                resultTablePanel_->addResult(toolName, toolResult);
            }

            // 处理displayObjects - XLD轮廓显示
#ifdef USE_HALCON
            if (!toolResult.displayObjects.isEmpty()) {
                // 遍历所有显示对象
                QMapIterator<QString, QVariant> i(toolResult.displayObjects);
                while (i.hasNext()) {
                    i.next();
                    if (i.key() == "match_contours" || i.key().contains("contour")) {
                        QVariant val = i.value();
                        if (val.canConvert<Algorithm::HalconObjectPtr>()) {
                            auto ptr = val.value<Algorithm::HalconObjectPtr>();
                            if (ptr && ptr->type() == Algorithm::HalconObjectWrapper::XLD_Contour) {
                                auto* wrapper = static_cast<Algorithm::XLDContourWrapper*>(ptr.data());
                                
                                // 显示在图像查看器中
                                if (imageViewer_) {
                                    imageViewer_->addXLDContour(wrapper->contours());
                                }
                                if (isMultiViewMode_ && multiCameraView_) {
                                    // 多视图模式下也显示
                                    // TODO: 需要确定显示在哪个视图，目前简化为当前视图
                                    // multiCameraView_->displayXLD(wrapper->contours());
                                }
                            }
                        }
                    }
                }
            }
#endif
        }

        // 更新统计
        if (statisticsPanel_) {
            statisticsPanel_->recordRun(result.success, result.totalTime, QString(), result.toolResults.size());
        }

        statusLabel_->setText(QString("处理完成，耗时: %.2f ms").arg(result.totalTime));
    } else {
        // 即使失败也记录统计
        if (statisticsPanel_) {
            statisticsPanel_->recordRun(result.success, result.totalTime, QString(), 0);
        }
        statusLabel_->setText(QString("处理失败: %1").arg(result.errorMessage));
    }
}

void MainWindow::onEngineCameraStatusChanged(bool connected)
{
    if (connected) {
        statusLabel_->setText("相机已连接");

        // 更新工具参数面板的相机指针
        if (toolParameterPanel_) {
            toolParameterPanel_->setCamera(engine_.camera());
        }
    } else {
        statusLabel_->setText("相机已断开");
    }

    updateActions();
}

void MainWindow::onEngineImageSequenceChanged(int count, int currentIndex)
{
    // 更新图片序列相关动作状态
    bool hasSequence = count > 0;
    bool canGoPrev = hasSequence && currentIndex > 0;
    bool canGoNext = hasSequence && currentIndex < count - 1;

    if (previousImageAction_) previousImageAction_->setEnabled(canGoPrev);
    if (nextImageAction_) nextImageAction_->setEnabled(canGoNext);
    if (runAllImagesAction_) runAllImagesAction_->setEnabled(hasSequence);
}

void MainWindow::onEngineError(const QString& message)
{
    LOG_ERROR(message);
    QMessageBox::warning(this, "错误", message);
}

void MainWindow::onEngineStatusMessage(const QString& message)
{
    if (statusLabel_) {
        statusLabel_->setText(message);
    }
    LOG_INFO(message);
}

void MainWindow::updateImageSequenceActions()
{
    // 更新图片序列相关动作状态
    int sequenceCount = engine_.getImageSequenceCount();
    int currentIndex = engine_.getCurrentImageIndex();

    bool hasSequence = sequenceCount > 1;
    bool isFirstImage = currentIndex == 0;
    bool isLastImage = currentIndex == sequenceCount - 1;

    if (previousImageAction_) {
        previousImageAction_->setEnabled(hasSequence && !isFirstImage);
    }
    if (nextImageAction_) {
        nextImageAction_->setEnabled(hasSequence && !isLastImage);
    }
    if (runAllImagesAction_) {
        runAllImagesAction_->setEnabled(hasSequence);
    }
}

} // namespace UI
} // namespace VisionForge
