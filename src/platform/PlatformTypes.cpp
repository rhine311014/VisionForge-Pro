/**
 * @file PlatformTypes.cpp
 * @brief VisionForge Pro 平台类型实现
 *
 * 实现PlatformTypes.h中定义的平台类型相关功能。
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include "platform/PlatformTypes.h"
#include <QJsonArray>
#include <QStringList>
#include <QRegularExpression>
#include <cmath>

namespace VisionForge {
namespace Platform {

// ============ PlatformAxisPos 实现 ============

QString PlatformAxisPos::toString() const {
    return QString("X1:%1, X2:%2, Y1:%3, Y2:%4, D:%5, P:%6")
        .arg(x1)
        .arg(x2)
        .arg(y1)
        .arg(y2)
        .arg(d)
        .arg(p);
}

PlatformAxisPos PlatformAxisPos::fromString(const QString& str) {
    PlatformAxisPos pos;

    // 解析格式: "X1:value, X2:value, Y1:value, Y2:value, D:value, P:value"
    QRegularExpression rx(R"((\w+):\s*([-+]?\d*\.?\d+))");
    QRegularExpressionMatchIterator it = rx.globalMatch(str);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString name = match.captured(1).toUpper();
        double value = match.captured(2).toDouble();

        if (name == "X1" || name == "X") {
            pos.x1 = value;
        } else if (name == "X2") {
            pos.x2 = value;
        } else if (name == "Y1" || name == "Y") {
            pos.y1 = value;
        } else if (name == "Y2") {
            pos.y2 = value;
        } else if (name == "D") {
            pos.d = value;
        } else if (name == "P") {
            pos.p = value;
        } else if (name == "U") {
            pos.u = value;
        } else if (name == "V") {
            pos.v = value;
        } else if (name == "W") {
            pos.w = value;
        }
    }

    return pos;
}

QJsonObject PlatformAxisPos::toJson() const {
    QJsonObject json;
    json["x1"] = x1;
    json["x2"] = x2;
    json["y1"] = y1;
    json["y2"] = y2;
    json["d"] = d;
    json["p"] = p;
    json["u"] = u;
    json["v"] = v;
    json["w"] = w;
    return json;
}

PlatformAxisPos PlatformAxisPos::fromJson(const QJsonObject& json) {
    PlatformAxisPos pos;
    pos.x1 = json.value("x1").toDouble(0.0);
    pos.x2 = json.value("x2").toDouble(0.0);
    pos.y1 = json.value("y1").toDouble(0.0);
    pos.y2 = json.value("y2").toDouble(0.0);
    pos.d = json.value("d").toDouble(0.0);
    pos.p = json.value("p").toDouble(0.0);
    pos.u = json.value("u").toDouble(0.0);
    pos.v = json.value("v").toDouble(0.0);
    pos.w = json.value("w").toDouble(0.0);
    return pos;
}

// ============ 平台类型工具函数实现 ============

QString getPlatformTypeName(PlatformType type) {
    switch (type) {
        case PlatformType::Unknown:
            return QStringLiteral("未知");
        case PlatformType::XYD:
            return QStringLiteral("XYD三轴平台");
        case PlatformType::X1X2Y:
            return QStringLiteral("X1X2Y龙门平台");
        case PlatformType::XY1Y2:
            return QStringLiteral("XY1Y2双Y平台");
        case PlatformType::XY:
            return QStringLiteral("XY双轴平台");
        case PlatformType::YX1X2PD:
            return QStringLiteral("YX1X2PD复合平台");
        case PlatformType::YDPX1X2:
            return QStringLiteral("YDPX1X2复合平台");
        case PlatformType::DXY:
            return QStringLiteral("DXY旋转优先平台");
        case PlatformType::XD:
            return QStringLiteral("XD双轴平台");
        case PlatformType::YD:
            return QStringLiteral("YD双轴平台");
        case PlatformType::XDPY:
            return QStringLiteral("XDPY分离平台");
        case PlatformType::XDPY1:
            return QStringLiteral("XDPY1分离平台");
        case PlatformType::XYPD:
            return QStringLiteral("XYPD分离平台");
        case PlatformType::X:
            return QStringLiteral("X单轴平台");
        case PlatformType::Y:
            return QStringLiteral("Y单轴平台");
        case PlatformType::XYDP:
            return QStringLiteral("XYDP四轴平台");
        // UVW精密对位平台系列
        case PlatformType::UVW:
            return QStringLiteral("UVW精密平台");
        case PlatformType::UVW_XXY:
            return QStringLiteral("UVW-XXY平台 (双X单Y)");
        case PlatformType::UVW_XYY:
            return QStringLiteral("UVW-XYY平台 (单X双Y)");
        case PlatformType::UVW_XXYY:
            return QStringLiteral("UVW-XXYY四轴平台 (双X双Y)");
        case PlatformType::UVW_HOLLOW:
            return QStringLiteral("UVW中空平台 (光学)");
        case PlatformType::UVW_HIGH_PRECISION:
            return QStringLiteral("UVW高精度平台 (2μm)");
        default:
            return QStringLiteral("未知类型");
    }
}

PlatformType getPlatformTypeFromName(const QString& name) {
    // 支持中英文名称
    static const QMap<QString, PlatformType> nameMap = {
        // 英文标识
        {"XYD", PlatformType::XYD},
        {"X1X2Y", PlatformType::X1X2Y},
        {"XY1Y2", PlatformType::XY1Y2},
        {"XY", PlatformType::XY},
        {"YX1X2PD", PlatformType::YX1X2PD},
        {"YDPX1X2", PlatformType::YDPX1X2},
        {"DXY", PlatformType::DXY},
        {"XD", PlatformType::XD},
        {"YD", PlatformType::YD},
        {"XDPY", PlatformType::XDPY},
        {"XDPY1", PlatformType::XDPY1},
        {"XYPD", PlatformType::XYPD},
        {"X", PlatformType::X},
        {"Y", PlatformType::Y},
        {"XYDP", PlatformType::XYDP},
        {"UVW", PlatformType::UVW},
        {"UVW_XXY", PlatformType::UVW_XXY},
        {"UVW_XYY", PlatformType::UVW_XYY},
        {"UVW_XXYY", PlatformType::UVW_XXYY},
        {"UVW_HOLLOW", PlatformType::UVW_HOLLOW},
        {"UVW_HIGH_PRECISION", PlatformType::UVW_HIGH_PRECISION},
        // 中文名称
        {QStringLiteral("XYD三轴平台"), PlatformType::XYD},
        {QStringLiteral("X1X2Y龙门平台"), PlatformType::X1X2Y},
        {QStringLiteral("XY1Y2双Y平台"), PlatformType::XY1Y2},
        {QStringLiteral("XY双轴平台"), PlatformType::XY},
        {QStringLiteral("DXY旋转优先平台"), PlatformType::DXY},
        {QStringLiteral("UVW精密平台"), PlatformType::UVW},
        {QStringLiteral("UVW-XXY平台"), PlatformType::UVW_XXY},
        {QStringLiteral("UVW-XYY平台"), PlatformType::UVW_XYY},
        {QStringLiteral("UVW-XXYY四轴平台"), PlatformType::UVW_XXYY},
        {QStringLiteral("UVW中空平台"), PlatformType::UVW_HOLLOW},
        {QStringLiteral("UVW高精度平台"), PlatformType::UVW_HIGH_PRECISION},
    };

    QString upperName = name.toUpper().trimmed();

    // 先尝试精确匹配
    if (nameMap.contains(name)) {
        return nameMap[name];
    }

    // 尝试大写匹配
    if (nameMap.contains(upperName)) {
        return nameMap[upperName];
    }

    // 未找到匹配项
    return PlatformType::Unknown;
}

int getPlatformAxisCount(PlatformType type) {
    switch (type) {
        case PlatformType::X:
        case PlatformType::Y:
            return 1;

        case PlatformType::XY:
        case PlatformType::XD:
        case PlatformType::YD:
            return 2;

        case PlatformType::XYD:
        case PlatformType::X1X2Y:
        case PlatformType::XY1Y2:
        case PlatformType::DXY:
        case PlatformType::XDPY:
        case PlatformType::XDPY1:
        case PlatformType::XYPD:
        case PlatformType::UVW:
        case PlatformType::UVW_XXY:
        case PlatformType::UVW_XYY:
        case PlatformType::UVW_HOLLOW:
        case PlatformType::UVW_HIGH_PRECISION:
            return 3;

        case PlatformType::YX1X2PD:
        case PlatformType::YDPX1X2:
        case PlatformType::XYDP:
        case PlatformType::UVW_XXYY:
            return 4;

        default:
            return 0;
    }
}

bool platformHasRotation(PlatformType type) {
    switch (type) {
        case PlatformType::XYD:
        case PlatformType::DXY:
        case PlatformType::XD:
        case PlatformType::YD:
        case PlatformType::XDPY:
        case PlatformType::XDPY1:
        case PlatformType::XYPD:
        case PlatformType::YX1X2PD:
        case PlatformType::YDPX1X2:
        case PlatformType::XYDP:
        // 所有UVW平台都有旋转能力（通过并联运动实现）
        case PlatformType::UVW:
        case PlatformType::UVW_XXY:
        case PlatformType::UVW_XYY:
        case PlatformType::UVW_XXYY:
        case PlatformType::UVW_HOLLOW:
        case PlatformType::UVW_HIGH_PRECISION:
            return true;

        case PlatformType::X:
        case PlatformType::Y:
        case PlatformType::XY:
        case PlatformType::X1X2Y:
        case PlatformType::XY1Y2:
            return false;

        default:
            return false;
    }
}

bool platformIsDualX(PlatformType type) {
    switch (type) {
        case PlatformType::X1X2Y:
        case PlatformType::YX1X2PD:
        case PlatformType::YDPX1X2:
        // UVW的XXY和XXYY结构本质上是双X轴
        case PlatformType::UVW_XXY:
        case PlatformType::UVW_XXYY:
            return true;
        default:
            return false;
    }
}

bool platformIsDualY(PlatformType type) {
    switch (type) {
        case PlatformType::XY1Y2:
        // UVW的XYY和XXYY结构本质上是双Y轴
        case PlatformType::UVW_XYY:
        case PlatformType::UVW_XXYY:
            return true;
        default:
            return false;
    }
}

QList<PlatformType> getSupportedPlatformTypes() {
    return {
        PlatformType::XYD,
        PlatformType::X1X2Y,
        PlatformType::XY1Y2,
        PlatformType::XY,
        PlatformType::DXY,
        PlatformType::XD,
        PlatformType::YD,
        PlatformType::XDPY,
        PlatformType::XYPD,
        PlatformType::YX1X2PD,
        PlatformType::YDPX1X2,
        PlatformType::XYDP,
        PlatformType::X,
        PlatformType::Y,
        // UVW精密对位平台系列
        PlatformType::UVW,
        PlatformType::UVW_XXY,
        PlatformType::UVW_XYY,
        PlatformType::UVW_XXYY,
        PlatformType::UVW_HOLLOW,
        PlatformType::UVW_HIGH_PRECISION
    };
}

bool platformIsUVW(PlatformType type) {
    switch (type) {
        case PlatformType::UVW:
        case PlatformType::UVW_XXY:
        case PlatformType::UVW_XYY:
        case PlatformType::UVW_XXYY:
        case PlatformType::UVW_HOLLOW:
        case PlatformType::UVW_HIGH_PRECISION:
            return true;
        default:
            return false;
    }
}

UVWStructureType getUVWStructureType(PlatformType type) {
    switch (type) {
        case PlatformType::UVW_XXY:
            return UVWStructureType::XXY;
        case PlatformType::UVW_XYY:
            return UVWStructureType::XYY;
        case PlatformType::UVW_XXYY:
            // XXYY同时具有双X和双Y，返回特殊标识或默认为XXY
            return UVWStructureType::XXY;  // XXYY基础上可扩展
        case PlatformType::UVW:
        case PlatformType::UVW_HOLLOW:
        case PlatformType::UVW_HIGH_PRECISION:
        default:
            // 默认使用XXY结构（更常见）
            return UVWStructureType::XXY;
    }
}

PlatformType getUVWPlatformType(const UVWPlatformConfig& config) {
    // 先检查特殊配置
    if (config.precisionGrade == UVWPrecisionGrade::G_Level) {
        return PlatformType::UVW_HIGH_PRECISION;
    }
    if (config.isHollow) {
        return PlatformType::UVW_HOLLOW;
    }

    // 根据结构类型选择
    // 注意：工业上常见的四轴配置是XXYY（双X双Y），不是Z轴组合
    if (config.structureType == UVWStructureType::XYY) {
        return PlatformType::UVW_XYY;
    } else if (config.structureType == UVWStructureType::XXY) {
        return PlatformType::UVW_XXY;
    } else {
        return PlatformType::UVW;
    }
}

// ============ UVWPlatformConfig JSON序列化 ============

QJsonObject UVWPlatformConfig::toJson() const {
    QJsonObject json;
    json["structureType"] = static_cast<int>(structureType);
    json["sliderType"] = static_cast<int>(sliderType);
    json["precisionGrade"] = static_cast<int>(precisionGrade);
    json["hasZAxis"] = hasZAxis;
    json["isHollow"] = isHollow;
    json["axis1Direction"] = static_cast<int>(axis1Direction);
    json["axis2Direction"] = static_cast<int>(axis2Direction);
    json["axis3Direction"] = static_cast<int>(axis3Direction);
    json["zAxisDirection"] = static_cast<int>(zAxisDirection);
    json["node1PosX"] = node1PosX;
    json["node1PosY"] = node1PosY;
    json["node2PosX"] = node2PosX;
    json["node2PosY"] = node2PosY;
    json["node3PosX"] = node3PosX;
    json["node3PosY"] = node3PosY;
    json["travelXY"] = travelXY;
    json["travelTheta"] = travelTheta;
    json["travelZ"] = travelZ;
    json["maxLoad"] = maxLoad;
    return json;
}

UVWPlatformConfig UVWPlatformConfig::fromJson(const QJsonObject& json) {
    UVWPlatformConfig config;
    config.structureType = static_cast<UVWStructureType>(json.value("structureType").toInt(0));
    config.sliderType = static_cast<UVWSliderType>(json.value("sliderType").toInt(0));
    config.precisionGrade = static_cast<UVWPrecisionGrade>(json.value("precisionGrade").toInt(0));
    config.hasZAxis = json.value("hasZAxis").toBool(false);
    config.isHollow = json.value("isHollow").toBool(false);
    config.axis1Direction = static_cast<AxisDirectionType>(json.value("axis1Direction").toInt(1));
    config.axis2Direction = static_cast<AxisDirectionType>(json.value("axis2Direction").toInt(1));
    config.axis3Direction = static_cast<AxisDirectionType>(json.value("axis3Direction").toInt(1));
    config.zAxisDirection = static_cast<AxisDirectionType>(json.value("zAxisDirection").toInt(1));
    config.node1PosX = json.value("node1PosX").toDouble(67.5);
    config.node1PosY = json.value("node1PosY").toDouble(-57.0);
    config.node2PosX = json.value("node2PosX").toDouble(67.5);
    config.node2PosY = json.value("node2PosY").toDouble(57.0);
    config.node3PosX = json.value("node3PosX").toDouble(-57.0);
    config.node3PosY = json.value("node3PosY").toDouble(67.5);
    config.travelXY = json.value("travelXY").toDouble(5.0);
    config.travelTheta = json.value("travelTheta").toDouble(2.0);
    config.travelZ = json.value("travelZ").toDouble(10.0);
    config.maxLoad = json.value("maxLoad").toDouble(50.0);
    return config;
}

} // namespace Platform
} // namespace VisionForge
