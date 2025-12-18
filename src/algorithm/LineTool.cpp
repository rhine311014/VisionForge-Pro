/**
 * @file LineTool.cpp
 * @brief 线检测工具实现
 */

#include "algorithm/LineTool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <algorithm>
#include <cmath>

#ifdef USE_HALCON
#include <HalconCpp.h>
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

LineTool::LineTool(QObject* parent)
    : VisionTool(parent)
    , backend_(Auto)
    , method_(HoughLinesP)
    , minLength_(50.0)
    , maxGap_(10.0)
    , maxCount_(20)
    , rho_(1.0)
    , theta_(1.0)
    , threshold_(50)
    , cannyThreshold1_(50.0)
    , cannyThreshold2_(150.0)
{
    setDisplayName(toolName());
}

bool LineTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
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

    bool success = false;
    lines_.clear();

    // 根据后端和方法选择处理方式
#ifdef USE_HALCON
    if (backend_ == Halcon || (backend_ == Auto && method_ == EdgeFit)) {
        success = processWithHalconEdgeFit(gray, output);
        if (success) {
            output.setValue("backend", "Halcon");
        }
    }
#endif

    if (!success) {
        switch (method_) {
        case HoughLinesP:
            success = processWithHoughLinesP(gray, output);
            break;
        case HoughLines:
            success = processWithHoughLines(gray, output);
            break;
        case ContourFit:
            success = processWithContourFit(gray, output);
            break;
        default:
            success = processWithHoughLinesP(gray, output);
            break;
        }
        if (success) {
            output.setValue("backend", "OpenCV");
        }
    }

    if (success) {
        // 限制数量
        if (maxCount_ > 0 && lines_.size() > static_cast<size_t>(maxCount_)) {
            lines_.resize(maxCount_);
        }

        output.executionTime = timer.elapsed();
        output.setValue("lineCount", static_cast<int>(lines_.size()));

        // 记录每条线的信息
        QJsonArray lineArray;
        for (const auto& line : lines_) {
            QJsonObject lineObj;
            lineObj["id"] = line.id;
            lineObj["startX"] = line.startPoint.x();
            lineObj["startY"] = line.startPoint.y();
            lineObj["endX"] = line.endPoint.x();
            lineObj["endY"] = line.endPoint.y();
            lineObj["angle"] = line.angle;
            lineObj["length"] = line.length;
            lineObj["score"] = line.score;
            lineArray.append(lineObj);
        }
        output.data["lines"] = lineArray;

        // 绘制结果
        cv::Mat resultImage = drawResults(src);
        output.outputImage = std::make_shared<Base::ImageData>(resultImage);

        setDebugImage(output.outputImage);
        setStatusText(QString("检测到 %1 条线").arg(lines_.size()));

        LOG_DEBUG(QString("线检测完成，检测到 %1 条线，耗时: %2ms")
                 .arg(lines_.size())
                 .arg(output.executionTime));
    }

    return success;
}

