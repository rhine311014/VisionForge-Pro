/**
 * @file TemplateMatchTool.cpp
 * @brief OpenCV模板匹配工具实现文件
 * @details 实现模板匹配算法的核心处理逻辑，包括：
 *          - 标准模板匹配
 *          - 金字塔加速匹配
 *          - 角度搜索匹配
 *          - 非极大值抑制
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025-12-18
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
#define M_PI 3.14159265358979323846  ///< 圆周率常量
#endif

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 构造函数 - 初始化默认参数
 * @param parent 父对象指针
 *
 * @details 默认参数设置：
 *          - 匹配方法：归一化相关系数(CCOEFF_NORMED)
 *          - 匹配阈值：0.7
 *          - 最大匹配数：10
 *          - 金字塔加速：启用，3层
 *          - 角度搜索：禁用
 */
TemplateMatchTool::TemplateMatchTool(QObject* parent)
    : VisionTool(parent)
    , method_(MatchMethod::CCOEFF_NORMED)  // 默认使用归一化相关系数匹配
    , threshold_(0.7)                       // 默认阈值70%
    , maxMatches_(10)                       // 最多返回10个匹配
    , usePyramid_(true)                     // 默认启用金字塔加速
    , pyramidLevels_(3)                     // 3层金字塔
    , enableAngleSearch_(false)             // 默认不进行角度搜索
    , angleMin_(-10.0)                      // 角度范围: -10度
    , angleMax_(10.0)                       // 角度范围: +10度
    , angleStep_(2.0)                       // 角度步长: 2度
    , nmsDistance_(20.0)                    // NMS距离: 20像素
{
    setDisplayName(toolName());
}

/**
 * @brief 执行模板匹配处理
 * @param input 输入图像数据
 * @param output 输出结果
 * @return 匹配成功返回true
 *
 * @par 算法流程:
 * 1. 输入验证 - 检查模板和输入图像有效性
 * 2. 图像预处理 - 转换为灰度图
 * 3. 模板匹配 - 根据配置选择不同策略
 * 4. 后处理 - NMS和结果数量限制
 * 5. 输出生成 - 绘制结果图像
 */
