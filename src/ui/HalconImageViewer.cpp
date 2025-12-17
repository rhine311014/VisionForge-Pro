/**
 * @file HalconImageViewer.cpp
 * @brief Halcon图像显示组件实现
 */

#include "ui/HalconImageViewer.h"
#include "ui/HalconUtils.h"
#include "base/Logger.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QShowEvent>
#include <QTimer>
#include <cmath>

// 解决Halcon与OpenCV/STL的宏冲突
// Halcon定义了high/low/min/max作为宏
#ifdef high
#undef high
#endif
#ifdef low
#undef low
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace VisionForge {
namespace UI {

HalconImageViewer::HalconImageViewer(QWidget* parent)
    : QWidget(parent)
    , halconWindowInitialized_(false)
    , initTimer_(nullptr)
    , displayWorker_(nullptr)
    , currentImage_(nullptr)
    , imageWidth_(0)
    , imageHeight_(0)
    , scale_(1.0)
    , fitMode_(FitToWindow)
    , partRow1_(0.0)
    , partCol1_(0.0)
    , partRow2_(-1.0)
    , partCol2_(-1.0)
    , isPanning_(false)
    , interactionMode_(PanMode)
    , selectedROI_(nullptr)
    , currentROI_(nullptr)
    , roiColor_(Qt::green)
    , roiLineWidth_(2)
    , isDrawing_(false)
    , isDrawingROI_(false)
{
    // 设置窗口属性
    setMinimumSize(200, 200);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    // 移除所有边距，确保Halcon窗口占据全部空间
    setContentsMargins(0, 0, 0, 0);

    // 设置尺寸策略为扩展，占据所有可用空间
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 设置背景色为黑色
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    // 创建显示工作线程
    displayWorker_ = new HalconDisplayWorker(this);
    connect(displayWorker_, &HalconDisplayWorker::imageDisplayed,
            this, &HalconImageViewer::onImageDisplayed);
    connect(displayWorker_, &HalconDisplayWorker::displayError,
            this, &HalconImageViewer::onDisplayError);
    displayWorker_->start();

    // 延迟初始化Halcon窗口
    initTimer_ = new QTimer(this);
    initTimer_->setSingleShot(true);
    connect(initTimer_, &QTimer::timeout, this, &HalconImageViewer::onInitTimer);
}

HalconImageViewer::~HalconImageViewer()
{
    // 停止显示线程
    if (displayWorker_) {
        displayWorker_->stop();
        displayWorker_->wait();
        delete displayWorker_;
        displayWorker_ = nullptr;
    }

    closeHalconWindow();
}

void HalconImageViewer::setImage(const Base::ImageData::Ptr& image)
{
    if (!image || image->isEmpty()) {
        clearImage();
        return;
    }

    currentImage_ = image;
    imageWidth_ = image->width();
    imageHeight_ = image->height();

    // 如果是FitToWindow模式，重新计算缩放
    if (fitMode_ == FitToWindow) {
        scale_ = calculateFitScale();
    } else if (fitMode_ == FitToWidth) {
        scale_ = calculateFitWidthScale();
    } else if (fitMode_ == FitToHeight) {
        scale_ = calculateFitHeightScale();
    }

    updateImagePart();

    // 使用子线程显示图像（避免阻塞主线程）
    if (displayWorker_ && halconWindowInitialized_) {
        displayWorker_->requestDisplayImage(image);
    }

    LOG_DEBUG(QString("请求显示图像: %1x%2, 通道=%3")
        .arg(imageWidth_).arg(imageHeight_).arg(image->channels()));
}

void HalconImageViewer::clearImage()
{
    currentImage_ = nullptr;
    imageWidth_ = 0;
    imageHeight_ = 0;

    // 使用子线程清空窗口
    if (displayWorker_ && halconWindowInitialized_) {
        displayWorker_->requestClearWindow();
    }

    update();
}

#ifdef _WIN32
void HalconImageViewer::addXLDContour(const HXLDCont& contour, const QString& color)
{
    if (displayWorker_) {
        QList<HXLDCont> contours;
        contours.append(contour);
        QStringList colors;
        colors.append(color);
        displayWorker_->setXLDContours(contours, colors);
    }
}

void HalconImageViewer::setXLDContours(const QList<HXLDCont>& contours,
                                       const QStringList& colors)
{
    if (displayWorker_) {
        displayWorker_->setXLDContours(contours, colors);
    }
}

void HalconImageViewer::clearXLDContours()
{
    if (displayWorker_) {
        displayWorker_->clearXLDContours();
    }
}
#endif

void HalconImageViewer::setScale(double scale)
{
    scale = qBound(MIN_SCALE, scale, MAX_SCALE);
    if (qAbs(scale_ - scale) < 0.001) {
        return;
    }

    scale_ = scale;
    fitMode_ = NoFit;

    updateImagePart();

    // 重新显示图像以应用新的缩放
    if (currentImage_ && displayWorker_ && halconWindowInitialized_) {
        displayWorker_->requestDisplayImage(currentImage_);
    }

    emit scaleChanged(scale_);
}

void HalconImageViewer::fitToWindow()
{
    if (!currentImage_ || imageWidth_ <= 0 || imageHeight_ <= 0) {
        return;
    }

    scale_ = calculateFitScale();
    fitMode_ = FitToWindow;

    updateImagePart();

    // 重新显示图像以应用新的缩放
    if (displayWorker_ && halconWindowInitialized_) {
        displayWorker_->requestDisplayImage(currentImage_);
    }

    emit scaleChanged(scale_);
}

void HalconImageViewer::fitToWidth()
{
    if (!currentImage_ || imageWidth_ <= 0 || imageHeight_ <= 0) {
        return;
    }

    scale_ = calculateFitWidthScale();
    fitMode_ = FitToWidth;

    updateImagePart();

    // 重新显示图像以应用新的缩放
    if (displayWorker_ && halconWindowInitialized_) {
        displayWorker_->requestDisplayImage(currentImage_);
    }

    emit scaleChanged(scale_);
}

void HalconImageViewer::fitToHeight()
{
    if (!currentImage_ || imageWidth_ <= 0 || imageHeight_ <= 0) {
        return;
    }

    scale_ = calculateFitHeightScale();
    fitMode_ = FitToHeight;

    updateImagePart();

    // 重新显示图像以应用新的缩放
    if (displayWorker_ && halconWindowInitialized_) {
        displayWorker_->requestDisplayImage(currentImage_);
    }

    emit scaleChanged(scale_);
}

void HalconImageViewer::actualSize()
{
    setScale(1.0);
}

void HalconImageViewer::zoomIn()
{
    setScale(scale_ * ZOOM_STEP);
}

void HalconImageViewer::zoomOut()
{
    setScale(scale_ / ZOOM_STEP);
}

void HalconImageViewer::setFitMode(FitMode mode)
{
    fitMode_ = mode;

    switch (mode) {
    case FitToWindow:
        fitToWindow();
        break;
    case FitToWidth:
        fitToWidth();
        break;
    case FitToHeight:
        fitToHeight();
        break;
    default:
        break;
    }
}

void HalconImageViewer::setInteractionMode(InteractionMode mode)
{
    LOG_DEBUG(QString("设置交互模式: %1, currentImage_=%2")
        .arg(static_cast<int>(mode))
        .arg(currentImage_ ? "有效" : "空"));

    interactionMode_ = mode;

    // 切换模式时取消当前绘制
    if (isDrawing_) {
        isDrawing_ = false;
        currentROI_ = nullptr;
        updateDisplay();
    }
}

void HalconImageViewer::addROI(const ROIShapePtr& roi)
{
    if (!roi) {
        return;
    }

    rois_.push_back(roi);
    updateDisplay();
}

void HalconImageViewer::removeROI(const ROIShapePtr& roi)
{
    auto it = std::find(rois_.begin(), rois_.end(), roi);
    if (it != rois_.end()) {
        rois_.erase(it);

        if (selectedROI_ == roi) {
            selectedROI_ = nullptr;
        }

        updateDisplay();
        emit roiRemoved(roi);
    }
}

void HalconImageViewer::clearROIs()
{
    rois_.clear();
    selectedROI_ = nullptr;
    currentROI_ = nullptr;
    isDrawing_ = false;
    updateDisplay();
}

void HalconImageViewer::setROIColor(const QColor& color)
{
    roiColor_ = color;
    updateDisplay();
}

void HalconImageViewer::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    // 延迟100ms初始化Halcon窗口
    if (!halconWindowInitialized_) {
        initTimer_->start(100);
    }
}

void HalconImageViewer::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

#ifdef _WIN32
    if (halconWindowInitialized_ && windowHandle_.Length() > 0) {
        try {
            // 获取设备像素比，处理High DPI显示
            qreal dpr = devicePixelRatio();
            int scaledWidth = static_cast<int>(width() * dpr);
            int scaledHeight = static_cast<int>(height() * dpr);

            // 重新设置窗口大小（使用缩放后的尺寸）
            SetWindowExtents(windowHandle_, 0, 0, scaledWidth, scaledHeight);

            LOG_DEBUG(QString("Halcon窗口大小已调整: %1x%2 (DPR: %3)")
                .arg(scaledWidth).arg(scaledHeight).arg(dpr));

            // 如果是自动适应模式，重新计算缩放
            if (fitMode_ == FitToWindow) {
                fitToWindow();
            } else if (fitMode_ == FitToWidth) {
                fitToWidth();
            } else if (fitMode_ == FitToHeight) {
                fitToHeight();
            } else {
                updateImagePart();
                updateDisplay();
            }
        }
        catch (HException& e) {
            LOG_ERROR(QString("调整Halcon窗口大小失败: %1").arg(e.ErrorMessage().Text()));
        }
    }
#endif
}

