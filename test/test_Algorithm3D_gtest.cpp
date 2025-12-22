/**
 * @file test_Algorithm3D_gtest.cpp
 * @brief VisionForge Pro 3D算法模块单元测试
 *
 * 使用Google Test框架测试3D算法模块的各个组件。
 */

#include <gtest/gtest.h>
#include <QCoreApplication>

#include "algorithm3d/Algorithm3DTypes.h"
#include "algorithm3d/PointCloud3D.h"
#include "algorithm3d/DepthMap.h"
#include "algorithm3d/Calibration3D.h"
#include "algorithm3d/Measurement3D.h"

using namespace VisionForge::Algorithm3D;

// ============================================================
// Point3D 测试
// ============================================================

class Point3DTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(Point3DTest, DefaultConstructor) {
    Point3D point;
    EXPECT_FLOAT_EQ(point.x, 0.0f);
    EXPECT_FLOAT_EQ(point.y, 0.0f);
    EXPECT_FLOAT_EQ(point.z, 0.0f);
}

TEST_F(Point3DTest, ParameterizedConstructor) {
    Point3D point(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(point.x, 1.0f);
    EXPECT_FLOAT_EQ(point.y, 2.0f);
    EXPECT_FLOAT_EQ(point.z, 3.0f);
}

TEST_F(Point3DTest, ColorConstructor) {
    Point3D point(1.0f, 2.0f, 3.0f, 255, 128, 64);
    EXPECT_EQ(point.r, 255);
    EXPECT_EQ(point.g, 128);
    EXPECT_EQ(point.b, 64);
}

TEST_F(Point3DTest, Distance) {
    Point3D p1(0.0f, 0.0f, 0.0f);
    Point3D p2(3.0f, 4.0f, 0.0f);
    EXPECT_FLOAT_EQ(p1.distanceTo(p2), 5.0f);
}

TEST_F(Point3DTest, Norm) {
    Point3D point(3.0f, 4.0f, 0.0f);
    EXPECT_FLOAT_EQ(point.norm(), 5.0f);
}

TEST_F(Point3DTest, Normalized) {
    Point3D point(3.0f, 4.0f, 0.0f);
    Point3D normalized = point.normalized();
    EXPECT_NEAR(normalized.norm(), 1.0f, 1e-6f);
}

TEST_F(Point3DTest, DotProduct) {
    Point3D p1(1.0f, 0.0f, 0.0f);
    Point3D p2(0.0f, 1.0f, 0.0f);
    EXPECT_FLOAT_EQ(p1.dot(p2), 0.0f);

    Point3D p3(1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(p1.dot(p3), 1.0f);
}

TEST_F(Point3DTest, CrossProduct) {
    Point3D p1(1.0f, 0.0f, 0.0f);
    Point3D p2(0.0f, 1.0f, 0.0f);
    Point3D cross = p1.cross(p2);
    EXPECT_FLOAT_EQ(cross.x, 0.0f);
    EXPECT_FLOAT_EQ(cross.y, 0.0f);
    EXPECT_FLOAT_EQ(cross.z, 1.0f);
}

TEST_F(Point3DTest, IsValid) {
    Point3D valid(1.0f, 2.0f, 3.0f);
    EXPECT_TRUE(valid.isValid());

    Point3D invalid(std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f);
    EXPECT_FALSE(invalid.isValid());
}

TEST_F(Point3DTest, JsonSerialization) {
    Point3D original(1.0f, 2.0f, 3.0f, 255, 128, 64);
    original.setNormal(0.0f, 0.0f, 1.0f);

    QJsonObject json = original.toJson();
    Point3D restored = Point3D::fromJson(json);

    EXPECT_FLOAT_EQ(restored.x, original.x);
    EXPECT_FLOAT_EQ(restored.y, original.y);
    EXPECT_FLOAT_EQ(restored.z, original.z);
    EXPECT_EQ(restored.r, original.r);
    EXPECT_EQ(restored.g, original.g);
    EXPECT_EQ(restored.b, original.b);
}

// ============================================================
// BoundingBox3D 测试
// ============================================================

class BoundingBox3DTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(BoundingBox3DTest, DefaultConstructor) {
    BoundingBox3D box;
    EXPECT_FALSE(box.isValid());
}

TEST_F(BoundingBox3DTest, Extend) {
    BoundingBox3D box;
    box.extend(Point3D(0.0f, 0.0f, 0.0f));
    box.extend(Point3D(10.0f, 20.0f, 30.0f));

    EXPECT_FLOAT_EQ(box.minPoint.x, 0.0f);
    EXPECT_FLOAT_EQ(box.maxPoint.x, 10.0f);
    EXPECT_TRUE(box.isValid());
}

TEST_F(BoundingBox3DTest, Center) {
    BoundingBox3D box(Point3D(0.0f, 0.0f, 0.0f), Point3D(10.0f, 10.0f, 10.0f));
    Point3D center = box.center();
    EXPECT_FLOAT_EQ(center.x, 5.0f);
    EXPECT_FLOAT_EQ(center.y, 5.0f);
    EXPECT_FLOAT_EQ(center.z, 5.0f);
}

TEST_F(BoundingBox3DTest, Contains) {
    BoundingBox3D box(Point3D(0.0f, 0.0f, 0.0f), Point3D(10.0f, 10.0f, 10.0f));
    EXPECT_TRUE(box.contains(Point3D(5.0f, 5.0f, 5.0f)));
    EXPECT_FALSE(box.contains(Point3D(15.0f, 5.0f, 5.0f)));
}

// ============================================================
// PointCloud3D 测试
// ============================================================

class PointCloud3DTest : public ::testing::Test {
protected:
    void SetUp() override {
        cloud = std::make_unique<PointCloud3D>();
    }

    void TearDown() override {
        cloud.reset();
    }

    std::unique_ptr<PointCloud3D> cloud;
};

TEST_F(PointCloud3DTest, DefaultConstructor) {
    EXPECT_TRUE(cloud->empty());
    EXPECT_EQ(cloud->size(), 0);
}

TEST_F(PointCloud3DTest, AddPoint) {
    cloud->addPoint(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(cloud->size(), 1);
    EXPECT_FALSE(cloud->empty());
}

TEST_F(PointCloud3DTest, AddMultiplePoints) {
    for (int i = 0; i < 100; ++i) {
        cloud->addPoint(static_cast<float>(i), static_cast<float>(i), static_cast<float>(i));
    }
    EXPECT_EQ(cloud->size(), 100);
}

TEST_F(PointCloud3DTest, Clear) {
    cloud->addPoint(1.0f, 2.0f, 3.0f);
    cloud->clear();
    EXPECT_TRUE(cloud->empty());
}

TEST_F(PointCloud3DTest, AccessOperator) {
    cloud->addPoint(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ((*cloud)[0].x, 1.0f);
    EXPECT_FLOAT_EQ((*cloud)[0].y, 2.0f);
    EXPECT_FLOAT_EQ((*cloud)[0].z, 3.0f);
}

TEST_F(PointCloud3DTest, ComputeBoundingBox) {
    cloud->addPoint(0.0f, 0.0f, 0.0f);
    cloud->addPoint(10.0f, 20.0f, 30.0f);

    BoundingBox3D box = cloud->computeBoundingBox();
    EXPECT_FLOAT_EQ(box.minPoint.x, 0.0f);
    EXPECT_FLOAT_EQ(box.maxPoint.x, 10.0f);
    EXPECT_FLOAT_EQ(box.maxPoint.y, 20.0f);
    EXPECT_FLOAT_EQ(box.maxPoint.z, 30.0f);
}

TEST_F(PointCloud3DTest, ComputeCentroid) {
    cloud->addPoint(0.0f, 0.0f, 0.0f);
    cloud->addPoint(10.0f, 10.0f, 10.0f);

    Point3D centroid = cloud->computeCentroid();
    EXPECT_FLOAT_EQ(centroid.x, 5.0f);
    EXPECT_FLOAT_EQ(centroid.y, 5.0f);
    EXPECT_FLOAT_EQ(centroid.z, 5.0f);
}

TEST_F(PointCloud3DTest, Translate) {
    cloud->addPoint(0.0f, 0.0f, 0.0f);
    cloud->translate(10.0f, 20.0f, 30.0f);

    EXPECT_FLOAT_EQ((*cloud)[0].x, 10.0f);
    EXPECT_FLOAT_EQ((*cloud)[0].y, 20.0f);
    EXPECT_FLOAT_EQ((*cloud)[0].z, 30.0f);
}

TEST_F(PointCloud3DTest, Scale) {
    cloud->addPoint(1.0f, 2.0f, 3.0f);
    cloud->scale(2.0f);

    EXPECT_FLOAT_EQ((*cloud)[0].x, 2.0f);
    EXPECT_FLOAT_EQ((*cloud)[0].y, 4.0f);
    EXPECT_FLOAT_EQ((*cloud)[0].z, 6.0f);
}

TEST_F(PointCloud3DTest, FindNearestPoint) {
    cloud->addPoint(0.0f, 0.0f, 0.0f);
    cloud->addPoint(10.0f, 0.0f, 0.0f);
    cloud->addPoint(20.0f, 0.0f, 0.0f);

    size_t nearest = cloud->findNearestPoint(Point3D(9.0f, 0.0f, 0.0f));
    EXPECT_EQ(nearest, 1);  // 最近的是第二个点(10, 0, 0)
}

TEST_F(PointCloud3DTest, VoxelGridDownsample) {
    // 添加密集点云
    for (float x = 0; x < 10; x += 0.1f) {
        for (float y = 0; y < 10; y += 0.1f) {
            cloud->addPoint(x, y, 0.0f);
        }
    }

    size_t originalSize = cloud->size();
    PointCloud3D downsampled = cloud->voxelGridDownsample(1.0);

    EXPECT_LT(downsampled.size(), originalSize);
    EXPECT_GT(downsampled.size(), 0);
}

TEST_F(PointCloud3DTest, SetUniformColor) {
    cloud->addPoint(0.0f, 0.0f, 0.0f);
    cloud->addPoint(1.0f, 1.0f, 1.0f);

    cloud->setUniformColor(255, 128, 64);

    EXPECT_EQ((*cloud)[0].r, 255);
    EXPECT_EQ((*cloud)[0].g, 128);
    EXPECT_EQ((*cloud)[0].b, 64);
    EXPECT_EQ((*cloud)[1].r, 255);
}

TEST_F(PointCloud3DTest, Clone) {
    cloud->addPoint(1.0f, 2.0f, 3.0f);
    cloud->setName("TestCloud");

    PointCloud3D cloned = cloud->clone();
    EXPECT_EQ(cloned.size(), cloud->size());
    EXPECT_FLOAT_EQ(cloned[0].x, (*cloud)[0].x);
    EXPECT_EQ(cloned.getName(), cloud->getName());
}

// ============================================================
// DepthMap 测试
// ============================================================

class DepthMapTest : public ::testing::Test {
protected:
    void SetUp() override {
        depthMap = std::make_unique<DepthMap>(100, 100, DepthFormat::Float32);
    }

    std::unique_ptr<DepthMap> depthMap;
};

TEST_F(DepthMapTest, DefaultConstructor) {
    DepthMap dm;
    EXPECT_TRUE(dm.isEmpty());
}

TEST_F(DepthMapTest, SizeConstructor) {
    EXPECT_EQ(depthMap->width(), 100);
    EXPECT_EQ(depthMap->height(), 100);
    EXPECT_FALSE(depthMap->isEmpty());
}

TEST_F(DepthMapTest, SetAndGetDepth) {
    depthMap->setDepth(50, 50, 1000.0);
    EXPECT_DOUBLE_EQ(depthMap->getDepth(50, 50), 1000.0);
}

TEST_F(DepthMapTest, IsValid) {
    depthMap->setDepth(50, 50, 1000.0);
    EXPECT_TRUE(depthMap->isValid(50, 50));

    depthMap->setDepth(51, 51, 0.0);
    EXPECT_FALSE(depthMap->isValid(51, 51));
}

TEST_F(DepthMapTest, GetDepthRange) {
    depthMap->setDepth(0, 0, 100.0);
    depthMap->setDepth(50, 50, 500.0);
    depthMap->setDepth(99, 99, 1000.0);

    double minDepth, maxDepth;
    depthMap->getDepthRange(minDepth, maxDepth);

    EXPECT_DOUBLE_EQ(minDepth, 100.0);
    EXPECT_DOUBLE_EQ(maxDepth, 1000.0);
}

TEST_F(DepthMapTest, MedianFilter) {
    // 设置一些深度值
    for (int y = 40; y < 60; ++y) {
        for (int x = 40; x < 60; ++x) {
            depthMap->setDepth(x, y, 1000.0);
        }
    }

    // 添加一个异常值
    depthMap->setDepth(50, 50, 5000.0);

    DepthMap filtered = depthMap->medianFilter(5);

    // 中值滤波后异常值应该被平滑
    EXPECT_LT(filtered.getDepth(50, 50), 5000.0);
}

TEST_F(DepthMapTest, Clone) {
    depthMap->setDepth(50, 50, 1000.0);

    DepthMap cloned = depthMap->clone();
    EXPECT_EQ(cloned.width(), depthMap->width());
    EXPECT_EQ(cloned.height(), depthMap->height());
    EXPECT_DOUBLE_EQ(cloned.getDepth(50, 50), depthMap->getDepth(50, 50));
}

// ============================================================
// Plane3D 测试
// ============================================================

class Plane3DTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(Plane3DTest, DefaultConstructor) {
    Plane3D plane;
    EXPECT_DOUBLE_EQ(plane.c, 1.0);  // 默认水平面
}

TEST_F(Plane3DTest, FromNormalAndPoint) {
    Vector3d normal(0, 0, 1);
    Vector3d point(0, 0, 5);

    Plane3D plane = Plane3D::fromNormalAndPoint(normal, point);

    // 平面方程: z - 5 = 0
    EXPECT_DOUBLE_EQ(plane.c, 1.0);
    EXPECT_DOUBLE_EQ(plane.d, -5.0);
}

TEST_F(Plane3DTest, DistanceToPoint) {
    Plane3D plane = Plane3D::fromNormalAndPoint(Vector3d(0, 0, 1), Vector3d(0, 0, 0));

    double dist = plane.distanceToPoint(Vector3d(0, 0, 10));
    EXPECT_DOUBLE_EQ(dist, 10.0);
}

TEST_F(Plane3DTest, ProjectPoint) {
    Plane3D plane = Plane3D::fromNormalAndPoint(Vector3d(0, 0, 1), Vector3d(0, 0, 0));

    Vector3d point(5, 5, 10);
    Vector3d projected = plane.projectPoint(point);

    EXPECT_NEAR(projected.x(), 5.0, 1e-6);
    EXPECT_NEAR(projected.y(), 5.0, 1e-6);
    EXPECT_NEAR(projected.z(), 0.0, 1e-6);
}

// ============================================================
// Measurement3D 测试
// ============================================================

class Measurement3DTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = std::make_unique<Measurement3DManager>();
    }

    std::unique_ptr<Measurement3DManager> manager;
};

TEST_F(Measurement3DTest, PointToPointDistance) {
    Point3D p1(0.0f, 0.0f, 0.0f);
    Point3D p2(3.0f, 4.0f, 0.0f);

    Measurement3DResult result = manager->measureDistance(p1, p2);

    EXPECT_TRUE(result.success);
    EXPECT_DOUBLE_EQ(result.value, 5.0);
    EXPECT_EQ(result.type, Measurement3DType::Distance);
}

TEST_F(Measurement3DTest, PointToPlaneDistance) {
    Point3D point(0.0f, 0.0f, 10.0f);
    Plane3D plane = Plane3D::fromNormalAndPoint(Vector3d(0, 0, 1), Vector3d(0, 0, 0));

    Measurement3DResult result = manager->measurePointToPlane(point, plane);

    EXPECT_TRUE(result.success);
    EXPECT_NEAR(result.value, 10.0, 1e-6);
}

TEST_F(Measurement3DTest, LineToLineAngle) {
    Line3D line1(Point3D(0.0f, 0.0f, 0.0f), Point3D(1.0f, 0.0f, 0.0f));  // X轴方向
    Line3D line2(Point3D(0.0f, 0.0f, 0.0f), Point3D(0.0f, 1.0f, 0.0f));  // Y轴方向

    Measurement3DResult result = manager->measureAngle(line1, line2);

    EXPECT_TRUE(result.success);
    EXPECT_NEAR(result.value, 90.0, 1e-6);  // 90度
}

TEST_F(Measurement3DTest, PlaneToPlaneAngle) {
    Plane3D plane1 = Plane3D::fromNormalAndPoint(Vector3d(0, 0, 1), Vector3d(0, 0, 0));  // XY平面
    Plane3D plane2 = Plane3D::fromNormalAndPoint(Vector3d(1, 0, 0), Vector3d(0, 0, 0));  // YZ平面

    Measurement3DResult result = manager->measureAngle(plane1, plane2);

    EXPECT_TRUE(result.success);
    EXPECT_NEAR(result.value, 90.0, 1e-6);  // 90度
}

TEST_F(Measurement3DTest, FlatnessMeasurement) {
    PointCloud3D cloud;

    // 创建一个近似平面的点云
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            float z = 0.0f + static_cast<float>(rand() % 100) / 1000.0f;  // 小的噪声
            cloud.addPoint(static_cast<float>(i), static_cast<float>(j), z);
        }
    }

    Measurement3DResult result = manager->measureFlatness(cloud);

    EXPECT_TRUE(result.success);
    EXPECT_LT(result.value, 0.2);  // 平面度应该很小
}

