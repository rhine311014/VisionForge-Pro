/**
 * @file InstanceSegmentation.h
 * @brief 工业级实例分割模块
 *
 * 支持多种分割架构：
 * - YOLOv8-seg / YOLOv11-seg
 * - Mask R-CNN
 * - SAM (Segment Anything Model)
 *
 * @author VisionForge Pro Team
 * @version 1.0.0
 */

#pragma once

#include "InferenceEngine.h"
#include <QObject>
#include <QImage>
#include <QRectF>
#include <QPolygonF>
#include <opencv2/core.hpp>
#include <vector>
#include <memory>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// 分割模型类型
// ============================================================
enum class SegmentationModel {
    YOLOv8Seg,      // YOLOv8-seg
    YOLOv11Seg,     // YOLOv11-seg
    MaskRCNN,       // Mask R-CNN
    SAM,            // Segment Anything Model
    Custom          // 自定义模型
};

// ============================================================
// 分割结果结构
// ============================================================
struct SegmentedInstance {
    int instanceId = -1;          // 实例ID
    int classId = -1;             // 类别ID
    QString className;            // 类别名称
    float confidence = 0.0f;      // 置信度

    // 边界框
    QRectF boundingBox;           // 轴对齐边界框

    // 掩码
    cv::Mat mask;                 // 二值掩码 (CV_8UC1, 0/255)
    cv::Mat probabilityMask;      // 概率掩码 (CV_32FC1, 0.0-1.0)

    // 轮廓
    std::vector<QPolygonF> contours;      // 外轮廓
    std::vector<QPolygonF> holes;         // 内部孔洞

    // 几何属性
    float area = 0.0f;            // 面积 (像素)
    float perimeter = 0.0f;       // 周长 (像素)
    QPointF centroid;             // 质心
    float aspectRatio = 0.0f;     // 长宽比
    float solidity = 0.0f;        // 实心度 (面积/凸包面积)
    float circularity = 0.0f;     // 圆形度 (4π*面积/周长²)

    // 旋转矩形
    QPointF rotatedCenter;        // 旋转矩形中心
    QSizeF rotatedSize;           // 旋转矩形尺寸
    float rotatedAngle = 0.0f;    // 旋转角度 (度)

    // 扩展数据
    QJsonObject metadata;         // 额外元数据
};

// ============================================================
// 分割配置
// ============================================================
struct SegmentationConfig {
    // 模型设置
    SegmentationModel modelType = SegmentationModel::YOLOv8Seg;
    QString modelPath;
    QStringList classNames;

    // 检测参数
    float confidenceThreshold = 0.5f;
    float nmsThreshold = 0.45f;
    float maskThreshold = 0.5f;      // 掩码二值化阈值

    // 输入设置
    int inputWidth = 640;
    int inputHeight = 640;
    bool letterBox = true;

    // 掩码后处理
    bool refineMask = true;          // 使用GrabCut精化掩码
    int refineMaskIterations = 3;    // 精化迭代次数
    bool extractContours = true;     // 提取轮廓
    double contourApproxEpsilon = 2.0; // 轮廓近似精度
    int minContourArea = 100;        // 最小轮廓面积

    // 过滤器
    float minArea = 0.0f;            // 最小面积过滤
    float maxArea = 0.0f;            // 最大面积过滤 (0=不限制)
    std::vector<int> targetClasses;  // 目标类别 (空=所有类别)

    // 性能设置
    InferenceBackend backend = InferenceBackend::ONNXRuntime;
    ExecutionDevice device = ExecutionDevice::Auto;

    // 序列化
    QJsonObject toJson() const;
    static SegmentationConfig fromJson(const QJsonObject& json);
};

// ============================================================
// 分割结果
// ============================================================
struct SegmentationResult {
    bool success = false;
    QString errorMessage;

    // 实例列表
    std::vector<SegmentedInstance> instances;

    // 合并掩码
    cv::Mat combinedMask;            // 所有实例的合并掩码 (类别ID作为值)
    cv::Mat colorMask;               // 彩色可视化掩码

    // 统计信息
    int instanceCount = 0;
    std::map<int, int> classCount;   // 每个类别的实例数量

    // 性能统计
    float preprocessTime = 0.0f;     // 预处理时间 (ms)
    float inferenceTime = 0.0f;      // 推理时间 (ms)
    float postprocessTime = 0.0f;    // 后处理时间 (ms)
    float totalTime = 0.0f;          // 总时间 (ms)

    // 辅助方法
    SegmentedInstance* getInstanceById(int id);
    std::vector<SegmentedInstance*> getInstancesByClass(int classId);
    cv::Mat getMaskByClass(int classId) const;
};

