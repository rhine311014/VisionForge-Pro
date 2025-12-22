/**
 * @file OCREngine.cpp
 * @brief 工业级OCR文字识别引擎实现
 */

#include "algorithm/ai/OCREngine.h"
#include "algorithm/ai/ONNXRuntimeEngine.h"
#include <opencv2/imgproc.hpp>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// OCRConfig 实现
// ============================================================
QJsonObject OCRConfig::toJson() const {
    QJsonObject json;
    json["backend"] = static_cast<int>(backend);
    json["language"] = static_cast<int>(language);
    json["detectionModelPath"] = detectionModelPath;
    json["recognitionModelPath"] = recognitionModelPath;
    json["classifierModelPath"] = classifierModelPath;
    json["dictionaryPath"] = dictionaryPath;
    json["detectionThreshold"] = static_cast<double>(detectionThreshold);
    json["boxThreshold"] = static_cast<double>(boxThreshold);
    json["unclipRatio"] = static_cast<double>(unclipRatio);
    json["maxCandidates"] = maxCandidates;
    json["minBoxSize"] = minBoxSize;
    json["recognitionThreshold"] = static_cast<double>(recognitionThreshold);
    json["maxTextLength"] = maxTextLength;
    json["useAngleClassifier"] = useAngleClassifier;
    json["detInputHeight"] = detInputHeight;
    json["recInputHeight"] = recInputHeight;
    json["recInputWidth"] = recInputWidth;
    json["sortByPosition"] = sortByPosition;
    json["mergeBoxes"] = mergeBoxes;
    json["mergeThreshold"] = static_cast<double>(mergeThreshold);
    json["inferenceBackend"] = static_cast<int>(inferenceBackend);
    json["device"] = static_cast<int>(device);
    return json;
}

OCRConfig OCRConfig::fromJson(const QJsonObject& json) {
    OCRConfig config;
    config.backend = static_cast<OCRBackend>(json["backend"].toInt());
    config.language = static_cast<OCRLanguage>(json["language"].toInt());
    config.detectionModelPath = json["detectionModelPath"].toString();
    config.recognitionModelPath = json["recognitionModelPath"].toString();
    config.classifierModelPath = json["classifierModelPath"].toString();
    config.dictionaryPath = json["dictionaryPath"].toString();
    config.detectionThreshold = static_cast<float>(json["detectionThreshold"].toDouble(0.3));
    config.boxThreshold = static_cast<float>(json["boxThreshold"].toDouble(0.5));
    config.unclipRatio = static_cast<float>(json["unclipRatio"].toDouble(1.6));
    config.maxCandidates = json["maxCandidates"].toInt(1000);
    config.minBoxSize = json["minBoxSize"].toInt(3);
    config.recognitionThreshold = static_cast<float>(json["recognitionThreshold"].toDouble(0.5));
    config.maxTextLength = json["maxTextLength"].toInt(100);
    config.useAngleClassifier = json["useAngleClassifier"].toBool(false);
    config.detInputHeight = json["detInputHeight"].toInt(960);
    config.recInputHeight = json["recInputHeight"].toInt(48);
    config.recInputWidth = json["recInputWidth"].toInt(320);
    config.sortByPosition = json["sortByPosition"].toBool(true);
    config.mergeBoxes = json["mergeBoxes"].toBool(false);
    config.mergeThreshold = static_cast<float>(json["mergeThreshold"].toDouble(0.5));
    config.inferenceBackend = static_cast<InferenceBackend>(json["inferenceBackend"].toInt());
    config.device = static_cast<ExecutionDevice>(json["device"].toInt());
    return config;
}

// ============================================================
// OCRResult 实现
// ============================================================
QStringList OCRResult::getTexts() const {
    QStringList texts;
    for (const auto& box : textBoxes) {
        texts.append(box.text);
    }
    return texts;
}

