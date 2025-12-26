/**
 * @file ShapeMatchTool.h
 * @brief Halcon形状匹配工具
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <memory>

namespace VisionForge {
namespace Algorithm {

// 前向声明实现类
class ShapeMatchImpl;

/**
 * @struct ModelCheckPoint
 * @brief 模型校验点结构
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
    bool hasModel() const;

    /**
     * @brief 获取模型轮廓 (用于UI显示)
     * @return 包装的轮廓数据，如果无模型返回空QVariant
     */
    QVariant getModelContours() const;

    /**
     * @brief 从XLD轮廓训练模板（适用于高噪声场景）
     * @param contour 轮廓数据 (QVariant wrapping HalconObjectPtr)
     * @param highNoiseImage 高噪声样本图像（用于估计金字塔层级，可选）
     * @param sampleRow1 样本区域左上角行
     * @param sampleCol1 样本区域左上角列
     * @param sampleRow2 样本区域右下角行
     * @param sampleCol2 样本区域右下角列
     * @return 是否成功
     */
    bool trainModelFromContour(const QVariant& contour,
                               const Base::ImageData::Ptr& highNoiseImage = nullptr,
                               int sampleRow1 = 0, int sampleCol1 = 0,
                               int sampleRow2 = 0, int sampleCol2 = 0);


    // ========== 参数设置 ==========

    void setMinScore(double score);
    double getMinScore() const { return minScore_; }

    void setNumMatches(int num);
    int getNumMatches() const { return numMatches_; }

    void setAngleStart(double angle);
    double getAngleStart() const { return angleStart_; }

    void setAngleExtent(double angle);
    double getAngleExtent() const { return angleExtent_; }

    void setMinContrast(int contrast);
    int getMinContrast() const { return minContrast_; }

    void setScaleMin(double scale);
    double getScaleMin() const { return scaleMin_; }

    void setScaleMax(double scale);
    double getScaleMax() const { return scaleMax_; }

    void setMaxOverlap(double overlap);
    double getMaxOverlap() const { return maxOverlap_; }

    void setGreediness(double greediness);
    double getGreediness() const { return greediness_; }

    void setUseXLDDisplay(bool use);
    bool getUseXLDDisplay() const { return useXLDDisplay_; }

    void setMatchType(MatchType type);
    MatchType getMatchType() const { return matchType_; }

    void setScaleStepRow(double step);
    double getScaleStepRow() const { return scaleStepRow_; }

    void setScaleStepCol(double step);
    double getScaleStepCol() const { return scaleStepCol_; }

    // ========== 轮廓过滤参数 ==========

    void setContourFilterEnabled(bool enabled);
    bool isContourFilterEnabled() const { return contourFilterEnabled_; }

    void setMinContourLength(double length);
    double getMinContourLength() const { return minContourLength_; }

    void setUnionCollinearEnabled(bool enabled);
    bool isUnionCollinearEnabled() const { return unionCollinearEnabled_; }

    void setUnionAdjacentEnabled(bool enabled);
    bool isUnionAdjacentEnabled() const { return unionAdjacentEnabled_; }

    void setMaxDistCollinear(double dist);
    double getMaxDistCollinear() const { return maxDistCollinear_; }

    void setMaxDistAdjacent(double dist);
    double getMaxDistAdjacent() const { return maxDistAdjacent_; }

    // ========== 模型校验点接口 ==========

    void addCheckPoint(const ModelCheckPoint& checkPoint);
    void removeCheckPoint(int index);
    void clearCheckPoints();
    const std::vector<ModelCheckPoint>& getCheckPoints() const { return checkPoints_; }
    void setCheckPoints(const std::vector<ModelCheckPoint>& points);
    void setCheckPointsEnabled(bool enabled);
    bool isCheckPointsEnabled() const { return checkPointsEnabled_; }

    bool validateCheckPoints(const Base::ImageData::Ptr& image,
                            double row, double col, double angle,
                            std::vector<int>* failedPoints = nullptr);
                            
    // 供Impl访问的内部方法
    friend class ShapeMatchImpl;

    // 获取模型路径
    QString getModelPath() const;
    void setModelPath(const QString& path);

private:
    std::unique_ptr<ShapeMatchImpl> impl_;

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
