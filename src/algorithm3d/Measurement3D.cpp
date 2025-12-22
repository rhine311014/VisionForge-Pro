/**
 * @file Measurement3D.cpp
 * @brief VisionForge Pro 3D测量工具实现
 */

#include "algorithm3d/Measurement3D.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// Measurement3DTool
// ============================================================

Measurement3DTool::Measurement3DTool(QObject* parent)
    : QObject(parent)
{
}

Measurement3DTool::~Measurement3DTool() = default;

// ============================================================
// 距离测量
// ============================================================

PointToPointDistance::PointToPointDistance(QObject* parent)
    : Measurement3DTool(parent)
{
}

void PointToPointDistance::setPoints(const Point3D& p1, const Point3D& p2) {
    m_point1 = p1;
    m_point2 = p2;
}

Measurement3DResult PointToPointDistance::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Distance;
    m_lastResult.success = true;
    m_lastResult.value = m_point1.distanceTo(m_point2);
    m_lastResult.unit = "mm";
    m_lastResult.points.push_back(m_point1);
    m_lastResult.points.push_back(m_point2);
    m_lastResult.lines.push_back(Line3D(m_point1, m_point2));
    return m_lastResult;
}

PointToPlaneDistance::PointToPlaneDistance(QObject* parent)
    : Measurement3DTool(parent)
{
}

void PointToPlaneDistance::setPointAndPlane(const Point3D& point, const Plane3D& plane) {
    m_point = point;
    m_plane = plane;
}

Measurement3DResult PointToPlaneDistance::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Distance;
    m_lastResult.success = true;
    m_lastResult.value = m_plane.distanceToPoint(m_point.toVector3d());
    m_lastResult.unit = "mm";
    m_lastResult.points.push_back(m_point);
    m_lastResult.plane = m_plane;
    return m_lastResult;
}

PointToLineDistance::PointToLineDistance(QObject* parent)
    : Measurement3DTool(parent)
{
}

void PointToLineDistance::setPointAndLine(const Point3D& point, const Line3D& line) {
    m_point = point;
    m_line = line;
}

Measurement3DResult PointToLineDistance::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Distance;

    Vector3d p = m_point.toVector3d();
    Vector3d a = m_line.start.toVector3d();
    Vector3d b = m_line.end.toVector3d();
    Vector3d ab = b - a;
    Vector3d ap = p - a;

    double t = ap.dot(ab) / ab.squaredNorm();
    t = std::max(0.0, std::min(1.0, t));

    Vector3d closestPoint = a + t * ab;
    m_lastResult.value = (p - closestPoint).norm();
    m_lastResult.success = true;
    m_lastResult.unit = "mm";
    m_lastResult.points.push_back(m_point);
    m_lastResult.lines.push_back(m_line);

    return m_lastResult;
}

PlaneToPlaneDistance::PlaneToPlaneDistance(QObject* parent)
    : Measurement3DTool(parent)
{
}

void PlaneToPlaneDistance::setPlanes(const Plane3D& plane1, const Plane3D& plane2) {
    m_plane1 = plane1;
    m_plane2 = plane2;
}

Measurement3DResult PlaneToPlaneDistance::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Distance;

    // 检查平面是否平行
    Vector3d n1 = m_plane1.normal();
    Vector3d n2 = m_plane2.normal();

    double dotProduct = std::abs(n1.dot(n2));

    if (dotProduct > 0.99) {
        // 近似平行，计算距离
        double d1 = m_plane1.d / n1.norm();
        double d2 = m_plane2.d / n2.norm();
        m_lastResult.value = std::abs(d1 - d2);
        m_lastResult.success = true;
    } else {
        // 不平行，距离为0
        m_lastResult.value = 0;
        m_lastResult.success = true;
        m_lastResult.errorMessage = "平面不平行";
    }

    m_lastResult.unit = "mm";
    m_lastResult.plane = m_plane1;

    return m_lastResult;
}

// ============================================================
// 角度测量
// ============================================================

LineToLineAngle::LineToLineAngle(QObject* parent)
    : Measurement3DTool(parent)
{
}

