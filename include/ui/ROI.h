/**
 * @file ROI.h
 * @brief ROI（感兴趣区域）定义
 * @details 定义各种ROI形状和操作
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include <QPoint>
#include <QRect>
#include <QPolygon>
#include <QColor>
#include <QString>
#include <memory>
#include <vector>

// 解决Halcon与STL的宏冲突
// Halcon定义了high、low、min、max作为宏，但STL使用它们作为函数名
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

/**
 * @enum ROIType
 * @brief ROI类型
 */
enum class ROIType {
    Rectangle,      // 矩形
    Circle,         // 圆形
    Ellipse,        // 椭圆
    Polygon,        // 多边形
    Line,           // 直线
    Point           // 点
};

/**
 * @class ROIShape
 * @brief ROI形状基类
 */
class ROIShape {
public:
    ROIShape(ROIType type) : type_(type), color_(Qt::green), lineWidth_(2), selected_(false) {}
    virtual ~ROIShape() = default;

    ROIType getType() const { return type_; }
    QColor getColor() const { return color_; }
    void setColor(const QColor& color) { color_ = color; }
    int getLineWidth() const { return lineWidth_; }
    void setLineWidth(int width) { lineWidth_ = width; }
    bool isSelected() const { return selected_; }
    void setSelected(bool selected) { selected_ = selected; }

    virtual bool contains(const QPoint& point) const = 0;
    virtual QRect boundingRect() const = 0;
    virtual void translate(const QPoint& offset) = 0;

protected:
    ROIType type_;
    QColor color_;
    int lineWidth_;
    bool selected_;
};

/**
 * @class ROIRectangle
 * @brief 矩形ROI
 */
class ROIRectangle : public ROIShape {
public:
    ROIRectangle() : ROIShape(ROIType::Rectangle) {}
    ROIRectangle(const QRect& rect) : ROIShape(ROIType::Rectangle), rect_(rect) {}

    QRect getRect() const { return rect_; }
    void setRect(const QRect& rect) { rect_ = rect; }

    bool contains(const QPoint& point) const override {
        return rect_.contains(point);
    }

    QRect boundingRect() const override {
        return rect_;
    }

    void translate(const QPoint& offset) override {
        rect_.translate(offset);
    }

private:
    QRect rect_;
};

/**
 * @class ROICircle
 * @brief 圆形ROI
 */
class ROICircle : public ROIShape {
public:
    ROICircle() : ROIShape(ROIType::Circle), center_(0, 0), radius_(0) {}
    ROICircle(const QPoint& center, int radius)
        : ROIShape(ROIType::Circle), center_(center), radius_(radius) {}

    QPoint getCenter() const { return center_; }
    void setCenter(const QPoint& center) { center_ = center; }
    int getRadius() const { return radius_; }
    void setRadius(int radius) { radius_ = radius; }

    bool contains(const QPoint& point) const override {
        int dx = point.x() - center_.x();
        int dy = point.y() - center_.y();
        return (dx * dx + dy * dy) <= (radius_ * radius_);
    }

    QRect boundingRect() const override {
        return QRect(center_.x() - radius_, center_.y() - radius_,
                     radius_ * 2, radius_ * 2);
    }

    void translate(const QPoint& offset) override {
        center_ += offset;
    }

private:
    QPoint center_;
    int radius_;
};

/**
 * @class ROIEllipse
 * @brief 椭圆ROI
 */
class ROIEllipse : public ROIShape {
public:
    ROIEllipse() : ROIShape(ROIType::Ellipse) {}
    ROIEllipse(const QRect& rect) : ROIShape(ROIType::Ellipse), rect_(rect) {}

    QRect getRect() const { return rect_; }
    void setRect(const QRect& rect) { rect_ = rect; }

    bool contains(const QPoint& point) const override {
        QPoint center = rect_.center();
        int rx = rect_.width() / 2;
        int ry = rect_.height() / 2;
        if (rx == 0 || ry == 0) return false;

        int dx = point.x() - center.x();
        int dy = point.y() - center.y();
        return ((dx * dx) / (rx * rx) + (dy * dy) / (ry * ry)) <= 1;
    }