bool TemplateMatchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    // 启动计时器用于性能统计
    QElapsedTimer timer;
    timer.start();

    // 清空上次结果
    results_.clear();

    // ========== 步骤1: 输入验证 ==========
    // 检查模板是否已设置
    if (template_.empty()) {
        output.success = false;
        output.errorMessage = "未设置模板图像";
        setStatusText("错误: 未设置模板");
        return false;
    }

    // 检查输入图像是否有效
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像无效";
        setStatusText("错误: 输入图像无效");
        return false;
    }

    cv::Mat searchImage = input->mat();

    // ========== 步骤2: 图像预处理 - 转换为灰度图 ==========
    // 模板匹配通常在灰度图上进行以提高速度
    cv::Mat grayImage, grayTemplate;

    // 搜索图像灰度转换
    if (searchImage.channels() == 3) {
        cv::cvtColor(searchImage, grayImage, cv::COLOR_BGR2GRAY);
    } else if (searchImage.channels() == 4) {
        cv::cvtColor(searchImage, grayImage, cv::COLOR_BGRA2GRAY);
    } else {
        grayImage = searchImage;  // 已经是灰度图
    }

    // 模板图像灰度转换
    if (template_.channels() == 3) {
        cv::cvtColor(template_, grayTemplate, cv::COLOR_BGR2GRAY);
    } else if (template_.channels() == 4) {
        cv::cvtColor(template_, grayTemplate, cv::COLOR_BGRA2GRAY);
    } else {
        grayTemplate = template_;  // 已经是灰度图
    }

    // ========== 步骤3: 执行模板匹配 ==========
    if (enableAngleSearch_) {
        // 角度搜索模式：在指定角度范围内旋转模板进行多次匹配
        for (double angle = angleMin_; angle <= angleMax_; angle += angleStep_) {
            // 旋转模板
            cv::Mat rotatedTemplate = rotateTemplate(grayTemplate, angle);

            // 根据是否启用金字塔选择匹配方法
            if (usePyramid_) {
                pyramidMatch(grayImage, rotatedTemplate);
            } else {
                performMatch(grayImage, rotatedTemplate, angle);
            }
        }
    } else {
        // 标准匹配模式：不进行角度搜索
        if (usePyramid_) {
            pyramidMatch(grayImage, grayTemplate);  // 金字塔加速匹配
        } else {
            performMatch(grayImage, grayTemplate);  // 标准全图匹配
        }
    }

    // ========== 步骤4: 后处理 ==========
    // 非极大值抑制 - 去除重复检测
    nonMaximumSuppression();

    // 限制结果数量
    if (static_cast<int>(results_.size()) > maxMatches_) {
        results_.resize(maxMatches_);
    }

    // ========== 步骤5: 生成输出 ==========
    output.success = !results_.empty();
    output.executionTime = timer.elapsed();

    // 记录匹配结果数据
    if (!results_.empty()) {
        const auto& best = results_[0];  // 最佳匹配结果
        output.setValue("matchCount", static_cast<int>(results_.size()));
        output.setValue("bestScore", best.score);
        output.setValue("centerX", best.center.x());
        output.setValue("centerY", best.center.y());
        output.setValue("angle", best.angle);
    } else {
        output.setValue("matchCount", 0);
        output.setValue("bestScore", 0.0);
    }

    // 绘制匹配结果到输出图像
    cv::Mat resultImage = drawResults(input->mat());
    output.outputImage = Base::ImageMemoryPool::instance().allocate(
        resultImage.cols, resultImage.rows, resultImage.type());
    if (output.outputImage) {
        resultImage.copyTo(output.outputImage->mat());
    }
    setDebugImage(output.outputImage);

    // 更新状态文本
    QString statusText = QString("找到 %1 个匹配").arg(results_.size());
    if (!results_.empty()) {
        statusText += QString(" (最佳: %.1f%%)").arg(results_[0].score * 100);
    }
    setStatusText(statusText);

    LOG_DEBUG(QString("模板匹配完成: %1个匹配, 耗时: %2ms")
             .arg(results_.size()).arg(output.executionTime));

    // 发送匹配完成信号
    emit matchCompleted(static_cast<int>(results_.size()));
    return output.success;
}

/**
 * @brief 执行单次模板匹配
 * @param image 搜索图像(灰度)
 * @param templ 模板图像(灰度)
 * @param angle 模板旋转角度
 *
 * @par 算法原理:
 * OpenCV的matchTemplate函数通过滑动窗口计算每个位置的相似度，
 * 生成一个(W-w+1) x (H-h+1)的响应图，其中W/H是搜索图尺寸，w/h是模板尺寸
 */
void TemplateMatchTool::performMatch(const cv::Mat& image, const cv::Mat& templ,
                                     double angle)
{
    // 边界检查：模板不能大于搜索图像
    if (templ.cols > image.cols || templ.rows > image.rows) {
        return;
    }

    // 计算结果矩阵尺寸: (图像尺寸 - 模板尺寸 + 1)
    int resultCols = image.cols - templ.cols + 1;
    int resultRows = image.rows - templ.rows + 1;
    cv::Mat result(resultRows, resultCols, CV_32FC1);

    // 获取OpenCV匹配方法常量
    int method = getOpenCVMatchMethod();

    // 调用OpenCV模板匹配函数
    cv::matchTemplate(image, templ, result, method);

    // 对非归一化方法进行归一化处理，使结果在[0,1]范围内
    if (method == cv::TM_SQDIFF || method == cv::TM_CCORR ||
        method == cv::TM_CCOEFF) {
        cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);
    }

    // 判断匹配方法的评分规则：平方差方法值越小越好
    bool isMinBetter = (method == cv::TM_SQDIFF || method == cv::TM_SQDIFF_NORMED);

    // 遍历响应图，提取高于阈值的匹配结果
    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            double score = result.at<float>(y, x);

            // 平方差方法：将分数反转，使其与其他方法一致(值越大越好)
            if (isMinBetter) {
                score = 1.0 - score;
            }

            // 检查是否超过阈值
            if (score >= threshold_) {
                // 创建匹配结果
                MatchResult match;
                match.position = QPointF(x, y);  // 左上角位置
                match.center = QPointF(x + templ.cols / 2.0, y + templ.rows / 2.0);  // 中心位置
                match.score = score;  // 匹配分数
                match.angle = angle;  // 旋转角度
                match.scale = 1.0;    // 缩放比例(固定为1)
                match.boundingRect = QRectF(x, y, templ.cols, templ.rows);  // 外接矩形
                match.valid = true;   // 有效标志
                results_.push_back(match);
            }
        }
    }
}

