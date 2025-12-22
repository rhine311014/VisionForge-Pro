/**
 * @file DepthMap.h
 * @brief VisionForge Pro 深度图处理类
 *
 * 提供深度图数据管理和处理功能，包括：
 * - 多格式深度数据存储
 * - 深度图滤波和增强
 * - 空洞填充
 * - 深度图转点云
 * - 深度图可视化
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_DEPTHMAP_H
#define VISIONFORGE_DEPTHMAP_H

#include "Algorithm3DTypes.h"
#include <QObject>
#include <QImage>
#include <QString>
#include <QMutex>
#include <memory>

namespace VisionForge {
namespace Algorithm3D {

// 前向声明
class PointCloud3D;

/**
 * @brief 深度图类
 *
 * 管理和处理深度图像数据，支持多种深度格式。
 */
class DepthMap : public QObject {
    Q_OBJECT

public:
    // ============ 构造函数 ============

    /**
     * @brief 默认构造函数
     */
    explicit DepthMap(QObject* parent = nullptr);

    /**
     * @brief 从尺寸构造
     */
    DepthMap(int width, int height, DepthFormat format = DepthFormat::Float32,
             QObject* parent = nullptr);

    /**
     * @brief 从QImage构造（16位灰度图）
     */
    explicit DepthMap(const QImage& image, double depthScale = 0.1,
                      QObject* parent = nullptr);

    /**
     * @brief 拷贝构造
     */
    DepthMap(const DepthMap& other);

    /**
     * @brief 移动构造
     */
    DepthMap(DepthMap&& other) noexcept;

    /**
     * @brief 析构函数
     */
    ~DepthMap() override;

    // ============ 赋值运算符 ============
    DepthMap& operator=(const DepthMap& other);
    DepthMap& operator=(DepthMap&& other) noexcept;

    // ============ 基本属性 ============

    /**
     * @brief 获取宽度
     */
    int width() const { return m_width; }

    /**
     * @brief 获取高度
     */
    int height() const { return m_height; }

    /**
     * @brief 获取格式
     */
    DepthFormat format() const { return m_format; }

    /**
     * @brief 是否为空
     */
    bool isEmpty() const { return m_width == 0 || m_height == 0; }

    /**
     * @brief 获取像素总数
     */
    int pixelCount() const { return m_width * m_height; }

    // ============ 深度访问 ============

    /**
     * @brief 获取深度值（mm）
     */
    double getDepth(int x, int y) const;

    /**
     * @brief 设置深度值（mm）
     */
    void setDepth(int x, int y, double depth);

    /**
     * @brief 获取深度值（带插值）
     */
    double getDepthInterpolated(double x, double y) const;

    /**
     * @brief 检查像素是否有效
     */
    bool isValid(int x, int y) const;

    /**
     * @brief 获取有效像素数量
     */
    int getValidPixelCount() const;

    /**
     * @brief 获取无效像素掩码
     */
    QImage getInvalidMask() const;

    // ============ 数据访问 ============

    /**
     * @brief 获取Float32数据指针
     */
    float* dataFloat32();
    const float* dataFloat32() const;

    /**
     * @brief 获取Float64数据指针
     */
    double* dataFloat64();
    const double* dataFloat64() const;

    /**
     * @brief 获取UInt16数据指针
     */
    uint16_t* dataUInt16();
    const uint16_t* dataUInt16() const;

    /**
     * @brief 获取原始字节指针
     */
    void* data();
    const void* data() const;

    /**
     * @brief 获取数据大小（字节）
     */
    size_t dataSize() const;

    // ============ 格式转换 ============

    /**
     * @brief 转换为指定格式
     */
    DepthMap convertTo(DepthFormat targetFormat) const;

    /**
     * @brief 从QImage创建（16位灰度）
     */
    static DepthMap fromImage16(const QImage& image, double depthScale = 0.1);

    /**
     * @brief 从QImage创建（32位浮点编码）
     */
    static DepthMap fromImageFloat(const QImage& image);

    /**
     * @brief 转换为16位灰度图像
     */
    QImage toImage16(double depthScale = 0.1) const;

    /**
     * @brief 转换为可视化图像（伪彩色）
     */
    QImage toColorImage(double minDepth = 0, double maxDepth = 0) const;

    // ============ 统计信息 ============

    /**
     * @brief 获取最小深度
     */
    double getMinDepth() const;

    /**
     * @brief 获取最大深度
     */
    double getMaxDepth() const;

    /**
     * @brief 获取深度范围
     */
    void getDepthRange(double& minDepth, double& maxDepth) const;

    /**
     * @brief 获取平均深度
     */
    double getAverageDepth() const;

