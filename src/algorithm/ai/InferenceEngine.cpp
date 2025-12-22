/**
 * @file InferenceEngine.cpp
 * @brief 推理引擎基类实现
 */

#include "algorithm/ai/InferenceEngine.h"
#include <QFileInfo>
#include <QJsonArray>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// Tensor
// ============================================================

QJsonObject Tensor::toJson() const {
    QJsonObject json;
    json["name"] = QString::fromStdString(name);

    QJsonArray shapeArray;
    for (auto dim : shape) {
        shapeArray.append(static_cast<qint64>(dim));
    }
    json["shape"] = shapeArray;
    json["dataType"] = dataType;
    json["elementCount"] = static_cast<qint64>(elementCount());

    return json;
}

Tensor Tensor::fromJson(const QJsonObject& json) {
    Tensor tensor;
    tensor.name = json["name"].toString().toStdString();
    tensor.dataType = json["dataType"].toInt();

    QJsonArray shapeArray = json["shape"].toArray();
    for (const auto& val : shapeArray) {
        tensor.shape.push_back(val.toInteger());
    }

    return tensor;
}

// ============================================================
// InferenceConfig
// ============================================================

QJsonObject InferenceConfig::toJson() const {
    QJsonObject json;
    json["backend"] = static_cast<int>(backend);
    json["device"] = static_cast<int>(device);
    json["precision"] = static_cast<int>(precision);
    json["deviceId"] = deviceId;
    json["numThreads"] = numThreads;
    json["enableProfiling"] = enableProfiling;
    json["enableMemoryOptimization"] = enableMemoryOptimization;
    json["dynamicBatching"] = dynamicBatching;
    json["maxBatchSize"] = maxBatchSize;
    json["inputWidth"] = inputSize.width();
    json["inputHeight"] = inputSize.height();
    json["cacheDir"] = cacheDir;
    json["useCachedEngine"] = useCachedEngine;
    return json;
}

InferenceConfig InferenceConfig::fromJson(const QJsonObject& json) {
    InferenceConfig config;
    config.backend = static_cast<InferenceBackend>(json["backend"].toInt());
    config.device = static_cast<ExecutionDevice>(json["device"].toInt());
    config.precision = static_cast<PrecisionMode>(json["precision"].toInt());
    config.deviceId = json["deviceId"].toInt();
    config.numThreads = json["numThreads"].toInt(4);
    config.enableProfiling = json["enableProfiling"].toBool();
    config.enableMemoryOptimization = json["enableMemoryOptimization"].toBool(true);
    config.dynamicBatching = json["dynamicBatching"].toBool();
    config.maxBatchSize = json["maxBatchSize"].toInt(1);
    config.inputSize = QSize(json["inputWidth"].toInt(640), json["inputHeight"].toInt(640));
    config.cacheDir = json["cacheDir"].toString();
    config.useCachedEngine = json["useCachedEngine"].toBool(true);
    return config;
}

// ============================================================
// ModelInfo
// ============================================================

QJsonObject ModelInfo::toJson() const {
    QJsonObject json;
    json["path"] = path;
    json["configPath"] = configPath;
    json["format"] = static_cast<int>(format);
    json["taskType"] = static_cast<int>(taskType);

    QJsonArray inputNamesArray;
    for (const auto& name : inputNames) inputNamesArray.append(name);
    json["inputNames"] = inputNamesArray;

    QJsonArray outputNamesArray;
    for (const auto& name : outputNames) outputNamesArray.append(name);
    json["outputNames"] = outputNamesArray;

    QJsonArray classNamesArray;
    for (const auto& name : classNames) classNamesArray.append(name);
    json["classNames"] = classNamesArray;

    json["numClasses"] = numClasses;
    json["name"] = name;
    json["version"] = version;
    json["author"] = author;
    json["description"] = description;

    return json;
}

ModelInfo ModelInfo::fromJson(const QJsonObject& json) {
    ModelInfo info;
    info.path = json["path"].toString();
    info.configPath = json["configPath"].toString();
    info.format = static_cast<ModelFormat>(json["format"].toInt());
    info.taskType = static_cast<AITaskType>(json["taskType"].toInt());

    for (const auto& val : json["inputNames"].toArray())
        info.inputNames.append(val.toString());

    for (const auto& val : json["outputNames"].toArray())
        info.outputNames.append(val.toString());

    for (const auto& val : json["classNames"].toArray())
        info.classNames.append(val.toString());

    info.numClasses = json["numClasses"].toInt();
    info.name = json["name"].toString();
    info.version = json["version"].toString();
    info.author = json["author"].toString();
    info.description = json["description"].toString();

    return info;
}

