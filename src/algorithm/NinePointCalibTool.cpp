/**
 * @file NinePointCalibTool.cpp
 * @brief 九点标定工具实现
 */

#include "algorithm/NinePointCalibTool.h"
#include "base/Logger.h"
#include <QDateTime>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

#ifdef USE_HALCON
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

NinePointCalibTool::NinePointCalibTool(QObject* parent)
    : VisionTool(parent)
    , calibMode_(NinePointCalibMode::Homography)
    , backend_(CalibBackend::OpenCV)
{
    // 默认9个标定点
    setPointCount(9);
}

NinePointCalibTool::~NinePointCalibTool()
{
}

// ========== VisionTool 接口实现 ==========

bool NinePointCalibTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    Q_UNUSED(input);

    // 九点标定不需要处理图像，仅需要标定点数据
    output.success = isCalibrated();
    output.setValue("isCalibrated", isCalibrated());
    output.setValue("validPointCount", validPointCount());

    if (isCalibrated()) {
        output.setValue("pixelToMMX", result_.pixelToMMX());
        output.setValue("pixelToMMY", result_.pixelToMMY());
        output.setValue("rotationAngle", rotationAngleDegrees());
        output.setValue("calibrationError", result_.calibrationError());
    }

    return output.success;
}

QJsonObject NinePointCalibTool::serializeParams() const
{
    QJsonObject json;

    // 标定模式
    json["calibMode"] = static_cast<int>(calibMode_);
    json["backend"] = static_cast<int>(backend_);

    // 标定点
    QJsonArray pointsArray;
    for (const auto& pt : calibPoints_) {
        QJsonObject ptObj;
        ptObj["imageX"] = pt.imagePos.x;
        ptObj["imageY"] = pt.imagePos.y;
        ptObj["worldX"] = pt.worldPos.x;
        ptObj["worldY"] = pt.worldPos.y;
        ptObj["valid"] = pt.valid;
        ptObj["name"] = pt.name;
        pointsArray.append(ptObj);
    }
    json["calibPoints"] = pointsArray;

    // 标定结果
    if (result_.isValid()) {
        json["calibResult"] = result_.serialize();
    }

    return json;
}

void NinePointCalibTool::deserializeParams(const QJsonObject& json)
{
    // 标定模式
    calibMode_ = static_cast<NinePointCalibMode>(json["calibMode"].toInt());
    backend_ = static_cast<CalibBackend>(json["backend"].toInt());

    // 标定点
    calibPoints_.clear();
    QJsonArray pointsArray = json["calibPoints"].toArray();
    for (const auto& val : pointsArray) {
        QJsonObject ptObj = val.toObject();
        CalibPoint pt;
        pt.imagePos.x = ptObj["imageX"].toDouble();
        pt.imagePos.y = ptObj["imageY"].toDouble();
        pt.worldPos.x = ptObj["worldX"].toDouble();
        pt.worldPos.y = ptObj["worldY"].toDouble();
        pt.valid = ptObj["valid"].toBool();
        pt.name = ptObj["name"].toString();
        calibPoints_.push_back(pt);
    }

    // 标定结果
    if (json.contains("calibResult")) {
        result_.deserialize(json["calibResult"].toObject());
    }
}

// ========== 标定模式 ==========

void NinePointCalibTool::setBackend(CalibBackend backend)
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

// ========== 标定点管理 ==========

void NinePointCalibTool::setPointCount(int count)
{
    count = qMax(4, count);  // 至少4个点

    if (count > static_cast<int>(calibPoints_.size())) {
        // 添加新点
        while (static_cast<int>(calibPoints_.size()) < count) {
            CalibPoint pt;
            pt.name = QString("P%1").arg(calibPoints_.size() + 1);
            calibPoints_.push_back(pt);
        }
    } else if (count < static_cast<int>(calibPoints_.size())) {
        // 移除多余点
        calibPoints_.resize(count);
    }

    emit calibPointsChanged();
}

int NinePointCalibTool::validPointCount() const
{
    int count = 0;
    for (const auto& pt : calibPoints_) {
        if (pt.valid) {
            count++;
        }
    }
    return count;
}

bool NinePointCalibTool::setCalibPoint(int index, const cv::Point2d& imagePos, const cv::Point2d& worldPos)
{
    if (index < 0 || index >= static_cast<int>(calibPoints_.size())) {
        return false;
    }

    calibPoints_[index].imagePos = imagePos;
    calibPoints_[index].worldPos = worldPos;
    calibPoints_[index].valid = true;

    emit calibPointsChanged();
    return true;
}

