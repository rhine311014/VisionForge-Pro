/**
 * @file YOLODetector.cpp
 * @brief YOLO系列检测器实现
 */

#include "algorithm/ai/YOLODetector.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QElapsedTimer>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <algorithm>
#include <cmath>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// DetectionBox
// ============================================================

QJsonObject DetectionBox::toJson() const {
    QJsonObject json;
    json["classId"] = classId;
    json["className"] = className;
    json["confidence"] = static_cast<double>(confidence);
    json["x"] = box.x();
    json["y"] = box.y();
    json["width"] = box.width();
    json["height"] = box.height();
    json["centerX"] = center.x();
    json["centerY"] = center.y();
    json["angle"] = static_cast<double>(angle);

    if (!keypoints.empty()) {
        QJsonArray kpArray;
        for (size_t i = 0; i < keypoints.size(); ++i) {
            QJsonObject kp;
            kp["x"] = keypoints[i].x();
            kp["y"] = keypoints[i].y();
            if (i < keypointScores.size()) {
                kp["score"] = static_cast<double>(keypointScores[i]);
            }
            kpArray.append(kp);
        }
        json["keypoints"] = kpArray;
    }

    return json;
}

// ============================================================
// YOLOConfig
// ============================================================

QJsonObject YOLOConfig::toJson() const {
    QJsonObject json;
    json["version"] = static_cast<int>(version);
    json["taskMode"] = static_cast<int>(taskMode);
    json["confidenceThreshold"] = static_cast<double>(confidenceThreshold);
    json["nmsThreshold"] = static_cast<double>(nmsThreshold);
    json["maskThreshold"] = static_cast<double>(maskThreshold);
    json["inputWidth"] = inputSize.width();
    json["inputHeight"] = inputSize.height();
    json["letterBox"] = letterBox;
    json["agnosticNMS"] = agnosticNMS;
    json["maxDetections"] = maxDetections;
    json["numKeypoints"] = numKeypoints;

    QJsonArray classArray;
    for (const auto& name : classNames) {
        classArray.append(name);
    }
    json["classNames"] = classArray;

    return json;
}

YOLOConfig YOLOConfig::fromJson(const QJsonObject& json) {
    YOLOConfig config;
    config.version = static_cast<YOLOVersion>(json["version"].toInt());
    config.taskMode = static_cast<YOLOTaskMode>(json["taskMode"].toInt());
    config.confidenceThreshold = static_cast<float>(json["confidenceThreshold"].toDouble(0.5));
    config.nmsThreshold = static_cast<float>(json["nmsThreshold"].toDouble(0.45));
    config.maskThreshold = static_cast<float>(json["maskThreshold"].toDouble(0.5));
    config.inputSize = QSize(json["inputWidth"].toInt(640), json["inputHeight"].toInt(640));
    config.letterBox = json["letterBox"].toBool(true);
    config.agnosticNMS = json["agnosticNMS"].toBool(false);
    config.maxDetections = json["maxDetections"].toInt(300);
    config.numKeypoints = json["numKeypoints"].toInt(17);

    for (const auto& val : json["classNames"].toArray()) {
        config.classNames.append(val.toString());
    }

    return config;
}

// ============================================================
// YOLODetector
// ============================================================

YOLODetector::YOLODetector(QObject* parent)
    : QObject(parent)
{
    // 初始化类别颜色
    m_classColors = {
        cv::Scalar(255, 56, 56), cv::Scalar(255, 157, 151), cv::Scalar(255, 112, 31),
        cv::Scalar(255, 178, 29), cv::Scalar(207, 210, 49), cv::Scalar(72, 249, 10),
        cv::Scalar(146, 204, 23), cv::Scalar(61, 219, 134), cv::Scalar(26, 147, 52),
        cv::Scalar(0, 212, 187), cv::Scalar(44, 153, 168), cv::Scalar(0, 194, 255),
        cv::Scalar(52, 69, 147), cv::Scalar(100, 115, 255), cv::Scalar(0, 24, 236),
        cv::Scalar(132, 56, 255), cv::Scalar(82, 0, 133), cv::Scalar(203, 56, 255),
        cv::Scalar(255, 149, 200), cv::Scalar(255, 55, 199)
    };

    // COCO骨骼连接
    m_skeleton = {
        {15, 13}, {13, 11}, {16, 14}, {14, 12}, {11, 12},
        {5, 11}, {6, 12}, {5, 6}, {5, 7}, {6, 8},
        {7, 9}, {8, 10}, {1, 2}, {0, 1}, {0, 2},
        {1, 3}, {2, 4}, {3, 5}, {4, 6}
    };

    // 默认类别名称
    m_config.classNames = loadCOCOClassNames();
}

