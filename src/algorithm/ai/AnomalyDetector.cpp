/**
 * @file AnomalyDetector.cpp
 * @brief 工业级异常/缺陷检测模块实现
 */

#include "algorithm/ai/AnomalyDetector.h"
#include "algorithm/ai/ONNXRuntimeEngine.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <chrono>
#include <algorithm>
#include <random>
#include <cmath>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// AnomalyConfig 实现
// ============================================================
QJsonObject AnomalyConfig::toJson() const {
    QJsonObject json;
    json["method"] = static_cast<int>(method);
    json["backbone"] = static_cast<int>(backbone);
    json["modelPath"] = modelPath;
    json["featureExtractorPath"] = featureExtractorPath;
    json["memoryBankPath"] = memoryBankPath;
    json["inputWidth"] = inputWidth;
    json["inputHeight"] = inputHeight;
    json["centerCrop"] = centerCrop;
    json["scoreThreshold"] = static_cast<double>(scoreThreshold);
    json["pixelThreshold"] = static_cast<double>(pixelThreshold);
    json["smoothAnomalyMap"] = smoothAnomalyMap;
    json["smoothKernelSize"] = smoothKernelSize;
    json["smoothSigma"] = static_cast<double>(smoothSigma);
    json["minRegionArea"] = minRegionArea;
    json["extractRegions"] = extractRegions;
    json["backend"] = static_cast<int>(backend);
    json["device"] = static_cast<int>(device);
    return json;
}

AnomalyConfig AnomalyConfig::fromJson(const QJsonObject& json) {
    AnomalyConfig config;
    config.method = static_cast<AnomalyMethod>(json["method"].toInt());
    config.backbone = static_cast<FeatureBackbone>(json["backbone"].toInt());
    config.modelPath = json["modelPath"].toString();
    config.featureExtractorPath = json["featureExtractorPath"].toString();
    config.memoryBankPath = json["memoryBankPath"].toString();
    config.inputWidth = json["inputWidth"].toInt(224);
    config.inputHeight = json["inputHeight"].toInt(224);
    config.centerCrop = json["centerCrop"].toBool(true);
    config.scoreThreshold = static_cast<float>(json["scoreThreshold"].toDouble(0.5));
    config.pixelThreshold = static_cast<float>(json["pixelThreshold"].toDouble(0.5));
    config.smoothAnomalyMap = json["smoothAnomalyMap"].toBool(true);
    config.smoothKernelSize = json["smoothKernelSize"].toInt(21);
    config.smoothSigma = static_cast<float>(json["smoothSigma"].toDouble(4.0));
    config.minRegionArea = json["minRegionArea"].toInt(50);
    config.extractRegions = json["extractRegions"].toBool(true);
    config.backend = static_cast<InferenceBackend>(json["backend"].toInt());
    config.device = static_cast<ExecutionDevice>(json["device"].toInt());
    return config;
}

// ============================================================
// AnomalyResult 实现
// ============================================================
AnomalyRegion* AnomalyResult::getRegionById(int id) {
    for (auto& region : regions) {
        if (region.id == id) {
            return &region;
        }
    }
    return nullptr;
}

std::vector<AnomalyRegion*> AnomalyResult::getRegionsByScore(float minScore) {
    std::vector<AnomalyRegion*> result;
    for (auto& region : regions) {
        if (region.anomalyScore >= minScore) {
            result.push_back(&region);
        }
    }
    return result;
}

// ============================================================
// FeatureMemoryBank 实现
// ============================================================
FeatureMemoryBank::FeatureMemoryBank() {
}

FeatureMemoryBank::~FeatureMemoryBank() {
    clear();
}

bool FeatureMemoryBank::addSample(const cv::Mat& features) {
    if (features.empty()) {
        return false;
    }

    if (m_features.empty()) {
        m_featureDim = features.cols;
    } else if (features.cols != m_featureDim) {
        return false;
    }

    m_features.push_back(features.clone());
    m_isBuilt = false;
    return true;
}

