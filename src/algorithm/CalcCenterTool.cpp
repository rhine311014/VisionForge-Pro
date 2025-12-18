/**
 * @file CalcCenterTool.cpp
 * @brief 中心计算工具实现
 */

#include "algorithm/CalcCenterTool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

CalcCenterTool::CalcCenterTool(QObject* parent)
    : VisionTool(parent)
    , calcMethod_(Centroid)
    , sourceType_(FromImage)
    , threshold_(128)
    , inverted_(false)
    , center_(0, 0)
    , area_(0)
    , radius_(0)
{
    setDisplayName(toolName());
}

bool CalcCenterTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (sourceType_ == FromPoints) {
        return calcFromPoints(output);
    }

    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    cv::Mat src = input->mat();
    cv::Mat gray;

    // 转换为灰度图
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else if (src.channels() == 4) {
        cv::cvtColor(src, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = src.clone();
    }

    bool success = calcFromImage(gray, output);

    if (success) {
        output.executionTime = timer.elapsed();

        // 绘制结果
        cv::Mat resultImage = drawResults(src);
        output.outputImage = std::make_shared<Base::ImageData>(resultImage);

        setDebugImage(output.outputImage);
        setStatusText(QString("中心: (%.1f, %.1f)").arg(center_.x()).arg(center_.y()));

        LOG_DEBUG(QString("中心计算完成: (%.1f, %.1f), 耗时: %2ms")
                 .arg(center_.x()).arg(center_.y())
                 .arg(output.executionTime));
    }

    return success;
}

