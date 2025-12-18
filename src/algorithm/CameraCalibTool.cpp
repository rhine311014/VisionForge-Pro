/**
 * @file CameraCalibTool.cpp
 * @brief 相机标定工具实现
 */

#include "algorithm/CameraCalibTool.h"
#include "base/Logger.h"
#include <QDateTime>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#ifdef USE_HALCON
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

CameraCalibTool::CameraCalibTool(QObject* parent)
    : VisionTool(parent)
    , patternType_(CalibPatternType::Chessboard)
    , boardWidth_(9)
    , boardHeight_(6)
    , squareSize_(25.0)
    , backend_(CalibBackend::OpenCV)
    , useSubPixel_(true)
    , useAdaptiveThresh_(true)
    , useFastCheck_(true)
    , minImageCount_(10)
{
}

CameraCalibTool::~CameraCalibTool()
{
    clearImages();
}

// ========== VisionTool 接口实现 ==========

bool CameraCalibTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->mat().empty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    // 检测角点
    std::vector<cv::Point2f> corners;
    bool found = detectCorners(input->mat(), corners);

    output.success = found;
    output.setValue("cornerCount", static_cast<int>(corners.size()));
    output.setValue("patternFound", found);

    if (found) {
        // 保存角点坐标
        QJsonArray cornersArray;
        for (const auto& pt : corners) {
            QJsonObject ptObj;
            ptObj["x"] = pt.x;
            ptObj["y"] = pt.y;
            cornersArray.append(ptObj);
        }
        output.data["corners"] = cornersArray;
    }

    emit cornersDetected(found, static_cast<int>(corners.size()));

    return found;
}

QJsonObject CameraCalibTool::serializeParams() const
{
    QJsonObject json;

    // 标定板参数
    json["patternType"] = static_cast<int>(patternType_);
    json["boardWidth"] = boardWidth_;
    json["boardHeight"] = boardHeight_;
    json["squareSize"] = squareSize_;

    // 后端和检测参数
    json["backend"] = static_cast<int>(backend_);
    json["useSubPixel"] = useSubPixel_;
    json["useAdaptiveThresh"] = useAdaptiveThresh_;
    json["useFastCheck"] = useFastCheck_;
    json["minImageCount"] = minImageCount_;

    // 标定结果
    if (result_.isValid()) {
        json["calibResult"] = result_.serialize();
    }

    return json;
}

void CameraCalibTool::deserializeParams(const QJsonObject& json)
{
    // 标定板参数
    patternType_ = static_cast<CalibPatternType>(json["patternType"].toInt());
    boardWidth_ = json["boardWidth"].toInt(9);
    boardHeight_ = json["boardHeight"].toInt(6);
    squareSize_ = json["squareSize"].toDouble(25.0);

    // 后端和检测参数
    backend_ = static_cast<CalibBackend>(json["backend"].toInt());
    useSubPixel_ = json["useSubPixel"].toBool(true);
    useAdaptiveThresh_ = json["useAdaptiveThresh"].toBool(true);
    useFastCheck_ = json["useFastCheck"].toBool(true);
    minImageCount_ = json["minImageCount"].toInt(10);

    // 标定结果
    if (json.contains("calibResult")) {
        result_.deserialize(json["calibResult"].toObject());
    }
}

// ========== 标定板参数 ==========

void CameraCalibTool::setBoardSize(int width, int height)
{
    boardWidth_ = width;
    boardHeight_ = height;
}

void CameraCalibTool::setBackend(CalibBackend backend)
{
#ifndef USE_HALCON
    if (backend == CalibBackend::Halcon) {
        LOG_WARNING("Halcon未启用，使用OpenCV后端");
        backend_ = CalibBackend::OpenCV;
        return;
    }
#endif
    backend_ = backend;
}

// ========== 角点检测 ==========

