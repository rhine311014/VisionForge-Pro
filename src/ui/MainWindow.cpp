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
#include <QMessageBox>
#include <QInputDialog>
#include <QSplitter>
#include <QActionGroup>
#include <opencv2/opencv.hpp>
#include <chrono>

#ifdef USE_HALCON
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/HalconObjectWrapper.h"
#include "ui/ShapeMatchToolDialog.h"
#endif

#include "ui/PLCConfigDialog.h"
#include "ui/CameraConfigDialog.h"
#include "ui/RecipeEditorDialog.h"
#include "ui/CameraCalibDialog.h"
#include "ui/NinePointCalibDialog.h"
#include "ui/SystemSettingsDialog.h"
// 工具对话框 - 预处理
#include "ui/GrayToolDialog.h"
#include "ui/BlurToolDialog.h"
#include "ui/ThresholdToolDialog.h"
#include "ui/ColorConvertToolDialog.h"
#include "ui/EdgeToolDialog.h"
#include "ui/MorphologyToolDialog.h"
// 工具对话框 - 检测
#include "ui/TemplateMatchToolDialog.h"
#include "ui/CircleToolDialog.h"
#include "ui/LineToolDialog.h"
#include "ui/FindEdgeToolDialog.h"
#include "ui/BlobToolDialog.h"
#include "ui/AIDetectionToolDialog.h"
// 工具对话框 - 测量和判定
#include "ui/MeasureDistanceToolDialog.h"
#include "ui/MeasureAngleToolDialog.h"
#include "ui/MeasureAreaToolDialog.h"
#include "ui/CalcCenterToolDialog.h"
#include "ui/CalcOrientationToolDialog.h"
#include "ui/RangeJudgeToolDialog.h"
#include "ui/LogicOperationToolDialog.h"
#include "ui/SaveImageToolDialog.h"
// 工具对话框 - ROI和PLC
#include "ui/ROIToolDialog.h"
#include "ui/PLCOutputToolDialog.h"
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
    , camera_(nullptr)
    , isContinuousGrabbing_(false)
    , currentImageIndex_(-1)
{
    setWindowTitle("VisionForge Pro - 机器视觉检测平台");

    // 设置主窗口样式，移除所有内边距和边距
    setStyleSheet("QMainWindow { padding: 0px; margin: 0px; } "
                  "QMainWindow::separator { width: 0px; height: 0px; }");

    // 创建中央图像查看器
#ifdef USE_HALCON
    imageViewer_ = new HalconImageViewer(this);
#else
    imageViewer_ = new ImageViewer(this);
#endif

    // 设置中央窗口，确保没有边距
    setCentralWidget(imageViewer_);

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

    // 创建模拟相机作为默认相机（备用）
    HAL::SimulatedCamera* simCamera = new HAL::SimulatedCamera(this);
    simCamera->useTestPattern(0);  // 使用渐变测试图案
    camera_.reset(simCamera);

    // 连续采集定时器
    continuousTimer_ = new QTimer(this);
    connect(continuousTimer_, &QTimer::timeout, this, &MainWindow::onContinuousTimer);

    // 初始化配方管理器（扫描目录并加载上次使用的配方）
    Core::RecipeManager::instance().initialize();

    // 尝试自动连接已保存的相机配置
    if (tryAutoConnectCamera()) {
        LOG_INFO("已自动连接保存的相机配置");
    } else {
        LOG_DEBUG("使用模拟相机作为默认相机");
    }

    // 更新界面状态
    updateActions();
    updateStatusBar();

    LOG_INFO("主窗口初始化完成");
}

MainWindow::~MainWindow()
{
    if (camera_) {
        camera_->close();
    }

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

    // 使用Qt读取文件（支持中文路径），然后用OpenCV解码
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法打开图像文件");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // 使用OpenCV解码图像数据
    std::vector<uchar> buffer(fileData.begin(), fileData.end());
    cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

    if (mat.empty()) {
        QMessageBox::warning(this, "错误", "无法解码图像文件");
        return;
    }

    currentImage_ = std::make_shared<Base::ImageData>(mat);
    imageViewer_->setImage(currentImage_);

    statusLabel_->setText(QString("已加载图像: %1").arg(fileName));
    updateActions();
    updateStatusBar();

    LOG_INFO(QString("加载图像: %1").arg(fileName));
}

