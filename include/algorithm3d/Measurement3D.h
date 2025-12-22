/**
 * @file Measurement3D.h
 * @brief VisionForge Pro 3D测量工具
 *
 * 提供工业级3D测量功能，包括：
 * - 距离测量
 * - 角度测量
 * - 平面度测量
 * - 圆柱度测量
 * - 高度/厚度测量
 * - 间隙/平齐度测量
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_MEASUREMENT3D_H
#define VISIONFORGE_MEASUREMENT3D_H

#include "Algorithm3DTypes.h"
#include "PointCloud3D.h"
#include <QObject>
#include <QString>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// 几何拟合结果
// ============================================================

/**
 * @brief 平面拟合结果
 */
struct PlaneFitResult {
    bool success = false;
    Plane3D plane;
    double rmsError = 0.0;      ///< RMS拟合误差
    double maxError = 0.0;      ///< 最大拟合误差
    int inlierCount = 0;        ///< 内点数量
    std::vector<size_t> inliers; ///< 内点索引

    /**
     * @brief 获取平面度
     */
    double getFlatness() const { return maxError * 2.0; }
};

/**
 * @brief 圆柱拟合结果
 */
struct CylinderFitResult {
    bool success = false;
    Cylinder3D cylinder;
    double rmsError = 0.0;
    double maxError = 0.0;
    int inlierCount = 0;

    /**
     * @brief 获取圆柱度
     */
    double getCylindricity() const { return maxError * 2.0; }
};

/**
 * @brief 球体拟合结果
 */
struct SphereFitResult {
    bool success = false;
    Sphere3D sphere;
    double rmsError = 0.0;
    double maxError = 0.0;
    int inlierCount = 0;

    /**
     * @brief 获取球度
     */
    double getSphericity() const { return maxError * 2.0; }
};

/**
 * @brief 直线拟合结果
 */
struct LineFitResult {
    bool success = false;
    Line3D line;
    double rmsError = 0.0;
    double maxError = 0.0;
    int inlierCount = 0;

    /**
     * @brief 获取直线度
     */
    double getStraightness() const { return maxError * 2.0; }
};

/**
 * @brief 圆拟合结果
 */
struct CircleFitResult {
    bool success = false;
    Vector3d center;
    Vector3d normal;
    double radius = 0.0;
    double rmsError = 0.0;
    double maxError = 0.0;
    int inlierCount = 0;

    /**
     * @brief 获取圆度
     */
    double getCircularity() const { return maxError * 2.0; }
};

// ============================================================
// 3D测量工具基类
// ============================================================

/**
 * @brief 3D测量工具基类
 */
class Measurement3DTool : public QObject {
    Q_OBJECT

public:
    explicit Measurement3DTool(QObject* parent = nullptr);
    virtual ~Measurement3DTool();

    /**
     * @brief 获取测量类型
     */
    virtual Measurement3DType getType() const = 0;

    /**
     * @brief 获取名称
     */
    virtual QString getName() const = 0;

    /**
     * @brief 执行测量
     */
    virtual Measurement3DResult measure() = 0;

    /**
     * @brief 获取最后的结果
     */
    const Measurement3DResult& getLastResult() const { return m_lastResult; }

protected:
    Measurement3DResult m_lastResult;
};

// ============================================================
// 距离测量
// ============================================================

/**
 * @brief 点到点距离测量
 */
class PointToPointDistance : public Measurement3DTool {
    Q_OBJECT

public:
    explicit PointToPointDistance(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Distance; }
    QString getName() const override { return "PointToPointDistance"; }

    /**
     * @brief 设置两个点
     */
    void setPoints(const Point3D& p1, const Point3D& p2);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    Point3D m_point1;
    Point3D m_point2;
};

/**
 * @brief 点到平面距离测量
 */
class PointToPlaneDistance : public Measurement3DTool {
    Q_OBJECT

public:
    explicit PointToPlaneDistance(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Distance; }
    QString getName() const override { return "PointToPlaneDistance"; }

