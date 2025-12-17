/**
 * @file RecipeManager.h
 * @brief 方案管理器
 * @details 管理多个方案的加载、保存、切换
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "core/Recipe.h"
#include <QObject>
#include <QMap>
#include <QDir>

namespace VisionForge {
namespace Core {

/**
 * @class RecipeManager
 * @brief 方案管理器
 *
 * 功能：
 * - 管理方案库（加载、保存、删除）
 * - 方案切换（应用到当前工具链）
 * - 方案搜索（按名称、标签、产品编号）
 * - 方案导入/导出
 */
class RecipeManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static RecipeManager& instance();

    /**
     * @brief 设置方案库目录
     * @param path 目录路径
     */
    void setRecipeDirectory(const QString& path);

    /**
     * @brief 获取方案库目录
     */
    QString recipeDirectory() const { return recipeDirectory_; }

    // ========== 方案管理 ==========

    /**
     * @brief 创建新方案
     * @param name 方案名称
     * @return 新创建的方案指针
     */
    Recipe* createRecipe(const QString& name);

    /**
     * @brief 从文件加载方案
     * @param filePath 文件路径
     * @return 加载的方案指针，失败返回nullptr
     */
    Recipe* loadRecipe(const QString& filePath);

    /**
     * @brief 保存方案
     * @param recipe 方案指针
     * @param filePath 文件路径（为空则使用默认路径）
     * @return true如果成功
     */
    bool saveRecipe(Recipe* recipe, const QString& filePath = QString());

    /**
     * @brief 删除方案
     * @param name 方案名称
     * @param deleteFile 是否删除文件
     * @return true如果成功
     */
    bool removeRecipe(const QString& name, bool deleteFile = false);

    /**
     * @brief 获取方案
     * @param name 方案名称
     * @return 方案指针，不存在返回nullptr
     */
    Recipe* getRecipe(const QString& name);

    /**
     * @brief 获取所有方案
     */
    QList<Recipe*> getAllRecipes() const;

    /**
     * @brief 获取所有方案名称
     */
    QStringList getAllRecipeNames() const;

    /**
     * @brief 方案是否存在
     */
    bool hasRecipe(const QString& name) const;

    /**
     * @brief 获取方案数量
     */
    int recipeCount() const { return recipes_.count(); }

    // ========== 当前方案 ==========

    /**
     * @brief 设置当前方案
     * @param name 方案名称
     * @return true如果成功
     */
    bool setCurrentRecipe(const QString& name);

    /**
     * @brief 获取当前方案
     */
    Recipe* currentRecipe() { return currentRecipe_; }
    const Recipe* currentRecipe() const { return currentRecipe_; }

    /**
     * @brief 获取当前方案名称
     */
    QString currentRecipeName() const;

    // ========== 搜索 ==========

    /**
     * @brief 按名称搜索方案
     * @param keyword 关键词（支持模糊匹配）
     */
    QList<Recipe*> searchByName(const QString& keyword) const;

    /**
     * @brief 按标签搜索方案
     * @param tag 标签
     */
    QList<Recipe*> searchByTag(const QString& tag) const;

    /**
     * @brief 按产品编号搜索方案
     * @param productCode 产品编号
     */
    QList<Recipe*> searchByProductCode(const QString& productCode) const;

    /**
     * @brief 获取所有标签
     */
    QStringList getAllTags() const;

    /**
     * @brief 获取所有产品编号
     */
    QStringList getAllProductCodes() const;

    // ========== 方案库操作 ==========

    /**
     * @brief 扫描方案目录，加载所有方案
     * @return 加载的方案数量
     */
    int scanRecipeDirectory();

    /**
     * @brief 清除所有方案（从内存）
     */
    void clearAll();

    /**
     * @brief 导入方案
     * @param filePath 外部方案文件路径
     * @return 导入的方案指针
     */
    Recipe* importRecipe(const QString& filePath);

    /**
     * @brief 导出方案
     * @param name 方案名称
     * @param targetPath 目标路径
     * @return true如果成功
     */
    bool exportRecipe(const QString& name, const QString& targetPath);

    /**
     * @brief 复制方案
     * @param sourceName 源方案名称
     * @param newName 新方案名称
     * @return 新方案指针
     */
    Recipe* duplicateRecipe(const QString& sourceName, const QString& newName);

    // ========== 最近使用 ==========

    /**
     * @brief 获取最近使用的方案名称列表
     */
    QStringList getRecentRecipes() const { return recentRecipes_; }

    /**
     * @brief 设置最近使用数量限制
     */
    void setMaxRecentRecipes(int max);

    /**
     * @brief 清除最近使用记录
     */
    void clearRecentRecipes();

signals:
    /**
     * @brief 方案加载信号
     */
    void recipeLoaded(const QString& name);

    /**
     * @brief 方案保存信号
     */
    void recipeSaved(const QString& name);

    /**
     * @brief 方案删除信号
     */
    void recipeRemoved(const QString& name);

    /**
     * @brief 当前方案改变信号
     */
    void currentRecipeChanged(const QString& name);

    /**
     * @brief 方案列表改变信号
     */
    void recipeListChanged();

private:
    RecipeManager();
    ~RecipeManager();

    // 禁止拷贝
    RecipeManager(const RecipeManager&) = delete;
    RecipeManager& operator=(const RecipeManager&) = delete;

    void addToRecentRecipes(const QString& name);
    QString generateFilePath(const QString& name) const;

private:
    QString recipeDirectory_;               ///< 方案库目录
    QMap<QString, Recipe*> recipes_;        ///< 方案映射（名称->方案）
    Recipe* currentRecipe_;                 ///< 当前方案
    QStringList recentRecipes_;             ///< 最近使用的方案
    int maxRecentRecipes_;                  ///< 最近使用数量限制
};

} // namespace Core
} // namespace VisionForge