YOLODetector::~YOLODetector() {
    unloadModel();
}

bool YOLODetector::loadModel(const QString& modelPath, InferenceBackend backend) {
    QFileInfo fileInfo(modelPath);
    if (!fileInfo.exists()) {
        m_lastError = QString("模型文件不存在: %1").arg(modelPath);
        emit errorOccurred(m_lastError);
        return false;
    }

    try {
        // 根据后端选择加载方式
        if (backend == InferenceBackend::OpenCVDNN || backend == InferenceBackend::Auto) {
            QString ext = fileInfo.suffix().toLower();

            if (ext == "onnx") {
                m_cvNet = cv::dnn::readNetFromONNX(modelPath.toStdString());
            } else {
                m_lastError = "不支持的模型格式，请使用ONNX格式";
                emit errorOccurred(m_lastError);
                return false;
            }

            if (m_cvNet.empty()) {
                m_lastError = "模型加载失败";
                emit errorOccurred(m_lastError);
                return false;
            }

            // 设置后端
            m_cvNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            m_cvNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

#ifdef USE_CUDA
            // 尝试使用CUDA
            try {
                m_cvNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
                m_cvNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
            } catch (...) {
                m_cvNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
                m_cvNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
            }
#endif
        }
        else if (m_engine) {
            InferenceConfig config;
            config.inputSize = m_config.inputSize;
            if (!m_engine->initialize(config)) {
                m_lastError = m_engine->lastError();
                emit errorOccurred(m_lastError);
                return false;
            }
            if (!m_engine->loadModel(modelPath)) {
                m_lastError = m_engine->lastError();
                emit errorOccurred(m_lastError);
                return false;
            }
        }

        m_modelLoaded = true;
        emit modelLoaded(true);
        return true;

    } catch (const cv::Exception& e) {
        m_lastError = QString("模型加载异常: %1").arg(e.what());
        emit errorOccurred(m_lastError);
        emit modelLoaded(false);
        return false;
    }
}

void YOLODetector::unloadModel() {
    m_cvNet = cv::dnn::Net();
    if (m_engine) {
        m_engine->unloadModel();
    }
    m_modelLoaded = false;
}

void YOLODetector::setInferenceEngine(std::shared_ptr<InferenceEngine> engine) {
    m_engine = engine;
}

void YOLODetector::setConfig(const YOLOConfig& config) {
    m_config = config;
}

void YOLODetector::setConfidenceThreshold(float threshold) {
    m_config.confidenceThreshold = threshold;
}

void YOLODetector::setNmsThreshold(float threshold) {
    m_config.nmsThreshold = threshold;
}

void YOLODetector::setClassNames(const QStringList& names) {
    m_config.classNames = names;
}

void YOLODetector::setInputSize(const QSize& size) {
    m_config.inputSize = size;
}

