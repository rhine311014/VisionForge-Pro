/**
 * @file ShapeMatchUtils.h
 * @brief ShapeMatchTool的辅助工具类 (包含Halcon依赖)
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/ShapeMatchTool.h"

#ifdef _WIN32
#include <HalconCpp.h>
using namespace HalconCpp;

namespace VisionForge {
namespace Algorithm {

class ShapeMatchUtils {
public:
    /**
     * @brief 生成标准十字形XLD轮廓
     */
    static HXLDCont generateCrossContour(double armLength, double armWidth,
                                         double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成T字形XLD轮廓
     */
    static HXLDCont generateTShapeContour(double width, double height, double thickness,
                                          double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成L字形XLD轮廓
     */
    static HXLDCont generateLShapeContour(double width, double height, double thickness,
                                          double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成圆形XLD轮廓
     */
    static HXLDCont generateCircleContour(double radius,
                                          double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成圆环形XLD轮廓
     */
    static HXLDCont generateRingContour(double outerRadius, double innerRadius,
                                        double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成矩形XLD轮廓
     */
    static HXLDCont generateRectangleContour(double width, double height,
                                             double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成三角形XLD轮廓
     */
    static HXLDCont generateTriangleContour(double sideLength,
                                            double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成菱形XLD轮廓
     */
    static HXLDCont generateDiamondContour(double width, double height,
                                           double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成箭头形XLD轮廓
     */
    static HXLDCont generateArrowContour(double length, double headWidth, double shaftWidth,
                                         double centerRow = 0, double centerCol = 0);

    /**
     * @brief 根据参数生成Mark形状轮廓（统一接口）
     */
    static HXLDCont generateMarkContour(const MarkShapeParams& params);
};

} // namespace Algorithm
} // namespace VisionForge

#endif // _WIN32
