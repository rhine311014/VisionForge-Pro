/**
 * @file Algorithm3DTypes.cpp
 * @brief VisionForge Pro 3D算法类型实现
 */

#include "algorithm3d/Algorithm3DTypes.h"

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// Point3D
// ============================================================

QJsonObject Point3D::toJson() const {
    QJsonObject json;
    json["x"] = static_cast<double>(x);
    json["y"] = static_cast<double>(y);
    json["z"] = static_cast<double>(z);
    json["r"] = r;
    json["g"] = g;
    json["b"] = b;
    json["a"] = a;
    json["normal_x"] = static_cast<double>(normal_x);
    json["normal_y"] = static_cast<double>(normal_y);
    json["normal_z"] = static_cast<double>(normal_z);
    json["curvature"] = static_cast<double>(curvature);
    return json;
}

Point3D Point3D::fromJson(const QJsonObject& json) {
    Point3D point;
    point.x = static_cast<float>(json["x"].toDouble());
    point.y = static_cast<float>(json["y"].toDouble());
    point.z = static_cast<float>(json["z"].toDouble());
    point.r = static_cast<uint8_t>(json["r"].toInt(255));
    point.g = static_cast<uint8_t>(json["g"].toInt(255));
    point.b = static_cast<uint8_t>(json["b"].toInt(255));
    point.a = static_cast<uint8_t>(json["a"].toInt(255));
    point.normal_x = static_cast<float>(json["normal_x"].toDouble());
    point.normal_y = static_cast<float>(json["normal_y"].toDouble());
    point.normal_z = static_cast<float>(json["normal_z"].toDouble());
    point.curvature = static_cast<float>(json["curvature"].toDouble());
    return point;
}

QString Point3D::toString() const {
    return QString("Point3D(%1, %2, %3)")
        .arg(x, 0, 'f', 3)
        .arg(y, 0, 'f', 3)
        .arg(z, 0, 'f', 3);
}

// ============================================================
// BoundingBox3D
// ============================================================

QJsonObject BoundingBox3D::toJson() const {
    QJsonObject json;
    json["min"] = minPoint.toJson();
    json["max"] = maxPoint.toJson();
    return json;
}

BoundingBox3D BoundingBox3D::fromJson(const QJsonObject& json) {
    BoundingBox3D box;
    box.minPoint = Point3D::fromJson(json["min"].toObject());
    box.maxPoint = Point3D::fromJson(json["max"].toObject());
    return box;
}

// ============================================================
// CameraIntrinsics
// ============================================================

QJsonObject CameraIntrinsics::toJson() const {
    QJsonObject json;
    json["fx"] = fx;
    json["fy"] = fy;
    json["cx"] = cx;
    json["cy"] = cy;
    json["k1"] = k1;
    json["k2"] = k2;
    json["k3"] = k3;
    json["p1"] = p1;
    json["p2"] = p2;
    json["width"] = width;
    json["height"] = height;
    return json;
}

CameraIntrinsics CameraIntrinsics::fromJson(const QJsonObject& json) {
    CameraIntrinsics intrinsics;
    intrinsics.fx = json["fx"].toDouble();
    intrinsics.fy = json["fy"].toDouble();
    intrinsics.cx = json["cx"].toDouble();
    intrinsics.cy = json["cy"].toDouble();
    intrinsics.k1 = json["k1"].toDouble();
    intrinsics.k2 = json["k2"].toDouble();
    intrinsics.k3 = json["k3"].toDouble();
    intrinsics.p1 = json["p1"].toDouble();
    intrinsics.p2 = json["p2"].toDouble();
    intrinsics.width = json["width"].toInt();
    intrinsics.height = json["height"].toInt();
    return intrinsics;
}

// ============================================================
// CameraExtrinsics
// ============================================================

QJsonObject CameraExtrinsics::toJson() const {
    QJsonObject json;
    QJsonArray rotArray;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rotArray.append(rotation(i, j));
        }
    }
    json["rotation"] = rotArray;

    QJsonArray transArray;
    transArray.append(translation.x());
    transArray.append(translation.y());
    transArray.append(translation.z());
    json["translation"] = transArray;

    return json;
}

