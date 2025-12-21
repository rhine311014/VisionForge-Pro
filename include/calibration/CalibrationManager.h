/**
 * @file CalibrationManager.h
 * @brief VisionForge Pro 标定管理器
 *
 * 本文件定义了视觉标定系统的核心管理器类，提供：
 * - 5点标定、9点标定等多种标定方法
 * - H矩阵计算和优化（SVD分解、DLT算法）
 * - 标定精度评估和误差分析
 * - 配置的保存和加载
 *
 * 数学算法参考VisionASM成熟实现：
 * - 仿射变换：最小二乘法
 * - 齐次变换：DLT（Direct Linear Transform）+ SVD
 * - 可选LM优化提高精度
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#ifndef VISIONFORGE_CALIBRATION_MANAGER_H
#define VISIONFORGE_CALIBRATION_MANAGER_H

#include "CalibrationTypes.h"
#include "platform/CoordPosition.h"
#include "platform/PlatformConfig.h"

#include <QObject>
#include <QString>
#include <QPointF>
#include <QJsonObject>
#include <memory>
#include <functional>

namespace VisionForge {
namespace Calibration {

/**
 * @brief 标定进度回调
 *
 * @param progress 进度（0-100）
 * @param message 当前状态消息
 */
using CalibrationProgressCallback = std::function<void(int progress, const QString& message)>;

/**
 * @brief 标定管理器类
 *
 * 提供完整的视觉标定功能，是标定系统的核心类。
 *
 * 支持的标定方法：
 * 1. 仿射变换标定（3-5点）：适用于简单场景
 * 2. 齐次变换标定（4+点）：适用于透视变换场景
 * 3. 9点/16点标定：工业标准精度
 * 4. DM码自动标定：自动识别标定点
 *
 * 使用示例：
 * @code
 * CalibrationManager manager;
 *
 * // 配置标定
 * CalibrationConfig config;
 * config.type = CalibrationType::Point9;
 * config.gridRows = 3;
 * config.gridCols = 3;
 * manager.setConfig(config);
 *
 * // 添加标定点
 * manager.addCalibrationPoint(QPointF(100, 100), QPointF(0, 0));
 * manager.addCalibrationPoint(QPointF(200, 100), QPointF(10, 0));
 * // ... 添加更多点
 *
 * // 执行标定
 * CalibrationResult result = manager.calibrate();
 * if (result.isValid()) {
 *     // 使用标定结果进行坐标变换
 *     QPointF worldPos = result.pixelToWorld(QPointF(150, 150));
 * }
 * @endcode
 */
