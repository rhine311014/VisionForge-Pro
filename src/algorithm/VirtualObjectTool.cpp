/**
 * @file VirtualObjectTool.cpp
 * @brief 虚拟对象计算工具实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "algorithm/VirtualObjectTool.h"
#include "base/Logger.h"

#include <cmath>
#include <limits>

namespace VisionForge {
namespace Algorithm {

// ============== Line2D 实现 ==============

Line2D::Line2D(const Point2D& p1, const Point2D& p2)
    : startPoint(p1)
    , endPoint(p2)
{
    // 计算一般式系数 ax + by + c = 0
    a = p2.y - p1.y;
    b = p1.x - p2.x;
    c = p2.x * p1.y - p1.x * p2.y;

    // 归一化
    double norm = std::sqrt(a * a + b * b);
    if (norm > 1e-10) {
        a /= norm;
        b /= norm;
        c /= norm;
    }
}

double Line2D::angle() const
{
    return std::atan2(-a, b) * 180.0 / M_PI;
}

double Line2D::length() const
{
    return startPoint.distanceTo(endPoint);
}

Point2D Line2D::midpoint() const
{
    return Point2D((startPoint.x + endPoint.x) / 2.0,
                   (startPoint.y + endPoint.y) / 2.0);
}

// ============== VirtualObjectTool 实现 ==============

VirtualObjectTool::VirtualObjectTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(tr("虚拟对象"));
}

bool VirtualObjectTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    Q_UNUSED(input);

    // 虚拟对象计算不需要图像输入，只需要几何数据
    output.success = true;
    output.executionTime = 0;

    // 根据结果类型设置输出
    switch (resultType_) {
        case VirtualPoint:
            output.setValue("x", resultPoint_.x);
            output.setValue("y", resultPoint_.y);
            setStatusText(tr("虚拟点: (%.2f, %.2f)").arg(resultPoint_.x).arg(resultPoint_.y));
            break;
        case VirtualLine:
            output.setValue("startX", resultLine_.startPoint.x);
            output.setValue("startY", resultLine_.startPoint.y);
            output.setValue("endX", resultLine_.endPoint.x);
            output.setValue("endY", resultLine_.endPoint.y);
            output.setValue("angle", resultLine_.angle());
            setStatusText(tr("虚拟线: 角度 %.2f°").arg(resultLine_.angle()));
            break;
        case VirtualCircle:
            output.setValue("centerX", resultCircle_.center.x);
            output.setValue("centerY", resultCircle_.center.y);
            output.setValue("radius", resultCircle_.radius);
            setStatusText(tr("虚拟圆: 圆心(%.2f, %.2f), 半径 %.2f")
                .arg(resultCircle_.center.x)
                .arg(resultCircle_.center.y)
                .arg(resultCircle_.radius));
            break;
    }

    return true;
}

// ========== 点计算 ==========

bool VirtualObjectTool::lineLineIntersection(const Line2D& line1, const Line2D& line2, Point2D& result)
{
    double det = line1.a * line2.b - line2.a * line1.b;

    if (std::abs(det) < 1e-10) {
        // 平行线，无交点
        return false;
    }

    result.x = (line1.b * line2.c - line2.b * line1.c) / det;
    result.y = (line2.a * line1.c - line1.a * line2.c) / det;

    return true;
}

int VirtualObjectTool::circleCircleIntersection(const Circle2D& c1, const Circle2D& c2,
                                                 Point2D& result1, Point2D& result2)
{
    double d = c1.center.distanceTo(c2.center);

    // 检查是否相交
    if (d > c1.radius + c2.radius + 1e-10) {
        // 相离
        return 0;
    }
    if (d < std::abs(c1.radius - c2.radius) - 1e-10) {
        // 内含
        return 0;
    }

    double a = (c1.radius * c1.radius - c2.radius * c2.radius + d * d) / (2 * d);
    double h = std::sqrt(std::max(0.0, c1.radius * c1.radius - a * a));

    // 中间点
    double px = c1.center.x + a * (c2.center.x - c1.center.x) / d;
    double py = c1.center.y + a * (c2.center.y - c1.center.y) / d;

    // 垂直方向
    double dx = h * (c2.center.y - c1.center.y) / d;
    double dy = h * (c2.center.x - c1.center.x) / d;

    result1.x = px + dx;
    result1.y = py - dy;
    result2.x = px - dx;
    result2.y = py + dy;

    if (std::abs(h) < 1e-10) {
        // 相切
        return 1;
    }

    return 2;
}

Point2D VirtualObjectTool::midpoint(const Point2D& p1, const Point2D& p2)
{
    return Point2D((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
}

Point2D VirtualObjectTool::centroid(const std::vector<Point2D>& points)
{
    if (points.empty()) {
        return Point2D();
    }

    double sumX = 0, sumY = 0;
    for (const auto& p : points) {
        sumX += p.x;
        sumY += p.y;
    }

    return Point2D(sumX / points.size(), sumY / points.size());
}

Point2D VirtualObjectTool::pointToLineProjection(const Point2D& point, const Line2D& line)
{
    // 计算点到直线的投影
    double t = -(line.a * point.x + line.b * point.y + line.c) / (line.a * line.a + line.b * line.b);
    return Point2D(point.x + line.a * t, point.y + line.b * t);
}

int VirtualObjectTool::lineCircleIntersection(const Line2D& line, const Circle2D& circle,
                                               Point2D& result1, Point2D& result2)
{
    // 直线 ax + by + c = 0, 圆心 (cx, cy), 半径 r
    double dist = std::abs(line.a * circle.center.x + line.b * circle.center.y + line.c);

    if (dist > circle.radius + 1e-10) {
        // 不相交
        return 0;
    }

    // 投影点
    Point2D proj = pointToLineProjection(circle.center, line);

    if (std::abs(dist - circle.radius) < 1e-10) {
        // 相切
        result1 = proj;
        return 1;
    }

    // 计算交点距离投影点的距离
    double h = std::sqrt(circle.radius * circle.radius - dist * dist);

    // 直线方向向量（单位化）
    double dx = -line.b;
    double dy = line.a;

    result1.x = proj.x + h * dx;
    result1.y = proj.y + h * dy;
    result2.x = proj.x - h * dx;
    result2.y = proj.y - h * dy;

    return 2;
}

// ========== 线计算 ==========

Line2D VirtualObjectTool::twoPointsLine(const Point2D& p1, const Point2D& p2)
{
    return Line2D(p1, p2);
}

Line2D VirtualObjectTool::parallelLine(const Line2D& line, const Point2D& point)
{
    // 平行线：相同的a, b，不同的c
    Line2D result;
    result.a = line.a;
    result.b = line.b;
    result.c = -(line.a * point.x + line.b * point.y);

    // 计算起点终点
    if (std::abs(line.b) > 1e-10) {
        result.startPoint = Point2D(point.x - 100, (-result.a * (point.x - 100) - result.c) / result.b);
        result.endPoint = Point2D(point.x + 100, (-result.a * (point.x + 100) - result.c) / result.b);
    } else {
        result.startPoint = Point2D(point.x, point.y - 100);
        result.endPoint = Point2D(point.x, point.y + 100);
    }

    return result;
}

Line2D VirtualObjectTool::parallelLineWithOffset(const Line2D& line, double offset)
{
    Line2D result;
    result.a = line.a;
    result.b = line.b;
    result.c = line.c + offset;  // 偏移c值

    // 计算新的起点终点
    Point2D mid = line.midpoint();
    Point2D normal(line.a, line.b);
    Point2D newMid = mid + normal * offset;

    return parallelLine(line, newMid);
}

Line2D VirtualObjectTool::perpendicularLine(const Line2D& line, const Point2D& point)
{
    // 垂直线：交换a, b
    Line2D result;
    result.a = -line.b;
    result.b = line.a;
    result.c = -(result.a * point.x + result.b * point.y);

    // 计算起点终点
    if (std::abs(result.b) > 1e-10) {
        result.startPoint = Point2D(point.x - 100, (-result.a * (point.x - 100) - result.c) / result.b);
        result.endPoint = Point2D(point.x + 100, (-result.a * (point.x + 100) - result.c) / result.b);
    } else {
        result.startPoint = Point2D(point.x, point.y - 100);
        result.endPoint = Point2D(point.x, point.y + 100);
    }

    return result;
}

Line2D VirtualObjectTool::angleBisector(const Line2D& line1, const Line2D& line2)
{
    // 角平分线
    Point2D intersection;
    if (!lineLineIntersection(line1, line2, intersection)) {
        return Line2D();
    }

    // 两条直线的方向向量
    double angle1 = line1.angle() * M_PI / 180.0;
    double angle2 = line2.angle() * M_PI / 180.0;
    double bisectorAngle = (angle1 + angle2) / 2.0;

    Point2D p2(intersection.x + 100 * std::cos(bisectorAngle),
               intersection.y + 100 * std::sin(bisectorAngle));

    return Line2D(intersection, p2);
}

Line2D VirtualObjectTool::fitLine(const std::vector<Point2D>& points)
{
    if (points.size() < 2) {
        return Line2D();
    }

    // 最小二乘拟合直线
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    size_t n = points.size();

    for (const auto& p : points) {
        sumX += p.x;
        sumY += p.y;
        sumXY += p.x * p.y;
        sumX2 += p.x * p.x;
    }

    double det = n * sumX2 - sumX * sumX;
    if (std::abs(det) < 1e-10) {
        // 垂直线
        double avgX = sumX / n;
        return Line2D(Point2D(avgX, 0), Point2D(avgX, 100));
    }

    double slope = (n * sumXY - sumX * sumY) / det;
    double intercept = (sumY - slope * sumX) / n;

    // 使用点集的范围确定起点终点
    double minX = points[0].x, maxX = points[0].x;
    for (const auto& p : points) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
    }

    return Line2D(Point2D(minX, slope * minX + intercept),
                  Point2D(maxX, slope * maxX + intercept));
}

// ========== 圆计算 ==========

bool VirtualObjectTool::threePointsCircle(const Point2D& p1, const Point2D& p2, const Point2D& p3,
                                          Circle2D& result)
{
    // 计算行列式
    double ax = p1.x, ay = p1.y;
    double bx = p2.x, by = p2.y;
    double cx = p3.x, cy = p3.y;

    double d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));

    if (std::abs(d) < 1e-10) {
        // 三点共线
        return false;
    }

    double ux = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / d;
    double uy = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / d;

    result.center = Point2D(ux, uy);
    result.radius = result.center.distanceTo(p1);

    return true;
}

Circle2D VirtualObjectTool::concentricCircle(const Circle2D& circle, double radiusOffset)
{
    return Circle2D(circle.center, circle.radius + radiusOffset);
}

bool VirtualObjectTool::fitCircle(const std::vector<Point2D>& points, Circle2D& result)
{
    if (points.size() < 3) {
        return false;
    }

    // 简化的最小二乘圆拟合
    // 使用代数距离最小化
    size_t n = points.size();
    double sumX = 0, sumY = 0;
    double sumX2 = 0, sumY2 = 0, sumXY = 0;
    double sumX3 = 0, sumY3 = 0, sumX2Y = 0, sumXY2 = 0;

    for (const auto& p : points) {
        double x = p.x, y = p.y;
        double x2 = x * x, y2 = y * y;
        sumX += x;
        sumY += y;
        sumX2 += x2;
        sumY2 += y2;
        sumXY += x * y;
        sumX3 += x * x2;
        sumY3 += y * y2;
        sumX2Y += x2 * y;
        sumXY2 += x * y2;
    }

    double C = n * sumX2 - sumX * sumX;
    double D = n * sumXY - sumX * sumY;
    double E = n * sumX3 + n * sumXY2 - (sumX2 + sumY2) * sumX;
    double G = n * sumY2 - sumY * sumY;
    double H = n * sumX2Y + n * sumY3 - (sumX2 + sumY2) * sumY;

    double denom = C * G - D * D;
    if (std::abs(denom) < 1e-10) {
        return false;
    }

    double a = (H * D - E * G) / (denom) / 2.0;
    double b = (E * D - H * C) / (denom) / 2.0;

    result.center = Point2D(a, b);

    // 计算平均半径
    double sumR = 0;
    for (const auto& p : points) {
        sumR += result.center.distanceTo(p);
    }
    result.radius = sumR / n;

    return true;
}

QJsonObject VirtualObjectTool::serializeParams() const
{
    QJsonObject json;
    json["resultType"] = static_cast<int>(resultType_);
    return json;
}

void VirtualObjectTool::deserializeParams(const QJsonObject& json)
{
    resultType_ = static_cast<ObjectType>(json.value("resultType").toInt(VirtualPoint));
}

} // namespace Algorithm
} // namespace VisionForge
