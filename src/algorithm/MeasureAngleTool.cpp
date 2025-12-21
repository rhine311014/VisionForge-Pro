/**
 * @file MeasureAngleTool.cpp
 * @brief 角度测量工具实现
 */

#include "algorithm/MeasureAngleTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace VisionForge {
namespace Algorithm {

MeasureAngleTool::MeasureAngleTool(QObject* parent)
    : VisionTool(parent)
    , mode_(LineToLine)
    , angleRange_(Range0To180)
{
    setDisplayName(toolName());
}

bool MeasureAngleTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    result_ = AngleResult();

    // 根据模式计算角度
    double angle = 0.0;

    switch (mode_) {
    case LineToLine:
        angle = measureLineToLine();
        result_.description = "两线夹角";
        break;
    case ThreePoints:
        angle = measureThreePoints();
        result_.description = "三点角度";
        break;
    case LineToHorizontal:
        angle = measureLineToHorizontal();
        result_.description = "水平方向角度";
        break;
    case LineToVertical:
        angle = measureLineToVertical();
        result_.description = "垂直方向角度";
        break;
    }

    result_.angle = normalizeAngle(angle);
    result_.angleRad = result_.angle * M_PI / 180.0;
    result_.valid = true;

    // 设置输出
    output.success = true;
    output.executionTime = timer.elapsed();
    output.setValue("angle", result_.angle);
    output.setValue("angleRad", result_.angleRad);
    output.setValue("vertexX", result_.vertex.x());
    output.setValue("vertexY", result_.vertex.y());

    // 绘制结果
    if (input && !input->isEmpty()) {
        cv::Mat resultImage = drawResults(input->mat());
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }
        setDebugImage(output.outputImage);
    }

    setStatusText(QString("角度: %.2f°").arg(result_.angle));

    LOG_DEBUG(QString("角度测量完成: %.2f°, 耗时: %1ms")
             .arg(result_.angle).arg(output.executionTime));

    emit measurementCompleted(result_);
    return true;
}

double MeasureAngleTool::measureLineToLine()
{
    // 计算两条线的方向向量
    double dx1 = line1_.dx();
    double dy1 = line1_.dy();
    double dx2 = line2_.dx();
    double dy2 = line2_.dy();

    // 计算点积和叉积
    double dot = dx1 * dx2 + dy1 * dy2;
    double cross = dx1 * dy2 - dy1 * dx2;

    // 计算角度
    double angle = std::atan2(std::abs(cross), dot) * 180.0 / M_PI;

    // 计算交点作为顶点
    QPointF intersection;
    QLineF::IntersectionType type = line1_.intersects(line2_, &intersection);
    if (type == QLineF::BoundedIntersection || type == QLineF::UnboundedIntersection) {
        result_.vertex = intersection;
    } else {
        result_.vertex = line1_.p1();
    }

    result_.point1 = line1_.p2();
    result_.point2 = line2_.p2();

    return angle;
}

double MeasureAngleTool::measureThreePoints()
{
    // 计算向量
    double dx1 = point1_.x() - vertex_.x();
    double dy1 = point1_.y() - vertex_.y();
    double dx2 = point2_.x() - vertex_.x();
    double dy2 = point2_.y() - vertex_.y();

    // 计算点积和模
    double dot = dx1 * dx2 + dy1 * dy2;
    double len1 = std::sqrt(dx1 * dx1 + dy1 * dy1);
    double len2 = std::sqrt(dx2 * dx2 + dy2 * dy2);

    if (len1 < 1e-6 || len2 < 1e-6) {
        return 0.0;
    }

    double cosAngle = dot / (len1 * len2);
    cosAngle = std::max(-1.0, std::min(1.0, cosAngle)); // clamp

    double angle = std::acos(cosAngle) * 180.0 / M_PI;

    result_.vertex = vertex_;
    result_.point1 = point1_;
    result_.point2 = point2_;

    return angle;
}

double MeasureAngleTool::measureLineToHorizontal()
{
    double dx = line1_.dx();
    double dy = line1_.dy();

    double angle = std::atan2(dy, dx) * 180.0 / M_PI;

    result_.vertex = line1_.p1();
    result_.point1 = line1_.p2();
    result_.point2 = QPointF(line1_.p1().x() + 100, line1_.p1().y()); // 水平参考线

    return angle;
}

