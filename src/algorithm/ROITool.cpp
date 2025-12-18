/**
 * @file ROITool.cpp
 * @brief ROI区域工具实现
 */

#include "algorithm/ROITool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

// ========== ROIShape静态方法 ==========

std::shared_ptr<ROIShape> ROIShape::fromJson(const QJsonObject& json)
{
    int typeInt = json.value("type").toInt(0);
    Type type = static_cast<Type>(typeInt);
    std::shared_ptr<ROIShape> shape;

    switch (type) {
    case Rectangle: {
        auto rect = std::make_shared<RectROI>();
        rect->setRect(QRectF(
            json.value("x").toDouble(),
            json.value("y").toDouble(),
            json.value("width").toDouble(),
            json.value("height").toDouble()
        ));
        shape = rect;
        break;
    }
    case Circle: {
        auto circle = std::make_shared<CircleROI>();
        circle->setCenter(QPointF(
            json.value("centerX").toDouble(),
            json.value("centerY").toDouble()
        ));
        circle->setRadius(json.value("radius").toDouble());
        shape = circle;
        break;
    }
    case Ellipse: {
        auto ellipse = std::make_shared<EllipseROI>();
        ellipse->setCenter(QPointF(
            json.value("centerX").toDouble(),
            json.value("centerY").toDouble()
        ));
        ellipse->setRadiusX(json.value("radiusX").toDouble());
        ellipse->setRadiusY(json.value("radiusY").toDouble());
        ellipse->setAngle(json.value("angle").toDouble());
        shape = ellipse;
        break;
    }
    case Polygon: {
        auto polygon = std::make_shared<PolygonROI>();
        QJsonArray pointsArray = json.value("points").toArray();
        QPolygonF poly;
        for (const auto& pointVal : pointsArray) {
            QJsonObject pt = pointVal.toObject();
            poly.append(QPointF(pt.value("x").toDouble(), pt.value("y").toDouble()));
        }
        polygon->setPolygon(poly);
        shape = polygon;
        break;
    }
    case RotatedRect: {
        auto rotRect = std::make_shared<RotatedRectROI>();
        rotRect->setCenter(QPointF(
            json.value("centerX").toDouble(),
            json.value("centerY").toDouble()
        ));
        rotRect->setWidth(json.value("width").toDouble());
        rotRect->setHeight(json.value("height").toDouble());
        rotRect->setAngle(json.value("angle").toDouble());
        shape = rotRect;
        break;
    }
    }

    if (shape) {
        shape->setEnabled(json.value("enabled").toBool(true));
        shape->setName(json.value("name").toString());
    }

    return shape;
}

// ========== RectROI ==========

cv::Mat RectROI::createMask(const cv::Size& imageSize) const
{
    cv::Mat mask = cv::Mat::zeros(imageSize, CV_8UC1);
    cv::Rect cvRect(
        static_cast<int>(rect_.x()),
        static_cast<int>(rect_.y()),
        static_cast<int>(rect_.width()),
        static_cast<int>(rect_.height())
    );

    // 限制在图像范围内
    cvRect &= cv::Rect(0, 0, imageSize.width, imageSize.height);

    if (cvRect.width > 0 && cvRect.height > 0) {
        mask(cvRect) = 255;
    }
    return mask;
}

QJsonObject RectROI::toJson() const
{
    QJsonObject json;
    json["type"] = static_cast<int>(type_);
    json["enabled"] = enabled_;
    json["name"] = name_;
    json["x"] = rect_.x();
    json["y"] = rect_.y();
    json["width"] = rect_.width();
    json["height"] = rect_.height();
    return json;
}

// ========== CircleROI ==========

cv::Mat CircleROI::createMask(const cv::Size& imageSize) const
{
    cv::Mat mask = cv::Mat::zeros(imageSize, CV_8UC1);
    cv::circle(mask,
              cv::Point(static_cast<int>(center_.x()), static_cast<int>(center_.y())),
              static_cast<int>(radius_),
              cv::Scalar(255),
              -1);  // -1表示填充
    return mask;
}

QJsonObject CircleROI::toJson() const
{
    QJsonObject json;
    json["type"] = static_cast<int>(type_);
    json["enabled"] = enabled_;
    json["name"] = name_;
    json["centerX"] = center_.x();
    json["centerY"] = center_.y();
    json["radius"] = radius_;
    return json;
}

// ========== EllipseROI ==========

QRectF EllipseROI::boundingRect() const
{
    // 对于旋转椭圆，计算外接矩形
    double angleRad = angle_ * M_PI / 180.0;
    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    double a = radiusX_ * cosA;
    double b = radiusY_ * sinA;
    double c = radiusX_ * sinA;
    double d = radiusY_ * cosA;

    double halfW = std::sqrt(a * a + b * b);
    double halfH = std::sqrt(c * c + d * d);

    return QRectF(center_.x() - halfW, center_.y() - halfH, halfW * 2, halfH * 2);
}

