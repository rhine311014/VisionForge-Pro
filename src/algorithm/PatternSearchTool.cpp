/**
 * @file PatternSearchTool.cpp
 * @brief 图案搜索工具实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "algorithm/PatternSearchTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"

#include <QElapsedTimer>
#include <QFileInfo>
#include <QJsonArray>
#include <cmath>
#include <opencv2/imgproc.hpp>

#ifdef USE_HALCON
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

PatternSearchTool::PatternSearchTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(tr("图案搜索"));
}

PatternSearchTool::~PatternSearchTool()
{
    clearTemplate();
}

bool PatternSearchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = tr("输入图像为空");
        return false;
    }

    if (!hasTemplate_) {
        output.success = false;
        output.errorMessage = tr("未创建图案模板");
        return false;
    }

    QElapsedTimer timer;
    timer.start();

#ifdef USE_HALCON
    bool success = processHalcon(input, output);
#else
    bool success = processOpenCV(input, output);
#endif

    output.executionTime = timer.elapsed();

    if (success) {
        output.success = true;
        output.setValue("matchCount", static_cast<int>(matches_.size()));

        // 将匹配结果转为JSON数组
        QJsonArray matchArray;
        for (const auto& match : matches_) {
            QJsonObject matchObj;
            matchObj["x"] = match.col;
            matchObj["y"] = match.row;
            matchObj["angle"] = match.angle * 180.0 / M_PI;  // 转换为度
            matchObj["scale"] = match.scale;
            matchObj["score"] = match.score;
            matchArray.append(matchObj);
        }
        output.data["matches"] = matchArray;

        setStatusText(tr("找到 %1 个匹配，耗时 %2 ms")
            .arg(matches_.size()).arg(output.executionTime));
    }

    return success;
}

bool PatternSearchTool::processHalcon(const Base::ImageData::Ptr& input, ToolResult& output)
{
#ifdef USE_HALCON
    try {
        // 转换输入图像到Halcon格式
        cv::Mat mat = input->mat();
        cv::Mat grayMat;
        if (mat.channels() == 3) {
            cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
        } else {
            grayMat = mat;
        }

        HImage hImage;
        hImage.GenImage1("byte", grayMat.cols, grayMat.rows,
                         reinterpret_cast<void*>(grayMat.data));

        // 执行形状匹配
        HTuple rows, cols, angles, scales, scores;

        // 根据是否启用缩放选择不同的匹配函数
        if (scaleSearchEnabled_) {
            shapeModel_.FindScaledShapeModel(
                hImage,
                angleMin_ * M_PI / 180.0,
                (angleMax_ - angleMin_) * M_PI / 180.0,
                scaleMin_,
                scaleMax_,
                minScore_,
                maxMatches_,
                overlapThreshold_,
                "least_squares",
                pyramidLevels_,
                0.9,
                &rows, &cols, &angles, &scales, &scores);
        } else {
            scales = HTuple(1.0);
            shapeModel_.FindShapeModel(
                hImage,
                angleMin_ * M_PI / 180.0,
                (angleMax_ - angleMin_) * M_PI / 180.0,
                minScore_,
                maxMatches_,
                overlapThreshold_,
                "least_squares",
                pyramidLevels_,
                0.9,
                &rows, &cols, &angles, &scores);
        }

        // 收集结果
        matches_.clear();
        int numMatches = scores.Length();
        for (int i = 0; i < numMatches; ++i) {
            PatternMatch match;
            match.row = rows[i].D();
            match.col = cols[i].D();
            match.angle = angles[i].D();
            match.scale = scaleSearchEnabled_ ? scales[i].D() : 1.0;
            match.score = scores[i].D();
            matches_.push_back(match);
        }

        LOG_INFO(QString("Halcon形状匹配完成，找到 %1 个结果").arg(numMatches));
        return true;

    } catch (HException& e) {
        output.errorMessage = QString("Halcon错误: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    }
#else
    Q_UNUSED(input);
    Q_UNUSED(output);
    return false;
#endif
}

bool PatternSearchTool::processOpenCV(const Base::ImageData::Ptr& input, ToolResult& output)
{
    // OpenCV备用实现（简化版模板匹配）
    Q_UNUSED(input);
    output.success = false;
    output.errorMessage = tr("图案搜索需要Halcon支持");
    LOG_WARNING("PatternSearchTool: OpenCV fallback not implemented");
    return false;
}

bool PatternSearchTool::createTemplate(const Base::ImageData::Ptr& image, const QRectF& roi)
{
    if (!image || image->isEmpty()) {
        LOG_ERROR("创建模板失败: 图像为空");
        return false;
    }

#ifdef USE_HALCON
    try {
        // 转换图像
        cv::Mat mat = image->mat();
        cv::Mat grayMat;
        if (mat.channels() == 3) {
            cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
        } else {
            grayMat = mat;
        }

        HImage hImage;
        hImage.GenImage1("byte", grayMat.cols, grayMat.rows,
                         reinterpret_cast<void*>(grayMat.data));

        // 裁剪ROI区域
        HImage roiImage = hImage.CropRectangle1(
            static_cast<Hlong>(roi.top()),
            static_cast<Hlong>(roi.left()),
            static_cast<Hlong>(roi.bottom()),
            static_cast<Hlong>(roi.right()));

        templateImage_ = roiImage;

        // 创建形状模型
        HTuple modelID;
        if (scaleSearchEnabled_) {
            shapeModel_ = roiImage.CreateScaledShapeModel(
                pyramidLevels_,
                angleMin_ * M_PI / 180.0,
                (angleMax_ - angleMin_) * M_PI / 180.0,
                "auto",
                scaleMin_,
                scaleMax_,
                "auto",
                "auto",
                "use_polarity",
                "auto",
                "auto");
        } else {
            shapeModel_ = roiImage.CreateShapeModel(
                pyramidLevels_,
                angleMin_ * M_PI / 180.0,
                (angleMax_ - angleMin_) * M_PI / 180.0,
                "auto",
                "auto",
                "use_polarity",
                "auto",
                "auto");
        }

        hasTemplate_ = true;
        LOG_INFO(QString("形状模板创建成功，ROI: (%1,%2) %3x%4")
            .arg(roi.x()).arg(roi.y()).arg(roi.width()).arg(roi.height()));
        return true;

    } catch (HException& e) {
        LOG_ERROR(QString("创建形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    Q_UNUSED(image);
    Q_UNUSED(roi);
    LOG_WARNING("创建模板需要Halcon支持");
    return false;
#endif
}

bool PatternSearchTool::loadTemplate(const QString& filePath)
{
    if (!QFileInfo::exists(filePath)) {
        LOG_ERROR(QString("模板文件不存在: %1").arg(filePath));
        return false;
    }

#ifdef USE_HALCON
    try {
        shapeModel_.ReadShapeModel(filePath.toStdString().c_str());
        hasTemplate_ = true;
        LOG_INFO(QString("形状模板加载成功: %1").arg(filePath));
        return true;
    } catch (HException& e) {
        LOG_ERROR(QString("加载形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    Q_UNUSED(filePath);
    LOG_WARNING("加载模板需要Halcon支持");
    return false;
#endif
}

bool PatternSearchTool::saveTemplate(const QString& filePath)
{
    if (!hasTemplate_) {
        LOG_ERROR("保存模板失败: 未创建模板");
        return false;
    }

#ifdef USE_HALCON
    try {
        shapeModel_.WriteShapeModel(filePath.toStdString().c_str());
        LOG_INFO(QString("形状模板保存成功: %1").arg(filePath));
        return true;
    } catch (HException& e) {
        LOG_ERROR(QString("保存形状模板失败: %1").arg(e.ErrorMessage().Text()));
        return false;
    }
#else
    Q_UNUSED(filePath);
    LOG_WARNING("保存模板需要Halcon支持");
    return false;
#endif
}

void PatternSearchTool::clearTemplate()
{
#ifdef USE_HALCON
    if (hasTemplate_) {
        try {
            shapeModel_.ClearShapeModel();
        } catch (...) {
            // 忽略清除时的错误
        }
    }
#endif
    hasTemplate_ = false;
    matches_.clear();
}

void PatternSearchTool::setAngleRange(double min, double max, double step)
{
    angleMin_ = min;
    angleMax_ = max;
    angleStep_ = qMax(0.01, step);
}

void PatternSearchTool::setScaleRange(double min, double max, double step)
{
    scaleMin_ = qMax(0.1, min);
    scaleMax_ = qMax(scaleMin_, max);
    scaleStep_ = qMax(0.01, step);
}

PatternMatch PatternSearchTool::bestMatch() const
{
    if (matches_.empty()) {
        return PatternMatch();
    }

    auto it = std::max_element(matches_.begin(), matches_.end(),
        [](const PatternMatch& a, const PatternMatch& b) {
            return a.score < b.score;
        });

    return *it;
}

QJsonObject PatternSearchTool::serializeParams() const
{
    QJsonObject json;
    json["matchMode"] = static_cast<int>(matchMode_);
    json["minScore"] = minScore_;
    json["maxMatches"] = maxMatches_;
    json["overlapThreshold"] = overlapThreshold_;
    json["subPixelMode"] = static_cast<int>(subPixelMode_);
    json["pyramidLevels"] = pyramidLevels_;

    json["angleSearchEnabled"] = angleSearchEnabled_;
    json["angleMin"] = angleMin_;
    json["angleMax"] = angleMax_;
    json["angleStep"] = angleStep_;

    json["scaleSearchEnabled"] = scaleSearchEnabled_;
    json["scaleMin"] = scaleMin_;
    json["scaleMax"] = scaleMax_;
    json["scaleStep"] = scaleStep_;

    return json;
}

void PatternSearchTool::deserializeParams(const QJsonObject& json)
{
    matchMode_ = static_cast<MatchMode>(json.value("matchMode").toInt(EdgeMatch));
    minScore_ = json.value("minScore").toDouble(0.5);
    maxMatches_ = json.value("maxMatches").toInt(10);
    overlapThreshold_ = json.value("overlapThreshold").toDouble(0.5);
    subPixelMode_ = static_cast<SubPixelMode>(json.value("subPixelMode").toInt(LeastSquares));
    pyramidLevels_ = json.value("pyramidLevels").toInt(4);

    angleSearchEnabled_ = json.value("angleSearchEnabled").toBool(true);
    angleMin_ = json.value("angleMin").toDouble(-30.0);
    angleMax_ = json.value("angleMax").toDouble(30.0);
    angleStep_ = json.value("angleStep").toDouble(1.0);

    scaleSearchEnabled_ = json.value("scaleSearchEnabled").toBool(false);
    scaleMin_ = json.value("scaleMin").toDouble(0.8);
    scaleMax_ = json.value("scaleMax").toDouble(1.2);
    scaleStep_ = json.value("scaleStep").toDouble(0.05);
}

} // namespace Algorithm
} // namespace VisionForge
