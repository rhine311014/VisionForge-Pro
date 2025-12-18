/**
 * @file ImageViewer.cpp
 * @brief 图像显示组件实现
 */

#include "ui/ImageViewer.h"
#include "base/Logger.h"
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <algorithm>

namespace VisionForge {
namespace UI {

ImageViewer::ImageViewer(QWidget* parent)
    : QWidget(parent)
    , currentImage_(nullptr)
    , scale_(1.0)
    , fitMode_(FitToWindow)
    , isPanning_(false)
    , offset_(0, 0)
    , interactionMode_(PanMode)
    , selectedROI_(nullptr)
    , currentROI_(nullptr)
    , roiColor_(Qt::green)
    , roiLineWidth_(2)
    , isDrawing_(false)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(100, 100);
}

void ImageViewer::setImage(const Base::ImageData::Ptr& image)
{
    if (!image || image->isEmpty()) {
        clearImage();
        return;
    }

    currentImage_ = image;
    updatePixmap();

    // 根据适应模式自动调整缩放
    if (fitMode_ == FitToWindow) {
        fitToWindow();
    } else if (fitMode_ == FitToWidth) {
        fitToWidth();
    } else if (fitMode_ == FitToHeight) {
        fitToHeight();
    }

    update();
}

void ImageViewer::clearImage()
{
    currentImage_.reset();
    displayPixmap_ = QPixmap();
    offset_ = QPoint(0, 0);
    update();
}

void ImageViewer::setScale(double scale)
{
    scale = std::clamp(scale, MIN_SCALE, MAX_SCALE);

    if (qAbs(scale_ - scale) > 0.001) {
        scale_ = scale;
        fitMode_ = NoFit;  // 手动缩放时取消自动适应
        emit scaleChanged(scale_);
        update();
    }
}

void ImageViewer::fitToWindow()
{
    if (!currentImage_) {
        return;
    }

    fitMode_ = FitToWindow;
    scale_ = calculateFitScale();
    offset_ = QPoint(0, 0);

    emit scaleChanged(scale_);
    update();
}

void ImageViewer::fitToWidth()
{
    if (!currentImage_) {
        return;
    }

    fitMode_ = FitToWidth;
    scale_ = calculateFitWidthScale();
    offset_.setX(0);

    emit scaleChanged(scale_);
    update();
}

void ImageViewer::fitToHeight()
{
    if (!currentImage_) {
        return;
    }

    fitMode_ = FitToHeight;
    scale_ = calculateFitHeightScale();
    offset_.setY(0);

    emit scaleChanged(scale_);
    update();
}

void ImageViewer::actualSize()
{
    setScale(1.0);
}

void ImageViewer::zoomIn()
{
    setScale(scale_ * ZOOM_STEP);
}

void ImageViewer::zoomOut()
{
    setScale(scale_ / ZOOM_STEP);
}

void ImageViewer::setFitMode(FitMode mode)
{
    if (fitMode_ != mode) {
        fitMode_ = mode;

        if (currentImage_) {
            if (mode == FitToWindow) {
                fitToWindow();
            } else if (mode == FitToWidth) {
                fitToWidth();
            } else if (mode == FitToHeight) {
                fitToHeight();
            }
        }
    }
}

void ImageViewer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::darkGray);

    if (displayPixmap_.isNull()) {
        // 显示提示文本
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "无图像");
        return;
    }

    // 计算绘制区域
    int scaledWidth = static_cast<int>(displayPixmap_.width() * scale_);
    int scaledHeight = static_cast<int>(displayPixmap_.height() * scale_);

    // 居中显示
    int x = (width() - scaledWidth) / 2 + offset_.x();
    int y = (height() - scaledHeight) / 2 + offset_.y();

    QRect targetRect(x, y, scaledWidth, scaledHeight);

    // 绘制图像
    painter.setRenderHint(QPainter::SmoothPixmapTransform, scale_ < 1.0);
    painter.drawPixmap(targetRect, displayPixmap_);

    // 绘制边框
    painter.setPen(Qt::white);
    painter.drawRect(targetRect);

    // 绘制ROI
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawROIs(painter);
}

