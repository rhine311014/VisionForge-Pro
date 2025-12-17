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

    // 新增匹配参数
    json["min_score"] = minScore;
    json["num_matches"] = numMatches;
    json["max_overlap"] = maxOverlap;
    json["sub_pixel"] = subPixel;
    json["greediness"] = greediness;
    json["scale_min"] = scaleMin;
    json["scale_max"] = scaleMax;
    json["match_type"] = matchType;

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

    // 新增匹配参数（带默认值以兼容旧数据）
    info.minScore = json["min_score"].toDouble(0.5);
    info.numMatches = json["num_matches"].toInt(1);
    info.maxOverlap = json["max_overlap"].toDouble(0.5);
    info.subPixel = json["sub_pixel"].toString("least_squares");
    info.greediness = json["greediness"].toDouble(0.9);
    info.scaleMin = json["scale_min"].toDouble(0.8);
    info.scaleMax = json["scale_max"].toDouble(1.2);
    info.matchType = json["match_type"].toInt(0);

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
    if (!info) {
        LOG_ERROR("addModel: 模板信息为空");
        return false;
    }
    if (modelFilePath.isEmpty()) {
        LOG_ERROR("addModel: 模板文件路径为空");
        return false;
    }
    if (libraryPath_.isEmpty()) {
        LOG_ERROR("addModel: 模板库路径未设置");
        return false;
    }

    // 检查源文件是否存在
    if (!QFile::exists(modelFilePath)) {
        LOG_ERROR(QString("addModel: 源模板文件不存在: %1").arg(modelFilePath));
        return false;
    }

    // 生成ID
    if (info->id.isEmpty()) {
        info->id = generateModelId();
    }
    LOG_DEBUG(QString("addModel: 模板ID=%1, 名称=%2").arg(info->id).arg(info->name));

    // 设置时间
    QDateTime now = QDateTime::currentDateTime();
    info->createTime = now;
    info->modifyTime = now;

    // 复制模型文件到库目录
    QDir dir(libraryPath_);
    if (!dir.exists()) {
        LOG_ERROR(QString("addModel: 模板库目录不存在: %1").arg(libraryPath_));
        return false;
    }

    QString modelFileName = QString("%1.gshm").arg(info->id);  // 使用.gshm扩展名以区分通用模型
    QString targetModelPath = dir.filePath(modelFileName);

    LOG_DEBUG(QString("addModel: 复制文件 %1 -> %2").arg(modelFilePath).arg(targetModelPath));

    if (!copyModelFile(modelFilePath, targetModelPath)) {
        LOG_ERROR(QString("addModel: 复制模型文件失败: %1 -> %2").arg(modelFilePath).arg(targetModelPath));
        return false;
    }

    info->filePath = modelFileName;  // 相对路径

    // 保存缩略图
    if (!thumbnail.isNull()) {
        QString thumbnailFileName = QString("%1_thumb.png").arg(info->id);
        QString targetThumbnailPath = dir.filePath(thumbnailFileName);

        if (saveThumbnail(thumbnail, targetThumbnailPath)) {
            info->thumbnailPath = thumbnailFileName;  // 相对路径
            LOG_DEBUG(QString("addModel: 缩略图已保存: %1").arg(targetThumbnailPath));
        } else {
            LOG_WARNING(QString("addModel: 保存缩略图失败: %1").arg(targetThumbnailPath));
        }
    }

    // 添加到列表
    models_.push_back(info);

    // 保存配置
    if (!saveLibrary()) {
        LOG_ERROR("addModel: 保存模板库配置失败");
        // 仍然返回true，因为模型文件已复制成功
    }

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
    LOG_DEBUG(QString("copyModelFile: 源文件=%1").arg(srcPath));
    LOG_DEBUG(QString("copyModelFile: 目标文件=%1").arg(dstPath));

    // 检查源文件是否存在
    if (!QFile::exists(srcPath)) {
        LOG_ERROR(QString("copyModelFile: 源文件不存在: %1").arg(srcPath));
        return false;
    }

    // 获取源文件大小
    QFileInfo srcInfo(srcPath);
    LOG_DEBUG(QString("copyModelFile: 源文件大小=%1 bytes").arg(srcInfo.size()));

    // 如果目标文件存在，先删除
    if (QFile::exists(dstPath)) {
        if (!QFile::remove(dstPath)) {
            LOG_ERROR(QString("copyModelFile: 无法删除已存在的目标文件: %1").arg(dstPath));
            return false;
        }
        LOG_DEBUG("copyModelFile: 已删除旧的目标文件");
    }

    // 复制文件
    bool success = QFile::copy(srcPath, dstPath);
    if (!success) {
        // 尝试使用二进制读写方式复制
        LOG_WARNING("copyModelFile: QFile::copy失败，尝试手动复制...");

        QFile srcFile(srcPath);
        if (!srcFile.open(QIODevice::ReadOnly)) {
            LOG_ERROR(QString("copyModelFile: 无法打开源文件: %1").arg(srcPath));
            return false;
        }

        QFile dstFile(dstPath);
        if (!dstFile.open(QIODevice::WriteOnly)) {
            LOG_ERROR(QString("copyModelFile: 无法创建目标文件: %1").arg(dstPath));
            srcFile.close();
            return false;
        }

        // 读取并写入
        QByteArray data = srcFile.readAll();
        qint64 written = dstFile.write(data);
        srcFile.close();
        dstFile.close();

        if (written != data.size()) {
            LOG_ERROR(QString("copyModelFile: 写入不完整: %1/%2 bytes").arg(written).arg(data.size()));
            return false;
        }

        LOG_INFO(QString("copyModelFile: 手动复制成功，大小=%1 bytes").arg(written));
        return true;
    }

    LOG_DEBUG("copyModelFile: 复制成功");
    return true;
}

bool ShapeModelManager::saveThumbnail(const QImage& thumbnail, const QString& path)
{
    return thumbnail.save(path, "PNG");
}

} // namespace Algorithm
} // namespace VisionForge

#endif // _WIN32