YOLOResult YOLODetector::detect(const cv::Mat& image) {
    YOLOResult result;

    if (!m_modelLoaded) {
        result.errorMessage = "模型未加载";
        emit errorOccurred(result.errorMessage);
        return result;
    }

    if (image.empty()) {
        result.errorMessage = "输入图像为空";
        emit errorOccurred(result.errorMessage);
        return result;
    }

    QElapsedTimer timer;
    timer.start();

    try {
        // 预处理
        float scale;
        int padLeft, padTop;
        cv::Mat blob = preprocess(image, scale, padLeft, padTop);
        result.preprocessTime = timer.elapsed();

        timer.restart();

        // 推理
        std::vector<cv::Mat> outputs;

        if (!m_cvNet.empty()) {
            m_cvNet.setInput(blob);
            m_cvNet.forward(outputs, m_cvNet.getUnconnectedOutLayersNames());
        }
        else if (m_engine && m_engine->isModelLoaded()) {
            cv::Mat inputMat;
            blob.copyTo(inputMat);
            if (!m_engine->infer(inputMat, outputs)) {
                result.errorMessage = m_engine->lastError();
                return result;
            }
        }

        result.inferenceTime = timer.elapsed();
        timer.restart();

        // 后处理
        if (!outputs.empty()) {
            // 检测版本
            if (m_detectedVersion == YOLOVersion::Auto) {
                m_detectedVersion = detectVersion(outputs[0]);
            }

            switch (m_config.taskMode) {
            case YOLOTaskMode::Detect:
            case YOLOTaskMode::Classify:
                postprocessDetect(outputs[0], result, image.size(), scale, padLeft, padTop);
                break;

            case YOLOTaskMode::Segment:
                postprocessSegment(outputs, result, image.size(), scale, padLeft, padTop);
                break;

            case YOLOTaskMode::Pose:
                postprocessPose(outputs[0], result, image.size(), scale, padLeft, padTop);
                break;

            case YOLOTaskMode::OBB:
                postprocessOBB(outputs[0], result, image.size(), scale, padLeft, padTop);
                break;
            }
        }

        result.postprocessTime = timer.elapsed();
        result.totalTime = result.preprocessTime + result.inferenceTime + result.postprocessTime;
        result.success = true;

        emit detectionCompleted(result.detectionCount(), result.totalTime);

    } catch (const cv::Exception& e) {
        result.errorMessage = QString("检测失败: %1").arg(e.what());
        emit errorOccurred(result.errorMessage);
    }

    return result;
}

std::vector<YOLOResult> YOLODetector::detectBatch(const std::vector<cv::Mat>& images) {
    std::vector<YOLOResult> results;
    results.reserve(images.size());

    for (const auto& image : images) {
        results.push_back(detect(image));
    }

    return results;
}

cv::Mat YOLODetector::preprocess(const cv::Mat& image, float& scale, int& padLeft, int& padTop) {
    cv::Mat processed;

    if (m_config.letterBox) {
        processed = letterBoxImage(image, cv::Size(m_config.inputSize.width(),
                                                    m_config.inputSize.height()),
                                   scale, padLeft, padTop);
    } else {
        cv::resize(image, processed,
                   cv::Size(m_config.inputSize.width(), m_config.inputSize.height()));
        scale = static_cast<float>(m_config.inputSize.width()) / image.cols;
        padLeft = padTop = 0;
    }

    cv::Mat blob;
    cv::dnn::blobFromImage(processed, blob, 1.0 / 255.0,
                           cv::Size(m_config.inputSize.width(), m_config.inputSize.height()),
                           cv::Scalar(0, 0, 0), true, false);

    return blob;
}

cv::Mat YOLODetector::letterBoxImage(const cv::Mat& image, const cv::Size& targetSize,
                                      float& scale, int& padLeft, int& padTop) {
    int origWidth = image.cols;
    int origHeight = image.rows;

    float scaleW = static_cast<float>(targetSize.width) / origWidth;
    float scaleH = static_cast<float>(targetSize.height) / origHeight;
    scale = std::min(scaleW, scaleH);

    int newWidth = static_cast<int>(origWidth * scale);
    int newHeight = static_cast<int>(origHeight * scale);

    padLeft = (targetSize.width - newWidth) / 2;
    padTop = (targetSize.height - newHeight) / 2;

    cv::Mat resized;
    cv::resize(image, resized, cv::Size(newWidth, newHeight));

    cv::Mat letterboxed(targetSize, image.type(), cv::Scalar(114, 114, 114));
    resized.copyTo(letterboxed(cv::Rect(padLeft, padTop, newWidth, newHeight)));

    return letterboxed;
}

