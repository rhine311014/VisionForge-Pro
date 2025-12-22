/**
 * @file PointCloud3D.cpp
 * @brief VisionForge Pro 3D点云类实现
 */

#include "algorithm3d/PointCloud3D.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <algorithm>
#include <random>
#include <cmath>

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// 构造函数和析构函数
// ============================================================

PointCloud3D::PointCloud3D(QObject* parent)
    : QObject(parent)
{
    m_timestamp = QDateTime::currentDateTime();
}

PointCloud3D::PointCloud3D(const PointVector& points, QObject* parent)
    : QObject(parent)
    , m_points(points)
{
    m_timestamp = QDateTime::currentDateTime();
    updateStatus();
}

PointCloud3D::PointCloud3D(const PointCloud3D& other)
    : QObject(other.parent())
    , m_points(other.m_points)
    , m_name(other.m_name)
    , m_description(other.m_description)
    , m_timestamp(other.m_timestamp)
    , m_type(other.m_type)
    , m_status(other.m_status)
    , m_boundingBox(other.m_boundingBox)
    , m_boundingBoxValid(other.m_boundingBoxValid)
    , m_hasNormals(other.m_hasNormals)
    , m_hasColors(other.m_hasColors)
{
}

PointCloud3D::PointCloud3D(PointCloud3D&& other) noexcept
    : QObject(other.parent())
    , m_points(std::move(other.m_points))
    , m_name(std::move(other.m_name))
    , m_description(std::move(other.m_description))
    , m_timestamp(other.m_timestamp)
    , m_type(other.m_type)
    , m_status(other.m_status)
    , m_boundingBox(other.m_boundingBox)
    , m_boundingBoxValid(other.m_boundingBoxValid)
    , m_hasNormals(other.m_hasNormals)
    , m_hasColors(other.m_hasColors)
{
    other.m_status = PointCloudStatus::Empty;
    other.m_boundingBoxValid = false;
}

PointCloud3D::~PointCloud3D() = default;

// ============================================================
// 赋值运算符
// ============================================================

PointCloud3D& PointCloud3D::operator=(const PointCloud3D& other) {
    if (this != &other) {
        QMutexLocker locker(&m_mutex);
        m_points = other.m_points;
        m_name = other.m_name;
        m_description = other.m_description;
        m_timestamp = other.m_timestamp;
        m_type = other.m_type;
        m_status = other.m_status;
        m_boundingBox = other.m_boundingBox;
        m_boundingBoxValid = other.m_boundingBoxValid;
        m_hasNormals = other.m_hasNormals;
        m_hasColors = other.m_hasColors;
    }
    return *this;
}

PointCloud3D& PointCloud3D::operator=(PointCloud3D&& other) noexcept {
    if (this != &other) {
        QMutexLocker locker(&m_mutex);
        m_points = std::move(other.m_points);
        m_name = std::move(other.m_name);
        m_description = std::move(other.m_description);
        m_timestamp = other.m_timestamp;
        m_type = other.m_type;
        m_status = other.m_status;
        m_boundingBox = other.m_boundingBox;
        m_boundingBoxValid = other.m_boundingBoxValid;
        m_hasNormals = other.m_hasNormals;
        m_hasColors = other.m_hasColors;
        other.m_status = PointCloudStatus::Empty;
        other.m_boundingBoxValid = false;
    }
    return *this;
}

// ============================================================
// 点访问
// ============================================================

size_t PointCloud3D::size() const {
    QMutexLocker locker(&m_mutex);
    return m_points.size();
}

bool PointCloud3D::empty() const {
    QMutexLocker locker(&m_mutex);
    return m_points.empty();
}

size_t PointCloud3D::capacity() const {
    QMutexLocker locker(&m_mutex);
    return m_points.capacity();
}

void PointCloud3D::reserve(size_t n) {
    QMutexLocker locker(&m_mutex);
    m_points.reserve(n);
}

void PointCloud3D::resize(size_t n) {
    QMutexLocker locker(&m_mutex);
    m_points.resize(n);
    invalidateCache();
    updateStatus();
}

void PointCloud3D::clear() {
    QMutexLocker locker(&m_mutex);
    m_points.clear();
    invalidateCache();
    m_status = PointCloudStatus::Empty;
    m_hasNormals = false;
    m_hasColors = false;
}

Point3D& PointCloud3D::at(size_t index) {
    QMutexLocker locker(&m_mutex);
    return m_points.at(index);
}

const Point3D& PointCloud3D::at(size_t index) const {
    QMutexLocker locker(&m_mutex);
    return m_points.at(index);
}

Point3D& PointCloud3D::operator[](size_t index) {
    return m_points[index];
}

const Point3D& PointCloud3D::operator[](size_t index) const {
    return m_points[index];
}

Point3D& PointCloud3D::front() {
    return m_points.front();
}

const Point3D& PointCloud3D::front() const {
    return m_points.front();
}

Point3D& PointCloud3D::back() {
    return m_points.back();
}

const Point3D& PointCloud3D::back() const {
    return m_points.back();
}

Point3D* PointCloud3D::data() {
    return m_points.data();
}

const Point3D* PointCloud3D::data() const {
    return m_points.data();
}

// ============================================================
// 迭代器
// ============================================================

PointCloud3D::Iterator PointCloud3D::begin() { return m_points.begin(); }
PointCloud3D::Iterator PointCloud3D::end() { return m_points.end(); }
PointCloud3D::ConstIterator PointCloud3D::begin() const { return m_points.begin(); }
PointCloud3D::ConstIterator PointCloud3D::end() const { return m_points.end(); }
PointCloud3D::ConstIterator PointCloud3D::cbegin() const { return m_points.cbegin(); }
PointCloud3D::ConstIterator PointCloud3D::cend() const { return m_points.cend(); }

// ============================================================
// 点操作
// ============================================================

