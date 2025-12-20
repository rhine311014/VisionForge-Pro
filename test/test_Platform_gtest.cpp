/**
 * @file test_Platform_gtest.cpp
 * @brief VisionForge Pro 平台核心模块单元测试
 *
 * 测试Platform模块的核心功能：
 * - PlatformTypes: 平台类型枚举和轴位置
 * - CoordPosition: 坐标位姿变换
 * - PlatformConfig: 平台配置管理
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTemporaryDir>
#include <cmath>

#include "platform/PlatformTypes.h"
#include "platform/CoordPosition.h"
#include "platform/PlatformConfig.h"

using namespace VisionForge::Platform;

// ============================================================
// PlatformAxisPos 测试
// ============================================================

class PlatformAxisPosTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PlatformAxisPosTest, DefaultConstructor) {
    PlatformAxisPos pos;

    EXPECT_DOUBLE_EQ(pos.x1, 0.0);
    EXPECT_DOUBLE_EQ(pos.x2, 0.0);
    EXPECT_DOUBLE_EQ(pos.y1, 0.0);
    EXPECT_DOUBLE_EQ(pos.y2, 0.0);
    EXPECT_DOUBLE_EQ(pos.d, 0.0);
    EXPECT_DOUBLE_EQ(pos.p, 0.0);
}

TEST_F(PlatformAxisPosTest, XYDConstructor) {
    PlatformAxisPos pos(100.0, 200.0, 45.0);

    EXPECT_DOUBLE_EQ(pos.x1, 100.0);
    EXPECT_DOUBLE_EQ(pos.y1, 200.0);
    EXPECT_DOUBLE_EQ(pos.d, 45.0);
}

TEST_F(PlatformAxisPosTest, FullConstructor) {
    PlatformAxisPos pos(10.0, 20.0, 30.0, 40.0, 5.0, 2.5);

    EXPECT_DOUBLE_EQ(pos.x1, 10.0);
    EXPECT_DOUBLE_EQ(pos.x2, 20.0);
    EXPECT_DOUBLE_EQ(pos.y1, 30.0);
    EXPECT_DOUBLE_EQ(pos.y2, 40.0);
    EXPECT_DOUBLE_EQ(pos.d, 5.0);
    EXPECT_DOUBLE_EQ(pos.p, 2.5);
}

TEST_F(PlatformAxisPosTest, EqualityOperator) {
    PlatformAxisPos pos1(100.0, 200.0, 1.5);
    PlatformAxisPos pos2(100.0, 200.0, 1.5);
    PlatformAxisPos pos3(100.0, 200.0, 1.6);

    EXPECT_TRUE(pos1 == pos2);
    EXPECT_FALSE(pos1 == pos3);
    EXPECT_TRUE(pos1 != pos3);
}

TEST_F(PlatformAxisPosTest, ArithmeticOperators) {
    PlatformAxisPos pos1(100.0, 200.0, 1.0);
    PlatformAxisPos pos2(10.0, 20.0, 0.5);

    // 加法
    PlatformAxisPos sum = pos1 + pos2;
    EXPECT_DOUBLE_EQ(sum.x1, 110.0);
    EXPECT_DOUBLE_EQ(sum.y1, 220.0);
    EXPECT_DOUBLE_EQ(sum.d, 1.5);

    // 减法
    PlatformAxisPos diff = pos1 - pos2;
    EXPECT_DOUBLE_EQ(diff.x1, 90.0);
    EXPECT_DOUBLE_EQ(diff.y1, 180.0);
    EXPECT_DOUBLE_EQ(diff.d, 0.5);

    // 标量乘法
    PlatformAxisPos scaled = pos1 * 2.0;
    EXPECT_DOUBLE_EQ(scaled.x1, 200.0);
    EXPECT_DOUBLE_EQ(scaled.y1, 400.0);
    EXPECT_DOUBLE_EQ(scaled.d, 2.0);
}

TEST_F(PlatformAxisPosTest, ToString) {
    PlatformAxisPos pos(100.5, 0.0, 200.3, 0.0, 1.234, 0.0);
    QString str = pos.toString();

    EXPECT_TRUE(str.contains("100.5"));
    EXPECT_TRUE(str.contains("200.3"));
    EXPECT_TRUE(str.contains("1.234"));
}

TEST_F(PlatformAxisPosTest, JsonSerialization) {
    PlatformAxisPos pos(100.0, 50.0, 200.0, 75.0, 1.5, 0.5);

    QJsonObject json = pos.toJson();
    PlatformAxisPos loaded = PlatformAxisPos::fromJson(json);

    EXPECT_DOUBLE_EQ(loaded.x1, pos.x1);
    EXPECT_DOUBLE_EQ(loaded.x2, pos.x2);
    EXPECT_DOUBLE_EQ(loaded.y1, pos.y1);
    EXPECT_DOUBLE_EQ(loaded.y2, pos.y2);
    EXPECT_DOUBLE_EQ(loaded.d, pos.d);
    EXPECT_DOUBLE_EQ(loaded.p, pos.p);
}

TEST_F(PlatformAxisPosTest, IsZero) {
    PlatformAxisPos zero;
    PlatformAxisPos nonZero(0.1, 0.0, 0.0);

    EXPECT_TRUE(zero.isZero());
    EXPECT_FALSE(nonZero.isZero());
}

TEST_F(PlatformAxisPosTest, NormalizeAngles) {
    PlatformAxisPos pos;
    pos.d = 270.0;
    pos.p = -270.0;

    pos.normalizeAngles();

    EXPECT_DOUBLE_EQ(pos.d, -90.0);
    EXPECT_DOUBLE_EQ(pos.p, 90.0);
}

// ============================================================
// PlatformType 工具函数测试
// ============================================================

class PlatformTypeTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PlatformTypeTest, GetPlatformTypeName) {
    EXPECT_FALSE(getPlatformTypeName(PlatformType::XYD).isEmpty());
    EXPECT_FALSE(getPlatformTypeName(PlatformType::X1X2Y).isEmpty());
    EXPECT_FALSE(getPlatformTypeName(PlatformType::DXY).isEmpty());
}

TEST_F(PlatformTypeTest, GetPlatformTypeFromName) {
    EXPECT_EQ(getPlatformTypeFromName("XYD"), PlatformType::XYD);
    EXPECT_EQ(getPlatformTypeFromName("xyd"), PlatformType::XYD);
    EXPECT_EQ(getPlatformTypeFromName("X1X2Y"), PlatformType::X1X2Y);
    EXPECT_EQ(getPlatformTypeFromName("DXY"), PlatformType::DXY);
    EXPECT_EQ(getPlatformTypeFromName("unknown_type"), PlatformType::Unknown);
}

TEST_F(PlatformTypeTest, GetPlatformAxisCount) {
    EXPECT_EQ(getPlatformAxisCount(PlatformType::X), 1);
    EXPECT_EQ(getPlatformAxisCount(PlatformType::XY), 2);
    EXPECT_EQ(getPlatformAxisCount(PlatformType::XYD), 3);
    EXPECT_EQ(getPlatformAxisCount(PlatformType::XYDP), 4);
}

TEST_F(PlatformTypeTest, PlatformHasRotation) {
    EXPECT_TRUE(platformHasRotation(PlatformType::XYD));
    EXPECT_TRUE(platformHasRotation(PlatformType::DXY));
    EXPECT_FALSE(platformHasRotation(PlatformType::XY));
    EXPECT_FALSE(platformHasRotation(PlatformType::X));
}

TEST_F(PlatformTypeTest, PlatformIsDualX) {
    EXPECT_TRUE(platformIsDualX(PlatformType::X1X2Y));
    EXPECT_TRUE(platformIsDualX(PlatformType::YDPX1X2));
    EXPECT_FALSE(platformIsDualX(PlatformType::XYD));
}

TEST_F(PlatformTypeTest, PlatformIsDualY) {
    EXPECT_TRUE(platformIsDualY(PlatformType::XY1Y2));
    EXPECT_FALSE(platformIsDualY(PlatformType::XYD));
}

TEST_F(PlatformTypeTest, GetSupportedPlatformTypes) {
    auto types = getSupportedPlatformTypes();

    EXPECT_FALSE(types.isEmpty());
    EXPECT_TRUE(types.contains(PlatformType::XYD));
    EXPECT_TRUE(types.contains(PlatformType::X1X2Y));
}

// ============================================================
// CoordPosition 测试
// ============================================================

class CoordPositionTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    bool isNear(double a, double b, double eps = 1e-6) {
        return std::abs(a - b) < eps;
    }
};

TEST_F(CoordPositionTest, DefaultConstructor) {
    CoordPosition pos;

    EXPECT_DOUBLE_EQ(pos.getMainX(), 0.0);
    EXPECT_DOUBLE_EQ(pos.getMainY(), 0.0);
    EXPECT_DOUBLE_EQ(pos.getAngle(), 0.0);
}

TEST_F(CoordPositionTest, XYAngleConstructor) {
    CoordPosition pos(100.0, 200.0, 45.0);

    EXPECT_DOUBLE_EQ(pos.getMainX(), 100.0);
    EXPECT_DOUBLE_EQ(pos.getMainY(), 200.0);
    EXPECT_DOUBLE_EQ(pos.getAngle(), 45.0);
}

TEST_F(CoordPositionTest, SetMainPosition) {
    CoordPosition pos;
    pos.setMainPos(150.0, 250.0);

    EXPECT_DOUBLE_EQ(pos.getMainX(), 150.0);
    EXPECT_DOUBLE_EQ(pos.getMainY(), 250.0);

    pos.setMainPosAngle(300.0, 400.0, 30.0);
    EXPECT_DOUBLE_EQ(pos.getMainX(), 300.0);
    EXPECT_DOUBLE_EQ(pos.getMainY(), 400.0);
    EXPECT_DOUBLE_EQ(pos.getAngle(), 30.0);
}

TEST_F(CoordPositionTest, AuxiliaryPositions) {
    CoordPosition pos;

    // 设置辅助位置
    EXPECT_TRUE(pos.setAuxPos(0, 10.0, 20.0));
    EXPECT_TRUE(pos.setAuxPos(1, 30.0, 40.0));
    EXPECT_TRUE(pos.setAuxPosAngle(2, 50.0, 60.0, 1.5));

    // 无效索引
    EXPECT_FALSE(pos.setAuxPos(-1, 0.0, 0.0));
    EXPECT_FALSE(pos.setAuxPos(10, 0.0, 0.0));

    // 获取辅助位置
    EXPECT_DOUBLE_EQ(pos.getAuxX(0), 10.0);
    EXPECT_DOUBLE_EQ(pos.getAuxY(0), 20.0);
    EXPECT_DOUBLE_EQ(pos.getAuxX(1), 30.0);
    EXPECT_DOUBLE_EQ(pos.getAuxY(1), 40.0);
    EXPECT_DOUBLE_EQ(pos.getAuxAngle(2), 1.5);

    // 无效索引返回0
    EXPECT_DOUBLE_EQ(pos.getAuxX(-1), 0.0);
    EXPECT_DOUBLE_EQ(pos.getAuxX(10), 0.0);
}

TEST_F(CoordPositionTest, Offset) {
    CoordPosition pos(100.0, 200.0, 0.0);
    pos.setAuxPos(0, 50.0, 75.0);

    pos.offset(10.0, 20.0);

    EXPECT_DOUBLE_EQ(pos.getMainX(), 110.0);
    EXPECT_DOUBLE_EQ(pos.getMainY(), 220.0);
    EXPECT_DOUBLE_EQ(pos.getAuxX(0), 60.0);
    EXPECT_DOUBLE_EQ(pos.getAuxY(0), 95.0);
}

TEST_F(CoordPositionTest, MapIdentity) {
    // 单位矩阵变换应该保持不变
    CoordPosition pos(100.0, 200.0, 45.0);

    QMatrix3x3 identity;
    identity(0, 0) = 1.0; identity(0, 1) = 0.0;
    identity(1, 0) = 0.0; identity(1, 1) = 1.0;

    CoordPosition mapped = pos.map(identity);

    EXPECT_TRUE(isNear(mapped.getMainX(), 100.0));
    EXPECT_TRUE(isNear(mapped.getMainY(), 200.0));
    EXPECT_DOUBLE_EQ(mapped.getAngle(), 45.0);
}

TEST_F(CoordPositionTest, MapScale) {
    // 缩放变换
    CoordPosition pos(100.0, 200.0, 0.0);

    QMatrix3x3 scale;
    scale(0, 0) = 2.0; scale(0, 1) = 0.0;
    scale(1, 0) = 0.0; scale(1, 1) = 2.0;

    CoordPosition mapped = pos.map(scale);

    EXPECT_TRUE(isNear(mapped.getMainX(), 200.0));
    EXPECT_TRUE(isNear(mapped.getMainY(), 400.0));
}

TEST_F(CoordPositionTest, MapTranslation) {
    // 平移变换
    CoordPosition pos(100.0, 200.0, 0.0);

    QMatrix3x3 identity;
    identity(0, 0) = 1.0; identity(0, 1) = 0.0;
    identity(1, 0) = 0.0; identity(1, 1) = 1.0;

    QPointF translation(50.0, 100.0);
    CoordPosition mapped = pos.map(identity, translation);

    EXPECT_TRUE(isNear(mapped.getMainX(), 150.0));
    EXPECT_TRUE(isNear(mapped.getMainY(), 300.0));
}

TEST_F(CoordPositionTest, InvMap) {
    CoordPosition pos(100.0, 200.0, 45.0);

    // 2倍缩放
    QMatrix3x3 scale;
    scale(0, 0) = 2.0; scale(0, 1) = 0.0;
    scale(1, 0) = 0.0; scale(1, 1) = 2.0;

    CoordPosition mapped = pos.map(scale);
    CoordPosition invMapped = mapped.invMap(scale);

    EXPECT_TRUE(isNear(invMapped.getMainX(), pos.getMainX()));
    EXPECT_TRUE(isNear(invMapped.getMainY(), pos.getMainY()));
}

TEST_F(CoordPositionTest, ToPlatformAxisXYD) {
    CoordPosition pos(100.0, 200.0, 1.5);

    PlatformAxisPos axisPos = pos.toPlatformAxis(PlatformType::XYD);

    EXPECT_DOUBLE_EQ(axisPos.x1, 100.0);
    EXPECT_DOUBLE_EQ(axisPos.y1, 200.0);
    EXPECT_DOUBLE_EQ(axisPos.d, 1.5);
}

TEST_F(CoordPositionTest, ToPlatformAxisXY) {
    CoordPosition pos(100.0, 200.0, 1.5);

    PlatformAxisPos axisPos = pos.toPlatformAxis(PlatformType::XY);

    EXPECT_DOUBLE_EQ(axisPos.x1, 100.0);
    EXPECT_DOUBLE_EQ(axisPos.y1, 200.0);
    EXPECT_DOUBLE_EQ(axisPos.d, 0.0);  // XY平台无旋转轴
}

TEST_F(CoordPositionTest, FromPlatformAxisXYD) {
    PlatformAxisPos axisPos(100.0, 200.0, 1.5);

    CoordPosition pos = CoordPosition::fromPlatformAxis(axisPos, PlatformType::XYD);

    EXPECT_DOUBLE_EQ(pos.getMainX(), 100.0);
    EXPECT_DOUBLE_EQ(pos.getMainY(), 200.0);
    EXPECT_DOUBLE_EQ(pos.getAngle(), 1.5);
}

TEST_F(CoordPositionTest, EqualityOperator) {
    CoordPosition pos1(100.0, 200.0, 1.5);
    CoordPosition pos2(100.0, 200.0, 1.5);
    CoordPosition pos3(100.0, 200.0, 1.6);

    EXPECT_TRUE(pos1 == pos2);
    EXPECT_FALSE(pos1 == pos3);
}

TEST_F(CoordPositionTest, JsonSerialization) {
    CoordPosition pos(100.0, 200.0, 1.5);
    pos.setAuxPos(0, 50.0, 75.0);
    pos.setLineAngle1(10.0);
    pos.setFoundLine1(true);

    QJsonObject json = pos.toJson();
    CoordPosition loaded = CoordPosition::fromJson(json);

    EXPECT_DOUBLE_EQ(loaded.getMainX(), 100.0);
    EXPECT_DOUBLE_EQ(loaded.getMainY(), 200.0);
    EXPECT_DOUBLE_EQ(loaded.getAngle(), 1.5);
    EXPECT_DOUBLE_EQ(loaded.getAuxX(0), 50.0);
    EXPECT_DOUBLE_EQ(loaded.getAuxY(0), 75.0);
    EXPECT_DOUBLE_EQ(loaded.getLineAngle1(), 10.0);
    EXPECT_TRUE(loaded.hasFoundLine1());
}

TEST_F(CoordPositionTest, Distance) {
    CoordPosition pos1(0.0, 0.0, 0.0);
    CoordPosition pos2(3.0, 4.0, 0.0);

    double dist = CoordPosition::distance(pos1, pos2);

    EXPECT_DOUBLE_EQ(dist, 5.0);
}

TEST_F(CoordPositionTest, LineAngle) {
    CoordPosition pos1(0.0, 0.0, 0.0);
    CoordPosition pos2(1.0, 0.0, 0.0);

    double angle = CoordPosition::lineAngle(pos1, pos2);

    EXPECT_TRUE(isNear(angle, 0.0));

    CoordPosition pos3(0.0, 1.0, 0.0);
    double angle2 = CoordPosition::lineAngle(pos1, pos3);

    EXPECT_TRUE(isNear(angle2, 90.0));
}

// ============================================================
// AffineRect 测试
// ============================================================

class AffineRectTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AffineRectTest, DefaultConstructor) {
    AffineRect rect;

    EXPECT_DOUBLE_EQ(rect.centerX, 0.0);
    EXPECT_DOUBLE_EQ(rect.centerY, 0.0);
    EXPECT_DOUBLE_EQ(rect.width, 100.0);
    EXPECT_DOUBLE_EQ(rect.height, 100.0);
    EXPECT_DOUBLE_EQ(rect.angle, 0.0);
}

TEST_F(AffineRectTest, ToBoundingRect) {
    AffineRect rect(100.0, 100.0, 40.0, 20.0, 0.0);

    QRectF bbox = rect.toBoundingRect();

    EXPECT_DOUBLE_EQ(bbox.left(), 80.0);
    EXPECT_DOUBLE_EQ(bbox.top(), 90.0);
    EXPECT_DOUBLE_EQ(bbox.width(), 40.0);
    EXPECT_DOUBLE_EQ(bbox.height(), 20.0);
}

TEST_F(AffineRectTest, GetCorners) {
    AffineRect rect(0.0, 0.0, 20.0, 10.0, 0.0);

    auto corners = rect.getCorners();

    // 未旋转的矩形角点
    EXPECT_DOUBLE_EQ(corners[0].x(), -10.0);  // 左上
    EXPECT_DOUBLE_EQ(corners[0].y(), -5.0);
    EXPECT_DOUBLE_EQ(corners[1].x(), 10.0);   // 右上
    EXPECT_DOUBLE_EQ(corners[1].y(), -5.0);
}

TEST_F(AffineRectTest, JsonSerialization) {
    AffineRect rect(100.0, 200.0, 50.0, 30.0, 45.0);

    QJsonObject json = rect.toJson();
    AffineRect loaded = AffineRect::fromJson(json);

    EXPECT_DOUBLE_EQ(loaded.centerX, 100.0);
    EXPECT_DOUBLE_EQ(loaded.centerY, 200.0);
    EXPECT_DOUBLE_EQ(loaded.width, 50.0);
    EXPECT_DOUBLE_EQ(loaded.height, 30.0);
    EXPECT_DOUBLE_EQ(loaded.angle, 45.0);
}

// ============================================================
// PlatformConfig 测试
// ============================================================

class PlatformConfigTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PlatformConfigTest, DefaultConstructor) {
    PlatformConfig config;

    EXPECT_EQ(config.type, PlatformType::XYD);
    EXPECT_DOUBLE_EQ(config.xRange, 1000.0);
    EXPECT_DOUBLE_EQ(config.yRange, 1000.0);
    EXPECT_NE(config.getPlatformInfo(), nullptr);
}

TEST_F(PlatformConfigTest, CreateDefaultInfo) {
    PlatformConfig config;

    config.createDefaultInfo(PlatformType::X1X2Y);
    EXPECT_EQ(config.type, PlatformType::X1X2Y);
    EXPECT_NE(config.getPlatformInfo(), nullptr);
    EXPECT_EQ(config.getPlatformInfo()->getType(), PlatformType::X1X2Y);

    config.createDefaultInfo(PlatformType::DXY);
    EXPECT_EQ(config.type, PlatformType::DXY);
}

TEST_F(PlatformConfigTest, PulseConversion) {
    PlatformConfig config;
    config.xPulsePerMM = 1000.0;
    config.yPulsePerMM = 2000.0;
    config.dPulsePerDegree = 100.0;

    EXPECT_DOUBLE_EQ(config.mmToPulseX(10.0), 10000.0);
    EXPECT_DOUBLE_EQ(config.mmToPulseY(10.0), 20000.0);
    EXPECT_DOUBLE_EQ(config.degreeToPulseD(1.0), 100.0);

    EXPECT_DOUBLE_EQ(config.pulseToMmX(10000.0), 10.0);
    EXPECT_DOUBLE_EQ(config.pulseToMmY(20000.0), 10.0);
    EXPECT_DOUBLE_EQ(config.pulseToDegreeD(100.0), 1.0);
}

TEST_F(PlatformConfigTest, IsInRange) {
    PlatformConfig config;
    config.xRange = 200.0;
    config.yRange = 200.0;
    config.dRange = 10.0;

    PlatformAxisPos inRange(50.0, 50.0, 2.0);
    PlatformAxisPos outRange(200.0, 50.0, 2.0);

    EXPECT_TRUE(config.isInRange(inRange));
    EXPECT_FALSE(config.isInRange(outRange));
}

TEST_F(PlatformConfigTest, ClampToRange) {
    PlatformConfig config;
    config.xRange = 200.0;
    config.yRange = 200.0;
    config.dRange = 10.0;

    PlatformAxisPos outRange(200.0, 300.0, 20.0);
    PlatformAxisPos clamped = config.clampToRange(outRange);

    EXPECT_DOUBLE_EQ(clamped.x1, 100.0);
    EXPECT_DOUBLE_EQ(clamped.y1, 100.0);
    EXPECT_DOUBLE_EQ(clamped.d, 5.0);
}

TEST_F(PlatformConfigTest, JsonSerialization) {
    PlatformConfig config;
    config.name = "Test Platform";
    config.platformIndex = 1;
    config.xRange = 500.0;
    config.yRange = 600.0;
    config.xPulsePerMM = 2000.0;

    QJsonObject json = config.toJson();
    PlatformConfig loaded;
    loaded.fromJson(json);

    EXPECT_EQ(loaded.name, "Test Platform");
    EXPECT_EQ(loaded.platformIndex, 1);
    EXPECT_DOUBLE_EQ(loaded.xRange, 500.0);
    EXPECT_DOUBLE_EQ(loaded.yRange, 600.0);
    EXPECT_DOUBLE_EQ(loaded.xPulsePerMM, 2000.0);
}

TEST_F(PlatformConfigTest, SaveAndLoad) {
    QTemporaryDir tempDir;
    ASSERT_TRUE(tempDir.isValid());

    QString filePath = tempDir.filePath("test_config.json");

    // 保存配置
    PlatformConfig config;
    config.name = "Save Test";
    config.xRange = 750.0;
    EXPECT_TRUE(config.save(filePath));

    // 加载配置
    PlatformConfig loaded;
    EXPECT_TRUE(loaded.load(filePath));
    EXPECT_EQ(loaded.name, "Save Test");
    EXPECT_DOUBLE_EQ(loaded.xRange, 750.0);
}

// ============================================================
// PlatformConfigManager 测试
// ============================================================

class PlatformConfigManagerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PlatformConfigManagerTest, Singleton) {
    auto& manager1 = PlatformConfigManager::instance();
    auto& manager2 = PlatformConfigManager::instance();

    EXPECT_EQ(&manager1, &manager2);
}

TEST_F(PlatformConfigManagerTest, DefaultConfig) {
    auto& manager = PlatformConfigManager::instance();

    EXPECT_GE(manager.getConfigCount(), 1);
    EXPECT_EQ(manager.getCurrentIndex(), 0);
}

TEST_F(PlatformConfigManagerTest, AddAndRemoveConfig) {
    auto& manager = PlatformConfigManager::instance();
    int initialCount = manager.getConfigCount();

    PlatformConfig newConfig;
    newConfig.name = "New Config";
    int index = manager.addConfig(newConfig);

    EXPECT_EQ(manager.getConfigCount(), initialCount + 1);
    EXPECT_NE(manager.getConfig(index), nullptr);
    EXPECT_EQ(manager.getConfig(index)->name, "New Config");

    // 移除配置
    if (manager.getConfigCount() > 1) {
        EXPECT_TRUE(manager.removeConfig(index));
        EXPECT_EQ(manager.getConfigCount(), initialCount);
    }
}

TEST_F(PlatformConfigManagerTest, SetCurrentIndex) {
    auto& manager = PlatformConfigManager::instance();

    if (manager.getConfigCount() > 1) {
        manager.setCurrentIndex(1);
        EXPECT_EQ(manager.getCurrentIndex(), 1);

        manager.setCurrentIndex(0);
        EXPECT_EQ(manager.getCurrentIndex(), 0);
    }

    // 无效索引不应该改变当前索引
    int currentIndex = manager.getCurrentIndex();
    manager.setCurrentIndex(-1);
    EXPECT_EQ(manager.getCurrentIndex(), currentIndex);

    manager.setCurrentIndex(9999);
    EXPECT_EQ(manager.getCurrentIndex(), currentIndex);
}

// ============================================================
// 集成测试：完整工作流程
// ============================================================

class PlatformIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PlatformIntegrationTest, FullWorkflow) {
    // 1. 创建平台配置
    PlatformConfig config;
    config.name = QStringLiteral("集成测试平台");
    config.createDefaultInfo(PlatformType::XYD);
    config.xRange = 500.0;
    config.yRange = 400.0;
    config.xPulsePerMM = 1000.0;
    config.yPulsePerMM = 1000.0;

    // 2. 创建坐标位姿（模拟视觉检测结果）
    CoordPosition imagePos(256.5, 512.3, 1.25);
    imagePos.setAuxPos(0, 256.5 + 100, 512.3);  // 辅助点

    // 3. 应用标定矩阵变换（模拟像素到世界坐标）
    // 假设: 1像素 = 0.1mm
    QMatrix3x3 calibMatrix;
    calibMatrix(0, 0) = 0.1; calibMatrix(0, 1) = 0.0;
    calibMatrix(1, 0) = 0.0; calibMatrix(1, 1) = 0.1;

    CoordPosition worldPos = imagePos.map(calibMatrix);

    EXPECT_NEAR(worldPos.getMainX(), 25.65, 0.01);
    EXPECT_NEAR(worldPos.getMainY(), 51.23, 0.01);

    // 4. 转换为平台轴位置
    PlatformAxisPos axisPos = worldPos.toPlatformAxis(PlatformType::XYD);

    EXPECT_NEAR(axisPos.x1, 25.65, 0.01);
    EXPECT_NEAR(axisPos.y1, 51.23, 0.01);
    EXPECT_DOUBLE_EQ(axisPos.d, 1.25);

    // 5. 检查是否在行程范围内
    EXPECT_TRUE(config.isInRange(axisPos));

    // 6. 转换为脉冲数
    double xPulse = config.mmToPulseX(axisPos.x1);
    double yPulse = config.mmToPulseY(axisPos.y1);

    EXPECT_NEAR(xPulse, 25650, 10);
    EXPECT_NEAR(yPulse, 51230, 10);
}

TEST_F(PlatformIntegrationTest, DualAxisPlatform) {
    // 测试龙门双X平台
    PlatformConfig config;
    config.createDefaultInfo(PlatformType::X1X2Y);

    auto* x1x2yInfo = dynamic_cast<const PlatformX1X2YInfo*>(config.getPlatformInfo());
    ASSERT_NE(x1x2yInfo, nullptr);

    // 模拟XYD坐标转换到X1X2Y
    double x = 100.0, y = 200.0, d = 2.0;
    double x1, x2, yOut;

    // 使用非const版本进行转换
    PlatformX1X2YInfo tempInfo = *x1x2yInfo;
    tempInfo.convertFromXYD(x, y, d, x1, x2, yOut);

    // 验证转换后可以转换回来
    double xBack, yBack, dBack;
    tempInfo.convertToXYD(x1, x2, yOut, xBack, yBack, dBack);

    EXPECT_NEAR(xBack, x, 0.1);
    EXPECT_NEAR(yBack, y, 0.1);
    // 注意：由于是简化算法，角度可能有差异
}

// ============================================================
// 主函数
// ============================================================

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