void LineToLineAngle::setLines(const Line3D& line1, const Line3D& line2) {
    m_line1 = line1;
    m_line2 = line2;
}

Measurement3DResult LineToLineAngle::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Angle;

    Vector3d dir1 = m_line1.direction();
    Vector3d dir2 = m_line2.direction();

    double dotProduct = dir1.dot(dir2);
    dotProduct = std::max(-1.0, std::min(1.0, dotProduct));

    double angleRad = std::acos(std::abs(dotProduct));
    m_lastResult.value = angleRad * 180.0 / M_PI;
    m_lastResult.success = true;
    m_lastResult.unit = "°";
    m_lastResult.lines.push_back(m_line1);
    m_lastResult.lines.push_back(m_line2);

    return m_lastResult;
}

PlaneToPlaneAngle::PlaneToPlaneAngle(QObject* parent)
    : Measurement3DTool(parent)
{
}

void PlaneToPlaneAngle::setPlanes(const Plane3D& plane1, const Plane3D& plane2) {
    m_plane1 = plane1;
    m_plane2 = plane2;
}

Measurement3DResult PlaneToPlaneAngle::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Angle;

    Vector3d n1 = m_plane1.normal();
    Vector3d n2 = m_plane2.normal();

    double dotProduct = n1.dot(n2);
    dotProduct = std::max(-1.0, std::min(1.0, dotProduct));

    double angleRad = std::acos(std::abs(dotProduct));
    m_lastResult.value = angleRad * 180.0 / M_PI;
    m_lastResult.success = true;
    m_lastResult.unit = "°";
    m_lastResult.plane = m_plane1;

    return m_lastResult;
}

LineToPlaneAngle::LineToPlaneAngle(QObject* parent)
    : Measurement3DTool(parent)
{
}

void LineToPlaneAngle::setLineAndPlane(const Line3D& line, const Plane3D& plane) {
    m_line = line;
    m_plane = plane;
}

Measurement3DResult LineToPlaneAngle::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Angle;

    Vector3d lineDir = m_line.direction();
    Vector3d planeNormal = m_plane.normal();

    double dotProduct = lineDir.dot(planeNormal);
    dotProduct = std::max(-1.0, std::min(1.0, dotProduct));

    // 直线与平面的夹角 = 90° - 直线与法线的夹角
    double angleRad = std::asin(std::abs(dotProduct));
    m_lastResult.value = angleRad * 180.0 / M_PI;
    m_lastResult.success = true;
    m_lastResult.unit = "°";
    m_lastResult.lines.push_back(m_line);
    m_lastResult.plane = m_plane;

    return m_lastResult;
}

// ============================================================
// 形位公差测量
// ============================================================

FlatnessMeasurement::FlatnessMeasurement(QObject* parent)
    : Measurement3DTool(parent)
{
}

void FlatnessMeasurement::setPointCloud(const PointCloud3D& cloud) {
    m_cloud = cloud;
}

void FlatnessMeasurement::setReferencePlane(const Plane3D& plane) {
    m_referencePlane = plane;
    m_hasReferencePlane = true;
}

Measurement3DResult FlatnessMeasurement::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Flatness;

    if (m_cloud.size() < 3) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "点数不足";
        return m_lastResult;
    }

    // 拟合平面
    m_fitResult = GeometryFitter::fitPlane(m_cloud);

    if (!m_fitResult.success) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "平面拟合失败";
        return m_lastResult;
    }

    Plane3D& plane = m_hasReferencePlane ? m_referencePlane : m_fitResult.plane;

    // 计算所有点到平面的距离
    double maxDist = 0;
    double minDist = std::numeric_limits<double>::max();

    for (size_t i = 0; i < m_cloud.size(); ++i) {
        double dist = plane.distanceToPoint(m_cloud[i].toVector3d());
        maxDist = std::max(maxDist, dist);
        minDist = std::min(minDist, dist);
    }

    m_lastResult.value = maxDist - minDist;  // 平面度 = 最大距离差
    m_lastResult.success = true;
    m_lastResult.unit = "mm";
    m_lastResult.plane = plane;
    m_lastResult.stdDev = m_fitResult.rmsError;

    return m_lastResult;
}

