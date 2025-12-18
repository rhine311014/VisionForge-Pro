/**
 * @file CalibrationResult.cpp
 * @brief 标定结果类实现
 */

#include "algorithm/CalibrationResult.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace Algorithm {

CalibrationResult::CalibrationResult()
    : type_(CalibrationType::None)
    , isValid_(false)
    , rmsError_(0.0)
    , imageSize_(0, 0)
    , pixelToMMX_(1.0)
    , pixelToMMY_(1.0)
    , rotationAngle_(0.0)
    , calibrationError_(0.0)
    , calibrationTime_(0)
{
    // 初始化默认矩阵
    cameraMatrix_ = cv::Mat::eye(3, 3, CV_64F);
    distCoeffs_ = cv::Mat::zeros(1, 5, CV_64F);
    homography_ = cv::Mat::eye(3, 3, CV_64F);
    homographyInv_ = cv::Mat::eye(3, 3, CV_64F);
    affineMatrix_ = cv::Mat::zeros(2, 3, CV_64F);
    affineMatrix_.at<double>(0, 0) = 1.0;
    affineMatrix_.at<double>(1, 1) = 1.0;
}

CalibrationResult::~CalibrationResult()
{
}

void CalibrationResult::reset()
{
    type_ = CalibrationType::None;
    isValid_ = false;

    cameraMatrix_ = cv::Mat::eye(3, 3, CV_64F);
    distCoeffs_ = cv::Mat::zeros(1, 5, CV_64F);
    rmsError_ = 0.0;
    imageSize_ = cv::Size(0, 0);

    homography_ = cv::Mat::eye(3, 3, CV_64F);
    homographyInv_ = cv::Mat::eye(3, 3, CV_64F);
    affineMatrix_ = cv::Mat::zeros(2, 3, CV_64F);
    affineMatrix_.at<double>(0, 0) = 1.0;
    affineMatrix_.at<double>(1, 1) = 1.0;
    pixelToMMX_ = 1.0;
    pixelToMMY_ = 1.0;
    rotationAngle_ = 0.0;
    calibrationError_ = 0.0;

    imagePoints_.clear();
    worldPoints_.clear();

    calibrationTime_ = 0;
    description_.clear();
}

// ========== 相机标定参数 ==========

void CalibrationResult::setCameraMatrix(const cv::Mat& matrix)
{
    if (matrix.rows == 3 && matrix.cols == 3) {
        matrix.convertTo(cameraMatrix_, CV_64F);
        type_ = CalibrationType::Camera;
        isValid_ = true;
    }
}

void CalibrationResult::setDistCoeffs(const cv::Mat& coeffs)
{
    if (coeffs.total() >= 4) {
        coeffs.reshape(1, 1).convertTo(distCoeffs_, CV_64F);
    }
}

cv::Point2d CalibrationResult::focalLength() const
{
    return cv::Point2d(
        cameraMatrix_.at<double>(0, 0),
        cameraMatrix_.at<double>(1, 1)
    );
}

cv::Point2d CalibrationResult::principalPoint() const
{
    return cv::Point2d(
        cameraMatrix_.at<double>(0, 2),
        cameraMatrix_.at<double>(1, 2)
    );
}

// ========== 九点标定参数 ==========

void CalibrationResult::setHomography(const cv::Mat& H)
{
    if (H.rows == 3 && H.cols == 3) {
        H.convertTo(homography_, CV_64F);
        homographyInv_ = homography_.inv();
        type_ = CalibrationType::NinePoint;
        isValid_ = true;
    }
}

void CalibrationResult::setAffineMatrix(const cv::Mat& M)
{
    if (M.rows == 2 && M.cols == 3) {
        M.convertTo(affineMatrix_, CV_64F);
    }
}

void CalibrationResult::setCalibPoints(const std::vector<cv::Point2d>& imagePoints,
                                       const std::vector<cv::Point2d>& worldPoints)
{
    if (imagePoints.size() == worldPoints.size()) {
        imagePoints_ = imagePoints;
        worldPoints_ = worldPoints;
    }
}

// ========== 坐标转换 ==========

cv::Point2d CalibrationResult::imageToWorld(const cv::Point2d& imagePoint) const
{
    if (!isValid_ || type_ != CalibrationType::NinePoint) {
        return imagePoint;
    }

    // 使用单应性矩阵进行转换
    std::vector<cv::Point2d> src = {imagePoint};
    std::vector<cv::Point2d> dst;

    cv::perspectiveTransform(src, dst, homography_);

    return dst.empty() ? imagePoint : dst[0];
}