// ============================================================
// InferenceEngine
// ============================================================

InferenceEngine::InferenceEngine(QObject* parent)
    : QObject(parent)
{
}

InferenceEngine::~InferenceEngine()
{
}

bool InferenceEngine::inferBatch(const std::vector<cv::Mat>& inputs,
                                  std::vector<std::vector<cv::Mat>>& outputs) {
    outputs.clear();
    outputs.reserve(inputs.size());

    for (const auto& input : inputs) {
        std::vector<cv::Mat> output;
        if (!infer(input, output)) {
            return false;
        }
        outputs.push_back(std::move(output));
    }

    return true;
}

cv::Mat InferenceEngine::preprocess(const cv::Mat& image) {
    cv::Mat blob;
    cv::dnn::blobFromImage(
        image,
        blob,
        m_scaleFactor,
        cv::Size(m_config.inputSize.width(), m_config.inputSize.height()),
        m_mean,
        m_swapRB,
        m_crop
    );
    return blob;
}

void InferenceEngine::setPreprocessParams(double scaleFactor, const cv::Scalar& mean,
                                           bool swapRB, bool crop) {
    m_scaleFactor = scaleFactor;
    m_mean = mean;
    m_swapRB = swapRB;
    m_crop = crop;
}

QStringList InferenceEngine::availableDevices() const {
    QStringList devices;
    devices << "CPU";

#ifdef USE_CUDA
    // 检测CUDA设备 - 使用简化检测，避免依赖opencv_cudaarithm模块
    try {
        // 使用OpenCV DNN后端检测CUDA支持
        auto backends = cv::dnn::getAvailableBackends();
        for (const auto& backend : backends) {
            if (backend.first == cv::dnn::DNN_BACKEND_CUDA) {
                devices << "CUDA:0 (NVIDIA GPU)";
                break;
            }
        }
    } catch (...) {
        // 如果检测失败，不添加CUDA设备
    }
#endif

    return devices;
}

bool InferenceEngine::setDevice(ExecutionDevice device, int deviceId) {
    m_config.device = device;
    m_config.deviceId = deviceId;
    return true;
}

bool InferenceEngine::warmup(int iterations) {
    if (!isModelLoaded()) {
        setError("模型未加载，无法预热");
        return false;
    }

    // 创建随机输入
    cv::Mat dummyInput(m_config.inputSize.height(),
                       m_config.inputSize.width(),
                       CV_8UC3, cv::Scalar(128, 128, 128));
    cv::Mat output;

    for (int i = 0; i < iterations; ++i) {
        if (!infer(dummyInput, output)) {
            return false;
        }
    }

    return true;
}

void InferenceEngine::setError(const QString& error) {
    m_lastError = error;
    emit errorOccurred(error);
}

ModelFormat InferenceEngine::detectModelFormat(const QString& path) const {
    QFileInfo fileInfo(path);
    QString ext = fileInfo.suffix().toLower();

    if (ext == "onnx") return ModelFormat::ONNX;
    if (ext == "pb" || ext == "pbtxt") return ModelFormat::TensorFlow;
    if (ext == "pt" || ext == "pth") return ModelFormat::PyTorch;
    if (ext == "engine" || ext == "trt") return ModelFormat::TensorRT;
    if (ext == "xml" || ext == "bin") return ModelFormat::OpenVINO;
    if (ext == "caffemodel" || ext == "prototxt") return ModelFormat::Caffe;
    if (ext == "weights" || ext == "cfg") return ModelFormat::Darknet;

    return ModelFormat::Unknown;
}

// ============================================================
// 工厂方法实现
// ============================================================

// 前向声明（具体实现在各引擎文件中）
class OpenCVDNNEngine;
class ONNXRuntimeEngine;
class TensorRTEngine;

