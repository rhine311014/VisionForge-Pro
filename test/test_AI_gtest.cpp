/**
 * @file test_AI_gtest.cpp
 * @brief AI深度学习模块单元测试
 *
 * P6 AI Detection Module Unit Tests
 */

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// AI模块头文件
#include "algorithm/ai/InferenceEngine.h"
#include "algorithm/ai/TensorRTEngine.h"
#include "algorithm/ai/InstanceSegmentation.h"
#include "algorithm/ai/OCREngine.h"
#include "algorithm/ai/AnomalyDetector.h"
#include "algorithm/ai/AIModelManager.h"

using namespace VisionForge::Algorithm::AI;

// ============================================================
// 测试夹具
// ============================================================
class AITestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建测试图像
        testImage = cv::Mat(640, 640, CV_8UC3, cv::Scalar(128, 128, 128));

        // 绘制一些测试对象
        cv::rectangle(testImage, cv::Rect(100, 100, 200, 150), cv::Scalar(255, 0, 0), -1);
        cv::circle(testImage, cv::Point(450, 300), 80, cv::Scalar(0, 255, 0), -1);
        cv::putText(testImage, "TEST", cv::Point(200, 500),
                    cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3);
    }

    void TearDown() override {
        testImage.release();
    }

    cv::Mat testImage;
};

// ============================================================
// InferenceEngine 测试
// ============================================================
class InferenceEngineTest : public AITestFixture {};

TEST_F(InferenceEngineTest, BackendNameConversion) {
    EXPECT_EQ(getBackendName(InferenceBackend::OpenCVDNN), "OpenCV DNN");
    EXPECT_EQ(getBackendName(InferenceBackend::ONNXRuntime), "ONNX Runtime");
    EXPECT_EQ(getBackendName(InferenceBackend::TensorRT), "TensorRT");
    EXPECT_EQ(getBackendName(InferenceBackend::Auto), "Auto");
}

TEST_F(InferenceEngineTest, DeviceNameConversion) {
    EXPECT_EQ(getDeviceName(ExecutionDevice::CPU), "CPU");
    EXPECT_EQ(getDeviceName(ExecutionDevice::CUDA), "CUDA");
    EXPECT_EQ(getDeviceName(ExecutionDevice::DirectML), "DirectML");
    EXPECT_EQ(getDeviceName(ExecutionDevice::Auto), "Auto");
}

TEST_F(InferenceEngineTest, PrecisionModeNameConversion) {
    EXPECT_EQ(getPrecisionName(PrecisionMode::FP32), "FP32");
    EXPECT_EQ(getPrecisionName(PrecisionMode::FP16), "FP16");
    EXPECT_EQ(getPrecisionName(PrecisionMode::INT8), "INT8");
}

TEST_F(InferenceEngineTest, GetBestAvailableBackend) {
    InferenceBackend backend = getBestAvailableBackend();
    // 应该至少返回OpenCVDNN（总是可用）
    EXPECT_TRUE(backend == InferenceBackend::OpenCVDNN ||
                backend == InferenceBackend::ONNXRuntime ||
                backend == InferenceBackend::TensorRT);
}

TEST_F(InferenceEngineTest, CreateInferenceEngine) {
    // OpenCV DNN应该总是可用
    auto engine = createInferenceEngine(InferenceBackend::OpenCVDNN);
    EXPECT_NE(engine, nullptr);
}

TEST_F(InferenceEngineTest, TensorJsonSerialization) {
    Tensor tensor;
    tensor.name = "test_tensor";
    tensor.shape = {1, 3, 224, 224};
    tensor.data = {1.0f, 2.0f, 3.0f};

    QJsonObject json = tensor.toJson();
    EXPECT_EQ(json["name"].toString().toStdString(), "test_tensor");
    EXPECT_EQ(json["shape"].toArray().size(), 4);
}

TEST_F(InferenceEngineTest, InferenceConfigJsonSerialization) {
    InferenceConfig config;
    config.backend = InferenceBackend::ONNXRuntime;
    config.device = ExecutionDevice::CUDA;
    config.precision = PrecisionMode::FP16;
    config.numThreads = 8;
    config.inputSize = QSize(512, 512);

    QJsonObject json = config.toJson();
    InferenceConfig restored = InferenceConfig::fromJson(json);

    EXPECT_EQ(restored.backend, config.backend);
    EXPECT_EQ(restored.device, config.device);
    EXPECT_EQ(restored.precision, config.precision);
    EXPECT_EQ(restored.numThreads, config.numThreads);
    EXPECT_EQ(restored.inputSize, config.inputSize);
}

// ============================================================
// TensorRTEngine 测试
// ============================================================
class TensorRTEngineTest : public AITestFixture {};

