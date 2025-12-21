/**
 * @file FindEdgeTool.cpp
 * @brief 边缘查找工具实现
 */

#include "algorithm/FindEdgeTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <cmath>
#include <algorithm>

namespace VisionForge {
namespace Algorithm {

FindEdgeTool::FindEdgeTool(QObject* parent)
    : VisionTool(parent)
    , searchMode_(LineSearch)
    , lineStart_(100, 100)
    , lineEnd_(400, 100)
    , searchWidth_(10)
    , searchRect_(100, 100, 300, 50)
    , searchAngle_(0)
    , edgeType_(AllEdges)
    , selectMode_(FirstEdge)
    , minAmplitude_(20.0)
    , sigma_(1.0)
    , subPixelEnabled_(true)  // 默认启用亚像素
    , subPixelMethod_(SubPixelMethod::QuadraticFit)
{
    setDisplayName(toolName());
}

bool FindEdgeTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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

    // 高斯平滑
    if (sigma_ > 0) {
        int ksize = static_cast<int>(std::ceil(sigma_ * 3)) * 2 + 1;
        cv::GaussianBlur(gray, gray, cv::Size(ksize, ksize), sigma_);
    }

    edgePoints_.clear();

    bool success = false;
    switch (searchMode_) {
    case LineSearch:
        success = findEdgesAlongLine(gray, output);
        break;
    case RectSearch:
        success = findEdgesInRect(gray, output);
        break;
    }

    if (success) {
        output.executionTime = timer.elapsed();
        output.setValue("edgeCount", edgeCount());

        // 记录边缘点信息
        QJsonArray edgeArray;
        for (const auto& edge : edgePoints_) {
            QJsonObject edgeObj;
            edgeObj["id"] = edge.id;
            edgeObj["x"] = edge.position.x();
            edgeObj["y"] = edge.position.y();
            edgeObj["amplitude"] = edge.amplitude;
            edgeObj["direction"] = edge.direction;
            edgeObj["isRising"] = edge.isRising;
            edgeArray.append(edgeObj);
        }
        output.data["edges"] = edgeArray;

        // 主边缘点
        if (!edgePoints_.empty()) {
            EdgePoint primary = primaryEdge();
            output.setValue("primaryX", primary.position.x());
            output.setValue("primaryY", primary.position.y());
            output.setValue("primaryAmplitude", primary.amplitude);
        }

        // 绘制结果
        cv::Mat resultImage = drawResults(src);
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }

        setDebugImage(output.outputImage);
        setStatusText(QString("找到 %1 个边缘点").arg(edgeCount()));

        LOG_DEBUG(QString("边缘查找完成，找到 %1 个边缘点，耗时: %2ms")
                 .arg(edgeCount())
                 .arg(output.executionTime));
    }

    return success;
}

