/**
 * @file AnomalyDetector.h
 * @brief 工业级异常/缺陷检测模块
 *
 * 支持多种无监督异常检测方法：
 * - PatchCore: 基于特征库的方法
 * - FastFlow: 基于正则化流的方法
 * - STFPM: 学生-教师特征匹配
 * - PaDiM: 概率分布建模
 *
 * 特点：
 * - 仅需正常样本训练
 * - 像素级异常定位
 * - 异常分数热力图
 *
 * @author VisionForge Pro Team
 * @version 1.0.0
 */

#pragma once

#include "InferenceEngine.h"
#include <QObject>
#include <QImage>
#include <QRectF>
#include <opencv2/core.hpp>
#include <vector>
#include <memory>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// 异常检测方法
// ============================================================
enum class AnomalyMethod {
    PatchCore,      // 基于特征库 (快速推理)
    FastFlow,       // 基于正则化流 (高精度)
    STFPM,          // 学生-教师特征匹配
    PaDiM,          // 概率分布建模
    EfficientAD,    // 高效异常检测
    Custom          // 自定义模型
};

// ============================================================
// 特征提取器骨干网络
// ============================================================
enum class FeatureBackbone {
    ResNet18,
    ResNet50,
    WideResNet50,
    EfficientNet_B0,
    EfficientNet_B4,
    Custom
};

// ============================================================
// 异常区域
// ============================================================
struct AnomalyRegion {
    int id = -1;
    QRectF boundingBox;           // 边界框
    cv::Mat mask;                 // 区域掩码
    std::vector<QPointF> contour; // 轮廓点

    float anomalyScore = 0.0f;    // 异常分数
    float area = 0.0f;            // 面积
    QPointF centroid;             // 质心

    QString defectType;           // 缺陷类型（如果已分类）
};

// ============================================================
// 异常检测结果
// ============================================================
struct AnomalyResult {
    bool success = false;
    QString errorMessage;

    // 整体结果
    bool isAnomaly = false;       // 是否存在异常
    float anomalyScore = 0.0f;    // 整体异常分数 (0-1)

    // 像素级结果
    cv::Mat anomalyMap;           // 异常热力图 (CV_32FC1, 0-1)
    cv::Mat binaryMask;           // 二值异常掩码 (CV_8UC1)
    cv::Mat heatmapVisualization; // 彩色热力图可视化

    // 异常区域
    std::vector<AnomalyRegion> regions;
    int regionCount = 0;

    // 统计信息
    float minScore = 0.0f;
    float maxScore = 0.0f;
    float meanScore = 0.0f;
    float stdScore = 0.0f;

    // 性能统计
    float featureTime = 0.0f;     // 特征提取时间 (ms)
    float inferenceTime = 0.0f;   // 推理时间 (ms)
    float postprocessTime = 0.0f; // 后处理时间 (ms)
    float totalTime = 0.0f;

    // 辅助方法
    AnomalyRegion* getRegionById(int id);
    std::vector<AnomalyRegion*> getRegionsByScore(float minScore);
};

// ============================================================
// 异常检测配置
// ============================================================
struct AnomalyConfig {
    // 方法设置
    AnomalyMethod method = AnomalyMethod::PatchCore;
    FeatureBackbone backbone = FeatureBackbone::WideResNet50;

    // 模型路径
    QString modelPath;            // 主模型路径
    QString featureExtractorPath; // 特征提取器路径（某些方法需要）
    QString memoryBankPath;       // 特征库路径（PatchCore）

    // 输入设置
    int inputWidth = 224;
    int inputHeight = 224;
    bool centerCrop = true;

    // 检测阈值
    float scoreThreshold = 0.5f;  // 判断异常的阈值
    float pixelThreshold = 0.5f;  // 像素级异常阈值

    // 后处理
    bool smoothAnomalyMap = true; // 平滑异常图
    int smoothKernelSize = 21;    // 高斯平滑核大小
    float smoothSigma = 4.0f;     // 高斯平滑sigma
    int minRegionArea = 50;       // 最小异常区域面积
    bool extractRegions = true;   // 提取异常区域

    // 推理设置
    InferenceBackend backend = InferenceBackend::ONNXRuntime;
    ExecutionDevice device = ExecutionDevice::Auto;

    // 序列化
    QJsonObject toJson() const;
    static AnomalyConfig fromJson(const QJsonObject& json);
};

// ============================================================
// 特征库（PatchCore用）
// ============================================================
class FeatureMemoryBank {
public:
    FeatureMemoryBank();
    ~FeatureMemoryBank();

    // 构建特征库
    bool addSample(const cv::Mat& features);
    bool build(int coresetSize = -1);  // -1 = 自动选择
    void clear();

    // 保存/加载
    bool save(const QString& path);
    bool load(const QString& path);

