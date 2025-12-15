/**
 * @file ShapeModelManager.cpp
 * @brief 形状模板管理器实现
 */

#ifdef _WIN32

#include "algorithm/ShapeModelManager.h"
#include "base/Logger.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QUuid>
#include <QPainter>
#include <HalconCpp.h>

using namespace HalconCpp;

namespace VisionForge {
namespace Algorithm {

// ========== ShapeModelInfo ==========

QJsonObject ShapeModelInfo::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["name"] = name;
    json["description"] = description;
    json["file_path"] = filePath;
    json["thumbnail_path"] = thumbnailPath;
    json["create_time"] = createTime.toString(Qt::ISODate);
    json["modify_time"] = modifyTime.toString(Qt::ISODate);
    json["num_levels"] = numLevels;
    json["angle_start"] = angleStart;
    json["angle_extent"] = angleExtent;
    json["min_contrast"] = minContrast;
    json["usage_count"] = usageCount;
    return json;
}

ShapeModelInfo ShapeModelInfo::fromJson(const QJsonObject& json)
{
    ShapeModelInfo info;
    info.id = json["id"].toString();
    info.name = json["name"].toString();
    info.description = json["description"].toString();
    info.filePath = json["file_path"].toString();
    info.thumbnailPath = json["thumbnail_path"].toString();
    info.createTime = QDateTime::fromString(json["create_time"].toString(), Qt::ISODate);
    info.modifyTime = QDateTime::fromString(json["modify_time"].toString(), Qt::ISODate);
    info.numLevels = json["num_levels"].toInt(0);
    info.angleStart = json["angle_start"].toDouble(0.0);
    info.angleExtent = json["angle_extent"].toDouble(6.28);
    info.minContrast = json["min_contrast"].toInt(10);
    info.usageCount = json["usage_count"].toInt(0);
    return info;
}

// ========== ShapeModelManager ==========

ShapeModelManager::ShapeModelManager()
    : configFileName_("models.json")
{
}

ShapeModelManager::~ShapeModelManager()
{
}

ShapeModelManager& ShapeModelManager::instance()
{
    static ShapeModelManager instance;
    return instance;
}

bool ShapeModelManager::loadLibrary(const QString& libraryPath)
{
    libraryPath_ = libraryPath;

    // 确保目录存在
    QDir dir(libraryPath_);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            LOG_ERROR(QString("无法创建模板库目录: %1").arg(libraryPath_));
            return false;
        }
    }

    // 加载配置文件
    QString configPath = dir.filePath(configFileName_);
    QFile file(configPath);

    if (!file.exists()) {
        // 配置文件不存在，创建空库
        models_.clear();
        emit libraryLoaded();
        LOG_INFO(QString("创建新的模板库: %1").arg(libraryPath_));
        return true;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开配置文件: %1").arg(configPath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        LOG_ERROR("配置文件格式错误");
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray modelsArray = root["models"].toArray();

    models_.clear();
    for (const QJsonValue& value : modelsArray) {
        if (value.isObject()) {
            ShapeModelInfo info = ShapeModelInfo::fromJson(value.toObject());
            models_.push_back(std::make_shared<ShapeModelInfo>(info));
        }
    }

    emit libraryLoaded();
    LOG_INFO(QString("模板库加载成功: %1, 共 %2 个模板")
        .arg(libraryPath_).arg(models_.size()));

    return true;
}

