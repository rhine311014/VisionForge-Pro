/**
 * @file YOLODetector.h
 * @brief YOLO系列检测器
 *
 * 支持多种YOLO版本和任务：
 * - YOLOv5/v8/v11检测
 * - YOLOv8-seg实例分割
 * - YOLOv8-pose姿态估计
 * - YOLOv8-obb旋转框检测
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_YOLO_DETECTOR_H
#define VISIONFORGE_YOLO_DETECTOR_H

#include "algorithm/ai/InferenceEngine.h"
#include <QObject>
#include <QString>
#include <QRectF>
#include <QPointF>
#include <QPolygonF>
#include <opencv2/core.hpp>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// 枚举和结构体
// ============================================================

/**
 * @brief YOLO模型版本
 */
enum class YOLOVersion {
    YOLOv5,         ///< YOLOv5
    YOLOv8,         ///< YOLOv8
    YOLOv11,        ///< YOLOv11 (YOLO11)
    Auto            ///< 自动检测
};

/**
 * @brief YOLO任务模式
 */
enum class YOLOTaskMode {
    Detect,         ///< 目标检测
    Segment,        ///< 实例分割
    Pose,           ///< 姿态估计
    OBB,            ///< 旋转框检测
    Classify        ///< 图像分类
};

/**
 * @brief 检测框结果
 */
struct DetectionBox {
    int classId = -1;           ///< 类别ID
    QString className;          ///< 类别名称
    float confidence = 0;       ///< 置信度
    QRectF box;                 ///< 边界框 (x, y, width, height)
    QPointF center;             ///< 中心点

    // 旋转框（OBB任务）
    float angle = 0;            ///< 旋转角度（度）
    QPolygonF rotatedBox;       ///< 旋转框四角点

    // 实例分割（Segment任务）
    cv::Mat mask;               ///< 实例掩码
    std::vector<QPointF> contour;  ///< 轮廓点

    // 姿态估计（Pose任务）
    std::vector<QPointF> keypoints;     ///< 关键点
    std::vector<float> keypointScores;  ///< 关键点置信度

    QJsonObject toJson() const;
};

/**
 * @brief YOLO检测配置
 */
struct YOLOConfig {
    YOLOVersion version = YOLOVersion::Auto;
    YOLOTaskMode taskMode = YOLOTaskMode::Detect;

    float confidenceThreshold = 0.5f;   ///< 置信度阈值
    float nmsThreshold = 0.45f;         ///< NMS阈值
    float maskThreshold = 0.5f;         ///< 掩码阈值（分割任务）

    QSize inputSize = QSize(640, 640);  ///< 输入尺寸
    bool letterBox = true;              ///< 是否使用letterbox填充
    bool agnosticNMS = false;           ///< 类别无关NMS

    int maxDetections = 300;            ///< 最大检测数量
    QStringList classNames;             ///< 类别名称

    // 姿态估计配置
    int numKeypoints = 17;              ///< 关键点数量（COCO: 17）

    QJsonObject toJson() const;
    static YOLOConfig fromJson(const QJsonObject& json);
};

/**
 * @brief YOLO检测结果
 */
struct YOLOResult {
    bool success = false;
    QString errorMessage;

    std::vector<DetectionBox> detections;   ///< 检测结果
    cv::Mat segmentationMask;               ///< 语义分割掩码（可选）
    cv::Mat visualizedImage;                ///< 可视化结果

    double preprocessTime = 0;
    double inferenceTime = 0;
    double postprocessTime = 0;
    double totalTime = 0;

    int detectionCount() const { return static_cast<int>(detections.size()); }
};

// ============================================================
// YOLO检测器
// ============================================================

/**
 * @brief YOLO系列检测器
 *
 * 统一的YOLO检测接口，支持多版本和多任务。
 */
class YOLODetector : public QObject {
    Q_OBJECT

public:
    explicit YOLODetector(QObject* parent = nullptr);
    ~YOLODetector() override;

    // ============ 模型管理 ============

    /**
     * @brief 加载模型
     */
    bool loadModel(const QString& modelPath,
                   InferenceBackend backend = InferenceBackend::Auto);

    /**
     * @brief 卸载模型
     */
    void unloadModel();

    /**
     * @brief 是否已加载模型
     */
    bool isModelLoaded() const { return m_modelLoaded; }

