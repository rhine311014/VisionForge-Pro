/**
 * @file MainWindow.cpp
 * @brief 主窗口实现
 */

#include "ui/MainWindow.h"
#include "ui/Theme.h"
#include "algorithm/ToolFactory.h"
#include "base/Logger.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QSplitter>
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
#include "hal/CameraFactory.h"
#include "core/RecipeManager.h"
#include "algorithm/CalibrationManager.h"

// 定义ImageViewer类型宏，根据USE_HALCON选择使用哪个ImageViewer
#ifdef USE_HALCON
#define IMAGEVIEWER_CLASS HalconImageViewer
#else
#define IMAGEVIEWER_CLASS ImageViewer
#endif

namespace VisionForge {
namespace UI {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , imageViewer_(nullptr)
    , toolChainDock_(nullptr)
    , toolParameterDock_(nullptr)
    , resultTableDock_(nullptr)
    , historyDock_(nullptr)
    , recipeDock_(nullptr)
    , toolChainPanel_(nullptr)
    , toolParameterPanel_(nullptr)
    , resultTablePanel_(nullptr)
    , historyPanel_(nullptr)
    , recipeManagerWidget_(nullptr)
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

    // 创建模拟相机作为默认相机
    HAL::SimulatedCamera* simCamera = new HAL::SimulatedCamera(this);
    simCamera->useTestPattern(0);  // 使用渐变测试图案
    camera_ = simCamera;

    // 连续采集定时器
    continuousTimer_ = new QTimer(this);
    connect(continuousTimer_, &QTimer::timeout, this, &MainWindow::onContinuousTimer);

    // 初始化配方管理器（扫描目录并加载上次使用的配方）
    Core::RecipeManager::instance().initialize();

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
    int successCount = 0;
    double totalTime = 0.0;

    statusLabel_->setText(QString("正在批量处理 %1 张图片...").arg(totalImages));
    QApplication::processEvents();

    for (int i = 0; i < totalImages; ++i) {
        // 加载图片
        currentImageIndex_ = i;
        loadImageAtIndex(i);

        if (!currentImage_) {
            LOG_WARNING(QString("无法加载图片: %1").arg(imageSequence_[i]));
            continue;
        }

        // 记录开始时间
        auto startTime = std::chrono::high_resolution_clock::now();

        // 处理图片
        processImage(currentImage_);

        // 计算处理时间
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = endTime - startTime;
        totalTime += duration.count();
        successCount++;

        // 更新状态
        statusLabel_->setText(QString("批量处理中: %1/%2").arg(i + 1).arg(totalImages));
        QApplication::processEvents();
    }

    // 显示完成信息
    QString resultMsg = QString("批量处理完成!\n"
                                "处理图片: %1/%2 张\n"
                                "总耗时: %3 ms\n"
                                "平均耗时: %4 ms/张")
        .arg(successCount)
        .arg(totalImages)
        .arg(totalTime, 0, 'f', 2)
        .arg(successCount > 0 ? totalTime / successCount : 0, 0, 'f', 2);

    QMessageBox::information(this, "批量处理完成", resultMsg);
    statusLabel_->setText(QString("批量处理完成: %1张图片, 总耗时%.2f ms")
        .arg(successCount).arg(totalTime));

    updateImageSequenceActions();

    LOG_INFO(QString("批量处理完成: %1/%2张图片, 总耗时%3ms")
        .arg(successCount).arg(totalImages).arg(totalTime, 0, 'f', 2));
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

    addToolAction_ = new QAction(Theme::getIcon(Icons::TOOL_ADD), "添加工具(&A)...", this);
    addToolAction_->setStatusTip("添加新的视觉处理工具");
    connect(addToolAction_, &QAction::triggered, this, &MainWindow::onAddTool);
    toolMenu_->addAction(addToolAction_);

    removeToolAction_ = new QAction(Theme::getIcon(Icons::TOOL_REMOVE), "删除工具(&R)", this);
    removeToolAction_->setStatusTip("删除选中的工具");
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

    grabImageAction_ = new QAction(Theme::getIcon(Icons::CAMERA_PHOTO), "采集单帧(&G)", this);
    grabImageAction_->setShortcut(Qt::Key_F7);
    grabImageAction_->setStatusTip("采集单帧图像（F7）");
    connect(grabImageAction_, &QAction::triggered, this, &MainWindow::onGrabImage);
    cameraMenu_->addAction(grabImageAction_);

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

    // 帮助菜单
    helpMenu_ = menuBar()->addMenu("帮助(&H)");

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

    // 默认隐藏所有侧边面板，最大化Halcon窗口显示空间
    toolChainDock_->hide();
    toolParameterDock_->hide();
    resultTableDock_->hide();
    historyDock_->hide();
    recipeDock_->hide();

    // 设置停靠窗口的初始大小（当显示时使用）
    resizeDocks({toolChainDock_}, {250}, Qt::Horizontal);
    resizeDocks({toolParameterDock_}, {250}, Qt::Horizontal);
    resizeDocks({resultTableDock_}, {200}, Qt::Vertical);
}

void MainWindow::createStatusBar()
{
    statusLabel_ = new QLabel("就绪", this);
    statusBar()->addWidget(statusLabel_, 1);

    imageInfoLabel_ = new QLabel("图像: --", this);
    statusBar()->addPermanentWidget(imageInfoLabel_);

    scaleLabel_ = new QLabel("缩放: 100%", this);
    statusBar()->addPermanentWidget(scaleLabel_);

    positionLabel_ = new QLabel("位置: --", this);
    statusBar()->addPermanentWidget(positionLabel_);
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
        dialog.setCamera(camera_);
    }

    if (dialog.exec() == QDialog::Accepted) {
        // 获取选择的相机
        HAL::ICamera* newCamera = dialog.takeCamera();
        if (newCamera && newCamera != camera_) {
            // 关闭旧相机
            if (camera_) {
                camera_->close();
                delete camera_;
            }
            // 直接赋值，camera_已改为ICamera*类型，支持所有相机类型
            camera_ = newCamera;
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

// ========== 系统设置 ==========

void MainWindow::onSystemSettings()
{
    SystemSettingsDialog dialog(this);
    dialog.exec();
}

} // namespace UI
} // namespace VisionForge
