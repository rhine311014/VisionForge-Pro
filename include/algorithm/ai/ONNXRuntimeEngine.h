/**
 * @file ONNXRuntimeEngine.h
 * @brief ONNX Runtime推理引擎
 *
 * 提供基于ONNX Runtime的高性能推理：
 * - 支持CPU和CUDA执行提供者
 * - 动态批处理
 * - 内存优化
 * - 多线程推理
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_ONNXRUNTIME_ENGINE_H
#define VISIONFORGE_ONNXRUNTIME_ENGINE_H

#include "algorithm/ai/InferenceEngine.h"
#include <memory>
#include <mutex>

// 条件编译ONNX Runtime
#ifdef USE_ONNX_RUNTIME
#include <onnxruntime_cxx_api.h>
#endif

namespace VisionForge {
namespace Algorithm {
namespace AI {

/**
 * @brief ONNX Runtime推理引擎
 *
 * 使用ONNX Runtime进行高效模型推理。
 */
class ONNXRuntimeEngine : public InferenceEngine {
    Q_OBJECT

public:
    explicit ONNXRuntimeEngine(QObject* parent = nullptr);
    ~ONNXRuntimeEngine() override;

    // ============ 基类接口实现 ============

    bool initialize(const InferenceConfig& config) override;
    bool loadModel(const QString& modelPath,
                   const QString& configPath = QString()) override;
    void unloadModel() override;
    bool isInitialized() const override;
    bool isModelLoaded() const override;
    void release() override;

    bool infer(const cv::Mat& input, cv::Mat& output) override;
    bool infer(const cv::Mat& input, std::vector<cv::Mat>& outputs) override;
    bool infer(const std::vector<Tensor>& inputs,
               std::vector<Tensor>& outputs) override;

    InferenceBackend backend() const override { return InferenceBackend::ONNXRuntime; }
    QString backendName() const override { return "ONNX Runtime"; }

    // ============ ONNX Runtime特有功能 ============

    /**
     * @brief 获取ONNX Runtime版本
     */
    static QString getVersion();

    /**
     * @brief 获取可用执行提供者
     */
    static QStringList getAvailableProviders();

    /**
     * @brief 设置执行提供者
     */
    bool setExecutionProvider(const QString& provider);

    /**
     * @brief 设置CPU线程数
     */
    void setIntraOpNumThreads(int threads);
    void setInterOpNumThreads(int threads);

    /**
     * @brief 启用/禁用内存模式优化
     */
    void setMemoryPatternEnabled(bool enabled);

    /**
     * @brief 设置图优化级别
     */
    enum OptimizationLevel {
        ORT_DISABLE_ALL = 0,
        ORT_ENABLE_BASIC = 1,
        ORT_ENABLE_EXTENDED = 2,
        ORT_ENABLE_ALL = 99
    };
    void setGraphOptimizationLevel(OptimizationLevel level);

    /**
     * @brief 获取输入信息
     */
    struct IOInfo {
        QString name;
        std::vector<int64_t> shape;
        QString typeString;
    };
    std::vector<IOInfo> getInputInfo() const;
    std::vector<IOInfo> getOutputInfo() const;

    /**
     * @brief 动态设置输入形状
     */
    bool setInputShape(const QString& name, const std::vector<int64_t>& shape);

private:
#ifdef USE_ONNX_RUNTIME
    // ONNX Runtime组件
    std::unique_ptr<Ort::Env> m_env;
    std::unique_ptr<Ort::Session> m_session;
    std::unique_ptr<Ort::SessionOptions> m_sessionOptions;
    Ort::AllocatorWithDefaultOptions m_allocator;

    // 输入输出信息
    std::vector<std::string> m_inputNames;
    std::vector<std::string> m_outputNames;
    std::vector<std::vector<int64_t>> m_inputShapes;
    std::vector<std::vector<int64_t>> m_outputShapes;
    std::vector<const char*> m_inputNamesCStr;
    std::vector<const char*> m_outputNamesCStr;

    // 配置
    int m_intraOpThreads = 4;
    int m_interOpThreads = 2;
    OptimizationLevel m_optLevel = ORT_ENABLE_ALL;
    bool m_memoryPatternEnabled = true;
    QString m_executionProvider = "CPU";

    // 状态
    bool m_initialized = false;
    bool m_modelLoaded = false;

    // 线程安全
    mutable std::mutex m_mutex;

    // 辅助方法
    bool setupExecutionProvider();
    void extractIOInfo();
    cv::Mat tensorToMat(const Ort::Value& tensor);
    Ort::Value matToTensor(const cv::Mat& mat, Ort::MemoryInfo& memoryInfo);
#else
    bool m_initialized = false;
    bool m_modelLoaded = false;
#endif
};

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge

#endif // VISIONFORGE_ONNXRUNTIME_ENGINE_H
