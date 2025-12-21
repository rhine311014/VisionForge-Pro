/**
 * @file TemplateMatchTool.cpp
 * @brief OpenCV模板匹配工具实现
 */

#include "algorithm/TemplateMatchTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QElapsedTimer>
#include <QFileInfo>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace VisionForge {
namespace Algorithm {

TemplateMatchTool::TemplateMatchTool(QObject* parent)
    : VisionTool(parent)
    , method_(MatchMethod::CCOEFF_NORMED)
    , threshold_(0.7)
    , maxMatches_(10)
    , usePyramid_(true)
    , pyramidLevels_(3)
    , enableAngleSearch_(false)
    , angleMin_(-10.0)
    , angleMax_(10.0)
    , angleStep_(2.0)
    , nmsDistance_(20.0)
{
    setDisplayName(toolName());
}

bool TemplateMatchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    results_.clear();

    // 检查模板
    if (template_.empty()) {
        output.success = false;
        output.errorMessage = "未设置模板图像";
        setStatusText("错误: 未设置模板");
        return false;
    }

    // 检查输入
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像无效";
        setStatusText("错误: 输入图像无效");
        return false;
    }

    cv::Mat searchImage = input->mat();

    // 转换为灰度图
    cv::Mat grayImage, grayTemplate;
    if (searchImage.channels() == 3) {
        cv::cvtColor(searchImage, grayImage, cv::COLOR_BGR2GRAY);
    } else if (searchImage.channels() == 4) {
        cv::cvtColor(searchImage, grayImage, cv::COLOR_BGRA2GRAY);
    } else {
        grayImage = searchImage;
    }

    if (template_.channels() == 3) {
        cv::cvtColor(template_, grayTemplate, cv::COLOR_BGR2GRAY);
    } else if (template_.channels() == 4) {
        cv::cvtColor(template_, grayTemplate, cv::COLOR_BGRA2GRAY);
    } else {
        grayTemplate = template_;
    }

    // 执行匹配
    if (enableAngleSearch_) {
        // 角度搜索匹配
        for (double angle = angleMin_; angle <= angleMax_; angle += angleStep_) {
            cv::Mat rotatedTemplate = rotateTemplate(grayTemplate, angle);
            if (usePyramid_) {
                pyramidMatch(grayImage, rotatedTemplate);
            } else {
                performMatch(grayImage, rotatedTemplate, angle);
            }
        }
    } else {
        // 普通匹配
        if (usePyramid_) {
            pyramidMatch(grayImage, grayTemplate);
        } else {
            performMatch(grayImage, grayTemplate);
        }
    }

    // 非极大值抑制
    nonMaximumSuppression();

    // 限制结果数量
    if (static_cast<int>(results_.size()) > maxMatches_) {
        results_.resize(maxMatches_);
    }

    // 设置输出
    output.success = !results_.empty();
    output.executionTime = timer.elapsed();

    if (!results_.empty()) {
        const auto& best = results_[0];
        output.setValue("matchCount", static_cast<int>(results_.size()));
        output.setValue("bestScore", best.score);
        output.setValue("centerX", best.center.x());
        output.setValue("centerY", best.center.y());
        output.setValue("angle", best.angle);
    } else {
        output.setValue("matchCount", 0);
        output.setValue("bestScore", 0.0);
    }

    // 绘制结果
    cv::Mat resultImage = drawResults(input->mat());
    output.outputImage = Base::ImageMemoryPool::instance().allocate(
        resultImage.cols, resultImage.rows, resultImage.type());
    if (output.outputImage) {
        resultImage.copyTo(output.outputImage->mat());
    }
    setDebugImage(output.outputImage);

    QString statusText = QString("找到 %1 个匹配").arg(results_.size());
    if (!results_.empty()) {
        statusText += QString(" (最佳: %.1f%%)").arg(results_[0].score * 100);
    }
    setStatusText(statusText);

    LOG_DEBUG(QString("模板匹配完成: %1个匹配, 耗时: %2ms")
             .arg(results_.size()).arg(output.executionTime));

    emit matchCompleted(static_cast<int>(results_.size()));
    return output.success;
}

