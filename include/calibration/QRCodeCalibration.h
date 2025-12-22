/**
 * @file QRCodeCalibration.h
 * @brief VisionForge Pro QR码/DM码自动标定模块
 *
 * 本文件定义了基于2D码（QR码、DataMatrix码）的自动标定系统，提供：
 * - 2D码检测和解码
 * - 角点自动提取
 * - 与CalibrationManager的集成
 * - 自动标定点生成
 *
 * 设计参考VisionASM的DmCodeCalibrate模块架构。
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025
 */

#ifndef VISIONFORGE_QRCODE_CALIBRATION_H
#define VISIONFORGE_QRCODE_CALIBRATION_H

#include "CalibrationTypes.h"
#include "CalibrationManager.h"

#include <QObject>
#include <QString>
#include <QPointF>
#include <QRectF>
#include <QImage>
#include <QJsonObject>
#include <vector>
#include <array>
#include <memory>
#include <functional>

namespace VisionForge {
namespace Calibration {

// ============================================================
// 2D码类型枚举
// ============================================================

/**
 * @brief 2D码类型
 */
enum class Code2DType {
    None = 0,           ///< 无
    QRCode = 1,         ///< QR码
    DataMatrix = 2,     ///< DataMatrix码（DM码）
    PDF417 = 3,         ///< PDF417码
    Aztec = 4,          ///< Aztec码
    Auto = 99,          ///< 自动检测
};

/**
 * @brief 标定板布局类型
 */
enum class CalibrationBoardLayout {
    Single = 0,         ///< 单个码
    Grid2x2 = 1,        ///< 2x2网格
    Grid3x3 = 2,        ///< 3x3网格
    Grid4x4 = 3,        ///< 4x4网格
    Custom = 99,        ///< 自定义布局
};

/**
 * @brief 角点排序方式
 */
enum class CornerOrder {
    TopLeftFirst = 0,       ///< 从左上角开始顺时针
    BottomLeftFirst = 1,    ///< 从左下角开始顺时针
    CenterOutward = 2,      ///< 从中心向外
};

// ============================================================
// 2D码检测结果
// ============================================================

/**
 * @brief 单个2D码的检测结果
 */
struct Code2DResult {
    // ============ 基本信息 ============
    Code2DType type = Code2DType::None;     ///< 码类型
    QString content;                         ///< 解码内容
    bool decoded = false;                    ///< 是否成功解码

    // ============ 位置信息 ============
    QRectF boundingRect;                     ///< 边界矩形
    QPointF center;                          ///< 中心点

    /**
     * @brief 四个角点（像素坐标）
     *
     * 角点顺序：左上、右上、右下、左下（顺时针）
     * corners[0] = 左上角
     * corners[1] = 右上角
     * corners[2] = 右下角
     * corners[3] = 左下角
     */
    std::array<QPointF, 4> corners;

    // ============ 质量信息 ============
    double confidence = 0.0;                 ///< 检测置信度（0-1）
    double angle = 0.0;                      ///< 旋转角度（度）
    int moduleSize = 0;                      ///< 模块大小（像素）
    int version = 0;                         ///< QR码版本号

    // ============ 索引 ============
    int index = -1;                          ///< 在检测结果中的索引

    // ============ 方法 ============

    /**
     * @brief 检查结果是否有效
     */
    bool isValid() const {
        return type != Code2DType::None && confidence > 0.5;
    }

    /**
     * @brief 获取角点作为CalibrationPoint（需要提供世界坐标）
     * @param worldCorners 四个角点的世界坐标
     * @return 四个标定点
     */
    std::vector<CalibrationPoint> toCalibrationPoints(
        const std::array<QPointF, 4>& worldCorners) const;

    /**
     * @brief 计算码的物理尺寸（基于世界坐标）
     * @param worldCorners 四个角点的世界坐标
     * @return 尺寸（宽度, 高度）
     */
    QPointF calculatePhysicalSize(const std::array<QPointF, 4>& worldCorners) const;

    QJsonObject toJson() const;
    static Code2DResult fromJson(const QJsonObject& json);
    QString toString() const;
};

/**
 * @brief 2D码检测配置
 */
struct Code2DDetectionConfig {
    // ============ 检测参数 ============
    Code2DType expectedType = Code2DType::Auto;  ///< 期望的码类型
    int maxCodes = 10;                            ///< 最大检测数量
    bool decodeContent = true;                    ///< 是否解码内容

