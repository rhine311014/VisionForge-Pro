/**
 * @file NinePointCalibTool.h
 * @brief 九点标定工具
 * @details 图像坐标与物理坐标之间的映射标定，支持仿射变换和透视变换
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

/**
 * @brief 标定模式
 */
enum class NinePointCalibMode {
    Affine,         ///< 仿射变换（需要3点以上）
    Perspective,    ///< 透视变换（需要4点）
    Homography      ///< 单应性变换（需要4点以上，最小二乘）
};

/**
 * @brief 标定点结构
 */
struct CalibPoint {
    cv::Point2d imagePos;   ///< 图像坐标（像素）
    cv::Point2d worldPos;   ///< 世界坐标（毫米）
    bool valid;             ///< 是否有效
    QString name;           ///< 点名称（可选）

    CalibPoint()
        : imagePos(0, 0)
        , worldPos(0, 0)
        , valid(false)
    {}

    CalibPoint(const cv::Point2d& img, const cv::Point2d& world, const QString& n = QString())
        : imagePos(img)
        , worldPos(world)
        , valid(true)
        , name(n)
    {}
};

/**
 * @class NinePointCalibTool
 * @brief 九点标定工具类
 *
 * 功能：
 * - 建立图像坐标与物理坐标的映射关系
 * - 支持仿射变换和透视变换
 * - 提供坐标转换接口
 */
class NinePointCalibTool : public VisionTool {
    Q_OBJECT

public:
    explicit NinePointCalibTool(QObject* parent = nullptr);
    ~NinePointCalibTool() override;

    // ========== VisionTool 接口实现 ==========
    QString toolName() const override { return "NinePointCalibTool"; }
    ToolType toolType() const override { return ToolType::NinePointCalib; }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 标定模式 ==========

    /**
     * @brief 设置标定模式
     */
    void setCalibMode(NinePointCalibMode mode) { calibMode_ = mode; }
    NinePointCalibMode calibMode() const { return calibMode_; }

    /**
     * @brief 设置后端类型
     */
    void setBackend(CalibBackend backend);
    CalibBackend backend() const { return backend_; }

    // ========== 标定点管理 ==========

    /**
     * @brief 设置标定点数量
     * @param count 点数量（推荐9点，最少4点）
     */
    void setPointCount(int count);
    int pointCount() const { return static_cast<int>(calibPoints_.size()); }

    /**
     * @brief 获取有效点数量
     */
    int validPointCount() const;

    /**
     * @brief 设置标定点
     * @param index 点索引
     * @param imagePos 图像坐标
     * @param worldPos 物理坐标
     * @return 是否成功
     */
    bool setCalibPoint(int index, const cv::Point2d& imagePos, const cv::Point2d& worldPos);

    /**
     * @brief 设置标定点（仅图像坐标）
     */
    bool setImagePoint(int index, const cv::Point2d& imagePos);

    /**
     * @brief 设置标定点（仅物理坐标）
     */
    bool setWorldPoint(int index, const cv::Point2d& worldPos);

    /**
     * @brief 获取标定点
     */
    CalibPoint getCalibPoint(int index) const;

    /**
     * @brief 获取所有标定点
     */
    std::vector<CalibPoint> getAllCalibPoints() const { return calibPoints_; }

    /**
     * @brief 移除标定点
     */
    bool removeCalibPoint(int index);

    /**
     * @brief 清除所有标定点
     */
    void clearCalibPoints();

    /**
     * @brief 设置标定点有效性
     */
    void setPointValid(int index, bool valid);

    /**
     * @brief 设置标定点名称
     */
    void setPointName(int index, const QString& name);

    // ========== 预设网格 ==========

    /**
     * @brief 生成规则网格的物理坐标
     * @param rows 行数
     * @param cols 列数
     * @param spacingX X方向间距（毫米）
     * @param spacingY Y方向间距（毫米）
     * @param originX 原点X坐标（毫米）
     * @param originY 原点Y坐标（毫米）
     */
    void generateGridWorldPoints(int rows, int cols,
                                 double spacingX, double spacingY,
                                 double originX = 0.0, double originY = 0.0);

    // ========== 标定执行 ==========

    /**
     * @brief 执行标定
     * @param result 输出标定结果
     * @return 是否成功
     */
    bool calibrate(CalibrationResult& result);

    /**
     * @brief 获取最新的标定结果
     */
    const CalibrationResult& getResult() const { return result_; }

    /**
     * @brief 是否已标定
     */
    bool isCalibrated() const { return result_.isValid(); }

    /**
     * @brief 获取标定精度（毫米）
     */
    double getCalibrationError() const { return result_.calibrationError(); }

    // ========== 坐标转换 ==========

    /**
     * @brief 图像坐标转物理坐标
     * @param imagePoint 图像坐标（像素）
     * @return 物理坐标（毫米）
     */
    cv::Point2d imageToWorld(const cv::Point2d& imagePoint) const;

    /**
     * @brief 物理坐标转图像坐标
     * @param worldPoint 物理坐标（毫米）
     * @return 图像坐标（像素）
     */
    cv::Point2d worldToImage(const cv::Point2d& worldPoint) const;

    /**
     * @brief 批量坐标转换（图像到物理）
     */
    std::vector<cv::Point2d> imageToWorld(const std::vector<cv::Point2d>& imagePoints) const;

    /**
     * @brief 批量坐标转换（物理到图像）
     */
    std::vector<cv::Point2d> worldToImage(const std::vector<cv::Point2d>& worldPoints) const;

    /**
     * @brief 像素距离转物理距离（毫米）
     */
    double pixelToMM(double pixelDistance) const;

    /**
     * @brief 物理距离转像素距离
     */
    double mmToPixel(double mmDistance) const;

    // ========== 获取标定参数 ==========

    /**
     * @brief 获取X方向像素比例（毫米/像素）
     */
    double pixelToMMX() const { return result_.pixelToMMX(); }

    /**
     * @brief 获取Y方向像素比例（毫米/像素）
     */
    double pixelToMMY() const { return result_.pixelToMMY(); }

    /**
     * @brief 获取旋转角度（弧度）
     */
    double rotationAngle() const { return result_.rotationAngle(); }

    /**
     * @brief 获取旋转角度（度）
     */
    double rotationAngleDegrees() const;

signals:
    /**
     * @brief 标定点变更信号
     */
    void calibPointsChanged();

    /**
     * @brief 标定完成信号
     */
    void calibrationFinished(bool success, double error);

protected:
    // OpenCV实现
    bool calibrateOpenCV(CalibrationResult& result);

#ifdef USE_HALCON
    // Halcon实现
    bool calibrateHalcon(CalibrationResult& result);
#endif

    // 计算标定误差
    double calculateError(const cv::Mat& H) const;

    // 计算像素比例和旋转角度
    void calculateScaleAndRotation(CalibrationResult& result) const;

private:
    // 标定模式
    NinePointCalibMode calibMode_;

    // 后端
    CalibBackend backend_;

    // 标定点
    std::vector<CalibPoint> calibPoints_;

    // 标定结果
    CalibrationResult result_;
};

} // namespace Algorithm
} // namespace VisionForge