void HalconImageViewer::wheelEvent(QWheelEvent* event)
{
    if (!currentImage_) {
        return;
    }

    // 获取鼠标位置（图像坐标）
    QPoint mousePos = event->position().toPoint();
    QPoint imagePosBeforeZoom = widgetToImage(mousePos);

    // 缩放
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }

    // 调整显示区域，使鼠标位置对应的图像点保持不变
    QPoint imagePosAfterZoom = widgetToImage(mousePos);
    double rowOffset = imagePosBeforeZoom.y() - imagePosAfterZoom.y();
    double colOffset = imagePosBeforeZoom.x() - imagePosAfterZoom.x();

    partRow1_ += rowOffset;
    partRow2_ += rowOffset;
    partCol1_ += colOffset;
    partCol2_ += colOffset;

    updateImagePart();
    updateDisplay();

    event->accept();
}

void HalconImageViewer::mousePressEvent(QMouseEvent* event)
{
    LOG_DEBUG(QString("鼠标按下: button=%1, interactionMode_=%2, currentImage_=%3")
        .arg(static_cast<int>(event->button()))
        .arg(static_cast<int>(interactionMode_))
        .arg(currentImage_ ? "有效" : "空"));

    if (!currentImage_) {
        LOG_DEBUG("mousePressEvent: currentImage_为空，忽略事件");
        return;
    }

    QPoint imagePos = widgetToImage(event->pos());

    if (event->button() == Qt::LeftButton) {
        if (interactionMode_ == PanMode) {
            // 平移模式
            isPanning_ = true;
            lastMousePos_ = event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
        else if (interactionMode_ == SelectMode) {
            // 选择模式：查找点击的ROI
            ROIShapePtr roi = findROIAt(imagePos);
            if (roi != selectedROI_) {
                selectedROI_ = roi;
                updateDisplay();
                emit roiSelectionChanged(selectedROI_);
            }
        }
        else {
            // 绘制模式
            isDrawing_ = true;
            roiStartPoint_ = imagePos;

            // 创建对应类型的ROI
            switch (interactionMode_) {
            case DrawRectangle:
                currentROI_ = std::make_shared<ROIRectangle>();
                break;
            case DrawCircle:
                currentROI_ = std::make_shared<ROICircle>();
                break;
            case DrawEllipse:
                currentROI_ = std::make_shared<ROIEllipse>();
                break;
            case DrawLine:
                currentROI_ = std::make_shared<ROILine>();
                break;
            case DrawPoint:
                currentROI_ = std::make_shared<ROIPoint>();
                break;
            case DrawPolygon:
                // 多边形需要多次点击
                if (!currentROI_) {
                    currentROI_ = std::make_shared<ROIPolygon>();
                }
                {
                    auto polygon = std::dynamic_pointer_cast<ROIPolygon>(currentROI_);
                    polygon->addPoint(imagePos);
                    updateDisplay();
                }
                return;  // 多边形不在这里完成
            case DrawFreehand:
                // 自由绘制模式：创建ROIFreehand并添加起始点
                currentROI_ = std::make_shared<ROIFreehand>();
                {
                    auto freehand = std::dynamic_pointer_cast<ROIFreehand>(currentROI_);
                    freehand->addPoint(imagePos);
                }
                break;
            default:
                break;
            }

            if (currentROI_) {
                currentROI_->setColor(roiColor_);
                currentROI_->setLineWidth(roiLineWidth_);
            }
        }
    }
    else if (event->button() == Qt::RightButton) {
        // 右键：完成多边形绘制
        if (interactionMode_ == DrawPolygon && currentROI_) {
            finishCurrentROI();
        }
        else if (interactionMode_ == PanMode) {
            // 平移模式下右键重置视图
            fitToWindow();
        }
    }

    event->accept();
}

void HalconImageViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (!currentImage_) {
        return;
    }

    QPoint imagePos = widgetToImage(event->pos());

    // 发送鼠标位置信号
    bool valid = (imagePos.x() >= 0 && imagePos.x() < imageWidth_ &&
                  imagePos.y() >= 0 && imagePos.y() < imageHeight_);
    emit mousePositionChanged(imagePos.x(), imagePos.y(), valid);

    if (isPanning_) {
        // 平移图像
        QPoint delta = event->pos() - lastMousePos_;
        lastMousePos_ = event->pos();

        // 转换为图像坐标的偏移
        double imageScale = 1.0 / scale_;
        double rowOffset = -delta.y() * imageScale;
        double colOffset = -delta.x() * imageScale;

        partRow1_ += rowOffset;
        partRow2_ += rowOffset;
        partCol1_ += colOffset;
        partCol2_ += colOffset;

        updateImagePart();
        updateDisplay();
    }
    else if (isDrawing_ && currentROI_) {
        // 更新正在绘制的ROI
        switch (interactionMode_) {
        case DrawRectangle: {
            auto rect = std::dynamic_pointer_cast<ROIRectangle>(currentROI_);
            QRect r(roiStartPoint_, imagePos);
            rect->setRect(r.normalized());
            break;
        }
        case DrawCircle: {
            auto circle = std::dynamic_pointer_cast<ROICircle>(currentROI_);
            double dx = imagePos.x() - roiStartPoint_.x();
            double dy = imagePos.y() - roiStartPoint_.y();
            double radius = std::sqrt(dx * dx + dy * dy);
            circle->setCenter(roiStartPoint_);
            circle->setRadius(radius);
            break;
        }
        case DrawEllipse: {
            auto ellipse = std::dynamic_pointer_cast<ROIEllipse>(currentROI_);
            QRect r(roiStartPoint_, imagePos);
            r = r.normalized();
            ellipse->setRect(r);
            break;
        }
        case DrawLine: {
            auto line = std::dynamic_pointer_cast<ROILine>(currentROI_);
            line->setStart(roiStartPoint_);
            line->setEnd(imagePos);
            break;
        }
        case DrawPoint: {
            auto point = std::dynamic_pointer_cast<ROIPoint>(currentROI_);
            point->setPoint(imagePos);
            break;
        }
        case DrawFreehand: {
            // 自由绘制：持续添加点（带距离过滤避免点过密）
            auto freehand = std::dynamic_pointer_cast<ROIFreehand>(currentROI_);
            if (freehand && freehand->pointCount() > 0) {
                QPoint lastPt = freehand->lastPoint();
                // 曼哈顿距离大于2像素才添加新点（避免点过密）
                int dist = std::abs(imagePos.x() - lastPt.x()) + std::abs(imagePos.y() - lastPt.y());
                if (dist > 2) {
                    freehand->addPoint(imagePos);
                }
            }
            break;
        }
        default:
            break;
        }

        updateDisplay();
    }

    event->accept();
}

void HalconImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        if (isPanning_) {
            isPanning_ = false;
            setCursor(Qt::ArrowCursor);
        }
        else if (isDrawing_ && currentROI_) {
            // 完成ROI绘制（除了多边形）
            if (interactionMode_ != DrawPolygon) {
                finishCurrentROI();
            }
        }
    }

    event->accept();
}

void HalconImageViewer::paintEvent(QPaintEvent* event)
{
    // 未初始化时，显示提示信息
    if (!halconWindowInitialized_) {
        QWidget::paintEvent(event);
        QPainter painter(this);
        painter.fillRect(rect(), Qt::black);
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "正在初始化Halcon窗口...");
        event->accept();
        return;
    }

    // Halcon窗口已初始化，完全不绘制，让Halcon自己处理
    // 不调用任何绘制函数，避免覆盖Halcon窗口
    event->accept();
}

void HalconImageViewer::onInitTimer()
{
    initHalconWindow();
}

void HalconImageViewer::onImageDisplayed()
{
    // 图像显示完成后，在主线程绘制ROI
    // 不使用mutex，因为这个函数已经在主线程中执行
    // 始终绘制ROI（包括正在绘制的currentROI_）
    drawROIs();
}

void HalconImageViewer::onDisplayError(const QString& errorMsg)
{
    LOG_ERROR(QString("显示错误: %1").arg(errorMsg));
}