    // 查询
    float queryNearest(const cv::Mat& features) const;
    void queryKNearest(const cv::Mat& features, int k,
                       std::vector<float>& distances,
                       std::vector<int>& indices) const;

    // 状态
    bool isBuilt() const { return m_isBuilt; }
    int size() const { return static_cast<int>(m_features.size()); }
    int featureDim() const { return m_featureDim; }

private:
    std::vector<cv::Mat> m_features;
    cv::Mat m_coresetFeatures;
    int m_featureDim = 0;
    bool m_isBuilt = false;

    // 核心集采样
    cv::Mat coresetSampling(const cv::Mat& features, int k);
};

// ============================================================
// 异常检测器
// ============================================================
class AnomalyDetector : public QObject {
    Q_OBJECT

public:
    explicit AnomalyDetector(QObject* parent = nullptr);
    ~AnomalyDetector();

    // 初始化
    bool initialize(const AnomalyConfig& config);
    bool loadModel(const QString& modelPath);
    void release();

    // 训练/建库（仅PatchCore需要）
    bool trainFromImages(const std::vector<cv::Mat>& normalImages);
    bool trainFromFolder(const QString& folderPath);
    bool saveMemoryBank(const QString& path);
    bool loadMemoryBank(const QString& path);

    // 检测
    AnomalyResult detect(const cv::Mat& image);
    AnomalyResult detect(const QImage& image);

    // 批量检测
    std::vector<AnomalyResult> detectBatch(const std::vector<cv::Mat>& images);

    // 可视化
    cv::Mat visualize(const cv::Mat& image, const AnomalyResult& result,
                      float alpha = 0.5f, bool showRegions = true);
    static cv::Mat applyHeatmapColormap(const cv::Mat& anomalyMap);

    // 配置
    void setConfig(const AnomalyConfig& config);
    AnomalyConfig getConfig() const { return m_config; }

    // 阈值调整
    void setScoreThreshold(float threshold);
    void setPixelThreshold(float threshold);

    // 状态
    bool isInitialized() const { return m_initialized; }
    bool isTrained() const { return m_isTrained; }
    QString getLastError() const { return m_lastError; }

signals:
    void progressChanged(int percent, const QString& status);
    void trainingComplete(int sampleCount);
    void detectionComplete(const AnomalyResult& result);
    void errorOccurred(const QString& error);

private:
    // 预处理
    cv::Mat preprocess(const cv::Mat& image);

    // 特征提取
    cv::Mat extractFeatures(const cv::Mat& image);

    // 各方法的后处理
    AnomalyResult postprocessPatchCore(const cv::Mat& features,
                                        const cv::Size& originalSize);
    AnomalyResult postprocessFastFlow(const std::vector<Tensor>& outputs,
                                       const cv::Size& originalSize);
    AnomalyResult postprocessSTFPM(const std::vector<Tensor>& studentOutputs,
                                    const std::vector<Tensor>& teacherOutputs,
                                    const cv::Size& originalSize);
    AnomalyResult postprocessPaDiM(const cv::Mat& features,
                                    const cv::Size& originalSize);

    // 异常区域提取
    std::vector<AnomalyRegion> extractAnomalyRegions(const cv::Mat& binaryMask,
                                                      const cv::Mat& anomalyMap);

    // 归一化
    cv::Mat normalizeAnomalyMap(const cv::Mat& anomalyMap);

private:
    AnomalyConfig m_config;

    // 推理引擎
    std::unique_ptr<InferenceEngine> m_featureExtractor;
    std::unique_ptr<InferenceEngine> m_mainModel;

    // 特征库 (PatchCore)
    std::unique_ptr<FeatureMemoryBank> m_memoryBank;

    // PaDiM参数
    cv::Mat m_padimMean;
    cv::Mat m_padimCov;

    bool m_initialized = false;
    bool m_isTrained = false;
    QString m_lastError;

    // 归一化参数
    float m_normalizationMin = 0.0f;
    float m_normalizationMax = 1.0f;
};

// ============================================================
// 多模型异常检测器（集成多种方法）
// ============================================================
class EnsembleAnomalyDetector : public QObject {
    Q_OBJECT

public:
    struct EnsembleConfig {
        std::vector<AnomalyConfig> detectorConfigs;
        std::vector<float> weights;  // 各检测器权重
        float ensembleThreshold = 0.5f;
        bool useVoting = false;  // true=投票, false=加权平均
    };

public:
    explicit EnsembleAnomalyDetector(QObject* parent = nullptr);
    ~EnsembleAnomalyDetector();

    bool initialize(const EnsembleConfig& config);
    void release();

    AnomalyResult detect(const cv::Mat& image);

    int getDetectorCount() const { return static_cast<int>(m_detectors.size()); }

private:
    std::vector<std::unique_ptr<AnomalyDetector>> m_detectors;
    std::vector<float> m_weights;
    float m_threshold = 0.5f;
    bool m_useVoting = false;
    bool m_initialized = false;
};

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
