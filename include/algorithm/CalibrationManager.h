/**
 * @file CalibrationManager.h
 * @brief 标定管理器
 * @details 管理标定数据，与配方系统集成，提供全局坐标转换接口
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>
#include <opencv2/core.hpp>

namespace VisionForge {

namespace Core {
class Recipe;
}

namespace Algorithm {

class CalibrationResult;

/**
 * @class CalibrationManager
 * @brief 标定管理器（单例）
 *
 * 功能：
 * - 管理当前的相机标定和九点标定结果
 * - 与配方系统集成，配方切换时自动加载标定
 * - 提供全局坐标转换和畸变校正接口
 */
class CalibrationManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static CalibrationManager& instance();

    ~CalibrationManager();

    // 禁用拷贝和移动
    CalibrationManager(const CalibrationManager&) = delete;
    CalibrationManager& operator=(const CalibrationManager&) = delete;

    // ========== 配方集成 ==========

    /**
     * @brief 设置当前配方
     * @param recipe 配方指针
     */
    void setCurrentRecipe(Core::Recipe* recipe);

    /**
     * @brief 获取当前配方
     */
    Core::Recipe* currentRecipe() const { return currentRecipe_; }

    /**
     * @brief 获取配方标定目录
     * @param recipe 配方指针
     * @return 标定目录路径 ({配方目录}/calibration/)
     */
    QString getCalibrationPath(Core::Recipe* recipe) const;

    /**
     * @brief 确保标定目录存在
     */
    bool ensureCalibrationDirectory(Core::Recipe* recipe) const;

    // ========== 标定加载/保存 ==========

    /**
     * @brief 从配方加载标定
     * @param recipe 配方指针
     * @return 是否成功
     */
    bool loadFromRecipe(Core::Recipe* recipe);

    /**
     * @brief 保存标定到配方
     * @param recipe 配方指针
     * @return 是否成功
     */
    bool saveToRecipe(Core::Recipe* recipe);

    /**
     * @brief 加载相机标定
     * @param filePath 标定文件路径
     * @return 是否成功
     */
    bool loadCameraCalib(const QString& filePath);

    /**
     * @brief 保存相机标定
     * @param filePath 标定文件路径
     * @return 是否成功
     */
    bool saveCameraCalib(const QString& filePath);

    /**
     * @brief 加载九点标定
     * @param filePath 标定文件路径
     * @return 是否成功
     */
    bool loadNinePointCalib(const QString& filePath);

    /**
     * @brief 保存九点标定
     * @param filePath 标定文件路径
     * @return 是否成功
     */
    bool saveNinePointCalib(const QString& filePath);

    // ========== 标定结果访问 ==========

    /**
     * @brief 获取相机标定结果
     */
    CalibrationResult* cameraCalibResult() const { return cameraCalibResult_.get(); }

    /**
     * @brief 获取九点标定结果
     */
    CalibrationResult* ninePointCalibResult() const { return ninePointCalibResult_.get(); }

    /**
     * @brief 设置相机标定结果
     */
    void setCameraCalibResult(const CalibrationResult& result);

    /**
     * @brief 设置九点标定结果
     */
    void setNinePointCalibResult(const CalibrationResult& result);

    /**
     * @brief 清除相机标定
     */
    void clearCameraCalib();

    /**
     * @brief 清除九点标定
     */
    void clearNinePointCalib();

    /**
     * @brief 清除所有标定
     */
    void clearAll();

    // ========== 标定状态 ==========

    /**
     * @brief 是否有相机标定
     */
    bool hasCameraCalib() const;

    /**
     * @brief 是否有九点标定
     */
    bool hasNinePointCalib() const;

    /**
     * @brief 是否有任何标定
     */
    bool isCalibrated() const;

    // ========== 坐标转换（使用九点标定）==========

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
     * @brief 批量坐标转换
     */
    std::vector<cv::Point2d> imageToWorld(const std::vector<cv::Point2d>& imagePoints) const;
    std::vector<cv::Point2d> worldToImage(const std::vector<cv::Point2d>& worldPoints) const;

    /**
     * @brief 像素距离转物理距离（毫米）
     */
    double pixelToMM(double pixelDistance) const;

    /**
     * @brief 物理距离转像素距离
     */
    double mmToPixel(double mmDistance) const;

    /**
     * @brief 获取像素比例（毫米/像素）
     */
    double pixelScale() const;

    // ========== 畸变校正（使用相机标定）==========

    /**
     * @brief 对图像进行畸变校正
     * @param src 输入图像
     * @return 校正后的图像
     */
    cv::Mat undistortImage(const cv::Mat& src) const;

    /**
     * @brief 对点进行畸变校正
     */
    std::vector<cv::Point2f> undistortPoints(const std::vector<cv::Point2f>& points) const;

    // ========== 标定信息 ==========

    /**
     * @brief 获取标定信息摘要
     */
    QString getSummary() const;

signals:
    /**
     * @brief 标定加载完成
     */
    void calibrationLoaded();

    /**
     * @brief 标定保存完成
     */
    void calibrationSaved();

    /**
     * @brief 标定变更
     */
    void calibrationChanged();

    /**
     * @brief 相机标定变更
     */
    void cameraCalibChanged();

    /**
     * @brief 九点标定变更
     */
    void ninePointCalibChanged();

private:
    // 私有构造函数（单例）
    CalibrationManager();

private:
    // 当前配方
    Core::Recipe* currentRecipe_;

    // 标定结果
    std::unique_ptr<CalibrationResult> cameraCalibResult_;
    std::unique_ptr<CalibrationResult> ninePointCalibResult_;
};

} // namespace Algorithm
} // namespace VisionForge