void HalconImageViewer::initHalconWindow()
{
    if (halconWindowInitialized_) {
        return;
    }

#ifdef _WIN32
    try {
        LOG_DEBUG("开始初始化Halcon窗口...");

        // 检查窗口是否可见和有效尺寸
        if (!isVisible() || width() <= 0 || height() <= 0) {
            LOG_WARNING(QString("窗口尺寸无效或不可见，延迟初始化: %1x%2, visible=%3")
                .arg(width()).arg(height()).arg(isVisible()));
            return;
        }

        // 获取Qt窗口句柄
        LOG_DEBUG("获取Qt窗口句柄...");
        WId winId = this->winId();

        // 获取设备像素比，处理High DPI显示
        qreal dpr = devicePixelRatio();
        int scaledWidth = static_cast<int>(width() * dpr);
        int scaledHeight = static_cast<int>(height() * dpr);
        LOG_DEBUG(QString("窗口尺寸: %1x%2 (DPR: %3)").arg(scaledWidth).arg(scaledHeight).arg(dpr));

        // 创建Halcon窗口（使用缩放后的尺寸）
        LOG_DEBUG("调用OpenWindow创建Halcon窗口...");
        OpenWindow(0, 0, scaledWidth, scaledHeight, (Hlong)winId, "visible", "", &windowHandle_);
        LOG_DEBUG("OpenWindow调用成功");

        // 使用工具类初始化窗口配置（UTF-8、颜色模式、字体、线宽等）
        LOG_DEBUG("初始化Halcon窗口配置...");
        HalconUtils::InitializeWindow(windowHandle_);

        // 设置显示区域
        LOG_DEBUG("设置显示区域...");
        SetPart(windowHandle_, 0, 0, -1, -1);

        halconWindowInitialized_ = true;

        // 将窗口句柄传递给显示线程
        LOG_DEBUG("设置显示工作线程窗口句柄...");
        if (displayWorker_) {
            displayWorker_->setWindowHandle(windowHandle_);
        }

        LOG_INFO(QString("Halcon窗口初始化成功: %1x%2 (DPR: %3, 含UTF-8支持、字体配置)")
            .arg(scaledWidth).arg(scaledHeight).arg(dpr));

        // 触发重绘，移除"正在初始化"提示
        update();

        // 如果已经有图像，显示它
        if (currentImage_) {
            LOG_DEBUG("发送待显示图像到工作线程...");
            displayWorker_->requestDisplayImage(currentImage_);
        }
    }
    catch (HException& e) {
        LOG_ERROR(QString("初始化Halcon窗口失败: %1").arg(e.ErrorMessage().Text()));
        halconWindowInitialized_ = false;
        // 触发重绘，显示错误状态
        update();
    }
#endif
}