    QRect boundingRect() const override {
        return rect_;
    }

    void translate(const QPoint& offset) override {
        rect_.translate(offset);
    }

private:
    QRect rect_;
};

/**
 * @class ROIPolygon
 * @brief 多边形ROI
 */
class ROIPolygon : public ROIShape {
public:
    ROIPolygon() : ROIShape(ROIType::Polygon) {}
    ROIPolygon(const QPolygon& polygon) : ROIShape(ROIType::Polygon), polygon_(polygon) {}

    QPolygon getPolygon() const { return polygon_; }
    void setPolygon(const QPolygon& polygon) { polygon_ = polygon; }
    void addPoint(const QPoint& point) { polygon_.append(point); }
    void clearPoints() { polygon_.clear(); }
    int pointCount() const { return polygon_.size(); }

    bool contains(const QPoint& point) const override {
        return polygon_.containsPoint(point, Qt::OddEvenFill);
    }

    QRect boundingRect() const override {
        return polygon_.boundingRect();
    }

    void translate(const QPoint& offset) override {
        polygon_.translate(offset);
    }

private:
    QPolygon polygon_;
};

/**
 * @class ROILine
 * @brief 直线ROI
 */
class ROILine : public ROIShape {
public:
    ROILine() : ROIShape(ROIType::Line) {}
    ROILine(const QPoint& start, const QPoint& end)
        : ROIShape(ROIType::Line), start_(start), end_(end) {}

    QPoint getStart() const { return start_; }
    void setStart(const QPoint& start) { start_ = start; }
    QPoint getEnd() const { return end_; }
    void setEnd(const QPoint& end) { end_ = end; }

    bool contains(const QPoint& point) const override {
        // 检测点是否在线段附近（5像素容差）
        const int tolerance = 5;

        // 计算点到线段的距离
        int x1 = start_.x(), y1 = start_.y();
        int x2 = end_.x(), y2 = end_.y();
        int x0 = point.x(), y0 = point.y();

        int dx = x2 - x1;
        int dy = y2 - y1;
        int len2 = dx * dx + dy * dy;
        if (len2 == 0) return false;

        double t = std::max(0.0, std::min(1.0,
            ((x0 - x1) * dx + (y0 - y1) * dy) / static_cast<double>(len2)));

        int projX = static_cast<int>(x1 + t * dx);
        int projY = static_cast<int>(y1 + t * dy);

        int dist2 = (x0 - projX) * (x0 - projX) + (y0 - projY) * (y0 - projY);
        return dist2 <= (tolerance * tolerance);
    }

    QRect boundingRect() const override {
        int left = std::min(start_.x(), end_.x());
        int top = std::min(start_.y(), end_.y());
        int right = std::max(start_.x(), end_.x());
        int bottom = std::max(start_.y(), end_.y());
        return QRect(left, top, right - left, bottom - top);
    }

    void translate(const QPoint& offset) override {
        start_ += offset;
        end_ += offset;
    }

private:
    QPoint start_;
    QPoint end_;
};

/**
 * @class ROIPoint
 * @brief 点ROI
 */
class ROIPoint : public ROIShape {
public:
    ROIPoint() : ROIShape(ROIType::Point) {}
    ROIPoint(const QPoint& point) : ROIShape(ROIType::Point), point_(point) {}

    QPoint getPoint() const { return point_; }
    void setPoint(const QPoint& point) { point_ = point; }

    bool contains(const QPoint& point) const override {
        const int tolerance = 5;
        int dx = point.x() - point_.x();
        int dy = point.y() - point_.y();
        return (dx * dx + dy * dy) <= (tolerance * tolerance);
    }

    QRect boundingRect() const override {
        return QRect(point_.x() - 5, point_.y() - 5, 10, 10);
    }

    void translate(const QPoint& offset) override {
        point_ += offset;
    }

private:
    QPoint point_;
};

// ROI智能指针类型定义
using ROIShapePtr = std::shared_ptr<ROIShape>;

} // namespace UI
} // namespace VisionForge
