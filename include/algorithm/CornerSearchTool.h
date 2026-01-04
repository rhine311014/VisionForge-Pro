/**
 * @file CornerSearchTool.h
 * @brief 角点搜索工具 - 支持多种角点检测算法的视觉工具
 *
 * @details
 * 本工具是VisionForge视觉检测系统中的特征点检测工具，用于在图像中
 * 检测和定位角点特征。角点是图像中两条边缘交汇的点，具有良好的
 * 可重复性和区分性，是许多计算机视觉应用的基础。
 *
 * 主要功能特点：
 * 1. 支持多种检测算法：Harris、Shi-Tomasi、FAST、ORB
 * 2. 支持亚像素精度：可通过角点细化提高定位精度
 * 3. 支持参数调整：每种算法都有独立的参数设置
 * 4. 支持数量限制：可限制检测的最大角点数量
 *
 * 支持的角点检测算法：
 *
 * 1. Harris角点检测器
 *    - 经典的角点检测算法，基于图像梯度的自相关矩阵
 *    - 对图像旋转具有不变性
 *    - 适用于需要检测"真正"角点的场景
 *
 * 2. Shi-Tomasi角点检测器（Good Features to Track）
 *    - Harris的改进版本，使用最小特征值作为角点度量
 *    - 检测的角点更适合用于光流跟踪
 *    - 是OpenCV中goodFeaturesToTrack函数的实现
 *
 * 3. FAST特征点检测器
 *    - 高速角点检测算法，适合实时应用
 *    - 基于像素强度比较的简单规则
 *    - 速度最快，但可能检测到更多噪声点
 *
 * 4. ORB特征点检测器
 *    - 结合了FAST检测器和BRIEF描述子
 *    - 具有旋转和尺度不变性
 *    - 适用于特征匹配和目标识别
 *
 * 典型应用场景：
 * - 特征点跟踪（光流法）
 * - 图像配准和拼接
 * - 相机标定（棋盘格角点检测）
 * - SLAM中的特征提取
 * - 目标识别中的特征匹配
 *
 * @author VisionForge Team
 * @date 2025-12-30
 * @version 1.0
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 角点检测结果结构体
 *
 * 用于存储单个检测到的角点的详细信息。
 * 不同的检测算法可能只填充部分字段。
 */
struct CornerPoint {
    double x = 0;           ///< 角点X坐标（列坐标），单位：像素
    double y = 0;           ///< 角点Y坐标（行坐标），单位：像素
    double response = 0;    ///< 角点响应值/强度，值越大表示角点越显著
    double angle = 0;       ///< 特征点方向角度（仅ORB等有方向的特征点有效），单位：度
    double size = 0;        ///< 特征点尺度/大小（仅支持尺度的检测器有效），单位：像素
};

/**
 * @class CornerSearchTool
 * @brief 角点搜索工具类
 *
 * 该类封装了多种角点检测算法，是VisionTool的派生类。
 * 提供了统一的接口来访问不同的角点检测方法。
 *
 * 使用流程：
 * 1. 创建CornerSearchTool实例
 * 2. 通过setMethod()选择检测算法
 * 3. 设置对应算法的参数
 * 4. 调用process()执行检测
 * 5. 通过corners()获取检测结果
 *
 * 线程安全性：本类不是线程安全的，如需在多线程环境下使用，
 * 请为每个线程创建独立的实例。
 */
class CornerSearchTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 检测方法枚举
     *
     * 定义了四种不同的角点检测算法
     */
    enum Method {
        Harris = 0,         ///< Harris角点检测：经典算法，适合检测真正的角点
        ShiTomasi,          ///< Shi-Tomasi角点检测：改进版Harris，适合特征跟踪
        FAST,               ///< FAST特征点检测：高速检测，适合实时应用
        ORB                 ///< ORB特征点检测：具有旋转和尺度不变性
    };

    /**
     * @brief 构造函数
     * @param parent 父对象指针，用于Qt对象树管理
     */
    explicit CornerSearchTool(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     *
     * 使用默认实现，无需特殊资源清理
     */
    ~CornerSearchTool() override = default;

    // ==================== VisionTool接口实现 ====================

    /**
     * @brief 获取工具类型
     * @return 返回Custom类型
     */
    ToolType toolType() const override { return CornerSearch; }

    /**
     * @brief 获取工具显示名称
     * @return 返回本地化的工具名称"角点搜索"
     */
    QString toolName() const override { return tr("角点搜索"); }

    /**
     * @brief 执行角点检测处理
     *
     * 根据设置的检测方法，在输入图像上执行角点检测。
     *
     * @param input 输入图像数据指针，支持灰度图和彩色图（内部会自动转换）
     * @param output 输出结果结构，包含检测到的角点数量和详细信息
     * @return true表示检测执行成功，false表示执行出错
     */
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    /**
     * @brief 序列化参数到JSON
     * @return 包含所有参数的JSON对象
     */
    QJsonObject serializeParams() const override;

    /**
     * @brief 从JSON反序列化参数
     * @param json 包含参数的JSON对象
     */
    void deserializeParams(const QJsonObject& json) override;

    // ==================== 参数设置方法 ====================

    // ---------- 检测方法相关 ----------

    /**
     * @brief 获取当前检测方法
     * @return 当前设置的检测方法
     */
    Method method() const { return method_; }

    /**
     * @brief 设置检测方法
     * @param m 检测方法，参见Method枚举
     */
    void setMethod(Method m) { method_ = m; }

    // ---------- Harris参数相关 ----------

    /**
     * @brief 获取Harris检测器的邻域块大小
     * @return 块大小，单位：像素
     */
    int harrisBlockSize() const { return harrisBlockSize_; }

    /**
     * @brief 设置Harris检测器的邻域块大小
     *
     * 用于计算梯度协方差矩阵的邻域大小。
     * 较大的值可以检测到更大尺度的角点，但会降低定位精度。
     *
     * @param size 块大小，范围[2,31]
     */
    void setHarrisBlockSize(int size) { harrisBlockSize_ = qBound(2, size, 31); }

    /**
     * @brief 获取Harris检测器的Sobel算子大小
     * @return Sobel算子大小（奇数）
     */
    int harrisKSize() const { return harrisKSize_; }

    /**
     * @brief 设置Harris检测器的Sobel算子大小
     *
     * 用于计算图像梯度的Sobel算子孔径大小。
     * 必须为奇数，通常使用3或5。
     *
     * @param size Sobel算子大小（将自动转换为奇数）
     */
    void setHarrisKSize(int size) { harrisKSize_ = size | 1; }  // 位或1确保为奇数

    /**
     * @brief 获取Harris检测器的k参数
     * @return k参数值
     */
    double harrisK() const { return harrisK_; }

    /**
     * @brief 设置Harris检测器的k参数
     *
     * Harris响应函数中的自由参数k。
     * R = det(M) - k * trace(M)^2
     * 通常取值在0.04到0.06之间。
     *
     * @param k k参数，范围[0.01,0.2]
     */
    void setHarrisK(double k) { harrisK_ = qBound(0.01, k, 0.2); }

    /**
     * @brief 获取Harris检测器的阈值
     * @return 阈值，范围[0,1]
     */
    double harrisThreshold() const { return harrisThreshold_; }

    /**
     * @brief 设置Harris检测器的阈值
     *
     * 只有归一化后的Harris响应值大于此阈值的点才被认为是角点。
     * 较高的阈值可以减少检测数量，保留更显著的角点。
     *
     * @param t 阈值，范围[0,1]
     */
    void setHarrisThreshold(double t) { harrisThreshold_ = qBound(0.0, t, 1.0); }

    // ---------- Shi-Tomasi参数相关 ----------

    /**
     * @brief 获取最大角点数量
     * @return 最大角点数量
     */
    int maxCorners() const { return maxCorners_; }

    /**
     * @brief 设置最大角点数量
     *
     * 限制返回的最大角点数量。算法会返回响应值最高的前N个角点。
     * 此参数对所有检测方法都有效。
     *
     * @param count 最大数量，最小值为1
     */
    void setMaxCorners(int count) { maxCorners_ = qMax(1, count); }

    /**
     * @brief 获取质量水平参数
     * @return 质量水平，范围[0,1]
     */
    double qualityLevel() const { return qualityLevel_; }

    /**
     * @brief 设置质量水平参数
     *
     * Shi-Tomasi检测器的质量水平参数。
     * 表示角点的最小可接受质量水平，相对于最佳角点的响应值。
     * 例如，0.01表示角点的响应值必须至少是最佳角点响应值的1%。
     *
     * @param q 质量水平，范围[0.001,1]
     */
    void setQualityLevel(double q) { qualityLevel_ = qBound(0.001, q, 1.0); }

    /**
     * @brief 获取最小角点间距
     * @return 最小间距，单位：像素
     */
    double minDistance() const { return minDistance_; }

    /**
     * @brief 设置最小角点间距
     *
     * 检测到的角点之间的最小欧氏距离。
     * 在此距离内，只保留响应值最高的角点。
     * 用于避免角点过于密集。
     *
     * @param d 最小间距，单位：像素，最小值为0
     */
    void setMinDistance(double d) { minDistance_ = qMax(0.0, d); }

    // ---------- FAST参数相关 ----------

    /**
     * @brief 获取FAST检测器的阈值
     * @return 阈值，范围[1,255]
     */
    int fastThreshold() const { return fastThreshold_; }

    /**
     * @brief 设置FAST检测器的阈值
     *
     * FAST检测器的强度差阈值。
     * 当某点周围像素与中心点的强度差超过此阈值的数量
     * 满足条件时，该点被认为是特征点。
     *
     * @param t 阈值，范围[1,255]
     */
    void setFastThreshold(int t) { fastThreshold_ = qBound(1, t, 255); }

    /**
     * @brief 获取FAST检测器是否使用非极大值抑制
     * @return true表示使用NMS，false表示不使用
     */
    bool fastNMS() const { return fastNMS_; }

    /**
     * @brief 设置FAST检测器是否使用非极大值抑制
     *
     * 非极大值抑制（NMS）可以去除相邻的冗余特征点，
     * 只保留局部响应值最高的点。
     *
     * @param nms true启用NMS，false禁用
     */
    void setFastNMS(bool nms) { fastNMS_ = nms; }

    // ---------- 通用参数相关 ----------

    /**
     * @brief 获取是否使用亚像素精化
     * @return true表示使用亚像素精化
     */
    bool useSubPixel() const { return useSubPixel_; }

    /**
     * @brief 设置是否使用亚像素精化
     *
     * 启用后，将使用cornerSubPix函数对角点位置进行亚像素级别的精化，
     * 可以提高定位精度到0.1像素级别。
     * 注意：仅对Harris和Shi-Tomasi检测器有效。
     *
     * @param use true启用，false禁用
     */
    void setUseSubPixel(bool use) { useSubPixel_ = use; }

    /**
     * @brief 获取亚像素精化的窗口大小
     * @return 窗口大小（奇数）
     */
    int subPixelWinSize() const { return subPixelWinSize_; }

    /**
     * @brief 设置亚像素精化的窗口大小
     *
     * 亚像素精化时搜索窗口的半边长。
     * 例如，设置为5表示使用11x11的搜索窗口。
     *
     * @param size 窗口大小（将自动转换为奇数）
     */
    void setSubPixelWinSize(int size) { subPixelWinSize_ = size | 1; }

    // ==================== 结果获取 ====================

    /**
     * @brief 获取检测到的所有角点
     *
     * 返回最近一次process()调用检测到的所有角点。
     *
     * @return 角点向量的常引用
     */
    const std::vector<CornerPoint>& corners() const { return corners_; }

private:
    /**
     * @brief 使用Harris算法检测角点
     * @param gray 灰度输入图像
     * @return 是否成功
     */
    bool processHarris(const cv::Mat& gray);

    /**
     * @brief 使用Shi-Tomasi算法检测角点
     * @param gray 灰度输入图像
     * @return 是否成功
     */
    bool processShiTomasi(const cv::Mat& gray);

    /**
     * @brief 使用FAST算法检测特征点
     * @param gray 灰度输入图像
     * @return 是否成功
     */
    bool processFAST(const cv::Mat& gray);

    /**
     * @brief 使用ORB算法检测特征点
     * @param gray 灰度输入图像
     * @return 是否成功
     */
    bool processORB(const cv::Mat& gray);

private:
    // ==================== 检测方法 ====================
    Method method_ = ShiTomasi;     ///< 当前使用的检测方法，默认Shi-Tomasi

    // ==================== Harris参数 ====================
    int harrisBlockSize_ = 2;       ///< Harris邻域块大小
    int harrisKSize_ = 3;           ///< Harris Sobel算子大小
    double harrisK_ = 0.04;         ///< Harris响应函数k参数
    double harrisThreshold_ = 0.01; ///< Harris响应阈值

    // ==================== Shi-Tomasi参数 ====================
    int maxCorners_ = 100;          ///< 最大角点数量（也用于其他算法）
    double qualityLevel_ = 0.01;    ///< 质量水平参数
    double minDistance_ = 10.0;     ///< 最小角点间距
    int blockSize_ = 3;             ///< 计算块大小

    // ==================== FAST参数 ====================
    int fastThreshold_ = 10;        ///< FAST强度差阈值
    bool fastNMS_ = true;           ///< 是否使用非极大值抑制
    int fastType_ = cv::FastFeatureDetector::TYPE_9_16;  ///< FAST检测器类型

    // ==================== 通用参数 ====================
    bool useSubPixel_ = true;       ///< 是否使用亚像素精化
    int subPixelWinSize_ = 5;       ///< 亚像素精化窗口大小

    // ==================== 检测结果 ====================
    std::vector<CornerPoint> corners_;  ///< 存储检测到的角点
};

} // namespace Algorithm
} // namespace VisionForge
