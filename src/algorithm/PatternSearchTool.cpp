/**
 * @file PatternSearchTool.cpp
 * @brief 图案搜索工具实现文件
 *
 * @details
 * 本文件实现了PatternSearchTool类的所有方法，包括：
 * - 模板创建、加载、保存和清除
 * - 基于Halcon的形状匹配搜索
 * - 参数序列化和反序列化
 *
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

/**
 * @brief 构造函数实现
 *
 * 初始化工具的显示名称，其他成员变量使用类内初始化的默认值
 */
PatternSearchTool::PatternSearchTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(tr("图案搜索"));
}

/**
 * @brief 析构函数实现
 *
 * 调用clearTemplate()确保释放所有Halcon资源
 */
PatternSearchTool::~PatternSearchTool()
{
    clearTemplate();
}

/**
 * @brief 执行图案搜索的主入口方法
 *
 * 处理流程：
 * 1. 验证输入图像和模板的有效性
 * 2. 启动计时器记录执行时间
 * 3. 根据编译配置选择Halcon或OpenCV实现
 * 4. 整理输出结果并设置状态文本
 */
bool PatternSearchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    // 步骤1：输入验证 - 检查图像是否有效
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = tr("输入图像为空");
        return false;
    }

    // 步骤2：模板验证 - 必须先创建模板才能搜索
    if (!hasTemplate_) {
        output.success = false;
        output.errorMessage = tr("未创建图案模板");
        return false;
    }

    // 步骤3：启动计时器，用于统计执行耗时
    QElapsedTimer timer;
    timer.start();

    // 步骤4：根据编译配置选择处理方法
#ifdef USE_HALCON
    // 优先使用Halcon实现，提供更好的匹配效果
    bool success = processHalcon(input, output);
#else
    // Halcon不可用时使用OpenCV备用实现
    bool success = processOpenCV(input, output);
#endif

    // 步骤5：记录执行时间
    output.executionTime = timer.elapsed();

    // 步骤6：处理成功时整理输出结果
    if (success) {
        output.success = true;
        // 设置匹配数量
        output.setValue("matchCount", static_cast<int>(matches_.size()));

        // 将所有匹配结果转换为JSON数组格式，方便上层使用
        QJsonArray matchArray;
        for (const auto& match : matches_) {
            QJsonObject matchObj;
            matchObj["x"] = match.col;                        // X坐标（列）
            matchObj["y"] = match.row;                        // Y坐标（行）
            matchObj["angle"] = match.angle * 180.0 / M_PI;   // 角度从弧度转换为度
            matchObj["scale"] = match.scale;                   // 缩放比例
            matchObj["score"] = match.score;                   // 匹配分数
            matchArray.append(matchObj);
        }
        output.data["matches"] = matchArray;

        // 更新状态文本显示
        setStatusText(tr("找到 %1 个匹配，耗时 %2 ms")
            .arg(matches_.size()).arg(output.executionTime));
    }

    return success;
}

/**
 * @brief 使用Halcon执行形状匹配搜索
 *
 * 这是核心的搜索实现，使用Halcon的形状匹配算法。
 *
 * 算法流程：
 * 1. 将输入图像从OpenCV格式转换为Halcon格式
 * 2. 如果是彩色图像，转换为灰度图
 * 3. 根据是否启用缩放搜索，选择不同的匹配函数
 * 4. 收集并返回所有匹配结果
 */
