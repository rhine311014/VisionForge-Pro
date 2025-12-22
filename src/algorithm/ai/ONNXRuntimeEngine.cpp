/**
 * @file ONNXRuntimeEngine.cpp
 * @brief ONNX Runtime推理引擎实现
 */

#include "algorithm/ai/ONNXRuntimeEngine.h"
#include <QFileInfo>
#include <QDebug>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace Algorithm {
namespace AI {

ONNXRuntimeEngine::ONNXRuntimeEngine(QObject* parent)
    : InferenceEngine(parent)
{
}

ONNXRuntimeEngine::~ONNXRuntimeEngine() {
    release();
}

#ifdef USE_ONNX_RUNTIME

QString ONNXRuntimeEngine::getVersion() {
    return QString::fromStdString(Ort::GetVersionString());
}

QStringList ONNXRuntimeEngine::getAvailableProviders() {
    QStringList providers;
    auto available = Ort::GetAvailableProviders();
    for (const auto& p : available) {
        providers << QString::fromStdString(p);
    }
    return providers;
}

bool ONNXRuntimeEngine::initialize(const InferenceConfig& config) {
    std::lock_guard<std::mutex> lock(m_mutex);

    try {
        m_config = config;

        // 创建环境
        m_env = std::make_unique<Ort::Env>(
            ORT_LOGGING_LEVEL_WARNING, "VisionForgeORT");

        // 创建会话选项
        m_sessionOptions = std::make_unique<Ort::SessionOptions>();

        // 设置线程数
        m_sessionOptions->SetIntraOpNumThreads(config.numThreads);
        m_sessionOptions->SetInterOpNumThreads(m_interOpThreads);

        // 设置图优化级别
        m_sessionOptions->SetGraphOptimizationLevel(
            static_cast<GraphOptimizationLevel>(m_optLevel));

        // 内存模式
        if (m_memoryPatternEnabled) {
            m_sessionOptions->EnableMemPattern();
        } else {
            m_sessionOptions->DisableMemPattern();
        }

        // 设置执行提供者
        if (!setupExecutionProvider()) {
            setError("设置执行提供者失败");
            return false;
        }

        m_initialized = true;
        return true;

    } catch (const Ort::Exception& e) {
        setError(QString("ONNX Runtime初始化失败: %1").arg(e.what()));
        return false;
    }
}

bool ONNXRuntimeEngine::setupExecutionProvider() {
    try {
        if (m_config.device == ExecutionDevice::CUDA) {
            // CUDA执行提供者
            OrtCUDAProviderOptions cudaOptions;
            cudaOptions.device_id = m_config.deviceId;
            cudaOptions.cudnn_conv_algo_search = OrtCudnnConvAlgoSearchExhaustive;
            cudaOptions.gpu_mem_limit = 0;  // 不限制GPU内存
            cudaOptions.arena_extend_strategy = 0;
            cudaOptions.do_copy_in_default_stream = 1;

            m_sessionOptions->AppendExecutionProvider_CUDA(cudaOptions);
            m_executionProvider = "CUDA";
        }
        else if (m_config.device == ExecutionDevice::DirectML) {
#ifdef _WIN32
            // DirectML执行提供者（Windows）
            OrtSessionOptionsAppendExecutionProvider_DML(*m_sessionOptions, m_config.deviceId);
            m_executionProvider = "DirectML";
#endif
        }
        // CPU总是作为后备
        m_executionProvider = m_executionProvider.isEmpty() ? "CPU" : m_executionProvider;
        return true;

    } catch (const Ort::Exception& e) {
        qWarning() << "执行提供者设置失败，回退到CPU:" << e.what();
        m_executionProvider = "CPU";
        return true;
    }
}

bool ONNXRuntimeEngine::loadModel(const QString& modelPath, const QString& configPath) {
    Q_UNUSED(configPath);
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_initialized) {
        setError("引擎未初始化");
        return false;
    }

    QFileInfo fileInfo(modelPath);
    if (!fileInfo.exists()) {
        setError(QString("模型文件不存在: %1").arg(modelPath));
        return false;
    }