void TemplateMatchTool::performMatch(const cv::Mat& image, const cv::Mat& templ,
                                     double angle)
{
    if (templ.cols > image.cols || templ.rows > image.rows) {
        return;
    }

    // 创建结果矩阵
    int resultCols = image.cols - templ.cols + 1;
    int resultRows = image.rows - templ.rows + 1;
    cv::Mat result(resultRows, resultCols, CV_32FC1);

    // 执行模板匹配
    int method = getOpenCVMatchMethod();
    cv::matchTemplate(image, templ, result, method);

    // 归一化结果 (对于非归一化方法)
    if (method == cv::TM_SQDIFF || method == cv::TM_CCORR ||
        method == cv::TM_CCOEFF) {
        cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);
    }

    // 查找匹配点
    bool isMinBetter = (method == cv::TM_SQDIFF || method == cv::TM_SQDIFF_NORMED);

    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            double score = result.at<float>(y, x);

            // 对于平方差方法，转换分数
            if (isMinBetter) {
                score = 1.0 - score;
            }

            if (score >= threshold_) {
                MatchResult match;
                match.position = QPointF(x, y);
                match.center = QPointF(x + templ.cols / 2.0, y + templ.rows / 2.0);
                match.score = score;
                match.angle = angle;
                match.scale = 1.0;
                match.boundingRect = QRectF(x, y, templ.cols, templ.rows);
                match.valid = true;
                results_.push_back(match);
            }
        }
    }
}

void TemplateMatchTool::pyramidMatch(const cv::Mat& image, const cv::Mat& templ)
{
    // 构建图像金字塔
    std::vector<cv::Mat> imagePyramid, templPyramid;
    imagePyramid.push_back(image);
    templPyramid.push_back(templ);

    cv::Mat currentImage = image;
    cv::Mat currentTempl = templ;

    for (int i = 1; i < pyramidLevels_; ++i) {
        cv::pyrDown(currentImage, currentImage);
        cv::pyrDown(currentTempl, currentTempl);
        imagePyramid.push_back(currentImage);
        templPyramid.push_back(currentTempl);
    }

    // 从最小层开始匹配
    std::vector<cv::Point> candidates;
    int scaleFactor = 1 << (pyramidLevels_ - 1);

    // 在最顶层进行全局搜索
    cv::Mat topImage = imagePyramid[pyramidLevels_ - 1];
    cv::Mat topTempl = templPyramid[pyramidLevels_ - 1];

    if (topTempl.cols >= topImage.cols || topTempl.rows >= topImage.rows) {
        // 模板太大，直接在原图匹配
        performMatch(image, templ);
        return;
    }

    int resultCols = topImage.cols - topTempl.cols + 1;
    int resultRows = topImage.rows - topTempl.rows + 1;
    cv::Mat result(resultRows, resultCols, CV_32FC1);

    int method = getOpenCVMatchMethod();
    cv::matchTemplate(topImage, topTempl, result, method);

    bool isMinBetter = (method == cv::TM_SQDIFF || method == cv::TM_SQDIFF_NORMED);

    // 找到候选点
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    cv::Point bestLoc = isMinBetter ? minLoc : maxLoc;

    // 在候选点周围进行精细搜索
    int searchRadius = 2;
    for (int dy = -searchRadius; dy <= searchRadius; ++dy) {
        for (int dx = -searchRadius; dx <= searchRadius; ++dx) {
            int x = (bestLoc.x + dx) * scaleFactor;
            int y = (bestLoc.y + dy) * scaleFactor;
            if (x >= 0 && y >= 0 &&
                x + templ.cols <= image.cols &&
                y + templ.rows <= image.rows) {
                candidates.push_back(cv::Point(x, y));
            }
        }
    }

    // 在原图上对候选区域进行精确匹配
    for (const auto& pt : candidates) {
        int x = pt.x;
        int y = pt.y;
        int w = std::min(templ.cols * 2, image.cols - x);
        int h = std::min(templ.rows * 2, image.rows - y);

        if (w < templ.cols || h < templ.rows) continue;

        cv::Rect roi(x, y, w, h);
        cv::Mat roiImage = image(roi);

        int rCols = roiImage.cols - templ.cols + 1;
        int rRows = roiImage.rows - templ.rows + 1;
        if (rCols <= 0 || rRows <= 0) continue;

        cv::Mat localResult(rRows, rCols, CV_32FC1);
        cv::matchTemplate(roiImage, templ, localResult, method);

        cv::minMaxLoc(localResult, &minVal, &maxVal, &minLoc, &maxLoc);

        double score = isMinBetter ? (1.0 - minVal) : maxVal;
        cv::Point loc = isMinBetter ? minLoc : maxLoc;

        if (score >= threshold_) {
            MatchResult match;
            match.position = QPointF(x + loc.x, y + loc.y);
            match.center = QPointF(x + loc.x + templ.cols / 2.0,
                                  y + loc.y + templ.rows / 2.0);
            match.score = score;
            match.angle = 0.0;
            match.scale = 1.0;
            match.boundingRect = QRectF(x + loc.x, y + loc.y, templ.cols, templ.rows);
            match.valid = true;
            results_.push_back(match);
        }
    }
}

