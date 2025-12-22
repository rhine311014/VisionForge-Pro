/**
 * @file QRCodeCalibration.cpp
 * @brief VisionForge Pro QR码/DM码自动标定模块实现
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#include "calibration/QRCodeCalibration.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <algorithm>
#include <cmath>

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

// 微信QR码检测器（如果可用）
#ifdef HAVE_OPENCV_WECHAT_QRCODE
#include <opencv2/wechat_qrcode.hpp>
#endif

namespace VisionForge {
namespace Calibration {

// ============================================================
// Code2DResult 实现
// ============================================================

std::vector<CalibrationPoint> Code2DResult::toCalibrationPoints(
    const std::array<QPointF, 4>& worldCorners) const
{
    std::vector<CalibrationPoint> points;
    points.reserve(4);

    for (int i = 0; i < 4; ++i) {
        CalibrationPoint pt(corners[i], worldCorners[i], index * 4 + i);
        pt.isValid = true;
        pt.isEnabled = true;
        points.push_back(pt);
    }

    return points;
}

QPointF Code2DResult::calculatePhysicalSize(const std::array<QPointF, 4>& worldCorners) const
{
    // 计算宽度（上边和下边的平均）
    double topWidth = std::sqrt(
        std::pow(worldCorners[1].x() - worldCorners[0].x(), 2) +
        std::pow(worldCorners[1].y() - worldCorners[0].y(), 2));
    double bottomWidth = std::sqrt(
        std::pow(worldCorners[2].x() - worldCorners[3].x(), 2) +
        std::pow(worldCorners[2].y() - worldCorners[3].y(), 2));
    double width = (topWidth + bottomWidth) / 2.0;

    // 计算高度（左边和右边的平均）
    double leftHeight = std::sqrt(
        std::pow(worldCorners[3].x() - worldCorners[0].x(), 2) +
        std::pow(worldCorners[3].y() - worldCorners[0].y(), 2));
    double rightHeight = std::sqrt(
        std::pow(worldCorners[2].x() - worldCorners[1].x(), 2) +
        std::pow(worldCorners[2].y() - worldCorners[1].y(), 2));
    double height = (leftHeight + rightHeight) / 2.0;

    return QPointF(width, height);
}

QJsonObject Code2DResult::toJson() const
{
    QJsonObject json;
    json["type"] = static_cast<int>(type);
    json["content"] = content;
    json["decoded"] = decoded;

    // 边界矩形
    QJsonObject rectJson;
    rectJson["x"] = boundingRect.x();
    rectJson["y"] = boundingRect.y();
    rectJson["width"] = boundingRect.width();
    rectJson["height"] = boundingRect.height();
    json["boundingRect"] = rectJson;

    // 中心点
    QJsonObject centerJson;
    centerJson["x"] = center.x();
    centerJson["y"] = center.y();
    json["center"] = centerJson;

    // 角点
    QJsonArray cornersArray;
    for (const auto& corner : corners) {
        QJsonObject cornerJson;
        cornerJson["x"] = corner.x();
        cornerJson["y"] = corner.y();
        cornersArray.append(cornerJson);
    }
    json["corners"] = cornersArray;

    json["confidence"] = confidence;
    json["angle"] = angle;
    json["moduleSize"] = moduleSize;
    json["version"] = version;
    json["index"] = index;

    return json;
}

Code2DResult Code2DResult::fromJson(const QJsonObject& json)
{
    Code2DResult result;
    result.type = static_cast<Code2DType>(json["type"].toInt());
    result.content = json["content"].toString();
    result.decoded = json["decoded"].toBool();

    // 边界矩形
    QJsonObject rectJson = json["boundingRect"].toObject();
    result.boundingRect = QRectF(
        rectJson["x"].toDouble(),
        rectJson["y"].toDouble(),
        rectJson["width"].toDouble(),
        rectJson["height"].toDouble());

    // 中心点
    QJsonObject centerJson = json["center"].toObject();
    result.center = QPointF(centerJson["x"].toDouble(), centerJson["y"].toDouble());

    // 角点
    QJsonArray cornersArray = json["corners"].toArray();
    for (int i = 0; i < 4 && i < cornersArray.size(); ++i) {
        QJsonObject cornerJson = cornersArray[i].toObject();
        result.corners[i] = QPointF(cornerJson["x"].toDouble(), cornerJson["y"].toDouble());
    }

    result.confidence = json["confidence"].toDouble();
    result.angle = json["angle"].toDouble();
    result.moduleSize = json["moduleSize"].toInt();
    result.version = json["version"].toInt();
    result.index = json["index"].toInt();

    return result;
}

QString Code2DResult::toString() const
{
    return QString("Code2DResult[type=%1, content='%2', center=(%3,%4), confidence=%5]")
        .arg(getCode2DTypeName(type))
        .arg(content.left(20))
        .arg(center.x(), 0, 'f', 1)
        .arg(center.y(), 0, 'f', 1)
        .arg(confidence, 0, 'f', 2);
}

// ============================================================
// Code2DDetectionConfig 实现
// ============================================================

QJsonObject Code2DDetectionConfig::toJson() const
{
    QJsonObject json;
    json["expectedType"] = static_cast<int>(expectedType);
    json["maxCodes"] = maxCodes;
    json["decodeContent"] = decodeContent;
    json["enablePreprocessing"] = enablePreprocessing;
    json["blurKernelSize"] = blurKernelSize;
    json["contrastEnhancement"] = contrastEnhancement;
    json["adaptiveThreshold"] = adaptiveThreshold;
    json["refineCorners"] = refineCorners;
    json["refineWindowSize"] = refineWindowSize;
    json["refineEpsilon"] = refineEpsilon;
    json["refineMaxIterations"] = refineMaxIterations;
    json["minConfidence"] = minConfidence;
    json["minModuleSize"] = minModuleSize;
    json["maxModuleSize"] = maxModuleSize;

    if (!regionOfInterest.isNull()) {
        QJsonObject roiJson;
        roiJson["x"] = regionOfInterest.x();
        roiJson["y"] = regionOfInterest.y();
        roiJson["width"] = regionOfInterest.width();
        roiJson["height"] = regionOfInterest.height();
        json["regionOfInterest"] = roiJson;
    }

    return json;
}

Code2DDetectionConfig Code2DDetectionConfig::fromJson(const QJsonObject& json)
{
    Code2DDetectionConfig config;
    config.expectedType = static_cast<Code2DType>(json["expectedType"].toInt());
    config.maxCodes = json["maxCodes"].toInt(10);
    config.decodeContent = json["decodeContent"].toBool(true);
    config.enablePreprocessing = json["enablePreprocessing"].toBool(true);
    config.blurKernelSize = json["blurKernelSize"].toInt(3);
    config.contrastEnhancement = json["contrastEnhancement"].toDouble(1.0);
    config.adaptiveThreshold = json["adaptiveThreshold"].toBool(true);
    config.refineCorners = json["refineCorners"].toBool(true);
    config.refineWindowSize = json["refineWindowSize"].toInt(11);
    config.refineEpsilon = json["refineEpsilon"].toDouble(0.01);
    config.refineMaxIterations = json["refineMaxIterations"].toInt(30);
    config.minConfidence = json["minConfidence"].toDouble(0.6);
    config.minModuleSize = json["minModuleSize"].toInt(3);
    config.maxModuleSize = json["maxModuleSize"].toInt(100);

    if (json.contains("regionOfInterest")) {
        QJsonObject roiJson = json["regionOfInterest"].toObject();
        config.regionOfInterest = QRectF(
            roiJson["x"].toDouble(),
            roiJson["y"].toDouble(),
            roiJson["width"].toDouble(),
            roiJson["height"].toDouble());
    }

    return config;
}

// ============================================================
// CalibrationBoardConfig 实现
// ============================================================

std::array<QPointF, 4> CalibrationBoardConfig::getCodeWorldCorners(int row, int col) const
{
    std::array<QPointF, 4> corners;

    // 计算码中心位置
    double centerX = originOffset.x() + col * codeSpacingX;
    double centerY = originOffset.y() + row * codeSpacingY;

    // 半边长
    double halfSize = codeSize / 2.0;

    // 根据角点顺序生成角点（顺时针：左上、右上、右下、左下）
    switch (cornerOrder) {
    case CornerOrder::TopLeftFirst:
    default:
        corners[0] = QPointF(centerX - halfSize, centerY - halfSize);  // 左上
        corners[1] = QPointF(centerX + halfSize, centerY - halfSize);  // 右上
        corners[2] = QPointF(centerX + halfSize, centerY + halfSize);  // 右下
        corners[3] = QPointF(centerX - halfSize, centerY + halfSize);  // 左下
        break;

    case CornerOrder::BottomLeftFirst:
        corners[0] = QPointF(centerX - halfSize, centerY + halfSize);  // 左下
        corners[1] = QPointF(centerX - halfSize, centerY - halfSize);  // 左上
        corners[2] = QPointF(centerX + halfSize, centerY - halfSize);  // 右上
        corners[3] = QPointF(centerX + halfSize, centerY + halfSize);  // 右下
        break;

    case CornerOrder::CenterOutward:
        // 从中心开始，按象限顺序
        corners[0] = QPointF(centerX - halfSize, centerY - halfSize);
        corners[1] = QPointF(centerX + halfSize, centerY - halfSize);
        corners[2] = QPointF(centerX + halfSize, centerY + halfSize);
        corners[3] = QPointF(centerX - halfSize, centerY + halfSize);
        break;
    }

    return corners;
}

std::vector<std::array<QPointF, 4>> CalibrationBoardConfig::getAllCodeWorldCorners() const
{
    std::vector<std::array<QPointF, 4>> allCorners;
    allCorners.reserve(gridRows * gridCols);

    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {
            allCorners.push_back(getCodeWorldCorners(row, col));
        }
    }

    return allCorners;
}

QJsonObject CalibrationBoardConfig::toJson() const
{
    QJsonObject json;
    json["layout"] = static_cast<int>(layout);
    json["gridRows"] = gridRows;
    json["gridCols"] = gridCols;
    json["codeSize"] = codeSize;
    json["codeSpacingX"] = codeSpacingX;
    json["codeSpacingY"] = codeSpacingY;

    QJsonObject originJson;
    originJson["x"] = originOffset.x();
    originJson["y"] = originOffset.y();
    json["originOffset"] = originJson;

    json["cornerOrder"] = static_cast<int>(cornerOrder);
    json["validateContent"] = validateContent;

    QJsonArray contentsArray;
    for (const auto& content : expectedContents) {
        contentsArray.append(content);
    }
    json["expectedContents"] = contentsArray;

    return json;
}

CalibrationBoardConfig CalibrationBoardConfig::fromJson(const QJsonObject& json)
{
    CalibrationBoardConfig config;
    config.layout = static_cast<CalibrationBoardLayout>(json["layout"].toInt());
    config.gridRows = json["gridRows"].toInt(1);
    config.gridCols = json["gridCols"].toInt(1);
    config.codeSize = json["codeSize"].toDouble(10.0);
    config.codeSpacingX = json["codeSpacingX"].toDouble(20.0);
    config.codeSpacingY = json["codeSpacingY"].toDouble(20.0);

    if (json.contains("originOffset")) {
        QJsonObject originJson = json["originOffset"].toObject();
        config.originOffset = QPointF(originJson["x"].toDouble(), originJson["y"].toDouble());
    }

    config.cornerOrder = static_cast<CornerOrder>(json["cornerOrder"].toInt());
    config.validateContent = json["validateContent"].toBool(false);

    QJsonArray contentsArray = json["expectedContents"].toArray();
    for (const auto& content : contentsArray) {
        config.expectedContents.append(content.toString());
    }

    return config;
}

// ============================================================
// QRCodeCalibration 实现
// ============================================================

QRCodeCalibration::QRCodeCalibration(QObject* parent)
    : QObject(parent)
    , m_calibrationManager(std::make_unique<CalibrationManager>())
{
    // 默认配置
    m_detectionConfig.expectedType = Code2DType::Auto;
    m_detectionConfig.maxCodes = 16;
    m_detectionConfig.refineCorners = true;

    m_boardConfig.layout = CalibrationBoardLayout::Single;
    m_boardConfig.gridRows = 1;
    m_boardConfig.gridCols = 1;
    m_boardConfig.codeSize = 10.0;

    // 设置CalibrationManager使用DMCode类型
    CalibrationConfig calConfig;
    calConfig.type = CalibrationType::DMCode;
    m_calibrationManager->setConfig(calConfig);
}

QRCodeCalibration::~QRCodeCalibration() = default;

// ============================================================
// 配置
// ============================================================

void QRCodeCalibration::setDetectionConfig(const Code2DDetectionConfig& config)
{
    m_detectionConfig = config;
}

void QRCodeCalibration::setBoardConfig(const CalibrationBoardConfig& config)
{
    m_boardConfig = config;

    // 更新布局相关的行列数
    switch (config.layout) {
    case CalibrationBoardLayout::Single:
        m_boardConfig.gridRows = 1;
        m_boardConfig.gridCols = 1;
        break;
    case CalibrationBoardLayout::Grid2x2:
        m_boardConfig.gridRows = 2;
        m_boardConfig.gridCols = 2;
        break;
    case CalibrationBoardLayout::Grid3x3:
        m_boardConfig.gridRows = 3;
        m_boardConfig.gridCols = 3;
        break;
    case CalibrationBoardLayout::Grid4x4:
        m_boardConfig.gridRows = 4;
        m_boardConfig.gridCols = 4;
        break;
    case CalibrationBoardLayout::Custom:
        // 保持用户设置的行列数
        break;
    }
}

void QRCodeCalibration::setCalibrationConfig(const CalibrationConfig& config)
{
    m_calibrationManager->setConfig(config);
}

const CalibrationConfig& QRCodeCalibration::getCalibrationConfig() const
{
    return m_calibrationManager->getConfig();
}

// ============================================================
// 2D码检测
// ============================================================

std::vector<Code2DResult> QRCodeCalibration::detect(const QImage& image)
{
    m_lastDetectionResults.clear();

    if (image.isNull()) {
        setError("输入图像为空");
        return m_lastDetectionResults;
    }

    reportProgress(10, "开始检测2D码...");

    // 图像预处理
    QImage processedImage = image;
    if (m_detectionConfig.enablePreprocessing) {
        processedImage = preprocessImage(image);
    }

    reportProgress(20, "图像预处理完成");

    // 根据配置的码类型进行检测
    std::vector<Code2DResult> results;

    switch (m_detectionConfig.expectedType) {
    case Code2DType::QRCode:
        results = detectQRCodes(processedImage);
        break;

    case Code2DType::DataMatrix:
        results = detectDataMatrixCodes(processedImage);
        break;

    case Code2DType::Auto:
    default:
        // 自动检测：先尝试QR码，再尝试DataMatrix
        results = detectQRCodes(processedImage);
        if (results.empty()) {
            auto dmResults = detectDataMatrixCodes(processedImage);
            results.insert(results.end(), dmResults.begin(), dmResults.end());
        }
        break;
    }

    reportProgress(70, QString("检测到 %1 个码").arg(results.size()));

    // 角点精化
    if (m_detectionConfig.refineCorners && !results.empty()) {
        for (auto& result : results) {
            extractCodeCorners(processedImage, result);
        }
    }

    reportProgress(90, "角点精化完成");

    // 过滤结果
    std::vector<Code2DResult> filteredResults;
    for (const auto& result : results) {
        if (result.confidence >= m_detectionConfig.minConfidence) {
            filteredResults.push_back(result);
        }
    }

    // 按置信度排序
    std::sort(filteredResults.begin(), filteredResults.end(),
              [](const Code2DResult& a, const Code2DResult& b) {
                  return a.confidence > b.confidence;
              });

    // 限制数量
    if (static_cast<int>(filteredResults.size()) > m_detectionConfig.maxCodes) {
        filteredResults.resize(m_detectionConfig.maxCodes);
    }

    // 设置索引
    for (int i = 0; i < static_cast<int>(filteredResults.size()); ++i) {
        filteredResults[i].index = i;
    }

    m_lastDetectionResults = filteredResults;

    reportProgress(100, "检测完成");
    emit detectionCompleted(m_lastDetectionResults);

    return m_lastDetectionResults;
}

std::vector<Code2DResult> QRCodeCalibration::detect(const QString& imagePath)
{
    QImage image(imagePath);
    if (image.isNull()) {
        setError(QString("无法加载图像文件: %1").arg(imagePath));
        return {};
    }
    return detect(image);
}

std::vector<Code2DResult> QRCodeCalibration::detect(const uchar* imageData, int width, int height,
                                                     QImage::Format format)
{
    QImage image(imageData, width, height, format);
    if (image.isNull()) {
        setError("无法从原始数据创建图像");
        return {};
    }
    return detect(image.copy());  // 复制以确保数据生命周期
}

void QRCodeCalibration::clearDetectionResults()
{
    m_lastDetectionResults.clear();
}

// ============================================================
// 角点提取
// ============================================================

CalibrationPointSet QRCodeCalibration::extractCalibrationPoints(
    const std::vector<Code2DResult>& detectionResults)
{
    CalibrationPointSet pointSet;

    // 获取标定板世界坐标
    auto allWorldCorners = m_boardConfig.getAllCodeWorldCorners();

    // 匹配检测结果与标定板位置
    auto matches = matchCodesToBoard(detectionResults);

    for (const auto& [detectionIdx, boardIdx] : matches) {
        if (detectionIdx < 0 || detectionIdx >= static_cast<int>(detectionResults.size())) {
            continue;
        }
        if (boardIdx < 0 || boardIdx >= static_cast<int>(allWorldCorners.size())) {
            continue;
        }

        const auto& detection = detectionResults[detectionIdx];
        const auto& worldCorners = allWorldCorners[boardIdx];

        // 添加四个角点
        auto calibPoints = detection.toCalibrationPoints(worldCorners);
        for (const auto& pt : calibPoints) {
            pointSet.addPoint(pt);
        }
    }

    return pointSet;
}

std::vector<QPointF> QRCodeCalibration::refineCorners(const QImage& image,
                                                       const std::vector<QPointF>& corners)
{
    std::vector<QPointF> refinedCorners;
    refinedCorners.reserve(corners.size());

    for (const auto& corner : corners) {
        QPointF refined = refineCornerSubpixel(image, corner);
        refinedCorners.push_back(refined);
    }

    return refinedCorners;
}

std::vector<std::pair<int, int>> QRCodeCalibration::matchCodesToBoard(
    const std::vector<Code2DResult>& detectionResults)
{
    std::vector<std::pair<int, int>> matches;

    if (m_boardConfig.validateContent && !m_boardConfig.expectedContents.isEmpty()) {
        // 按内容匹配
        for (int i = 0; i < static_cast<int>(detectionResults.size()); ++i) {
            int boardIdx = matchCodeByContent(detectionResults[i]);
            if (boardIdx >= 0) {
                matches.emplace_back(i, boardIdx);
            }
        }
    } else {
        // 按位置匹配（简单的顺序匹配）
        int expectedCount = m_boardConfig.gridRows * m_boardConfig.gridCols;
        for (int i = 0; i < std::min(static_cast<int>(detectionResults.size()), expectedCount); ++i) {
            matches.emplace_back(i, i);
        }
    }

    return matches;
}

// ============================================================
// 自动标定
// ============================================================

CalibrationResult QRCodeCalibration::detectAndCalibrate(const QImage& image)
{
    return detectAndCalibrate(image, nullptr);
}

CalibrationResult QRCodeCalibration::detectAndCalibrate(const QImage& image,
                                                         QRCalibrationProgressCallback progressCallback)
{
    m_progressCallback = std::move(progressCallback);

    // 检测2D码
    reportProgress(0, "开始自动标定...");
    auto detectionResults = detect(image);

    if (detectionResults.empty()) {
        setError("未检测到任何2D码");
        CalibrationResult result;
        result.status = CalibrationStatus::Failed;
        result.error = CalibrationError::InvalidInput;
        emit calibrationCompleted(false, result);
        return result;
    }

    reportProgress(50, QString("检测到 %1 个码，开始标定...").arg(detectionResults.size()));

    // 执行标定
    return calibrate(detectionResults);
}

CalibrationResult QRCodeCalibration::calibrate(const std::vector<Code2DResult>& detectionResults)
{
    // 验证检测结果
    if (!validateDetectionResults(detectionResults)) {
        CalibrationResult result;
        result.status = CalibrationStatus::Failed;
        result.error = CalibrationError::InsufficientPoints;
        emit calibrationCompleted(false, result);
        return result;
    }

    reportProgress(60, "提取标定点...");

    // 提取标定点
    auto pointSet = extractCalibrationPoints(detectionResults);

    if (pointSet.count() < 4) {
        setError(QString("标定点数量不足，需要至少4个点，当前: %1").arg(pointSet.count()));
        CalibrationResult result;
        result.status = CalibrationStatus::Failed;
        result.error = CalibrationError::InsufficientPoints;
        emit calibrationCompleted(false, result);
        return result;
    }

    reportProgress(70, QString("使用 %1 个点进行标定...").arg(pointSet.count()));

    // 清空并添加标定点
    m_calibrationManager->clearCalibrationPoints();
    for (const auto& pt : pointSet.points()) {
        m_calibrationManager->addCalibrationPoint(pt);
    }

    // 执行标定
    CalibrationResult result = m_calibrationManager->calibrate();

    reportProgress(90, "计算标定矩阵...");

    if (result.isValid()) {
        reportProgress(100, QString("标定成功，RMS误差: %1 像素")
                           .arg(result.errorStats.rmsError, 0, 'f', 3));
    } else {
        reportProgress(100, "标定失败");
        setError(getCalibrationErrorDescription(result.error));
    }

    emit calibrationCompleted(result.isValid(), result);
    return result;
}

int QRCodeCalibration::addManualPoint(const QPointF& pixelPos, const QPointF& worldPos)
{
    return m_calibrationManager->addCalibrationPoint(pixelPos, worldPos);
}

bool QRCodeCalibration::removePoint(int index)
{
    return m_calibrationManager->removeCalibrationPoint(index);
}

void QRCodeCalibration::clearPoints()
{
    m_calibrationManager->clearCalibrationPoints();
}

// ============================================================
// 标定结果访问
// ============================================================

const CalibrationResult& QRCodeCalibration::getResult() const
{
    return m_calibrationManager->getResult();
}

bool QRCodeCalibration::isCalibrated() const
{
    return m_calibrationManager->isCalibrated();
}

QPointF QRCodeCalibration::pixelToWorld(const QPointF& pixelPos) const
{
    return m_calibrationManager->pixelToWorld(pixelPos);
}

QPointF QRCodeCalibration::worldToPixel(const QPointF& worldPos) const
{
    return m_calibrationManager->worldToPixel(worldPos);
}

// ============================================================
// 验证和诊断
// ============================================================

bool QRCodeCalibration::validateDetectionResults(const std::vector<Code2DResult>& detectionResults) const
{
    if (detectionResults.empty()) {
        return false;
    }

    // 检查是否有足够的有效检测
    int validCount = 0;
    for (const auto& result : detectionResults) {
        if (result.isValid()) {
            ++validCount;
        }
    }

    // 至少需要1个有效检测来生成4个角点
    return validCount >= 1;
}

double QRCodeCalibration::calculateDetectionQuality(const std::vector<Code2DResult>& detectionResults) const
{
    if (detectionResults.empty()) {
        return 0.0;
    }

    // 计算平均置信度
    double totalConfidence = 0.0;
    int validCount = 0;

    for (const auto& result : detectionResults) {
        if (result.isValid()) {
            totalConfidence += result.confidence;
            ++validCount;
        }
    }

    if (validCount == 0) {
        return 0.0;
    }

    double avgConfidence = totalConfidence / validCount;

    // 计算检测完整度（相对于期望的码数量）
    int expectedCount = m_boardConfig.gridRows * m_boardConfig.gridCols;
    double completeness = std::min(1.0, static_cast<double>(validCount) / expectedCount);

    // 综合质量分数
    return (avgConfidence * 0.6 + completeness * 0.4) * 100.0;
}

QString QRCodeCalibration::getDiagnosticInfo() const
{
    QString info;
    info += QString("=== QR码标定诊断信息 ===\n");
    info += QString("标定板配置: %1x%2 网格, 码尺寸: %3mm\n")
                .arg(m_boardConfig.gridRows)
                .arg(m_boardConfig.gridCols)
                .arg(m_boardConfig.codeSize);
    info += QString("检测配置: 类型=%1, 最大数量=%2\n")
                .arg(getCode2DTypeName(m_detectionConfig.expectedType))
                .arg(m_detectionConfig.maxCodes);

    if (!m_lastDetectionResults.empty()) {
        info += QString("最后检测: %1 个码\n").arg(m_lastDetectionResults.size());
        for (const auto& result : m_lastDetectionResults) {
            info += QString("  - %1\n").arg(result.toString());
        }
    }

    if (isCalibrated()) {
        const auto& result = getResult();
        info += QString("标定结果: RMS=%1px, 点数=%2\n")
                    .arg(result.errorStats.rmsError, 0, 'f', 3)
                    .arg(result.pointCount);
    } else {
        info += "标定状态: 未标定\n";
    }

    if (!m_lastError.isEmpty()) {
        info += QString("最后错误: %1\n").arg(m_lastError);
    }

    return info;
}

// ============================================================
// 持久化
// ============================================================

bool QRCodeCalibration::saveToFile(const QString& filePath) const
{
    QJsonObject json = exportToJson();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool QRCodeCalibration::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        setError(QString("无法打开文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        setError("无效的JSON文件");
        return false;
    }

    return importFromJson(doc.object());
}

QJsonObject QRCodeCalibration::exportToJson() const
{
    QJsonObject json;
    json["version"] = "1.0.0";
    json["type"] = "QRCodeCalibration";

    json["detectionConfig"] = m_detectionConfig.toJson();
    json["boardConfig"] = m_boardConfig.toJson();

    // 导出检测结果
    QJsonArray resultsArray;
    for (const auto& result : m_lastDetectionResults) {
        resultsArray.append(result.toJson());
    }
    json["lastDetectionResults"] = resultsArray;

    // 导出标定结果
    json["calibration"] = m_calibrationManager->exportToJson();

    return json;
}

bool QRCodeCalibration::importFromJson(const QJsonObject& json)
{
    if (json["type"].toString() != "QRCodeCalibration") {
        setError("无效的QRCodeCalibration文件格式");
        return false;
    }

    m_detectionConfig = Code2DDetectionConfig::fromJson(json["detectionConfig"].toObject());
    m_boardConfig = CalibrationBoardConfig::fromJson(json["boardConfig"].toObject());

    // 导入检测结果
    m_lastDetectionResults.clear();
    QJsonArray resultsArray = json["lastDetectionResults"].toArray();
    for (const auto& resultJson : resultsArray) {
        m_lastDetectionResults.push_back(Code2DResult::fromJson(resultJson.toObject()));
    }

    // 导入标定结果
    if (json.contains("calibration")) {
        m_calibrationManager->importFromJson(json["calibration"].toObject());
    }

    return true;
}

// ============================================================
// OpenCV辅助函数
// ============================================================

namespace {

/**
 * @brief QImage转cv::Mat
 */