cv::Mat FeatureMemoryBank::coresetSampling(const cv::Mat& features, int k) {
    if (features.rows <= k) {
        return features.clone();
    }

    // 使用贪婪核心集采样
    std::vector<int> selectedIndices;
    cv::Mat distances = cv::Mat::ones(features.rows, 1, CV_32FC1) *
                        std::numeric_limits<float>::max();

    // 随机选择第一个点
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, features.rows - 1);
    selectedIndices.push_back(dis(gen));

    for (int i = 1; i < k; ++i) {
        // 更新距离
        cv::Mat lastSelected = features.row(selectedIndices.back());
        for (int j = 0; j < features.rows; ++j) {
            cv::Mat diff = features.row(j) - lastSelected;
            float dist = static_cast<float>(cv::norm(diff));
            distances.at<float>(j) = std::min(distances.at<float>(j), dist);
        }

        // 选择最远的点
        double maxVal;
        cv::Point maxLoc;
        cv::minMaxLoc(distances, nullptr, &maxVal, nullptr, &maxLoc);
        selectedIndices.push_back(maxLoc.y);
    }

    // 构建核心集
    cv::Mat coreset(k, features.cols, CV_32FC1);
    for (int i = 0; i < k; ++i) {
        features.row(selectedIndices[i]).copyTo(coreset.row(i));
    }

    return coreset;
}

bool FeatureMemoryBank::build(int coresetSize) {
    if (m_features.empty()) {
        return false;
    }

    // 合并所有特征
    cv::Mat allFeatures;
    for (const auto& f : m_features) {
        if (allFeatures.empty()) {
            allFeatures = f.clone();
        } else {
            cv::vconcat(allFeatures, f, allFeatures);
        }
    }

    // 核心集采样
    if (coresetSize <= 0) {
        // 自动选择核心集大小 (约10%或最大10000)
        coresetSize = std::min(allFeatures.rows / 10, 10000);
        coresetSize = std::max(coresetSize, 100);
    }

    if (allFeatures.rows > coresetSize) {
        m_coresetFeatures = coresetSampling(allFeatures, coresetSize);
    } else {
        m_coresetFeatures = allFeatures;
    }

    m_isBuilt = true;
    qDebug() << "Memory bank built with" << m_coresetFeatures.rows << "features";
    return true;
}

void FeatureMemoryBank::clear() {
    m_features.clear();
    m_coresetFeatures.release();
    m_featureDim = 0;
    m_isBuilt = false;
}

bool FeatureMemoryBank::save(const QString& path) {
    if (!m_isBuilt || m_coresetFeatures.empty()) {
        return false;
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream << static_cast<qint32>(m_coresetFeatures.rows);
    stream << static_cast<qint32>(m_coresetFeatures.cols);

    // 写入特征数据
    for (int i = 0; i < m_coresetFeatures.rows; ++i) {
        for (int j = 0; j < m_coresetFeatures.cols; ++j) {
            stream << m_coresetFeatures.at<float>(i, j);
        }
    }

    file.close();
    return true;
}

bool FeatureMemoryBank::load(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);
    qint32 rows, cols;
    stream >> rows >> cols;

    m_coresetFeatures = cv::Mat(rows, cols, CV_32FC1);
    m_featureDim = cols;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float val;
            stream >> val;
            m_coresetFeatures.at<float>(i, j) = val;
        }
    }

    file.close();
    m_isBuilt = true;
    return true;
}

float FeatureMemoryBank::queryNearest(const cv::Mat& features) const {
    if (!m_isBuilt || m_coresetFeatures.empty()) {
        return 0.0f;
    }

    float minDist = std::numeric_limits<float>::max();

    for (int i = 0; i < m_coresetFeatures.rows; ++i) {
        cv::Mat diff = features - m_coresetFeatures.row(i);
        float dist = static_cast<float>(cv::norm(diff));
        minDist = std::min(minDist, dist);
    }

    return minDist;
}