bool NinePointCalibTool::setImagePoint(int index, const cv::Point2d& imagePos)
{
    if (index < 0 || index >= static_cast<int>(calibPoints_.size())) {
        return false;
    }

    calibPoints_[index].imagePos = imagePos;
    emit calibPointsChanged();
    return true;
}

bool NinePointCalibTool::setWorldPoint(int index, const cv::Point2d& worldPos)
{
    if (index < 0 || index >= static_cast<int>(calibPoints_.size())) {
        return false;
    }

    calibPoints_[index].worldPos = worldPos;
    emit calibPointsChanged();
    return true;
}

CalibPoint NinePointCalibTool::getCalibPoint(int index) const
{
    if (index >= 0 && index < static_cast<int>(calibPoints_.size())) {
        return calibPoints_[index];
    }
    return CalibPoint();
}

bool NinePointCalibTool::removeCalibPoint(int index)
{
    if (index < 0 || index >= static_cast<int>(calibPoints_.size())) {
        return false;
    }

    calibPoints_.erase(calibPoints_.begin() + index);
    emit calibPointsChanged();
    return true;
}

void NinePointCalibTool::clearCalibPoints()
{
    for (auto& pt : calibPoints_) {
        pt.valid = false;
        pt.imagePos = cv::Point2d(0, 0);
        pt.worldPos = cv::Point2d(0, 0);
    }
    emit calibPointsChanged();
}

void NinePointCalibTool::setPointValid(int index, bool valid)
{
    if (index >= 0 && index < static_cast<int>(calibPoints_.size())) {
        calibPoints_[index].valid = valid;
        emit calibPointsChanged();
    }
}

void NinePointCalibTool::setPointName(int index, const QString& name)
{
    if (index >= 0 && index < static_cast<int>(calibPoints_.size())) {
        calibPoints_[index].name = name;
    }
}

// ========== 预设网格 ==========

void NinePointCalibTool::generateGridWorldPoints(int rows, int cols,
                                                  double spacingX, double spacingY,
                                                  double originX, double originY)
{
    int count = rows * cols;
    setPointCount(count);

    int idx = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (idx < static_cast<int>(calibPoints_.size())) {
                calibPoints_[idx].worldPos.x = originX + c * spacingX;
                calibPoints_[idx].worldPos.y = originY + r * spacingY;
                calibPoints_[idx].name = QString("P%1(%2,%3)")
                    .arg(idx + 1).arg(r + 1).arg(c + 1);
                idx++;
            }
        }
    }

    LOG_INFO(QString("生成 %1x%2 网格物理坐标，间距: %.2f x %.2f mm")
             .arg(rows).arg(cols).arg(spacingX).arg(spacingY));

    emit calibPointsChanged();
}

// ========== 标定执行 ==========

bool NinePointCalibTool::calibrate(CalibrationResult& result)
{
    int validCount = validPointCount();

    // 检查点数
    int minRequired = 4;
    if (calibMode_ == NinePointCalibMode::Affine) {
        minRequired = 3;
    }

    if (validCount < minRequired) {
        QString msg = QString("有效标定点不足，需要至少 %1 个，当前 %2 个")
                      .arg(minRequired).arg(validCount);
        LOG_ERROR(msg);
        setStatusText(msg);
        emit calibrationFinished(false, 0.0);
        return false;
    }

    result.reset();

#ifdef USE_HALCON
    if (backend_ == CalibBackend::Halcon) {
        bool success = calibrateHalcon(result);
        emit calibrationFinished(success, result.calibrationError());
        if (success) {
            result_ = result;
        }
        return success;
    }
#endif

    bool success = calibrateOpenCV(result);
    emit calibrationFinished(success, result.calibrationError());
    if (success) {
        result_ = result;
    }
    return success;
}

