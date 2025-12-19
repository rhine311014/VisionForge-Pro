/**
 * @file MultiPointAlignmentTool.cpp
 * @brief 多点对位检测工具实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "algorithm/MultiPointAlignmentTool.h"
#include "base/Logger.h"
#include <QElapsedTimer>
#include <QDateTime>
#include <QJsonArray>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

// ============================================================
// AlignmentPointConfig 序列化
// ============================================================

QJsonObject AlignmentPointConfig::toJson() const
{
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = name;
    obj["type"] = static_cast<int>(type);
    obj["cameraId"] = cameraId;
    obj["searchROI"] = QJsonObject{
        {"x", searchROI.x()},
        {"y", searchROI.y()},
        {"width", searchROI.width()},
        {"height", searchROI.height()}
    };
    obj["standardX"] = standardPosition.x();
    obj["standardY"] = standardPosition.y();
    obj["standardAngle"] = standardAngle;
    obj["enabled"] = enabled;
    obj["useCoordinateTransform"] = useCoordinateTransform;
    obj["calibrationId"] = calibrationId;
    obj["parameters"] = QJsonObject::fromVariantMap(parameters);
    return obj;
}

AlignmentPointConfig AlignmentPointConfig::fromJson(const QJsonObject& json)
{
    AlignmentPointConfig config;
    config.id = json["id"].toString();
    config.name = json["name"].toString();
    config.type = static_cast<AlignmentPointType>(json["type"].toInt());
    config.cameraId = json["cameraId"].toString();

    QJsonObject roiObj = json["searchROI"].toObject();
    config.searchROI = QRectF(
        roiObj["x"].toDouble(),
        roiObj["y"].toDouble(),
        roiObj["width"].toDouble(),
        roiObj["height"].toDouble()
    );

    config.standardPosition = QPointF(json["standardX"].toDouble(), json["standardY"].toDouble());
    config.standardAngle = json["standardAngle"].toDouble();
    config.enabled = json["enabled"].toBool(true);
    config.useCoordinateTransform = json["useCoordinateTransform"].toBool(false);
    config.calibrationId = json["calibrationId"].toString();
    config.parameters = json["parameters"].toObject().toVariantMap();

    return config;
}

// ============================================================
// PointDetectionResult 序列化
// ============================================================

QJsonObject PointDetectionResult::toJson() const
{
    QJsonObject obj;
    obj["pointId"] = pointId;
    obj["success"] = success;
    obj["detectedX"] = detectedPosition.x();
    obj["detectedY"] = detectedPosition.y();
    obj["detectedAngle"] = detectedAngle;
    obj["worldX"] = worldPosition.x();
    obj["worldY"] = worldPosition.y();
    obj["score"] = score;
    obj["offsetX"] = offsetX;
    obj["offsetY"] = offsetY;
    obj["offsetAngle"] = offsetAngle;
    obj["errorMessage"] = errorMessage;
    obj["processTime"] = processTime;
    return obj;
}

// ============================================================
// AlignmentResult 序列化
// ============================================================

QJsonObject AlignmentResult::toJson() const
{
    QJsonObject obj;
    obj["success"] = success;
    obj["offsetX"] = offsetX;
    obj["offsetY"] = offsetY;
    obj["offsetTheta"] = offsetTheta;
    obj["rotationCenterX"] = rotationCenter.x();
    obj["rotationCenterY"] = rotationCenter.y();
    obj["residualError"] = residualError;
    obj["maxPointError"] = maxPointError;
    obj["totalPoints"] = totalPoints;
    obj["successPoints"] = successPoints;
    obj["failedPoints"] = failedPoints;
    obj["totalProcessTime"] = totalProcessTime;
    obj["timestamp"] = timestamp;

    QJsonObject pointsObj;
    for (auto it = pointResults.begin(); it != pointResults.end(); ++it) {
        pointsObj[it.key()] = it.value().toJson();
    }
    obj["pointResults"] = pointsObj;

    return obj;
}

// ============================================================
// MultiPointAlignmentTool 实现
// ============================================================

MultiPointAlignmentTool::MultiPointAlignmentTool(QObject* parent)
    : VisionTool(parent)
    , deviationMode_(DeviationCalcMode::RigidTransform)
    , autoRotationCenter_(true)
    , coordTransformEnabled_(false)
    , pointIdCounter_(0)
{
}

MultiPointAlignmentTool::~MultiPointAlignmentTool() = default;

// ========== VisionTool 接口 ==========

bool MultiPointAlignmentTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    AlignmentResult result = detectSingleImage(input);
    lastResult_ = result;

    if (result.success) {
        output.success = true;
        output.data["offsetX"] = result.offsetX;
        output.data["offsetY"] = result.offsetY;
        output.data["offsetTheta"] = result.offsetTheta;
    } else {
        output.success = false;
        output.errorMessage = "对位检测失败";
        return false;
    }

    return result.success;
}

QJsonObject MultiPointAlignmentTool::serializeParams() const
{
    QJsonObject params;
    params["deviationMode"] = static_cast<int>(deviationMode_);
    params["rotationCenterX"] = rotationCenter_.x();
    params["rotationCenterY"] = rotationCenter_.y();
    params["autoRotationCenter"] = autoRotationCenter_;
    params["coordTransformEnabled"] = coordTransformEnabled_;

    QJsonArray pointsArray;
    for (auto it = alignmentPoints_.begin(); it != alignmentPoints_.end(); ++it) {
        pointsArray.append(it.value().toJson());
    }
    params["alignmentPoints"] = pointsArray;

    return params;
}

void MultiPointAlignmentTool::deserializeParams(const QJsonObject& params)
{
    deviationMode_ = static_cast<DeviationCalcMode>(params["deviationMode"].toInt());
    rotationCenter_ = QPointF(params["rotationCenterX"].toDouble(),
                               params["rotationCenterY"].toDouble());
    autoRotationCenter_ = params["autoRotationCenter"].toBool(true);
    coordTransformEnabled_ = params["coordTransformEnabled"].toBool(false);

    alignmentPoints_.clear();
    QJsonArray pointsArray = params["alignmentPoints"].toArray();
    for (const QJsonValue& val : pointsArray) {
        AlignmentPointConfig config = AlignmentPointConfig::fromJson(val.toObject());
        alignmentPoints_[config.id] = config;
    }
}

// ========== 对位点管理 ==========

QString MultiPointAlignmentTool::addAlignmentPoint(const AlignmentPointConfig& config)
{
    AlignmentPointConfig newConfig = config;

    if (newConfig.id.isEmpty()) {
        newConfig.id = generatePointId();
    }

    alignmentPoints_[newConfig.id] = newConfig;

    LOG_INFO(QString("添加对位点: %1 (%2)").arg(newConfig.name).arg(newConfig.id));
    emit alignmentPointChanged(newConfig.id);

    return newConfig.id;
}

bool MultiPointAlignmentTool::removeAlignmentPoint(const QString& pointId)
{
    if (!alignmentPoints_.contains(pointId)) {
        return false;
    }

    alignmentPoints_.remove(pointId);
    pointTemplates_.remove(pointId);

    LOG_INFO(QString("移除对位点: %1").arg(pointId));
    emit alignmentPointChanged(pointId);

    return true;
}

bool MultiPointAlignmentTool::updateAlignmentPoint(const QString& pointId,
                                                    const AlignmentPointConfig& config)
{
    if (!alignmentPoints_.contains(pointId)) {
        return false;
    }

    AlignmentPointConfig updated = config;
    updated.id = pointId; // 保持ID不变

    alignmentPoints_[pointId] = updated;
    emit alignmentPointChanged(pointId);

    return true;
}

AlignmentPointConfig MultiPointAlignmentTool::getAlignmentPoint(const QString& pointId) const
{
    if (alignmentPoints_.contains(pointId)) {
        return alignmentPoints_[pointId];
    }
    return AlignmentPointConfig();
}

QVector<AlignmentPointConfig> MultiPointAlignmentTool::getAllAlignmentPoints() const
{
    QVector<AlignmentPointConfig> result;
    for (auto it = alignmentPoints_.begin(); it != alignmentPoints_.end(); ++it) {
        result.append(it.value());
    }
    return result;
}

int MultiPointAlignmentTool::alignmentPointCount() const
{
    return alignmentPoints_.size();
}

void MultiPointAlignmentTool::clearAlignmentPoints()
{
    alignmentPoints_.clear();
    pointTemplates_.clear();
    pointIdCounter_ = 0;
}

// ========== 偏差计算设置 ==========

void MultiPointAlignmentTool::setDeviationCalcMode(DeviationCalcMode mode)
{
    deviationMode_ = mode;
}

DeviationCalcMode MultiPointAlignmentTool::getDeviationCalcMode() const
{
    return deviationMode_;
}

void MultiPointAlignmentTool::setRotationCenter(const QPointF& center)
{
    rotationCenter_ = center;
}

QPointF MultiPointAlignmentTool::getRotationCenter() const
{
    return rotationCenter_;
}

void MultiPointAlignmentTool::setAutoRotationCenter(bool autoCalc)
{
    autoRotationCenter_ = autoCalc;
}

bool MultiPointAlignmentTool::isAutoRotationCenter() const
{
    return autoRotationCenter_;
}

// ========== 坐标转换 ==========

void MultiPointAlignmentTool::setCoordinateTransform(const QString& calibId,
                                                      std::shared_ptr<CoordinateTransform> transform)
{
    transforms_[calibId] = transform;
}

std::shared_ptr<CoordinateTransform> MultiPointAlignmentTool::getCoordinateTransform(
    const QString& calibId) const
{
    if (transforms_.contains(calibId)) {
        return transforms_[calibId];
    }
    return nullptr;
}

void MultiPointAlignmentTool::setCoordinateTransformEnabled(bool enabled)
{
    coordTransformEnabled_ = enabled;
}

bool MultiPointAlignmentTool::isCoordinateTransformEnabled() const
{
    return coordTransformEnabled_;
}

// ========== 检测执行 ==========

AlignmentResult MultiPointAlignmentTool::detectSingleImage(const Base::ImageData::Ptr& image)
{
    AlignmentResult result;
    result.timestamp = QDateTime::currentMSecsSinceEpoch();

    if (!image || image->isEmpty()) {
        result.success = false;
        return result;
    }

    QElapsedTimer totalTimer;
    totalTimer.start();

    cv::Mat cvImage = image->mat();

    // 检测每个启用的点
    for (auto it = alignmentPoints_.begin(); it != alignmentPoints_.end(); ++it) {
        const AlignmentPointConfig& config = it.value();

        if (!config.enabled) {
            continue;
        }

        PointDetectionResult pointResult = detectPoint(config, cvImage);
        pointResult.pointId = config.id;

        // 应用坐标转换
        if (coordTransformEnabled_ && config.useCoordinateTransform) {
            applyCoordinateTransform(pointResult, config);
        }

        result.pointResults[config.id] = pointResult;
        result.totalPoints++;

        if (pointResult.success) {
            result.successPoints++;
        } else {
            result.failedPoints++;
        }

        emit pointDetected(config.id, pointResult);
    }

    // 计算综合偏差
    if (result.successPoints > 0) {
        calculateOverallDeviation(result);
        result.success = true;
    } else {
        result.success = false;
    }

    result.totalProcessTime = totalTimer.elapsed();

    lastResult_ = result;
    emit detectionCompleted(result);

    return result;
}

AlignmentResult MultiPointAlignmentTool::detectMultiCamera(
    const QMap<QString, Base::ImageData::Ptr>& images)
{
    AlignmentResult result;
    result.timestamp = QDateTime::currentMSecsSinceEpoch();

    QElapsedTimer totalTimer;
    totalTimer.start();

    // 按相机分组的点
    QMap<QString, QVector<AlignmentPointConfig>> cameraPoints;
    for (auto it = alignmentPoints_.begin(); it != alignmentPoints_.end(); ++it) {
        const AlignmentPointConfig& config = it.value();
        if (config.enabled) {
            cameraPoints[config.cameraId].append(config);
        }
    }

    // 对每个相机的图像检测对应的点
    for (auto it = cameraPoints.begin(); it != cameraPoints.end(); ++it) {
        const QString& cameraId = it.key();
        const QVector<AlignmentPointConfig>& points = it.value();

        if (!images.contains(cameraId)) {
            LOG_WARNING(QString("相机图像不存在: %1").arg(cameraId));
            for (const AlignmentPointConfig& config : points) {
                PointDetectionResult pointResult;
                pointResult.pointId = config.id;
                pointResult.success = false;
                pointResult.errorMessage = "相机图像不存在";
                result.pointResults[config.id] = pointResult;
                result.totalPoints++;
                result.failedPoints++;
            }
            continue;
        }

        const Base::ImageData::Ptr& image = images[cameraId];
        if (!image || image->isEmpty()) {
            continue;
        }

        cv::Mat cvImage = image->mat();

        for (const AlignmentPointConfig& config : points) {
            PointDetectionResult pointResult = detectPoint(config, cvImage);
            pointResult.pointId = config.id;

            if (coordTransformEnabled_ && config.useCoordinateTransform) {
                applyCoordinateTransform(pointResult, config);
            }

            result.pointResults[config.id] = pointResult;
            result.totalPoints++;

            if (pointResult.success) {
                result.successPoints++;
            } else {
                result.failedPoints++;
            }

            emit pointDetected(config.id, pointResult);
        }
    }

    // 计算综合偏差
    if (result.successPoints > 0) {
        calculateOverallDeviation(result);
        result.success = true;
    } else {
        result.success = false;
    }

    result.totalProcessTime = totalTimer.elapsed();

    lastResult_ = result;
    emit detectionCompleted(result);

    return result;
}

AlignmentResult MultiPointAlignmentTool::getLastResult() const
{
    return lastResult_;
}

// ========== 模板管理 ==========

void MultiPointAlignmentTool::setPointTemplate(const QString& pointId, const cv::Mat& templateImage)
{
    pointTemplates_[pointId] = templateImage.clone();
}

cv::Mat MultiPointAlignmentTool::getPointTemplate(const QString& pointId) const
{
    if (pointTemplates_.contains(pointId)) {
        return pointTemplates_[pointId];
    }
    return cv::Mat();
}

bool MultiPointAlignmentTool::createTemplateFromImage(const QString& pointId,
                                                       const Base::ImageData::Ptr& image,
                                                       const QRectF& roi)
{
    if (!image || image->isEmpty()) {
        return false;
    }

    if (!alignmentPoints_.contains(pointId)) {
        return false;
    }

    cv::Mat cvImage = image->mat();
    cv::Rect cvRoi(
        static_cast<int>(roi.x()),
        static_cast<int>(roi.y()),
        static_cast<int>(roi.width()),
        static_cast<int>(roi.height())
    );

    // 边界检查
    cvRoi &= cv::Rect(0, 0, cvImage.cols, cvImage.rows);
    if (cvRoi.width <= 0 || cvRoi.height <= 0) {
        return false;
    }

    cv::Mat templateImg = cvImage(cvRoi).clone();
    pointTemplates_[pointId] = templateImg;

    LOG_INFO(QString("创建模板: 点=%1, 大小=%2x%3")
             .arg(pointId).arg(templateImg.cols).arg(templateImg.rows));

    return true;
}

// ========== 私有方法 ==========

PointDetectionResult MultiPointAlignmentTool::detectPoint(const AlignmentPointConfig& config,
                                                           const cv::Mat& image)
{
    PointDetectionResult result;
    result.pointId = config.id;

    QElapsedTimer timer;
    timer.start();

    switch (config.type) {
    case AlignmentPointType::TemplateMatch:
        result = detectTemplateMatch(config, image);
        break;
    case AlignmentPointType::BlobCenter:
        result = detectBlobCenter(config, image);
        break;
    case AlignmentPointType::CircleCenter:
        result = detectCircleCenter(config, image);
        break;
    case AlignmentPointType::EdgePoint:
        result = detectEdgePoint(config, image);
        break;
    case AlignmentPointType::ShapeMatch:
        // Halcon形状匹配需要单独实现
        result.success = false;
        result.errorMessage = "形状匹配需要Halcon支持";
        break;
    case AlignmentPointType::CrosshairCenter:
        // 十字中心可使用Blob或模板匹配
        result = detectBlobCenter(config, image);
        break;
    default:
        result.success = false;
        result.errorMessage = "不支持的检测类型";
        break;
    }

    result.processTime = timer.elapsed();
    result.pointId = config.id;

    // 计算单点偏差
    if (result.success) {
        result.offsetX = result.detectedPosition.x() - config.standardPosition.x();
        result.offsetY = result.detectedPosition.y() - config.standardPosition.y();
        result.offsetAngle = result.detectedAngle - config.standardAngle;
    }

    return result;
}

PointDetectionResult MultiPointAlignmentTool::detectTemplateMatch(
    const AlignmentPointConfig& config, const cv::Mat& image)
{
    PointDetectionResult result;

    if (!pointTemplates_.contains(config.id)) {
        result.success = false;
        result.errorMessage = "模板图像未设置";
        return result;
    }

    cv::Mat templateImg = pointTemplates_[config.id];
    if (templateImg.empty()) {
        result.success = false;
        result.errorMessage = "模板图像为空";
        return result;
    }

    // 提取ROI
    cv::Mat searchImage;
    cv::Rect roi;

    if (config.searchROI.isValid() && config.searchROI.width() > 0) {
        roi = cv::Rect(
            static_cast<int>(config.searchROI.x()),
            static_cast<int>(config.searchROI.y()),
            static_cast<int>(config.searchROI.width()),
            static_cast<int>(config.searchROI.height())
        );
        roi &= cv::Rect(0, 0, image.cols, image.rows);
        searchImage = image(roi);
    } else {
        searchImage = image;
        roi = cv::Rect(0, 0, image.cols, image.rows);
    }

    // 确保图像通道一致
    cv::Mat graySearch, grayTemplate;
    if (searchImage.channels() == 3) {
        cv::cvtColor(searchImage, graySearch, cv::COLOR_BGR2GRAY);
    } else {
        graySearch = searchImage;
    }

    if (templateImg.channels() == 3) {
        cv::cvtColor(templateImg, grayTemplate, cv::COLOR_BGR2GRAY);
    } else {
        grayTemplate = templateImg;
    }

    // 检查尺寸
    if (grayTemplate.cols > graySearch.cols || grayTemplate.rows > graySearch.rows) {
        result.success = false;
        result.errorMessage = "模板尺寸大于搜索区域";
        return result;
    }

    // 模板匹配
    cv::Mat matchResult;
    cv::matchTemplate(graySearch, grayTemplate, matchResult, cv::TM_CCOEFF_NORMED);

    // 查找最佳匹配
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(matchResult, &minVal, &maxVal, &minLoc, &maxLoc);

    // 阈值判断
    double threshold = config.parameters.value("matchThreshold", 0.7).toDouble();
    if (maxVal < threshold) {
        result.success = false;
        result.errorMessage = QString("匹配得分过低: %1 < %2").arg(maxVal).arg(threshold);
        return result;
    }

    // 计算匹配中心点
    double centerX = roi.x + maxLoc.x + grayTemplate.cols / 2.0;
    double centerY = roi.y + maxLoc.y + grayTemplate.rows / 2.0;

    result.success = true;
    result.detectedPosition = QPointF(centerX, centerY);
    result.detectedAngle = 0.0; // 基本模板匹配不提供角度
    result.score = maxVal;

    return result;
}

PointDetectionResult MultiPointAlignmentTool::detectBlobCenter(
    const AlignmentPointConfig& config, const cv::Mat& image)
{
    PointDetectionResult result;

    // 提取ROI
    cv::Mat searchImage;
    cv::Rect roi;

    if (config.searchROI.isValid() && config.searchROI.width() > 0) {
        roi = cv::Rect(
            static_cast<int>(config.searchROI.x()),
            static_cast<int>(config.searchROI.y()),
            static_cast<int>(config.searchROI.width()),
            static_cast<int>(config.searchROI.height())
        );
        roi &= cv::Rect(0, 0, image.cols, image.rows);
        searchImage = image(roi);
    } else {
        searchImage = image;
        roi = cv::Rect(0, 0, image.cols, image.rows);
    }

    // 转灰度
    cv::Mat gray;
    if (searchImage.channels() == 3) {
        cv::cvtColor(searchImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = searchImage;
    }

    // 二值化
    cv::Mat binary;
    int threshValue = config.parameters.value("blobThreshold", 128).toInt();
    bool invertPolarity = config.parameters.value("invertPolarity", false).toBool();

    if (invertPolarity) {
        cv::threshold(gray, binary, threshValue, 255, cv::THRESH_BINARY_INV);
    } else {
        cv::threshold(gray, binary, threshValue, 255, cv::THRESH_BINARY);
    }

    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        result.success = false;
        result.errorMessage = "未找到Blob";
        return result;
    }

    // 找最大轮廓
    int maxIdx = 0;
    double maxArea = 0;
    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea) {
            maxArea = area;
            maxIdx = static_cast<int>(i);
        }
    }

    // 计算矩和中心
    cv::Moments m = cv::moments(contours[maxIdx]);
    if (m.m00 < 1e-6) {
        result.success = false;
        result.errorMessage = "Blob面积过小";
        return result;
    }

    double centerX = roi.x + m.m10 / m.m00;
    double centerY = roi.y + m.m01 / m.m00;

    // 计算角度（主轴方向）
    double angle = 0.5 * std::atan2(2 * m.mu11, m.mu20 - m.mu02) * 180.0 / CV_PI;

    result.success = true;
    result.detectedPosition = QPointF(centerX, centerY);
    result.detectedAngle = angle;
    result.score = maxArea;

    return result;
}

PointDetectionResult MultiPointAlignmentTool::detectCircleCenter(
    const AlignmentPointConfig& config, const cv::Mat& image)
{
    PointDetectionResult result;

    // 提取ROI
    cv::Mat searchImage;
    cv::Rect roi;

    if (config.searchROI.isValid() && config.searchROI.width() > 0) {
        roi = cv::Rect(
            static_cast<int>(config.searchROI.x()),
            static_cast<int>(config.searchROI.y()),
            static_cast<int>(config.searchROI.width()),
            static_cast<int>(config.searchROI.height())
        );
        roi &= cv::Rect(0, 0, image.cols, image.rows);
        searchImage = image(roi);
    } else {
        searchImage = image;
        roi = cv::Rect(0, 0, image.cols, image.rows);
    }

    // 转灰度
    cv::Mat gray;
    if (searchImage.channels() == 3) {
        cv::cvtColor(searchImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = searchImage;
    }

    // 高斯模糊减少噪声
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);

    // 霍夫圆检测
    std::vector<cv::Vec3f> circles;
    double minRadius = config.parameters.value("minRadius", 10).toDouble();
    double maxRadius = config.parameters.value("maxRadius", 100).toDouble();
    double param1 = config.parameters.value("cannyThreshold", 100).toDouble();
    double param2 = config.parameters.value("accumulatorThreshold", 30).toDouble();

    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                     gray.rows / 8.0, param1, param2,
                     static_cast<int>(minRadius), static_cast<int>(maxRadius));

    if (circles.empty()) {
        result.success = false;
        result.errorMessage = "未检测到圆";
        return result;
    }

    // 使用第一个检测到的圆（最强的）
    cv::Vec3f bestCircle = circles[0];

    double centerX = roi.x + bestCircle[0];
    double centerY = roi.y + bestCircle[1];
    double radius = bestCircle[2];

    result.success = true;
    result.detectedPosition = QPointF(centerX, centerY);
    result.detectedAngle = 0.0; // 圆没有角度
    result.score = radius;

    return result;
}

PointDetectionResult MultiPointAlignmentTool::detectEdgePoint(
    const AlignmentPointConfig& config, const cv::Mat& image)
{
    PointDetectionResult result;

    // 提取ROI
    cv::Mat searchImage;
    cv::Rect roi;

    if (config.searchROI.isValid() && config.searchROI.width() > 0) {
        roi = cv::Rect(
            static_cast<int>(config.searchROI.x()),
            static_cast<int>(config.searchROI.y()),
            static_cast<int>(config.searchROI.width()),
            static_cast<int>(config.searchROI.height())
        );
        roi &= cv::Rect(0, 0, image.cols, image.rows);
        searchImage = image(roi);
    } else {
        searchImage = image;
        roi = cv::Rect(0, 0, image.cols, image.rows);
    }

    // 转灰度
    cv::Mat gray;
    if (searchImage.channels() == 3) {
        cv::cvtColor(searchImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = searchImage;
    }

    // 边缘检测
    cv::Mat edges;
    double lowThreshold = config.parameters.value("edgeLowThreshold", 50).toDouble();
    double highThreshold = config.parameters.value("edgeHighThreshold", 150).toDouble();
    cv::Canny(gray, edges, lowThreshold, highThreshold);

    // 查找边缘点
    std::vector<cv::Point> edgePoints;
    for (int y = 0; y < edges.rows; ++y) {
        for (int x = 0; x < edges.cols; ++x) {
            if (edges.at<uchar>(y, x) > 0) {
                edgePoints.push_back(cv::Point(x, y));
            }
        }
    }

    if (edgePoints.empty()) {
        result.success = false;
        result.errorMessage = "未检测到边缘点";
        return result;
    }

    // 计算边缘点的质心
    double sumX = 0, sumY = 0;
    for (const cv::Point& p : edgePoints) {
        sumX += p.x;
        sumY += p.y;
    }

    double centerX = roi.x + sumX / edgePoints.size();
    double centerY = roi.y + sumY / edgePoints.size();

    result.success = true;
    result.detectedPosition = QPointF(centerX, centerY);
    result.detectedAngle = 0.0;
    result.score = static_cast<double>(edgePoints.size());

    return result;
}

void MultiPointAlignmentTool::calculateOverallDeviation(AlignmentResult& result)
{
    switch (deviationMode_) {
    case DeviationCalcMode::SinglePoint:
        // 使用第一个成功的点
        for (auto it = result.pointResults.begin(); it != result.pointResults.end(); ++it) {
            if (it.value().success) {
                result.offsetX = it.value().offsetX;
                result.offsetY = it.value().offsetY;
                result.offsetTheta = it.value().offsetAngle;
                break;
            }
        }
        break;

    case DeviationCalcMode::TwoPoints:
        // 使用两点计算X/Y/θ
        {
            QVector<PointDetectionResult> successPoints;
            for (auto it = result.pointResults.begin(); it != result.pointResults.end(); ++it) {
                if (it.value().success) {
                    successPoints.append(it.value());
                    if (successPoints.size() >= 2) break;
                }
            }

            if (successPoints.size() >= 2) {
                // 计算两点的平均偏差
                result.offsetX = (successPoints[0].offsetX + successPoints[1].offsetX) / 2.0;
                result.offsetY = (successPoints[0].offsetY + successPoints[1].offsetY) / 2.0;

                // 计算角度偏差
                double dx1 = successPoints[1].detectedPosition.x() - successPoints[0].detectedPosition.x();
                double dy1 = successPoints[1].detectedPosition.y() - successPoints[0].detectedPosition.y();

                const AlignmentPointConfig& config0 = alignmentPoints_[successPoints[0].pointId];
                const AlignmentPointConfig& config1 = alignmentPoints_[successPoints[1].pointId];
                double dx0 = config1.standardPosition.x() - config0.standardPosition.x();
                double dy0 = config1.standardPosition.y() - config0.standardPosition.y();

                double detectedAngle = std::atan2(dy1, dx1) * 180.0 / CV_PI;
                double standardAngle = std::atan2(dy0, dx0) * 180.0 / CV_PI;
                result.offsetTheta = detectedAngle - standardAngle;
            } else if (successPoints.size() == 1) {
                result.offsetX = successPoints[0].offsetX;
                result.offsetY = successPoints[0].offsetY;
                result.offsetTheta = successPoints[0].offsetAngle;
            }
        }
        break;

    case DeviationCalcMode::MultiPoints:
        // 多点平均
        {
            double sumX = 0, sumY = 0, sumTheta = 0;
            int count = 0;
            for (auto it = result.pointResults.begin(); it != result.pointResults.end(); ++it) {
                if (it.value().success) {
                    sumX += it.value().offsetX;
                    sumY += it.value().offsetY;
                    sumTheta += it.value().offsetAngle;
                    count++;
                }
            }
            if (count > 0) {
                result.offsetX = sumX / count;
                result.offsetY = sumY / count;
                result.offsetTheta = sumTheta / count;
            }
        }
        break;

    case DeviationCalcMode::RigidTransform:
        calculateRigidTransformDeviation(result);
        break;
    }
}

void MultiPointAlignmentTool::calculateRigidTransformDeviation(AlignmentResult& result)
{
    // 收集成功检测的点
    std::vector<cv::Point2f> detectedPoints, standardPoints;

    for (auto it = result.pointResults.begin(); it != result.pointResults.end(); ++it) {
        if (!it.value().success) continue;

        const QString& pointId = it.key();
        if (!alignmentPoints_.contains(pointId)) continue;

        const AlignmentPointConfig& config = alignmentPoints_[pointId];
        const PointDetectionResult& detection = it.value();

        detectedPoints.push_back(cv::Point2f(detection.detectedPosition.x(),
                                              detection.detectedPosition.y()));
        standardPoints.push_back(cv::Point2f(config.standardPosition.x(),
                                              config.standardPosition.y()));
    }

    int n = static_cast<int>(detectedPoints.size());
    if (n < 2) {
        // 点数不足，使用简单偏差
        if (n == 1) {
            result.offsetX = detectedPoints[0].x - standardPoints[0].x;
            result.offsetY = detectedPoints[0].y - standardPoints[0].y;
            result.offsetTheta = 0;
        }
        return;
    }

    // SVD刚性变换求解
    // 参考：Arun, K. S., Huang, T. S., & Blostein, S. D. (1987)
    // "Least-squares fitting of two 3-D point sets"

    // 1. 计算质心
    cv::Point2f detectedCentroid(0, 0), standardCentroid(0, 0);
    for (int i = 0; i < n; ++i) {
        detectedCentroid += detectedPoints[i];
        standardCentroid += standardPoints[i];
    }
    detectedCentroid *= (1.0f / n);
    standardCentroid *= (1.0f / n);

    // 设置旋转中心
    if (autoRotationCenter_) {
        result.rotationCenter = QPointF(standardCentroid.x, standardCentroid.y);
    } else {
        result.rotationCenter = rotationCenter_;
    }

    // 2. 中心化点集
    cv::Mat P(n, 2, CV_64F); // 检测点 - 质心
    cv::Mat Q(n, 2, CV_64F); // 标准点 - 质心

    for (int i = 0; i < n; ++i) {
        P.at<double>(i, 0) = detectedPoints[i].x - detectedCentroid.x;
        P.at<double>(i, 1) = detectedPoints[i].y - detectedCentroid.y;
        Q.at<double>(i, 0) = standardPoints[i].x - standardCentroid.x;
        Q.at<double>(i, 1) = standardPoints[i].y - standardCentroid.y;
    }

    // 3. 计算协方差矩阵 H = P^T * Q
    cv::Mat H = P.t() * Q;

    // 4. SVD分解
    cv::Mat U, S, Vt;
    cv::SVD::compute(H, S, U, Vt);

    // 5. 旋转矩阵 R = V * U^T
    cv::Mat R = Vt.t() * U.t();

    // 检查反射（行列式为-1时修正）
    double det = cv::determinant(R);
    if (det < 0) {
        Vt.row(1) *= -1;
        R = Vt.t() * U.t();
    }

    // 6. 计算角度
    double angle = std::atan2(R.at<double>(1, 0), R.at<double>(0, 0)) * 180.0 / CV_PI;
    result.offsetTheta = angle;

    // 7. 计算平移 (检测质心相对于旋转后的标准质心的偏移)
    // t = detected_centroid - R * standard_centroid
    cv::Mat stdCentroidMat = (cv::Mat_<double>(2, 1) << standardCentroid.x, standardCentroid.y);
    cv::Mat detCentroidMat = (cv::Mat_<double>(2, 1) << detectedCentroid.x, detectedCentroid.y);
    cv::Mat t = detCentroidMat - R * stdCentroidMat;

    result.offsetX = t.at<double>(0, 0);
    result.offsetY = t.at<double>(1, 0);

    // 8. 计算残差
    double totalError = 0;
    double maxError = 0;

    for (int i = 0; i < n; ++i) {
        // 用变换后的标准点与检测点比较
        cv::Mat stdPt = (cv::Mat_<double>(2, 1) << standardPoints[i].x, standardPoints[i].y);
        cv::Mat transformed = R * stdPt + t;

        double dx = transformed.at<double>(0, 0) - detectedPoints[i].x;
        double dy = transformed.at<double>(1, 0) - detectedPoints[i].y;
        double error = std::sqrt(dx * dx + dy * dy);

        totalError += error * error;
        if (error > maxError) {
            maxError = error;
        }
    }

    result.residualError = std::sqrt(totalError / n);
    result.maxPointError = maxError;
}

void MultiPointAlignmentTool::applyCoordinateTransform(PointDetectionResult& result,
                                                        const AlignmentPointConfig& config)
{
    if (!config.useCoordinateTransform || config.calibrationId.isEmpty()) {
        return;
    }

    auto transform = getCoordinateTransform(config.calibrationId);
    if (!transform || !transform->isCalibrated()) {
        return;
    }

    // 转换检测位置到世界坐标
    result.worldPosition = transform->pixelToWorld(result.detectedPosition);

    // 转换偏差
    QPointF standardWorld = transform->pixelToWorld(QPointF(
        result.detectedPosition.x() - result.offsetX,
        result.detectedPosition.y() - result.offsetY
    ));

    result.offsetX = result.worldPosition.x() - standardWorld.x();
    result.offsetY = result.worldPosition.y() - standardWorld.y();

    // 转换角度
    result.offsetAngle = transform->pixelToWorldAngle(result.offsetAngle * CV_PI / 180.0) * 180.0 / CV_PI;
}

QString MultiPointAlignmentTool::generatePointId()
{
    return QString("AP_%1").arg(++pointIdCounter_, 4, 10, QChar('0'));
}

} // namespace Algorithm
} // namespace VisionForge
