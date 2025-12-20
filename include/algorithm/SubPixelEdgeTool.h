/**
 * @file SubPixelEdgeTool.h
 * @brief 亚像素边缘检测工具
 * @details 实现多种亚像素边缘检测算法，精度可达0.05像素
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <opencv2/core.hpp>
#include <vector>
#include <QPointF>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 亚像素检测方法枚举
 */
enum class SubPixelMethod {
    SobelInterpolation,     ///< Sobel梯度插值（速度快，精度0.1像素）
    ZernikeMoment,          ///< Zernike矩（精度高，0.05像素）
    QuadraticFit,           ///< 二次曲线拟合（平衡型）
    Gaussian1D,             ///< 一维高斯拟合（适合单边缘）
    ESRFit                  ///< Error Surface Fit（复杂边缘）
};

/**
 * @brief 亚像素边缘检测配置
 */
struct SubPixelEdgeConfig {
    SubPixelMethod method = SubPixelMethod::QuadraticFit;  ///< 检测方法
    double gradientThreshold = 20.0;    ///< 梯度阈值
    int windowSize = 5;                 ///< 窗口大小
    bool useSubPixel = true;            ///< 是否启用亚像素
    double sigma = 1.0;                 ///< 高斯平滑sigma
    int cannyLow = 50;                  ///< Canny低阈值
    int cannyHigh = 150;                ///< Canny高阈值

    SubPixelEdgeConfig() = default;
};

/**
 * @brief 亚像素边缘点结构
 */
struct SubPixelEdgePoint {
    cv::Point2f position;       ///< 亚像素位置
    float gradient;             ///< 梯度幅值
    float direction;            ///< 梯度方向（弧度）
    float subPixelShift;        ///< 亚像素偏移量
    bool isValid;               ///< 是否有效

    SubPixelEdgePoint()
        : position(0, 0), gradient(0), direction(0),
          subPixelShift(0), isValid(false) {}

    SubPixelEdgePoint(const cv::Point2f& pos, float grad, float dir, float shift)
        : position(pos), gradient(grad), direction(dir),
          subPixelShift(shift), isValid(true) {}
};

/**
 * @brief 亚像素边缘检测结果
 */
struct SubPixelEdgeResult {
    std::vector<SubPixelEdgePoint> edgePoints;  ///< 亚像素边缘点集合
    std::vector<float> gradients;               ///< 梯度幅值列表
    std::vector<float> directions;              ///< 梯度方向列表
    double averageSubPixelShift;                ///< 平均亚像素偏移
    double maxGradient;                         ///< 最大梯度值
    int validPointCount;                        ///< 有效点数量
    double processTime;                         ///< 处理时间(ms)

    SubPixelEdgeResult()
        : averageSubPixelShift(0), maxGradient(0),
          validPointCount(0), processTime(0) {}
};

/**
 * @class SubPixelEdgeTool
 * @brief 亚像素边缘检测工具类
 *
 * 实现多种亚像素边缘检测算法：
 * - Sobel梯度插值：基于Sobel算子的梯度方向插值
 * - Zernike矩：使用Zernike正交矩进行边缘定位
 * - 二次曲线拟合：在梯度极值点附近拟合抛物线
 * - 一维高斯拟合：沿梯度方向拟合高斯分布
 * - ESR拟合：Error Surface Regression拟合
 *
 * 精度目标：0.05像素
 */