/**
 * @brief 金字塔加速模板匹配
 * @param image 搜索图像(灰度)
 * @param templ 模板图像(灰度)
 *
 * @par 算法原理:
 * 金字塔匹配通过多尺度策略加速搜索：
 * 1. 构建图像金字塔：逐层缩小图像和模板
 * 2. 粗搜索：在最小尺度层进行全局匹配，快速定位候选区域
 * 3. 精搜索：在原图上对候选区域进行精确匹配
 *
 * @par 性能优势:
 * - 大幅减少全局搜索的计算量
 * - 特别适合大图像上的模板匹配
 * - 层数越多加速效果越明显，但可能降低精度
 */
void TemplateMatchTool::pyramidMatch(const cv::Mat& image, const cv::Mat& templ)
{
    // ========== 步骤1: 构建图像金字塔 ==========
    std::vector<cv::Mat> imagePyramid, templPyramid;
    imagePyramid.push_back(image);   // 第0层是原图
    templPyramid.push_back(templ);

    cv::Mat currentImage = image;
    cv::Mat currentTempl = templ;

    // 逐层下采样构建金字塔
    for (int i = 1; i < pyramidLevels_; ++i) {
        cv::pyrDown(currentImage, currentImage);  // 尺寸减半
        cv::pyrDown(currentTempl, currentTempl);
        imagePyramid.push_back(currentImage);
        templPyramid.push_back(currentTempl);
    }

    // ========== 步骤2: 在金字塔顶层进行粗搜索 ==========
    std::vector<cv::Point> candidates;
    int scaleFactor = 1 << (pyramidLevels_ - 1);  // 缩放因子 = 2^(层数-1)

    // 获取金字塔顶层图像
    cv::Mat topImage = imagePyramid[pyramidLevels_ - 1];
    cv::Mat topTempl = templPyramid[pyramidLevels_ - 1];

    // 边界检查：如果模板在顶层已经太大，直接在原图匹配
    if (topTempl.cols >= topImage.cols || topTempl.rows >= topImage.rows) {
        performMatch(image, templ);
        return;
    }

    // 计算顶层响应图尺寸
    int resultCols = topImage.cols - topTempl.cols + 1;
    int resultRows = topImage.rows - topTempl.rows + 1;
    cv::Mat result(resultRows, resultCols, CV_32FC1);

    // 在顶层执行模板匹配
    int method = getOpenCVMatchMethod();
    cv::matchTemplate(topImage, topTempl, result, method);

    bool isMinBetter = (method == cv::TM_SQDIFF || method == cv::TM_SQDIFF_NORMED);

    // 找到最佳匹配位置
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // 根据匹配方法选择最佳位置
    cv::Point bestLoc = isMinBetter ? minLoc : maxLoc;

    // ========== 步骤3: 生成候选点 ==========
    // 在最佳位置周围扩展搜索范围
    int searchRadius = 2;  // 搜索半径
    for (int dy = -searchRadius; dy <= searchRadius; ++dy) {
        for (int dx = -searchRadius; dx <= searchRadius; ++dx) {
            // 将顶层坐标映射回原图坐标
            int x = (bestLoc.x + dx) * scaleFactor;
            int y = (bestLoc.y + dy) * scaleFactor;

            // 边界检查
            if (x >= 0 && y >= 0 &&
                x + templ.cols <= image.cols &&
                y + templ.rows <= image.rows) {
                candidates.push_back(cv::Point(x, y));
            }
        }
    }

    // ========== 步骤4: 在原图上精确匹配 ==========
    for (const auto& pt : candidates) {
        int x = pt.x;
        int y = pt.y;

        // 定义搜索ROI区域（模板尺寸的2倍）
        int w = std::min(templ.cols * 2, image.cols - x);
        int h = std::min(templ.rows * 2, image.rows - y);

        // 检查ROI是否足够大
        if (w < templ.cols || h < templ.rows) continue;

        // 提取ROI区域
        cv::Rect roi(x, y, w, h);
        cv::Mat roiImage = image(roi);

        // 在ROI上执行精确匹配
        int rCols = roiImage.cols - templ.cols + 1;
        int rRows = roiImage.rows - templ.rows + 1;
        if (rCols <= 0 || rRows <= 0) continue;

        cv::Mat localResult(rRows, rCols, CV_32FC1);
        cv::matchTemplate(roiImage, templ, localResult, method);

        // 找到局部最佳匹配
        cv::minMaxLoc(localResult, &minVal, &maxVal, &minLoc, &maxLoc);

        double score = isMinBetter ? (1.0 - minVal) : maxVal;
        cv::Point loc = isMinBetter ? minLoc : maxLoc;

        // 如果分数超过阈值，添加到结果
        if (score >= threshold_) {
            MatchResult match;
            match.position = QPointF(x + loc.x, y + loc.y);  // 原图坐标
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

/**
 * @brief 非极大值抑制
 * @details 去除重复检测，避免同一目标被多次报告
 *
 * @par 算法步骤:
 * 1. 按匹配分数降序排序
 * 2. 从最高分开始，保留该结果
 * 3. 抑制距离该结果小于nmsDistance_的其他结果
 * 4. 重复直到处理完所有结果
 */
void TemplateMatchTool::nonMaximumSuppression()
{
    // 单个结果无需NMS处理
    if (results_.size() <= 1) return;

    // 步骤1: 按分数降序排序
    std::sort(results_.begin(), results_.end(),
             [](const MatchResult& a, const MatchResult& b) {
                 return a.score > b.score;  // 分数高的在前
             });

    std::vector<MatchResult> filtered;  // 保留的结果
    std::vector<bool> suppressed(results_.size(), false);  // 抑制标记

    // 步骤2-4: 迭代抑制
    for (size_t i = 0; i < results_.size(); ++i) {
        // 跳过已被抑制的结果
        if (suppressed[i]) continue;

        // 保留当前结果
        filtered.push_back(results_[i]);

        // 抑制距离过近的其他匹配
        for (size_t j = i + 1; j < results_.size(); ++j) {
            if (suppressed[j]) continue;

            // 计算两个结果中心点之间的欧氏距离
            double dx = results_[i].center.x() - results_[j].center.x();
            double dy = results_[i].center.y() - results_[j].center.y();
            double dist = std::sqrt(dx * dx + dy * dy);

            // 如果距离小于NMS阈值，抑制分数较低的结果
            if (dist < nmsDistance_) {
                suppressed[j] = true;
            }
        }
    }

    // 更新结果列表
    results_ = filtered;
}

/**
 * @brief 旋转模板图像
 * @param templ 原始模板
 * @param angle 旋转角度(度)，正值为逆时针
 * @return 旋转后的模板图像
 *
 * @par 算法说明:
 * 1. 计算旋转矩阵
 * 2. 扩展输出尺寸以容纳旋转后的完整图像
 * 3. 调整旋转矩阵的平移分量
 * 4. 执行仿射变换
 */
cv::Mat TemplateMatchTool::rotateTemplate(const cv::Mat& templ, double angle)
{
    // 角度接近0时直接返回原图
    if (std::abs(angle) < 0.001) {
        return templ;
    }

    // 计算模板中心点
    cv::Point2f center(templ.cols / 2.0f, templ.rows / 2.0f);

    // 获取旋转矩阵 (2x3仿射变换矩阵)
    cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, 1.0);

    // 计算旋转后的边界框大小
    // 旋转后的尺寸需要能够完全容纳旋转后的图像
    double abscos = std::abs(rotMat.at<double>(0, 0));  // |cos(angle)|
    double abssin = std::abs(rotMat.at<double>(0, 1));  // |sin(angle)|
    int newW = static_cast<int>(templ.rows * abssin + templ.cols * abscos);
    int newH = static_cast<int>(templ.rows * abscos + templ.cols * abssin);

    // 调整旋转矩阵的平移分量，使旋转后的图像居中
    rotMat.at<double>(0, 2) += (newW - templ.cols) / 2.0;
    rotMat.at<double>(1, 2) += (newH - templ.rows) / 2.0;

    // 执行仿射变换
    cv::Mat rotated;
    cv::warpAffine(templ, rotated, rotMat, cv::Size(newW, newH));

    return rotated;
}

/**
 * @brief 绘制匹配结果到图像
 * @param input 原始输入图像
 * @return 带有匹配标注的结果图像
 *
 * @par 绘制内容:
 * - 匹配区域矩形（最佳匹配绿色，其他橙色）
 * - 中心点和十字标记
 * - 序号和匹配分数
 * - 总匹配数量
 */
cv::Mat TemplateMatchTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    // 确保输出是彩色图像
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 遍历绘制所有匹配结果
    for (size_t i = 0; i < results_.size(); ++i) {
        const auto& match = results_[i];

        // 获取匹配区域矩形
        cv::Rect rect(
            static_cast<int>(match.boundingRect.x()),
            static_cast<int>(match.boundingRect.y()),
            static_cast<int>(match.boundingRect.width()),
            static_cast<int>(match.boundingRect.height())
        );

        // 颜色设置：最佳匹配绿色，其他橙色
        cv::Scalar color = (i == 0) ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 200, 255);

        // 绘制匹配区域矩形
        cv::rectangle(result, rect, color, 2);

        // 绘制中心点（红色实心圆）
        cv::Point center(static_cast<int>(match.center.x()),
                        static_cast<int>(match.center.y()));
        cv::circle(result, center, 5, cv::Scalar(0, 0, 255), -1);

        // 绘制十字标记
        cv::line(result, cv::Point(center.x - 10, center.y),
                cv::Point(center.x + 10, center.y), cv::Scalar(0, 0, 255), 1);
        cv::line(result, cv::Point(center.x, center.y - 10),
                cv::Point(center.x, center.y + 10), cv::Scalar(0, 0, 255), 1);

        // 绘制序号和分数文本
        QString text = QString("#%1: %.1f%%").arg(i + 1).arg(match.score * 100);
        cv::putText(result, text.toStdString(),
                   cv::Point(rect.x, rect.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }

    // 在图像左上角显示匹配总数
    QString countText = QString("Matches: %1").arg(results_.size());
    cv::putText(result, countText.toStdString(),
               cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
               cv::Scalar(255, 255, 0), 2);

    return result;
}

/**
 * @brief 将MatchMethod枚举转换为OpenCV匹配方法常量
 * @return OpenCV的cv::TemplateMatchModes枚举值
 */
int TemplateMatchTool::getOpenCVMatchMethod() const
{
    switch (method_) {
    case MatchMethod::SQDIFF:        return cv::TM_SQDIFF;         // 平方差
    case MatchMethod::SQDIFF_NORMED: return cv::TM_SQDIFF_NORMED;  // 归一化平方差
    case MatchMethod::CCORR:         return cv::TM_CCORR;          // 相关
    case MatchMethod::CCORR_NORMED:  return cv::TM_CCORR_NORMED;   // 归一化相关
    case MatchMethod::CCOEFF:        return cv::TM_CCOEFF;         // 相关系数
    case MatchMethod::CCOEFF_NORMED: return cv::TM_CCOEFF_NORMED;  // 归一化相关系数
    default:                         return cv::TM_CCOEFF_NORMED;  // 默认
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