// ============================================================
// 实例分割器
// ============================================================
class InstanceSegmentor : public QObject {
    Q_OBJECT

public:
    explicit InstanceSegmentor(QObject* parent = nullptr);
    ~InstanceSegmentor();

    // 初始化
    bool initialize(const SegmentationConfig& config);
    bool loadModel(const QString& modelPath);
    void release();

    // 分割
    SegmentationResult segment(const cv::Mat& image);
    SegmentationResult segment(const QImage& image);

    // 批量分割
    std::vector<SegmentationResult> segmentBatch(const std::vector<cv::Mat>& images);

    // SAM特定功能
    SegmentationResult segmentWithPoints(const cv::Mat& image,
                                         const std::vector<QPointF>& points,
                                         const std::vector<int>& labels);  // 1=前景, 0=背景
    SegmentationResult segmentWithBox(const cv::Mat& image, const QRectF& box);

    // 掩码操作
    static cv::Mat refineMaskWithGrabCut(const cv::Mat& image,
                                         const cv::Mat& mask,
                                         int iterations = 3);
    static std::vector<QPolygonF> extractContours(const cv::Mat& mask,
                                                  double approxEpsilon = 2.0);
    static void computeGeometricProperties(SegmentedInstance& instance);

    // 可视化
    cv::Mat visualize(const cv::Mat& image,
                      const SegmentationResult& result,
                      float maskAlpha = 0.5f,
                      bool showContours = true,
                      bool showLabels = true);

    // 配置
    void setConfig(const SegmentationConfig& config);
    SegmentationConfig getConfig() const { return m_config; }

    // 状态
    bool isInitialized() const { return m_initialized; }
    QString getLastError() const { return m_lastError; }

signals:
    void progressChanged(int percent);
    void segmentationComplete(const SegmentationResult& result);
    void errorOccurred(const QString& error);

private:
    // 预处理
    cv::Mat preprocess(const cv::Mat& image, float& scaleX, float& scaleY,
                       int& padLeft, int& padTop);

    // 后处理
    SegmentationResult postprocessYOLOSeg(const std::vector<Tensor>& outputs,
                                          const cv::Size& originalSize,
                                          float scaleX, float scaleY,
                                          int padLeft, int padTop);
    SegmentationResult postprocessMaskRCNN(const std::vector<Tensor>& outputs,
                                           const cv::Size& originalSize);
    SegmentationResult postprocessSAM(const std::vector<Tensor>& outputs,
                                      const cv::Size& originalSize);

    // 掩码处理
    cv::Mat processMaskPrototypes(const cv::Mat& prototypes,
                                  const std::vector<float>& coefficients,
                                  const QRectF& box,
                                  const cv::Size& originalSize);

    // 生成颜色
    static cv::Vec3b getClassColor(int classId);
    static std::vector<cv::Vec3b> generateColorPalette(int numClasses);

private:
    SegmentationConfig m_config;
    std::unique_ptr<InferenceEngine> m_engine;
    bool m_initialized = false;
    QString m_lastError;

    // 颜色调色板
    std::vector<cv::Vec3b> m_colorPalette;
};

// ============================================================
// 语义分割器 (像素级分类)
// ============================================================
class SemanticSegmentor : public QObject {
    Q_OBJECT

public:
    struct Config {
        QString modelPath;
        QStringList classNames;
        int inputWidth = 512;
        int inputHeight = 512;
        InferenceBackend backend = InferenceBackend::ONNXRuntime;
        ExecutionDevice device = ExecutionDevice::Auto;

        QJsonObject toJson() const;
        static Config fromJson(const QJsonObject& json);
    };

    struct Result {
        bool success = false;
        QString errorMessage;

        cv::Mat segmentationMap;     // CV_8UC1, 每个像素的类别ID
        cv::Mat confidenceMap;       // CV_32FC1, 每个像素的置信度
        cv::Mat colorMap;            // CV_8UC3, 彩色可视化

        std::map<int, float> classRatios;  // 每个类别占比

        float inferenceTime = 0.0f;
    };

public:
    explicit SemanticSegmentor(QObject* parent = nullptr);
    ~SemanticSegmentor();

    bool initialize(const Config& config);
    void release();

    Result segment(const cv::Mat& image);
    Result segment(const QImage& image);

    // 可视化
    cv::Mat visualize(const cv::Mat& image, const Result& result,
                      float alpha = 0.5f);

    bool isInitialized() const { return m_initialized; }

private:
    cv::Mat preprocess(const cv::Mat& image);
    Result postprocess(const Tensor& output, const cv::Size& originalSize);

private:
    Config m_config;
    std::unique_ptr<InferenceEngine> m_engine;
    bool m_initialized = false;
    std::vector<cv::Vec3b> m_colorPalette;
};

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
