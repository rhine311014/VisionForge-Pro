/**
 * @file CalibrationTypes.cpp
 * @brief VisionForge Pro 标定类型实现
 *
 * 使用Eigen库进行精密几何计算。
 *
 * Eigen API 说明：
 * - Eigen::Matrix3d：固定大小3x3双精度矩阵，使用 (i,j) 访问元素
 * - Eigen::Matrix3d::Identity()：单位矩阵
 * - Eigen::Matrix<double, 2, 3>::Zero()：2x3零矩阵
 * - matrix.determinant()：计算行列式
 * - matrix.inverse()：矩阵求逆
 * - Eigen::Vector3d：3元素向量，使用 (i) 或 [i] 访问元素
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include "calibration/CalibrationTypes.h"
#include <QJsonDocument>
#include <QtMath>
#include <algorithm>
#include <numeric>

namespace VisionForge {
namespace Calibration {

// ============================================================
// CalibrationPoint 实现
// ============================================================

QJsonObject CalibrationPoint::toJson() const {
    QJsonObject json;
    json["pixelX"] = pixelPos.x();
    json["pixelY"] = pixelPos.y();
    json["worldX"] = worldPos.x();
    json["worldY"] = worldPos.y();
    json["index"] = index;
    json["isValid"] = isValid;
    json["isEnabled"] = isEnabled;
    json["error"] = error;
    json["weight"] = weight;
    return json;
}

CalibrationPoint CalibrationPoint::fromJson(const QJsonObject& json) {
    CalibrationPoint point;
    point.pixelPos = QPointF(json["pixelX"].toDouble(), json["pixelY"].toDouble());
    point.worldPos = QPointF(json["worldX"].toDouble(), json["worldY"].toDouble());
    point.index = json["index"].toInt(-1);
    point.isValid = json["isValid"].toBool(false);
    point.isEnabled = json["isEnabled"].toBool(true);
    point.error = json["error"].toDouble(0.0);
    point.weight = json["weight"].toDouble(1.0);
    return point;
}

QString CalibrationPoint::toString() const {
    return QString("Point[%1]: Pixel(%2, %3) -> World(%4, %5) Error=%6")
        .arg(index)
        .arg(pixelPos.x(), 0, 'f', 2)
        .arg(pixelPos.y(), 0, 'f', 2)
        .arg(worldPos.x(), 0, 'f', 4)
        .arg(worldPos.y(), 0, 'f', 4)
        .arg(error, 0, 'f', 4);
}

// ============================================================
// CalibrationPointSet 实现
// ============================================================

CalibrationPointSet::CalibrationPointSet(int expectedCount)
    : m_expectedCount(expectedCount) {
    if (expectedCount > 0) {
        m_points.reserve(expectedCount);
    }
}

int CalibrationPointSet::addPoint(const CalibrationPoint& point) {
    CalibrationPoint newPoint = point;
    if (newPoint.index < 0) {
        newPoint.index = static_cast<int>(m_points.size());
    }
    m_points.push_back(newPoint);
    return static_cast<int>(m_points.size()) - 1;
}

bool CalibrationPointSet::setPoint(int index, const CalibrationPoint& point) {
    if (index < 0) {
        return false;
    }

    // 自动扩展
    while (static_cast<int>(m_points.size()) <= index) {
        CalibrationPoint emptyPoint;
        emptyPoint.index = static_cast<int>(m_points.size());
        emptyPoint.isValid = false;
        m_points.push_back(emptyPoint);
    }

    m_points[index] = point;
    m_points[index].index = index;
    return true;
}

CalibrationPoint* CalibrationPointSet::getPoint(int index) {
    if (index < 0 || index >= static_cast<int>(m_points.size())) {
        return nullptr;
    }
    return &m_points[index];
}

const CalibrationPoint* CalibrationPointSet::getPoint(int index) const {
    if (index < 0 || index >= static_cast<int>(m_points.size())) {
        return nullptr;
    }
    return &m_points[index];
}

bool CalibrationPointSet::removePoint(int index) {
    if (index < 0 || index >= static_cast<int>(m_points.size())) {
        return false;
    }
    m_points.erase(m_points.begin() + index);

    // 重新编号
    for (int i = 0; i < static_cast<int>(m_points.size()); ++i) {
        m_points[i].index = i;
    }
    return true;
}

void CalibrationPointSet::clear() {
    m_points.clear();
}

int CalibrationPointSet::validCount() const {
    return static_cast<int>(std::count_if(m_points.begin(), m_points.end(),
        [](const CalibrationPoint& p) { return p.isValid; }));
}

int CalibrationPointSet::enabledCount() const {
    return static_cast<int>(std::count_if(m_points.begin(), m_points.end(),
        [](const CalibrationPoint& p) { return p.isValid && p.isEnabled; }));
}

std::vector<CalibrationPoint> CalibrationPointSet::getValidEnabledPoints() const {
    std::vector<CalibrationPoint> result;
    std::copy_if(m_points.begin(), m_points.end(), std::back_inserter(result),
        [](const CalibrationPoint& p) { return p.isValid && p.isEnabled; });
    return result;
}

bool CalibrationPointSet::hasEnoughPoints(CalibrationType type) const {
    int minPoints = getMinPointsForType(type);
    return enabledCount() >= minPoints;
}

bool CalibrationPointSet::arePointsCollinear(double tolerance) const {
    auto validPoints = getValidEnabledPoints();
    if (validPoints.size() < 3) {
        return true;  // 少于3点认为共线
    }

    // 使用叉积判断共线性
    const QPointF& p0 = validPoints[0].pixelPos;

    for (size_t i = 2; i < validPoints.size(); ++i) {
        const QPointF& p1 = validPoints[i - 1].pixelPos;
        const QPointF& p2 = validPoints[i].pixelPos;

        // 计算向量叉积
        double v1x = p1.x() - p0.x();
        double v1y = p1.y() - p0.y();
        double v2x = p2.x() - p0.x();
        double v2y = p2.y() - p0.y();

        double cross = v1x * v2y - v1y * v2x;

        // 归一化到单位向量长度
        double len1 = std::sqrt(v1x * v1x + v1y * v1y);
        double len2 = std::sqrt(v2x * v2x + v2y * v2y);

        if (len1 > 1e-10 && len2 > 1e-10) {
            double sinAngle = std::abs(cross) / (len1 * len2);
            if (sinAngle > tolerance) {
                return false;  // 不共线
            }
        }
    }

    return true;  // 共线
}

double CalibrationPointSet::calculateDistributionScore() const {
    auto validPoints = getValidEnabledPoints();
    if (validPoints.size() < 2) {
        return 0.0;
    }

    // 计算点的边界框
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& point : validPoints) {
        minX = std::min(minX, point.pixelPos.x());
        maxX = std::max(maxX, point.pixelPos.x());
        minY = std::min(minY, point.pixelPos.y());
        maxY = std::max(maxY, point.pixelPos.y());
    }

    double rangeX = maxX - minX;
    double rangeY = maxY - minY;

    if (rangeX < 1e-10 || rangeY < 1e-10) {
        return 0.1;  // 点几乎重合
    }

    // 计算点间最小距离
    double minDist = std::numeric_limits<double>::max();
    for (size_t i = 0; i < validPoints.size(); ++i) {
        for (size_t j = i + 1; j < validPoints.size(); ++j) {
            double dx = validPoints[i].pixelPos.x() - validPoints[j].pixelPos.x();
            double dy = validPoints[i].pixelPos.y() - validPoints[j].pixelPos.y();
            double dist = std::sqrt(dx * dx + dy * dy);
            minDist = std::min(minDist, dist);
        }
    }

    // 计算理想最小距离
    double area = rangeX * rangeY;
    double idealMinDist = std::sqrt(area / validPoints.size()) * 0.5;

    // 分数 = 实际最小距离 / 理想最小距离（限制在0-1之间）
    double score = std::min(1.0, minDist / idealMinDist);

    // 如果共线，降低分数
    if (arePointsCollinear()) {
        score *= 0.3;
    }

    return score;
}

QJsonArray CalibrationPointSet::toJson() const {
    QJsonArray array;
    for (const auto& point : m_points) {
        array.append(point.toJson());
    }
    return array;
}

CalibrationPointSet CalibrationPointSet::fromJson(const QJsonArray& json) {
    CalibrationPointSet set(json.size());
    for (const auto& item : json) {
        set.addPoint(CalibrationPoint::fromJson(item.toObject()));
    }
    return set;
}

// ============================================================
// ErrorStatistics 实现
// ============================================================

QJsonObject ErrorStatistics::toJson() const {
    QJsonObject json;
    json["meanError"] = meanError;
    json["maxError"] = maxError;
    json["minError"] = minError;
    json["rmsError"] = rmsError;
    json["stdDev"] = stdDev;
    json["worstPointIndex"] = worstPointIndex;
    json["pointCount"] = pointCount;
    return json;
}

ErrorStatistics ErrorStatistics::fromJson(const QJsonObject& json) {
    ErrorStatistics stats;
    stats.meanError = json["meanError"].toDouble();
    stats.maxError = json["maxError"].toDouble();
    stats.minError = json["minError"].toDouble();
    stats.rmsError = json["rmsError"].toDouble();
    stats.stdDev = json["stdDev"].toDouble();
    stats.worstPointIndex = json["worstPointIndex"].toInt(-1);
    stats.pointCount = json["pointCount"].toInt();
    return stats;
}

QString ErrorStatistics::toString() const {
    return QString("ErrorStats: Mean=%1, RMS=%2, Max=%3 (Point %4), StdDev=%5, N=%6")
        .arg(meanError, 0, 'f', 4)
        .arg(rmsError, 0, 'f', 4)
        .arg(maxError, 0, 'f', 4)
        .arg(worstPointIndex)
        .arg(stdDev, 0, 'f', 4)
        .arg(pointCount);
}

// ============================================================
// CalibrationResult 实现
// ============================================================

QPointF CalibrationResult::pixelToWorld(const QPointF& pixelPos) const {
    if (status != CalibrationStatus::Calibrated) {
        return pixelPos;
    }

    // 使用Eigen进行齐次坐标变换
    // Eigen::Vector3d 支持 (i) 索引访问
    Vector3d pixel(pixelPos.x(), pixelPos.y(), 1.0);
    Vector3d world = homography * pixel;

    // Eigen::Vector3d 使用 (i) 访问元素
    if (std::abs(world(2)) < 1e-10) {
        return pixelPos;  // 无效变换
    }

    return QPointF(world(0) / world(2), world(1) / world(2));
}

QPointF CalibrationResult::worldToPixel(const QPointF& worldPos) const {
    if (status != CalibrationStatus::Calibrated) {
        return worldPos;
    }

    Vector3d world(worldPos.x(), worldPos.y(), 1.0);
    Vector3d pixel = inverseHomography * world;

    if (std::abs(pixel(2)) < 1e-10) {
        return worldPos;  // 无效变换
    }

    return QPointF(pixel(0) / pixel(2), pixel(1) / pixel(2));
}

std::vector<QPointF> CalibrationResult::pixelToWorld(const std::vector<QPointF>& pixelPoints) const {
    std::vector<QPointF> result;
    result.reserve(pixelPoints.size());
    for (const auto& pt : pixelPoints) {
        result.push_back(pixelToWorld(pt));
    }
    return result;
}

std::vector<QPointF> CalibrationResult::worldToPixel(const std::vector<QPointF>& worldPoints) const {
    std::vector<QPointF> result;
    result.reserve(worldPoints.size());
    for (const auto& pt : worldPoints) {
        result.push_back(worldToPixel(pt));
    }
    return result;
}

bool CalibrationResult::isMatrixInvertible() const {
    // 使用Eigen计算行列式
    // Eigen::Matrix3d::determinant() 直接返回行列式值
    double det = homography.determinant();
    return std::abs(det) > 1e-10;
}

QJsonObject CalibrationResult::toJson() const {
    QJsonObject json;

    json["status"] = static_cast<int>(status);
    json["error"] = static_cast<int>(error);
    json["type"] = static_cast<int>(type);

    json["homography"] = matrix3dToJson(homography);
    json["inverseHomography"] = matrix3dToJson(inverseHomography);

    json["scaleX"] = scaleX;
    json["scaleY"] = scaleY;
    json["rotation"] = rotation;

    json["errorStats"] = errorStats.toJson();

    QJsonArray errorsArray;
    for (double e : pointErrors) {
        errorsArray.append(e);
    }
    json["pointErrors"] = errorsArray;

    json["calibrationTime"] = calibrationTime.toString(Qt::ISODate);
    json["pointCount"] = pointCount;
    json["description"] = description;

    return json;
}

CalibrationResult CalibrationResult::fromJson(const QJsonObject& json) {
    CalibrationResult result;

    result.status = static_cast<CalibrationStatus>(json["status"].toInt());
    result.error = static_cast<CalibrationError>(json["error"].toInt());
    result.type = static_cast<CalibrationType>(json["type"].toInt());

    if (json.contains("homography")) {
        result.homography = matrix3dFromJson(json["homography"].toArray());
    }
    if (json.contains("inverseHomography")) {
        result.inverseHomography = matrix3dFromJson(json["inverseHomography"].toArray());
    }

    result.scaleX = json["scaleX"].toDouble(1.0);
    result.scaleY = json["scaleY"].toDouble(1.0);
    result.rotation = json["rotation"].toDouble(0.0);

    if (json.contains("errorStats")) {
        result.errorStats = ErrorStatistics::fromJson(json["errorStats"].toObject());
    }

    if (json.contains("pointErrors")) {
        for (const auto& e : json["pointErrors"].toArray()) {
            result.pointErrors.push_back(e.toDouble());
        }
    }

    result.calibrationTime = QDateTime::fromString(json["calibrationTime"].toString(), Qt::ISODate);
    result.pointCount = json["pointCount"].toInt();
    result.description = json["description"].toString();

    return result;
}

QString CalibrationResult::toString() const {
    QString statusStr = getCalibrationStatusName(status);
    QString typeStr = getCalibrationTypeName(type);

    return QString("CalibrationResult: Status=%1, Type=%2\n"
                   "  Scale: X=%3 mm/px, Y=%4 mm/px, Rotation=%5 deg\n"
                   "  %6")
        .arg(statusStr)
        .arg(typeStr)
        .arg(scaleX, 0, 'f', 6)
        .arg(scaleY, 0, 'f', 6)
        .arg(rotation, 0, 'f', 4)
        .arg(errorStats.toString());
}

// ============================================================
// AlignmentOffset 实现
// ============================================================

QJsonObject AlignmentOffset::toJson() const {
    QJsonObject json;
    json["dx"] = dx;
    json["dy"] = dy;
    json["dAngle"] = dAngle;
    json["scaleX"] = scaleX;
    json["scaleY"] = scaleY;
    json["shear"] = shear;
    return json;
}

AlignmentOffset AlignmentOffset::fromJson(const QJsonObject& json) {
    AlignmentOffset offset;
    offset.dx = json["dx"].toDouble();
    offset.dy = json["dy"].toDouble();
    offset.dAngle = json["dAngle"].toDouble();
    offset.scaleX = json["scaleX"].toDouble(1.0);
    offset.scaleY = json["scaleY"].toDouble(1.0);
    offset.shear = json["shear"].toDouble(0.0);
    return offset;
}

QString AlignmentOffset::toString() const {
    return QString("Offset: dx=%1mm, dy=%2mm, dAngle=%3deg (dist=%4mm)")
        .arg(dx, 0, 'f', 4)
        .arg(dy, 0, 'f', 4)
        .arg(dAngle, 0, 'f', 4)
        .arg(getTranslationDistance(), 0, 'f', 4);
}

// ============================================================
// AlignmentResult 实现
// ============================================================

QJsonObject AlignmentResult::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(type);
    json["success"] = success;
    json["error"] = static_cast<int>(error);
    json["offset"] = offset.toJson();
    json["transform"] = matrix3dToJson(transform);
    json["confidence"] = confidence;
    json["residualError"] = residualError;
    return json;
}

AlignmentResult AlignmentResult::fromJson(const QJsonObject& json) {
    AlignmentResult result;
    result.type = static_cast<AlignmentType>(json["type"].toInt());
    result.success = json["success"].toBool();
    result.error = static_cast<CalibrationError>(json["error"].toInt());
    result.offset = AlignmentOffset::fromJson(json["offset"].toObject());
    if (json.contains("transform")) {
        result.transform = matrix3dFromJson(json["transform"].toArray());
    }
    result.confidence = json["confidence"].toDouble();
    result.residualError = json["residualError"].toDouble();
    return result;
}

// ============================================================
// CalibrationConfig 实现
// ============================================================

QJsonObject CalibrationConfig::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(type);
    json["name"] = name;
    json["description"] = description;
    json["gridRows"] = gridRows;
    json["gridCols"] = gridCols;
    json["gridSpacingX"] = gridSpacingX;
    json["gridSpacingY"] = gridSpacingY;
    json["maxAllowedRMS"] = maxAllowedRMS;
    json["maxAllowedMax"] = maxAllowedMax;
    json["useWeightedLeastSquares"] = useWeightedLeastSquares;
    json["enableOutlierRejection"] = enableOutlierRejection;
    json["outlierThreshold"] = outlierThreshold;
    json["enableNonlinearOptimization"] = enableNonlinearOptimization;
    json["maxIterations"] = maxIterations;
    json["convergenceThreshold"] = convergenceThreshold;
    json["validityDays"] = validityDays;
    json["autoExpire"] = autoExpire;
    return json;
}

CalibrationConfig CalibrationConfig::fromJson(const QJsonObject& json) {
    CalibrationConfig config;
    config.type = static_cast<CalibrationType>(json["type"].toInt());
    config.name = json["name"].toString("Default");
    config.description = json["description"].toString();
    config.gridRows = json["gridRows"].toInt(3);
    config.gridCols = json["gridCols"].toInt(3);
    config.gridSpacingX = json["gridSpacingX"].toDouble(10.0);
    config.gridSpacingY = json["gridSpacingY"].toDouble(10.0);
    config.maxAllowedRMS = json["maxAllowedRMS"].toDouble(1.0);
    config.maxAllowedMax = json["maxAllowedMax"].toDouble(3.0);
    config.useWeightedLeastSquares = json["useWeightedLeastSquares"].toBool(false);
    config.enableOutlierRejection = json["enableOutlierRejection"].toBool(true);
    config.outlierThreshold = json["outlierThreshold"].toDouble(3.0);
    config.enableNonlinearOptimization = json["enableNonlinearOptimization"].toBool(false);
    config.maxIterations = json["maxIterations"].toInt(100);
    config.convergenceThreshold = json["convergenceThreshold"].toDouble(1e-6);
    config.validityDays = json["validityDays"].toInt(30);
    config.autoExpire = json["autoExpire"].toBool(false);
    return config;
}

// ============================================================
// 工具函数实现
// ============================================================

QString getCalibrationTypeName(CalibrationType type) {
    switch (type) {
        case CalibrationType::None:         return QStringLiteral("无标定");
        case CalibrationType::Point3:       return QStringLiteral("3点标定");
        case CalibrationType::Point4:       return QStringLiteral("4点标定");
        case CalibrationType::Point5:       return QStringLiteral("5点标定");
        case CalibrationType::Point9:       return QStringLiteral("9点标定");
        case CalibrationType::Point16:      return QStringLiteral("16点标定");
        case CalibrationType::Point25:      return QStringLiteral("25点标定");
        case CalibrationType::DMCode:       return QStringLiteral("DM码标定");
        case CalibrationType::Chessboard:   return QStringLiteral("棋盘格标定");
        case CalibrationType::Circle:       return QStringLiteral("圆点标定");
        case CalibrationType::CrossMark:    return QStringLiteral("十字标记标定");
        case CalibrationType::NonLinear:    return QStringLiteral("非线性标定");
        case CalibrationType::Polynomial:   return QStringLiteral("多项式标定");
        default:                            return QStringLiteral("未知类型");
    }
}

CalibrationType getCalibrationTypeFromName(const QString& name) {
    QString lowerName = name.toLower();
    if (lowerName.contains("3")) return CalibrationType::Point3;
    if (lowerName.contains("4")) return CalibrationType::Point4;
    if (lowerName.contains("5")) return CalibrationType::Point5;
    if (lowerName.contains("9")) return CalibrationType::Point9;
    if (lowerName.contains("16")) return CalibrationType::Point16;
    if (lowerName.contains("25")) return CalibrationType::Point25;
    if (lowerName.contains("dm")) return CalibrationType::DMCode;
    if (lowerName.contains("chess")) return CalibrationType::Chessboard;
    if (lowerName.contains("circle")) return CalibrationType::Circle;
    if (lowerName.contains("cross")) return CalibrationType::CrossMark;
    if (lowerName.contains("nonlinear")) return CalibrationType::NonLinear;
    if (lowerName.contains("poly")) return CalibrationType::Polynomial;
    return CalibrationType::None;
}

int getMinPointsForType(CalibrationType type) {
    switch (type) {
        case CalibrationType::None:         return 0;
        case CalibrationType::Point3:       return 3;
        case CalibrationType::Point4:       return 4;
        case CalibrationType::Point5:       return 5;
        case CalibrationType::Point9:       return 9;
        case CalibrationType::Point16:      return 16;
        case CalibrationType::Point25:      return 25;
        case CalibrationType::DMCode:       return 4;
        case CalibrationType::Chessboard:   return 4;
        case CalibrationType::Circle:       return 4;
        case CalibrationType::CrossMark:    return 1;
        case CalibrationType::NonLinear:    return 4;
        case CalibrationType::Polynomial:   return 6;
        default:                            return 4;
    }
}

QString getCalibrationStatusName(CalibrationStatus status) {
    switch (status) {
        case CalibrationStatus::NotCalibrated:  return QStringLiteral("未标定");
        case CalibrationStatus::Calibrating:    return QStringLiteral("标定中");
        case CalibrationStatus::Calibrated:     return QStringLiteral("已标定");
        case CalibrationStatus::Failed:         return QStringLiteral("标定失败");
        case CalibrationStatus::Expired:        return QStringLiteral("已过期");
        default:                                return QStringLiteral("未知状态");
    }
}

QString getCalibrationErrorDescription(CalibrationError error) {
    switch (error) {
        case CalibrationError::Success:
            return QStringLiteral("成功");
        case CalibrationError::InsufficientPoints:
            return QStringLiteral("标定点数量不足");
        case CalibrationError::SingularMatrix:
            return QStringLiteral("奇异矩阵（标定点可能共线）");
        case CalibrationError::LargeError:
            return QStringLiteral("标定误差过大");
        case CalibrationError::InvalidInput:
            return QStringLiteral("无效输入参数");
        case CalibrationError::ComputationFailed:
            return QStringLiteral("计算失败");
        case CalibrationError::FileIOError:
            return QStringLiteral("文件读写错误");
        case CalibrationError::NotCalibrated:
            return QStringLiteral("尚未执行标定");
        default:
            return QStringLiteral("未知错误");
    }
}

QString getAlignmentTypeName(AlignmentType type) {
    switch (type) {
        case AlignmentType::None:           return QStringLiteral("无对齐");
        case AlignmentType::SinglePoint:    return QStringLiteral("单点对齐");
        case AlignmentType::TwoPoint:       return QStringLiteral("两点对齐");
        case AlignmentType::ThreePoint:     return QStringLiteral("三点对齐");
        case AlignmentType::FourPoint:      return QStringLiteral("四点对齐");
        default:                            return QStringLiteral("未知类型");
    }
}

QJsonArray matrix3dToJson(const Matrix3d& matrix) {
    // Eigen::Matrix3d 支持 (row, col) 索引访问
    QJsonArray array;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            array.append(matrix(i, j));
        }
    }
    return array;
}

Matrix3d matrix3dFromJson(const QJsonArray& json) {
    // 使用Eigen::Matrix3d::Identity()创建单位矩阵
    Matrix3d matrix = Matrix3d::Identity();
    if (json.size() >= 9) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                // Eigen::Matrix3d 支持 (row, col) 索引赋值
                matrix(i, j) = json[i * 3 + j].toDouble();
            }
        }
    }
    return matrix;
}

} // namespace Calibration
} // namespace VisionForge
