/**
 * @file MeasureDistanceTool.cpp
 * @brief 距离测量工具实现
 */

#include "algorithm/MeasureDistanceTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

MeasureDistanceTool::MeasureDistanceTool(QObject* parent)
    : VisionTool(parent)
    , mode_(PointToPoint)
    , circle1Radius_(0)
    , circle2Radius_(0)
    , pixelToMm_(1.0)
    , useToolChainInput_(false)
{
    setDisplayName(toolName());
}

bool MeasureDistanceTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    result_ = DistanceResult();

    // 根据模式计算距离
    double distance = 0.0;

    switch (mode_) {
    case PointToPoint:
        distance = measurePointToPoint();
        result_.description = "点到点距离";
        break;
    case PointToLine:
        distance = measurePointToLine();
        result_.description = "点到线距离";
        break;
    case LineToLine:
        distance = measureLineToLine();
        result_.description = "线到线距离";
        break;
    case CircleToPoint:
        distance = measureCircleToPoint();
        result_.description = "圆心到点距离";
        break;
    case CircleToLine:
        distance = measureCircleToLine();
        result_.description = "圆心到线距离";
        break;
    case CircleToCircle:
        distance = measureCircleToCircle();
        result_.description = "圆心到圆心距离";
        break;
    default:
        distance = measurePointToPoint();
        break;
    }

    result_.distance = distance;
    result_.distanceMm = distance * pixelToMm_;
    result_.pixelToMm = pixelToMm_;
    result_.valid = true;

    // 设置输出
    output.success = true;
    output.executionTime = timer.elapsed();
    output.setValue("distance", distance);
    output.setValue("distanceMm", result_.distanceMm);
    output.setValue("point1X", result_.point1.x());
    output.setValue("point1Y", result_.point1.y());
    output.setValue("point2X", result_.point2.x());
    output.setValue("point2Y", result_.point2.y());

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

    setStatusText(QString("距离: %.2f px (%.3f mm)")
                 .arg(distance).arg(result_.distanceMm));

    LOG_DEBUG(QString("距离测量完成: %.2f px, 耗时: %1ms")
             .arg(distance).arg(output.executionTime));

    emit measurementCompleted(result_);
    return true;
}

double MeasureDistanceTool::measurePointToPoint()
{
    result_.point1 = point1_;
    result_.point2 = point2_;

    double dx = point2_.x() - point1_.x();
    double dy = point2_.y() - point1_.y();
    return std::sqrt(dx * dx + dy * dy);
}

double MeasureDistanceTool::measurePointToLine()
{
    // 点到直线的距离公式: |Ax + By + C| / sqrt(A^2 + B^2)
    // 直线方程: (y2-y1)x - (x2-x1)y + (x2-x1)y1 - (y2-y1)x1 = 0

    double x1 = line1_.x1(), y1 = line1_.y1();
    double x2 = line1_.x2(), y2 = line1_.y2();
    double px = point1_.x(), py = point1_.y();

    double A = y2 - y1;
    double B = x1 - x2;
    double C = (x2 - x1) * y1 - (y2 - y1) * x1;

    double dist = std::abs(A * px + B * py + C) / std::sqrt(A * A + B * B);

    // 计算垂足点
    double t = ((px - x1) * (x2 - x1) + (py - y1) * (y2 - y1)) /
               ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    result_.point1 = point1_;
    result_.point2 = QPointF(x1 + t * (x2 - x1), y1 + t * (y2 - y1));

    return dist;
}