CylindricityMeasurement::CylindricityMeasurement(QObject* parent)
    : Measurement3DTool(parent)
{
}

void CylindricityMeasurement::setPointCloud(const PointCloud3D& cloud) {
    m_cloud = cloud;
}

void CylindricityMeasurement::setInitialAxis(const Vector3d& axis) {
    m_initialAxis = axis;
    m_hasInitialAxis = true;
}

Measurement3DResult CylindricityMeasurement::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Cylindricity;

    if (m_cloud.size() < 6) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "点数不足";
        return m_lastResult;
    }

    // 拟合圆柱
    Vector3d axis = m_hasInitialAxis ? m_initialAxis : Vector3d(0, 0, 1);
    m_fitResult = GeometryFitter::fitCylinder(m_cloud, axis);

    if (!m_fitResult.success) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "圆柱拟合失败";
        return m_lastResult;
    }

    m_lastResult.value = m_fitResult.getCylindricity();
    m_lastResult.success = true;
    m_lastResult.unit = "mm";
    m_lastResult.stdDev = m_fitResult.rmsError;

    return m_lastResult;
}

SphericityMeasurement::SphericityMeasurement(QObject* parent)
    : Measurement3DTool(parent)
{
}

void SphericityMeasurement::setPointCloud(const PointCloud3D& cloud) {
    m_cloud = cloud;
}

Measurement3DResult SphericityMeasurement::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Sphericity;

    if (m_cloud.size() < 4) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "点数不足";
        return m_lastResult;
    }

    m_fitResult = GeometryFitter::fitSphere(m_cloud);

    if (!m_fitResult.success) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "球体拟合失败";
        return m_lastResult;
    }

    m_lastResult.value = m_fitResult.getSphericity();
    m_lastResult.success = true;
    m_lastResult.unit = "mm";
    m_lastResult.stdDev = m_fitResult.rmsError;

    return m_lastResult;
}

// ============================================================
// 高度/厚度测量
// ============================================================

HeightMeasurement::HeightMeasurement(QObject* parent)
    : Measurement3DTool(parent)
{
}

void HeightMeasurement::setPointCloudAndReference(const PointCloud3D& cloud,
                                                   const Plane3D& basePlane) {
    m_cloud = cloud;
    m_basePlane = basePlane;
}

void HeightMeasurement::setMeasurementRegion(const BoundingBox3D& region) {
    m_region = region;
    m_hasRegion = true;
}

Measurement3DResult HeightMeasurement::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Height;

    if (m_cloud.empty()) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "点云为空";
        return m_lastResult;
    }

    double maxHeight = std::numeric_limits<double>::lowest();
    double minHeight = std::numeric_limits<double>::max();
    double sumHeight = 0;
    int count = 0;

    for (size_t i = 0; i < m_cloud.size(); ++i) {
        const Point3D& point = m_cloud[i];

        if (m_hasRegion && !m_region.contains(point)) {
            continue;
        }

        double height = m_basePlane.distanceToPoint(point.toVector3d());
        maxHeight = std::max(maxHeight, height);
        minHeight = std::min(minHeight, height);
        sumHeight += height;
        ++count;
    }

    if (count == 0) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "区域内无有效点";
        return m_lastResult;
    }

    m_lastResult.value = maxHeight;
    m_lastResult.value2 = sumHeight / count;  // 平均高度
    m_lastResult.value3 = maxHeight - minHeight;  // 高度变化范围
    m_lastResult.success = true;
    m_lastResult.unit = "mm";
    m_lastResult.plane = m_basePlane;

    return m_lastResult;
}

ThicknessMeasurement::ThicknessMeasurement(QObject* parent)
    : Measurement3DTool(parent)
{
}

void ThicknessMeasurement::setSurfaces(const PointCloud3D& topSurface,
                                        const PointCloud3D& bottomSurface) {
    m_topSurface = topSurface;
    m_bottomSurface = bottomSurface;
}

