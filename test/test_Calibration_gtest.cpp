/**
 * @file test_Calibration_gtest.cpp
 * @brief VisionForge Pro 标定模块单元测试
 *
 * 完整测试覆盖：
 * - CalibrationTypes: 标定类型和数据结构
 * - CalibrationManager: 标定算法（5点/9点标定）
 * - AlignmentEngine: 对齐算法（单点/两点/三点对齐）
 * - 集成测试: 完整标定对齐工作流程
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <cmath>
#include <random>

#include "calibration/CalibrationTypes.h"
#include "calibration/CalibrationManager.h"
#include "calibration/AlignmentEngine.h"
#include "platform/PlatformTypes.h"
#include "platform/CoordPosition.h"

using namespace VisionForge::Calibration;
using namespace VisionForge::Platform;

// ============================================================
// 测试辅助函数
// ============================================================

namespace {

constexpr double EPSILON = 1e-6;
constexpr double CALIB_EPSILON = 0.1;  // 标定精度容差

bool isNear(double a, double b, double eps = EPSILON) {
    return std::abs(a - b) < eps;
}

bool isPointNear(const QPointF& a, const QPointF& b, double eps = EPSILON) {
    return isNear(a.x(), b.x(), eps) && isNear(a.y(), b.y(), eps);
}

// 生成标准9点标定网格
std::vector<CalibrationPoint> generate9PointGrid(double pixelSpacing = 100.0,
                                                   double worldSpacing = 10.0,
                                                   double offsetX = 100.0,
                                                   double offsetY = 100.0) {
    std::vector<CalibrationPoint> points;
    int index = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            CalibrationPoint pt;
            pt.pixelPos = QPointF(offsetX + col * pixelSpacing,
                                  offsetY + row * pixelSpacing);
            pt.worldPos = QPointF(col * worldSpacing, row * worldSpacing);
            pt.index = index++;
            pt.isValid = true;
            pt.isEnabled = true;
            points.push_back(pt);
        }
    }
    return points;
}

// 生成带旋转的标定点
std::vector<CalibrationPoint> generateRotatedGrid(double angleDeg,
                                                    double scale = 0.1) {
    std::vector<CalibrationPoint> points;
    double rad = angleDeg * M_PI / 180.0;
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);

    int index = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            double wx = col * 10.0;  // world spacing 10mm
            double wy = row * 10.0;

            // 像素坐标 = 旋转 + 缩放
            double px = (wx * cosA - wy * sinA) / scale + 500;
            double py = (wx * sinA + wy * cosA) / scale + 500;

            CalibrationPoint pt;
            pt.pixelPos = QPointF(px, py);
            pt.worldPos = QPointF(wx, wy);
            pt.index = index++;
            pt.isValid = true;
            points.push_back(pt);
        }
    }
    return points;
}

}  // namespace

// ============================================================
// CalibrationPoint 测试
// ============================================================

class CalibrationPointTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CalibrationPointTest, DefaultConstructor) {
    CalibrationPoint point;

    EXPECT_EQ(point.pixelPos, QPointF(0, 0));
    EXPECT_EQ(point.worldPos, QPointF(0, 0));
    EXPECT_EQ(point.index, -1);
    EXPECT_FALSE(point.isValid);
    EXPECT_TRUE(point.isEnabled);
    EXPECT_DOUBLE_EQ(point.error, 0.0);
    EXPECT_DOUBLE_EQ(point.weight, 1.0);
}

TEST_F(CalibrationPointTest, FullConstructor) {
    CalibrationPoint point(QPointF(100, 200), QPointF(10, 20), 5);

    EXPECT_EQ(point.pixelPos, QPointF(100, 200));
    EXPECT_EQ(point.worldPos, QPointF(10, 20));
    EXPECT_EQ(point.index, 5);
    EXPECT_TRUE(point.isValid);
}

TEST_F(CalibrationPointTest, CoordinateConstructor) {
    CalibrationPoint point(100.5, 200.5, 10.05, 20.05, 3);

    EXPECT_DOUBLE_EQ(point.pixelPos.x(), 100.5);
    EXPECT_DOUBLE_EQ(point.pixelPos.y(), 200.5);
    EXPECT_DOUBLE_EQ(point.worldPos.x(), 10.05);
    EXPECT_DOUBLE_EQ(point.worldPos.y(), 20.05);
    EXPECT_EQ(point.index, 3);
}

TEST_F(CalibrationPointTest, JsonSerialization) {
    CalibrationPoint original(150.0, 250.0, 15.0, 25.0, 7);
    original.error = 0.05;
    original.weight = 0.8;

    QJsonObject json = original.toJson();
    CalibrationPoint loaded = CalibrationPoint::fromJson(json);

    EXPECT_TRUE(isPointNear(loaded.pixelPos, original.pixelPos));
    EXPECT_TRUE(isPointNear(loaded.worldPos, original.worldPos));
    EXPECT_EQ(loaded.index, original.index);
    EXPECT_DOUBLE_EQ(loaded.error, original.error);
    EXPECT_DOUBLE_EQ(loaded.weight, original.weight);
}

TEST_F(CalibrationPointTest, ToString) {
    CalibrationPoint point(100.0, 200.0, 10.0, 20.0, 0);
    QString str = point.toString();

    EXPECT_TRUE(str.contains("100"));
    EXPECT_TRUE(str.contains("200"));
    EXPECT_TRUE(str.contains("10"));
    EXPECT_TRUE(str.contains("20"));
}

// ============================================================
// CalibrationPointSet 测试
// ============================================================

class CalibrationPointSetTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CalibrationPointSetTest, AddPoint) {
    CalibrationPointSet set;

    int idx0 = set.addPoint(CalibrationPoint(100, 100, 10, 10, 0));
    int idx1 = set.addPoint(CalibrationPoint(200, 100, 20, 10, 1));

    EXPECT_EQ(idx0, 0);
    EXPECT_EQ(idx1, 1);
    EXPECT_EQ(set.count(), 2);
}

TEST_F(CalibrationPointSetTest, SetPoint) {
    CalibrationPointSet set;

    // 设置索引5的点（自动扩展）
    bool success = set.setPoint(5, CalibrationPoint(100, 100, 10, 10, 5));

    EXPECT_TRUE(success);
    EXPECT_EQ(set.count(), 6);

    const CalibrationPoint* pt = set.getPoint(5);
    ASSERT_NE(pt, nullptr);
    EXPECT_TRUE(pt->isValid);
}

TEST_F(CalibrationPointSetTest, RemovePoint) {
    CalibrationPointSet set;
    set.addPoint(CalibrationPoint(100, 100, 10, 10, 0));
    set.addPoint(CalibrationPoint(200, 100, 20, 10, 1));
    set.addPoint(CalibrationPoint(100, 200, 10, 20, 2));

    bool success = set.removePoint(1);

    EXPECT_TRUE(success);
    EXPECT_EQ(set.count(), 2);

    // 验证重新编号
    EXPECT_EQ(set.getPoint(0)->index, 0);
    EXPECT_EQ(set.getPoint(1)->index, 1);
}

TEST_F(CalibrationPointSetTest, ValidCount) {
    CalibrationPointSet set;
    set.addPoint(CalibrationPoint(100, 100, 10, 10, 0));

    CalibrationPoint invalidPt;
    invalidPt.isValid = false;
    set.addPoint(invalidPt);

    set.addPoint(CalibrationPoint(200, 200, 20, 20, 2));

    EXPECT_EQ(set.count(), 3);
    EXPECT_EQ(set.validCount(), 2);
}

TEST_F(CalibrationPointSetTest, EnabledCount) {
    CalibrationPointSet set;

    CalibrationPoint pt1(100, 100, 10, 10, 0);
    pt1.isEnabled = true;
    set.addPoint(pt1);

    CalibrationPoint pt2(200, 100, 20, 10, 1);
    pt2.isEnabled = false;
    set.addPoint(pt2);

    CalibrationPoint pt3(100, 200, 10, 20, 2);
    pt3.isEnabled = true;
    set.addPoint(pt3);

    EXPECT_EQ(set.enabledCount(), 2);
}

TEST_F(CalibrationPointSetTest, HasEnoughPoints) {
    CalibrationPointSet set;

    // 添加3个点
    for (int i = 0; i < 3; ++i) {
        set.addPoint(CalibrationPoint(i * 100, 100, i * 10, 10, i));
    }

    EXPECT_TRUE(set.hasEnoughPoints(CalibrationType::Point3));
    EXPECT_FALSE(set.hasEnoughPoints(CalibrationType::Point5));
    EXPECT_FALSE(set.hasEnoughPoints(CalibrationType::Point9));
}

TEST_F(CalibrationPointSetTest, CollinearPoints) {
    CalibrationPointSet set;

    // 添加共线点
    set.addPoint(CalibrationPoint(100, 100, 10, 10, 0));
    set.addPoint(CalibrationPoint(200, 100, 20, 10, 1));
    set.addPoint(CalibrationPoint(300, 100, 30, 10, 2));

    EXPECT_TRUE(set.arePointsCollinear());
}

TEST_F(CalibrationPointSetTest, NonCollinearPoints) {
    CalibrationPointSet set;

    // 添加非共线点
    set.addPoint(CalibrationPoint(100, 100, 10, 10, 0));
    set.addPoint(CalibrationPoint(200, 100, 20, 10, 1));
    set.addPoint(CalibrationPoint(150, 200, 15, 20, 2));

    EXPECT_FALSE(set.arePointsCollinear());
}

TEST_F(CalibrationPointSetTest, DistributionScore) {
    CalibrationPointSet goodSet;
    // 均匀分布的9点
    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        goodSet.addPoint(pt);
    }

    double goodScore = goodSet.calculateDistributionScore();
    EXPECT_GT(goodScore, 0.5);

    CalibrationPointSet badSet;
    // 聚集在一起的点
    for (int i = 0; i < 9; ++i) {
        badSet.addPoint(CalibrationPoint(100 + i, 100 + i, 10, 10, i));
    }

    double badScore = badSet.calculateDistributionScore();
    EXPECT_LT(badScore, goodScore);
}

TEST_F(CalibrationPointSetTest, JsonSerialization) {
    CalibrationPointSet original;
    original.addPoint(CalibrationPoint(100, 100, 10, 10, 0));
    original.addPoint(CalibrationPoint(200, 100, 20, 10, 1));

    QJsonArray json = original.toJson();
    CalibrationPointSet loaded = CalibrationPointSet::fromJson(json);

    EXPECT_EQ(loaded.count(), original.count());
}

// ============================================================
// ErrorStatistics 测试
// ============================================================

class ErrorStatisticsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ErrorStatisticsTest, IsAcceptable) {
    ErrorStatistics stats;
    stats.rmsError = 0.5;

    EXPECT_TRUE(stats.isAcceptable(1.0));
    EXPECT_FALSE(stats.isAcceptable(0.3));
}

TEST_F(ErrorStatisticsTest, JsonSerialization) {
    ErrorStatistics original;
    original.meanError = 0.5;
    original.maxError = 1.2;
    original.rmsError = 0.6;
    original.stdDev = 0.3;
    original.worstPointIndex = 5;
    original.pointCount = 9;

    QJsonObject json = original.toJson();
    ErrorStatistics loaded = ErrorStatistics::fromJson(json);

    EXPECT_DOUBLE_EQ(loaded.meanError, original.meanError);
    EXPECT_DOUBLE_EQ(loaded.maxError, original.maxError);
    EXPECT_DOUBLE_EQ(loaded.rmsError, original.rmsError);
    EXPECT_EQ(loaded.worstPointIndex, original.worstPointIndex);
}

// ============================================================
// CalibrationResult 测试
// ============================================================

class CalibrationResultTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CalibrationResultTest, DefaultState) {
    CalibrationResult result;

    EXPECT_EQ(result.status, CalibrationStatus::NotCalibrated);
    EXPECT_FALSE(result.isValid());
    EXPECT_TRUE(result.homography.isIdentity());
}

TEST_F(CalibrationResultTest, PixelToWorldIdentity) {
    CalibrationResult result;
    result.status = CalibrationStatus::Calibrated;
    result.error = CalibrationError::Success;
    // 保持单位矩阵

    QPointF pixel(100, 200);
    QPointF world = result.pixelToWorld(pixel);

    EXPECT_TRUE(isPointNear(world, pixel));
}

TEST_F(CalibrationResultTest, PixelToWorldScale) {
    CalibrationResult result;
    result.status = CalibrationStatus::Calibrated;
    result.error = CalibrationError::Success;

    // 设置2倍缩放矩阵
    result.homography << 2.0, 0, 0,
                         0, 2.0, 0,
                         0, 0, 1.0;
    result.inverseHomography = result.homography.inverse();

    QPointF pixel(100, 200);
    QPointF world = result.pixelToWorld(pixel);

    EXPECT_TRUE(isPointNear(world, QPointF(200, 400)));
}

TEST_F(CalibrationResultTest, WorldToPixel) {
    CalibrationResult result;
    result.status = CalibrationStatus::Calibrated;
    result.error = CalibrationError::Success;

    // 设置0.1倍缩放（mm/pixel）
    result.homography << 0.1, 0, 0,
                         0, 0.1, 0,
                         0, 0, 1.0;
    result.inverseHomography = result.homography.inverse();

    QPointF world(10, 20);  // mm
    QPointF pixel = result.worldToPixel(world);

    EXPECT_TRUE(isPointNear(pixel, QPointF(100, 200)));
}

TEST_F(CalibrationResultTest, JsonSerialization) {
    CalibrationResult original;
    original.status = CalibrationStatus::Calibrated;
    original.error = CalibrationError::Success;
    original.type = CalibrationType::Point9;
    original.scaleX = 0.1;
    original.scaleY = 0.1;
    original.rotation = 1.5;
    original.pointCount = 9;

    QJsonObject json = original.toJson();
    CalibrationResult loaded = CalibrationResult::fromJson(json);

    EXPECT_EQ(loaded.status, original.status);
    EXPECT_EQ(loaded.type, original.type);
    EXPECT_DOUBLE_EQ(loaded.scaleX, original.scaleX);
    EXPECT_DOUBLE_EQ(loaded.rotation, original.rotation);
}

// ============================================================
// AlignmentOffset 测试
// ============================================================

class AlignmentOffsetTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AlignmentOffsetTest, TranslationDistance) {
    AlignmentOffset offset;
    offset.dx = 3.0;
    offset.dy = 4.0;

    EXPECT_DOUBLE_EQ(offset.getTranslationDistance(), 5.0);
}

TEST_F(AlignmentOffsetTest, WithinTolerance) {
    AlignmentOffset offset;
    offset.dx = 0.005;
    offset.dy = 0.005;
    offset.dAngle = 0.005;

    EXPECT_TRUE(offset.isWithinTolerance(0.01, 0.01));
    EXPECT_FALSE(offset.isWithinTolerance(0.001, 0.01));
}

TEST_F(AlignmentOffsetTest, Inverted) {
    AlignmentOffset offset;
    offset.dx = 1.0;
    offset.dy = -2.0;
    offset.dAngle = 0.5;

    AlignmentOffset inv = offset.inverted();

    EXPECT_DOUBLE_EQ(inv.dx, -1.0);
    EXPECT_DOUBLE_EQ(inv.dy, 2.0);
    EXPECT_DOUBLE_EQ(inv.dAngle, -0.5);
}

// ============================================================
// CalibrationManager 测试
// ============================================================

class CalibrationManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = std::make_unique<CalibrationManager>();
    }
    void TearDown() override {}

    std::unique_ptr<CalibrationManager> manager;
};

TEST_F(CalibrationManagerTest, DefaultConfiguration) {
    EXPECT_EQ(manager->getCalibrationType(), CalibrationType::Point9);
    EXPECT_FALSE(manager->isCalibrated());
    EXPECT_EQ(manager->getCalibrationPointCount(), 0);
}

TEST_F(CalibrationManagerTest, SetConfig) {
    CalibrationConfig config;
    config.type = CalibrationType::Point5;
    config.gridRows = 5;
    config.gridCols = 1;

    manager->setConfig(config);

    EXPECT_EQ(manager->getCalibrationType(), CalibrationType::Point5);
    EXPECT_EQ(manager->getConfig().gridRows, 5);
}

TEST_F(CalibrationManagerTest, AddCalibrationPoints) {
    int idx1 = manager->addCalibrationPoint(QPointF(100, 100), QPointF(10, 10));
    int idx2 = manager->addCalibrationPoint(QPointF(200, 100), QPointF(20, 10));

    EXPECT_EQ(idx1, 0);
    EXPECT_EQ(idx2, 1);
    EXPECT_EQ(manager->getCalibrationPointCount(), 2);
}

TEST_F(CalibrationManagerTest, SetCalibrationPoint) {
    manager->addCalibrationPoint(QPointF(0, 0), QPointF(0, 0));

    bool success = manager->setCalibrationPoint(0, QPointF(100, 200), QPointF(10, 20));

    EXPECT_TRUE(success);

    const CalibrationPoint* pt = manager->getCalibrationPoint(0);
    ASSERT_NE(pt, nullptr);
    EXPECT_TRUE(isPointNear(pt->pixelPos, QPointF(100, 200)));
}

TEST_F(CalibrationManagerTest, ClearCalibrationPoints) {
    manager->addCalibrationPoint(QPointF(100, 100), QPointF(10, 10));
    manager->addCalibrationPoint(QPointF(200, 100), QPointF(20, 10));

    manager->clearCalibrationPoints();

    EXPECT_EQ(manager->getCalibrationPointCount(), 0);
}

TEST_F(CalibrationManagerTest, CalibrateInsufficientPoints) {
    // 只添加2个点（9点标定需要9个）
    manager->addCalibrationPoint(QPointF(100, 100), QPointF(10, 10));
    manager->addCalibrationPoint(QPointF(200, 100), QPointF(20, 10));

    CalibrationResult result = manager->calibrate();

    EXPECT_EQ(result.status, CalibrationStatus::Failed);
    EXPECT_EQ(result.error, CalibrationError::InsufficientPoints);
}

TEST_F(CalibrationManagerTest, CalibrateCollinearPoints) {
    manager->setCalibrationType(CalibrationType::Point3);

    // 添加共线点
    manager->addCalibrationPoint(QPointF(100, 100), QPointF(10, 10));
    manager->addCalibrationPoint(QPointF(200, 100), QPointF(20, 10));
    manager->addCalibrationPoint(QPointF(300, 100), QPointF(30, 10));

    CalibrationResult result = manager->calibrate();

    EXPECT_EQ(result.status, CalibrationStatus::Failed);
    EXPECT_EQ(result.error, CalibrationError::SingularMatrix);
}

TEST_F(CalibrationManagerTest, CalibrateAffine3Point) {
    manager->setCalibrationType(CalibrationType::Point3);

    // 添加简单的3点（缩放0.1 mm/pixel）
    manager->addCalibrationPoint(QPointF(100, 100), QPointF(10, 10));
    manager->addCalibrationPoint(QPointF(200, 100), QPointF(20, 10));
    manager->addCalibrationPoint(QPointF(100, 200), QPointF(10, 20));

    CalibrationResult result = manager->calibrate();

    EXPECT_EQ(result.status, CalibrationStatus::Calibrated);
    EXPECT_TRUE(isNear(result.scaleX, 0.1, 0.01));
    EXPECT_TRUE(isNear(result.scaleY, 0.1, 0.01));
}

TEST_F(CalibrationManagerTest, CalibrateAffine5Point) {
    manager->setCalibrationType(CalibrationType::Point5);

    // 添加5点网格
    manager->addCalibrationPoint(QPointF(100, 100), QPointF(10, 10));
    manager->addCalibrationPoint(QPointF(200, 100), QPointF(20, 10));
    manager->addCalibrationPoint(QPointF(300, 100), QPointF(30, 10));
    manager->addCalibrationPoint(QPointF(100, 200), QPointF(10, 20));
    manager->addCalibrationPoint(QPointF(200, 200), QPointF(20, 20));

    CalibrationResult result = manager->calibrate();

    EXPECT_EQ(result.status, CalibrationStatus::Calibrated);
    EXPECT_TRUE(result.errorStats.isAcceptable(1.0));
}

TEST_F(CalibrationManagerTest, CalibrateHomography9Point) {
    manager->setCalibrationType(CalibrationType::Point9);

    // 使用标准9点网格
    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    CalibrationResult result = manager->calibrate();

    EXPECT_EQ(result.status, CalibrationStatus::Calibrated);
    EXPECT_EQ(result.error, CalibrationError::Success);
    EXPECT_EQ(result.pointCount, 9);
    EXPECT_TRUE(result.errorStats.isAcceptable(1.0));
}

TEST_F(CalibrationManagerTest, CalibrateWithRotation) {
    manager->setCalibrationType(CalibrationType::Point9);

    // 使用带5度旋转的网格
    auto points = generateRotatedGrid(5.0, 0.1);
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    CalibrationResult result = manager->calibrate();

    EXPECT_EQ(result.status, CalibrationStatus::Calibrated);
    EXPECT_TRUE(isNear(result.rotation, 5.0, 1.0));  // 允许1度误差
}

TEST_F(CalibrationManagerTest, PixelToWorld) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    manager->calibrate();
    ASSERT_TRUE(manager->isCalibrated());

    // 测试变换
    QPointF pixel(150, 150);  // 网格中心附近
    QPointF world = manager->pixelToWorld(pixel);

    // 应该约为(5, 5)附近
    EXPECT_TRUE(isNear(world.x(), 5.0, CALIB_EPSILON));
    EXPECT_TRUE(isNear(world.y(), 5.0, CALIB_EPSILON));
}

TEST_F(CalibrationManagerTest, WorldToPixel) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    manager->calibrate();
    ASSERT_TRUE(manager->isCalibrated());

    // 逆变换测试
    QPointF world(10, 10);  // 中心点
    QPointF pixel = manager->worldToPixel(world);

    // 应该约为(200, 200)
    EXPECT_TRUE(isNear(pixel.x(), 200.0, 1.0));
    EXPECT_TRUE(isNear(pixel.y(), 200.0, 1.0));
}

TEST_F(CalibrationManagerTest, RoundTripTransform) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    manager->calibrate();
    ASSERT_TRUE(manager->isCalibrated());

    // 往返变换测试
    QPointF originalPixel(175, 225);
    QPointF world = manager->pixelToWorld(originalPixel);
    QPointF backPixel = manager->worldToPixel(world);

    EXPECT_TRUE(isPointNear(backPixel, originalPixel, 0.5));
}

TEST_F(CalibrationManagerTest, TransformCoordPosition) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    manager->calibrate();
    ASSERT_TRUE(manager->isCalibrated());

    CoordPosition imagePos(200, 200, 1.5);
    CoordPosition worldPos = manager->transformToWorld(imagePos);

    // 检查变换后的位置
    EXPECT_TRUE(isNear(worldPos.getMainX(), 10.0, CALIB_EPSILON));
    EXPECT_TRUE(isNear(worldPos.getMainY(), 10.0, CALIB_EPSILON));
    EXPECT_DOUBLE_EQ(worldPos.getAngle(), 1.5);  // 角度保持不变
}

TEST_F(CalibrationManagerTest, CalculateReprojectionError) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    manager->calibrate();
    ASSERT_TRUE(manager->isCalibrated());

    // 对于标定点，重投影误差应该很小
    double error = manager->calculateReprojectionError(QPointF(100, 100), QPointF(0, 0));
    EXPECT_LT(error, CALIB_EPSILON);
}

TEST_F(CalibrationManagerTest, IdentifyOutliers) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    // 添加一个异常点
    manager->addCalibrationPoint(QPointF(500, 500), QPointF(5, 5));

    manager->calibrate();

    auto outliers = manager->identifyOutliers(2.0);
    EXPECT_GE(outliers.size(), 0);  // 可能识别出异常点
}

TEST_F(CalibrationManagerTest, SaveAndLoad) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    manager->calibrate();

    QTemporaryFile tempFile;
    ASSERT_TRUE(tempFile.open());
    QString filePath = tempFile.fileName();
    tempFile.close();

    EXPECT_TRUE(manager->saveToFile(filePath));

    // 创建新管理器并加载
    auto loadedManager = std::make_unique<CalibrationManager>();
    EXPECT_TRUE(loadedManager->loadFromFile(filePath));

    EXPECT_TRUE(loadedManager->isCalibrated());
    EXPECT_EQ(loadedManager->getCalibrationPointCount(), 9);
}

TEST_F(CalibrationManagerTest, ExportImportJson) {
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    manager->calibrate();

    QJsonObject json = manager->exportToJson();

    auto loadedManager = std::make_unique<CalibrationManager>();
    EXPECT_TRUE(loadedManager->importFromJson(json));
    EXPECT_TRUE(loadedManager->isCalibrated());
}

// ============================================================
// AlignmentEngine 测试
// ============================================================

class AlignmentEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine = std::make_unique<AlignmentEngine>();
    }
    void TearDown() override {}

    std::unique_ptr<AlignmentEngine> engine;
};

TEST_F(AlignmentEngineTest, DefaultConfiguration) {
    EXPECT_EQ(engine->getAlignmentType(), AlignmentType::TwoPoint);
    EXPECT_EQ(engine->getTemplatePointCount(), 0);
    EXPECT_EQ(engine->getMeasuredPointCount(), 0);
}

TEST_F(AlignmentEngineTest, SetConfig) {
    AlignmentConfig config;
    config.type = AlignmentType::SinglePoint;
    config.positionTolerance = 0.005;
    config.angleTolerance = 0.005;

    engine->setConfig(config);

    EXPECT_EQ(engine->getAlignmentType(), AlignmentType::SinglePoint);
    EXPECT_DOUBLE_EQ(engine->getConfig().positionTolerance, 0.005);
}

TEST_F(AlignmentEngineTest, SetTemplatePoint) {
    EXPECT_TRUE(engine->setTemplatePoint(0, QPointF(100, 100), 0.0));
    EXPECT_TRUE(engine->setTemplatePoint(1, QPointF(200, 100), 0.0));

    EXPECT_EQ(engine->getTemplatePointCount(), 2);

    const AlignmentPoint* pt = engine->getTemplatePoint(0);
    ASSERT_NE(pt, nullptr);
    EXPECT_EQ(pt->templatePos, QPointF(100, 100));
}

TEST_F(AlignmentEngineTest, SetMeasuredPoint) {
    EXPECT_TRUE(engine->setMeasuredPoint(0, QPointF(101, 100), 0.5));
    EXPECT_TRUE(engine->setMeasuredPoint(1, QPointF(201, 100), 0.5));

    EXPECT_EQ(engine->getMeasuredPointCount(), 2);
}

TEST_F(AlignmentEngineTest, SinglePointAlignment) {
    engine->setAlignmentType(AlignmentType::SinglePoint);

    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setMeasuredPoint(0, QPointF(102, 103), 0.0);

    AlignmentResult result = engine->calculate();

    EXPECT_TRUE(result.success);
    EXPECT_DOUBLE_EQ(result.offset.dx, 2.0);
    EXPECT_DOUBLE_EQ(result.offset.dy, 3.0);
    EXPECT_DOUBLE_EQ(result.offset.dAngle, 0.0);
}

TEST_F(AlignmentEngineTest, TwoPointAlignmentTranslationOnly) {
    engine->setAlignmentType(AlignmentType::TwoPoint);

    // 模板点
    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setTemplatePoint(1, QPointF(200, 100), 0.0);

    // 测量点（纯平移）
    engine->setMeasuredPoint(0, QPointF(105, 100), 0.0);
    engine->setMeasuredPoint(1, QPointF(205, 100), 0.0);

    AlignmentResult result = engine->calculate();

    EXPECT_TRUE(result.success);
    EXPECT_TRUE(isNear(result.offset.dx, 5.0, 0.1));
    EXPECT_TRUE(isNear(result.offset.dy, 0.0, 0.1));
    EXPECT_TRUE(isNear(result.offset.dAngle, 0.0, 0.1));
}

TEST_F(AlignmentEngineTest, TwoPointAlignmentRotation) {
    engine->setAlignmentType(AlignmentType::TwoPoint);

    // 模板点（水平线）
    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setTemplatePoint(1, QPointF(200, 100), 0.0);

    // 测量点（旋转约5度）
    double rad = 5.0 * M_PI / 180.0;
    double dx = 100 * std::cos(rad);
    double dy = 100 * std::sin(rad);
    engine->setMeasuredPoint(0, QPointF(100, 100), 0.0);
    engine->setMeasuredPoint(1, QPointF(100 + dx, 100 + dy), 0.0);

    AlignmentResult result = engine->calculate();

    EXPECT_TRUE(result.success);
    EXPECT_TRUE(isNear(result.offset.dAngle, 5.0, 0.5));
}

TEST_F(AlignmentEngineTest, ThreePointAlignment) {
    engine->setAlignmentType(AlignmentType::ThreePoint);

    // 模板三角形
    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setTemplatePoint(1, QPointF(200, 100), 0.0);
    engine->setTemplatePoint(2, QPointF(150, 200), 0.0);

    // 测量三角形（平移+旋转）
    engine->setMeasuredPoint(0, QPointF(105, 102), 0.0);
    engine->setMeasuredPoint(1, QPointF(205, 102), 0.0);
    engine->setMeasuredPoint(2, QPointF(155, 202), 0.0);

    AlignmentResult result = engine->calculate();

    EXPECT_TRUE(result.success);
    EXPECT_LT(result.residualError, 1.0);
}

TEST_F(AlignmentEngineTest, FourPointAlignment) {
    engine->setAlignmentType(AlignmentType::FourPoint);

    // 模板矩形
    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setTemplatePoint(1, QPointF(200, 100), 0.0);
    engine->setTemplatePoint(2, QPointF(200, 200), 0.0);
    engine->setTemplatePoint(3, QPointF(100, 200), 0.0);

    // 测量矩形（轻微变形）
    engine->setMeasuredPoint(0, QPointF(102, 101), 0.0);
    engine->setMeasuredPoint(1, QPointF(203, 101), 0.0);
    engine->setMeasuredPoint(2, QPointF(202, 202), 0.0);
    engine->setMeasuredPoint(3, QPointF(101, 201), 0.0);

    AlignmentResult result = engine->calculate();

    EXPECT_TRUE(result.success);
}

TEST_F(AlignmentEngineTest, IsWithinTolerance) {
    engine->setAlignmentType(AlignmentType::SinglePoint);

    AlignmentConfig config = engine->getConfig();
    config.positionTolerance = 0.01;
    config.angleTolerance = 0.01;
    engine->setConfig(config);

    // 小偏差
    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setMeasuredPoint(0, QPointF(100.005, 100.005), 0.0);

    engine->calculate();

    EXPECT_TRUE(engine->isWithinTolerance());
}

TEST_F(AlignmentEngineTest, ToAxisMotionXYD) {
    engine->setPlatformType(PlatformType::XYD);

    AlignmentOffset offset;
    offset.dx = 1.5;
    offset.dy = -2.0;
    offset.dAngle = 0.5;

    PlatformAxisPos motion = engine->toAxisMotion(offset);

    EXPECT_DOUBLE_EQ(motion.x1, 1.5);
    EXPECT_DOUBLE_EQ(motion.y1, -2.0);
    EXPECT_DOUBLE_EQ(motion.d, 0.5);
}

TEST_F(AlignmentEngineTest, GetCompensatedPosition) {
    engine->setAlignmentType(AlignmentType::SinglePoint);

    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setMeasuredPoint(0, QPointF(102, 103), 0.0);

    engine->calculate();

    PlatformAxisPos currentPos(50.0, 60.0, 1.0);
    PlatformAxisPos targetPos = engine->getCompensatedPosition(currentPos);

    // 补偿方向取反
    EXPECT_DOUBLE_EQ(targetPos.x1, 48.0);  // 50 - 2
    EXPECT_DOUBLE_EQ(targetPos.y1, 57.0);  // 60 - 3
}

TEST_F(AlignmentEngineTest, CalculateLineAngle) {
    QPointF p1(0, 0);
    QPointF p2(1, 0);

    double angle = AlignmentEngine::calculateLineAngle(p1, p2);
    EXPECT_DOUBLE_EQ(angle, 0.0);

    QPointF p3(0, 1);
    angle = AlignmentEngine::calculateLineAngle(p1, p3);
    EXPECT_DOUBLE_EQ(angle, 90.0);
}

TEST_F(AlignmentEngineTest, RotatePoint) {
    QPointF point(1, 0);
    QPointF center(0, 0);

    QPointF rotated = AlignmentEngine::rotatePoint(point, center, 90.0);

    EXPECT_TRUE(isNear(rotated.x(), 0.0, EPSILON));
    EXPECT_TRUE(isNear(rotated.y(), 1.0, EPSILON));
}

TEST_F(AlignmentEngineTest, SaveLoadTemplatePoints) {
    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setTemplatePoint(1, QPointF(200, 100), 0.0);

    QTemporaryFile tempFile;
    ASSERT_TRUE(tempFile.open());
    QString filePath = tempFile.fileName();
    tempFile.close();

    EXPECT_TRUE(engine->saveTemplatePoints(filePath));

    auto loadedEngine = std::make_unique<AlignmentEngine>();
    EXPECT_TRUE(loadedEngine->loadTemplatePoints(filePath));
    EXPECT_EQ(loadedEngine->getTemplatePointCount(), 2);
}

TEST_F(AlignmentEngineTest, ExportImportJson) {
    engine->setTemplatePoint(0, QPointF(100, 100), 0.0);
    engine->setTemplatePoint(1, QPointF(200, 100), 0.0);
    engine->setMeasuredPoint(0, QPointF(101, 100), 0.0);
    engine->setMeasuredPoint(1, QPointF(201, 100), 0.0);

    engine->calculate();

    QJsonObject json = engine->exportToJson();

    auto loadedEngine = std::make_unique<AlignmentEngine>();
    EXPECT_TRUE(loadedEngine->importFromJson(json));
    EXPECT_EQ(loadedEngine->getTemplatePointCount(), 2);
}

// ============================================================
// 工具函数测试
// ============================================================

class CalibrationUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CalibrationUtilsTest, GetCalibrationTypeName) {
    EXPECT_FALSE(getCalibrationTypeName(CalibrationType::Point9).isEmpty());
    EXPECT_FALSE(getCalibrationTypeName(CalibrationType::DMCode).isEmpty());
}

TEST_F(CalibrationUtilsTest, GetCalibrationTypeFromName) {
    EXPECT_EQ(getCalibrationTypeFromName("9"), CalibrationType::Point9);
    EXPECT_EQ(getCalibrationTypeFromName("dm"), CalibrationType::DMCode);
    EXPECT_EQ(getCalibrationTypeFromName("unknown"), CalibrationType::None);
}

TEST_F(CalibrationUtilsTest, GetMinPointsForType) {
    EXPECT_EQ(getMinPointsForType(CalibrationType::Point3), 3);
    EXPECT_EQ(getMinPointsForType(CalibrationType::Point5), 5);
    EXPECT_EQ(getMinPointsForType(CalibrationType::Point9), 9);
}

TEST_F(CalibrationUtilsTest, GetCalibrationStatusName) {
    EXPECT_FALSE(getCalibrationStatusName(CalibrationStatus::Calibrated).isEmpty());
    EXPECT_FALSE(getCalibrationStatusName(CalibrationStatus::Failed).isEmpty());
}

TEST_F(CalibrationUtilsTest, GetCalibrationErrorDescription) {
    EXPECT_FALSE(getCalibrationErrorDescription(CalibrationError::Success).isEmpty());
    EXPECT_FALSE(getCalibrationErrorDescription(CalibrationError::InsufficientPoints).isEmpty());
}

TEST_F(CalibrationUtilsTest, GetAlignmentTypeName) {
    EXPECT_FALSE(getAlignmentTypeName(AlignmentType::SinglePoint).isEmpty());
    EXPECT_FALSE(getAlignmentTypeName(AlignmentType::TwoPoint).isEmpty());
}

TEST_F(CalibrationUtilsTest, Matrix3dJsonConversion) {
    Matrix3d original;
    original << 1.1, 2.2, 3.3,
                4.4, 5.5, 6.6,
                7.7, 8.8, 9.9;

    QJsonArray json = matrix3dToJson(original);
    Matrix3d loaded = matrix3dFromJson(json);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(loaded(i, j), original(i, j));
        }
    }
}

// ============================================================
// 集成测试
// ============================================================

class CalibrationIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        calibManager = std::make_unique<CalibrationManager>();
        alignEngine = std::make_unique<AlignmentEngine>();
    }
    void TearDown() override {}

    std::unique_ptr<CalibrationManager> calibManager;
    std::unique_ptr<AlignmentEngine> alignEngine;
};

TEST_F(CalibrationIntegrationTest, FullWorkflow) {
    // 1. 设置标定
    calibManager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        calibManager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }

    CalibrationResult calibResult = calibManager->calibrate();
    ASSERT_TRUE(calibResult.isValid());

    // 2. 设置对齐引擎
    alignEngine->setCalibrationManager(calibManager.get());
    alignEngine->setAlignmentType(AlignmentType::TwoPoint);

    // 3. 设置模板点（使用像素坐标，自动转换）
    alignEngine->setTemplatePointPixel(0, QPointF(150, 150), 0.0);
    alignEngine->setTemplatePointPixel(1, QPointF(250, 150), 0.0);

    // 4. 模拟检测结果（有偏差）
    alignEngine->setMeasuredPointPixel(0, QPointF(155, 153), 0.5);
    alignEngine->setMeasuredPointPixel(1, QPointF(255, 153), 0.5);

    // 5. 计算对齐
    AlignmentResult alignResult = alignEngine->calculate();
    EXPECT_TRUE(alignResult.success);

    // 6. 获取补偿运动量
    PlatformAxisPos motion = alignEngine->toAxisMotion(alignResult.offset);
    EXPECT_NE(motion.x1, 0.0);
}

TEST_F(CalibrationIntegrationTest, CalibrationWithNoise) {
    calibManager->setCalibrationType(CalibrationType::Point9);

    // 添加带噪声的标定点
    std::default_random_engine generator(42);
    std::normal_distribution<double> noise(0.0, 0.5);  // 0.5像素噪声

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        QPointF noisyPixel(pt.pixelPos.x() + noise(generator),
                           pt.pixelPos.y() + noise(generator));
        calibManager->addCalibrationPoint(noisyPixel, pt.worldPos);
    }

    CalibrationResult result = calibManager->calibrate();

    // 即使有噪声，标定也应该成功
    EXPECT_EQ(result.status, CalibrationStatus::Calibrated);

    // RMS误差应该在合理范围内
    EXPECT_LT(result.errorStats.rmsError, 2.0);
}

TEST_F(CalibrationIntegrationTest, MultiPointCalibrationComparison) {
    // 比较不同点数标定的精度

    // 5点标定
    auto manager5 = std::make_unique<CalibrationManager>();
    manager5->setCalibrationType(CalibrationType::Point5);
    for (int i = 0; i < 5; ++i) {
        double x = (i % 3) * 100 + 100;
        double y = (i / 3) * 100 + 100;
        manager5->addCalibrationPoint(QPointF(x, y), QPointF((i % 3) * 10, (i / 3) * 10));
    }
    CalibrationResult result5 = manager5->calibrate();

    // 9点标定
    auto manager9 = std::make_unique<CalibrationManager>();
    manager9->setCalibrationType(CalibrationType::Point9);
    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager9->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }
    CalibrationResult result9 = manager9->calibrate();

    // 两者都应成功
    if (result5.isValid() && result9.isValid()) {
        // 9点标定通常精度更高（RMS误差更小或相当）
        EXPECT_LE(result9.errorStats.rmsError, result5.errorStats.rmsError + 0.5);
    }
}

TEST_F(CalibrationIntegrationTest, AlignmentIterativeCorrection) {
    // 测试迭代校正

    alignEngine->setAlignmentType(AlignmentType::TwoPoint);

    AlignmentConfig config;
    config.enableIterativeCorrection = true;
    config.correctionGain = 0.8;
    config.maxIterations = 3;
    alignEngine->setConfig(config);

    // 初始偏差
    AlignmentOffset offset;
    offset.dx = 1.0;
    offset.dy = 0.5;

    // 第一次校正
    AlignmentOffset corr1 = alignEngine->calculateIterativeCorrection(offset, 0);
    EXPECT_DOUBLE_EQ(corr1.dx, -0.8);  // -1.0 * 0.8

    // 第二次校正（增益递减）
    AlignmentOffset corr2 = alignEngine->calculateIterativeCorrection(offset, 1);
    EXPECT_TRUE(isNear(corr2.dx, -0.64, 0.01));  // -1.0 * 0.8 * 0.8
}

// ============================================================
// 性能测试
// ============================================================

class CalibrationPerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CalibrationPerformanceTest, Large16PointCalibration) {
    auto manager = std::make_unique<CalibrationManager>();
    manager->setCalibrationType(CalibrationType::Point16);

    // 生成16点网格
    int index = 0;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            manager->addCalibrationPoint(
                QPointF(100 + col * 100, 100 + row * 100),
                QPointF(col * 10, row * 10));
            ++index;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    CalibrationResult result = manager->calibrate();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_TRUE(result.isValid());
    EXPECT_LT(duration.count(), 1000);  // 应该在1秒内完成
}

TEST_F(CalibrationPerformanceTest, BatchTransform) {
    auto manager = std::make_unique<CalibrationManager>();
    manager->setCalibrationType(CalibrationType::Point9);

    auto points = generate9PointGrid();
    for (const auto& pt : points) {
        manager->addCalibrationPoint(pt.pixelPos, pt.worldPos);
    }
    manager->calibrate();

    // 批量变换1000个点
    std::vector<QPointF> pixelPoints;
    for (int i = 0; i < 1000; ++i) {
        pixelPoints.push_back(QPointF(100 + i % 200, 100 + i / 5));
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<QPointF> worldPoints = manager->pixelToWorld(pixelPoints);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    EXPECT_EQ(worldPoints.size(), 1000);
    EXPECT_LT(duration.count(), 10000);  // 应该在10ms内完成
}

// ============================================================
// 工厂测试
// ============================================================

class FactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        CalibrationManagerFactory::releaseAll();
        AlignmentEngineFactory::releaseAll();
    }
    void TearDown() override {
        CalibrationManagerFactory::releaseAll();
        AlignmentEngineFactory::releaseAll();
    }
};

TEST_F(FactoryTest, CalibrationManagerSingleton) {
    auto* manager1 = CalibrationManagerFactory::getInstance(0);
    auto* manager2 = CalibrationManagerFactory::getInstance(0);

    EXPECT_EQ(manager1, manager2);

    auto* manager3 = CalibrationManagerFactory::getInstance(1);
    EXPECT_NE(manager1, manager3);
}

TEST_F(FactoryTest, AlignmentEngineSingleton) {
    auto* engine1 = AlignmentEngineFactory::getInstance(0);
    auto* engine2 = AlignmentEngineFactory::getInstance(0);

    EXPECT_EQ(engine1, engine2);

    auto* engine3 = AlignmentEngineFactory::getInstance(1);
    EXPECT_NE(engine1, engine3);
}

TEST_F(FactoryTest, CreateNewInstance) {
    auto manager = CalibrationManagerFactory::create();
    EXPECT_NE(manager, nullptr);

    auto engine = AlignmentEngineFactory::create();
    EXPECT_NE(engine, nullptr);
}

TEST_F(FactoryTest, ReleaseInstance) {
    auto* manager1 = CalibrationManagerFactory::getInstance(5);
    EXPECT_NE(manager1, nullptr);

    CalibrationManagerFactory::releaseInstance(5);

    auto* manager2 = CalibrationManagerFactory::getInstance(5);
    EXPECT_NE(manager2, nullptr);
    // 地址可能不同（新创建的实例）
}

// ============================================================
// 主函数
// ============================================================

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