CameraExtrinsics CameraExtrinsics::fromJson(const QJsonObject& json) {
    CameraExtrinsics extrinsics;

    QJsonArray rotArray = json["rotation"].toArray();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            extrinsics.rotation(i, j) = rotArray[i * 3 + j].toDouble();
        }
    }

    QJsonArray transArray = json["translation"].toArray();
    extrinsics.translation.x() = transArray[0].toDouble();
    extrinsics.translation.y() = transArray[1].toDouble();
    extrinsics.translation.z() = transArray[2].toDouble();

    return extrinsics;
}

// ============================================================
// StereoParameters
// ============================================================

QJsonObject StereoParameters::toJson() const {
    QJsonObject json;
    json["leftCamera"] = leftCamera.toJson();
    json["rightCamera"] = rightCamera.toJson();
    json["rightToLeft"] = rightToLeft.toJson();
    json["baseline"] = baseline;
    json["minDisparity"] = minDisparity;
    json["maxDisparity"] = maxDisparity;
    json["blockSize"] = blockSize;
    return json;
}

StereoParameters StereoParameters::fromJson(const QJsonObject& json) {
    StereoParameters params;
    params.leftCamera = CameraIntrinsics::fromJson(json["leftCamera"].toObject());
    params.rightCamera = CameraIntrinsics::fromJson(json["rightCamera"].toObject());
    params.rightToLeft = CameraExtrinsics::fromJson(json["rightToLeft"].toObject());
    params.baseline = json["baseline"].toDouble();
    params.minDisparity = json["minDisparity"].toDouble();
    params.maxDisparity = json["maxDisparity"].toDouble(128.0);
    params.blockSize = json["blockSize"].toInt(5);
    return params;
}

// ============================================================
// Plane3D
// ============================================================

QJsonObject Plane3D::toJson() const {
    QJsonObject json;
    json["a"] = a;
    json["b"] = b;
    json["c"] = c;
    json["d"] = d;
    return json;
}

Plane3D Plane3D::fromJson(const QJsonObject& json) {
    return Plane3D(
        json["a"].toDouble(),
        json["b"].toDouble(),
        json["c"].toDouble(1.0),
        json["d"].toDouble()
    );
}

// ============================================================
// Line3D
// ============================================================

QJsonObject Line3D::toJson() const {
    QJsonObject json;
    json["start"] = start.toJson();
    json["end"] = end.toJson();
    return json;
}

Line3D Line3D::fromJson(const QJsonObject& json) {
    Line3D line;
    line.start = Point3D::fromJson(json["start"].toObject());
    line.end = Point3D::fromJson(json["end"].toObject());
    return line;
}

// ============================================================
// Cylinder3D
// ============================================================

QJsonObject Cylinder3D::toJson() const {
    QJsonObject json;
    QJsonArray axisPointArray;
    axisPointArray.append(axisPoint.x());
    axisPointArray.append(axisPoint.y());
    axisPointArray.append(axisPoint.z());
    json["axisPoint"] = axisPointArray;

    QJsonArray axisDirArray;
    axisDirArray.append(axisDirection.x());
    axisDirArray.append(axisDirection.y());
    axisDirArray.append(axisDirection.z());
    json["axisDirection"] = axisDirArray;

    json["radius"] = radius;
    json["height"] = height;
    return json;
}

Cylinder3D Cylinder3D::fromJson(const QJsonObject& json) {
    Cylinder3D cylinder;
    QJsonArray axisPointArray = json["axisPoint"].toArray();
    cylinder.axisPoint = Vector3d(
        axisPointArray[0].toDouble(),
        axisPointArray[1].toDouble(),
        axisPointArray[2].toDouble()
    );

    QJsonArray axisDirArray = json["axisDirection"].toArray();
    cylinder.axisDirection = Vector3d(
        axisDirArray[0].toDouble(),
        axisDirArray[1].toDouble(),
        axisDirArray[2].toDouble()
    );

    cylinder.radius = json["radius"].toDouble();
    cylinder.height = json["height"].toDouble();
    return cylinder;
}

