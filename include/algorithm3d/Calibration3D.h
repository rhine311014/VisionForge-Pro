/**
 * @file Calibration3D.h
 * @brief VisionForge Pro 3D标定模块
 *
 * 提供3D视觉系统标定功能，包括：
 * - 相机内参标定
 * - 立体相机标定
 * - 手眼标定
 * - 多相机标定
 * - 结构光标定
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_CALIBRATION3D_H
#define VISIONFORGE_CALIBRATION3D_H

#include "Algorithm3DTypes.h"
#include <QObject>
#include <QString>
#include <QImage>
#include <QMutex>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Algorithm3D {

// ============================================================
// 标定类型
// ============================================================

/**
 * @brief 3D标定类型
 */
enum class Calibration3DType {
    CameraIntrinsic,        ///< 相机内参标定
    StereoPair,             ///< 双目立体标定
    MultiCamera,            ///< 多相机标定
    HandEye,                ///< 手眼标定
    StructuredLight,        ///< 结构光标定
    RobotCamera,            ///< 机器人相机标定
};

/**
 * @brief 标定板类型
 */
enum class CalibrationBoardType {
    Chessboard,             ///< 棋盘格
    CircleGrid,             ///< 圆点阵列
    AsymmetricCircleGrid,   ///< 非对称圆点阵列
    CharucoBoard,           ///< ChArUco板
    AprilTag,               ///< AprilTag
};

/**
 * @brief 手眼标定类型
 */
enum class HandEyeMethod {
    Tsai,                   ///< Tsai方法
    Park,                   ///< Park方法
    Horaud,                 ///< Horaud方法
    Daniilidis,             ///< Daniilidis方法
    Andreff,                ///< Andreff方法
};

// ============================================================
// 标定数据结构
// ============================================================

/**
 * @brief 标定板配置
 */
struct CalibrationBoardConfig {
    CalibrationBoardType type = CalibrationBoardType::Chessboard;
    int rows = 9;                   ///< 行数（内角点）
    int cols = 6;                   ///< 列数（内角点）
    double squareSize = 25.0;       ///< 方格大小（mm）
    double markerSize = 18.0;       ///< 标记大小（用于ChArUco）

    /**
     * @brief 获取角点总数
     */
    int getCornerCount() const { return rows * cols; }

    /**
     * @brief 生成3D角点坐标
     */
    std::vector<Point3D> generate3DPoints() const;

    QJsonObject toJson() const;
    static CalibrationBoardConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 单帧标定数据
 */
struct CalibrationFrame {
    QImage image;                           ///< 图像
    std::vector<QPointF> imagePoints;       ///< 检测到的2D点
    std::vector<Point3D> objectPoints;      ///< 对应的3D点
    bool detected = false;                  ///< 是否检测成功
    double reprojectionError = 0.0;         ///< 重投影误差
    Matrix4d pose = Matrix4d::Identity();   ///< 标定板位姿

    bool isValid() const {
        return detected && imagePoints.size() == objectPoints.size() &&
               !imagePoints.empty();
    }
};

/**
 * @brief 立体标定帧对
 */
struct StereoCalibrationFrame {
    CalibrationFrame left;      ///< 左相机帧
    CalibrationFrame right;     ///< 右相机帧

    bool isValid() const {
        return left.isValid() && right.isValid() &&
               left.imagePoints.size() == right.imagePoints.size();
    }
};

/**
 * @brief 手眼标定数据
 */
struct HandEyeCalibrationData {
    Matrix4d robotPose;         ///< 机器人末端位姿
    Matrix4d cameraPose;        ///< 相机位姿（标定板到相机）
    bool valid = false;

    QJsonObject toJson() const;
    static HandEyeCalibrationData fromJson(const QJsonObject& json);
};

// ============================================================
// 标定结果
// ============================================================

/**
 * @brief 相机标定结果
 */
struct CameraCalibrationResult {
    bool success = false;
    QString errorMessage;

    CameraIntrinsics intrinsics;        ///< 相机内参
    double reprojectionError = 0.0;     ///< 平均重投影误差

    // 每帧的位姿
    std::vector<Matrix4d> framePoses;

    // 每帧的误差
    std::vector<double> frameErrors;

    QJsonObject toJson() const;
    static CameraCalibrationResult fromJson(const QJsonObject& json);
};

/**
 * @brief 立体标定结果
 */
struct StereoCalibrationResult {
    bool success = false;
    QString errorMessage;

    // 相机参数
    CameraIntrinsics leftIntrinsics;
    CameraIntrinsics rightIntrinsics;
    CameraExtrinsics rightToLeft;       ///< 右相机到左相机的变换

    // 完整立体参数
    StereoParameters stereoParams;