    try {
        // 创建会话
#ifdef _WIN32
        std::wstring wpath = modelPath.toStdWString();
        m_session = std::make_unique<Ort::Session>(*m_env, wpath.c_str(), *m_sessionOptions);
#else
        m_session = std::make_unique<Ort::Session>(*m_env, modelPath.toStdString().c_str(), *m_sessionOptions);
#endif

        // 提取输入输出信息
        extractIOInfo();

        // 更新模型信息
        m_modelInfo.path = modelPath;
        m_modelInfo.format = ModelFormat::ONNX;

        for (const auto& name : m_inputNames) {
            m_modelInfo.inputNames.append(QString::fromStdString(name));
        }
        for (const auto& name : m_outputNames) {
            m_modelInfo.outputNames.append(QString::fromStdString(name));
        }
        m_modelInfo.inputShapes = m_inputShapes;
        m_modelInfo.outputShapes = m_outputShapes;

        m_modelLoaded = true;
        emit modelLoaded(true);

        qDebug() << "ONNX模型加载成功:" << modelPath;
        qDebug() << "  执行提供者:" << m_executionProvider;
        qDebug() << "  输入数量:" << m_inputNames.size();
        qDebug() << "  输出数量:" << m_outputNames.size();

        return true;

    } catch (const Ort::Exception& e) {
        setError(QString("模型加载失败: %1").arg(e.what()));
        emit modelLoaded(false);
        return false;
    }
}

void ONNXRuntimeEngine::extractIOInfo() {
    // 清除旧信息
    m_inputNames.clear();
    m_outputNames.clear();
    m_inputShapes.clear();
    m_outputShapes.clear();
    m_inputNamesCStr.clear();
    m_outputNamesCStr.clear();

    // 获取输入信息
    size_t numInputs = m_session->GetInputCount();
    for (size_t i = 0; i < numInputs; ++i) {
        auto namePtr = m_session->GetInputNameAllocated(i, m_allocator);
        m_inputNames.push_back(namePtr.get());

        auto typeInfo = m_session->GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        m_inputShapes.push_back(tensorInfo.GetShape());
    }

    // 获取输出信息
    size_t numOutputs = m_session->GetOutputCount();
    for (size_t i = 0; i < numOutputs; ++i) {
        auto namePtr = m_session->GetOutputNameAllocated(i, m_allocator);
        m_outputNames.push_back(namePtr.get());

        auto typeInfo = m_session->GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        m_outputShapes.push_back(tensorInfo.GetShape());
    }

    // 准备C字符串指针
    for (const auto& name : m_inputNames) {
        m_inputNamesCStr.push_back(name.c_str());
    }
    for (const auto& name : m_outputNames) {
        m_outputNamesCStr.push_back(name.c_str());
    }
}

void ONNXRuntimeEngine::unloadModel() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_session.reset();
    m_modelLoaded = false;
    m_inputNames.clear();
    m_outputNames.clear();
    m_inputShapes.clear();
    m_outputShapes.clear();
}

bool ONNXRuntimeEngine::isInitialized() const {
    return m_initialized;
}

bool ONNXRuntimeEngine::isModelLoaded() const {
    return m_modelLoaded;
}

void ONNXRuntimeEngine::release() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_session.reset();
    m_sessionOptions.reset();
    m_env.reset();
    m_initialized = false;
    m_modelLoaded = false;
}

bool ONNXRuntimeEngine::infer(const cv::Mat& input, cv::Mat& output) {
    std::vector<cv::Mat> outputs;
    if (!infer(input, outputs)) {
        return false;
    }
    if (!outputs.empty()) {
        output = outputs[0];
    }
    return true;
}

bool ONNXRuntimeEngine::infer(const cv::Mat& input, std::vector<cv::Mat>& outputs) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_modelLoaded) {
        setError("模型未加载");
        return false;
    }

    m_timer.start();

    try {
        // 预处理
        cv::Mat blob = preprocess(input);
        double preprocessTime = m_timer.elapsed();

        // 准备输入张量
        auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        // 获取输入形状
        std::vector<int64_t> inputShape;
        if (!m_inputShapes.empty() && !m_inputShapes[0].empty()) {
            inputShape = m_inputShapes[0];
            // 更新动态维度
            if (inputShape[0] == -1) inputShape[0] = 1;  // batch
            if (inputShape.size() > 2) {
                if (inputShape[2] == -1) inputShape[2] = m_config.inputSize.height();
                if (inputShape[3] == -1) inputShape[3] = m_config.inputSize.width();
            }
        } else {
            // 默认NCHW格式
            inputShape = {1, 3, m_config.inputSize.height(), m_config.inputSize.width()};
        }

        // 创建输入张量
        size_t inputTensorSize = 1;
        for (auto dim : inputShape) inputTensorSize *= dim;

        std::vector<float> inputTensorValues(inputTensorSize);

        // 从blob复制数据
        if (blob.isContinuous()) {
            memcpy(inputTensorValues.data(), blob.ptr<float>(),
                   inputTensorSize * sizeof(float));
        } else {
            int idx = 0;
            for (int c = 0; c < blob.size[1]; ++c) {
                for (int h = 0; h < blob.size[2]; ++h) {
                    for (int w = 0; w < blob.size[3]; ++w) {
                        inputTensorValues[idx++] = blob.ptr<float>(0, c)[h * blob.size[3] + w];
                    }
                }
            }
        }

        Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
            memoryInfo, inputTensorValues.data(), inputTensorSize,
            inputShape.data(), inputShape.size());

        std::vector<Ort::Value> inputTensors;
        inputTensors.push_back(std::move(inputTensor));

        m_timer.restart();

        // 执行推理
        auto outputTensors = m_session->Run(
            Ort::RunOptions{nullptr},
            m_inputNamesCStr.data(),
            inputTensors.data(),
            inputTensors.size(),
            m_outputNamesCStr.data(),
            m_outputNamesCStr.size());

        double inferenceTime = m_timer.elapsed();
        m_timer.restart();

        // 转换输出
        outputs.clear();
        for (auto& tensor : outputTensors) {
            outputs.push_back(tensorToMat(tensor));
        }

        double postprocessTime = m_timer.elapsed();

        // 更新统计
        m_stats.update(preprocessTime, inferenceTime, postprocessTime);
        emit inferenceCompleted(m_stats.totalTime);

        return true;

    } catch (const Ort::Exception& e) {
        setError(QString("推理失败: %1").arg(e.what()));
        return false;
    }
}

