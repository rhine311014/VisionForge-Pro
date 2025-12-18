/**
 * @file CalibrationManager.cpp
 * @brief 标定管理器实现
 */

#include "algorithm/CalibrationManager.h"
#include "algorithm/CalibrationResult.h"
#include "core/Recipe.h"
#include "base/Logger.h"
#include <QDir>
#include <QFile>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace Algorithm {

CalibrationManager::CalibrationManager()
    : currentRecipe_(nullptr)
    , cameraCalibResult_(std::make_unique<CalibrationResult>())
    , ninePointCalibResult_(std::make_unique<CalibrationResult>())
{
}

CalibrationManager::~CalibrationManager()
{
}

CalibrationManager& CalibrationManager::instance()
{
    static CalibrationManager instance;
    return instance;
}

// ========== 配方集成 ==========

void CalibrationManager::setCurrentRecipe(Core::Recipe* recipe)
{
    if (currentRecipe_ != recipe) {
        currentRecipe_ = recipe;

        // 切换配方时自动加载标定
        if (recipe) {
            loadFromRecipe(recipe);
        } else {
            clearAll();
        }
    }
}

QString CalibrationManager::getCalibrationPath(Core::Recipe* recipe) const
{
    if (!recipe) {
        return QString();
    }

    QString recipeDir = recipe->recipeDirectory();
    if (recipeDir.isEmpty()) {
        return QString();
    }

    return QDir(recipeDir).filePath("calibration");
}

bool CalibrationManager::ensureCalibrationDirectory(Core::Recipe* recipe) const
{
    QString calibPath = getCalibrationPath(recipe);
    if (calibPath.isEmpty()) {
        return false;
    }

    QDir dir(calibPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            LOG_ERROR(QString("无法创建标定目录: %1").arg(calibPath));
            return false;
        }
        LOG_INFO(QString("创建标定目录: %1").arg(calibPath));
    }

    return true;
}

// ========== 标定加载/保存 ==========

bool CalibrationManager::loadFromRecipe(Core::Recipe* recipe)
{
    if (!recipe) {
        return false;
    }

    QString calibPath = getCalibrationPath(recipe);
    if (calibPath.isEmpty()) {
        return false;
    }

    bool loadedAny = false;

    // 加载相机标定
    QString cameraFile = QDir(calibPath).filePath("camera.json");
    if (QFile::exists(cameraFile)) {
        if (loadCameraCalib(cameraFile)) {
            loadedAny = true;
        }
    }

    // 加载九点标定
    QString ninePointFile = QDir(calibPath).filePath("ninepoint.json");
    if (QFile::exists(ninePointFile)) {
        if (loadNinePointCalib(ninePointFile)) {
            loadedAny = true;
        }
    }

    if (loadedAny) {
        LOG_INFO(QString("从配方加载标定: %1").arg(recipe->name()));
        emit calibrationLoaded();
    }

    return loadedAny;
}

bool CalibrationManager::saveToRecipe(Core::Recipe* recipe)
{
    if (!recipe) {
        return false;
    }

    if (!ensureCalibrationDirectory(recipe)) {
        return false;
    }

    QString calibPath = getCalibrationPath(recipe);
    bool savedAny = false;

    // 保存相机标定
    if (hasCameraCalib()) {
        QString cameraFile = QDir(calibPath).filePath("camera.json");
        if (saveCameraCalib(cameraFile)) {
            savedAny = true;
        }
    }

    // 保存九点标定
    if (hasNinePointCalib()) {
        QString ninePointFile = QDir(calibPath).filePath("ninepoint.json");
        if (saveNinePointCalib(ninePointFile)) {
            savedAny = true;
        }
    }

    if (savedAny) {
        LOG_INFO(QString("保存标定到配方: %1").arg(recipe->name()));
        emit calibrationSaved();
    }

    return savedAny;
}

bool CalibrationManager::loadCameraCalib(const QString& filePath)
{
    if (!cameraCalibResult_->loadFromFile(filePath)) {
        return false;
    }

    LOG_INFO(QString("加载相机标定: %1").arg(filePath));
    emit cameraCalibChanged();
    emit calibrationChanged();
    return true;
}

bool CalibrationManager::saveCameraCalib(const QString& filePath)
{
    if (!hasCameraCalib()) {
        return false;
    }

    if (!cameraCalibResult_->saveToFile(filePath)) {
        return false;
    }

    LOG_INFO(QString("保存相机标定: %1").arg(filePath));
    return true;
}

bool CalibrationManager::loadNinePointCalib(const QString& filePath)
{
    if (!ninePointCalibResult_->loadFromFile(filePath)) {
        return false;
    }

    LOG_INFO(QString("加载九点标定: %1").arg(filePath));
    emit ninePointCalibChanged();
    emit calibrationChanged();
    return true;
}

