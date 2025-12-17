/**
 * @file Recipe.h
 * @brief 方案管理类
 * @details 方案(Recipe)是工具链配置的轻量级存储单元，
 *          允许快速切换不同产品的检测参数而无需更改整个项目
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "core/ToolChain.h"
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QDateTime>

namespace VisionForge {
namespace Core {

/**
 * @brief 方案信息结构
 */
struct RecipeInfo {
    QString name;                   ///< 方案名称
    QString description;            ///< 方案描述
    QString author;                 ///< 创建者
    qint64 createTime;              ///< 创建时间
    qint64 modifyTime;              ///< 修改时间
    QString version;                ///< 版本号
    QString productCode;            ///< 产品编号（关联产品）
    QStringList tags;               ///< 标签（用于分类/搜索）

    RecipeInfo()
        : createTime(0)
        , modifyTime(0)
        , version("1.0.0")
    {}
};

/**
 * @class Recipe
 * @brief 方案类
 *
 * 方案是工具链配置的存储单元，包含：
 * - 方案元信息（名称、描述、版本等）
 * - 工具链配置（工具序列及其参数）
 * - 自定义参数（产品相关的额外配置）
 *
 * 方案可以独立于项目保存/加载，方便：
 * - 在同一项目中切换不同产品的检测参数
 * - 在不同项目间复用工具链配置
 * - 导出/导入检测方案
 */
class Recipe : public QObject {
    Q_OBJECT

public:
    explicit Recipe(QObject* parent = nullptr);
    ~Recipe();

    // ========== 基本信息 ==========

    /**
     * @brief 获取方案信息
     */
    RecipeInfo& info() { return info_; }
    const RecipeInfo& info() const { return info_; }

    /**
     * @brief 设置方案名称
     */
    void setName(const QString& name);
    QString name() const { return info_.name; }

    /**
     * @brief 设置方案描述
     */
    void setDescription(const QString& description);
    QString description() const { return info_.description; }

    /**
     * @brief 设置产品编号
     */
    void setProductCode(const QString& code);
    QString productCode() const { return info_.productCode; }

    /**
     * @brief 设置标签
     */
    void setTags(const QStringList& tags);
    QStringList tags() const { return info_.tags; }
    void addTag(const QString& tag);
    void removeTag(const QString& tag);

    // ========== 工具链管理 ==========

    /**
     * @brief 获取工具链
     */
    ToolChain* toolChain() { return toolChain_; }
    const ToolChain* toolChain() const { return toolChain_; }

    /**
     * @brief 从现有工具链复制配置
     * @param source 源工具链
     * @return true如果成功
     */
    bool copyFromToolChain(const ToolChain* source);

    /**
     * @brief 应用到目标工具链
     * @param target 目标工具链
     * @return true如果成功
     */
    bool applyToToolChain(ToolChain* target) const;

    // ========== 自定义参数 ==========

    /**
     * @brief 设置自定义参数
     * @param key 参数键
     * @param value 参数值
     */
    void setCustomParam(const QString& key, const QVariant& value);

    /**
     * @brief 获取自定义参数
     * @param key 参数键
     * @param defaultValue 默认值
     */
    QVariant customParam(const QString& key, const QVariant& defaultValue = QVariant()) const;

    /**
     * @brief 移除自定义参数
     */
    void removeCustomParam(const QString& key);

    /**
     * @brief 获取所有自定义参数键
     */
    QStringList customParamKeys() const;

    /**
     * @brief 清除所有自定义参数
     */
    void clearCustomParams();

    // ========== 文件操作 ==========

    /**
     * @brief 保存方案到文件
     * @param filePath 文件路径（.vfr）
     * @return true如果成功
     */
    bool saveToFile(const QString& filePath);

    /**
     * @brief 从文件加载方案
     * @param filePath 文件路径（.vfr）
     * @return true如果成功
     */
    bool loadFromFile(const QString& filePath);

    /**
     * @brief 获取当前文件路径
     */
    QString filePath() const { return filePath_; }

    /**
     * @brief 获取方案目录路径
     * @return 方案所在目录（包含方案文件和模板库等资源）
     */
    QString recipeDirectory() const;

    /**
     * @brief 获取模板库路径
     * @return 模板库目录路径（{方案目录}/models/）
     */
    QString modelLibraryPath() const;

    /**
     * @brief 确保方案目录结构存在
     * @return true如果目录结构创建/存在成功
     */
    bool ensureDirectoryStructure() const;

    /**
     * @brief 是否已修改（未保存）
     */
    bool isModified() const { return isModified_; }

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
     * @brief 导出为可读文本（用于报告/文档）
     */
    QString exportAsText() const;

    // ========== 验证 ==========

    /**
     * @brief 验证方案有效性
     * @return 错误消息列表（空表示有效）
     */
    QStringList validate() const;

    /**
     * @brief 方案是否有效
     */
    bool isValid() const { return validate().isEmpty(); }

signals:
    /**
     * @brief 方案修改信号
     */
    void modified();

    /**
     * @brief 方案保存信号
     */
    void saved(const QString& filePath);

    /**
     * @brief 方案加载信号
     */
    void loaded(const QString& filePath);

    /**
     * @brief 信息变更信号
     */
    void infoChanged();

private:
    void setModified(bool modified);

private:
    RecipeInfo info_;                       ///< 方案信息
    ToolChain* toolChain_;                  ///< 工具链
    QVariantMap customParams_;              ///< 自定义参数
    QString filePath_;                      ///< 当前文件路径
    bool isModified_;                       ///< 是否已修改
};

} // namespace Core
} // namespace VisionForge