    // ============ 图像预处理 ============
    bool enablePreprocessing = true;              ///< 启用预处理
    int blurKernelSize = 3;                       ///< 模糊核大小
    double contrastEnhancement = 1.0;             ///< 对比度增强因子
    bool adaptiveThreshold = true;                ///< 自适应阈值

    // ============ 角点精化 ============
    bool refineCorners = true;                    ///< 精化角点位置
    int refineWindowSize = 11;                    ///< 精化窗口大小
    double refineEpsilon = 0.01;                  ///< 精化收敛阈值
    int refineMaxIterations = 30;                 ///< 精化最大迭代次数

    // ============ 过滤条件 ============
    double minConfidence = 0.6;                   ///< 最小置信度
    int minModuleSize = 3;                        ///< 最小模块大小（像素）
    int maxModuleSize = 100;                      ///< 最大模块大小（像素）
    QRectF regionOfInterest;                      ///< 感兴趣区域（空=全图）

    QJsonObject toJson() const;
    static Code2DDetectionConfig fromJson(const QJsonObject& json);
};

// ============================================================
// 标定板配置
// ============================================================

/**
 * @brief 标定板配置
 *
 * 定义标定板的物理布局和尺寸。
 */
struct CalibrationBoardConfig {
    // ============ 布局 ============
    CalibrationBoardLayout layout = CalibrationBoardLayout::Single;
    int gridRows = 1;                            ///< 网格行数
    int gridCols = 1;                            ///< 网格列数

    // ============ 尺寸（mm） ============
    double codeSize = 10.0;                      ///< 单个码的边长（mm）
    double codeSpacingX = 20.0;                  ///< 码之间X间距（mm）
    double codeSpacingY = 20.0;                  ///< 码之间Y间距（mm）

    // ============ 原点设置 ============
    QPointF originOffset{0, 0};                  ///< 原点偏移（mm）
    CornerOrder cornerOrder = CornerOrder::TopLeftFirst;

    // ============ 码内容验证 ============
    bool validateContent = false;                ///< 验证码内容
    QStringList expectedContents;                ///< 期望的码内容列表

    /**
     * @brief 计算指定位置码的世界坐标角点
     * @param row 行索引
     * @param col 列索引
     * @return 四个角点的世界坐标
     */
    std::array<QPointF, 4> getCodeWorldCorners(int row, int col) const;

    /**
     * @brief 获取所有码的世界坐标角点
     * @return 每个码的四个角点
     */
    std::vector<std::array<QPointF, 4>> getAllCodeWorldCorners() const;

    /**
     * @brief 获取总标定点数
     */
    int getTotalPointCount() const {
        return gridRows * gridCols * 4;  // 每个码4个角点
    }

