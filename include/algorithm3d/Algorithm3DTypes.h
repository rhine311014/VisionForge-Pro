/**
 * @file Algorithm3DTypes.h
 * @brief VisionForge Pro 3D算法类型定义
 *
 * 本文件定义了3D视觉处理系统中的核心数据类型，包括：
 * - 3D点和向量结构
 * - 点云数据结构（兼容PCL）
 * - 深度图数据结构
 * - 3D测量结果结构
 * - 3D处理配置参数
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_ALGORITHM3D_TYPES_H
#define VISIONFORGE_ALGORITHM3D_TYPES_H

#include <QString>
#include <QPointF>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QImage>
#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <memory>

// Eigen矩阵库
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// 类型别名
// ============================================================

/// 4x4变换矩阵
using Matrix4d = Eigen::Matrix4d;

/// 3x3旋转矩阵
using Matrix3d = Eigen::Matrix3d;

/// 3D向量
using Vector3d = Eigen::Vector3d;

/// 4D向量（齐次坐标）
using Vector4d = Eigen::Vector4d;

/// 3x4投影矩阵
using Matrix34d = Eigen::Matrix<double, 3, 4>;

/// 四元数
using Quaterniond = Eigen::Quaterniond;

/// 仿射变换
using Affine3d = Eigen::Affine3d;

/// 动态大小矩阵
using MatrixXd = Eigen::MatrixXd;

/// 动态大小向量
using VectorXd = Eigen::VectorXd;

// ============================================================
// 3D点结构
// ============================================================

/**
 * @brief 3D点结构
 *
 * 存储3D空间中的点，包含位置和可选的颜色、法线信息。
 * 设计为与PCL PointXYZRGBNormal兼容。
 */
struct Point3D {
    // ============ 位置 ============
    float x = 0.0f;         ///< X坐标
    float y = 0.0f;         ///< Y坐标
    float z = 0.0f;         ///< Z坐标

    // ============ 颜色（RGBA） ============
    uint8_t r = 255;        ///< 红色分量
    uint8_t g = 255;        ///< 绿色分量
    uint8_t b = 255;        ///< 蓝色分量
    uint8_t a = 255;        ///< 透明度

    // ============ 法线 ============
    float normal_x = 0.0f;  ///< 法线X分量
    float normal_y = 0.0f;  ///< 法线Y分量
    float normal_z = 0.0f;  ///< 法线Z分量

    // ============ 曲率 ============
    float curvature = 0.0f; ///< 曲率

    // ============ 构造函数 ============
    Point3D() = default;

    Point3D(float x_, float y_, float z_)
        : x(x_), y(y_), z(z_) {}

    Point3D(float x_, float y_, float z_, uint8_t r_, uint8_t g_, uint8_t b_)
        : x(x_), y(y_), z(z_), r(r_), g(g_), b(b_) {}

    Point3D(double x_, double y_, double z_)
        : x(static_cast<float>(x_))
        , y(static_cast<float>(y_))
        , z(static_cast<float>(z_)) {}

    // ============ 运算符 ============
    Point3D operator+(const Point3D& other) const {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }

    Point3D operator-(const Point3D& other) const {
        return Point3D(x - other.x, y - other.y, z - other.z);
    }

    Point3D operator*(float scalar) const {
        return Point3D(x * scalar, y * scalar, z * scalar);
    }