void PointCloud3D::addPoint(const Point3D& point) {
    QMutexLocker locker(&m_mutex);
    m_points.push_back(point);
    invalidateCache();
    updateStatus();
}

void PointCloud3D::addPoint(float x, float y, float z) {
    addPoint(Point3D(x, y, z));
}

void PointCloud3D::addPoint(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b) {
    addPoint(Point3D(x, y, z, r, g, b));
    m_hasColors = true;
}

void PointCloud3D::addPoints(const PointVector& points) {
    QMutexLocker locker(&m_mutex);
    m_points.insert(m_points.end(), points.begin(), points.end());
    invalidateCache();
    updateStatus();
}

void PointCloud3D::addPoints(const PointCloud3D& other) {
    addPoints(other.m_points);
}

void PointCloud3D::removePoint(size_t index) {
    QMutexLocker locker(&m_mutex);
    if (index < m_points.size()) {
        m_points.erase(m_points.begin() + static_cast<ptrdiff_t>(index));
        invalidateCache();
        updateStatus();
    }
}

void PointCloud3D::removeIf(std::function<bool(const Point3D&)> predicate) {
    QMutexLocker locker(&m_mutex);
    m_points.erase(
        std::remove_if(m_points.begin(), m_points.end(), predicate),
        m_points.end()
    );
    invalidateCache();
    updateStatus();
}

void PointCloud3D::swapPoints(size_t i, size_t j) {
    QMutexLocker locker(&m_mutex);
    if (i < m_points.size() && j < m_points.size()) {
        std::swap(m_points[i], m_points[j]);
    }
}

void PointCloud3D::forEach(PointCallback callback) {
    QMutexLocker locker(&m_mutex);
    for (auto& point : m_points) {
        callback(point);
    }
    invalidateCache();
}

void PointCloud3D::forEach(ConstPointCallback callback) const {
    QMutexLocker locker(&m_mutex);
    for (const auto& point : m_points) {
        callback(point);
    }
}

// ============================================================
// 几何计算
// ============================================================

BoundingBox3D PointCloud3D::computeBoundingBox() const {
    QMutexLocker locker(&m_mutex);
    BoundingBox3D box;
    for (const auto& point : m_points) {
        if (point.isValid()) {
            box.extend(point);
        }
    }
    return box;
}

const BoundingBox3D& PointCloud3D::getBoundingBox() {
    QMutexLocker locker(&m_mutex);
    if (!m_boundingBoxValid) {
        m_boundingBox = BoundingBox3D();
        for (const auto& point : m_points) {
            if (point.isValid()) {
                m_boundingBox.extend(point);
            }
        }
        m_boundingBoxValid = true;
    }
    return m_boundingBox;
}

Point3D PointCloud3D::computeCentroid() const {
    QMutexLocker locker(&m_mutex);
    if (m_points.empty()) {
        return Point3D();
    }

    double sumX = 0, sumY = 0, sumZ = 0;
    size_t validCount = 0;

    for (const auto& point : m_points) {
        if (point.isValid()) {
            sumX += point.x;
            sumY += point.y;
            sumZ += point.z;
            ++validCount;
        }
    }

    if (validCount == 0) {
        return Point3D();
    }

    return Point3D(
        static_cast<float>(sumX / validCount),
        static_cast<float>(sumY / validCount),
        static_cast<float>(sumZ / validCount)
    );
}

Matrix3d PointCloud3D::computeCovarianceMatrix() const {
    QMutexLocker locker(&m_mutex);
    Matrix3d cov = Matrix3d::Zero();

    if (m_points.size() < 3) {
        return cov;
    }

    // 计算质心
    Point3D centroid = computeCentroid();

    for (const auto& point : m_points) {
        if (point.isValid()) {
            Vector3d diff(point.x - centroid.x,
                         point.y - centroid.y,
                         point.z - centroid.z);
            cov += diff * diff.transpose();
        }
    }

    cov /= static_cast<double>(m_points.size() - 1);
    return cov;
}

void PointCloud3D::computePCA(Vector3d& eigenValues, Matrix3d& eigenVectors) const {
    Matrix3d cov = computeCovarianceMatrix();

    Eigen::SelfAdjointEigenSolver<Matrix3d> solver(cov);
    eigenValues = solver.eigenvalues();
    eigenVectors = solver.eigenvectors();
}

size_t PointCloud3D::findNearestPoint(const Point3D& query) const {
    QMutexLocker locker(&m_mutex);
    if (m_points.empty()) {
        return 0;
    }

    size_t nearestIdx = 0;
    float minDist = std::numeric_limits<float>::max();

    for (size_t i = 0; i < m_points.size(); ++i) {
        float dist = query.distanceTo(m_points[i]);
        if (dist < minDist) {
            minDist = dist;
            nearestIdx = i;
        }
    }

    return nearestIdx;
}

std::vector<size_t> PointCloud3D::findPointsInRadius(const Point3D& query, float radius) const {
    QMutexLocker locker(&m_mutex);
    std::vector<size_t> result;

    for (size_t i = 0; i < m_points.size(); ++i) {
        if (query.distanceTo(m_points[i]) <= radius) {
            result.push_back(i);
        }
    }

    return result;
}

std::vector<size_t> PointCloud3D::findKNearestNeighbors(const Point3D& query, int k) const {
    QMutexLocker locker(&m_mutex);

    std::vector<std::pair<float, size_t>> distances;
    distances.reserve(m_points.size());

    for (size_t i = 0; i < m_points.size(); ++i) {
        distances.emplace_back(query.distanceTo(m_points[i]), i);
    }

    std::partial_sort(distances.begin(),
                      distances.begin() + std::min(static_cast<size_t>(k), distances.size()),
                      distances.end());

    std::vector<size_t> result;
    result.reserve(static_cast<size_t>(k));
    for (int i = 0; i < k && i < static_cast<int>(distances.size()); ++i) {
        result.push_back(distances[static_cast<size_t>(i)].second);
    }

    return result;
}

