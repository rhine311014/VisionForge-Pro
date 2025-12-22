/**
 * @file InstanceSegmentation.cpp
 * @brief 工业级实例分割模块实现
 */

#include "algorithm/ai/InstanceSegmentation.h"
#include "algorithm/ai/ONNXRuntimeEngine.h"
#include <opencv2/imgproc.hpp>
#include <QDebug>
#include <chrono>
#include <algorithm>
#include <cmath>

namespace VisionForge {
namespace Algorithm {
namespace AI {

// ============================================================
// SegmentationConfig 实现
// ============================================================
QJsonObject SegmentationConfig::toJson() const {
    QJsonObject json;
    json["modelType"] = static_cast<int>(modelType);
    json["modelPath"] = modelPath;
    json["confidenceThreshold"] = confidenceThreshold;
    json["nmsThreshold"] = nmsThreshold;
    json["maskThreshold"] = maskThreshold;
    json["inputWidth"] = inputWidth;
    json["inputHeight"] = inputHeight;
    json["letterBox"] = letterBox;
    json["refineMask"] = refineMask;
    json["refineMaskIterations"] = refineMaskIterations;
    json["extractContours"] = extractContours;
    json["contourApproxEpsilon"] = contourApproxEpsilon;
    json["minContourArea"] = minContourArea;
    json["minArea"] = static_cast<double>(minArea);
    json["maxArea"] = static_cast<double>(maxArea);
    json["backend"] = static_cast<int>(backend);
    json["device"] = static_cast<int>(device);

    QJsonArray classArr;
    for (const auto& name : classNames) {
        classArr.append(name);
    }
    json["classNames"] = classArr;

    QJsonArray targetArr;
    for (int cls : targetClasses) {
        targetArr.append(cls);
    }
    json["targetClasses"] = targetArr;

    return json;
}

SegmentationConfig SegmentationConfig::fromJson(const QJsonObject& json) {
    SegmentationConfig config;
    config.modelType = static_cast<SegmentationModel>(json["modelType"].toInt());
    config.modelPath = json["modelPath"].toString();
    config.confidenceThreshold = static_cast<float>(json["confidenceThreshold"].toDouble(0.5));
    config.nmsThreshold = static_cast<float>(json["nmsThreshold"].toDouble(0.45));
    config.maskThreshold = static_cast<float>(json["maskThreshold"].toDouble(0.5));
    config.inputWidth = json["inputWidth"].toInt(640);
    config.inputHeight = json["inputHeight"].toInt(640);
    config.letterBox = json["letterBox"].toBool(true);
    config.refineMask = json["refineMask"].toBool(true);
    config.refineMaskIterations = json["refineMaskIterations"].toInt(3);
    config.extractContours = json["extractContours"].toBool(true);
    config.contourApproxEpsilon = json["contourApproxEpsilon"].toDouble(2.0);
    config.minContourArea = json["minContourArea"].toInt(100);
    config.minArea = static_cast<float>(json["minArea"].toDouble(0.0));
    config.maxArea = static_cast<float>(json["maxArea"].toDouble(0.0));
    config.backend = static_cast<InferenceBackend>(json["backend"].toInt());
    config.device = static_cast<ExecutionDevice>(json["device"].toInt());

    QJsonArray classArr = json["classNames"].toArray();
    for (const auto& val : classArr) {
        config.classNames.append(val.toString());
    }

    QJsonArray targetArr = json["targetClasses"].toArray();
    for (const auto& val : targetArr) {
        config.targetClasses.push_back(val.toInt());
    }

    return config;
}

// ============================================================
// SegmentationResult 实现
// ============================================================
SegmentedInstance* SegmentationResult::getInstanceById(int id) {
    for (auto& instance : instances) {
        if (instance.instanceId == id) {
            return &instance;
        }
    }
    return nullptr;
}

std::vector<SegmentedInstance*> SegmentationResult::getInstancesByClass(int classId) {
    std::vector<SegmentedInstance*> result;
    for (auto& instance : instances) {
        if (instance.classId == classId) {
            result.push_back(&instance);
        }
    }
    return result;
}

cv::Mat SegmentationResult::getMaskByClass(int classId) const {
    if (combinedMask.empty()) {
        return cv::Mat();
    }

    cv::Mat classMask;
    cv::compare(combinedMask, classId, classMask, cv::CMP_EQ);
    return classMask;
}

// ============================================================
// InstanceSegmentor 实现
// ============================================================
InstanceSegmentor::InstanceSegmentor(QObject* parent)
    : QObject(parent)
{
    // 生成颜色调色板
    m_colorPalette = generateColorPalette(80);
}

InstanceSegmentor::~InstanceSegmentor() {
    release();
}

bool InstanceSegmentor::initialize(const SegmentationConfig& config) {
    m_config = config;
    m_lastError.clear();

    // 创建推理引擎
    m_engine = createInferenceEngine(config.backend);
    if (!m_engine) {
        m_lastError = "Failed to create inference engine";
        emit errorOccurred(m_lastError);
        return false;
    }

    // 配置引擎
    InferenceConfig engineConfig;
    engineConfig.device = config.device;
    engineConfig.backend = config.backend;

    if (!m_engine->initialize(engineConfig)) {
        m_lastError = m_engine->lastError();
        emit errorOccurred(m_lastError);
        return false;
    }

    // 加载模型
    if (!config.modelPath.isEmpty()) {
        if (!loadModel(config.modelPath)) {
            return false;
        }
    }

    m_initialized = true;
    return true;
}

bool InstanceSegmentor::loadModel(const QString& modelPath) {
    if (!m_engine) {
        m_lastError = "Engine not initialized";
        return false;
    }

    if (!m_engine->loadModel(modelPath)) {
        m_lastError = m_engine->lastError();
        emit errorOccurred(m_lastError);
        return false;
    }

    m_config.modelPath = modelPath;
    return true;
}

void InstanceSegmentor::release() {
    if (m_engine) {
        m_engine->release();
        m_engine.reset();
    }
    m_initialized = false;
}

cv::Mat InstanceSegmentor::preprocess(const cv::Mat& image, float& scaleX, float& scaleY,
                                       int& padLeft, int& padTop) {
    cv::Mat result;
    int targetW = m_config.inputWidth;
    int targetH = m_config.inputHeight;

    if (m_config.letterBox) {
        // Letterbox: 保持纵横比
        float scale = std::min(static_cast<float>(targetW) / image.cols,
                              static_cast<float>(targetH) / image.rows);
        int newW = static_cast<int>(image.cols * scale);
        int newH = static_cast<int>(image.rows * scale);

        cv::Mat resized;
        cv::resize(image, resized, cv::Size(newW, newH));

        // 计算填充
        padLeft = (targetW - newW) / 2;
        padTop = (targetH - newH) / 2;
        int padRight = targetW - newW - padLeft;
        int padBottom = targetH - newH - padTop;

        // 填充
        cv::copyMakeBorder(resized, result, padTop, padBottom, padLeft, padRight,
                          cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));

        scaleX = scale;
        scaleY = scale;
    } else {
        // 直接缩放
        cv::resize(image, result, cv::Size(targetW, targetH));
        scaleX = static_cast<float>(targetW) / image.cols;
        scaleY = static_cast<float>(targetH) / image.rows;
        padLeft = 0;
        padTop = 0;
    }