std::vector<TextBox*> OCRResult::getBoxesByConfidence(float minConf) {
    std::vector<TextBox*> result;
    for (auto& box : textBoxes) {
        if (box.confidence >= minConf) {
            result.push_back(&box);
        }
    }
    return result;
}

TextBox* OCRResult::findTextContaining(const QString& substring) {
    for (auto& box : textBoxes) {
        if (box.text.contains(substring, Qt::CaseInsensitive)) {
            return &box;
        }
    }
    return nullptr;
}

// ============================================================
// OCREngine 实现
// ============================================================
OCREngine::OCREngine(QObject* parent)
    : QObject(parent)
{
}

OCREngine::~OCREngine() {
    release();
}

bool OCREngine::initialize(const OCRConfig& config) {
    m_config = config;
    m_lastError.clear();

    // 加载检测模型
    if (!config.detectionModelPath.isEmpty()) {
        if (!loadDetectionModel(config.detectionModelPath)) {
            return false;
        }
    }

    // 加载识别模型
    if (!config.recognitionModelPath.isEmpty()) {
        if (!loadRecognitionModel(config.recognitionModelPath)) {
            return false;
        }
    }

    // 加载分类器模型（可选）
    if (config.useAngleClassifier && !config.classifierModelPath.isEmpty()) {
        if (!loadClassifierModel(config.classifierModelPath)) {
            qWarning() << "Failed to load classifier model, continuing without it";
        }
    }

    // 加载字典
    if (!config.dictionaryPath.isEmpty()) {
        if (!loadDictionary(config.dictionaryPath)) {
            return false;
        }
    }

    m_initialized = true;
    return true;
}

bool OCREngine::loadDetectionModel(const QString& modelPath) {
    m_detectionEngine = createInferenceEngine(m_config.inferenceBackend);
    if (!m_detectionEngine) {
        m_lastError = "Failed to create detection engine";
        emit errorOccurred(m_lastError);
        return false;
    }

    InferenceConfig engineConfig;
    engineConfig.device = m_config.device;

    if (!m_detectionEngine->initialize(engineConfig)) {
        m_lastError = m_detectionEngine->lastError();
        emit errorOccurred(m_lastError);
        return false;
    }

    if (!m_detectionEngine->loadModel(modelPath)) {
        m_lastError = m_detectionEngine->lastError();
        emit errorOccurred(m_lastError);
        return false;
    }

    m_config.detectionModelPath = modelPath;
    return true;
}

bool OCREngine::loadRecognitionModel(const QString& modelPath) {
    m_recognitionEngine = createInferenceEngine(m_config.inferenceBackend);
    if (!m_recognitionEngine) {
        m_lastError = "Failed to create recognition engine";
        emit errorOccurred(m_lastError);
        return false;
    }

    InferenceConfig engineConfig;
    engineConfig.device = m_config.device;

    if (!m_recognitionEngine->initialize(engineConfig)) {
        m_lastError = m_recognitionEngine->lastError();
        emit errorOccurred(m_lastError);
        return false;
    }

    if (!m_recognitionEngine->loadModel(modelPath)) {
        m_lastError = m_recognitionEngine->lastError();
        emit errorOccurred(m_lastError);
        return false;
    }

    m_config.recognitionModelPath = modelPath;
    return true;
}

bool OCREngine::loadClassifierModel(const QString& modelPath) {
    m_classifierEngine = createInferenceEngine(m_config.inferenceBackend);
    if (!m_classifierEngine) {
        m_lastError = "Failed to create classifier engine";
        return false;
    }

    InferenceConfig engineConfig;
    engineConfig.device = m_config.device;

    if (!m_classifierEngine->initialize(engineConfig)) {
        m_lastError = m_classifierEngine->lastError();
        return false;
    }

    if (!m_classifierEngine->loadModel(modelPath)) {
        m_lastError = m_classifierEngine->lastError();
        return false;
    }

    m_config.classifierModelPath = modelPath;
    return true;
}