void MainWindow::onOpenImageFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
        "选择图片文件夹", "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (folderPath.isEmpty()) {
        return;
    }

    // 获取文件夹中的所有图片文件
    QDir dir(folderPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.tiff" << "*.tif";
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Name);

    if (files.isEmpty()) {
        QMessageBox::information(this, "提示", "所选文件夹中没有找到图片文件");
        return;
    }

    // 构建完整路径列表
    imageSequence_.clear();
    for (const QString& file : files) {
        imageSequence_.append(dir.absoluteFilePath(file));
    }

    // 加载第一张图片
    currentImageIndex_ = 0;
    loadImageAtIndex(currentImageIndex_);

    statusLabel_->setText(QString("已加载图片文件夹: %1 (%2张图片)")
        .arg(folderPath).arg(imageSequence_.size()));
    updateImageSequenceActions();

    LOG_INFO(QString("加载图片文件夹: %1, 共%2张图片")
        .arg(folderPath).arg(imageSequence_.size()));
}

void MainWindow::onPreviousImage()
{
    if (imageSequence_.isEmpty() || currentImageIndex_ <= 0) {
        return;
    }

    currentImageIndex_--;
    loadImageAtIndex(currentImageIndex_);
    updateImageSequenceActions();
}

void MainWindow::onNextImage()
{
    if (imageSequence_.isEmpty() || currentImageIndex_ >= imageSequence_.size() - 1) {
        return;
    }

    currentImageIndex_++;
    loadImageAtIndex(currentImageIndex_);
    updateImageSequenceActions();
}

void MainWindow::onRunAllImages()
{
    if (imageSequence_.isEmpty()) {
        QMessageBox::information(this, "提示", "请先加载图片文件夹");
        return;
    }

    QList<Algorithm::VisionTool*> tools = toolChainPanel_->getTools();
    if (tools.isEmpty()) {
        QMessageBox::information(this, "提示", "请先添加处理工具");
        return;
    }

    int totalImages = imageSequence_.size();

    statusLabel_->setText(QString("正在批量处理 %1 张图片...").arg(totalImages));

    // 禁用UI控件，防止在处理期间进行其他操作
    setEnabled(false);
    statusLabel_->setText(QString("批量处理中: 0/%1").arg(totalImages));

    // 创建工作线程（使用BatchProcessWorker替代InspectionWorker）
    auto worker = new BatchProcessWorker(imageSequence_, tools, this);

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

void MainWindow::loadImageAtIndex(int index)
{
    if (index < 0 || index >= imageSequence_.size()) {
        return;
    }

    QString filePath = imageSequence_[index];

    // 使用Qt读取文件（支持中文路径）
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误",
            QString("无法打开图像文件: %1").arg(filePath));
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // 使用OpenCV解码图像数据
    std::vector<uchar> buffer(fileData.begin(), fileData.end());
    cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

    if (mat.empty()) {
        QMessageBox::warning(this, "错误",
            QString("无法解码图像文件: %1").arg(filePath));
        return;
    }

    currentImage_ = std::make_shared<Base::ImageData>(mat);
    imageViewer_->setImage(currentImage_);

    // 更新状态栏显示当前图片信息
    QFileInfo fileInfo(filePath);
    statusLabel_->setText(QString("图片 %1/%2: %3")
        .arg(index + 1)
        .arg(imageSequence_.size())
        .arg(fileInfo.fileName()));
    updateActions();
    updateStatusBar();
}

void MainWindow::updateImageSequenceActions()
{
    bool hasSequence = !imageSequence_.isEmpty();
    bool canGoPrev = hasSequence && currentImageIndex_ > 0;
    bool canGoNext = hasSequence && currentImageIndex_ < imageSequence_.size() - 1;

    previousImageAction_->setEnabled(canGoPrev);
    nextImageAction_->setEnabled(canGoNext);
    runAllImagesAction_->setEnabled(hasSequence);
}

void MainWindow::onSaveImage()
{
    if (!currentImage_) {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
        "保存图像", "",
        "PNG图像 (*.png);;JPEG图像 (*.jpg *.jpeg);;BMP图像 (*.bmp)");

    if (fileName.isEmpty()) {
        return;
    }

    if (cv::imwrite(fileName.toStdString(), currentImage_->mat())) {
        statusLabel_->setText(QString("图像已保存: %1").arg(fileName));
        LOG_INFO(QString("保存图像: %1").arg(fileName));
    } else {
        QMessageBox::warning(this, "错误", "保存图像失败");
    }
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
        delete tool;

        statusLabel_->setText("已删除工具");
        updateActions();
    }
}