    // BGR -> RGB
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);

    // 归一化到0-1
    result.convertTo(result, CV_32FC3, 1.0 / 255.0);

    // HWC -> CHW
    cv::Mat channels[3];
    cv::split(result, channels);
    cv::Mat chw;
    cv::vconcat(std::vector<cv::Mat>{channels[0].reshape(1, 1),
                                      channels[1].reshape(1, 1),
                                      channels[2].reshape(1, 1)}, chw);

    return chw.reshape(1, {1, 3, targetH, targetW});
}

SegmentationResult InstanceSegmentor::segment(const cv::Mat& image) {
    SegmentationResult result;

    if (!m_initialized || !m_engine) {
        result.errorMessage = "Segmentor not initialized";
        emit errorOccurred(result.errorMessage);
        return result;
    }

    auto startTotal = std::chrono::high_resolution_clock::now();

    // 预处理
    auto startPre = std::chrono::high_resolution_clock::now();
    float scaleX, scaleY;
    int padLeft, padTop;
    cv::Mat preprocessed = preprocess(image, scaleX, scaleY, padLeft, padTop);

    Tensor inputTensor;
    inputTensor.shape = {1, 3, m_config.inputHeight, m_config.inputWidth};
    inputTensor.data.assign(reinterpret_cast<float*>(preprocessed.data),
                           reinterpret_cast<float*>(preprocessed.data) + preprocessed.total());

    auto endPre = std::chrono::high_resolution_clock::now();
    result.preprocessTime = std::chrono::duration<float, std::milli>(endPre - startPre).count();

    // 推理
    auto startInfer = std::chrono::high_resolution_clock::now();
    std::vector<Tensor> outputs;
    bool inferSuccess = m_engine->infer({inputTensor}, outputs);
    auto endInfer = std::chrono::high_resolution_clock::now();
    result.inferenceTime = std::chrono::duration<float, std::milli>(endInfer - startInfer).count();

    if (!inferSuccess || outputs.empty()) {
        result.errorMessage = m_engine->lastError();
        emit errorOccurred(result.errorMessage);
        return result;
    }

    // 后处理
    auto startPost = std::chrono::high_resolution_clock::now();

    switch (m_config.modelType) {
        case SegmentationModel::YOLOv8Seg:
        case SegmentationModel::YOLOv11Seg:
            result = postprocessYOLOSeg(outputs, image.size(), scaleX, scaleY, padLeft, padTop);
            break;
        case SegmentationModel::MaskRCNN:
            result = postprocessMaskRCNN(outputs, image.size());
            break;
        case SegmentationModel::SAM:
            result = postprocessSAM(outputs, image.size());
            break;
        default:
            result = postprocessYOLOSeg(outputs, image.size(), scaleX, scaleY, padLeft, padTop);
            break;
    }

    auto endPost = std::chrono::high_resolution_clock::now();
    result.postprocessTime = std::chrono::duration<float, std::milli>(endPost - startPost).count();

    auto endTotal = std::chrono::high_resolution_clock::now();
    result.totalTime = std::chrono::duration<float, std::milli>(endTotal - startTotal).count();

    result.success = true;
    emit segmentationComplete(result);
    return result;
}

SegmentationResult InstanceSegmentor::segment(const QImage& image) {
    cv::Mat mat(image.height(), image.width(), CV_8UC4,
               const_cast<uchar*>(image.bits()), image.bytesPerLine());
    cv::Mat bgr;
    cv::cvtColor(mat, bgr, cv::COLOR_RGBA2BGR);
    return segment(bgr);
}

std::vector<SegmentationResult> InstanceSegmentor::segmentBatch(const std::vector<cv::Mat>& images) {
    std::vector<SegmentationResult> results;
    results.reserve(images.size());

    for (size_t i = 0; i < images.size(); ++i) {
        results.push_back(segment(images[i]));
        emit progressChanged(static_cast<int>((i + 1) * 100 / images.size()));
    }

    return results;
}

SegmentationResult InstanceSegmentor::postprocessYOLOSeg(
    const std::vector<Tensor>& outputs,
    const cv::Size& originalSize,
    float scaleX, float scaleY,
    int padLeft, int padTop)
{
    SegmentationResult result;

    if (outputs.size() < 2) {
        result.errorMessage = "Invalid output count for YOLO segmentation";
        return result;
    }

    // YOLO seg输出: [1, num_classes+4+mask_coeffs, num_detections] 和 [1, mask_dims, H, W]
    const Tensor& detOutput = outputs[0];
    const Tensor& protoOutput = outputs[1];

    // 解析检测输出
    int numDetections = detOutput.shape.size() > 2 ? detOutput.shape[2] : 0;
    int numFeatures = detOutput.shape.size() > 1 ? detOutput.shape[1] : 0;

    if (numDetections == 0 || numFeatures < 5) {
        return result;
    }

    // 假设: 4(box) + numClasses + maskCoeffs
    int numClasses = m_config.classNames.size() > 0 ?
                     m_config.classNames.size() : 80;
    int maskCoeffs = numFeatures - 4 - numClasses;

    if (maskCoeffs <= 0) {
        maskCoeffs = 32; // 默认掩码系数数量
        numClasses = numFeatures - 4 - maskCoeffs;
    }

    // 解析prototype维度
    int protoH = protoOutput.shape.size() > 2 ? protoOutput.shape[2] : 160;
    int protoW = protoOutput.shape.size() > 3 ? protoOutput.shape[3] : 160;
    int protoDims = protoOutput.shape.size() > 1 ? protoOutput.shape[1] : 32;

    // 收集候选框
    struct Detection {
        float x, y, w, h;
        int classId;
        float score;
        std::vector<float> maskCoeffs;
    };
    std::vector<Detection> detections;

    for (int i = 0; i < numDetections; ++i) {
        // YOLOv8格式: [cx, cy, w, h, class_scores..., mask_coeffs...]
        float cx = detOutput.data[0 * numDetections + i];
        float cy = detOutput.data[1 * numDetections + i];
        float w = detOutput.data[2 * numDetections + i];
        float h = detOutput.data[3 * numDetections + i];

        // 找最高分类别
        int bestClass = 0;
        float bestScore = 0;
        for (int c = 0; c < numClasses; ++c) {
            float score = detOutput.data[(4 + c) * numDetections + i];
            if (score > bestScore) {
                bestScore = score;
                bestClass = c;
            }
        }

        if (bestScore < m_config.confidenceThreshold) {
            continue;
        }

        // 过滤目标类别
        if (!m_config.targetClasses.empty()) {
            if (std::find(m_config.targetClasses.begin(),
                         m_config.targetClasses.end(),
                         bestClass) == m_config.targetClasses.end()) {
                continue;
            }
        }

        Detection det;
        det.x = cx;
        det.y = cy;
        det.w = w;
        det.h = h;
        det.classId = bestClass;
        det.score = bestScore;

        // 提取掩码系数
        det.maskCoeffs.resize(maskCoeffs);
        for (int m = 0; m < maskCoeffs; ++m) {
            det.maskCoeffs[m] = detOutput.data[(4 + numClasses + m) * numDetections + i];
        }

        detections.push_back(det);
    }

    // NMS
    std::vector<int> indices;
    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    for (const auto& det : detections) {
        boxes.push_back(cv::Rect(
            static_cast<int>(det.x - det.w / 2),
            static_cast<int>(det.y - det.h / 2),
            static_cast<int>(det.w),
            static_cast<int>(det.h)
        ));
        scores.push_back(det.score);
    }
    cv::dnn::NMSBoxes(boxes, scores, m_config.confidenceThreshold,
                      m_config.nmsThreshold, indices);

    // 创建prototype矩阵
    cv::Mat prototypes(protoDims, protoH * protoW, CV_32FC1,
                       const_cast<float*>(protoOutput.data.data()));

    // 生成合并掩码
    result.combinedMask = cv::Mat::zeros(originalSize, CV_8UC1);
    result.colorMask = cv::Mat::zeros(originalSize, CV_8UC3);

    int instanceId = 0;
    for (int idx : indices) {
        const Detection& det = detections[idx];

        SegmentedInstance instance;
        instance.instanceId = instanceId++;
        instance.classId = det.classId;
        instance.confidence = det.score;

        if (det.classId < m_config.classNames.size()) {
            instance.className = m_config.classNames[det.classId];
        }

        // 转换坐标到原始图像
        float x1 = (det.x - det.w / 2 - padLeft) / scaleX;
        float y1 = (det.y - det.h / 2 - padTop) / scaleY;
        float x2 = (det.x + det.w / 2 - padLeft) / scaleX;
        float y2 = (det.y + det.h / 2 - padTop) / scaleY;

        x1 = std::max(0.0f, std::min(x1, static_cast<float>(originalSize.width - 1)));
        y1 = std::max(0.0f, std::min(y1, static_cast<float>(originalSize.height - 1)));
        x2 = std::max(0.0f, std::min(x2, static_cast<float>(originalSize.width)));
        y2 = std::max(0.0f, std::min(y2, static_cast<float>(originalSize.height)));

        instance.boundingBox = QRectF(x1, y1, x2 - x1, y2 - y1);

        // 生成掩码
        cv::Mat coeffMat(1, maskCoeffs, CV_32FC1, const_cast<float*>(det.maskCoeffs.data()));
        cv::Mat maskOutput = coeffMat * prototypes;
        maskOutput = maskOutput.reshape(1, protoH);

        // Sigmoid
        cv::exp(-maskOutput, maskOutput);
        maskOutput = 1.0 / (1.0 + maskOutput);

        // 缩放到原图大小
        cv::Mat fullMask;
        cv::resize(maskOutput, fullMask, originalSize);

        // 裁剪到边界框
        int boxX1 = static_cast<int>(x1);
        int boxY1 = static_cast<int>(y1);
        int boxX2 = static_cast<int>(x2);
        int boxY2 = static_cast<int>(y2);

        cv::Mat binaryMask = cv::Mat::zeros(originalSize, CV_8UC1);
        cv::Rect roi(boxX1, boxY1, boxX2 - boxX1, boxY2 - boxY1);
        if (roi.width > 0 && roi.height > 0 &&
            roi.x >= 0 && roi.y >= 0 &&
            roi.x + roi.width <= originalSize.width &&
            roi.y + roi.height <= originalSize.height) {

            cv::Mat roiMask = fullMask(roi);
            cv::Mat roiBinary;
            cv::threshold(roiMask, roiBinary, m_config.maskThreshold, 255, cv::THRESH_BINARY);
            roiBinary.convertTo(binaryMask(roi), CV_8UC1);
        }

        instance.mask = binaryMask;

        // 计算面积
        instance.area = static_cast<float>(cv::countNonZero(instance.mask));

        // 面积过滤
        if (m_config.minArea > 0 && instance.area < m_config.minArea) {
            continue;
        }
        if (m_config.maxArea > 0 && instance.area > m_config.maxArea) {
            continue;
        }

        // 提取轮廓
        if (m_config.extractContours) {
            instance.contours = extractContours(instance.mask, m_config.contourApproxEpsilon);
        }

        // 计算几何属性
        computeGeometricProperties(instance);

        // 更新合并掩码
        result.combinedMask.setTo(det.classId + 1, instance.mask);

        // 更新彩色掩码
        cv::Vec3b color = getClassColor(det.classId);
        result.colorMask.setTo(cv::Scalar(color[0], color[1], color[2]), instance.mask);

        result.instances.push_back(instance);
        result.classCount[det.classId]++;
    }

    result.instanceCount = static_cast<int>(result.instances.size());
    result.success = true;
    return result;
}

SegmentationResult InstanceSegmentor::postprocessMaskRCNN(
    const std::vector<Tensor>& outputs,
    const cv::Size& originalSize)
{
    SegmentationResult result;

    // Mask R-CNN通常有3个输出: boxes, labels, masks
    if (outputs.size() < 3) {
        result.errorMessage = "Invalid output count for Mask R-CNN";
        return result;
    }

    // 简化实现 - 实际需要根据具体模型调整
    result.success = true;
    result.errorMessage = "Mask R-CNN postprocessing not fully implemented";
    return result;
}

SegmentationResult InstanceSegmentor::postprocessSAM(
    const std::vector<Tensor>& outputs,
    const cv::Size& originalSize)
{
    SegmentationResult result;

    // SAM输出处理
    if (outputs.empty()) {
        result.errorMessage = "Empty SAM output";
        return result;
    }

    // 简化实现
    result.success = true;
    result.errorMessage = "SAM postprocessing not fully implemented";
    return result;
}

SegmentationResult InstanceSegmentor::segmentWithPoints(
    const cv::Mat& image,
    const std::vector<QPointF>& points,
    const std::vector<int>& labels)
{
    SegmentationResult result;

    if (m_config.modelType != SegmentationModel::SAM) {
        result.errorMessage = "Point prompts only supported for SAM";
        return result;
    }

    // SAM点提示分割实现
    result.errorMessage = "SAM point prompt not yet implemented";
    return result;
}

SegmentationResult InstanceSegmentor::segmentWithBox(
    const cv::Mat& image,
    const QRectF& box)
{
    SegmentationResult result;

    if (m_config.modelType != SegmentationModel::SAM) {
        result.errorMessage = "Box prompts only supported for SAM";
        return result;
    }

    // SAM框提示分割实现
    result.errorMessage = "SAM box prompt not yet implemented";
    return result;
}

cv::Mat InstanceSegmentor::refineMaskWithGrabCut(
    const cv::Mat& image,
    const cv::Mat& mask,
    int iterations)
{
    if (image.empty() || mask.empty()) {
        return mask.clone();
    }

    // 准备GrabCut掩码
    cv::Mat gcMask = cv::Mat::zeros(image.size(), CV_8UC1);
    gcMask.setTo(cv::GC_PR_BGD);  // 可能是背景
    gcMask.setTo(cv::GC_PR_FGD, mask);  // 可能是前景

    // 边界区域设为明确前景/背景
    cv::Mat eroded, dilated;
    cv::erode(mask, eroded, cv::Mat(), cv::Point(-1, -1), 3);
    cv::dilate(mask, dilated, cv::Mat(), cv::Point(-1, -1), 3);

    gcMask.setTo(cv::GC_FGD, eroded);  // 明确前景
    gcMask.setTo(cv::GC_BGD, ~dilated);  // 明确背景

    // 运行GrabCut
    cv::Mat bgdModel, fgdModel;
    try {
        cv::grabCut(image, gcMask, cv::Rect(), bgdModel, fgdModel,
                    iterations, cv::GC_INIT_WITH_MASK);
    } catch (const cv::Exception& e) {
        qWarning() << "GrabCut failed:" << e.what();
        return mask.clone();
    }

    // 提取前景
    cv::Mat refinedMask;
    cv::compare(gcMask, cv::GC_FGD, refinedMask, cv::CMP_EQ);
    cv::Mat prFgd;
    cv::compare(gcMask, cv::GC_PR_FGD, prFgd, cv::CMP_EQ);
    refinedMask = refinedMask | prFgd;

    return refinedMask;
}

std::vector<QPolygonF> InstanceSegmentor::extractContours(
    const cv::Mat& mask,
    double approxEpsilon)
{
    std::vector<QPolygonF> result;

    if (mask.empty()) {
        return result;
    }

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, approxEpsilon, true);