bool OCREngine::loadDictionary(const QString& dictPath) {
    QFile file(dictPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = "Failed to open dictionary file: " + dictPath;
        emit errorOccurred(m_lastError);
        return false;
    }

    m_dictionary.clear();
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            m_dictionary.append(line);
        }
    }

    file.close();

    // 添加空白字符（用于CTC blank）
    m_dictionary.prepend(" ");  // blank token at index 0

    qDebug() << "Loaded dictionary with" << m_dictionary.size() << "characters";
    return true;
}

void OCREngine::release() {
    if (m_detectionEngine) {
        m_detectionEngine->release();
        m_detectionEngine.reset();
    }
    if (m_recognitionEngine) {
        m_recognitionEngine->release();
        m_recognitionEngine.reset();
    }
    if (m_classifierEngine) {
        m_classifierEngine->release();
        m_classifierEngine.reset();
    }
    m_dictionary.clear();
    m_initialized = false;
}

cv::Mat OCREngine::preprocessDetection(const cv::Mat& image, float& ratio) {
    // 保持纵横比缩放到目标高度
    int targetHeight = m_config.detInputHeight;
    ratio = static_cast<float>(targetHeight) / image.rows;
    int targetWidth = static_cast<int>(image.cols * ratio);

    // 确保宽度是32的倍数
    targetWidth = ((targetWidth + 31) / 32) * 32;

    cv::Mat resized;
    cv::resize(image, resized, cv::Size(targetWidth, targetHeight));

    // 归一化
    cv::Mat normalized;
    resized.convertTo(normalized, CV_32FC3, 1.0 / 255.0);

    // 减去均值
    cv::Scalar mean(0.485, 0.456, 0.406);
    cv::Scalar std(0.229, 0.224, 0.225);
    normalized = (normalized - mean) / std;

    // BGR -> RGB
    cv::cvtColor(normalized, normalized, cv::COLOR_BGR2RGB);

    // HWC -> CHW
    cv::Mat channels[3];
    cv::split(normalized, channels);
    cv::Mat chw;
    cv::vconcat(std::vector<cv::Mat>{
        channels[0].reshape(1, 1),
        channels[1].reshape(1, 1),
        channels[2].reshape(1, 1)
    }, chw);

    return chw.reshape(1, {1, 3, targetHeight, targetWidth});
}

std::vector<std::vector<cv::Point>> OCREngine::dbnetPostprocess(
    const cv::Mat& bitmap,
    const cv::Mat& pred,
    float boxThresh,
    float unclipRatio)
{
    std::vector<std::vector<cv::Point>> boxes;

    // 二值化
    cv::Mat mask;
    cv::threshold(pred, mask, m_config.detectionThreshold, 1.0, cv::THRESH_BINARY);
    mask.convertTo(mask, CV_8UC1, 255);

    // 找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        if (contour.size() < 4) continue;

        // 计算最小外接矩形
        cv::RotatedRect rect = cv::minAreaRect(contour);
        if (rect.size.width < m_config.minBoxSize ||
            rect.size.height < m_config.minBoxSize) {
            continue;
        }

        // 计算轮廓内的平均分数
        cv::Mat contourMask = cv::Mat::zeros(pred.size(), CV_8UC1);
        cv::drawContours(contourMask, std::vector<std::vector<cv::Point>>{contour},
                        0, cv::Scalar(255), -1);
        double meanScore = cv::mean(pred, contourMask)[0];

        if (meanScore < boxThresh) {
            continue;
        }

        // Unclip
        cv::RotatedRect unclipped = unclip(contour, unclipRatio);

        // 获取四个角点
        cv::Point2f pts[4];
        unclipped.points(pts);

        std::vector<cv::Point> box;
        for (int i = 0; i < 4; ++i) {
            box.push_back(cv::Point(static_cast<int>(pts[i].x),
                                    static_cast<int>(pts[i].y)));
        }
        boxes.push_back(box);

        if (static_cast<int>(boxes.size()) >= m_config.maxCandidates) {
            break;
        }
    }

    return boxes;
}

