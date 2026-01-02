/**
 * @file CornerSearchTool.cpp
 * @brief 角点搜索工具实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "algorithm/CornerSearchTool.h"
#include "base/Logger.h"

#include <QElapsedTimer>
#include <QJsonArray>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

namespace VisionForge {
namespace Algorithm {

CornerSearchTool::CornerSearchTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(tr("角点搜索"));
}

bool CornerSearchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = tr("输入图像为空");
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    // 转灰度
    cv::Mat mat = input->mat();
    cv::Mat grayMat;
    if (mat.channels() == 3) {
        cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
    } else {
        grayMat = mat;
    }

    bool success = false;
    corners_.clear();

    try {
        switch (method_) {
            case Harris:
                success = processHarris(grayMat);
                break;
            case ShiTomasi:
                success = processShiTomasi(grayMat);
                break;
            case FAST:
                success = processFAST(grayMat);
                break;
            case ORB:
                success = processORB(grayMat);
                break;
        }
    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("OpenCV错误: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }

    output.executionTime = timer.elapsed();

    if (success) {
        output.success = true;
        output.setValue("cornerCount", static_cast<int>(corners_.size()));

        // 转换结果为JSON数组
        QJsonArray cornerArray;
        for (const auto& corner : corners_) {
            QJsonObject obj;
            obj["x"] = corner.x;
            obj["y"] = corner.y;
            obj["response"] = corner.response;
            obj["angle"] = corner.angle;
            obj["size"] = corner.size;
            cornerArray.append(obj);
        }
        output.data["corners"] = cornerArray;

        setStatusText(tr("找到 %1 个角点，耗时 %2 ms")
            .arg(corners_.size()).arg(output.executionTime));
    }

    return success;
}

bool CornerSearchTool::processHarris(const cv::Mat& gray)
{
    cv::Mat harrisResponse;
    cv::cornerHarris(gray, harrisResponse, harrisBlockSize_, harrisKSize_, harrisK_);

    // 归一化
    cv::Mat normalized;
    cv::normalize(harrisResponse, normalized, 0, 1, cv::NORM_MINMAX);

    std::vector<cv::Point2f> points;

    // 阈值化提取角点
    for (int y = 1; y < normalized.rows - 1; ++y) {
        for (int x = 1; x < normalized.cols - 1; ++x) {
            float val = normalized.at<float>(y, x);
            if (val > harrisThreshold_) {
                // 局部极大值检测
                bool isMax = true;
                for (int dy = -1; dy <= 1 && isMax; ++dy) {
                    for (int dx = -1; dx <= 1 && isMax; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        if (normalized.at<float>(y + dy, x + dx) >= val) {
                            isMax = false;
                        }
                    }
                }
                if (isMax) {
                    points.push_back(cv::Point2f(static_cast<float>(x), static_cast<float>(y)));
                }
            }
        }
    }

    // 亚像素精化
    if (useSubPixel_ && !points.empty()) {
        cv::Size winSize(subPixelWinSize_, subPixelWinSize_);
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.01);
        cv::cornerSubPix(gray, points, winSize, cv::Size(-1, -1), criteria);
    }

    // 限制数量
    if (static_cast<int>(points.size()) > maxCorners_) {
        points.resize(maxCorners_);
    }

    // 转换结果
    for (const auto& pt : points) {
        CornerPoint cp;
        cp.x = pt.x;
        cp.y = pt.y;
        cp.response = normalized.at<float>(static_cast<int>(pt.y), static_cast<int>(pt.x));
        corners_.push_back(cp);
    }

    return true;
}

bool CornerSearchTool::processShiTomasi(const cv::Mat& gray)
{
    std::vector<cv::Point2f> points;

    cv::goodFeaturesToTrack(gray, points, maxCorners_, qualityLevel_, minDistance_,
                            cv::noArray(), blockSize_);

    // 亚像素精化
    if (useSubPixel_ && !points.empty()) {
        cv::Size winSize(subPixelWinSize_, subPixelWinSize_);
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.01);
        cv::cornerSubPix(gray, points, winSize, cv::Size(-1, -1), criteria);
    }

    // 转换结果
    for (const auto& pt : points) {
        CornerPoint cp;
        cp.x = pt.x;
        cp.y = pt.y;
        corners_.push_back(cp);
    }

    return true;
}

bool CornerSearchTool::processFAST(const cv::Mat& gray)
{
    std::vector<cv::KeyPoint> keypoints;
    cv::FAST(gray, keypoints, fastThreshold_, fastNMS_,
             static_cast<cv::FastFeatureDetector::DetectorType>(fastType_));

    // 限制数量
    if (static_cast<int>(keypoints.size()) > maxCorners_) {
        // 按响应值排序
        std::sort(keypoints.begin(), keypoints.end(),
            [](const cv::KeyPoint& a, const cv::KeyPoint& b) {
                return a.response > b.response;
            });
        keypoints.resize(maxCorners_);
    }

    // 转换结果
    for (const auto& kp : keypoints) {
        CornerPoint cp;
        cp.x = kp.pt.x;
        cp.y = kp.pt.y;
        cp.response = kp.response;
        cp.angle = kp.angle;
        cp.size = kp.size;
        corners_.push_back(cp);
    }

    return true;
}

bool CornerSearchTool::processORB(const cv::Mat& gray)
{
    auto orb = cv::ORB::create(maxCorners_);
    std::vector<cv::KeyPoint> keypoints;
    orb->detect(gray, keypoints);

    // 转换结果
    for (const auto& kp : keypoints) {
        CornerPoint cp;
        cp.x = kp.pt.x;
        cp.y = kp.pt.y;
        cp.response = kp.response;
        cp.angle = kp.angle;
        cp.size = kp.size;
        corners_.push_back(cp);
    }

    return true;
}

QJsonObject CornerSearchTool::serializeParams() const
{
    QJsonObject json;
    json["method"] = static_cast<int>(method_);

    // Harris参数
    json["harrisBlockSize"] = harrisBlockSize_;
    json["harrisKSize"] = harrisKSize_;
    json["harrisK"] = harrisK_;
    json["harrisThreshold"] = harrisThreshold_;

    // Shi-Tomasi参数
    json["maxCorners"] = maxCorners_;
    json["qualityLevel"] = qualityLevel_;
    json["minDistance"] = minDistance_;
    json["blockSize"] = blockSize_;

    // FAST参数
    json["fastThreshold"] = fastThreshold_;
    json["fastNMS"] = fastNMS_;
    json["fastType"] = fastType_;

    // 通用参数
    json["useSubPixel"] = useSubPixel_;
    json["subPixelWinSize"] = subPixelWinSize_;

    return json;
}

void CornerSearchTool::deserializeParams(const QJsonObject& json)
{
    method_ = static_cast<Method>(json.value("method").toInt(ShiTomasi));

    // Harris参数
    harrisBlockSize_ = json.value("harrisBlockSize").toInt(2);
    harrisKSize_ = json.value("harrisKSize").toInt(3);
    harrisK_ = json.value("harrisK").toDouble(0.04);
    harrisThreshold_ = json.value("harrisThreshold").toDouble(0.01);

    // Shi-Tomasi参数
    maxCorners_ = json.value("maxCorners").toInt(100);
    qualityLevel_ = json.value("qualityLevel").toDouble(0.01);
    minDistance_ = json.value("minDistance").toDouble(10.0);
    blockSize_ = json.value("blockSize").toInt(3);

    // FAST参数
    fastThreshold_ = json.value("fastThreshold").toInt(10);
    fastNMS_ = json.value("fastNMS").toBool(true);
    fastType_ = json.value("fastType").toInt(cv::FastFeatureDetector::TYPE_9_16);

    // 通用参数
    useSubPixel_ = json.value("useSubPixel").toBool(true);
    subPixelWinSize_ = json.value("subPixelWinSize").toInt(5);
}

} // namespace Algorithm
} // namespace VisionForge