void HalconImageViewer::closeHalconWindow()
{
#ifdef _WIN32
    if (halconWindowInitialized_ && windowHandle_.Length() > 0) {
        try {
            CloseWindow(windowHandle_);
            windowHandle_.Clear();
            halconWindowInitialized_ = false;
        }
        catch (HException& e) {
            LOG_ERROR(QString("关闭Halcon窗口失败: %1").arg(e.ErrorMessage().Text()));
        }
    }
#endif
}

void HalconImageViewer::updateDisplay()
{
    // 请求显示线程重新显示图像
    // 图像显示完成后会触发onImageDisplayed()信号，在其中绘制ROI
    if (currentImage_ && displayWorker_ && halconWindowInitialized_) {
        displayWorker_->requestDisplayImage(currentImage_);
    }
}

void HalconImageViewer::updateImagePart()
{
#ifdef _WIN32
    if (!halconWindowInitialized_ || windowHandle_.Length() == 0 || !currentImage_) {
        return;
    }

    try {
        // 计算显示区域
        double centerRow = (partRow1_ + partRow2_) / 2.0;
        double centerCol = (partCol1_ + partCol2_) / 2.0;

        if (partRow2_ < 0 || partCol2_ < 0) {
            // 首次设置，居中显示
            centerRow = imageHeight_ / 2.0;
            centerCol = imageWidth_ / 2.0;
        }

        double halfHeight = height() / (2.0 * scale_);
        double halfWidth = width() / (2.0 * scale_);

        partRow1_ = centerRow - halfHeight;
        partRow2_ = centerRow + halfHeight;
        partCol1_ = centerCol - halfWidth;
        partCol2_ = centerCol + halfWidth;

        // 设置显示区域
        SetPart(windowHandle_, partRow1_, partCol1_, partRow2_, partCol2_);
    }
    catch (HException& e) {
        LOG_ERROR(QString("设置Halcon显示区域失败: %1").arg(e.ErrorMessage().Text()));
    }
#endif
}