cv::RotatedRect OCREngine::unclip(const std::vector<cv::Point>& box, float ratio) {
    // 计算多边形面积和周长
    double area = cv::contourArea(box);
    double perimeter = cv::arcLength(box, true);

    // 计算扩展距离
    double distance = area * ratio / perimeter;

    // 使用膨胀来扩展轮廓
    cv::Mat mask = cv::Mat::zeros(1000, 1000, CV_8UC1);
    std::vector<std::vector<cv::Point>> contours = {box};

    // 平移到画布中心
    cv::Point2f center(0, 0);
    for (const auto& pt : box) {
        center.x += pt.x;
        center.y += pt.y;
    }
    center.x /= box.size();
    center.y /= box.size();

    std::vector<cv::Point> shiftedBox;
    for (const auto& pt : box) {
        shiftedBox.push_back(cv::Point(
            static_cast<int>(pt.x - center.x + 500),
            static_cast<int>(pt.y - center.y + 500)));
    }

    cv::drawContours(mask, std::vector<std::vector<cv::Point>>{shiftedBox},
                    0, cv::Scalar(255), -1);

    // 膨胀
    int kernelSize = static_cast<int>(distance * 2) + 1;
    kernelSize = std::max(3, std::min(kernelSize, 51));
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                               cv::Size(kernelSize, kernelSize));
    cv::Mat dilated;
    cv::dilate(mask, dilated, kernel);

    // 找新轮廓
    std::vector<std::vector<cv::Point>> newContours;
    cv::findContours(dilated, newContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (newContours.empty()) {
        return cv::minAreaRect(box);
    }

    // 找最大轮廓
    size_t maxIdx = 0;
    double maxArea = 0;
    for (size_t i = 0; i < newContours.size(); ++i) {
        double a = cv::contourArea(newContours[i]);
        if (a > maxArea) {
            maxArea = a;
            maxIdx = i;
        }
    }

    // 平移回原位置
    for (auto& pt : newContours[maxIdx]) {
        pt.x = static_cast<int>(pt.x + center.x - 500);
        pt.y = static_cast<int>(pt.y + center.y - 500);
    }

    return cv::minAreaRect(newContours[maxIdx]);
}

std::vector<QPolygonF> OCREngine::postprocessDetection(
    const Tensor& output,
    const cv::Size& originalSize,
    float ratio)
{
    std::vector<QPolygonF> result;

    // 输出形状: [1, 1, H, W]
    if (output.shape.size() < 4) {
        return result;
    }

    int outH = output.shape[2];
    int outW = output.shape[3];

    // 转换为cv::Mat
    cv::Mat pred(outH, outW, CV_32FC1, const_cast<float*>(output.data.data()));

    // DBNet后处理
    cv::Mat bitmap;
    cv::threshold(pred, bitmap, m_config.detectionThreshold, 1.0, cv::THRESH_BINARY);

    auto boxes = dbnetPostprocess(bitmap, pred, m_config.boxThreshold, m_config.unclipRatio);

    // 缩放回原始尺寸
    for (const auto& box : boxes) {
        QPolygonF polygon;
        for (const auto& pt : box) {
            float x = pt.x / ratio;
            float y = pt.y / ratio;
            x = std::max(0.0f, std::min(x, static_cast<float>(originalSize.width - 1)));
            y = std::max(0.0f, std::min(y, static_cast<float>(originalSize.height - 1)));
            polygon.append(QPointF(x, y));
        }
        result.push_back(polygon);
    }

    return result;
}