bool FindEdgeTool::findEdgesAlongLine(const cv::Mat& gray, ToolResult& output)
{
    try {
        // 提取剖面
        std::vector<double> profile = extractProfile(gray, lineStart_, lineEnd_, searchWidth_);

        if (profile.empty()) {
            output.success = false;
            output.errorMessage = "无法提取剖面";
            return false;
        }

        // 计算搜索方向
        double dx = lineEnd_.x() - lineStart_.x();
        double dy = lineEnd_.y() - lineStart_.y();
        double len = std::sqrt(dx * dx + dy * dy);
        QPointF dir(dx / len, dy / len);

        // 检测边缘
        edgePoints_ = detect1DEdges(profile, lineStart_, dir);

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("边缘查找失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool FindEdgeTool::findEdgesInRect(const cv::Mat& gray, ToolResult& output)
{
    try {
        // 矩形区域转换为搜索线
        double cx = searchRect_.center().x();
        double cy = searchRect_.center().y();
        double w = searchRect_.width();
        double h = searchRect_.height();

        double angleRad = searchAngle_ * M_PI / 180.0;
        double cosA = std::cos(angleRad);
        double sinA = std::sin(angleRad);

        // 沿矩形长边方向搜索
        QPointF start(cx - w / 2 * cosA, cy - w / 2 * sinA);
        QPointF end(cx + w / 2 * cosA, cy + w / 2 * sinA);

        // 提取剖面（宽度为矩形高度）
        std::vector<double> profile = extractProfile(gray, start, end, static_cast<int>(h));

        if (profile.empty()) {
            output.success = false;
            output.errorMessage = "无法提取剖面";
            return false;
        }

        // 计算搜索方向
        QPointF dir(cosA, sinA);

        // 检测边缘
        edgePoints_ = detect1DEdges(profile, start, dir);

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("边缘查找失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

std::vector<double> FindEdgeTool::extractProfile(const cv::Mat& gray,
                                                  const QPointF& start, const QPointF& end,
                                                  int width)
{
    std::vector<double> profile;

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();
    double len = std::sqrt(dx * dx + dy * dy);

    if (len < 1) return profile;

    // 归一化方向
    double dirX = dx / len;
    double dirY = dy / len;

    // 垂直方向
    double perpX = -dirY;
    double perpY = dirX;

    int numSamples = static_cast<int>(len);
    profile.reserve(numSamples);

    for (int i = 0; i < numSamples; ++i) {
        double t = static_cast<double>(i) / numSamples;
        double cx = start.x() + t * dx;
        double cy = start.y() + t * dy;

        // 垂直方向平均
        double sum = 0;
        int count = 0;

        for (int w = -width / 2; w <= width / 2; ++w) {
            int px = static_cast<int>(cx + w * perpX);
            int py = static_cast<int>(cy + w * perpY);

            if (px >= 0 && px < gray.cols && py >= 0 && py < gray.rows) {
                sum += gray.at<uchar>(py, px);
                ++count;
            }
        }

        if (count > 0) {
            profile.push_back(sum / count);
        }
    }

    return profile;
}

std::vector<EdgePoint> FindEdgeTool::detect1DEdges(const std::vector<double>& profile,
                                                    const QPointF& start, const QPointF& dir)
{
    std::vector<EdgePoint> edges;

    if (profile.size() < 3) return edges;

    // 计算一阶导数
    std::vector<double> derivative(profile.size() - 1);
    for (size_t i = 0; i < profile.size() - 1; ++i) {
        derivative[i] = profile[i + 1] - profile[i];
    }

    // 查找极值点
    int edgeId = 0;
    for (size_t i = 1; i < derivative.size() - 1; ++i) {
        double prev = derivative[i - 1];
        double curr = derivative[i];
        double next = derivative[i + 1];

        bool isMax = (curr > prev && curr > next);
        bool isMin = (curr < prev && curr < next);

        if (!isMax && !isMin) continue;

        double amplitude = std::abs(curr);
        if (amplitude < minAmplitude_) continue;

        bool isRising = (curr > 0);

        // 根据边缘类型过滤
        if (edgeType_ == RisingEdge && !isRising) continue;
        if (edgeType_ == FallingEdge && isRising) continue;

        // 亚像素精度定位（抛物线拟合）
        double subPixelOffset = 0;
        double denom = prev - 2 * curr + next;
        if (std::abs(denom) > 1e-6) {
            subPixelOffset = 0.5 * (prev - next) / denom;
        }

        double pos = i + 0.5 + subPixelOffset;
        double x = start.x() + pos * dir.x();
        double y = start.y() + pos * dir.y();

        double direction = std::atan2(dir.y(), dir.x()) * 180.0 / M_PI;
        if (!isRising) direction += 180;
        if (direction < 0) direction += 360;
        if (direction >= 360) direction -= 360;

        edges.emplace_back(++edgeId, QPointF(x, y), amplitude, direction, isRising);
    }

    return edges;
}

EdgePoint FindEdgeTool::primaryEdge() const
{
    if (edgePoints_.empty()) {
        return EdgePoint();
    }

    switch (selectMode_) {
    case FirstEdge:
        return edgePoints_.front();

    case LastEdge:
        return edgePoints_.back();

    case StrongestEdge: {
        auto it = std::max_element(edgePoints_.begin(), edgePoints_.end(),
                                   [](const EdgePoint& a, const EdgePoint& b) {
                                       return a.amplitude < b.amplitude;
                                   });
        return *it;
    }

    case AllEdgesFound:
    default:
        return edgePoints_.front();
    }
}

cv::Mat FindEdgeTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 绘制搜索区域
    if (searchMode_ == LineSearch) {
        cv::line(result,
                cv::Point(static_cast<int>(lineStart_.x()), static_cast<int>(lineStart_.y())),
                cv::Point(static_cast<int>(lineEnd_.x()), static_cast<int>(lineEnd_.y())),
                cv::Scalar(255, 255, 0), 1);
    } else {
        // 绘制矩形
        cv::RotatedRect rr(
            cv::Point2f(static_cast<float>(searchRect_.center().x()),
                       static_cast<float>(searchRect_.center().y())),
            cv::Size2f(static_cast<float>(searchRect_.width()),
                      static_cast<float>(searchRect_.height())),
            static_cast<float>(searchAngle_));
        cv::Point2f vertices[4];
        rr.points(vertices);
        for (int i = 0; i < 4; ++i) {
            cv::line(result, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 255, 0), 1);
        }
    }

    // 绘制边缘点
    for (const auto& edge : edgePoints_) {
        cv::Point pt(static_cast<int>(edge.position.x()),
                    static_cast<int>(edge.position.y()));

        // 边缘点颜色：上升沿绿色，下降沿红色
        cv::Scalar color = edge.isRising ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
        cv::circle(result, pt, 5, color, -1);
        cv::circle(result, pt, 5, cv::Scalar(255, 255, 255), 1);

        // 绘制方向箭头
        double dirRad = edge.direction * M_PI / 180.0;
        cv::Point arrowEnd(pt.x + static_cast<int>(15 * std::cos(dirRad)),
                          pt.y + static_cast<int>(15 * std::sin(dirRad)));
        cv::arrowedLine(result, pt, arrowEnd, color, 2, cv::LINE_AA, 0, 0.3);
    }

    // 高亮主边缘点
    if (!edgePoints_.empty()) {
        EdgePoint primary = primaryEdge();
        cv::Point pt(static_cast<int>(primary.position.x()),
                    static_cast<int>(primary.position.y()));
        cv::circle(result, pt, 8, cv::Scalar(0, 255, 255), 2);
    }

    return result;
}

QJsonObject FindEdgeTool::serializeParams() const
{
    QJsonObject json;
    json["searchMode"] = static_cast<int>(searchMode_);
    json["lineStartX"] = lineStart_.x();
    json["lineStartY"] = lineStart_.y();
    json["lineEndX"] = lineEnd_.x();
    json["lineEndY"] = lineEnd_.y();
    json["searchWidth"] = searchWidth_;
    json["rectX"] = searchRect_.x();
    json["rectY"] = searchRect_.y();
    json["rectWidth"] = searchRect_.width();
    json["rectHeight"] = searchRect_.height();
    json["searchAngle"] = searchAngle_;
    json["edgeType"] = static_cast<int>(edgeType_);
    json["selectMode"] = static_cast<int>(selectMode_);
    json["minAmplitude"] = minAmplitude_;
    json["sigma"] = sigma_;
    json["subPixelEnabled"] = subPixelEnabled_;
    json["subPixelMethod"] = static_cast<int>(subPixelMethod_);
    return json;
}

void FindEdgeTool::deserializeParams(const QJsonObject& json)
{
    searchMode_ = static_cast<SearchMode>(json.value("searchMode").toInt(static_cast<int>(LineSearch)));
    lineStart_.setX(json.value("lineStartX").toDouble(100));
    lineStart_.setY(json.value("lineStartY").toDouble(100));
    lineEnd_.setX(json.value("lineEndX").toDouble(400));
    lineEnd_.setY(json.value("lineEndY").toDouble(100));
    searchWidth_ = json.value("searchWidth").toInt(10);
    searchRect_.setX(json.value("rectX").toDouble(100));
    searchRect_.setY(json.value("rectY").toDouble(100));
    searchRect_.setWidth(json.value("rectWidth").toDouble(300));
    searchRect_.setHeight(json.value("rectHeight").toDouble(50));
    searchAngle_ = json.value("searchAngle").toDouble(0);
    edgeType_ = static_cast<EdgeType>(json.value("edgeType").toInt(static_cast<int>(AllEdges)));
    selectMode_ = static_cast<SelectMode>(json.value("selectMode").toInt(static_cast<int>(FirstEdge)));
    minAmplitude_ = json.value("minAmplitude").toDouble(20.0);
    sigma_ = json.value("sigma").toDouble(1.0);
    subPixelEnabled_ = json.value("subPixelEnabled").toBool(true);
    subPixelMethod_ = static_cast<SubPixelMethod>(
        json.value("subPixelMethod").toInt(static_cast<int>(SubPixelMethod::QuadraticFit)));
}

void FindEdgeTool::setSearchMode(SearchMode mode)
{
    if (searchMode_ != mode) {
        searchMode_ = mode;
        emit paramChanged();
    }
}

void FindEdgeTool::setSearchLine(const QPointF& start, const QPointF& end)
{
    if (lineStart_ != start || lineEnd_ != end) {
        lineStart_ = start;
        lineEnd_ = end;
        emit paramChanged();
    }
}

void FindEdgeTool::setSearchWidth(int width)
{
    if (searchWidth_ != width) {
        searchWidth_ = std::max(1, width);
        emit paramChanged();
    }
}

void FindEdgeTool::setSearchRect(const QRectF& rect, double angle)
{
    if (searchRect_ != rect || searchAngle_ != angle) {
        searchRect_ = rect;
        searchAngle_ = angle;
        emit paramChanged();
    }
}

void FindEdgeTool::setEdgeType(EdgeType type)
{
    if (edgeType_ != type) {
        edgeType_ = type;
        emit paramChanged();
    }
}

void FindEdgeTool::setSelectMode(SelectMode mode)
{
    if (selectMode_ != mode) {
        selectMode_ = mode;
        emit paramChanged();
    }
}

void FindEdgeTool::setMinAmplitude(double amplitude)
{
    if (qAbs(minAmplitude_ - amplitude) > 0.001) {
        minAmplitude_ = amplitude;
        emit paramChanged();
    }
}

void FindEdgeTool::setSigma(double sigma)
{
    if (qAbs(sigma_ - sigma) > 0.001) {
        sigma_ = sigma;
        emit paramChanged();
    }
}

void FindEdgeTool::setSubPixelEnabled(bool enabled)
{
    if (subPixelEnabled_ != enabled) {
        subPixelEnabled_ = enabled;
        emit paramChanged();
    }
}

void FindEdgeTool::setSubPixelMethod(SubPixelMethod method)
{
    if (subPixelMethod_ != method) {
        subPixelMethod_ = method;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
