/**
 * @file TensorRTEngine.cpp
 * @brief TensorRT高性能推理引擎实现
 */

#include "algorithm/ai/TensorRTEngine.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <chrono>
#include <fstream>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// TensorRTConfig 实现
// ============================================================
QJsonObject TensorRTConfig::toJson() const {
    QJsonObject json;
    json["onnxModelPath"] = onnxModelPath;
    json["engineCachePath"] = engineCachePath;
    json["precision"] = static_cast<int>(precision);
    json["strictTypeConstraints"] = strictTypeConstraints;
    json["maxWorkspaceSize"] = static_cast<qint64>(maxWorkspaceSize);
    json["enableDynamicShape"] = enableDynamicShape;
    json["numStreams"] = numStreams;
    json["enableCudaGraph"] = enableCudaGraph;
    json["enableINT8Calibration"] = enableINT8Calibration;
    json["calibrationCachePath"] = calibrationCachePath;
    json["calibrationBatchSize"] = calibrationBatchSize;
    json["enableProfilingVerbosity"] = enableProfilingVerbosity;
    json["builderOptimizationLevel"] = builderOptimizationLevel;

    QJsonArray minBatchArr, optBatchArr, maxBatchArr;
    for (int v : minBatchSize) minBatchArr.append(v);
    for (int v : optBatchSize) optBatchArr.append(v);
    for (int v : maxBatchSize) maxBatchArr.append(v);
    json["minBatchSize"] = minBatchArr;
    json["optBatchSize"] = optBatchArr;
    json["maxBatchSize"] = maxBatchArr;

    return json;
}

TensorRTConfig TensorRTConfig::fromJson(const QJsonObject& json) {
    TensorRTConfig config;
    config.onnxModelPath = json["onnxModelPath"].toString();
    config.engineCachePath = json["engineCachePath"].toString();
    config.precision = static_cast<TRTPrecision>(json["precision"].toInt());
    config.strictTypeConstraints = json["strictTypeConstraints"].toBool(false);
    config.maxWorkspaceSize = static_cast<size_t>(json["maxWorkspaceSize"].toDouble(1ULL << 30));
    config.enableDynamicShape = json["enableDynamicShape"].toBool(false);
    config.numStreams = json["numStreams"].toInt(1);
    config.enableCudaGraph = json["enableCudaGraph"].toBool(false);
    config.enableINT8Calibration = json["enableINT8Calibration"].toBool(false);
    config.calibrationCachePath = json["calibrationCachePath"].toString();
    config.calibrationBatchSize = json["calibrationBatchSize"].toInt(8);
    config.enableProfilingVerbosity = json["enableProfilingVerbosity"].toBool(false);
    config.builderOptimizationLevel = json["builderOptimizationLevel"].toInt(3);

    config.minBatchSize.clear();
    config.optBatchSize.clear();
    config.maxBatchSize.clear();

    for (const auto& v : json["minBatchSize"].toArray()) {
        config.minBatchSize.push_back(v.toInt());
    }
    for (const auto& v : json["optBatchSize"].toArray()) {
        config.optBatchSize.push_back(v.toInt());
    }
    for (const auto& v : json["maxBatchSize"].toArray()) {
        config.maxBatchSize.push_back(v.toInt());
    }

    return config;
}

#ifdef USE_TENSORRT

// ============================================================
// TRTLogger 实现
// ============================================================
void TRTLogger::log(Severity severity, const char* msg) noexcept {
    switch (severity) {
        case Severity::kINTERNAL_ERROR:
        case Severity::kERROR:
            if (m_level <= LogLevel::Error) {
                qCritical() << "[TensorRT ERROR]" << msg;
            }
            break;
        case Severity::kWARNING:
            if (m_level <= LogLevel::Warning) {
                qWarning() << "[TensorRT WARNING]" << msg;
            }
            break;
        case Severity::kINFO:
            if (m_level <= LogLevel::Info) {
                qInfo() << "[TensorRT INFO]" << msg;
            }
            break;
        case Severity::kVERBOSE:
            if (m_level <= LogLevel::Verbose) {
                qDebug() << "[TensorRT VERBOSE]" << msg;
            }
            break;
    }
}