Measurement3DResult ThicknessMeasurement::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Thickness;

    if (m_topSurface.empty() || m_bottomSurface.empty()) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "表面点云为空";
        return m_lastResult;
    }

    // 拟合两个平面
    PlaneFitResult topFit = GeometryFitter::fitPlane(m_topSurface);
    PlaneFitResult bottomFit = GeometryFitter::fitPlane(m_bottomSurface);

    if (!topFit.success || !bottomFit.success) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "平面拟合失败";
        return m_lastResult;
    }

    // 计算两平面之间的距离
    Vector3d topNormal = topFit.plane.normal();
    Vector3d bottomNormal = bottomFit.plane.normal();

    // 取顶面质心到底面的距离作为厚度
    Point3D topCentroid = m_topSurface.computeCentroid();
    double thickness = bottomFit.plane.distanceToPoint(topCentroid.toVector3d());

    m_lastResult.value = std::abs(thickness);
    m_lastResult.success = true;
    m_lastResult.unit = "mm";
    m_lastResult.plane = topFit.plane;

    return m_lastResult;
}

// ============================================================
// 间隙/平齐度测量
// ============================================================

GapMeasurement::GapMeasurement(QObject* parent)
    : Measurement3DTool(parent)
{
}

void GapMeasurement::setEdges(const PointCloud3D& edge1, const PointCloud3D& edge2) {
    m_edge1 = edge1;
    m_edge2 = edge2;
}

void GapMeasurement::setMeasurementDirection(const Vector3d& direction) {
    m_direction = direction;
    m_hasDirection = true;
}

Measurement3DResult GapMeasurement::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Gap;

    if (m_edge1.empty() || m_edge2.empty()) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "边缘点云为空";
        return m_lastResult;
    }

    // 计算两个边缘之间的最小距离
    double minGap = std::numeric_limits<double>::max();
    double sumGap = 0;
    int count = 0;

    for (size_t i = 0; i < m_edge1.size(); ++i) {
        size_t nearestIdx = m_edge2.findNearestPoint(m_edge1[i]);
        double gap = m_edge1[i].distanceTo(m_edge2[nearestIdx]);

        if (m_hasDirection) {
            Vector3d diff = (m_edge2[nearestIdx] - m_edge1[i]).toVector3d();
            gap = std::abs(diff.dot(m_direction.normalized()));
        }

        minGap = std::min(minGap, gap);
        sumGap += gap;
        ++count;
    }

    m_lastResult.value = minGap;
    m_lastResult.value2 = sumGap / count;  // 平均间隙
    m_lastResult.success = true;
    m_lastResult.unit = "mm";

    return m_lastResult;
}

FlushMeasurement::FlushMeasurement(QObject* parent)
    : Measurement3DTool(parent)
{
}

void FlushMeasurement::setSurfaces(const PointCloud3D& surface1,
                                    const PointCloud3D& surface2) {
    m_surface1 = surface1;
    m_surface2 = surface2;
}

Measurement3DResult FlushMeasurement::measure() {
    m_lastResult = Measurement3DResult();
    m_lastResult.type = Measurement3DType::Flush;

    if (m_surface1.empty() || m_surface2.empty()) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "表面点云为空";
        return m_lastResult;
    }

    // 拟合两个平面
    PlaneFitResult fit1 = GeometryFitter::fitPlane(m_surface1);
    PlaneFitResult fit2 = GeometryFitter::fitPlane(m_surface2);

    if (!fit1.success || !fit2.success) {
        m_lastResult.success = false;
        m_lastResult.errorMessage = "平面拟合失败";
        return m_lastResult;
    }

    // 计算两平面的高度差
    Point3D centroid1 = m_surface1.computeCentroid();
    Point3D centroid2 = m_surface2.computeCentroid();

    // 使用拟合平面的法线方向计算高度差
    Vector3d avgNormal = (fit1.plane.normal() + fit2.plane.normal()).normalized();
    Vector3d diff = centroid2.toVector3d() - centroid1.toVector3d();
    double flush = diff.dot(avgNormal);

    m_lastResult.value = std::abs(flush);
    m_lastResult.success = true;
    m_lastResult.unit = "mm";

    return m_lastResult;
}

