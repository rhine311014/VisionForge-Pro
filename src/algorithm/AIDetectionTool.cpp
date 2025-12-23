/**
 * @file AIDetectionTool.cpp
 * @brief AI检测工具实现
 */

#include "algorithm/AIDetectionTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QJsonArray>
#include <algorithm>

namespace VisionForge {
namespace Algorithm {

AIDetectionTool::AIDetectionTool(QObject* parent)
    : VisionTool(parent)
    , modelLoaded_(false)
    , taskType_(ObjectDetection)
    , inferenceEngine_(OpenCVDNN)
    , confidenceThreshold_(0.5f)
    , nmsThreshold_(0.4f)
    , inputSize_(640, 640)
    , useGPU_(false)
{
    setDisplayName(toolName());

    // 默认类别名称（COCO数据集部分类别）
    classNames_ = {
        "person", "bicycle", "car", "motorcycle", "airplane",
        "bus", "train", "truck", "boat", "traffic light",
        "fire hydrant", "stop sign", "parking meter", "bench", "bird",
        "cat", "dog", "horse", "sheep", "cow"
    };
}

AIDetectionTool::~AIDetectionTool()
{
    unloadModel();
}

bool AIDetectionTool::loadModel(const QString& modelPath, const QString& configPath)
{
    try {
        QFileInfo fileInfo(modelPath);
        if (!fileInfo.exists()) {
            LOG_ERROR(QString("模型文件不存在: %1").arg(modelPath));
            return false;
        }

        QString ext = fileInfo.suffix().toLower();

        // 根据文件扩展名选择加载方式
        if (ext == "onnx") {
            net_ = cv::dnn::readNetFromONNX(modelPath.toStdString());
        }
        else if (ext == "pb" || ext == "pbtxt") {
            if (!configPath.isEmpty()) {
                net_ = cv::dnn::readNetFromTensorflow(modelPath.toStdString(),
                                                      configPath.toStdString());
            } else {
                net_ = cv::dnn::readNetFromTensorflow(modelPath.toStdString());
            }
        }
        else if (ext == "caffemodel") {
            if (!configPath.isEmpty()) {
                net_ = cv::dnn::readNetFromCaffe(configPath.toStdString(),
                                                 modelPath.toStdString());
            } else {
                LOG_ERROR("Caffe模型需要配置文件(.prototxt)");
                return false;
            }
        }
        else if (ext == "weights") {
            // Darknet YOLO
            if (!configPath.isEmpty()) {
                net_ = cv::dnn::readNetFromDarknet(configPath.toStdString(),
                                                   modelPath.toStdString());
            } else {
                LOG_ERROR("Darknet模型需要配置文件(.cfg)");
                return false;
            }
        }
        else if (ext == "hdvppmodel" || ext == "model" || ext == "bin") {
            // 海康深度学习模型 (.bin是海康训练后的模型格式)
            inferenceEngine_ = HikvisionDL;
            modelPath_ = modelPath;
            configPath_ = configPath;
            modelLoaded_ = true;
            LOG_INFO(QString("海康模型已设置: %1 (需要海康SDK支持)").arg(modelPath));
            return true;
        }
        else if (ext == "hdl" || ext == "hdev" || ext == "dltp") {
            // Halcon深度学习模型 (.dltp是Halcon DL训练后的模型格式)
            inferenceEngine_ = HalconDL;
            modelPath_ = modelPath;
            configPath_ = configPath;
            modelLoaded_ = true;
            LOG_INFO(QString("Halcon模型已设置: %1 (需要Halcon DL支持)").arg(modelPath));
            return true;
        }
        else {
            LOG_ERROR(QString("不支持的模型格式: %1\n支持的格式: .onnx, .pb, .caffemodel, .weights, .bin(海康), .dltp(Halcon)").arg(ext));
            return false;
        }

        if (net_.empty()) {
            LOG_ERROR("模型加载失败");
            return false;
        }

        // 设置后端
        if (useGPU_) {
            net_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            net_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
            LOG_INFO("使用CUDA后端");
        } else {
            net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }

        modelPath_ = modelPath;
        configPath_ = configPath;
        modelLoaded_ = true;

        LOG_INFO(QString("模型加载成功: %1").arg(modelPath));
        return true;

    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("模型加载异常: %1").arg(e.what()));
        modelLoaded_ = false;
        return false;
    }
}

void AIDetectionTool::unloadModel()
{
    net_ = cv::dnn::Net();
    modelLoaded_ = false;
    modelPath_.clear();
    configPath_.clear();
}

bool AIDetectionTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    if (!modelLoaded_) {
        output.success = false;
        output.errorMessage = "模型未加载";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    cv::Mat src = input->mat();
    detections_.clear();

    try {
        cv::Mat netOutput;

        switch (inferenceEngine_) {
        case OpenCVDNN:
            if (!inferWithOpenCVDNN(src, netOutput)) {
                output.success = false;
                output.errorMessage = "推理失败";
                return false;
            }
            break;

        case ONNXRuntime:
        case TensorRT:
            // 暂时回退到OpenCV DNN
            LOG_WARNING("ONNX Runtime/TensorRT暂未实现，使用OpenCV DNN");
            if (!inferWithOpenCVDNN(src, netOutput)) {
                output.success = false;
                output.errorMessage = "推理失败";
                return false;
            }
            break;

        case HikvisionDL:
            // 海康深度学习推理
            LOG_INFO("使用海康深度学习引擎进行推理");
            output.success = false;
            output.errorMessage = "海康深度学习推理需要MVS SDK支持，请确保已安装海康SDK并配置正确";
            // TODO: 实现海康深度学习推理
            // 1. 使用 MV_DL_CreateSession 创建推理会话
            // 2. 使用 MV_DL_Process 进行推理
            // 3. 使用 MV_DL_GetResult 获取结果
            return false;

        case HalconDL:
            // Halcon深度学习推理
            LOG_INFO("使用Halcon深度学习引擎进行推理");
            output.success = false;
            output.errorMessage = "Halcon深度学习推理需要Halcon DL许可证，请确保已配置正确";
            // TODO: 实现Halcon深度学习推理
            // 1. 使用 read_dl_model 读取模型
            // 2. 使用 apply_dl_model 进行推理
            // 3. 使用 get_dl_model_result 获取结果
            return false;
        }

        // 后处理
        switch (taskType_) {
        case ObjectDetection:
        case DefectDetection:
            postprocessDetection(netOutput, src.size());
            break;

        case Classification:
            postprocessClassification(netOutput);
            break;

        case Segmentation:
            // TODO: 实现语义分割后处理
            break;
        }

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("detectionCount", detectionCount());

        // 记录检测结果
        QJsonArray detectionArray;
        for (const auto& det : detections_) {
            QJsonObject detObj;
            detObj["classId"] = det.classId;
            detObj["className"] = det.className;
            detObj["confidence"] = static_cast<double>(det.confidence);
            detObj["x"] = det.boundingBox.x();
            detObj["y"] = det.boundingBox.y();
            detObj["width"] = det.boundingBox.width();
            detObj["height"] = det.boundingBox.height();
            detObj["centerX"] = det.center.x();
            detObj["centerY"] = det.center.y();
            detectionArray.append(detObj);
        }
        output.data["detections"] = detectionArray;

        // 绘制结果
        cv::Mat resultImage = drawResults(src);
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }

        setDebugImage(output.outputImage);
        setStatusText(QString("检测到 %1 个目标").arg(detectionCount()));

        LOG_DEBUG(QString("AI检测完成，检测到 %1 个目标，耗时: %2ms")
                 .arg(detectionCount())
                 .arg(output.executionTime));

        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("AI检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool AIDetectionTool::inferWithOpenCVDNN(const cv::Mat& input, cv::Mat& output)
{
    try {
        // 预处理
        cv::Mat blob = preprocess(input);

        // 设置输入
        net_.setInput(blob);

        // 前向推理
        std::vector<cv::Mat> outputs;
        net_.forward(outputs, net_.getUnconnectedOutLayersNames());

        if (!outputs.empty()) {
            output = outputs[0];
        }

        return true;

    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("OpenCV DNN推理失败: %1").arg(e.what()));
        return false;
    }
}

cv::Mat AIDetectionTool::preprocess(const cv::Mat& input)
{
    // 创建blob
    cv::Mat blob;
    double scaleFactor = 1.0 / 255.0;

    // YOLO类模型的预处理
    cv::dnn::blobFromImage(input, blob, scaleFactor,
                           cv::Size(inputSize_.width(), inputSize_.height()),
                           cv::Scalar(0, 0, 0), true, false);

    return blob;
}

void AIDetectionTool::postprocessDetection(const cv::Mat& output, const cv::Size& originalSize)
{
    detections_.clear();

    // 处理YOLO格式输出
    // 输出格式: [batch, num_detections, 85] 其中85 = 4(box) + 1(confidence) + 80(classes)
    // 或者 [batch, 85, num_detections] 需要转置

    cv::Mat detection = output;

    // 检查输出格式并转置如果需要
    if (detection.dims == 3) {
        // 取第一个batch
        detection = detection.reshape(1, detection.size[1]);
    }

    // 检查是否需要转置 (YOLOv5/v8格式)
    if (detection.cols > detection.rows && detection.rows < 100) {
        cv::transpose(detection, detection);
    }

    float xFactor = static_cast<float>(originalSize.width) / inputSize_.width();
    float yFactor = static_cast<float>(originalSize.height) / inputSize_.height();

    std::vector<DetectionResult> rawDetections;

    for (int i = 0; i < detection.rows; ++i) {
        const float* data = detection.ptr<float>(i);

        // 获取类别置信度
        float confidence = data[4];  // objectness

        if (confidence < confidenceThreshold_) continue;

        // 获取最大类别分数
        int classId = -1;
        float maxClassScore = 0;

        for (int j = 5; j < detection.cols && j - 5 < classNames_.size(); ++j) {
            float classScore = data[j];
            if (classScore > maxClassScore) {
                maxClassScore = classScore;
                classId = j - 5;
            }
        }

        float finalConfidence = confidence * maxClassScore;
        if (finalConfidence < confidenceThreshold_) continue;

        // 解析边界框 (center_x, center_y, width, height)
        float cx = data[0] * xFactor;
        float cy = data[1] * yFactor;
        float w = data[2] * xFactor;
        float h = data[3] * yFactor;

        float x = cx - w / 2;
        float y = cy - h / 2;

        QString className = (classId >= 0 && classId < classNames_.size())
                            ? classNames_[classId] : QString("class_%1").arg(classId);

        rawDetections.emplace_back(classId, className, finalConfidence,
                                   QRectF(x, y, w, h));
    }

    // 应用NMS
    applyNMS(rawDetections);
    detections_ = std::move(rawDetections);
}

void AIDetectionTool::postprocessClassification(const cv::Mat& output)
{
    detections_.clear();

    cv::Mat scores = output.reshape(1, 1);  // 展平为1行

    // 找到最大分数的类别
    cv::Point maxLoc;
    double maxScore;
    cv::minMaxLoc(scores, nullptr, &maxScore, nullptr, &maxLoc);

    int classId = maxLoc.x;
    QString className = (classId >= 0 && classId < classNames_.size())
                        ? classNames_[classId] : QString("class_%1").arg(classId);

    // 对于分类任务，边界框是整个图像
    detections_.emplace_back(classId, className, static_cast<float>(maxScore),
                            QRectF(0, 0, 1, 1));
}

void AIDetectionTool::applyNMS(std::vector<DetectionResult>& detections)
{
    if (detections.size() <= 1) return;

    // 按置信度排序
    std::sort(detections.begin(), detections.end(),
             [](const DetectionResult& a, const DetectionResult& b) {
                 return a.confidence > b.confidence;
             });

    std::vector<bool> suppressed(detections.size(), false);

    for (size_t i = 0; i < detections.size(); ++i) {
        if (suppressed[i]) continue;

        for (size_t j = i + 1; j < detections.size(); ++j) {
            if (suppressed[j]) continue;
            if (detections[i].classId != detections[j].classId) continue;

            // 计算IoU
            QRectF intersection = detections[i].boundingBox.intersected(detections[j].boundingBox);
            double intersectionArea = intersection.width() * intersection.height();
            double unionArea = detections[i].boundingBox.width() * detections[i].boundingBox.height() +
                              detections[j].boundingBox.width() * detections[j].boundingBox.height() -
                              intersectionArea;

            double iou = (unionArea > 0) ? intersectionArea / unionArea : 0;

            if (iou > nmsThreshold_) {
                suppressed[j] = true;
            }
        }
    }

    // 移除被抑制的检测
    auto it = std::remove_if(detections.begin(), detections.end(),
                            [&suppressed, &detections](const DetectionResult& det) {
                                size_t idx = &det - &detections[0];
                                return suppressed[idx];
                            });
    detections.erase(it, detections.end());
}

cv::Mat AIDetectionTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result = input.clone();
    if (result.channels() == 1) {
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }

    // 颜色列表
    std::vector<cv::Scalar> colors = {
        cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255),
        cv::Scalar(255, 255, 0), cv::Scalar(255, 0, 255), cv::Scalar(0, 255, 255),
        cv::Scalar(128, 0, 255), cv::Scalar(255, 128, 0), cv::Scalar(0, 128, 255)
    };