// ============================================================
// INT8Calibrator 实现
// ============================================================
INT8Calibrator::INT8Calibrator(int batchSize,
                               const std::vector<cv::Mat>& calibrationImages,
                               const QString& cacheFile,
                               int inputH, int inputW)
    : m_batchSize(batchSize)
    , m_inputH(inputH)
    , m_inputW(inputW)
    , m_calibrationImages(calibrationImages)
    , m_cacheFile(cacheFile)
{
    size_t inputSize = batchSize * 3 * inputH * inputW * sizeof(float);
    cudaMalloc(&m_deviceInput, inputSize);
}

INT8Calibrator::~INT8Calibrator() {
    if (m_deviceInput) {
        cudaFree(m_deviceInput);
    }
}

bool INT8Calibrator::getBatch(void* bindings[], const char* names[], int nbBindings) noexcept {
    if (m_currentBatch * m_batchSize >= static_cast<int>(m_calibrationImages.size())) {
        return false;
    }

    std::vector<float> batchData;
    batchData.reserve(m_batchSize * 3 * m_inputH * m_inputW);

    for (int i = 0; i < m_batchSize; ++i) {
        int idx = m_currentBatch * m_batchSize + i;
        if (idx >= static_cast<int>(m_calibrationImages.size())) {
            idx = static_cast<int>(m_calibrationImages.size()) - 1;
        }

        cv::Mat image = m_calibrationImages[idx];
        cv::Mat resized, normalized;

        cv::resize(image, resized, cv::Size(m_inputW, m_inputH));
        resized.convertTo(normalized, CV_32FC3, 1.0 / 255.0);

        cv::Scalar mean(0.485, 0.456, 0.406);
        cv::Scalar std(0.229, 0.224, 0.225);
        normalized = (normalized - mean) / std;

        // HWC -> CHW
        for (int c = 0; c < 3; ++c) {
            for (int h = 0; h < m_inputH; ++h) {
                for (int w = 0; w < m_inputW; ++w) {
                    batchData.push_back(normalized.at<cv::Vec3f>(h, w)[c]);
                }
            }
        }
    }

    cudaMemcpy(m_deviceInput, batchData.data(),
               batchData.size() * sizeof(float), cudaMemcpyHostToDevice);
    bindings[0] = m_deviceInput;

    m_currentBatch++;
    return true;
}

const void* INT8Calibrator::readCalibrationCache(size_t& length) noexcept {
    m_calibrationCache.clear();

    QFile file(m_cacheFile);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        m_calibrationCache.assign(data.begin(), data.end());
        length = m_calibrationCache.size();
        return m_calibrationCache.data();
    }

    length = 0;
    return nullptr;
}

void INT8Calibrator::writeCalibrationCache(const void* cache, size_t length) noexcept {
    QFile file(m_cacheFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(static_cast<const char*>(cache), length);
    }
}

#endif  // USE_TENSORRT

// ============================================================
// TensorRTEngine 实现
// ============================================================
TensorRTEngine::TensorRTEngine(QObject* parent)
    : QObject(parent)
{
#ifdef USE_TENSORRT
    m_logger = std::make_unique<TRTLogger>();
#endif
}

TensorRTEngine::~TensorRTEngine() {
    release();
}

bool TensorRTEngine::initialize(const InferenceConfig& config) {
#ifdef USE_TENSORRT
    m_trtConfig.precision = (config.precision == PrecisionMode::FP16) ?
                            TRTPrecision::FP16 :
                            (config.precision == PrecisionMode::INT8) ?
                            TRTPrecision::INT8 : TRTPrecision::FP32;

    m_initialized = true;
    return true;
#else
    m_lastError = "TensorRT not available - compile with USE_TENSORRT";
    return false;
#endif
}