// ============================================================
// GeometryFitter
// ============================================================

PlaneFitResult GeometryFitter::fitPlane(const PointCloud3D& cloud) {
    PlaneFitResult result;

    if (cloud.size() < 3) {
        result.success = false;
        return result;
    }

    // 计算质心
    Point3D centroid = cloud.computeCentroid();
    Vector3d center(centroid.x, centroid.y, centroid.z);

    // 计算协方差矩阵
    Matrix3d cov = Matrix3d::Zero();
    for (size_t i = 0; i < cloud.size(); ++i) {
        Vector3d p(cloud[i].x - centroid.x,
                  cloud[i].y - centroid.y,
                  cloud[i].z - centroid.z);
        cov += p * p.transpose();
    }

    // SVD分解
    Eigen::JacobiSVD<Matrix3d> svd(cov, Eigen::ComputeFullV);
    Vector3d normal = svd.matrixV().col(2);  // 最小特征值对应的特征向量

    result.plane = Plane3D::fromNormalAndPoint(normal, center);
    result.success = true;

    // 计算误差
    double sumSqError = 0;
    double maxError = 0;

    for (size_t i = 0; i < cloud.size(); ++i) {
        double error = result.plane.distanceToPoint(cloud[i].toVector3d());
        sumSqError += error * error;
        if (error > maxError) {
            maxError = error;
        }
        result.inliers.push_back(i);
    }

    result.rmsError = std::sqrt(sumSqError / cloud.size());
    result.maxError = maxError;
    result.inlierCount = static_cast<int>(cloud.size());

    return result;
}

PlaneFitResult GeometryFitter::fitPlaneRANSAC(const PointCloud3D& cloud,
                                               double threshold,
                                               int maxIterations) {
    PlaneFitResult bestResult;

    if (cloud.size() < 3) {
        bestResult.success = false;
        return bestResult;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, cloud.size() - 1);

    for (int iter = 0; iter < maxIterations; ++iter) {
        // 随机选择3个点
        size_t i1 = dis(gen);
        size_t i2 = dis(gen);
        size_t i3 = dis(gen);

        if (i1 == i2 || i2 == i3 || i1 == i3) {
            continue;
        }

        Vector3d p1 = cloud[i1].toVector3d();
        Vector3d p2 = cloud[i2].toVector3d();
        Vector3d p3 = cloud[i3].toVector3d();

        Vector3d v1 = p2 - p1;
        Vector3d v2 = p3 - p1;
        Vector3d normal = v1.cross(v2);

        if (normal.norm() < 1e-6) {
            continue;
        }

        normal.normalize();
        Plane3D plane = Plane3D::fromNormalAndPoint(normal, p1);

        // 计算内点
        std::vector<size_t> inliers;
        double sumSqError = 0;
        double maxError = 0;

        for (size_t i = 0; i < cloud.size(); ++i) {
            double error = plane.distanceToPoint(cloud[i].toVector3d());
            if (error <= threshold) {
                inliers.push_back(i);
                sumSqError += error * error;
                maxError = std::max(maxError, error);
            }
        }

        if (inliers.size() > bestResult.inliers.size()) {
            bestResult.plane = plane;
            bestResult.inliers = inliers;
            bestResult.inlierCount = static_cast<int>(inliers.size());
            bestResult.rmsError = std::sqrt(sumSqError / inliers.size());
            bestResult.maxError = maxError;
            bestResult.success = true;
        }
    }

    return bestResult;
}

