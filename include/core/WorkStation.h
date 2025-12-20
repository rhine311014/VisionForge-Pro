/**
 * @file WorkStation.h
 * @brief 工位管理 - 参考VisionASM架构
 * @details 将相机、位置、标定、产品等组织在一起
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>
#include <QJsonObject>
#include <memory>
#include <functional>

// 前向声明
namespace VisionForge {
namespace HAL {
    class IMotionController;
}
namespace Base {
    class ImageData;
}
}

namespace VisionForge {
namespace Core {

/**
 * @brief 位置类型
 */
enum class PositionType {
    Target,         ///< 目标位置（基准）
    Object          ///< 对象位置（待对位）
};

/**
 * @brief 位置配置 - 每个位置对应一个相机拍摄点
 */
struct PositionConfig {
    QString id;                     ///< 位置ID
    QString name;                   ///< 位置名称
    PositionType type;              ///< 位置类型(Target/Object)
    bool enabled;                   ///< 是否启用

    // 关联配置
    QString cameraId;               ///< 关联的相机ID
    QString calibrationId;          ///< 关联的标定ID
    int positionIndex;              ///< 位置索引(同一相机多位置时使用)

    // 平台坐标（拍摄时的机械坐标）
    double platX;                   ///< 平台X坐标
    double platY;                   ///< 平台Y坐标
    double platD;                   ///< 平台D坐标(角度)

    // 搜索工具配置
    QString searchToolId;           ///< 关联的搜索工具ID
    QStringList auxiliaryToolIds;   ///< 辅助搜索工具ID列表

    PositionConfig()
        : type(PositionType::Target)
        , enabled(true)
        , positionIndex(0)
        , platX(0.0)
        , platY(0.0)
        , platD(0.0)
    {}

    QJsonObject toJson() const;
    static PositionConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 对位补偿配置
 */
struct AlignmentOffset {
    double offsetX;                 ///< X方向补偿
    double offsetY;                 ///< Y方向补偿
    double offsetD;                 ///< 角度补偿
    QString description;            ///< 描述

    AlignmentOffset() : offsetX(0.0), offsetY(0.0), offsetD(0.0) {}

    QJsonObject toJson() const;
    static AlignmentOffset fromJson(const QJsonObject& json);
};

/**
 * @brief 对位精度要求
 */
struct AlignmentPrecision {
    double precisionX;              ///< X方向精度要求
    double precisionY;              ///< Y方向精度要求
    double precisionD;              ///< 角度精度要求
    int maxIterations;              ///< 最大对位次数

    AlignmentPrecision()
        : precisionX(0.01)
        , precisionY(0.01)
        , precisionD(0.01)
        , maxIterations(3)
    {}

    QJsonObject toJson() const;
    static AlignmentPrecision fromJson(const QJsonObject& json);
};

/**
 * @brief 对位方式枚举
 */
enum class AlignmentMethod {
    OnePoint,           ///< 单点对位 (1VS1)
    TwoPoint,           ///< 两点对位 (2VS2)
    ThreePoint,         ///< 三点对位 (3VS3)
    FourPoint,          ///< 四点对位 (4VS4)
    MultiPoint          ///< 多点对位 (NVS N)
};

/**
 * @brief 迭代对位配置
 */
struct IterativeAlignConfig {
    bool enabled = false;                   ///< 是否启用迭代对位
    int maxIterations = 3;                  ///< 最大迭代次数
    double convergenceThresholdX = 0.01;    ///< X方向收敛阈值(mm)
    double convergenceThresholdY = 0.01;    ///< Y方向收敛阈值(mm)
    double convergenceThresholdD = 0.01;    ///< 角度收敛阈值(度)
    QString motionControllerId;             ///< 运动控制器ID
    QString plcConnectionName;              ///< PLC连接名称
    double compensationFactorX = 1.0;       ///< X方向补偿系数
    double compensationFactorY = 1.0;       ///< Y方向补偿系数
    double compensationFactorD = 1.0;       ///< 角度补偿系数
    int motionSettleTimeMs = 100;           ///< 运动稳定时间(毫秒)
    int captureDelayMs = 50;                ///< 采集前延时(毫秒)
    bool useIncrementalCompensation = true; ///< 使用增量补偿