bool TensorRTEngine::loadModel(const QString& modelPath) {
#ifdef USE_TENSORRT
    QString path = modelPath;
    QFileInfo fileInfo(path);

    if (fileInfo.suffix().toLower() == "engine" ||
        fileInfo.suffix().toLower() == "trt") {
        return loadSerializedEngine(path);
    } else if (fileInfo.suffix().toLower() == "onnx") {
        // 检查是否有缓存的引擎
        QString cachePath = path + ".engine";
        if (QFile::exists(cachePath)) {
            qInfo() << "Loading cached TensorRT engine:" << cachePath;
            return loadSerializedEngine(cachePath);
        }

        // 构建新引擎
        if (buildEngineFromONNX(path, m_trtConfig)) {
            saveSerializedEngine(cachePath);
            return true;
        }
        return false;
    }

    m_lastError = "Unsupported model format for TensorRT";
    return false;
#else
    m_lastError = "TensorRT not available";
    return false;
#endif
}

#ifdef USE_TENSORRT

bool TensorRTEngine::buildEngineFromONNX(const QString& onnxPath,
                                          const TensorRTConfig& config) {
    std::lock_guard<std::mutex> lock(m_mutex);

    qInfo() << "Building TensorRT engine from ONNX:" << onnxPath;
    auto startTime = std::chrono::high_resolution_clock::now();

    // 创建构建器
    auto builder = std::unique_ptr<nvinfer1::IBuilder>(
        nvinfer1::createInferBuilder(*m_logger));
    if (!builder) {
        m_lastError = "Failed to create TensorRT builder";
        return false;
    }

    // 创建网络定义
    uint32_t flag = 1U << static_cast<uint32_t>(
        nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    auto network = std::unique_ptr<nvinfer1::INetworkDefinition>(
        builder->createNetworkV2(flag));
    if (!network) {
        m_lastError = "Failed to create network definition";
        return false;
    }

    // 创建ONNX解析器
    auto parser = std::unique_ptr<nvonnxparser::IParser>(
        nvonnxparser::createParser(*network, *m_logger));
    if (!parser) {
        m_lastError = "Failed to create ONNX parser";
        return false;
    }

    // 解析ONNX模型
    if (!parser->parseFromFile(onnxPath.toStdString().c_str(),
                               static_cast<int>(nvinfer1::ILogger::Severity::kWARNING))) {
        m_lastError = "Failed to parse ONNX model";
        return false;
    }

    // 创建构建配置
    auto builderConfig = std::unique_ptr<nvinfer1::IBuilderConfig>(
        builder->createBuilderConfig());
    if (!builderConfig) {
        m_lastError = "Failed to create builder config";
        return false;
    }

    // 设置工作空间大小
    builderConfig->setMemoryPoolLimit(nvinfer1::MemoryPoolType::kWORKSPACE,
                                      config.maxWorkspaceSize);

    // 设置精度
    if (config.precision == TRTPrecision::FP16 &&
        builder->platformHasFastFp16()) {
        builderConfig->setFlag(nvinfer1::BuilderFlag::kFP16);
        qInfo() << "Enabling FP16 precision";
    } else if (config.precision == TRTPrecision::INT8 &&
               builder->platformHasFastInt8()) {
        builderConfig->setFlag(nvinfer1::BuilderFlag::kINT8);
        qInfo() << "Enabling INT8 precision";
        // INT8需要校准器
    }

    // 设置优化级别
    builderConfig->setBuilderOptimizationLevel(config.builderOptimizationLevel);

    // 动态形状配置
    if (config.enableDynamicShape) {
        auto profile = builder->createOptimizationProfile();
        for (int i = 0; i < network->getNbInputs(); ++i) {
            auto input = network->getInput(i);
            auto dims = input->getDimensions();

            nvinfer1::Dims minDims = dims;
            nvinfer1::Dims optDims = dims;
            nvinfer1::Dims maxDims = dims;

            if (!config.minBatchSize.empty()) {
                minDims.d[0] = config.minBatchSize[0];
                optDims.d[0] = config.optBatchSize[0];
                maxDims.d[0] = config.maxBatchSize[0];
            }

            profile->setDimensions(input->getName(),
                                   nvinfer1::OptProfileSelector::kMIN, minDims);
            profile->setDimensions(input->getName(),
                                   nvinfer1::OptProfileSelector::kOPT, optDims);
            profile->setDimensions(input->getName(),
                                   nvinfer1::OptProfileSelector::kMAX, maxDims);
        }
        builderConfig->addOptimizationProfile(profile);
    }

    // 构建引擎
    auto serializedNetwork = std::unique_ptr<nvinfer1::IHostMemory>(
        builder->buildSerializedNetwork(*network, *builderConfig));
    if (!serializedNetwork) {
        m_lastError = "Failed to build serialized network";
        return false;
    }

    // 反序列化创建引擎
    auto runtime = std::unique_ptr<nvinfer1::IRuntime>(
        nvinfer1::createInferRuntime(*m_logger));
    m_engine.reset(runtime->deserializeCudaEngine(
        serializedNetwork->data(), serializedNetwork->size()));
    if (!m_engine) {
        m_lastError = "Failed to deserialize CUDA engine";
        return false;
    }

    // 创建执行上下文
    m_context.reset(m_engine->createExecutionContext());
    if (!m_context) {
        m_lastError = "Failed to create execution context";
        return false;
    }

    // 分配缓冲区
    if (!allocateBuffers()) {
        return false;
    }

    // 创建CUDA流
    cudaStreamCreate(&m_stream);

    auto endTime = std::chrono::high_resolution_clock::now();
    float buildTime = std::chrono::duration<float, std::milli>(
        endTime - startTime).count();
    qInfo() << "TensorRT engine built in" << buildTime << "ms";

    m_initialized = true;
    return true;
}

bool TensorRTEngine::loadSerializedEngine(const QString& enginePath) {
    std::lock_guard<std::mutex> lock(m_mutex);

    QFile file(enginePath);
    if (!file.open(QIODevice::ReadOnly)) {
        m_lastError = "Failed to open engine file: " + enginePath.toStdString();
        return false;
    }

    QByteArray engineData = file.readAll();
    file.close();

    auto runtime = std::unique_ptr<nvinfer1::IRuntime>(
        nvinfer1::createInferRuntime(*m_logger));
    m_engine.reset(runtime->deserializeCudaEngine(
        engineData.data(), engineData.size()));
    if (!m_engine) {
        m_lastError = "Failed to deserialize CUDA engine";
        return false;
    }

    m_context.reset(m_engine->createExecutionContext());
    if (!m_context) {
        m_lastError = "Failed to create execution context";
        return false;
    }

    if (!allocateBuffers()) {
        return false;
    }

    cudaStreamCreate(&m_stream);

    m_initialized = true;
    qInfo() << "Loaded TensorRT engine:" << enginePath;
    return true;
}

bool TensorRTEngine::saveSerializedEngine(const QString& enginePath) {
    if (!m_engine) {
        m_lastError = "No engine to save";
        return false;
    }

    auto serialized = std::unique_ptr<nvinfer1::IHostMemory>(
        m_engine->serialize());
    if (!serialized) {
        m_lastError = "Failed to serialize engine";
        return false;
    }

    QFile file(enginePath);
    if (!file.open(QIODevice::WriteOnly)) {
        m_lastError = "Failed to create engine file";
        return false;
    }

    file.write(static_cast<const char*>(serialized->data()), serialized->size());
    file.close();

    qInfo() << "Saved TensorRT engine:" << enginePath;
    return true;
}

bool TensorRTEngine::allocateBuffers() {
    freeBuffers();
    m_bindings.clear();

    int numBindings = m_engine->getNbIOTensors();

    for (int i = 0; i < numBindings; ++i) {
        const char* name = m_engine->getIOTensorName(i);
        nvinfer1::Dims dims = m_engine->getTensorShape(name);
        nvinfer1::DataType dtype = m_engine->getTensorDataType(name);
        bool isInput = m_engine->getTensorIOMode(name) == nvinfer1::TensorIOMode::kINPUT;

        BindingInfo binding;
        binding.name = QString::fromUtf8(name);
        binding.dims = dims;
        binding.dataType = dtype;
        binding.isInput = isInput;
        binding.bindingIndex = i;
        binding.size = getBindingSize(binding);

        m_bindings.push_back(binding);

        void* buffer = nullptr;
        cudaMalloc(&buffer, binding.size);
        m_deviceBuffers.push_back(buffer);
        m_bufferSizes.push_back(binding.size);

        m_context->setTensorAddress(name, buffer);
    }

    return true;
}

void TensorRTEngine::freeBuffers() {
    for (void* buffer : m_deviceBuffers) {
        if (buffer) {
            cudaFree(buffer);
        }
    }
    m_deviceBuffers.clear();
    m_bufferSizes.clear();
}

size_t TensorRTEngine::getBindingSize(const BindingInfo& binding) const {
    size_t size = 1;
    for (int i = 0; i < binding.dims.nbDims; ++i) {
        size *= binding.dims.d[i];
    }

    switch (binding.dataType) {
        case nvinfer1::DataType::kFLOAT:
            size *= sizeof(float);
            break;
        case nvinfer1::DataType::kHALF:
            size *= sizeof(uint16_t);
            break;
        case nvinfer1::DataType::kINT8:
            size *= sizeof(int8_t);
            break;
        case nvinfer1::DataType::kINT32:
            size *= sizeof(int32_t);
            break;
        default:
            size *= sizeof(float);
            break;
    }

    return size;
}

nvinfer1::Dims TensorRTEngine::toDims(const std::vector<int>& shape) {
    nvinfer1::Dims dims;
    dims.nbDims = static_cast<int>(shape.size());
    for (size_t i = 0; i < shape.size(); ++i) {
        dims.d[i] = shape[i];
    }
    return dims;
}

std::vector<int> TensorRTEngine::toShape(const nvinfer1::Dims& dims) {
    std::vector<int> shape;
    for (int i = 0; i < dims.nbDims; ++i) {
        shape.push_back(dims.d[i]);
    }
    return shape;
}

#endif  // USE_TENSORRT

bool TensorRTEngine::infer(const std::vector<Tensor>& inputs, std::vector<Tensor>& outputs) {
    outputs.clear();

#ifdef USE_TENSORRT
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_initialized || !m_engine || !m_context) {
        m_lastError = "Engine not initialized";
        return false;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    // 复制输入数据到GPU
    int inputIdx = 0;
    for (const auto& binding : m_bindings) {
        if (binding.isInput && inputIdx < static_cast<int>(inputs.size())) {
            const Tensor& input = inputs[inputIdx];

            // 设置动态形状
            if (m_trtConfig.enableDynamicShape) {
                nvinfer1::Dims inputDims = toDims(input.shape);
                m_context->setInputShape(binding.name.toStdString().c_str(), inputDims);
            }

            // 复制数据
            cudaMemcpyAsync(m_deviceBuffers[binding.bindingIndex],
                           input.data.data(),
                           input.data.size() * sizeof(float),
                           cudaMemcpyHostToDevice,
                           m_stream);
            inputIdx++;
        }
    }

    // 执行推理
    if (!m_context->enqueueV3(m_stream)) {
        m_lastError = "Inference execution failed";
        return false;
    }

    // 复制输出数据回CPU
    for (const auto& binding : m_bindings) {
        if (!binding.isInput) {
            Tensor output;
            output.shape = toShape(m_context->getTensorShape(
                binding.name.toStdString().c_str()));

            size_t numElements = 1;
            for (int dim : output.shape) {
                numElements *= dim;
            }

            output.data.resize(numElements);
            cudaMemcpyAsync(output.data.data(),
                           m_deviceBuffers[binding.bindingIndex],
                           numElements * sizeof(float),
                           cudaMemcpyDeviceToHost,
                           m_stream);
            output.name = binding.name.toStdString();

            outputs.push_back(output);
        }
    }

    cudaStreamSynchronize(m_stream);

    auto endTime = std::chrono::high_resolution_clock::now();
    m_lastInferenceTimeMs = std::chrono::duration<float, std::milli>(
        endTime - startTime).count();

    return true;
#else
    m_lastError = "TensorRT not available";
    return false;
#endif
}

void TensorRTEngine::release() {
#ifdef USE_TENSORRT
    std::lock_guard<std::mutex> lock(m_mutex);

    freeBuffers();

    if (m_stream) {
        cudaStreamDestroy(m_stream);
        m_stream = nullptr;
    }

    m_context.reset();
    m_engine.reset();
#endif

    m_initialized = false;
}

bool TensorRTEngine::calibrateINT8(const std::vector<cv::Mat>& calibrationImages,
                                    const QString& cacheFile) {
#ifdef USE_TENSORRT
    if (calibrationImages.empty()) {
        m_lastError = "No calibration images provided";
        return false;
    }

    m_trtConfig.enableINT8Calibration = true;
    m_trtConfig.calibrationCachePath = cacheFile;

    qInfo() << "INT8 calibration with" << calibrationImages.size() << "images";
    return true;
#else
    m_lastError = "TensorRT not available";
    return false;
#endif
}

std::vector<TensorRTEngine::LayerProfile> TensorRTEngine::getLayerProfiles() const {
    std::vector<LayerProfile> profiles;
    // 需要启用profiling才能获取层级性能数据
    return profiles;
}

int TensorRTEngine::getGPUCount() {
#ifdef USE_TENSORRT
    int count = 0;
    cudaGetDeviceCount(&count);
    return count;
#else
    return 0;
#endif
}

QString TensorRTEngine::getGPUName(int deviceId) {
#ifdef USE_TENSORRT
    cudaDeviceProp prop;
    if (cudaGetDeviceProperties(&prop, deviceId) == cudaSuccess) {
        return QString::fromUtf8(prop.name);
    }
#endif
    return QString();
}

size_t TensorRTEngine::getGPUMemory(int deviceId) {
#ifdef USE_TENSORRT
    cudaDeviceProp prop;
    if (cudaGetDeviceProperties(&prop, deviceId) == cudaSuccess) {
        return prop.totalGlobalMem;
    }
#endif
    return 0;
}

bool TensorRTEngine::isGPUAvailable() {
    return getGPUCount() > 0;
}

void TensorRTEngine::setConfig(const TensorRTConfig& config) {
    m_trtConfig = config;
}

// ============================================================
// TensorRTEngineBuilder 实现
// ============================================================
TensorRTEngineBuilder::TensorRTEngineBuilder() {
}

TensorRTEngineBuilder::~TensorRTEngineBuilder() {
}

TensorRTEngineBuilder& TensorRTEngineBuilder::setONNXModel(const QString& path) {
    m_config.onnxModelPath = path;
    return *this;
}

TensorRTEngineBuilder& TensorRTEngineBuilder::setPrecision(TRTPrecision precision) {
    m_config.precision = precision;
    return *this;
}

TensorRTEngineBuilder& TensorRTEngineBuilder::setMaxWorkspaceSize(size_t bytes) {
    m_config.maxWorkspaceSize = bytes;
    return *this;
}

TensorRTEngineBuilder& TensorRTEngineBuilder::setDynamicShape(bool enable) {
    m_config.enableDynamicShape = enable;
    return *this;
}

TensorRTEngineBuilder& TensorRTEngineBuilder::setBatchSizes(int min, int opt, int max) {
    m_config.minBatchSize = {min};
    m_config.optBatchSize = {opt};
    m_config.maxBatchSize = {max};
    return *this;
}

TensorRTEngineBuilder& TensorRTEngineBuilder::setCalibrationImages(
    const std::vector<cv::Mat>& images) {
    m_calibrationImages = images;
    m_config.enableINT8Calibration = !images.empty();
    return *this;
}

std::unique_ptr<TensorRTEngine> TensorRTEngineBuilder::build() {
    auto engine = std::make_unique<TensorRTEngine>();
    engine->setConfig(m_config);

    InferenceConfig baseConfig;
    if (!engine->initialize(baseConfig)) {
        m_lastError = engine->getLastError();
        return nullptr;
    }

#ifdef USE_TENSORRT
    if (!m_config.onnxModelPath.isEmpty()) {
        if (!engine->buildEngineFromONNX(m_config.onnxModelPath, m_config)) {
            m_lastError = engine->getLastError();
            return nullptr;
        }
    }
#endif

    return engine;
}

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
