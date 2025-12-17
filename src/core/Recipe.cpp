/**
 * @file Recipe.cpp
 * @brief 方案管理实现
 */

#include "core/Recipe.h"
#include "base/Logger.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>

namespace VisionForge {
namespace Core {

Recipe::Recipe(QObject* parent)
    : QObject(parent)
    , toolChain_(nullptr)
    , isModified_(false)
{
    // 创建工具链
    toolChain_ = new ToolChain(this);
    toolChain_->setName("方案工具链");

    // 设置默认信息
    info_.createTime = QDateTime::currentMSecsSinceEpoch();
    info_.modifyTime = info_.createTime;
    info_.author = qgetenv("USERNAME");

    // 连接工具链修改信号
    connect(toolChain_, &ToolChain::structureChanged,
            this, [this]() { setModified(true); });
}

Recipe::~Recipe()
{
    if (toolChain_) {
        delete toolChain_;
        toolChain_ = nullptr;
    }
}

// ========== 基本信息 ==========

void Recipe::setName(const QString& name)
{
    if (info_.name != name) {
        info_.name = name;
        setModified(true);
        emit infoChanged();
    }
}

void Recipe::setDescription(const QString& description)
{
    if (info_.description != description) {
        info_.description = description;
        setModified(true);
        emit infoChanged();
    }
}

void Recipe::setProductCode(const QString& code)
{
    if (info_.productCode != code) {
        info_.productCode = code;
        setModified(true);
        emit infoChanged();
    }
}

void Recipe::setTags(const QStringList& tags)
{
    info_.tags = tags;
    setModified(true);
    emit infoChanged();
}

void Recipe::addTag(const QString& tag)
{
    if (!info_.tags.contains(tag)) {
        info_.tags.append(tag);
        setModified(true);
        emit infoChanged();
    }
}

void Recipe::removeTag(const QString& tag)
{
    if (info_.tags.removeAll(tag) > 0) {
        setModified(true);
        emit infoChanged();
    }
}

// ========== 工具链管理 ==========

bool Recipe::copyFromToolChain(const ToolChain* source)
{
    if (!source) {
        LOG_ERROR("复制工具链失败：源工具链为空");
        return false;
    }

    // 序列化源工具链再反序列化到目标
    QJsonObject json = source->serialize();
    bool result = toolChain_->deserialize(json);

    if (result) {
        LOG_INFO(QString("从工具链复制配置: %1 个工具").arg(toolChain_->toolCount()));
        setModified(true);
    } else {
        LOG_ERROR("复制工具链配置失败");
    }

    return result;
}

bool Recipe::applyToToolChain(ToolChain* target) const
{
    if (!target) {
        LOG_ERROR("应用工具链失败：目标工具链为空");
        return false;
    }

    // 序列化当前工具链再反序列化到目标
    QJsonObject json = toolChain_->serialize();
    bool result = target->deserialize(json);

    if (result) {
        LOG_INFO(QString("应用方案到工具链: %1").arg(info_.name));
    } else {
        LOG_ERROR("应用方案到工具链失败");
    }

    return result;
}

// ========== 自定义参数 ==========

void Recipe::setCustomParam(const QString& key, const QVariant& value)
{
    customParams_[key] = value;
    setModified(true);
}

QVariant Recipe::customParam(const QString& key, const QVariant& defaultValue) const
{
    return customParams_.value(key, defaultValue);
}

void Recipe::removeCustomParam(const QString& key)
{
    if (customParams_.remove(key) > 0) {
        setModified(true);
    }
}

QStringList Recipe::customParamKeys() const
{
    return customParams_.keys();
}

void Recipe::clearCustomParams()
{
    if (!customParams_.isEmpty()) {
        customParams_.clear();
        setModified(true);
    }
}

// ========== 文件操作 ==========

bool Recipe::saveToFile(const QString& filePath)
{
    QJsonObject json = serialize();
    QJsonDocument doc(json);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("保存方案文件失败: %1").arg(filePath));
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    filePath_ = filePath;
    info_.modifyTime = QDateTime::currentMSecsSinceEpoch();
    isModified_ = false;