void TemplateMatchTool::nonMaximumSuppression()
{
    if (results_.size() <= 1) return;

    // 按分数排序
    std::sort(results_.begin(), results_.end(),
             [](const MatchResult& a, const MatchResult& b) {
                 return a.score > b.score;
             });

    std::vector<MatchResult> filtered;
    std::vector<bool> suppressed(results_.size(), false);

    for (size_t i = 0; i < results_.size(); ++i) {
        if (suppressed[i]) continue;

        filtered.push_back(results_[i]);

        // 抑制距离过近的其他匹配
        for (size_t j = i + 1; j < results_.size(); ++j) {
            if (suppressed[j]) continue;

            double dx = results_[i].center.x() - results_[j].center.x();
            double dy = results_[i].center.y() - results_[j].center.y();
            double dist = std::sqrt(dx * dx + dy * dy);

            if (dist < nmsDistance_) {
                suppressed[j] = true;
            }
        }
    }

    results_ = filtered;
}

cv::Mat TemplateMatchTool::rotateTemplate(const cv::Mat& templ, double angle)
{
    if (std::abs(angle) < 0.001) {
        return templ;
    }

    cv::Point2f center(templ.cols / 2.0f, templ.rows / 2.0f);
    cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, 1.0);

    // 计算旋转后的边界框大小
    double abscos = std::abs(rotMat.at<double>(0, 0));
    double abssin = std::abs(rotMat.at<double>(0, 1));
    int newW = static_cast<int>(templ.rows * abssin + templ.cols * abscos);
    int newH = static_cast<int>(templ.rows * abscos + templ.cols * abssin);

    // 调整旋转矩阵
    rotMat.at<double>(0, 2) += (newW - templ.cols) / 2.0;
    rotMat.at<double>(1, 2) += (newH - templ.rows) / 2.0;

    cv::Mat rotated;
    cv::warpAffine(templ, rotated, rotMat, cv::Size(newW, newH));

    return rotated;
}

cv::Mat TemplateMatchTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 绘制所有匹配
    for (size_t i = 0; i < results_.size(); ++i) {
        const auto& match = results_[i];

        // 匹配区域矩形
        cv::Rect rect(
            static_cast<int>(match.boundingRect.x()),
            static_cast<int>(match.boundingRect.y()),
            static_cast<int>(match.boundingRect.width()),
            static_cast<int>(match.boundingRect.height())
        );

        cv::Scalar color = (i == 0) ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 200, 255);
        cv::rectangle(result, rect, color, 2);

        // 中心点
        cv::Point center(static_cast<int>(match.center.x()),
                        static_cast<int>(match.center.y()));
        cv::circle(result, center, 5, cv::Scalar(0, 0, 255), -1);
        cv::line(result, cv::Point(center.x - 10, center.y),
                cv::Point(center.x + 10, center.y), cv::Scalar(0, 0, 255), 1);
        cv::line(result, cv::Point(center.x, center.y - 10),
                cv::Point(center.x, center.y + 10), cv::Scalar(0, 0, 255), 1);

        // 序号和分数
        QString text = QString("#%1: %.1f%%").arg(i + 1).arg(match.score * 100);
        cv::putText(result, text.toStdString(),
                   cv::Point(rect.x, rect.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }

    // 显示匹配数量
    QString countText = QString("Matches: %1").arg(results_.size());
    cv::putText(result, countText.toStdString(),
               cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
               cv::Scalar(255, 255, 0), 2);

    return result;
}

int TemplateMatchTool::getOpenCVMatchMethod() const
{
    switch (method_) {
    case MatchMethod::SQDIFF:        return cv::TM_SQDIFF;
    case MatchMethod::SQDIFF_NORMED: return cv::TM_SQDIFF_NORMED;
    case MatchMethod::CCORR:         return cv::TM_CCORR;
    case MatchMethod::CCORR_NORMED:  return cv::TM_CCORR_NORMED;
    case MatchMethod::CCOEFF:        return cv::TM_CCOEFF;
    case MatchMethod::CCOEFF_NORMED: return cv::TM_CCOEFF_NORMED;
    default:                         return cv::TM_CCOEFF_NORMED;
    }
}

void TemplateMatchTool::setTemplate(const cv::Mat& templateImage)
{
    template_ = templateImage.clone();
    templatePath_.clear();
    emit templateChanged();
    emit paramChanged();
}

bool TemplateMatchTool::loadTemplate(const QString& filePath)
{
    cv::Mat templ = cv::imread(filePath.toStdString(), cv::IMREAD_UNCHANGED);
    if (templ.empty()) {
        LOG_ERROR(QString("加载模板失败: %1").arg(filePath));
        return false;
    }

    template_ = templ;
    templatePath_ = filePath;
    emit templateChanged();
    emit paramChanged();

    LOG_INFO(QString("加载模板: %1 (%2x%3)")
            .arg(filePath).arg(templ.cols).arg(templ.rows));
    return true;
}

