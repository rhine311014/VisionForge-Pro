/**
 * @file QuickSearchTool.cpp
 * @brief 快速搜索工具实现文件
 *
 * @details
 * 本文件实现了QuickSearchTool类的所有方法，包括：
 * - 模板设置、加载、保存和清除
 * - 单尺度和多尺度（金字塔）模板匹配
 * - 局部极大值检测（多目标检测）
 * - 参数序列化和反序列化
 *
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

/**
 * @brief 构造函数实现
 *
 * 初始化工具的显示名称
 */
QuickSearchTool::QuickSearchTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(tr("快速搜索"));
}

/**
 * @brief 执行快速搜索的主入口方法
 *
 * 处理流程：
 * 1. 验证输入图像和模板的有效性
 * 2. 将图像和模板转换为灰度
 * 3. 根据设置选择单尺度或多尺度搜索
 * 4. 整理输出结果
 */
bool QuickSearchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    // 步骤1：验证输入图像
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = tr("输入图像为空");
        return false;
    }

    // 验证是否有模板
    if (!hasTemplate()) {
        output.success = false;
        output.errorMessage = tr("未设置模板");
        return false;
    }

    // 启动计时器
    QElapsedTimer timer;
    timer.start();

    // 步骤2：转换图像为灰度
    cv::Mat mat = input->mat();
    cv::Mat grayMat;
    if (mat.channels() == 3) {
        cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
    } else {
        grayMat = mat;
    }

    // 转换模板为灰度
    cv::Mat grayTempl;
    if (templateImage_.channels() == 3) {
        cv::cvtColor(templateImage_, grayTempl, cv::COLOR_BGR2GRAY);
    } else {
        grayTempl = templateImage_;
    }

    bool success = false;
    matches_.clear();  // 清空之前的搜索结果

    // 步骤3：根据设置选择搜索方法
    try {
        if (useMultiScale_) {
            // 使用金字塔加速的多尺度搜索
            success = processMultiScale(grayMat, grayTempl);
        } else {
            // 单尺度直接搜索
            success = processSingleScale(grayMat, grayTempl);
        }
    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("OpenCV错误: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }

    // 记录执行时间
    output.executionTime = timer.elapsed();

    // 步骤4：处理成功时整理输出结果
    if (success) {
        output.success = true;
        output.setValue("matchCount", static_cast<int>(matches_.size()));

        // 将搜索结果转换为JSON数组
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

        // 更新状态文本
        setStatusText(tr("找到 %1 个匹配，耗时 %2 ms")
            .arg(matches_.size()).arg(output.executionTime));
    }

    return success;
}

/**
 * @brief 使用金字塔加速的多尺度模板匹配
 *
 * 算法步骤：
 * 1. 构建图像和模板的金字塔
 * 2. 在最粗糙层进行快速搜索，找到候选位置
 * 3. 将候选位置映射回原始分辨率
 * 4. 在原始分辨率的局部区域进行精确匹配
 *
 * @param image 待搜索的灰度图像
 * @param templ 模板灰度图像
 * @return 是否搜索成功
 */
bool QuickSearchTool::processMultiScale(const cv::Mat& image, const cv::Mat& templ)
{
    // 步骤1：构建图像金字塔
    // 金字塔是一系列逐渐缩小的图像，每层分辨率是上一层的一半
    std::vector<cv::Mat> imagePyramid, templPyramid;
    imagePyramid.push_back(image);
    templPyramid.push_back(templ);

    cv::Mat prevImg = image;
    cv::Mat prevTempl = templ;

    // 逐层构建金字塔
    for (int i = 1; i < pyramidLevels_; ++i) {
        cv::Mat downImg, downTempl;
        // pyrDown将图像尺寸缩小一半
        cv::pyrDown(prevImg, downImg);
        cv::pyrDown(prevTempl, downTempl);

        // 检查模板是否太小，如果太小则停止构建金字塔
        // 模板太小会导致匹配不准确
        if (downTempl.cols < 10 || downTempl.rows < 10) {
            break;
        }

        imagePyramid.push_back(downImg);
        templPyramid.push_back(downTempl);
        prevImg = downImg;
        prevTempl = downTempl;
    }

    // 步骤2：在最粗糙层（顶层）进行快速搜索
    int topLevel = static_cast<int>(imagePyramid.size()) - 1;
    cv::Mat result;
    // 执行模板匹配
    cv::matchTemplate(imagePyramid[topLevel], templPyramid[topLevel], result, matchMethod_);

    // 对于平方差方法，需要反转结果（因为平方差越小越好）
    if (matchMethod_ == cv::TM_SQDIFF || matchMethod_ == cv::TM_SQDIFF_NORMED) {
        result = 1.0 - result;  // 反转使得值越大越好
    }

    // 步骤3：在顶层找到所有超过阈值的候选位置
    std::vector<cv::Point> candidates;
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    // 获取最大最小值
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // 遍历结果图，找到所有超过阈值的点作为候选
    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            if (result.at<float>(y, x) >= minScore_) {
                candidates.push_back(cv::Point(x, y));
            }
        }
    }

    // 步骤4：在原始分辨率上对每个候选位置进行精确匹配
    for (const auto& cand : candidates) {
        // 将坐标从金字塔顶层映射回原始分辨率
        // 每层金字塔分辨率减半，所以需要乘以2^topLevel
        int scale = 1 << topLevel;  // 等价于 pow(2, topLevel)
        cv::Point refinedPos(cand.x * scale, cand.y * scale);

        // 定义局部搜索区域（在候选位置周围扩展一定边距）
        int margin = 10;  // 搜索边距
        int x1 = std::max(0, refinedPos.x - margin);
        int y1 = std::max(0, refinedPos.y - margin);
        int x2 = std::min(image.cols - templ.cols, refinedPos.x + margin);
        int y2 = std::min(image.rows - templ.rows, refinedPos.y + margin);

        // 检查搜索区域是否有效
        if (x2 <= x1 || y2 <= y1) continue;

        // 定义搜索矩形区域
        cv::Mat localResult;
        cv::Rect searchRect(x1, y1, x2 - x1 + templ.cols, y2 - y1 + templ.rows);

        // 确保搜索区域不超出图像边界
        if (searchRect.x + searchRect.width > image.cols ||
            searchRect.y + searchRect.height > image.rows) {
            continue;
        }

        // 在局部区域进行精确模板匹配
        cv::matchTemplate(image(searchRect), templ, localResult, matchMethod_);

        // 处理平方差方法
        if (matchMethod_ == cv::TM_SQDIFF || matchMethod_ == cv::TM_SQDIFF_NORMED) {
            localResult = 1.0 - localResult;
        }

        // 找到局部区域的最佳匹配位置
        cv::minMaxLoc(localResult, &minVal, &maxVal, &minLoc, &maxLoc);

        // 如果分数满足阈值，添加到结果中
        if (maxVal >= minScore_) {
            QuickMatch match;
            // 计算匹配中心点坐标（搜索区域起点 + 局部位置 + 模板半宽/半高）
            match.x = x1 + maxLoc.x + templ.cols / 2.0;
            match.y = y1 + maxLoc.y + templ.rows / 2.0;
            match.score = maxVal;
            // 记录匹配矩形
            match.rect = cv::Rect(x1 + maxLoc.x, y1 + maxLoc.y, templ.cols, templ.rows);
            matches_.push_back(match);

            // 检查是否达到最大匹配数
            if (static_cast<int>(matches_.size()) >= maxMatches_) {
                break;
            }
        }
    }

    // 步骤5：按分数降序排序结果
    std::sort(matches_.begin(), matches_.end(),
        [](const QuickMatch& a, const QuickMatch& b) {
            return a.score > b.score;  // 分数高的排在前面
        });

    return true;
}

