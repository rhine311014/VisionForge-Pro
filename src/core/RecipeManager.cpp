/**
 * @file RecipeManager.cpp
 * @brief 方案管理器实现
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

// 配置键名
static const QString CONFIG_KEY_LAST_RECIPE = "Recipe/LastUsedRecipe";

namespace VisionForge {
namespace Core {

RecipeManager::RecipeManager()
    : currentRecipe_(nullptr)
    , maxRecentRecipes_(10)
    , preloader_(nullptr)
{
    // 默认方案目录
    recipeDirectory_ = QDir::currentPath() + "/recipes";

    // 初始化模型预加载器
    preloader_ = new ModelPreloader(this);

    // 连接预加载器信号
    connect(preloader_, &ModelPreloader::started,
            this, &RecipeManager::modelPreloadStarted);
    connect(preloader_, &ModelPreloader::progressChanged,
            this, &RecipeManager::modelPreloadProgress);
    connect(preloader_, &ModelPreloader::finished,
            this, [this](int success, int failure, qint64) {
                emit modelPreloadFinished(success, failure);
            });
}

RecipeManager::~RecipeManager()
{
    clearAll();
}

RecipeManager& RecipeManager::instance()
{
    static RecipeManager instance;
    return instance;
}

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

Recipe* RecipeManager::createRecipe(const QString& name)
{
    if (recipes_.contains(name)) {
        LOG_WARNING(QString("方案已存在: %1").arg(name));
        return recipes_[name];
    }

    Recipe* recipe = new Recipe(this);
    recipe->setName(name);

    recipes_[name] = recipe;

    LOG_INFO(QString("创建新方案: %1").arg(name));

    emit recipeListChanged();
    return recipe;
}

Recipe* RecipeManager::loadRecipe(const QString& filePath)
{
    Recipe* recipe = new Recipe(this);

    if (!recipe->loadFromFile(filePath)) {
        delete recipe;
        return nullptr;
    }

    QString name = recipe->name();

    // 如果已存在同名方案，先移除
    if (recipes_.contains(name)) {
        delete recipes_.take(name);
    }

    recipes_[name] = recipe;
    addToRecentRecipes(name);

    LOG_INFO(QString("加载方案: %1 <- %2").arg(name).arg(filePath));

    emit recipeLoaded(name);
    emit recipeListChanged();

    return recipe;
}

bool RecipeManager::saveRecipe(Recipe* recipe, const QString& filePath)
{
    if (!recipe) {
        LOG_ERROR("保存方案失败：方案为空");
        return false;
    }

    QString savePath = filePath;
    if (savePath.isEmpty()) {
        savePath = generateFilePath(recipe->name());
    }

    if (!recipe->saveToFile(savePath)) {
        return false;
    }

    addToRecentRecipes(recipe->name());

    emit recipeSaved(recipe->name());
    return true;
}

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

    // 从最近使用中移除
    recentRecipes_.removeAll(name);

    delete recipe;

    LOG_INFO(QString("移除方案: %1").arg(name));

    emit recipeRemoved(name);
    emit recipeListChanged();

    return true;
}

Recipe* RecipeManager::getRecipe(const QString& name)
{
    return recipes_.value(name, nullptr);
}

QList<Recipe*> RecipeManager::getAllRecipes() const
{
    return recipes_.values();
}

QStringList RecipeManager::getAllRecipeNames() const
{
    return recipes_.keys();
}

bool RecipeManager::hasRecipe(const QString& name) const
{
    return recipes_.contains(name);
}

// ========== 当前方案 ==========

bool RecipeManager::setCurrentRecipe(const QString& name)
{
    if (name.isEmpty()) {
        currentRecipe_ = nullptr;
        emit currentRecipeChanged(QString());
        return true;
    }

    if (!recipes_.contains(name)) {
        LOG_ERROR(QString("设置当前方案失败：方案不存在: %1").arg(name));
        return false;
    }

    currentRecipe_ = recipes_[name];
    addToRecentRecipes(name);

    // 保存最后使用的配方名称
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

QString RecipeManager::currentRecipeName() const
{
    return currentRecipe_ ? currentRecipe_->name() : QString();
}

// ========== 搜索 ==========

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

    // 扫描所有.vfr文件
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

void RecipeManager::clearAll()
{
    // 清除当前方案引用
    currentRecipe_ = nullptr;

    // 删除所有方案
    for (Recipe* recipe : recipes_) {
        delete recipe;
    }
    recipes_.clear();

    LOG_INFO("清除所有方案");

    emit currentRecipeChanged(QString());
    emit recipeListChanged();
}

Recipe* RecipeManager::importRecipe(const QString& filePath)
{
    Recipe* recipe = loadRecipe(filePath);
    if (!recipe) {
        return nullptr;
    }

    // 复制文件到方案目录
    QString targetPath = generateFilePath(recipe->name());
    if (filePath != targetPath) {
        QFile::copy(filePath, targetPath);
        recipe->saveToFile(targetPath);
    }

    LOG_INFO(QString("导入方案: %1").arg(recipe->name()));

    return recipe;
}

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

    // 复制内容
    QJsonObject json = source->serialize();
    newRecipe->deserialize(json);

    // 更新信息
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

// ========== 最近使用 ==========

void RecipeManager::setMaxRecentRecipes(int max)
{
    maxRecentRecipes_ = qMax(1, max);

    // 截断
    while (recentRecipes_.size() > maxRecentRecipes_) {
        recentRecipes_.removeLast();
    }
}

void RecipeManager::clearRecentRecipes()
{
    recentRecipes_.clear();
}

// ========== 私有方法 ==========

void RecipeManager::addToRecentRecipes(const QString& name)
{
    // 移除已存在的
    recentRecipes_.removeAll(name);

    // 添加到开头
    recentRecipes_.prepend(name);

    // 截断
    while (recentRecipes_.size() > maxRecentRecipes_) {
        recentRecipes_.removeLast();
    }
}

QString RecipeManager::generateFilePath(const QString& name) const
{
    // 清理文件名（移除非法字符）
    QString safeName = name;
    safeName.replace(QRegularExpression("[/\\\\:*?\"<>|]"), "_");

    // 每个方案使用独立的子目录
    // 目录结构: recipes/{方案名}/{方案名}.vfr
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

bool RecipeManager::initialize()
{
    // 扫描方案目录
    int count = scanRecipeDirectory();
    LOG_INFO(QString("方案管理器初始化完成，加载了 %1 个方案").arg(count));

    // 尝试加载上次使用的配方
    if (loadLastRecipe()) {
        LOG_INFO(QString("已自动加载上次使用的配方: %1").arg(currentRecipeName()));
        return true;
    }

    return false;
}

void RecipeManager::saveLastRecipeName()
{
    QString recipeName = currentRecipeName();
    Base::ConfigManager::instance().setValue(CONFIG_KEY_LAST_RECIPE, recipeName);
    Base::ConfigManager::instance().save();
    LOG_DEBUG(QString("保存最后使用的配方: %1").arg(recipeName));
}

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

QString RecipeManager::getLastRecipeName() const
{
    return Base::ConfigManager::instance().getValue(CONFIG_KEY_LAST_RECIPE, QString()).toString();
}

// ========== 模型预加载 ==========

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
