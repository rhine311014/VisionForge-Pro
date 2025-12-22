/**
 * @file PointCloud3D.h
 * @brief VisionForge Pro 3D点云数据类
 *
 * 提供高效的3D点云数据管理，包括：
 * - 点云存储和访问
 * - 边界盒计算
 * - 点云变换
 * - 法线估计
 * - 滤波和下采样
 * - 文件I/O（PLY、PCD格式）
 *
 * 设计为可选集成PCL库，无PCL时使用内置实现。
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_POINTCLOUD3D_H
#define VISIONFORGE_POINTCLOUD3D_H

#include "Algorithm3DTypes.h"
#include <QObject>
#include <QString>
#include <QMutex>
#include <memory>
#include <functional>

namespace VisionForge {
namespace Algorithm3D {

/**
 * @brief 3D点云类
 *
 * 高效管理3D点云数据，提供各种处理功能。
 * 支持多线程安全访问。
 */
class PointCloud3D : public QObject {
    Q_OBJECT

public:
    // ============ 类型定义 ============
    using PointVector = std::vector<Point3D>;
    using Iterator = PointVector::iterator;
    using ConstIterator = PointVector::const_iterator;
    using PointCallback = std::function<void(Point3D&)>;
    using ConstPointCallback = std::function<void(const Point3D&)>;

    // ============ 构造函数 ============

    /**
     * @brief 默认构造函数
     */
    explicit PointCloud3D(QObject* parent = nullptr);

    /**
     * @brief 从点向量构造
     */
    explicit PointCloud3D(const PointVector& points, QObject* parent = nullptr);

    /**
     * @brief 拷贝构造
     */
    PointCloud3D(const PointCloud3D& other);

    /**
     * @brief 移动构造
     */
    PointCloud3D(PointCloud3D&& other) noexcept;

    /**
     * @brief 析构函数
     */
    ~PointCloud3D() override;

    // ============ 赋值运算符 ============
    PointCloud3D& operator=(const PointCloud3D& other);
    PointCloud3D& operator=(PointCloud3D&& other) noexcept;

    // ============ 点访问 ============

    /**
     * @brief 获取点数量
     */
    size_t size() const;

    /**
     * @brief 是否为空
     */
    bool empty() const;

    /**
     * @brief 获取容量
     */
    size_t capacity() const;

    /**
     * @brief 预留空间
     */
    void reserve(size_t n);

    /**
     * @brief 调整大小
     */
    void resize(size_t n);

    /**
     * @brief 清空点云
     */
    void clear();

    /**
     * @brief 访问点（带边界检查）
     */
    Point3D& at(size_t index);
    const Point3D& at(size_t index) const;

    /**
     * @brief 访问点（无边界检查）
     */
    Point3D& operator[](size_t index);
    const Point3D& operator[](size_t index) const;

    /**
     * @brief 获取第一个点
     */
    Point3D& front();
    const Point3D& front() const;

    /**
     * @brief 获取最后一个点
     */
    Point3D& back();
    const Point3D& back() const;

    /**
     * @brief 获取原始数据指针
     */
    Point3D* data();
    const Point3D* data() const;

    // ============ 迭代器 ============
    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    // ============ 点操作 ============