void YOLODetector::postprocessDetect(const cv::Mat& output, YOLOResult& result,
                                      const cv::Size& originalSize, float scale,
                                      int padLeft, int padTop) {
    cv::Mat detection = output;

    // 处理不同的输出格式
    if (detection.dims == 3) {
        detection = detection.reshape(1, detection.size[1]);
    }

    // YOLOv8格式: [batch, 84, num_detections] -> 需要转置
    // YOLOv5格式: [batch, num_detections, 85]
    bool needTranspose = (detection.cols > detection.rows && detection.rows < 100);
    if (needTranspose) {
        cv::transpose(detection, detection);
    }

    std::vector<DetectionBox> rawDetections;
    int numClasses = m_config.classNames.size();
    if (numClasses == 0) numClasses = 80;  // COCO默认

    // 解析检测结果
    for (int i = 0; i < detection.rows; ++i) {
        const float* data = detection.ptr<float>(i);

        // YOLOv8格式: [x, y, w, h, class_scores...]
        // YOLOv5格式: [x, y, w, h, objectness, class_scores...]
        int scoreOffset = needTranspose ? 4 : 5;  // v8没有objectness

        // 找最大类别分数
        int classId = -1;
        float maxScore = 0;

        for (int j = 0; j < numClasses && j + scoreOffset < detection.cols; ++j) {
            float score = data[j + scoreOffset];
            if (score > maxScore) {
                maxScore = score;
                classId = j;
            }
        }

        // v5有objectness
        float confidence = needTranspose ? maxScore : data[4] * maxScore;

        if (confidence < m_config.confidenceThreshold) continue;

        // 解析边界框
        float cx = data[0];
        float cy = data[1];
        float w = data[2];
        float h = data[3];

        // 还原到原图坐标
        float x = (cx - padLeft) / scale - w / 2 / scale;
        float y = (cy - padTop) / scale - h / 2 / scale;
        w /= scale;
        h /= scale;

        // 裁剪到图像边界
        x = std::max(0.0f, x);
        y = std::max(0.0f, y);
        w = std::min(w, static_cast<float>(originalSize.width) - x);
        h = std::min(h, static_cast<float>(originalSize.height) - y);

        DetectionBox det;
        det.classId = classId;
        det.className = (classId >= 0 && classId < m_config.classNames.size())
                        ? m_config.classNames[classId]
                        : QString("class_%1").arg(classId);
        det.confidence = confidence;
        det.box = QRectF(x, y, w, h);
        det.center = det.box.center();

        rawDetections.push_back(det);
    }

    // NMS
    applyNMS(rawDetections);

    // 限制最大检测数
    if (rawDetections.size() > static_cast<size_t>(m_config.maxDetections)) {
        rawDetections.resize(m_config.maxDetections);
    }

    result.detections = std::move(rawDetections);
}

void YOLODetector::postprocessSegment(const std::vector<cv::Mat>& outputs, YOLOResult& result,
                                       const cv::Size& originalSize, float scale,
                                       int padLeft, int padTop) {
    if (outputs.size() < 2) {
        // 回退到普通检测
        postprocessDetect(outputs[0], result, originalSize, scale, padLeft, padTop);
        return;
    }

    cv::Mat detection = outputs[0];
    cv::Mat protos = outputs[1];

    if (detection.dims == 3) {
        detection = detection.reshape(1, detection.size[1]);
    }

    bool needTranspose = (detection.cols > detection.rows && detection.rows < 100);
    if (needTranspose) {
        cv::transpose(detection, detection);
    }

    int numClasses = m_config.classNames.size();
    if (numClasses == 0) numClasses = 80;
    int maskCoeffsStart = 4 + numClasses;  // YOLOv8-seg: [x,y,w,h,classes...,mask_coeffs...]

    std::vector<DetectionBox> rawDetections;
    std::vector<cv::Mat> maskCoeffsList;

    for (int i = 0; i < detection.rows; ++i) {
        const float* data = detection.ptr<float>(i);

        int classId = -1;
        float maxScore = 0;

        for (int j = 0; j < numClasses; ++j) {
            float score = data[4 + j];
            if (score > maxScore) {
                maxScore = score;
                classId = j;
            }
        }

        if (maxScore < m_config.confidenceThreshold) continue;

        float cx = data[0];
        float cy = data[1];
        float w = data[2];
        float h = data[3];

        float x = (cx - padLeft) / scale - w / 2 / scale;
        float y = (cy - padTop) / scale - h / 2 / scale;
        w /= scale;
        h /= scale;

        x = std::max(0.0f, x);
        y = std::max(0.0f, y);
        w = std::min(w, static_cast<float>(originalSize.width) - x);
        h = std::min(h, static_cast<float>(originalSize.height) - y);

        DetectionBox det;
        det.classId = classId;
        det.className = (classId >= 0 && classId < m_config.classNames.size())
                        ? m_config.classNames[classId]
                        : QString("class_%1").arg(classId);
        det.confidence = maxScore;
        det.box = QRectF(x, y, w, h);
        det.center = det.box.center();

        rawDetections.push_back(det);

        // 保存mask系数
        if (detection.cols > maskCoeffsStart) {
            cv::Mat coeffs(1, detection.cols - maskCoeffsStart, CV_32F);
            for (int j = maskCoeffsStart; j < detection.cols; ++j) {
                coeffs.at<float>(0, j - maskCoeffsStart) = data[j];
            }
            maskCoeffsList.push_back(coeffs);
        }
    }

    // NMS
    std::vector<int> keepIndices;
    {
        std::vector<cv::Rect> boxes;
        std::vector<float> scores;
        std::vector<int> classIds;
        for (const auto& det : rawDetections) {
            boxes.push_back(cv::Rect(static_cast<int>(det.box.x()),
                                     static_cast<int>(det.box.y()),
                                     static_cast<int>(det.box.width()),
                                     static_cast<int>(det.box.height())));
            scores.push_back(det.confidence);
            classIds.push_back(det.classId);
        }
        cv::dnn::NMSBoxes(boxes, scores, m_config.confidenceThreshold,
                          m_config.nmsThreshold, keepIndices);
    }

    // 处理保留的检测和掩码
    result.detections.clear();
    for (int idx : keepIndices) {
        DetectionBox& det = rawDetections[idx];

        // 处理掩码
        if (idx < static_cast<int>(maskCoeffsList.size())) {
            det.mask = processSegmentMask(protos, maskCoeffsList[idx],
                                          det.box, originalSize);
        }

        result.detections.push_back(det);

        if (result.detections.size() >= static_cast<size_t>(m_config.maxDetections)) {
            break;
        }
    }
}