        QPolygonF polygon;
        for (const auto& pt : approx) {
            polygon.append(QPointF(pt.x, pt.y));
        }
        result.push_back(polygon);
    }

    return result;
}

void InstanceSegmentor::computeGeometricProperties(SegmentedInstance& instance) {
    if (instance.mask.empty()) {
        return;
    }

    // 找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(instance.mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        return;
    }

    // 使用最大轮廓
    size_t maxIdx = 0;
    double maxArea = 0;
    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea) {
            maxArea = area;
            maxIdx = i;
        }
    }

    const auto& contour = contours[maxIdx];

    // 面积和周长
    instance.area = static_cast<float>(cv::contourArea(contour));
    instance.perimeter = static_cast<float>(cv::arcLength(contour, true));

    // 质心
    cv::Moments moments = cv::moments(contour);
    if (moments.m00 != 0) {
        instance.centroid = QPointF(moments.m10 / moments.m00, moments.m01 / moments.m00);
    }

    // 旋转矩形
    if (contour.size() >= 5) {
        cv::RotatedRect rotRect = cv::minAreaRect(contour);
        instance.rotatedCenter = QPointF(rotRect.center.x, rotRect.center.y);
        instance.rotatedSize = QSizeF(rotRect.size.width, rotRect.size.height);
        instance.rotatedAngle = rotRect.angle;

        // 长宽比
        float minSide = std::min(rotRect.size.width, rotRect.size.height);
        float maxSide = std::max(rotRect.size.width, rotRect.size.height);
        instance.aspectRatio = (minSide > 0) ? (maxSide / minSide) : 0;
    }

    // 实心度
    std::vector<cv::Point> hull;
    cv::convexHull(contour, hull);
    double hullArea = cv::contourArea(hull);
    instance.solidity = (hullArea > 0) ? static_cast<float>(instance.area / hullArea) : 0;

    // 圆形度
    if (instance.perimeter > 0) {
        instance.circularity = static_cast<float>(
            4 * CV_PI * instance.area / (instance.perimeter * instance.perimeter));
    }
}