bool TemplateMatchTool::saveTemplate(const QString& filePath) const
{
    if (template_.empty()) {
        LOG_ERROR("没有模板可保存");
        return false;
    }

    bool result = cv::imwrite(filePath.toStdString(), template_);
    if (result) {
        LOG_INFO(QString("保存模板: %1").arg(filePath));
    } else {
        LOG_ERROR(QString("保存模板失败: %1").arg(filePath));
    }
    return result;
}

void TemplateMatchTool::createTemplateFromROI(const cv::Mat& image, const QRectF& roi)
{
    cv::Rect cvRoi(
        static_cast<int>(roi.x()),
        static_cast<int>(roi.y()),
        static_cast<int>(roi.width()),
        static_cast<int>(roi.height())
    );

    // 边界检查
    cvRoi.x = std::max(0, cvRoi.x);
    cvRoi.y = std::max(0, cvRoi.y);
    cvRoi.width = std::min(cvRoi.width, image.cols - cvRoi.x);
    cvRoi.height = std::min(cvRoi.height, image.rows - cvRoi.y);

    if (cvRoi.width <= 0 || cvRoi.height <= 0) {
        LOG_ERROR("无效的ROI区域");
        return;
    }

    template_ = image(cvRoi).clone();
    templatePath_.clear();
    emit templateChanged();
    emit paramChanged();

    LOG_INFO(QString("从ROI创建模板: %1x%2").arg(template_.cols).arg(template_.rows));
}

MatchResult TemplateMatchTool::bestMatch() const
{
    if (results_.empty()) {
        return MatchResult();
    }
    return results_[0];
}

void TemplateMatchTool::setMatchMethod(MatchMethod method)
{
    if (method_ != method) {
        method_ = method;
        emit paramChanged();
    }
}

void TemplateMatchTool::setThreshold(double threshold)
{
    threshold = std::clamp(threshold, 0.0, 1.0);
    if (std::abs(threshold_ - threshold) > 0.001) {
        threshold_ = threshold;
        emit paramChanged();
    }
}

void TemplateMatchTool::setMaxMatches(int count)
{
    count = std::max(1, count);
    if (maxMatches_ != count) {
        maxMatches_ = count;
        emit paramChanged();
    }
}

void TemplateMatchTool::setUsePyramid(bool use)
{
    if (usePyramid_ != use) {
        usePyramid_ = use;
        emit paramChanged();
    }
}

void TemplateMatchTool::setPyramidLevels(int levels)
{
    levels = std::clamp(levels, 1, 6);
    if (pyramidLevels_ != levels) {
        pyramidLevels_ = levels;
        emit paramChanged();
    }
}

void TemplateMatchTool::setEnableAngleSearch(bool enable)
{
    if (enableAngleSearch_ != enable) {
        enableAngleSearch_ = enable;
        emit paramChanged();
    }
}

void TemplateMatchTool::setAngleRange(double minAngle, double maxAngle, double step)
{
    angleMin_ = minAngle;
    angleMax_ = maxAngle;
    angleStep_ = std::max(0.5, step);
    emit paramChanged();
}

void TemplateMatchTool::setNmsDistance(double distance)
{
    distance = std::max(1.0, distance);
    if (std::abs(nmsDistance_ - distance) > 0.001) {
        nmsDistance_ = distance;
        emit paramChanged();
    }
}

QJsonObject TemplateMatchTool::serializeParams() const
{
    QJsonObject json;
    json["method"] = static_cast<int>(method_);
    json["threshold"] = threshold_;
    json["maxMatches"] = maxMatches_;
    json["usePyramid"] = usePyramid_;
    json["pyramidLevels"] = pyramidLevels_;
    json["enableAngleSearch"] = enableAngleSearch_;
    json["angleMin"] = angleMin_;
    json["angleMax"] = angleMax_;
    json["angleStep"] = angleStep_;
    json["nmsDistance"] = nmsDistance_;
    json["templatePath"] = templatePath_;

    return json;
}

void TemplateMatchTool::deserializeParams(const QJsonObject& json)
{
    method_ = static_cast<MatchMethod>(json.value("method").toInt(5));
    threshold_ = json.value("threshold").toDouble(0.7);
    maxMatches_ = json.value("maxMatches").toInt(10);
    usePyramid_ = json.value("usePyramid").toBool(true);
    pyramidLevels_ = json.value("pyramidLevels").toInt(3);
    enableAngleSearch_ = json.value("enableAngleSearch").toBool(false);
    angleMin_ = json.value("angleMin").toDouble(-10.0);
    angleMax_ = json.value("angleMax").toDouble(10.0);
    angleStep_ = json.value("angleStep").toDouble(2.0);
    nmsDistance_ = json.value("nmsDistance").toDouble(20.0);

    templatePath_ = json.value("templatePath").toString();
    if (!templatePath_.isEmpty()) {
        loadTemplate(templatePath_);
    }
}

} // namespace Algorithm
} // namespace VisionForge