void MainWindow::onRunSingle()
{
    Algorithm::VisionTool* tool = toolChainPanel_->getCurrentTool();
    if (!tool || !currentImage_) {
        return;
    }

    if (!tool->isEnabled()) {
        QMessageBox::information(this, "提示", "当前工具已禁用");
        return;
    }

    Algorithm::ToolResult result;
    if (tool->process(currentImage_, result)) {
        if (result.outputImage) {
            currentImage_ = result.outputImage;
            imageViewer_->setImage(currentImage_);

            // 处理displayObjects - XLD轮廓显示
#ifdef USE_HALCON
            if (!result.displayObjects.isEmpty() &&
                result.displayObjects.contains("match_contours")) {
                QVariant objVariant = result.displayObjects["match_contours"];
                if (objVariant.canConvert<Algorithm::HalconObjectPtr>()) {
                    Algorithm::HalconObjectPtr objPtr =
                        objVariant.value<Algorithm::HalconObjectPtr>();

                    if (objPtr && objPtr->type() == Algorithm::HalconObjectWrapper::XLD_Contour) {
                        Algorithm::XLDContourPtr xldPtr =
                            qSharedPointerCast<Algorithm::XLDContourWrapper>(objPtr);

                        QList<HXLDCont> contours;
                        contours.append(xldPtr->contours());
                        imageViewer_->setXLDContours(contours);
                    }
                }
            } else {
                // 清除之前的XLD显示
                imageViewer_->clearXLDContours();
            }
#endif

            statusLabel_->setText(QString("工具 \"%1\" 处理完成").arg(tool->displayName()));
        }
    } else {
        QMessageBox::warning(this, "处理失败", result.errorMessage);
    }

    updateStatusBar();
}

void MainWindow::onRunAll()
{
    if (!currentImage_) {
        QMessageBox::information(this, "提示", "请先加载或采集图像");
        return;
    }

    processImage(currentImage_);
}

// ========== 相机操作 ==========

void MainWindow::onOpenCamera()
{
    if (!camera_) {
        return;
    }

    if (camera_->open()) {
        statusLabel_->setText("相机已打开");
        updateActions();
        LOG_INFO("相机已打开");
    } else {
        QMessageBox::warning(this, "错误", "打开相机失败");
    }
}

void MainWindow::onCloseCamera()
{
    if (!camera_) {
        return;
    }

    camera_->close();
    continuousTimer_->stop();
    isContinuousGrabbing_ = false;

    statusLabel_->setText("相机已关闭");
    updateActions();
    LOG_INFO("相机已关闭");
}

void MainWindow::onGrabImage()
{
    if (!camera_ || !camera_->isOpen()) {
        return;
    }

    camera_->trigger();
    Base::ImageData::Ptr image = camera_->grabImage(5000);

    if (image) {
        // 应用图像变换（旋转、镜像）
        applyImageTransform(image);

        currentImage_ = image;
        imageViewer_->setImage(currentImage_);
        statusLabel_->setText("图像采集成功");
        updateActions();
        updateStatusBar();

        LOG_DEBUG("采集图像成功");
    } else {
        QMessageBox::warning(this, "错误", "图像采集失败");
    }
}

