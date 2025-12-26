/**
 * @file ShapeMatchImpl.h
 * @brief ShapeMatchTool的私有实现类
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/ShapeMatchTool.h"

#ifdef _WIN32
#include <HalconCpp.h>
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

class ShapeMatchImpl {
public:
    ShapeMatchImpl();
    ~ShapeMatchImpl();

    void clearModel();
    bool loadModel(const QString& modelPath);
    bool saveModel(const QString& modelPath);
    bool trainModel(const Base::ImageData::Ptr& image, int row1, int col1, int row2, int col2);
    
#ifdef _WIN32
    bool trainModelFromContour(const HXLDCont& contour,
                               const Base::ImageData::Ptr& highNoiseImage,
                               int sampleRow1, int sampleCol1,
                               int sampleRow2, int sampleCol2);
    
    // 处理函数
    bool process(const Base::ImageData::Ptr& input, ToolResult& output, ShapeMatchTool* parent);
    
    // 辅助函数
    HImage imageDataToHImage(const Base::ImageData::Ptr& image);
    void drawMatchResults(cv::Mat& image, const HTuple& row, const HTuple& col, const HTuple& angle, const HTuple& score);
    HXLDCont createMatchContours(const HTuple& row, const HTuple& col, const HTuple& angle, ShapeMatchTool* parent);
    HXLDCont filterContours(const HXLDCont& contours) const;

    // 成员变量
    HTuple shapeModel_;         // Halcon形状模型句柄
    HXLDCont standardContour_;  // 标准轮廓（用于显示和从轮廓训练）
    HXLDCont lastMatchContours_; // 最后一次匹配的轮廓结果（从Generic模型直接获取）
    bool useGenericModel_;      // 是否使用通用形状模型（从XLD轮廓创建）
#endif

    bool modelLoaded_;          // 模板是否已加载
    QString modelPath_;         // 模板文件路径

    // 模板信息
    int modelWidth_;            // 模板宽度
    int modelHeight_;           // 模板高度
    double modelRefRow_;        // 模板参考点行
    double modelRefCol_;        // 模板参考点列
};

} // namespace Algorithm
} // namespace VisionForge