bool ShapeModelManager::saveLibrary()
{
    if (libraryPath_.isEmpty()) {
        LOG_ERROR("模板库路径未设置");
        return false;
    }

    QJsonObject root;
    QJsonArray modelsArray;

    for (const auto& info : models_) {
        modelsArray.append(info->toJson());
    }

    root["models"] = modelsArray;
    root["version"] = "1.0";
    root["last_modified"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonDocument doc(root);
    QByteArray data = doc.toJson(QJsonDocument::Indented);

    QDir dir(libraryPath_);
    QString configPath = dir.filePath(configFileName_);

    QFile file(configPath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法保存配置文件: %1").arg(configPath));
        return false;
    }

    file.write(data);
    file.close();

    LOG_INFO("模板库配置已保存");
    return true;
}

void ShapeModelManager::setLibraryPath(const QString& path)
{
    libraryPath_ = path;
}

bool ShapeModelManager::addModel(ShapeModelInfoPtr info, const QString& modelFilePath,
                                const QImage& thumbnail)
{
    if (!info || modelFilePath.isEmpty()) {
        return false;
    }

    // 生成ID
    if (info->id.isEmpty()) {
        info->id = generateModelId();
    }

    // 设置时间
    QDateTime now = QDateTime::currentDateTime();
    info->createTime = now;
    info->modifyTime = now;

    // 复制模型文件到库目录
    QDir dir(libraryPath_);
    QString modelFileName = QString("%1.shm").arg(info->id);
    QString targetModelPath = dir.filePath(modelFileName);

    if (!copyModelFile(modelFilePath, targetModelPath)) {
        LOG_ERROR("复制模型文件失败");
        return false;
    }

    info->filePath = modelFileName;  // 相对路径

    // 保存缩略图
    if (!thumbnail.isNull()) {
        QString thumbnailFileName = QString("%1_thumb.png").arg(info->id);
        QString targetThumbnailPath = dir.filePath(thumbnailFileName);

        if (saveThumbnail(thumbnail, targetThumbnailPath)) {
            info->thumbnailPath = thumbnailFileName;  // 相对路径
        }
    }

    // 添加到列表
    models_.push_back(info);

    // 保存配置
    saveLibrary();

    emit modelAdded(info->id);
    LOG_INFO(QString("模板已添加: %1 (%2)").arg(info->name).arg(info->id));

    return true;
}

bool ShapeModelManager::removeModel(const QString& modelId)
{
    auto it = std::find_if(models_.begin(), models_.end(),
        [&modelId](const ShapeModelInfoPtr& info) { return info->id == modelId; });

    if (it == models_.end()) {
        return false;
    }

    // 删除文件
    QDir dir(libraryPath_);

    QString modelPath = dir.filePath((*it)->filePath);
    QFile::remove(modelPath);

    if (!(*it)->thumbnailPath.isEmpty()) {
        QString thumbnailPath = dir.filePath((*it)->thumbnailPath);
        QFile::remove(thumbnailPath);
    }

    // 从列表移除
    models_.erase(it);

    // 保存配置
    saveLibrary();

    emit modelRemoved(modelId);
    LOG_INFO(QString("模板已移除: %1").arg(modelId));

    return true;
}

bool ShapeModelManager::updateModel(ShapeModelInfoPtr info)
{
    if (!info) {
        return false;
    }

    auto it = std::find_if(models_.begin(), models_.end(),
        [&info](const ShapeModelInfoPtr& item) { return item->id == info->id; });

    if (it == models_.end()) {
        return false;
    }

    info->modifyTime = QDateTime::currentDateTime();
    *it = info;

    // 保存配置
    saveLibrary();

    emit modelUpdated(info->id);
    LOG_INFO(QString("模板已更新: %1").arg(info->id));

    return true;
}

std::vector<ShapeModelInfoPtr> ShapeModelManager::getAllModels() const
{
    return models_;
}

ShapeModelInfoPtr ShapeModelManager::getModel(const QString& modelId) const
{
    auto it = std::find_if(models_.begin(), models_.end(),
        [&modelId](const ShapeModelInfoPtr& info) { return info->id == modelId; });

    return (it != models_.end()) ? *it : nullptr;
}

std::vector<ShapeModelInfoPtr> ShapeModelManager::searchModels(const QString& keyword) const
{
    if (keyword.isEmpty()) {
        return models_;
    }

    std::vector<ShapeModelInfoPtr> results;
    QString lowerKeyword = keyword.toLower();

    for (const auto& info : models_) {
        if (info->name.toLower().contains(lowerKeyword) ||
            info->description.toLower().contains(lowerKeyword)) {
            results.push_back(info);
        }
    }

    return results;
}

bool ShapeModelManager::exportModel(const QString& modelId, const QString& targetPath)
{
    auto info = getModel(modelId);
    if (!info) {
        return false;
    }

    QDir dir(libraryPath_);
    QString srcPath = dir.filePath(info->filePath);

    return QFile::copy(srcPath, targetPath);
}

bool ShapeModelManager::exportModels(const QStringList& modelIds, const QString& targetDir)
{
    QDir dir(targetDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            return false;
        }
    }

    int successCount = 0;
    for (const QString& modelId : modelIds) {
        auto info = getModel(modelId);
        if (!info) {
            continue;
        }

        QString targetPath = dir.filePath(QString("%1.shm").arg(info->name));
        if (exportModel(modelId, targetPath)) {
            successCount++;
        }
    }

    LOG_INFO(QString("批量导出完成: 成功 %1/%2").arg(successCount).arg(modelIds.size()));
    return successCount > 0;
}