bool CalcCenterTool::calcFromImage(const cv::Mat& gray, ToolResult& output)
{
    try {
        // 二值化
        cv::Mat binary;
        int threshType = inverted_ ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY;
        cv::threshold(gray, binary, threshold_, 255, threshType);

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        if (contours.empty()) {
            output.success = false;
            output.errorMessage = "未找到轮廓";
            return false;
        }

        // 找最大轮廓
        size_t maxIdx = 0;
        double maxArea = 0;
        for (size_t i = 0; i < contours.size(); ++i) {
            double area = cv::contourArea(contours[i]);
            if (area > maxArea) {
                maxArea = area;
                maxIdx = i;
            }
        }

        return calcFromContour(contours[maxIdx], output);

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("中心计算失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool CalcCenterTool::calcFromContour(const std::vector<cv::Point>& contour, ToolResult& output)
{
    if (contour.empty()) {
        output.success = false;
        output.errorMessage = "轮廓为空";
        return false;
    }

    try {
        switch (calcMethod_) {
        case Centroid: {
            // 使用矩计算质心
            cv::Moments m = cv::moments(contour);
            if (m.m00 > 0) {
                center_.setX(m.m10 / m.m00);
                center_.setY(m.m01 / m.m00);
                area_ = m.m00;
            } else {
                // 退化为几何中心
                double sumX = 0, sumY = 0;
                for (const auto& pt : contour) {
                    sumX += pt.x;
                    sumY += pt.y;
                }
                center_.setX(sumX / contour.size());
                center_.setY(sumY / contour.size());
                area_ = 0;
            }
            break;
        }

        case GeometricCenter: {
            // 轮廓点平均
            double sumX = 0, sumY = 0;
            for (const auto& pt : contour) {
                sumX += pt.x;
                sumY += pt.y;
            }
            center_.setX(sumX / contour.size());
            center_.setY(sumY / contour.size());
            area_ = cv::contourArea(contour);
            break;
        }

        case BoundingBoxCenter: {
            // 边界框中心
            cv::Rect bbox = cv::boundingRect(contour);
            center_.setX(bbox.x + bbox.width / 2.0);
            center_.setY(bbox.y + bbox.height / 2.0);
            area_ = cv::contourArea(contour);
            break;
        }

        case MinCircleCenter: {
            // 最小外接圆中心
            cv::Point2f c;
            float r;
            cv::minEnclosingCircle(contour, c, r);
            center_.setX(c.x);
            center_.setY(c.y);
            radius_ = r;
            area_ = cv::contourArea(contour);
            break;
        }

        default:
            // 默认使用质心
            cv::Moments m = cv::moments(contour);
            if (m.m00 > 0) {
                center_.setX(m.m10 / m.m00);
                center_.setY(m.m01 / m.m00);
                area_ = m.m00;
            }
            break;
        }

        output.success = true;
        output.setValue("centerX", center_.x());
        output.setValue("centerY", center_.y());
        output.setValue("area", area_);
        if (calcMethod_ == MinCircleCenter) {
            output.setValue("radius", radius_);
        }

        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("中心计算失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool CalcCenterTool::calcFromPoints(ToolResult& output)
{
    if (inputPoints_.empty()) {
        output.success = false;
        output.errorMessage = "输入点列表为空";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    try {
        switch (calcMethod_) {
        case PointsAverage:
        case GeometricCenter:
        case Centroid: {
            // 点平均
            double sumX = 0, sumY = 0;
            for (const auto& pt : inputPoints_) {
                sumX += pt.x();
                sumY += pt.y();
            }
            center_.setX(sumX / inputPoints_.size());
            center_.setY(sumY / inputPoints_.size());
            break;
        }

        case BoundingBoxCenter: {
            // 边界框中心
            double minX = inputPoints_[0].x(), maxX = minX;
            double minY = inputPoints_[0].y(), maxY = minY;
            for (const auto& pt : inputPoints_) {
                minX = std::min(minX, pt.x());
                maxX = std::max(maxX, pt.x());
                minY = std::min(minY, pt.y());
                maxY = std::max(maxY, pt.y());
            }
            center_.setX((minX + maxX) / 2.0);
            center_.setY((minY + maxY) / 2.0);
            break;
        }

        case MinCircleCenter: {
            // 转换为OpenCV格式
            std::vector<cv::Point2f> cvPoints;
            cvPoints.reserve(inputPoints_.size());
            for (const auto& pt : inputPoints_) {
                cvPoints.emplace_back(static_cast<float>(pt.x()),
                                     static_cast<float>(pt.y()));
            }

            cv::Point2f c;
            float r;
            cv::minEnclosingCircle(cvPoints, c, r);
            center_.setX(c.x);
            center_.setY(c.y);
            radius_ = r;
            break;
        }

        default:
            // 默认点平均
            double sumX = 0, sumY = 0;
            for (const auto& pt : inputPoints_) {
                sumX += pt.x();
                sumY += pt.y();
            }
            center_.setX(sumX / inputPoints_.size());
            center_.setY(sumY / inputPoints_.size());
            break;
        }

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("centerX", center_.x());
        output.setValue("centerY", center_.y());
        output.setValue("pointCount", static_cast<int>(inputPoints_.size()));
        if (calcMethod_ == MinCircleCenter) {
            output.setValue("radius", radius_);
        }

        setStatusText(QString("中心: (%.1f, %.1f)").arg(center_.x()).arg(center_.y()));

        return true;

    } catch (const std::exception& e) {
        output.success = false;
        output.errorMessage = QString("中心计算失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

cv::Mat CalcCenterTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 绘制中心点
    cv::Point centerPt(static_cast<int>(center_.x()),
                      static_cast<int>(center_.y()));

    // 十字标记
    int crossSize = 15;
    cv::line(result, cv::Point(centerPt.x - crossSize, centerPt.y),
            cv::Point(centerPt.x + crossSize, centerPt.y),
            cv::Scalar(0, 255, 0), 2);
    cv::line(result, cv::Point(centerPt.x, centerPt.y - crossSize),
            cv::Point(centerPt.x, centerPt.y + crossSize),
            cv::Scalar(0, 255, 0), 2);

    // 中心圆
    cv::circle(result, centerPt, 5, cv::Scalar(0, 0, 255), -1);

    // 如果是最小外接圆，绘制圆
    if (calcMethod_ == MinCircleCenter && radius_ > 0) {
        cv::circle(result, centerPt, static_cast<int>(radius_),
                  cv::Scalar(255, 255, 0), 2);
    }

    // 标注坐标
    QString text = QString("(%.1f, %.1f)").arg(center_.x()).arg(center_.y());
    cv::putText(result, text.toStdString(),
               cv::Point(centerPt.x + 10, centerPt.y - 10),
               cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1);

    return result;
}

QJsonObject CalcCenterTool::serializeParams() const
{
    QJsonObject json;
    json["calcMethod"] = static_cast<int>(calcMethod_);
    json["sourceType"] = static_cast<int>(sourceType_);
    json["threshold"] = threshold_;
    json["inverted"] = inverted_;
    return json;
}

void CalcCenterTool::deserializeParams(const QJsonObject& json)
{
    calcMethod_ = static_cast<CalcMethod>(json.value("calcMethod").toInt(static_cast<int>(Centroid)));
    sourceType_ = static_cast<SourceType>(json.value("sourceType").toInt(static_cast<int>(FromImage)));
    threshold_ = json.value("threshold").toInt(128);
    inverted_ = json.value("inverted").toBool(false);
}

void CalcCenterTool::setCalcMethod(CalcMethod method)
{
    if (calcMethod_ != method) {
        calcMethod_ = method;
        emit paramChanged();
    }
}

void CalcCenterTool::setSourceType(SourceType type)
{
    if (sourceType_ != type) {
        sourceType_ = type;
        emit paramChanged();
    }
}

void CalcCenterTool::setInputPoints(const std::vector<QPointF>& points)
{
    inputPoints_ = points;
    emit paramChanged();
}

void CalcCenterTool::setThreshold(int threshold)
{
    if (threshold_ != threshold) {
        threshold_ = threshold;
        emit paramChanged();
    }
}

void CalcCenterTool::setInverted(bool inverted)
{
    if (inverted_ != inverted) {
        inverted_ = inverted;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
