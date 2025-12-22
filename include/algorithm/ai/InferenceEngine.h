/**
 * @file InferenceEngine.h
 * @brief 深度学习推理引擎抽象基类
 *
 * 提供统一的推理引擎接口，支持多种后端：
 * - OpenCV DNN
 * - ONNX Runtime
 * - TensorRT
 * - OpenVINO
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_INFERENCE_ENGINE_H
#define VISIONFORGE_INFERENCE_ENGINE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSize>
#include <QJsonObject>
#include <QElapsedTimer>
#include <opencv2/core.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// 枚举定义
// ============================================================

/**
 * @brief 推理引擎后端类型
 */
enum class InferenceBackend {
    OpenCVDNN,      ///< OpenCV DNN模块
    ONNXRuntime,    ///< ONNX Runtime
    TensorRT,       ///< NVIDIA TensorRT
    OpenVINO,       ///< Intel OpenVINO
    Auto            ///< 自动选择最佳后端
};

/**
 * @brief 执行设备
 */
enum class ExecutionDevice {
    CPU,            ///< CPU执行
    CUDA,           ///< NVIDIA CUDA GPU
    DirectML,       ///< Windows DirectML (AMD/Intel GPU)
    CoreML,         ///< Apple CoreML
    Auto            ///< 自动选择
};

/**
 * @brief 精度模式
 */
enum class PrecisionMode {
    FP32,           ///< 32位浮点（默认）
    FP16,           ///< 16位浮点（半精度）
    INT8,           ///< 8位整数量化
    Mixed           ///< 混合精度
};

/**
 * @brief 模型格式
 */
enum class ModelFormat {
    Unknown,
    ONNX,           ///< ONNX格式
    TensorFlow,     ///< TensorFlow SavedModel或.pb
    PyTorch,        ///< PyTorch .pt/.pth
    TensorRT,       ///< TensorRT序列化引擎
    OpenVINO,       ///< OpenVINO IR格式
    Caffe,          ///< Caffe模型
    Darknet         ///< Darknet YOLO
};

/**
 * @brief AI任务类型
 */
enum class AITaskType {
    Detection,              ///< 目标检测
    Classification,         ///< 图像分类
    InstanceSegmentation,   ///< 实例分割
    SemanticSegmentation,   ///< 语义分割
    PoseEstimation,         ///< 姿态估计
    OCR,                    ///< 文字识别
    AnomalyDetection,       ///< 异常检测
    FeatureExtraction,      ///< 特征提取
    Custom                  ///< 自定义任务
};

// ============================================================
// 数据结构
// ============================================================

/**
 * @brief 张量数据
 */
struct Tensor {
    std::string name;                   ///< 张量名称
    std::vector<int64_t> shape;         ///< 形状
    std::vector<float> data;            ///< 数据（float类型）
    int dataType = 0;                   ///< 数据类型（0=float32, 1=float16, 2=int8）

    size_t elementCount() const {
        if (shape.empty()) return 0;
        size_t count = 1;
        for (auto dim : shape) count *= dim;
        return count;
    }

    bool isEmpty() const { return data.empty(); }

    QJsonObject toJson() const;
    static Tensor fromJson(const QJsonObject& json);
};

/**
 * @brief 推理会话配置
 */
struct InferenceConfig {
    InferenceBackend backend = InferenceBackend::Auto;
    ExecutionDevice device = ExecutionDevice::Auto;
    PrecisionMode precision = PrecisionMode::FP32;

    int deviceId = 0;                   ///< GPU设备ID
    int numThreads = 4;                 ///< CPU线程数
    bool enableProfiling = false;       ///< 启用性能分析
    bool enableMemoryOptimization = true;  ///< 内存优化

    // 动态形状
    bool dynamicBatching = false;       ///< 动态批处理
    int maxBatchSize = 1;               ///< 最大批大小
    QSize inputSize = QSize(640, 640);  ///< 输入尺寸

    // 缓存
    QString cacheDir;                   ///< 缓存目录
    bool useCachedEngine = true;        ///< 使用缓存引擎

    QJsonObject toJson() const;
    static InferenceConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 模型信息
 */
struct ModelInfo {
    QString path;                       ///< 模型路径
    QString configPath;                 ///< 配置文件路径
    ModelFormat format = ModelFormat::Unknown;
    AITaskType taskType = AITaskType::Detection;

    QStringList inputNames;             ///< 输入层名称
    QStringList outputNames;            ///< 输出层名称
    std::vector<std::vector<int64_t>> inputShapes;   ///< 输入形状
    std::vector<std::vector<int64_t>> outputShapes;  ///< 输出形状

    QStringList classNames;             ///< 类别名称
    int numClasses = 0;                 ///< 类别数量

    QString name;                       ///< 模型名称
    QString version;                    ///< 版本号
    QString author;                     ///< 作者
    QString description;                ///< 描述

    QJsonObject toJson() const;
    static ModelInfo fromJson(const QJsonObject& json);
};

/**
 * @brief 推理统计信息
 */
struct InferenceStats {
    double preprocessTime = 0;          ///< 预处理时间(ms)
    double inferenceTime = 0;           ///< 推理时间(ms)
    double postprocessTime = 0;         ///< 后处理时间(ms)
    double totalTime = 0;               ///< 总时间(ms)

    int batchSize = 1;                  ///< 批大小
    int frameCount = 0;                 ///< 已处理帧数
    double fps = 0;                     ///< 实时FPS

    size_t memoryUsage = 0;             ///< 内存使用(字节)
    size_t gpuMemoryUsage = 0;          ///< GPU内存使用(字节)

