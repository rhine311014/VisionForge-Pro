/**
 * @file TensorRTEngine.h
 * @brief TensorRT高性能推理引擎
 *
 * 基于NVIDIA TensorRT的GPU加速推理引擎
 * 特点：
 * - FP16/INT8精度支持
 * - 动态批处理
 * - 引擎序列化/反序列化
 * - CUDA流管理
 * - 内存池优化
 *
 * @author VisionForge Pro Team
 * @version 1.0.0
 */

#pragma once

#include "InferenceEngine.h"
#include <QString>
#include <vector>
#include <memory>
#include <mutex>

// TensorRT条件编译
#ifdef USE_TENSORRT
#include <NvInfer.h>
#include <NvOnnxParser.h>
#include <cuda_runtime_api.h>
#endif

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// TensorRT精度模式
// ============================================================
enum class TRTPrecision {
    FP32,       // 32位浮点 (最高精度)
    FP16,       // 16位浮点 (2x加速)
    INT8        // 8位整数 (4x加速，需要校准)
};

// ============================================================
// TensorRT配置
// ============================================================
struct TensorRTConfig {
    // 模型路径
    QString onnxModelPath;        // ONNX模型路径
    QString engineCachePath;      // 引擎缓存路径

    // 精度设置
    TRTPrecision precision = TRTPrecision::FP16;
    bool strictTypeConstraints = false;

    // 工作空间
    size_t maxWorkspaceSize = 1ULL << 30;  // 1GB

    // 动态形状支持
    bool enableDynamicShape = false;
    std::vector<int> minBatchSize = {1};
    std::vector<int> optBatchSize = {1};
    std::vector<int> maxBatchSize = {8};

    // 流和并行
    int numStreams = 1;
    bool enableCudaGraph = false;

    // INT8校准
    bool enableINT8Calibration = false;
    QString calibrationCachePath;
    int calibrationBatchSize = 8;

    // 性能调优
    bool enableProfilingVerbosity = false;
    int builderOptimizationLevel = 3;  // 0-5

    // 序列化
    QJsonObject toJson() const;
    static TensorRTConfig fromJson(const QJsonObject& json);
};

#ifdef USE_TENSORRT

// ============================================================
// TensorRT日志器
// ============================================================
class TRTLogger : public nvinfer1::ILogger {
public:
    enum class LogLevel {
        Verbose,
        Info,
        Warning,
        Error
    };

    void setLogLevel(LogLevel level) { m_level = level; }
    void log(Severity severity, const char* msg) noexcept override;

private:
    LogLevel m_level = LogLevel::Warning;
};

// ============================================================
// INT8校准器
// ============================================================
class INT8Calibrator : public nvinfer1::IInt8EntropyCalibrator2 {
public:
    INT8Calibrator(int batchSize, const std::vector<cv::Mat>& calibrationImages,
                   const QString& cacheFile, int inputH, int inputW);
    ~INT8Calibrator();

    int getBatchSize() const noexcept override { return m_batchSize; }
    bool getBatch(void* bindings[], const char* names[], int nbBindings) noexcept override;
    const void* readCalibrationCache(size_t& length) noexcept override;
    void writeCalibrationCache(const void* cache, size_t length) noexcept override;

private:
    int m_batchSize;
    int m_inputH;
    int m_inputW;
    int m_currentBatch = 0;
    std::vector<cv::Mat> m_calibrationImages;
    QString m_cacheFile;
    std::vector<char> m_calibrationCache;

    void* m_deviceInput = nullptr;
};

#endif  // USE_TENSORRT

// ============================================================
// TensorRT推理引擎
// ============================================================
class TensorRTEngine : public QObject {
    Q_OBJECT

public:
    explicit TensorRTEngine(QObject* parent = nullptr);
    ~TensorRTEngine();

    // 基本接口
    bool initialize(const InferenceConfig& config);
    bool loadModel(const QString& modelPath);
    bool infer(const std::vector<Tensor>& inputs, std::vector<Tensor>& outputs);
    void release();
    QString getLastError() const { return QString::fromStdString(m_lastError); }
    bool isInitialized() const { return m_initialized; }

    // TensorRT特定方法
    bool buildEngineFromONNX(const QString& onnxPath, const TensorRTConfig& config);
    bool loadSerializedEngine(const QString& enginePath);
    bool saveSerializedEngine(const QString& enginePath);

    // INT8校准
    bool calibrateINT8(const std::vector<cv::Mat>& calibrationImages,
                       const QString& cacheFile);

    // 性能分析
    struct LayerProfile {
        QString name;
        float timeMs;
        float percentageOfTotal;
    };
    std::vector<LayerProfile> getLayerProfiles() const;
    float getTotalInferenceTime() const { return m_lastInferenceTimeMs; }

    // GPU信息
    static int getGPUCount();
    static QString getGPUName(int deviceId = 0);
    static size_t getGPUMemory(int deviceId = 0);
    static bool isGPUAvailable();

    // 配置
    void setConfig(const TensorRTConfig& config);
    TensorRTConfig getConfig() const { return m_trtConfig; }

private:
#ifdef USE_TENSORRT
    // 引擎和上下文
    std::unique_ptr<nvinfer1::ICudaEngine> m_engine;
    std::unique_ptr<nvinfer1::IExecutionContext> m_context;
    std::unique_ptr<TRTLogger> m_logger;

    // CUDA资源
    std::vector<void*> m_deviceBuffers;
    std::vector<size_t> m_bufferSizes;
    cudaStream_t m_stream = nullptr;

    // 输入/输出信息
    struct BindingInfo {
        QString name;
        nvinfer1::Dims dims;
        nvinfer1::DataType dataType;
        bool isInput;
        size_t size;
        int bindingIndex;
    };
    std::vector<BindingInfo> m_bindings;

    // 辅助方法
    bool allocateBuffers();
    void freeBuffers();
    size_t getBindingSize(const BindingInfo& binding) const;
    nvinfer1::Dims toDims(const std::vector<int>& shape);
    std::vector<int> toShape(const nvinfer1::Dims& dims);

#endif  // USE_TENSORRT

    TensorRTConfig m_trtConfig;
    bool m_initialized = false;
    std::string m_lastError;
    float m_lastInferenceTimeMs = 0.0f;

    mutable std::mutex m_mutex;
};

// ============================================================
// TensorRT引擎构建器
// ============================================================
class TensorRTEngineBuilder {
public:
    TensorRTEngineBuilder();
    ~TensorRTEngineBuilder();

    // 设置
    TensorRTEngineBuilder& setONNXModel(const QString& path);
    TensorRTEngineBuilder& setPrecision(TRTPrecision precision);
    TensorRTEngineBuilder& setMaxWorkspaceSize(size_t bytes);
    TensorRTEngineBuilder& setDynamicShape(bool enable);
    TensorRTEngineBuilder& setBatchSizes(int min, int opt, int max);
    TensorRTEngineBuilder& setCalibrationImages(const std::vector<cv::Mat>& images);

    // 构建
    std::unique_ptr<TensorRTEngine> build();

    // 错误
    QString getLastError() const { return m_lastError; }

private:
    TensorRTConfig m_config;
    std::vector<cv::Mat> m_calibrationImages;
    QString m_lastError;
};

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
