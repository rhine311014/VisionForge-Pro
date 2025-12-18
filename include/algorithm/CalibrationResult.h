/**
 * @file CalibrationResult.h
 * @brief 标定结果类
 * @details 存储相机标定和九点标定的结果数据，支持序列化和文件操作
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <opencv2/core.hpp>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 标定类型枚举
 */
enum class CalibrationType {
    None = 0,           ///< 未标定
    Camera,             ///< 相机标定（畸变校正）
    NinePoint           ///< 九点标定（坐标映射）
};

/**
 * @brief 标定后端类型
 */
enum class CalibBackend {
    OpenCV,             ///< 使用OpenCV
    Halcon              ///< 使用Halcon
};

/**
 * @brief 标定板类型
 */
enum class CalibPatternType {
    Chessboard,         ///< 棋盘格
    CircleGrid,         ///< 对称圆点阵
    AsymCircleGrid      ///< 非对称圆点阵
};

/**
 * @class CalibrationResult
 * @brief 标定结果类
 *
 * 存储两种类型的标定结果：
 * - 相机标定：内参矩阵、畸变系数、重投影误差
 * - 九点标定：单应性矩阵、像素比例、旋转角度
 */
class CalibrationResult {
public:
    CalibrationResult();
    ~CalibrationResult();

    /**
     * @brief 重置所有标定数据
     */
    void reset();

    /**
     * @brief 是否有效
     */
    bool isValid() const { return isValid_; }

    /**
     * @brief 获取标定类型
     */
    CalibrationType type() const { return type_; }

    /**
     * @brief 设置标定类型
     */
    void setType(CalibrationType type) { type_ = type; }

    // ========== 相机标定参数 ==========

    /**
     * @brief 获取相机内参矩阵 (3x3)
     * @details fx, 0, cx
     *          0, fy, cy
     *          0,  0,  1
     */
    cv::Mat cameraMatrix() const { return cameraMatrix_.clone(); }
    void setCameraMatrix(const cv::Mat& matrix);

    /**
     * @brief 获取畸变系数 (1x5 或 1x8)
     * @details k1, k2, p1, p2, k3 [, k4, k5, k6]
     */
    cv::Mat distCoeffs() const { return distCoeffs_.clone(); }
    void setDistCoeffs(const cv::Mat& coeffs);

    /**
     * @brief 获取重投影误差 (RMS, 像素)
     */
    double rmsError() const { return rmsError_; }
    void setRmsError(double rms) { rmsError_ = rms; }

    /**
     * @brief 获取图像尺寸
     */
    cv::Size imageSize() const { return imageSize_; }
    void setImageSize(const cv::Size& size) { imageSize_ = size; }

    /**
     * @brief 获取焦距 (fx, fy)
     */
    cv::Point2d focalLength() const;

    /**
     * @brief 获取光心 (cx, cy)
     */
    cv::Point2d principalPoint() const;

    // ========== 九点标定参数 ==========

    /**
     * @brief 获取单应性矩阵 (3x3)
     * @details 图像坐标 -> 世界坐标的变换
     */
    cv::Mat homography() const { return homography_.clone(); }
    void setHomography(const cv::Mat& H);

    /**
     * @brief 获取单应性矩阵逆 (3x3)
     * @details 世界坐标 -> 图像坐标的变换
     */
    cv::Mat homographyInv() const { return homographyInv_.clone(); }

    /**
     * @brief 获取仿射变换矩阵 (2x3)
     */
    cv::Mat affineMatrix() const { return affineMatrix_.clone(); }
    void setAffineMatrix(const cv::Mat& M);

    /**
     * @brief 获取X方向像素到毫米的比例
     */
    double pixelToMMX() const { return pixelToMMX_; }
    void setPixelToMMX(double ratio) { pixelToMMX_ = ratio; }

    /**
     * @brief 获取Y方向像素到毫米的比例
     */
    double pixelToMMY() const { return pixelToMMY_; }
    void setPixelToMMY(double ratio) { pixelToMMY_ = ratio; }

    /**
     * @brief 获取旋转角度（弧度）
     */
    double rotationAngle() const { return rotationAngle_; }
    void setRotationAngle(double angle) { rotationAngle_ = angle; }

    /**
     * @brief 获取平均像素比例
     */
    double pixelToMM() const { return (pixelToMMX_ + pixelToMMY_) / 2.0; }