cv::Point2d CalibrationResult::worldToImage(const cv::Point2d& worldPoint) const
{
    if (!isValid_ || type_ != CalibrationType::NinePoint) {
        return worldPoint;
    }

    // 使用单应性矩阵逆进行转换
    std::vector<cv::Point2d> src = {worldPoint};
    std::vector<cv::Point2d> dst;

    cv::perspectiveTransform(src, dst, homographyInv_);

    return dst.empty() ? worldPoint : dst[0];
}

std::vector<cv::Point2d> CalibrationResult::imageToWorld(const std::vector<cv::Point2d>& imagePoints) const
{
    if (!isValid_ || type_ != CalibrationType::NinePoint || imagePoints.empty()) {
        return imagePoints;
    }

    std::vector<cv::Point2d> worldPoints;
    cv::perspectiveTransform(imagePoints, worldPoints, homography_);
    return worldPoints;
}

std::vector<cv::Point2d> CalibrationResult::worldToImage(const std::vector<cv::Point2d>& worldPoints) const
{
    if (!isValid_ || type_ != CalibrationType::NinePoint || worldPoints.empty()) {
        return worldPoints;
    }

    std::vector<cv::Point2d> imagePoints;
    cv::perspectiveTransform(worldPoints, imagePoints, homographyInv_);
    return imagePoints;
}

cv::Mat CalibrationResult::undistort(const cv::Mat& src) const
{
    if (!isValid_ || type_ != CalibrationType::Camera || src.empty()) {
        return src.clone();
    }

    cv::Mat dst;
    cv::undistort(src, dst, cameraMatrix_, distCoeffs_);
    return dst;
}

// ========== 序列化 ==========

QJsonObject CalibrationResult::serialize() const
{
    QJsonObject json;

    // 格式标识
    json["formatType"] = "VisionForgeCalibration";
    json["formatVersion"] = "1.0";

    // 基本信息
    json["type"] = static_cast<int>(type_);
    json["isValid"] = isValid_;
    json["calibrationTime"] = calibrationTime_;
    json["description"] = description_;

    // 相机标定参数
    if (type_ == CalibrationType::Camera) {
        QJsonObject cameraObj;
        cameraObj["cameraMatrix"] = matToJsonArray(cameraMatrix_);
        cameraObj["distCoeffs"] = matToJsonArray(distCoeffs_);
        cameraObj["rmsError"] = rmsError_;
        cameraObj["imageWidth"] = imageSize_.width;
        cameraObj["imageHeight"] = imageSize_.height;
        json["camera"] = cameraObj;
    }

    // 九点标定参数
    if (type_ == CalibrationType::NinePoint) {
        QJsonObject ninePointObj;
        ninePointObj["homography"] = matToJsonArray(homography_);
        ninePointObj["affineMatrix"] = matToJsonArray(affineMatrix_);
        ninePointObj["pixelToMMX"] = pixelToMMX_;
        ninePointObj["pixelToMMY"] = pixelToMMY_;
        ninePointObj["rotationAngle"] = rotationAngle_;
        ninePointObj["calibrationError"] = calibrationError_;

        // 标定点
        QJsonArray imagePtsArray;
        for (const auto& pt : imagePoints_) {
            QJsonObject ptObj;
            ptObj["x"] = pt.x;
            ptObj["y"] = pt.y;
            imagePtsArray.append(ptObj);
        }
        ninePointObj["imagePoints"] = imagePtsArray;

        QJsonArray worldPtsArray;
        for (const auto& pt : worldPoints_) {
            QJsonObject ptObj;
            ptObj["x"] = pt.x;
            ptObj["y"] = pt.y;
            worldPtsArray.append(ptObj);
        }
        ninePointObj["worldPoints"] = worldPtsArray;

        json["ninePoint"] = ninePointObj;
    }

    return json;
}