    /**
     * @brief 获取深度标准差
     */
    double getDepthStdDev() const;

    // ============ 滤波 ============

    /**
     * @brief 中值滤波
     */
    DepthMap medianFilter(int kernelSize = 5) const;

    /**
     * @brief 高斯滤波
     */
    DepthMap gaussianFilter(int kernelSize = 5, double sigma = 1.0) const;

    /**
     * @brief 双边滤波
     */
    DepthMap bilateralFilter(int d = 9, double sigmaColor = 75,
                              double sigmaSpace = 75) const;

    /**
     * @brief 导向滤波
     */
    DepthMap guidedFilter(const QImage& guide, int radius = 8,
                           double eps = 0.01) const;

    /**
     * @brief 边缘保持滤波
     */
    DepthMap edgePreservingFilter(double threshold = 30.0) const;

    /**
     * @brief 应用配置滤波
     */
    DepthMap applyFilter(const DepthMapConfig& config) const;

    // ============ 空洞填充 ============

    /**
     * @brief 填充空洞（最近邻）
     */
    DepthMap fillHolesNearestNeighbor() const;

    /**
     * @brief 填充空洞（插值）
     */
    DepthMap fillHolesInterpolation(int maxHoleSize = 10) const;

    /**
     * @brief 填充空洞（修复算法）
     */
    DepthMap fillHolesInpaint(int radius = 3) const;

    /**
     * @brief 填充空洞（自定义值）
     */
    DepthMap fillHolesWithValue(double value) const;

    // ============ 几何操作 ============

    /**
     * @brief 缩放
     */
    DepthMap resize(int newWidth, int newHeight) const;

    /**
     * @brief 裁剪
     */
    DepthMap crop(int x, int y, int width, int height) const;

    /**
     * @brief 翻转
     */
    DepthMap flip(bool horizontal, bool vertical) const;

    /**
     * @brief 旋转（90度倍数）
     */
    DepthMap rotate90(int times = 1) const;

    // ============ 深度操作 ============

    /**
     * @brief 深度范围裁剪
     */
    DepthMap clipDepthRange(double minDepth, double maxDepth) const;

    /**
     * @brief 深度缩放
     */
    DepthMap scaleDepth(double factor) const;

    /**
     * @brief 深度偏移
     */
    DepthMap offsetDepth(double offset) const;

    /**
     * @brief 深度归一化
     */
    DepthMap normalizeDepth(double targetMin = 0, double targetMax = 1) const;

    // ============ 边缘和梯度 ============

    /**
     * @brief 计算深度梯度
     */
    void computeGradient(DepthMap& gradX, DepthMap& gradY) const;

    /**
     * @brief 计算边缘图
     */
    QImage computeEdges(double threshold = 30.0) const;

    /**
     * @brief 计算法线图
     */
    QImage computeNormalMap() const;

    // ============ 点云转换 ============

    /**
     * @brief 转换为点云
     */
    PointCloud3D toPointCloud(const CameraIntrinsics& intrinsics) const;

    /**
     * @brief 转换为点云（带颜色）
     */
    PointCloud3D toPointCloud(const CameraIntrinsics& intrinsics,
                               const QImage& colorImage) const;

    /**
     * @brief 从点云投影
     */
    static DepthMap fromPointCloud(const PointCloud3D& cloud,
                                    const CameraIntrinsics& intrinsics,
                                    int width, int height);

    // ============ 3D坐标计算 ============

    /**
     * @brief 像素坐标转3D点
     */
    Point3D pixelTo3D(int x, int y, const CameraIntrinsics& intrinsics) const;

    /**
     * @brief 批量像素坐标转3D点
     */
    std::vector<Point3D> pixelsTo3D(const std::vector<QPoint>& pixels,
                                     const CameraIntrinsics& intrinsics) const;

    /**
     * @brief 3D点投影到像素
     */
    QPoint project3DToPixel(const Point3D& point,
                            const CameraIntrinsics& intrinsics) const;

    // ============ 立体视觉 ============

    /**
     * @brief 从视差图创建深度图
     */
    static DepthMap fromDisparity(const QImage& disparity,
                                   const StereoParameters& stereoParams);

    /**
     * @brief 从视差图创建深度图（浮点视差）
     */
    static DepthMap fromDisparity(const DepthMap& disparity,
                                   const StereoParameters& stereoParams);

    /**
     * @brief 转换为视差图
     */
    DepthMap toDisparity(const StereoParameters& stereoParams) const;

    // ============ 分析 ============

    /**
     * @brief 拟合平面
     */
    bool fitPlane(Plane3D& plane, const CameraIntrinsics& intrinsics,
                  double* rmsError = nullptr) const;