    void reset() {
        preprocessTime = inferenceTime = postprocessTime = totalTime = 0;
        batchSize = 1;
        frameCount = 0;
        fps = 0;
    }

    void update(double preTime, double infTime, double postTime) {
        preprocessTime = preTime;
        inferenceTime = infTime;
        postprocessTime = postTime;
        totalTime = preTime + infTime + postTime;
        frameCount++;
        if (totalTime > 0) fps = 1000.0 / totalTime;
    }
};

// ============================================================
// 推理引擎基类
// ============================================================

/**
 * @brief 推理引擎抽象基类
 *
 * 所有推理后端的统一接口。
 */
class InferenceEngine : public QObject {
    Q_OBJECT

public:
    explicit InferenceEngine(QObject* parent = nullptr);
    virtual ~InferenceEngine();

    // ============ 生命周期 ============

    /**
     * @brief 初始化引擎
     */
    virtual bool initialize(const InferenceConfig& config) = 0;

    /**
     * @brief 加载模型
     */
    virtual bool loadModel(const QString& modelPath,
                           const QString& configPath = QString()) = 0;

    /**
     * @brief 卸载模型
     */
    virtual void unloadModel() = 0;

    /**
     * @brief 是否已初始化
     */
    virtual bool isInitialized() const = 0;

    /**
     * @brief 是否已加载模型
     */
    virtual bool isModelLoaded() const = 0;

    /**
     * @brief 释放资源
     */
    virtual void release() = 0;

    // ============ 推理 ============

    /**
     * @brief 执行推理（单输入单输出）
     */
    virtual bool infer(const cv::Mat& input, cv::Mat& output) = 0;

    /**
     * @brief 执行推理（单输入多输出）
     */
    virtual bool infer(const cv::Mat& input, std::vector<cv::Mat>& outputs) = 0;

    /**
     * @brief 执行推理（张量接口）
     */
    virtual bool infer(const std::vector<Tensor>& inputs,
                       std::vector<Tensor>& outputs) = 0;

    /**
     * @brief 批量推理
     */
    virtual bool inferBatch(const std::vector<cv::Mat>& inputs,
                            std::vector<std::vector<cv::Mat>>& outputs);

    // ============ 预处理 ============

    /**
     * @brief 预处理图像
     */
    virtual cv::Mat preprocess(const cv::Mat& image);

    /**
     * @brief 设置预处理参数
     */
    void setPreprocessParams(double scaleFactor, const cv::Scalar& mean,
                              bool swapRB = true, bool crop = false);

    // ============ 信息获取 ============

    /**
     * @brief 获取后端类型
     */
    virtual InferenceBackend backend() const = 0;

    /**
     * @brief 获取后端名称
     */
    virtual QString backendName() const = 0;

    /**
     * @brief 获取配置
     */
    const InferenceConfig& config() const { return m_config; }

    /**
     * @brief 获取模型信息
     */
    const ModelInfo& modelInfo() const { return m_modelInfo; }

    /**
     * @brief 获取统计信息
     */
    const InferenceStats& stats() const { return m_stats; }

    /**
     * @brief 获取最后错误信息
     */
    QString lastError() const { return m_lastError; }

    // ============ 设备管理 ============

    /**
     * @brief 获取可用设备列表
     */
    virtual QStringList availableDevices() const;

    /**
     * @brief 设置执行设备
     */
    virtual bool setDevice(ExecutionDevice device, int deviceId = 0);

    /**
     * @brief 预热推理（优化首次推理延迟）
     */
    virtual bool warmup(int iterations = 3);

signals:
    /**
     * @brief 模型加载完成
     */
    void modelLoaded(bool success);

    /**
     * @brief 推理完成
     */
    void inferenceCompleted(double timeMs);

    /**
     * @brief 错误发生
     */
    void errorOccurred(const QString& error);

    /**
     * @brief 进度更新
     */
    void progressUpdated(int percent);

protected:
    // 配置和状态
    InferenceConfig m_config;
    ModelInfo m_modelInfo;
    InferenceStats m_stats;
    QString m_lastError;

    // 预处理参数
    double m_scaleFactor = 1.0 / 255.0;
    cv::Scalar m_mean = cv::Scalar(0, 0, 0);
    bool m_swapRB = true;
    bool m_crop = false;

    // 计时器
    QElapsedTimer m_timer;

    // 辅助方法
    void setError(const QString& error);
    ModelFormat detectModelFormat(const QString& path) const;
};

// ============================================================
// 工厂方法
// ============================================================

/**
 * @brief 创建推理引擎
 */
std::unique_ptr<InferenceEngine> createInferenceEngine(InferenceBackend backend);

/**
 * @brief 检查后端是否可用
 */
bool isBackendAvailable(InferenceBackend backend);

/**
 * @brief 获取最佳可用后端
 */
InferenceBackend getBestAvailableBackend(bool preferGPU = true);

// ============================================================
// 工具函数
// ============================================================

/**
 * @brief 获取后端名称
 */
QString getBackendName(InferenceBackend backend);

/**
 * @brief 获取设备名称
 */
QString getDeviceName(ExecutionDevice device);

/**
 * @brief 获取精度模式名称
 */
QString getPrecisionName(PrecisionMode precision);

/**
 * @brief 获取模型格式名称
 */
QString getModelFormatName(ModelFormat format);

/**
 * @brief 获取任务类型名称
 */
QString getAITaskTypeName(AITaskType type);

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge

#endif // VISIONFORGE_INFERENCE_ENGINE_H