    QJsonObject toJson() const;
    static CalibrationBoardConfig fromJson(const QJsonObject& json);
};

// ============================================================
// QR码标定类
// ============================================================

/**
 * @brief QR码/DM码自动标定进度回调
 */
using QRCalibrationProgressCallback = std::function<void(int progress, const QString& message)>;

/**
 * @brief QR码/DM码自动标定类
 *
 * 提供基于2D码的自动视觉标定功能：
 * 1. 检测图像中的QR码或DataMatrix码
 * 2. 提取码的角点作为标定点
 * 3. 与CalibrationManager集成进行标定计算
 *
 * 使用示例：
 * @code
 * QRCodeCalibration calibrator;
 *
 * // 配置标定板
 * CalibrationBoardConfig boardConfig;
 * boardConfig.layout = CalibrationBoardLayout::Grid3x3;
 * boardConfig.codeSize = 10.0;  // 10mm
 * boardConfig.codeSpacingX = 30.0;
 * boardConfig.codeSpacingY = 30.0;
 * calibrator.setBoardConfig(boardConfig);
 *
 * // 检测并标定
 * QImage image("calibration_image.png");
 * auto result = calibrator.detectAndCalibrate(image);
 * if (result.isValid()) {
 *     QPointF worldPos = result.pixelToWorld(QPointF(320, 240));
 * }
 * @endcode
 */
class QRCodeCalibration : public QObject {
    Q_OBJECT

public:
    // ============================================================
    // 构造和析构
    // ============================================================

    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit QRCodeCalibration(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~QRCodeCalibration() override;

    // 禁用拷贝
    QRCodeCalibration(const QRCodeCalibration&) = delete;
    QRCodeCalibration& operator=(const QRCodeCalibration&) = delete;

    // ============================================================
    // 配置
    // ============================================================

    /**
     * @brief 设置检测配置
     */
    void setDetectionConfig(const Code2DDetectionConfig& config);

    /**
     * @brief 获取检测配置
     */
    const Code2DDetectionConfig& getDetectionConfig() const { return m_detectionConfig; }

    /**
     * @brief 设置标定板配置
     */
    void setBoardConfig(const CalibrationBoardConfig& config);

    /**
     * @brief 获取标定板配置
     */
    const CalibrationBoardConfig& getBoardConfig() const { return m_boardConfig; }

    /**
     * @brief 设置CalibrationManager配置
     */
    void setCalibrationConfig(const CalibrationConfig& config);

    /**
     * @brief 获取CalibrationManager配置
     */
    const CalibrationConfig& getCalibrationConfig() const;

    // ============================================================
    // 2D码检测
    // ============================================================

    /**
     * @brief 检测图像中的2D码
     * @param image 输入图像
     * @return 检测结果列表
     */
    std::vector<Code2DResult> detect(const QImage& image);

    /**
     * @brief 检测图像中的2D码（从文件）
     * @param imagePath 图像文件路径
     * @return 检测结果列表
     */
    std::vector<Code2DResult> detect(const QString& imagePath);

    /**
     * @brief 检测图像中的2D码（原始数据）
     * @param imageData 图像数据指针
     * @param width 图像宽度
     * @param height 图像高度
     * @param format 图像格式
     * @return 检测结果列表
     */
    std::vector<Code2DResult> detect(const uchar* imageData, int width, int height,
                                      QImage::Format format = QImage::Format_Grayscale8);

    /**
     * @brief 获取最后一次检测结果
     */
    const std::vector<Code2DResult>& getLastDetectionResults() const {
        return m_lastDetectionResults;
    }

    /**
     * @brief 清除检测结果
     */
    void clearDetectionResults();

    // ============================================================
    // 角点提取
    // ============================================================

    /**
     * @brief 从检测结果中提取标定点
     * @param detectionResults 检测结果
     * @return 标定点集
     */
    CalibrationPointSet extractCalibrationPoints(
        const std::vector<Code2DResult>& detectionResults);

    /**
     * @brief 精化角点位置（亚像素精度）
     * @param image 原始图像
     * @param corners 初始角点
     * @return 精化后的角点
     */
    std::vector<QPointF> refineCorners(const QImage& image,
                                        const std::vector<QPointF>& corners);

    /**
     * @brief 匹配检测到的码与标定板配置
     * @param detectionResults 检测结果
     * @return 匹配成功的索引对（检测索引, 标定板位置索引）
     */
    std::vector<std::pair<int, int>> matchCodesToBoard(
        const std::vector<Code2DResult>& detectionResults);

    // ============================================================
    // 自动标定
    // ============================================================

    /**
     * @brief 检测并执行标定（一键标定）
     * @param image 输入图像
     * @return 标定结果
     */
    CalibrationResult detectAndCalibrate(const QImage& image);

    /**
     * @brief 检测并执行标定（带进度回调）
     * @param image 输入图像
     * @param progressCallback 进度回调
     * @return 标定结果
     */
    CalibrationResult detectAndCalibrate(const QImage& image,
                                          QRCalibrationProgressCallback progressCallback);

    /**
     * @brief 使用检测结果执行标定
     * @param detectionResults 检测结果
     * @return 标定结果
     */
    CalibrationResult calibrate(const std::vector<Code2DResult>& detectionResults);

    /**
     * @brief 添加额外的标定点（手动补充）
     * @param pixelPos 像素坐标
     * @param worldPos 世界坐标
     * @return 添加后的索引
     */
    int addManualPoint(const QPointF& pixelPos, const QPointF& worldPos);

    /**
     * @brief 移除标定点
     * @param index 点索引
     */
    bool removePoint(int index);

    /**
     * @brief 清空所有标定点
     */
    void clearPoints();

    // ============================================================
    // 标定结果访问
    // ============================================================

    /**
     * @brief 获取标定结果
     */
    const CalibrationResult& getResult() const;

    /**
     * @brief 检查是否已标定
     */
    bool isCalibrated() const;

    /**
     * @brief 像素坐标转世界坐标
     */
    QPointF pixelToWorld(const QPointF& pixelPos) const;

    /**
     * @brief 世界坐标转像素坐标
     */
    QPointF worldToPixel(const QPointF& worldPos) const;

    /**
     * @brief 获取内部CalibrationManager
     */
    CalibrationManager* getCalibrationManager() { return m_calibrationManager.get(); }
    const CalibrationManager* getCalibrationManager() const { return m_calibrationManager.get(); }

    // ============================================================
    // 验证和诊断
    // ============================================================

    /**
     * @brief 验证检测结果与标定板配置是否匹配
     * @param detectionResults 检测结果
     * @return 验证通过返回true
     */
    bool validateDetectionResults(const std::vector<Code2DResult>& detectionResults) const;

    /**
     * @brief 计算检测质量分数
     * @param detectionResults 检测结果
     * @return 质量分数（0-100）
     */
    double calculateDetectionQuality(const std::vector<Code2DResult>& detectionResults) const;

    /**
     * @brief 获取诊断信息
     * @return 诊断信息字符串
     */
    QString getDiagnosticInfo() const;

    /**
     * @brief 获取最后一次错误信息
     */
    QString getLastError() const { return m_lastError; }

    // ============================================================
    // 持久化
    // ============================================================

    /**
     * @brief 保存配置和结果到文件
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool saveToFile(const QString& filePath) const;

    /**
     * @brief 从文件加载配置和结果
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool loadFromFile(const QString& filePath);

    /**
     * @brief 导出为JSON
     */
    QJsonObject exportToJson() const;

    /**
     * @brief 从JSON导入
     */
    bool importFromJson(const QJsonObject& json);

signals:
    /**
     * @brief 检测完成信号
     * @param results 检测结果
     */
    void detectionCompleted(const std::vector<Code2DResult>& results);

    /**
     * @brief 标定完成信号
     * @param success 是否成功
     * @param result 标定结果
     */
    void calibrationCompleted(bool success, const CalibrationResult& result);

    /**
     * @brief 进度信号
     * @param progress 进度（0-100）
     * @param message 消息
     */
    void progressUpdated(int progress, const QString& message);

    /**
     * @brief 错误信号
     * @param errorMessage 错误消息
     */
    void errorOccurred(const QString& errorMessage);

private:
    // ============================================================
    // 内部实现
    // ============================================================

    /**
     * @brief 检测QR码
     */
    std::vector<Code2DResult> detectQRCodes(const QImage& image);

    /**
     * @brief 检测DataMatrix码
     */
    std::vector<Code2DResult> detectDataMatrixCodes(const QImage& image);

    /**
     * @brief 图像预处理
     */
    QImage preprocessImage(const QImage& image);

    /**
     * @brief 提取码的角点
     */
    bool extractCodeCorners(const QImage& image, Code2DResult& result);

    /**
     * @brief 亚像素角点精化
     */
    QPointF refineCornerSubpixel(const QImage& image, const QPointF& corner);

    /**
     * @brief 根据内容匹配码的位置
     */
    int matchCodeByContent(const Code2DResult& result) const;

    /**
     * @brief 根据位置匹配码的位置
     */
    int matchCodeByPosition(const Code2DResult& result,
                            const std::vector<std::array<QPointF, 4>>& expectedPositions) const;

    /**
     * @brief 计算两点距离
     */
    static double distance(const QPointF& p1, const QPointF& p2);

    /**
     * @brief 报告进度
     */
    void reportProgress(int progress, const QString& message);

    /**
     * @brief 设置错误信息
     */
    void setError(const QString& error);

private:
    Code2DDetectionConfig m_detectionConfig;     ///< 检测配置
    CalibrationBoardConfig m_boardConfig;        ///< 标定板配置

    std::unique_ptr<CalibrationManager> m_calibrationManager;  ///< 标定管理器

    std::vector<Code2DResult> m_lastDetectionResults;  ///< 最后检测结果
    QString m_lastError;                               ///< 最后错误信息

    QRCalibrationProgressCallback m_progressCallback;  ///< 进度回调
};

// ============================================================
// 工具函数
// ============================================================

/**
 * @brief 获取2D码类型名称
 */
QString getCode2DTypeName(Code2DType type);

/**
 * @brief 从名称获取2D码类型
 */
Code2DType getCode2DTypeFromName(const QString& name);

/**
 * @brief 获取标定板布局名称
 */
QString getBoardLayoutName(CalibrationBoardLayout layout);

/**
 * @brief 从名称获取标定板布局
 */
CalibrationBoardLayout getBoardLayoutFromName(const QString& name);

} // namespace Calibration
} // namespace VisionForge

// 注册Qt元类型
Q_DECLARE_METATYPE(VisionForge::Calibration::Code2DType)
Q_DECLARE_METATYPE(VisionForge::Calibration::Code2DResult)
Q_DECLARE_METATYPE(VisionForge::Calibration::CalibrationBoardLayout)

#endif // VISIONFORGE_QRCODE_CALIBRATION_H