// ============================================================
// GeometryFitter 测试
// ============================================================

class GeometryFitterTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(GeometryFitterTest, FitPlane) {
    PointCloud3D cloud;

    // 创建XY平面上的点
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            cloud.addPoint(static_cast<float>(i), static_cast<float>(j), 0.0f);
        }
    }

    PlaneFitResult result = GeometryFitter::fitPlane(cloud);

    EXPECT_TRUE(result.success);
    EXPECT_NEAR(std::abs(result.plane.c), 1.0, 0.1);  // 法线应该接近Z轴
    EXPECT_NEAR(result.plane.d, 0.0, 0.1);  // d应该接近0
}

TEST_F(GeometryFitterTest, FitSphere) {
    PointCloud3D cloud;

    // 创建球面上的点
    double radius = 10.0;
    for (int i = 0; i < 100; ++i) {
        double theta = 2.0 * M_PI * i / 100;
        for (int j = 0; j < 50; ++j) {
            double phi = M_PI * j / 50;
            float x = static_cast<float>(radius * sin(phi) * cos(theta));
            float y = static_cast<float>(radius * sin(phi) * sin(theta));
            float z = static_cast<float>(radius * cos(phi));
            cloud.addPoint(x, y, z);
        }
    }

    SphereFitResult result = GeometryFitter::fitSphere(cloud);

    EXPECT_TRUE(result.success);
    EXPECT_NEAR(result.sphere.radius, radius, 0.5);
    EXPECT_NEAR(result.sphere.center.x(), 0.0, 0.5);
    EXPECT_NEAR(result.sphere.center.y(), 0.0, 0.5);
    EXPECT_NEAR(result.sphere.center.z(), 0.0, 0.5);
}