    /**
     * @brief 设置点和平面
     */
    void setPointAndPlane(const Point3D& point, const Plane3D& plane);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    Point3D m_point;
    Plane3D m_plane;
};

/**
 * @brief 点到直线距离测量
 */
class PointToLineDistance : public Measurement3DTool {
    Q_OBJECT

public:
    explicit PointToLineDistance(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Distance; }
    QString getName() const override { return "PointToLineDistance"; }

    /**
     * @brief 设置点和直线
     */
    void setPointAndLine(const Point3D& point, const Line3D& line);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    Point3D m_point;
    Line3D m_line;
};

/**
 * @brief 平面到平面距离测量
 */
class PlaneToPlaneDistance : public Measurement3DTool {
    Q_OBJECT

public:
    explicit PlaneToPlaneDistance(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Distance; }
    QString getName() const override { return "PlaneToPlaneDistance"; }

    /**
     * @brief 设置两个平面
     */
    void setPlanes(const Plane3D& plane1, const Plane3D& plane2);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    Plane3D m_plane1;
    Plane3D m_plane2;
};

// ============================================================
// 角度测量
// ============================================================

/**
 * @brief 两条直线夹角测量
 */
class LineToLineAngle : public Measurement3DTool {
    Q_OBJECT

public:
    explicit LineToLineAngle(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Angle; }
    QString getName() const override { return "LineToLineAngle"; }

    /**
     * @brief 设置两条直线
     */
    void setLines(const Line3D& line1, const Line3D& line2);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    Line3D m_line1;
    Line3D m_line2;
};

/**
 * @brief 两个平面夹角测量
 */
class PlaneToPlaneAngle : public Measurement3DTool {
    Q_OBJECT

public:
    explicit PlaneToPlaneAngle(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Angle; }
    QString getName() const override { return "PlaneToPlaneAngle"; }

    /**
     * @brief 设置两个平面
     */
    void setPlanes(const Plane3D& plane1, const Plane3D& plane2);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    Plane3D m_plane1;
    Plane3D m_plane2;
};

/**
 * @brief 直线与平面夹角测量
 */
class LineToPlaneAngle : public Measurement3DTool {
    Q_OBJECT

public:
    explicit LineToPlaneAngle(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Angle; }
    QString getName() const override { return "LineToPlaneAngle"; }

    /**
     * @brief 设置直线和平面
     */
    void setLineAndPlane(const Line3D& line, const Plane3D& plane);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    Line3D m_line;
    Plane3D m_plane;
};

// ============================================================
// 形位公差测量
// ============================================================

/**
 * @brief 平面度测量
 */
class FlatnessMeasurement : public Measurement3DTool {
    Q_OBJECT

public:
    explicit FlatnessMeasurement(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Flatness; }
    QString getName() const override { return "FlatnessMeasurement"; }

    /**
     * @brief 设置点云
     */
    void setPointCloud(const PointCloud3D& cloud);

    /**
     * @brief 设置参考平面（可选）
     */
    void setReferencePlane(const Plane3D& plane);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

    /**
     * @brief 获取拟合结果
     */
    const PlaneFitResult& getFitResult() const { return m_fitResult; }

private:
    PointCloud3D m_cloud;
    Plane3D m_referencePlane;
    bool m_hasReferencePlane = false;
    PlaneFitResult m_fitResult;
};

/**
 * @brief 圆柱度测量
 */
class CylindricityMeasurement : public Measurement3DTool {
    Q_OBJECT

public:
    explicit CylindricityMeasurement(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Cylindricity; }
    QString getName() const override { return "CylindricityMeasurement"; }

    /**
     * @brief 设置点云
     */
    void setPointCloud(const PointCloud3D& cloud);

    /**
     * @brief 设置初始轴方向（可选）
     */
    void setInitialAxis(const Vector3d& axis);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

    /**
     * @brief 获取拟合结果
     */
    const CylinderFitResult& getFitResult() const { return m_fitResult; }

private:
    PointCloud3D m_cloud;
    Vector3d m_initialAxis;
    bool m_hasInitialAxis = false;
    CylinderFitResult m_fitResult;
};

