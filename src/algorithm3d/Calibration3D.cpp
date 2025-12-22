/**
 * @file Calibration3D.cpp
 * @brief VisionForge Pro 3D标定模块实现
 */

#include "algorithm3d/Calibration3D.h"
#include <QFile>
#include <QJsonDocument>
#include <algorithm>
#include <cmath>

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// CalibrationBoardConfig
// ============================================================

std::vector<Point3D> CalibrationBoardConfig::generate3DPoints() const {
    std::vector<Point3D> points;
    points.reserve(static_cast<size_t>(rows * cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            points.emplace_back(
                static_cast<float>(j * squareSize),
                static_cast<float>(i * squareSize),
                0.0f
            );
        }
    }

    return points;
}

QJsonObject CalibrationBoardConfig::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(type);
    json["rows"] = rows;
    json["cols"] = cols;
    json["squareSize"] = squareSize;
    json["markerSize"] = markerSize;
    return json;
}

CalibrationBoardConfig CalibrationBoardConfig::fromJson(const QJsonObject& json) {
    CalibrationBoardConfig config;
    config.type = static_cast<CalibrationBoardType>(json["type"].toInt());
    config.rows = json["rows"].toInt(9);
    config.cols = json["cols"].toInt(6);
    config.squareSize = json["squareSize"].toDouble(25.0);
    config.markerSize = json["markerSize"].toDouble(18.0);
    return config;
}

// ============================================================
// HandEyeCalibrationData
// ============================================================

QJsonObject HandEyeCalibrationData::toJson() const {
    QJsonObject json;

    QJsonArray robotPoseArray;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            robotPoseArray.append(robotPose(i, j));
        }
    }
    json["robotPose"] = robotPoseArray;

    QJsonArray cameraPoseArray;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cameraPoseArray.append(cameraPose(i, j));
        }
    }
    json["cameraPose"] = cameraPoseArray;

    json["valid"] = valid;
    return json;
}

HandEyeCalibrationData HandEyeCalibrationData::fromJson(const QJsonObject& json) {
    HandEyeCalibrationData data;

    QJsonArray robotPoseArray = json["robotPose"].toArray();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            data.robotPose(i, j) = robotPoseArray[i * 4 + j].toDouble();
        }
    }

    QJsonArray cameraPoseArray = json["cameraPose"].toArray();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            data.cameraPose(i, j) = cameraPoseArray[i * 4 + j].toDouble();
        }
    }

    data.valid = json["valid"].toBool();
    return data;
}

// ============================================================
// CameraCalibrationResult
// ============================================================

QJsonObject CameraCalibrationResult::toJson() const {
    QJsonObject json;
    json["success"] = success;
    json["errorMessage"] = errorMessage;
    json["intrinsics"] = intrinsics.toJson();
    json["reprojectionError"] = reprojectionError;
    return json;
}

CameraCalibrationResult CameraCalibrationResult::fromJson(const QJsonObject& json) {
    CameraCalibrationResult result;
    result.success = json["success"].toBool();
    result.errorMessage = json["errorMessage"].toString();
    result.intrinsics = CameraIntrinsics::fromJson(json["intrinsics"].toObject());
    result.reprojectionError = json["reprojectionError"].toDouble();
    return result;
}

// ============================================================
// StereoCalibrationResult
// ============================================================

QJsonObject StereoCalibrationResult::toJson() const {
    QJsonObject json;
    json["success"] = success;
    json["errorMessage"] = errorMessage;
    json["leftIntrinsics"] = leftIntrinsics.toJson();
    json["rightIntrinsics"] = rightIntrinsics.toJson();
    json["rightToLeft"] = rightToLeft.toJson();
    json["stereoParams"] = stereoParams.toJson();
    json["reprojectionError"] = reprojectionError;
    return json;
}