bool CalibrationResult::deserialize(const QJsonObject& json)
{
    // 检查格式
    QString formatType = json["formatType"].toString();
    if (formatType != "VisionForgeCalibration") {
        LOG_WARNING(QString("未知的标定格式类型: %1").arg(formatType));
    }

    // 基本信息
    type_ = static_cast<CalibrationType>(json["type"].toInt());
    isValid_ = json["isValid"].toBool();
    calibrationTime_ = json["calibrationTime"].toVariant().toLongLong();
    description_ = json["description"].toString();

    // 相机标定参数
    if (json.contains("camera")) {
        QJsonObject cameraObj = json["camera"].toObject();

        cameraMatrix_ = jsonArrayToMat(cameraObj["cameraMatrix"].toArray(), 3, 3, CV_64F);
        distCoeffs_ = jsonArrayToMat(cameraObj["distCoeffs"].toArray(), 1, -1, CV_64F);
        rmsError_ = cameraObj["rmsError"].toDouble();
        imageSize_.width = cameraObj["imageWidth"].toInt();
        imageSize_.height = cameraObj["imageHeight"].toInt();
    }

    // 九点标定参数
    if (json.contains("ninePoint")) {
        QJsonObject npObj = json["ninePoint"].toObject();

        homography_ = jsonArrayToMat(npObj["homography"].toArray(), 3, 3, CV_64F);
        if (!homography_.empty()) {
            homographyInv_ = homography_.inv();
        }
        affineMatrix_ = jsonArrayToMat(npObj["affineMatrix"].toArray(), 2, 3, CV_64F);
        pixelToMMX_ = npObj["pixelToMMX"].toDouble();
        pixelToMMY_ = npObj["pixelToMMY"].toDouble();
        rotationAngle_ = npObj["rotationAngle"].toDouble();
        calibrationError_ = npObj["calibrationError"].toDouble();

        // 标定点
        imagePoints_.clear();
        QJsonArray imagePtsArray = npObj["imagePoints"].toArray();
        for (const auto& val : imagePtsArray) {
            QJsonObject ptObj = val.toObject();
            imagePoints_.emplace_back(ptObj["x"].toDouble(), ptObj["y"].toDouble());
        }

        worldPoints_.clear();
        QJsonArray worldPtsArray = npObj["worldPoints"].toArray();
        for (const auto& val : worldPtsArray) {
            QJsonObject ptObj = val.toObject();
            worldPoints_.emplace_back(ptObj["x"].toDouble(), ptObj["y"].toDouble());
        }
    }

    return true;
}

bool CalibrationResult::saveToFile(const QString& filePath) const
{
    QJsonObject json = serialize();
    QJsonDocument doc(json);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("保存标定文件失败: %1").arg(filePath));
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("保存标定结果: %1").arg(filePath));
    return true;
}

bool CalibrationResult::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("打开标定文件失败: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("标定文件格式错误: %1").arg(filePath));
        return false;
    }

    if (!deserialize(doc.object())) {
        LOG_ERROR(QString("反序列化标定失败: %1").arg(filePath));
        return false;
    }

    LOG_INFO(QString("加载标定结果: %1").arg(filePath));
    return true;
}

// ========== 辅助方法 ==========

QString CalibrationResult::summary() const
{
    QString text;
    QTextStream stream(&text);

    if (type_ == CalibrationType::Camera) {
        stream << "相机标定结果:\n";
        stream << "  图像尺寸: " << imageSize_.width << " x " << imageSize_.height << "\n";
        stream << "  焦距: fx=" << cameraMatrix_.at<double>(0, 0)
               << ", fy=" << cameraMatrix_.at<double>(1, 1) << "\n";
        stream << "  光心: cx=" << cameraMatrix_.at<double>(0, 2)
               << ", cy=" << cameraMatrix_.at<double>(1, 2) << "\n";
        stream << "  重投影误差: " << QString::number(rmsError_, 'f', 4) << " 像素\n";
    }
    else if (type_ == CalibrationType::NinePoint) {
        stream << "九点标定结果:\n";
        stream << "  标定点数: " << pointCount() << "\n";
        stream << "  像素比例: X=" << QString::number(pixelToMMX_, 'f', 4)
               << " mm/px, Y=" << QString::number(pixelToMMY_, 'f', 4) << " mm/px\n";
        stream << "  旋转角度: " << QString::number(rotationAngle_ * 180.0 / CV_PI, 'f', 2) << " 度\n";
        stream << "  标定精度: " << QString::number(calibrationError_, 'f', 4) << " mm\n";
    }
    else {
        stream << "未标定\n";
    }

    if (calibrationTime_ > 0) {
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(calibrationTime_);
        stream << "  标定时间: " << dt.toString("yyyy-MM-dd HH:mm:ss") << "\n";
    }

    return text;
}

// ========== 私有方法 ==========

QJsonArray CalibrationResult::matToJsonArray(const cv::Mat& mat)
{
    QJsonArray array;
    cv::Mat temp;
    mat.convertTo(temp, CV_64F);

    for (int i = 0; i < temp.rows; ++i) {
        for (int j = 0; j < temp.cols; ++j) {
            array.append(temp.at<double>(i, j));
        }
    }

    return array;
}

cv::Mat CalibrationResult::jsonArrayToMat(const QJsonArray& array, int rows, int cols, int type)
{
    if (array.isEmpty()) {
        return cv::Mat();
    }

    // 如果cols为-1，自动推断（用于1行矩阵）
    if (cols < 0) {
        cols = array.size();
        rows = 1;
    }

    if (array.size() != rows * cols) {
        return cv::Mat();
    }

    cv::Mat mat(rows, cols, type);
    int idx = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (type == CV_64F) {
                mat.at<double>(i, j) = array[idx++].toDouble();
            }
        }
    }

    return mat;
}

} // namespace Algorithm
} // namespace VisionForge