TEST_F(TensorRTEngineTest, TensorRTConfigDefaults) {
    TensorRTConfig config;

    EXPECT_EQ(config.precision, TRTPrecision::FP32);
    EXPECT_FALSE(config.strictTypeConstraints);
    EXPECT_EQ(config.numStreams, 1);
    EXPECT_FALSE(config.enableCudaGraph);
}

TEST_F(TensorRTEngineTest, TensorRTConfigSerialization) {
    TensorRTConfig config;
    config.onnxModelPath = "/path/to/model.onnx";
    config.engineCachePath = "/path/to/cache";
    config.precision = TRTPrecision::FP16;
    config.enableDynamicShape = true;
    config.numStreams = 2;

    QJsonObject json = config.toJson();
    TensorRTConfig restored = TensorRTConfig::fromJson(json);

    EXPECT_EQ(restored.onnxModelPath, config.onnxModelPath);
    EXPECT_EQ(restored.precision, config.precision);
    EXPECT_EQ(restored.enableDynamicShape, config.enableDynamicShape);
    EXPECT_EQ(restored.numStreams, config.numStreams);
}

TEST_F(TensorRTEngineTest, CreateTensorRTEngine) {
    TensorRTEngine engine;

    // 没有CUDA/TensorRT时，初始化应该失败
    InferenceConfig config;
#ifndef USE_TENSORRT
    EXPECT_FALSE(engine.initialize(config));
#endif
}

// ============================================================
// InstanceSegmentation 测试
// ============================================================
class InstanceSegmentationTest : public AITestFixture {};

TEST_F(InstanceSegmentationTest, SegmentationConfigDefaults) {
    SegmentationConfig config;

    EXPECT_EQ(config.modelType, SegmentationModel::YOLOv8Seg);
    EXPECT_FLOAT_EQ(config.confidenceThreshold, 0.5f);
    EXPECT_FLOAT_EQ(config.nmsThreshold, 0.45f);
    EXPECT_EQ(config.inputWidth, 640);
    EXPECT_EQ(config.inputHeight, 640);
}

TEST_F(InstanceSegmentationTest, SegmentationConfigSerialization) {
    SegmentationConfig config;
    config.modelPath = "/path/to/model.onnx";
    config.modelType = SegmentationModel::MaskRCNN;
    config.confidenceThreshold = 0.7f;
    config.inputWidth = 512;
    config.inputHeight = 512;
    config.classNames = {"class1", "class2", "class3"};

    QJsonObject json = config.toJson();
    SegmentationConfig restored = SegmentationConfig::fromJson(json);

    EXPECT_EQ(restored.modelPath, config.modelPath);
    EXPECT_EQ(restored.modelType, config.modelType);
    EXPECT_FLOAT_EQ(restored.confidenceThreshold, config.confidenceThreshold);
    EXPECT_EQ(restored.inputWidth, config.inputWidth);
    EXPECT_EQ(restored.classNames.size(), config.classNames.size());
}

TEST_F(InstanceSegmentationTest, CreateInstanceSegmentor) {
    InstanceSegmentor segmentor;
    EXPECT_FALSE(segmentor.isInitialized());
}

TEST_F(InstanceSegmentationTest, ComputeGeometricProperties) {
    SegmentedInstance instance;
    instance.mask = cv::Mat(100, 100, CV_8UC1, cv::Scalar(0));
    cv::rectangle(instance.mask, cv::Rect(20, 20, 60, 40), cv::Scalar(255), -1);

    InstanceSegmentor::computeGeometricProperties(instance);

    EXPECT_GT(instance.area, 0.0f);
    EXPECT_GT(instance.perimeter, 0.0f);
}

// ============================================================
// OCREngine 测试
// ============================================================
class OCREngineTest : public AITestFixture {};

TEST_F(OCREngineTest, OCRConfigDefaults) {
    OCRConfig config;

    EXPECT_EQ(config.backend, OCRBackend::PaddleOCR);
    EXPECT_EQ(config.language, OCRLanguage::ChineseEnglish);
    EXPECT_FLOAT_EQ(config.detectionThreshold, 0.3f);
    EXPECT_EQ(config.maxTextLength, 100);
}

TEST_F(OCREngineTest, OCRConfigSerialization) {
    OCRConfig config;
    config.detectionModelPath = "/path/to/det.onnx";
    config.recognitionModelPath = "/path/to/rec.onnx";
    config.language = OCRLanguage::English;
    config.maxTextLength = 50;
    config.recognitionThreshold = 0.6f;

    QJsonObject json = config.toJson();
    OCRConfig restored = OCRConfig::fromJson(json);

    EXPECT_EQ(restored.detectionModelPath, config.detectionModelPath);
    EXPECT_EQ(restored.recognitionModelPath, config.recognitionModelPath);
    EXPECT_EQ(restored.language, config.language);
    EXPECT_EQ(restored.maxTextLength, config.maxTextLength);
}

