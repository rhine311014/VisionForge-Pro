/**
 * @file VirtualObjectTool.h
 * @brief 虚拟对象计算工具
 * @details 用于计算虚拟几何对象（虚拟点、虚拟线、虚拟圆等）
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 2D点结构
 */
struct Point2D {
    double x = 0;
    double y = 0;

    Point2D() = default;
    Point2D(double x_, double y_) : x(x_), y(y_) {}

    double distanceTo(const Point2D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    Point2D operator+(const Point2D& other) const {
        return Point2D(x + other.x, y + other.y);
    }

    Point2D operator-(const Point2D& other) const {
        return Point2D(x - other.x, y - other.y);
    }

    Point2D operator*(double scale) const {
        return Point2D(x * scale, y * scale);
    }
};

/**
 * @brief 2D直线结构 (ax + by + c = 0)
 */
struct Line2D {
    double a = 0;   ///< 系数a
    double b = 0;   ///< 系数b
    double c = 0;   ///< 系数c

    // 起点终点表示
    Point2D startPoint;
    Point2D endPoint;

    Line2D() = default;
    Line2D(const Point2D& p1, const Point2D& p2);
    Line2D(double a_, double b_, double c_) : a(a_), b(b_), c(c_) {}

    double angle() const;  ///< 获取角度（度）
    double length() const; ///< 获取长度（起点到终点）
    Point2D midpoint() const; ///< 获取中点
};

/**
 * @brief 2D圆结构
 */
struct Circle2D {
    Point2D center;     ///< 圆心
    double radius = 0;  ///< 半径

    Circle2D() = default;
    Circle2D(const Point2D& c, double r) : center(c), radius(r) {}
    Circle2D(double cx, double cy, double r) : center(cx, cy), radius(r) {}
};

/**
 * @brief 虚拟对象计算工具
 *
 * 提供各种虚拟几何对象的计算功能：
 * - 虚拟点：两线交点、两圆交点、中点、重心等
 * - 虚拟线：两点连线、平行线、垂直线等
 * - 虚拟圆：三点拟合圆、同心圆等
 */
class VirtualObjectTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 虚拟对象类型
     */
    enum ObjectType {
        VirtualPoint = 0,
        VirtualLine,
        VirtualCircle
    };

    explicit VirtualObjectTool(QObject* parent = nullptr);
    ~VirtualObjectTool() override = default;

    // VisionTool接口实现
    ToolType toolType() const override { return Custom; }
    QString toolName() const override { return tr("虚拟对象"); }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 点计算 ==========

    /**
     * @brief 计算两线交点
     */
    static bool lineLineIntersection(const Line2D& line1, const Line2D& line2, Point2D& result);

    /**
     * @brief 计算两圆交点
     * @return 交点数量 (0, 1, 或 2)
     */
    static int circleCircleIntersection(const Circle2D& c1, const Circle2D& c2,
                                        Point2D& result1, Point2D& result2);

    /**
     * @brief 计算两点中点
     */
    static Point2D midpoint(const Point2D& p1, const Point2D& p2);

    /**
     * @brief 计算多点重心
     */
    static Point2D centroid(const std::vector<Point2D>& points);

    /**
     * @brief 计算点到线的投影
     */
    static Point2D pointToLineProjection(const Point2D& point, const Line2D& line);

    /**
     * @brief 计算直线与圆的交点
     * @return 交点数量 (0, 1, 或 2)
     */
    static int lineCircleIntersection(const Line2D& line, const Circle2D& circle,
                                      Point2D& result1, Point2D& result2);

    // ========== 线计算 ==========

    /**
     * @brief 两点连线
     */
    static Line2D twoPointsLine(const Point2D& p1, const Point2D& p2);

    /**
     * @brief 过点作平行线
     */
    static Line2D parallelLine(const Line2D& line, const Point2D& point);

    /**
     * @brief 过点作平行线（指定偏移距离）
     */
    static Line2D parallelLineWithOffset(const Line2D& line, double offset);

    /**
     * @brief 过点作垂直线
     */
    static Line2D perpendicularLine(const Line2D& line, const Point2D& point);

    /**
     * @brief 角平分线
     */
    static Line2D angleBisector(const Line2D& line1, const Line2D& line2);

    /**
     * @brief 多点拟合直线（最小二乘）
     */
    static Line2D fitLine(const std::vector<Point2D>& points);

    // ========== 圆计算 ==========

    /**
     * @brief 三点拟合圆
     */
    static bool threePointsCircle(const Point2D& p1, const Point2D& p2, const Point2D& p3,
                                  Circle2D& result);

    /**
     * @brief 同心圆（偏移半径）
     */
    static Circle2D concentricCircle(const Circle2D& circle, double radiusOffset);

    /**
     * @brief 多点拟合圆（最小二乘）
     */
    static bool fitCircle(const std::vector<Point2D>& points, Circle2D& result);

    // ========== 结果获取 ==========

    ObjectType resultType() const { return resultType_; }
    Point2D resultPoint() const { return resultPoint_; }
    Line2D resultLine() const { return resultLine_; }
    Circle2D resultCircle() const { return resultCircle_; }

private:
    ObjectType resultType_ = VirtualPoint;
    Point2D resultPoint_;
    Line2D resultLine_;
    Circle2D resultCircle_;
};

} // namespace Algorithm
} // namespace VisionForge