// ============================================================
// Sphere3D
// ============================================================

QJsonObject Sphere3D::toJson() const {
    QJsonObject json;
    QJsonArray centerArray;
    centerArray.append(center.x());
    centerArray.append(center.y());
    centerArray.append(center.z());
    json["center"] = centerArray;
    json["radius"] = radius;
    return json;
}

Sphere3D Sphere3D::fromJson(const QJsonObject& json) {
    QJsonArray centerArray = json["center"].toArray();
    Vector3d c(
        centerArray[0].toDouble(),
        centerArray[1].toDouble(),
        centerArray[2].toDouble()
    );
    return Sphere3D(c, json["radius"].toDouble());
}

// ============================================================
// Measurement3DResult
// ============================================================

QJsonObject Measurement3DResult::toJson() const {
    QJsonObject json;
    json["type"] = static_cast<int>(type);
    json["success"] = success;
    json["errorMessage"] = errorMessage;
    json["value"] = value;
    json["unit"] = unit;
    json["value2"] = value2;
    json["value3"] = value3;
    json["stdDev"] = stdDev;
    json["confidence"] = confidence;

    QJsonArray pointsArray;
    for (const auto& point : points) {
        pointsArray.append(point.toJson());
    }
    json["points"] = pointsArray;

    QJsonArray linesArray;
    for (const auto& line : lines) {
        linesArray.append(line.toJson());
    }
    json["lines"] = linesArray;

    json["plane"] = plane.toJson();
    return json;
}

Measurement3DResult Measurement3DResult::fromJson(const QJsonObject& json) {
    Measurement3DResult result;
    result.type = static_cast<Measurement3DType>(json["type"].toInt());
    result.success = json["success"].toBool();
    result.errorMessage = json["errorMessage"].toString();
    result.value = json["value"].toDouble();
    result.unit = json["unit"].toString("mm");
    result.value2 = json["value2"].toDouble();
    result.value3 = json["value3"].toDouble();
    result.stdDev = json["stdDev"].toDouble();
    result.confidence = json["confidence"].toDouble();

    QJsonArray pointsArray = json["points"].toArray();
    for (const auto& pointVal : pointsArray) {
        result.points.push_back(Point3D::fromJson(pointVal.toObject()));
    }

    QJsonArray linesArray = json["lines"].toArray();
    for (const auto& lineVal : linesArray) {
        result.lines.push_back(Line3D::fromJson(lineVal.toObject()));
    }

    result.plane = Plane3D::fromJson(json["plane"].toObject());
    return result;
}

QString Measurement3DResult::toString() const {
    return QString("%1: %2 %3 (success: %4)")
        .arg(getMeasurement3DTypeName(type))
        .arg(value, 0, 'f', 3)
        .arg(unit)
        .arg(success ? "true" : "false");
}

// ============================================================
// PointCloudConfig
// ============================================================

QJsonObject PointCloudConfig::toJson() const {
    QJsonObject json;
    json["filterType"] = static_cast<int>(filterType);
    json["statisticalMeanK"] = statisticalMeanK;
    json["statisticalStddevMul"] = statisticalStddevMul;
    json["radiusSearchRadius"] = radiusSearchRadius;
    json["radiusMinNeighbors"] = radiusMinNeighbors;
    json["enableDownsampling"] = enableDownsampling;
    json["voxelLeafSize"] = voxelLeafSize;
    json["computeNormals"] = computeNormals;
    json["normalKSearch"] = normalKSearch;
    json["normalRadiusSearch"] = normalRadiusSearch;
    json["icpMaxCorrespondenceDistance"] = icpMaxCorrespondenceDistance;
    json["icpMaxIterations"] = icpMaxIterations;
    json["icpTransformationEpsilon"] = icpTransformationEpsilon;
    json["icpEuclideanFitnessEpsilon"] = icpEuclideanFitnessEpsilon;
    json["segmentDistanceThreshold"] = segmentDistanceThreshold;
    json["segmentMaxIterations"] = segmentMaxIterations;
    return json;
}