cv::Mat InstanceSegmentor::visualize(
    const cv::Mat& image,
    const SegmentationResult& result,
    float maskAlpha,
    bool showContours,
    bool showLabels)
{
    cv::Mat output = image.clone();

    // 叠加彩色掩码
    if (!result.colorMask.empty()) {
        cv::addWeighted(output, 1.0 - maskAlpha, result.colorMask, maskAlpha, 0, output);
    }

    for (const auto& instance : result.instances) {
        cv::Vec3b color = getClassColor(instance.classId);
        cv::Scalar colorScalar(color[0], color[1], color[2]);

        // 绘制轮廓
        if (showContours) {
            for (const auto& contour : instance.contours) {
                std::vector<cv::Point> cvContour;
                for (const auto& pt : contour) {
                    cvContour.push_back(cv::Point(static_cast<int>(pt.x()),
                                                  static_cast<int>(pt.y())));
                }
                std::vector<std::vector<cv::Point>> contours = {cvContour};
                cv::drawContours(output, contours, 0, colorScalar, 2);
            }
        }

        // 绘制标签
        if (showLabels) {
            QString label = QString("%1: %2%")
                .arg(instance.className.isEmpty() ?
                     QString::number(instance.classId) : instance.className)
                .arg(static_cast<int>(instance.confidence * 100));

            cv::Point labelPos(static_cast<int>(instance.boundingBox.x()),
                              static_cast<int>(instance.boundingBox.y()) - 5);
            if (labelPos.y < 15) {
                labelPos.y = static_cast<int>(instance.boundingBox.y() + 15);
            }

            cv::putText(output, label.toStdString(), labelPos,
                       cv::FONT_HERSHEY_SIMPLEX, 0.5, colorScalar, 1);
        }
    }

    return output;
}