class CalibrationManager : public QObject {
    Q_OBJECT

public:
    // ============================================================
    // 构造和析构
    // ============================================================

    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit CalibrationManager(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~CalibrationManager() override;

    // 禁用拷贝
    CalibrationManager(const CalibrationManager&) = delete;
    CalibrationManager& operator=(const CalibrationManager&) = delete;

    // ============================================================
    // 配置管理
    // ============================================================

    /**
     * @brief 设置标定配置
     * @param config 标定配置
     */
    void setConfig(const CalibrationConfig& config);

    /**
     * @brief 获取当前配置
     */
    const CalibrationConfig& getConfig() const { return m_config; }

    /**
     * @brief 设置标定类型
     */
    void setCalibrationType(CalibrationType type);

    /**
     * @brief 获取标定类型
     */
    CalibrationType getCalibrationType() const { return m_config.type; }

    // ============================================================
    // 标定点管理
    // ============================================================

    /**
     * @brief 添加标定点
     * @param pixelPos 像素坐标
     * @param worldPos 世界坐标
     * @return 添加后的点索引
     */
    int addCalibrationPoint(const QPointF& pixelPos, const QPointF& worldPos);

    /**
     * @brief 添加标定点（使用CalibrationPoint结构）
     */
    int addCalibrationPoint(const CalibrationPoint& point);

    /**
     * @brief 设置指定索引的标定点
     * @param index 点索引
     * @param pixelPos 像素坐标
     * @param worldPos 世界坐标
     * @return 是否成功
     */
    bool setCalibrationPoint(int index, const QPointF& pixelPos, const QPointF& worldPos);

    /**
     * @brief 获取指定索引的标定点
     */
    const CalibrationPoint* getCalibrationPoint(int index) const;

    /**
     * @brief 移除指定索引的标定点
     */
    bool removeCalibrationPoint(int index);

    /**
     * @brief 清空所有标定点
     */
    void clearCalibrationPoints();

    /**
     * @brief 获取标定点数量
     */
    int getCalibrationPointCount() const;

    /**
     * @brief 获取有效标定点数量
     */
    int getValidPointCount() const;

    /**
     * @brief 获取所有标定点
     */
    const CalibrationPointSet& getCalibrationPoints() const { return m_points; }

    /**
     * @brief 启用/禁用指定标定点
     */
    bool setPointEnabled(int index, bool enabled);

    // ============================================================
    // 标定执行
    // ============================================================

    /**
     * @brief 执行标定
     *
     * 根据当前配置和已添加的标定点执行标定计算。
     *
     * @return 标定结果
     */
    CalibrationResult calibrate();

    /**
     * @brief 执行标定（带进度回调）
     * @param progressCallback 进度回调函数
     * @return 标定结果
     */
    CalibrationResult calibrate(CalibrationProgressCallback progressCallback);

    /**
     * @brief 使用指定点集执行标定
     * @param points 标定点集
     * @return 标定结果
     */
    CalibrationResult calibrate(const CalibrationPointSet& points);

    /**
     * @brief 验证当前标定
     *
     * 使用当前标定点重新计算误差。
     *
     * @return 验证是否通过
     */
    bool validateCalibration();

    /**
     * @brief 重新计算标定（排除指定点）
     * @param excludeIndices 要排除的点索引列表
     * @return 标定结果
     */
    CalibrationResult recalibrateExcluding(const std::vector<int>& excludeIndices);

    // ============================================================
    // 标定结果
    // ============================================================

    /**
     * @brief 获取当前标定结果
     */
    const CalibrationResult& getResult() const { return m_result; }

    /**
     * @brief 检查是否已标定
     */
    bool isCalibrated() const {
        return m_result.status == CalibrationStatus::Calibrated;
    }

    /**
     * @brief 获取标定状态
     */
    CalibrationStatus getStatus() const { return m_result.status; }

    /**
     * @brief 获取误差统计
     */
    const ErrorStatistics& getErrorStatistics() const {
        return m_result.errorStats;
    }

    // ============================================================
    // 坐标变换
    // ============================================================

    /**
     * @brief 像素坐标转世界坐标
     * @param pixelPos 像素坐标
     * @return 世界坐标，未标定时返回输入值
     */
    QPointF pixelToWorld(const QPointF& pixelPos) const;

    /**
     * @brief 世界坐标转像素坐标
     * @param worldPos 世界坐标
     * @return 像素坐标，未标定时返回输入值
     */
    QPointF worldToPixel(const QPointF& worldPos) const;

    /**
     * @brief 批量像素坐标转世界坐标
     */
    std::vector<QPointF> pixelToWorld(const std::vector<QPointF>& pixelPoints) const;

    /**
     * @brief 批量世界坐标转像素坐标
     */
    std::vector<QPointF> worldToPixel(const std::vector<QPointF>& worldPoints) const;

    /**
     * @brief 转换CoordPosition
     * @param imagePos 图像坐标位姿
     * @return 世界坐标位姿
     */
    Platform::CoordPosition transformToWorld(const Platform::CoordPosition& imagePos) const;

    /**
     * @brief 转换CoordPosition到图像坐标
     * @param worldPos 世界坐标位姿
     * @return 图像坐标位姿
     */
    Platform::CoordPosition transformToImage(const Platform::CoordPosition& worldPos) const;

    // ============================================================
    // 精度评估
    // ============================================================

    /**
     * @brief 计算单点重投影误差
     * @param pixelPos 像素坐标
     * @param worldPos 世界坐标
     * @return 重投影误差（像素）
     */
    double calculateReprojectionError(const QPointF& pixelPos, const QPointF& worldPos) const;

    /**
     * @brief 计算所有点的重投影误差
     * @return 误差统计
     */
    ErrorStatistics calculateAllErrors() const;

    /**
     * @brief 识别异常点
     * @param threshold 阈值（标准差倍数）
     * @return 异常点索引列表
     */
    std::vector<int> identifyOutliers(double threshold = 3.0) const;

    /**
     * @brief 获取缩放因子
     * @param scaleX 输出X方向缩放（mm/pixel）
     * @param scaleY 输出Y方向缩放（mm/pixel）
     */
    void getScale(double& scaleX, double& scaleY) const;

    /**
     * @brief 获取旋转角度
     * @return 旋转角度（度）
     */
    double getRotation() const;

    // ============================================================
    // 持久化
    // ============================================================

    /**
     * @brief 保存标定结果到文件
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool saveToFile(const QString& filePath) const;

    /**
     * @brief 从文件加载标定结果
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool loadFromFile(const QString& filePath);

    /**
     * @brief 导出标定结果为JSON
     */
    QJsonObject exportToJson() const;

    /**
     * @brief 从JSON导入标定结果
     */
    bool importFromJson(const QJsonObject& json);

    /**
     * @brief 获取默认保存路径
     */
    static QString getDefaultSavePath();

    // ============================================================
    // 进度回调
    // ============================================================

    /**
     * @brief 设置进度回调
     */
    void setProgressCallback(CalibrationProgressCallback callback) {
        m_progressCallback = std::move(callback);
    }

signals:
    /**
     * @brief 标定完成信号
     * @param success 是否成功
     * @param result 标定结果
     */
    void calibrationCompleted(bool success, const CalibrationResult& result);

    /**
     * @brief 标定进度信号
     * @param progress 进度（0-100）
     * @param message 当前消息
     */
    void calibrationProgress(int progress, const QString& message);

    /**
     * @brief 标定点变化信号
     */
    void calibrationPointsChanged();

    /**
     * @brief 配置变化信号
     */
    void configChanged();

private:
    // ============================================================
    // 核心标定算法
    // ============================================================

    /**
     * @brief 执行仿射变换标定（3-5点）
     *
     * 使用最小二乘法求解仿射变换矩阵：
     * [x'] = [a b tx] [x]
     * [y']   [c d ty] [y]
     *                 [1]
     *
     * @param points 标定点集
     * @return 标定结果
     */
    CalibrationResult calibrateAffine(const std::vector<CalibrationPoint>& points);

    /**
     * @brief 执行齐次变换标定（DLT算法）
     *
     * 使用直接线性变换（DLT）算法求解齐次矩阵：
     * w*[x'] = [h00 h01 h02] [x]
     * w*[y']   [h10 h11 h12] [y]
     * w*[1 ]   [h20 h21 h22] [1]
     *
     * @param points 标定点集
     * @return 标定结果
     */
    CalibrationResult calibrateHomography(const std::vector<CalibrationPoint>& points);

    /**
     * @brief SVD求解齐次矩阵
     * @param A 系数矩阵
     * @return 3x3齐次矩阵
     */
    Matrix3d solveHomographySVD(const MatrixXd& A);

    /**
     * @brief 构建DLT系数矩阵
     * @param points 标定点
     * @return 系数矩阵
     */
    MatrixXd buildDLTMatrix(const std::vector<CalibrationPoint>& points);

    /**
     * @brief 数据归一化（提高数值稳定性）
     * @param points 输入点集
     * @param normalizedPoints 归一化后的点集
     * @param T1 像素坐标归一化矩阵
     * @param T2 世界坐标归一化矩阵
     */
    void normalizePoints(const std::vector<CalibrationPoint>& points,
                        std::vector<CalibrationPoint>& normalizedPoints,
                        Matrix3d& T1, Matrix3d& T2);

    /**
     * @brief 非线性优化（可选）
     *
     * 使用Levenberg-Marquardt算法优化H矩阵。
     *
     * @param H 初始H矩阵
     * @param points 标定点
     * @return 优化后的H矩阵
     */
    Matrix3d optimizeHomography(const Matrix3d& H,
                                const std::vector<CalibrationPoint>& points);

    /**
     * @brief 计算重投影误差
     * @param H 齐次矩阵
     * @param points 标定点
     * @return 误差统计
     */
    ErrorStatistics computeReprojectionErrors(const Matrix3d& H,
                                              const std::vector<CalibrationPoint>& points) const;

    /**
     * @brief 提取变换参数
     * @param H 齐次矩阵
     * @param scaleX 输出X缩放
     * @param scaleY 输出Y缩放
     * @param rotation 输出旋转角度
     */
    void extractTransformParams(const Matrix3d& H,
                               double& scaleX, double& scaleY, double& rotation);

    /**
     * @brief 报告进度
     */
    void reportProgress(int progress, const QString& message);

private:
    CalibrationConfig m_config;     ///< 标定配置
    CalibrationPointSet m_points;   ///< 标定点集
    CalibrationResult m_result;     ///< 标定结果

    CalibrationProgressCallback m_progressCallback;  ///< 进度回调
};

/**
 * @brief 标定管理器工厂
 *
 * 提供全局标定管理器实例。
 */
class CalibrationManagerFactory {
public:
    /**
     * @brief 获取全局实例
     * @param cameraIndex 相机索引
     */
    static CalibrationManager* getInstance(int cameraIndex = 0);

    /**
     * @brief 创建新实例
     */
    static std::unique_ptr<CalibrationManager> create();

    /**
     * @brief 释放指定相机的实例
     */
    static void releaseInstance(int cameraIndex);

    /**
     * @brief 释放所有实例
     */
    static void releaseAll();

private:
    static std::map<int, std::unique_ptr<CalibrationManager>> s_instances;
};

} // namespace Calibration
} // namespace VisionForge

#endif // VISIONFORGE_CALIBRATION_MANAGER_H