bool NinePointCalibTool::calibrateOpenCV(CalibrationResult& result)
{
    // 收集有效点
    std::vector<cv::Point2d> imagePts, worldPts;
    for (const auto& pt : calibPoints_) {
        if (pt.valid) {
            imagePts.push_back(pt.imagePos);
            worldPts.push_back(pt.worldPos);
        }
    }

    if (imagePts.size() < 3) {
        return false;
    }

    cv::Mat H;

    switch (calibMode_) {
    case NinePointCalibMode::Affine: {
        // 仿射变换：需要至少3个点
        if (imagePts.size() < 3) {
            LOG_ERROR("仿射变换需要至少3个点");
            return false;
        }

        // 使用前3个点计算仿射变换
        cv::Point2f srcPts[3], dstPts[3];
        for (int i = 0; i < 3; ++i) {
            srcPts[i] = cv::Point2f(static_cast<float>(imagePts[i].x),
                                     static_cast<float>(imagePts[i].y));
            dstPts[i] = cv::Point2f(static_cast<float>(worldPts[i].x),
                                     static_cast<float>(worldPts[i].y));
        }

        cv::Mat affine = cv::getAffineTransform(srcPts, dstPts);
        result.setAffineMatrix(affine);

        // 转换为3x3单应性矩阵
        H = cv::Mat::eye(3, 3, CV_64F);
        affine.copyTo(H(cv::Rect(0, 0, 3, 2)));
        break;
    }

    case NinePointCalibMode::Perspective: {
        // 透视变换：需要4个点
        if (imagePts.size() < 4) {
            LOG_ERROR("透视变换需要至少4个点");
            return false;
        }

        // 使用前4个点计算透视变换
        cv::Point2f srcPts[4], dstPts[4];
        for (int i = 0; i < 4; ++i) {
            srcPts[i] = cv::Point2f(static_cast<float>(imagePts[i].x),
                                     static_cast<float>(imagePts[i].y));
            dstPts[i] = cv::Point2f(static_cast<float>(worldPts[i].x),
                                     static_cast<float>(worldPts[i].y));
        }

        H = cv::getPerspectiveTransform(srcPts, dstPts);
        break;
    }

    case NinePointCalibMode::Homography:
    default: {
        // 单应性变换：需要至少4个点，使用最小二乘法
        std::vector<cv::Point2f> srcPts, dstPts;
        for (size_t i = 0; i < imagePts.size(); ++i) {
            srcPts.emplace_back(static_cast<float>(imagePts[i].x),
                               static_cast<float>(imagePts[i].y));
            dstPts.emplace_back(static_cast<float>(worldPts[i].x),
                               static_cast<float>(worldPts[i].y));
        }

        // 使用RANSAC或最小二乘
        H = cv::findHomography(srcPts, dstPts, cv::RANSAC, 3.0);
        break;
    }
    }

    if (H.empty()) {
        LOG_ERROR("计算变换矩阵失败");
        return false;
    }

    // 确保H是CV_64F类型
    H.convertTo(H, CV_64F);

    // 保存结果
    result.setType(CalibrationType::NinePoint);
    result.setHomography(H);
    result.setCalibPoints(imagePts, worldPts);
    result.setCalibrationTime(QDateTime::currentMSecsSinceEpoch());

    // 计算标定误差
    double error = calculateError(H);
    result.setCalibrationError(error);

    // 计算像素比例和旋转角度
    calculateScaleAndRotation(result);

    LOG_INFO(QString("九点标定完成 (OpenCV, %1 点)")
             .arg(imagePts.size()));
    LOG_INFO(QString("  像素比例: X=%.4f mm/px, Y=%.4f mm/px")
             .arg(result.pixelToMMX()).arg(result.pixelToMMY()));
    LOG_INFO(QString("  旋转角度: %.2f 度")
             .arg(result.rotationAngle() * 180.0 / CV_PI));
    LOG_INFO(QString("  标定精度: %.4f mm").arg(error));

    return true;
}