/**
 * @brief 球度测量
 */
class SphericityMeasurement : public Measurement3DTool {
    Q_OBJECT

public:
    explicit SphericityMeasurement(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Sphericity; }
    QString getName() const override { return "SphericityMeasurement"; }

    /**
     * @brief 设置点云
     */
    void setPointCloud(const PointCloud3D& cloud);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

    /**
     * @brief 获取拟合结果
     */
    const SphereFitResult& getFitResult() const { return m_fitResult; }

private:
    PointCloud3D m_cloud;
    SphereFitResult m_fitResult;
};

// ============================================================
// 高度/厚度测量
// ============================================================

/**
 * @brief 高度测量
 */
class HeightMeasurement : public Measurement3DTool {
    Q_OBJECT

public:
    explicit HeightMeasurement(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Height; }
    QString getName() const override { return "HeightMeasurement"; }

    /**
     * @brief 设置测量点云和基准平面
     */
    void setPointCloudAndReference(const PointCloud3D& cloud, const Plane3D& basePlane);

    /**
     * @brief 设置测量区域（可选）
     */
    void setMeasurementRegion(const BoundingBox3D& region);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    PointCloud3D m_cloud;
    Plane3D m_basePlane;
    BoundingBox3D m_region;
    bool m_hasRegion = false;
};

/**
 * @brief 厚度测量
 */
class ThicknessMeasurement : public Measurement3DTool {
    Q_OBJECT

public:
    explicit ThicknessMeasurement(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Thickness; }
    QString getName() const override { return "ThicknessMeasurement"; }

    /**
     * @brief 设置上下表面点云
     */
    void setSurfaces(const PointCloud3D& topSurface, const PointCloud3D& bottomSurface);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    PointCloud3D m_topSurface;
    PointCloud3D m_bottomSurface;
};

// ============================================================
// 间隙/平齐度测量
// ============================================================

/**
 * @brief 间隙测量
 */
class GapMeasurement : public Measurement3DTool {
    Q_OBJECT

public:
    explicit GapMeasurement(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Gap; }
    QString getName() const override { return "GapMeasurement"; }

    /**
     * @brief 设置两个边缘点云
     */
    void setEdges(const PointCloud3D& edge1, const PointCloud3D& edge2);

    /**
     * @brief 设置测量方向
     */
    void setMeasurementDirection(const Vector3d& direction);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    PointCloud3D m_edge1;
    PointCloud3D m_edge2;
    Vector3d m_direction;
    bool m_hasDirection = false;
};

/**
 * @brief 平齐度测量
 */
class FlushMeasurement : public Measurement3DTool {
    Q_OBJECT

public:
    explicit FlushMeasurement(QObject* parent = nullptr);

    Measurement3DType getType() const override { return Measurement3DType::Flush; }
    QString getName() const override { return "FlushMeasurement"; }

    /**
     * @brief 设置两个表面点云
     */
    void setSurfaces(const PointCloud3D& surface1, const PointCloud3D& surface2);

    /**
     * @brief 执行测量
     */
    Measurement3DResult measure() override;

private:
    PointCloud3D m_surface1;
    PointCloud3D m_surface2;
};

// ============================================================
// 几何拟合工具
// ============================================================

/**
 * @brief 3D几何拟合工具
 *
 * 提供各种几何元素的拟合功能。
 */
class GeometryFitter {
public:
    /**
     * @brief 平面拟合（最小二乘）
     */
    static PlaneFitResult fitPlane(const PointCloud3D& cloud);

    /**
     * @brief 平面拟合（RANSAC）
     */
    static PlaneFitResult fitPlaneRANSAC(const PointCloud3D& cloud,
                                          double threshold = 1.0,
                                          int maxIterations = 100);

    /**
     * @brief 直线拟合
     */
    static LineFitResult fitLine(const PointCloud3D& cloud);

