/**
 * @file CameraCalibTool.h
 * @brief 相机标定工具
 * @details 使用棋盘格/圆点阵进行相机内参标定，支持OpenCV和Halcon两种后端
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "algorithm/CalibrationResult.h"
#include <vector>
#include <opencv2/core.hpp>

#ifdef USE_HALCON
#include <HalconCpp.h>
#endif

namespace VisionForge {
namespace Algorithm {

// CalibPatternType 和 CalibBackend 已在 CalibrationResult.h 中定义

/**
 * @class CameraCalibTool
 * @brief 相机标定工具类
 *
 * 功能：
 * - 检测标定板角点/圆心
 * - 采集多张标定图像
 * - 计算相机内参和畸变系数
 * - 对图像进行畸变校正
 */
class CameraCalibTool : public VisionTool {
    Q_OBJECT

public:
    explicit CameraCalibTool(QObject* parent = nullptr);
    ~CameraCalibTool() override;

    // ========== VisionTool 接口实现 ==========
    QString toolName() const override { return "CameraCalibTool"; }
    ToolType toolType() const override { return ToolType::CameraCalib; }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 标定板参数 ==========

    /**
     * @brief 设置标定板类型
     */
    void setPatternType(CalibPatternType type) { patternType_ = type; }
    CalibPatternType patternType() const { return patternType_; }

    /**
     * @brief 设置标定板尺寸（内角点数）
     * @param width 水平方向角点数
     * @param height 垂直方向角点数
     */
    void setBoardSize(int width, int height);
    int boardWidth() const { return boardWidth_; }
    int boardHeight() const { return boardHeight_; }
    cv::Size boardSize() const { return cv::Size(boardWidth_, boardHeight_); }

    /**
     * @brief 设置格子尺寸（毫米）
     */
    void setSquareSize(double size) { squareSize_ = size; }
    double squareSize() const { return squareSize_; }

    /**
     * @brief 设置后端类型
     */
    void setBackend(CalibBackend backend);
    CalibBackend backend() const { return backend_; }

    // ========== 角点检测 ==========

    /**
     * @brief 检测标定板角点
     * @param image 输入图像
     * @param corners 输出角点坐标
     * @return 是否检测成功
     */
    bool detectCorners(const cv::Mat& image, std::vector<cv::Point2f>& corners);

    /**
     * @brief 检测并绘制角点
     * @param image 输入图像
     * @param output 输出图像（带角点标注）
     * @return 是否检测成功
     */
    bool detectAndDraw(const cv::Mat& image, cv::Mat& output);

    // ========== 图像采集 ==========

    /**
     * @brief 添加标定图像
     * @param image 图像数据
     * @return 是否添加成功（检测到角点才添加）
     */
    bool addCalibImage(const cv::Mat& image);

    /**
     * @brief 添加已检测角点的图像
     * @param image 图像数据
     * @param corners 角点坐标
     */
    void addCalibImageWithCorners(const cv::Mat& image, const std::vector<cv::Point2f>& corners);

    /**
     * @brief 移除指定索引的图像
     */
    bool removeCalibImage(int index);

    /**
     * @brief 清除所有采集的图像
     */
    void clearImages();

    /**
     * @brief 获取已采集图像数量
     */
    int imageCount() const { return static_cast<int>(calibImages_.size()); }

    /**
     * @brief 获取指定图像
     */
    cv::Mat getImage(int index) const;

    /**
     * @brief 获取指定图像的角点
     */
    std::vector<cv::Point2f> getImageCorners(int index) const;

    // ========== 标定执行 ==========

    /**
     * @brief 执行相机标定
     * @param result 输出标定结果
     * @return 是否成功
     */
    bool calibrate(CalibrationResult& result);

    /**
     * @brief 获取最新的标定结果
     */
    const CalibrationResult& getResult() const { return result_; }

    /**
     * @brief 获取重投影误差
     */
    double getRmsError() const { return result_.rmsError(); }

    /**
     * @brief 是否已标定
     */
    bool isCalibrated() const { return result_.isValid(); }

    // ========== 畸变校正 ==========

    /**
     * @brief 对图像进行畸变校正
     * @param src 输入图像
     * @return 校正后的图像
     */
    cv::Mat undistort(const cv::Mat& src) const;

    /**
     * @brief 对点进行畸变校正
     * @param distortedPoints 畸变点
     * @return 校正后的点
     */
    std::vector<cv::Point2f> undistortPoints(const std::vector<cv::Point2f>& distortedPoints) const;

    // ========== 参数设置 ==========

    /**
     * @brief 设置亚像素精度优化
     */
    void setSubPixelRefinement(bool enable) { useSubPixel_ = enable; }
    bool subPixelRefinement() const { return useSubPixel_; }

    /**
     * @brief 设置最小采集图像数
     */
    void setMinImageCount(int count) { minImageCount_ = count; }
    int minImageCount() const { return minImageCount_; }

    /**
     * @brief 设置自适应阈值
     */
    void setAdaptiveThreshold(bool enable) { useAdaptiveThresh_ = enable; }
    bool adaptiveThreshold() const { return useAdaptiveThresh_; }

    /**
     * @brief 设置快速检测模式
     */
    void setFastCheck(bool enable) { useFastCheck_ = enable; }
    bool fastCheck() const { return useFastCheck_; }

signals:
    /**
     * @brief 角点检测完成信号
     */
    void cornersDetected(bool success, int cornerCount);

    /**
     * @brief 图像添加信号
     */
    void imageAdded(int totalCount);

    /**
     * @brief 标定完成信号
     */
    void calibrationFinished(bool success, double rmsError);

protected:
    // OpenCV实现
    bool detectCornersOpenCV(const cv::Mat& image, std::vector<cv::Point2f>& corners);
    bool calibrateOpenCV(CalibrationResult& result);

#ifdef USE_HALCON
    // Halcon实现
    bool detectCornersHalcon(const cv::Mat& image, std::vector<cv::Point2f>& corners);
    bool calibrateHalcon(CalibrationResult& result);
#endif

    // 生成世界坐标点
    std::vector<cv::Point3f> generateObjectPoints() const;

    // 亚像素优化
    void refineCorners(const cv::Mat& grayImage, std::vector<cv::Point2f>& corners);

private:
    // 标定板参数
    CalibPatternType patternType_;
    int boardWidth_;            ///< 内角点水平数
    int boardHeight_;           ///< 内角点垂直数
    double squareSize_;         ///< 格子尺寸(mm)

    // 后端
    CalibBackend backend_;

    // 检测参数
    bool useSubPixel_;          ///< 亚像素精度
    bool useAdaptiveThresh_;    ///< 自适应阈值
    bool useFastCheck_;         ///< 快速检测
    int minImageCount_;         ///< 最小图像数

    // 采集数据
    std::vector<cv::Mat> calibImages_;                      ///< 标定图像
    std::vector<std::vector<cv::Point2f>> imagePoints_;     ///< 图像角点

    // 标定结果
    CalibrationResult result_;
};

} // namespace Algorithm
} // namespace VisionForge