bool CameraCalibTool::detectCorners(const cv::Mat& image, std::vector<cv::Point2f>& corners)
{
    if (image.empty()) {
        return false;
    }

#ifdef USE_HALCON
    if (backend_ == CalibBackend::Halcon) {
        return detectCornersHalcon(image, corners);
    }
#endif

    return detectCornersOpenCV(image, corners);
}

bool CameraCalibTool::detectAndDraw(const cv::Mat& image, cv::Mat& output)
{
    if (image.empty()) {
        return false;
    }

    std::vector<cv::Point2f> corners;
    bool found = detectCorners(image, corners);

    // 复制图像用于绘制
    if (image.channels() == 1) {
        cv::cvtColor(image, output, cv::COLOR_GRAY2BGR);
    } else {
        output = image.clone();
    }

    // 绘制角点
    cv::drawChessboardCorners(output, boardSize(), corners, found);

    return found;
}

bool CameraCalibTool::detectCornersOpenCV(const cv::Mat& image, std::vector<cv::Point2f>& corners)
{
    // 转换为灰度图
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image;
    }

    bool found = false;
    cv::Size patternSize(boardWidth_, boardHeight_);

    // 设置检测标志
    int flags = 0;
    if (useAdaptiveThresh_) {
        flags |= cv::CALIB_CB_ADAPTIVE_THRESH;
    }
    if (useFastCheck_) {
        flags |= cv::CALIB_CB_FAST_CHECK;
    }
    flags |= cv::CALIB_CB_NORMALIZE_IMAGE;

    switch (patternType_) {
    case CalibPatternType::Chessboard:
        found = cv::findChessboardCorners(gray, patternSize, corners, flags);
        break;

    case CalibPatternType::CircleGrid:
        found = cv::findCirclesGrid(gray, patternSize, corners);
        break;

    case CalibPatternType::AsymCircleGrid:
        found = cv::findCirclesGrid(gray, patternSize, corners, cv::CALIB_CB_ASYMMETRIC_GRID);
        break;
    }

    // 亚像素优化
    if (found && useSubPixel_ && patternType_ == CalibPatternType::Chessboard) {
        refineCorners(gray, corners);
    }

    return found;
}

#ifdef USE_HALCON
bool CameraCalibTool::detectCornersHalcon(const cv::Mat& image, std::vector<cv::Point2f>& corners)
{
    try {
        // 将OpenCV Mat转换为Halcon HImage
        cv::Mat gray;
        if (image.channels() == 3) {
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = image.clone();
        }

        HImage hImage;
        hImage.GenImage1("byte", gray.cols, gray.rows, gray.data);

        if (patternType_ == CalibPatternType::Chessboard) {
            // 使用Halcon的棋盘格角点检测
            // 注意: Halcon的标定板检测方式与OpenCV不同
            // 这里提供一个简化的实现，实际使用时可能需要根据具体标定板调整

            HRegion caltabRegion;
            HTuple sizeGauss = 3, markThresh = 128, minDiamMarks = 5;

            // 尝试使用find_caltab查找标定板区域
            // 如果失败，回退到OpenCV
            try {
                FindCaltab(hImage, &caltabRegion, "caltab.descr",
                          sizeGauss, markThresh, minDiamMarks);

                // 使用saddle_points_sub_pix检测角点
                HTuple row, col;
                SaddlePointsSubPix(hImage, "facet", 1, 3, &row, &col);

                // 转换为OpenCV格式
                corners.clear();
                int expectedCount = boardWidth_ * boardHeight_;
                for (Hlong i = 0; i < row.Length() && static_cast<int>(corners.size()) < expectedCount; ++i) {
                    corners.emplace_back(static_cast<float>(col[i].D()),
                                        static_cast<float>(row[i].D()));
                }

                if (static_cast<int>(corners.size()) == expectedCount) {
                    return true;
                }
            }
            catch (const HException&) {
                // Halcon检测失败，尝试备用方法
            }
        }

        // 回退到OpenCV
        LOG_DEBUG("Halcon角点检测失败，使用OpenCV后端");
        return detectCornersOpenCV(image, corners);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("Halcon角点检测异常: %1").arg(e.ErrorMessage().Text()));
        // 回退到OpenCV
        return detectCornersOpenCV(image, corners);
    }
}
#endif