class SubPixelEdgeTool : public VisionTool {
    Q_OBJECT

public:
    explicit SubPixelEdgeTool(QObject* parent = nullptr);
    ~SubPixelEdgeTool() override = default;

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return Edge; }
    QString toolName() const override { return tr("亚像素边缘检测"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 亚像素边缘检测核心方法 ==========

    /**
     * @brief 检测亚像素边缘
     * @param image 输入图像
     * @param roi 感兴趣区域（空则处理整图）
     * @param config 配置参数
     * @return 检测结果
     */
    SubPixelEdgeResult detectSubPixelEdge(
        const cv::Mat& image,
        const cv::Rect& roi,
        const SubPixelEdgeConfig& config);

    /**
     * @brief Sobel梯度插值实现亚像素定位
     * @param gradX X方向梯度图
     * @param gradY Y方向梯度图
     * @param x 整像素X坐标
     * @param y 整像素Y坐标
     * @return 亚像素坐标
     */
    cv::Point2f interpolateEdgePosition(
        const cv::Mat& gradX,
        const cv::Mat& gradY,
        int x, int y);

    /**
     * @brief Zernike矩亚像素边缘定位
     * @param image 输入图像
     * @param x 整像素X坐标
     * @param y 整像素Y坐标
     * @param windowSize 窗口大小
     * @return 亚像素坐标
     */
    cv::Point2f zernikeMomentSubPixel(
        const cv::Mat& image,
        int x, int y,
        int windowSize = 7);

    /**
     * @brief 二次曲线拟合亚像素定位
     * @param profile 一维剖面数据
     * @param peakIndex 峰值索引
     * @return 亚像素偏移量
     */
    float quadraticFitSubPixel(
        const std::vector<float>& profile,
        int peakIndex);

    /**
     * @brief 一维高斯拟合亚像素定位
     * @param profile 一维剖面数据
     * @param peakIndex 峰值索引
     * @param windowSize 拟合窗口
     * @return 亚像素偏移量
     */
    float gaussianFitSubPixel(
        const std::vector<float>& profile,
        int peakIndex,
        int windowSize = 5);

    /**
     * @brief 沿法线方向提取剖面
     * @param image 输入图像
     * @param point 起始点
     * @param direction 梯度方向
     * @param length 剖面长度
     * @return 剖面数据
     */
    std::vector<float> extractProfileAlongNormal(
        const cv::Mat& image,
        const cv::Point2f& point,
        float direction,
        int length = 11);

    // ========== 参数设置 ==========

    void setConfig(const SubPixelEdgeConfig& config);
    SubPixelEdgeConfig getConfig() const { return config_; }

    void setMethod(SubPixelMethod method);
    SubPixelMethod method() const { return config_.method; }

    void setGradientThreshold(double threshold);
    double gradientThreshold() const { return config_.gradientThreshold; }

    void setWindowSize(int size);
    int windowSize() const { return config_.windowSize; }

    void setUseSubPixel(bool enable);
    bool useSubPixel() const { return config_.useSubPixel; }

    void setSigma(double sigma);
    double sigma() const { return config_.sigma; }

    // ========== 结果获取 ==========

    const SubPixelEdgeResult& lastResult() const { return lastResult_; }

private:
    /**
     * @brief 计算Sobel梯度
     */
    void computeSobelGradient(const cv::Mat& gray, cv::Mat& gradX, cv::Mat& gradY, cv::Mat& magnitude);

    /**
     * @brief 非极大值抑制
     */
    cv::Mat nonMaximumSuppression(const cv::Mat& magnitude, const cv::Mat& gradX, const cv::Mat& gradY);

    /**
     * @brief 计算Zernike矩
     * @param image 输入图像区域
     * @param n, m Zernike矩阶数
     * @return Zernike矩值
     */
    std::complex<double> computeZernikeMoment(const cv::Mat& image, int n, int m);

    /**
     * @brief 从Zernike矩计算边缘参数
     */
    void computeEdgeParamsFromZernike(
        const cv::Mat& patch,
        double& subPixelX,
        double& subPixelY,
        double& edgeAngle);

    /**
     * @brief ESR边缘定位
     */
    cv::Point2f esrSubPixel(const cv::Mat& image, int x, int y);

    /**
     * @brief 双线性插值取值
     */
    float bilinearInterpolate(const cv::Mat& image, float x, float y);

private:
    SubPixelEdgeConfig config_;         ///< 配置参数
    SubPixelEdgeResult lastResult_;     ///< 最近检测结果

    // Zernike矩预计算表
    cv::Mat zernikeRealKernel_;         ///< Zernike实部核
    cv::Mat zernikeImagKernel_;         ///< Zernike虚部核
    bool zernikeKernelReady_ = false;   ///< Zernike核是否已计算
    int zernikeKernelSize_ = 0;         ///< Zernike核大小
};

// ============================================================
// 辅助函数声明
// ============================================================

/**
 * @brief 在整像素边缘点上应用亚像素精化
 * @param grayImage 灰度图像
 * @param edgePoints 整像素边缘点
 * @param method 亚像素方法
 * @param windowSize 窗口大小
 * @return 亚像素边缘点
 */
std::vector<cv::Point2f> refineEdgesToSubPixel(
    const cv::Mat& grayImage,
    const std::vector<cv::Point>& edgePoints,
    SubPixelMethod method = SubPixelMethod::QuadraticFit,
    int windowSize = 5);

/**
 * @brief 亚像素圆心拟合
 * @param edgePoints 边缘点集合
 * @param centerX 输出圆心X
 * @param centerY 输出圆心Y
 * @param radius 输出半径
 * @return 拟合残差
 */
double fitCircleSubPixel(
    const std::vector<cv::Point2f>& edgePoints,
    double& centerX,
    double& centerY,
    double& radius);

/**
 * @brief 亚像素直线拟合
 * @param edgePoints 边缘点集合
 * @param lineParams 输出直线参数 [a, b, c] (ax + by + c = 0)
 * @return 拟合残差
 */
double fitLineSubPixel(
    const std::vector<cv::Point2f>& edgePoints,
    cv::Vec3d& lineParams);

} // namespace Algorithm
} // namespace VisionForge