    IterativeAlignConfig() = default;

    QJsonObject toJson() const;
    static IterativeAlignConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 对位配置
 */
struct AlignmentConfig {
    AlignmentMethod method;                         ///< 对位方式
    AlignmentPrecision precision;                   ///< 精度要求
    QVector<AlignmentOffset> offsets;               ///< 补偿列表(支持多组)
    int currentOffsetIndex;                         ///< 当前使用的补偿索引

    // 目标和对象位置ID
    QStringList targetPositionIds;                  ///< 目标位置ID列表
    QStringList objectPositionIds;                  ///< 对象位置ID列表

    // 对位选项
    bool enableIterativeAlign;                      ///< 启用迭代对位
    bool useHomography;                             ///< 使用单应性矩阵(vs 仿射变换)

    // 迭代对位配置
    IterativeAlignConfig iterativeConfig;           ///< 迭代对位配置

    AlignmentConfig()
        : method(AlignmentMethod::TwoPoint)
        , currentOffsetIndex(0)
        , enableIterativeAlign(false)
        , useHomography(false)
    {}

    QJsonObject toJson() const;
    static AlignmentConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 搜索结果
 */
struct SearchResult {
    QString positionId;             ///< 位置ID
    bool valid;                     ///< 是否有效

    // 图像坐标
    double imageX;                  ///< 图像X坐标
    double imageY;                  ///< 图像Y坐标
    double imageAngle;              ///< 图像角度
    double score;                   ///< 匹配得分

    // 平台坐标（标定后）
    double platX;                   ///< 平台X坐标
    double platY;                   ///< 平台Y坐标

    // 辅助点坐标
    QVector<QPointF> auxiliaryPoints;

    // 时间戳
    qint64 timestamp;
    double processTime;             ///< 处理时间(ms)

    SearchResult()
        : valid(false)
        , imageX(0.0), imageY(0.0), imageAngle(0.0)
        , score(0.0)
        , platX(0.0), platY(0.0)
        , timestamp(0), processTime(0.0)
    {}

    QJsonObject toJson() const;
};

/**
 * @brief 对位结果
 */
struct WorkStationAlignResult {
    bool success;                               ///< 对位是否成功

    // 对位偏差
    double offsetX;                             ///< X偏差
    double offsetY;                             ///< Y偏差
    double offsetD;                             ///< 角度偏差

    // 补偿后的输出值
    double outputX;                             ///< 输出X值(含补偿)
    double outputY;                             ///< 输出Y值(含补偿)
    double outputD;                             ///< 输出角度值(含补偿)

    // 精度判定
    bool withinTolerance;                       ///< 是否在精度范围内
    int alignmentCount;                         ///< 对位次数

    // 各位置搜索结果
    QMap<QString, SearchResult> targetResults;  ///< 目标位置结果
    QMap<QString, SearchResult> objectResults;  ///< 对象位置结果

    // 残差
    double residualError;                       ///< 残差误差

    // 时间
    qint64 timestamp;
    double totalTime;                           ///< 总耗时(ms)

    WorkStationAlignResult()
        : success(false)
        , offsetX(0.0), offsetY(0.0), offsetD(0.0)
        , outputX(0.0), outputY(0.0), outputD(0.0)
        , withinTolerance(false), alignmentCount(0)
        , residualError(0.0), timestamp(0), totalTime(0.0)
    {}

    QJsonObject toJson() const;
};

/**
 * @brief 迭代对位结果
 */
struct IterativeAlignResult {
    bool success = false;                   ///< 是否成功
    int iterationCount = 0;                 ///< 实际迭代次数
    bool converged = false;                 ///< 是否收敛
    double finalErrorX = 0;                 ///< 最终X误差
    double finalErrorY = 0;                 ///< 最终Y误差
    double finalErrorD = 0;                 ///< 最终角度误差
    double residualError = 0;               ///< 残差误差
    QVector<WorkStationAlignResult> iterationResults;  ///< 每次迭代的结果
    double totalTime = 0;                   ///< 总耗时(毫秒)
    QString errorMessage;                   ///< 错误信息

