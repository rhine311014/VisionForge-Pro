/**
 * @file AlignmentEngine.cpp
 * @brief VisionForge Pro 对齐引擎实现
 *
 * 实现核心对齐算法：
 * - 单点对齐：平移计算
 * - 两点对齐：平移+旋转计算
 * - 三点对齐：仿射变换
 * - 四点对齐：过约束最小二乘
 * - 误差补偿和迭代优化
 *
 * 使用Eigen库进行精密几何计算，主要API说明：
 * - Eigen::MatrixXd：动态大小矩阵，使用 (i,j) 访问元素
 * - Eigen::Matrix3d：固定3x3矩阵，使用 (i,j) 访问元素
 * - Eigen::Matrix<double, 2, 3>：2x3仿射矩阵
 * - Eigen::Vector3d：3元素向量，使用 (i) 访问元素
 * - Eigen::JacobiSVD<MatrixXd>：SVD分解
 * - Eigen::FullPivLU<MatrixXd>：LU分解求解
 * - Matrix3d::Identity()：单位矩阵
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include "calibration/AlignmentEngine.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <cmath>
#include <algorithm>

// Eigen SVD和LU分解
#include <Eigen/SVD>
#include <Eigen/LU>

namespace VisionForge {
namespace Calibration {

// ============================================================
// 静态成员初始化
// ============================================================

std::map<int, std::unique_ptr<AlignmentEngine>> AlignmentEngineFactory::s_instances;

// ============================================================
// AlignmentPoint 实现
// ============================================================

QJsonObject AlignmentPoint::toJson() const {
    QJsonObject json;
    json["templateX"] = templatePos.x();
    json["templateY"] = templatePos.y();
    json["measuredX"] = measuredPos.x();
    json["measuredY"] = measuredPos.y();
    json["templateAngle"] = templateAngle;
    json["measuredAngle"] = measuredAngle;
    json["index"] = index;
    json["isValid"] = isValid;
    json["weight"] = weight;
    return json;
}

AlignmentPoint AlignmentPoint::fromJson(const QJsonObject& json) {
    AlignmentPoint point;
    point.templatePos = QPointF(json["templateX"].toDouble(),
                                 json["templateY"].toDouble());
    point.measuredPos = QPointF(json["measuredX"].toDouble(),
                                 json["measuredY"].toDouble());
    point.templateAngle = json["templateAngle"].toDouble();
    point.measuredAngle = json["measuredAngle"].toDouble();
    point.index = json["index"].toInt();
    point.isValid = json["isValid"].toBool(true);
    point.weight = json["weight"].toDouble(1.0);
    return point;
}

// ============================================================
// RotationCenterConfig 实现
// ============================================================

QJsonObject RotationCenterConfig::toJson() const {
    QJsonObject json;
    json["centerX"] = center.x();
    json["centerY"] = center.y();
    json["useFixedCenter"] = useFixedCenter;
    json["autoCalculate"] = autoCalculate;
    return json;
}

RotationCenterConfig RotationCenterConfig::fromJson(const QJsonObject& json) {
    RotationCenterConfig config;
    config.center = QPointF(json["centerX"].toDouble(),
                            json["centerY"].toDouble());
    config.useFixedCenter = json["useFixedCenter"].toBool(true);
    config.autoCalculate = json["autoCalculate"].toBool(false);
    return config;
}

// ============================================================
// AlignmentConfig 实现
// ============================================================

QJsonObject AlignmentConfig::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(type);
    json["rotationCenter"] = rotationCenter.toJson();
    json["compensateRotation"] = compensateRotation;
    json["positionTolerance"] = positionTolerance;
    json["angleTolerance"] = angleTolerance;
    json["maxIterations"] = maxIterations;
    json["convergenceThreshold"] = convergenceThreshold;
    json["enableIterativeCorrection"] = enableIterativeCorrection;
    json["correctionGain"] = correctionGain;
    json["platformType"] = static_cast<int>(platformType);
    return json;
}

AlignmentConfig AlignmentConfig::fromJson(const QJsonObject& json) {
    AlignmentConfig config;
    config.type = static_cast<AlignmentType>(json["type"].toInt());
    if (json.contains("rotationCenter")) {
        config.rotationCenter = RotationCenterConfig::fromJson(
            json["rotationCenter"].toObject());
    }
    config.compensateRotation = json["compensateRotation"].toBool(true);
    config.positionTolerance = json["positionTolerance"].toDouble(0.01);
    config.angleTolerance = json["angleTolerance"].toDouble(0.01);
    config.maxIterations = json["maxIterations"].toInt(3);
    config.convergenceThreshold = json["convergenceThreshold"].toDouble(0.001);
    config.enableIterativeCorrection = json["enableIterativeCorrection"].toBool(false);
    config.correctionGain = json["correctionGain"].toDouble(1.0);
    config.platformType = static_cast<Platform::PlatformType>(
        json["platformType"].toInt());
    return config;
}

// ============================================================
// AlignmentEngine 实现
// ============================================================

AlignmentEngine::AlignmentEngine(QObject* parent)
    : QObject(parent) {
    m_templatePoints.reserve(4);
    m_measuredPoints.reserve(4);
}

AlignmentEngine::~AlignmentEngine() = default;

void AlignmentEngine::setConfig(const AlignmentConfig& config) {
    m_config = config;
    emit configChanged();
}

void AlignmentEngine::setAlignmentType(AlignmentType type) {
    m_config.type = type;
    emit configChanged();
}

void AlignmentEngine::setRotationCenter(const QPointF& center) {
    m_config.rotationCenter.center = center;
    emit configChanged();
}

void AlignmentEngine::setPlatformType(Platform::PlatformType type) {
    m_config.platformType = type;
    emit configChanged();
}

// ============================================================
// 标定集成
// ============================================================

void AlignmentEngine::setCalibrationManager(CalibrationManager* manager) {
    m_calibManager = manager;
}

bool AlignmentEngine::hasCalibration() const {
    return m_calibManager != nullptr && m_calibManager->isCalibrated();
}

// ============================================================
// 模板点管理
// ============================================================

bool AlignmentEngine::setTemplatePoint(int index, const QPointF& worldPos, double angle) {
    if (index < 0) {
        return false;
    }

    // 自动扩展
    while (static_cast<int>(m_templatePoints.size()) <= index) {
        AlignmentPoint emptyPoint;
        emptyPoint.index = static_cast<int>(m_templatePoints.size());
        emptyPoint.isValid = false;
        m_templatePoints.push_back(emptyPoint);
    }

    m_templatePoints[index].templatePos = worldPos;
    m_templatePoints[index].templateAngle = angle;
    m_templatePoints[index].index = index;
    m_templatePoints[index].isValid = true;

    return true;
}

bool AlignmentEngine::setTemplatePointPixel(int index, const QPointF& pixelPos, double angle) {
    if (!hasCalibration()) {
        qWarning() << "Cannot convert pixel to world: no calibration";
        return setTemplatePoint(index, pixelPos, angle);  // 直接使用像素坐标
    }

    QPointF worldPos = m_calibManager->pixelToWorld(pixelPos);
    return setTemplatePoint(index, worldPos, angle);
}

bool AlignmentEngine::setTemplatePoint(int index, const Platform::CoordPosition& pos) {
    QPointF worldPos = pos.getMainPos();
    double angle = pos.getAngle();
    return setTemplatePoint(index, worldPos, angle);
}

const AlignmentPoint* AlignmentEngine::getTemplatePoint(int index) const {
    if (index < 0 || index >= static_cast<int>(m_templatePoints.size())) {
        return nullptr;
    }
    return &m_templatePoints[index];
}

void AlignmentEngine::clearTemplatePoints() {
    m_templatePoints.clear();
}

int AlignmentEngine::getTemplatePointCount() const {
    return static_cast<int>(m_templatePoints.size());
}

// ============================================================
// 测量点管理
// ============================================================

bool AlignmentEngine::setMeasuredPoint(int index, const QPointF& worldPos, double angle) {
    if (index < 0) {
        return false;
    }

    // 自动扩展
    while (static_cast<int>(m_measuredPoints.size()) <= index) {
        AlignmentPoint emptyPoint;
        emptyPoint.index = static_cast<int>(m_measuredPoints.size());
        emptyPoint.isValid = false;
        m_measuredPoints.push_back(emptyPoint);
    }

    m_measuredPoints[index].measuredPos = worldPos;
    m_measuredPoints[index].measuredAngle = angle;
    m_measuredPoints[index].index = index;
    m_measuredPoints[index].isValid = true;

    // 同步模板点的测量值
    if (index < static_cast<int>(m_templatePoints.size())) {
        m_templatePoints[index].measuredPos = worldPos;
        m_templatePoints[index].measuredAngle = angle;
    }

    return true;
}

bool AlignmentEngine::setMeasuredPointPixel(int index, const QPointF& pixelPos, double angle) {
    if (!hasCalibration()) {
        return setMeasuredPoint(index, pixelPos, angle);
    }

    QPointF worldPos = m_calibManager->pixelToWorld(pixelPos);
    return setMeasuredPoint(index, worldPos, angle);
}

bool AlignmentEngine::setMeasuredPoint(int index, const Platform::CoordPosition& pos) {
    QPointF worldPos = pos.getMainPos();
    double angle = pos.getAngle();
    return setMeasuredPoint(index, worldPos, angle);
}

const AlignmentPoint* AlignmentEngine::getMeasuredPoint(int index) const {
    if (index < 0 || index >= static_cast<int>(m_measuredPoints.size())) {
        return nullptr;
    }
    return &m_measuredPoints[index];
}

void AlignmentEngine::clearMeasuredPoints() {
    m_measuredPoints.clear();
}

int AlignmentEngine::getMeasuredPointCount() const {
    return static_cast<int>(m_measuredPoints.size());
}

// ============================================================
// 对齐计算
// ============================================================

AlignmentResult AlignmentEngine::calculate() {
    switch (m_config.type) {
        case AlignmentType::SinglePoint:
            return calculateSinglePoint();
        case AlignmentType::TwoPoint:
            return calculateTwoPoint();
        case AlignmentType::ThreePoint:
            return calculateThreePoint();
        case AlignmentType::FourPoint:
            return calculateFourPoint();
        default:
            m_result.success = false;
            m_result.error = CalibrationError::InvalidInput;
            return m_result;
    }
}

AlignmentResult AlignmentEngine::calculateSinglePoint() {
    m_result = AlignmentResult();
    m_result.type = AlignmentType::SinglePoint;

    // 验证点数
    if (m_templatePoints.empty() || m_measuredPoints.empty()) {
        m_result.error = CalibrationError::InsufficientPoints;
        return m_result;
    }

    if (!m_templatePoints[0].isValid || !m_measuredPoints[0].isValid) {
        m_result.error = CalibrationError::InvalidInput;
        return m_result;
    }

    // 单点对齐：仅计算平移偏差
    // 偏差 = 测量位置 - 模板位置
    const auto& tmpl = m_templatePoints[0];
    const auto& meas = m_measuredPoints[0];

    m_result.offset.dx = meas.measuredPos.x() - tmpl.templatePos.x();
    m_result.offset.dy = meas.measuredPos.y() - tmpl.templatePos.y();
    m_result.offset.dAngle = 0.0;  // 单点对齐不计算角度

    // 构建变换矩阵（纯平移）
    // 使用Eigen Matrix3d::Identity()创建单位矩阵
    m_result.transform = Matrix3d::Identity();
    m_result.transform(0, 2) = m_result.offset.dx;
    m_result.transform(1, 2) = m_result.offset.dy;

    m_result.confidence = calculateConfidence(m_result.offset);
    m_result.success = true;
    m_result.error = CalibrationError::Success;

    emit alignmentCompleted(true, m_result);
    return m_result;
}

AlignmentResult AlignmentEngine::calculateTwoPoint() {
    m_result = AlignmentResult();
    m_result.type = AlignmentType::TwoPoint;

    // 验证点数
    if (m_templatePoints.size() < 2 || m_measuredPoints.size() < 2) {
        m_result.error = CalibrationError::InsufficientPoints;
        return m_result;
    }

    if (!m_templatePoints[0].isValid || !m_templatePoints[1].isValid ||
        !m_measuredPoints[0].isValid || !m_measuredPoints[1].isValid) {
        m_result.error = CalibrationError::InvalidInput;
        return m_result;
    }

    const auto& tmpl0 = m_templatePoints[0];
    const auto& tmpl1 = m_templatePoints[1];
    const auto& meas0 = m_measuredPoints[0];
    const auto& meas1 = m_measuredPoints[1];

    // 计算两点连线角度
    double tmplAngle = calculateLineAngle(tmpl0.templatePos, tmpl1.templatePos);
    double measAngle = calculateLineAngle(meas0.measuredPos, meas1.measuredPos);

    // 角度偏差
    double dAngle = measAngle - tmplAngle;

    // 归一化角度到[-180, 180]
    while (dAngle > 180.0) dAngle -= 360.0;
    while (dAngle < -180.0) dAngle += 360.0;

    m_result.offset.dAngle = dAngle;

    // 计算平移偏差（考虑旋转中心）
    QPointF rotCenter = m_config.rotationCenter.center;

    if (m_config.compensateRotation && std::abs(dAngle) > 0.0001) {
        // 计算旋转导致的位移
        // 使用两点中心作为参考
        QPointF tmplCenter((tmpl0.templatePos.x() + tmpl1.templatePos.x()) / 2,
                           (tmpl0.templatePos.y() + tmpl1.templatePos.y()) / 2);
        QPointF measCenter((meas0.measuredPos.x() + meas1.measuredPos.x()) / 2,
                           (meas0.measuredPos.y() + meas1.measuredPos.y()) / 2);

        // 将模板中心绕旋转中心旋转后与测量中心比较
        QPointF rotatedTmplCenter = rotatePoint(tmplCenter, rotCenter, dAngle);

        m_result.offset.dx = measCenter.x() - rotatedTmplCenter.x();
        m_result.offset.dy = measCenter.y() - rotatedTmplCenter.y();
    } else {
        // 不补偿旋转时，直接计算中心点偏移
        QPointF tmplCenter((tmpl0.templatePos.x() + tmpl1.templatePos.x()) / 2,
                           (tmpl0.templatePos.y() + tmpl1.templatePos.y()) / 2);
        QPointF measCenter((meas0.measuredPos.x() + meas1.measuredPos.x()) / 2,
                           (meas0.measuredPos.y() + meas1.measuredPos.y()) / 2);

        m_result.offset.dx = measCenter.x() - tmplCenter.x();
        m_result.offset.dy = measCenter.y() - tmplCenter.y();
    }

    // 构建变换矩阵（旋转+平移）
    double rad = dAngle * M_PI / 180.0;
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);

    // 使用Eigen逗号初始化器构建变换矩阵
    m_result.transform << cosA, -sinA, m_result.offset.dx,
                          sinA,  cosA, m_result.offset.dy,
                          0,     0,    1;

    // 计算残差误差
    double err0 = (meas0.measuredPos - tmpl0.templatePos -
                   QPointF(m_result.offset.dx, m_result.offset.dy)).manhattanLength();
    double err1 = (meas1.measuredPos - tmpl1.templatePos -
                   QPointF(m_result.offset.dx, m_result.offset.dy)).manhattanLength();
    m_result.residualError = (err0 + err1) / 2.0;

    m_result.confidence = calculateConfidence(m_result.offset);
    m_result.success = true;
    m_result.error = CalibrationError::Success;

    emit alignmentCompleted(true, m_result);
    return m_result;
}

AlignmentResult AlignmentEngine::calculateThreePoint() {
    m_result = AlignmentResult();
    m_result.type = AlignmentType::ThreePoint;

    // 验证点数
    if (m_templatePoints.size() < 3 || m_measuredPoints.size() < 3) {
        m_result.error = CalibrationError::InsufficientPoints;
        return m_result;
    }

    // 提取有效点
    std::vector<QPointF> srcPoints, dstPoints;
    for (int i = 0; i < 3; ++i) {
        if (!m_templatePoints[i].isValid || !m_measuredPoints[i].isValid) {
            m_result.error = CalibrationError::InvalidInput;
            return m_result;
        }
        srcPoints.push_back(m_templatePoints[i].templatePos);
        dstPoints.push_back(m_measuredPoints[i].measuredPos);
    }

    // 计算仿射变换矩阵
    Matrix23d affine = computeAffineTransform(srcPoints, dstPoints);

    // 提取参数
    // Eigen::Matrix<double, 2, 3> 使用 (row, col) 访问
    double a = affine(0, 0);
    double b = affine(0, 1);
    double tx = affine(0, 2);
    double c = affine(1, 0);
    double d = affine(1, 1);
    double ty = affine(1, 2);

    // 计算缩放和旋转
    m_result.offset.scaleX = std::sqrt(a * a + c * c);
    m_result.offset.scaleY = std::sqrt(b * b + d * d);
    m_result.offset.dAngle = std::atan2(c, a) * 180.0 / M_PI;

    // 平移分量
    m_result.offset.dx = tx;
    m_result.offset.dy = ty;

    // 剪切
    m_result.offset.shear = (a * b + c * d) /
                            (m_result.offset.scaleX * m_result.offset.scaleY);

    // 构建齐次矩阵
    // 使用Eigen逗号初始化器
    m_result.transform << a,   b,   tx,
                          c,   d,   ty,
                          0,   0,   1;

    // 计算残差
    double totalError = 0;
    for (int i = 0; i < 3; ++i) {
        // 使用Eigen::Vector3d进行矩阵向量乘法
        Vector3d src(srcPoints[i].x(), srcPoints[i].y(), 1.0);
        Vector3d dst = m_result.transform * src;
        // Eigen::Vector3d使用(i)访问元素
        double dx = dst(0) - dstPoints[i].x();
        double dy = dst(1) - dstPoints[i].y();
        totalError += std::sqrt(dx * dx + dy * dy);
    }
    m_result.residualError = totalError / 3.0;

    m_result.confidence = calculateConfidence(m_result.offset);
    m_result.success = true;
    m_result.error = CalibrationError::Success;

    emit alignmentCompleted(true, m_result);
    return m_result;
}

AlignmentResult AlignmentEngine::calculateFourPoint() {
    m_result = AlignmentResult();
    m_result.type = AlignmentType::FourPoint;

    // 验证点数
    if (m_templatePoints.size() < 4 || m_measuredPoints.size() < 4) {
        m_result.error = CalibrationError::InsufficientPoints;
        return m_result;
    }

    // 四点对齐使用齐次变换（透视变换）
    // 这里简化为使用前4点的仿射变换（过约束最小二乘）
    // 完整实现应使用DLT算法

    std::vector<QPointF> srcPoints, dstPoints;
    for (int i = 0; i < 4; ++i) {
        if (!m_templatePoints[i].isValid || !m_measuredPoints[i].isValid) {
            m_result.error = CalibrationError::InvalidInput;
            return m_result;
        }
        srcPoints.push_back(m_templatePoints[i].templatePos);
        dstPoints.push_back(m_measuredPoints[i].measuredPos);
    }

    // 使用最小二乘法拟合仿射变换（过约束）
    // 构建方程组
    // 使用Eigen MatrixXd创建动态矩阵
    MatrixXd A(8, 6);
    VectorXd b(8);

    for (int i = 0; i < 4; ++i) {
        double sx = srcPoints[i].x();
        double sy = srcPoints[i].y();
        double dx = dstPoints[i].x();
        double dy = dstPoints[i].y();

        // Eigen::MatrixXd使用(row, col)访问
        A(2 * i, 0) = sx;
        A(2 * i, 1) = sy;
        A(2 * i, 2) = 1;
        A(2 * i, 3) = 0;
        A(2 * i, 4) = 0;
        A(2 * i, 5) = 0;
        b(2 * i) = dx;

        A(2 * i + 1, 0) = 0;
        A(2 * i + 1, 1) = 0;
        A(2 * i + 1, 2) = 0;
        A(2 * i + 1, 3) = sx;
        A(2 * i + 1, 4) = sy;
        A(2 * i + 1, 5) = 1;
        b(2 * i + 1) = dy;
    }

    // 使用Eigen JacobiSVD求解最小二乘
    Eigen::JacobiSVD<MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    VectorXd h = svd.solve(b);

    // 构建变换矩阵
    // 使用Eigen逗号初始化器
    m_result.transform << h(0), h(1), h(2),
                          h(3), h(4), h(5),
                          0,    0,    1;

    // 提取参数
    m_result.offset.dx = h(2);
    m_result.offset.dy = h(5);
    m_result.offset.scaleX = std::sqrt(h(0) * h(0) + h(3) * h(3));
    m_result.offset.scaleY = std::sqrt(h(1) * h(1) + h(4) * h(4));
    m_result.offset.dAngle = std::atan2(h(3), h(0)) * 180.0 / M_PI;

    // 计算残差
    double totalError = 0;
    for (int i = 0; i < 4; ++i) {
        Vector3d src(srcPoints[i].x(), srcPoints[i].y(), 1.0);
        Vector3d dst = m_result.transform * src;
        double dx = dst(0) - dstPoints[i].x();
        double dy = dst(1) - dstPoints[i].y();
        totalError += std::sqrt(dx * dx + dy * dy);
    }
    m_result.residualError = totalError / 4.0;

    m_result.confidence = calculateConfidence(m_result.offset);
    m_result.success = true;
    m_result.error = CalibrationError::Success;

    emit alignmentCompleted(true, m_result);
    return m_result;
}

AlignmentResult AlignmentEngine::calculate(
    const std::vector<AlignmentPoint>& templatePoints,
    const std::vector<AlignmentPoint>& measuredPoints) {

    // 临时替换内部点
    std::vector<AlignmentPoint> oldTemplate = m_templatePoints;
    std::vector<AlignmentPoint> oldMeasured = m_measuredPoints;

    m_templatePoints = templatePoints;
    m_measuredPoints = measuredPoints;

    AlignmentResult result = calculate();

    // 恢复
    m_templatePoints = oldTemplate;
    m_measuredPoints = oldMeasured;

    return result;
}

bool AlignmentEngine::isWithinTolerance() const {
    return m_result.offset.isWithinTolerance(m_config.positionTolerance,
                                              m_config.angleTolerance);
}

// ============================================================
// 平台运动转换
// ============================================================

Platform::PlatformAxisPos AlignmentEngine::toAxisMotion(
    const AlignmentOffset& offset) const {

    switch (m_config.platformType) {
        case Platform::PlatformType::XYD:
        case Platform::PlatformType::DXY:
            return toXYDMotion(offset, m_config.rotationCenter.center);

        case Platform::PlatformType::X1X2Y:
            // 需要平台配置信息
            return toXYDMotion(offset, m_config.rotationCenter.center);

        case Platform::PlatformType::XY:
            {
                Platform::PlatformAxisPos motion;
                motion.x1 = offset.dx;
                motion.y1 = offset.dy;
                motion.d = 0.0;
                return motion;
            }

        default:
            return toXYDMotion(offset, m_config.rotationCenter.center);
    }
}

Platform::PlatformAxisPos AlignmentEngine::toXYDMotion(
    const AlignmentOffset& offset, const QPointF& rotCenter) const {

    Platform::PlatformAxisPos motion;

    // 对于XYD平台：
    // 1. 先计算旋转补偿
    // 2. 再计算旋转后的平移补偿

    double dAngleRad = offset.dAngle * M_PI / 180.0;
    // 使用dAngleRad防止编译器警告（未使用变量）
    (void)dAngleRad;
    (void)rotCenter;

    if (std::abs(offset.dAngle) > 0.0001) {
        // 有旋转时，需要计算旋转中心到当前位置的位移补偿
        // 假设当前位置就是旋转中心，简化处理
        motion.x1 = offset.dx;
        motion.y1 = offset.dy;
        motion.d = offset.dAngle;
    } else {
        // 无旋转，纯平移
        motion.x1 = offset.dx;
        motion.y1 = offset.dy;
        motion.d = 0.0;
    }

    return motion;
}

Platform::PlatformAxisPos AlignmentEngine::toX1X2YMotion(
    const AlignmentOffset& offset,
    const Platform::PlatformX1X2YInfo* platformInfo) const {

    Platform::PlatformAxisPos motion;

    // X1X2Y龙门平台的运动分解
    // 需要根据平台配置计算X1和X2的运动量

    if (!platformInfo) {
        // 简化处理
        motion.x1 = offset.dx + offset.dAngle * 0.1;  // 假设旋转臂长0.1m
        motion.x2 = offset.dx - offset.dAngle * 0.1;
        motion.y1 = offset.dy;
        return motion;
    }

    // 使用平台配置转换
    double x, y, d;
    x = offset.dx;
    y = offset.dy;
    d = offset.dAngle;

    platformInfo->convertFromXYD(x, y, d, motion.x1, motion.x2, motion.y1);

    return motion;
}

Platform::PlatformAxisPos AlignmentEngine::getCompensatedPosition(
    const Platform::PlatformAxisPos& currentPos) const {

    if (!m_result.success) {
        return currentPos;
    }

    Platform::PlatformAxisPos motion = toAxisMotion(m_result.offset);

    // 补偿方向取反
    Platform::PlatformAxisPos targetPos;
    targetPos.x1 = currentPos.x1 - motion.x1;
    targetPos.x2 = currentPos.x2 - motion.x2;
    targetPos.y1 = currentPos.y1 - motion.y1;
    targetPos.y2 = currentPos.y2 - motion.y2;
    targetPos.d = currentPos.d - motion.d;
    targetPos.p = currentPos.p - motion.p;

    return targetPos;
}

// ============================================================
// 迭代优化
// ============================================================

AlignmentResult AlignmentEngine::iterativeAlign(
    MeasureCallback measureCallback, MoveCallback moveCallback) {

    for (int iter = 0; iter < m_config.maxIterations; ++iter) {
        // 获取测量点
        auto measuredPoints = measureCallback();
        if (measuredPoints.empty()) {
            m_result.success = false;
            m_result.error = CalibrationError::InvalidInput;
            return m_result;
        }

        // 更新测量点
        for (size_t i = 0; i < measuredPoints.size() && i < m_measuredPoints.size(); ++i) {
            m_measuredPoints[i] = measuredPoints[i];
        }

        // 计算对齐
        calculate();

        emit iterationProgress(iter + 1, m_config.maxIterations, m_result.offset);

        // 检查是否收敛
        if (isWithinTolerance()) {
            break;
        }

        // 应用校正（如果启用迭代校正）
        if (m_config.enableIterativeCorrection && iter < m_config.maxIterations - 1) {
            AlignmentOffset correction = calculateIterativeCorrection(m_result.offset, iter);
            Platform::PlatformAxisPos motion = toAxisMotion(correction);

            if (!moveCallback(motion)) {
                qWarning() << "Move failed at iteration" << iter;
                break;
            }
        }
    }

    return m_result;
}

AlignmentOffset AlignmentEngine::calculateIterativeCorrection(
    const AlignmentOffset& offset, int iteration) {

    // 使用递减增益防止过补偿
    double gain = m_config.correctionGain * std::pow(0.8, iteration);

    AlignmentOffset correction;
    correction.dx = -offset.dx * gain;
    correction.dy = -offset.dy * gain;
    correction.dAngle = -offset.dAngle * gain;

    return correction;
}

// ============================================================
// 辅助计算
// ============================================================

double AlignmentEngine::calculateLineAngle(const QPointF& p1, const QPointF& p2) {
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    return std::atan2(dy, dx) * 180.0 / M_PI;
}

QPointF AlignmentEngine::rotatePoint(const QPointF& point, const QPointF& center,
                                      double angleDeg) {
    double rad = angleDeg * M_PI / 180.0;
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);

    double dx = point.x() - center.x();
    double dy = point.y() - center.y();

    double rx = center.x() + dx * cosA - dy * sinA;
    double ry = center.y() + dx * sinA + dy * cosA;

    return QPointF(rx, ry);
}

QPointF AlignmentEngine::calculateRotationDisplacement(const QPointF& point,
                                                        const QPointF& center,
                                                        double angleDeg) {
    QPointF rotated = rotatePoint(point, center, angleDeg);
    return rotated - point;
}

Matrix23d AlignmentEngine::computeAffineTransform(const std::vector<QPointF>& srcPoints,
                                                   const std::vector<QPointF>& dstPoints) {
    // 使用3点计算仿射变换
    // [dx0]   [sx0 sy0 1 0   0   0] [a]
    // [dy0]   [0   0   0 sx0 sy0 1] [b]
    // [dx1] = [sx1 sy1 1 0   0   0] [tx]
    // [dy1]   [0   0   0 sx1 sy1 1] [c]
    // [dx2]   [sx2 sy2 1 0   0   0] [d]
    // [dy2]   [0   0   0 sx2 sy2 1] [ty]

    // 使用Eigen MatrixXd创建6x6矩阵
    MatrixXd A(6, 6);
    VectorXd b(6);

    for (int i = 0; i < 3; ++i) {
        double sx = srcPoints[i].x();
        double sy = srcPoints[i].y();
        double dx = dstPoints[i].x();
        double dy = dstPoints[i].y();

        // Eigen::MatrixXd使用(row, col)访问
        A(2 * i, 0) = sx;
        A(2 * i, 1) = sy;
        A(2 * i, 2) = 1;
        A(2 * i, 3) = 0;
        A(2 * i, 4) = 0;
        A(2 * i, 5) = 0;
        b(2 * i) = dx;

        A(2 * i + 1, 0) = 0;
        A(2 * i + 1, 1) = 0;
        A(2 * i + 1, 2) = 0;
        A(2 * i + 1, 3) = sx;
        A(2 * i + 1, 4) = sy;
        A(2 * i + 1, 5) = 1;
        b(2 * i + 1) = dy;
    }

    // 使用Eigen FullPivLU求解精确解
    Eigen::FullPivLU<MatrixXd> lu(A);
    VectorXd h = lu.solve(b);

    // 构建 Eigen::Matrix<double, 2, 3> 仿射矩阵
    Matrix23d affine;
    affine << h(0), h(1), h(2),
              h(3), h(4), h(5);

    return affine;
}

bool AlignmentEngine::validatePointCount() const {
    int required = 0;
    switch (m_config.type) {
        case AlignmentType::SinglePoint: required = 1; break;
        case AlignmentType::TwoPoint: required = 2; break;
        case AlignmentType::ThreePoint: required = 3; break;
        case AlignmentType::FourPoint: required = 4; break;
        default: return false;
    }

    return static_cast<int>(m_templatePoints.size()) >= required &&
           static_cast<int>(m_measuredPoints.size()) >= required;
}

std::vector<std::pair<AlignmentPoint, AlignmentPoint>>
AlignmentEngine::preparePointPairs() const {

    std::vector<std::pair<AlignmentPoint, AlignmentPoint>> pairs;
    int count = std::min(m_templatePoints.size(), m_measuredPoints.size());

    for (int i = 0; i < count; ++i) {
        if (m_templatePoints[i].isValid && m_measuredPoints[i].isValid) {
            pairs.emplace_back(m_templatePoints[i], m_measuredPoints[i]);
        }
    }

    return pairs;
}

double AlignmentEngine::calculateConfidence(const AlignmentOffset& offset) const {
    // 基于偏差大小计算置信度
    double distError = offset.getTranslationDistance();
    double angleError = std::abs(offset.dAngle);

    // 假设最大允许偏差
    double maxDist = 10.0;  // mm
    double maxAngle = 10.0; // degree

    double distConf = std::max(0.0, 1.0 - distError / maxDist);
    double angleConf = std::max(0.0, 1.0 - angleError / maxAngle);

    return (distConf + angleConf) / 2.0;
}

// ============================================================
// 持久化
// ============================================================

bool AlignmentEngine::saveTemplatePoints(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonObject json;
    json["config"] = m_config.toJson();

    QJsonArray pointsArray;
    for (const auto& pt : m_templatePoints) {
        pointsArray.append(pt.toJson());
    }
    json["templatePoints"] = pointsArray;

    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool AlignmentEngine::loadTemplatePoints(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return false;
    }

    return importFromJson(doc.object());
}

QJsonObject AlignmentEngine::exportToJson() const {
    QJsonObject json;
    json["config"] = m_config.toJson();

    QJsonArray templateArray;
    for (const auto& pt : m_templatePoints) {
        templateArray.append(pt.toJson());
    }
    json["templatePoints"] = templateArray;

    QJsonArray measuredArray;
    for (const auto& pt : m_measuredPoints) {
        measuredArray.append(pt.toJson());
    }
    json["measuredPoints"] = measuredArray;

    json["result"] = m_result.toJson();

    return json;
}

bool AlignmentEngine::importFromJson(const QJsonObject& json) {
    if (json.contains("config")) {
        m_config = AlignmentConfig::fromJson(json["config"].toObject());
    }

    if (json.contains("templatePoints")) {
        m_templatePoints.clear();
        for (const auto& item : json["templatePoints"].toArray()) {
            m_templatePoints.push_back(AlignmentPoint::fromJson(item.toObject()));
        }
    }

    if (json.contains("measuredPoints")) {
        m_measuredPoints.clear();
        for (const auto& item : json["measuredPoints"].toArray()) {
            m_measuredPoints.push_back(AlignmentPoint::fromJson(item.toObject()));
        }
    }

    if (json.contains("result")) {
        m_result = AlignmentResult::fromJson(json["result"].toObject());
    }

    emit configChanged();
    return true;
}

// ============================================================
// AlignmentEngineFactory 实现
// ============================================================

AlignmentEngine* AlignmentEngineFactory::getInstance(int platformIndex) {
    auto it = s_instances.find(platformIndex);
    if (it == s_instances.end()) {
        s_instances[platformIndex] = std::make_unique<AlignmentEngine>();
    }
    return s_instances[platformIndex].get();
}

std::unique_ptr<AlignmentEngine> AlignmentEngineFactory::create() {
    return std::make_unique<AlignmentEngine>();
}

void AlignmentEngineFactory::releaseInstance(int platformIndex) {
    s_instances.erase(platformIndex);
}

void AlignmentEngineFactory::releaseAll() {
    s_instances.clear();
}

} // namespace Calibration
} // namespace VisionForge