    // 校正参数
    Matrix3d R1, R2;                    ///< 校正旋转矩阵
    Matrix34d P1, P2;                   ///< 投影矩阵
    Matrix4d Q;                         ///< 视差到深度映射矩阵

    double reprojectionError = 0.0;

    QJsonObject toJson() const;
    static StereoCalibrationResult fromJson(const QJsonObject& json);
};

/**
 * @brief 手眼标定结果
 */
struct HandEyeCalibrationResult {
    bool success = false;
    QString errorMessage;

    Matrix4d handEyeTransform;          ///< 手眼变换矩阵
    double rotationError = 0.0;         ///< 旋转误差（度）
    double translationError = 0.0;      ///< 平移误差（mm）

    QJsonObject toJson() const;
    static HandEyeCalibrationResult fromJson(const QJsonObject& json);
};

// ============================================================
// 3D标定管理器
// ============================================================

/**
 * @brief 3D标定管理器
 *
 * 统一管理各种3D标定任务。
 */
class Calibration3DManager : public QObject {
    Q_OBJECT

public:
    explicit Calibration3DManager(QObject* parent = nullptr);
    ~Calibration3DManager() override;

    // ============ 标定板配置 ============

    /**
     * @brief 设置标定板配置
     */
    void setBoardConfig(const CalibrationBoardConfig& config);
    const CalibrationBoardConfig& getBoardConfig() const { return m_boardConfig; }

    // ============ 角点检测 ============

    /**
     * @brief 检测标定板角点
     */
    bool detectCorners(const QImage& image, std::vector<QPointF>& corners);

    /**
     * @brief 绘制检测到的角点
     */
    QImage drawCorners(const QImage& image, const std::vector<QPointF>& corners,
                       bool found = true);

    /**
     * @brief 亚像素精化
     */
    void refineCorners(const QImage& image, std::vector<QPointF>& corners,
                       int winSize = 11);

    // ============ 相机标定 ============

    /**
     * @brief 添加标定帧
     */
    bool addCalibrationFrame(const QImage& image);

    /**
     * @brief 获取已采集的帧数
     */
    int getFrameCount() const { return static_cast<int>(m_frames.size()); }

    /**
     * @brief 清除所有帧
     */
    void clearFrames();

    /**
     * @brief 移除指定帧
     */
    void removeFrame(int index);

    /**
     * @brief 执行相机标定
     */
    CameraCalibrationResult calibrateCamera(int flags = 0);

    // ============ 立体标定 ============

    /**
     * @brief 添加立体标定帧对
     */
    bool addStereoFrame(const QImage& leftImage, const QImage& rightImage);

    /**
     * @brief 获取立体帧数
     */
    int getStereoFrameCount() const {
        return static_cast<int>(m_stereoFrames.size());
    }

    /**
     * @brief 清除立体帧
     */
    void clearStereoFrames();

    /**
     * @brief 执行立体标定
     */
    StereoCalibrationResult calibrateStereo(int flags = 0);

    /**
     * @brief 计算校正映射
     */
    bool computeRectificationMaps(const StereoCalibrationResult& result,
                                   QImage& mapLeft, QImage& mapRight);

    // ============ 手眼标定 ============

    /**
     * @brief 添加手眼标定数据
     */
    bool addHandEyeData(const Matrix4d& robotPose, const QImage& cameraImage);

    /**
     * @brief 获取手眼数据数量
     */
    int getHandEyeDataCount() const {
        return static_cast<int>(m_handEyeData.size());
    }

    /**
     * @brief 清除手眼数据
     */
    void clearHandEyeData();

    /**
     * @brief 执行手眼标定（Eye-in-Hand）
     */
    HandEyeCalibrationResult calibrateHandEyeEIH(HandEyeMethod method = HandEyeMethod::Tsai);

    /**
     * @brief 执行手眼标定（Eye-to-Hand）
     */
    HandEyeCalibrationResult calibrateHandEyeETH(HandEyeMethod method = HandEyeMethod::Tsai);

    // ============ 畸变校正 ============

    /**
     * @brief 去畸变
     */
    QImage undistort(const QImage& image, const CameraIntrinsics& intrinsics);

    /**
     * @brief 计算去畸变映射
     */
    void computeUndistortMap(const CameraIntrinsics& intrinsics,
                              int width, int height,
                              QImage& mapX, QImage& mapY);

    /**
     * @brief 应用映射
     */
    QImage remap(const QImage& image, const QImage& mapX, const QImage& mapY);

    // ============ 文件I/O ============

    /**
     * @brief 保存相机标定结果
     */
    bool saveCameraCalibration(const QString& filename,
                                const CameraCalibrationResult& result);