void CameraCalibTool::refineCorners(const cv::Mat& grayImage, std::vector<cv::Point2f>& corners)
{
    cv::Size winSize(11, 11);
    cv::Size zeroZone(-1, -1);
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);

    cv::cornerSubPix(grayImage, corners, winSize, zeroZone, criteria);
}

// ========== 图像采集 ==========

bool CameraCalibTool::addCalibImage(const cv::Mat& image)
{
    if (image.empty()) {
        return false;
    }

    std::vector<cv::Point2f> corners;
    if (!detectCorners(image, corners)) {
        LOG_WARNING("添加标定图像失败：未检测到角点");
        return false;
    }

    addCalibImageWithCorners(image, corners);
    return true;
}

void CameraCalibTool::addCalibImageWithCorners(const cv::Mat& image, const std::vector<cv::Point2f>& corners)
{
    calibImages_.push_back(image.clone());
    imagePoints_.push_back(corners);

    LOG_INFO(QString("添加标定图像 #%1，检测到 %2 个角点")
             .arg(calibImages_.size())
             .arg(corners.size()));

    emit imageAdded(static_cast<int>(calibImages_.size()));
}

bool CameraCalibTool::removeCalibImage(int index)
{
    if (index < 0 || index >= static_cast<int>(calibImages_.size())) {
        return false;
    }

    calibImages_.erase(calibImages_.begin() + index);
    imagePoints_.erase(imagePoints_.begin() + index);

    LOG_INFO(QString("移除标定图像 #%1").arg(index + 1));
    return true;
}

void CameraCalibTool::clearImages()
{
    calibImages_.clear();
    imagePoints_.clear();
    LOG_INFO("清除所有标定图像");
}

cv::Mat CameraCalibTool::getImage(int index) const
{
    if (index >= 0 && index < static_cast<int>(calibImages_.size())) {
        return calibImages_[index].clone();
    }
    return cv::Mat();
}

std::vector<cv::Point2f> CameraCalibTool::getImageCorners(int index) const
{
    if (index >= 0 && index < static_cast<int>(imagePoints_.size())) {
        return imagePoints_[index];
    }
    return std::vector<cv::Point2f>();
}

// ========== 标定执行 ==========

bool CameraCalibTool::calibrate(CalibrationResult& result)
{
    if (static_cast<int>(calibImages_.size()) < minImageCount_) {
        QString msg = QString("标定图像数量不足，需要至少 %1 张，当前 %2 张")
                      .arg(minImageCount_).arg(calibImages_.size());
        LOG_ERROR(msg);
        setStatusText(msg);
        emit calibrationFinished(false, 0.0);
        return false;
    }

    result.reset();

#ifdef USE_HALCON
    if (backend_ == CalibBackend::Halcon) {
        bool success = calibrateHalcon(result);
        emit calibrationFinished(success, result.rmsError());
        if (success) {
            result_ = result;
        }
        return success;
    }
#endif

    bool success = calibrateOpenCV(result);
    emit calibrationFinished(success, result.rmsError());
    if (success) {
        result_ = result;
    }
    return success;
}

