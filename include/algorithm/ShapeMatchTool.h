/**
 * @file ShapeMatchTool.h
 * @brief Halcon形状匹配工具
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>

#ifdef _WIN32
#include "HalconCpp.h"
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

/**
 * @struct ModelCheckPoint
 * @brief 模型校验点结构
 *
 * 校验点用于在形状匹配后验证匹配结果的可靠性，
 * 通过检查特定位置的灰度值是否在期望范围内
 */
struct ModelCheckPoint {
    double relRow;          // 相对于模板参考点的行偏移
    double relCol;          // 相对于模板参考点的列偏移
    int expectedGray;       // 期望灰度值
    int tolerance;          // 灰度容差（期望值±容差）
    QString name;           // 校验点名称（可选）
    bool enabled;           // 是否启用

    ModelCheckPoint()
        : relRow(0), relCol(0), expectedGray(128), tolerance(30), enabled(true) {}

    ModelCheckPoint(double row, double col, int gray, int tol, const QString& n = "")
        : relRow(row), relCol(col), expectedGray(gray), tolerance(tol), name(n), enabled(true) {}
};

/**
 * @enum MarkShapeType
 * @brief 标准Mark形状类型枚举
 */
enum class MarkShapeType {
    Cross,          // 十字形
    TShape,         // T字形
    LShape,         // L字形
    Circle,         // 圆形
    Ring,           // 圆环形
    Rectangle,      // 矩形
    Triangle,       // 三角形
    Diamond,        // 菱形
    Arrow,          // 箭头形
    Custom          // 用户自定义
};

/**
 * @struct MarkShapeParams
 * @brief Mark形状参数结构
 */
struct MarkShapeParams {
    MarkShapeType type = MarkShapeType::Cross;
    double size1 = 100.0;       // 主尺寸（臂长/半径/边长等）
    double size2 = 20.0;        // 辅尺寸（臂宽/内径/宽度等）
    double size3 = 0.0;         // 额外尺寸（用于复杂形状）
    double rotation = 0.0;      // 旋转角度（弧度）
    double centerRow = 256.0;   // 中心行坐标
    double centerCol = 256.0;   // 中心列坐标

    // 获取形状名称
    static QString getShapeName(MarkShapeType type) {
        switch (type) {
            case MarkShapeType::Cross:     return "十字形";
            case MarkShapeType::TShape:    return "T字形";
            case MarkShapeType::LShape:    return "L字形";
            case MarkShapeType::Circle:    return "圆形";
            case MarkShapeType::Ring:      return "圆环形";
            case MarkShapeType::Rectangle: return "矩形";
            case MarkShapeType::Triangle:  return "三角形";
            case MarkShapeType::Diamond:   return "菱形";
            case MarkShapeType::Arrow:     return "箭头形";
            case MarkShapeType::Custom:    return "自定义";
            default: return "未知";
        }
    }
};

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
    /**
     * @brief 匹配类型
     */
    enum MatchType {
        Standard,       // 标准匹配（仅旋转）
        Scaled,         // 缩放匹配（旋转+缩放）
        Anisotropic     // 仿射匹配（旋转+各向异性缩放）
    };

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