void FeatureMemoryBank::queryKNearest(const cv::Mat& features, int k,
                                       std::vector<float>& distances,
                                       std::vector<int>& indices) const {
    if (!m_isBuilt || m_coresetFeatures.empty()) {
        return;
    }

    std::vector<std::pair<float, int>> distIdx;
    distIdx.reserve(m_coresetFeatures.rows);

    for (int i = 0; i < m_coresetFeatures.rows; ++i) {
        cv::Mat diff = features - m_coresetFeatures.row(i);
        float dist = static_cast<float>(cv::norm(diff));
        distIdx.push_back({dist, i});
    }

    std::partial_sort(distIdx.begin(), distIdx.begin() + std::min(k, static_cast<int>(distIdx.size())),
                     distIdx.end());

    distances.clear();
    indices.clear();
    for (int i = 0; i < std::min(k, static_cast<int>(distIdx.size())); ++i) {
        distances.push_back(distIdx[i].first);
        indices.push_back(distIdx[i].second);
    }
}

// ============================================================
// AnomalyDetector 实现
// ============================================================
AnomalyDetector::AnomalyDetector(QObject* parent)
    : QObject(parent)
{
    m_memoryBank = std::make_unique<FeatureMemoryBank>();
}

AnomalyDetector::~AnomalyDetector() {
    release();
}

bool AnomalyDetector::initialize(const AnomalyConfig& config) {
    m_config = config;
    m_lastError.clear();

    // 创建特征提取器
    if (!config.featureExtractorPath.isEmpty()) {
        m_featureExtractor = createInferenceEngine(config.backend);
        if (!m_featureExtractor) {
            m_lastError = "Failed to create feature extractor engine";
            emit errorOccurred(m_lastError);
            return false;
        }

        InferenceConfig engineConfig;
        engineConfig.device = config.device;

        if (!m_featureExtractor->initialize(engineConfig)) {
            m_lastError = m_featureExtractor->lastError();
            emit errorOccurred(m_lastError);
            return false;
        }

        if (!m_featureExtractor->loadModel(config.featureExtractorPath)) {
            m_lastError = m_featureExtractor->lastError();
            emit errorOccurred(m_lastError);
            return false;
        }
    }

    // 创建主模型（非PatchCore方法）
    if (config.method != AnomalyMethod::PatchCore && !config.modelPath.isEmpty()) {
        m_mainModel = createInferenceEngine(config.backend);
        if (!m_mainModel) {
            m_lastError = "Failed to create main model engine";
            emit errorOccurred(m_lastError);
            return false;
        }

        InferenceConfig engineConfig;
        engineConfig.device = config.device;

        if (!m_mainModel->initialize(engineConfig)) {
            m_lastError = m_mainModel->lastError();
            emit errorOccurred(m_lastError);
            return false;
        }

        if (!m_mainModel->loadModel(config.modelPath)) {
            m_lastError = m_mainModel->lastError();
            emit errorOccurred(m_lastError);
            return false;
        }
    }

    // 加载特征库（PatchCore）
    if (config.method == AnomalyMethod::PatchCore && !config.memoryBankPath.isEmpty()) {
        if (!loadMemoryBank(config.memoryBankPath)) {
            qWarning() << "Failed to load memory bank, training required";
        }
    }

    m_initialized = true;
    return true;
}

bool AnomalyDetector::loadModel(const QString& modelPath) {
    if (m_config.method == AnomalyMethod::PatchCore) {
        m_config.featureExtractorPath = modelPath;
        return true;
    }

    m_config.modelPath = modelPath;
    if (m_mainModel) {
        return m_mainModel->loadModel(modelPath);
    }
    return false;
}

void AnomalyDetector::release() {
    if (m_featureExtractor) {
        m_featureExtractor->release();
        m_featureExtractor.reset();
    }
    if (m_mainModel) {
        m_mainModel->release();
        m_mainModel.reset();
    }
    if (m_memoryBank) {
        m_memoryBank->clear();
    }
    m_initialized = false;
    m_isTrained = false;
}