bool EllipseROI::contains(const QPointF& point) const
{
    // 将点转换到椭圆坐标系
    double dx = point.x() - center_.x();
    double dy = point.y() - center_.y();

    double angleRad = -angle_ * M_PI / 180.0;
    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    double x = dx * cosA - dy * sinA;
    double y = dx * sinA + dy * cosA;

    // 检查是否在椭圆内
    return (x * x) / (radiusX_ * radiusX_) + (y * y) / (radiusY_ * radiusY_) <= 1.0;
}

cv::Mat EllipseROI::createMask(const cv::Size& imageSize) const
{
    cv::Mat mask = cv::Mat::zeros(imageSize, CV_8UC1);
    cv::ellipse(mask,
               cv::Point(static_cast<int>(center_.x()), static_cast<int>(center_.y())),
               cv::Size(static_cast<int>(radiusX_), static_cast<int>(radiusY_)),
               angle_,
               0, 360,
               cv::Scalar(255),
               -1);
    return mask;
}

QJsonObject EllipseROI::toJson() const
{
    QJsonObject json;
    json["type"] = static_cast<int>(type_);
    json["enabled"] = enabled_;
    json["name"] = name_;
    json["centerX"] = center_.x();
    json["centerY"] = center_.y();
    json["radiusX"] = radiusX_;
    json["radiusY"] = radiusY_;
    json["angle"] = angle_;
    return json;
}

// ========== PolygonROI ==========

PolygonROI::PolygonROI(const std::vector<QPointF>& points)
    : ROIShape(Polygon)
{
    for (const auto& pt : points) {
        polygon_.append(pt);
    }
}

cv::Mat PolygonROI::createMask(const cv::Size& imageSize) const
{
    cv::Mat mask = cv::Mat::zeros(imageSize, CV_8UC1);

    if (polygon_.size() < 3) {
        return mask;
    }

    std::vector<cv::Point> points;
    points.reserve(polygon_.size());
    for (int i = 0; i < polygon_.size(); ++i) {
        points.emplace_back(
            static_cast<int>(polygon_[i].x()),
            static_cast<int>(polygon_[i].y())
        );
    }

    std::vector<std::vector<cv::Point>> contours = { points };
    cv::fillPoly(mask, contours, cv::Scalar(255));

    return mask;
}

QJsonObject PolygonROI::toJson() const
{
    QJsonObject json;
    json["type"] = static_cast<int>(type_);
    json["enabled"] = enabled_;
    json["name"] = name_;

    QJsonArray pointsArray;
    for (int i = 0; i < polygon_.size(); ++i) {
        QJsonObject pt;
        pt["x"] = polygon_[i].x();
        pt["y"] = polygon_[i].y();
        pointsArray.append(pt);
    }
    json["points"] = pointsArray;

    return json;
}

void PolygonROI::scale(double factor)
{
    QPointF center = polygon_.boundingRect().center();
    for (int i = 0; i < polygon_.size(); ++i) {
        QPointF pt = polygon_[i];
        pt.setX(center.x() + (pt.x() - center.x()) * factor);
        pt.setY(center.y() + (pt.y() - center.y()) * factor);
        polygon_[i] = pt;
    }
}

// ========== RotatedRectROI ==========

std::vector<QPointF> RotatedRectROI::vertices() const
{
    double angleRad = angle_ * M_PI / 180.0;
    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    double halfW = width_ / 2;
    double halfH = height_ / 2;

    std::vector<QPointF> verts(4);

    // 四个角点相对于中心的偏移
    double offsets[4][2] = {
        { -halfW, -halfH },
        {  halfW, -halfH },
        {  halfW,  halfH },
        { -halfW,  halfH }
    };

    for (int i = 0; i < 4; ++i) {
        double x = offsets[i][0] * cosA - offsets[i][1] * sinA;
        double y = offsets[i][0] * sinA + offsets[i][1] * cosA;
        verts[i] = QPointF(center_.x() + x, center_.y() + y);
    }

    return verts;
}