// ============================================================
// 变换
// ============================================================

void PointCloud3D::transform(const Matrix4d& transformMatrix) {
    QMutexLocker locker(&m_mutex);

    Matrix3d rotation = transformMatrix.block<3, 3>(0, 0);
    Vector3d translation = transformMatrix.block<3, 1>(0, 3);

    for (auto& point : m_points) {
        Vector3d p(point.x, point.y, point.z);
        Vector3d transformed = rotation * p + translation;
        point.x = static_cast<float>(transformed.x());
        point.y = static_cast<float>(transformed.y());
        point.z = static_cast<float>(transformed.z());

        // 变换法线（仅旋转）
        if (m_hasNormals) {
            Vector3d n(point.normal_x, point.normal_y, point.normal_z);
            Vector3d transformedN = rotation * n;
            point.normal_x = static_cast<float>(transformedN.x());
            point.normal_y = static_cast<float>(transformedN.y());
            point.normal_z = static_cast<float>(transformedN.z());
        }
    }

    invalidateCache();
}

void PointCloud3D::translate(float dx, float dy, float dz) {
    translate(Vector3d(dx, dy, dz));
}

void PointCloud3D::translate(const Vector3d& offset) {
    QMutexLocker locker(&m_mutex);

    for (auto& point : m_points) {
        point.x += static_cast<float>(offset.x());
        point.y += static_cast<float>(offset.y());
        point.z += static_cast<float>(offset.z());
    }

    invalidateCache();
}

void PointCloud3D::rotate(const Matrix3d& rotationMatrix) {
    QMutexLocker locker(&m_mutex);

    for (auto& point : m_points) {
        Vector3d p(point.x, point.y, point.z);
        Vector3d rotated = rotationMatrix * p;
        point.x = static_cast<float>(rotated.x());
        point.y = static_cast<float>(rotated.y());
        point.z = static_cast<float>(rotated.z());

        if (m_hasNormals) {
            Vector3d n(point.normal_x, point.normal_y, point.normal_z);
            Vector3d rotatedN = rotationMatrix * n;
            point.normal_x = static_cast<float>(rotatedN.x());
            point.normal_y = static_cast<float>(rotatedN.y());
            point.normal_z = static_cast<float>(rotatedN.z());
        }
    }

    invalidateCache();
}

void PointCloud3D::rotate(const Quaterniond& quaternion) {
    rotate(quaternion.toRotationMatrix());
}

void PointCloud3D::rotateAroundAxis(const Vector3d& axis, double angle) {
    Eigen::AngleAxisd rotation(angle, axis.normalized());
    rotate(rotation.toRotationMatrix());
}

void PointCloud3D::scale(float factor) {
    scale(factor, factor, factor);
}

void PointCloud3D::scale(float sx, float sy, float sz) {
    QMutexLocker locker(&m_mutex);

    for (auto& point : m_points) {
        point.x *= sx;
        point.y *= sy;
        point.z *= sz;
    }

    invalidateCache();
}

void PointCloud3D::centerToOrigin() {
    Point3D centroid = computeCentroid();
    translate(-centroid.x, -centroid.y, -centroid.z);
}

PointCloud3D PointCloud3D::transformed(const Matrix4d& transformMatrix) const {
    PointCloud3D result(*this);
    result.transform(transformMatrix);
    return result;
}

// ============================================================
// 法线计算
// ============================================================

void PointCloud3D::estimateNormals(int kNeighbors) {
    QMutexLocker locker(&m_mutex);

    if (m_points.size() < 3) {
        return;
    }

    for (size_t i = 0; i < m_points.size(); ++i) {
        std::vector<size_t> neighbors = findKNearestNeighbors(m_points[i], kNeighbors);

        if (neighbors.size() < 3) {
            continue;
        }

        // 计算局部协方差矩阵
        Point3D centroid;
        for (size_t idx : neighbors) {
            centroid.x += m_points[idx].x;
            centroid.y += m_points[idx].y;
            centroid.z += m_points[idx].z;
        }
        centroid.x /= static_cast<float>(neighbors.size());
        centroid.y /= static_cast<float>(neighbors.size());
        centroid.z /= static_cast<float>(neighbors.size());

        Matrix3d cov = Matrix3d::Zero();
        for (size_t idx : neighbors) {
            Vector3d diff(m_points[idx].x - centroid.x,
                         m_points[idx].y - centroid.y,
                         m_points[idx].z - centroid.z);
            cov += diff * diff.transpose();
        }

        // 最小特征值对应的特征向量是法线方向
        Eigen::SelfAdjointEigenSolver<Matrix3d> solver(cov);
        Vector3d normal = solver.eigenvectors().col(0);

        m_points[i].normal_x = static_cast<float>(normal.x());
        m_points[i].normal_y = static_cast<float>(normal.y());
        m_points[i].normal_z = static_cast<float>(normal.z());

        // 曲率估计
        Vector3d eigenValues = solver.eigenvalues();
        double curvature = eigenValues(0) / (eigenValues(0) + eigenValues(1) + eigenValues(2));
        m_points[i].curvature = static_cast<float>(curvature);

        if ((i + 1) % 1000 == 0) {
            emit processingProgress(static_cast<int>(100 * i / m_points.size()));
        }
    }

    m_hasNormals = true;
}

