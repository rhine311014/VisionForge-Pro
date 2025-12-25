/**
 * @file PositionToolChainManager.h
 * @brief 位置级工具链管理器
 *
 * 管理多工位多位置的独立工具链，每个位置拥有独立的检测工具链配置
 */

#ifndef VISIONFORGE_CORE_POSITION_TOOLCHAIN_MANAGER_H
#define VISIONFORGE_CORE_POSITION_TOOLCHAIN_MANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QString>
#include <memory>

namespace VisionForge {

namespace Algorithm {
class VisionTool;
}

namespace Core {

struct StationConfig;
struct PositionCameraBinding;

/**
 * @brief 位置工具链数据
 */
struct PositionToolChain {
    QString stationId;          ///< 工位ID
    QString positionId;         ///< 位置ID
    QString positionName;       ///< 位置名称
    QList<Algorithm::VisionTool*> tools; ///< 工具列表
    bool modified = false;      ///< 是否已修改
    QString configFile;         ///< 配置文件路径

    /**
     * @brief 获取唯一键
     */
    QString key() const { return stationId + "_" + positionId; }
};

/**
 * @brief 位置级工具链管理器
 *
 * 功能：
 * - 为每个位置（工位+位置）管理独立的工具链
 * - 切换位置时自动加载/卸载工具链
 * - 保存/加载工具链配置到文件
 * - 支持工具链的复制、清空等操作
 */
class PositionToolChainManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static PositionToolChainManager& instance();

    /**
     * @brief 析构函数
     */
    ~PositionToolChainManager();

    // ========== 当前位置管理 ==========

    /**
     * @brief 设置当前位置
     * @param stationId 工位ID
     * @param positionId 位置ID
     * @return 是否成功
     */
    bool setCurrentPosition(const QString& stationId, const QString& positionId);

    /**
     * @brief 获取当前工位ID
     */
    QString currentStationId() const { return currentStationId_; }

    /**
     * @brief 获取当前位置ID
     */
    QString currentPositionId() const { return currentPositionId_; }

    /**
     * @brief 获取当前位置的唯一键
     */
    QString currentKey() const;

    // ========== 工具链访问 ==========

    /**
     * @brief 获取当前位置的工具链
     */
    PositionToolChain* currentToolChain();

    /**
     * @brief 获取指定位置的工具链
     */
    PositionToolChain* getToolChain(const QString& stationId, const QString& positionId);

    /**
     * @brief 获取当前位置的工具列表
     */
    QList<Algorithm::VisionTool*> currentTools();

    /**
     * @brief 获取指定位置的工具列表
     */
    QList<Algorithm::VisionTool*> getTools(const QString& stationId, const QString& positionId);

    // ========== 工具操作 ==========

    /**
     * @brief 添加工具到当前位置
     */
    void addTool(Algorithm::VisionTool* tool);

    /**
     * @brief 移除当前位置的工具
     */
    void removeTool(Algorithm::VisionTool* tool);

    /**
     * @brief 清空当前位置的工具链
     */
    void clearCurrentToolChain();

    /**
     * @brief 设置当前位置的工具列表
     */
    void setCurrentTools(const QList<Algorithm::VisionTool*>& tools);

    // ========== 工位初始化 ==========

    /**
     * @brief 初始化工位的所有位置工具链
     * @param station 工位配置
     */
    void initializeStation(StationConfig* station);

    /**
     * @brief 清理工位的所有位置工具链
     */
    void cleanupStation(const QString& stationId);

    // ========== 保存/加载 ==========

    /**
     * @brief 保存当前位置的工具链
     */
    bool saveCurrentToolChain();

    /**
     * @brief 加载指定位置的工具链
     */
    bool loadToolChain(const QString& stationId, const QString& positionId);

    /**
     * @brief 保存所有工具链
     */
    bool saveAll();

    /**
     * @brief 加载工位的所有工具链
     */
    bool loadStationToolChains(const QString& stationId);

    // ========== 辅助功能 ==========

    /**
     * @brief 复制工具链到另一个位置
     */
    bool copyToolChain(const QString& srcStationId, const QString& srcPositionId,
                       const QString& dstStationId, const QString& dstPositionId);

    /**
     * @brief 检查位置是否有工具链
     */
    bool hasToolChain(const QString& stationId, const QString& positionId) const;

    /**
     * @brief 获取所有位置的键列表
     */
    QStringList allPositionKeys() const;

    /**
     * @brief 标记当前工具链已修改
     */
    void markCurrentAsModified();

signals:
    /**
     * @brief 当前位置改变
     */
    void currentPositionChanged(const QString& stationId, const QString& positionId);

    /**
     * @brief 工具链改变（工具添加/删除/修改）
     */
    void toolChainChanged(const QString& stationId, const QString& positionId);

    /**
     * @brief 工具链加载完成
     */
    void toolChainLoaded(const QString& stationId, const QString& positionId);

    /**
     * @brief 工具链保存完成
     */
    void toolChainSaved(const QString& stationId, const QString& positionId);

private:
    /**
     * @brief 私有构造函数（单例模式）
     */
    PositionToolChainManager();

    /**
     * @brief 获取或创建工具链
     */
    PositionToolChain* getOrCreateToolChain(const QString& stationId, const QString& positionId);

    /**
     * @brief 生成工具链配置文件路径
     */
    QString getToolChainFilePath(const QString& stationId, const QString& positionId) const;

    /**
     * @brief 清理工具链中的工具（释放内存）
     */
    void cleanupTools(PositionToolChain* chain);

    // ========== 成员变量 ==========
    QMap<QString, PositionToolChain*> toolChains_;  ///< 所有工具链 (key -> chain)
    QString currentStationId_;                       ///< 当前工位ID
    QString currentPositionId_;                      ///< 当前位置ID
    QString configBaseDir_;                          ///< 配置文件基目录
};

} // namespace Core
} // namespace VisionForge

#endif // VISIONFORGE_CORE_POSITION_TOOLCHAIN_MANAGER_H