StereoCalibrationResult StereoCalibrationResult::fromJson(const QJsonObject& json) {
    StereoCalibrationResult result;
    result.success = json["success"].toBool();
    result.errorMessage = json["errorMessage"].toString();
    result.leftIntrinsics = CameraIntrinsics::fromJson(json["leftIntrinsics"].toObject());
    result.rightIntrinsics = CameraIntrinsics::fromJson(json["rightIntrinsics"].toObject());
    result.rightToLeft = CameraExtrinsics::fromJson(json["rightToLeft"].toObject());
    result.stereoParams = StereoParameters::fromJson(json["stereoParams"].toObject());
    result.reprojectionError = json["reprojectionError"].toDouble();
    return result;
}

// ============================================================
// HandEyeCalibrationResult
// ============================================================

QJsonObject HandEyeCalibrationResult::toJson() const {
    QJsonObject json;
    json["success"] = success;
    json["errorMessage"] = errorMessage;

    QJsonArray transformArray;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            transformArray.append(handEyeTransform(i, j));
        }
    }
    json["handEyeTransform"] = transformArray;

    json["rotationError"] = rotationError;
    json["translationError"] = translationError;
    return json;
}

HandEyeCalibrationResult HandEyeCalibrationResult::fromJson(const QJsonObject& json) {
    HandEyeCalibrationResult result;
    result.success = json["success"].toBool();
    result.errorMessage = json["errorMessage"].toString();

    QJsonArray transformArray = json["handEyeTransform"].toArray();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.handEyeTransform(i, j) = transformArray[i * 4 + j].toDouble();
        }
    }

    result.rotationError = json["rotationError"].toDouble();
    result.translationError = json["translationError"].toDouble();
    return result;
}

// ============================================================
// Calibration3DManager
// ============================================================

Calibration3DManager::Calibration3DManager(QObject* parent)
    : QObject(parent)
{
}

Calibration3DManager::~Calibration3DManager() = default;

void Calibration3DManager::setBoardConfig(const CalibrationBoardConfig& config) {
    QMutexLocker locker(&m_mutex);
    m_boardConfig = config;
}

bool Calibration3DManager::detectCorners(const QImage& image, std::vector<QPointF>& corners) {
    switch (m_boardConfig.type) {
        case CalibrationBoardType::Chessboard:
            return detectChessboardCorners(image, corners);
        case CalibrationBoardType::CircleGrid:
        case CalibrationBoardType::AsymmetricCircleGrid:
            return detectCircleGridCorners(image, corners);
        default:
            return false;
    }
}

bool Calibration3DManager::detectChessboardCorners(const QImage& image,
                                                    std::vector<QPointF>& corners) {
    Q_UNUSED(image);
    Q_UNUSED(corners);
    // 需要OpenCV实现
    return false;
}

bool Calibration3DManager::detectCircleGridCorners(const QImage& image,
                                                    std::vector<QPointF>& corners) {
    Q_UNUSED(image);
    Q_UNUSED(corners);
    // 需要OpenCV实现
    return false;
}

QImage Calibration3DManager::drawCorners(const QImage& image,
                                          const std::vector<QPointF>& corners,
                                          bool found) {
    QImage result = image.copy();

    // 简单绘制角点
    for (const auto& corner : corners) {
        int x = static_cast<int>(corner.x());
        int y = static_cast<int>(corner.y());

        QRgb color = found ? qRgb(0, 255, 0) : qRgb(255, 0, 0);

        for (int dx = -2; dx <= 2; ++dx) {
            for (int dy = -2; dy <= 2; ++dy) {
                int px = x + dx;
                int py = y + dy;
                if (px >= 0 && px < result.width() && py >= 0 && py < result.height()) {
                    result.setPixel(px, py, color);
                }
            }
        }
    }

    return result;
}

void Calibration3DManager::refineCorners(const QImage& image,
                                          std::vector<QPointF>& corners,
                                          int winSize) {
    Q_UNUSED(image);
    Q_UNUSED(corners);
    Q_UNUSED(winSize);
    // 需要OpenCV实现
}