void ImageViewer::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    // 如果是自动适应模式，重新计算缩放
    if (currentImage_) {
        if (fitMode_ == FitToWindow) {
            scale_ = calculateFitScale();
            offset_ = QPoint(0, 0);
            emit scaleChanged(scale_);
        } else if (fitMode_ == FitToWidth) {
            scale_ = calculateFitWidthScale();
            offset_.setX(0);
            emit scaleChanged(scale_);
        } else if (fitMode_ == FitToHeight) {
            scale_ = calculateFitHeightScale();
            offset_.setY(0);
            emit scaleChanged(scale_);
        }
    }
}

void ImageViewer::wheelEvent(QWheelEvent* event)
{
    if (!currentImage_) {
        return;
    }

    // 鼠标滚轮缩放
    double delta = event->angleDelta().y() / 120.0;
    double factor = qPow(ZOOM_STEP, delta);

    // 以鼠标位置为中心缩放
    QPoint mousePos = event->position().toPoint();
    QPoint imagePos = widgetToImage(mousePos);

    double newScale = scale_ * factor;
    newScale = std::clamp(newScale, MIN_SCALE, MAX_SCALE);

    if (qAbs(newScale - scale_) > 0.001) {
        // 调整偏移量，使鼠标位置保持不变
        double scaleFactor = newScale / scale_;
        offset_ = mousePos - (mousePos - offset_) * scaleFactor;

        scale_ = newScale;
        fitMode_ = NoFit;

        emit scaleChanged(scale_);
        update();
    }

    event->accept();
}

void ImageViewer::mousePressEvent(QMouseEvent* event)
{
    if (!currentImage_) {
        QWidget::mousePressEvent(event);
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
            // 选择模式
            ROIShapePtr roi = findROIAt(imagePos);
            if (selectedROI_) {
                selectedROI_->setSelected(false);
            }
            selectedROI_ = roi;
            if (selectedROI_) {
                selectedROI_->setSelected(true);
                emit roiSelectionChanged(selectedROI_);
            }
            update();
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
            case DrawPolygon:
                currentROI_ = std::make_shared<ROIPolygon>();
                std::dynamic_pointer_cast<ROIPolygon>(currentROI_)->addPoint(imagePos);
                break;
            case DrawLine:
                currentROI_ = std::make_shared<ROILine>();
                break;
            case DrawPoint:
                currentROI_ = std::make_shared<ROIPoint>(imagePos);
                finishCurrentROI();
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
        // 右键完成多边形绘制
        if (interactionMode_ == DrawPolygon && isDrawing_) {
            finishCurrentROI();
        }
    }

    QWidget::mousePressEvent(event);
}

void ImageViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (currentImage_) {
        // 更新鼠标位置（图像坐标）
        QPoint imagePos = widgetToImage(event->pos());
        bool valid = (imagePos.x() >= 0 && imagePos.x() < currentImage_->width() &&
                     imagePos.y() >= 0 && imagePos.y() < currentImage_->height());
        emit mousePositionChanged(imagePos.x(), imagePos.y(), valid);

        // 拖拽平移
        if (isPanning_) {
            QPoint delta = event->pos() - lastMousePos_;
            offset_ += delta;
            lastMousePos_ = event->pos();
            update();
        }
        // 绘制ROI
        else if (isDrawing_ && currentROI_) {
            switch (interactionMode_) {
            case DrawRectangle: {
                auto rect = std::dynamic_pointer_cast<ROIRectangle>(currentROI_);
                int left = std::min(roiStartPoint_.x(), imagePos.x());
                int top = std::min(roiStartPoint_.y(), imagePos.y());
                int width = std::abs(imagePos.x() - roiStartPoint_.x());
                int height = std::abs(imagePos.y() - roiStartPoint_.y());
                rect->setRect(QRect(left, top, width, height));
                break;
            }
            case DrawCircle: {
                auto circle = std::dynamic_pointer_cast<ROICircle>(currentROI_);
                int dx = imagePos.x() - roiStartPoint_.x();
                int dy = imagePos.y() - roiStartPoint_.y();
                int radius = static_cast<int>(std::sqrt(dx * dx + dy * dy));
                circle->setCenter(roiStartPoint_);
                circle->setRadius(radius);
                break;
            }
            case DrawEllipse: {
                auto ellipse = std::dynamic_pointer_cast<ROIEllipse>(currentROI_);
                int left = std::min(roiStartPoint_.x(), imagePos.x());
                int top = std::min(roiStartPoint_.y(), imagePos.y());
                int width = std::abs(imagePos.x() - roiStartPoint_.x());
                int height = std::abs(imagePos.y() - roiStartPoint_.y());
                ellipse->setRect(QRect(left, top, width, height));
                break;
            }
            case DrawLine: {
                auto line = std::dynamic_pointer_cast<ROILine>(currentROI_);
                line->setStart(roiStartPoint_);
                line->setEnd(imagePos);
                break;
            }
            default:
                break;
            }
            update();
        }
    }

    QWidget::mouseMoveEvent(event);
}

void ImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint imagePos = widgetToImage(event->pos());

        if (isPanning_) {
            isPanning_ = false;
            setCursor(Qt::ArrowCursor);
        }
        else if (isDrawing_ && currentROI_) {
            // 完成ROI绘制（多边形除外，多边形需要右键完成）
            if (interactionMode_ != DrawPolygon) {
                finishCurrentROI();
            }
            else {
                // 多边形：添加点
                auto polygon = std::dynamic_pointer_cast<ROIPolygon>(currentROI_);
                polygon->addPoint(imagePos);
                update();
            }
        }

        // 发出鼠标点击信号（用于九点标定等功能）
        if (currentImage_) {
            bool valid = (imagePos.x() >= 0 && imagePos.x() < currentImage_->width() &&
                         imagePos.y() >= 0 && imagePos.y() < currentImage_->height());
            if (valid) {
                emit mouseClicked(imagePos.x(), imagePos.y());
            }
        }
    }

    QWidget::mouseReleaseEvent(event);
}

// ========== 私有方法 ==========

void ImageViewer::updatePixmap()
{
    if (!currentImage_ || currentImage_->isEmpty()) {
        displayPixmap_ = QPixmap();
        return;
    }

    // 将ImageData转换为QPixmap
    QImage qimage = currentImage_->toQImage();
    displayPixmap_ = QPixmap::fromImage(qimage);
}

double ImageViewer::calculateFitScale() const
{
    if (!currentImage_) {
        return 1.0;
    }

    double scaleX = static_cast<double>(width()) / currentImage_->width();
    double scaleY = static_cast<double>(height()) / currentImage_->height();

    return std::min(scaleX, scaleY) * 0.95;  // 留5%边距
}

double ImageViewer::calculateFitWidthScale() const
{
    if (!currentImage_) {
        return 1.0;
    }

    return static_cast<double>(width()) / currentImage_->width() * 0.95;
}

double ImageViewer::calculateFitHeightScale() const
{
    if (!currentImage_) {
        return 1.0;
    }

    return static_cast<double>(height()) / currentImage_->height() * 0.95;
}

QPoint ImageViewer::widgetToImage(const QPoint& pos) const
{
    if (!currentImage_) {
        return QPoint(-1, -1);
    }

    int scaledWidth = static_cast<int>(displayPixmap_.width() * scale_);
    int scaledHeight = static_cast<int>(displayPixmap_.height() * scale_);

    int x = (width() - scaledWidth) / 2 + offset_.x();
    int y = (height() - scaledHeight) / 2 + offset_.y();

    // 将窗口坐标转换为图像坐标
    int imageX = static_cast<int>((pos.x() - x) / scale_);
    int imageY = static_cast<int>((pos.y() - y) / scale_);

    return QPoint(imageX, imageY);
}

QPoint ImageViewer::imageToWidget(const QPoint& pos) const
{
    if (!currentImage_) {
        return QPoint(-1, -1);
    }

    int scaledWidth = static_cast<int>(displayPixmap_.width() * scale_);
    int scaledHeight = static_cast<int>(displayPixmap_.height() * scale_);

    int x = (width() - scaledWidth) / 2 + offset_.x();
    int y = (height() - scaledHeight) / 2 + offset_.y();

    // 将图像坐标转换为窗口坐标
    int widgetX = static_cast<int>(pos.x() * scale_) + x;
    int widgetY = static_cast<int>(pos.y() * scale_) + y;

    return QPoint(widgetX, widgetY);
}

// ========== ROI相关方法 ==========

void ImageViewer::setInteractionMode(InteractionMode mode)
{
    interactionMode_ = mode;
    isDrawing_ = false;
    currentROI_.reset();

    if (mode == PanMode) {
        setCursor(Qt::ArrowCursor);
    }
    else if (mode == SelectMode) {
        setCursor(Qt::ArrowCursor);
    }
    else {
        setCursor(Qt::CrossCursor);
    }
}

