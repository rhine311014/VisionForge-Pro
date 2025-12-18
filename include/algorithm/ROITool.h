/**
 * @file ROITool.h
 * @brief ROI区域工具
 * @details 支持矩形、圆形、多边形等多种ROI类型
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QPolygonF>
#include <QRectF>
#include <QPointF>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief ROI形状基类
 */
class ROIShape {
public:
    /**
     * @brief ROI类型枚举
     */
    enum Type {
        Rectangle,      // 矩形
        Circle,         // 圆形
        Ellipse,        // 椭圆
        Polygon,        // 多边形
        RotatedRect     // 旋转矩形
    };

    explicit ROIShape(Type type) : type_(type), enabled_(true) {}
    virtual ~ROIShape() = default;

    /**
     * @brief 获取ROI类型
     */
    Type type() const { return type_; }

    /**
     * @brief 是否启用
     */
    bool isEnabled() const { return enabled_; }
    void setEnabled(bool enabled) { enabled_ = enabled; }

    /**
     * @brief 获取名称
     */
    QString name() const { return name_; }
    void setName(const QString& name) { name_ = name; }

    /**
     * @brief 获取外接矩形
     */
    virtual QRectF boundingRect() const = 0;

    /**
     * @brief 检查点是否在ROI内
     */
    virtual bool contains(const QPointF& point) const = 0;

    /**
     * @brief 创建OpenCV mask
     * @param imageSize 图像尺寸
     * @return CV_8UC1 mask图像
     */
    virtual cv::Mat createMask(const cv::Size& imageSize) const = 0;

    /**
     * @brief 序列化为JSON
     */
    virtual QJsonObject toJson() const = 0;

    /**
     * @brief 从JSON反序列化
     */
    static std::shared_ptr<ROIShape> fromJson(const QJsonObject& json);

    /**
     * @brief 平移ROI
     */
    virtual void translate(double dx, double dy) = 0;

    /**
     * @brief 缩放ROI
     */
    virtual void scale(double factor) = 0;

protected:
    Type type_;
    bool enabled_;
    QString name_;
};

/**
 * @brief 矩形ROI
 */
class RectROI : public ROIShape {
public:
    RectROI() : ROIShape(Rectangle), rect_(0, 0, 100, 100) {}
    RectROI(const QRectF& rect) : ROIShape(Rectangle), rect_(rect) {}
    RectROI(double x, double y, double width, double height)
        : ROIShape(Rectangle), rect_(x, y, width, height) {}

    QRectF rect() const { return rect_; }
    void setRect(const QRectF& rect) { rect_ = rect; }

    QRectF boundingRect() const override { return rect_; }
    bool contains(const QPointF& point) const override { return rect_.contains(point); }
    cv::Mat createMask(const cv::Size& imageSize) const override;
    QJsonObject toJson() const override;

    void translate(double dx, double dy) override { rect_.translate(dx, dy); }
    void scale(double factor) override {
        QPointF center = rect_.center();
        rect_.setWidth(rect_.width() * factor);
        rect_.setHeight(rect_.height() * factor);
        rect_.moveCenter(center);
    }

private:
    QRectF rect_;
};

/**
 * @brief 圆形ROI
 */
class CircleROI : public ROIShape {
public:
    CircleROI() : ROIShape(Circle), center_(50, 50), radius_(50) {}
    CircleROI(const QPointF& center, double radius)
        : ROIShape(Circle), center_(center), radius_(radius) {}
    CircleROI(double cx, double cy, double radius)
        : ROIShape(Circle), center_(cx, cy), radius_(radius) {}

    QPointF center() const { return center_; }
    void setCenter(const QPointF& center) { center_ = center; }

    double radius() const { return radius_; }
    void setRadius(double radius) { radius_ = radius; }

    QRectF boundingRect() const override {
        return QRectF(center_.x() - radius_, center_.y() - radius_,
                     radius_ * 2, radius_ * 2);
    }

    bool contains(const QPointF& point) const override {
        double dx = point.x() - center_.x();
        double dy = point.y() - center_.y();
        return (dx * dx + dy * dy) <= (radius_ * radius_);
    }

