/**
 * @file CoordinateTransform.h
 * @brief 坐标转换类 - 9点标定和坐标变换
 * @details 支持仿射变换、透视变换、相似变换、刚性变换
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QJsonObject>
#include <opencv2/core.hpp>
#include <memory>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 变换类型
 */
enum class TransformType {
    Affine,         ///< 仿射变换（6自由度）：平移、旋转、缩放、剪切
    Perspective,    ///< 透视变换（8自由度）：完整投影变换
    Similarity,     ///< 相似变换（4自由度）：平移、旋转、均匀缩放
    Rigid           ///< 刚性变换（3自由度）：平移、旋转
};

/**
 * @brief 标定点对
 */
struct CalibPointPair {
    int index;              ///< 点索引
    QPointF pixelCoord;     ///< 像素坐标
    QPointF worldCoord;     ///< 世界坐标（机械坐标）
    bool isSet = false;     ///< 是否已设置

    CalibPointPair() : index(-1), isSet(false) {}
    CalibPointPair(int idx, const QPointF& pixel, const QPointF& world)
        : index(idx), pixelCoord(pixel), worldCoord(world), isSet(true) {}
};

/**
 * @brief 标定统计信息
 */
struct CalibStats {
    bool success = false;           ///< 标定是否成功
    double meanError = 0.0;         ///< 平均误差（像素）
    double maxError = 0.0;          ///< 最大误差（像素）
    double rmsError = 0.0;          ///< 均方根误差（像素）
    double pixelPerMm = 0.0;        ///< 每毫米像素数（估计值）
    int pointCount = 0;             ///< 使用的标定点数
    QVector<double> pointErrors;    ///< 各点的误差

    QJsonObject toJson() const;
    static CalibStats fromJson(const QJsonObject& json);
};

/**
 * @brief 坐标变换标定结果
 */
struct CoordCalibResult {
    QString id;                     ///< 标定ID
    QString name;                   ///< 标定名称
    QString cameraId;               ///< 关联的相机ID
    TransformType transformType;    ///< 变换类型
    cv::Mat transformMatrix;        ///< 变换矩阵
    cv::Mat inverseMatrix;          ///< 逆变换矩阵
    CalibStats stats;               ///< 标定统计
    QVector<CalibPointPair> calibPoints;  ///< 标定点
    qint64 timestamp;               ///< 标定时间戳

    CoordCalibResult() : transformType(TransformType::Affine), timestamp(0) {}

    bool isValid() const { return stats.success && !transformMatrix.empty(); }

    QJsonObject toJson() const;
    static CoordCalibResult fromJson(const QJsonObject& json);
};

/**
 * @class CoordinateTransform
 * @brief 坐标转换类
 *
 * 功能：
 * - 支持N点标定（默认9点，3x3网格）
 * - 支持多种变换类型
 * - 像素坐标与世界坐标互转
 * - 标定结果的序列化/反序列化
 */
class CoordinateTransform : public QObject
{
    Q_OBJECT

public:
    explicit CoordinateTransform(QObject* parent = nullptr);
    ~CoordinateTransform();

    // ========== 标定点管理 ==========

    /**
     * @brief 设置标定网格大小
     * @param rows 行数
     * @param cols 列数
     * @note 总点数 = rows * cols
     */
    void setPointCount(int rows, int cols);

    /**
     * @brief 获取标定网格大小
     */
    void getPointCount(int& rows, int& cols) const;

    /**
     * @brief 获取总点数
     */
    int totalPointCount() const;

    /**
     * @brief 设置单个标定点
     * @param index 点索引（0 到 rows*cols-1）
     * @param point 标定点对
     */
    void setCalibPoint(int index, const CalibPointPair& point);

    /**
     * @brief 设置单个标定点的像素坐标
     */
    void setPixelCoord(int index, const QPointF& pixel);

    /**
     * @brief 设置单个标定点的世界坐标
     */
    void setWorldCoord(int index, const QPointF& world);

    /**
     * @brief 获取标定点
     * @param index 点索引
     * @return 标定点对
     */
    CalibPointPair getCalibPoint(int index) const;

    /**
     * @brief 获取所有标定点
     */
    QVector<CalibPointPair> getAllCalibPoints() const;

    /**
     * @brief 清空所有标定点
     */
    void clearCalibPoints();

    /**
     * @brief 检查标定点是否完整
     * @return true如果所有点都已设置
     */
    bool isCalibPointsComplete() const;

    /**
     * @brief 获取已设置的点数
     */
    int setPointCount() const;

    // ========== 网格预设 ==========

    /**
     * @brief 生成网格世界坐标
     * @param spacingX X方向间距（mm）
     * @param spacingY Y方向间距（mm）
     * @param originX 原点X坐标（mm）
     * @param originY 原点Y坐标（mm）
     * @note 按行优先顺序生成坐标
     */
    void generateGridWorldCoords(double spacingX, double spacingY,
                                 double originX = 0.0, double originY = 0.0);

