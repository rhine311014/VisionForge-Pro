/**
 * @file AIDetectionTool.h
 * @brief AI检测工具
 * @details 使用深度学习模型进行目标检测和分类
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <opencv2/dnn.hpp>
#include <QRectF>
#include <vector>
#include <memory>

#ifdef USE_HALCON
#include <HalconCpp.h>
#endif

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 检测结果
 */
struct DetectionResult {
    int classId;                // 类别ID
    QString className;          // 类别名称
    float confidence;           // 置信度
    QRectF boundingBox;         // 边界框
    QPointF center;             // 中心点

    DetectionResult()
        : classId(-1), confidence(0) {}

    DetectionResult(int id, const QString& name, float conf, const QRectF& box)
        : classId(id), className(name), confidence(conf), boundingBox(box)
    {
        center = box.center();
    }
};

/**
 * @brief OCR识别结果
 */
struct OCRResult {
    QString text;               // 识别文本
    float confidence;           // 置信度
    QRectF boundingBox;         // 文字区域边界框
    double angle;               // 文字角度

    OCRResult()
        : confidence(0), angle(0) {}

    OCRResult(const QString& txt, float conf, const QRectF& box, double ang = 0)
        : text(txt), confidence(conf), boundingBox(box), angle(ang) {}
};

/**
 * @brief AI检测工具
 *
 * 支持多种深度学习任务：
 * - 目标检测（YOLO、SSD等）
 * - 图像分类
 * - 缺陷检测
 */
class AIDetectionTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 任务类型
     */
    enum TaskType {
        ObjectDetection,    // 目标检测
        Classification,     // 图像分类
        DefectDetection,    // 缺陷检测
        Segmentation,       // 语义分割
        OCR                 // 文字识别 (OCR/Deep OCR)
    };
    Q_ENUM(TaskType)

    /**
     * @brief 推理引擎
     */
    enum InferenceEngine {
        OpenCVDNN,          // OpenCV DNN模块
        ONNXRuntime,        // ONNX Runtime
        TensorRT,           // NVIDIA TensorRT
        HikvisionDL,        // 海康深度学习推理
        HalconDL            // Halcon深度学习推理
    };
    Q_ENUM(InferenceEngine)

    explicit AIDetectionTool(QObject* parent = nullptr);
    virtual ~AIDetectionTool();

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return AIDetection; }
    QString toolName() const override { return tr("AI检测"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 模型管理 ==========

    /**
     * @brief 加载模型
     * @param modelPath 模型文件路径（.onnx, .pb, .caffemodel等）
     * @param configPath 配置文件路径（可选）
     * @return true如果加载成功
     */
    bool loadModel(const QString& modelPath, const QString& configPath = QString());

    /**
     * @brief 卸载模型
     */
    void unloadModel();

    /**
     * @brief 是否已加载模型
     */
    bool isModelLoaded() const { return modelLoaded_; }

    /**
     * @brief 获取模型路径
     */
    QString modelPath() const { return modelPath_; }

    // ========== 参数设置 ==========

    /**
     * @brief 设置任务类型
     */
    void setTaskType(TaskType type);
    TaskType taskType() const { return taskType_; }

    /**
     * @brief 设置推理引擎
     */
    void setInferenceEngine(InferenceEngine engine);
    InferenceEngine inferenceEngine() const { return inferenceEngine_; }

    /**
     * @brief 设置置信度阈值
     */
    void setConfidenceThreshold(float threshold);
    float confidenceThreshold() const { return confidenceThreshold_; }

    /**
     * @brief 设置NMS阈值
     */
    void setNmsThreshold(float threshold);
    float nmsThreshold() const { return nmsThreshold_; }

    /**
     * @brief 设置输入尺寸
     */
    void setInputSize(int width, int height);
    QSize inputSize() const { return inputSize_; }

    /**
     * @brief 设置类别名称列表
     */
    void setClassNames(const QStringList& names);
    QStringList classNames() const { return classNames_; }

    /**
     * @brief 设置是否使用GPU
     */
    void setUseGPU(bool use);
    bool useGPU() const { return useGPU_; }

    // ========== 结果获取 ==========

    /**
     * @brief 获取检测结果列表
     */
    const std::vector<DetectionResult>& detections() const { return detections_; }

    /**
     * @brief 获取检测数量
     */
    int detectionCount() const { return static_cast<int>(detections_.size()); }

    /**
     * @brief 获取OCR识别结果列表
     */
    const std::vector<OCRResult>& ocrResults() const { return ocrResults_; }

    /**
     * @brief 获取OCR识别数量
     */
    int ocrResultCount() const { return static_cast<int>(ocrResults_.size()); }

private:
    /**
     * @brief 预处理图像
     */
    cv::Mat preprocess(const cv::Mat& input);

    /**
     * @brief 后处理检测结果
     */
    void postprocessDetection(const cv::Mat& output, const cv::Size& originalSize);

    /**
     * @brief 后处理分类结果
     */
    void postprocessClassification(const cv::Mat& output);

    /**
     * @brief 非极大值抑制
     */
    void applyNMS(std::vector<DetectionResult>& detections);

    /**
     * @brief 绘制检测结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

    /**
     * @brief 使用OpenCV DNN推理
     */
    bool inferWithOpenCVDNN(const cv::Mat& input, cv::Mat& output);

#ifdef USE_HALCON
    /**
     * @brief 使用Halcon深度学习推理
     */
    bool inferWithHalconDL(const cv::Mat& input, ToolResult& output);

    /**
     * @brief 使用Halcon Deep OCR推理
     */
    bool inferWithHalconDeepOCR(const cv::Mat& input, ToolResult& output);

    /**
     * @brief 加载Halcon DL模型
     */
    bool loadHalconDLModel(const QString& modelPath);

    /**
     * @brief 加载Halcon Deep OCR模型
     */
    bool loadHalconDeepOCRModel(const QString& modelPath);

    /**
     * @brief 卸载Halcon DL模型
     */
    void unloadHalconDLModel();
#endif

private:
    // 模型参数
    QString modelPath_;
    QString configPath_;
    bool modelLoaded_;

    // 推理参数
    TaskType taskType_;
    InferenceEngine inferenceEngine_;
    float confidenceThreshold_;
    float nmsThreshold_;
    QSize inputSize_;
    QStringList classNames_;
    bool useGPU_;

    // OpenCV DNN网络
    cv::dnn::Net net_;

#ifdef USE_HALCON
    // Halcon DL模型 (使用指针避免头文件依赖)
    void* halconDLModel_;  // 实际类型为 HalconCpp::HDlModel*
    bool halconModelLoaded_;

    // Halcon Deep OCR句柄
    void* halconDeepOCRHandle_;  // 实际类型为 HalconCpp::HDeepOcr* 或 HTuple (句柄)
    bool halconDeepOCRLoaded_;
#endif

    // 结果
    std::vector<DetectionResult> detections_;
    std::vector<OCRResult> ocrResults_;
};

} // namespace Algorithm
} // namespace VisionForge