TEST_F(OCREngineTest, CreateOCREngine) {
    OCREngine engine;
    EXPECT_FALSE(engine.isInitialized());
}

// ============================================================
// AnomalyDetector 测试
// ============================================================
class AnomalyDetectorTest : public AITestFixture {};

TEST_F(AnomalyDetectorTest, AnomalyConfigDefaults) {
    AnomalyConfig config;

    EXPECT_EQ(config.method, AnomalyMethod::PatchCore);
    EXPECT_FLOAT_EQ(config.scoreThreshold, 0.5f);
    EXPECT_EQ(config.inputWidth, 224);
    EXPECT_EQ(config.inputHeight, 224);
}

TEST_F(AnomalyDetectorTest, AnomalyConfigSerialization) {
    AnomalyConfig config;
    config.method = AnomalyMethod::FastFlow;
    config.modelPath = "/path/to/model.onnx";
    config.scoreThreshold = 0.7f;
    config.inputWidth = 256;
    config.inputHeight = 256;

    QJsonObject json = config.toJson();
    AnomalyConfig restored = AnomalyConfig::fromJson(json);

    EXPECT_EQ(restored.method, config.method);
    EXPECT_EQ(restored.modelPath, config.modelPath);
    EXPECT_FLOAT_EQ(restored.scoreThreshold, config.scoreThreshold);
    EXPECT_EQ(restored.inputWidth, config.inputWidth);
}

TEST_F(AnomalyDetectorTest, CreateAnomalyDetector) {
    AnomalyDetector detector;
    EXPECT_FALSE(detector.isInitialized());
}

TEST_F(AnomalyDetectorTest, FeatureMemoryBankBasic) {
    FeatureMemoryBank bank;
    EXPECT_EQ(bank.size(), 0);

    // 测试清空
    bank.clear();
    EXPECT_EQ(bank.size(), 0);
}

// ============================================================
// AIModelManager 测试
// ============================================================
class AIModelManagerTest : public AITestFixture {};

TEST_F(AIModelManagerTest, SingletonInstance) {
    AIModelManager& manager1 = AIModelManager::instance();
    AIModelManager& manager2 = AIModelManager::instance();

    EXPECT_EQ(&manager1, &manager2);
}

TEST_F(AIModelManagerTest, AIModelInfoDefaults) {
    AIModelInfo info;

    EXPECT_TRUE(info.id.isEmpty());
    EXPECT_TRUE(info.name.isEmpty());
    EXPECT_EQ(info.format, AIModelFormat::Unknown);
    EXPECT_EQ(info.status, ModelStatus::Available);
}

TEST_F(AIModelManagerTest, AIModelInfoSerialization) {
    AIModelInfo info;
    info.id = "model_001";
    info.name = "Test Model";
    info.path = "/path/to/model.onnx";
    info.format = AIModelFormat::ONNX;
    info.taskType = ModelTaskType::Detection;
    info.inputSize = QSize(640, 640);

    QJsonObject json = info.toJson();
    AIModelInfo restored = AIModelInfo::fromJson(json);

    EXPECT_EQ(restored.id, info.id);
    EXPECT_EQ(restored.name, info.name);
    EXPECT_EQ(restored.path, info.path);
    EXPECT_EQ(restored.format, info.format);
    EXPECT_EQ(restored.taskType, info.taskType);
    EXPECT_EQ(restored.inputSize, info.inputSize);
}

TEST_F(AIModelManagerTest, ModelConversionConfigDefaults) {
    ModelConversionConfig config;

    EXPECT_EQ(config.inputFormat, AIModelFormat::ONNX);
    EXPECT_EQ(config.outputFormat, AIModelFormat::TensorRT);
    EXPECT_EQ(config.precision, PrecisionMode::FP32);
}

// ============================================================
// 集成测试
// ============================================================
class AIIntegrationTest : public AITestFixture {};

TEST_F(AIIntegrationTest, BackendAvailability) {
    // 检查至少有一个后端可用
    bool hasOpenCV = isBackendAvailable(InferenceBackend::OpenCVDNN);
    bool hasONNX = isBackendAvailable(InferenceBackend::ONNXRuntime);
    bool hasTensorRT = isBackendAvailable(InferenceBackend::TensorRT);

    // OpenCV DNN应该总是可用
    EXPECT_TRUE(hasOpenCV);
}

TEST_F(AIIntegrationTest, EngineCreationByBackend) {
    auto opencvEngine = createInferenceEngine(InferenceBackend::OpenCVDNN);
    EXPECT_NE(opencvEngine, nullptr);

    auto onnxEngine = createInferenceEngine(InferenceBackend::ONNXRuntime);
    // 可能为空（取决于是否启用了ONNX Runtime）

    auto autoEngine = createInferenceEngine(InferenceBackend::Auto);
    EXPECT_NE(autoEngine, nullptr);
}

// ============================================================
// Main
// ============================================================
int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