void PointCloud3D::estimateNormalsRadius(double radius) {
    QMutexLocker locker(&m_mutex);

    for (size_t i = 0; i < m_points.size(); ++i) {
        std::vector<size_t> neighbors = findPointsInRadius(m_points[i], static_cast<float>(radius));

        if (neighbors.size() < 3) {
            continue;
        }

        // 计算局部协方差矩阵
        Point3D centroid;
        for (size_t idx : neighbors) {
            centroid.x += m_points[idx].x;
            centroid.y += m_points[idx].y;
            centroid.z += m_points[idx].z;
        }
        centroid.x /= static_cast<float>(neighbors.size());
        centroid.y /= static_cast<float>(neighbors.size());
        centroid.z /= static_cast<float>(neighbors.size());

        Matrix3d cov = Matrix3d::Zero();
        for (size_t idx : neighbors) {
            Vector3d diff(m_points[idx].x - centroid.x,
                         m_points[idx].y - centroid.y,
                         m_points[idx].z - centroid.z);
            cov += diff * diff.transpose();
        }

        Eigen::SelfAdjointEigenSolver<Matrix3d> solver(cov);
        Vector3d normal = solver.eigenvectors().col(0);

        m_points[i].normal_x = static_cast<float>(normal.x());
        m_points[i].normal_y = static_cast<float>(normal.y());
        m_points[i].normal_z = static_cast<float>(normal.z());
    }

    m_hasNormals = true;
}

void PointCloud3D::orientNormalsTowardsViewpoint(const Vector3d& viewpoint) {
    QMutexLocker locker(&m_mutex);

    for (auto& point : m_points) {
        Vector3d toViewpoint(viewpoint.x() - point.x,
                            viewpoint.y() - point.y,
                            viewpoint.z() - point.z);
        Vector3d normal(point.normal_x, point.normal_y, point.normal_z);

        if (normal.dot(toViewpoint) < 0) {
            point.normal_x = -point.normal_x;
            point.normal_y = -point.normal_y;
            point.normal_z = -point.normal_z;
        }
    }
}

void PointCloud3D::flipNormals() {
    QMutexLocker locker(&m_mutex);

    for (auto& point : m_points) {
        point.normal_x = -point.normal_x;
        point.normal_y = -point.normal_y;
        point.normal_z = -point.normal_z;
    }
}

bool PointCloud3D::hasNormals() const {
    return m_hasNormals;
}

// ============================================================
// 滤波
// ============================================================

PointCloud3D PointCloud3D::statisticalOutlierRemoval(int meanK, double stddevMul) const {
    QMutexLocker locker(&m_mutex);
    PointCloud3D result;

    if (m_points.size() < static_cast<size_t>(meanK)) {
        return result;
    }

    // 计算每个点到其K近邻的平均距离
    std::vector<double> meanDistances(m_points.size());

    for (size_t i = 0; i < m_points.size(); ++i) {
        std::vector<size_t> neighbors = findKNearestNeighbors(m_points[i], meanK + 1);
        double sumDist = 0;
        for (size_t j = 1; j < neighbors.size(); ++j) {
            sumDist += m_points[i].distanceTo(m_points[neighbors[j]]);
        }
        meanDistances[i] = sumDist / static_cast<double>(meanK);
    }

    // 计算全局平均距离和标准差
    double globalMean = 0;
    for (double d : meanDistances) {
        globalMean += d;
    }
    globalMean /= static_cast<double>(meanDistances.size());

    double variance = 0;
    for (double d : meanDistances) {
        variance += (d - globalMean) * (d - globalMean);
    }
    double stdDev = std::sqrt(variance / static_cast<double>(meanDistances.size()));

    // 过滤
    double threshold = globalMean + stddevMul * stdDev;
    for (size_t i = 0; i < m_points.size(); ++i) {
        if (meanDistances[i] <= threshold) {
            result.addPoint(m_points[i]);
        }
    }

    return result;
}

PointCloud3D PointCloud3D::radiusOutlierRemoval(double radius, int minNeighbors) const {
    QMutexLocker locker(&m_mutex);
    PointCloud3D result;

    for (size_t i = 0; i < m_points.size(); ++i) {
        std::vector<size_t> neighbors = findPointsInRadius(m_points[i], static_cast<float>(radius));
        if (static_cast<int>(neighbors.size()) >= minNeighbors) {
            result.addPoint(m_points[i]);
        }
    }

    return result;
}

PointCloud3D PointCloud3D::voxelGridDownsample(double leafSize) const {
    QMutexLocker locker(&m_mutex);
    PointCloud3D result;

    if (m_points.empty() || leafSize <= 0) {
        return result;
    }

    BoundingBox3D box = computeBoundingBox();
    Point3D size = box.size();

    int nx = static_cast<int>(std::ceil(size.x / leafSize));
    int ny = static_cast<int>(std::ceil(size.y / leafSize));
    int nz = static_cast<int>(std::ceil(size.z / leafSize));

    // 使用map存储每个体素的点
    std::map<std::tuple<int, int, int>, std::vector<size_t>> voxelMap;

    for (size_t i = 0; i < m_points.size(); ++i) {
        int vx = static_cast<int>((m_points[i].x - box.minPoint.x) / leafSize);
        int vy = static_cast<int>((m_points[i].y - box.minPoint.y) / leafSize);
        int vz = static_cast<int>((m_points[i].z - box.minPoint.z) / leafSize);

        vx = std::min(vx, nx - 1);
        vy = std::min(vy, ny - 1);
        vz = std::min(vz, nz - 1);

        voxelMap[{vx, vy, vz}].push_back(i);
    }

    // 计算每个体素的质心
    for (const auto& [key, indices] : voxelMap) {
        Point3D centroid;
        for (size_t idx : indices) {
            centroid.x += m_points[idx].x;
            centroid.y += m_points[idx].y;
            centroid.z += m_points[idx].z;
            centroid.r += m_points[idx].r;
            centroid.g += m_points[idx].g;
            centroid.b += m_points[idx].b;
        }

        float n = static_cast<float>(indices.size());
        centroid.x /= n;
        centroid.y /= n;
        centroid.z /= n;
        centroid.r = static_cast<uint8_t>(centroid.r / indices.size());
        centroid.g = static_cast<uint8_t>(centroid.g / indices.size());
        centroid.b = static_cast<uint8_t>(centroid.b / indices.size());

        result.addPoint(centroid);
    }

    return result;
}