bool PatternSearchTool::processHalcon(const Base::ImageData::Ptr& input, ToolResult& output)
{
#ifdef USE_HALCON
    try {
        // 步骤1：获取输入图像的OpenCV Mat对象
        cv::Mat mat = input->mat();
        cv::Mat grayMat;

        // 步骤2：转换为灰度图（Halcon形状匹配需要灰度图像）
        if (mat.channels() == 3) {
            cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
        } else {
            grayMat = mat;
        }

        // 步骤3：创建Halcon图像对象
        // GenImage1用于从内存数据创建单通道图像
        // 参数：像素类型、宽度、高度、数据指针
        HImage hImage;
        hImage.GenImage1("byte", grayMat.cols, grayMat.rows,
                         reinterpret_cast<void*>(grayMat.data));

        // 步骤4：执行形状匹配
        // 准备存储结果的Tuple变量
        HTuple rows, cols, angles, scales, scores;

        // 步骤5：根据是否启用缩放选择不同的匹配函数
        if (scaleSearchEnabled_) {
            // 使用支持缩放的形状匹配函数
            // FindScaledShapeModel: 在指定角度和缩放范围内搜索形状模型
            shapeModel_.FindScaledShapeModel(
                hImage,                                     // 搜索图像
                angleMin_ * M_PI / 180.0,                   // 起始角度（弧度）
                (angleMax_ - angleMin_) * M_PI / 180.0,     // 角度范围（弧度）
                scaleMin_,                                  // 最小缩放
                scaleMax_,                                  // 最大缩放
                minScore_,                                  // 最小匹配分数
                maxMatches_,                                // 最大匹配数
                overlapThreshold_,                          // 重叠阈值
                "least_squares",                            // 亚像素精度方法
                pyramidLevels_,                             // 金字塔起始层
                0.9,                                        // 贪婪度参数
                &rows, &cols, &angles, &scales, &scores);   // 输出参数
        } else {
            // 使用不支持缩放的形状匹配函数（更快）
            scales = HTuple(1.0);  // 缩放固定为1.0
            shapeModel_.FindShapeModel(
                hImage,                                     // 搜索图像
                angleMin_ * M_PI / 180.0,                   // 起始角度（弧度）
                (angleMax_ - angleMin_) * M_PI / 180.0,     // 角度范围（弧度）
                minScore_,                                  // 最小匹配分数
                maxMatches_,                                // 最大匹配数
                overlapThreshold_,                          // 重叠阈值
                "least_squares",                            // 亚像素精度方法
                pyramidLevels_,                             // 金字塔起始层
                0.9,                                        // 贪婪度参数
                &rows, &cols, &angles, &scores);            // 输出参数
        }

        // 步骤6：收集匹配结果到matches_向量
        matches_.clear();
        int numMatches = scores.Length();  // 获取匹配数量
        for (int i = 0; i < numMatches; ++i) {
            PatternMatch match;
            match.row = rows[i].D();    // 获取行坐标（D()返回double值）
            match.col = cols[i].D();    // 获取列坐标
            match.angle = angles[i].D(); // 获取角度（弧度）
            match.scale = scaleSearchEnabled_ ? scales[i].D() : 1.0;  // 获取缩放
            match.score = scores[i].D(); // 获取匹配分数
            matches_.push_back(match);
        }

        LOG_INFO(QString("Halcon形状匹配完成，找到 %1 个结果").arg(numMatches));
        return true;

    } catch (HException& e) {
        // 捕获Halcon异常并记录错误信息
        output.errorMessage = QString("Halcon错误: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    }
#else
    // Halcon未启用时的占位代码
    Q_UNUSED(input);
    Q_UNUSED(output);
    return false;
#endif
}

/**
 * @brief 使用OpenCV执行模板匹配（备用方案）
 *
 * 当Halcon不可用时的备用实现。
 * 目前该方法未实现完整功能，仅返回错误提示。
 *
 * @note OpenCV的模板匹配不支持旋转和缩放不变性，
 *       功能受限，建议使用Halcon版本
 */
bool PatternSearchTool::processOpenCV(const Base::ImageData::Ptr& input, ToolResult& output)
{
    // OpenCV备用实现（简化版模板匹配）
    // 由于OpenCV原生模板匹配不支持旋转/缩放不变性，
    // 此处仅提供基本功能或返回错误
    Q_UNUSED(input);
    output.success = false;
    output.errorMessage = tr("图案搜索需要Halcon支持");
    LOG_WARNING("PatternSearchTool: OpenCV fallback not implemented");
    return false;
}

/**
 * @brief 从图像ROI创建形状模板
 *
 * 创建流程：
 * 1. 验证输入图像有效性
 * 2. 转换图像格式（OpenCV -> Halcon）
 * 3. 裁剪ROI区域
 * 4. 调用Halcon创建形状模型
 */
bool PatternSearchTool::createTemplate(const Base::ImageData::Ptr& image, const QRectF& roi)
{
    // 步骤1：验证输入图像
    if (!image || image->isEmpty()) {
        LOG_ERROR("创建模板失败: 图像为空");
        return false;
    }

#ifdef USE_HALCON
    try {
        // 步骤2：转换图像格式
        cv::Mat mat = image->mat();
        cv::Mat grayMat;
        if (mat.channels() == 3) {
            cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
        } else {
            grayMat = mat;
        }

        // 创建Halcon图像对象
        HImage hImage;
        hImage.GenImage1("byte", grayMat.cols, grayMat.rows,
                         reinterpret_cast<void*>(grayMat.data));

        // 步骤3：裁剪ROI区域
        // CropRectangle1参数顺序：Row1, Column1, Row2, Column2
        HImage roiImage = hImage.CropRectangle1(
            static_cast<Hlong>(roi.top()),      // 上边界（行）
            static_cast<Hlong>(roi.left()),     // 左边界（列）
            static_cast<Hlong>(roi.bottom()),   // 下边界（行）
            static_cast<Hlong>(roi.right()));   // 右边界（列）

        // 保存模板图像用于显示
        templateImage_ = roiImage;

        // 步骤4：创建形状模型
        HTuple modelID;
        if (scaleSearchEnabled_) {
            // 创建支持缩放的形状模型
            // CreateScaledShapeModel: 创建可缩放的形状模型
            shapeModel_ = roiImage.CreateScaledShapeModel(
                pyramidLevels_,                             // 金字塔层数
                angleMin_ * M_PI / 180.0,                   // 起始角度
                (angleMax_ - angleMin_) * M_PI / 180.0,     // 角度范围
                "auto",                                     // 角度步长（自动）
                scaleMin_,                                  // 最小缩放
                scaleMax_,                                  // 最大缩放
                "auto",                                     // 缩放步长（自动）
                "auto",                                     // 优化方法
                "use_polarity",                             // 使用极性（区分正负边缘）
                "auto",                                     // 对比度阈值
                "auto");                                    // 最小对比度
        } else {
            // 创建标准形状模型（不支持缩放，更快）
            shapeModel_ = roiImage.CreateShapeModel(
                pyramidLevels_,                             // 金字塔层数
                angleMin_ * M_PI / 180.0,                   // 起始角度
                (angleMax_ - angleMin_) * M_PI / 180.0,     // 角度范围
                "auto",                                     // 角度步长
                "auto",                                     // 优化方法
                "use_polarity",                             // 使用极性
                "auto",                                     // 对比度阈值
                "auto");                                    // 最小对比度
        }

        // 标记模板创建成功
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

/**
 * @brief 从文件加载形状模板
 *
 * 加载之前使用saveTemplate()保存的Halcon形状模型文件
 */
bool PatternSearchTool::loadTemplate(const QString& filePath)
{
    // 首先检查文件是否存在
    if (!QFileInfo::exists(filePath)) {
        LOG_ERROR(QString("模板文件不存在: %1").arg(filePath));
        return false;
    }

#ifdef USE_HALCON
    try {
        // 使用Halcon的ReadShapeModel读取形状模型文件
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

/**
 * @brief 保存形状模板到文件
 *
 * 将当前模板保存为Halcon形状模型文件(.shm格式)
 */
bool PatternSearchTool::saveTemplate(const QString& filePath)
{
    // 检查是否有模板可保存
    if (!hasTemplate_) {
        LOG_ERROR("保存模板失败: 未创建模板");
        return false;
    }

#ifdef USE_HALCON
    try {
        // 使用Halcon的WriteShapeModel保存形状模型
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

/**
 * @brief 清除当前模板
 *
 * 释放Halcon形状模型资源，清空匹配结果
 */
void PatternSearchTool::clearTemplate()
{
#ifdef USE_HALCON
    if (hasTemplate_) {
        try {
            // 清除Halcon形状模型，释放内存
            shapeModel_.ClearShapeModel();
        } catch (...) {
            // 忽略清除时可能发生的错误（如模型已无效）
        }
    }
#endif
    hasTemplate_ = false;
    matches_.clear();  // 同时清空匹配结果
}

/**
 * @brief 设置角度搜索范围
 *
 * @param min 最小角度（度）
 * @param max 最大角度（度）
 * @param step 角度步长（度），最小值0.01
 */
void PatternSearchTool::setAngleRange(double min, double max, double step)
{
    angleMin_ = min;
    angleMax_ = max;
    angleStep_ = qMax(0.01, step);  // 确保步长不小于0.01度
}

/**
 * @brief 设置缩放搜索范围
 *
 * @param min 最小缩放比例，最小值0.1
 * @param max 最大缩放比例，必须大于min
 * @param step 缩放步长，最小值0.01
 */
void PatternSearchTool::setScaleRange(double min, double max, double step)
{
    scaleMin_ = qMax(0.1, min);           // 确保缩放比例不小于0.1
    scaleMax_ = qMax(scaleMin_, max);      // 确保最大值不小于最小值
    scaleStep_ = qMax(0.01, step);         // 确保步长不小于0.01
}

/**
 * @brief 获取最佳匹配结果
 *
 * 从所有匹配结果中找出分数最高的一个。
 * 使用std::max_element算法进行查找。
 *
 * @return 分数最高的匹配结果，如果没有结果则返回默认初始化的PatternMatch
 */
PatternMatch PatternSearchTool::bestMatch() const
{
    // 如果没有匹配结果，返回默认值
    if (matches_.empty()) {
        return PatternMatch();
    }

    // 使用max_element查找分数最高的匹配
    // Lambda表达式定义比较规则：按score字段升序比较
    auto it = std::max_element(matches_.begin(), matches_.end(),
        [](const PatternMatch& a, const PatternMatch& b) {
            return a.score < b.score;
        });

    return *it;
}

/**
 * @brief 序列化参数到JSON对象
 *
 * 将所有可配置参数打包为JSON格式，用于保存配置到文件或传输
 */
QJsonObject PatternSearchTool::serializeParams() const
{
    QJsonObject json;

    // 基本匹配参数
    json["matchMode"] = static_cast<int>(matchMode_);
    json["minScore"] = minScore_;
    json["maxMatches"] = maxMatches_;
    json["overlapThreshold"] = overlapThreshold_;
    json["subPixelMode"] = static_cast<int>(subPixelMode_);
    json["pyramidLevels"] = pyramidLevels_;

    // 角度搜索参数
    json["angleSearchEnabled"] = angleSearchEnabled_;
    json["angleMin"] = angleMin_;
    json["angleMax"] = angleMax_;
    json["angleStep"] = angleStep_;

    // 缩放搜索参数
    json["scaleSearchEnabled"] = scaleSearchEnabled_;
    json["scaleMin"] = scaleMin_;
    json["scaleMax"] = scaleMax_;
    json["scaleStep"] = scaleStep_;

    return json;
}

/**
 * @brief 从JSON对象反序列化参数
 *
 * 从JSON对象恢复所有参数设置，带有默认值处理
 */
void PatternSearchTool::deserializeParams(const QJsonObject& json)
{
    // 基本匹配参数（带默认值）
    matchMode_ = static_cast<MatchMode>(json.value("matchMode").toInt(EdgeMatch));
    minScore_ = json.value("minScore").toDouble(0.5);
    maxMatches_ = json.value("maxMatches").toInt(10);
    overlapThreshold_ = json.value("overlapThreshold").toDouble(0.5);
    subPixelMode_ = static_cast<SubPixelMode>(json.value("subPixelMode").toInt(LeastSquares));
    pyramidLevels_ = json.value("pyramidLevels").toInt(4);

    // 角度搜索参数
    angleSearchEnabled_ = json.value("angleSearchEnabled").toBool(true);
    angleMin_ = json.value("angleMin").toDouble(-30.0);
    angleMax_ = json.value("angleMax").toDouble(30.0);
    angleStep_ = json.value("angleStep").toDouble(1.0);

    // 缩放搜索参数
    scaleSearchEnabled_ = json.value("scaleSearchEnabled").toBool(false);
    scaleMin_ = json.value("scaleMin").toDouble(0.8);
    scaleMax_ = json.value("scaleMax").toDouble(1.2);
    scaleStep_ = json.value("scaleStep").toDouble(0.05);
}

} // namespace Algorithm
} // namespace VisionForge
