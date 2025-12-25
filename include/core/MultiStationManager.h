/**
 * @file MultiStationManager.h
 * @brief 多工位管理器
 *
 * 统一管理所有工位配置和切换，提供工位的CRUD操作和配置持久化
 */

#ifndef VISIONFORGE_CORE_MULTI_STATION_MANAGER_H
#define VISIONFORGE_CORE_MULTI_STATION_MANAGER_H

#include "core/StationConfig.h"
#include <QObject>
#include <QMap>
#include <memory>

namespace VisionForge {
namespace Core {

/**
 * @brief 多工位管理器 (单例)
 *
 * 功能：
 * - 管理多个工位配置
 * - 支持工位切换
 * - 配置持久化（JSON文件）
 * - 与现有模块同步（WorkStation、CameraManager）
 */
class MultiStationManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static MultiStationManager& instance();

    /**
     * @brief 最大工位数
     */
    static constexpr int MAX_STATION_NUM = 6;

    // ========== 工位管理 ==========

    /**
     * @brief 创建新工位
     * @param name 工位名称
     * @param type 平台类型
     * @return 新创建的工位ID，失败返回空字符串
     */
    QString createStation(const QString& name,
                          Platform::PlatformType type = Platform::PlatformType::XYD);

    /**
     * @brief 删除工位
     * @param stationId 工位ID
     * @return 是否成功
     */
    bool removeStation(const QString& stationId);

    /**
     * @brief 获取工位配置（通过ID）
     */
    StationConfig* getStation(const QString& stationId);
    const StationConfig* getStation(const QString& stationId) const;

    /**
     * @brief 获取工位配置（通过索引）
     */
    StationConfig* getStationByIndex(int index);
    const StationConfig* getStationByIndex(int index) const;

    /**
     * @brief 获取所有工位
     */
    QVector<StationConfig*> getAllStations();
    QVector<const StationConfig*> getAllStations() const;

    /**
     * @brief 获取工位数量
     */
    int getStationCount() const;

    /**
     * @brief 检查工位是否存在
     */
    bool hasStation(const QString& stationId) const;

    // ========== 当前工位 ==========

    /**
     * @brief 设置当前工位（通过ID）
     */
    bool setCurrentStation(const QString& stationId);

    /**
     * @brief 设置当前工位（通过索引）
     */
    bool setCurrentStationByIndex(int index);

    /**
     * @brief 获取当前工位
     */
    StationConfig* currentStation();
    const StationConfig* currentStation() const;

    /**
     * @brief 获取当前工位ID
     */
    QString currentStationId() const;

    /**
     * @brief 获取当前工位索引
     */
    int currentStationIndex() const;

    // ========== 位置-相机绑定 ==========

    /**
     * @brief 为工位添加位置绑定
     */
    bool addPositionBinding(const QString& stationId, const PositionCameraBinding& binding);

    /**
     * @brief 更新工位的位置绑定
     */
    bool updatePositionBinding(const QString& stationId, const PositionCameraBinding& binding);

    /**
     * @brief 移除工位的位置绑定
     */
    bool removePositionBinding(const QString& stationId, const QString& positionId);

    /**
     * @brief 获取位置绑定
     */
    PositionCameraBinding* getPositionBinding(const QString& stationId, const QString& positionId);

    // ========== 相机共享 ==========

    /**
     * @brief 设置相机共享
     * @param targetStationId 目标工位ID（使用共享相机）
     * @param sourceStationId 源工位ID（提供相机）
     */
    bool setSharedCamera(const QString& targetStationId, const QString& sourceStationId);

    /**
     * @brief 清除相机共享
     */
    bool clearSharedCamera(const QString& stationId);

    // ========== 配置持久化 ==========

    /**
     * @brief 保存所有配置
     * @param configDir 配置目录，为空则使用默认目录
     */
    bool saveConfig(const QString& configDir = QString());

    /**
     * @brief 加载配置
     * @param configDir 配置目录，为空则使用默认目录
     */
    bool loadConfig(const QString& configDir = QString());

    /**
     * @brief 获取配置目录
     */
    QString getConfigDirectory() const;

    /**
     * @brief 设置配置目录
     */
    void setConfigDirectory(const QString& dir);

    // ========== 与现有模块同步 ==========

    /**
     * @brief 同步到 WorkStation 模块
     */
    void syncToWorkStation(const QString& stationId);

    /**
     * @brief 同步到 CameraManager 模块
     */
    void syncToCameraManager(const QString& stationId);

    /**
     * @brief 同步所有工位
     */
    void syncAll();

    // ========== 快速配置 ==========

    /**
     * @brief 创建默认配置（用于新项目）
     * @param stationCount 工位数量
     * @param camerasPerStation 每工位相机数
     * @param positionsPerStation 每工位位置数
     */
    void createDefaultConfig(int stationCount = 1,
                             int camerasPerStation = 2,
                             int positionsPerStation = 4);

    /**
     * @brief 清除所有配置
     */
    void clearAll();

signals:
    /**
     * @brief 工位创建信号
     */
    void stationCreated(const QString& stationId);

    /**
     * @brief 工位删除信号
     */
    void stationRemoved(const QString& stationId);

    /**
     * @brief 当前工位切换信号
     */
    void currentStationChanged(const QString& stationId, int index);

    /**
     * @brief 工位配置变更信号
     */
    void stationConfigChanged(const QString& stationId);

    /**
     * @brief 位置绑定变更信号
     */
    void positionBindingChanged(const QString& stationId, const QString& positionId);

    /**
     * @brief 配置加载完成信号
     */
    void configLoaded();

    /**
     * @brief 配置保存完成信号
     */
    void configSaved();

private:
    /**
     * @brief 私有构造函数（单例模式）
     */
    explicit MultiStationManager(QObject* parent = nullptr);

    /**
     * @brief 禁止拷贝
     */
    MultiStationManager(const MultiStationManager&) = delete;
    MultiStationManager& operator=(const MultiStationManager&) = delete;

    /**
     * @brief 生成唯一的工位ID
     */
    QString generateStationId() const;

    /**
     * @brief 保存工位索引文件
     */
    bool saveStationsIndex(const QString& configDir) const;

    /**
     * @brief 加载工位索引文件
     */
    bool loadStationsIndex(const QString& configDir);

    // ========== 成员变量 ==========
    QMap<QString, StationConfig> stations_;     ///< 所有工位配置
    QString currentStationId_;                   ///< 当前工位ID
    QString configDirectory_;                    ///< 配置目录
};

} // namespace Core
} // namespace VisionForge

#endif // VISIONFORGE_CORE_MULTI_STATION_MANAGER_H