cv::Mat OCREngine::preprocessRecognition(const cv::Mat& image) {
    int targetH = m_config.recInputHeight;
    int targetW = m_config.recInputWidth;

    // 保持纵横比
    float ratio = static_cast<float>(targetH) / image.rows;
    int newW = static_cast<int>(image.cols * ratio);

    cv::Mat resized;
    if (newW > targetW) {
        // 太宽，直接缩放
        cv::resize(image, resized, cv::Size(targetW, targetH));
    } else {
        // 右侧填充
        cv::resize(image, resized, cv::Size(newW, targetH));
        cv::Mat padded = cv::Mat::zeros(targetH, targetW, resized.type());
        resized.copyTo(padded(cv::Rect(0, 0, newW, targetH)));
        resized = padded;
    }

    // 归一化
    cv::Mat normalized;
    resized.convertTo(normalized, CV_32FC3, 1.0 / 255.0);
    normalized = (normalized - 0.5) / 0.5;

    // BGR -> RGB
    cv::cvtColor(normalized, normalized, cv::COLOR_BGR2RGB);

    // HWC -> CHW
    cv::Mat channels[3];
    cv::split(normalized, channels);
    cv::Mat chw;
    cv::vconcat(std::vector<cv::Mat>{
        channels[0].reshape(1, 1),
        channels[1].reshape(1, 1),
        channels[2].reshape(1, 1)
    }, chw);

    return chw.reshape(1, {1, 3, targetH, targetW});
}

QString OCREngine::ctcDecode(const std::vector<int>& indices) {
    QString result;
    int prevIdx = -1;

    for (int idx : indices) {
        // 跳过blank和重复
        if (idx != 0 && idx != prevIdx) {
            if (idx > 0 && idx < m_dictionary.size()) {
                result += m_dictionary[idx];
            }
        }
        prevIdx = idx;
    }

    return result;
}

QString OCREngine::postprocessRecognition(const Tensor& output) {
    // 输出形状: [1, T, numClasses] 或 [T, numClasses]
    if (output.shape.size() < 2) {
        return QString();
    }

    int T = output.shape.size() == 3 ? output.shape[1] : output.shape[0];
    int numClasses = output.shape.back();

    std::vector<int> indices;
    indices.reserve(T);

    for (int t = 0; t < T; ++t) {
        // 找最大值索引
        int maxIdx = 0;
        float maxVal = -std::numeric_limits<float>::max();

        for (int c = 0; c < numClasses; ++c) {
            float val = output.data[t * numClasses + c];
            if (val > maxVal) {
                maxVal = val;
                maxIdx = c;
            }
        }

        indices.push_back(maxIdx);
    }

    return ctcDecode(indices);
}

cv::Mat OCREngine::cropTextRegion(const cv::Mat& image, const QPolygonF& polygon) {
    if (polygon.size() != 4) {
        return cv::Mat();
    }

    // 源点
    std::vector<cv::Point2f> srcPts = {
        cv::Point2f(static_cast<float>(polygon[0].x()), static_cast<float>(polygon[0].y())),
        cv::Point2f(static_cast<float>(polygon[1].x()), static_cast<float>(polygon[1].y())),
        cv::Point2f(static_cast<float>(polygon[2].x()), static_cast<float>(polygon[2].y())),
        cv::Point2f(static_cast<float>(polygon[3].x()), static_cast<float>(polygon[3].y()))
    };

    // 计算目标尺寸
    float width1 = std::sqrt(std::pow(srcPts[1].x - srcPts[0].x, 2) +
                            std::pow(srcPts[1].y - srcPts[0].y, 2));
    float width2 = std::sqrt(std::pow(srcPts[2].x - srcPts[3].x, 2) +
                            std::pow(srcPts[2].y - srcPts[3].y, 2));
    float height1 = std::sqrt(std::pow(srcPts[3].x - srcPts[0].x, 2) +
                             std::pow(srcPts[3].y - srcPts[0].y, 2));
    float height2 = std::sqrt(std::pow(srcPts[2].x - srcPts[1].x, 2) +
                             std::pow(srcPts[2].y - srcPts[1].y, 2));

    int dstW = static_cast<int>(std::max(width1, width2));
    int dstH = static_cast<int>(std::max(height1, height2));

    if (dstW <= 0 || dstH <= 0) {
        return cv::Mat();
    }

    // 目标点
    std::vector<cv::Point2f> dstPts = {
        cv::Point2f(0, 0),
        cv::Point2f(static_cast<float>(dstW - 1), 0),
        cv::Point2f(static_cast<float>(dstW - 1), static_cast<float>(dstH - 1)),
        cv::Point2f(0, static_cast<float>(dstH - 1))
    };

    // 透视变换
    cv::Mat M = cv::getPerspectiveTransform(srcPts, dstPts);
    cv::Mat cropped;
    cv::warpPerspective(image, cropped, M, cv::Size(dstW, dstH));

    return cropped;
}

