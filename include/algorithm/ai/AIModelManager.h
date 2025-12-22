/**
 * @file AIModelManager.h
 * @brief AI模型管理器
 *
 * 功能：
 * - 模型注册与索引
 * - 版本管理
 * - 性能基准测试
 * - 模型缓存
 * - 模型转换
 *
 * @author VisionForge Pro Team
 * @version 1.0.0
 */

#pragma once

#include "InferenceEngine.h"
#include "TensorRTEngine.h"
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QDateTime>
#include <memory>
#include <map>
#include <vector>
#include <mutex>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// 模型格式
// ============================================================
enum class AIModelFormat {
    ONNX,           // ONNX格式
    TensorRT,       // TensorRT引擎
    OpenVINO,       // OpenVINO IR
    PyTorch,        // PyTorch TorchScript
    TensorFlow,     // TensorFlow SavedModel
    Caffe,          // Caffe模型
    Unknown
};

// ============================================================
// 模型任务类型
// ============================================================
enum class ModelTaskType {
    Classification,      // 分类
    Detection,          // 检测
    Segmentation,       // 分割
    OCR,                // 文字识别
    AnomalyDetection,   // 异常检测
    PoseEstimation,     // 姿态估计
    FeatureExtraction,  // 特征提取
    Custom              // 自定义
};

// ============================================================
// 模型状态
// ============================================================
enum class ModelStatus {
    Available,      // 可用
    Loading,        // 加载中
    Loaded,         // 已加载
    Error,          // 错误
    Deprecated      // 已废弃
};

// ============================================================
// 模型信息
// ============================================================
struct AIModelInfo {
    // 基本信息
    QString id;                 // 唯一标识符
    QString name;               // 显示名称
    QString description;        // 描述
    QString version;            // 版本号
    QString author;             // 作者

    // 文件信息
    QString path;               // 模型文件路径
    AIModelFormat format;       // 模型格式
    qint64 fileSize = 0;        // 文件大小 (bytes)
    QString hash;               // 文件哈希 (SHA256)

    // 任务信息
    ModelTaskType taskType;     // 任务类型
    QStringList classNames;     // 类别名称列表
    int numClasses = 0;         // 类别数量

    // 输入信息
    QSize inputSize;            // 输入尺寸
    int inputChannels = 3;      // 输入通道数
    QString inputFormat;        // 输入格式 ("NCHW" 或 "NHWC")

    // 性能信息
    float inferenceTimeMs = 0;  // 平均推理时间 (ms)
    float throughput = 0;       // 吞吐量 (fps)
    size_t memoryUsage = 0;     // 显存占用 (bytes)

    // 元数据
    QJsonObject metadata;       // 额外元数据
    QDateTime createdAt;        // 创建时间
    QDateTime lastUsedAt;       // 最后使用时间
    int usageCount = 0;         // 使用次数

    // 状态
    ModelStatus status = ModelStatus::Available;
    QString errorMessage;

    // 序列化
    QJsonObject toJson() const;
    static AIModelInfo fromJson(const QJsonObject& json);
};

// ============================================================
// 性能基准结果
// ============================================================
struct BenchmarkResult {
    QString modelId;
    QString deviceName;

    // 延迟统计
    float minLatencyMs = 0;
    float maxLatencyMs = 0;
    float avgLatencyMs = 0;
    float p50LatencyMs = 0;     // 50th percentile
    float p95LatencyMs = 0;     // 95th percentile
    float p99LatencyMs = 0;     // 99th percentile

    // 吞吐量
    float throughputFps = 0;

    // 资源使用
    size_t peakMemoryBytes = 0;
    float avgGpuUtilization = 0;

    // 测试参数
    int warmupIterations = 0;
    int testIterations = 0;
    int batchSize = 1;

    // 时间戳
    QDateTime timestamp;

    QJsonObject toJson() const;
    static BenchmarkResult fromJson(const QJsonObject& json);
};

// ============================================================
// 模型转换配置
// ============================================================
struct ModelConversionConfig {
    QString inputPath;
    QString outputPath;
    AIModelFormat inputFormat;
    AIModelFormat outputFormat;

    // 优化选项
    bool optimize = true;
    PrecisionMode precision = PrecisionMode::FP32;
    bool simplifyONNX = true;

    // 动态形状
    bool enableDynamicShape = false;
    std::vector<int> minInputShape;
    std::vector<int> maxInputShape;

    // INT8量化
    bool enableINT8 = false;
    QString calibrationDataPath;
    int calibrationBatchSize = 32;
};

// ============================================================
// AI模型管理器
// ============================================================
class AIModelManager : public QObject {
    Q_OBJECT

public:
    static AIModelManager& instance();

    // 初始化
    bool initialize(const QString& modelDirectory);
    void shutdown();

    // 模型注册
    bool registerModel(const AIModelInfo& info);
    bool unregisterModel(const QString& modelId);
    bool updateModel(const AIModelInfo& info);

    // 模型查询
    AIModelInfo getModel(const QString& modelId) const;
    std::vector<AIModelInfo> getAllModels() const;
    std::vector<AIModelInfo> getModelsByTask(ModelTaskType taskType) const;
    std::vector<AIModelInfo> getModelsByFormat(AIModelFormat format) const;
    bool hasModel(const QString& modelId) const;