bool CalibrationManager::saveNinePointCalib(const QString& filePath)
{
    if (!hasNinePointCalib()) {
        return false;
    }

    if (!ninePointCalibResult_->saveToFile(filePath)) {
        return false;
    }

    LOG_INFO(QString("保存九点标定: %1").arg(filePath));
    return true;
}

// ========== 标定结果访问 ==========

void CalibrationManager::setCameraCalibResult(const CalibrationResult& result)
{
    *cameraCalibResult_ = result;
    emit cameraCalibChanged();
    emit calibrationChanged();
}

void CalibrationManager::setNinePointCalibResult(const CalibrationResult& result)
{
    *ninePointCalibResult_ = result;
    emit ninePointCalibChanged();
    emit calibrationChanged();
}

void CalibrationManager::clearCameraCalib()
{
    cameraCalibResult_->reset();
    emit cameraCalibChanged();
    emit calibrationChanged();
}

void CalibrationManager::clearNinePointCalib()
{
    ninePointCalibResult_->reset();
    emit ninePointCalibChanged();
    emit calibrationChanged();
}

void CalibrationManager::clearAll()
{
    cameraCalibResult_->reset();
    ninePointCalibResult_->reset();
    emit cameraCalibChanged();
    emit ninePointCalibChanged();
    emit calibrationChanged();
}

// ========== 标定状态 ==========

bool CalibrationManager::hasCameraCalib() const
{
    return cameraCalibResult_ && cameraCalibResult_->isValid() &&
           cameraCalibResult_->type() == CalibrationType::Camera;
}

bool CalibrationManager::hasNinePointCalib() const
{
    return ninePointCalibResult_ && ninePointCalibResult_->isValid() &&
           ninePointCalibResult_->type() == CalibrationType::NinePoint;
}

bool CalibrationManager::isCalibrated() const
{
    return hasCameraCalib() || hasNinePointCalib();
}

// ========== 坐标转换 ==========

cv::Point2d CalibrationManager::imageToWorld(const cv::Point2d& imagePoint) const
{
    if (hasNinePointCalib()) {
        return ninePointCalibResult_->imageToWorld(imagePoint);
    }
    return imagePoint;
}

cv::Point2d CalibrationManager::worldToImage(const cv::Point2d& worldPoint) const
{
    if (hasNinePointCalib()) {
        return ninePointCalibResult_->worldToImage(worldPoint);
    }
    return worldPoint;
}

std::vector<cv::Point2d> CalibrationManager::imageToWorld(const std::vector<cv::Point2d>& imagePoints) const
{
    if (hasNinePointCalib()) {
        return ninePointCalibResult_->imageToWorld(imagePoints);
    }
    return imagePoints;
}

std::vector<cv::Point2d> CalibrationManager::worldToImage(const std::vector<cv::Point2d>& worldPoints) const
{
    if (hasNinePointCalib()) {
        return ninePointCalibResult_->worldToImage(worldPoints);
    }
    return worldPoints;
}

double CalibrationManager::pixelToMM(double pixelDistance) const
{
    if (hasNinePointCalib()) {
        return pixelDistance * ninePointCalibResult_->pixelToMM();
    }
    return pixelDistance;
}

double CalibrationManager::mmToPixel(double mmDistance) const
{
    if (hasNinePointCalib()) {
        double scale = ninePointCalibResult_->pixelToMM();
        return scale > 0 ? mmDistance / scale : mmDistance;
    }
    return mmDistance;
}

double CalibrationManager::pixelScale() const
{
    if (hasNinePointCalib()) {
        return ninePointCalibResult_->pixelToMM();
    }
    return 1.0;
}

// ========== 畸变校正 ==========

cv::Mat CalibrationManager::undistortImage(const cv::Mat& src) const
{
    if (hasCameraCalib()) {
        return cameraCalibResult_->undistort(src);
    }
    return src.clone();
}

std::vector<cv::Point2f> CalibrationManager::undistortPoints(const std::vector<cv::Point2f>& points) const
{
    if (!hasCameraCalib() || points.empty()) {
        return points;
    }

    std::vector<cv::Point2f> undistorted;
    cv::undistortPoints(points, undistorted,
                        cameraCalibResult_->cameraMatrix(),
                        cameraCalibResult_->distCoeffs(),
                        cv::noArray(),
                        cameraCalibResult_->cameraMatrix());

    return undistorted;
}

// ========== 标定信息 ==========

QString CalibrationManager::getSummary() const
{
    QString summary;
    QTextStream stream(&summary);

    stream << "========== 标定信息 ==========\n\n";

    if (hasCameraCalib()) {
        stream << cameraCalibResult_->summary() << "\n";
    } else {
        stream << "【相机标定】未标定\n\n";
    }

    if (hasNinePointCalib()) {
        stream << ninePointCalibResult_->summary() << "\n";
    } else {
        stream << "【九点标定】未标定\n\n";
    }

    if (currentRecipe_) {
        stream << "关联配方: " << currentRecipe_->name() << "\n";
    }

    return summary;
}

} // namespace Algorithm
} // namespace VisionForge