double MeasureDistanceTool::measureLineToLine()
{
    // 两平行线之间的距离
    // 取line1上的一点，计算到line2的距离

    QPointF pt = line1_.p1();

    double x1 = line2_.x1(), y1 = line2_.y1();
    double x2 = line2_.x2(), y2 = line2_.y2();
    double px = pt.x(), py = pt.y();

    double A = y2 - y1;
    double B = x1 - x2;
    double C = (x2 - x1) * y1 - (y2 - y1) * x1;

    double dist = std::abs(A * px + B * py + C) / std::sqrt(A * A + B * B);

    // 计算两条线上最近的点
    double t = ((px - x1) * (x2 - x1) + (py - y1) * (y2 - y1)) /
               ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    result_.point1 = pt;
    result_.point2 = QPointF(x1 + t * (x2 - x1), y1 + t * (y2 - y1));

    return dist;
}

double MeasureDistanceTool::measureCircleToPoint()
{
    double dx = point1_.x() - circle1Center_.x();
    double dy = point1_.y() - circle1Center_.y();
    double centerToPoint = std::sqrt(dx * dx + dy * dy);

    result_.point1 = circle1Center_;
    result_.point2 = point1_;

    return centerToPoint;
}

double MeasureDistanceTool::measureCircleToLine()
{
    // 圆心到直线的距离
    double x1 = line1_.x1(), y1 = line1_.y1();
    double x2 = line1_.x2(), y2 = line1_.y2();
    double px = circle1Center_.x(), py = circle1Center_.y();

    double A = y2 - y1;
    double B = x1 - x2;
    double C = (x2 - x1) * y1 - (y2 - y1) * x1;

    double dist = std::abs(A * px + B * py + C) / std::sqrt(A * A + B * B);

    double t = ((px - x1) * (x2 - x1) + (py - y1) * (y2 - y1)) /
               ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    result_.point1 = circle1Center_;
    result_.point2 = QPointF(x1 + t * (x2 - x1), y1 + t * (y2 - y1));

    return dist;
}

double MeasureDistanceTool::measureCircleToCircle()
{
    double dx = circle2Center_.x() - circle1Center_.x();
    double dy = circle2Center_.y() - circle1Center_.y();
    double centerDistance = std::sqrt(dx * dx + dy * dy);

    result_.point1 = circle1Center_;
    result_.point2 = circle2Center_;

    return centerDistance;
}

cv::Mat MeasureDistanceTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 绘制测量点
    cv::Point pt1(static_cast<int>(result_.point1.x()),
                  static_cast<int>(result_.point1.y()));
    cv::Point pt2(static_cast<int>(result_.point2.x()),
                  static_cast<int>(result_.point2.y()));

    // 绘制端点
    cv::circle(result, pt1, 5, cv::Scalar(0, 255, 0), -1);
    cv::circle(result, pt2, 5, cv::Scalar(0, 255, 0), -1);

    // 绘制测量线
    cv::line(result, pt1, pt2, cv::Scalar(0, 255, 255), 2);

    // 绘制距离标注
    cv::Point midPt((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);
    QString text = QString("%.2f px").arg(result_.distance);
    if (pixelToMm_ != 1.0) {
        text += QString(" (%.3f mm)").arg(result_.distanceMm);
    }

    cv::putText(result, text.toStdString(),
               cv::Point(midPt.x + 10, midPt.y - 10),
               cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 0), 2);

    // 根据模式绘制额外元素
    if (mode_ == CircleToPoint || mode_ == CircleToLine || mode_ == CircleToCircle) {
        cv::Point c1(static_cast<int>(circle1Center_.x()),
                    static_cast<int>(circle1Center_.y()));
        cv::circle(result, c1, static_cast<int>(circle1Radius_), cv::Scalar(255, 0, 0), 2);
    }

    if (mode_ == CircleToCircle) {
        cv::Point c2(static_cast<int>(circle2Center_.x()),
                    static_cast<int>(circle2Center_.y()));
        cv::circle(result, c2, static_cast<int>(circle2Radius_), cv::Scalar(255, 0, 0), 2);
    }

    if (mode_ == PointToLine || mode_ == LineToLine || mode_ == CircleToLine) {
        cv::Point l1p1(static_cast<int>(line1_.x1()), static_cast<int>(line1_.y1()));
        cv::Point l1p2(static_cast<int>(line1_.x2()), static_cast<int>(line1_.y2()));
        cv::line(result, l1p1, l1p2, cv::Scalar(255, 0, 255), 2);
    }

    if (mode_ == LineToLine) {
        cv::Point l2p1(static_cast<int>(line2_.x1()), static_cast<int>(line2_.y1()));
        cv::Point l2p2(static_cast<int>(line2_.x2()), static_cast<int>(line2_.y2()));
        cv::line(result, l2p1, l2p2, cv::Scalar(255, 0, 255), 2);
    }

    return result;
}