double MeasureAngleTool::measureLineToVertical()
{
    double dx = line1_.dx();
    double dy = line1_.dy();

    double angle = std::atan2(dx, dy) * 180.0 / M_PI;

    result_.vertex = line1_.p1();
    result_.point1 = line1_.p2();
    result_.point2 = QPointF(line1_.p1().x(), line1_.p1().y() - 100); // 垂直参考线

    return angle;
}

double MeasureAngleTool::normalizeAngle(double angle)
{
    switch (angleRange_) {
    case Range0To180:
        angle = std::abs(angle);
        if (angle > 180.0) angle = 360.0 - angle;
        break;
    case Range0To360:
        while (angle < 0) angle += 360.0;
        while (angle >= 360.0) angle -= 360.0;
        break;
    case RangeMinus180To180:
        while (angle <= -180.0) angle += 360.0;
        while (angle > 180.0) angle -= 360.0;
        break;
    }
    return angle;
}

cv::Mat MeasureAngleTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    cv::Point vertex(static_cast<int>(result_.vertex.x()),
                    static_cast<int>(result_.vertex.y()));
    cv::Point pt1(static_cast<int>(result_.point1.x()),
                 static_cast<int>(result_.point1.y()));
    cv::Point pt2(static_cast<int>(result_.point2.x()),
                 static_cast<int>(result_.point2.y()));

    // 绘制两条边
    cv::line(result, vertex, pt1, cv::Scalar(0, 255, 0), 2);
    cv::line(result, vertex, pt2, cv::Scalar(0, 255, 0), 2);

    // 绘制顶点
    cv::circle(result, vertex, 6, cv::Scalar(0, 0, 255), -1);

    // 绘制角度弧
    double radius = 40;
    double startAngle = std::atan2(pt1.y - vertex.y, pt1.x - vertex.x) * 180.0 / M_PI;
    double endAngle = std::atan2(pt2.y - vertex.y, pt2.x - vertex.x) * 180.0 / M_PI;

    cv::ellipse(result, vertex, cv::Size(static_cast<int>(radius), static_cast<int>(radius)),
               0, startAngle, endAngle, cv::Scalar(255, 255, 0), 2);

    // 绘制角度标注
    QString text = QString::fromUtf8("%.2f°").arg(result_.angle);
    cv::Point textPos(vertex.x + 50, vertex.y - 20);
    cv::putText(result, text.toStdString(), textPos,
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 0), 2);

    return result;
}

void MeasureAngleTool::setMeasureMode(MeasureMode mode)
{
    if (mode_ != mode) {
        mode_ = mode;
        emit paramChanged();
    }
}

void MeasureAngleTool::setAngleRange(AngleRange range)
{
    if (angleRange_ != range) {
        angleRange_ = range;
        emit paramChanged();
    }
}

QJsonObject MeasureAngleTool::serializeParams() const
{
    QJsonObject json;
    json["mode"] = static_cast<int>(mode_);
    json["angleRange"] = static_cast<int>(angleRange_);
    json["line1X1"] = line1_.x1();
    json["line1Y1"] = line1_.y1();
    json["line1X2"] = line1_.x2();
    json["line1Y2"] = line1_.y2();
    json["line2X1"] = line2_.x1();
    json["line2Y1"] = line2_.y1();
    json["line2X2"] = line2_.x2();
    json["line2Y2"] = line2_.y2();
    json["vertexX"] = vertex_.x();
    json["vertexY"] = vertex_.y();
    json["point1X"] = point1_.x();
    json["point1Y"] = point1_.y();
    json["point2X"] = point2_.x();
    json["point2Y"] = point2_.y();
    return json;
}

void MeasureAngleTool::deserializeParams(const QJsonObject& json)
{
    mode_ = static_cast<MeasureMode>(json.value("mode").toInt(0));
    angleRange_ = static_cast<AngleRange>(json.value("angleRange").toInt(0));
    line1_ = QLineF(json.value("line1X1").toDouble(), json.value("line1Y1").toDouble(),
                    json.value("line1X2").toDouble(), json.value("line1Y2").toDouble());
    line2_ = QLineF(json.value("line2X1").toDouble(), json.value("line2Y1").toDouble(),
                    json.value("line2X2").toDouble(), json.value("line2Y2").toDouble());
    vertex_ = QPointF(json.value("vertexX").toDouble(), json.value("vertexY").toDouble());
    point1_ = QPointF(json.value("point1X").toDouble(), json.value("point1Y").toDouble());
    point2_ = QPointF(json.value("point2X").toDouble(), json.value("point2Y").toDouble());
}

} // namespace Algorithm
} // namespace VisionForge
