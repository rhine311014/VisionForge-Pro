/**
 * @file MeasureAreaTool.cpp
 * @brief 面积测量工具实现
 */

#include "algorithm/MeasureAreaTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace VisionForge {
namespace Algorithm {

MeasureAreaTool::MeasureAreaTool(QObject* parent)
    : VisionTool(parent)
    , mode_(Contour)
    , circleRadius_(0)
    , ellipseA_(0)
    , ellipseB_(0)
    , ellipseAngle_(0)
    , pixelToMm_(1.0)
    , threshold_(128)
    , minArea_(100)
{
    setDisplayName(toolName());
}

bool MeasureAreaTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    result_ = AreaResult();
    allResults_.clear();
    contours_.clear();

    double area = 0.0;

    switch (mode_) {
    case Polygon:
        area = measurePolygon();
        break;
    case Contour:
        if (input && !input->isEmpty()) {
            measureContours(input->mat());
            if (!allResults_.empty()) {
                result_ = allResults_[0];  // 取最大面积
                area = result_.area;
            }
        }
        break;
    case Circle:
        area = measureCircle();
        break;
    case Ellipse:
        area = measureEllipse();
        break;
    }

    result_.area = area;
    result_.areaMm2 = area * pixelToMm_ * pixelToMm_;
    result_.valid = (area > 0);

    // 设置输出
    output.success = result_.valid;
    output.executionTime = timer.elapsed();
    output.setValue("area", area);
    output.setValue("areaMm2", result_.areaMm2);
    output.setValue("perimeter", result_.perimeter);
    output.setValue("centroidX", result_.centroid.x());
    output.setValue("centroidY", result_.centroid.y());
    output.setValue("circularity", result_.circularity);
    output.setValue("contourCount", static_cast<int>(allResults_.size()));

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

    QString statusText = QString("面积: %.1f px² (%.3f mm²)")
                        .arg(area).arg(result_.areaMm2);
    if (mode_ == Contour) {
        statusText += QString(" [%1个轮廓]").arg(allResults_.size());
    }
    setStatusText(statusText);

    LOG_DEBUG(QString("面积测量完成: %.1f px², 耗时: %1ms")
             .arg(area).arg(output.executionTime));

    emit measurementCompleted(result_);
    return result_.valid;
}

double MeasureAreaTool::measurePolygon()
{
    if (polygon_.size() < 3) {
        return 0.0;
    }

    // 使用向量积计算多边形面积
    double area = 0.0;
    int n = polygon_.size();

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += polygon_[i].x() * polygon_[j].y();
        area -= polygon_[j].x() * polygon_[i].y();
    }
    area = std::abs(area) / 2.0;

    // 计算质心
    double cx = 0, cy = 0;
    for (const auto& pt : polygon_) {
        cx += pt.x();
        cy += pt.y();
    }
    result_.centroid = QPointF(cx / n, cy / n);

    // 计算周长
    double perimeter = 0.0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        double dx = polygon_[j].x() - polygon_[i].x();
        double dy = polygon_[j].y() - polygon_[i].y();
        perimeter += std::sqrt(dx * dx + dy * dy);
    }
    result_.perimeter = perimeter;

    // 计算圆度
    if (perimeter > 0) {
        result_.circularity = 4.0 * M_PI * area / (perimeter * perimeter);
    }

    // 计算外接矩形
    result_.boundingRect = polygon_.boundingRect();

    return area;
}