QRectF RotatedRectROI::boundingRect() const
{
    auto verts = vertices();
    double minX = verts[0].x(), maxX = verts[0].x();
    double minY = verts[0].y(), maxY = verts[0].y();

    for (int i = 1; i < 4; ++i) {
        minX = std::min(minX, verts[i].x());
        maxX = std::max(maxX, verts[i].x());
        minY = std::min(minY, verts[i].y());
        maxY = std::max(maxY, verts[i].y());
    }

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

bool RotatedRectROI::contains(const QPointF& point) const
{
    // 将点转换到矩形坐标系
    double dx = point.x() - center_.x();
    double dy = point.y() - center_.y();

    double angleRad = -angle_ * M_PI / 180.0;
    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    double x = dx * cosA - dy * sinA;
    double y = dx * sinA + dy * cosA;

    return std::abs(x) <= width_ / 2 && std::abs(y) <= height_ / 2;
}

cv::Mat RotatedRectROI::createMask(const cv::Size& imageSize) const
{
    cv::Mat mask = cv::Mat::zeros(imageSize, CV_8UC1);

    auto verts = vertices();
    std::vector<cv::Point> points;
    points.reserve(4);
    for (const auto& v : verts) {
        points.emplace_back(static_cast<int>(v.x()), static_cast<int>(v.y()));
    }

    std::vector<std::vector<cv::Point>> contours = { points };
    cv::fillPoly(mask, contours, cv::Scalar(255));

    return mask;
}

QJsonObject RotatedRectROI::toJson() const
{
    QJsonObject json;
    json["type"] = static_cast<int>(type_);
    json["enabled"] = enabled_;
    json["name"] = name_;
    json["centerX"] = center_.x();
    json["centerY"] = center_.y();
    json["width"] = width_;
    json["height"] = height_;
    json["angle"] = angle_;
    return json;
}

// ========== ROICollection ==========

void ROICollection::addROI(const ROIPtr& roi)
{
    if (roi) {
        rois_.push_back(roi);
    }
}

void ROICollection::removeROI(int index)
{
    if (index >= 0 && index < static_cast<int>(rois_.size())) {
        rois_.erase(rois_.begin() + index);
    }
}

void ROICollection::clear()
{
    rois_.clear();
}

ROICollection::ROIPtr ROICollection::getROI(int index) const
{
    if (index >= 0 && index < static_cast<int>(rois_.size())) {
        return rois_[index];
    }
    return nullptr;
}

cv::Mat ROICollection::createCombinedMask(const cv::Size& imageSize, const QString& combineMode) const
{
    if (rois_.empty()) {
        return cv::Mat::ones(imageSize, CV_8UC1) * 255;  // 没有ROI时，整个图像都是有效区域
    }

    cv::Mat combinedMask;
    bool first = true;

    for (const auto& roi : rois_) {
        if (!roi->isEnabled()) {
            continue;
        }

        cv::Mat mask = roi->createMask(imageSize);

        if (first) {
            combinedMask = mask.clone();
            first = false;
        } else {
            if (combineMode == "or" || combineMode == "union") {
                cv::bitwise_or(combinedMask, mask, combinedMask);
            } else if (combineMode == "and" || combineMode == "intersection") {
                cv::bitwise_and(combinedMask, mask, combinedMask);
            } else if (combineMode == "xor") {
                cv::bitwise_xor(combinedMask, mask, combinedMask);
            } else {
                cv::bitwise_or(combinedMask, mask, combinedMask);  // 默认并集
            }
        }
    }

    if (combinedMask.empty()) {
        return cv::Mat::ones(imageSize, CV_8UC1) * 255;
    }

    return combinedMask;
}

QJsonArray ROICollection::toJson() const
{
    QJsonArray array;
    for (const auto& roi : rois_) {
        if (roi) {
            array.append(roi->toJson());
        }
    }
    return array;
}

void ROICollection::fromJson(const QJsonArray& jsonArray)
{
    clear();
    for (const auto& item : jsonArray) {
        auto roi = ROIShape::fromJson(item.toObject());
        if (roi) {
            addROI(roi);
        }
    }
}

// ========== ROITool ==========

ROITool::ROITool(QObject* parent)
    : VisionTool(parent)
    , combineMode_(Union)
    , invertMask_(false)
    , cropOutput_(false)
{
    setDisplayName(toolName());
}

bool ROITool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    cv::Mat src = input->mat();
    cv::Size imageSize = src.size();

    // 生成组合mask
    QString modeStr;
    switch (combineMode_) {
    case Union: modeStr = "or"; break;
    case Intersection: modeStr = "and"; break;
    case XOR: modeStr = "xor"; break;
    }

    lastMask_ = rois_.createCombinedMask(imageSize, modeStr);

    // 反转mask
    if (invertMask_) {
        cv::bitwise_not(lastMask_, lastMask_);
    }

    cv::Mat result;

    if (cropOutput_) {
        // 裁剪到mask的外接矩形
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(lastMask_.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        if (!contours.empty()) {
            cv::Rect boundingBox;
            for (const auto& contour : contours) {
                boundingBox |= cv::boundingRect(contour);
            }

            // 确保在图像范围内
            boundingBox &= cv::Rect(0, 0, src.cols, src.rows);

            if (boundingBox.width > 0 && boundingBox.height > 0) {
                result = src(boundingBox).clone();

                // 应用mask
                cv::Mat roiMask = lastMask_(boundingBox);
                cv::Mat masked;
                result.copyTo(masked, roiMask);
                result = masked;
            } else {
                result = src.clone();
            }
        } else {
            result = src.clone();
        }
    } else {
        // 应用mask但不裁剪
        src.copyTo(result, lastMask_);
    }

    output.outputImage = std::make_shared<Base::ImageData>(result);
    output.success = true;
    output.executionTime = timer.elapsed();

    // 统计信息
    int maskArea = cv::countNonZero(lastMask_);
    int totalArea = imageSize.width * imageSize.height;
    double ratio = 100.0 * maskArea / totalArea;

    output.setValue("roiCount", rois_.count());
    output.setValue("maskArea", maskArea);
    output.setValue("totalArea", totalArea);
    output.setValue("areaRatio", ratio);

    // 设置显示对象（mask可以用于其他工具的可视化）
    output.setDisplayObject("mask", QVariant::fromValue(lastMask_.clone()));

    setDebugImage(std::make_shared<Base::ImageData>(lastMask_));
    setStatusText(QString("ROI数量: %1, 有效面积: %2 (%.1f%%)")
                 .arg(rois_.count())
                 .arg(maskArea)
                 .arg(ratio));

    LOG_DEBUG(QString("ROI处理完成，耗时: %1ms").arg(output.executionTime));

    return true;
}

QJsonObject ROITool::serializeParams() const
{
    QJsonObject json;
    json["combineMode"] = static_cast<int>(combineMode_);
    json["invertMask"] = invertMask_;
    json["cropOutput"] = cropOutput_;
    json["rois"] = rois_.toJson();
    return json;
}

void ROITool::deserializeParams(const QJsonObject& json)
{
    combineMode_ = static_cast<CombineMode>(json.value("combineMode").toInt(static_cast<int>(Union)));
    invertMask_ = json.value("invertMask").toBool(false);
    cropOutput_ = json.value("cropOutput").toBool(false);
    rois_.fromJson(json.value("rois").toArray());
}

void ROITool::addRectROI(const QRectF& rect, const QString& name)
{
    auto roi = std::make_shared<RectROI>(rect);
    if (!name.isEmpty()) {
        roi->setName(name);
    } else {
        roi->setName(QString("矩形_%1").arg(rois_.count() + 1));
    }
    rois_.addROI(roi);
    emit roiChanged();
    emit paramChanged();
}

void ROITool::addCircleROI(const QPointF& center, double radius, const QString& name)
{
    auto roi = std::make_shared<CircleROI>(center, radius);
    if (!name.isEmpty()) {
        roi->setName(name);
    } else {
        roi->setName(QString("圆形_%1").arg(rois_.count() + 1));
    }
    rois_.addROI(roi);
    emit roiChanged();
    emit paramChanged();
}

void ROITool::addEllipseROI(const QPointF& center, double rx, double ry, double angle, const QString& name)
{
    auto roi = std::make_shared<EllipseROI>(center, rx, ry, angle);
    if (!name.isEmpty()) {
        roi->setName(name);
    } else {
        roi->setName(QString("椭圆_%1").arg(rois_.count() + 1));
    }
    rois_.addROI(roi);
    emit roiChanged();
    emit paramChanged();
}

void ROITool::addPolygonROI(const QPolygonF& polygon, const QString& name)
{
    auto roi = std::make_shared<PolygonROI>(polygon);
    if (!name.isEmpty()) {
        roi->setName(name);
    } else {
        roi->setName(QString("多边形_%1").arg(rois_.count() + 1));
    }
    rois_.addROI(roi);
    emit roiChanged();
    emit paramChanged();
}

void ROITool::addRotatedRectROI(const QPointF& center, double width, double height, double angle, const QString& name)
{
    auto roi = std::make_shared<RotatedRectROI>(center, width, height, angle);
    if (!name.isEmpty()) {
        roi->setName(name);
    } else {
        roi->setName(QString("旋转矩形_%1").arg(rois_.count() + 1));
    }
    rois_.addROI(roi);
    emit roiChanged();
    emit paramChanged();
}

void ROITool::clearROIs()
{
    rois_.clear();
    emit roiChanged();
    emit paramChanged();
}

void ROITool::removeROI(int index)
{
    rois_.removeROI(index);
    emit roiChanged();
    emit paramChanged();
}

void ROITool::setCombineMode(CombineMode mode)
{
    if (combineMode_ != mode) {
        combineMode_ = mode;
        emit paramChanged();
    }
}

void ROITool::setInvertMask(bool invert)
{
    if (invertMask_ != invert) {
        invertMask_ = invert;
        emit paramChanged();
    }
}

void ROITool::setCropOutput(bool crop)
{
    if (cropOutput_ != crop) {
        cropOutput_ = crop;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