void MainWindow::onContinuousGrab()
{
    if (!camera_ || !camera_->isOpen()) {
        return;
    }

    if (!isContinuousGrabbing_) {
        isContinuousGrabbing_ = true;
        continuousTimer_->start(100);  // 10 FPS
        continuousGrabAction_->setText("停止连续采集");
        statusLabel_->setText("连续采集中...");
        LOG_INFO("开始连续采集");
    } else {
        isContinuousGrabbing_ = false;
        continuousTimer_->stop();
        continuousGrabAction_->setText("连续采集");
        statusLabel_->setText("已停止连续采集");
        LOG_INFO("停止连续采集");
    }

    updateActions();
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
    // 检查是否是形状匹配工具
    Algorithm::ShapeMatchTool* shapeMatchTool =
        dynamic_cast<Algorithm::ShapeMatchTool*>(tool);

    if (shapeMatchTool) {
        // 弹出形状匹配工具对话框
        ShapeMatchToolDialog dialog(shapeMatchTool, this);

        // 设置当前图像
        if (currentImage_) {
            dialog.setImage(currentImage_);
        }

        // 连接训练请求信号
        connect(&dialog, &ShapeMatchToolDialog::trainModelRequested, this, [this, shapeMatchTool]() {
            // 获取ROI区域并训练
            std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
            if (!rois.empty() && currentImage_) {
                ROIShapePtr roi = rois.front();
                QRectF rect = roi->boundingRect();

                // 训练模板
                if (shapeMatchTool->trainModel(currentImage_,
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
            // 参数已应用
            statusLabel_->setText("工具参数已更新");
            updateActions();
        }
        return;
    }
#endif

    // ========== 预处理工具 ==========
    // 灰度转换工具
    if (Algorithm::GrayTool* grayTool = dynamic_cast<Algorithm::GrayTool*>(tool)) {
        GrayToolDialog dialog(grayTool, this);
        dialog.setPreviewImage(currentImage_);
        connect(&dialog, &GrayToolDialog::previewRequested, this, &MainWindow::onRunSingle);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("灰度转换参数已更新");
        }
        return;
    }

    // 模糊工具
    if (Algorithm::BlurTool* blurTool = dynamic_cast<Algorithm::BlurTool*>(tool)) {
        BlurToolDialog dialog(blurTool, this);
        dialog.setPreviewImage(currentImage_);
        connect(&dialog, &BlurToolDialog::previewRequested, this, &MainWindow::onRunSingle);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("模糊参数已更新");
        }
        return;
    }

    // 二值化工具
    if (Algorithm::ThresholdTool* thresholdTool = dynamic_cast<Algorithm::ThresholdTool*>(tool)) {
        ThresholdToolDialog dialog(thresholdTool, this);
        dialog.setPreviewImage(currentImage_);
        connect(&dialog, &ThresholdToolDialog::previewRequested, this, &MainWindow::onRunSingle);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("二值化参数已更新");
        }
        return;
    }

    // 颜色转换工具
    if (Algorithm::ColorConvertTool* colorTool = dynamic_cast<Algorithm::ColorConvertTool*>(tool)) {
        ColorConvertToolDialog dialog(colorTool, this);
        dialog.setPreviewImage(currentImage_);
        connect(&dialog, &ColorConvertToolDialog::previewRequested, this, &MainWindow::onRunSingle);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("颜色转换参数已更新");
        }
        return;
    }

    // 边缘检测工具
    if (Algorithm::EdgeTool* edgeTool = dynamic_cast<Algorithm::EdgeTool*>(tool)) {
        EdgeToolDialog dialog(edgeTool, this);
        dialog.setPreviewImage(currentImage_);
        connect(&dialog, &EdgeToolDialog::previewRequested, this, &MainWindow::onRunSingle);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("边缘检测参数已更新");
        }
        return;
    }

    // 形态学工具
    if (Algorithm::MorphologyTool* morphTool = dynamic_cast<Algorithm::MorphologyTool*>(tool)) {
        MorphologyToolDialog dialog(morphTool, this);
        dialog.setPreviewImage(currentImage_);
        connect(&dialog, &MorphologyToolDialog::previewRequested, this, &MainWindow::onRunSingle);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("形态学参数已更新");
        }
        return;
    }

    // ========== 检测工具 ==========
    // 模板匹配工具
    if (Algorithm::TemplateMatchTool* templateTool = dynamic_cast<Algorithm::TemplateMatchTool*>(tool)) {
        TemplateMatchToolDialog dialog(templateTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("模板匹配参数已更新");
        }
        return;
    }

    // 圆检测工具
    if (Algorithm::CircleTool* circleTool = dynamic_cast<Algorithm::CircleTool*>(tool)) {
        CircleToolDialog dialog(circleTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("圆检测参数已更新");
        }
        return;
    }

    // 线检测工具
    if (Algorithm::LineTool* lineTool = dynamic_cast<Algorithm::LineTool*>(tool)) {
        LineToolDialog dialog(lineTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("线检测参数已更新");
        }
        return;
    }

    // 边缘查找工具
    if (Algorithm::FindEdgeTool* findEdgeTool = dynamic_cast<Algorithm::FindEdgeTool*>(tool)) {
        FindEdgeToolDialog dialog(findEdgeTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("边缘查找参数已更新");
        }
        return;
    }

    // Blob分析工具
    if (Algorithm::BlobTool* blobTool = dynamic_cast<Algorithm::BlobTool*>(tool)) {
        BlobToolDialog dialog(blobTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("Blob分析参数已更新");
        }
        return;
    }

    // AI检测工具
    if (Algorithm::AIDetectionTool* aiTool = dynamic_cast<Algorithm::AIDetectionTool*>(tool)) {
        AIDetectionToolDialog dialog(aiTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("AI检测参数已更新");
        }
        return;
    }

    // ========== 测量工具 ==========
    // 距离测量工具
    if (Algorithm::MeasureDistanceTool* distTool = dynamic_cast<Algorithm::MeasureDistanceTool*>(tool)) {
        MeasureDistanceToolDialog dialog(distTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("距离测量参数已更新");
        }
        return;
    }

    // 角度测量工具
    if (Algorithm::MeasureAngleTool* angleTool = dynamic_cast<Algorithm::MeasureAngleTool*>(tool)) {
        MeasureAngleToolDialog dialog(angleTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("角度测量参数已更新");
        }
        return;
    }

    // 面积测量工具
    if (Algorithm::MeasureAreaTool* areaTool = dynamic_cast<Algorithm::MeasureAreaTool*>(tool)) {
        MeasureAreaToolDialog dialog(areaTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("面积测量参数已更新");
        }
        return;
    }

    // 中心计算工具
    if (Algorithm::CalcCenterTool* centerTool = dynamic_cast<Algorithm::CalcCenterTool*>(tool)) {
        CalcCenterToolDialog dialog(centerTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("中心计算参数已更新");
        }
        return;
    }

    // 方向计算工具
    if (Algorithm::CalcOrientationTool* orientTool = dynamic_cast<Algorithm::CalcOrientationTool*>(tool)) {
        CalcOrientationToolDialog dialog(orientTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("方向计算参数已更新");
        }
        return;
    }

    // ========== 判定工具 ==========
    // 范围判定工具
    if (Algorithm::RangeJudgeTool* judgeTool = dynamic_cast<Algorithm::RangeJudgeTool*>(tool)) {
        RangeJudgeToolDialog dialog(judgeTool, this);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("范围判定参数已更新");
        }
        return;
    }

    // 逻辑运算工具
    if (Algorithm::LogicOperationTool* logicTool = dynamic_cast<Algorithm::LogicOperationTool*>(tool)) {
        LogicOperationToolDialog dialog(logicTool, this);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("逻辑运算参数已更新");
        }
        return;
    }

    // 图像保存工具
    if (Algorithm::SaveImageTool* saveTool = dynamic_cast<Algorithm::SaveImageTool*>(tool)) {
        SaveImageToolDialog dialog(saveTool, this);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("图像保存参数已更新");
        }
        return;
    }

    // ========== ROI和PLC工具 ==========
    // ROI区域工具
    if (Algorithm::ROITool* roiTool = dynamic_cast<Algorithm::ROITool*>(tool)) {
        ROIToolDialog dialog(roiTool, this);
        dialog.setImage(currentImage_);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("ROI区域参数已更新");
        }
        return;
    }

    // PLC输出工具
    if (Algorithm::PLCOutputTool* plcTool = dynamic_cast<Algorithm::PLCOutputTool*>(tool)) {
        PLCOutputToolDialog dialog(plcTool, this);
        if (dialog.exec() == QDialog::Accepted) {
            statusLabel_->setText("PLC输出参数已更新");
        }
        return;
    }

    // 其他工具类型，显示提示（基于OpenCV的工具参数在右侧面板编辑）
    QMessageBox::information(this, "提示",
        QString("工具 \"%1\" 的参数可在右侧参数面板中编辑").arg(tool->displayName()));
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

