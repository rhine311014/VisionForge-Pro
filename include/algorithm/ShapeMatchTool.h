/**
 * @file ShapeMatchTool.h
 * @brief Halcon形状匹配工具
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/VisionTool.h"

#ifdef _WIN32
#include "HalconCpp.h"
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

/**
 * @class ShapeMatchTool
 * @brief 基于Halcon的形状匹配工具
 *
 * 功能:
 * - 加载或创建形状模板
 * - 在图像中查找匹配的形状
 * - 返回匹配位置、角度、得分等信息
 */
class ShapeMatchTool : public VisionTool
{
    Q_OBJECT

public:
    explicit ShapeMatchTool(QObject* parent = nullptr);
    ~ShapeMatchTool() override;

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return Match; }
    QString toolName() const override { return "形状匹配"; }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 形状匹配特有接口 ==========

    /**
     * @brief 从文件加载模板
     * @param modelPath 模板文件路径(.shm)
     * @return 是否成功
     */
    bool loadModel(const QString& modelPath);

    /**
     * @brief 从图像区域训练模板
     * @param image 训练图像
     * @param row1 ROI左上角行
     * @param col1 ROI左上角列
     * @param row2 ROI右下角行
     * @param col2 ROI右下角列
     * @return 是否成功
     */
    bool trainModel(const Base::ImageData::Ptr& image,
                   int row1, int col1, int row2, int col2);

    /**
     * @brief 保存当前模板
     * @param modelPath 保存路径(.shm)
     * @return 是否成功
     */
    bool saveModel(const QString& modelPath);

    /**
     * @brief 清除模板
     */
    void clearModel();

    /**
     * @brief 检查是否已加载模板
     */
    bool hasModel() const { return modelLoaded_; }

    // ========== 参数设置 ==========

    void setMinScore(double score) { minScore_ = score; emit paramChanged(); }
    double getMinScore() const { return minScore_; }

    void setNumMatches(int num) { numMatches_ = num; emit paramChanged(); }
    int getNumMatches() const { return numMatches_; }

    void setAngleStart(double angle) { angleStart_ = angle; emit paramChanged(); }
    double getAngleStart() const { return angleStart_; }

    void setAngleExtent(double angle) { angleExtent_ = angle; emit paramChanged(); }
    double getAngleExtent() const { return angleExtent_; }

    void setMinContrast(int contrast) { minContrast_ = contrast; emit paramChanged(); }
    int getMinContrast() const { return minContrast_; }

    void setScaleMin(double scale) { scaleMin_ = scale; emit paramChanged(); }
    double getScaleMin() const { return scaleMin_; }

    void setScaleMax(double scale) { scaleMax_ = scale; emit paramChanged(); }
    double getScaleMax() const { return scaleMax_; }

    void setModelPath(const QString& path) { modelPath_ = path; }
    QString getModelPath() const { return modelPath_; }

private:
#ifdef _WIN32
    /**
     * @brief ImageData转换为HImage
     */
    HImage imageDataToHImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 绘制匹配结果到图像
     */
    void drawMatchResults(cv::Mat& image,
                         const HTuple& row, const HTuple& col,
                         const HTuple& angle, const HTuple& score);
#endif

private:
#ifdef _WIN32
    HTuple shapeModel_;         // Halcon形状模型句柄
#endif
    bool modelLoaded_;          // 模板是否已加载
    QString modelPath_;         // 模板文件路径

    // 匹配参数
    double minScore_;           // 最小匹配分数 (0.0-1.0)
    int numMatches_;            // 查找匹配数量 (0=全部)
    double angleStart_;         // 起始角度 (弧度)
    double angleExtent_;        // 角度范围 (弧度)
    int minContrast_;           // 最小对比度
    double scaleMin_;           // 最小缩放
    double scaleMax_;           // 最大缩放

    // 模板信息
    int modelWidth_;            // 模板宽度
    int modelHeight_;           // 模板高度
    double modelRefRow_;        // 模板参考点行
    double modelRefCol_;        // 模板参考点列
};

} // namespace Algorithm
} // namespace VisionForge
