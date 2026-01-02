/**
 * @file QuickSearchTool.cpp
 * @brief 快速搜索工具实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "algorithm/QuickSearchTool.h"
#include "base/Logger.h"

#include <QElapsedTimer>
#include <QFileInfo>
#include <QJsonArray>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace Algorithm {

QuickSearchTool::QuickSearchTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(tr("快速搜索"));
}

bool QuickSearchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = tr("输入图像为空");
        return false;
    }

    if (!hasTemplate()) {
        output.success = false;
        output.errorMessage = tr("未设置模板");
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

    cv::Mat grayTempl;
    if (templateImage_.channels() == 3) {
        cv::cvtColor(templateImage_, grayTempl, cv::COLOR_BGR2GRAY);
    } else {
        grayTempl = templateImage_;
    }

    bool success = false;
    matches_.clear();

    try {
        if (useMultiScale_) {
            success = processMultiScale(grayMat, grayTempl);
        } else {
            success = processSingleScale(grayMat, grayTempl);
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
        output.setValue("matchCount", static_cast<int>(matches_.size()));

        // 转换结果为JSON数组
        QJsonArray matchArray;
        for (const auto& match : matches_) {
            QJsonObject obj;
            obj["x"] = match.x;
            obj["y"] = match.y;
            obj["score"] = match.score;
            obj["width"] = match.rect.width;
            obj["height"] = match.rect.height;
            matchArray.append(obj);
        }
        output.data["matches"] = matchArray;

        setStatusText(tr("找到 %1 个匹配，耗时 %2 ms")
            .arg(matches_.size()).arg(output.executionTime));
    }

    return success;
}

bool QuickSearchTool::processMultiScale(const cv::Mat& image, const cv::Mat& templ)
{
    // 构建金字塔
    std::vector<cv::Mat> imagePyramid, templPyramid;
    imagePyramid.push_back(image);
    templPyramid.push_back(templ);

    cv::Mat prevImg = image;
    cv::Mat prevTempl = templ;

    for (int i = 1; i < pyramidLevels_; ++i) {
        cv::Mat downImg, downTempl;
        cv::pyrDown(prevImg, downImg);
        cv::pyrDown(prevTempl, downTempl);

        // 模板太小则停止
        if (downTempl.cols < 10 || downTempl.rows < 10) {
            break;
        }

        imagePyramid.push_back(downImg);
        templPyramid.push_back(downTempl);
        prevImg = downImg;
        prevTempl = downTempl;
    }

    // 从最粗糙层开始搜索
    int topLevel = static_cast<int>(imagePyramid.size()) - 1;
    cv::Mat result;
    cv::matchTemplate(imagePyramid[topLevel], templPyramid[topLevel], result, matchMethod_);

    // 归一化
    if (matchMethod_ == cv::TM_SQDIFF || matchMethod_ == cv::TM_SQDIFF_NORMED) {
        result = 1.0 - result;
    }

    // 在顶层找到候选位置
    std::vector<cv::Point> candidates;
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // 找到所有超过阈值的位置
    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            if (result.at<float>(y, x) >= minScore_) {
                candidates.push_back(cv::Point(x, y));
            }
        }
    }

    // 在精细层进行精确匹配
    for (const auto& cand : candidates) {
        // 将坐标映射到原始分辨率
        int scale = 1 << topLevel;
        cv::Point refinedPos(cand.x * scale, cand.y * scale);

        // 在原始图像的局部区域进行精确匹配
        int margin = 10;
        int x1 = std::max(0, refinedPos.x - margin);
        int y1 = std::max(0, refinedPos.y - margin);
        int x2 = std::min(image.cols - templ.cols, refinedPos.x + margin);
        int y2 = std::min(image.rows - templ.rows, refinedPos.y + margin);

        if (x2 <= x1 || y2 <= y1) continue;

        cv::Mat localResult;
        cv::Rect searchRect(x1, y1, x2 - x1 + templ.cols, y2 - y1 + templ.rows);
        if (searchRect.x + searchRect.width > image.cols ||
            searchRect.y + searchRect.height > image.rows) {
            continue;
        }

        cv::matchTemplate(image(searchRect), templ, localResult, matchMethod_);

        if (matchMethod_ == cv::TM_SQDIFF || matchMethod_ == cv::TM_SQDIFF_NORMED) {
            localResult = 1.0 - localResult;
        }

        cv::minMaxLoc(localResult, &minVal, &maxVal, &minLoc, &maxLoc);

        if (maxVal >= minScore_) {
            QuickMatch match;
            match.x = x1 + maxLoc.x + templ.cols / 2.0;
            match.y = y1 + maxLoc.y + templ.rows / 2.0;
            match.score = maxVal;
            match.rect = cv::Rect(x1 + maxLoc.x, y1 + maxLoc.y, templ.cols, templ.rows);
            matches_.push_back(match);

            if (static_cast<int>(matches_.size()) >= maxMatches_) {
                break;
            }
        }
    }

    // 按分数排序
    std::sort(matches_.begin(), matches_.end(),
        [](const QuickMatch& a, const QuickMatch& b) {
            return a.score > b.score;
        });

    return true;
}

bool QuickSearchTool::processSingleScale(const cv::Mat& image, const cv::Mat& templ)
{
    cv::Mat result;
    cv::matchTemplate(image, templ, result, matchMethod_);

    // 归一化
    if (matchMethod_ == cv::TM_SQDIFF || matchMethod_ == cv::TM_SQDIFF_NORMED) {
        result = 1.0 - result;
    }

    findLocalMaxima(result, matches_);

    return true;
}

void QuickSearchTool::findLocalMaxima(const cv::Mat& result, std::vector<QuickMatch>& matches)
{
    // 找到所有局部极大值
    std::vector<std::pair<cv::Point, float>> candidates;

    for (int y = 1; y < result.rows - 1; ++y) {
        for (int x = 1; x < result.cols - 1; ++x) {
            float val = result.at<float>(y, x);
            if (val < minScore_) continue;

            // 检查是否为局部极大值
            bool isMax = true;
            for (int dy = -1; dy <= 1 && isMax; ++dy) {
                for (int dx = -1; dx <= 1 && isMax; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    if (result.at<float>(y + dy, x + dx) >= val) {
                        isMax = false;
                    }
                }
            }

            if (isMax) {
                candidates.push_back({cv::Point(x, y), val});
            }
        }
    }

    // 按分数排序
    std::sort(candidates.begin(), candidates.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    // 转换为结果
    int count = std::min(static_cast<int>(candidates.size()), maxMatches_);
    for (int i = 0; i < count; ++i) {
        QuickMatch match;
        match.x = candidates[i].first.x + templateImage_.cols / 2.0;
        match.y = candidates[i].first.y + templateImage_.rows / 2.0;
        match.score = candidates[i].second;
        match.rect = cv::Rect(candidates[i].first.x, candidates[i].first.y,
                              templateImage_.cols, templateImage_.rows);
        matches.push_back(match);
    }
}

bool QuickSearchTool::setTemplate(const cv::Mat& templateImg)
{
    if (templateImg.empty()) {
        LOG_WARNING("设置模板失败: 模板图像为空");
        return false;
    }

    templateImage_ = templateImg.clone();
    LOG_INFO(QString("模板已设置: %1x%2").arg(templateImage_.cols).arg(templateImage_.rows));
    return true;
}

bool QuickSearchTool::createTemplate(const Base::ImageData::Ptr& image, const QRect& roi)
{
    if (!image || image->isEmpty()) {
        LOG_WARNING("创建模板失败: 图像为空");
        return false;
    }

    cv::Mat mat = image->mat();
    cv::Rect cvRoi(roi.x(), roi.y(), roi.width(), roi.height());

    if (cvRoi.x < 0 || cvRoi.y < 0 ||
        cvRoi.x + cvRoi.width > mat.cols ||
        cvRoi.y + cvRoi.height > mat.rows) {
        LOG_WARNING("创建模板失败: ROI超出图像范围");
        return false;
    }

    templateImage_ = mat(cvRoi).clone();
    LOG_INFO(QString("模板已创建: %1x%2").arg(templateImage_.cols).arg(templateImage_.rows));
    return true;
}

bool QuickSearchTool::loadTemplate(const QString& filePath)
{
    if (!QFileInfo::exists(filePath)) {
        LOG_WARNING(QString("加载模板失败: 文件不存在 %1").arg(filePath));
        return false;
    }

    templateImage_ = cv::imread(filePath.toStdString());
    if (templateImage_.empty()) {
        LOG_WARNING(QString("加载模板失败: 无法读取图像 %1").arg(filePath));
        return false;
    }

    LOG_INFO(QString("模板已加载: %1").arg(filePath));
    return true;
}

bool QuickSearchTool::saveTemplate(const QString& filePath)
{
    if (!hasTemplate()) {
        LOG_WARNING("保存模板失败: 没有模板");
        return false;
    }

    bool success = cv::imwrite(filePath.toStdString(), templateImage_);
    if (success) {
        LOG_INFO(QString("模板已保存: %1").arg(filePath));
    } else {
        LOG_WARNING(QString("保存模板失败: %1").arg(filePath));
    }
    return success;
}

void QuickSearchTool::clearTemplate()
{
    templateImage_.release();
    matches_.clear();
    LOG_INFO("模板已清除");
}

QJsonObject QuickSearchTool::serializeParams() const
{
    QJsonObject json;
    json["matchMethod"] = matchMethod_;
    json["minScore"] = minScore_;
    json["maxMatches"] = maxMatches_;
    json["speedLevel"] = static_cast<int>(speedLevel_);
    json["useMultiScale"] = useMultiScale_;
    json["pyramidLevels"] = pyramidLevels_;
    return json;
}

void QuickSearchTool::deserializeParams(const QJsonObject& json)
{
    matchMethod_ = json.value("matchMethod").toInt(cv::TM_CCOEFF_NORMED);
    minScore_ = json.value("minScore").toDouble(0.7);
    maxMatches_ = json.value("maxMatches").toInt(1);
    speedLevel_ = static_cast<SpeedLevel>(json.value("speedLevel").toInt(Fast));
    useMultiScale_ = json.value("useMultiScale").toBool(true);
    pyramidLevels_ = json.value("pyramidLevels").toInt(3);
}

} // namespace Algorithm
} // namespace VisionForge