void ImageViewer::addROI(const ROIShapePtr& roi)
{
    if (roi) {
        rois_.push_back(roi);
        update();
    }
}

void ImageViewer::removeROI(const ROIShapePtr& roi)
{
    auto it = std::find(rois_.begin(), rois_.end(), roi);
    if (it != rois_.end()) {
        if (selectedROI_ == roi) {
            selectedROI_.reset();
        }
        rois_.erase(it);
        emit roiRemoved(roi);
        update();
    }
}

void ImageViewer::clearROIs()
{
    rois_.clear();
    selectedROI_.reset();
    currentROI_.reset();
    update();
}

void ImageViewer::drawROIs(QPainter& painter)
{
    // 绘制已有的ROI
    for (const auto& roi : rois_) {
        drawROI(painter, roi);
    }

    // 绘制正在绘制的ROI
    if (currentROI_) {
        drawROI(painter, currentROI_);
    }
}

void ImageViewer::drawROI(QPainter& painter, const ROIShapePtr& roi)
{
    if (!roi) return;

    QColor color = roi->getColor();
    int lineWidth = roi->getLineWidth();

    // 选中时使用高亮颜色
    if (roi->isSelected()) {
        color = Qt::yellow;
        lineWidth = lineWidth + 1;
    }

    QPen pen(color, lineWidth);
    painter.setPen(pen);

    switch (roi->getType()) {
    case ROIType::Rectangle: {
        auto rect = std::dynamic_pointer_cast<ROIRectangle>(roi);
        QRect imageRect = rect->getRect();
        QPoint topLeft = imageToWidget(imageRect.topLeft());
        QPoint bottomRight = imageToWidget(imageRect.bottomRight());
        painter.drawRect(QRect(topLeft, bottomRight));
        break;
    }
    case ROIType::Circle: {
        auto circle = std::dynamic_pointer_cast<ROICircle>(roi);
        QPoint center = imageToWidget(circle->getCenter());
        int radius = static_cast<int>(circle->getRadius() * scale_);
        painter.drawEllipse(center, radius, radius);
        break;
    }
    case ROIType::Ellipse: {
        auto ellipse = std::dynamic_pointer_cast<ROIEllipse>(roi);
        QRect imageRect = ellipse->getRect();
        QPoint topLeft = imageToWidget(imageRect.topLeft());
        QPoint bottomRight = imageToWidget(imageRect.bottomRight());
        painter.drawEllipse(QRect(topLeft, bottomRight));
        break;
    }
    case ROIType::Polygon: {
        auto polygon = std::dynamic_pointer_cast<ROIPolygon>(roi);
        QPolygon imagePolygon = polygon->getPolygon();
        QPolygon widgetPolygon;
        for (int i = 0; i < imagePolygon.size(); ++i) {
            widgetPolygon.append(imageToWidget(imagePolygon[i]));
        }
        painter.drawPolygon(widgetPolygon);

        // 绘制顶点
        painter.setBrush(color);
        for (const QPoint& p : widgetPolygon) {
            painter.drawEllipse(p, 3, 3);
        }
        painter.setBrush(Qt::NoBrush);
        break;
    }
    case ROIType::Line: {
        auto line = std::dynamic_pointer_cast<ROILine>(roi);
        QPoint start = imageToWidget(line->getStart());
        QPoint end = imageToWidget(line->getEnd());
        painter.drawLine(start, end);
        break;
    }
    case ROIType::Point: {
        auto point = std::dynamic_pointer_cast<ROIPoint>(roi);
        QPoint p = imageToWidget(point->getPoint());
        painter.setBrush(color);
        painter.drawEllipse(p, 5, 5);
        painter.setBrush(Qt::NoBrush);
        break;
    }
    }
}

ROIShapePtr ImageViewer::findROIAt(const QPoint& imagePos)
{
    // 从后向前查找（最后绘制的在最上层）
    for (auto it = rois_.rbegin(); it != rois_.rend(); ++it) {
        if ((*it)->contains(imagePos)) {
            return *it;
        }
    }
    return nullptr;
}

void ImageViewer::finishCurrentROI()
{
    if (currentROI_) {
        rois_.push_back(currentROI_);
        emit roiCreated(currentROI_);
        currentROI_.reset();
        isDrawing_ = false;
        update();
    }
}

} // namespace UI
} // namespace VisionForge