bool Calibration3DManager::addCalibrationFrame(const QImage& image) {
    CalibrationFrame frame;
    frame.image = image;

    if (!detectCorners(image, frame.imagePoints)) {
        emit cornersDetected(false, 0);
        return false;
    }

    frame.objectPoints = m_boardConfig.generate3DPoints();
    frame.detected = true;

    QMutexLocker locker(&m_mutex);
    m_frames.push_back(frame);

    emit cornersDetected(true, static_cast<int>(frame.imagePoints.size()));
    return true;
}

void Calibration3DManager::clearFrames() {
    QMutexLocker locker(&m_mutex);
    m_frames.clear();
}

void Calibration3DManager::removeFrame(int index) {
    QMutexLocker locker(&m_mutex);
    if (index >= 0 && index < static_cast<int>(m_frames.size())) {
        m_frames.erase(m_frames.begin() + index);
    }
}

CameraCalibrationResult Calibration3DManager::calibrateCamera(int flags) {
    Q_UNUSED(flags);

    CameraCalibrationResult result;

    QMutexLocker locker(&m_mutex);

    if (m_frames.size() < 3) {
        result.success = false;
        result.errorMessage = "标定图像数量不足（至少需要3张）";
        emit calibrationCompleted(false);
        return result;
    }

    // 简化实现：需要OpenCV进行实际标定
    result.success = false;
    result.errorMessage = "相机标定需要OpenCV支持";

    emit calibrationCompleted(result.success);
    return result;
}

bool Calibration3DManager::addStereoFrame(const QImage& leftImage, const QImage& rightImage) {
    StereoCalibrationFrame frame;

    if (!detectCorners(leftImage, frame.left.imagePoints)) {
        return false;
    }

    if (!detectCorners(rightImage, frame.right.imagePoints)) {
        return false;
    }

    frame.left.objectPoints = m_boardConfig.generate3DPoints();
    frame.right.objectPoints = m_boardConfig.generate3DPoints();
    frame.left.detected = true;
    frame.right.detected = true;

    QMutexLocker locker(&m_mutex);
    m_stereoFrames.push_back(frame);

    return true;
}

void Calibration3DManager::clearStereoFrames() {
    QMutexLocker locker(&m_mutex);
    m_stereoFrames.clear();
}

StereoCalibrationResult Calibration3DManager::calibrateStereo(int flags) {
    Q_UNUSED(flags);

    StereoCalibrationResult result;
    result.success = false;
    result.errorMessage = "立体标定需要OpenCV支持";

    emit calibrationCompleted(result.success);
    return result;
}

bool Calibration3DManager::computeRectificationMaps(const StereoCalibrationResult& result,
                                                     QImage& mapLeft, QImage& mapRight) {
    Q_UNUSED(result);
    Q_UNUSED(mapLeft);
    Q_UNUSED(mapRight);
    return false;
}

bool Calibration3DManager::addHandEyeData(const Matrix4d& robotPose, const QImage& cameraImage) {
    CalibrationFrame frame;
    frame.image = cameraImage;

    if (!detectCorners(cameraImage, frame.imagePoints)) {
        return false;
    }

    frame.objectPoints = m_boardConfig.generate3DPoints();
    frame.detected = true;

    HandEyeCalibrationData data;
    data.robotPose = robotPose;
    // 计算相机位姿需要标定结果
    data.valid = false;

    QMutexLocker locker(&m_mutex);
    m_handEyeData.push_back(data);

    return true;
}

void Calibration3DManager::clearHandEyeData() {
    QMutexLocker locker(&m_mutex);
    m_handEyeData.clear();
}

HandEyeCalibrationResult Calibration3DManager::calibrateHandEyeEIH(HandEyeMethod method) {
    Q_UNUSED(method);

    HandEyeCalibrationResult result;
    result.success = false;
    result.errorMessage = "手眼标定需要OpenCV支持";

    return result;
}

HandEyeCalibrationResult Calibration3DManager::calibrateHandEyeETH(HandEyeMethod method) {
    Q_UNUSED(method);

    HandEyeCalibrationResult result;
    result.success = false;
    result.errorMessage = "手眼标定需要OpenCV支持";

    return result;
}