bool CameraCalibTool::calibrateOpenCV(CalibrationResult& result)
{
    if (calibImages_.empty() || imagePoints_.empty()) {
        return false;
    }

    // 生成世界坐标点
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<cv::Point3f> objPts = generateObjectPoints();
    for (size_t i = 0; i < imagePoints_.size(); ++i) {
        objectPoints.push_back(objPts);
    }

    // 获取图像尺寸
    cv::Size imageSize = calibImages_[0].size();

    // 初始化标定参数
    cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
    std::vector<cv::Mat> rvecs, tvecs;

    // 执行标定
    int flags = 0;
    // flags |= cv::CALIB_FIX_PRINCIPAL_POINT;
    // flags |= cv::CALIB_FIX_ASPECT_RATIO;

    double rms = cv::calibrateCamera(
        objectPoints, imagePoints_, imageSize,
        cameraMatrix, distCoeffs, rvecs, tvecs, flags
    );

    // 保存结果
    result.setType(CalibrationType::Camera);
    result.setCameraMatrix(cameraMatrix);
    result.setDistCoeffs(distCoeffs);
    result.setRmsError(rms);
    result.setImageSize(imageSize);
    result.setCalibrationTime(QDateTime::currentMSecsSinceEpoch());

    LOG_INFO(QString("相机标定完成 (OpenCV)"));
    LOG_INFO(QString("  图像尺寸: %1 x %2").arg(imageSize.width).arg(imageSize.height));
    LOG_INFO(QString("  焦距: fx=%.2f, fy=%.2f")
             .arg(cameraMatrix.at<double>(0, 0))
             .arg(cameraMatrix.at<double>(1, 1)));
    LOG_INFO(QString("  光心: cx=%.2f, cy=%.2f")
             .arg(cameraMatrix.at<double>(0, 2))
             .arg(cameraMatrix.at<double>(1, 2)));
    LOG_INFO(QString("  重投影误差: %.4f 像素").arg(rms));

    return true;
}

#ifdef USE_HALCON
bool CameraCalibTool::calibrateHalcon(CalibrationResult& result)
{
    // Halcon标定实现比较复杂，需要根据具体的标定板配置文件
    // 这里提供一个框架，实际使用时回退到OpenCV实现
    //
    // 完整的Halcon标定流程需要:
    // 1. 标定板描述文件 (.descr)
    // 2. 初始相机参数设置
    // 3. 多图像观测数据采集
    // 4. calibrate_cameras 执行标定

    LOG_WARNING("Halcon相机标定功能尚未完整实现，使用OpenCV后端");

    // 回退到OpenCV实现
    return calibrateOpenCV(result);
}
#endif

std::vector<cv::Point3f> CameraCalibTool::generateObjectPoints() const
{
    std::vector<cv::Point3f> corners;

    switch (patternType_) {
    case CalibPatternType::Chessboard:
    case CalibPatternType::CircleGrid:
        for (int i = 0; i < boardHeight_; ++i) {
            for (int j = 0; j < boardWidth_; ++j) {
                corners.emplace_back(
                    static_cast<float>(j * squareSize_),
                    static_cast<float>(i * squareSize_),
                    0.0f
                );
            }
        }
        break;

    case CalibPatternType::AsymCircleGrid:
        for (int i = 0; i < boardHeight_; ++i) {
            for (int j = 0; j < boardWidth_; ++j) {
                corners.emplace_back(
                    static_cast<float>((2 * j + i % 2) * squareSize_),
                    static_cast<float>(i * squareSize_),
                    0.0f
                );
            }
        }
        break;
    }

    return corners;
}

// ========== 畸变校正 ==========

cv::Mat CameraCalibTool::undistort(const cv::Mat& src) const
{
    if (!result_.isValid() || src.empty()) {
        return src.clone();
    }

    return result_.undistort(src);
}

std::vector<cv::Point2f> CameraCalibTool::undistortPoints(const std::vector<cv::Point2f>& distortedPoints) const
{
    if (!result_.isValid() || distortedPoints.empty()) {
        return distortedPoints;
    }

    std::vector<cv::Point2f> undistortedPoints;
    cv::undistortPoints(distortedPoints, undistortedPoints,
                        result_.cameraMatrix(), result_.distCoeffs(),
                        cv::noArray(), result_.cameraMatrix());

    return undistortedPoints;
}

} // namespace Algorithm
} // namespace VisionForge