/**
 * @brief 单尺度直接模板匹配
 *
 * 在原始分辨率上直接进行模板匹配，不使用金字塔加速。
 * 适用于小图像或需要最高精度的场景。
 *
 * @param image 待搜索的灰度图像
 * @param templ 模板灰度图像
 * @return 是否搜索成功
 */
bool QuickSearchTool::processSingleScale(const cv::Mat& image, const cv::Mat& templ)
{
    // 执行模板匹配
    cv::Mat result;
    cv::matchTemplate(image, templ, result, matchMethod_);

    // 处理平方差方法：反转结果使得值越大越好
    if (matchMethod_ == cv::TM_SQDIFF || matchMethod_ == cv::TM_SQDIFF_NORMED) {
        result = 1.0 - result;
    }

    // 查找多个局部极大值（多目标检测）
    findLocalMaxima(result, matches_);

    return true;
}

/**
 * @brief 在匹配结果图上查找局部极大值
 *
 * 通过非极大值抑制（NMS）找到多个不重叠的匹配位置。
 * 局部极大值是指在3x3邻域内值最大的点。
 *
 * @param result 模板匹配的结果图
 * @param matches 输出的匹配结果向量
 */
void QuickSearchTool::findLocalMaxima(const cv::Mat& result, std::vector<QuickMatch>& matches)
{
    // 存储所有候选点及其分数
    std::vector<std::pair<cv::Point, float>> candidates;

    // 遍历结果图（避开边界）
    for (int y = 1; y < result.rows - 1; ++y) {
        for (int x = 1; x < result.cols - 1; ++x) {
            float val = result.at<float>(y, x);

            // 首先检查是否超过阈值
            if (val < minScore_) continue;

            // 检查是否为局部极大值（3x3邻域非极大值抑制）
            bool isMax = true;
            for (int dy = -1; dy <= 1 && isMax; ++dy) {
                for (int dx = -1; dx <= 1 && isMax; ++dx) {
                    if (dx == 0 && dy == 0) continue;  // 跳过中心点
                    // 如果邻域点的值>=当前点，则不是极大值
                    if (result.at<float>(y + dy, x + dx) >= val) {
                        isMax = false;
                    }
                }
            }

            // 如果是局部极大值，添加到候选列表
            if (isMax) {
                candidates.push_back({cv::Point(x, y), val});
            }
        }
    }

    // 按分数降序排序
    std::sort(candidates.begin(), candidates.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;  // 分数高的排在前面
        });

    // 转换为QuickMatch结构，限制最大数量
    int count = std::min(static_cast<int>(candidates.size()), maxMatches_);
    for (int i = 0; i < count; ++i) {
        QuickMatch match;
        // 计算匹配中心点坐标
        match.x = candidates[i].first.x + templateImage_.cols / 2.0;
        match.y = candidates[i].first.y + templateImage_.rows / 2.0;
        match.score = candidates[i].second;
        // 设置匹配矩形
        match.rect = cv::Rect(candidates[i].first.x, candidates[i].first.y,
                              templateImage_.cols, templateImage_.rows);
        matches.push_back(match);
    }
}