cv::Mat AnomalyDetector::preprocess(const cv::Mat& image) {
    cv::Mat result;

    // 缩放
    cv::resize(image, result, cv::Size(m_config.inputWidth, m_config.inputHeight));

    // 中心裁剪（可选）
    if (m_config.centerCrop && result.cols > m_config.inputWidth) {
        int x = (result.cols - m_config.inputWidth) / 2;
        int y = (result.rows - m_config.inputHeight) / 2;
        result = result(cv::Rect(x, y, m_config.inputWidth, m_config.inputHeight)).clone();
    }

    // BGR -> RGB
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);

    // 归一化 (ImageNet标准)
    result.convertTo(result, CV_32FC3, 1.0 / 255.0);
    cv::Scalar mean(0.485, 0.456, 0.406);
    cv::Scalar std(0.229, 0.224, 0.225);
    result = (result - mean) / std;

    // HWC -> CHW
    cv::Mat channels[3];
    cv::split(result, channels);
    cv::Mat chw;
    cv::vconcat(std::vector<cv::Mat>{
        channels[0].reshape(1, 1),
        channels[1].reshape(1, 1),
        channels[2].reshape(1, 1)
    }, chw);

    return chw.reshape(1, {1, 3, m_config.inputHeight, m_config.inputWidth});
}

cv::Mat AnomalyDetector::extractFeatures(const cv::Mat& image) {
    if (!m_featureExtractor) {
        return cv::Mat();
    }

    cv::Mat preprocessed = preprocess(image);

    Tensor inputTensor;
    inputTensor.shape = {1, 3, m_config.inputHeight, m_config.inputWidth};
    inputTensor.data.assign(
        reinterpret_cast<float*>(preprocessed.data),
        reinterpret_cast<float*>(preprocessed.data) + preprocessed.total());

    std::vector<Tensor> outputs;
    bool inferSuccess = m_featureExtractor->infer({inputTensor}, outputs);

    if (!inferSuccess || outputs.empty()) {
        return cv::Mat();
    }

    // 转换为cv::Mat
    const Tensor& output = outputs[0];
    int featureDim = 1;
    for (int i = 1; i < static_cast<int>(output.shape.size()); ++i) {
        featureDim *= output.shape[i];
    }

    cv::Mat features(1, featureDim, CV_32FC1,
                    const_cast<float*>(output.data.data()));
    return features.clone();
}

bool AnomalyDetector::trainFromImages(const std::vector<cv::Mat>& normalImages) {
    if (!m_initialized) {
        m_lastError = "Detector not initialized";
        emit errorOccurred(m_lastError);
        return false;
    }

    if (m_config.method != AnomalyMethod::PatchCore) {
        m_lastError = "Training only supported for PatchCore method";
        emit errorOccurred(m_lastError);
        return false;
    }

    m_memoryBank->clear();

    for (size_t i = 0; i < normalImages.size(); ++i) {
        cv::Mat features = extractFeatures(normalImages[i]);
        if (features.empty()) {
            continue;
        }

        m_memoryBank->addSample(features);
        emit progressChanged(static_cast<int>((i + 1) * 100 / normalImages.size()),
                            QString("Extracting features: %1/%2").arg(i + 1).arg(normalImages.size()));
    }

    if (!m_memoryBank->build()) {
        m_lastError = "Failed to build memory bank";
        emit errorOccurred(m_lastError);
        return false;
    }

    m_isTrained = true;
    emit trainingComplete(static_cast<int>(normalImages.size()));
    return true;
}

bool AnomalyDetector::trainFromFolder(const QString& folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        m_lastError = "Folder does not exist: " + folderPath;
        emit errorOccurred(m_lastError);
        return false;
    }

    QStringList filters = {"*.jpg", "*.jpeg", "*.png", "*.bmp"};
    QStringList files = dir.entryList(filters, QDir::Files);

    std::vector<cv::Mat> images;
    for (const QString& file : files) {
        QString fullPath = dir.filePath(file);
        cv::Mat image = cv::imread(fullPath.toStdString());
        if (!image.empty()) {
            images.push_back(image);
        }
    }

    if (images.empty()) {
        m_lastError = "No valid images found in folder";
        emit errorOccurred(m_lastError);
        return false;
    }

    return trainFromImages(images);
}

bool AnomalyDetector::saveMemoryBank(const QString& path) {
    if (!m_memoryBank || !m_memoryBank->isBuilt()) {
        m_lastError = "Memory bank not built";
        return false;
    }

    return m_memoryBank->save(path);
}