    /**
     * @brief 添加点
     */
    void addPoint(const Point3D& point);
    void addPoint(float x, float y, float z);
    void addPoint(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief 批量添加点
     */
    void addPoints(const PointVector& points);
    void addPoints(const PointCloud3D& other);

    /**
     * @brief 移除点
     */
    void removePoint(size_t index);

    /**
     * @brief 移除满足条件的点
     */
    void removeIf(std::function<bool(const Point3D&)> predicate);

    /**
     * @brief 交换两点
     */
    void swapPoints(size_t i, size_t j);

    /**
     * @brief 遍历所有点（可修改）
     */
    void forEach(PointCallback callback);

    /**
     * @brief 遍历所有点（只读）
     */
    void forEach(ConstPointCallback callback) const;

    // ============ 几何计算 ============

    /**
     * @brief 计算边界盒
     */
    BoundingBox3D computeBoundingBox() const;

    /**
     * @brief 获取缓存的边界盒
     */
    const BoundingBox3D& getBoundingBox();

    /**
     * @brief 计算质心
     */
    Point3D computeCentroid() const;

    /**
     * @brief 计算协方差矩阵
     */
    Matrix3d computeCovarianceMatrix() const;

    /**
     * @brief 计算主成分方向
     */
    void computePCA(Vector3d& eigenValues, Matrix3d& eigenVectors) const;

    /**
     * @brief 查找最近点
     */
    size_t findNearestPoint(const Point3D& query) const;

    /**
     * @brief 查找半径内的点
     */
    std::vector<size_t> findPointsInRadius(const Point3D& query, float radius) const;

    /**
     * @brief 查找K个最近邻
     */
    std::vector<size_t> findKNearestNeighbors(const Point3D& query, int k) const;

    // ============ 变换 ============

    /**
     * @brief 应用4x4变换矩阵
     */
    void transform(const Matrix4d& transformMatrix);

    /**
     * @brief 平移
     */
    void translate(float dx, float dy, float dz);
    void translate(const Vector3d& offset);

    /**
     * @brief 旋转（绕原点）
     */
    void rotate(const Matrix3d& rotationMatrix);
    void rotate(const Quaterniond& quaternion);

    /**
     * @brief 绕轴旋转
     */
    void rotateAroundAxis(const Vector3d& axis, double angle);

    /**
     * @brief 缩放
     */
    void scale(float factor);
    void scale(float sx, float sy, float sz);

    /**
     * @brief 移动到原点（质心对齐）
     */
    void centerToOrigin();

    /**
     * @brief 获取变换后的副本
     */
    PointCloud3D transformed(const Matrix4d& transformMatrix) const;

    // ============ 法线计算 ============

    /**
     * @brief 估计法线（K近邻）
     */
    void estimateNormals(int kNeighbors = 30);

    /**
     * @brief 估计法线（半径搜索）
     */
    void estimateNormalsRadius(double radius);

    /**
     * @brief 法线一致性调整（朝向视点）
     */
    void orientNormalsTowardsViewpoint(const Vector3d& viewpoint);

    /**
     * @brief 反转所有法线
     */
    void flipNormals();

    /**
     * @brief 检查是否有法线数据
     */
    bool hasNormals() const;

    // ============ 滤波 ============

    /**
     * @brief 统计离群点滤波
     */
    PointCloud3D statisticalOutlierRemoval(int meanK = 50, double stddevMul = 1.0) const;

    /**
     * @brief 半径离群点滤波
     */
    PointCloud3D radiusOutlierRemoval(double radius, int minNeighbors = 2) const;

    /**
     * @brief 体素网格下采样
     */
    PointCloud3D voxelGridDownsample(double leafSize) const;

    /**
     * @brief 直通滤波（范围滤波）
     */
    PointCloud3D passThroughFilter(const QString& axis, double min, double max) const;

    /**
     * @brief 应用配置进行滤波
     */
    PointCloud3D applyFilter(const PointCloudConfig& config) const;

    // ============ 分割 ============

    /**
     * @brief 平面分割（RANSAC）
     */
    bool segmentPlane(Plane3D& plane, std::vector<size_t>& inliers,
                      double distanceThreshold = 1.0, int maxIterations = 100) const;

    /**
     * @brief 提取平面内点
     */
    PointCloud3D extractPlaneInliers(double distanceThreshold = 1.0) const;

    /**
     * @brief 提取平面外点
     */
    PointCloud3D extractPlaneOutliers(double distanceThreshold = 1.0) const;

    /**
     * @brief 欧式聚类
     */
    std::vector<PointCloud3D> euclideanClustering(double clusterTolerance,
                                                   int minClusterSize = 10,
                                                   int maxClusterSize = 100000) const;

    // ============ 配准 ============

    /**
     * @brief ICP配准
     */
    Matrix4d alignICP(const PointCloud3D& target,
                      double maxCorrespondenceDistance = 10.0,
                      int maxIterations = 50,
                      double* finalScore = nullptr) const;

    /**
     * @brief 点到平面ICP配准
     */
    Matrix4d alignPointToPlaneICP(const PointCloud3D& target,
                                   double maxCorrespondenceDistance = 10.0,
                                   int maxIterations = 50,
                                   double* finalScore = nullptr) const;

    // ============ 曲面重建 ============

    /**
     * @brief 计算曲率
     */
    void computeCurvature(int kNeighbors = 30);

    // ============ 颜色操作 ============

    /**
     * @brief 设置统一颜色
     */
    void setUniformColor(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief 根据高度着色
     */
    void colorByHeight(float minZ, float maxZ);

    /**
     * @brief 根据法线着色
     */
    void colorByNormal();

    /**
     * @brief 根据曲率着色
     */
    void colorByCurvature();

    /**
     * @brief 检查是否有颜色数据
     */
    bool hasColors() const;

    // ============ 文件I/O ============

    /**
     * @brief 从PLY文件加载
     */
    bool loadPLY(const QString& filename);

    /**
     * @brief 保存为PLY文件
     */
    bool savePLY(const QString& filename, bool binary = true) const;

    /**
     * @brief 从PCD文件加载
     */
    bool loadPCD(const QString& filename);

    /**
     * @brief 保存为PCD文件
     */
    bool savePCD(const QString& filename, bool binary = true) const;

    /**
     * @brief 从XYZ文本文件加载
     */
    bool loadXYZ(const QString& filename);

    /**
     * @brief 保存为XYZ文本文件
     */
    bool saveXYZ(const QString& filename) const;

    // ============ 统计信息 ============

    /**
     * @brief 获取点云类型
     */
    PointCloudType getType() const;

    /**
     * @brief 设置点云类型
     */
    void setType(PointCloudType type);

    /**
     * @brief 获取状态
     */
    PointCloudStatus getStatus() const;

    /**
     * @brief 获取有效点数量
     */
    size_t getValidPointCount() const;

    /**
     * @brief 获取密度估计（点/mm³）
     */
    double estimateDensity() const;

    /**
     * @brief 获取平均点间距
     */
    double estimateAverageSpacing(int sampleCount = 100) const;

    // ============ 元数据 ============

    /**
     * @brief 设置名称
     */
    void setName(const QString& name);
    QString getName() const;

    /**
     * @brief 设置描述
     */
    void setDescription(const QString& description);
    QString getDescription() const;

    /**
     * @brief 设置时间戳
     */
    void setTimestamp(const QDateTime& timestamp);
    QDateTime getTimestamp() const;

    // ============ 序列化 ============

    /**
     * @brief 转换为JSON
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON创建
     */
    static PointCloud3D fromJson(const QJsonObject& json);

    // ============ 克隆 ============

    /**
     * @brief 深拷贝
     */
    PointCloud3D clone() const;

    /**
     * @brief 提取子集
     */
    PointCloud3D extract(const std::vector<size_t>& indices) const;

signals:
    /**
     * @brief 点云更新信号
     */
    void pointCloudUpdated();

    /**
     * @brief 处理进度信号
     */
    void processingProgress(int percent);

private:
    // 点数据
    PointVector m_points;

    // 元数据
    QString m_name;
    QString m_description;
    QDateTime m_timestamp;
    PointCloudType m_type = PointCloudType::XYZ;
    PointCloudStatus m_status = PointCloudStatus::Empty;

    // 缓存
    mutable BoundingBox3D m_boundingBox;
    mutable bool m_boundingBoxValid = false;
    mutable bool m_hasNormals = false;
    mutable bool m_hasColors = false;

    // 线程安全
    mutable QMutex m_mutex;

    // 私有方法
    void invalidateCache();
    void updateStatus();
};

// ============================================================
// 点云处理工具函数
// ============================================================

/**
 * @brief 合并多个点云
 */
PointCloud3D mergePointClouds(const std::vector<PointCloud3D>& clouds);

/**
 * @brief 计算两个点云之间的距离
 */
double computeCloudToCloudDistance(const PointCloud3D& source,
                                    const PointCloud3D& target,
                                    double maxDistance = std::numeric_limits<double>::max());

/**
 * @brief 计算点云之间的Hausdorff距离
 */
double computeHausdorffDistance(const PointCloud3D& cloud1, const PointCloud3D& cloud2);

/**
 * @brief 从深度图生成点云
 */
PointCloud3D pointCloudFromDepthMap(const QImage& depthImage,
                                     const CameraIntrinsics& intrinsics,
                                     float depthScale = 1.0f);

} // namespace Algorithm3D
} // namespace VisionForge

#endif // VISIONFORGE_POINTCLOUD3D_H