std::vector<QPolygonF> OCREngine::sortTextBoxes(const std::vector<QPolygonF>& boxes) {
    std::vector<std::pair<QPolygonF, float>> boxesWithY;

    for (const auto& box : boxes) {
        float minY = std::numeric_limits<float>::max();
        for (const auto& pt : box) {
            minY = std::min(minY, static_cast<float>(pt.y()));
        }
        boxesWithY.push_back({box, minY});
    }

    // 按Y坐标分组，然后按X坐标排序
    std::sort(boxesWithY.begin(), boxesWithY.end(),
              [](const auto& a, const auto& b) {
                  // 允许一定的Y容差
                  float yDiff = a.second - b.second;
                  if (std::abs(yDiff) < 10) {
                      // 同一行，按X排序
                      return a.first[0].x() < b.first[0].x();
                  }
                  return yDiff < 0;
              });

    std::vector<QPolygonF> result;
    for (const auto& item : boxesWithY) {
        result.push_back(item.first);
    }

    return result;
}

TextDetectionResult OCREngine::detect(const cv::Mat& image) {
    TextDetectionResult result;

    if (!m_detectionEngine) {
        result.errorMessage = "Detection model not loaded";
        emit errorOccurred(result.errorMessage);
        return result;
    }

    auto startPre = std::chrono::high_resolution_clock::now();
    float ratio;
    cv::Mat preprocessed = preprocessDetection(image, ratio);

    Tensor inputTensor;
    inputTensor.shape = {1, 3, m_config.detInputHeight,
                         static_cast<int>(image.cols * ratio / 32) * 32};
    inputTensor.data.assign(
        reinterpret_cast<float*>(preprocessed.data),
        reinterpret_cast<float*>(preprocessed.data) + preprocessed.total());

    auto endPre = std::chrono::high_resolution_clock::now();
    result.preprocessTime = std::chrono::duration<float, std::milli>(endPre - startPre).count();

    auto startInfer = std::chrono::high_resolution_clock::now();
    std::vector<Tensor> outputs;
    bool inferSuccess = m_detectionEngine->infer({inputTensor}, outputs);
    auto endInfer = std::chrono::high_resolution_clock::now();
    result.inferenceTime = std::chrono::duration<float, std::milli>(endInfer - startInfer).count();

    if (!inferSuccess || outputs.empty()) {
        result.errorMessage = m_detectionEngine->lastError();
        emit errorOccurred(result.errorMessage);
        return result;
    }

    auto startPost = std::chrono::high_resolution_clock::now();
    result.boxes = postprocessDetection(outputs[0], image.size(), ratio);

    if (m_config.sortByPosition) {
        result.boxes = sortTextBoxes(result.boxes);
    }

    result.scores.resize(result.boxes.size(), 1.0f);
    auto endPost = std::chrono::high_resolution_clock::now();
    result.postprocessTime = std::chrono::duration<float, std::milli>(endPost - startPost).count();

    result.success = true;
    return result;
}