void YOLODetector::postprocessPose(const cv::Mat& output, YOLOResult& result,
                                    const cv::Size& originalSize, float scale,
                                    int padLeft, int padTop) {
    cv::Mat detection = output;

    if (detection.dims == 3) {
        detection = detection.reshape(1, detection.size[1]);
    }

    bool needTranspose = (detection.cols > detection.rows && detection.rows < 100);
    if (needTranspose) {
        cv::transpose(detection, detection);
    }

    std::vector<DetectionBox> rawDetections;
    int numKeypoints = m_config.numKeypoints;

    for (int i = 0; i < detection.rows; ++i) {
        const float* data = detection.ptr<float>(i);

        float confidence = data[4];  // person置信度
        if (confidence < m_config.confidenceThreshold) continue;

        float cx = data[0];
        float cy = data[1];
        float w = data[2];
        float h = data[3];

        float x = (cx - padLeft) / scale - w / 2 / scale;
        float y = (cy - padTop) / scale - h / 2 / scale;
        w /= scale;
        h /= scale;

        DetectionBox det;
        det.classId = 0;  // person
        det.className = "person";
        det.confidence = confidence;
        det.box = QRectF(x, y, w, h);
        det.center = det.box.center();

        // 解析关键点
        int kpOffset = 5;
        for (int k = 0; k < numKeypoints; ++k) {
            float kpX = (data[kpOffset + k * 3] - padLeft) / scale;
            float kpY = (data[kpOffset + k * 3 + 1] - padTop) / scale;
            float kpScore = data[kpOffset + k * 3 + 2];

            det.keypoints.push_back(QPointF(kpX, kpY));
            det.keypointScores.push_back(kpScore);
        }

        rawDetections.push_back(det);
    }

    applyNMS(rawDetections);
    result.detections = std::move(rawDetections);
}