    IterativeAlignResult() = default;
};

/**
 * @class WorkStation
 * @brief 工位类 - 管理一个完整的视觉对位单元
 *
 * 参考VisionASM设计，一个工位包含：
 * - 位置配置（多个拍摄位置）
 * - 对位配置（对位方式、精度、补偿）
 * - 关联的相机组
 * - 关联的标定数据
 * - 搜索结果数据
 */
class WorkStation : public QObject
{
    Q_OBJECT

public:
    explicit WorkStation(const QString& id, QObject* parent = nullptr);
    ~WorkStation() override;

    // ========== 基本信息 ==========

    QString id() const { return id_; }
    QString name() const { return name_; }
    void setName(const QString& name) { name_ = name; }

    bool isEnabled() const { return enabled_; }
    void setEnabled(bool enabled) { enabled_ = enabled; }

    // ========== 位置管理 ==========

    /**
     * @brief 添加位置
     * @return 位置ID
     */
    QString addPosition(const PositionConfig& config);

    /**
     * @brief 移除位置
     */
    bool removePosition(const QString& positionId);

    /**
     * @brief 更新位置配置
     */
    bool updatePosition(const QString& positionId, const PositionConfig& config);

    /**
     * @brief 获取位置配置
     */
    PositionConfig getPosition(const QString& positionId) const;

    /**
     * @brief 获取所有位置
     */
    QVector<PositionConfig> getAllPositions() const;

    /**
     * @brief 获取目标位置列表
     */
    QVector<PositionConfig> getTargetPositions() const;

    /**
     * @brief 获取对象位置列表
     */
    QVector<PositionConfig> getObjectPositions() const;

    /**
     * @brief 位置数量
     */
    int positionCount() const { return positions_.size(); }

    // ========== 对位配置 ==========

    /**
     * @brief 设置对位配置
     */
    void setAlignmentConfig(const AlignmentConfig& config);

    /**
     * @brief 获取对位配置
     */
    AlignmentConfig getAlignmentConfig() const { return alignmentConfig_; }

    /**
     * @brief 添加对位补偿
     */
    int addAlignmentOffset(const AlignmentOffset& offset);

    /**
     * @brief 设置当前补偿索引
     */
    void setCurrentOffsetIndex(int index);

    /**
     * @brief 获取当前补偿
     */
    AlignmentOffset getCurrentOffset() const;

    // ========== 搜索结果管理 ==========

    /**
     * @brief 设置位置搜索结果
     */
    void setSearchResult(const QString& positionId, const SearchResult& result);

    /**
     * @brief 获取位置搜索结果
     */
    SearchResult getSearchResult(const QString& positionId) const;

    /**
     * @brief 清空搜索结果
     */
    void clearSearchResults();

    /**
     * @brief 获取最近的对位结果
     */
    WorkStationAlignResult getLastAlignResult() const { return lastAlignResult_; }

    // ========== 迭代对位 ==========

    /**
     * @brief 设置运动控制器
     */
    void setMotionController(std::shared_ptr<HAL::IMotionController> controller);

    /**
     * @brief 获取运动控制器
     */
    std::shared_ptr<HAL::IMotionController> motionController() const { return motionController_; }

    /**
     * @brief 设置图像采集回调
     * @param callback 回调函数，返回各相机的图像
     */
    using ImageCaptureCallback = std::function<QMap<QString, std::shared_ptr<Base::ImageData>>()>;
    void setImageCaptureCallback(ImageCaptureCallback callback);

    /**
     * @brief 设置对位计算回调
     * @param callback 回调函数，输入图像返回对位结果
     */
    using AlignmentCallback = std::function<WorkStationAlignResult(
        const QMap<QString, std::shared_ptr<Base::ImageData>>&)>;
    void setAlignmentCallback(AlignmentCallback callback);