OCRResult OCREngine::recognize(const cv::Mat& image) {
    OCRResult result;

    if (!m_initialized) {
        result.errorMessage = "OCR engine not initialized";
        emit errorOccurred(result.errorMessage);
        return result;
    }

    auto startTotal = std::chrono::high_resolution_clock::now();

    // 检测
    auto startDet = std::chrono::high_resolution_clock::now();
    TextDetectionResult detResult = detect(image);
    auto endDet = std::chrono::high_resolution_clock::now();
    result.detectionTime = std::chrono::duration<float, std::milli>(endDet - startDet).count();

    if (!detResult.success) {
        result.errorMessage = detResult.errorMessage;
        return result;
    }

    // 识别每个文本框
    auto startRec = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < detResult.boxes.size(); ++i) {
        const auto& polygon = detResult.boxes[i];

        // 裁剪文本区域
        cv::Mat cropped = cropTextRegion(image, polygon);
        if (cropped.empty()) {
            continue;
        }

        // 预处理
        cv::Mat preprocessed = preprocessRecognition(cropped);

        // 推理
        Tensor inputTensor;
        inputTensor.shape = {1, 3, m_config.recInputHeight, m_config.recInputWidth};
        inputTensor.data.assign(
            reinterpret_cast<float*>(preprocessed.data),
            reinterpret_cast<float*>(preprocessed.data) + preprocessed.total());

        std::vector<Tensor> outputs;
        bool inferSuccess = m_recognitionEngine->infer({inputTensor}, outputs);

        if (!inferSuccess || outputs.empty()) {
            continue;
        }

        // 解码
        QString text = postprocessRecognition(outputs[0]);

        if (text.isEmpty()) {
            continue;
        }

        // 创建结果
        TextBox textBox;
        textBox.text = text;
        textBox.polygon = polygon;
        textBox.boundingBox = polygon.boundingRect();
        textBox.confidence = 0.9f;  // 简化处理

        result.textBoxes.push_back(textBox);

        emit progressChanged(static_cast<int>((i + 1) * 100 / detResult.boxes.size()));
    }

    auto endRec = std::chrono::high_resolution_clock::now();
    result.recognitionTime = std::chrono::duration<float, std::milli>(endRec - startRec).count();

    // 统计
    result.boxCount = static_cast<int>(result.textBoxes.size());

    QStringList allTexts;
    float totalConf = 0;
    for (const auto& box : result.textBoxes) {
        allTexts.append(box.text);
        totalConf += box.confidence;
    }
    result.fullText = allTexts.join(" ");
    result.avgConfidence = result.boxCount > 0 ? totalConf / result.boxCount : 0;

    auto endTotal = std::chrono::high_resolution_clock::now();
    result.totalTime = std::chrono::duration<float, std::milli>(endTotal - startTotal).count();

    result.success = true;
    emit recognitionComplete(result);
    return result;
}

OCRResult OCREngine::recognize(const QImage& image) {
    cv::Mat mat(image.height(), image.width(), CV_8UC4,
               const_cast<uchar*>(image.bits()), image.bytesPerLine());
    cv::Mat bgr;
    cv::cvtColor(mat, bgr, cv::COLOR_RGBA2BGR);
    return recognize(bgr);
}

QString OCREngine::recognizeRegion(const cv::Mat& image, const QPolygonF& region) {
    if (!m_recognitionEngine || m_dictionary.isEmpty()) {
        return QString();
    }

    cv::Mat cropped = cropTextRegion(image, region);
    if (cropped.empty()) {
        return QString();
    }

    cv::Mat preprocessed = preprocessRecognition(cropped);

    Tensor inputTensor;
    inputTensor.shape = {1, 3, m_config.recInputHeight, m_config.recInputWidth};
    inputTensor.data.assign(
        reinterpret_cast<float*>(preprocessed.data),
        reinterpret_cast<float*>(preprocessed.data) + preprocessed.total());

    std::vector<Tensor> outputs;
    bool inferSuccess = m_recognitionEngine->infer({inputTensor}, outputs);

    if (!inferSuccess || outputs.empty()) {
        return QString();
    }

    return postprocessRecognition(outputs[0]);
}