bool AnomalyDetector::loadMemoryBank(const QString& path) {
    if (!m_memoryBank) {
        m_memoryBank = std::make_unique<FeatureMemoryBank>();
    }

    if (m_memoryBank->load(path)) {
        m_isTrained = true;
        return true;
    }
    return false;
}

AnomalyResult AnomalyDetector::detect(const cv::Mat& image) {
    AnomalyResult result;

    if (!m_initialized) {
        result.errorMessage = "Detector not initialized";
        emit errorOccurred(result.errorMessage);
        return result;
    }

    auto startTotal = std::chrono::high_resolution_clock::now();

    // 特征提取
    auto startFeature = std::chrono::high_resolution_clock::now();
    cv::Mat features;

    switch (m_config.method) {
        case AnomalyMethod::PatchCore:
        case AnomalyMethod::PaDiM: {
            features = extractFeatures(image);
            if (features.empty()) {
                result.errorMessage = "Feature extraction failed";
                emit errorOccurred(result.errorMessage);
                return result;
            }
            break;
        }
        default:
            break;
    }

    auto endFeature = std::chrono::high_resolution_clock::now();
    result.featureTime = std::chrono::duration<float, std::milli>(endFeature - startFeature).count();

    // 推理
    auto startInfer = std::chrono::high_resolution_clock::now();

    switch (m_config.method) {
        case AnomalyMethod::PatchCore:
            result = postprocessPatchCore(features, image.size());
            break;
        case AnomalyMethod::FastFlow: {
            if (m_mainModel) {
                cv::Mat preprocessed = preprocess(image);
                Tensor inputTensor;
                inputTensor.shape = {1, 3, m_config.inputHeight, m_config.inputWidth};
                inputTensor.data.assign(
                    reinterpret_cast<float*>(preprocessed.data),
                    reinterpret_cast<float*>(preprocessed.data) + preprocessed.total());
                std::vector<Tensor> outputs;
                if (m_mainModel->infer({inputTensor}, outputs)) {
                    result = postprocessFastFlow(outputs, image.size());
                }
            }
            break;
        }
        case AnomalyMethod::PaDiM:
            result = postprocessPaDiM(features, image.size());
            break;
        default:
            result.errorMessage = "Unsupported method";
            break;
    }

    auto endInfer = std::chrono::high_resolution_clock::now();
    result.inferenceTime = std::chrono::duration<float, std::milli>(endInfer - startInfer).count();

    // 后处理
    auto startPost = std::chrono::high_resolution_clock::now();

    // 平滑异常图
    if (m_config.smoothAnomalyMap && !result.anomalyMap.empty()) {
        cv::GaussianBlur(result.anomalyMap, result.anomalyMap,
                        cv::Size(m_config.smoothKernelSize, m_config.smoothKernelSize),
                        m_config.smoothSigma);
    }

    // 归一化
    if (!result.anomalyMap.empty()) {
        result.anomalyMap = normalizeAnomalyMap(result.anomalyMap);
    }

    // 生成二值掩码
    if (!result.anomalyMap.empty()) {
        cv::Mat scaledMap;
        result.anomalyMap.convertTo(scaledMap, CV_8UC1, 255);
        cv::threshold(scaledMap, result.binaryMask,
                     static_cast<int>(m_config.pixelThreshold * 255),
                     255, cv::THRESH_BINARY);
    }

    // 提取异常区域
    if (m_config.extractRegions && !result.binaryMask.empty()) {
        result.regions = extractAnomalyRegions(result.binaryMask, result.anomalyMap);
        result.regionCount = static_cast<int>(result.regions.size());
    }

    // 生成热力图可视化
    if (!result.anomalyMap.empty()) {
        result.heatmapVisualization = applyHeatmapColormap(result.anomalyMap);
    }

    // 计算统计信息
    if (!result.anomalyMap.empty()) {
        cv::Scalar mean, stddev;
        cv::meanStdDev(result.anomalyMap, mean, stddev);
        double minVal, maxVal;
        cv::minMaxLoc(result.anomalyMap, &minVal, &maxVal);

        result.minScore = static_cast<float>(minVal);
        result.maxScore = static_cast<float>(maxVal);
        result.meanScore = static_cast<float>(mean[0]);
        result.stdScore = static_cast<float>(stddev[0]);
    }

    // 判断是否异常
    result.isAnomaly = result.anomalyScore >= m_config.scoreThreshold;

    auto endPost = std::chrono::high_resolution_clock::now();
    result.postprocessTime = std::chrono::duration<float, std::milli>(endPost - startPost).count();

    auto endTotal = std::chrono::high_resolution_clock::now();
    result.totalTime = std::chrono::duration<float, std::milli>(endTotal - startTotal).count();

    result.success = true;
    emit detectionComplete(result);
    return result;
}