    cv::Mat createMask(const cv::Size& imageSize) const override;
    QJsonObject toJson() const override;

    void translate(double dx, double dy) override {
        center_.setX(center_.x() + dx);
        center_.setY(center_.y() + dy);
    }
    void scale(double factor) override { radius_ *= factor; }

private:
    QPointF center_;
    double radius_;
};

/**
 * @brief 椭圆ROI
 */
class EllipseROI : public ROIShape {
public:
    EllipseROI() : ROIShape(Ellipse), center_(50, 50), radiusX_(60), radiusY_(40), angle_(0) {}
    EllipseROI(const QPointF& center, double rx, double ry, double angle = 0)
        : ROIShape(Ellipse), center_(center), radiusX_(rx), radiusY_(ry), angle_(angle) {}

    QPointF center() const { return center_; }
    void setCenter(const QPointF& center) { center_ = center; }

    double radiusX() const { return radiusX_; }
    void setRadiusX(double rx) { radiusX_ = rx; }

    double radiusY() const { return radiusY_; }
    void setRadiusY(double ry) { radiusY_ = ry; }

    double angle() const { return angle_; }
    void setAngle(double angle) { angle_ = angle; }

    QRectF boundingRect() const override;
    bool contains(const QPointF& point) const override;
    cv::Mat createMask(const cv::Size& imageSize) const override;
    QJsonObject toJson() const override;

    void translate(double dx, double dy) override {
        center_.setX(center_.x() + dx);
        center_.setY(center_.y() + dy);
    }
    void scale(double factor) override {
        radiusX_ *= factor;
        radiusY_ *= factor;
    }

private:
    QPointF center_;
    double radiusX_;
    double radiusY_;
    double angle_;  // 旋转角度（度）
};

/**
 * @brief 多边形ROI
 */
class PolygonROI : public ROIShape {
public:
    PolygonROI() : ROIShape(Polygon) {}
    explicit PolygonROI(const QPolygonF& polygon) : ROIShape(Polygon), polygon_(polygon) {}
    explicit PolygonROI(const std::vector<QPointF>& points);

    QPolygonF polygon() const { return polygon_; }
    void setPolygon(const QPolygonF& polygon) { polygon_ = polygon; }

    void addPoint(const QPointF& point) { polygon_.append(point); }
    void insertPoint(int index, const QPointF& point) { polygon_.insert(index, point); }
    void removePoint(int index) { polygon_.remove(index); }
    void setPoint(int index, const QPointF& point) { polygon_[index] = point; }
    int pointCount() const { return polygon_.size(); }

    QRectF boundingRect() const override { return polygon_.boundingRect(); }
    bool contains(const QPointF& point) const override { return polygon_.containsPoint(point, Qt::OddEvenFill); }
    cv::Mat createMask(const cv::Size& imageSize) const override;
    QJsonObject toJson() const override;

    void translate(double dx, double dy) override { polygon_.translate(dx, dy); }
    void scale(double factor) override;

private:
    QPolygonF polygon_;
};

/**
 * @brief 旋转矩形ROI
 */
class RotatedRectROI : public ROIShape {
public:
    RotatedRectROI() : ROIShape(RotatedRect), center_(50, 50), width_(100), height_(50), angle_(0) {}
    RotatedRectROI(const QPointF& center, double width, double height, double angle)
        : ROIShape(RotatedRect), center_(center), width_(width), height_(height), angle_(angle) {}

    QPointF center() const { return center_; }
    void setCenter(const QPointF& center) { center_ = center; }

    double width() const { return width_; }
    void setWidth(double w) { width_ = w; }

    double height() const { return height_; }
    void setHeight(double h) { height_ = h; }

    double angle() const { return angle_; }
    void setAngle(double angle) { angle_ = angle; }

    /**
     * @brief 获取四个顶点
     */
    std::vector<QPointF> vertices() const;

    QRectF boundingRect() const override;
    bool contains(const QPointF& point) const override;
    cv::Mat createMask(const cv::Size& imageSize) const override;
    QJsonObject toJson() const override;