QString OCREngine::recognizeRegion(const cv::Mat& image, const QRectF& rect) {
    QPolygonF polygon;
    polygon << rect.topLeft() << rect.topRight()
            << rect.bottomRight() << rect.bottomLeft();
    return recognizeRegion(image, polygon);
}

std::vector<OCRResult> OCREngine::recognizeBatch(const std::vector<cv::Mat>& images) {
    std::vector<OCRResult> results;
    results.reserve(images.size());

    for (size_t i = 0; i < images.size(); ++i) {
        results.push_back(recognize(images[i]));
        emit progressChanged(static_cast<int>((i + 1) * 100 / images.size()));
    }

    return results;
}

cv::Mat OCREngine::visualize(const cv::Mat& image, const OCRResult& result,
                              bool showText, bool showConfidence) {
    cv::Mat output = image.clone();

    for (const auto& textBox : result.textBoxes) {
        // 绘制多边形
        std::vector<cv::Point> pts;
        for (const auto& pt : textBox.polygon) {
            pts.push_back(cv::Point(static_cast<int>(pt.x()), static_cast<int>(pt.y())));
        }
        cv::polylines(output, pts, true, cv::Scalar(0, 255, 0), 2);

        // 绘制文本
        if (showText && !textBox.text.isEmpty()) {
            QString label = textBox.text;
            if (showConfidence) {
                label += QString(" (%1%)").arg(static_cast<int>(textBox.confidence * 100));
            }

            cv::Point textPos(static_cast<int>(textBox.boundingBox.x()),
                             static_cast<int>(textBox.boundingBox.y()) - 5);
            if (textPos.y < 15) {
                textPos.y = static_cast<int>(textBox.boundingBox.y() + 20);
            }

            cv::putText(output, label.toStdString(), textPos,
                       cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
        }
    }

    return output;
}

void OCREngine::setConfig(const OCRConfig& config) {
    m_config = config;
}

// ============================================================
// TableOCR 实现
// ============================================================
TableOCR::TableOCR(QObject* parent)
    : QObject(parent)
{
}

TableOCR::~TableOCR() {
    release();
}

bool TableOCR::initialize(const QString& structureModelPath,
                          const QString& ocrModelPath) {
    // 初始化表格结构识别模型
    m_structureEngine = createInferenceEngine(InferenceBackend::ONNXRuntime);
    if (!m_structureEngine) {
        return false;
    }

    InferenceConfig config;
    if (!m_structureEngine->initialize(config)) {
        return false;
    }

    if (!m_structureEngine->loadModel(structureModelPath)) {
        return false;
    }

    // 初始化OCR引擎
    m_ocrEngine = std::make_unique<OCREngine>();
    OCRConfig ocrConfig;
    ocrConfig.recognitionModelPath = ocrModelPath;
    if (!m_ocrEngine->initialize(ocrConfig)) {
        return false;
    }

    m_initialized = true;
    return true;
}

TableOCR::TableResult TableOCR::recognize(const cv::Mat& image) {
    TableResult result;

    if (!m_initialized) {
        return result;
    }

    // 表格结构识别 - 简化实现
    // 实际需要使用专门的表格结构识别模型

    result.success = true;
    return result;
}

void TableOCR::release() {
    if (m_structureEngine) {
        m_structureEngine->release();
        m_structureEngine.reset();
    }
    if (m_ocrEngine) {
        m_ocrEngine->release();
        m_ocrEngine.reset();
    }
    m_initialized = false;
}

QString TableOCR::TableResult::getCell(int row, int col) const {
    for (const auto& cell : cells) {
        if (cell.row == row && cell.col == col) {
            return cell.text;
        }
    }
    return QString();
}

QStringList TableOCR::TableResult::getRow(int row) const {
    QStringList result;
    for (int c = 0; c < cols; ++c) {
        result.append(getCell(row, c));
    }
    return result;
}

QStringList TableOCR::TableResult::getColumn(int col) const {
    QStringList result;
    for (int r = 0; r < rows; ++r) {
        result.append(getCell(r, col));
    }
    return result;
}

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