PointCloudConfig PointCloudConfig::fromJson(const QJsonObject& json) {
    PointCloudConfig config;
    config.filterType = static_cast<PointCloudFilterType>(json["filterType"].toInt());
    config.statisticalMeanK = json["statisticalMeanK"].toInt(50);
    config.statisticalStddevMul = json["statisticalStddevMul"].toDouble(1.0);
    config.radiusSearchRadius = json["radiusSearchRadius"].toDouble(5.0);
    config.radiusMinNeighbors = json["radiusMinNeighbors"].toInt(2);
    config.enableDownsampling = json["enableDownsampling"].toBool(true);
    config.voxelLeafSize = json["voxelLeafSize"].toDouble(1.0);
    config.computeNormals = json["computeNormals"].toBool(true);
    config.normalKSearch = json["normalKSearch"].toInt(30);
    config.normalRadiusSearch = json["normalRadiusSearch"].toDouble();
    config.icpMaxCorrespondenceDistance = json["icpMaxCorrespondenceDistance"].toDouble(10.0);
    config.icpMaxIterations = json["icpMaxIterations"].toInt(50);
    config.icpTransformationEpsilon = json["icpTransformationEpsilon"].toDouble(1e-8);
    config.icpEuclideanFitnessEpsilon = json["icpEuclideanFitnessEpsilon"].toDouble(1e-6);
    config.segmentDistanceThreshold = json["segmentDistanceThreshold"].toDouble(1.0);
    config.segmentMaxIterations = json["segmentMaxIterations"].toInt(100);
    return config;
}

// ============================================================
// DepthMapConfig
// ============================================================

QJsonObject DepthMapConfig::toJson() const {
    QJsonObject json;
    json["enableMedianFilter"] = enableMedianFilter;
    json["medianKernelSize"] = medianKernelSize;
    json["enableBilateralFilter"] = enableBilateralFilter;
    json["bilateralD"] = bilateralD;
    json["bilateralSigmaColor"] = bilateralSigmaColor;
    json["bilateralSigmaSpace"] = bilateralSigmaSpace;
    json["enableHoleFilling"] = enableHoleFilling;
    json["holeFillMaxSize"] = holeFillMaxSize;
    json["enableEdgePreserving"] = enableEdgePreserving;
    json["edgeThreshold"] = edgeThreshold;
    json["minDepth"] = minDepth;
    json["maxDepth"] = maxDepth;
    return json;
}

DepthMapConfig DepthMapConfig::fromJson(const QJsonObject& json) {
    DepthMapConfig config;
    config.enableMedianFilter = json["enableMedianFilter"].toBool(true);
    config.medianKernelSize = json["medianKernelSize"].toInt(5);
    config.enableBilateralFilter = json["enableBilateralFilter"].toBool(true);
    config.bilateralD = json["bilateralD"].toInt(9);
    config.bilateralSigmaColor = json["bilateralSigmaColor"].toDouble(75.0);
    config.bilateralSigmaSpace = json["bilateralSigmaSpace"].toDouble(75.0);
    config.enableHoleFilling = json["enableHoleFilling"].toBool(true);
    config.holeFillMaxSize = json["holeFillMaxSize"].toInt(10);
    config.enableEdgePreserving = json["enableEdgePreserving"].toBool(true);
    config.edgeThreshold = json["edgeThreshold"].toDouble(30.0);
    config.minDepth = json["minDepth"].toDouble(100.0);
    config.maxDepth = json["maxDepth"].toDouble(10000.0);
    return config;
}

// ============================================================
// StereoMatchConfig
// ============================================================