LineFitResult GeometryFitter::fitLine(const PointCloud3D& cloud) {
    LineFitResult result;

    if (cloud.size() < 2) {
        result.success = false;
        return result;
    }

    // 使用PCA拟合直线
    Point3D centroid = cloud.computeCentroid();
    Matrix3d cov = cloud.computeCovarianceMatrix();

    Eigen::SelfAdjointEigenSolver<Matrix3d> solver(cov);
    Vector3d direction = solver.eigenvectors().col(2);  // 最大特征值对应的方向

    // 找到直线两端点
    double minProj = std::numeric_limits<double>::max();
    double maxProj = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < cloud.size(); ++i) {
        Vector3d p = cloud[i].toVector3d() - centroid.toVector3d();
        double proj = p.dot(direction);
        minProj = std::min(minProj, proj);
        maxProj = std::max(maxProj, proj);
    }

    Vector3d c = centroid.toVector3d();
    result.line.start = Point3D::fromVector3d(c + minProj * direction);
    result.line.end = Point3D::fromVector3d(c + maxProj * direction);
    result.success = true;

    // 计算误差
    double sumSqError = 0;
    double maxError = 0;

    for (size_t i = 0; i < cloud.size(); ++i) {
        Vector3d p = cloud[i].toVector3d();
        Vector3d ap = p - c;
        double proj = ap.dot(direction);
        Vector3d closestPoint = c + proj * direction;
        double error = (p - closestPoint).norm();

        sumSqError += error * error;
        maxError = std::max(maxError, error);
    }

    result.rmsError = std::sqrt(sumSqError / cloud.size());
    result.maxError = maxError;
    result.inlierCount = static_cast<int>(cloud.size());

    return result;
}

LineFitResult GeometryFitter::fitLineRANSAC(const PointCloud3D& cloud,
                                             double threshold,
                                             int maxIterations) {
    Q_UNUSED(threshold);
    Q_UNUSED(maxIterations);
    // 简化实现
    return fitLine(cloud);
}

CylinderFitResult GeometryFitter::fitCylinder(const PointCloud3D& cloud,
                                               const Vector3d& initialAxis) {
    CylinderFitResult result;

    if (cloud.size() < 6) {
        result.success = false;
        return result;
    }

    // 简化实现：使用初始轴方向，计算点到轴的距离作为半径
    Point3D centroid = cloud.computeCentroid();
    Vector3d axisPoint = centroid.toVector3d();
    Vector3d axisDir = initialAxis.normalized();

    double sumRadius = 0;
    double maxError = 0;
    double sumSqError = 0;

    for (size_t i = 0; i < cloud.size(); ++i) {
        Vector3d p = cloud[i].toVector3d();
        Vector3d ap = p - axisPoint;
        double proj = ap.dot(axisDir);
        Vector3d closestPoint = axisPoint + proj * axisDir;
        double radius = (p - closestPoint).norm();
        sumRadius += radius;
    }

    double avgRadius = sumRadius / cloud.size();

    // 计算误差
    for (size_t i = 0; i < cloud.size(); ++i) {
        Vector3d p = cloud[i].toVector3d();
        Vector3d ap = p - axisPoint;
        double proj = ap.dot(axisDir);
        Vector3d closestPoint = axisPoint + proj * axisDir;
        double radius = (p - closestPoint).norm();
        double error = std::abs(radius - avgRadius);
        sumSqError += error * error;
        maxError = std::max(maxError, error);
    }

    result.cylinder.axisPoint = axisPoint;
    result.cylinder.axisDirection = axisDir;
    result.cylinder.radius = avgRadius;
    result.rmsError = std::sqrt(sumSqError / cloud.size());
    result.maxError = maxError;
    result.inlierCount = static_cast<int>(cloud.size());
    result.success = true;

    return result;
}

CylinderFitResult GeometryFitter::fitCylinderRANSAC(const PointCloud3D& cloud,
                                                     double threshold,
                                                     int maxIterations) {
    Q_UNUSED(threshold);
    Q_UNUSED(maxIterations);
    return fitCylinder(cloud);
}