    LOG_INFO(QString("保存方案: %1 -> %2").arg(info_.name).arg(filePath));

    emit saved(filePath);
    return true;
}

bool Recipe::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("打开方案文件失败: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("方案文件格式错误: %1").arg(filePath));
        return false;
    }

    if (!deserialize(doc.object())) {
        LOG_ERROR(QString("反序列化方案失败: %1").arg(filePath));
        return false;
    }

    filePath_ = filePath;
    isModified_ = false;

    LOG_INFO(QString("加载方案: %1 <- %2").arg(info_.name).arg(filePath));

    emit loaded(filePath);
    return true;
}

// ========== 目录管理 ==========

QString Recipe::recipeDirectory() const
{
    if (filePath_.isEmpty()) {
        return QString();
    }

    QFileInfo fileInfo(filePath_);
    return fileInfo.absolutePath();
}

QString Recipe::modelLibraryPath() const
{
    QString recipeDir = recipeDirectory();
    if (recipeDir.isEmpty()) {
        return QString();
    }

    // 模板库存储在方案目录下的 models 子目录
    return QDir(recipeDir).filePath("models");
}

bool Recipe::ensureDirectoryStructure() const
{
    QString recipeDir = recipeDirectory();
    if (recipeDir.isEmpty()) {
        LOG_WARNING("方案目录为空，无法创建目录结构");
        return false;
    }

    // 确保方案目录存在
    QDir dir(recipeDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            LOG_ERROR(QString("无法创建方案目录: %1").arg(recipeDir));
            return false;
        }
        LOG_INFO(QString("创建方案目录: %1").arg(recipeDir));
    }

    // 确保模板库目录存在
    QString modelsDir = modelLibraryPath();
    if (!modelsDir.isEmpty()) {
        QDir modelsPath(modelsDir);
        if (!modelsPath.exists()) {
            if (!modelsPath.mkpath(".")) {
                LOG_ERROR(QString("无法创建模板库目录: %1").arg(modelsDir));
                return false;
            }
            LOG_INFO(QString("创建模板库目录: %1").arg(modelsDir));
        }
    }

    return true;
}

// ========== 序列化 ==========

QJsonObject Recipe::serialize() const
{
    QJsonObject json;

    // 格式标识
    json["formatType"] = "VisionForgeRecipe";
    json["formatVersion"] = "1.0";

    // 方案信息
    QJsonObject infoObj;
    infoObj["name"] = info_.name;
    infoObj["description"] = info_.description;
    infoObj["author"] = info_.author;
    infoObj["createTime"] = info_.createTime;
    infoObj["modifyTime"] = info_.modifyTime;
    infoObj["version"] = info_.version;
    infoObj["productCode"] = info_.productCode;

    // 标签
    QJsonArray tagsArray;
    for (const QString& tag : info_.tags) {
        tagsArray.append(tag);
    }
    infoObj["tags"] = tagsArray;

    json["info"] = infoObj;

    // 工具链
    if (toolChain_) {
        json["toolChain"] = toolChain_->serialize();
    }

    // 自定义参数
    QJsonObject customParamsObj;
    for (auto it = customParams_.constBegin(); it != customParams_.constEnd(); ++it) {
        customParamsObj[it.key()] = QJsonValue::fromVariant(it.value());
    }
    json["customParams"] = customParamsObj;

    return json;
}