    /**
     * @brief 计算深度直方图
     */
    std::vector<int> computeHistogram(int bins = 256,
                                       double minDepth = 0,
                                       double maxDepth = 0) const;

    // ============ 比较 ============

    /**
     * @brief 计算与另一深度图的差异
     */
    DepthMap computeDifference(const DepthMap& other) const;

    /**
     * @brief 计算RMSE
     */
    double computeRMSE(const DepthMap& groundTruth) const;

    /**
     * @brief 计算MAE
     */
    double computeMAE(const DepthMap& groundTruth) const;

    // ============ 文件I/O ============

    /**
     * @brief 从文件加载
     */
    bool load(const QString& filename);

    /**
     * @brief 保存到文件
     */
    bool save(const QString& filename) const;

    /**
     * @brief 从PNG加载（16位）
     */
    bool loadPNG16(const QString& filename, double depthScale = 0.1);

    /**
     * @brief 保存为PNG（16位）
     */
    bool savePNG16(const QString& filename, double depthScale = 0.1) const;

    /**
     * @brief 从EXR加载
     */
    bool loadEXR(const QString& filename);

    /**
     * @brief 保存为EXR
     */
    bool saveEXR(const QString& filename) const;

    // ============ 元数据 ============

    /**
     * @brief 设置深度来源
     */
    void setSource(DepthSource source) { m_source = source; }
    DepthSource getSource() const { return m_source; }

    /**
     * @brief 设置相机内参
     */
    void setIntrinsics(const CameraIntrinsics& intrinsics);
    const CameraIntrinsics& getIntrinsics() const { return m_intrinsics; }
    bool hasIntrinsics() const { return m_intrinsics.isValid(); }

    /**
     * @brief 设置深度单位（mm）
     */
    void setDepthUnit(double unit) { m_depthUnit = unit; }
    double getDepthUnit() const { return m_depthUnit; }

    /**
     * @brief 设置时间戳
     */
    void setTimestamp(const QDateTime& timestamp) { m_timestamp = timestamp; }
    QDateTime getTimestamp() const { return m_timestamp; }

    // ============ 序列化 ============

    /**
     * @brief 转换为JSON（元数据）
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON创建
     */
    static DepthMap fromJson(const QJsonObject& json);

    // ============ 克隆 ============

    /**
     * @brief 深拷贝
     */
    DepthMap clone() const;

signals:
    /**
     * @brief 深度图更新信号
     */
    void depthMapUpdated();

    /**
     * @brief 处理进度信号
     */
    void processingProgress(int percent);

private:
    // 图像数据
    int m_width = 0;
    int m_height = 0;
    DepthFormat m_format = DepthFormat::Float32;

    // 数据存储（根据格式使用不同类型）
    std::vector<float> m_dataFloat32;
    std::vector<double> m_dataFloat64;
    std::vector<uint16_t> m_dataUInt16;
    std::vector<uint8_t> m_dataUInt8;

    // 元数据
    DepthSource m_source = DepthSource::Unknown;
    CameraIntrinsics m_intrinsics;
    double m_depthUnit = 1.0;   // 深度单位（mm）
    QDateTime m_timestamp;

    // 缓存
    mutable double m_minDepth = -1;
    mutable double m_maxDepth = -1;
    mutable bool m_rangeValid = false;

    // 线程安全
    mutable QMutex m_mutex;

    // 私有方法
    void allocate(int width, int height, DepthFormat format);
    void invalidateCache();
    double bilinearInterpolate(double x, double y) const;
};

// ============================================================
// 立体匹配
// ============================================================

/**
 * @brief 立体匹配类
 *
 * 从双目图像计算视差图和深度图。
 */
class StereoMatcher : public QObject {
    Q_OBJECT

public:
    explicit StereoMatcher(QObject* parent = nullptr);
    ~StereoMatcher() override;

    /**
     * @brief 设置立体相机参数
     */
    void setStereoParameters(const StereoParameters& params);

    /**
     * @brief 设置匹配配置
     */
    void setConfig(const StereoMatchConfig& config);

    /**
     * @brief 计算视差图
     */
    DepthMap computeDisparity(const QImage& leftImage,
                               const QImage& rightImage);

    /**
     * @brief 计算深度图
     */
    DepthMap computeDepth(const QImage& leftImage,
                           const QImage& rightImage);

    /**
     * @brief 获取最后的错误信息
     */
    QString getLastError() const { return m_lastError; }

signals:
    void processingProgress(int percent);

private:
    StereoParameters m_stereoParams;
    StereoMatchConfig m_config;
    QString m_lastError;
};

} // namespace Algorithm3D
} // namespace VisionForge

#endif // VISIONFORGE_DEPTHMAP_H