QImage Calibration3DManager::undistort(const QImage& image, const CameraIntrinsics& intrinsics) {
    Q_UNUSED(intrinsics);
    // 需要OpenCV实现
    return image;
}

void Calibration3DManager::computeUndistortMap(const CameraIntrinsics& intrinsics,
                                                int width, int height,
                                                QImage& mapX, QImage& mapY) {
    Q_UNUSED(intrinsics);
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(mapX);
    Q_UNUSED(mapY);
}

QImage Calibration3DManager::remap(const QImage& image,
                                    const QImage& mapX, const QImage& mapY) {
    Q_UNUSED(mapX);
    Q_UNUSED(mapY);
    return image;
}

Matrix4d Calibration3DManager::solvePnP(const std::vector<Point3D>& objectPoints,
                                         const std::vector<QPointF>& imagePoints,
                                         const CameraIntrinsics& intrinsics) {
    Q_UNUSED(objectPoints);
    Q_UNUSED(imagePoints);
    Q_UNUSED(intrinsics);
    return Matrix4d::Identity();
}

bool Calibration3DManager::saveCameraCalibration(const QString& filename,
                                                  const CameraCalibrationResult& result) {
    QJsonDocument doc(result.toJson());
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    return true;
}

bool Calibration3DManager::loadCameraCalibration(const QString& filename,
                                                  CameraCalibrationResult& result) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    result = CameraCalibrationResult::fromJson(doc.object());
    return true;
}

bool Calibration3DManager::saveStereoCalibration(const QString& filename,
                                                  const StereoCalibrationResult& result) {
    QJsonDocument doc(result.toJson());
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    return true;
}

bool Calibration3DManager::loadStereoCalibration(const QString& filename,
                                                  StereoCalibrationResult& result) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    result = StereoCalibrationResult::fromJson(doc.object());
    return true;
}

bool Calibration3DManager::saveHandEyeCalibration(const QString& filename,
                                                   const HandEyeCalibrationResult& result) {
    QJsonDocument doc(result.toJson());
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    return true;
}

bool Calibration3DManager::loadHandEyeCalibration(const QString& filename,
                                                   HandEyeCalibrationResult& result) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    result = HandEyeCalibrationResult::fromJson(doc.object());
    return true;
}

double Calibration3DManager::validateCalibration(const CameraCalibrationResult& result,
                                                  const std::vector<CalibrationFrame>& testFrames) {
    Q_UNUSED(result);
    Q_UNUSED(testFrames);
    return 0.0;
}

double Calibration3DManager::validateStereoCalibration(
    const StereoCalibrationResult& result,
    const std::vector<StereoCalibrationFrame>& testFrames) {
    Q_UNUSED(result);
    Q_UNUSED(testFrames);
    return 0.0;
}

// ============================================================
// CameraCalibrator
// ============================================================

CameraCalibrator::CameraCalibrator() = default;
CameraCalibrator::~CameraCalibrator() = default;

void CameraCalibrator::setBoard(const CalibrationBoardConfig& config) {
    m_boardConfig = config;
}

bool CameraCalibrator::addImage(const QImage& image) {
    Q_UNUSED(image);
    return false;
}

CameraCalibrationResult CameraCalibrator::calibrate() {
    CameraCalibrationResult result;
    result.success = false;
    result.errorMessage = "需要OpenCV支持";
    return result;
}

int CameraCalibrator::getFrameCount() const {
    return static_cast<int>(m_frames.size());
}

void CameraCalibrator::clear() {
    m_frames.clear();
}

// ============================================================
// StereoCalibrator
// ============================================================

StereoCalibrator::StereoCalibrator() = default;
StereoCalibrator::~StereoCalibrator() = default;

void StereoCalibrator::setBoard(const CalibrationBoardConfig& config) {
    m_boardConfig = config;
}

bool StereoCalibrator::addImagePair(const QImage& leftImage, const QImage& rightImage) {
    Q_UNUSED(leftImage);
    Q_UNUSED(rightImage);
    return false;
}