bool Recipe::deserialize(const QJsonObject& json)
{
    // 检查格式
    QString formatType = json["formatType"].toString();
    if (formatType != "VisionForgeRecipe") {
        LOG_WARNING(QString("未知的方案格式类型: %1").arg(formatType));
    }

    QString formatVersion = json["formatVersion"].toString();
    if (formatVersion != "1.0") {
        LOG_WARNING(QString("未知的方案格式版本: %1").arg(formatVersion));
    }

    // 方案信息
    if (json.contains("info")) {
        QJsonObject infoObj = json["info"].toObject();

        info_.name = infoObj["name"].toString();
        info_.description = infoObj["description"].toString();
        info_.author = infoObj["author"].toString();
        info_.createTime = infoObj["createTime"].toVariant().toLongLong();
        info_.modifyTime = infoObj["modifyTime"].toVariant().toLongLong();
        info_.version = infoObj["version"].toString("1.0.0");
        info_.productCode = infoObj["productCode"].toString();

        // 标签
        info_.tags.clear();
        QJsonArray tagsArray = infoObj["tags"].toArray();
        for (const QJsonValue& val : tagsArray) {
            info_.tags.append(val.toString());
        }
    }

    // 工具链
    if (json.contains("toolChain") && toolChain_) {
        if (!toolChain_->deserialize(json["toolChain"].toObject())) {
            LOG_WARNING("反序列化工具链失败");
        }
    }

    // 自定义参数
    customParams_.clear();
    if (json.contains("customParams")) {
        QJsonObject customParamsObj = json["customParams"].toObject();
        for (auto it = customParamsObj.constBegin(); it != customParamsObj.constEnd(); ++it) {
            customParams_[it.key()] = it.value().toVariant();
        }
    }

    emit infoChanged();
    return true;
}

QString Recipe::exportAsText() const
{
    QString text;
    QTextStream stream(&text);

    stream << "========================================\n";
    stream << "VisionForge 方案报告\n";
    stream << "========================================\n\n";

    // 基本信息
    stream << "【方案信息】\n";
    stream << "  名称: " << info_.name << "\n";
    stream << "  描述: " << info_.description << "\n";
    stream << "  作者: " << info_.author << "\n";
    stream << "  版本: " << info_.version << "\n";
    stream << "  产品编号: " << info_.productCode << "\n";

    if (!info_.tags.isEmpty()) {
        stream << "  标签: " << info_.tags.join(", ") << "\n";
    }

    QDateTime createDt = QDateTime::fromMSecsSinceEpoch(info_.createTime);
    QDateTime modifyDt = QDateTime::fromMSecsSinceEpoch(info_.modifyTime);
    stream << "  创建时间: " << createDt.toString("yyyy-MM-dd HH:mm:ss") << "\n";
    stream << "  修改时间: " << modifyDt.toString("yyyy-MM-dd HH:mm:ss") << "\n";
    stream << "\n";

    // 工具链信息
    stream << "【工具链】\n";
    if (toolChain_) {
        stream << "  工具数量: " << toolChain_->toolCount() << "\n";
        stream << "  工具列表:\n";

        for (int i = 0; i < toolChain_->toolCount(); ++i) {
            const ToolChainNode* node = toolChain_->getNode(i);
            if (node && node->tool) {
                stream << "    " << (i + 1) << ". " << node->name;
                stream << " [" << node->tool->toolName() << "]";
                if (!node->tool->isEnabled()) {
                    stream << " (禁用)";
                }
                stream << "\n";
            }
        }
    }
    stream << "\n";

    // 自定义参数
    if (!customParams_.isEmpty()) {
        stream << "【自定义参数】\n";
        for (auto it = customParams_.constBegin(); it != customParams_.constEnd(); ++it) {
            stream << "  " << it.key() << ": " << it.value().toString() << "\n";
        }
        stream << "\n";
    }

    stream << "========================================\n";

    return text;
}

// ========== 验证 ==========

QStringList Recipe::validate() const
{
    QStringList errors;

    // 检查名称
    if (info_.name.isEmpty()) {
        errors.append("方案名称不能为空");
    }

    // 检查工具链
    if (!toolChain_ || toolChain_->toolCount() == 0) {
        errors.append("工具链为空");
    }

    return errors;
}

// ========== 私有方法 ==========

void Recipe::setModified(bool modified)
{
    if (isModified_ != modified) {
        isModified_ = modified;
        if (modified) {
            emit this->modified();
        }
    }
}

} // namespace Core
} // namespace VisionForge