    /**
     * @brief 从标定板图像自动检测标定点
     * @param image 标定板图像
     * @param patternSize 角点网格大小
     * @return 是否检测成功
     */
    bool detectCalibPattern(const cv::Mat& image, cv::Size patternSize = cv::Size());

    // ========== 标定执行 ==========

    /**
     * @brief 设置变换类型
     */
    void setTransformType(TransformType type);

    /**
     * @brief 获取变换类型
     */
    TransformType getTransformType() const;

    /**
     * @brief 执行标定
     * @return true如果成功
     */
    bool calibrate();

    /**
     * @brief 获取标定统计信息
     */
    CalibStats getCalibStats() const;

    /**
     * @brief 获取完整标定结果
     */
    CoordCalibResult getCalibrationResult() const;

    /**
     * @brief 设置标定结果（从已有结果恢复）
     */
    void setCalibrationResult(const CoordCalibResult& result);

    /**
     * @brief 检查标定是否有效
     */
    bool isCalibrated() const;

    // ========== 坐标转换 ==========

    /**
     * @brief 像素坐标转世界坐标
     * @param pixel 像素坐标
     * @return 世界坐标
     */
    QPointF pixelToWorld(const QPointF& pixel) const;

    /**
     * @brief 世界坐标转像素坐标
     * @param world 世界坐标
     * @return 像素坐标
     */
    QPointF worldToPixel(const QPointF& world) const;

    /**
     * @brief 批量像素坐标转世界坐标
     */
    QVector<QPointF> pixelToWorld(const QVector<QPointF>& pixels) const;

    /**
     * @brief 批量世界坐标转像素坐标
     */
    QVector<QPointF> worldToPixel(const QVector<QPointF>& worlds) const;

    /**
     * @brief 像素距离转世界距离
     * @param pixelDist 像素距离
     * @return 世界距离（mm）
     */
    double pixelToWorldDistance(double pixelDist) const;

    /**
     * @brief 世界距离转像素距离
     * @param worldDist 世界距离（mm）
     * @return 像素距离
     */
    double worldToPixelDistance(double worldDist) const;

    /**
     * @brief 像素角度转世界角度
     * @param pixelAngle 像素坐标系中的角度（弧度）
     * @return 世界坐标系中的角度（弧度）
     */
    double pixelToWorldAngle(double pixelAngle) const;

    /**
     * @brief 获取变换矩阵
     */
    cv::Mat getTransformMatrix() const;

    /**
     * @brief 获取逆变换矩阵
     */
    cv::Mat getInverseMatrix() const;

    // ========== 序列化 ==========

    /**
     * @brief 保存标定结果到文件
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool saveToFile(const QString& filePath) const;

    /**
     * @brief 从文件加载标定结果
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool loadFromFile(const QString& filePath);

    /**
     * @brief 导出为JSON
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON导入
     */
    bool fromJson(const QJsonObject& json);

    // ========== 标定信息 ==========

    /**
     * @brief 设置标定ID
     */
    void setId(const QString& id);

    /**
     * @brief 获取标定ID
     */
    QString getId() const;

    /**
     * @brief 设置标定名称
     */
    void setName(const QString& name);

    /**
     * @brief 获取标定名称
     */
    QString getName() const;

    /**
     * @brief 设置关联的相机ID
     */
    void setCameraId(const QString& cameraId);

    /**
     * @brief 获取关联的相机ID
     */
    QString getCameraId() const;

signals:
    /**
     * @brief 标定点变化信号
     */
    void calibPointChanged(int index);

    /**
     * @brief 标定完成信号
     */
    void calibrationCompleted(bool success);

    /**
     * @brief 标定结果变化信号
     */
    void calibrationChanged();

private:
    // 计算变换矩阵
    bool computeAffineTransform();
    bool computePerspectiveTransform();
    bool computeSimilarityTransform();
    bool computeRigidTransform();

    // 计算标定误差
    void computeCalibrationErrors();

    // 应用变换
    QPointF applyTransform(const QPointF& point, const cv::Mat& matrix) const;

private:
    QString id_;                        ///< 标定ID
    QString name_;                      ///< 标定名称
    QString cameraId_;                  ///< 关联相机ID

    int gridRows_;                      ///< 网格行数
    int gridCols_;                      ///< 网格列数
    QVector<CalibPointPair> calibPoints_;  ///< 标定点列表

    TransformType transformType_;       ///< 变换类型
    cv::Mat transformMatrix_;           ///< 变换矩阵
    cv::Mat inverseMatrix_;             ///< 逆变换矩阵

    CalibStats stats_;                  ///< 标定统计
    bool isCalibrated_;                 ///< 是否已标定
    qint64 calibrationTimestamp_;       ///< 标定时间戳
};

} // namespace Algorithm
} // namespace VisionForge