void YOLODetector::postprocessOBB(const cv::Mat& output, YOLOResult& result,
                                   const cv::Size& originalSize, float scale,
                                   int padLeft, int padTop) {
    cv::Mat detection = output;

    if (detection.dims == 3) {
        detection = detection.reshape(1, detection.size[1]);
    }

    bool needTranspose = (detection.cols > detection.rows && detection.rows < 100);
    if (needTranspose) {
        cv::transpose(detection, detection);
    }

    std::vector<DetectionBox> rawDetections;
    int numClasses = m_config.classNames.size();
    if (numClasses == 0) numClasses = 15;  // DOTA默认

    for (int i = 0; i < detection.rows; ++i) {
        const float* data = detection.ptr<float>(i);

        int classId = -1;
        float maxScore = 0;

        for (int j = 0; j < numClasses; ++j) {
            float score = data[4 + j];
            if (score > maxScore) {
                maxScore = score;
                classId = j;
            }
        }

        if (maxScore < m_config.confidenceThreshold) continue;

        float cx = data[0];
        float cy = data[1];
        float w = data[2];
        float h = data[3];
        float angle = data[4 + numClasses];  // 角度在类别分数之后

        // 还原到原图坐标
        cx = (cx - padLeft) / scale;
        cy = (cy - padTop) / scale;
        w /= scale;
        h /= scale;

        DetectionBox det;
        det.classId = classId;
        det.className = (classId >= 0 && classId < m_config.classNames.size())
                        ? m_config.classNames[classId]
                        : QString("class_%1").arg(classId);
        det.confidence = maxScore;
        det.angle = angle * 180.0f / static_cast<float>(CV_PI);  // 转换为度
        det.center = QPointF(cx, cy);

        // 计算旋转框四角
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        float hw = w / 2, hh = h / 2;

        det.rotatedBox << QPointF(cx + (-hw * cosA - (-hh) * sinA), cy + (-hw * sinA + (-hh) * cosA));
        det.rotatedBox << QPointF(cx + (hw * cosA - (-hh) * sinA), cy + (hw * sinA + (-hh) * cosA));
        det.rotatedBox << QPointF(cx + (hw * cosA - hh * sinA), cy + (hw * sinA + hh * cosA));
        det.rotatedBox << QPointF(cx + (-hw * cosA - hh * sinA), cy + (-hw * sinA + hh * cosA));

        // 计算外接矩形作为box
        float minX = cx, maxX = cx, minY = cy, maxY = cy;
        for (const auto& pt : det.rotatedBox) {
            minX = std::min(minX, static_cast<float>(pt.x()));
            maxX = std::max(maxX, static_cast<float>(pt.x()));
            minY = std::min(minY, static_cast<float>(pt.y()));
            maxY = std::max(maxY, static_cast<float>(pt.y()));
        }
        det.box = QRectF(minX, minY, maxX - minX, maxY - minY);

        rawDetections.push_back(det);
    }

    applyNMS(rawDetections);
    result.detections = std::move(rawDetections);
}

void YOLODetector::applyNMS(std::vector<DetectionBox>& detections) {
    if (detections.size() <= 1) return;

    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    std::vector<int> classIds;

    for (const auto& det : detections) {
        boxes.push_back(cv::Rect(static_cast<int>(det.box.x()),
                                 static_cast<int>(det.box.y()),
                                 static_cast<int>(det.box.width()),
                                 static_cast<int>(det.box.height())));
        scores.push_back(det.confidence);
        classIds.push_back(m_config.agnosticNMS ? 0 : det.classId);
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, scores, m_config.confidenceThreshold,
                      m_config.nmsThreshold, indices);

    std::vector<DetectionBox> filtered;
    for (int idx : indices) {
        filtered.push_back(detections[idx]);
    }

    detections = std::move(filtered);
}

cv::Mat YOLODetector::processSegmentMask(const cv::Mat& protos, const cv::Mat& maskCoeffs,
                                          const QRectF& box, const cv::Size& originalSize) {
    // protos: [1, 32, H, W] 或 [32, H, W]
    // maskCoeffs: [1, 32]

    cv::Mat protosMat = protos;
    if (protosMat.dims == 4) {
        // 取第一个batch
        int sizes[] = {protosMat.size[1], protosMat.size[2], protosMat.size[3]};
        protosMat = cv::Mat(3, sizes, CV_32F, protosMat.ptr<float>());
    }

    int protoH = protosMat.size[1];
    int protoW = protosMat.size[2];
    int numProtos = protosMat.size[0];

    // 计算掩码
    cv::Mat mask = cv::Mat::zeros(protoH, protoW, CV_32F);
    const float* coeffs = maskCoeffs.ptr<float>();

    for (int c = 0; c < numProtos && c < maskCoeffs.cols; ++c) {
        cv::Mat proto(protoH, protoW, CV_32F,
                      protosMat.ptr<float>() + c * protoH * protoW);
        mask += proto * coeffs[c];
    }

    // Sigmoid
    cv::exp(-mask, mask);
    mask = 1.0 / (1.0 + mask);

    // 阈值化
    cv::threshold(mask, mask, m_config.maskThreshold, 1.0, cv::THRESH_BINARY);

    // 裁剪到边界框区域
    int x1 = static_cast<int>(box.x() * protoW / originalSize.width);
    int y1 = static_cast<int>(box.y() * protoH / originalSize.height);
    int x2 = static_cast<int>((box.x() + box.width()) * protoW / originalSize.width);
    int y2 = static_cast<int>((box.y() + box.height()) * protoH / originalSize.height);

    x1 = std::max(0, x1);
    y1 = std::max(0, y1);
    x2 = std::min(protoW, x2);
    y2 = std::min(protoH, y2);

    cv::Mat croppedMask = mask(cv::Rect(x1, y1, x2 - x1, y2 - y1));

    // 缩放到原图尺寸
    cv::Mat finalMask;
    cv::resize(croppedMask, finalMask,
               cv::Size(static_cast<int>(box.width()), static_cast<int>(box.height())),
               0, 0, cv::INTER_LINEAR);

    finalMask.convertTo(finalMask, CV_8U, 255);

    return finalMask;
}