SphereFitResult GeometryFitter::fitSphere(const PointCloud3D& cloud) {
    SphereFitResult result;

    if (cloud.size() < 4) {
        result.success = false;
        return result;
    }

    // 使用最小二乘拟合球体
    // 球方程: (x-a)^2 + (y-b)^2 + (z-c)^2 = r^2
    // 展开: x^2 + y^2 + z^2 = 2ax + 2by + 2cz + (r^2 - a^2 - b^2 - c^2)

    Eigen::MatrixXd A(static_cast<int>(cloud.size()), 4);
    Eigen::VectorXd b(static_cast<int>(cloud.size()));

    for (size_t i = 0; i < cloud.size(); ++i) {
        int row = static_cast<int>(i);
        A(row, 0) = 2.0 * cloud[i].x;
        A(row, 1) = 2.0 * cloud[i].y;
        A(row, 2) = 2.0 * cloud[i].z;
        A(row, 3) = 1.0;
        b(row) = cloud[i].x * cloud[i].x +
                 cloud[i].y * cloud[i].y +
                 cloud[i].z * cloud[i].z;
    }

    // 最小二乘求解
    Eigen::VectorXd x = A.householderQr().solve(b);

    Vector3d center(x(0), x(1), x(2));
    double radius = std::sqrt(x(3) + center.squaredNorm());

    result.sphere.center = center;
    result.sphere.radius = radius;

    // 计算误差
    double sumSqError = 0;
    double maxError = 0;

    for (size_t i = 0; i < cloud.size(); ++i) {
        double dist = (cloud[i].toVector3d() - center).norm();
        double error = std::abs(dist - radius);
        sumSqError += error * error;
        maxError = std::max(maxError, error);
    }

    result.rmsError = std::sqrt(sumSqError / cloud.size());
    result.maxError = maxError;
    result.inlierCount = static_cast<int>(cloud.size());
    result.success = true;

    return result;
}

SphereFitResult GeometryFitter::fitSphereRANSAC(const PointCloud3D& cloud,
                                                 double threshold,
                                                 int maxIterations) {
    Q_UNUSED(threshold);
    Q_UNUSED(maxIterations);
    return fitSphere(cloud);
}

CircleFitResult GeometryFitter::fitCircle(const PointCloud3D& cloud, const Plane3D& plane) {
    CircleFitResult result;

    if (cloud.size() < 3) {
        result.success = false;
        return result;
    }

    // 将点投影到平面上
    std::vector<Vector3d> projectedPoints;
    for (size_t i = 0; i < cloud.size(); ++i) {
        Vector3d p = cloud[i].toVector3d();
        Vector3d projected = plane.projectPoint(p);
        projectedPoints.push_back(projected);
    }

    // 2D圆拟合（在平面局部坐标系中）
    Vector3d normal = plane.normal();
    Vector3d u = Vector3d::UnitX();
    if (std::abs(normal.dot(u)) > 0.9) {
        u = Vector3d::UnitY();
    }
    Vector3d xAxis = (u - u.dot(normal) * normal).normalized();
    Vector3d yAxis = normal.cross(xAxis);

    // 转换到2D
    std::vector<Eigen::Vector2d> points2D;
    Vector3d center3D = Vector3d::Zero();
    for (const auto& p : projectedPoints) {
        center3D += p;
    }
    center3D /= static_cast<double>(projectedPoints.size());

    for (const auto& p : projectedPoints) {
        Vector3d diff = p - center3D;
        points2D.emplace_back(diff.dot(xAxis), diff.dot(yAxis));
    }

    // 2D圆拟合
    Eigen::MatrixXd A(static_cast<int>(points2D.size()), 3);
    Eigen::VectorXd b(static_cast<int>(points2D.size()));

    for (size_t i = 0; i < points2D.size(); ++i) {
        int row = static_cast<int>(i);
        A(row, 0) = 2.0 * points2D[i].x();
        A(row, 1) = 2.0 * points2D[i].y();
        A(row, 2) = 1.0;
        b(row) = points2D[i].squaredNorm();
    }

    Eigen::VectorXd x = A.householderQr().solve(b);

    Eigen::Vector2d center2D(x(0), x(1));
    double radius = std::sqrt(x(2) + center2D.squaredNorm());

    // 转回3D
    result.center = center3D + center2D.x() * xAxis + center2D.y() * yAxis;
    result.normal = normal;
    result.radius = radius;

    // 计算误差
    double sumSqError = 0;
    double maxError = 0;

    for (const auto& p2d : points2D) {
        double dist = (p2d - center2D).norm();
        double error = std::abs(dist - radius);
        sumSqError += error * error;
        maxError = std::max(maxError, error);
    }

    result.rmsError = std::sqrt(sumSqError / points2D.size());
    result.maxError = maxError;
    result.inlierCount = static_cast<int>(cloud.size());
    result.success = true;

    return result;
}