bool ONNXRuntimeEngine::infer(const std::vector<Tensor>& inputs,
                               std::vector<Tensor>& outputs) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_modelLoaded) {
        setError("模型未加载");
        return false;
    }

    try {
        auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        // 创建输入张量
        std::vector<Ort::Value> inputTensors;
        for (const auto& input : inputs) {
            std::vector<float> data = input.data;
            Ort::Value tensor = Ort::Value::CreateTensor<float>(
                memoryInfo, data.data(), data.size(),
                input.shape.data(), input.shape.size());
            inputTensors.push_back(std::move(tensor));
        }

        // 执行推理
        auto outputTensors = m_session->Run(
            Ort::RunOptions{nullptr},
            m_inputNamesCStr.data(),
            inputTensors.data(),
            inputTensors.size(),
            m_outputNamesCStr.data(),
            m_outputNamesCStr.size());

        // 转换输出
        outputs.clear();
        for (size_t i = 0; i < outputTensors.size(); ++i) {
            Tensor output;
            output.name = m_outputNames[i];

            auto typeInfo = outputTensors[i].GetTensorTypeAndShapeInfo();
            output.shape = typeInfo.GetShape();

            auto* data = outputTensors[i].GetTensorMutableData<float>();
            size_t count = typeInfo.GetElementCount();
            output.data.assign(data, data + count);

            outputs.push_back(std::move(output));
        }

        return true;

    } catch (const Ort::Exception& e) {
        setError(QString("推理失败: %1").arg(e.what()));
        return false;
    }
}

cv::Mat ONNXRuntimeEngine::tensorToMat(const Ort::Value& tensor) {
    auto typeInfo = tensor.GetTensorTypeAndShapeInfo();
    auto shape = typeInfo.GetShape();
    auto* data = tensor.GetTensorData<float>();

    // 根据形状创建Mat
    if (shape.size() == 2) {
        // [rows, cols]
        cv::Mat mat(static_cast<int>(shape[0]), static_cast<int>(shape[1]), CV_32F);
        memcpy(mat.data, data, shape[0] * shape[1] * sizeof(float));
        return mat;
    }
    else if (shape.size() == 3) {
        // [batch/channels, height, width] or [height, width, channels]
        int dims[] = {static_cast<int>(shape[0]),
                      static_cast<int>(shape[1]),
                      static_cast<int>(shape[2])};
        cv::Mat mat(3, dims, CV_32F);
        memcpy(mat.data, data, shape[0] * shape[1] * shape[2] * sizeof(float));
        return mat;
    }
    else if (shape.size() == 4) {
        // [batch, channels, height, width]
        int dims[] = {static_cast<int>(shape[0]),
                      static_cast<int>(shape[1]),
                      static_cast<int>(shape[2]),
                      static_cast<int>(shape[3])};
        cv::Mat mat(4, dims, CV_32F);
        memcpy(mat.data, data, shape[0] * shape[1] * shape[2] * shape[3] * sizeof(float));
        return mat;
    }

    return cv::Mat();
}

bool ONNXRuntimeEngine::setExecutionProvider(const QString& provider) {
    m_executionProvider = provider;
    return true;
}

void ONNXRuntimeEngine::setIntraOpNumThreads(int threads) {
    m_intraOpThreads = threads;
    if (m_sessionOptions) {
        m_sessionOptions->SetIntraOpNumThreads(threads);
    }
}