/**
 * @brief 直接设置模板图像
 *
 * @param templateImg 模板图像
 * @return 是否设置成功
 */
bool QuickSearchTool::setTemplate(const cv::Mat& templateImg)
{
    if (templateImg.empty()) {
        LOG_WARNING("设置模板失败: 模板图像为空");
        return false;
    }

    // 克隆模板图像以确保独立性
    templateImage_ = templateImg.clone();
    LOG_INFO(QString("模板已设置: %1x%2").arg(templateImage_.cols).arg(templateImage_.rows));
    return true;
}

/**
 * @brief 从图像ROI创建模板
 *
 * @param image 源图像
 * @param roi 模板区域
 * @return 是否创建成功
 */
bool QuickSearchTool::createTemplate(const Base::ImageData::Ptr& image, const QRect& roi)
{
    // 验证图像
    if (!image || image->isEmpty()) {
        LOG_WARNING("创建模板失败: 图像为空");
        return false;
    }

    cv::Mat mat = image->mat();
    // 将Qt矩形转换为OpenCV矩形
    cv::Rect cvRoi(roi.x(), roi.y(), roi.width(), roi.height());

    // 验证ROI是否在图像范围内
    if (cvRoi.x < 0 || cvRoi.y < 0 ||
        cvRoi.x + cvRoi.width > mat.cols ||
        cvRoi.y + cvRoi.height > mat.rows) {
        LOG_WARNING("创建模板失败: ROI超出图像范围");
        return false;
    }

    // 从图像中提取ROI区域并克隆
    templateImage_ = mat(cvRoi).clone();
    LOG_INFO(QString("模板已创建: %1x%2").arg(templateImage_.cols).arg(templateImage_.rows));
    return true;
}

/**
 * @brief 从文件加载模板图像
 *
 * @param filePath 文件路径
 * @return 是否加载成功
 */
bool QuickSearchTool::loadTemplate(const QString& filePath)
{
    // 检查文件是否存在
    if (!QFileInfo::exists(filePath)) {
        LOG_WARNING(QString("加载模板失败: 文件不存在 %1").arg(filePath));
        return false;
    }

    // 使用OpenCV读取图像
    templateImage_ = cv::imread(filePath.toStdString());
    if (templateImage_.empty()) {
        LOG_WARNING(QString("加载模板失败: 无法读取图像 %1").arg(filePath));
        return false;
    }

    LOG_INFO(QString("模板已加载: %1").arg(filePath));
    return true;
}

/**
 * @brief 保存模板图像到文件
 *
 * @param filePath 文件路径
 * @return 是否保存成功
 */
bool QuickSearchTool::saveTemplate(const QString& filePath)
{
    if (!hasTemplate()) {
        LOG_WARNING("保存模板失败: 没有模板");
        return false;
    }

    // 使用OpenCV保存图像
    bool success = cv::imwrite(filePath.toStdString(), templateImage_);
    if (success) {
        LOG_INFO(QString("模板已保存: %1").arg(filePath));
    } else {
        LOG_WARNING(QString("保存模板失败: %1").arg(filePath));
    }
    return success;
}

/**
 * @brief 清除模板
 *
 * 释放模板图像内存，同时清空搜索结果
 */
void QuickSearchTool::clearTemplate()
{
    templateImage_.release();  // 释放Mat内存
    matches_.clear();          // 清空搜索结果
    LOG_INFO("模板已清除");
}

/**
 * @brief 序列化参数到JSON对象
 */
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

/**
 * @brief 从JSON对象反序列化参数
 */
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