    /**
     * @brief 获取标定精度（毫米）
     */
    double calibrationError() const { return calibrationError_; }
    void setCalibrationError(double error) { calibrationError_ = error; }

    // ========== 标定点数据 ==========

    /**
     * @brief 设置标定点对
     * @param imagePoints 图像坐标点
     * @param worldPoints 世界坐标点
     */
    void setCalibPoints(const std::vector<cv::Point2d>& imagePoints,
                        const std::vector<cv::Point2d>& worldPoints);

    /**
     * @brief 获取图像坐标点
     */
    std::vector<cv::Point2d> imagePoints() const { return imagePoints_; }

    /**
     * @brief 获取世界坐标点
     */
    std::vector<cv::Point2d> worldPoints() const { return worldPoints_; }

    /**
     * @brief 获取标定点数量
     */
    int pointCount() const { return static_cast<int>(imagePoints_.size()); }

    // ========== 坐标转换 ==========

    /**
     * @brief 图像坐标转世界坐标
     * @param imagePoint 图像坐标(像素)
     * @return 世界坐标(毫米)
     */
    cv::Point2d imageToWorld(const cv::Point2d& imagePoint) const;

    /**
     * @brief 世界坐标转图像坐标
     * @param worldPoint 世界坐标(毫米)
     * @return 图像坐标(像素)
     */
    cv::Point2d worldToImage(const cv::Point2d& worldPoint) const;

    /**
     * @brief 批量图像坐标转世界坐标
     */
    std::vector<cv::Point2d> imageToWorld(const std::vector<cv::Point2d>& imagePoints) const;

    /**
     * @brief 批量世界坐标转图像坐标
     */
    std::vector<cv::Point2d> worldToImage(const std::vector<cv::Point2d>& worldPoints) const;

    /**
     * @brief 对图像进行畸变校正
     * @param src 输入图像
     * @return 校正后的图像
     */
    cv::Mat undistort(const cv::Mat& src) const;

    // ========== 序列化 ==========

    /**
     * @brief 序列化为JSON
     */
    QJsonObject serialize() const;

    /**
     * @brief 从JSON反序列化
     */
    bool deserialize(const QJsonObject& json);

    /**
     * @brief 保存到文件
     * @param filePath 文件路径(.json)
     */
    bool saveToFile(const QString& filePath) const;

    /**
     * @brief 从文件加载
     * @param filePath 文件路径(.json)
     */
    bool loadFromFile(const QString& filePath);

    // ========== 辅助方法 ==========

    /**
     * @brief 获取标定信息摘要
     */
    QString summary() const;

    /**
     * @brief 获取标定时间
     */
    qint64 calibrationTime() const { return calibrationTime_; }
    void setCalibrationTime(qint64 time) { calibrationTime_ = time; }

    /**
     * @brief 获取标定描述
     */
    QString description() const { return description_; }
    void setDescription(const QString& desc) { description_ = desc; }

private:
    // 将cv::Mat转换为QJsonArray
    static QJsonArray matToJsonArray(const cv::Mat& mat);
    // 从QJsonArray恢复cv::Mat
    static cv::Mat jsonArrayToMat(const QJsonArray& array, int rows, int cols, int type);

private:
    CalibrationType type_;          ///< 标定类型
    bool isValid_;                  ///< 是否有效

    // 相机标定参数
    cv::Mat cameraMatrix_;          ///< 相机内参矩阵 3x3
    cv::Mat distCoeffs_;            ///< 畸变系数
    double rmsError_;               ///< 重投影误差
    cv::Size imageSize_;            ///< 图像尺寸

    // 九点标定参数
    cv::Mat homography_;            ///< 单应性矩阵 3x3
    cv::Mat homographyInv_;         ///< 单应性矩阵逆
    cv::Mat affineMatrix_;          ///< 仿射变换矩阵 2x3
    double pixelToMMX_;             ///< X方向像素比例
    double pixelToMMY_;             ///< Y方向像素比例
    double rotationAngle_;          ///< 旋转角度
    double calibrationError_;       ///< 标定精度

    // 标定点
    std::vector<cv::Point2d> imagePoints_;   ///< 图像坐标
    std::vector<cv::Point2d> worldPoints_;   ///< 世界坐标

    // 元数据
    qint64 calibrationTime_;        ///< 标定时间
    QString description_;           ///< 描述
};

} // namespace Algorithm
} // namespace VisionForge