bool LineTool::processWithHoughLinesP(const cv::Mat& input, ToolResult& output)
{
    try {
        // Canny边缘检测
        cv::Mat edges;
        cv::Canny(input, edges, cannyThreshold1_, cannyThreshold2_);

        // 概率霍夫变换
        std::vector<cv::Vec4i> houghLines;
        double thetaRad = theta_ * M_PI / 180.0;
        cv::HoughLinesP(edges, houghLines, rho_, thetaRad, threshold_,
                       minLength_, maxGap_);

        // 转换结果
        lines_.reserve(houghLines.size());
        for (size_t i = 0; i < houghLines.size(); ++i) {
            QPointF start(houghLines[i][0], houghLines[i][1]);
            QPointF end(houghLines[i][2], houghLines[i][3]);

            LineResult result(static_cast<int>(i + 1), start, end, 1.0);
            lines_.push_back(result);
        }

        // 按长度排序（降序）
        std::sort(lines_.begin(), lines_.end(),
                 [](const LineResult& a, const LineResult& b) {
                     return a.length > b.length;
                 });

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("概率霍夫线检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool LineTool::processWithHoughLines(const cv::Mat& input, ToolResult& output)
{
    try {
        // Canny边缘检测
        cv::Mat edges;
        cv::Canny(input, edges, cannyThreshold1_, cannyThreshold2_);

        // 标准霍夫变换
        std::vector<cv::Vec2f> houghLines;
        double thetaRad = theta_ * M_PI / 180.0;
        cv::HoughLines(edges, houghLines, rho_, thetaRad, threshold_);

        // 图像边界
        double xMin = 0, yMin = 0;
        double xMax = input.cols - 1;
        double yMax = input.rows - 1;

        // 转换结果（标准霍夫返回的是极坐标形式，需要转换为端点）
        int lineId = 0;
        for (const auto& line : houghLines) {
            float rhoVal = line[0];
            float thetaVal = line[1];

            double cosTheta = std::cos(thetaVal);
            double sinTheta = std::sin(thetaVal);
            double x0 = cosTheta * rhoVal;
            double y0 = sinTheta * rhoVal;

            // 计算线段的两个端点（延伸到图像边界）
            double lineLen = std::max(input.cols, input.rows) * 2;
            double x1 = x0 - lineLen * sinTheta;
            double y1 = y0 + lineLen * cosTheta;
            double x2 = x0 + lineLen * sinTheta;
            double y2 = y0 - lineLen * cosTheta;

            // 使用Liang-Barsky算法裁剪到图像边界
            double dx = x2 - x1;
            double dy = y2 - y1;
            double tMin = 0.0, tMax = 1.0;

            // 定义裁剪边界的p和q值
            double p[4] = {-dx, dx, -dy, dy};
            double q[4] = {x1 - xMin, xMax - x1, y1 - yMin, yMax - y1};

            bool valid = true;
            for (int i = 0; i < 4; ++i) {
                if (std::abs(p[i]) < 1e-10) {
                    // 线段平行于裁剪边界
                    if (q[i] < 0) {
                        valid = false;
                        break;
                    }
                } else {
                    double t = q[i] / p[i];
                    if (p[i] < 0) {
                        tMin = std::max(tMin, t);
                    } else {
                        tMax = std::min(tMax, t);
                    }
                }
            }

            if (!valid || tMin > tMax) {
                // 线段完全在图像外
                continue;
            }

            // 计算裁剪后的端点
            QPointF start(x1 + tMin * dx, y1 + tMin * dy);
            QPointF end(x1 + tMax * dx, y1 + tMax * dy);

            LineResult result(++lineId, start, end, 1.0);
            result.angle = thetaVal * 180.0 / M_PI;

            // 过滤短线
            if (result.length < minLength_) continue;

            lines_.push_back(result);

            if (lineId >= maxCount_) break;
        }

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("标准霍夫线检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool LineTool::processWithContourFit(const cv::Mat& input, ToolResult& output)
{
    try {
        // Canny边缘检测
        cv::Mat edges;
        cv::Canny(input, edges, cannyThreshold1_, cannyThreshold2_);

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(edges, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        int lineId = 0;
        for (const auto& contour : contours) {
            if (contour.size() < 2) continue;

            // 拟合直线
            cv::Vec4f fitLine;
            cv::fitLine(contour, fitLine, cv::DIST_L2, 0, 0.01, 0.01);

            // 方向向量
            double vx = fitLine[0];
            double vy = fitLine[1];
            // 线上的点
            double x0 = fitLine[2];
            double y0 = fitLine[3];

            // 计算轮廓的范围
            cv::Rect boundRect = cv::boundingRect(contour);

            // 计算线段端点
            double t1 = -1000, t2 = 1000;
            for (const auto& pt : contour) {
                double t = (pt.x - x0) * vx + (pt.y - y0) * vy;
                t1 = std::max(t1, t);
                t2 = std::min(t2, t);
            }

            QPointF start(x0 + t2 * vx, y0 + t2 * vy);
            QPointF end(x0 + t1 * vx, y0 + t1 * vy);

            LineResult result(++lineId, start, end, 1.0);

            // 过滤短线
            if (result.length < minLength_) continue;

            lines_.push_back(result);
        }

        // 按长度排序（降序）
        std::sort(lines_.begin(), lines_.end(),
                 [](const LineResult& a, const LineResult& b) {
                     return a.length > b.length;
                 });

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("轮廓拟合线检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

#ifdef USE_HALCON
bool LineTool::processWithHalconEdgeFit(const cv::Mat& input, ToolResult& output)
{
    try {
        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", input.cols, input.rows, (void*)input.data);

        // 边缘检测获取亚像素轮廓
        HXLDCont edges = hImage.EdgesSubPix("canny", 1.0,
                                            cannyThreshold1_ / 5, cannyThreshold2_ / 5);

        // 分割轮廓
        HXLDCont segContours = edges.SegmentContoursXld("lines", 5, 4, 2);

        // 选择线段轮廓
        HTuple attribName("cont_approx");
        HTuple attribMin(1);  // 线段
        HTuple attribMax(1);
        HXLDCont lineContours = segContours.SelectContoursXld(attribName, attribMin, attribMax, 0, 0);

        // 获取轮廓数量
        Hlong count = lineContours.CountObj();

        int lineId = 0;
        for (Hlong i = 1; i <= count; ++i) {
            HXLDCont singleContour = lineContours.SelectObj(i);

            // 获取轮廓的起点和终点
            HTuple rowBegin, colBegin, rowEnd, colEnd;
            singleContour.GetContourXld(&rowBegin, &colBegin);

            if (rowBegin.Length() < 2) continue;

            // 取轮廓首尾点作为线段端点
            int lastIdx = rowBegin.Length() - 1;
            QPointF start(colBegin[0].D(), rowBegin[0].D());
            QPointF end(colBegin[lastIdx].D(), rowBegin[lastIdx].D());

            LineResult result(++lineId, start, end, 1.0);

            // 过滤短线
            if (result.length < minLength_) continue;

            lines_.push_back(result);
        }

        // 按长度排序（降序）
        std::sort(lines_.begin(), lines_.end(),
                 [](const LineResult& a, const LineResult& b) {
                     return a.length > b.length;
                 });

        output.success = true;
        return true;

    } catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("Halcon线检测失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}
#endif

cv::Mat LineTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    for (const auto& line : lines_) {
        cv::Point pt1(static_cast<int>(line.startPoint.x()),
                     static_cast<int>(line.startPoint.y()));
        cv::Point pt2(static_cast<int>(line.endPoint.x()),
                     static_cast<int>(line.endPoint.y()));

        // 绘制线段
        cv::line(result, pt1, pt2, cv::Scalar(0, 255, 0), 2);

        // 绘制端点
        cv::circle(result, pt1, 4, cv::Scalar(0, 0, 255), -1);
        cv::circle(result, pt2, 4, cv::Scalar(0, 0, 255), -1);

        // 绘制信息
        cv::Point midPt((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);
        QString text = QString("#%1 L:%.0f A:%.1f").arg(line.id).arg(line.length).arg(line.angle);
        cv::putText(result, text.toStdString(),
                   cv::Point(midPt.x + 5, midPt.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 255), 1);
    }

    return result;
}

QJsonObject LineTool::serializeParams() const
{
    QJsonObject json;
    json["backend"] = static_cast<int>(backend_);
    json["method"] = static_cast<int>(method_);
    json["minLength"] = minLength_;
    json["maxGap"] = maxGap_;
    json["maxCount"] = maxCount_;
    json["rho"] = rho_;
    json["theta"] = theta_;
    json["threshold"] = threshold_;
    json["cannyThreshold1"] = cannyThreshold1_;
    json["cannyThreshold2"] = cannyThreshold2_;
    return json;
}

void LineTool::deserializeParams(const QJsonObject& json)
{
    backend_ = static_cast<BackendType>(json.value("backend").toInt(static_cast<int>(Auto)));
    method_ = static_cast<DetectionMethod>(json.value("method").toInt(static_cast<int>(HoughLinesP)));
    minLength_ = json.value("minLength").toDouble(50.0);
    maxGap_ = json.value("maxGap").toDouble(10.0);
    maxCount_ = json.value("maxCount").toInt(20);
    rho_ = json.value("rho").toDouble(1.0);
    theta_ = json.value("theta").toDouble(1.0);
    threshold_ = json.value("threshold").toInt(50);
    cannyThreshold1_ = json.value("cannyThreshold1").toDouble(50.0);
    cannyThreshold2_ = json.value("cannyThreshold2").toDouble(150.0);
}

void LineTool::setBackend(BackendType backend)
{
    if (backend_ != backend) {
        backend_ = backend;
        emit paramChanged();
    }
}

void LineTool::setMethod(DetectionMethod method)
{
    if (method_ != method) {
        method_ = method;
        emit paramChanged();
    }
}

void LineTool::setMinLength(double length)
{
    if (qAbs(minLength_ - length) > 0.001) {
        minLength_ = length;
        emit paramChanged();
    }
}

void LineTool::setMaxGap(double gap)
{
    if (qAbs(maxGap_ - gap) > 0.001) {
        maxGap_ = gap;
        emit paramChanged();
    }
}

void LineTool::setMaxCount(int count)
{
    if (maxCount_ != count) {
        maxCount_ = count;
        emit paramChanged();
    }
}

void LineTool::setRho(double rho)
{
    if (qAbs(rho_ - rho) > 0.001) {
        rho_ = rho;
        emit paramChanged();
    }
}

void LineTool::setTheta(double theta)
{
    if (qAbs(theta_ - theta) > 0.001) {
        theta_ = theta;
        emit paramChanged();
    }
}

void LineTool::setThreshold(int threshold)
{
    if (threshold_ != threshold) {
        threshold_ = threshold;
        emit paramChanged();
    }
}

void LineTool::setCannyThreshold1(double threshold)
{
    if (qAbs(cannyThreshold1_ - threshold) > 0.001) {
        cannyThreshold1_ = threshold;
        emit paramChanged();
    }
}

void LineTool::setCannyThreshold2(double threshold)
{
    if (qAbs(cannyThreshold2_ - threshold) > 0.001) {
        cannyThreshold2_ = threshold;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