QJsonObject StereoMatchConfig::toJson() const {
    QJsonObject json;
    json["algorithm"] = static_cast<int>(algorithm);
    json["minDisparity"] = minDisparity;
    json["numDisparities"] = numDisparities;
    json["blockSize"] = blockSize;
    json["p1"] = p1;
    json["p2"] = p2;
    json["disp12MaxDiff"] = disp12MaxDiff;
    json["preFilterCap"] = preFilterCap;
    json["uniquenessRatio"] = uniquenessRatio;
    json["speckleWindowSize"] = speckleWindowSize;
    json["speckleRange"] = speckleRange;
    json["enableWLSFilter"] = enableWLSFilter;
    json["wlsLambda"] = wlsLambda;
    json["wlsSigma"] = wlsSigma;
    return json;
}

StereoMatchConfig StereoMatchConfig::fromJson(const QJsonObject& json) {
    StereoMatchConfig config;
    config.algorithm = static_cast<StereoMatchConfig::Algorithm>(json["algorithm"].toInt(1));
    config.minDisparity = json["minDisparity"].toInt();
    config.numDisparities = json["numDisparities"].toInt(128);
    config.blockSize = json["blockSize"].toInt(5);
    config.p1 = json["p1"].toInt();
    config.p2 = json["p2"].toInt();
    config.disp12MaxDiff = json["disp12MaxDiff"].toInt(1);
    config.preFilterCap = json["preFilterCap"].toInt(63);
    config.uniquenessRatio = json["uniquenessRatio"].toInt(10);
    config.speckleWindowSize = json["speckleWindowSize"].toInt(100);
    config.speckleRange = json["speckleRange"].toInt(32);
    config.enableWLSFilter = json["enableWLSFilter"].toBool(true);
    config.wlsLambda = json["wlsLambda"].toDouble(8000.0);
    config.wlsSigma = json["wlsSigma"].toDouble(1.5);
    return config;
}

// ============================================================
// 工具函数
// ============================================================

QString getAlgorithm3DErrorDescription(Algorithm3DError error) {
    switch (error) {
        case Algorithm3DError::Success:
            return "成功";
        case Algorithm3DError::InvalidInput:
            return "无效输入";
        case Algorithm3DError::InsufficientPoints:
            return "点数不足";
        case Algorithm3DError::ComputationFailed:
            return "计算失败";
        case Algorithm3DError::ConvergenceFailed:
            return "收敛失败";
        case Algorithm3DError::MemoryError:
            return "内存错误";
        case Algorithm3DError::FileIOError:
            return "文件读写错误";
        case Algorithm3DError::CalibrationError:
            return "标定错误";
        case Algorithm3DError::NotSupported:
            return "不支持的操作";
        default:
            return "未知错误";
    }
}

QString getMeasurement3DTypeName(Measurement3DType type) {
    switch (type) {
        case Measurement3DType::Distance:
            return "距离测量";
        case Measurement3DType::Angle:
            return "角度测量";
        case Measurement3DType::Area:
            return "面积测量";
        case Measurement3DType::Volume:
            return "体积测量";
        case Measurement3DType::Flatness:
            return "平面度测量";
        case Measurement3DType::Cylindricity:
            return "圆柱度测量";
        case Measurement3DType::Sphericity:
            return "球度测量";
        case Measurement3DType::Height:
            return "高度测量";
        case Measurement3DType::Thickness:
            return "厚度测量";
        case Measurement3DType::Gap:
            return "间隙测量";
        case Measurement3DType::Flush:
            return "平齐度测量";
        default:
            return "未知测量";
    }
}

QString getPointCloudFilterTypeName(PointCloudFilterType type) {
    switch (type) {
        case PointCloudFilterType::None:
            return "无滤波";
        case PointCloudFilterType::StatisticalOutlier:
            return "统计离群点滤波";
        case PointCloudFilterType::RadiusOutlier:
            return "半径离群点滤波";
        case PointCloudFilterType::VoxelGrid:
            return "体素网格下采样";
        case PointCloudFilterType::PassThrough:
            return "直通滤波";
        case PointCloudFilterType::Bilateral:
            return "双边滤波";
        default:
            return "未知滤波";
    }
}

} // namespace Algorithm3D
} // namespace VisionForge