    // 模型加载
    std::unique_ptr<InferenceEngine> loadModel(const QString& modelId,
                                                InferenceBackend backend = InferenceBackend::Auto);
    void unloadModel(const QString& modelId);
    bool isModelLoaded(const QString& modelId) const;

    // 性能基准测试
    BenchmarkResult benchmarkModel(const QString& modelId,
                                   int warmupIterations = 10,
                                   int testIterations = 100,
                                   int batchSize = 1);
    std::vector<BenchmarkResult> getBenchmarkHistory(const QString& modelId) const;

    // 模型转换
    bool convertModel(const ModelConversionConfig& config);
    bool optimizeONNX(const QString& inputPath, const QString& outputPath);
    bool exportToTensorRT(const QString& onnxPath, const QString& enginePath,
                          PrecisionMode precision = PrecisionMode::FP16);

    // 模型验证
    bool validateModel(const QString& modelPath);
    QString getModelHash(const QString& modelPath) const;

    // 模型缓存
    void enableCache(bool enable);
    void setCacheDirectory(const QString& path);
    void clearCache();
    size_t getCacheSize() const;

    // 配置
    QString getModelDirectory() const { return m_modelDirectory; }
    void setModelDirectory(const QString& path);

    // 持久化
    bool saveRegistry(const QString& path = QString());
    bool loadRegistry(const QString& path = QString());

signals:
    void modelRegistered(const QString& modelId);
    void modelUnregistered(const QString& modelId);
    void modelLoaded(const QString& modelId);
    void modelUnloaded(const QString& modelId);
    void benchmarkStarted(const QString& modelId);
    void benchmarkProgress(const QString& modelId, int percent);
    void benchmarkComplete(const QString& modelId, const BenchmarkResult& result);
    void conversionStarted(const QString& inputPath);
    void conversionProgress(int percent);
    void conversionComplete(const QString& outputPath);
    void errorOccurred(const QString& error);

private:
    AIModelManager();
    ~AIModelManager();
    AIModelManager(const AIModelManager&) = delete;
    AIModelManager& operator=(const AIModelManager&) = delete;

    // 内部方法
    QString generateModelId(const QString& name) const;
    AIModelFormat detectModelFormat(const QString& path) const;
    qint64 getFileSize(const QString& path) const;
    void scanModelDirectory();

private:
    QString m_modelDirectory;
    QString m_cacheDirectory;
    QString m_registryPath;

    std::map<QString, AIModelInfo> m_models;
    std::map<QString, std::unique_ptr<InferenceEngine>> m_loadedEngines;
    std::map<QString, std::vector<BenchmarkResult>> m_benchmarkHistory;

    bool m_cacheEnabled = true;
    bool m_initialized = false;

    mutable std::mutex m_mutex;
};

// ============================================================
// 模型转换器
// ============================================================
class ModelConverter : public QObject {
    Q_OBJECT

public:
    explicit ModelConverter(QObject* parent = nullptr);
    ~ModelConverter();

    // ONNX操作
    bool simplifyONNX(const QString& inputPath, const QString& outputPath);
    bool quantizeONNX(const QString& inputPath, const QString& outputPath,
                      PrecisionMode precision);

    // 格式转换
    bool onnxToTensorRT(const QString& onnxPath, const QString& enginePath,
                        const TensorRTConfig& config);
    bool pytorchToONNX(const QString& ptPath, const QString& onnxPath,
                       const std::vector<int>& inputShape);

    // 验证
    bool compareOutputs(const QString& model1Path, const QString& model2Path,
                        const cv::Mat& testInput, float tolerance = 1e-3f);

    // 错误
    QString getLastError() const { return m_lastError; }

signals:
    void progress(int percent, const QString& status);
    void completed(bool success);

private:
    QString m_lastError;
};

// ============================================================
// 模型仓库（远程模型管理）
// ============================================================
class ModelRepository : public QObject {
    Q_OBJECT

public:
    struct RemoteModel {
        QString id;
        QString name;
        QString version;
        QString downloadUrl;
        QString hash;
        qint64 size;
        ModelTaskType taskType;
        QJsonObject metadata;
    };

public:
    explicit ModelRepository(QObject* parent = nullptr);
    ~ModelRepository();

    // 仓库操作
    bool connectToRepository(const QString& url);
    void disconnect();

    // 浏览
    std::vector<RemoteModel> listModels();
    std::vector<RemoteModel> searchModels(const QString& query);
    RemoteModel getModelInfo(const QString& modelId);

    // 下载
    bool downloadModel(const QString& modelId, const QString& localPath);
    void cancelDownload();

    // 上传 (需要认证)
    bool uploadModel(const QString& localPath, const RemoteModel& info);

signals:
    void connected();
    void disconnected();
    void downloadProgress(const QString& modelId, int percent);
    void downloadComplete(const QString& modelId, const QString& localPath);
    void errorOccurred(const QString& error);

private:
    QString m_repositoryUrl;
    bool m_connected = false;
};

// ============================================================
// 辅助函数
// ============================================================
QString modelFormatToString(AIModelFormat format);
AIModelFormat stringToModelFormat(const QString& str);
QString modelTaskTypeToString(ModelTaskType taskType);
ModelTaskType stringToModelTaskType(const QString& str);

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