    bool operator==(const Point3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // ============ 方法 ============

    /**
     * @brief 计算到另一点的距离
     */
    float distanceTo(const Point3D& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }

    /**
     * @brief 计算点的模长
     */
    float norm() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    /**
     * @brief 归一化
     */
    Point3D normalized() const {
        float n = norm();
        if (n > 0) {
            return Point3D(x/n, y/n, z/n);
        }
        return *this;
    }

    /**
     * @brief 点积
     */
    float dot(const Point3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    /**
     * @brief 叉积
     */
    Point3D cross(const Point3D& other) const {
        return Point3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    /**
     * @brief 转换为Eigen向量
     */
    Vector3d toVector3d() const {
        return Vector3d(x, y, z);
    }

    /**
     * @brief 从Eigen向量创建
     */
    static Point3D fromVector3d(const Vector3d& v) {
        return Point3D(v.x(), v.y(), v.z());
    }

    /**
     * @brief 设置颜色
     */
    void setColor(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) {
        r = r_; g = g_; b = b_; a = a_;
    }

    /**
     * @brief 设置法线
     */
    void setNormal(float nx, float ny, float nz) {
        normal_x = nx; normal_y = ny; normal_z = nz;
    }

    /**
     * @brief 获取法线向量
     */
    Vector3d getNormal() const {
        return Vector3d(normal_x, normal_y, normal_z);
    }

    /**
     * @brief 检查是否有效（非NaN）
     */
    bool isValid() const {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }

    /**
     * @brief 转换为JSON
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON创建
     */
    static Point3D fromJson(const QJsonObject& json);

    /**
     * @brief 转换为字符串
     */
    QString toString() const;
};

// ============================================================
// 点云数据类型
// ============================================================

/**
 * @brief 点云数据类型枚举
 */
enum class PointCloudType {
    XYZ,            ///< 仅位置
    XYZRGB,         ///< 位置+颜色
    XYZNormal,      ///< 位置+法线
    XYZRGBNormal,   ///< 完整数据
    XYZIntensity,   ///< 位置+强度（激光扫描）
};

/**
 * @brief 点云处理状态
 */
enum class PointCloudStatus {
    Empty,          ///< 空点云
    Valid,          ///< 有效
    Processing,     ///< 处理中
    Error,          ///< 错误
};

/**
 * @brief 3D边界盒
 */
struct BoundingBox3D {
    Point3D minPoint;   ///< 最小点
    Point3D maxPoint;   ///< 最大点

    BoundingBox3D() {
        minPoint = Point3D(std::numeric_limits<float>::max(),
                          std::numeric_limits<float>::max(),
                          std::numeric_limits<float>::max());
        maxPoint = Point3D(std::numeric_limits<float>::lowest(),
                          std::numeric_limits<float>::lowest(),
                          std::numeric_limits<float>::lowest());
    }

    BoundingBox3D(const Point3D& min_, const Point3D& max_)
        : minPoint(min_), maxPoint(max_) {}

    /**
     * @brief 扩展边界盒以包含点
     */
    void extend(const Point3D& point) {
        minPoint.x = std::min(minPoint.x, point.x);
        minPoint.y = std::min(minPoint.y, point.y);
        minPoint.z = std::min(minPoint.z, point.z);
        maxPoint.x = std::max(maxPoint.x, point.x);
        maxPoint.y = std::max(maxPoint.y, point.y);
        maxPoint.z = std::max(maxPoint.z, point.z);
    }

    /**
     * @brief 获取中心点
     */
    Point3D center() const {
        return Point3D(
            (minPoint.x + maxPoint.x) / 2.0f,
            (minPoint.y + maxPoint.y) / 2.0f,
            (minPoint.z + maxPoint.z) / 2.0f
        );
    }

    /**
     * @brief 获取尺寸
     */
    Point3D size() const {
        return maxPoint - minPoint;
    }

    /**
     * @brief 获取对角线长度
     */
    float diagonal() const {
        return minPoint.distanceTo(maxPoint);
    }

    /**
     * @brief 检查点是否在边界盒内
     */
    bool contains(const Point3D& point) const {
        return point.x >= minPoint.x && point.x <= maxPoint.x &&
               point.y >= minPoint.y && point.y <= maxPoint.y &&
               point.z >= minPoint.z && point.z <= maxPoint.z;
    }

    /**
     * @brief 检查是否有效
     */
    bool isValid() const {
        return minPoint.x <= maxPoint.x &&
               minPoint.y <= maxPoint.y &&
               minPoint.z <= maxPoint.z;
    }

    QJsonObject toJson() const;
    static BoundingBox3D fromJson(const QJsonObject& json);
};

// ============================================================
// 深度图类型
// ============================================================

/**
 * @brief 深度图格式
 */
enum class DepthFormat {
    Float32,        ///< 32位浮点（单位：mm）
    Float64,        ///< 64位浮点（单位：mm）
    UInt16,         ///< 16位无符号整数（单位：0.1mm）
    UInt8,          ///< 8位无符号整数（归一化）
};

/**
 * @brief 深度图来源
 */
enum class DepthSource {
    Unknown,        ///< 未知
    StereoCamera,   ///< 双目相机
    StructuredLight,///< 结构光
    ToF,            ///< ToF相机
    LiDAR,          ///< 激光雷达
    Computed,       ///< 计算生成
};

/**
 * @brief 相机内参
 */
struct CameraIntrinsics {
    double fx = 0.0;    ///< X方向焦距
    double fy = 0.0;    ///< Y方向焦距
    double cx = 0.0;    ///< 光心X
    double cy = 0.0;    ///< 光心Y

    // 畸变系数（Brown-Conrady模型）
    double k1 = 0.0;    ///< 径向畸变k1
    double k2 = 0.0;    ///< 径向畸变k2
    double k3 = 0.0;    ///< 径向畸变k3
    double p1 = 0.0;    ///< 切向畸变p1
    double p2 = 0.0;    ///< 切向畸变p2

    int width = 0;      ///< 图像宽度
    int height = 0;     ///< 图像高度

    /**
     * @brief 获取内参矩阵
     */
    Matrix3d getMatrix() const {
        Matrix3d K;
        K << fx, 0, cx,
             0, fy, cy,
             0, 0, 1;
        return K;
    }

    /**
     * @brief 设置内参矩阵
     */
    void setFromMatrix(const Matrix3d& K) {
        fx = K(0, 0);
        fy = K(1, 1);
        cx = K(0, 2);
        cy = K(1, 2);
    }

    /**
     * @brief 检查是否有效
     */
    bool isValid() const {
        return fx > 0 && fy > 0 && width > 0 && height > 0;
    }

    QJsonObject toJson() const;
    static CameraIntrinsics fromJson(const QJsonObject& json);
};

/**
 * @brief 相机外参（位姿）
 */
struct CameraExtrinsics {
    Matrix3d rotation = Matrix3d::Identity();   ///< 旋转矩阵
    Vector3d translation = Vector3d::Zero();    ///< 平移向量

    /**
     * @brief 获取4x4变换矩阵
     */
    Matrix4d getTransformMatrix() const {
        Matrix4d T = Matrix4d::Identity();
        T.block<3, 3>(0, 0) = rotation;
        T.block<3, 1>(0, 3) = translation;
        return T;
    }

    /**
     * @brief 从4x4矩阵设置
     */
    void setFromMatrix(const Matrix4d& T) {
        rotation = T.block<3, 3>(0, 0);
        translation = T.block<3, 1>(0, 3);
    }

    /**
     * @brief 获取逆变换
     */
    CameraExtrinsics inverse() const {
        CameraExtrinsics inv;
        inv.rotation = rotation.transpose();
        inv.translation = -inv.rotation * translation;
        return inv;
    }

    /**
     * @brief 变换点
     */
    Vector3d transformPoint(const Vector3d& point) const {
        return rotation * point + translation;
    }

    QJsonObject toJson() const;
    static CameraExtrinsics fromJson(const QJsonObject& json);
};

/**
 * @brief 立体相机参数
 */
struct StereoParameters {
    CameraIntrinsics leftCamera;    ///< 左相机内参
    CameraIntrinsics rightCamera;   ///< 右相机内参
    CameraExtrinsics rightToLeft;   ///< 右相机到左相机的变换

    double baseline = 0.0;          ///< 基线距离（mm）
    double minDisparity = 0.0;      ///< 最小视差
    double maxDisparity = 128.0;    ///< 最大视差
    int blockSize = 5;              ///< 匹配块大小

    /**
     * @brief 计算基线距离
     */
    void computeBaseline() {
        baseline = rightToLeft.translation.norm();
    }

    /**
     * @brief 视差转深度
     */
    double disparityToDepth(double disparity) const {
        if (disparity <= 0) return std::numeric_limits<double>::infinity();
        return (leftCamera.fx * baseline) / disparity;
    }

    /**
     * @brief 深度转视差
     */
    double depthToDisparity(double depth) const {
        if (depth <= 0) return 0;
        return (leftCamera.fx * baseline) / depth;
    }

    QJsonObject toJson() const;
    static StereoParameters fromJson(const QJsonObject& json);
};

// ============================================================
// 3D测量类型
// ============================================================

/**
 * @brief 3D测量类型
 */
enum class Measurement3DType {
    Distance,           ///< 距离测量
    Angle,              ///< 角度测量
    Area,               ///< 面积测量
    Volume,             ///< 体积测量
    Flatness,           ///< 平面度
    Cylindricity,       ///< 圆柱度
    Sphericity,         ///< 球度
    Height,             ///< 高度测量
    Thickness,          ///< 厚度测量
    Gap,                ///< 间隙测量
    Flush,              ///< 平齐度测量
};

/**
 * @brief 平面参数（ax + by + cz + d = 0）
 */
struct Plane3D {
    double a = 0.0;
    double b = 0.0;
    double c = 1.0;     ///< 默认水平面
    double d = 0.0;

    Plane3D() = default;
    Plane3D(double a_, double b_, double c_, double d_)
        : a(a_), b(b_), c(c_), d(d_) {}

    /**
     * @brief 从法线和点创建
     */
    static Plane3D fromNormalAndPoint(const Vector3d& normal, const Vector3d& point) {
        Plane3D plane;
        plane.a = normal.x();
        plane.b = normal.y();
        plane.c = normal.z();
        plane.d = -normal.dot(point);
        return plane;
    }

    /**
     * @brief 获取法线
     */
    Vector3d normal() const {
        return Vector3d(a, b, c).normalized();
    }

    /**
     * @brief 点到平面的距离
     */
    double distanceToPoint(const Vector3d& point) const {
        return std::abs(a * point.x() + b * point.y() + c * point.z() + d) /
               std::sqrt(a*a + b*b + c*c);
    }

    /**
     * @brief 点在平面上的投影
     */
    Vector3d projectPoint(const Vector3d& point) const {
        double dist = (a * point.x() + b * point.y() + c * point.z() + d) /
                     (a*a + b*b + c*c);
        return point - dist * Vector3d(a, b, c);
    }

    QJsonObject toJson() const;
    static Plane3D fromJson(const QJsonObject& json);
};

/**
 * @brief 线段3D
 */
struct Line3D {
    Point3D start;      ///< 起点
    Point3D end;        ///< 终点

    Line3D() = default;
    Line3D(const Point3D& s, const Point3D& e) : start(s), end(e) {}

    /**
     * @brief 获取长度
     */
    float length() const {
        return start.distanceTo(end);
    }

    /**
     * @brief 获取方向向量
     */
    Vector3d direction() const {
        return (end - start).toVector3d().normalized();
    }

    /**
     * @brief 获取中点
     */
    Point3D midpoint() const {
        return Point3D(
            (start.x + end.x) / 2.0f,
            (start.y + end.y) / 2.0f,
            (start.z + end.z) / 2.0f
        );
    }

    QJsonObject toJson() const;
    static Line3D fromJson(const QJsonObject& json);
};

/**
 * @brief 圆柱参数
 */
struct Cylinder3D {
    Vector3d axisPoint;     ///< 轴上一点
    Vector3d axisDirection; ///< 轴方向
    double radius = 0.0;    ///< 半径
    double height = 0.0;    ///< 高度

    QJsonObject toJson() const;
    static Cylinder3D fromJson(const QJsonObject& json);
};

/**
 * @brief 球体参数
 */
struct Sphere3D {
    Vector3d center;        ///< 中心
    double radius = 0.0;    ///< 半径

    Sphere3D() : center(Vector3d::Zero()) {}
    Sphere3D(const Vector3d& c, double r) : center(c), radius(r) {}

    QJsonObject toJson() const;
    static Sphere3D fromJson(const QJsonObject& json);
};

/**
 * @brief 3D测量结果
 */
struct Measurement3DResult {
    Measurement3DType type = Measurement3DType::Distance;
    bool success = false;
    QString errorMessage;

    double value = 0.0;         ///< 主测量值
    QString unit = "mm";        ///< 单位

    // 附加测量值
    double value2 = 0.0;        ///< 次测量值（如角度测量的第二个边）
    double value3 = 0.0;        ///< 第三测量值

    // 误差统计
    double stdDev = 0.0;        ///< 标准差
    double confidence = 0.0;    ///< 置信度

    // 几何元素
    std::vector<Point3D> points;    ///< 相关点
    std::vector<Line3D> lines;      ///< 相关线段
    Plane3D plane;                  ///< 相关平面

    QJsonObject toJson() const;
    static Measurement3DResult fromJson(const QJsonObject& json);
    QString toString() const;
};

// ============================================================
// 3D处理配置
// ============================================================

/**
 * @brief 点云滤波类型
 */
enum class PointCloudFilterType {
    None,               ///< 无滤波
    StatisticalOutlier, ///< 统计离群点滤波
    RadiusOutlier,      ///< 半径离群点滤波
    VoxelGrid,          ///< 体素网格下采样
    PassThrough,        ///< 直通滤波
    Bilateral,          ///< 双边滤波
};

/**
 * @brief 点云处理配置
 */
struct PointCloudConfig {
    // ============ 滤波配置 ============
    PointCloudFilterType filterType = PointCloudFilterType::StatisticalOutlier;
    int statisticalMeanK = 50;          ///< 统计滤波近邻数
    double statisticalStddevMul = 1.0;  ///< 统计滤波标准差倍数
    double radiusSearchRadius = 5.0;    ///< 半径滤波搜索半径
    int radiusMinNeighbors = 2;         ///< 半径滤波最小邻居数

    // ============ 下采样配置 ============
    bool enableDownsampling = true;
    double voxelLeafSize = 1.0;         ///< 体素大小（mm）

    // ============ 法线估计配置 ============
    bool computeNormals = true;
    int normalKSearch = 30;             ///< 法线估计近邻数
    double normalRadiusSearch = 0.0;    ///< 法线估计搜索半径（0表示使用K近邻）

    // ============ 配准配置 ============
    double icpMaxCorrespondenceDistance = 10.0;  ///< ICP最大对应距离
    int icpMaxIterations = 50;                    ///< ICP最大迭代次数
    double icpTransformationEpsilon = 1e-8;       ///< ICP变换收敛阈值
    double icpEuclideanFitnessEpsilon = 1e-6;     ///< ICP欧几里得适应度阈值

    // ============ 分割配置 ============
    double segmentDistanceThreshold = 1.0;        ///< 分割距离阈值
    int segmentMaxIterations = 100;               ///< 分割最大迭代次数

    QJsonObject toJson() const;
    static PointCloudConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 深度图处理配置
 */
struct DepthMapConfig {
    // ============ 滤波配置 ============
    bool enableMedianFilter = true;
    int medianKernelSize = 5;           ///< 中值滤波核大小

    bool enableBilateralFilter = true;
    int bilateralD = 9;                 ///< 双边滤波直径
    double bilateralSigmaColor = 75.0;  ///< 颜色空间标准差
    double bilateralSigmaSpace = 75.0;  ///< 坐标空间标准差

    // ============ 空洞填充配置 ============
    bool enableHoleFilling = true;
    int holeFillMaxSize = 10;           ///< 最大填充空洞大小

    // ============ 边缘保持配置 ============
    bool enableEdgePreserving = true;
    double edgeThreshold = 30.0;        ///< 边缘检测阈值

    // ============ 深度范围 ============
    double minDepth = 100.0;            ///< 最小有效深度（mm）
    double maxDepth = 10000.0;          ///< 最大有效深度（mm）

    QJsonObject toJson() const;
    static DepthMapConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 立体匹配配置
 */
struct StereoMatchConfig {
    // ============ 算法选择 ============
    enum class Algorithm {
        BM,             ///< Block Matching
        SGBM,           ///< Semi-Global Block Matching
        ELAS,           ///< Efficient Large-Scale Stereo
    } algorithm = Algorithm::SGBM;

    // ============ SGBM参数 ============
    int minDisparity = 0;           ///< 最小视差
    int numDisparities = 128;       ///< 视差数（必须是16的倍数）
    int blockSize = 5;              ///< 匹配块大小
    int p1 = 0;                     ///< 平滑惩罚P1（0表示自动计算）
    int p2 = 0;                     ///< 平滑惩罚P2（0表示自动计算）
    int disp12MaxDiff = 1;          ///< 左右一致性检查阈值
    int preFilterCap = 63;          ///< 预滤波截断值
    int uniquenessRatio = 10;       ///< 唯一性比率
    int speckleWindowSize = 100;    ///< 散斑窗口大小
    int speckleRange = 32;          ///< 散斑范围

    // ============ 后处理 ============
    bool enableWLSFilter = true;    ///< 启用WLS滤波
    double wlsLambda = 8000.0;      ///< WLS平滑参数
    double wlsSigma = 1.5;          ///< WLS边缘参数

    QJsonObject toJson() const;
    static StereoMatchConfig fromJson(const QJsonObject& json);
};

// ============================================================
// 3D处理错误码
// ============================================================

/**
 * @brief 3D处理错误码
 */
enum class Algorithm3DError {
    Success = 0,                ///< 成功
    InvalidInput = 1,           ///< 无效输入
    InsufficientPoints = 2,     ///< 点数不足
    ComputationFailed = 3,      ///< 计算失败
    ConvergenceFailed = 4,      ///< 收敛失败
    MemoryError = 5,            ///< 内存错误
    FileIOError = 6,            ///< 文件读写错误
    CalibrationError = 7,       ///< 标定错误
    NotSupported = 8,           ///< 不支持的操作
};

/**
 * @brief 获取错误描述
 */
QString getAlgorithm3DErrorDescription(Algorithm3DError error);

/**
 * @brief 获取测量类型名称
 */
QString getMeasurement3DTypeName(Measurement3DType type);

/**
 * @brief 获取点云滤波类型名称
 */
QString getPointCloudFilterTypeName(PointCloudFilterType type);

} // namespace Algorithm3D
} // namespace VisionForge

// 注册Qt元类型
Q_DECLARE_METATYPE(VisionForge::Algorithm3D::Point3D)
Q_DECLARE_METATYPE(VisionForge::Algorithm3D::PointCloudType)
Q_DECLARE_METATYPE(VisionForge::Algorithm3D::DepthFormat)
Q_DECLARE_METATYPE(VisionForge::Algorithm3D::Measurement3DType)
Q_DECLARE_METATYPE(VisionForge::Algorithm3D::Measurement3DResult)

#endif // VISIONFORGE_ALGORITHM3D_TYPES_H
