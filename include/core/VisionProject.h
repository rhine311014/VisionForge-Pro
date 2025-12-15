/**
 * @file VisionProject.h
 * @brief 视觉项目管理
 * @details 管理整个视觉检测项目，包括工具链、配置、图像等
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "core/ToolChain.h"
#include "hal/ICamera.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <memory>

namespace VisionForge {
namespace Core {

/**
 * @brief 项目配置
 */
struct ProjectConfig {
    QString projectName;                // 项目名称
    QString projectPath;                // 项目路径
    QString description;                // 项目描述
    QString author;                     // 作者
    qint64 createTime;                  // 创建时间
    qint64 modifyTime;                  // 修改时间
    QString version;                    // 版本号

    // 相机配置
    QString cameraType;                 // 相机类型
    QJsonObject cameraParams;           // 相机参数

    // 其他配置
    bool autoSaveResults;               // 自动保存结果
    QString resultSavePath;             // 结果保存路径
    int maxHistoryCount;                // 最大历史记录数

    ProjectConfig()
        : createTime(0)
        , modifyTime(0)
        , version("1.0.0")
        , autoSaveResults(false)
        , maxHistoryCount(100)
    {}
};

/**
 * @brief 视觉项目类
 *
 * 管理一个完整的视觉检测项目
 */
class VisionProject : public QObject {
    Q_OBJECT

public:
    explicit VisionProject(QObject* parent = nullptr);
    ~VisionProject();

    // ========== 项目管理 ==========

    /**
     * @brief 创建新项目
     * @param projectPath 项目路径
     * @param projectName 项目名称
     * @return true如果成功
     */
    bool createNew(const QString& projectPath, const QString& projectName);

    /**
     * @brief 打开项目
     * @param projectFilePath 项目文件路径（.vfp）
     * @return true如果成功
     */
    bool open(const QString& projectFilePath);

    /**
     * @brief 保存项目
     * @return true如果成功
     */
    bool save();

    /**
     * @brief 另存为
     * @param projectFilePath 新项目文件路径
     * @return true如果成功
     */
    bool saveAs(const QString& projectFilePath);

    /**
     * @brief 关闭项目
     */
    void close();

    /**
     * @brief 项目是否已打开
     */
    bool isOpened() const { return isOpened_; }

    /**
     * @brief 项目是否已修改
     */
    bool isModified() const { return isModified_; }

    /**
     * @brief 设置修改标志
     */
    void setModified(bool modified);

    // ========== 工具链管理 ==========

    /**
     * @brief 获取主工具链
     */
    ToolChain* mainToolChain() { return mainToolChain_; }

    /**
     * @brief 获取主工具链（const版本）
     */
    const ToolChain* mainToolChain() const { return mainToolChain_; }

    /**
     * @brief 添加子工具链
     * @param name 工具链名称
     * @return 工具链指针
     */
    ToolChain* addSubToolChain(const QString& name);

    /**
     * @brief 移除子工具链
     * @param name 工具链名称
     * @return true如果成功
     */
    bool removeSubToolChain(const QString& name);

    /**
     * @brief 获取子工具链
     */
    ToolChain* getSubToolChain(const QString& name);

    /**
     * @brief 获取所有子工具链名称
     */
    QStringList getSubToolChainNames() const;

    // ========== 相机管理 ==========

    /**
     * @brief 设置相机
     * @param camera 相机实例（项目拥有所有权）
     */
    void setCamera(HAL::ICamera* camera);

    /**
     * @brief 获取相机
     */
    HAL::ICamera* camera() { return camera_; }

    /**
     * @brief 获取相机（const版本）
     */
    const HAL::ICamera* camera() const { return camera_; }

    // ========== 配置访问 ==========

    /**
     * @brief 获取项目配置
     */
    ProjectConfig& config() { return config_; }

    /**
     * @brief 获取项目配置（const版本）
     */
    const ProjectConfig& config() const { return config_; }

    /**
     * @brief 获取项目名称
     */
    QString projectName() const { return config_.projectName; }

    /**
     * @brief 获取项目路径
     */
    QString projectPath() const { return config_.projectPath; }

    /**
     * @brief 获取项目文件路径
     */
    QString projectFilePath() const { return projectFilePath_; }

    // ========== 序列化 ==========

    /**
     * @brief 序列化项目
     */
    QJsonObject serialize() const;

    /**
     * @brief 反序列化项目
     */
    bool deserialize(const QJsonObject& json);

signals:
    /**
     * @brief 项目打开信号
     */
    void projectOpened(const QString& projectPath);

    /**
     * @brief 项目关闭信号
     */
    void projectClosed();

    /**
     * @brief 项目保存信号
     */
    void projectSaved(const QString& projectPath);

    /**
     * @brief 项目修改状态改变信号
     */
    void modifiedChanged(bool modified);

    /**
     * @brief 配置改变信号
     */
    void configChanged();

private:
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath);

private:
    ProjectConfig config_;                          // 项目配置
    QString projectFilePath_;                       // 项目文件路径
    bool isOpened_;                                 // 是否已打开
    bool isModified_;                               // 是否已修改

    ToolChain* mainToolChain_;                      // 主工具链
    QMap<QString, ToolChain*> subToolChains_;       // 子工具链映射

    HAL::ICamera* camera_;                          // 相机实例
};

} // namespace Core
} // namespace VisionForge