cv::Mat qImageToMat(const QImage& image)
{
    QImage convertedImage = image;

    // 确保图像格式正确
    if (image.format() != QImage::Format_RGB888 &&
        image.format() != QImage::Format_Grayscale8) {
        convertedImage = image.convertToFormat(QImage::Format_RGB888);
    }

    cv::Mat mat;
    if (convertedImage.format() == QImage::Format_Grayscale8) {
        mat = cv::Mat(convertedImage.height(), convertedImage.width(),
                      CV_8UC1, const_cast<uchar*>(convertedImage.bits()),
                      convertedImage.bytesPerLine()).clone();
    } else {
        mat = cv::Mat(convertedImage.height(), convertedImage.width(),
                      CV_8UC3, const_cast<uchar*>(convertedImage.bits()),
                      convertedImage.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    }

    return mat;
}

/**
 * @brief cv::Mat转QImage
 */
QImage matToQImage(const cv::Mat& mat)
{
    if (mat.empty()) {
        return QImage();
    }

    if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows,
                      static_cast<int>(mat.step), QImage::Format_Grayscale8).copy();
    } else if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows,
                      static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    }

    return QImage();
}

/**
 * @brief 计算四边形角点的角度和尺寸
 */
void analyzeQuadrilateral(const std::vector<cv::Point2f>& points,
                          double& angle, int& moduleSize)
{
    if (points.size() < 4) {
        angle = 0.0;
        moduleSize = 0;
        return;
    }

    // 计算旋转角度（基于顶边）
    double dx = points[1].x - points[0].x;
    double dy = points[1].y - points[0].y;
    angle = std::atan2(dy, dx) * 180.0 / CV_PI;

    // 计算平均边长
    double side1 = cv::norm(cv::Point2f(points[1].x - points[0].x, points[1].y - points[0].y));
    double side2 = cv::norm(cv::Point2f(points[2].x - points[1].x, points[2].y - points[1].y));
    double side3 = cv::norm(cv::Point2f(points[3].x - points[2].x, points[3].y - points[2].y));
    double side4 = cv::norm(cv::Point2f(points[0].x - points[3].x, points[0].y - points[3].y));

    double avgSide = (side1 + side2 + side3 + side4) / 4.0;

    // QR码版本1有21个模块，每增加一个版本增加4个模块
    // 估算模块大小
    moduleSize = static_cast<int>(avgSide / 21.0);  // 假设版本1
}

}  // namespace