#ifdef _WIN32
    /**
     * @brief 获取模型轮廓（用于特征预览）
     * @return XLD轮廓对象
     */
    HXLDCont getModelContours() const;

    /**
     * @brief 生成标准十字形XLD轮廓
     * @param armLength 十字臂长度（像素）
     * @param armWidth 十字臂宽度（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return 十字形XLD轮廓
     */
    static HXLDCont generateCrossContour(double armLength, double armWidth,
                                         double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成T字形XLD轮廓
     * @param width T字顶部宽度（像素）
     * @param height T字总高度（像素）
     * @param thickness 线宽（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return T字形XLD轮廓
     */
    static HXLDCont generateTShapeContour(double width, double height, double thickness,
                                          double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成L字形XLD轮廓
     * @param width L字水平臂长度（像素）
     * @param height L字垂直臂长度（像素）
     * @param thickness 线宽（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return L字形XLD轮廓
     */
    static HXLDCont generateLShapeContour(double width, double height, double thickness,
                                          double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成圆形XLD轮廓
     * @param radius 圆半径（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return 圆形XLD轮廓
     */
    static HXLDCont generateCircleContour(double radius,
                                          double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成圆环形XLD轮廓
     * @param outerRadius 外圆半径（像素）
     * @param innerRadius 内圆半径（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return 圆环形XLD轮廓
     */
    static HXLDCont generateRingContour(double outerRadius, double innerRadius,
                                        double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成矩形XLD轮廓
     * @param width 矩形宽度（像素）
     * @param height 矩形高度（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return 矩形XLD轮廓
     */
    static HXLDCont generateRectangleContour(double width, double height,
                                             double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成三角形XLD轮廓
     * @param sideLength 三角形边长（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return 三角形XLD轮廓
     */
    static HXLDCont generateTriangleContour(double sideLength,
                                            double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成菱形XLD轮廓
     * @param width 菱形宽度（像素）
     * @param height 菱形高度（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return 菱形XLD轮廓
     */
    static HXLDCont generateDiamondContour(double width, double height,
                                           double centerRow = 0, double centerCol = 0);

    /**
     * @brief 生成箭头形XLD轮廓
     * @param length 箭头长度（像素）
     * @param headWidth 箭头头部宽度（像素）
     * @param shaftWidth 箭杆宽度（像素）
     * @param centerRow 中心点行坐标
     * @param centerCol 中心点列坐标
     * @return 箭头形XLD轮廓
     */
    static HXLDCont generateArrowContour(double length, double headWidth, double shaftWidth,
                                         double centerRow = 0, double centerCol = 0);

    /**
     * @brief 根据参数生成Mark形状轮廓（统一接口）
     * @param params 形状参数
     * @return XLD轮廓
     */
    static HXLDCont generateMarkContour(const MarkShapeParams& params);

    /**
     * @brief 从XLD轮廓训练模板（适用于高噪声场景）
     * @param contour XLD轮廓（如标准十字轮廓）
     * @param highNoiseImage 高噪声样本图像（用于估计金字塔层级，可选）
     * @param sampleRow1 样本区域左上角行
     * @param sampleCol1 样本区域左上角列
     * @param sampleRow2 样本区域右下角行
     * @param sampleCol2 样本区域右下角列
     * @return 是否成功
     */
    bool trainModelFromContour(const HXLDCont& contour,
                               const Base::ImageData::Ptr& highNoiseImage = nullptr,
                               int sampleRow1 = 0, int sampleCol1 = 0,
                               int sampleRow2 = 0, int sampleCol2 = 0);

    /**
     * @brief 设置当前使用的标准轮廓（用于显示）
     */
    void setStandardContour(const HXLDCont& contour) { standardContour_ = contour; }
    HXLDCont getStandardContour() const { return standardContour_; }
#endif

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

    void setMaxOverlap(double overlap) { maxOverlap_ = overlap; emit paramChanged(); }
    double getMaxOverlap() const { return maxOverlap_; }

    void setGreediness(double greediness) { greediness_ = greediness; emit paramChanged(); }
    double getGreediness() const { return greediness_; }

    void setModelPath(const QString& path) { modelPath_ = path; }
    QString getModelPath() const { return modelPath_; }

    void setUseXLDDisplay(bool use) { useXLDDisplay_ = use; emit paramChanged(); }
    bool getUseXLDDisplay() const { return useXLDDisplay_; }

    void setMatchType(MatchType type) {
        if (matchType_ != type) {
            matchType_ = type;
            emit paramChanged();
        }
    }
    MatchType getMatchType() const { return matchType_; }

    void setScaleStepRow(double step) { scaleStepRow_ = step; emit paramChanged(); }
    double getScaleStepRow() const { return scaleStepRow_; }

    void setScaleStepCol(double step) { scaleStepCol_ = step; emit paramChanged(); }
    double getScaleStepCol() const { return scaleStepCol_; }

    // ========== 轮廓过滤参数 ==========

    void setContourFilterEnabled(bool enabled) { contourFilterEnabled_ = enabled; emit paramChanged(); }
    bool isContourFilterEnabled() const { return contourFilterEnabled_; }

    void setMinContourLength(double length) { minContourLength_ = length; emit paramChanged(); }
    double getMinContourLength() const { return minContourLength_; }

    void setUnionCollinearEnabled(bool enabled) { unionCollinearEnabled_ = enabled; emit paramChanged(); }
    bool isUnionCollinearEnabled() const { return unionCollinearEnabled_; }

    void setUnionAdjacentEnabled(bool enabled) { unionAdjacentEnabled_ = enabled; emit paramChanged(); }
    bool isUnionAdjacentEnabled() const { return unionAdjacentEnabled_; }

    void setMaxDistCollinear(double dist) { maxDistCollinear_ = dist; emit paramChanged(); }
    double getMaxDistCollinear() const { return maxDistCollinear_; }

    void setMaxDistAdjacent(double dist) { maxDistAdjacent_ = dist; emit paramChanged(); }
    double getMaxDistAdjacent() const { return maxDistAdjacent_; }

    // ========== 模型校验点接口 ==========

    /**
     * @brief 添加校验点
     * @param checkPoint 校验点
     */
    void addCheckPoint(const ModelCheckPoint& checkPoint);

    /**
     * @brief 移除校验点
     * @param index 索引
     */
    void removeCheckPoint(int index);

    /**
     * @brief 清除所有校验点
     */
    void clearCheckPoints();

    /**
     * @brief 获取校验点列表
     */
    const std::vector<ModelCheckPoint>& getCheckPoints() const { return checkPoints_; }

    /**
     * @brief 设置校验点列表
     */
    void setCheckPoints(const std::vector<ModelCheckPoint>& points) { checkPoints_ = points; emit paramChanged(); }

    /**
     * @brief 启用/禁用校验点功能
     */
    void setCheckPointsEnabled(bool enabled) { checkPointsEnabled_ = enabled; emit paramChanged(); }
    bool isCheckPointsEnabled() const { return checkPointsEnabled_; }

    /**
     * @brief 验证匹配结果的校验点
     * @param image 搜索图像
     * @param row 匹配位置行
     * @param col 匹配位置列
     * @param angle 匹配角度
     * @param failedPoints 输出：失败的校验点索引
     * @return 是否所有校验点都通过
     */
    bool validateCheckPoints(const Base::ImageData::Ptr& image,
                            double row, double col, double angle,
                            std::vector<int>* failedPoints = nullptr);

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

    /**
     * @brief 创建匹配结果的XLD轮廓
     */
    HXLDCont createMatchContours(const HTuple& row, const HTuple& col,
                                 const HTuple& angle);

    /**
     * @brief 过滤和优化XLD轮廓
     * @param contours 输入轮廓
     * @return 过滤后的轮廓
     */
    HXLDCont filterContours(const HXLDCont& contours) const;
#endif

private:
#ifdef _WIN32
    HTuple shapeModel_;         // Halcon形状模型句柄
    HXLDCont standardContour_;  // 标准轮廓（用于显示和从轮廓训练）
    HXLDCont lastMatchContours_; // 最后一次匹配的轮廓结果（从Generic模型直接获取）
    bool useGenericModel_;      // 是否使用通用形状模型（从XLD轮廓创建）
#endif
    bool modelLoaded_;          // 模板是否已加载
    QString modelPath_;         // 模板文件路径

    // 匹配参数
    MatchType matchType_;       // 匹配类型
    double minScore_;           // 最小匹配分数 (0.0-1.0)
    int numMatches_;            // 查找匹配数量 (0=全部)
    double angleStart_;         // 起始角度 (弧度)
    double angleExtent_;        // 角度范围 (弧度)
    int minContrast_;           // 最小对比度
    double scaleMin_;           // 最小缩放
    double scaleMax_;           // 最大缩放
    double scaleStepRow_;       // 行方向缩放步长（仿射匹配用）
    double scaleStepCol_;       // 列方向缩放步长（仿射匹配用）
    double maxOverlap_;         // 最大重叠度 (0.0-1.0)
    double greediness_;         // 贪婪度 (0.0-1.0)
    bool useXLDDisplay_;        // 是否使用XLD轮廓显示

    // 模板信息
    int modelWidth_;            // 模板宽度
    int modelHeight_;           // 模板高度
    double modelRefRow_;        // 模板参考点行
    double modelRefCol_;        // 模板参考点列

    // 校验点
    std::vector<ModelCheckPoint> checkPoints_;  // 校验点列表
    bool checkPointsEnabled_;   // 是否启用校验点验证

    // 轮廓过滤参数
    bool contourFilterEnabled_;     // 是否启用轮廓过滤
    double minContourLength_;       // 最小轮廓长度（像素）
    bool unionCollinearEnabled_;    // 是否连接共线轮廓
    bool unionAdjacentEnabled_;     // 是否连接相邻轮廓
    double maxDistCollinear_;       // 共线轮廓最大距离
    double maxDistAdjacent_;        // 相邻轮廓最大距离
};

} // namespace Algorithm
} // namespace VisionForge
