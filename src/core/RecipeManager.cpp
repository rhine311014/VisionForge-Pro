/**
 * @file RecipeManager.cpp
 * @brief 方案管理器核心实现文件
 * @details 本文件实现了VisionForge视觉检测系统的方案统一管理功能，包括：
 *          - 方案的创建、加载、保存、删除等生命周期管理
 *          - 当前方案的切换与自动恢复
 *          - 方案库的扫描与批量加载
 *          - 方案搜索（按名称、标签、产品编号）
 *          - 方案导入导出与复制
 *          - 最近使用方案的记录
 *          - 方案模型的预加载
 *          采用单例模式确保全局唯一的方案管理实例
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-01-02
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 */

#include "core/RecipeManager.h"
#include "base/Logger.h"
#include "base/ConfigManager.h"
#ifdef USE_HALCON
#include "algorithm/ShapeModelManager.h"
#endif
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>

// 配置键名 - 用于保存最后使用的方案名称
static const QString CONFIG_KEY_LAST_RECIPE = "Recipe/LastUsedRecipe";

namespace VisionForge {
namespace Core {

/**
 * @brief 构造函数 - 初始化方案管理器
 * @details 设置默认方案目录，初始化模型预加载器
 */
RecipeManager::RecipeManager()
    : currentRecipe_(nullptr)
    , maxRecentRecipes_(10)    // 最多记录10个最近使用的方案
    , preloader_(nullptr)
{
    // 默认方案目录设置在程序运行目录下的recipes子目录
    recipeDirectory_ = QDir::currentPath() + "/recipes";

    // 初始化模型预加载器，用于方案切换时预热模型
    preloader_ = new ModelPreloader(this);

    // 连接预加载器信号到管理器信号
    connect(preloader_, &ModelPreloader::started,
            this, &RecipeManager::modelPreloadStarted);
    connect(preloader_, &ModelPreloader::progressChanged,
            this, &RecipeManager::modelPreloadProgress);
    connect(preloader_, &ModelPreloader::finished,
            this, [this](int success, int failure, qint64) {
                emit modelPreloadFinished(success, failure);
            });
}

/**
 * @brief 析构函数 - 清理管理器资源
 * @details 释放所有加载的方案对象
 */
RecipeManager::~RecipeManager()
{
    clearAll();
}

/**
 * @brief 获取方案管理器单例实例
 * @return 管理器单例引用
 */
RecipeManager& RecipeManager::instance()
{
    static RecipeManager instance;
    return instance;
}

/**
 * @brief 设置方案目录路径
 * @param path 方案目录路径
 * @details 如果目录不存在会自动创建
 */
void RecipeManager::setRecipeDirectory(const QString& path)
{
    recipeDirectory_ = path;

    // 确保目录存在
    QDir dir;
    if (!dir.exists(path)) {
        dir.mkpath(path);
    }

    LOG_INFO(QString("设置方案目录: %1").arg(path));
}

// ========== 方案管理 ==========

/**
 * @brief 创建新方案
 * @param name 方案名称
 * @return 创建的方案对象指针，已存在同名方案则返回已有的
 */
Recipe* RecipeManager::createRecipe(const QString& name)
{
    // 检查是否已存在同名方案
    if (recipes_.contains(name)) {
        LOG_WARNING(QString("方案已存在: %1").arg(name));
        return recipes_[name];
    }

    // 创建新方案
    Recipe* recipe = new Recipe(this);
    recipe->setName(name);

    recipes_[name] = recipe;

    LOG_INFO(QString("创建新方案: %1").arg(name));

    emit recipeListChanged();
    return recipe;
}

/**
 * @brief 从文件加载方案
 * @param filePath 方案文件路径
 * @return 加载的方案对象指针，失败返回nullptr
 * @details 如果已存在同名方案，会先移除旧方案
 */
Recipe* RecipeManager::loadRecipe(const QString& filePath)
{
    // 创建临时方案对象用于加载
    Recipe* recipe = new Recipe(this);

    // 尝试加载方案文件
    if (!recipe->loadFromFile(filePath)) {
        delete recipe;
        return nullptr;
    }

    QString name = recipe->name();

    // 如果已存在同名方案，先移除
    if (recipes_.contains(name)) {
        delete recipes_.take(name);
    }

    // 添加到方案集合
    recipes_[name] = recipe;
    addToRecentRecipes(name);  // 记录到最近使用

    LOG_INFO(QString("加载方案: %1 <- %2").arg(name).arg(filePath));

    emit recipeLoaded(name);
    emit recipeListChanged();

    return recipe;
}

/**
 * @brief 保存方案到文件
 * @param recipe 要保存的方案对象
 * @param filePath 目标文件路径，为空则使用默认路径
 * @return 保存成功返回true，失败返回false
 */
bool RecipeManager::saveRecipe(Recipe* recipe, const QString& filePath)
{
    if (!recipe) {
        LOG_ERROR("保存方案失败：方案为空");
        return false;
    }

    // 确定保存路径
    QString savePath = filePath;
    if (savePath.isEmpty()) {
        savePath = generateFilePath(recipe->name());
    }

    // 执行保存
    if (!recipe->saveToFile(savePath)) {
        return false;
    }

    addToRecentRecipes(recipe->name());

    emit recipeSaved(recipe->name());
    return true;
}

/**
 * @brief 移除方案
 * @param name 方案名称
 * @param deleteFile 是否同时删除方案文件和目录
 * @return 移除成功返回true，方案不存在返回false
 * @details 如果deleteFile为true，会递归删除整个方案目录
 */
bool RecipeManager::removeRecipe(const QString& name, bool deleteFile)
{
    if (!recipes_.contains(name)) {
        LOG_WARNING(QString("方案不存在: %1").arg(name));
        return false;
    }

    Recipe* recipe = recipes_.take(name);

    // 如果是当前方案，清除引用
    if (currentRecipe_ == recipe) {
        currentRecipe_ = nullptr;
        emit currentRecipeChanged(QString());
    }

    // 删除文件和目录
    if (deleteFile && !recipe->filePath().isEmpty()) {
        QString recipeDir = recipe->recipeDirectory();
        if (!recipeDir.isEmpty()) {
            // 删除整个方案目录（包含模板库）
            QDir dir(recipeDir);
            if (dir.exists()) {
                dir.removeRecursively();
                LOG_INFO(QString("删除方案目录: %1").arg(recipeDir));
            }
        } else {
            // 仅删除方案文件
            QFile::remove(recipe->filePath());
            LOG_INFO(QString("删除方案文件: %1").arg(recipe->filePath()));
        }
    }

    // 从最近使用列表中移除
    recentRecipes_.removeAll(name);

    delete recipe;

    LOG_INFO(QString("移除方案: %1").arg(name));

    emit recipeRemoved(name);
    emit recipeListChanged();

    return true;
}

/**
 * @brief 按名称获取方案
 * @param name 方案名称
 * @return 方案对象指针，不存在返回nullptr
 */
Recipe* RecipeManager::getRecipe(const QString& name)
{
    return recipes_.value(name, nullptr);
}

/**
 * @brief 获取所有方案列表
 * @return 方案对象指针列表
 */
QList<Recipe*> RecipeManager::getAllRecipes() const
{
    return recipes_.values();
}

/**
 * @brief 获取所有方案名称列表
 * @return 方案名称字符串列表
 */
QStringList RecipeManager::getAllRecipeNames() const
{
    return recipes_.keys();
}

/**
 * @brief 检查方案是否存在
 * @param name 方案名称
 * @return 存在返回true，否则返回false
 */
bool RecipeManager::hasRecipe(const QString& name) const
{
    return recipes_.contains(name);
}

// ========== 当前方案管理 ==========

/**
 * @brief 设置当前方案
 * @param name 方案名称，为空则清除当前方案
 * @return 设置成功返回true，方案不存在返回false
 * @details 切换方案时会自动：
 *          1. 保存最后使用的方案名称
 *          2. 切换到对应方案的模板库
 */
bool RecipeManager::setCurrentRecipe(const QString& name)
{
    // 空名称表示清除当前方案
    if (name.isEmpty()) {
        currentRecipe_ = nullptr;
        emit currentRecipeChanged(QString());
        return true;
    }

    // 检查方案是否存在
    if (!recipes_.contains(name)) {
        LOG_ERROR(QString("设置当前方案失败：方案不存在: %1").arg(name));
        return false;
    }

    currentRecipe_ = recipes_[name];
    addToRecentRecipes(name);

    // 保存最后使用的方案名称（用于下次启动时自动加载）
    saveLastRecipeName();

    // 切换模板库到当前方案的模板库目录
#ifdef USE_HALCON
    QString modelLibPath = currentRecipe_->modelLibraryPath();
    if (!modelLibPath.isEmpty()) {
        // 确保模板库目录存在
        currentRecipe_->ensureDirectoryStructure();

        // 加载对应方案的模板库
        Algorithm::ShapeModelManager& modelMgr = Algorithm::ShapeModelManager::instance();
        modelMgr.loadLibrary(modelLibPath);
        LOG_INFO(QString("切换模板库: %1").arg(modelLibPath));
    }
#endif

    LOG_INFO(QString("设置当前方案: %1").arg(name));

    emit currentRecipeChanged(name);
    return true;
}

/**
 * @brief 获取当前方案名称
 * @return 当前方案名称，无当前方案返回空字符串
 */
QString RecipeManager::currentRecipeName() const
{
    return currentRecipe_ ? currentRecipe_->name() : QString();
}

// ========== 搜索功能 ==========

/**
 * @brief 按名称搜索方案
 * @param keyword 搜索关键词
 * @return 名称包含关键词的方案列表
 * @details 搜索不区分大小写
 */
QList<Recipe*> RecipeManager::searchByName(const QString& keyword) const
{
    QList<Recipe*> result;

    QString lowerKeyword = keyword.toLower();
    for (Recipe* recipe : recipes_) {
        if (recipe->name().toLower().contains(lowerKeyword)) {
            result.append(recipe);
        }
    }

    return result;
}

/**
 * @brief 按标签搜索方案
 * @param tag 标签名称
 * @return 包含指定标签的方案列表
 */
QList<Recipe*> RecipeManager::searchByTag(const QString& tag) const
{
    QList<Recipe*> result;

    for (Recipe* recipe : recipes_) {
        if (recipe->tags().contains(tag, Qt::CaseInsensitive)) {
            result.append(recipe);
        }
    }

    return result;
}

/**
 * @brief 按产品编号搜索方案
 * @param productCode 产品编号
 * @return 产品编号完全匹配的方案列表
 */
QList<Recipe*> RecipeManager::searchByProductCode(const QString& productCode) const
{
    QList<Recipe*> result;

    for (Recipe* recipe : recipes_) {
        if (recipe->productCode() == productCode) {
            result.append(recipe);
        }
    }

    return result;
}

/**
 * @brief 获取所有标签列表
 * @return 去重后的所有方案标签列表
 */
QStringList RecipeManager::getAllTags() const
{
    QSet<QString> tags;

    for (Recipe* recipe : recipes_) {
        for (const QString& tag : recipe->tags()) {
            tags.insert(tag);
        }
    }

    return QStringList(tags.begin(), tags.end());
}

/**
 * @brief 获取所有产品编号列表
 * @return 去重后的所有方案产品编号列表
 */
QStringList RecipeManager::getAllProductCodes() const
{
    QSet<QString> codes;

    for (Recipe* recipe : recipes_) {
        if (!recipe->productCode().isEmpty()) {
            codes.insert(recipe->productCode());
        }
    }

    return QStringList(codes.begin(), codes.end());
}

// ========== 方案库操作 ==========

/**
 * @brief 扫描方案目录并加载所有方案
 * @return 成功加载的方案数量
 * @details 递归扫描目录下所有.vfr方案文件
 */
int RecipeManager::scanRecipeDirectory()
{
    if (recipeDirectory_.isEmpty()) {
        LOG_WARNING("方案目录未设置");
        return 0;
    }

    QDir dir(recipeDirectory_);
    if (!dir.exists()) {
        LOG_WARNING(QString("方案目录不存在: %1").arg(recipeDirectory_));
        return 0;
    }

    int loadedCount = 0;

    // 扫描所有.vfr文件（递归包含子目录）
    QDirIterator it(recipeDirectory_, QStringList() << "*.vfr",
                    QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        if (loadRecipe(filePath)) {
            loadedCount++;
        }
    }

    LOG_INFO(QString("扫描方案目录完成，加载 %1 个方案").arg(loadedCount));

    return loadedCount;
}

/**
 * @brief 清空所有方案
 * @details 释放所有方案对象内存，但不删除文件
 */
void RecipeManager::clearAll()
{
    // 清除当前方案引用
    currentRecipe_ = nullptr;

    // 删除所有方案对象
    for (Recipe* recipe : recipes_) {
        delete recipe;
    }
    recipes_.clear();

    LOG_INFO("清除所有方案");

    emit currentRecipeChanged(QString());
    emit recipeListChanged();
}

/**
 * @brief 导入方案文件
 * @param filePath 要导入的方案文件路径
 * @return 导入的方案对象指针，失败返回nullptr
 * @details 导入后会将方案文件复制到方案目录
 */
Recipe* RecipeManager::importRecipe(const QString& filePath)
{
    // 先加载方案
    Recipe* recipe = loadRecipe(filePath);
    if (!recipe) {
        return nullptr;
    }

    // 复制文件到方案目录
    QString targetPath = generateFilePath(recipe->name());
    if (filePath != targetPath) {
        QFile::copy(filePath, targetPath);
        recipe->saveToFile(targetPath);  // 更新文件路径
    }

    LOG_INFO(QString("导入方案: %1").arg(recipe->name()));

    return recipe;
}

/**
 * @brief 导出方案到指定文件
 * @param name 方案名称
 * @param targetPath 导出目标路径
 * @return 导出成功返回true，失败返回false
 */
bool RecipeManager::exportRecipe(const QString& name, const QString& targetPath)
{
    Recipe* recipe = getRecipe(name);
    if (!recipe) {
        LOG_ERROR(QString("导出方案失败：方案不存在: %1").arg(name));
        return false;
    }

    if (!recipe->saveToFile(targetPath)) {
        return false;
    }

    LOG_INFO(QString("导出方案: %1 -> %2").arg(name).arg(targetPath));

    return true;
}

/**
 * @brief 复制方案
 * @param sourceName 源方案名称
 * @param newName 新方案名称
 * @return 复制后的新方案对象指针，失败返回nullptr
 * @details 复制包括：
 *          1. 方案配置信息
 *          2. 工具链配置
 *          3. 模板库目录中的所有文件
 */
Recipe* RecipeManager::duplicateRecipe(const QString& sourceName, const QString& newName)
{
    Recipe* source = getRecipe(sourceName);
    if (!source) {
        LOG_ERROR(QString("复制方案失败：源方案不存在: %1").arg(sourceName));
        return nullptr;
    }

    if (recipes_.contains(newName)) {
        LOG_ERROR(QString("复制方案失败：目标名称已存在: %1").arg(newName));
        return nullptr;
    }

    // 创建新方案
    Recipe* newRecipe = createRecipe(newName);
    if (!newRecipe) {
        return nullptr;
    }

    // 复制方案内容
    QJsonObject json = source->serialize();
    newRecipe->deserialize(json);

    // 更新方案信息
    newRecipe->setName(newName);
    newRecipe->info().createTime = QDateTime::currentMSecsSinceEpoch();
    newRecipe->info().modifyTime = newRecipe->info().createTime;

    // 保存新方案以生成文件路径
    QString newFilePath = generateFilePath(newName);
    newRecipe->saveToFile(newFilePath);

    // 复制模板库目录
    QString srcModelsDir = source->modelLibraryPath();
    QString dstModelsDir = newRecipe->modelLibraryPath();

    if (!srcModelsDir.isEmpty() && !dstModelsDir.isEmpty()) {
        QDir srcDir(srcModelsDir);
        QDir dstDir(dstModelsDir);

        // 确保目标目录存在
        if (!dstDir.exists()) {
            dstDir.mkpath(".");
        }

        // 复制所有文件
        if (srcDir.exists()) {
            QStringList files = srcDir.entryList(QDir::Files);
            for (const QString& fileName : files) {
                QString srcPath = srcDir.filePath(fileName);
                QString dstPath = dstDir.filePath(fileName);
                QFile::copy(srcPath, dstPath);
            }
            LOG_INFO(QString("复制模板库: %1 -> %2 (%3 个文件)")
                .arg(srcModelsDir).arg(dstModelsDir).arg(files.size()));
        }
    }

    LOG_INFO(QString("复制方案: %1 -> %2").arg(sourceName).arg(newName));

    return newRecipe;
}

// ========== 最近使用管理 ==========

/**
 * @brief 设置最大最近使用记录数
 * @param max 最大记录数（至少为1）
 */
void RecipeManager::setMaxRecentRecipes(int max)
{
    maxRecentRecipes_ = qMax(1, max);

    // 截断超出的记录
    while (recentRecipes_.size() > maxRecentRecipes_) {
        recentRecipes_.removeLast();
    }
}

/**
 * @brief 清空最近使用记录
 */
void RecipeManager::clearRecentRecipes()
{
    recentRecipes_.clear();
}

// ========== 私有方法 ==========

/**
 * @brief 添加方案到最近使用列表
 * @param name 方案名称
 * @details 新添加的方案放到列表头部，自动移除重复和超出的记录
 */
void RecipeManager::addToRecentRecipes(const QString& name)
{
    // 移除已存在的相同名称
    recentRecipes_.removeAll(name);

    // 添加到开头
    recentRecipes_.prepend(name);

    // 截断超出的记录
    while (recentRecipes_.size() > maxRecentRecipes_) {
        recentRecipes_.removeLast();
    }
}

/**
 * @brief 生成方案文件路径
 * @param name 方案名称
 * @return 方案文件的完整路径
 * @details 目录结构: recipes/{方案名}/{方案名}.vfr
 *          同时自动创建models子目录用于存放模板库
 */
QString RecipeManager::generateFilePath(const QString& name) const
{
    // 清理文件名（移除非法字符）
    QString safeName = name;
    safeName.replace(QRegularExpression("[/\\\\:*?\"<>|]"), "_");

    // 每个方案使用独立的子目录
    QString recipeDir = QDir(recipeDirectory_).filePath(safeName);

    // 确保方案目录存在
    QDir dir;
    if (!dir.exists(recipeDir)) {
        dir.mkpath(recipeDir);
    }

    // 确保模板库目录存在
    QString modelsDir = QDir(recipeDir).filePath("models");
    if (!dir.exists(modelsDir)) {
        dir.mkpath(modelsDir);
    }

    return QDir(recipeDir).filePath(safeName + ".vfr");
}

// ========== 初始化和自动加载 ==========

/**
 * @brief 初始化方案管理器
 * @return 是否成功加载了上次使用的方案
 * @details 扫描方案目录并尝试加载上次使用的方案
 */
bool RecipeManager::initialize()
{
    // 扫描方案目录
    int count = scanRecipeDirectory();
    LOG_INFO(QString("方案管理器初始化完成，加载了 %1 个方案").arg(count));

    // 尝试加载上次使用的方案
    if (loadLastRecipe()) {
        LOG_INFO(QString("已自动加载上次使用的配方: %1").arg(currentRecipeName()));
        return true;
    }

    return false;
}

/**
 * @brief 保存最后使用的方案名称
 * @details 将当前方案名称保存到配置文件，用于下次启动时恢复
 */
void RecipeManager::saveLastRecipeName()
{
    QString recipeName = currentRecipeName();
    Base::ConfigManager::instance().setValue(CONFIG_KEY_LAST_RECIPE, recipeName);
    Base::ConfigManager::instance().save();
    LOG_DEBUG(QString("保存最后使用的配方: %1").arg(recipeName));
}

/**
 * @brief 加载上次使用的方案
 * @return 加载成功返回true，失败返回false
 */
bool RecipeManager::loadLastRecipe()
{
    QString lastRecipeName = getLastRecipeName();

    if (lastRecipeName.isEmpty()) {
        LOG_DEBUG("没有保存的上次配方记录");
        return false;
    }

    if (!hasRecipe(lastRecipeName)) {
        LOG_WARNING(QString("上次使用的配方不存在: %1").arg(lastRecipeName));
        return false;
    }

    return setCurrentRecipe(lastRecipeName);
}

/**
 * @brief 获取上次使用的方案名称
 * @return 方案名称，无记录返回空字符串
 */
QString RecipeManager::getLastRecipeName() const
{
    return Base::ConfigManager::instance().getValue(CONFIG_KEY_LAST_RECIPE, QString()).toString();
}

// ========== 模型预加载 ==========

/**
 * @brief 预加载当前方案的模型
 * @param parallel 是否并行加载
 * @return 需要预加载的模型数量
 * @details 预加载可以加速方案切换后的首次检测，
 *          避免模型加载导致的延迟
 */
int RecipeManager::preloadCurrentRecipeModels(bool parallel)
{
    if (!currentRecipe_) {
        LOG_WARNING("预加载失败：当前没有加载配方");
        return 0;
    }

    ToolChain* toolChain = currentRecipe_->toolChain();
    if (!toolChain) {
        LOG_WARNING("预加载失败：配方工具链为空");
        return 0;
    }

    LOG_INFO(QString("开始预加载配方 '%1' 的模型...").arg(currentRecipe_->name()));

    return preloader_->preloadModels(toolChain, parallel);
}

} // namespace Core
} // namespace VisionForge