#ifdef USE_HALCON
bool NinePointCalibTool::calibrateHalcon(CalibrationResult& result)
{
    try {
        // 收集有效点
        HTuple rowsImg, colsImg, rowsWorld, colsWorld;

        for (const auto& pt : calibPoints_) {
            if (pt.valid) {
                rowsImg.Append(pt.imagePos.y);
                colsImg.Append(pt.imagePos.x);
                rowsWorld.Append(pt.worldPos.y);
                colsWorld.Append(pt.worldPos.x);
            }
        }

        if (rowsImg.Length() < 4) {
            LOG_ERROR("Halcon标定需要至少4个点");
            return false;
        }

        // 计算单应性矩阵
        HTuple homMat2D;
        VectorToHomMat2d(colsImg, rowsImg, colsWorld, rowsWorld, &homMat2D);

        // 转换为OpenCV格式的3x3矩阵
        cv::Mat H(3, 3, CV_64F);
        for (int i = 0; i < 9; ++i) {
            H.at<double>(i / 3, i % 3) = homMat2D[i].D();
        }

        // 收集点数据用于保存
        std::vector<cv::Point2d> imagePts, worldPts;
        for (const auto& pt : calibPoints_) {
            if (pt.valid) {
                imagePts.push_back(pt.imagePos);
                worldPts.push_back(pt.worldPos);
            }
        }

        // 保存结果
        result.setType(CalibrationType::NinePoint);
        result.setHomography(H);
        result.setCalibPoints(imagePts, worldPts);
        result.setCalibrationTime(QDateTime::currentMSecsSinceEpoch());

        // 计算标定误差
        double error = calculateError(H);
        result.setCalibrationError(error);

        // 计算像素比例和旋转角度
        calculateScaleAndRotation(result);

        LOG_INFO(QString("九点标定完成 (Halcon, %1 点), 精度: %.4f mm")
                 .arg(imagePts.size()).arg(error));

        return true;
    }
    catch (const HException& e) {
        LOG_ERROR(QString("Halcon标定失败: %1").arg(e.ErrorMessage().Text()));
        // 回退到OpenCV
        return calibrateOpenCV(result);
    }
}
#endif

double NinePointCalibTool::calculateError(const cv::Mat& H) const
{
    if (H.empty()) {
        return 0.0;
    }

    double totalError = 0.0;
    int validCount = 0;

    for (const auto& pt : calibPoints_) {
        if (pt.valid) {
            // 使用H将图像坐标转换为世界坐标
            std::vector<cv::Point2d> src = {pt.imagePos};
            std::vector<cv::Point2d> dst;
            cv::perspectiveTransform(src, dst, H);

            // 计算与实际世界坐标的误差
            double dx = dst[0].x - pt.worldPos.x;
            double dy = dst[0].y - pt.worldPos.y;
            double dist = std::sqrt(dx * dx + dy * dy);

            totalError += dist;
            validCount++;
        }
    }

    return validCount > 0 ? totalError / validCount : 0.0;
}

void NinePointCalibTool::calculateScaleAndRotation(CalibrationResult& result) const
{
    cv::Mat H = result.homography();
    if (H.empty()) {
        return;
    }

    // 从单应性矩阵提取缩放和旋转
    // H = [h11 h12 h13]
    //     [h21 h22 h23]
    //     [h31 h32 h33]

    double h11 = H.at<double>(0, 0);
    double h12 = H.at<double>(0, 1);
    double h21 = H.at<double>(1, 0);
    double h22 = H.at<double>(1, 1);

    // 像素比例（近似）
    double scaleX = std::sqrt(h11 * h11 + h21 * h21);
    double scaleY = std::sqrt(h12 * h12 + h22 * h22);

    result.setPixelToMMX(scaleX);
    result.setPixelToMMY(scaleY);

    // 旋转角度
    double angle = std::atan2(h21, h11);
    result.setRotationAngle(angle);
}

// ========== 坐标转换 ==========

cv::Point2d NinePointCalibTool::imageToWorld(const cv::Point2d& imagePoint) const
{
    if (!result_.isValid()) {
        return imagePoint;
    }
    return result_.imageToWorld(imagePoint);
}

cv::Point2d NinePointCalibTool::worldToImage(const cv::Point2d& worldPoint) const
{
    if (!result_.isValid()) {
        return worldPoint;
    }
    return result_.worldToImage(worldPoint);
}

std::vector<cv::Point2d> NinePointCalibTool::imageToWorld(const std::vector<cv::Point2d>& imagePoints) const
{
    if (!result_.isValid()) {
        return imagePoints;
    }
    return result_.imageToWorld(imagePoints);
}

std::vector<cv::Point2d> NinePointCalibTool::worldToImage(const std::vector<cv::Point2d>& worldPoints) const
{
    if (!result_.isValid()) {
        return worldPoints;
    }
    return result_.worldToImage(worldPoints);
}

double NinePointCalibTool::pixelToMM(double pixelDistance) const
{
    if (!result_.isValid()) {
        return pixelDistance;
    }
    return pixelDistance * result_.pixelToMM();
}

double NinePointCalibTool::mmToPixel(double mmDistance) const
{
    if (!result_.isValid()) {
        return mmDistance;
    }
    double scale = result_.pixelToMM();
    return scale > 0 ? mmDistance / scale : mmDistance;
}

double NinePointCalibTool::rotationAngleDegrees() const
{
    return result_.rotationAngle() * 180.0 / CV_PI;
}

} // namespace Algorithm
} // namespace VisionForge