YOLOVersion YOLODetector::detectVersion(const cv::Mat& output) {
    // 基于输出形状判断版本
    // YOLOv5: [batch, num_detections, 85] (5 + 80)
    // YOLOv8: [batch, 84, num_detections] (4 + 80)

    if (output.dims >= 2) {
        int dim1 = output.size[output.dims - 2];
        int dim2 = output.size[output.dims - 1];

        if (dim1 < dim2 && dim1 < 100) {
            return YOLOVersion::YOLOv8;
        }
    }

    return YOLOVersion::YOLOv5;
}

cv::Mat YOLODetector::drawResults(const cv::Mat& image, const YOLOResult& result) {
    cv::Mat output = image.clone();
    if (output.channels() == 1) {
        cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    }

    for (const auto& det : result.detections) {
        switch (m_config.taskMode) {
        case YOLOTaskMode::Segment:
            drawMask(output, det);
            drawBox(output, det);
            break;

        case YOLOTaskMode::Pose:
            drawKeypoints(output, det);
            drawBox(output, det);
            break;

        case YOLOTaskMode::OBB:
            drawOBB(output, det);
            break;

        default:
            drawBox(output, det);
            break;
        }
    }

    return output;
}

cv::Scalar YOLODetector::getClassColor(int classId) {
    return m_classColors[classId % m_classColors.size()];
}