AnomalyResult AnomalyDetector::detect(const QImage& image) {
    cv::Mat mat(image.height(), image.width(), CV_8UC4,
               const_cast<uchar*>(image.bits()), image.bytesPerLine());
    cv::Mat bgr;
    cv::cvtColor(mat, bgr, cv::COLOR_RGBA2BGR);
    return detect(bgr);
}

std::vector<AnomalyResult> AnomalyDetector::detectBatch(const std::vector<cv::Mat>& images) {
    std::vector<AnomalyResult> results;
    results.reserve(images.size());

    for (size_t i = 0; i < images.size(); ++i) {
        results.push_back(detect(images[i]));
        emit progressChanged(static_cast<int>((i + 1) * 100 / images.size()),
                            QString("Detecting: %1/%2").arg(i + 1).arg(images.size()));
    }

    return results;
}

AnomalyResult AnomalyDetector::postprocessPatchCore(const cv::Mat& features,
                                                     const cv::Size& originalSize) {
    AnomalyResult result;

    if (!m_memoryBank || !m_memoryBank->isBuilt()) {
        result.errorMessage = "Memory bank not built";
        return result;
    }

    // 查询最近邻距离
    float distance = m_memoryBank->queryNearest(features);

    // 归一化分数
    result.anomalyScore = distance / (m_normalizationMax - m_normalizationMin + 1e-6f);
    result.anomalyScore = std::min(1.0f, std::max(0.0f, result.anomalyScore));

    // 对于PatchCore，需要更细粒度的特征图来生成像素级热力图
    // 这里使用简化实现：生成均匀的异常图
    result.anomalyMap = cv::Mat(originalSize, CV_32FC1, cv::Scalar(result.anomalyScore));

    return result;
}

AnomalyResult AnomalyDetector::postprocessFastFlow(const std::vector<Tensor>& outputs,
                                                    const cv::Size& originalSize) {
    AnomalyResult result;

    if (outputs.empty()) {
        result.errorMessage = "Empty FastFlow output";
        return result;
    }

    // FastFlow输出通常是对数似然图
    const Tensor& output = outputs[0];

    int outH = output.shape.size() > 2 ? output.shape[2] : 1;
    int outW = output.shape.size() > 3 ? output.shape[3] : 1;

    cv::Mat logLikelihood(outH, outW, CV_32FC1,
                         const_cast<float*>(output.data.data()));

    // 转换为异常分数 (负对数似然)
    cv::Mat anomalyMap = -logLikelihood;

    // 缩放到原始尺寸
    cv::resize(anomalyMap, result.anomalyMap, originalSize);

    // 计算整体分数
    result.anomalyScore = static_cast<float>(cv::mean(result.anomalyMap)[0]);

    return result;
}

AnomalyResult AnomalyDetector::postprocessSTFPM(
    const std::vector<Tensor>& studentOutputs,
    const std::vector<Tensor>& teacherOutputs,
    const cv::Size& originalSize) {
    AnomalyResult result;

    // STFPM: 计算学生和教师网络特征的差异
    if (studentOutputs.size() != teacherOutputs.size()) {
        result.errorMessage = "Student and teacher output count mismatch";
        return result;
    }

    // 简化实现
    result.errorMessage = "STFPM postprocessing not fully implemented";
    return result;
}

