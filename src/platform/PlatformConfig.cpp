/**
 * @file PlatformConfig.cpp
 * @brief VisionForge Pro 平台配置管理实现
 *
 * 实现PlatformConfig.h中定义的所有平台配置类。
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include "platform/PlatformConfig.h"
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <cmath>

namespace VisionForge {
namespace Platform {

// ============ PlatformXYDInfo 实现 ============

PlatformXYDInfo::PlatformXYDInfo() = default;

std::unique_ptr<PlatformInfo> PlatformXYDInfo::clone() const {
    auto copy = std::make_unique<PlatformXYDInfo>();
    copy->xDirection = xDirection;
    copy->yDirection = yDirection;
    copy->dDirection = dDirection;
    copy->dDriveType = dDriveType;
    copy->dLinearType = dLinearType;
    copy->rotationLength = rotationLength;
    return copy;
}

QJsonObject PlatformXYDInfo::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(PlatformType::XYD);
    json["xDirection"] = static_cast<int>(xDirection);
    json["yDirection"] = static_cast<int>(yDirection);
    json["dDirection"] = static_cast<int>(dDirection);
    json["dDriveType"] = static_cast<int>(dDriveType);
    json["dLinearType"] = static_cast<int>(dLinearType);
    json["rotationLength"] = rotationLength;
    return json;
}

void PlatformXYDInfo::fromJson(const QJsonObject& json) {
    xDirection = static_cast<AxisDirectionType>(
        json.value("xDirection").toInt(static_cast<int>(AxisDirectionType::Positive)));
    yDirection = static_cast<AxisDirectionType>(
        json.value("yDirection").toInt(static_cast<int>(AxisDirectionType::Negative)));
    dDirection = static_cast<AxisDirectionType>(
        json.value("dDirection").toInt(static_cast<int>(AxisDirectionType::Negative)));
    dDriveType = static_cast<DDriveType>(
        json.value("dDriveType").toInt(static_cast<int>(DDriveType::Direct)));
    dLinearType = static_cast<DLinearDriveType>(
        json.value("dLinearType").toInt(static_cast<int>(DLinearDriveType::Fixed)));
    rotationLength = json.value("rotationLength").toDouble(100.0);
}

// ============ PlatformX1X2YInfo 实现 ============

PlatformX1X2YInfo::PlatformX1X2YInfo() {
    // 默认UVW平台轴位置（参考VisionASM的CPlatformX1X2YInfo）
    x1Pos.setMainPos(67.5, -57);
    x2Pos.setMainPos(67.5, 57);
    yPos.setMainPos(-57, 67.5);
}

std::unique_ptr<PlatformInfo> PlatformX1X2YInfo::clone() const {
    auto copy = std::make_unique<PlatformX1X2YInfo>();
    copy->x1Direction = x1Direction;
    copy->x2Direction = x2Direction;
    copy->yDirection = yDirection;
    copy->x1Pos = x1Pos;
    copy->x2Pos = x2Pos;
    copy->yPos = yPos;
    copy->sliderType = sliderType;
    return copy;
}

QJsonObject PlatformX1X2YInfo::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(PlatformType::X1X2Y);
    json["x1Direction"] = static_cast<int>(x1Direction);
    json["x2Direction"] = static_cast<int>(x2Direction);
    json["yDirection"] = static_cast<int>(yDirection);
    json["x1Pos"] = x1Pos.toJson();
    json["x2Pos"] = x2Pos.toJson();
    json["yPos"] = yPos.toJson();
    json["sliderType"] = sliderType;
    return json;
}

void PlatformX1X2YInfo::fromJson(const QJsonObject& json) {
    x1Direction = static_cast<AxisDirectionType>(
        json.value("x1Direction").toInt(static_cast<int>(AxisDirectionType::Positive)));
    x2Direction = static_cast<AxisDirectionType>(
        json.value("x2Direction").toInt(static_cast<int>(AxisDirectionType::Positive)));
    yDirection = static_cast<AxisDirectionType>(
        json.value("yDirection").toInt(static_cast<int>(AxisDirectionType::Positive)));
    x1Pos = CoordPosition::fromJson(json.value("x1Pos").toObject());
    x2Pos = CoordPosition::fromJson(json.value("x2Pos").toObject());
    yPos = CoordPosition::fromJson(json.value("yPos").toObject());
    sliderType = json.value("sliderType").toInt(0);
}

void PlatformX1X2YInfo::convertToXYD(double x1, double x2, double y,
                                      double& x, double& yOut, double& d) const {
    // 参考VisionASM的ConvertX1X2YPltfmPosToXYDPltfmPos
    // 使用几何关系计算

    // 获取轴作用点位置
    double x1PosX = x1Pos.getMainX();
    double x1PosY = x1Pos.getMainY();
    double x2PosX = x2Pos.getMainX();
    double x2PosY = x2Pos.getMainY();
    double yPosX = yPos.getMainX();
    double yPosY = yPos.getMainY();

    // 计算旋转中心和角度
    // 简化计算：假设X1和X2对称布置
    double dx = x2 - x1;  // X1X2的位置差产生旋转

    // 计算旋转臂长度
    double armLength = std::abs(x1PosY - x2PosY);
    if (armLength < 1e-6) {
        armLength = 1.0;  // 防止除零
    }

    // 旋转角度（弧度）
    double angleRad = std::atan2(dx, armLength);
    d = angleRad * 180.0 / M_PI;

    // 计算中心位置
    x = (x1 + x2) / 2.0;
    yOut = y;
}

void PlatformX1X2YInfo::convertFromXYD(double x, double y, double d,
                                        double& x1, double& x2, double& yOut) const {
    // 从XYD逻辑位置转换到X1X2Y轴位置

    double x1PosY = x1Pos.getMainY();
    double x2PosY = x2Pos.getMainY();

    double armLength = std::abs(x1PosY - x2PosY);
    if (armLength < 1e-6) {
        armLength = 1.0;
    }

    double angleRad = d * M_PI / 180.0;
    double dx = armLength * std::tan(angleRad);

    x1 = x - dx / 2.0;
    x2 = x + dx / 2.0;
    yOut = y;
}

// ============ PlatformXY1Y2Info 实现 ============

PlatformXY1Y2Info::PlatformXY1Y2Info() {
    // 默认位置（参考VisionASM）
    y1Pos.setMainPos(-78.5, 79);
    y2Pos.setMainPos(78.5, 79);
    xPos.setMainPos(70, -78.5);
}

std::unique_ptr<PlatformInfo> PlatformXY1Y2Info::clone() const {
    auto copy = std::make_unique<PlatformXY1Y2Info>();
    copy->y1Direction = y1Direction;
    copy->y2Direction = y2Direction;
    copy->xDirection = xDirection;
    copy->y1Pos = y1Pos;
    copy->y2Pos = y2Pos;
    copy->xPos = xPos;
    copy->sliderType = sliderType;
    return copy;
}

QJsonObject PlatformXY1Y2Info::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(PlatformType::XY1Y2);
    json["y1Direction"] = static_cast<int>(y1Direction);
    json["y2Direction"] = static_cast<int>(y2Direction);
    json["xDirection"] = static_cast<int>(xDirection);
    json["y1Pos"] = y1Pos.toJson();
    json["y2Pos"] = y2Pos.toJson();
    json["xPos"] = xPos.toJson();
    json["sliderType"] = sliderType;
    return json;
}

void PlatformXY1Y2Info::fromJson(const QJsonObject& json) {
    y1Direction = static_cast<AxisDirectionType>(
        json.value("y1Direction").toInt(static_cast<int>(AxisDirectionType::Positive)));
    y2Direction = static_cast<AxisDirectionType>(
        json.value("y2Direction").toInt(static_cast<int>(AxisDirectionType::Positive)));
    xDirection = static_cast<AxisDirectionType>(
        json.value("xDirection").toInt(static_cast<int>(AxisDirectionType::Positive)));
    y1Pos = CoordPosition::fromJson(json.value("y1Pos").toObject());
    y2Pos = CoordPosition::fromJson(json.value("y2Pos").toObject());
    xPos = CoordPosition::fromJson(json.value("xPos").toObject());
    sliderType = json.value("sliderType").toInt(0);
}

void PlatformXY1Y2Info::convertToXYD(double y1, double y2, double x,
                                      double& xOut, double& y, double& d) const {
    double y1PosX = y1Pos.getMainX();
    double y2PosX = y2Pos.getMainX();

    double armLength = std::abs(y2PosX - y1PosX);
    if (armLength < 1e-6) {
        armLength = 1.0;
    }

    double dy = y2 - y1;
    double angleRad = std::atan2(dy, armLength);

    d = angleRad * 180.0 / M_PI;
    xOut = x;
    y = (y1 + y2) / 2.0;
}

void PlatformXY1Y2Info::convertFromXYD(double x, double y, double d,
                                        double& y1, double& y2, double& xOut) const {
    double y1PosX = y1Pos.getMainX();
    double y2PosX = y2Pos.getMainX();

    double armLength = std::abs(y2PosX - y1PosX);
    if (armLength < 1e-6) {
        armLength = 1.0;
    }

    double angleRad = d * M_PI / 180.0;
    double dy = armLength * std::tan(angleRad);

    y1 = y - dy / 2.0;
    y2 = y + dy / 2.0;
    xOut = x;
}

// ============ PlatformDXYInfo 实现 ============

PlatformDXYInfo::PlatformDXYInfo() = default;

std::unique_ptr<PlatformInfo> PlatformDXYInfo::clone() const {
    auto copy = std::make_unique<PlatformDXYInfo>();
    copy->xDirection = xDirection;
    copy->yDirection = yDirection;
    copy->dDirection = dDirection;
    copy->dDriveType = dDriveType;
    copy->rotationLength = rotationLength;
    return copy;
}

QJsonObject PlatformDXYInfo::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(PlatformType::DXY);
    json["xDirection"] = static_cast<int>(xDirection);
    json["yDirection"] = static_cast<int>(yDirection);
    json["dDirection"] = static_cast<int>(dDirection);
    json["dDriveType"] = static_cast<int>(dDriveType);
    json["rotationLength"] = rotationLength;
    return json;
}

void PlatformDXYInfo::fromJson(const QJsonObject& json) {
    xDirection = static_cast<AxisDirectionType>(
        json.value("xDirection").toInt(static_cast<int>(AxisDirectionType::Positive)));
    yDirection = static_cast<AxisDirectionType>(
        json.value("yDirection").toInt(static_cast<int>(AxisDirectionType::Negative)));
    dDirection = static_cast<AxisDirectionType>(
        json.value("dDirection").toInt(static_cast<int>(AxisDirectionType::Negative)));
    dDriveType = static_cast<DDriveType>(
        json.value("dDriveType").toInt(static_cast<int>(DDriveType::Linear)));
    rotationLength = json.value("rotationLength").toDouble(100.0);
}

// ============ CameraPlatformInfo 实现 ============

CameraPlatformInfo::CameraPlatformInfo() {
    setCameraNumAndType(2, CameraPlatformType::SeparateFix);
}

void CameraPlatformInfo::setCameraNumAndType(int cameraNum, CameraPlatformType type) {
    m_cameraNum = cameraNum;
    m_camPlatformType = type;
    m_camDirectX.resize(cameraNum, AxisDirectionType::Positive);
    m_camDirectY.resize(cameraNum, AxisDirectionType::Positive);
    m_camAxisDirectX.resize(cameraNum, AxisDirectionType::Positive);
    m_camAxisDirectY.resize(cameraNum, AxisDirectionType::Positive);
}

bool CameraPlatformInfo::setCamDirectionX(int index, AxisDirectionType direction) {
    if (index < 0 || index >= static_cast<int>(m_camDirectX.size())) {
        return false;
    }
    m_camDirectX[index] = direction;
    return true;
}

bool CameraPlatformInfo::setCamDirectionY(int index, AxisDirectionType direction) {
    if (index < 0 || index >= static_cast<int>(m_camDirectY.size())) {
        return false;
    }
    m_camDirectY[index] = direction;
    return true;
}

AxisDirectionType CameraPlatformInfo::getCamDirectionX(int index) const {
    if (index < 0 || index >= static_cast<int>(m_camDirectX.size())) {
        return AxisDirectionType::Positive;
    }
    return m_camDirectX[index];
}

AxisDirectionType CameraPlatformInfo::getCamDirectionY(int index) const {
    if (index < 0 || index >= static_cast<int>(m_camDirectY.size())) {
        return AxisDirectionType::Positive;
    }
    return m_camDirectY[index];
}

QJsonObject CameraPlatformInfo::toJson() const {
    QJsonObject json;
    json["cameraNum"] = m_cameraNum;
    json["camPlatformType"] = static_cast<int>(m_camPlatformType);

    QJsonArray dirXArray, dirYArray, axisXArray, axisYArray;
    for (size_t i = 0; i < m_camDirectX.size(); ++i) {
        dirXArray.append(static_cast<int>(m_camDirectX[i]));
        dirYArray.append(static_cast<int>(m_camDirectY[i]));
        axisXArray.append(static_cast<int>(m_camAxisDirectX[i]));
        axisYArray.append(static_cast<int>(m_camAxisDirectY[i]));
    }
    json["camDirectX"] = dirXArray;
    json["camDirectY"] = dirYArray;
    json["camAxisDirectX"] = axisXArray;
    json["camAxisDirectY"] = axisYArray;

    return json;
}

void CameraPlatformInfo::fromJson(const QJsonObject& json) {
    int cameraNum = json.value("cameraNum").toInt(2);
    auto type = static_cast<CameraPlatformType>(
        json.value("camPlatformType").toInt(static_cast<int>(CameraPlatformType::SeparateFix)));

    setCameraNumAndType(cameraNum, type);

    QJsonArray dirXArray = json.value("camDirectX").toArray();
    QJsonArray dirYArray = json.value("camDirectY").toArray();
    QJsonArray axisXArray = json.value("camAxisDirectX").toArray();
    QJsonArray axisYArray = json.value("camAxisDirectY").toArray();

    for (int i = 0; i < cameraNum && i < dirXArray.size(); ++i) {
        m_camDirectX[i] = static_cast<AxisDirectionType>(dirXArray[i].toInt());
        m_camDirectY[i] = static_cast<AxisDirectionType>(dirYArray[i].toInt());
        if (i < axisXArray.size()) {
            m_camAxisDirectX[i] = static_cast<AxisDirectionType>(axisXArray[i].toInt());
        }
        if (i < axisYArray.size()) {
            m_camAxisDirectY[i] = static_cast<AxisDirectionType>(axisYArray[i].toInt());
        }
    }
}

// ============ PlatformConfig 实现 ============

PlatformConfig::PlatformConfig() {
    createDefaultInfo(PlatformType::XYD);
}

// 拷贝构造函数
PlatformConfig::PlatformConfig(const PlatformConfig& other)
    : name(other.name)
    , platformIndex(other.platformIndex)
    , type(other.type)
    , xRange(other.xRange)
    , yRange(other.yRange)
    , dRange(other.dRange)
    , xPulsePerMM(other.xPulsePerMM)
    , yPulsePerMM(other.yPulsePerMM)
    , dPulsePerDegree(other.dPulsePerDegree)
    , origin(other.origin)
    , cameraNum(other.cameraNum)
    , cameraPlatform(other.cameraPlatform)
    , positionNum(other.positionNum)
{
    // 深拷贝unique_ptr
    if (other.m_platformInfo) {
        m_platformInfo = other.m_platformInfo->clone();
    }
}

// 移动构造函数
PlatformConfig::PlatformConfig(PlatformConfig&& other) noexcept
    : name(std::move(other.name))
    , platformIndex(other.platformIndex)
    , type(other.type)
    , xRange(other.xRange)
    , yRange(other.yRange)
    , dRange(other.dRange)
    , xPulsePerMM(other.xPulsePerMM)
    , yPulsePerMM(other.yPulsePerMM)
    , dPulsePerDegree(other.dPulsePerDegree)
    , origin(other.origin)
    , cameraNum(other.cameraNum)
    , cameraPlatform(std::move(other.cameraPlatform))
    , positionNum(other.positionNum)
    , m_platformInfo(std::move(other.m_platformInfo))
{
}

// 拷贝赋值操作符
PlatformConfig& PlatformConfig::operator=(const PlatformConfig& other) {
    if (this != &other) {
        name = other.name;
        platformIndex = other.platformIndex;
        type = other.type;
        xRange = other.xRange;
        yRange = other.yRange;
        dRange = other.dRange;
        xPulsePerMM = other.xPulsePerMM;
        yPulsePerMM = other.yPulsePerMM;
        dPulsePerDegree = other.dPulsePerDegree;
        origin = other.origin;
        cameraNum = other.cameraNum;
        cameraPlatform = other.cameraPlatform;
        positionNum = other.positionNum;

        // 深拷贝unique_ptr
        if (other.m_platformInfo) {
            m_platformInfo = other.m_platformInfo->clone();
        } else {
            m_platformInfo.reset();
        }
    }
    return *this;
}

// 移动赋值操作符
PlatformConfig& PlatformConfig::operator=(PlatformConfig&& other) noexcept {
    if (this != &other) {
        name = other.name;
        platformIndex = other.platformIndex;
        type = other.type;
        xRange = other.xRange;
        yRange = other.yRange;
        dRange = other.dRange;
        xPulsePerMM = other.xPulsePerMM;
        yPulsePerMM = other.yPulsePerMM;
        dPulsePerDegree = other.dPulsePerDegree;
        origin = other.origin;
        cameraNum = other.cameraNum;
        cameraPlatform = other.cameraPlatform;
        positionNum = other.positionNum;
        m_platformInfo.swap(other.m_platformInfo);
    }
    return *this;
}

void PlatformConfig::createDefaultInfo(PlatformType platType) {
    type = platType;

    switch (platType) {
        case PlatformType::XYD:
            m_platformInfo = std::make_unique<PlatformXYDInfo>();
            break;
        case PlatformType::X1X2Y:
            m_platformInfo = std::make_unique<PlatformX1X2YInfo>();
            break;
        case PlatformType::XY1Y2:
            m_platformInfo = std::make_unique<PlatformXY1Y2Info>();
            break;
        case PlatformType::DXY:
            m_platformInfo = std::make_unique<PlatformDXYInfo>();
            break;
        default:
            // 对于其他类型，使用XYD配置
            m_platformInfo = std::make_unique<PlatformXYDInfo>();
            break;
    }
}

bool PlatformConfig::save(const QString& filePath) const {
    QJsonDocument doc(toJson());
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "PlatformConfig::save: Cannot open file:" << filePath;
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "PlatformConfig::save: Saved to" << filePath;
    return true;
}

bool PlatformConfig::load(const QString& filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "PlatformConfig::load: Cannot open file:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "PlatformConfig::load: Invalid JSON in file:" << filePath;
        return false;
    }

    fromJson(doc.object());

    qDebug() << "PlatformConfig::load: Loaded from" << filePath;
    return true;
}

QJsonObject PlatformConfig::toJson() const {
    QJsonObject json;

    // 基本信息
    json["name"] = name;
    json["platformIndex"] = platformIndex;
    json["type"] = static_cast<int>(type);

    // 平台参数
    json["xRange"] = xRange;
    json["yRange"] = yRange;
    json["dRange"] = dRange;

    // 脉冲当量
    json["xPulsePerMM"] = xPulsePerMM;
    json["yPulsePerMM"] = yPulsePerMM;
    json["dPulsePerDegree"] = dPulsePerDegree;

    // 原点
    QJsonObject originJson;
    originJson["x"] = origin.x();
    originJson["y"] = origin.y();
    json["origin"] = originJson;

    // 相机配置
    json["cameraNum"] = cameraNum;
    json["positionNum"] = positionNum;
    json["cameraPlatform"] = cameraPlatform.toJson();

    // 详细平台配置
    if (m_platformInfo) {
        json["platformInfo"] = m_platformInfo->toJson();
    }

    return json;
}

void PlatformConfig::fromJson(const QJsonObject& json) {
    // 基本信息
    name = json.value("name").toString();
    platformIndex = json.value("platformIndex").toInt(0);
    type = static_cast<PlatformType>(json.value("type").toInt(
        static_cast<int>(PlatformType::XYD)));

    // 平台参数
    xRange = json.value("xRange").toDouble(1000.0);
    yRange = json.value("yRange").toDouble(1000.0);
    dRange = json.value("dRange").toDouble(360.0);

    // 脉冲当量
    xPulsePerMM = json.value("xPulsePerMM").toDouble(1000.0);
    yPulsePerMM = json.value("yPulsePerMM").toDouble(1000.0);
    dPulsePerDegree = json.value("dPulsePerDegree").toDouble(1000.0);

    // 原点
    QJsonObject originJson = json.value("origin").toObject();
    origin.setX(originJson.value("x").toDouble(0.0));
    origin.setY(originJson.value("y").toDouble(0.0));

    // 相机配置
    cameraNum = json.value("cameraNum").toInt(2);
    positionNum = json.value("positionNum").toInt(2);
    cameraPlatform.fromJson(json.value("cameraPlatform").toObject());

    // 详细平台配置
    QJsonObject infoJson = json.value("platformInfo").toObject();
    m_platformInfo = createPlatformInfo(type, infoJson);
}

std::unique_ptr<PlatformInfo> PlatformConfig::createPlatformInfo(
    PlatformType type, const QJsonObject& json) {

    std::unique_ptr<PlatformInfo> info;

    switch (type) {
        case PlatformType::XYD:
            info = std::make_unique<PlatformXYDInfo>();
            break;
        case PlatformType::X1X2Y:
            info = std::make_unique<PlatformX1X2YInfo>();
            break;
        case PlatformType::XY1Y2:
            info = std::make_unique<PlatformXY1Y2Info>();
            break;
        case PlatformType::DXY:
            info = std::make_unique<PlatformDXYInfo>();
            break;
        default:
            info = std::make_unique<PlatformXYDInfo>();
            break;
    }

    if (info && !json.isEmpty()) {
        info->fromJson(json);
    }

    return info;
}

bool PlatformConfig::isInRange(const PlatformAxisPos& pos) const {
    // 检查是否在行程范围内
    if (std::abs(pos.x1) > xRange / 2.0) return false;
    if (std::abs(pos.x2) > xRange / 2.0) return false;
    if (std::abs(pos.y1) > yRange / 2.0) return false;
    if (std::abs(pos.y2) > yRange / 2.0) return false;
    if (std::abs(pos.d) > dRange / 2.0) return false;

    return true;
}

PlatformAxisPos PlatformConfig::clampToRange(const PlatformAxisPos& pos) const {
    PlatformAxisPos result = pos;

    auto clamp = [](double value, double range) -> double {
        double halfRange = range / 2.0;
        return std::max(-halfRange, std::min(halfRange, value));
    };

    result.x1 = clamp(result.x1, xRange);
    result.x2 = clamp(result.x2, xRange);
    result.y1 = clamp(result.y1, yRange);
    result.y2 = clamp(result.y2, yRange);
    result.d = clamp(result.d, dRange);

    return result;
}

// ============ PlatformConfigManager 实现 ============

PlatformConfigManager& PlatformConfigManager::instance() {
    static PlatformConfigManager instance;
    return instance;
}

PlatformConfigManager::PlatformConfigManager() {
    // 创建默认配置
    PlatformConfig defaultConfig;
    defaultConfig.name = QStringLiteral("默认平台");
    defaultConfig.platformIndex = 0;
    m_configs.push_back(defaultConfig);
}

PlatformConfig& PlatformConfigManager::currentConfig() {
    if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(m_configs.size())) {
        return m_configs[m_currentIndex];
    }
    // 如果索引无效，返回第一个配置（如果存在）或创建新配置
    if (m_configs.empty()) {
        m_configs.emplace_back();
    }
    m_currentIndex = 0;
    return m_configs[0];
}

const PlatformConfig& PlatformConfigManager::currentConfig() const {
    if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(m_configs.size())) {
        return m_configs[m_currentIndex];
    }
    static PlatformConfig defaultConfig;
    return defaultConfig;
}

PlatformConfig* PlatformConfigManager::getConfig(int index) {
    if (index >= 0 && index < static_cast<int>(m_configs.size())) {
        return &m_configs[index];
    }
    return nullptr;
}

int PlatformConfigManager::addConfig(const PlatformConfig& config) {
    m_configs.push_back(config);
    return static_cast<int>(m_configs.size()) - 1;
}

bool PlatformConfigManager::removeConfig(int index) {
    if (index < 0 || index >= static_cast<int>(m_configs.size())) {
        return false;
    }
    if (m_configs.size() <= 1) {
        return false;  // 保留至少一个配置
    }

    m_configs.erase(m_configs.begin() + index);

    if (m_currentIndex >= static_cast<int>(m_configs.size())) {
        m_currentIndex = static_cast<int>(m_configs.size()) - 1;
    }

    return true;
}

int PlatformConfigManager::getConfigCount() const {
    return static_cast<int>(m_configs.size());
}

void PlatformConfigManager::setCurrentIndex(int index) {
    if (index >= 0 && index < static_cast<int>(m_configs.size())) {
        m_currentIndex = index;
    }
}

int PlatformConfigManager::getCurrentIndex() const {
    return m_currentIndex;
}

bool PlatformConfigManager::saveAll(const QString& dirPath) {
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    for (size_t i = 0; i < m_configs.size(); ++i) {
        QString fileName = QString("platform_%1.json").arg(i);
        QString filePath = dir.filePath(fileName);
        if (!m_configs[i].save(filePath)) {
            return false;
        }
    }

    // 保存索引文件
    QJsonObject indexJson;
    indexJson["count"] = static_cast<int>(m_configs.size());
    indexJson["currentIndex"] = m_currentIndex;

    QFile indexFile(dir.filePath("platforms.json"));
    if (indexFile.open(QIODevice::WriteOnly)) {
        indexFile.write(QJsonDocument(indexJson).toJson());
        indexFile.close();
    }

    return true;
}

bool PlatformConfigManager::loadAll(const QString& dirPath) {
    QDir dir(dirPath);
    if (!dir.exists()) {
        return false;
    }

    // 读取索引文件
    QFile indexFile(dir.filePath("platforms.json"));
    if (!indexFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(indexFile.readAll());
    indexFile.close();

    if (doc.isNull()) {
        return false;
    }

    QJsonObject indexJson = doc.object();
    int count = indexJson.value("count").toInt(0);
    m_currentIndex = indexJson.value("currentIndex").toInt(0);

    m_configs.clear();
    for (int i = 0; i < count; ++i) {
        QString fileName = QString("platform_%1.json").arg(i);
        QString filePath = dir.filePath(fileName);

        PlatformConfig config;
        if (config.load(filePath)) {
            m_configs.push_back(config);
        }
    }

    if (m_configs.empty()) {
        // 确保至少有一个配置
        m_configs.emplace_back();
        m_currentIndex = 0;
    }

    if (m_currentIndex >= static_cast<int>(m_configs.size())) {
        m_currentIndex = 0;
    }

    return true;
}

} // namespace Platform
} // namespace VisionForge