void YOLODetector::drawBox(cv::Mat& image, const DetectionBox& det) {
    cv::Scalar color = getClassColor(det.classId);

    cv::Rect rect(static_cast<int>(det.box.x()),
                  static_cast<int>(det.box.y()),
                  static_cast<int>(det.box.width()),
                  static_cast<int>(det.box.height()));
    cv::rectangle(image, rect, color, 2);

    // 标签
    QString label = QString("%1 %.1f%%").arg(det.className).arg(det.confidence * 100);
    int baseline;
    cv::Size textSize = cv::getTextSize(label.toStdString(),
                                        cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);

    cv::rectangle(image,
                  cv::Point(rect.x, rect.y - textSize.height - 5),
                  cv::Point(rect.x + textSize.width, rect.y),
                  color, -1);

    cv::putText(image, label.toStdString(),
                cv::Point(rect.x, rect.y - 3),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
}

void YOLODetector::drawMask(cv::Mat& image, const DetectionBox& det) {
    if (det.mask.empty()) return;

    cv::Scalar color = getClassColor(det.classId);

    // 创建彩色掩码
    cv::Mat colorMask(det.mask.size(), CV_8UC3, color);

    // 叠加到图像
    int x = static_cast<int>(det.box.x());
    int y = static_cast<int>(det.box.y());

    if (x >= 0 && y >= 0 && x + det.mask.cols <= image.cols && y + det.mask.rows <= image.rows) {
        cv::Mat roi = image(cv::Rect(x, y, det.mask.cols, det.mask.rows));

        for (int i = 0; i < det.mask.rows; ++i) {
            for (int j = 0; j < det.mask.cols; ++j) {
                if (det.mask.at<uchar>(i, j) > 127) {
                    cv::Vec3b& pixel = roi.at<cv::Vec3b>(i, j);
                    pixel[0] = static_cast<uchar>(pixel[0] * 0.5 + color[0] * 0.5);
                    pixel[1] = static_cast<uchar>(pixel[1] * 0.5 + color[1] * 0.5);
                    pixel[2] = static_cast<uchar>(pixel[2] * 0.5 + color[2] * 0.5);
                }
            }
        }
    }
}

void YOLODetector::drawKeypoints(cv::Mat& image, const DetectionBox& det) {
    if (det.keypoints.empty()) return;

    cv::Scalar color = getClassColor(det.classId);

    // 绘制关键点
    for (size_t i = 0; i < det.keypoints.size(); ++i) {
        float score = (i < det.keypointScores.size()) ? det.keypointScores[i] : 1.0f;
        if (score < 0.5f) continue;

        cv::Point pt(static_cast<int>(det.keypoints[i].x()),
                     static_cast<int>(det.keypoints[i].y()));
        cv::circle(image, pt, 5, color, -1);
    }

    // 绘制骨骼连接
    for (const auto& bone : m_skeleton) {
        int i1 = bone.first;
        int i2 = bone.second;

        if (i1 >= static_cast<int>(det.keypoints.size()) ||
            i2 >= static_cast<int>(det.keypoints.size())) continue;

        float score1 = (i1 < static_cast<int>(det.keypointScores.size()))
                       ? det.keypointScores[i1] : 1.0f;
        float score2 = (i2 < static_cast<int>(det.keypointScores.size()))
                       ? det.keypointScores[i2] : 1.0f;

        if (score1 < 0.5f || score2 < 0.5f) continue;

        cv::Point pt1(static_cast<int>(det.keypoints[i1].x()),
                      static_cast<int>(det.keypoints[i1].y()));
        cv::Point pt2(static_cast<int>(det.keypoints[i2].x()),
                      static_cast<int>(det.keypoints[i2].y()));

        cv::line(image, pt1, pt2, color, 2);
    }
}

void YOLODetector::drawOBB(cv::Mat& image, const DetectionBox& det) {
    if (det.rotatedBox.isEmpty()) {
        drawBox(image, det);
        return;
    }

    cv::Scalar color = getClassColor(det.classId);

    std::vector<cv::Point> pts;
    for (const auto& pt : det.rotatedBox) {
        pts.push_back(cv::Point(static_cast<int>(pt.x()), static_cast<int>(pt.y())));
    }

    cv::polylines(image, pts, true, color, 2);

    // 标签
    QString label = QString("%1 %.1f%%").arg(det.className).arg(det.confidence * 100);
    cv::putText(image, label.toStdString(),
                pts[0], cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
}

// ============================================================
// 工具函数
// ============================================================

QString getYOLOVersionName(YOLOVersion version) {
    switch (version) {
    case YOLOVersion::YOLOv5: return "YOLOv5";
    case YOLOVersion::YOLOv8: return "YOLOv8";
    case YOLOVersion::YOLOv11: return "YOLOv11";
    case YOLOVersion::Auto: return "Auto";
    default: return "Unknown";
    }
}

QString getYOLOTaskModeName(YOLOTaskMode mode) {
    switch (mode) {
    case YOLOTaskMode::Detect: return "检测";
    case YOLOTaskMode::Segment: return "分割";
    case YOLOTaskMode::Pose: return "姿态";
    case YOLOTaskMode::OBB: return "旋转框";
    case YOLOTaskMode::Classify: return "分类";
    default: return "未知";
    }
}

QStringList loadCOCOClassNames() {
    return {
        "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck",
        "boat", "traffic light", "fire hydrant", "stop sign", "parking meter", "bench",
        "bird", "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra",
        "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
        "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove",
        "skateboard", "surfboard", "tennis racket", "bottle", "wine glass", "cup",
        "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich", "orange",
        "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
        "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse",
        "remote", "keyboard", "cell phone", "microwave", "oven", "toaster", "sink",
        "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", "hair drier",
        "toothbrush"
    };
}

QStringList loadClassNamesFromFile(const QString& filePath) {
    QStringList names;
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            if (!line.isEmpty()) {
                names.append(line);
            }
        }
        file.close();
    }

    return names;
}

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