PointCloud3D PointCloud3D::passThroughFilter(const QString& axis, double min, double max) const {
    QMutexLocker locker(&m_mutex);
    PointCloud3D result;

    for (const auto& point : m_points) {
        double value;
        if (axis.toLower() == "x") {
            value = point.x;
        } else if (axis.toLower() == "y") {
            value = point.y;
        } else {
            value = point.z;
        }

        if (value >= min && value <= max) {
            result.addPoint(point);
        }
    }

    return result;
}

PointCloud3D PointCloud3D::applyFilter(const PointCloudConfig& config) const {
    PointCloud3D result = *this;

    switch (config.filterType) {
        case PointCloudFilterType::StatisticalOutlier:
            result = result.statisticalOutlierRemoval(config.statisticalMeanK,
                                                       config.statisticalStddevMul);
            break;
        case PointCloudFilterType::RadiusOutlier:
            result = result.radiusOutlierRemoval(config.radiusSearchRadius,
                                                  config.radiusMinNeighbors);
            break;
        case PointCloudFilterType::VoxelGrid:
            result = result.voxelGridDownsample(config.voxelLeafSize);
            break;
        default:
            break;
    }

    if (config.enableDownsampling && config.filterType != PointCloudFilterType::VoxelGrid) {
        result = result.voxelGridDownsample(config.voxelLeafSize);
    }

    if (config.computeNormals) {
        if (config.normalRadiusSearch > 0) {
            result.estimateNormalsRadius(config.normalRadiusSearch);
        } else {
            result.estimateNormals(config.normalKSearch);
        }
    }

    return result;
}

// ============================================================
// 分割
// ============================================================

bool PointCloud3D::segmentPlane(Plane3D& plane, std::vector<size_t>& inliers,
                                 double distanceThreshold, int maxIterations) const {
    QMutexLocker locker(&m_mutex);

    if (m_points.size() < 3) {
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, m_points.size() - 1);

    std::vector<size_t> bestInliers;
    Plane3D bestPlane;

    for (int iter = 0; iter < maxIterations; ++iter) {
        // 随机选择3个点
        size_t i1 = dis(gen);
        size_t i2 = dis(gen);
        size_t i3 = dis(gen);

        if (i1 == i2 || i2 == i3 || i1 == i3) {
            continue;
        }

        Vector3d p1(m_points[i1].x, m_points[i1].y, m_points[i1].z);
        Vector3d p2(m_points[i2].x, m_points[i2].y, m_points[i2].z);
        Vector3d p3(m_points[i3].x, m_points[i3].y, m_points[i3].z);

        Vector3d v1 = p2 - p1;
        Vector3d v2 = p3 - p1;
        Vector3d normal = v1.cross(v2);

        if (normal.norm() < 1e-6) {
            continue;
        }

        normal.normalize();
        Plane3D candidatePlane = Plane3D::fromNormalAndPoint(normal, p1);

        // 计算内点
        std::vector<size_t> currentInliers;
        for (size_t i = 0; i < m_points.size(); ++i) {
            double dist = candidatePlane.distanceToPoint(
                Vector3d(m_points[i].x, m_points[i].y, m_points[i].z));
            if (dist <= distanceThreshold) {
                currentInliers.push_back(i);
            }
        }

        if (currentInliers.size() > bestInliers.size()) {
            bestInliers = currentInliers;
            bestPlane = candidatePlane;
        }
    }

    if (bestInliers.size() < 3) {
        return false;
    }

    plane = bestPlane;
    inliers = bestInliers;
    return true;
}

PointCloud3D PointCloud3D::extractPlaneInliers(double distanceThreshold) const {
    Plane3D plane;
    std::vector<size_t> inliers;

    if (!segmentPlane(plane, inliers, distanceThreshold)) {
        return PointCloud3D();
    }

    return extract(inliers);
}

PointCloud3D PointCloud3D::extractPlaneOutliers(double distanceThreshold) const {
    Plane3D plane;
    std::vector<size_t> inliers;

    if (!segmentPlane(plane, inliers, distanceThreshold)) {
        return *this;
    }

    std::set<size_t> inlierSet(inliers.begin(), inliers.end());
    std::vector<size_t> outliers;

    for (size_t i = 0; i < m_points.size(); ++i) {
        if (inlierSet.find(i) == inlierSet.end()) {
            outliers.push_back(i);
        }
    }

    return extract(outliers);
}

std::vector<PointCloud3D> PointCloud3D::euclideanClustering(double clusterTolerance,
                                                             int minClusterSize,
                                                             int maxClusterSize) const {
    QMutexLocker locker(&m_mutex);
    std::vector<PointCloud3D> clusters;

    if (m_points.empty()) {
        return clusters;
    }

    std::vector<bool> processed(m_points.size(), false);

    for (size_t i = 0; i < m_points.size(); ++i) {
        if (processed[i]) {
            continue;
        }

        std::vector<size_t> clusterIndices;
        std::vector<size_t> queue;
        queue.push_back(i);
        processed[i] = true;

        while (!queue.empty()) {
            size_t current = queue.back();
            queue.pop_back();
            clusterIndices.push_back(current);

            std::vector<size_t> neighbors = findPointsInRadius(
                m_points[current], static_cast<float>(clusterTolerance));

            for (size_t neighbor : neighbors) {
                if (!processed[neighbor]) {
                    processed[neighbor] = true;
                    queue.push_back(neighbor);
                }
            }
        }

        int clusterSize = static_cast<int>(clusterIndices.size());
        if (clusterSize >= minClusterSize && clusterSize <= maxClusterSize) {
            clusters.push_back(extract(clusterIndices));
        }
    }

    return clusters;
}

