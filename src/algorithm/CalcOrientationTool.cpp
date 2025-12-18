/**
 * @file CalcOrientationTool.cpp
 * @brief 方向计算工具实现
 */

#include "algorithm/CalcOrientationTool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

CalcOrientationTool::CalcOrientationTool(QObject* parent)
    : VisionTool(parent)
    , calcMethod_(Moments)
    , sourceType_(FromImage)
    , threshold_(128)
    , inverted_(false)
    , angleRange_(0)
    , angle_(0)
    , center_(0, 0)
    , majorAxisLength_(0)
    , minorAxisLength_(0)
{
    setDisplayName(toolName());
}

bool CalcOrientationTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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
        setStatusText(QString("角度: %.1f°").arg(angle_));

        LOG_DEBUG(QString("方向计算完成: %.1f°, 耗时: %2ms")
                 .arg(angle_)
                 .arg(output.executionTime));
    }

    return success;
}

bool CalcOrientationTool::calcFromImage(const cv::Mat& gray, ToolResult& output)
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
        output.errorMessage = QString("方向计算失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool CalcOrientationTool::calcFromContour(const std::vector<cv::Point>& contour, ToolResult& output)
{
    if (contour.size() < 5) {
        output.success = false;
        output.errorMessage = "轮廓点数不足（至少需要5个点）";
        return false;
    }

    try {
        switch (calcMethod_) {
        case PCA: {
            // 转换为浮点型
            std::vector<cv::Point2f> points;
            points.reserve(contour.size());
            for (const auto& pt : contour) {
                points.emplace_back(static_cast<float>(pt.x), static_cast<float>(pt.y));
            }
            calcPCA(points);
            break;
        }

        case MinAreaRect: {
            // 最小外接矩形
            cv::RotatedRect rect = cv::minAreaRect(contour);
            center_.setX(rect.center.x);
            center_.setY(rect.center.y);
            angle_ = rect.angle;

            // OpenCV的minAreaRect返回-90~0度，需要调整
            if (rect.size.width < rect.size.height) {
                angle_ += 90;
                majorAxisLength_ = rect.size.height;
                minorAxisLength_ = rect.size.width;
            } else {
                majorAxisLength_ = rect.size.width;
                minorAxisLength_ = rect.size.height;
            }
            break;
        }

        case Moments: {
            calcMoments(contour);
            break;
        }

        case FitEllipse: {
            // 拟合椭圆
            cv::RotatedRect ellipse = cv::fitEllipse(contour);
            center_.setX(ellipse.center.x);
            center_.setY(ellipse.center.y);
            angle_ = ellipse.angle;

            // 确保主轴是较长的轴
            if (ellipse.size.width > ellipse.size.height) {
                majorAxisLength_ = ellipse.size.width;
                minorAxisLength_ = ellipse.size.height;
                angle_ += 90;
            } else {
                majorAxisLength_ = ellipse.size.height;
                minorAxisLength_ = ellipse.size.width;
            }
            break;
        }

        default:
            calcMoments(contour);
            break;
        }

        // 归一化角度
        angle_ = normalizeAngle(angle_);

        output.success = true;
        output.setValue("angle", angle_);
        output.setValue("centerX", center_.x());
        output.setValue("centerY", center_.y());
        output.setValue("majorAxisLength", majorAxisLength_);
        output.setValue("minorAxisLength", minorAxisLength_);

        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("方向计算失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool CalcOrientationTool::calcFromPoints(ToolResult& output)
{
    if (inputPoints_.size() < 5) {
        output.success = false;
        output.errorMessage = "点数不足（至少需要5个点）";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    try {
        // 转换为OpenCV格式
        std::vector<cv::Point2f> cvPoints;
        cvPoints.reserve(inputPoints_.size());
        for (const auto& pt : inputPoints_) {
            cvPoints.emplace_back(static_cast<float>(pt.x()),
                                 static_cast<float>(pt.y()));
        }

        calcPCA(cvPoints);

        // 归一化角度
        angle_ = normalizeAngle(angle_);

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("angle", angle_);
        output.setValue("centerX", center_.x());
        output.setValue("centerY", center_.y());
        output.setValue("pointCount", static_cast<int>(inputPoints_.size()));

        setStatusText(QString("角度: %.1f°").arg(angle_));

        return true;

    } catch (const std::exception& e) {
        output.success = false;
        output.errorMessage = QString("方向计算失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

void CalcOrientationTool::calcPCA(const std::vector<cv::Point2f>& points)
{
    // 计算均值（中心点）
    double sumX = 0, sumY = 0;
    for (const auto& pt : points) {
        sumX += pt.x;
        sumY += pt.y;
    }
    center_.setX(sumX / points.size());
    center_.setY(sumY / points.size());

    // 计算协方差矩阵
    double cxx = 0, cyy = 0, cxy = 0;
    for (const auto& pt : points) {
        double dx = pt.x - center_.x();
        double dy = pt.y - center_.y();
        cxx += dx * dx;
        cyy += dy * dy;
        cxy += dx * dy;
    }

    double n = static_cast<double>(points.size());
    cxx /= n;
    cyy /= n;
    cxy /= n;

    // 计算特征值和特征向量
    double trace = cxx + cyy;
    double det = cxx * cyy - cxy * cxy;
    double discriminant = std::sqrt(std::max(0.0, trace * trace / 4.0 - det));

    double lambda1 = trace / 2.0 + discriminant;  // 较大特征值
    double lambda2 = trace / 2.0 - discriminant;  // 较小特征值

    majorAxisLength_ = 2.0 * std::sqrt(std::max(0.0, lambda1));
    minorAxisLength_ = 2.0 * std::sqrt(std::max(0.0, lambda2));

    // 主轴方向（对应较大特征值的特征向量）
    if (std::abs(cxy) > 1e-6) {
        angle_ = std::atan2(lambda1 - cxx, cxy) * 180.0 / M_PI;
    } else {
        angle_ = (cxx >= cyy) ? 0.0 : 90.0;
    }
}

void CalcOrientationTool::calcMoments(const std::vector<cv::Point>& contour)
{
    cv::Moments m = cv::moments(contour);

    if (m.m00 > 0) {
        center_.setX(m.m10 / m.m00);
        center_.setY(m.m01 / m.m00);
    }

    // 使用中心矩计算方向
    double mu20 = m.mu20;
    double mu02 = m.mu02;
    double mu11 = m.mu11;

    // 主轴方向
    angle_ = 0.5 * std::atan2(2 * mu11, mu20 - mu02) * 180.0 / M_PI;

    // 估算主轴长度（基于中心矩）
    double trace = mu20 + mu02;
    double det = mu20 * mu02 - mu11 * mu11;
    double discriminant = std::sqrt(std::max(0.0, trace * trace / 4.0 - det));

    double lambda1 = (trace / 2.0 + discriminant) / m.m00;
    double lambda2 = (trace / 2.0 - discriminant) / m.m00;

    majorAxisLength_ = 4.0 * std::sqrt(std::max(0.0, lambda1));
    minorAxisLength_ = 4.0 * std::sqrt(std::max(0.0, lambda2));
}

double CalcOrientationTool::normalizeAngle(double angle) const
{
    switch (angleRange_) {
    case 0:  // 0~360
        while (angle < 0) angle += 360;
        while (angle >= 360) angle -= 360;
        break;

    case 1:  // -180~180
        while (angle < -180) angle += 360;
        while (angle >= 180) angle -= 360;
        break;

    case 2:  // 0~180
        while (angle < 0) angle += 180;
        while (angle >= 180) angle -= 180;
        break;
    }
    return angle;
}

cv::Mat CalcOrientationTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    cv::Point centerPt(static_cast<int>(center_.x()),
                      static_cast<int>(center_.y()));

    // 绘制中心点
    cv::circle(result, centerPt, 5, cv::Scalar(0, 0, 255), -1);

    // 绘制主轴
    double angleRad = angle_ * M_PI / 180.0;
    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    int majorLen = static_cast<int>(majorAxisLength_ / 2);
    int minorLen = static_cast<int>(minorAxisLength_ / 2);

    // 主轴（绿色）
    cv::Point major1(centerPt.x + static_cast<int>(majorLen * cosA),
                    centerPt.y + static_cast<int>(majorLen * sinA));
    cv::Point major2(centerPt.x - static_cast<int>(majorLen * cosA),
                    centerPt.y - static_cast<int>(majorLen * sinA));
    cv::line(result, major1, major2, cv::Scalar(0, 255, 0), 2);
    cv::arrowedLine(result, centerPt, major1, cv::Scalar(0, 255, 0), 2, cv::LINE_AA, 0, 0.2);

    // 次轴（蓝色）
    cv::Point minor1(centerPt.x + static_cast<int>(minorLen * (-sinA)),
                    centerPt.y + static_cast<int>(minorLen * cosA));
    cv::Point minor2(centerPt.x - static_cast<int>(minorLen * (-sinA)),
                    centerPt.y - static_cast<int>(minorLen * cosA));
    cv::line(result, minor1, minor2, cv::Scalar(255, 0, 0), 1);

    // 标注角度
    QString text = QString("%.1f°").arg(angle_);
    cv::putText(result, text.toStdString(),
               cv::Point(centerPt.x + 10, centerPt.y - 10),
               cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);

    return result;
}

QJsonObject CalcOrientationTool::serializeParams() const
{
    QJsonObject json;
    json["calcMethod"] = static_cast<int>(calcMethod_);
    json["sourceType"] = static_cast<int>(sourceType_);
    json["threshold"] = threshold_;
    json["inverted"] = inverted_;
    json["angleRange"] = angleRange_;
    return json;
}

void CalcOrientationTool::deserializeParams(const QJsonObject& json)
{
    calcMethod_ = static_cast<CalcMethod>(json.value("calcMethod").toInt(static_cast<int>(Moments)));
    sourceType_ = static_cast<SourceType>(json.value("sourceType").toInt(static_cast<int>(FromImage)));
    threshold_ = json.value("threshold").toInt(128);
    inverted_ = json.value("inverted").toBool(false);
    angleRange_ = json.value("angleRange").toInt(0);
}

void CalcOrientationTool::setCalcMethod(CalcMethod method)
{
    if (calcMethod_ != method) {
        calcMethod_ = method;
        emit paramChanged();
    }
}

void CalcOrientationTool::setSourceType(SourceType type)
{
    if (sourceType_ != type) {
        sourceType_ = type;
        emit paramChanged();
    }
}

void CalcOrientationTool::setInputPoints(const std::vector<QPointF>& points)
{
    inputPoints_ = points;
    emit paramChanged();
}

void CalcOrientationTool::setThreshold(int threshold)
{
    if (threshold_ != threshold) {
        threshold_ = threshold;
        emit paramChanged();
    }
}

void CalcOrientationTool::setInverted(bool inverted)
{
    if (inverted_ != inverted) {
        inverted_ = inverted;
        emit paramChanged();
    }
}

void CalcOrientationTool::setAngleRange(int range)
{
    if (angleRange_ != range) {
        angleRange_ = range;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