// imageDataToHImage已移至HalconDisplayWorker

double HalconImageViewer::calculateFitScale() const
{
    if (imageWidth_ <= 0 || imageHeight_ <= 0) {
        return 1.0;
    }

    double scaleX = static_cast<double>(width()) / imageWidth_;
    double scaleY = static_cast<double>(height()) / imageHeight_;
    return std::min(scaleX, scaleY);
}

double HalconImageViewer::calculateFitWidthScale() const
{
    if (imageWidth_ <= 0) {
        return 1.0;
    }

    return static_cast<double>(width()) / imageWidth_;
}

double HalconImageViewer::calculateFitHeightScale() const
{
    if (imageHeight_ <= 0) {
        return 1.0;
    }

    return static_cast<double>(height()) / imageHeight_;
}

QPoint HalconImageViewer::widgetToImage(const QPoint& pos) const
{
    if (!currentImage_) {
        return QPoint(-1, -1);
    }

    // 从窗口坐标转换到图像坐标
    double imageScale = 1.0 / scale_;
    int imageX = static_cast<int>(partCol1_ + pos.x() * imageScale);
    int imageY = static_cast<int>(partRow1_ + pos.y() * imageScale);

    return QPoint(imageX, imageY);
}

QPoint HalconImageViewer::imageToWidget(const QPoint& pos) const
{
    if (!currentImage_) {
        return QPoint(-1, -1);
    }

    // 从图像坐标转换到窗口坐标
    int widgetX = static_cast<int>((pos.x() - partCol1_) * scale_);
    int widgetY = static_cast<int>((pos.y() - partRow1_) * scale_);

    return QPoint(widgetX, widgetY);
}

void HalconImageViewer::drawROIs()
{
#ifdef _WIN32
    if (!halconWindowInitialized_ || windowHandle_.Length() == 0) {
        return;
    }

    // 如果没有ROI需要绘制，直接返回
    if (rois_.empty() && !currentROI_) {
        return;
    }

    // 防止重入 - 如果正在绘制ROI，跳过
    if (isDrawingROI_) {
        LOG_DEBUG("drawROIs: 正在绘制中，跳过");
        return;
    }

    isDrawingROI_ = true;

    try {
        LOG_DEBUG(QString("drawROIs: 开始绘制, rois_数量=%1, currentROI_=%2")
            .arg(rois_.size()).arg(currentROI_ ? "有效" : "空"));

        // 绘制所有ROI
        for (const auto& roi : rois_) {
            if (roi) {
                drawROI(roi);
            }
        }

        // 绘制正在绘制的ROI
        if (isDrawing_ && currentROI_) {
            drawROI(currentROI_);
        }

        LOG_DEBUG("drawROIs: 绘制完成");
    }
    catch (HException& e) {
        LOG_ERROR(QString("绘制ROI失败: %1").arg(e.ErrorMessage().Text()));
    }
    catch (std::exception& e) {
        LOG_ERROR(QString("绘制ROI失败(std): %1").arg(e.what()));
    }
    catch (...) {
        LOG_ERROR("绘制ROI失败: 未知异常");
    }

    isDrawingROI_ = false;
#endif
}