void ONNXRuntimeEngine::setInterOpNumThreads(int threads) {
    m_interOpThreads = threads;
    if (m_sessionOptions) {
        m_sessionOptions->SetInterOpNumThreads(threads);
    }
}

void ONNXRuntimeEngine::setMemoryPatternEnabled(bool enabled) {
    m_memoryPatternEnabled = enabled;
}

void ONNXRuntimeEngine::setGraphOptimizationLevel(OptimizationLevel level) {
    m_optLevel = level;
    if (m_sessionOptions) {
        m_sessionOptions->SetGraphOptimizationLevel(
            static_cast<GraphOptimizationLevel>(level));
    }
}

std::vector<ONNXRuntimeEngine::IOInfo> ONNXRuntimeEngine::getInputInfo() const {
    std::vector<IOInfo> info;
    for (size_t i = 0; i < m_inputNames.size(); ++i) {
        IOInfo io;
        io.name = QString::fromStdString(m_inputNames[i]);
        if (i < m_inputShapes.size()) {
            io.shape = m_inputShapes[i];
        }
        io.typeString = "float32";
        info.push_back(io);
    }
    return info;
}

std::vector<ONNXRuntimeEngine::IOInfo> ONNXRuntimeEngine::getOutputInfo() const {
    std::vector<IOInfo> info;
    for (size_t i = 0; i < m_outputNames.size(); ++i) {
        IOInfo io;
        io.name = QString::fromStdString(m_outputNames[i]);
        if (i < m_outputShapes.size()) {
            io.shape = m_outputShapes[i];
        }
        io.typeString = "float32";
        info.push_back(io);
    }
    return info;
}

bool ONNXRuntimeEngine::setInputShape(const QString& name, const std::vector<int64_t>& shape) {
    for (size_t i = 0; i < m_inputNames.size(); ++i) {
        if (QString::fromStdString(m_inputNames[i]) == name) {
            m_inputShapes[i] = shape;
            return true;
        }
    }
    return false;
}

#else
// 无ONNX Runtime时的存根实现

QString ONNXRuntimeEngine::getVersion() {
    return "Not Available";
}

QStringList ONNXRuntimeEngine::getAvailableProviders() {
    return QStringList();
}

bool ONNXRuntimeEngine::initialize(const InferenceConfig& config) {
    Q_UNUSED(config);
    setError("ONNX Runtime未编译，请启用USE_ONNX_RUNTIME选项");
    return false;
}

bool ONNXRuntimeEngine::loadModel(const QString& modelPath, const QString& configPath) {
    Q_UNUSED(modelPath);
    Q_UNUSED(configPath);
    setError("ONNX Runtime未编译");
    return false;
}

void ONNXRuntimeEngine::unloadModel() {}
bool ONNXRuntimeEngine::isInitialized() const { return false; }
bool ONNXRuntimeEngine::isModelLoaded() const { return false; }
void ONNXRuntimeEngine::release() {}

bool ONNXRuntimeEngine::infer(const cv::Mat& input, cv::Mat& output) {
    Q_UNUSED(input);
    Q_UNUSED(output);
    return false;
}

bool ONNXRuntimeEngine::infer(const cv::Mat& input, std::vector<cv::Mat>& outputs) {
    Q_UNUSED(input);
    Q_UNUSED(outputs);
    return false;
}

bool ONNXRuntimeEngine::infer(const std::vector<Tensor>& inputs, std::vector<Tensor>& outputs) {
    Q_UNUSED(inputs);
    Q_UNUSED(outputs);
    return false;
}

bool ONNXRuntimeEngine::setExecutionProvider(const QString& provider) {
    Q_UNUSED(provider);
    return false;
}

void ONNXRuntimeEngine::setIntraOpNumThreads(int threads) { Q_UNUSED(threads); }
void ONNXRuntimeEngine::setInterOpNumThreads(int threads) { Q_UNUSED(threads); }
void ONNXRuntimeEngine::setMemoryPatternEnabled(bool enabled) { Q_UNUSED(enabled); }
void ONNXRuntimeEngine::setGraphOptimizationLevel(OptimizationLevel level) { Q_UNUSED(level); }

std::vector<ONNXRuntimeEngine::IOInfo> ONNXRuntimeEngine::getInputInfo() const {
    return {};
}

std::vector<ONNXRuntimeEngine::IOInfo> ONNXRuntimeEngine::getOutputInfo() const {
    return {};
}

bool ONNXRuntimeEngine::setInputShape(const QString& name, const std::vector<int64_t>& shape) {
    Q_UNUSED(name);
    Q_UNUSED(shape);
    return false;
}

#endif // USE_ONNX_RUNTIME

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