    /**
     * @brief 设置推理引擎
     */
    void setInferenceEngine(std::shared_ptr<InferenceEngine> engine);

    // ============ 配置 ============

    /**
     * @brief 设置配置
     */
    void setConfig(const YOLOConfig& config);
    const YOLOConfig& config() const { return m_config; }

    /**
     * @brief 设置置信度阈值
     */
    void setConfidenceThreshold(float threshold);

    /**
     * @brief 设置NMS阈值
     */
    void setNmsThreshold(float threshold);

    /**
     * @brief 设置类别名称
     */
    void setClassNames(const QStringList& names);

    /**
     * @brief 设置输入尺寸
     */
    void setInputSize(const QSize& size);

    // ============ 检测 ============

    /**
     * @brief 执行检测
     */
    YOLOResult detect(const cv::Mat& image);

    /**
     * @brief 批量检测
     */
    std::vector<YOLOResult> detectBatch(const std::vector<cv::Mat>& images);

    /**
     * @brief 在图像上绘制结果
     */
    cv::Mat drawResults(const cv::Mat& image, const YOLOResult& result);

    // ============ 信息 ============

    /**
     * @brief 获取检测到的版本
     */
    YOLOVersion detectedVersion() const { return m_detectedVersion; }

    /**
     * @brief 获取最后错误
     */
    QString lastError() const { return m_lastError; }

signals:
    void modelLoaded(bool success);
    void detectionCompleted(int detectionCount, double timeMs);
    void errorOccurred(const QString& error);

private:
    // 预处理
    cv::Mat preprocess(const cv::Mat& image, float& scale, int& padLeft, int& padTop);
    cv::Mat letterBoxImage(const cv::Mat& image, const cv::Size& targetSize,
                           float& scale, int& padLeft, int& padTop);

    // 后处理
    void postprocessDetect(const cv::Mat& output, YOLOResult& result,
                           const cv::Size& originalSize, float scale,
                           int padLeft, int padTop);
    void postprocessSegment(const std::vector<cv::Mat>& outputs, YOLOResult& result,
                            const cv::Size& originalSize, float scale,
                            int padLeft, int padTop);
    void postprocessPose(const cv::Mat& output, YOLOResult& result,
                         const cv::Size& originalSize, float scale,
                         int padLeft, int padTop);
    void postprocessOBB(const cv::Mat& output, YOLOResult& result,
                        const cv::Size& originalSize, float scale,
                        int padLeft, int padTop);

    // NMS
    void applyNMS(std::vector<DetectionBox>& detections);

    // 掩码处理
    cv::Mat processSegmentMask(const cv::Mat& protos, const cv::Mat& maskCoeffs,
                               const QRectF& box, const cv::Size& originalSize);

    // 版本检测
    YOLOVersion detectVersion(const cv::Mat& output);

    // 绘制辅助
    cv::Scalar getClassColor(int classId);
    void drawBox(cv::Mat& image, const DetectionBox& det);
    void drawMask(cv::Mat& image, const DetectionBox& det);
    void drawKeypoints(cv::Mat& image, const DetectionBox& det);
    void drawOBB(cv::Mat& image, const DetectionBox& det);

private:
    // 推理引擎
    std::shared_ptr<InferenceEngine> m_engine;
    cv::dnn::Net m_cvNet;  // OpenCV DNN后备

    // 配置和状态
    YOLOConfig m_config;
    bool m_modelLoaded = false;
    YOLOVersion m_detectedVersion = YOLOVersion::Auto;
    QString m_lastError;

    // 预定义颜色
    std::vector<cv::Scalar> m_classColors;

    // 骨骼连接（姿态估计）
    std::vector<std::pair<int, int>> m_skeleton;
};

// ============================================================
// 工具函数
// ============================================================

/**
 * @brief 获取YOLO版本名称
 */
QString getYOLOVersionName(YOLOVersion version);

/**
 * @brief 获取任务模式名称
 */
QString getYOLOTaskModeName(YOLOTaskMode mode);

/**
 * @brief 加载COCO类别名称
 */
QStringList loadCOCOClassNames();

/**
 * @brief 加载类别名称文件
 */
QStringList loadClassNamesFromFile(const QString& filePath);

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge

#endif // VISIONFORGE_YOLO_DETECTOR_H