// ============================================================
// CameraIntrinsics 测试
// ============================================================

class CameraIntrinsicsTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(CameraIntrinsicsTest, DefaultConstructor) {
    CameraIntrinsics intrinsics;
    EXPECT_FALSE(intrinsics.isValid());
}

TEST_F(CameraIntrinsicsTest, SetAndGetMatrix) {
    CameraIntrinsics intrinsics;
    intrinsics.fx = 1000.0;
    intrinsics.fy = 1000.0;
    intrinsics.cx = 320.0;
    intrinsics.cy = 240.0;
    intrinsics.width = 640;
    intrinsics.height = 480;

    EXPECT_TRUE(intrinsics.isValid());

    Matrix3d K = intrinsics.getMatrix();
    EXPECT_DOUBLE_EQ(K(0, 0), 1000.0);
    EXPECT_DOUBLE_EQ(K(1, 1), 1000.0);
    EXPECT_DOUBLE_EQ(K(0, 2), 320.0);
    EXPECT_DOUBLE_EQ(K(1, 2), 240.0);
}

TEST_F(CameraIntrinsicsTest, JsonSerialization) {
    CameraIntrinsics original;
    original.fx = 1000.0;
    original.fy = 1000.0;
    original.cx = 320.0;
    original.cy = 240.0;
    original.width = 640;
    original.height = 480;

    QJsonObject json = original.toJson();
    CameraIntrinsics restored = CameraIntrinsics::fromJson(json);

    EXPECT_DOUBLE_EQ(restored.fx, original.fx);
    EXPECT_DOUBLE_EQ(restored.fy, original.fy);
    EXPECT_DOUBLE_EQ(restored.cx, original.cx);
    EXPECT_DOUBLE_EQ(restored.cy, original.cy);
}