StereoCalibrationResult StereoCalibrator::calibrate() {
    StereoCalibrationResult result;
    result.success = false;
    result.errorMessage = "需要OpenCV支持";
    return result;
}

int StereoCalibrator::getFrameCount() const {
    return static_cast<int>(m_stereoFrames.size());
}

void StereoCalibrator::clear() {
    m_stereoFrames.clear();
}

// ============================================================
// 工具函数
// ============================================================

QString getCalibration3DTypeName(Calibration3DType type) {
    switch (type) {
        case Calibration3DType::CameraIntrinsic: return "相机内参标定";
        case Calibration3DType::StereoPair: return "双目立体标定";
        case Calibration3DType::MultiCamera: return "多相机标定";
        case Calibration3DType::HandEye: return "手眼标定";
        case Calibration3DType::StructuredLight: return "结构光标定";
        case Calibration3DType::RobotCamera: return "机器人相机标定";
        default: return "未知标定类型";
    }
}

QString getCalibrationBoardTypeName(CalibrationBoardType type) {
    switch (type) {
        case CalibrationBoardType::Chessboard: return "棋盘格";
        case CalibrationBoardType::CircleGrid: return "圆点阵列";
        case CalibrationBoardType::AsymmetricCircleGrid: return "非对称圆点阵列";
        case CalibrationBoardType::CharucoBoard: return "ChArUco板";
        case CalibrationBoardType::AprilTag: return "AprilTag";
        default: return "未知标定板";
    }
}

QString getHandEyeMethodName(HandEyeMethod method) {
    switch (method) {
        case HandEyeMethod::Tsai: return "Tsai";
        case HandEyeMethod::Park: return "Park";
        case HandEyeMethod::Horaud: return "Horaud";
        case HandEyeMethod::Daniilidis: return "Daniilidis";
        case HandEyeMethod::Andreff: return "Andreff";
        default: return "未知方法";
    }
}

double evaluatePoseDistribution(const std::vector<Matrix4d>& poses) {
    if (poses.size() < 2) {
        return 0.0;
    }

    // 简单评估：计算位姿之间的角度差异
    double totalAngleDiff = 0;
    int count = 0;

    for (size_t i = 0; i < poses.size(); ++i) {
        for (size_t j = i + 1; j < poses.size(); ++j) {
            Matrix3d R1 = poses[i].block<3, 3>(0, 0);
            Matrix3d R2 = poses[j].block<3, 3>(0, 0);
            Matrix3d Rdiff = R1.transpose() * R2;

            double trace = Rdiff.trace();
            double angle = std::acos(std::max(-1.0, std::min(1.0, (trace - 1) / 2)));
            totalAngleDiff += angle;
            ++count;
        }
    }

    // 返回归一化分数（0-1）
    double avgAngle = count > 0 ? totalAngleDiff / count : 0;
    return std::min(1.0, avgAngle / M_PI);
}

double computeReprojectionError(const std::vector<Point3D>& objectPoints,
                                 const std::vector<QPointF>& imagePoints,
                                 const CameraIntrinsics& intrinsics,
                                 const Matrix4d& pose) {
    if (objectPoints.size() != imagePoints.size() || objectPoints.empty()) {
        return 0.0;
    }

    double totalError = 0;
    Matrix3d R = pose.block<3, 3>(0, 0);
    Vector3d t = pose.block<3, 1>(0, 3);

    for (size_t i = 0; i < objectPoints.size(); ++i) {
        Vector3d p(objectPoints[i].x, objectPoints[i].y, objectPoints[i].z);
        Vector3d pc = R * p + t;

        double projX = intrinsics.fx * pc.x() / pc.z() + intrinsics.cx;
        double projY = intrinsics.fy * pc.y() / pc.z() + intrinsics.cy;

        double dx = projX - imagePoints[i].x();
        double dy = projY - imagePoints[i].y();

        totalError += std::sqrt(dx * dx + dy * dy);
    }

    return totalError / static_cast<double>(objectPoints.size());
}

} // namespace Algorithm3D
} // namespace VisionForge