// ========== 图像变换 ==========

void MainWindow::applyImageTransform(Base::ImageData::Ptr& image)
{
    if (!image || !camera_) return;

    HAL::ICamera::Config config = camera_->getConfig();

    // 如果没有任何变换，直接返回
    if (config.rotationAngle == 0 && !config.flipHorizontal && !config.flipVertical) {
        return;
    }

    cv::Mat mat = image->mat();
    if (mat.empty()) return;

    cv::Mat result = mat.clone();

    // 1. 旋转
    if (config.rotationAngle != 0) {
        int rotateCode = -1;
        switch (config.rotationAngle) {
            case 90:
                rotateCode = cv::ROTATE_90_CLOCKWISE;
                break;
            case 180:
                rotateCode = cv::ROTATE_180;
                break;
            case 270:
                rotateCode = cv::ROTATE_90_COUNTERCLOCKWISE;
                break;
            default:
                break;
        }
        if (rotateCode >= 0) {
            cv::Mat rotated;
            cv::rotate(result, rotated, rotateCode);
            result = rotated;
        }
    }

    // 2. 镜像
    if (config.flipHorizontal && config.flipVertical) {
        cv::Mat flipped;
        cv::flip(result, flipped, -1);  // 同时水平和垂直翻转
        result = flipped;
    } else if (config.flipHorizontal) {
        cv::Mat flipped;
        cv::flip(result, flipped, 1);   // 水平翻转
        result = flipped;
    } else if (config.flipVertical) {
        cv::Mat flipped;
        cv::flip(result, flipped, 0);   // 垂直翻转
        result = flipped;
    }

    // 更新ImageData
    image = std::make_shared<Base::ImageData>(result);
}