void MeasureAreaTool::measureContours(const cv::Mat& input)
{
    cv::Mat gray, binary;

    // 转灰度
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else if (input.channels() == 4) {
        cv::cvtColor(input, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = input;
    }

    // 二值化
    cv::threshold(gray, binary, threshold_, 255, cv::THRESH_BINARY);

    // 查找轮廓
    cv::findContours(binary, contours_, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 分析每个轮廓
    for (const auto& contour : contours_) {
        double area = cv::contourArea(contour);

        if (area < minArea_) continue;

        AreaResult res;
        res.area = area;
        res.areaMm2 = area * pixelToMm_ * pixelToMm_;
        res.perimeter = cv::arcLength(contour, true);
        res.valid = true;

        // 计算质心
        cv::Moments m = cv::moments(contour);
        if (m.m00 > 0) {
            res.centroid = QPointF(m.m10 / m.m00, m.m01 / m.m00);
        }

        // 计算圆度
        if (res.perimeter > 0) {
            res.circularity = 4.0 * M_PI * area / (res.perimeter * res.perimeter);
        }

        // 外接矩形
        cv::Rect bbox = cv::boundingRect(contour);
        res.boundingRect = QRectF(bbox.x, bbox.y, bbox.width, bbox.height);

        allResults_.push_back(res);
    }

    // 按面积降序排序
    std::sort(allResults_.begin(), allResults_.end(),
             [](const AreaResult& a, const AreaResult& b) {
                 return a.area > b.area;
             });
}

double MeasureAreaTool::measureCircle()
{
    double area = M_PI * circleRadius_ * circleRadius_;

    result_.centroid = circleCenter_;
    result_.perimeter = 2.0 * M_PI * circleRadius_;
    result_.circularity = 1.0;
    result_.boundingRect = QRectF(
        circleCenter_.x() - circleRadius_,
        circleCenter_.y() - circleRadius_,
        2 * circleRadius_,
        2 * circleRadius_
    );

    return area;
}

double MeasureAreaTool::measureEllipse()
{
    double area = M_PI * ellipseA_ * ellipseB_;

    result_.centroid = ellipseCenter_;
    // 椭圆周长近似公式
    result_.perimeter = M_PI * (3 * (ellipseA_ + ellipseB_) -
                               std::sqrt((3 * ellipseA_ + ellipseB_) * (ellipseA_ + 3 * ellipseB_)));

    // 椭圆圆度
    if (ellipseA_ > 0 && ellipseB_ > 0) {
        result_.circularity = std::min(ellipseA_, ellipseB_) / std::max(ellipseA_, ellipseB_);
    }

    double maxAxis = std::max(ellipseA_, ellipseB_);
    result_.boundingRect = QRectF(
        ellipseCenter_.x() - maxAxis,
        ellipseCenter_.y() - maxAxis,
        2 * maxAxis,
        2 * maxAxis
    );

    return area;
}

cv::Mat MeasureAreaTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    if (mode_ == Contour && !contours_.empty()) {
        // 绘制所有轮廓
        cv::drawContours(result, contours_, -1, cv::Scalar(0, 255, 0), 2);

        // 绘制质心和面积标注
        for (size_t i = 0; i < allResults_.size() && i < 10; ++i) {
            const auto& res = allResults_[i];
            cv::Point center(static_cast<int>(res.centroid.x()),
                           static_cast<int>(res.centroid.y()));

            cv::circle(result, center, 5, cv::Scalar(0, 0, 255), -1);

            QString text = QString("#%1: %.0f").arg(i + 1).arg(res.area);
            cv::putText(result, text.toStdString(),
                       cv::Point(center.x + 10, center.y),
                       cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1);
        }
    } else if (mode_ == Polygon && polygon_.size() >= 3) {
        std::vector<cv::Point> pts;
        for (const auto& p : polygon_) {
            pts.push_back(cv::Point(static_cast<int>(p.x()), static_cast<int>(p.y())));
        }
        cv::polylines(result, pts, true, cv::Scalar(0, 255, 0), 2);

        cv::Point center(static_cast<int>(result_.centroid.x()),
                        static_cast<int>(result_.centroid.y()));
        cv::circle(result, center, 5, cv::Scalar(0, 0, 255), -1);
    } else if (mode_ == Circle) {
        cv::Point center(static_cast<int>(circleCenter_.x()),
                        static_cast<int>(circleCenter_.y()));
        cv::circle(result, center, static_cast<int>(circleRadius_), cv::Scalar(0, 255, 0), 2);
        cv::circle(result, center, 5, cv::Scalar(0, 0, 255), -1);
    } else if (mode_ == Ellipse) {
        cv::Point center(static_cast<int>(ellipseCenter_.x()),
                        static_cast<int>(ellipseCenter_.y()));
        cv::ellipse(result, center,
                   cv::Size(static_cast<int>(ellipseA_), static_cast<int>(ellipseB_)),
                   ellipseAngle_, 0, 360, cv::Scalar(0, 255, 0), 2);
        cv::circle(result, center, 5, cv::Scalar(0, 0, 255), -1);
    }

    // 显示总面积
    QString areaText = QString("Area: %.1f px²").arg(result_.area);
    cv::putText(result, areaText.toStdString(),
               cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
               cv::Scalar(255, 255, 0), 2);

    return result;
}

void MeasureAreaTool::setMeasureMode(MeasureMode mode)
{
    if (mode_ != mode) {
        mode_ = mode;
        emit paramChanged();
    }
}

void MeasureAreaTool::setPolygon(const QPolygonF& polygon)
{
    polygon_ = polygon;
    emit paramChanged();
}

void MeasureAreaTool::setCircle(const QPointF& center, double radius)
{
    circleCenter_ = center;
    circleRadius_ = radius;
    emit paramChanged();
}

void MeasureAreaTool::setEllipse(const QPointF& center, double a, double b, double angle)
{
    ellipseCenter_ = center;
    ellipseA_ = a;
    ellipseB_ = b;
    ellipseAngle_ = angle;
    emit paramChanged();
}

void MeasureAreaTool::setPixelToMm(double ratio)
{
    if (qAbs(pixelToMm_ - ratio) > 0.0001) {
        pixelToMm_ = ratio;
        emit paramChanged();
    }
}

QJsonObject MeasureAreaTool::serializeParams() const
{
    QJsonObject json;
    json["mode"] = static_cast<int>(mode_);
    json["pixelToMm"] = pixelToMm_;
    json["threshold"] = threshold_;
    json["minArea"] = minArea_;

    // 多边形
    QJsonArray polyArray;
    for (const auto& pt : polygon_) {
        QJsonObject ptObj;
        ptObj["x"] = pt.x();
        ptObj["y"] = pt.y();
        polyArray.append(ptObj);
    }
    json["polygon"] = polyArray;

    // 圆形
    json["circleCenterX"] = circleCenter_.x();
    json["circleCenterY"] = circleCenter_.y();
    json["circleRadius"] = circleRadius_;

    // 椭圆
    json["ellipseCenterX"] = ellipseCenter_.x();
    json["ellipseCenterY"] = ellipseCenter_.y();
    json["ellipseA"] = ellipseA_;
    json["ellipseB"] = ellipseB_;
    json["ellipseAngle"] = ellipseAngle_;

    return json;
}

void MeasureAreaTool::deserializeParams(const QJsonObject& json)
{
    mode_ = static_cast<MeasureMode>(json.value("mode").toInt(1));
    pixelToMm_ = json.value("pixelToMm").toDouble(1.0);
    threshold_ = json.value("threshold").toInt(128);
    minArea_ = json.value("minArea").toDouble(100);

    // 多边形
    polygon_.clear();
    QJsonArray polyArray = json.value("polygon").toArray();
    for (const auto& ptVal : polyArray) {
        QJsonObject ptObj = ptVal.toObject();
        polygon_.append(QPointF(ptObj["x"].toDouble(), ptObj["y"].toDouble()));
    }

    // 圆形
    circleCenter_ = QPointF(json.value("circleCenterX").toDouble(),
                           json.value("circleCenterY").toDouble());
    circleRadius_ = json.value("circleRadius").toDouble();

    // 椭圆
    ellipseCenter_ = QPointF(json.value("ellipseCenterX").toDouble(),
                            json.value("ellipseCenterY").toDouble());
    ellipseA_ = json.value("ellipseA").toDouble();
    ellipseB_ = json.value("ellipseB").toDouble();
    ellipseAngle_ = json.value("ellipseAngle").toDouble();
}

} // namespace Algorithm
} // namespace VisionForge