std::unique_ptr<InferenceEngine> createInferenceEngine(InferenceBackend backend) {
    switch (backend) {
    case InferenceBackend::OpenCVDNN:
        // OpenCV DNN引擎由AIDetectionTool直接使用
        return nullptr;

    case InferenceBackend::ONNXRuntime:
#ifdef USE_ONNX_RUNTIME
        // 返回ONNX Runtime引擎
        return std::make_unique<ONNXRuntimeEngine>();
#else
        return nullptr;
#endif

    case InferenceBackend::TensorRT:
#ifdef USE_TENSORRT
        // 返回TensorRT引擎
        return std::make_unique<TensorRTEngine>();
#else
        return nullptr;
#endif

    case InferenceBackend::Auto:
        // 自动选择最佳后端
        return createInferenceEngine(getBestAvailableBackend());

    default:
        return nullptr;
    }
}

bool isBackendAvailable(InferenceBackend backend) {
    switch (backend) {
    case InferenceBackend::OpenCVDNN:
        return true;  // OpenCV DNN总是可用

    case InferenceBackend::ONNXRuntime:
#ifdef USE_ONNX_RUNTIME
        return true;
#else
        return false;
#endif

    case InferenceBackend::TensorRT:
#ifdef USE_TENSORRT
        return true;
#else
        return false;
#endif

    case InferenceBackend::OpenVINO:
#ifdef USE_OPENVINO
        return true;
#else
        return false;
#endif

    default:
        return false;
    }
}

InferenceBackend getBestAvailableBackend(bool preferGPU) {
    if (preferGPU) {
        // 优先TensorRT（NVIDIA GPU最佳性能）
        if (isBackendAvailable(InferenceBackend::TensorRT)) {
            return InferenceBackend::TensorRT;
        }
        // 其次ONNX Runtime with CUDA
        if (isBackendAvailable(InferenceBackend::ONNXRuntime)) {
            return InferenceBackend::ONNXRuntime;
        }
    }

    // 默认OpenCV DNN
    return InferenceBackend::OpenCVDNN;
}

// ============================================================
// 工具函数
// ============================================================

QString getBackendName(InferenceBackend backend) {
    switch (backend) {
    case InferenceBackend::OpenCVDNN: return "OpenCV DNN";
    case InferenceBackend::ONNXRuntime: return "ONNX Runtime";
    case InferenceBackend::TensorRT: return "TensorRT";
    case InferenceBackend::OpenVINO: return "OpenVINO";
    case InferenceBackend::Auto: return "Auto";
    default: return "Unknown";
    }
}

QString getDeviceName(ExecutionDevice device) {
    switch (device) {
    case ExecutionDevice::CPU: return "CPU";
    case ExecutionDevice::CUDA: return "CUDA";
    case ExecutionDevice::DirectML: return "DirectML";
    case ExecutionDevice::CoreML: return "CoreML";
    case ExecutionDevice::Auto: return "Auto";
    default: return "Unknown";
    }
}

QString getPrecisionName(PrecisionMode precision) {
    switch (precision) {
    case PrecisionMode::FP32: return "FP32";
    case PrecisionMode::FP16: return "FP16";
    case PrecisionMode::INT8: return "INT8";
    case PrecisionMode::Mixed: return "Mixed";
    default: return "Unknown";
    }
}

QString getModelFormatName(ModelFormat format) {
    switch (format) {
    case ModelFormat::ONNX: return "ONNX";
    case ModelFormat::TensorFlow: return "TensorFlow";
    case ModelFormat::PyTorch: return "PyTorch";
    case ModelFormat::TensorRT: return "TensorRT";
    case ModelFormat::OpenVINO: return "OpenVINO";
    case ModelFormat::Caffe: return "Caffe";
    case ModelFormat::Darknet: return "Darknet";
    default: return "Unknown";
    }
}

QString getAITaskTypeName(AITaskType type) {
    switch (type) {
    case AITaskType::Detection: return "目标检测";
    case AITaskType::Classification: return "图像分类";
    case AITaskType::InstanceSegmentation: return "实例分割";
    case AITaskType::SemanticSegmentation: return "语义分割";
    case AITaskType::PoseEstimation: return "姿态估计";
    case AITaskType::OCR: return "文字识别";
    case AITaskType::AnomalyDetection: return "异常检测";
    case AITaskType::FeatureExtraction: return "特征提取";
    case AITaskType::Custom: return "自定义";
    default: return "未知";
    }
}

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