AnomalyResult AnomalyDetector::postprocessPaDiM(const cv::Mat& features,
                                                 const cv::Size& originalSize) {
    AnomalyResult result;

    // PaDiM: 使用马氏距离计算异常分数
    if (m_padimMean.empty() || m_padimCov.empty()) {
        result.errorMessage = "PaDiM parameters not set";
        return result;
    }

    // 计算马氏距离
    cv::Mat diff = features - m_padimMean;
    cv::Mat covInv;
    cv::invert(m_padimCov, covInv);
    cv::Mat mahalDist = diff * covInv * diff.t();

    result.anomalyScore = static_cast<float>(mahalDist.at<double>(0, 0));
    result.anomalyMap = cv::Mat(originalSize, CV_32FC1, cv::Scalar(result.anomalyScore));

    return result;
}

std::vector<AnomalyRegion> AnomalyDetector::extractAnomalyRegions(
    const cv::Mat& binaryMask,
    const cv::Mat& anomalyMap) {
    std::vector<AnomalyRegion> regions;

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryMask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int id = 0;
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area < m_config.minRegionArea) {
            continue;
        }

        AnomalyRegion region;
        region.id = id++;
        region.area = static_cast<float>(area);

        // 边界框
        cv::Rect rect = cv::boundingRect(contour);
        region.boundingBox = QRectF(rect.x, rect.y, rect.width, rect.height);

        // 掩码
        region.mask = cv::Mat::zeros(binaryMask.size(), CV_8UC1);
        cv::drawContours(region.mask, std::vector<std::vector<cv::Point>>{contour},
                        0, cv::Scalar(255), -1);

        // 轮廓点
        for (const auto& pt : contour) {
            region.contour.push_back(QPointF(pt.x, pt.y));
        }

        // 质心
        cv::Moments m = cv::moments(contour);
        if (m.m00 != 0) {
            region.centroid = QPointF(m.m10 / m.m00, m.m01 / m.m00);
        }

        // 计算区域内的平均异常分数
        if (!anomalyMap.empty()) {
            double mean = cv::mean(anomalyMap, region.mask)[0];
            region.anomalyScore = static_cast<float>(mean);
        }

        regions.push_back(region);
    }

    // 按分数排序
    std::sort(regions.begin(), regions.end(),
              [](const AnomalyRegion& a, const AnomalyRegion& b) {
                  return a.anomalyScore > b.anomalyScore;
              });

    return regions;
}

cv::Mat AnomalyDetector::normalizeAnomalyMap(const cv::Mat& anomalyMap) {
    cv::Mat normalized;
    double minVal, maxVal;
    cv::minMaxLoc(anomalyMap, &minVal, &maxVal);

    if (maxVal - minVal > 1e-6) {
        normalized = (anomalyMap - minVal) / (maxVal - minVal);
    } else {
        normalized = cv::Mat::zeros(anomalyMap.size(), CV_32FC1);
    }

    return normalized;
}

cv::Mat AnomalyDetector::visualize(const cv::Mat& image, const AnomalyResult& result,
                                    float alpha, bool showRegions) {
    cv::Mat output = image.clone();

    // 叠加热力图
    if (!result.heatmapVisualization.empty()) {
        cv::Mat heatmapResized;
        cv::resize(result.heatmapVisualization, heatmapResized, image.size());
        cv::addWeighted(output, 1.0 - alpha, heatmapResized, alpha, 0, output);
    }

    // 绘制异常区域
    if (showRegions) {
        for (const auto& region : result.regions) {
            // 绘制轮廓
            std::vector<cv::Point> cvContour;
            for (const auto& pt : region.contour) {
                cvContour.push_back(cv::Point(static_cast<int>(pt.x()),
                                             static_cast<int>(pt.y())));
            }
            std::vector<std::vector<cv::Point>> contours = {cvContour};
            cv::drawContours(output, contours, 0, cv::Scalar(0, 0, 255), 2);

            // 绘制标签
            QString label = QString("Score: %1").arg(region.anomalyScore, 0, 'f', 2);
            cv::Point labelPos(static_cast<int>(region.boundingBox.x()),
                              static_cast<int>(region.boundingBox.y()) - 5);
            cv::putText(output, label.toStdString(), labelPos,
                       cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
        }
    }

    // 绘制整体分数
    QString scoreText = QString("Anomaly Score: %1%2")
        .arg(result.anomalyScore, 0, 'f', 3)
        .arg(result.isAnomaly ? " [ANOMALY]" : " [NORMAL]");
    cv::putText(output, scoreText.toStdString(), cv::Point(10, 30),
               cv::FONT_HERSHEY_SIMPLEX, 0.7,
               result.isAnomaly ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0), 2);

    return output;
}

