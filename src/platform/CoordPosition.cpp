/**
 * @file CoordPosition.cpp
 * @brief VisionForge Pro 核心坐标位姿类实现
 *
 * 实现CoordPosition类的所有功能，包括：
 * - 坐标变换（map/invMap）
 * - 平台轴位置转换
 * - 序列化/反序列化
 *
 * 核心算法参考VisionASM的CCoordPos实现。
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include "platform/CoordPosition.h"
#include <QJsonArray>
#include <cmath>
#include <algorithm>

namespace VisionForge {
namespace Platform {

// ============ AffineRect 实现 ============

QRectF AffineRect::toBoundingRect() const {
    auto corners = getCorners();
    double minX = corners[0].x(), maxX = corners[0].x();
    double minY = corners[0].y(), maxY = corners[0].y();

    for (int i = 1; i < 4; ++i) {
        minX = std::min(minX, corners[i].x());
        maxX = std::max(maxX, corners[i].x());
        minY = std::min(minY, corners[i].y());
        maxY = std::max(maxY, corners[i].y());
    }

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

std::array<QPointF, 4> AffineRect::getCorners() const {
    // 计算旋转后的四个角点
    double rad = angle * M_PI / 180.0;
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);

    double hw = width / 2.0;
    double hh = height / 2.0;

    // 四个角点相对于中心的偏移（未旋转）
    std::array<QPointF, 4> offsets = {
        QPointF(-hw, -hh),  // 左上
        QPointF(hw, -hh),   // 右上
        QPointF(hw, hh),    // 右下
        QPointF(-hw, hh)    // 左下
    };

    std::array<QPointF, 4> corners;
    for (int i = 0; i < 4; ++i) {
        // 应用旋转
        double x = offsets[i].x() * cosA - offsets[i].y() * sinA;
        double y = offsets[i].x() * sinA + offsets[i].y() * cosA;
        corners[i] = QPointF(centerX + x, centerY + y);
    }

    return corners;
}

QJsonObject AffineRect::toJson() const {
    QJsonObject json;
    json["centerX"] = centerX;
    json["centerY"] = centerY;
    json["width"] = width;
    json["height"] = height;
    json["angle"] = angle;
    return json;
}

AffineRect AffineRect::fromJson(const QJsonObject& json) {
    AffineRect rect;
    rect.centerX = json.value("centerX").toDouble(0.0);
    rect.centerY = json.value("centerY").toDouble(0.0);
    rect.width = json.value("width").toDouble(100.0);
    rect.height = json.value("height").toDouble(100.0);
    rect.angle = json.value("angle").toDouble(0.0);
    return rect;
}

// ============ CoordPosition 实现 ============

CoordPosition::CoordPosition() {
    m_auxX.fill(0.0);
    m_auxY.fill(0.0);
    m_auxAngle.fill(0.0);
}

CoordPosition::CoordPosition(double x, double y, double angle)
    : m_mainX(x), m_mainY(y), m_angle(angle) {
    m_auxX.fill(0.0);
    m_auxY.fill(0.0);
    m_auxAngle.fill(0.0);
}

CoordPosition::CoordPosition(double x, double y, double angle,
                             double lineAngle1, double lineAngle2)
    : m_mainX(x), m_mainY(y), m_angle(angle),
      m_lineAngle1(lineAngle1), m_lineAngle2(lineAngle2),
      m_foundLine1(true), m_foundLine2(true) {
    m_auxX.fill(0.0);
    m_auxY.fill(0.0);
    m_auxAngle.fill(0.0);
}

CoordPosition::CoordPosition(const CoordPosition& other) {
    m_mainX = other.m_mainX;
    m_mainY = other.m_mainY;
    m_angle = other.m_angle;
    m_auxX = other.m_auxX;
    m_auxY = other.m_auxY;
    m_auxAngle = other.m_auxAngle;
    m_lineAngle1 = other.m_lineAngle1;
    m_lineAngle2 = other.m_lineAngle2;
    m_foundLine1 = other.m_foundLine1;
    m_foundLine2 = other.m_foundLine2;
    m_searchRect = other.m_searchRect;
    m_foundSearchRect = other.m_foundSearchRect;
}

CoordPosition& CoordPosition::operator=(const CoordPosition& other) {
    if (this != &other) {
        m_mainX = other.m_mainX;
        m_mainY = other.m_mainY;
        m_angle = other.m_angle;
        m_auxX = other.m_auxX;
        m_auxY = other.m_auxY;
        m_auxAngle = other.m_auxAngle;
        m_lineAngle1 = other.m_lineAngle1;
        m_lineAngle2 = other.m_lineAngle2;
        m_foundLine1 = other.m_foundLine1;
        m_foundLine2 = other.m_foundLine2;
        m_searchRect = other.m_searchRect;
        m_foundSearchRect = other.m_foundSearchRect;
    }
    return *this;
}

// ============ 辅助位置访问 ============

double CoordPosition::getAuxX(int index) const {
    if (index >= 0 && index < MaxAuxiliaryPosNum) {
        return m_auxX[index];
    }
    return 0.0;
}

double CoordPosition::getAuxY(int index) const {
    if (index >= 0 && index < MaxAuxiliaryPosNum) {
        return m_auxY[index];
    }
    return 0.0;
}

double CoordPosition::getAuxAngle(int index) const {
    if (index >= 0 && index < MaxAuxiliaryPosNum) {
        return m_auxAngle[index];
    }
    return 0.0;
}

QPointF CoordPosition::getAuxPos(int index) const {
    if (index >= 0 && index < MaxAuxiliaryPosNum) {
        return QPointF(m_auxX[index], m_auxY[index]);
    }
    return QPointF(0, 0);
}

bool CoordPosition::setAuxPos(int index, double x, double y) {
    if (index < 0 || index >= MaxAuxiliaryPosNum) {
        return false;
    }
    m_auxX[index] = x;
    m_auxY[index] = y;
    return true;
}

bool CoordPosition::setAuxPosAngle(int index, double x, double y, double angle) {
    if (index < 0 || index >= MaxAuxiliaryPosNum) {
        return false;
    }
    m_auxX[index] = x;
    m_auxY[index] = y;
    m_auxAngle[index] = angle;
    return true;
}

// ============ 位置偏移 ============

void CoordPosition::offset(double offsetX, double offsetY) {
    m_mainX += offsetX;
    m_mainY += offsetY;

    for (int i = 0; i < MaxAuxiliaryPosNum; ++i) {
        m_auxX[i] += offsetX;
        m_auxY[i] += offsetY;
    }
}

// ============ 坐标变换 ============

QPointF CoordPosition::transformPoint(const QPointF& point,
                                       const QMatrix3x3& matrix,
                                       const QPointF& translation) {
    // P' = M * P + T
    // 提取2x2部分进行变换
    double x = matrix(0, 0) * point.x() + matrix(0, 1) * point.y() + translation.x();
    double y = matrix(1, 0) * point.x() + matrix(1, 1) * point.y() + translation.y();
    return QPointF(x, y);
}

bool CoordPosition::invertMatrix2x2(const QMatrix3x3& matrix, QMatrix3x3& invMatrix) {
    // 计算2x2矩阵的行列式
    double det = matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);

    if (std::abs(det) < 1e-10) {
        return false;  // 矩阵奇异，无法求逆
    }

    double invDet = 1.0 / det;

    // 计算逆矩阵
    invMatrix(0, 0) = matrix(1, 1) * invDet;
    invMatrix(0, 1) = -matrix(0, 1) * invDet;
    invMatrix(1, 0) = -matrix(1, 0) * invDet;
    invMatrix(1, 1) = matrix(0, 0) * invDet;
    invMatrix(2, 2) = 1.0;

    return true;
}

CoordPosition CoordPosition::map(const QMatrix3x3& matrix,
                                  const QPointF& translation) const {
    CoordPosition result;

    // 变换主位置
    QPointF mainPos = transformPoint(getMainPos(), matrix, translation);
    result.setMainPos(mainPos.x(), mainPos.y());
    result.setAngle(m_angle);  // 角度不变（除非需要考虑旋转变换）

    // 变换辅助位置
    for (int i = 0; i < MaxAuxiliaryPosNum; ++i) {
        QPointF auxPos = transformPoint(getAuxPos(i), matrix, translation);
        result.setAuxPosAngle(i, auxPos.x(), auxPos.y(), m_auxAngle[i]);
    }

    // 复制其他属性
    result.m_lineAngle1 = m_lineAngle1;
    result.m_lineAngle2 = m_lineAngle2;
    result.m_foundLine1 = m_foundLine1;
    result.m_foundLine2 = m_foundLine2;
    result.m_searchRect = m_searchRect;
    result.m_foundSearchRect = m_foundSearchRect;

    return result;
}

CoordPosition CoordPosition::mapHomography(const double h[9]) const {
    CoordPosition result;

    // 使用齐次坐标变换主位置
    auto transformHomography = [&h](double x, double y) -> QPointF {
        double w = h[6] * x + h[7] * y + h[8];
        if (std::abs(w) < 1e-10) {
            w = 1e-10;
        }
        double xNew = (h[0] * x + h[1] * y + h[2]) / w;
        double yNew = (h[3] * x + h[4] * y + h[5]) / w;
        return QPointF(xNew, yNew);
    };

    // 变换主位置
    QPointF mainPos = transformHomography(m_mainX, m_mainY);
    result.setMainPos(mainPos.x(), mainPos.y());
    result.setAngle(m_angle);

    // 变换辅助位置
    for (int i = 0; i < MaxAuxiliaryPosNum; ++i) {
        QPointF auxPos = transformHomography(m_auxX[i], m_auxY[i]);
        result.setAuxPosAngle(i, auxPos.x(), auxPos.y(), m_auxAngle[i]);
    }

    // 复制其他属性
    result.m_lineAngle1 = m_lineAngle1;
    result.m_lineAngle2 = m_lineAngle2;
    result.m_foundLine1 = m_foundLine1;
    result.m_foundLine2 = m_foundLine2;
    result.m_searchRect = m_searchRect;
    result.m_foundSearchRect = m_foundSearchRect;

    return result;
}

CoordPosition CoordPosition::invMap(const QMatrix3x3& matrix,
                                     const QPointF& translation) const {
    QMatrix3x3 invMatrix;
    if (!invertMatrix2x2(matrix, invMatrix)) {
        qWarning() << "CoordPosition::invMap: Matrix is singular, cannot invert";
        return *this;
    }

    // P = M^(-1) * (P' - T)
    CoordPosition result;

    // 逆变换主位置
    QPointF shiftedMain(m_mainX - translation.x(), m_mainY - translation.y());
    QPointF mainPos = transformPoint(shiftedMain, invMatrix, QPointF(0, 0));
    result.setMainPos(mainPos.x(), mainPos.y());
    result.setAngle(m_angle);

    // 逆变换辅助位置
    for (int i = 0; i < MaxAuxiliaryPosNum; ++i) {
        QPointF shiftedAux(m_auxX[i] - translation.x(), m_auxY[i] - translation.y());
        QPointF auxPos = transformPoint(shiftedAux, invMatrix, QPointF(0, 0));
        result.setAuxPosAngle(i, auxPos.x(), auxPos.y(), m_auxAngle[i]);
    }

    // 复制其他属性
    result.m_lineAngle1 = m_lineAngle1;
    result.m_lineAngle2 = m_lineAngle2;
    result.m_foundLine1 = m_foundLine1;
    result.m_foundLine2 = m_foundLine2;
    result.m_searchRect = m_searchRect;
    result.m_foundSearchRect = m_foundSearchRect;

    return result;
}

CoordPosition CoordPosition::invMapHomography(const double h[9]) const {
    // 计算3x3齐次矩阵的逆
    // 使用伴随矩阵法
    double det = h[0] * (h[4] * h[8] - h[5] * h[7])
               - h[1] * (h[3] * h[8] - h[5] * h[6])
               + h[2] * (h[3] * h[7] - h[4] * h[6]);

    if (std::abs(det) < 1e-10) {
        qWarning() << "CoordPosition::invMapHomography: Matrix is singular";
        return *this;
    }

    double invDet = 1.0 / det;
    double invH[9];

    invH[0] = (h[4] * h[8] - h[5] * h[7]) * invDet;
    invH[1] = (h[2] * h[7] - h[1] * h[8]) * invDet;
    invH[2] = (h[1] * h[5] - h[2] * h[4]) * invDet;
    invH[3] = (h[5] * h[6] - h[3] * h[8]) * invDet;
    invH[4] = (h[0] * h[8] - h[2] * h[6]) * invDet;
    invH[5] = (h[2] * h[3] - h[0] * h[5]) * invDet;
    invH[6] = (h[3] * h[7] - h[4] * h[6]) * invDet;
    invH[7] = (h[1] * h[6] - h[0] * h[7]) * invDet;
    invH[8] = (h[0] * h[4] - h[1] * h[3]) * invDet;

    return mapHomography(invH);
}

// ============ 平台轴位置转换 ============

PlatformAxisPos CoordPosition::toPlatformAxis(PlatformType type,
                                               const void* platformInfo) const {
    PlatformAxisPos axisPos;

    switch (type) {
        case PlatformType::XYD:
            // 标准XYD平台：直接映射
            axisPos.x1 = m_mainX;
            axisPos.y1 = m_mainY;
            axisPos.d = m_angle;
            break;

        case PlatformType::XY:
            // XY平台：无旋转
            axisPos.x1 = m_mainX;
            axisPos.y1 = m_mainY;
            break;

        case PlatformType::DXY:
            // DXY平台：旋转优先，需要根据旋转计算XY
            // 注意：DXY平台中，XY是在旋转后的坐标系中的
            axisPos.d = m_angle;
            // 将世界坐标转换为旋转后的局部坐标
            {
                double rad = -m_angle * M_PI / 180.0;
                double cosA = std::cos(rad);
                double sinA = std::sin(rad);
                axisPos.x1 = m_mainX * cosA - m_mainY * sinA;
                axisPos.y1 = m_mainX * sinA + m_mainY * cosA;
            }
            break;

        case PlatformType::X1X2Y:
            // 龙门双X平台：需要从XY+角度计算X1X2
            // 这里需要平台配置信息来进行精确计算
            // 简化处理：假设对称结构
            {
                // 简化模型：X1和X2的差值产生旋转
                // 实际应该根据platformInfo中的几何参数计算
                double halfAngleOffset = m_angle * 0.5;  // 简化处理
                axisPos.x1 = m_mainX - halfAngleOffset;
                axisPos.x2 = m_mainX + halfAngleOffset;
                axisPos.y1 = m_mainY;
            }
            break;

        case PlatformType::XY1Y2:
            // 双Y平台
            {
                double halfAngleOffset = m_angle * 0.5;
                axisPos.y1 = m_mainY - halfAngleOffset;
                axisPos.y2 = m_mainY + halfAngleOffset;
                axisPos.x1 = m_mainX;
            }
            break;

        case PlatformType::X:
            axisPos.x1 = m_mainX;
            break;

        case PlatformType::Y:
            axisPos.y1 = m_mainY;
            break;

        case PlatformType::XD:
            axisPos.x1 = m_mainX;
            axisPos.d = m_angle;
            break;

        case PlatformType::YD:
            axisPos.y1 = m_mainY;
            axisPos.d = m_angle;
            break;

        case PlatformType::XYPD:
        case PlatformType::XDPY:
        case PlatformType::XDPY1:
            axisPos.x1 = m_mainX;
            axisPos.y1 = m_mainY;
            axisPos.d = m_angle;
            break;

        case PlatformType::XYDP:
            axisPos.x1 = m_mainX;
            axisPos.y1 = m_mainY;
            axisPos.d = m_angle;
            // P轴通常从辅助位置或其他来源获取
            break;

        case PlatformType::UVW:
            // UVW平台通常需要特殊的运动学计算
            axisPos.u = m_mainX;
            axisPos.v = m_mainY;
            axisPos.w = m_angle;
            break;

        default:
            axisPos.x1 = m_mainX;
            axisPos.y1 = m_mainY;
            axisPos.d = m_angle;
            break;
    }

    return axisPos;
}

CoordPosition CoordPosition::fromPlatformAxis(const PlatformAxisPos& axisPos,
                                               PlatformType type,
                                               const void* platformInfo) {
    CoordPosition pos;

    switch (type) {
        case PlatformType::XYD:
            pos.setMainPosAngle(axisPos.x1, axisPos.y1, axisPos.d);
            break;

        case PlatformType::XY:
            pos.setMainPos(axisPos.x1, axisPos.y1);
            break;

        case PlatformType::DXY:
            // 将局部坐标转换回世界坐标
            {
                double rad = axisPos.d * M_PI / 180.0;
                double cosA = std::cos(rad);
                double sinA = std::sin(rad);
                double x = axisPos.x1 * cosA - axisPos.y1 * sinA;
                double y = axisPos.x1 * sinA + axisPos.y1 * cosA;
                pos.setMainPosAngle(x, y, axisPos.d);
            }
            break;

        case PlatformType::X1X2Y:
            // 从X1X2Y计算XYD
            {
                double x = (axisPos.x1 + axisPos.x2) / 2.0;
                double angle = axisPos.x2 - axisPos.x1;  // 简化计算
                pos.setMainPosAngle(x, axisPos.y1, angle);
            }
            break;

        case PlatformType::XY1Y2:
            {
                double y = (axisPos.y1 + axisPos.y2) / 2.0;
                double angle = axisPos.y2 - axisPos.y1;
                pos.setMainPosAngle(axisPos.x1, y, angle);
            }
            break;

        case PlatformType::X:
            pos.setMainX(axisPos.x1);
            break;

        case PlatformType::Y:
            pos.setMainY(axisPos.y1);
            break;

        case PlatformType::UVW:
            pos.setMainPosAngle(axisPos.u, axisPos.v, axisPos.w);
            break;

        default:
            pos.setMainPosAngle(axisPos.x1, axisPos.y1, axisPos.d);
            break;
    }

    return pos;
}

// ============ 运算符 ============

bool CoordPosition::operator==(const CoordPosition& other) const {
    constexpr double eps = 1e-9;

    if (std::abs(m_mainX - other.m_mainX) > eps ||
        std::abs(m_mainY - other.m_mainY) > eps ||
        std::abs(m_angle - other.m_angle) > eps) {
        return false;
    }

    if (std::abs(m_lineAngle1 - other.m_lineAngle1) > eps ||
        std::abs(m_lineAngle2 - other.m_lineAngle2) > eps) {
        return false;
    }

    if (m_foundLine1 != other.m_foundLine1 ||
        m_foundLine2 != other.m_foundLine2 ||
        m_foundSearchRect != other.m_foundSearchRect) {
        return false;
    }

    for (int i = 0; i < MaxAuxiliaryPosNum; ++i) {
        if (std::abs(m_auxX[i] - other.m_auxX[i]) > eps ||
            std::abs(m_auxY[i] - other.m_auxY[i]) > eps ||
            std::abs(m_auxAngle[i] - other.m_auxAngle[i]) > eps) {
            return false;
        }
    }

    return true;
}

// ============ 序列化 ============

QJsonObject CoordPosition::toJson() const {
    QJsonObject json;

    // 主位置
    json["mainX"] = m_mainX;
    json["mainY"] = m_mainY;
    json["angle"] = m_angle;

    // 辅助位置
    QJsonArray auxXArray, auxYArray, auxAngleArray;
    for (int i = 0; i < MaxAuxiliaryPosNum; ++i) {
        auxXArray.append(m_auxX[i]);
        auxYArray.append(m_auxY[i]);
        auxAngleArray.append(m_auxAngle[i]);
    }
    json["auxX"] = auxXArray;
    json["auxY"] = auxYArray;
    json["auxAngle"] = auxAngleArray;

    // 线角度
    json["lineAngle1"] = m_lineAngle1;
    json["lineAngle2"] = m_lineAngle2;
    json["foundLine1"] = m_foundLine1;
    json["foundLine2"] = m_foundLine2;

    // 搜索区域
    json["searchRect"] = m_searchRect.toJson();
    json["foundSearchRect"] = m_foundSearchRect;

    return json;
}

CoordPosition CoordPosition::fromJson(const QJsonObject& json) {
    CoordPosition pos;

    // 主位置
    pos.m_mainX = json.value("mainX").toDouble(0.0);
    pos.m_mainY = json.value("mainY").toDouble(0.0);
    pos.m_angle = json.value("angle").toDouble(0.0);

    // 辅助位置
    QJsonArray auxXArray = json.value("auxX").toArray();
    QJsonArray auxYArray = json.value("auxY").toArray();
    QJsonArray auxAngleArray = json.value("auxAngle").toArray();

    for (int i = 0; i < MaxAuxiliaryPosNum && i < auxXArray.size(); ++i) {
        pos.m_auxX[i] = auxXArray[i].toDouble(0.0);
        pos.m_auxY[i] = auxYArray[i].toDouble(0.0);
        pos.m_auxAngle[i] = auxAngleArray[i].toDouble(0.0);
    }

    // 线角度
    pos.m_lineAngle1 = json.value("lineAngle1").toDouble(0.0);
    pos.m_lineAngle2 = json.value("lineAngle2").toDouble(0.0);
    pos.m_foundLine1 = json.value("foundLine1").toBool(false);
    pos.m_foundLine2 = json.value("foundLine2").toBool(false);

    // 搜索区域
    pos.m_searchRect = AffineRect::fromJson(json.value("searchRect").toObject());
    pos.m_foundSearchRect = json.value("foundSearchRect").toBool(false);

    return pos;
}

QString CoordPosition::toString() const {
    return QString("CoordPos[Main:(%1,%2) Angle:%3 Line1:%4 Line2:%5]")
        .arg(m_mainX, 0, 'f', 3)
        .arg(m_mainY, 0, 'f', 3)
        .arg(m_angle, 0, 'f', 3)
        .arg(m_lineAngle1, 0, 'f', 3)
        .arg(m_lineAngle2, 0, 'f', 3);
}

// ============ 工具方法 ============

bool CoordPosition::isZero(double tolerance) const {
    return std::abs(m_mainX) < tolerance &&
           std::abs(m_mainY) < tolerance &&
           std::abs(m_angle) < tolerance;
}

double CoordPosition::distance(const CoordPosition& pos1, const CoordPosition& pos2) {
    double dx = pos1.m_mainX - pos2.m_mainX;
    double dy = pos1.m_mainY - pos2.m_mainY;
    return std::sqrt(dx * dx + dy * dy);
}

double CoordPosition::lineAngle(const CoordPosition& pos1, const CoordPosition& pos2) {
    double dx = pos2.m_mainX - pos1.m_mainX;
    double dy = pos2.m_mainY - pos1.m_mainY;
    return std::atan2(dy, dx) * 180.0 / M_PI;
}

} // namespace Platform
} // namespace VisionForge