// ============================================================
// 配准
// ============================================================

Matrix4d PointCloud3D::alignICP(const PointCloud3D& target,
                                 double maxCorrespondenceDistance,
                                 int maxIterations,
                                 double* finalScore) const {
    Matrix4d transform = Matrix4d::Identity();

    PointCloud3D source = *this;

    for (int iter = 0; iter < maxIterations; ++iter) {
        // 找对应点
        std::vector<std::pair<size_t, size_t>> correspondences;

        for (size_t i = 0; i < source.size(); ++i) {
            size_t nearestIdx = target.findNearestPoint(source[i]);
            double dist = source[i].distanceTo(target[nearestIdx]);

            if (dist <= maxCorrespondenceDistance) {
                correspondences.emplace_back(i, nearestIdx);
            }
        }

        if (correspondences.size() < 3) {
            break;
        }

        // 计算变换
        Point3D srcCentroid, tgtCentroid;
        for (const auto& [srcIdx, tgtIdx] : correspondences) {
            srcCentroid.x += source[srcIdx].x;
            srcCentroid.y += source[srcIdx].y;
            srcCentroid.z += source[srcIdx].z;
            tgtCentroid.x += target[tgtIdx].x;
            tgtCentroid.y += target[tgtIdx].y;
            tgtCentroid.z += target[tgtIdx].z;
        }

        float n = static_cast<float>(correspondences.size());
        srcCentroid.x /= n;
        srcCentroid.y /= n;
        srcCentroid.z /= n;
        tgtCentroid.x /= n;
        tgtCentroid.y /= n;
        tgtCentroid.z /= n;

        // SVD求解旋转
        Matrix3d H = Matrix3d::Zero();
        for (const auto& [srcIdx, tgtIdx] : correspondences) {
            Vector3d ps(source[srcIdx].x - srcCentroid.x,
                       source[srcIdx].y - srcCentroid.y,
                       source[srcIdx].z - srcCentroid.z);
            Vector3d pt(target[tgtIdx].x - tgtCentroid.x,
                       target[tgtIdx].y - tgtCentroid.y,
                       target[tgtIdx].z - tgtCentroid.z);
            H += ps * pt.transpose();
        }

        Eigen::JacobiSVD<Matrix3d> svd(H, Eigen::ComputeFullU | Eigen::ComputeFullV);
        Matrix3d R = svd.matrixV() * svd.matrixU().transpose();

        if (R.determinant() < 0) {
            Matrix3d V = svd.matrixV();
            V.col(2) *= -1;
            R = V * svd.matrixU().transpose();
        }

        Vector3d t = Vector3d(tgtCentroid.x, tgtCentroid.y, tgtCentroid.z) -
                     R * Vector3d(srcCentroid.x, srcCentroid.y, srcCentroid.z);

        Matrix4d iterTransform = Matrix4d::Identity();
        iterTransform.block<3, 3>(0, 0) = R;
        iterTransform.block<3, 1>(0, 3) = t;

        transform = iterTransform * transform;
        source.transform(iterTransform);
    }

    if (finalScore) {
        // 计算最终匹配分数
        double sumDist = 0;
        int count = 0;
        for (size_t i = 0; i < source.size(); ++i) {
            size_t nearestIdx = target.findNearestPoint(source[i]);
            double dist = source[i].distanceTo(target[nearestIdx]);
            if (dist <= maxCorrespondenceDistance) {
                sumDist += dist;
                ++count;
            }
        }
        *finalScore = count > 0 ? sumDist / count : std::numeric_limits<double>::max();
    }

    return transform;
}

Matrix4d PointCloud3D::alignPointToPlaneICP(const PointCloud3D& target,
                                             double maxCorrespondenceDistance,
                                             int maxIterations,
                                             double* finalScore) const {
    // 简化实现：使用点到点ICP
    // 完整的点到面ICP需要目标点云有法线
    return alignICP(target, maxCorrespondenceDistance, maxIterations, finalScore);
}

// ============================================================
// 曲率
// ============================================================

void PointCloud3D::computeCurvature(int kNeighbors) {
    // 已在estimateNormals中实现
    if (!m_hasNormals) {
        estimateNormals(kNeighbors);
    }
}

// ============================================================
// 颜色操作
// ============================================================

void PointCloud3D::setUniformColor(uint8_t r, uint8_t g, uint8_t b) {
    QMutexLocker locker(&m_mutex);

    for (auto& point : m_points) {
        point.r = r;
        point.g = g;
        point.b = b;
    }

    m_hasColors = true;
}

void PointCloud3D::colorByHeight(float minZ, float maxZ) {
    QMutexLocker locker(&m_mutex);

    if (minZ >= maxZ) {
        BoundingBox3D box = computeBoundingBox();
        minZ = box.minPoint.z;
        maxZ = box.maxPoint.z;
    }

    float range = maxZ - minZ;
    if (range <= 0) range = 1;

    for (auto& point : m_points) {
        float t = (point.z - minZ) / range;
        t = std::max(0.0f, std::min(1.0f, t));

        // 彩虹色映射
        if (t < 0.25f) {
            point.r = 0;
            point.g = static_cast<uint8_t>(255 * 4 * t);
            point.b = 255;
        } else if (t < 0.5f) {
            point.r = 0;
            point.g = 255;
            point.b = static_cast<uint8_t>(255 * (1 - 4 * (t - 0.25f)));
        } else if (t < 0.75f) {
            point.r = static_cast<uint8_t>(255 * 4 * (t - 0.5f));
            point.g = 255;
            point.b = 0;
        } else {
            point.r = 255;
            point.g = static_cast<uint8_t>(255 * (1 - 4 * (t - 0.75f)));
            point.b = 0;
        }
    }

    m_hasColors = true;
}