cv::Mat AnomalyDetector::applyHeatmapColormap(const cv::Mat& anomalyMap) {
    cv::Mat heatmap;
    cv::Mat scaled;

    anomalyMap.convertTo(scaled, CV_8UC1, 255);
    cv::applyColorMap(scaled, heatmap, cv::COLORMAP_JET);

    return heatmap;
}

void AnomalyDetector::setConfig(const AnomalyConfig& config) {
    m_config = config;
}

void AnomalyDetector::setScoreThreshold(float threshold) {
    m_config.scoreThreshold = threshold;
}

void AnomalyDetector::setPixelThreshold(float threshold) {
    m_config.pixelThreshold = threshold;
}

// ============================================================
// EnsembleAnomalyDetector 实现
// ============================================================
EnsembleAnomalyDetector::EnsembleAnomalyDetector(QObject* parent)
    : QObject(parent)
{
}

EnsembleAnomalyDetector::~EnsembleAnomalyDetector() {
    release();
}

bool EnsembleAnomalyDetector::initialize(const EnsembleConfig& config) {
    release();

    for (const auto& detConfig : config.detectorConfigs) {
        auto detector = std::make_unique<AnomalyDetector>();
        if (!detector->initialize(detConfig)) {
            return false;
        }
        m_detectors.push_back(std::move(detector));
    }

    if (config.weights.size() == m_detectors.size()) {
        m_weights = config.weights;
    } else {
        // 默认等权重
        m_weights.resize(m_detectors.size(), 1.0f / m_detectors.size());
    }

    m_threshold = config.ensembleThreshold;
    m_useVoting = config.useVoting;
    m_initialized = true;

    return true;
}

void EnsembleAnomalyDetector::release() {
    m_detectors.clear();
    m_weights.clear();
    m_initialized = false;
}

AnomalyResult EnsembleAnomalyDetector::detect(const cv::Mat& image) {
    AnomalyResult result;

    if (!m_initialized || m_detectors.empty()) {
        result.errorMessage = "Ensemble detector not initialized";
        return result;
    }

    std::vector<AnomalyResult> results;
    for (auto& detector : m_detectors) {
        results.push_back(detector->detect(image));
    }

    // 合并结果
    if (m_useVoting) {
        // 投票
        int anomalyVotes = 0;
        for (const auto& r : results) {
            if (r.isAnomaly) anomalyVotes++;
        }
        result.isAnomaly = anomalyVotes > static_cast<int>(results.size()) / 2;
    } else {
        // 加权平均
        float totalScore = 0;
        float totalWeight = 0;
        for (size_t i = 0; i < results.size(); ++i) {
            totalScore += results[i].anomalyScore * m_weights[i];
            totalWeight += m_weights[i];
        }
        result.anomalyScore = totalScore / totalWeight;
        result.isAnomaly = result.anomalyScore >= m_threshold;
    }

    // 合并异常图（加权平均）
    if (!results.empty() && !results[0].anomalyMap.empty()) {
        result.anomalyMap = cv::Mat::zeros(results[0].anomalyMap.size(), CV_32FC1);
        float totalWeight = 0;
        for (size_t i = 0; i < results.size(); ++i) {
            if (!results[i].anomalyMap.empty()) {
                cv::Mat resized;
                cv::resize(results[i].anomalyMap, resized, result.anomalyMap.size());
                result.anomalyMap += resized * m_weights[i];
                totalWeight += m_weights[i];
            }
        }
        result.anomalyMap /= totalWeight;
        result.heatmapVisualization = AnomalyDetector::applyHeatmapColormap(result.anomalyMap);
    }

    result.success = true;
    return result;
}

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