    /**
     * @brief 设置PLC输出回调
     * @param callback 回调函数，输出补偿值到PLC
     */
    using PLCOutputCallback = std::function<bool(double x, double y, double theta)>;
    void setPLCOutputCallback(PLCOutputCallback callback);

    /**
     * @brief 执行迭代对位
     * @param initialImages 初始图像（可选，为空则自动采集）
     * @return 迭代对位结果
     */
    IterativeAlignResult executeIterativeAlignment(
        const QMap<QString, std::shared_ptr<Base::ImageData>>& initialImages = {});

    /**
     * @brief 获取最近的迭代对位结果
     */
    IterativeAlignResult getLastIterativeResult() const { return lastIterativeResult_; }

    /**
     * @brief 检查是否达到收敛条件
     */
    bool checkConvergence(const WorkStationAlignResult& result) const;

    /**
     * @brief 计算补偿运动量
     */
    void calculateCompensation(const WorkStationAlignResult& result,
                               double& moveX, double& moveY, double& moveTheta);

    // ========== 配置保存/加载 ==========

    /**
     * @brief 保存到JSON
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON加载
     */
    static std::shared_ptr<WorkStation> fromJson(const QJsonObject& json, QObject* parent = nullptr);

    /**
     * @brief 保存到文件
     */
    bool saveToFile(const QString& filePath);

    /**
     * @brief 从文件加载
     */
    bool loadFromFile(const QString& filePath);

signals:
    void positionAdded(const QString& positionId);
    void positionRemoved(const QString& positionId);
    void positionChanged(const QString& positionId);
    void alignmentConfigChanged();
    void searchResultUpdated(const QString& positionId);
    void iterationStarted(int iterationNumber);
    void iterationCompleted(int iterationNumber, const WorkStationAlignResult& result);
    void iterativeAlignmentCompleted(const IterativeAlignResult& result);

private:
    QString generatePositionId();

private:
    QString id_;                                    ///< 工位ID
    QString name_;                                  ///< 工位名称
    bool enabled_;                                  ///< 是否启用

    QMap<QString, PositionConfig> positions_;       ///< 位置配置
    AlignmentConfig alignmentConfig_;               ///< 对位配置

    QMap<QString, SearchResult> searchResults_;     ///< 搜索结果缓存
    WorkStationAlignResult lastAlignResult_;        ///< 最近对位结果
    IterativeAlignResult lastIterativeResult_;      ///< 最近迭代对位结果

    // 迭代对位相关
    std::shared_ptr<HAL::IMotionController> motionController_;  ///< 运动控制器
    ImageCaptureCallback imageCaptureCallback_;     ///< 图像采集回调
    AlignmentCallback alignmentCallback_;           ///< 对位计算回调
    PLCOutputCallback plcOutputCallback_;           ///< PLC输出回调

    int positionIdCounter_;                         ///< 位置ID计数器
};

/**
 * @class WorkStationManager
 * @brief 工位管理器（单例）
 */
class WorkStationManager : public QObject
{
    Q_OBJECT

public:
    static WorkStationManager& instance();

    // 工位管理
    QString createWorkStation(const QString& name);
    bool removeWorkStation(const QString& wsId);
    WorkStation* getWorkStation(const QString& wsId);
    QStringList getAllWorkStationIds() const;
    int workStationCount() const;

    // 当前工位
    void setCurrentWorkStation(const QString& wsId);
    WorkStation* currentWorkStation();
    QString currentWorkStationId() const { return currentWsId_; }

    // 配置保存/加载
    bool saveAllConfig(const QString& dirPath);
    bool loadAllConfig(const QString& dirPath);

signals:
    void workStationCreated(const QString& wsId);
    void workStationRemoved(const QString& wsId);
    void currentWorkStationChanged(const QString& wsId);

private:
    WorkStationManager();
    ~WorkStationManager();
    WorkStationManager(const WorkStationManager&) = delete;
    WorkStationManager& operator=(const WorkStationManager&) = delete;

private:
    QMap<QString, std::shared_ptr<WorkStation>> workStations_;
    QString currentWsId_;
    int wsIdCounter_;
};

} // namespace Core
} // namespace VisionForge