void HalconImageViewer::drawROI(const ROIShapePtr& roi)
{
#ifdef _WIN32
    if (!roi || !halconWindowInitialized_ || windowHandle_.Length() == 0) {
        LOG_DEBUG("drawROI: 无效参数，跳过绘制");
        return;
    }

    try {
        LOG_DEBUG(QString("drawROI: 开始绘制ROI类型=%1").arg(static_cast<int>(roi->getType())));

        // 设置绘图属性
        QColor color = roi->getColor();
        if (roi == selectedROI_) {
            color = Qt::yellow;  // 选中的ROI用黄色
        }

        // 使用Halcon预定义颜色名以确保兼容性
        const char* colorName = "green";
        if (color == Qt::yellow) {
            colorName = "yellow";
        } else if (color == Qt::red) {
            colorName = "red";
        } else if (color == Qt::blue) {
            colorName = "blue";
        }

        SetColor(windowHandle_, colorName);
        SetLineWidth(windowHandle_, roi->getLineWidth());

        // 直接使用Halcon绘图函数，而不是通过HRegion
        switch (roi->getType()) {
        case ROIType::Rectangle: {
            auto rect = std::dynamic_pointer_cast<ROIRectangle>(roi);
            if (rect) {
                QRect r = rect->getRect();
                if (r.width() > 0 && r.height() > 0) {
                    LOG_DEBUG(QString("绘制矩形: row1=%1, col1=%2, row2=%3, col2=%4")
                        .arg(r.top()).arg(r.left()).arg(r.bottom()).arg(r.right()));
                    DispRectangle1(windowHandle_, r.top(), r.left(), r.bottom(), r.right());
                }
            }
            break;
        }
        case ROIType::Circle: {
            auto circle = std::dynamic_pointer_cast<ROICircle>(roi);
            if (circle && circle->getRadius() > 0) {
                QPoint center = circle->getCenter();
                DispCircle(windowHandle_, center.y(), center.x(), circle->getRadius());
            }
            break;
        }
        case ROIType::Ellipse: {
            auto ellipse = std::dynamic_pointer_cast<ROIEllipse>(roi);
            if (ellipse) {
                QRect r = ellipse->getRect();
                if (r.width() > 0 && r.height() > 0) {
                    QPoint center = r.center();
                    double rx = r.width() / 2.0;
                    double ry = r.height() / 2.0;
                    DispEllipse(windowHandle_, center.y(), center.x(), 0.0, ry, rx);
                }
            }
            break;
        }
        case ROIType::Line: {
            auto line = std::dynamic_pointer_cast<ROILine>(roi);
            if (line) {
                QPoint p1 = line->getStart();
                QPoint p2 = line->getEnd();
                if (p1 != p2) {
                    DispLine(windowHandle_, p1.y(), p1.x(), p2.y(), p2.x());
                }
            }
            break;
        }
        case ROIType::Point: {
            auto point = std::dynamic_pointer_cast<ROIPoint>(roi);
            if (point) {
                QPoint pos = point->getPoint();
                // 用十字表示点
                DispCross(windowHandle_, pos.y(), pos.x(), 10.0, 0.0);
            }
            break;
        }
        case ROIType::Polygon: {
            // 多边形使用HRegion方式
            HRegion region = roiToHRegion(roi);
            SetDraw(windowHandle_, "margin");
            DispObj(region, windowHandle_);
            break;
        }
        case ROIType::Freehand: {
            // 自由绘制轮廓：转换为XLD显示
            auto freehand = std::dynamic_pointer_cast<ROIFreehand>(roi);
            if (freehand && freehand->pointCount() >= 2) {
                QVector<QPoint> points = freehand->getPoints();
                HTuple rows, cols;
                for (const QPoint& pt : points) {
                    rows.Append(pt.y());
                    cols.Append(pt.x());
                }
                // 生成XLD轮廓（不闭合的多边形轮廓）
                HXLDCont contour;
                contour.GenContourPolygonXld(rows, cols);
                // 显示XLD轮廓
                DispObj(contour, windowHandle_);
            }
            break;
        }
        default:
            break;
        }

        LOG_DEBUG("drawROI: 绘制成功");
    }
    catch (HException& e) {
        LOG_ERROR(QString("绘制ROI失败: %1").arg(e.ErrorMessage().Text()));
    }
    catch (...) {
        LOG_ERROR("绘制ROI失败: 未知异常");
    }
#endif
}