CircleFitResult GeometryFitter::fitCircleAuto(const PointCloud3D& cloud) {
    // 先拟合平面
    PlaneFitResult planeFit = fitPlane(cloud);
    if (!planeFit.success) {
        CircleFitResult result;
        result.success = false;
        return result;
    }

    return fitCircle(cloud, planeFit.plane);
}

// ============================================================
// Measurement3DManager
// ============================================================

Measurement3DManager::Measurement3DManager(QObject* parent)
    : QObject(parent)
{
}

Measurement3DManager::~Measurement3DManager() = default;

Measurement3DResult Measurement3DManager::measureDistance(const Point3D& p1, const Point3D& p2) {
    PointToPointDistance tool;
    tool.setPoints(p1, p2);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measurePointToPlane(const Point3D& point,
                                                               const Plane3D& plane) {
    PointToPlaneDistance tool;
    tool.setPointAndPlane(point, plane);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureAngle(const Line3D& line1, const Line3D& line2) {
    LineToLineAngle tool;
    tool.setLines(line1, line2);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureAngle(const Plane3D& plane1, const Plane3D& plane2) {
    PlaneToPlaneAngle tool;
    tool.setPlanes(plane1, plane2);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureFlatness(const PointCloud3D& cloud) {
    FlatnessMeasurement tool;
    tool.setPointCloud(cloud);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureCylindricity(const PointCloud3D& cloud) {
    CylindricityMeasurement tool;
    tool.setPointCloud(cloud);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureSphericity(const PointCloud3D& cloud) {
    SphericityMeasurement tool;
    tool.setPointCloud(cloud);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureHeight(const PointCloud3D& cloud,
                                                         const Plane3D& basePlane) {
    HeightMeasurement tool;
    tool.setPointCloudAndReference(cloud, basePlane);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureGap(const PointCloud3D& edge1,
                                                      const PointCloud3D& edge2) {
    GapMeasurement tool;
    tool.setEdges(edge1, edge2);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

Measurement3DResult Measurement3DManager::measureFlush(const PointCloud3D& surface1,
                                                        const PointCloud3D& surface2) {
    FlushMeasurement tool;
    tool.setSurfaces(surface1, surface2);
    Measurement3DResult result = tool.measure();
    emit measurementCompleted(result);
    return result;
}

PlaneFitResult Measurement3DManager::fitPlane(const PointCloud3D& cloud, bool useRANSAC) {
    if (useRANSAC) {
        return GeometryFitter::fitPlaneRANSAC(cloud);
    }
    return GeometryFitter::fitPlane(cloud);
}

LineFitResult Measurement3DManager::fitLine(const PointCloud3D& cloud, bool useRANSAC) {
    if (useRANSAC) {
        return GeometryFitter::fitLineRANSAC(cloud);
    }
    return GeometryFitter::fitLine(cloud);
}

CylinderFitResult Measurement3DManager::fitCylinder(const PointCloud3D& cloud, bool useRANSAC) {
    if (useRANSAC) {
        return GeometryFitter::fitCylinderRANSAC(cloud);
    }
    return GeometryFitter::fitCylinder(cloud);
}

SphereFitResult Measurement3DManager::fitSphere(const PointCloud3D& cloud, bool useRANSAC) {
    if (useRANSAC) {
        return GeometryFitter::fitSphereRANSAC(cloud);
    }
    return GeometryFitter::fitSphere(cloud);
}

CircleFitResult Measurement3DManager::fitCircle(const PointCloud3D& cloud) {
    return GeometryFitter::fitCircleAuto(cloud);
}

} // namespace Algorithm3D
} // namespace VisionForge
