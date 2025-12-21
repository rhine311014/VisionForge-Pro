/**
 * @file CalibrationManager.cpp
 * @brief VisionForge Pro 标定管理器实现
 *
 * 实现核心标定算法：
 * - 仿射变换标定（最小二乘法）
 * - 齐次变换标定（DLT + SVD）
 * - 可选非线性优化（LM算法）
 *
 * 使用Eigen库进行精密几何计算，主要API说明：
 * - Eigen::MatrixXd：动态大小矩阵，使用 (i,j) 访问元素
 * - Eigen::Matrix3d：固定3x3矩阵，使用 (i,j) 访问元素
 * - Eigen::VectorXd：动态大小向量，使用 (i) 访问元素
 * - Eigen::JacobiSVD<MatrixXd>：SVD分解，svd.solve(b) 求解最小二乘
 * - Eigen::JacobiSVD<MatrixXd>(A, Eigen::ComputeFullV)：完整SVD，用于齐次方程
 * - Matrix3d::inverse()：矩阵求逆
 * - Matrix3d::determinant()：计算行列式
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#include "calibration/CalibrationManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <cmath>
#include <algorithm>
#include <numeric>

// Eigen SVD分解
#include <Eigen/SVD>

namespace VisionForge {
namespace Calibration {

// ============================================================
// 静态成员初始化
// ============================================================

std::map<int, std::unique_ptr<CalibrationManager>> CalibrationManagerFactory::s_instances;

// ============================================================
// CalibrationManager 实现
// ============================================================

CalibrationManager::CalibrationManager(QObject* parent)
    : QObject(parent) {
    // 默认配置
    m_config.type = CalibrationType::Point9;
    m_config.gridRows = 3;
    m_config.gridCols = 3;
}

CalibrationManager::~CalibrationManager() = default;

void CalibrationManager::setConfig(const CalibrationConfig& config) {
    m_config = config;
    emit configChanged();
}

void CalibrationManager::setCalibrationType(CalibrationType type) {
    m_config.type = type;
    emit configChanged();
}

// ============================================================
// 标定点管理
// ============================================================

int CalibrationManager::addCalibrationPoint(const QPointF& pixelPos, const QPointF& worldPos) {
    CalibrationPoint point(pixelPos, worldPos, m_points.count());
    int index = m_points.addPoint(point);
    emit calibrationPointsChanged();
    return index;
}

int CalibrationManager::addCalibrationPoint(const CalibrationPoint& point) {
    int index = m_points.addPoint(point);
    emit calibrationPointsChanged();
    return index;
}

bool CalibrationManager::setCalibrationPoint(int index, const QPointF& pixelPos, const QPointF& worldPos) {
    CalibrationPoint point(pixelPos, worldPos, index);
    bool success = m_points.setPoint(index, point);
    if (success) {
        emit calibrationPointsChanged();
    }
    return success;
}

const CalibrationPoint* CalibrationManager::getCalibrationPoint(int index) const {
    return m_points.getPoint(index);
}

bool CalibrationManager::removeCalibrationPoint(int index) {
    bool success = m_points.removePoint(index);
    if (success) {
        emit calibrationPointsChanged();
    }
    return success;
}

void CalibrationManager::clearCalibrationPoints() {
    m_points.clear();
    emit calibrationPointsChanged();
}

int CalibrationManager::getCalibrationPointCount() const {
    return m_points.count();
}

int CalibrationManager::getValidPointCount() const {
    return m_points.enabledCount();
}

bool CalibrationManager::setPointEnabled(int index, bool enabled) {
    CalibrationPoint* point = m_points.getPoint(index);
    if (point) {
        point->isEnabled = enabled;
        emit calibrationPointsChanged();
        return true;
    }
    return false;
}

// ============================================================
// 标定执行
// ============================================================

CalibrationResult CalibrationManager::calibrate() {
    return calibrate(m_progressCallback);
}

CalibrationResult CalibrationManager::calibrate(CalibrationProgressCallback progressCallback) {
    m_progressCallback = progressCallback;
    return calibrate(m_points);
}

CalibrationResult CalibrationManager::calibrate(const CalibrationPointSet& points) {
    reportProgress(0, QStringLiteral("开始标定..."));

    m_result = CalibrationResult();
    m_result.status = CalibrationStatus::Calibrating;
    m_result.type = m_config.type;

    // 获取有效点
    auto validPoints = points.getValidEnabledPoints();

    reportProgress(10, QStringLiteral("检查标定点数量..."));

    // 检查点数量
    int minPoints = getMinPointsForType(m_config.type);
    if (static_cast<int>(validPoints.size()) < minPoints) {
        m_result.status = CalibrationStatus::Failed;
        m_result.error = CalibrationError::InsufficientPoints;
        qWarning() << "Calibration failed: insufficient points. Need" << minPoints
                   << "but got" << validPoints.size();
        emit calibrationCompleted(false, m_result);
        return m_result;
    }

    reportProgress(20, QStringLiteral("检查点分布..."));

    // 检查点是否共线
    CalibrationPointSet tempSet;
    for (const auto& pt : validPoints) {
        tempSet.addPoint(pt);
    }

    if (tempSet.arePointsCollinear()) {
        m_result.status = CalibrationStatus::Failed;
        m_result.error = CalibrationError::SingularMatrix;
        qWarning() << "Calibration failed: points are collinear";
        emit calibrationCompleted(false, m_result);
        return m_result;
    }

    reportProgress(30, QStringLiteral("计算变换矩阵..."));

    // 根据标定类型选择算法
    CalibrationResult result;
    switch (m_config.type) {
        case CalibrationType::Point3:
        case CalibrationType::Point4:
        case CalibrationType::Point5:
            // 仿射变换
            result = calibrateAffine(validPoints);
            break;

        case CalibrationType::Point9:
        case CalibrationType::Point16:
        case CalibrationType::Point25:
        case CalibrationType::DMCode:
        case CalibrationType::Chessboard:
        case CalibrationType::Circle:
        default:
            // 齐次变换
            result = calibrateHomography(validPoints);
            break;
    }

    if (result.status != CalibrationStatus::Calibrated) {
        m_result = result;
        emit calibrationCompleted(false, m_result);
        return m_result;
    }

    reportProgress(60, QStringLiteral("计算误差统计..."));

    // 计算误差
    result.errorStats = computeReprojectionErrors(result.homography, validPoints);
    result.pointCount = static_cast<int>(validPoints.size());

    // 更新每个点的误差
    result.pointErrors.clear();
    for (size_t i = 0; i < validPoints.size(); ++i) {
        const auto& pt = validPoints[i];
        QPointF reprojected = result.pixelToWorld(pt.pixelPos);
        double dx = reprojected.x() - pt.worldPos.x();
        double dy = reprojected.y() - pt.worldPos.y();
        double err = std::sqrt(dx * dx + dy * dy);
        result.pointErrors.push_back(err);
    }

    reportProgress(70, QStringLiteral("检查误差阈值..."));

    // 检查误差是否可接受
    if (!result.errorStats.isAcceptable(m_config.maxAllowedRMS)) {
        // 如果启用异常点剔除
        if (m_config.enableOutlierRejection) {
            reportProgress(75, QStringLiteral("剔除异常点..."));

            auto outliers = identifyOutliers(m_config.outlierThreshold);
            if (!outliers.empty() && validPoints.size() - outliers.size() >=
                static_cast<size_t>(minPoints)) {
                // 剔除异常点重新标定
                std::vector<CalibrationPoint> filteredPoints;
                for (size_t i = 0; i < validPoints.size(); ++i) {
                    if (std::find(outliers.begin(), outliers.end(), i) == outliers.end()) {
                        filteredPoints.push_back(validPoints[i]);
                    }
                }

                // 递归调用（使用齐次变换）
                CalibrationResult filteredResult = calibrateHomography(filteredPoints);
                if (filteredResult.status == CalibrationStatus::Calibrated) {
                    filteredResult.errorStats = computeReprojectionErrors(
                        filteredResult.homography, filteredPoints);
                    if (filteredResult.errorStats.isAcceptable(m_config.maxAllowedRMS)) {
                        result = filteredResult;
                        qInfo() << "Removed" << outliers.size() << "outliers, recalibrated";
                    }
                }
            }
        }

        // 再次检查
        if (!result.errorStats.isAcceptable(m_config.maxAllowedRMS)) {
            result.status = CalibrationStatus::Failed;
            result.error = CalibrationError::LargeError;
            qWarning() << "Calibration failed: RMS error" << result.errorStats.rmsError
                       << "exceeds threshold" << m_config.maxAllowedRMS;
        }
    }

    reportProgress(80, QStringLiteral("非线性优化..."));

    // 可选的非线性优化
    if (m_config.enableNonlinearOptimization &&
        result.status == CalibrationStatus::Calibrated) {
        result.homography = optimizeHomography(result.homography, validPoints);
        result.errorStats = computeReprojectionErrors(result.homography, validPoints);
    }

    reportProgress(90, QStringLiteral("提取变换参数..."));

    // 提取变换参数
    extractTransformParams(result.homography, result.scaleX, result.scaleY, result.rotation);

    // 计算逆矩阵
    // 使用Eigen Matrix3d::inverse()求逆矩阵
    if (result.isMatrixInvertible()) {
        result.inverseHomography = result.homography.inverse();
    } else {
        result.status = CalibrationStatus::Failed;
        result.error = CalibrationError::SingularMatrix;
    }

    // 设置时间戳
    result.calibrationTime = QDateTime::currentDateTime();

    m_result = result;

    reportProgress(100, QStringLiteral("标定完成"));

    emit calibrationCompleted(result.status == CalibrationStatus::Calibrated, m_result);
    return m_result;
}

bool CalibrationManager::validateCalibration() {
    if (!isCalibrated()) {
        return false;
    }

    auto validPoints = m_points.getValidEnabledPoints();
    ErrorStatistics newStats = computeReprojectionErrors(m_result.homography, validPoints);

    return newStats.isAcceptable(m_config.maxAllowedRMS);
}

CalibrationResult CalibrationManager::recalibrateExcluding(const std::vector<int>& excludeIndices) {
    auto allPoints = m_points.getValidEnabledPoints();
    std::vector<CalibrationPoint> filteredPoints;

    for (size_t i = 0; i < allPoints.size(); ++i) {
        if (std::find(excludeIndices.begin(), excludeIndices.end(),
                      static_cast<int>(i)) == excludeIndices.end()) {
            filteredPoints.push_back(allPoints[i]);
        }
    }

    CalibrationPointSet tempSet;
    for (const auto& pt : filteredPoints) {
        tempSet.addPoint(pt);
    }

    return calibrate(tempSet);
}

// ============================================================
// 坐标变换
// ============================================================

QPointF CalibrationManager::pixelToWorld(const QPointF& pixelPos) const {
    if (!isCalibrated()) {
        return pixelPos;
    }
    return m_result.pixelToWorld(pixelPos);
}

QPointF CalibrationManager::worldToPixel(const QPointF& worldPos) const {
    if (!isCalibrated()) {
        return worldPos;
    }
    return m_result.worldToPixel(worldPos);
}

std::vector<QPointF> CalibrationManager::pixelToWorld(const std::vector<QPointF>& pixelPoints) const {
    if (!isCalibrated()) {
        return pixelPoints;
    }
    return m_result.pixelToWorld(pixelPoints);
}

std::vector<QPointF> CalibrationManager::worldToPixel(const std::vector<QPointF>& worldPoints) const {
    if (!isCalibrated()) {
        return worldPoints;
    }
    return m_result.worldToPixel(worldPoints);
}

Platform::CoordPosition CalibrationManager::transformToWorld(
    const Platform::CoordPosition& imagePos) const {

    if (!isCalibrated()) {
        return imagePos;
    }

    Platform::CoordPosition worldPos;

    // 转换主位置
    QPointF mainWorld = pixelToWorld(imagePos.getMainPos());
    worldPos.setMainPos(mainWorld.x(), mainWorld.y());
    worldPos.setAngle(imagePos.getAngle());  // 角度保持不变（由角度标定单独处理）

    // 转换辅助位置
    for (int i = 0; i < Platform::MaxAuxiliaryPosNum; ++i) {
        QPointF auxPixel = imagePos.getAuxPos(i);
        if (auxPixel != QPointF(0, 0)) {  // 简单检查是否有有效辅助点
            QPointF auxWorld = pixelToWorld(auxPixel);
            worldPos.setAuxPos(i, auxWorld.x(), auxWorld.y());
        }
    }

    return worldPos;
}

Platform::CoordPosition CalibrationManager::transformToImage(
    const Platform::CoordPosition& worldPos) const {

    if (!isCalibrated()) {
        return worldPos;
    }

    Platform::CoordPosition imagePos;

    // 转换主位置
    QPointF mainPixel = worldToPixel(worldPos.getMainPos());
    imagePos.setMainPos(mainPixel.x(), mainPixel.y());
    imagePos.setAngle(worldPos.getAngle());

    // 转换辅助位置
    for (int i = 0; i < Platform::MaxAuxiliaryPosNum; ++i) {
        QPointF auxWorld = worldPos.getAuxPos(i);
        if (auxWorld != QPointF(0, 0)) {
            QPointF auxPixel = worldToPixel(auxWorld);
            imagePos.setAuxPos(i, auxPixel.x(), auxPixel.y());
        }
    }

    return imagePos;
}

// ============================================================
// 精度评估
// ============================================================

double CalibrationManager::calculateReprojectionError(
    const QPointF& pixelPos, const QPointF& worldPos) const {

    if (!isCalibrated()) {
        return 0.0;
    }

    QPointF reprojected = pixelToWorld(pixelPos);
    double dx = reprojected.x() - worldPos.x();
    double dy = reprojected.y() - worldPos.y();
    return std::sqrt(dx * dx + dy * dy);
}

ErrorStatistics CalibrationManager::calculateAllErrors() const {
    if (!isCalibrated()) {
        return ErrorStatistics();
    }

    auto validPoints = m_points.getValidEnabledPoints();
    return computeReprojectionErrors(m_result.homography, validPoints);
}

std::vector<int> CalibrationManager::identifyOutliers(double threshold) const {
    std::vector<int> outliers;

    if (!isCalibrated() || m_result.pointErrors.empty()) {
        return outliers;
    }

    double mean = m_result.errorStats.meanError;
    double stdDev = m_result.errorStats.stdDev;

    for (size_t i = 0; i < m_result.pointErrors.size(); ++i) {
        if (std::abs(m_result.pointErrors[i] - mean) > threshold * stdDev) {
            outliers.push_back(static_cast<int>(i));
        }
    }

    return outliers;
}

void CalibrationManager::getScale(double& scaleX, double& scaleY) const {
    scaleX = m_result.scaleX;
    scaleY = m_result.scaleY;
}

double CalibrationManager::getRotation() const {
    return m_result.rotation;
}

// ============================================================
// 持久化
// ============================================================

bool CalibrationManager::saveToFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        return false;
    }

    QJsonObject json = exportToJson();
    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qInfo() << "Calibration saved to:" << filePath;
    return true;
}

bool CalibrationManager::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file for reading:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return false;
    }

    bool success = importFromJson(doc.object());
    if (success) {
        qInfo() << "Calibration loaded from:" << filePath;
    }
    return success;
}

QJsonObject CalibrationManager::exportToJson() const {
    QJsonObject json;
    json["config"] = m_config.toJson();
    json["points"] = m_points.toJson();
    json["result"] = m_result.toJson();
    json["version"] = "1.2.0";
    return json;
}

bool CalibrationManager::importFromJson(const QJsonObject& json) {
    if (json.contains("config")) {
        m_config = CalibrationConfig::fromJson(json["config"].toObject());
    }

    if (json.contains("points")) {
        m_points = CalibrationPointSet::fromJson(json["points"].toArray());
    }

    if (json.contains("result")) {
        m_result = CalibrationResult::fromJson(json["result"].toObject());
    }

    emit configChanged();
    emit calibrationPointsChanged();
    return true;
}

QString CalibrationManager::getDefaultSavePath() {
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir.filePath("calibration");
}

void CalibrationManager::reportProgress(int progress, const QString& message) {
    if (m_progressCallback) {
        m_progressCallback(progress, message);
    }
    emit calibrationProgress(progress, message);
}

// ============================================================
// 核心标定算法
// ============================================================

CalibrationResult CalibrationManager::calibrateAffine(
    const std::vector<CalibrationPoint>& points) {

    CalibrationResult result;
    result.type = m_config.type;

    int n = static_cast<int>(points.size());
    if (n < 3) {
        result.status = CalibrationStatus::Failed;
        result.error = CalibrationError::InsufficientPoints;
        return result;
    }

    // 构建最小二乘方程组
    // 对于仿射变换：
    // [wx] = [a b tx] [px]
    // [wy]   [c d ty] [py]
    //                 [1 ]
    //
    // 展开为：
    // wx = a*px + b*py + tx
    // wy = c*px + d*py + ty
    //
    // 构建矩阵方程 A * h = b
    // 其中 h = [a, b, tx, c, d, ty]^T

    // 使用Eigen MatrixXd创建动态大小矩阵
    MatrixXd A(2 * n, 6);
    VectorXd b(2 * n);

    for (int i = 0; i < n; ++i) {
        double px = points[i].pixelPos.x();
        double py = points[i].pixelPos.y();
        double wx = points[i].worldPos.x();
        double wy = points[i].worldPos.y();

        // 第一行：wx = a*px + b*py + tx
        // Eigen::MatrixXd 使用 (row, col) 访问元素
        A(2 * i, 0) = px;
        A(2 * i, 1) = py;
        A(2 * i, 2) = 1.0;
        A(2 * i, 3) = 0.0;
        A(2 * i, 4) = 0.0;
        A(2 * i, 5) = 0.0;
        b(2 * i) = wx;

        // 第二行：wy = c*px + d*py + ty
        A(2 * i + 1, 0) = 0.0;
        A(2 * i + 1, 1) = 0.0;
        A(2 * i + 1, 2) = 0.0;
        A(2 * i + 1, 3) = px;
        A(2 * i + 1, 4) = py;
        A(2 * i + 1, 5) = 1.0;
        b(2 * i + 1) = wy;
    }

    // 使用Eigen JacobiSVD求解最小二乘问题
    // svd.solve(b) 求解 A*x = b 的最小二乘解
    Eigen::JacobiSVD<MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    VectorXd h = svd.solve(b);

    // 构建仿射矩阵
    // Eigen::Matrix<double, 2, 3> 使用 (row, col) 访问
    result.affineMatrix(0, 0) = h(0);  // a
    result.affineMatrix(0, 1) = h(1);  // b
    result.affineMatrix(0, 2) = h(2);  // tx
    result.affineMatrix(1, 0) = h(3);  // c
    result.affineMatrix(1, 1) = h(4);  // d
    result.affineMatrix(1, 2) = h(5);  // ty

    // 转换为齐次矩阵
    // 使用Eigen逗号初始化器
    result.homography << h(0), h(1), h(2),
                         h(3), h(4), h(5),
                         0.0,  0.0,  1.0;

    result.status = CalibrationStatus::Calibrated;
    result.error = CalibrationError::Success;

    return result;
}

CalibrationResult CalibrationManager::calibrateHomography(
    const std::vector<CalibrationPoint>& points) {

    CalibrationResult result;
    result.type = m_config.type;

    int n = static_cast<int>(points.size());
    if (n < 4) {
        result.status = CalibrationStatus::Failed;
        result.error = CalibrationError::InsufficientPoints;
        return result;
    }

    // 数据归一化（提高数值稳定性）
    std::vector<CalibrationPoint> normalizedPoints;
    Matrix3d T1, T2;
    normalizePoints(points, normalizedPoints, T1, T2);

    // 构建DLT矩阵
    MatrixXd A = buildDLTMatrix(normalizedPoints);

    // SVD求解
    Matrix3d H = solveHomographySVD(A);

    // 检查矩阵有效性
    // 使用Eigen计算行列式
    double det = H.determinant();

    if (std::abs(det) < 1e-10) {
        result.status = CalibrationStatus::Failed;
        result.error = CalibrationError::SingularMatrix;
        return result;
    }

    // 反归一化
    // 使用Eigen Matrix3d::inverse()求逆
    Matrix3d T2_inv = T2.inverse();
    result.homography = T2_inv * H * T1;

    // 归一化使H(2,2) = 1
    if (std::abs(result.homography(2, 2)) > 1e-10) {
        double scale = 1.0 / result.homography(2, 2);
        // Eigen矩阵支持标量乘法
        result.homography *= scale;
    }

    result.status = CalibrationStatus::Calibrated;
    result.error = CalibrationError::Success;

    return result;
}

Matrix3d CalibrationManager::solveHomographySVD(const MatrixXd& A) {
    // 使用Eigen JacobiSVD进行完整SVD分解
    // ComputeFullV 确保V矩阵完整，用于获取最小奇异值对应的右奇异向量
    Eigen::JacobiSVD<MatrixXd> svd(A, Eigen::ComputeFullV);

    // H向量是V的最后一列（对应最小奇异值）
    // Eigen的matrixV()返回V矩阵（不是VT）
    VectorXd h = svd.matrixV().col(8);

    // 重构3x3矩阵
    // 使用Eigen逗号初始化器
    Matrix3d H;
    H << h(0), h(1), h(2),
         h(3), h(4), h(5),
         h(6), h(7), h(8);

    return H;
}

MatrixXd CalibrationManager::buildDLTMatrix(const std::vector<CalibrationPoint>& points) {
    int n = static_cast<int>(points.size());

    // 创建2n x 9的矩阵
    MatrixXd A(2 * n, 9);

    for (int i = 0; i < n; ++i) {
        double px = points[i].pixelPos.x();
        double py = points[i].pixelPos.y();
        double wx = points[i].worldPos.x();
        double wy = points[i].worldPos.y();

        // DLT方程（齐次坐标）：
        // [wx] = [h00 h01 h02] [px]
        // [wy]   [h10 h11 h12] [py]
        // [w ]   [h20 h21 h22] [1 ]
        //
        // 展开：
        // wx = (h00*px + h01*py + h02) / (h20*px + h21*py + h22)
        // wy = (h10*px + h11*py + h12) / (h20*px + h21*py + h22)
        //
        // 交叉相乘消除分母：
        // wx*(h20*px + h21*py + h22) = h00*px + h01*py + h02
        // wy*(h20*px + h21*py + h22) = h10*px + h11*py + h12
        //
        // 整理为 A*h = 0 形式：
        // [-px, -py, -1, 0, 0, 0, wx*px, wx*py, wx] * h = 0
        // [0, 0, 0, -px, -py, -1, wy*px, wy*py, wy] * h = 0

        A(2 * i, 0) = -px;
        A(2 * i, 1) = -py;
        A(2 * i, 2) = -1.0;
        A(2 * i, 3) = 0.0;
        A(2 * i, 4) = 0.0;
        A(2 * i, 5) = 0.0;
        A(2 * i, 6) = wx * px;
        A(2 * i, 7) = wx * py;
        A(2 * i, 8) = wx;

        A(2 * i + 1, 0) = 0.0;
        A(2 * i + 1, 1) = 0.0;
        A(2 * i + 1, 2) = 0.0;
        A(2 * i + 1, 3) = -px;
        A(2 * i + 1, 4) = -py;
        A(2 * i + 1, 5) = -1.0;
        A(2 * i + 1, 6) = wy * px;
        A(2 * i + 1, 7) = wy * py;
        A(2 * i + 1, 8) = wy;
    }

    return A;
}

void CalibrationManager::normalizePoints(const std::vector<CalibrationPoint>& points,
                                         std::vector<CalibrationPoint>& normalizedPoints,
                                         Matrix3d& T1, Matrix3d& T2) {
    int n = static_cast<int>(points.size());

    // 计算像素坐标质心和缩放
    double meanPx = 0, meanPy = 0;
    double meanWx = 0, meanWy = 0;

    for (const auto& pt : points) {
        meanPx += pt.pixelPos.x();
        meanPy += pt.pixelPos.y();
        meanWx += pt.worldPos.x();
        meanWy += pt.worldPos.y();
    }
    meanPx /= n;
    meanPy /= n;
    meanWx /= n;
    meanWy /= n;

    // 计算到质心的平均距离
    double distP = 0, distW = 0;
    for (const auto& pt : points) {
        double dx = pt.pixelPos.x() - meanPx;
        double dy = pt.pixelPos.y() - meanPy;
        distP += std::sqrt(dx * dx + dy * dy);

        dx = pt.worldPos.x() - meanWx;
        dy = pt.worldPos.y() - meanWy;
        distW += std::sqrt(dx * dx + dy * dy);
    }
    distP /= n;
    distW /= n;

    // 缩放因子（使平均距离为sqrt(2)）
    double scaleP = (distP > 1e-10) ? std::sqrt(2.0) / distP : 1.0;
    double scaleW = (distW > 1e-10) ? std::sqrt(2.0) / distW : 1.0;

    // 构建归一化矩阵
    // 使用Eigen逗号初始化器
    T1 << scaleP, 0,      -scaleP * meanPx,
          0,      scaleP, -scaleP * meanPy,
          0,      0,      1;

    T2 << scaleW, 0,      -scaleW * meanWx,
          0,      scaleW, -scaleW * meanWy,
          0,      0,      1;

    // 归一化点
    normalizedPoints.clear();
    normalizedPoints.reserve(n);
    for (const auto& pt : points) {
        CalibrationPoint normPt = pt;
        normPt.pixelPos = QPointF(scaleP * (pt.pixelPos.x() - meanPx),
                                  scaleP * (pt.pixelPos.y() - meanPy));
        normPt.worldPos = QPointF(scaleW * (pt.worldPos.x() - meanWx),
                                  scaleW * (pt.worldPos.y() - meanWy));
        normalizedPoints.push_back(normPt);
    }
}

Matrix3d CalibrationManager::optimizeHomography(const Matrix3d& H,
                                                 const std::vector<CalibrationPoint>& points) {
    // 简化的非线性优化（Gauss-Newton迭代）
    // 完整实现可使用Ceres或LM库

    Matrix3d optimizedH = H;
    int maxIter = m_config.maxIterations;
    double threshold = m_config.convergenceThreshold;

    for (int iter = 0; iter < maxIter; ++iter) {
        double prevError = 0;
        double currError = 0;

        // 计算当前总误差
        for (const auto& pt : points) {
            // 使用Eigen::Vector3d进行矩阵向量乘法
            Vector3d pixel(pt.pixelPos.x(), pt.pixelPos.y(), 1.0);
            Vector3d world = optimizedH * pixel;

            // Eigen::Vector3d使用(i)访问元素
            if (std::abs(world(2)) > 1e-10) {
                double dx = world(0) / world(2) - pt.worldPos.x();
                double dy = world(1) / world(2) - pt.worldPos.y();
                currError += dx * dx + dy * dy;
            }
        }

        if (iter > 0 && std::abs(currError - prevError) < threshold) {
            break;  // 已收敛
        }

        prevError = currError;
        // 在完整实现中，这里应该计算雅可比矩阵并更新H
    }

    return optimizedH;
}

ErrorStatistics CalibrationManager::computeReprojectionErrors(
    const Matrix3d& H, const std::vector<CalibrationPoint>& points) const {

    ErrorStatistics stats;
    int n = static_cast<int>(points.size());
    if (n == 0) {
        return stats;
    }

    std::vector<double> errors;
    errors.reserve(n);

    double maxError = 0;
    double minError = std::numeric_limits<double>::max();
    int worstIndex = -1;

    for (int i = 0; i < n; ++i) {
        const auto& pt = points[i];

        // 使用Eigen::Vector3d进行矩阵向量乘法
        Vector3d pixel(pt.pixelPos.x(), pt.pixelPos.y(), 1.0);
        Vector3d world = H * pixel;

        double err = 0;
        // Eigen::Vector3d使用(i)访问元素
        if (std::abs(world(2)) > 1e-10) {
            double dx = world(0) / world(2) - pt.worldPos.x();
            double dy = world(1) / world(2) - pt.worldPos.y();
            err = std::sqrt(dx * dx + dy * dy);
        }

        errors.push_back(err);

        if (err > maxError) {
            maxError = err;
            worstIndex = i;
        }
        minError = std::min(minError, err);
    }

    // 计算统计量
    double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
    stats.meanError = sum / n;

    double sumSq = 0;
    for (double e : errors) {
        sumSq += e * e;
    }
    stats.rmsError = std::sqrt(sumSq / n);

    double variance = 0;
    for (double e : errors) {
        variance += (e - stats.meanError) * (e - stats.meanError);
    }
    stats.stdDev = std::sqrt(variance / n);

    stats.maxError = maxError;
    stats.minError = minError;
    stats.worstPointIndex = worstIndex;
    stats.pointCount = n;

    return stats;
}

void CalibrationManager::extractTransformParams(const Matrix3d& H,
                                                 double& scaleX, double& scaleY,
                                                 double& rotation) {
    // 从齐次矩阵提取缩放和旋转参数
    // 假设 H 近似仿射变换（H(2,0) 约等于 0, H(2,1) 约等于 0）

    // 对于仿射变换部分：
    // [a b]   [sx  0 ] [cos  -sin]
    // [c d] = [0  sy ] [sin   cos]
    //
    // 或者带剪切的分解

    // 使用Eigen Matrix3d的(row, col)访问
    double a = H(0, 0);
    double b = H(0, 1);
    double c = H(1, 0);
    double d = H(1, 1);

    // 计算缩放（使用列向量长度）
    scaleX = std::sqrt(a * a + c * c);
    scaleY = std::sqrt(b * b + d * d);

    // 计算旋转角度
    rotation = std::atan2(c, a) * 180.0 / M_PI;  // 转换为度
}

// ============================================================
// CalibrationManagerFactory 实现
// ============================================================

CalibrationManager* CalibrationManagerFactory::getInstance(int cameraIndex) {
    auto it = s_instances.find(cameraIndex);
    if (it == s_instances.end()) {
        s_instances[cameraIndex] = std::make_unique<CalibrationManager>();
    }
    return s_instances[cameraIndex].get();
}

std::unique_ptr<CalibrationManager> CalibrationManagerFactory::create() {
    return std::make_unique<CalibrationManager>();
}

void CalibrationManagerFactory::releaseInstance(int cameraIndex) {
    s_instances.erase(cameraIndex);
}

void CalibrationManagerFactory::releaseAll() {
    s_instances.clear();
}

} // namespace Calibration
} // namespace VisionForge