void PointCloud3D::colorByNormal() {
    QMutexLocker locker(&m_mutex);

    if (!m_hasNormals) {
        estimateNormals();
    }

    for (auto& point : m_points) {
        point.r = static_cast<uint8_t>(128 + 127 * point.normal_x);
        point.g = static_cast<uint8_t>(128 + 127 * point.normal_y);
        point.b = static_cast<uint8_t>(128 + 127 * point.normal_z);
    }

    m_hasColors = true;
}

void PointCloud3D::colorByCurvature() {
    QMutexLocker locker(&m_mutex);

    if (!m_hasNormals) {
        estimateNormals();
    }

    // 找到曲率范围
    float minCurv = std::numeric_limits<float>::max();
    float maxCurv = std::numeric_limits<float>::lowest();

    for (const auto& point : m_points) {
        minCurv = std::min(minCurv, point.curvature);
        maxCurv = std::max(maxCurv, point.curvature);
    }

    float range = maxCurv - minCurv;
    if (range <= 0) range = 1;

    for (auto& point : m_points) {
        float t = (point.curvature - minCurv) / range;
        point.r = static_cast<uint8_t>(255 * t);
        point.g = static_cast<uint8_t>(255 * (1 - t));
        point.b = 0;
    }

    m_hasColors = true;
}

bool PointCloud3D::hasColors() const {
    return m_hasColors;
}

// ============================================================
// 文件I/O
// ============================================================

bool PointCloud3D::loadPLY(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString line;

    // 读取头部
    int vertexCount = 0;
    bool hasColor = false;
    bool hasNormal = false;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line.startsWith("element vertex")) {
            vertexCount = line.split(' ').last().toInt();
        } else if (line.startsWith("property") && line.contains("red")) {
            hasColor = true;
        } else if (line.startsWith("property") && line.contains("nx")) {
            hasNormal = true;
        } else if (line == "end_header") {
            break;
        }
    }

    // 读取数据
    QMutexLocker locker(&m_mutex);
    m_points.clear();
    m_points.reserve(static_cast<size_t>(vertexCount));

    for (int i = 0; i < vertexCount && !in.atEnd(); ++i) {
        line = in.readLine().trimmed();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);

        if (parts.size() >= 3) {
            Point3D point;
            point.x = parts[0].toFloat();
            point.y = parts[1].toFloat();
            point.z = parts[2].toFloat();

            int idx = 3;
            if (hasNormal && parts.size() >= idx + 3) {
                point.normal_x = parts[idx++].toFloat();
                point.normal_y = parts[idx++].toFloat();
                point.normal_z = parts[idx++].toFloat();
            }
            if (hasColor && parts.size() >= idx + 3) {
                point.r = static_cast<uint8_t>(parts[idx++].toInt());
                point.g = static_cast<uint8_t>(parts[idx++].toInt());
                point.b = static_cast<uint8_t>(parts[idx++].toInt());
            }

            m_points.push_back(point);
        }
    }

    m_hasNormals = hasNormal;
    m_hasColors = hasColor;
    invalidateCache();
    updateStatus();

    return true;
}

bool PointCloud3D::savePLY(const QString& filename, bool binary) const {
    Q_UNUSED(binary);  // 简化实现，只支持ASCII

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);

    // 写入头部
    out << "ply\n";
    out << "format ascii 1.0\n";
    out << "element vertex " << m_points.size() << "\n";
    out << "property float x\n";
    out << "property float y\n";
    out << "property float z\n";

    if (m_hasNormals) {
        out << "property float nx\n";
        out << "property float ny\n";
        out << "property float nz\n";
    }

    if (m_hasColors) {
        out << "property uchar red\n";
        out << "property uchar green\n";
        out << "property uchar blue\n";
    }

    out << "end_header\n";

    // 写入数据
    for (const auto& point : m_points) {
        out << point.x << " " << point.y << " " << point.z;

        if (m_hasNormals) {
            out << " " << point.normal_x << " " << point.normal_y << " " << point.normal_z;
        }

        if (m_hasColors) {
            out << " " << static_cast<int>(point.r)
                << " " << static_cast<int>(point.g)
                << " " << static_cast<int>(point.b);
        }

        out << "\n";
    }

    return true;
}

bool PointCloud3D::loadPCD(const QString& filename) {
    // 简化实现
    return loadPLY(filename);
}

bool PointCloud3D::savePCD(const QString& filename, bool binary) const {
    Q_UNUSED(binary);
    return savePLY(filename, false);
}

bool PointCloud3D::loadXYZ(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QMutexLocker locker(&m_mutex);
    m_points.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);

        if (parts.size() >= 3) {
            Point3D point;
            point.x = parts[0].toFloat();
            point.y = parts[1].toFloat();
            point.z = parts[2].toFloat();
            m_points.push_back(point);
        }
    }

    invalidateCache();
    updateStatus();
    return true;
}

bool PointCloud3D::saveXYZ(const QString& filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    for (const auto& point : m_points) {
        out << point.x << " " << point.y << " " << point.z << "\n";
    }

    return true;
}

// ============================================================
// 统计信息
// ============================================================

PointCloudType PointCloud3D::getType() const {
    return m_type;
}

void PointCloud3D::setType(PointCloudType type) {
    m_type = type;
}

PointCloudStatus PointCloud3D::getStatus() const {
    return m_status;
}

size_t PointCloud3D::getValidPointCount() const {
    QMutexLocker locker(&m_mutex);
    size_t count = 0;
    for (const auto& point : m_points) {
        if (point.isValid()) {
            ++count;
        }
    }
    return count;
}

double PointCloud3D::estimateDensity() const {
    if (m_points.size() < 2) {
        return 0;
    }

    BoundingBox3D box = computeBoundingBox();
    Point3D size = box.size();
    double volume = static_cast<double>(size.x) * size.y * size.z;

    if (volume <= 0) {
        return 0;
    }

    return static_cast<double>(m_points.size()) / volume;
}