bool ShapeModelManager::importModel(const QString& modelFilePath, const QString& name,
                                    const QString& description)
{
    if (!QFile::exists(modelFilePath)) {
        LOG_ERROR(QString("模板文件不存在: %1").arg(modelFilePath));
        return false;
    }

    // 检查库路径
    if (libraryPath_.isEmpty()) {
        LOG_ERROR("模板库路径未设置");
        return false;
    }

    LOG_INFO(QString("开始导入模板: %1").arg(modelFilePath));

    // 创建模板信息
    auto info = std::make_shared<ShapeModelInfo>();
    info->name = name.isEmpty() ? QFileInfo(modelFilePath).baseName() : name;
    info->description = description;
    info->usageCount = 0;

    // 尝试生成缩略图（如果失败不影响导入）
    QImage thumbnail;
    try {
        thumbnail = generateThumbnail(modelFilePath);
        if (thumbnail.isNull()) {
            LOG_WARNING("缩略图生成失败，将使用空缩略图");
        }
    } catch (...) {
        LOG_WARNING("生成缩略图时发生异常，将使用空缩略图");
    }

    return addModel(info, modelFilePath, thumbnail);
}

int ShapeModelManager::importModels(const QStringList& modelFilePaths)
{
    int successCount = 0;
    for (const QString& filePath : modelFilePaths) {
        if (importModel(filePath, QFileInfo(filePath).baseName())) {
            successCount++;
        }
    }

    LOG_INFO(QString("批量导入完成: 成功 %1/%2").arg(successCount).arg(modelFilePaths.size()));
    return successCount;
}

bool ShapeModelManager::removeModels(const QStringList& modelIds)
{
    int successCount = 0;
    for (const QString& modelId : modelIds) {
        if (removeModel(modelId)) {
            successCount++;
        }
    }

    LOG_INFO(QString("批量删除完成: 成功 %1/%2").arg(successCount).arg(modelIds.size()));
    return successCount > 0;
}

void ShapeModelManager::incrementUsage(const QString& modelId)
{
    auto info = getModel(modelId);
    if (info) {
        info->usageCount++;
        info->modifyTime = QDateTime::currentDateTime();
        saveLibrary();
    }
}

QImage ShapeModelManager::generateThumbnail(const QString& modelFilePath, int width, int height)
{
    // 暂时禁用缩略图生成，直接返回空图像
    // 这样可以确保导入功能正常工作
    LOG_DEBUG(QString("跳过缩略图生成: %1").arg(modelFilePath));

    // 创建一个简单的占位缩略图
    QImage thumbnail(width, height, QImage::Format_RGB888);
    thumbnail.fill(QColor(40, 40, 40));

    QPainter painter(&thumbnail);
    painter.setPen(QPen(Qt::gray, 1));
    painter.drawRect(0, 0, width - 1, height - 1);
    painter.setPen(Qt::white);
    painter.drawText(thumbnail.rect(), Qt::AlignCenter, "模板");

    return thumbnail;
}

QImage ShapeModelManager::getThumbnail(const QString& modelId) const
{
    auto info = getModel(modelId);
    if (!info || info->thumbnailPath.isEmpty()) {
        return QImage();
    }

    QDir dir(libraryPath_);
    QString thumbnailPath = dir.filePath(info->thumbnailPath);

    QImage thumbnail(thumbnailPath);
    return thumbnail;
}

QString ShapeModelManager::generateModelId() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

bool ShapeModelManager::copyModelFile(const QString& srcPath, const QString& dstPath)
{
    // 如果目标文件存在，先删除
    if (QFile::exists(dstPath)) {
        QFile::remove(dstPath);
    }

    return QFile::copy(srcPath, dstPath);
}

bool ShapeModelManager::saveThumbnail(const QImage& thumbnail, const QString& path)
{
    return thumbnail.save(path, "PNG");
}

} // namespace Algorithm
} // namespace VisionForge

#endif // _WIN32