    for (const auto& det : detections_) {
        cv::Scalar color = colors[det.classId % colors.size()];

        // 绘制边界框
        cv::Rect rect(static_cast<int>(det.boundingBox.x()),
                     static_cast<int>(det.boundingBox.y()),
                     static_cast<int>(det.boundingBox.width()),
                     static_cast<int>(det.boundingBox.height()));
        cv::rectangle(result, rect, color, 2);

        // 绘制中心点
        cv::Point center(static_cast<int>(det.center.x()),
                        static_cast<int>(det.center.y()));
        cv::circle(result, center, 4, color, -1);

        // 绘制标签
        QString label = QString("%1: %.1f%").arg(det.className).arg(det.confidence * 100);
        int baseline;
        cv::Size textSize = cv::getTextSize(label.toStdString(),
                                           cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);

        cv::rectangle(result,
                     cv::Point(rect.x, rect.y - textSize.height - 5),
                     cv::Point(rect.x + textSize.width, rect.y),
                     color, -1);

        cv::putText(result, label.toStdString(),
                   cv::Point(rect.x, rect.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }

    return result;
}

QJsonObject AIDetectionTool::serializeParams() const
{
    QJsonObject json;
    json["modelPath"] = modelPath_;
    json["configPath"] = configPath_;
    json["taskType"] = static_cast<int>(taskType_);
    json["inferenceEngine"] = static_cast<int>(inferenceEngine_);
    json["confidenceThreshold"] = static_cast<double>(confidenceThreshold_);
    json["nmsThreshold"] = static_cast<double>(nmsThreshold_);
    json["inputWidth"] = inputSize_.width();
    json["inputHeight"] = inputSize_.height();
    json["useGPU"] = useGPU_;

    QJsonArray classArray;
    for (const auto& name : classNames_) {
        classArray.append(name);
    }
    json["classNames"] = classArray;

    return json;
}

void AIDetectionTool::deserializeParams(const QJsonObject& json)
{
    modelPath_ = json.value("modelPath").toString();
    configPath_ = json.value("configPath").toString();
    taskType_ = static_cast<TaskType>(json.value("taskType").toInt(static_cast<int>(ObjectDetection)));
    inferenceEngine_ = static_cast<InferenceEngine>(json.value("inferenceEngine").toInt(static_cast<int>(OpenCVDNN)));
    confidenceThreshold_ = static_cast<float>(json.value("confidenceThreshold").toDouble(0.5));
    nmsThreshold_ = static_cast<float>(json.value("nmsThreshold").toDouble(0.4));
    inputSize_.setWidth(json.value("inputWidth").toInt(640));
    inputSize_.setHeight(json.value("inputHeight").toInt(640));
    useGPU_ = json.value("useGPU").toBool(false);

    classNames_.clear();
    QJsonArray classArray = json.value("classNames").toArray();
    for (const auto& val : classArray) {
        classNames_.append(val.toString());
    }

    // 如果有模型路径，尝试加载
    if (!modelPath_.isEmpty()) {
        loadModel(modelPath_, configPath_);
    }
}

void AIDetectionTool::setTaskType(TaskType type)
{
    if (taskType_ != type) {
        taskType_ = type;
        emit paramChanged();
    }
}

void AIDetectionTool::setInferenceEngine(InferenceEngine engine)
{
    if (inferenceEngine_ != engine) {
        inferenceEngine_ = engine;
        emit paramChanged();
    }
}

void AIDetectionTool::setConfidenceThreshold(float threshold)
{
    if (qAbs(confidenceThreshold_ - threshold) > 0.001f) {
        confidenceThreshold_ = threshold;
        emit paramChanged();
    }
}

void AIDetectionTool::setNmsThreshold(float threshold)
{
    if (qAbs(nmsThreshold_ - threshold) > 0.001f) {
        nmsThreshold_ = threshold;
        emit paramChanged();
    }
}

void AIDetectionTool::setInputSize(int width, int height)
{
    if (inputSize_.width() != width || inputSize_.height() != height) {
        inputSize_.setWidth(width);
        inputSize_.setHeight(height);
        emit paramChanged();
    }
}

void AIDetectionTool::setClassNames(const QStringList& names)
{
    classNames_ = names;
    emit paramChanged();
}

void AIDetectionTool::setUseGPU(bool use)
{
    if (useGPU_ != use) {
        useGPU_ = use;

        // 如果模型已加载，重新设置后端
        if (modelLoaded_) {
            if (useGPU_) {
                net_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
                net_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
            } else {
                net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
                net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
            }
        }

        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