    /**
     * @brief 加载相机标定结果
     */
    bool loadCameraCalibration(const QString& filename,
                                CameraCalibrationResult& result);

    /**
     * @brief 保存立体标定结果
     */
    bool saveStereoCalibration(const QString& filename,
                                const StereoCalibrationResult& result);

    /**
     * @brief 加载立体标定结果
     */
    bool loadStereoCalibration(const QString& filename,
                                StereoCalibrationResult& result);

    /**
     * @brief 保存手眼标定结果
     */
    bool saveHandEyeCalibration(const QString& filename,
                                 const HandEyeCalibrationResult& result);

    /**
     * @brief 加载手眼标定结果
     */
    bool loadHandEyeCalibration(const QString& filename,
                                 HandEyeCalibrationResult& result);

    // ============ 验证 ============

    /**
     * @brief 验证标定结果
     */
    double validateCalibration(const CameraCalibrationResult& result,
                               const std::vector<CalibrationFrame>& testFrames);

    /**
     * @brief 验证立体标定结果
     */
    double validateStereoCalibration(const StereoCalibrationResult& result,
                                      const std::vector<StereoCalibrationFrame>& testFrames);

signals:
    /**
     * @brief 角点检测完成
     */
    void cornersDetected(bool found, int cornerCount);

    /**
     * @brief 标定进度
     */
    void calibrationProgress(int percent);

    /**
     * @brief 标定完成
     */
    void calibrationCompleted(bool success);

private:
    // 配置
    CalibrationBoardConfig m_boardConfig;

    // 数据
    std::vector<CalibrationFrame> m_frames;
    std::vector<StereoCalibrationFrame> m_stereoFrames;
    std::vector<HandEyeCalibrationData> m_handEyeData;

    // 线程安全
    mutable QMutex m_mutex;

    // 私有方法
    bool detectChessboardCorners(const QImage& image, std::vector<QPointF>& corners);
    bool detectCircleGridCorners(const QImage& image, std::vector<QPointF>& corners);
    Matrix4d solvePnP(const std::vector<Point3D>& objectPoints,
                       const std::vector<QPointF>& imagePoints,
                       const CameraIntrinsics& intrinsics);
};

// ============================================================
// 相机标定器
// ============================================================

/**
 * @brief 相机标定器
 *
 * 独立的相机标定类，用于单相机内参标定。
 */
class CameraCalibrator {
public:
    CameraCalibrator();
    ~CameraCalibrator();

    /**
     * @brief 设置标定板
     */
    void setBoard(const CalibrationBoardConfig& config);

    /**
     * @brief 添加图像
     */
    bool addImage(const QImage& image);

    /**
     * @brief 标定
     */
    CameraCalibrationResult calibrate();

    /**
     * @brief 获取帧数
     */
    int getFrameCount() const;

    /**
     * @brief 清除数据
     */
    void clear();

private:
    CalibrationBoardConfig m_boardConfig;
    std::vector<CalibrationFrame> m_frames;
};

// ============================================================
// 立体相机标定器
// ============================================================

/**
 * @brief 立体相机标定器
 */
class StereoCalibrator {
public:
    StereoCalibrator();
    ~StereoCalibrator();

    /**
     * @brief 设置标定板
     */
    void setBoard(const CalibrationBoardConfig& config);

    /**
     * @brief 添加图像对
     */
    bool addImagePair(const QImage& leftImage, const QImage& rightImage);

    /**
     * @brief 标定
     */
    StereoCalibrationResult calibrate();

    /**
     * @brief 获取帧数
     */
    int getFrameCount() const;

    /**
     * @brief 清除数据
     */
    void clear();

private:
    CalibrationBoardConfig m_boardConfig;
    std::vector<StereoCalibrationFrame> m_stereoFrames;
};

// ============================================================
// 工具函数
// ============================================================

/**
 * @brief 获取标定类型名称
 */
QString getCalibration3DTypeName(Calibration3DType type);

/**
 * @brief 获取标定板类型名称
 */
QString getCalibrationBoardTypeName(CalibrationBoardType type);

/**
 * @brief 获取手眼方法名称
 */
QString getHandEyeMethodName(HandEyeMethod method);

/**
 * @brief 评估标定板姿态分布
 */
double evaluatePoseDistribution(const std::vector<Matrix4d>& poses);

/**
 * @brief 计算重投影误差
 */
double computeReprojectionError(const std::vector<Point3D>& objectPoints,
                                 const std::vector<QPointF>& imagePoints,
                                 const CameraIntrinsics& intrinsics,
                                 const Matrix4d& pose);

} // namespace Algorithm3D
} // namespace VisionForge

#endif // VISIONFORGE_CALIBRATION3D_H