// ============================================================
// Calibration3D 测试
// ============================================================

class Calibration3DTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = std::make_unique<Calibration3DManager>();
    }

    std::unique_ptr<Calibration3DManager> manager;
};

TEST_F(Calibration3DTest, BoardConfig) {
    CalibrationBoardConfig config;
    config.type = CalibrationBoardType::Chessboard;
    config.rows = 9;
    config.cols = 6;
    config.squareSize = 25.0;

    manager->setBoardConfig(config);

    const CalibrationBoardConfig& retrieved = manager->getBoardConfig();
    EXPECT_EQ(retrieved.rows, 9);
    EXPECT_EQ(retrieved.cols, 6);
    EXPECT_DOUBLE_EQ(retrieved.squareSize, 25.0);
}

TEST_F(Calibration3DTest, Generate3DPoints) {
    CalibrationBoardConfig config;
    config.rows = 3;
    config.cols = 3;
    config.squareSize = 10.0;

    std::vector<Point3D> points = config.generate3DPoints();

    EXPECT_EQ(points.size(), 9);
    EXPECT_FLOAT_EQ(points[0].x, 0.0f);
    EXPECT_FLOAT_EQ(points[0].y, 0.0f);
    EXPECT_FLOAT_EQ(points[8].x, 20.0f);  // (2 * 10)
    EXPECT_FLOAT_EQ(points[8].y, 20.0f);
}