void InstanceSegmentor::setConfig(const SegmentationConfig& config) {
    m_config = config;
}

cv::Vec3b InstanceSegmentor::getClassColor(int classId) {
    // 使用HSV生成区分度好的颜色
    float hue = (classId * 137.5f);  // 黄金角度
    while (hue >= 360.0f) hue -= 360.0f;

    // HSV to RGB
    float s = 0.8f;
    float v = 0.9f;
    float c = v * s;
    float x = c * (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if (hue < 60) { r = c; g = x; b = 0; }
    else if (hue < 120) { r = x; g = c; b = 0; }
    else if (hue < 180) { r = 0; g = c; b = x; }
    else if (hue < 240) { r = 0; g = x; b = c; }
    else if (hue < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    return cv::Vec3b(
        static_cast<uchar>((b + m) * 255),
        static_cast<uchar>((g + m) * 255),
        static_cast<uchar>((r + m) * 255)
    );
}

std::vector<cv::Vec3b> InstanceSegmentor::generateColorPalette(int numClasses) {
    std::vector<cv::Vec3b> palette;
    palette.reserve(numClasses);
    for (int i = 0; i < numClasses; ++i) {
        palette.push_back(getClassColor(i));
    }
    return palette;
}

// ============================================================
// SemanticSegmentor 实现
// ============================================================
QJsonObject SemanticSegmentor::Config::toJson() const {
    QJsonObject json;
    json["modelPath"] = modelPath;
    json["inputWidth"] = inputWidth;
    json["inputHeight"] = inputHeight;
    json["backend"] = static_cast<int>(backend);
    json["device"] = static_cast<int>(device);

    QJsonArray classArr;
    for (const auto& name : classNames) {
        classArr.append(name);
    }
    json["classNames"] = classArr;

    return json;
}

SemanticSegmentor::Config SemanticSegmentor::Config::fromJson(const QJsonObject& json) {
    Config config;
    config.modelPath = json["modelPath"].toString();
    config.inputWidth = json["inputWidth"].toInt(512);
    config.inputHeight = json["inputHeight"].toInt(512);
    config.backend = static_cast<InferenceBackend>(json["backend"].toInt());
    config.device = static_cast<ExecutionDevice>(json["device"].toInt());

    QJsonArray classArr = json["classNames"].toArray();
    for (const auto& val : classArr) {
        config.classNames.append(val.toString());
    }

    return config;
}

namespace {
// 辅助函数：为语义分割生成颜色调色板
std::vector<cv::Vec3b> generateSemanticColorPalette(int numClasses) {
    std::vector<cv::Vec3b> palette;
    palette.reserve(numClasses);
    for (int i = 0; i < numClasses; ++i) {
        float hue = (i * 360.0f / numClasses);
        float saturation = 0.8f;
        float value = 0.9f;

        float c = value * saturation;
        float x = c * (1 - std::abs(std::fmod(hue / 60.0f, 2) - 1));
        float m = value - c;
        float r, g, b;
        if (hue < 60) { r = c; g = x; b = 0; }
        else if (hue < 120) { r = x; g = c; b = 0; }
        else if (hue < 180) { r = 0; g = c; b = x; }
        else if (hue < 240) { r = 0; g = x; b = c; }
        else if (hue < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }
        palette.push_back(cv::Vec3b(
            static_cast<uchar>((b + m) * 255),
            static_cast<uchar>((g + m) * 255),
            static_cast<uchar>((r + m) * 255)
        ));
    }
    return palette;
}
} // anonymous namespace

SemanticSegmentor::SemanticSegmentor(QObject* parent)
    : QObject(parent)
{
    m_colorPalette = generateSemanticColorPalette(256);
}

SemanticSegmentor::~SemanticSegmentor() {
    release();
}

bool SemanticSegmentor::initialize(const Config& config) {
    m_config = config;

    m_engine = createInferenceEngine(config.backend);
    if (!m_engine) {
        return false;
    }

    InferenceConfig engineConfig;
    engineConfig.device = config.device;
    engineConfig.backend = config.backend;

    if (!m_engine->initialize(engineConfig)) {
        return false;
    }

    if (!config.modelPath.isEmpty()) {
        if (!m_engine->loadModel(config.modelPath)) {
            return false;
        }
    }

    m_initialized = true;
    return true;
}

void SemanticSegmentor::release() {
    if (m_engine) {
        m_engine->release();
        m_engine.reset();
    }
    m_initialized = false;
}

cv::Mat SemanticSegmentor::preprocess(const cv::Mat& image) {
    cv::Mat result;
    cv::resize(image, result, cv::Size(m_config.inputWidth, m_config.inputHeight));
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    result.convertTo(result, CV_32FC3, 1.0 / 255.0);

    // 标准化 (ImageNet均值和标准差)
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

SemanticSegmentor::Result SemanticSegmentor::postprocess(
    const Tensor& output,
    const cv::Size& originalSize)
{
    Result result;

    // 输出形状: [1, numClasses, H, W]
    if (output.shape.size() < 4) {
        result.errorMessage = "Invalid output shape";
        return result;
    }

    int numClasses = output.shape[1];
    int outH = output.shape[2];
    int outW = output.shape[3];

    // 找每个像素的最大类别
    cv::Mat segMap(outH, outW, CV_8UC1);
    cv::Mat confMap(outH, outW, CV_32FC1);

    for (int y = 0; y < outH; ++y) {
        for (int x = 0; x < outW; ++x) {
            int bestClass = 0;
            float bestScore = -std::numeric_limits<float>::max();

            for (int c = 0; c < numClasses; ++c) {
                float score = output.data[c * outH * outW + y * outW + x];
                if (score > bestScore) {
                    bestScore = score;
                    bestClass = c;
                }
            }

            segMap.at<uchar>(y, x) = static_cast<uchar>(bestClass);
            confMap.at<float>(y, x) = bestScore;
        }
    }

    // 缩放到原始尺寸
    cv::resize(segMap, result.segmentationMap, originalSize, 0, 0, cv::INTER_NEAREST);
    cv::resize(confMap, result.confidenceMap, originalSize);

    // 生成彩色图
    result.colorMap = cv::Mat(originalSize, CV_8UC3);
    for (int y = 0; y < originalSize.height; ++y) {
        for (int x = 0; x < originalSize.width; ++x) {
            int classId = result.segmentationMap.at<uchar>(y, x);
            result.colorMap.at<cv::Vec3b>(y, x) = m_colorPalette[classId % m_colorPalette.size()];
        }
    }

    // 计算类别占比
    int totalPixels = originalSize.width * originalSize.height;
    for (int c = 0; c < numClasses; ++c) {
        cv::Mat classMask;
        cv::compare(result.segmentationMap, c, classMask, cv::CMP_EQ);
        int count = cv::countNonZero(classMask);
        if (count > 0) {
            result.classRatios[c] = static_cast<float>(count) / totalPixels;
        }
    }

    result.success = true;
    return result;
}

SemanticSegmentor::Result SemanticSegmentor::segment(const cv::Mat& image) {
    Result result;

    if (!m_initialized || !m_engine) {
        result.errorMessage = "Segmentor not initialized";
        return result;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // 预处理
    cv::Mat preprocessed = preprocess(image);

    Tensor inputTensor;
    inputTensor.shape = {1, 3, m_config.inputHeight, m_config.inputWidth};
    inputTensor.data.assign(
        reinterpret_cast<float*>(preprocessed.data),
        reinterpret_cast<float*>(preprocessed.data) + preprocessed.total());

    // 推理
    std::vector<Tensor> outputs;
    bool inferSuccess = m_engine->infer({inputTensor}, outputs);

    if (!inferSuccess || outputs.empty()) {
        result.errorMessage = m_engine->lastError();
        return result;
    }

    // 后处理
    result = postprocess(outputs[0], image.size());

    auto end = std::chrono::high_resolution_clock::now();
    result.inferenceTime = std::chrono::duration<float, std::milli>(end - start).count();

    return result;
}

SemanticSegmentor::Result SemanticSegmentor::segment(const QImage& image) {
    cv::Mat mat(image.height(), image.width(), CV_8UC4,
               const_cast<uchar*>(image.bits()), image.bytesPerLine());
    cv::Mat bgr;
    cv::cvtColor(mat, bgr, cv::COLOR_RGBA2BGR);
    return segment(bgr);
}

cv::Mat SemanticSegmentor::visualize(const cv::Mat& image, const Result& result, float alpha) {
    cv::Mat output;
    cv::addWeighted(image, 1.0 - alpha, result.colorMap, alpha, 0, output);
    return output;
}

} // namespace AI
} // namespace Algorithm
} // namespace VisionForge