void MeasureDistanceTool::setMeasureMode(MeasureMode mode)
{
    if (mode_ != mode) {
        mode_ = mode;
        emit paramChanged();
    }
}

void MeasureDistanceTool::setCircle1(const QPointF& center, double radius)
{
    circle1Center_ = center;
    circle1Radius_ = radius;
    emit paramChanged();
}

void MeasureDistanceTool::setCircle2(const QPointF& center, double radius)
{
    circle2Center_ = center;
    circle2Radius_ = radius;
    emit paramChanged();
}

void MeasureDistanceTool::setPixelToMm(double ratio)
{
    if (qAbs(pixelToMm_ - ratio) > 0.0001) {
        pixelToMm_ = ratio;
        emit paramChanged();
    }
}

QJsonObject MeasureDistanceTool::serializeParams() const
{
    QJsonObject json;
    json["mode"] = static_cast<int>(mode_);
    json["point1X"] = point1_.x();
    json["point1Y"] = point1_.y();
    json["point2X"] = point2_.x();
    json["point2Y"] = point2_.y();
    json["line1X1"] = line1_.x1();
    json["line1Y1"] = line1_.y1();
    json["line1X2"] = line1_.x2();
    json["line1Y2"] = line1_.y2();
    json["line2X1"] = line2_.x1();
    json["line2Y1"] = line2_.y1();
    json["line2X2"] = line2_.x2();
    json["line2Y2"] = line2_.y2();
    json["circle1CenterX"] = circle1Center_.x();
    json["circle1CenterY"] = circle1Center_.y();
    json["circle1Radius"] = circle1Radius_;
    json["circle2CenterX"] = circle2Center_.x();
    json["circle2CenterY"] = circle2Center_.y();
    json["circle2Radius"] = circle2Radius_;
    json["pixelToMm"] = pixelToMm_;
    json["useToolChainInput"] = useToolChainInput_;
    return json;
}

void MeasureDistanceTool::deserializeParams(const QJsonObject& json)
{
    mode_ = static_cast<MeasureMode>(json.value("mode").toInt(0));
    point1_ = QPointF(json.value("point1X").toDouble(), json.value("point1Y").toDouble());
    point2_ = QPointF(json.value("point2X").toDouble(), json.value("point2Y").toDouble());
    line1_ = QLineF(json.value("line1X1").toDouble(), json.value("line1Y1").toDouble(),
                    json.value("line1X2").toDouble(), json.value("line1Y2").toDouble());
    line2_ = QLineF(json.value("line2X1").toDouble(), json.value("line2Y1").toDouble(),
                    json.value("line2X2").toDouble(), json.value("line2Y2").toDouble());
    circle1Center_ = QPointF(json.value("circle1CenterX").toDouble(),
                             json.value("circle1CenterY").toDouble());
    circle1Radius_ = json.value("circle1Radius").toDouble();
    circle2Center_ = QPointF(json.value("circle2CenterX").toDouble(),
                             json.value("circle2CenterY").toDouble());
    circle2Radius_ = json.value("circle2Radius").toDouble();
    pixelToMm_ = json.value("pixelToMm").toDouble(1.0);
    useToolChainInput_ = json.value("useToolChainInput").toBool(false);
}

} // namespace Algorithm
} // namespace VisionForge