// ========== 连续采集 ==========

void MainWindow::onContinuousTimer()
{
    if (!camera_ || !camera_->isOpen()) {
        continuousTimer_->stop();
        isContinuousGrabbing_ = false;
        return;
    }

    camera_->trigger();
    Base::ImageData::Ptr image = camera_->grabImage(100);

    if (image) {
        // 应用图像变换（旋转、镜像）
        applyImageTransform(image);

        // 处理图像
        processImage(image);
    }
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

    openCameraAction_ = new QAction(Theme::getIcon(Icons::CAMERA_PHOTO), "打开相机(&O)", this);
    openCameraAction_->setStatusTip("打开相机设备");
    connect(openCameraAction_, &QAction::triggered, this, &MainWindow::onOpenCamera);
    cameraMenu_->addAction(openCameraAction_);

    closeCameraAction_ = new QAction(Theme::getIcon(Icons::TOOL_STOP), "关闭相机(&C)", this);
    closeCameraAction_->setStatusTip("关闭相机设备");
    connect(closeCameraAction_, &QAction::triggered, this, &MainWindow::onCloseCamera);
    cameraMenu_->addAction(closeCameraAction_);

    cameraMenu_->addSeparator();

    grabImageAction_ = new QAction(Theme::getIcon(Icons::CAMERA_PHOTO), tr("采集单帧(&G)"), this);
    grabImageAction_->setShortcut(Qt::Key_F7);
    grabImageAction_->setStatusTip(tr("采集单帧图像 (F7 / Ctrl+G)"));
    connect(grabImageAction_, &QAction::triggered, this, &MainWindow::onGrabImage);
    cameraMenu_->addAction(grabImageAction_);

    // 添加备用快捷键 Ctrl+G
    QAction* grabShortcut = new QAction(this);
    grabShortcut->setShortcut(Qt::CTRL | Qt::Key_G);
    connect(grabShortcut, &QAction::triggered, this, &MainWindow::onGrabImage);
    addAction(grabShortcut);

    continuousGrabAction_ = new QAction(Theme::getIcon(Icons::CAMERA_VIDEO), "连续采集(&C)", this);
    continuousGrabAction_->setCheckable(true);
    continuousGrabAction_->setStatusTip("连续采集图像");
    connect(continuousGrabAction_, &QAction::triggered, this, &MainWindow::onContinuousGrab);
    cameraMenu_->addAction(continuousGrabAction_);

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
    // 文件工具栏
    fileToolBar_ = addToolBar("文件");
    fileToolBar_->addAction(openImageAction_);
    fileToolBar_->addAction(openImageFolderAction_);
    fileToolBar_->addAction(saveImageAction_);
    fileToolBar_->addSeparator();
    fileToolBar_->addAction(previousImageAction_);
    fileToolBar_->addAction(nextImageAction_);
    fileToolBar_->addAction(runAllImagesAction_);

    // 视图工具栏
    viewToolBar_ = addToolBar("视图");
    viewToolBar_->addAction(fitToWindowAction_);
    viewToolBar_->addAction(actualSizeAction_);
    viewToolBar_->addAction(zoomInAction_);
    viewToolBar_->addAction(zoomOutAction_);

    // 相机工具栏
    cameraToolBar_ = addToolBar("相机");
    cameraToolBar_->addAction(openCameraAction_);
    cameraToolBar_->addAction(closeCameraAction_);
    cameraToolBar_->addAction(grabImageAction_);
    cameraToolBar_->addAction(continuousGrabAction_);

    // 处理工具栏
    processToolBar_ = addToolBar("处理");
    processToolBar_->addAction(runSingleAction_);
    processToolBar_->addAction(runAllAction_);
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
    bool hasImage = (currentImage_ != nullptr);
    bool hasTool = (toolChainPanel_->getCurrentTool() != nullptr);
    bool cameraOpen = (camera_ && camera_->isOpen());

    saveImageAction_->setEnabled(hasImage);

    runSingleAction_->setEnabled(hasImage && hasTool);
    runAllAction_->setEnabled(hasImage);
    removeToolAction_->setEnabled(hasTool);

    openCameraAction_->setEnabled(!cameraOpen);
    closeCameraAction_->setEnabled(cameraOpen);
    grabImageAction_->setEnabled(cameraOpen && !isContinuousGrabbing_);
    continuousGrabAction_->setEnabled(cameraOpen);
}