TEST_F(Calibration3DTest, CameraCalibrationResultJson) {
    CameraCalibrationResult original;
    original.success = true;
    original.reprojectionError = 0.5;
    original.intrinsics.fx = 1000.0;
    original.intrinsics.fy = 1000.0;

    QJsonObject json = original.toJson();
    CameraCalibrationResult restored = CameraCalibrationResult::fromJson(json);

    EXPECT_EQ(restored.success, original.success);
    EXPECT_DOUBLE_EQ(restored.reprojectionError, original.reprojectionError);
    EXPECT_DOUBLE_EQ(restored.intrinsics.fx, original.intrinsics.fx);
}

// ============================================================
// 工具函数测试
// ============================================================

class UtilityFunctionsTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(UtilityFunctionsTest, ErrorDescriptions) {
    QString desc = getAlgorithm3DErrorDescription(Algorithm3DError::Success);
    EXPECT_FALSE(desc.isEmpty());

    desc = getAlgorithm3DErrorDescription(Algorithm3DError::InvalidInput);
    EXPECT_FALSE(desc.isEmpty());
}

TEST_F(UtilityFunctionsTest, MeasurementTypeNames) {
    QString name = getMeasurement3DTypeName(Measurement3DType::Distance);
    EXPECT_FALSE(name.isEmpty());

    name = getMeasurement3DTypeName(Measurement3DType::Flatness);
    EXPECT_FALSE(name.isEmpty());
}

TEST_F(UtilityFunctionsTest, FilterTypeNames) {
    QString name = getPointCloudFilterTypeName(PointCloudFilterType::StatisticalOutlier);
    EXPECT_FALSE(name.isEmpty());

    name = getPointCloudFilterTypeName(PointCloudFilterType::VoxelGrid);
    EXPECT_FALSE(name.isEmpty());
}

// ============================================================
// Main
// ============================================================

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