    /**
     * @brief 直线拟合（RANSAC）
     */
    static LineFitResult fitLineRANSAC(const PointCloud3D& cloud,
                                        double threshold = 1.0,
                                        int maxIterations = 100);

    /**
     * @brief 圆柱拟合
     */
    static CylinderFitResult fitCylinder(const PointCloud3D& cloud,
                                          const Vector3d& initialAxis = Vector3d(0, 0, 1));

    /**
     * @brief 圆柱拟合（RANSAC）
     */
    static CylinderFitResult fitCylinderRANSAC(const PointCloud3D& cloud,
                                                double threshold = 1.0,
                                                int maxIterations = 100);

    /**
     * @brief 球体拟合
     */
    static SphereFitResult fitSphere(const PointCloud3D& cloud);

    /**
     * @brief 球体拟合（RANSAC）
     */
    static SphereFitResult fitSphereRANSAC(const PointCloud3D& cloud,
                                            double threshold = 1.0,
                                            int maxIterations = 100);

    /**
     * @brief 圆拟合（平面圆）
     */
    static CircleFitResult fitCircle(const PointCloud3D& cloud,
                                      const Plane3D& plane);

    /**
     * @brief 圆拟合（自动确定平面）
     */
    static CircleFitResult fitCircleAuto(const PointCloud3D& cloud);
};

// ============================================================
// 3D测量管理器
// ============================================================

/**
 * @brief 3D测量管理器
 *
 * 统一管理各种3D测量工具。
 */
class Measurement3DManager : public QObject {
    Q_OBJECT

public:
    explicit Measurement3DManager(QObject* parent = nullptr);
    ~Measurement3DManager();

    // ============ 快捷测量方法 ============

    /**
     * @brief 测量两点距离
     */
    Measurement3DResult measureDistance(const Point3D& p1, const Point3D& p2);

    /**
     * @brief 测量点到平面距离
     */
    Measurement3DResult measurePointToPlane(const Point3D& point, const Plane3D& plane);

    /**
     * @brief 测量两直线夹角
     */
    Measurement3DResult measureAngle(const Line3D& line1, const Line3D& line2);

    /**
     * @brief 测量两平面夹角
     */
    Measurement3DResult measureAngle(const Plane3D& plane1, const Plane3D& plane2);

    /**
     * @brief 测量平面度
     */
    Measurement3DResult measureFlatness(const PointCloud3D& cloud);

    /**
     * @brief 测量圆柱度
     */
    Measurement3DResult measureCylindricity(const PointCloud3D& cloud);

    /**
     * @brief 测量球度
     */
    Measurement3DResult measureSphericity(const PointCloud3D& cloud);

    /**
     * @brief 测量高度
     */
    Measurement3DResult measureHeight(const PointCloud3D& cloud, const Plane3D& basePlane);

    /**
     * @brief 测量间隙
     */
    Measurement3DResult measureGap(const PointCloud3D& edge1, const PointCloud3D& edge2);

    /**
     * @brief 测量平齐度
     */
    Measurement3DResult measureFlush(const PointCloud3D& surface1, const PointCloud3D& surface2);

    // ============ 几何拟合 ============

    /**
     * @brief 拟合平面
     */
    PlaneFitResult fitPlane(const PointCloud3D& cloud, bool useRANSAC = false);

    /**
     * @brief 拟合直线
     */
    LineFitResult fitLine(const PointCloud3D& cloud, bool useRANSAC = false);

    /**
     * @brief 拟合圆柱
     */
    CylinderFitResult fitCylinder(const PointCloud3D& cloud, bool useRANSAC = false);

    /**
     * @brief 拟合球体
     */
    SphereFitResult fitSphere(const PointCloud3D& cloud, bool useRANSAC = false);

    /**
     * @brief 拟合圆
     */
    CircleFitResult fitCircle(const PointCloud3D& cloud);

signals:
    /**
     * @brief 测量完成
     */
    void measurementCompleted(const Measurement3DResult& result);
};

} // namespace Algorithm3D
} // namespace VisionForge

#endif // VISIONFORGE_MEASUREMENT3D_H