double PointCloud3D::estimateAverageSpacing(int sampleCount) const {
    QMutexLocker locker(&m_mutex);

    if (m_points.size() < 2) {
        return 0;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, m_points.size() - 1);

    double sumSpacing = 0;
    int actualSamples = std::min(sampleCount, static_cast<int>(m_points.size()));

    for (int i = 0; i < actualSamples; ++i) {
        size_t idx = dis(gen);
        std::vector<size_t> neighbors = findKNearestNeighbors(m_points[idx], 2);

        if (neighbors.size() >= 2) {
            sumSpacing += m_points[idx].distanceTo(m_points[neighbors[1]]);
        }
    }

    return sumSpacing / actualSamples;
}

// ============================================================
// 元数据
// ============================================================

void PointCloud3D::setName(const QString& name) {
    m_name = name;
}

QString PointCloud3D::getName() const {
    return m_name;
}

void PointCloud3D::setDescription(const QString& description) {
    m_description = description;
}

QString PointCloud3D::getDescription() const {
    return m_description;
}

void PointCloud3D::setTimestamp(const QDateTime& timestamp) {
    m_timestamp = timestamp;
}

QDateTime PointCloud3D::getTimestamp() const {
    return m_timestamp;
}

// ============================================================
// 序列化
// ============================================================

QJsonObject PointCloud3D::toJson() const {
    QJsonObject json;
    json["name"] = m_name;
    json["description"] = m_description;
    json["timestamp"] = m_timestamp.toString(Qt::ISODate);
    json["type"] = static_cast<int>(m_type);
    json["pointCount"] = static_cast<qint64>(m_points.size());
    json["hasNormals"] = m_hasNormals;
    json["hasColors"] = m_hasColors;
    json["boundingBox"] = computeBoundingBox().toJson();
    return json;
}

PointCloud3D PointCloud3D::fromJson(const QJsonObject& json) {
    PointCloud3D cloud;
    cloud.m_name = json["name"].toString();
    cloud.m_description = json["description"].toString();
    cloud.m_timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    cloud.m_type = static_cast<PointCloudType>(json["type"].toInt());
    cloud.m_hasNormals = json["hasNormals"].toBool();
    cloud.m_hasColors = json["hasColors"].toBool();
    return cloud;
}

// ============================================================
// 克隆
// ============================================================

PointCloud3D PointCloud3D::clone() const {
    return PointCloud3D(*this);
}

PointCloud3D PointCloud3D::extract(const std::vector<size_t>& indices) const {
    QMutexLocker locker(&m_mutex);
    PointCloud3D result;

    for (size_t idx : indices) {
        if (idx < m_points.size()) {
            result.addPoint(m_points[idx]);
        }
    }

    result.m_hasNormals = m_hasNormals;
    result.m_hasColors = m_hasColors;
    result.m_type = m_type;

    return result;
}

// ============================================================
// 私有方法
// ============================================================

void PointCloud3D::invalidateCache() {
    m_boundingBoxValid = false;
}

void PointCloud3D::updateStatus() {
    if (m_points.empty()) {
        m_status = PointCloudStatus::Empty;
    } else {
        m_status = PointCloudStatus::Valid;
    }
}

// ============================================================
// 工具函数
// ============================================================

PointCloud3D mergePointClouds(const std::vector<PointCloud3D>& clouds) {
    PointCloud3D result;

    for (const auto& cloud : clouds) {
        result.addPoints(cloud);
    }

    return result;
}

double computeCloudToCloudDistance(const PointCloud3D& source,
                                    const PointCloud3D& target,
                                    double maxDistance) {
    if (source.empty() || target.empty()) {
        return std::numeric_limits<double>::max();
    }

    double sumDist = 0;
    int count = 0;

    for (size_t i = 0; i < source.size(); ++i) {
        size_t nearestIdx = target.findNearestPoint(source[i]);
        double dist = source[i].distanceTo(target[nearestIdx]);

        if (dist <= maxDistance) {
            sumDist += dist;
            ++count;
        }
    }

    return count > 0 ? sumDist / count : std::numeric_limits<double>::max();
}

double computeHausdorffDistance(const PointCloud3D& cloud1, const PointCloud3D& cloud2) {
    double maxDist1 = 0;
    for (size_t i = 0; i < cloud1.size(); ++i) {
        size_t nearestIdx = cloud2.findNearestPoint(cloud1[i]);
        maxDist1 = std::max(maxDist1, static_cast<double>(cloud1[i].distanceTo(cloud2[nearestIdx])));
    }

    double maxDist2 = 0;
    for (size_t i = 0; i < cloud2.size(); ++i) {
        size_t nearestIdx = cloud1.findNearestPoint(cloud2[i]);
        maxDist2 = std::max(maxDist2, static_cast<double>(cloud2[i].distanceTo(cloud1[nearestIdx])));
    }

    return std::max(maxDist1, maxDist2);
}

PointCloud3D pointCloudFromDepthMap(const QImage& depthImage,
                                     const CameraIntrinsics& intrinsics,
                                     float depthScale) {
    PointCloud3D cloud;

    if (depthImage.isNull() || !intrinsics.isValid()) {
        return cloud;
    }

    for (int y = 0; y < depthImage.height(); ++y) {
        for (int x = 0; x < depthImage.width(); ++x) {
            float depth = qGray(depthImage.pixel(x, y)) * depthScale;

            if (depth > 0) {
                float px = (x - static_cast<float>(intrinsics.cx)) * depth /
                          static_cast<float>(intrinsics.fx);
                float py = (y - static_cast<float>(intrinsics.cy)) * depth /
                          static_cast<float>(intrinsics.fy);

                cloud.addPoint(px, py, depth);
            }
        }
    }

    return cloud;
}

} // namespace Algorithm3D
} // namespace VisionForge