HRegion HalconImageViewer::roiToHRegion(const ROIShapePtr& roi)
{
#ifdef _WIN32
    if (!roi) {
        return HRegion();
    }

    try {
        switch (roi->getType()) {
        case ROIType::Rectangle: {
            auto rect = std::dynamic_pointer_cast<ROIRectangle>(roi);
            if (!rect) return HRegion();
            QRect r = rect->getRect();
            // 验证矩形有效性
            if (r.width() <= 0 || r.height() <= 0) {
                return HRegion();
            }
            LOG_DEBUG(QString("创建矩形HRegion: top=%1, left=%2, bottom=%3, right=%4")
                .arg(r.top()).arg(r.left()).arg(r.bottom()).arg(r.right()));
            return HRegion(r.top(), r.left(), r.bottom(), r.right());
        }
        case ROIType::Circle: {
            auto circle = std::dynamic_pointer_cast<ROICircle>(roi);
            if (!circle) return HRegion();
            QPoint center = circle->getCenter();
            double radius = circle->getRadius();
            if (radius <= 0) return HRegion();
            return HRegion(center.y(), center.x(), radius);
        }
        case ROIType::Ellipse: {
            auto ellipse = std::dynamic_pointer_cast<ROIEllipse>(roi);
            if (!ellipse) return HRegion();
            QRect r = ellipse->getRect();
            if (r.width() <= 0 || r.height() <= 0) return HRegion();
            QPoint center = r.center();
            double rx = r.width() / 2.0;
            double ry = r.height() / 2.0;
            return HRegion(center.y(), center.x(), 0.0, ry, rx);
        }
        case ROIType::Polygon: {
            auto polygon = std::dynamic_pointer_cast<ROIPolygon>(roi);
            if (!polygon) return HRegion();
            QPolygon poly = polygon->getPolygon();

            if (poly.size() < 3) {
                return HRegion();
            }

            HTuple rows, cols;
            for (const QPoint& pt : poly) {
                rows.Append(pt.y());
                cols.Append(pt.x());
            }

            HRegion region;
            region.GenRegionPolygon(rows, cols);
            return region;
        }
        case ROIType::Line: {
            auto line = std::dynamic_pointer_cast<ROILine>(roi);
            if (!line) return HRegion();
            QPoint p1 = line->getStart();
            QPoint p2 = line->getEnd();
            if (p1 == p2) return HRegion();  // 无效线段

            // 用GenRegionLine生成线段区域
            HRegion region;
            region.GenRegionLine(p1.y(), p1.x(), p2.y(), p2.x());
            return region;
        }
        case ROIType::Point: {
            auto point = std::dynamic_pointer_cast<ROIPoint>(roi);
            if (!point) return HRegion();
            QPoint pos = point->getPoint();

            // 用一个小圆表示点
            return HRegion(pos.y(), pos.x(), 3.0);
        }
        default:
            return HRegion();
        }
    }
    catch (HException& e) {
        LOG_ERROR(QString("ROI转HRegion失败: %1").arg(e.ErrorMessage().Text()));
        return HRegion();
    }
#else
    return HRegion();
#endif
}

ROIShapePtr HalconImageViewer::findROIAt(const QPoint& imagePos)
{
    // 从后往前查找（后面的ROI绘制在上层）
    for (auto it = rois_.rbegin(); it != rois_.rend(); ++it) {
        if ((*it)->contains(imagePos)) {
            return *it;
        }
    }
    return nullptr;
}

void HalconImageViewer::finishCurrentROI()
{
    LOG_DEBUG(QString("finishCurrentROI: currentROI_=%1")
        .arg(currentROI_ ? "有效" : "空"));

    if (currentROI_) {
        // 验证ROI有效性
        bool valid = false;

        switch (currentROI_->getType()) {
        case ROIType::Rectangle: {
            auto rect = std::dynamic_pointer_cast<ROIRectangle>(currentROI_);
            valid = rect && rect->getRect().width() > 0 && rect->getRect().height() > 0;
            break;
        }
        case ROIType::Circle: {
            auto circle = std::dynamic_pointer_cast<ROICircle>(currentROI_);
            valid = circle && circle->getRadius() > 0;
            break;
        }
        case ROIType::Ellipse: {
            auto ellipse = std::dynamic_pointer_cast<ROIEllipse>(currentROI_);
            valid = ellipse && ellipse->getRect().width() > 0 && ellipse->getRect().height() > 0;
            break;
        }
        case ROIType::Polygon: {
            auto polygon = std::dynamic_pointer_cast<ROIPolygon>(currentROI_);
            valid = polygon && polygon->getPolygon().size() >= 3;
            break;
        }
        case ROIType::Line: {
            auto line = std::dynamic_pointer_cast<ROILine>(currentROI_);
            valid = line && line->getStart() != line->getEnd();
            break;
        }
        case ROIType::Point:
            valid = true;
            break;
        case ROIType::Freehand: {
            // 自由绘制轮廓：至少需要3个点才有效
            auto freehand = std::dynamic_pointer_cast<ROIFreehand>(currentROI_);
            valid = freehand && freehand->pointCount() >= 3;
            break;
        }
        }

        ROIShapePtr completedROI = currentROI_;
        currentROI_ = nullptr;
        isDrawing_ = false;

        if (valid) {
            rois_.push_back(completedROI);
            emit roiCreated(completedROI);
            LOG_DEBUG(QString("创建ROI: 类型=%1").arg(static_cast<int>(completedROI->getType())));
        }

        // 使用延迟调用来更新显示，避免与显示线程竞争
        QTimer::singleShot(50, this, [this]() {
            updateDisplay();
        });
    }
}

} // namespace UI
} // namespace VisionForge
