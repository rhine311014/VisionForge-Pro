/**
 * @file ShapeMatchUtils.cpp
 * @brief ShapeMatchUtils实现
 * @author VisionForge Team
 * @date 2025-12-25
 */

#include "algorithm/ShapeMatchUtils.h"
#include "base/Logger.h"

#ifdef _WIN32

namespace VisionForge {
namespace Algorithm {

HXLDCont ShapeMatchUtils::generateCrossContour(double armLength, double armWidth,
                                             double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        double halfLen = armLength / 2.0;
        double halfWid = armWidth / 2.0;

        // 水平矩形
        HTuple r1, c1, r2, c2;
        r1 = centerRow - halfWid;
        c1 = centerCol - halfLen;
        r2 = centerRow + halfWid;
        c2 = centerCol + halfLen;
        
        HXLDCont hRect;
        hRect.GenRectangle2ContourXld(centerRow, centerCol, 0, halfLen, halfWid);

        // 垂直矩形
        HXLDCont vRect;
        vRect.GenRectangle2ContourXld(centerRow, centerCol, 0, halfWid, halfLen);

        contour = hRect.ConcatObj(vRect);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成十字轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateTShapeContour(double width, double height, double thickness,
                                              double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        // 顶部水平部分
        HXLDCont topRect;
        topRect.GenRectangle2ContourXld(centerRow - height/2.0 + thickness/2.0, centerCol, 
                                      0, width/2.0, thickness/2.0);
        
        // 垂直部分
        HXLDCont vRect;
        vRect.GenRectangle2ContourXld(centerRow, centerCol, 
                                    0, thickness/2.0, height/2.0 - thickness); // 稍微缩短避免重叠过多

        contour = topRect.ConcatObj(vRect);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成T字轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateLShapeContour(double width, double height, double thickness,
                                              double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        // 垂直部分（左侧）
        HXLDCont vRect;
        vRect.GenRectangle2ContourXld(centerRow, centerCol - width/2.0 + thickness/2.0, 
                                    0, thickness/2.0, height/2.0);
        
        // 水平部分（底部）
        HXLDCont hRect;
        hRect.GenRectangle2ContourXld(centerRow + height/2.0 - thickness/2.0, centerCol, 
                                    0, width/2.0, thickness/2.0);

        contour = vRect.ConcatObj(hRect);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成L字轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateCircleContour(double radius,
                                              double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        contour.GenCircleContourXld(centerRow, centerCol, radius, 0, 6.28318, "positive", 1.0);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成圆形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateRingContour(double outerRadius, double innerRadius,
                                            double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        HXLDCont outer;
        outer.GenCircleContourXld(centerRow, centerCol, outerRadius, 0, 6.28318, "positive", 1.0);
        
        HXLDCont inner;
        inner.GenCircleContourXld(centerRow, centerCol, innerRadius, 0, 6.28318, "positive", 1.0);
        
        contour = outer.ConcatObj(inner);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成圆环轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateRectangleContour(double width, double height,
                                                 double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        contour.GenRectangle2ContourXld(centerRow, centerCol, 0, width/2.0, height/2.0);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成矩形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateTriangleContour(double sideLength,
                                                double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        // 等边三角形
        double h = sideLength * sqrt(3.0) / 2.0;
        // 顶点 (相对于中心)
        // 上顶点
        double r1 = -h * 2.0 / 3.0;
        double c1 = 0;
        // 左下
        double r2 = h / 3.0;
        double c2 = -sideLength / 2.0;
        // 右下
        double r3 = h / 3.0;
        double c3 = sideLength / 2.0;
        
        HTuple rows, cols;
        rows.Append(centerRow + r1); cols.Append(centerCol + c1);
        rows.Append(centerRow + r2); cols.Append(centerCol + c2);
        rows.Append(centerRow + r3); cols.Append(centerCol + c3);
        rows.Append(centerRow + r1); cols.Append(centerCol + c1); // 闭合
        
        contour.GenContourPolygonXld(rows, cols);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成三角形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateDiamondContour(double width, double height,
                                               double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        HTuple rows, cols;
        rows.Append(centerRow - height/2.0); cols.Append(centerCol);
        rows.Append(centerRow);              cols.Append(centerCol + width/2.0);
        rows.Append(centerRow + height/2.0); cols.Append(centerCol);
        rows.Append(centerRow);              cols.Append(centerCol - width/2.0);
        rows.Append(centerRow - height/2.0); cols.Append(centerCol); // 闭合
        
        contour.GenContourPolygonXld(rows, cols);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成菱形轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateArrowContour(double length, double headWidth, double shaftWidth,
                                             double centerRow, double centerCol)
{
    HXLDCont contour;
    try {
        // 简单箭头：直线+三角形
        // 杆
        HTuple r1, c1, r2, c2;
        r1 = centerRow; c1 = centerCol - length/2.0;
        r2 = centerRow; c2 = centerCol + length/2.0;
        
        HXLDCont shaft;
        // 使用矩形代替直线以获得宽度
        shaft.GenRectangle2ContourXld(centerRow, centerCol, 0, length/2.0, shaftWidth/2.0);
        
        // 头
        double headLen = headWidth; // 假设头长等于头宽
        double tipCol = centerCol + length/2.0 + headLen/2.0;
        
        // 这里的计算稍微简化，实际可能需要更精确的坐标计算
        // 暂用矩形+三角形组合
        
        // 重新实现：直接画多边形
        HTuple rows, cols;
        // 尾部左上
        rows.Append(centerRow - shaftWidth/2.0); cols.Append(centerCol - length/2.0);
        // 头部连接处上
        rows.Append(centerRow - shaftWidth/2.0); cols.Append(centerCol + length/2.0 - headLen);
        // 箭头翼上
        rows.Append(centerRow - headWidth/2.0);  cols.Append(centerCol + length/2.0 - headLen);
        // 箭头尖
        rows.Append(centerRow);                  cols.Append(centerCol + length/2.0);
        // 箭头翼下
        rows.Append(centerRow + headWidth/2.0);  cols.Append(centerCol + length/2.0 - headLen);
        // 头部连接处下
        rows.Append(centerRow + shaftWidth/2.0); cols.Append(centerCol + length/2.0 - headLen);
        // 尾部左下
        rows.Append(centerRow + shaftWidth/2.0); cols.Append(centerCol - length/2.0);
        // 闭合
        rows.Append(centerRow - shaftWidth/2.0); cols.Append(centerCol - length/2.0);
        
        contour.GenContourPolygonXld(rows, cols);
    }
    catch (const HException& e) {
        LOG_ERROR(QString("生成箭头轮廓失败: %1").arg(e.ErrorMessage().Text()));
    }
    return contour;
}

HXLDCont ShapeMatchUtils::generateMarkContour(const MarkShapeParams& params)
{
    HXLDCont contour;
    
    switch (params.type) {
    case MarkShapeType::Cross:
        contour = generateCrossContour(params.size1, params.size2, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::TShape:
        contour = generateTShapeContour(params.size1, params.size1, params.size2, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::LShape:
        contour = generateLShapeContour(params.size1, params.size1, params.size2, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::Circle:
        contour = generateCircleContour(params.size1/2.0, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::Ring:
        contour = generateRingContour(params.size1/2.0, params.size2/2.0, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::Rectangle:
        contour = generateRectangleContour(params.size1, params.size2, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::Triangle:
        contour = generateTriangleContour(params.size1, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::Diamond:
        contour = generateDiamondContour(params.size1, params.size2, params.centerRow, params.centerCol);
        break;
    case MarkShapeType::Arrow:
        contour = generateArrowContour(params.size1, params.size2, params.size3 > 0 ? params.size3 : params.size2/3.0, 
                                     params.centerRow, params.centerCol);
        break;
    default:
        break;
    }
    
    // 应用旋转
    if (params.rotation != 0.0 && contour.IsInitialized()) {
        try {
            HTuple homMat2D;
            HomMat2dIdentity(&homMat2D);
            HomMat2dRotate(homMat2D, params.rotation, params.centerRow, params.centerCol, &homMat2D);
            HXLDCont rotated;
            AffineTransContourXld(contour, &rotated, homMat2D);
            contour = rotated;
        }
        catch (const HException& e) {
            LOG_ERROR(QString("旋转轮廓失败: %1").arg(e.ErrorMessage().Text()));
        }
    }
    
    return contour;
}

} // namespace Algorithm
} // namespace VisionForge

#endif // _WIN32