// ============================================================
// 内部实现
// ============================================================

std::vector<Code2DResult> QRCodeCalibration::detectQRCodes(const QImage& image)
{
    std::vector<Code2DResult> results;

    if (image.isNull()) {
        return results;
    }

    try {
        // 转换为OpenCV格式
        cv::Mat cvImage = qImageToMat(image);
        if (cvImage.empty()) {
            qWarning() << "QRCodeCalibration: 图像转换失败";
            return results;
        }

        // 转换为灰度图
        cv::Mat gray;
        if (cvImage.channels() == 3) {
            cv::cvtColor(cvImage, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = cvImage;
        }

        // 使用OpenCV QRCodeDetector
        cv::QRCodeDetector detector;

        // 检测多个QR码
        std::vector<cv::Point2f> allPoints;
        std::vector<std::string> decodedContents;

        // 尝试检测并解码多个QR码
        bool detected = detector.detectAndDecodeMulti(gray, decodedContents, allPoints);

        if (detected && !allPoints.empty()) {
            // 每个QR码有4个角点
            size_t numCodes = allPoints.size() / 4;

            for (size_t i = 0; i < numCodes && static_cast<int>(i) < m_detectionConfig.maxCodes; ++i) {
                Code2DResult result;
                result.type = Code2DType::QRCode;
                result.index = static_cast<int>(i);

                // 提取4个角点
                for (int j = 0; j < 4; ++j) {
                    const auto& pt = allPoints[i * 4 + j];
                    result.corners[j] = QPointF(pt.x, pt.y);
                }

                // 计算边界矩形
                float minX = result.corners[0].x();
                float maxX = result.corners[0].x();
                float minY = result.corners[0].y();
                float maxY = result.corners[0].y();
                for (int j = 1; j < 4; ++j) {
                    minX = std::min(minX, static_cast<float>(result.corners[j].x()));
                    maxX = std::max(maxX, static_cast<float>(result.corners[j].x()));
                    minY = std::min(minY, static_cast<float>(result.corners[j].y()));
                    maxY = std::max(maxY, static_cast<float>(result.corners[j].y()));
                }
                result.boundingRect = QRectF(minX, minY, maxX - minX, maxY - minY);

                // 计算中心点
                result.center = QPointF(
                    (result.corners[0].x() + result.corners[2].x()) / 2.0,
                    (result.corners[0].y() + result.corners[2].y()) / 2.0);

                // 解码内容
                if (i < decodedContents.size() && !decodedContents[i].empty()) {
                    result.content = QString::fromStdString(decodedContents[i]);
                    result.decoded = true;
                }

                // 计算角度和模块大小
                std::vector<cv::Point2f> cornerPts;
                for (int j = 0; j < 4; ++j) {
                    cornerPts.emplace_back(
                        static_cast<float>(result.corners[j].x()),
                        static_cast<float>(result.corners[j].y()));
                }
                analyzeQuadrilateral(cornerPts, result.angle, result.moduleSize);

                // 设置置信度（基于解码成功与否）
                result.confidence = result.decoded ? 0.95 : 0.7;

                results.push_back(result);
            }
        }

        // 如果多码检测失败，尝试单码检测
        if (results.empty()) {
            std::vector<cv::Point2f> points;
            std::string content = detector.detectAndDecode(gray, points);

            if (!points.empty() && points.size() >= 4) {
                Code2DResult result;
                result.type = Code2DType::QRCode;
                result.index = 0;

                // 提取4个角点
                for (int j = 0; j < 4; ++j) {
                    result.corners[j] = QPointF(points[j].x, points[j].y);
                }

                // 计算边界矩形和中心
                float minX = points[0].x, maxX = points[0].x;
                float minY = points[0].y, maxY = points[0].y;
                for (size_t j = 1; j < points.size(); ++j) {
                    minX = std::min(minX, points[j].x);
                    maxX = std::max(maxX, points[j].x);
                    minY = std::min(minY, points[j].y);
                    maxY = std::max(maxY, points[j].y);
                }
                result.boundingRect = QRectF(minX, minY, maxX - minX, maxY - minY);
                result.center = QPointF((minX + maxX) / 2.0, (minY + maxY) / 2.0);

                // 解码内容
                if (!content.empty()) {
                    result.content = QString::fromStdString(content);
                    result.decoded = true;
                    result.confidence = 0.95;
                } else {
                    result.confidence = 0.7;
                }

                // 计算角度和模块大小
                analyzeQuadrilateral(points, result.angle, result.moduleSize);

                results.push_back(result);
            }
        }

        qDebug() << "QRCodeCalibration: 检测到" << results.size() << "个QR码";

    } catch (const cv::Exception& e) {
        qWarning() << "QRCodeCalibration: OpenCV异常 -" << e.what();
    } catch (const std::exception& e) {
        qWarning() << "QRCodeCalibration: 异常 -" << e.what();
    }

    return results;
}

std::vector<Code2DResult> QRCodeCalibration::detectDataMatrixCodes(const QImage& image)
{
    std::vector<Code2DResult> results;

    if (image.isNull()) {
        return results;
    }

    try {
        // 转换为OpenCV格式
        cv::Mat cvImage = qImageToMat(image);
        if (cvImage.empty()) {
            return results;
        }

        // 转换为灰度图
        cv::Mat gray;
        if (cvImage.channels() == 3) {
            cv::cvtColor(cvImage, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = cvImage;
        }

        // OpenCV没有内置的DataMatrix检测器
        // 使用轮廓检测来寻找可能的DataMatrix码区域
        cv::Mat blurred, thresh;
        cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
        cv::adaptiveThreshold(blurred, thresh, 255,
                              cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                              cv::THRESH_BINARY_INV, 11, 2);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int detectedCount = 0;
        for (const auto& contour : contours) {
            if (detectedCount >= m_detectionConfig.maxCodes) {
                break;
            }

            // 过滤太小或太大的轮廓
            double area = cv::contourArea(contour);
            if (area < 100 || area > gray.cols * gray.rows * 0.5) {
                continue;
            }

            // 近似多边形
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contour, approx, cv::arcLength(contour, true) * 0.02, true);

            // DataMatrix码通常是矩形
            if (approx.size() == 4) {
                // 检查是否接近正方形
                cv::RotatedRect rect = cv::minAreaRect(contour);
                float aspectRatio = std::min(rect.size.width, rect.size.height) /
                                    std::max(rect.size.width, rect.size.height);

                if (aspectRatio > 0.7) {  // 接近正方形
                    Code2DResult result;
                    result.type = Code2DType::DataMatrix;
                    result.index = detectedCount++;

                    // 排序角点（顺时针从左上开始）
                    std::vector<cv::Point2f> pts;
                    cv::Point2f rectPts[4];
                    rect.points(rectPts);
                    for (int i = 0; i < 4; ++i) {
                        pts.push_back(rectPts[i]);
                    }

                    // 按y坐标排序找出上面两个点
                    std::sort(pts.begin(), pts.end(),
                              [](const cv::Point2f& a, const cv::Point2f& b) {
                                  return a.y < b.y;
                              });

                    // 上面两个点按x排序
                    if (pts[0].x > pts[1].x) std::swap(pts[0], pts[1]);
                    // 下面两个点按x排序（逆序）
                    if (pts[2].x < pts[3].x) std::swap(pts[2], pts[3]);

                    for (int i = 0; i < 4; ++i) {
                        result.corners[i] = QPointF(pts[i].x, pts[i].y);
                    }

                    result.boundingRect = QRectF(rect.center.x - rect.size.width / 2,
                                                 rect.center.y - rect.size.height / 2,
                                                 rect.size.width, rect.size.height);
                    result.center = QPointF(rect.center.x, rect.center.y);
                    result.angle = rect.angle;
                    result.confidence = 0.6;  // 较低的置信度，因为未解码

                    results.push_back(result);
                }
            }
        }

        qDebug() << "QRCodeCalibration: 检测到" << results.size() << "个可能的DataMatrix码区域";

    } catch (const cv::Exception& e) {
        qWarning() << "QRCodeCalibration: OpenCV异常 -" << e.what();
    }

    return results;
}

QImage QRCodeCalibration::preprocessImage(const QImage& image)
{
    if (image.isNull()) {
        return image;
    }

    try {
        // 转换为OpenCV格式
        cv::Mat cvImage = qImageToMat(image);
        if (cvImage.empty()) {
            return image.convertToFormat(QImage::Format_Grayscale8);
        }

        // 转换为灰度图
        cv::Mat gray;
        if (cvImage.channels() == 3) {
            cv::cvtColor(cvImage, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = cvImage;
        }

        // 高斯模糊降噪
        if (m_detectionConfig.blurKernelSize > 1) {
            int kernelSize = m_detectionConfig.blurKernelSize;
            if (kernelSize % 2 == 0) kernelSize++;  // 确保是奇数
            cv::GaussianBlur(gray, gray, cv::Size(kernelSize, kernelSize), 0);
        }

        // 对比度增强
        if (std::abs(m_detectionConfig.contrastEnhancement - 1.0) > 0.01) {
            gray.convertTo(gray, -1, m_detectionConfig.contrastEnhancement, 0);
        }

        // 转换回QImage
        return matToQImage(gray);

    } catch (const cv::Exception& e) {
        qWarning() << "QRCodeCalibration: 预处理异常 -" << e.what();
        return image.convertToFormat(QImage::Format_Grayscale8);
    }
}

bool QRCodeCalibration::extractCodeCorners(const QImage& image, Code2DResult& result)
{
    // 如果已经有角点，进行精化
    bool hasCorners = true;
    for (const auto& corner : result.corners) {
        if (corner.isNull()) {
            hasCorners = false;
            break;
        }
    }

    if (!hasCorners) {
        // 从边界矩形推断角点
        result.corners[0] = result.boundingRect.topLeft();
        result.corners[1] = result.boundingRect.topRight();
        result.corners[2] = result.boundingRect.bottomRight();
        result.corners[3] = result.boundingRect.bottomLeft();
    }

    // 角点精化
    if (m_detectionConfig.refineCorners) {
        for (auto& corner : result.corners) {
            corner = refineCornerSubpixel(image, corner);
        }
    }

    return true;
}

QPointF QRCodeCalibration::refineCornerSubpixel(const QImage& image, const QPointF& corner)
{
    if (image.isNull()) {
        return corner;
    }

    try {
        // 转换为OpenCV格式
        cv::Mat cvImage = qImageToMat(image);
        if (cvImage.empty()) {
            return corner;
        }

        // 转换为灰度图
        cv::Mat gray;
        if (cvImage.channels() == 3) {
            cv::cvtColor(cvImage, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = cvImage;
        }

        // 准备角点
        std::vector<cv::Point2f> corners;
        corners.emplace_back(static_cast<float>(corner.x()),
                            static_cast<float>(corner.y()));

        // 确保角点在图像范围内
        if (corners[0].x < 0 || corners[0].x >= gray.cols ||
            corners[0].y < 0 || corners[0].y >= gray.rows) {
            corners[0].x = std::max(0.0f, std::min(corners[0].x, static_cast<float>(gray.cols - 1)));
            corners[0].y = std::max(0.0f, std::min(corners[0].y, static_cast<float>(gray.rows - 1)));
            return QPointF(corners[0].x, corners[0].y);
        }

        // 使用OpenCV亚像素角点精化
        int winSize = m_detectionConfig.refineWindowSize / 2;
        cv::Size searchWindow(winSize, winSize);
        cv::Size zeroZone(-1, -1);
        cv::TermCriteria criteria(
            cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER,
            m_detectionConfig.refineMaxIterations,
            m_detectionConfig.refineEpsilon);

        cv::cornerSubPix(gray, corners, searchWindow, zeroZone, criteria);

        return QPointF(corners[0].x, corners[0].y);

    } catch (const cv::Exception& e) {
        qWarning() << "QRCodeCalibration: 角点精化异常 -" << e.what();
        return corner;
    }
}

int QRCodeCalibration::matchCodeByContent(const Code2DResult& result) const
{
    if (!result.decoded || result.content.isEmpty()) {
        return -1;
    }

    for (int i = 0; i < m_boardConfig.expectedContents.size(); ++i) {
        if (result.content == m_boardConfig.expectedContents[i]) {
            return i;
        }
    }

    return -1;
}

int QRCodeCalibration::matchCodeByPosition(const Code2DResult& result,
                                            const std::vector<std::array<QPointF, 4>>& expectedPositions) const
{
    if (expectedPositions.empty()) {
        return -1;
    }

    double minDistance = std::numeric_limits<double>::max();
    int bestMatch = -1;

    for (int i = 0; i < static_cast<int>(expectedPositions.size()); ++i) {
        // 计算中心点距离
        QPointF expectedCenter(
            (expectedPositions[i][0].x() + expectedPositions[i][2].x()) / 2,
            (expectedPositions[i][0].y() + expectedPositions[i][2].y()) / 2);

        double dist = distance(result.center, expectedCenter);
        if (dist < minDistance) {
            minDistance = dist;
            bestMatch = i;
        }
    }

    return bestMatch;
}

double QRCodeCalibration::distance(const QPointF& p1, const QPointF& p2)
{
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    return std::sqrt(dx * dx + dy * dy);
}

void QRCodeCalibration::reportProgress(int progress, const QString& message)
{
    emit progressUpdated(progress, message);
    if (m_progressCallback) {
        m_progressCallback(progress, message);
    }
}

void QRCodeCalibration::setError(const QString& error)
{
    m_lastError = error;
    emit errorOccurred(error);
    qWarning() << "QRCodeCalibration Error:" << error;
}

// ============================================================
// 工具函数
// ============================================================

QString getCode2DTypeName(Code2DType type)
{
    switch (type) {
    case Code2DType::None:
        return QStringLiteral("无");
    case Code2DType::QRCode:
        return QStringLiteral("QR码");
    case Code2DType::DataMatrix:
        return QStringLiteral("DataMatrix");
    case Code2DType::PDF417:
        return QStringLiteral("PDF417");
    case Code2DType::Aztec:
        return QStringLiteral("Aztec");
    case Code2DType::Auto:
        return QStringLiteral("自动检测");
    default:
        return QStringLiteral("未知");
    }
}

Code2DType getCode2DTypeFromName(const QString& name)
{
    if (name == QStringLiteral("QR码") || name == "QRCode") {
        return Code2DType::QRCode;
    }
    if (name == QStringLiteral("DataMatrix") || name == "DM码") {
        return Code2DType::DataMatrix;
    }
    if (name == QStringLiteral("PDF417")) {
        return Code2DType::PDF417;
    }
    if (name == QStringLiteral("Aztec")) {
        return Code2DType::Aztec;
    }
    if (name == QStringLiteral("自动检测") || name == "Auto") {
        return Code2DType::Auto;
    }
    return Code2DType::None;
}

QString getBoardLayoutName(CalibrationBoardLayout layout)
{
    switch (layout) {
    case CalibrationBoardLayout::Single:
        return QStringLiteral("单码");
    case CalibrationBoardLayout::Grid2x2:
        return QStringLiteral("2x2网格");
    case CalibrationBoardLayout::Grid3x3:
        return QStringLiteral("3x3网格");
    case CalibrationBoardLayout::Grid4x4:
        return QStringLiteral("4x4网格");
    case CalibrationBoardLayout::Custom:
        return QStringLiteral("自定义");
    default:
        return QStringLiteral("未知");
    }
}

CalibrationBoardLayout getBoardLayoutFromName(const QString& name)
{
    if (name == QStringLiteral("单码") || name == "Single") {
        return CalibrationBoardLayout::Single;
    }
    if (name == QStringLiteral("2x2网格") || name == "Grid2x2") {
        return CalibrationBoardLayout::Grid2x2;
    }
    if (name == QStringLiteral("3x3网格") || name == "Grid3x3") {
        return CalibrationBoardLayout::Grid3x3;
    }
    if (name == QStringLiteral("4x4网格") || name == "Grid4x4") {
        return CalibrationBoardLayout::Grid4x4;
    }
    if (name == QStringLiteral("自定义") || name == "Custom") {
        return CalibrationBoardLayout::Custom;
    }
    return CalibrationBoardLayout::Single;
}

} // namespace Calibration
} // namespace VisionForge
