/**
 * @file CoordinateTransform.cpp
 * @brief 坐标转换类实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "algorithm/CoordinateTransform.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

// ============================================================
// CalibStats 序列化
// ============================================================

QJsonObject CalibStats::toJson() const
{
    QJsonObject obj;
    obj["success"] = success;
    obj["meanError"] = meanError;
    obj["maxError"] = maxError;
    obj["rmsError"] = rmsError;
    obj["pixelPerMm"] = pixelPerMm;
    obj["pointCount"] = pointCount;

    QJsonArray errorsArray;
    for (double err : pointErrors) {
        errorsArray.append(err);
    }
    obj["pointErrors"] = errorsArray;

    return obj;
}

CalibStats CalibStats::fromJson(const QJsonObject& json)
{
    CalibStats stats;
    stats.success = json["success"].toBool();
    stats.meanError = json["meanError"].toDouble();
    stats.maxError = json["maxError"].toDouble();
    stats.rmsError = json["rmsError"].toDouble();
    stats.pixelPerMm = json["pixelPerMm"].toDouble();
    stats.pointCount = json["pointCount"].toInt();

    QJsonArray errorsArray = json["pointErrors"].toArray();
    for (const QJsonValue& val : errorsArray) {
        stats.pointErrors.append(val.toDouble());
    }

    return stats;
}

// ============================================================
// CoordCalibResult 序列化
// ============================================================

QJsonObject CoordCalibResult::toJson() const
{
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = name;
    obj["cameraId"] = cameraId;
    obj["transformType"] = static_cast<int>(transformType);
    obj["timestamp"] = timestamp;
    obj["stats"] = stats.toJson();

    // 保存变换矩阵
    if (!transformMatrix.empty()) {
        QJsonArray matrixArray;
        for (int i = 0; i < transformMatrix.rows; ++i) {
            for (int j = 0; j < transformMatrix.cols; ++j) {
                matrixArray.append(transformMatrix.at<double>(i, j));
            }
        }
        obj["transformMatrix"] = matrixArray;
        obj["matrixRows"] = transformMatrix.rows;
        obj["matrixCols"] = transformMatrix.cols;
    }

    // 保存标定点
    QJsonArray pointsArray;
    for (const CalibPointPair& point : calibPoints) {
        if (point.isSet) {
            QJsonObject pointObj;
            pointObj["index"] = point.index;
            pointObj["pixelX"] = point.pixelCoord.x();
            pointObj["pixelY"] = point.pixelCoord.y();
            pointObj["worldX"] = point.worldCoord.x();
            pointObj["worldY"] = point.worldCoord.y();
            pointsArray.append(pointObj);
        }
    }
    obj["calibPoints"] = pointsArray;

    return obj;
}

CoordCalibResult CoordCalibResult::fromJson(const QJsonObject& json)
{
    CoordCalibResult result;
    result.id = json["id"].toString();
    result.name = json["name"].toString();
    result.cameraId = json["cameraId"].toString();
    result.transformType = static_cast<TransformType>(json["transformType"].toInt());
    result.timestamp = json["timestamp"].toVariant().toLongLong();
    result.stats = CalibStats::fromJson(json["stats"].toObject());

    // 恢复变换矩阵
    if (json.contains("transformMatrix")) {
        int rows = json["matrixRows"].toInt();
        int cols = json["matrixCols"].toInt();
        QJsonArray matrixArray = json["transformMatrix"].toArray();

        result.transformMatrix = cv::Mat(rows, cols, CV_64F);
        int idx = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.transformMatrix.at<double>(i, j) = matrixArray[idx++].toDouble();
            }
        }

        // 计算逆矩阵
        if (rows == 3 && cols == 3) {
            result.inverseMatrix = result.transformMatrix.inv();
        } else if (rows == 2 && cols == 3) {
            // 仿射变换需要扩展后求逆
            cv::Mat extended = cv::Mat::eye(3, 3, CV_64F);
            result.transformMatrix.copyTo(extended(cv::Rect(0, 0, 3, 2)));
            cv::Mat invExtended = extended.inv();
            result.inverseMatrix = invExtended(cv::Rect(0, 0, 3, 2)).clone();
        }
    }

    // 恢复标定点
    QJsonArray pointsArray = json["calibPoints"].toArray();
    for (const QJsonValue& val : pointsArray) {
        QJsonObject pointObj = val.toObject();
        CalibPointPair point;
        point.index = pointObj["index"].toInt();
        point.pixelCoord = QPointF(pointObj["pixelX"].toDouble(), pointObj["pixelY"].toDouble());
        point.worldCoord = QPointF(pointObj["worldX"].toDouble(), pointObj["worldY"].toDouble());
        point.isSet = true;
        result.calibPoints.append(point);
    }

    return result;
}

// ============================================================
// CoordinateTransform 实现
// ============================================================

CoordinateTransform::CoordinateTransform(QObject* parent)
    : QObject(parent)
    , gridRows_(3)
    , gridCols_(3)
    , transformType_(TransformType::Affine)
    , isCalibrated_(false)
    , calibrationTimestamp_(0)
{
    // 初始化标定点
    calibPoints_.resize(gridRows_ * gridCols_);
    for (int i = 0; i < calibPoints_.size(); ++i) {
        calibPoints_[i].index = i;
    }
}

CoordinateTransform::~CoordinateTransform() = default;

// ========== 标定点管理 ==========

void CoordinateTransform::setPointCount(int rows, int cols)
{
    if (rows < 2 || cols < 2) {
        LOG_WARNING("标定点数过少，最少需要2x2");
        return;
    }

    gridRows_ = rows;
    gridCols_ = cols;

    calibPoints_.resize(rows * cols);
    for (int i = 0; i < calibPoints_.size(); ++i) {
        calibPoints_[i].index = i;
        calibPoints_[i].isSet = false;
    }

    isCalibrated_ = false;
    transformMatrix_ = cv::Mat();
    inverseMatrix_ = cv::Mat();
}

void CoordinateTransform::getPointCount(int& rows, int& cols) const
{
    rows = gridRows_;
    cols = gridCols_;
}

int CoordinateTransform::totalPointCount() const
{
    return gridRows_ * gridCols_;
}

void CoordinateTransform::setCalibPoint(int index, const CalibPointPair& point)
{
    if (index < 0 || index >= calibPoints_.size()) {
        LOG_WARNING(QString("标定点索引超出范围: %1").arg(index));
        return;
    }

    calibPoints_[index] = point;
    calibPoints_[index].index = index;
    calibPoints_[index].isSet = true;

    isCalibrated_ = false;
    emit calibPointChanged(index);
}

void CoordinateTransform::setPixelCoord(int index, const QPointF& pixel)
{
    if (index < 0 || index >= calibPoints_.size()) {
        return;
    }

    calibPoints_[index].pixelCoord = pixel;
    calibPoints_[index].index = index;

    // 如果世界坐标已设置，标记为完整
    if (calibPoints_[index].worldCoord != QPointF(0, 0) || calibPoints_[index].isSet) {
        calibPoints_[index].isSet = true;
    }

    isCalibrated_ = false;
    emit calibPointChanged(index);
}

void CoordinateTransform::setWorldCoord(int index, const QPointF& world)
{
    if (index < 0 || index >= calibPoints_.size()) {
        return;
    }

    calibPoints_[index].worldCoord = world;
    calibPoints_[index].index = index;

    // 如果像素坐标已设置，标记为完整
    if (calibPoints_[index].pixelCoord != QPointF(0, 0) || calibPoints_[index].isSet) {
        calibPoints_[index].isSet = true;
    }

    isCalibrated_ = false;
    emit calibPointChanged(index);
}

CalibPointPair CoordinateTransform::getCalibPoint(int index) const
{
    if (index >= 0 && index < calibPoints_.size()) {
        return calibPoints_[index];
    }
    return CalibPointPair();
}

QVector<CalibPointPair> CoordinateTransform::getAllCalibPoints() const
{
    return calibPoints_;
}

void CoordinateTransform::clearCalibPoints()
{
    for (int i = 0; i < calibPoints_.size(); ++i) {
        calibPoints_[i].pixelCoord = QPointF();
        calibPoints_[i].worldCoord = QPointF();
        calibPoints_[i].isSet = false;
    }

    isCalibrated_ = false;
    transformMatrix_ = cv::Mat();
    inverseMatrix_ = cv::Mat();

    emit calibrationChanged();
}

bool CoordinateTransform::isCalibPointsComplete() const
{
    for (const CalibPointPair& point : calibPoints_) {
        if (!point.isSet) {
            return false;
        }
    }
    return true;
}

int CoordinateTransform::setPointCount() const
{
    int count = 0;
    for (const CalibPointPair& point : calibPoints_) {
        if (point.isSet) {
            ++count;
        }
    }
    return count;
}

// ========== 网格预设 ==========

void CoordinateTransform::generateGridWorldCoords(double spacingX, double spacingY,
                                                   double originX, double originY)
{
    for (int row = 0; row < gridRows_; ++row) {
        for (int col = 0; col < gridCols_; ++col) {
            int index = row * gridCols_ + col;
            double x = originX + col * spacingX;
            double y = originY + row * spacingY;
            calibPoints_[index].worldCoord = QPointF(x, y);
            calibPoints_[index].index = index;
        }
    }

    LOG_INFO(QString("生成网格世界坐标: %1x%2, 间距=(%3, %4), 原点=(%5, %6)")
             .arg(gridRows_).arg(gridCols_)
             .arg(spacingX).arg(spacingY)
             .arg(originX).arg(originY));
}

bool CoordinateTransform::detectCalibPattern(const cv::Mat& image, cv::Size patternSize)
{
    if (image.empty()) {
        LOG_WARNING("标定图像为空");
        return false;
    }

    // 如果未指定模式大小，使用当前网格大小
    if (patternSize.width <= 0 || patternSize.height <= 0) {
        patternSize = cv::Size(gridCols_, gridRows_);
    }

    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image;
    }

    // 检测棋盘格角点
    std::vector<cv::Point2f> corners;
    bool found = cv::findChessboardCorners(gray, patternSize, corners,
        cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

    if (!found) {
        LOG_WARNING("未检测到标定板角点");
        return false;
    }

    // 亚像素精化
    cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
        cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001));

    // 确保点数匹配
    if (static_cast<int>(corners.size()) != totalPointCount()) {
        setPointCount(patternSize.height, patternSize.width);
    }

    // 设置像素坐标
    for (int i = 0; i < static_cast<int>(corners.size()) && i < calibPoints_.size(); ++i) {
        calibPoints_[i].pixelCoord = QPointF(corners[i].x, corners[i].y);
        calibPoints_[i].index = i;
        // 如果世界坐标已设置，标记为完整
        if (calibPoints_[i].worldCoord != QPointF(0, 0)) {
            calibPoints_[i].isSet = true;
        }
    }

    LOG_INFO(QString("检测到 %1 个标定点").arg(corners.size()));
    return true;
}

// ========== 标定执行 ==========

void CoordinateTransform::setTransformType(TransformType type)
{
    if (transformType_ != type) {
        transformType_ = type;
        isCalibrated_ = false;
    }
}

TransformType CoordinateTransform::getTransformType() const
{
    return transformType_;
}

bool CoordinateTransform::calibrate()
{
    // 检查最小点数
    int minPoints = 3; // 仿射至少需要3点
    if (transformType_ == TransformType::Perspective) {
        minPoints = 4;
    }

    int validPoints = setPointCount();
    if (validPoints < minPoints) {
        LOG_ERROR(QString("标定点不足: 需要至少%1个，当前%2个").arg(minPoints).arg(validPoints));
        stats_.success = false;
        emit calibrationCompleted(false);
        return false;
    }

    bool success = false;

    switch (transformType_) {
    case TransformType::Affine:
        success = computeAffineTransform();
        break;
    case TransformType::Perspective:
        success = computePerspectiveTransform();
        break;
    case TransformType::Similarity:
        success = computeSimilarityTransform();
        break;
    case TransformType::Rigid:
        success = computeRigidTransform();
        break;
    }

    if (success) {
        computeCalibrationErrors();
        isCalibrated_ = true;
        calibrationTimestamp_ = QDateTime::currentMSecsSinceEpoch();

        LOG_INFO(QString("标定成功: 类型=%1, 平均误差=%2, 最大误差=%3")
                 .arg(static_cast<int>(transformType_))
                 .arg(stats_.meanError, 0, 'f', 4)
                 .arg(stats_.maxError, 0, 'f', 4));
    } else {
        isCalibrated_ = false;
        LOG_ERROR("标定失败");
    }

    emit calibrationCompleted(success);
    emit calibrationChanged();

    return success;
}

bool CoordinateTransform::computeAffineTransform()
{
    std::vector<cv::Point2f> srcPoints, dstPoints;

    for (const CalibPointPair& point : calibPoints_) {
        if (point.isSet) {
            srcPoints.push_back(cv::Point2f(point.pixelCoord.x(), point.pixelCoord.y()));
            dstPoints.push_back(cv::Point2f(point.worldCoord.x(), point.worldCoord.y()));
        }
    }

    if (srcPoints.size() < 3) {
        return false;
    }

    // 使用最小二乘法估计仿射变换
    // estimateAffinePartial2D 用于相似变换，estimateAffine2D 用于完整仿射
    cv::Mat inliers;
    transformMatrix_ = cv::estimateAffine2D(srcPoints, dstPoints, inliers);

    if (transformMatrix_.empty()) {
        return false;
    }

    // 计算逆变换矩阵
    cv::Mat extended = cv::Mat::eye(3, 3, CV_64F);
    transformMatrix_.copyTo(extended(cv::Rect(0, 0, 3, 2)));
    cv::Mat invExtended = extended.inv();
    inverseMatrix_ = invExtended(cv::Rect(0, 0, 3, 2)).clone();

    return true;
}

bool CoordinateTransform::computePerspectiveTransform()
{
    std::vector<cv::Point2f> srcPoints, dstPoints;

    for (const CalibPointPair& point : calibPoints_) {
        if (point.isSet) {
            srcPoints.push_back(cv::Point2f(point.pixelCoord.x(), point.pixelCoord.y()));
            dstPoints.push_back(cv::Point2f(point.worldCoord.x(), point.worldCoord.y()));
        }
    }

    if (srcPoints.size() < 4) {
        return false;
    }

    // 计算透视变换矩阵
    transformMatrix_ = cv::findHomography(srcPoints, dstPoints, cv::RANSAC);

    if (transformMatrix_.empty()) {
        return false;
    }

    // 计算逆矩阵
    inverseMatrix_ = transformMatrix_.inv();

    return true;
}

bool CoordinateTransform::computeSimilarityTransform()
{
    std::vector<cv::Point2f> srcPoints, dstPoints;

    for (const CalibPointPair& point : calibPoints_) {
        if (point.isSet) {
            srcPoints.push_back(cv::Point2f(point.pixelCoord.x(), point.pixelCoord.y()));
            dstPoints.push_back(cv::Point2f(point.worldCoord.x(), point.worldCoord.y()));
        }
    }

    if (srcPoints.size() < 2) {
        return false;
    }

    // 相似变换：只有平移、旋转、均匀缩放
    cv::Mat inliers;
    transformMatrix_ = cv::estimateAffinePartial2D(srcPoints, dstPoints, inliers);

    if (transformMatrix_.empty()) {
        return false;
    }

    // 计算逆变换
    cv::Mat extended = cv::Mat::eye(3, 3, CV_64F);
    transformMatrix_.copyTo(extended(cv::Rect(0, 0, 3, 2)));
    cv::Mat invExtended = extended.inv();
    inverseMatrix_ = invExtended(cv::Rect(0, 0, 3, 2)).clone();

    return true;
}

bool CoordinateTransform::computeRigidTransform()
{
    // 刚性变换：SVD分解求解最佳旋转和平移

    std::vector<cv::Point2f> srcPoints, dstPoints;

    for (const CalibPointPair& point : calibPoints_) {
        if (point.isSet) {
            srcPoints.push_back(cv::Point2f(point.pixelCoord.x(), point.pixelCoord.y()));
            dstPoints.push_back(cv::Point2f(point.worldCoord.x(), point.worldCoord.y()));
        }
    }

    if (srcPoints.size() < 2) {
        return false;
    }

    int n = srcPoints.size();

    // 计算质心
    cv::Point2f srcCentroid(0, 0), dstCentroid(0, 0);
    for (int i = 0; i < n; ++i) {
        srcCentroid += srcPoints[i];
        dstCentroid += dstPoints[i];
    }
    srcCentroid *= (1.0f / n);
    dstCentroid *= (1.0f / n);

    // 中心化
    cv::Mat srcMat(n, 2, CV_64F);
    cv::Mat dstMat(n, 2, CV_64F);

    for (int i = 0; i < n; ++i) {
        srcMat.at<double>(i, 0) = srcPoints[i].x - srcCentroid.x;
        srcMat.at<double>(i, 1) = srcPoints[i].y - srcCentroid.y;
        dstMat.at<double>(i, 0) = dstPoints[i].x - dstCentroid.x;
        dstMat.at<double>(i, 1) = dstPoints[i].y - dstCentroid.y;
    }

    // 协方差矩阵 H = src^T * dst
    cv::Mat H = srcMat.t() * dstMat;

    // SVD分解
    cv::Mat U, S, Vt;
    cv::SVD::compute(H, S, U, Vt);

    // 旋转矩阵 R = V * U^T
    cv::Mat R = Vt.t() * U.t();

    // 检查反射（行列式为-1）
    double det = cv::determinant(R);
    if (det < 0) {
        Vt.row(1) *= -1;
        R = Vt.t() * U.t();
    }

    // 平移向量 t = dstCentroid - R * srcCentroid
    cv::Mat srcCentroidMat = (cv::Mat_<double>(2, 1) << srcCentroid.x, srcCentroid.y);
    cv::Mat dstCentroidMat = (cv::Mat_<double>(2, 1) << dstCentroid.x, dstCentroid.y);
    cv::Mat t = dstCentroidMat - R * srcCentroidMat;

    // 构建2x3仿射矩阵
    transformMatrix_ = cv::Mat(2, 3, CV_64F);
    R.copyTo(transformMatrix_(cv::Rect(0, 0, 2, 2)));
    transformMatrix_.at<double>(0, 2) = t.at<double>(0, 0);
    transformMatrix_.at<double>(1, 2) = t.at<double>(1, 0);

    // 计算逆变换
    cv::Mat Rinv = R.t();
    cv::Mat tinv = -Rinv * t;

    inverseMatrix_ = cv::Mat(2, 3, CV_64F);
    Rinv.copyTo(inverseMatrix_(cv::Rect(0, 0, 2, 2)));
    inverseMatrix_.at<double>(0, 2) = tinv.at<double>(0, 0);
    inverseMatrix_.at<double>(1, 2) = tinv.at<double>(1, 0);

    return true;
}

void CoordinateTransform::computeCalibrationErrors()
{
    stats_.pointErrors.clear();
    stats_.pointCount = 0;
    stats_.meanError = 0;
    stats_.maxError = 0;
    stats_.rmsError = 0;

    double totalError = 0;
    double totalSquaredError = 0;
    double totalPixelDist = 0;
    double totalWorldDist = 0;

    for (const CalibPointPair& point : calibPoints_) {
        if (!point.isSet) continue;

        // 变换像素坐标到世界坐标
        QPointF transformed = pixelToWorld(point.pixelCoord);

        // 计算误差
        double dx = transformed.x() - point.worldCoord.x();
        double dy = transformed.y() - point.worldCoord.y();
        double error = std::sqrt(dx * dx + dy * dy);

        stats_.pointErrors.append(error);
        totalError += error;
        totalSquaredError += error * error;

        if (error > stats_.maxError) {
            stats_.maxError = error;
        }

        stats_.pointCount++;
    }

    if (stats_.pointCount > 0) {
        stats_.meanError = totalError / stats_.pointCount;
        stats_.rmsError = std::sqrt(totalSquaredError / stats_.pointCount);
    }

    // 估计像素/毫米比例
    if (calibPoints_.size() >= 2) {
        for (int i = 0; i < calibPoints_.size() - 1; ++i) {
            if (!calibPoints_[i].isSet || !calibPoints_[i + 1].isSet) continue;

            double pdx = calibPoints_[i + 1].pixelCoord.x() - calibPoints_[i].pixelCoord.x();
            double pdy = calibPoints_[i + 1].pixelCoord.y() - calibPoints_[i].pixelCoord.y();
            double pixelDist = std::sqrt(pdx * pdx + pdy * pdy);

            double wdx = calibPoints_[i + 1].worldCoord.x() - calibPoints_[i].worldCoord.x();
            double wdy = calibPoints_[i + 1].worldCoord.y() - calibPoints_[i].worldCoord.y();
            double worldDist = std::sqrt(wdx * wdx + wdy * wdy);

            if (worldDist > 0.001) {
                totalPixelDist += pixelDist;
                totalWorldDist += worldDist;
            }
        }

        if (totalWorldDist > 0.001) {
            stats_.pixelPerMm = totalPixelDist / totalWorldDist;
        }
    }

    stats_.success = true;
}

CalibStats CoordinateTransform::getCalibStats() const
{
    return stats_;
}

CoordCalibResult CoordinateTransform::getCalibrationResult() const
{
    CoordCalibResult result;
    result.id = id_;
    result.name = name_;
    result.cameraId = cameraId_;
    result.transformType = transformType_;
    result.transformMatrix = transformMatrix_.clone();
    result.inverseMatrix = inverseMatrix_.clone();
    result.stats = stats_;
    result.calibPoints = calibPoints_;
    result.timestamp = calibrationTimestamp_;
    return result;
}

void CoordinateTransform::setCalibrationResult(const CoordCalibResult& result)
{
    id_ = result.id;
    name_ = result.name;
    cameraId_ = result.cameraId;
    transformType_ = result.transformType;
    transformMatrix_ = result.transformMatrix.clone();
    inverseMatrix_ = result.inverseMatrix.clone();
    stats_ = result.stats;
    calibrationTimestamp_ = result.timestamp;

    // 恢复标定点
    if (!result.calibPoints.isEmpty()) {
        int maxIndex = 0;
        for (const CalibPointPair& p : result.calibPoints) {
            if (p.index > maxIndex) maxIndex = p.index;
        }

        // 计算网格大小
        int total = maxIndex + 1;
        gridRows_ = static_cast<int>(std::sqrt(total));
        gridCols_ = (total + gridRows_ - 1) / gridRows_;

        calibPoints_.resize(total);
        for (int i = 0; i < calibPoints_.size(); ++i) {
            calibPoints_[i].index = i;
            calibPoints_[i].isSet = false;
        }

        for (const CalibPointPair& p : result.calibPoints) {
            if (p.index >= 0 && p.index < calibPoints_.size()) {
                calibPoints_[p.index] = p;
            }
        }
    }

    isCalibrated_ = result.isValid();

    emit calibrationChanged();
}

bool CoordinateTransform::isCalibrated() const
{
    return isCalibrated_ && !transformMatrix_.empty();
}

// ========== 坐标转换 ==========

QPointF CoordinateTransform::pixelToWorld(const QPointF& pixel) const
{
    if (!isCalibrated_ || transformMatrix_.empty()) {
        return pixel;
    }
    return applyTransform(pixel, transformMatrix_);
}

QPointF CoordinateTransform::worldToPixel(const QPointF& world) const
{
    if (!isCalibrated_ || inverseMatrix_.empty()) {
        return world;
    }
    return applyTransform(world, inverseMatrix_);
}

QVector<QPointF> CoordinateTransform::pixelToWorld(const QVector<QPointF>& pixels) const
{
    QVector<QPointF> result;
    result.reserve(pixels.size());
    for (const QPointF& p : pixels) {
        result.append(pixelToWorld(p));
    }
    return result;
}

QVector<QPointF> CoordinateTransform::worldToPixel(const QVector<QPointF>& worlds) const
{
    QVector<QPointF> result;
    result.reserve(worlds.size());
    for (const QPointF& p : worlds) {
        result.append(worldToPixel(p));
    }
    return result;
}

double CoordinateTransform::pixelToWorldDistance(double pixelDist) const
{
    if (!isCalibrated_ || stats_.pixelPerMm <= 0) {
        return pixelDist;
    }
    return pixelDist / stats_.pixelPerMm;
}

double CoordinateTransform::worldToPixelDistance(double worldDist) const
{
    if (!isCalibrated_ || stats_.pixelPerMm <= 0) {
        return worldDist;
    }
    return worldDist * stats_.pixelPerMm;
}

double CoordinateTransform::pixelToWorldAngle(double pixelAngle) const
{
    if (!isCalibrated_ || transformMatrix_.empty()) {
        return pixelAngle;
    }

    // 从变换矩阵提取旋转分量
    double a11 = transformMatrix_.at<double>(0, 0);
    double a21 = transformMatrix_.at<double>(1, 0);
    double rotation = std::atan2(a21, a11);

    return pixelAngle + rotation;
}

cv::Mat CoordinateTransform::getTransformMatrix() const
{
    return transformMatrix_.clone();
}

cv::Mat CoordinateTransform::getInverseMatrix() const
{
    return inverseMatrix_.clone();
}

QPointF CoordinateTransform::applyTransform(const QPointF& point, const cv::Mat& matrix) const
{
    if (matrix.empty()) {
        return point;
    }

    double x = point.x();
    double y = point.y();
    double newX, newY;

    if (matrix.rows == 3 && matrix.cols == 3) {
        // 透视变换 3x3
        double w = matrix.at<double>(2, 0) * x + matrix.at<double>(2, 1) * y + matrix.at<double>(2, 2);
        newX = (matrix.at<double>(0, 0) * x + matrix.at<double>(0, 1) * y + matrix.at<double>(0, 2)) / w;
        newY = (matrix.at<double>(1, 0) * x + matrix.at<double>(1, 1) * y + matrix.at<double>(1, 2)) / w;
    } else if (matrix.rows == 2 && matrix.cols == 3) {
        // 仿射变换 2x3
        newX = matrix.at<double>(0, 0) * x + matrix.at<double>(0, 1) * y + matrix.at<double>(0, 2);
        newY = matrix.at<double>(1, 0) * x + matrix.at<double>(1, 1) * y + matrix.at<double>(1, 2);
    } else {
        return point;
    }

    return QPointF(newX, newY);
}

// ========== 序列化 ==========

bool CoordinateTransform::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法打开文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("保存标定结果: %1").arg(filePath));
    return true;
}

bool CoordinateTransform::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开文件: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        LOG_ERROR(QString("解析JSON失败: %1").arg(error.errorString()));
        return false;
    }

    if (!fromJson(doc.object())) {
        return false;
    }

    LOG_INFO(QString("加载标定结果: %1").arg(filePath));
    return true;
}

QJsonObject CoordinateTransform::toJson() const
{
    return getCalibrationResult().toJson();
}

bool CoordinateTransform::fromJson(const QJsonObject& json)
{
    CoordCalibResult result = CoordCalibResult::fromJson(json);
    setCalibrationResult(result);
    return result.isValid() || !result.calibPoints.isEmpty();
}

// ========== 标定信息 ==========

void CoordinateTransform::setId(const QString& id)
{
    id_ = id;
}

QString CoordinateTransform::getId() const
{
    return id_;
}

void CoordinateTransform::setName(const QString& name)
{
    name_ = name;
}

QString CoordinateTransform::getName() const
{
    return name_;
}

void CoordinateTransform::setCameraId(const QString& cameraId)
{
    cameraId_ = cameraId;
}

QString CoordinateTransform::getCameraId() const
{
    return cameraId_;
}

} // namespace Algorithm
} // namespace VisionForge