    void translate(double dx, double dy) override {
        center_.setX(center_.x() + dx);
        center_.setY(center_.y() + dy);
    }
    void scale(double factor) override {
        width_ *= factor;
        height_ *= factor;
    }

private:
    QPointF center_;
    double width_;
    double height_;
    double angle_;  // 旋转角度（度）
};

/**
 * @brief ROI管理集合
 */
class ROICollection {
public:
    using ROIPtr = std::shared_ptr<ROIShape>;

    ROICollection() = default;

    /**
     * @brief 添加ROI
     */
    void addROI(const ROIPtr& roi);

    /**
     * @brief 移除ROI
     */
    void removeROI(int index);

    /**
     * @brief 清空所有ROI
     */
    void clear();

    /**
     * @brief 获取ROI数量
     */
    int count() const { return static_cast<int>(rois_.size()); }

    /**
     * @brief 获取ROI
     */
    ROIPtr getROI(int index) const;

    /**
     * @brief 获取所有ROI
     */
    const std::vector<ROIPtr>& getAllROIs() const { return rois_; }

    /**
     * @brief 创建组合mask
     * @param imageSize 图像尺寸
     * @param combineMode 组合模式: "or"(并集), "and"(交集), "xor"(异或)
     */
    cv::Mat createCombinedMask(const cv::Size& imageSize, const QString& combineMode = "or") const;

    /**
     * @brief 序列化
     */
    QJsonArray toJson() const;

    /**
     * @brief 反序列化
     */
    void fromJson(const QJsonArray& jsonArray);

private:
    std::vector<ROIPtr> rois_;
};

/**
 * @brief ROI工具
 *
 * 支持创建和管理多种类型的ROI区域，
 * 可用于限定其他工具的处理区域
 */
class ROITool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief ROI组合模式
     */
    enum CombineMode {
        Union,          // 并集
        Intersection,   // 交集
        XOR             // 异或
    };
    Q_ENUM(CombineMode)

    explicit ROITool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return ROI; }
    QString toolName() const override { return tr("ROI区域"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== ROI管理接口 ==========

    /**
     * @brief 获取ROI集合
     */
    ROICollection& roiCollection() { return rois_; }
    const ROICollection& roiCollection() const { return rois_; }

    /**
     * @brief 添加矩形ROI
     */
    void addRectROI(const QRectF& rect, const QString& name = QString());

    /**
     * @brief 添加圆形ROI
     */
    void addCircleROI(const QPointF& center, double radius, const QString& name = QString());

    /**
     * @brief 添加椭圆ROI
     */
    void addEllipseROI(const QPointF& center, double rx, double ry, double angle = 0, const QString& name = QString());

    /**
     * @brief 添加多边形ROI
     */
    void addPolygonROI(const QPolygonF& polygon, const QString& name = QString());

    /**
     * @brief 添加旋转矩形ROI
     */
    void addRotatedRectROI(const QPointF& center, double width, double height, double angle, const QString& name = QString());

    /**
     * @brief 清空所有ROI
     */
    void clearROIs();

    /**
     * @brief 移除指定ROI
     */
    void removeROI(int index);

    // ========== 参数设置 ==========

    /**
     * @brief 设置组合模式
     */
    void setCombineMode(CombineMode mode);
    CombineMode combineMode() const { return combineMode_; }

    /**
     * @brief 设置是否反转mask
     */
    void setInvertMask(bool invert);
    bool invertMask() const { return invertMask_; }

    /**
     * @brief 设置是否裁剪输出图像
     */
    void setCropOutput(bool crop);
    bool cropOutput() const { return cropOutput_; }

    /**
     * @brief 获取最后生成的mask
     */
    cv::Mat lastMask() const { return lastMask_; }

signals:
    /**
     * @brief ROI改变信号
     */
    void roiChanged();

private:
    ROICollection rois_;            // ROI集合
    CombineMode combineMode_;       // 组合模式
    bool invertMask_;               // 是否反转mask
    bool cropOutput_;               // 是否裁剪输出
    cv::Mat lastMask_;              // 最后生成的mask
};

} // namespace Algorithm
} // namespace VisionForge