void MainWindow::updateStatusBar()
{
    if (currentImage_) {
        imageInfoLabel_->setText(QString("图像: %1x%2, %3通道")
            .arg(currentImage_->width())
            .arg(currentImage_->height())
            .arg(currentImage_->channels()));
    } else {
        imageInfoLabel_->setText("图像: --");
    }
}

void MainWindow::processImage(Base::ImageData::Ptr image)
{
    if (!image) {
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
    currentImage_ = result;
    imageViewer_->setImage(currentImage_);

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
                imageViewer_->setXLDContours(contours);
            }
        }
    } else {
        // 清除之前的XLD显示
        imageViewer_->clearXLDContours();
    }
#endif

    // 添加到历史记录
    QString description = toolChainDesc.isEmpty() ? "原始图像" : QString("处理结果 (%1)").arg(toolChainDesc);
    historyPanel_->addRecord(currentImage_, description, toolChainDesc, totalExecutionTime);

    // 更新状态栏
    statusLabel_->setText(QString("处理完成 - 总耗时: %1 ms").arg(totalExecutionTime, 0, 'f', 2));
    updateStatusBar();

    LOG_INFO(QString("图像处理完成 - 工具链: %1, 总耗时: %2 ms")
        .arg(toolChainDesc)
        .arg(totalExecutionTime, 0, 'f', 2));
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
        currentImage_ = record.image;
        imageViewer_->setImage(currentImage_);
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
    CameraConfigDialog dialog(this);

    // 如果有当前相机，传递给对话框
    if (camera_) {
        dialog.setCamera(camera_.get());
    }

    if (dialog.exec() == QDialog::Accepted) {
        // 获取选择的相机
        HAL::ICamera* newCamera = dialog.takeCamera();
        if (newCamera && newCamera != camera_.get()) {
            // 关闭旧相机（unique_ptr自动管理内存）
            if (camera_) {
                camera_->close();
            }
            // 使用reset转移所有权，旧相机自动释放
            camera_.reset(newCamera);
            LOG_INFO(QString("相机已切换: %1").arg(camera_->deviceName()));
        }

        updateActions();
        LOG_INFO("相机配置已更新");
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
    if (currentImage_) {
        dialog.setCurrentImage(currentImage_);
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
    if (currentImage_) {
        dialog.setCurrentImage(currentImage_);
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

    // 成功连接，替换当前相机（unique_ptr自动管理内存）
    if (camera_) {
        camera_->close();
    }
    camera_.reset(newCamera);

    statusLabel_->setText(QString("已自动连接相机: %1").arg(matchedCamera.modelName));
    updateActions();

    LOG_INFO(QString("相机自动连接成功: %1 (%2)")
        .arg(matchedCamera.modelName).arg(matchedCamera.serialNumber));

    return true;
}

} // namespace UI
} // namespace VisionForge

// 包含MOC生成的文件
#include "MainWindow.moc"
