/**
 * @file Recipe.cpp
 * @brief 工艺方案核心实现文件
 * @details 本文件实现了VisionForge视觉检测系统的工艺方案管理功能，包括：
 *          - 方案的创建、加载、保存等生命周期管理
 *          - 方案信息的维护（名称、描述、标签、产品编号等）
 *          - 内嵌工具链的管理与应用
 *          - 自定义参数的存储与访问
 *          - 方案文件的序列化与反序列化
 *          - 方案目录结构的自动创建
 *          方案是产品检测配置的完整快照，支持不同产品间快速切换
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-01-02
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
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

/**
 * @brief 构造函数 - 初始化方案对象
 * @param parent 父对象指针，用于Qt对象树内存管理
 * @details 创建内嵌工具链，初始化方案基本信息，设置创建时间为当前时间，
 *          并连接工具链修改信号以追踪方案修改状态
 */
Recipe::Recipe(QObject* parent)
    : QObject(parent)
    , toolChain_(nullptr)
    , isModified_(false)
{
    // 创建方案内嵌的工具链
    toolChain_ = new ToolChain(this);
    toolChain_->setName("方案工具链");

    // 设置默认方案信息
    info_.createTime = QDateTime::currentMSecsSinceEpoch();  // 记录创建时间
    info_.modifyTime = info_.createTime;                      // 初始修改时间等于创建时间
    info_.author = qgetenv("USERNAME");                       // 从环境变量获取当前用户名

    // 连接工具链修改信号，当工具链结构变化时自动标记方案为已修改
    connect(toolChain_, &ToolChain::structureChanged,
            this, [this]() { setModified(true); });
}

/**
 * @brief 析构函数 - 清理方案资源
 * @details 释放内嵌工具链的内存
 */
Recipe::~Recipe()
{
    if (toolChain_) {
        delete toolChain_;
        toolChain_ = nullptr;
    }
}

// ========== 基本信息管理 ==========

/**
 * @brief 设置方案名称
 * @param name 新的方案名称
 * @details 名称变化时自动标记方案为已修改
 */
void Recipe::setName(const QString& name)
{
    if (info_.name != name) {
        info_.name = name;
        setModified(true);
        emit infoChanged();
    }
}

/**
 * @brief 设置方案描述
 * @param description 新的方案描述信息
 */
void Recipe::setDescription(const QString& description)
{
    if (info_.description != description) {
        info_.description = description;
        setModified(true);
        emit infoChanged();
    }
}

/**
 * @brief 设置产品编号
 * @param code 产品编号/料号
 * @details 产品编号用于与生产系统集成，实现自动方案切换
 */
void Recipe::setProductCode(const QString& code)
{
    if (info_.productCode != code) {
        info_.productCode = code;
        setModified(true);
        emit infoChanged();
    }
}

/**
 * @brief 设置方案标签列表
 * @param tags 标签字符串列表
 * @details 标签用于方案分类和快速搜索
 */
void Recipe::setTags(const QStringList& tags)
{
    info_.tags = tags;
    setModified(true);
    emit infoChanged();
}

/**
 * @brief 添加单个标签
 * @param tag 要添加的标签
 * @details 如果标签已存在则不重复添加
 */
void Recipe::addTag(const QString& tag)
{
    if (!info_.tags.contains(tag)) {
        info_.tags.append(tag);
        setModified(true);
        emit infoChanged();
    }
}

/**
 * @brief 移除指定标签
 * @param tag 要移除的标签
 */
void Recipe::removeTag(const QString& tag)
{
    if (info_.tags.removeAll(tag) > 0) {
        setModified(true);
        emit infoChanged();
    }
}

// ========== 工具链管理 ==========

/**
 * @brief 从源工具链复制配置
 * @param source 源工具链指针
 * @return 复制成功返回true，失败返回false
 * @details 通过序列化-反序列化方式实现深拷贝
 */
bool Recipe::copyFromToolChain(const ToolChain* source)
{
    if (!source) {
        LOG_ERROR("复制工具链失败：源工具链为空");
        return false;
    }

    // 序列化源工具链再反序列化到目标（实现深拷贝）
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

/**
 * @brief 将方案工具链配置应用到目标工具链
 * @param target 目标工具链指针
 * @return 应用成功返回true，失败返回false
 * @details 用于将方案配置应用到当前运行的工具链
 */
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

// ========== 自定义参数管理 ==========

/**
 * @brief 设置自定义参数
 * @param key 参数键名
 * @param value 参数值（支持多种数据类型）
 * @details 自定义参数用于存储方案特有的扩展配置
 */
void Recipe::setCustomParam(const QString& key, const QVariant& value)
{
    customParams_[key] = value;
    setModified(true);
}

/**
 * @brief 获取自定义参数
 * @param key 参数键名
 * @param defaultValue 参数不存在时的默认值
 * @return 参数值或默认值
 */
QVariant Recipe::customParam(const QString& key, const QVariant& defaultValue) const
{
    return customParams_.value(key, defaultValue);
}

/**
 * @brief 移除自定义参数
 * @param key 要移除的参数键名
 */
void Recipe::removeCustomParam(const QString& key)
{
    if (customParams_.remove(key) > 0) {
        setModified(true);
    }
}

/**
 * @brief 获取所有自定义参数的键名列表
 * @return 参数键名字符串列表
 */
QStringList Recipe::customParamKeys() const
{
    return customParams_.keys();
}

/**
 * @brief 清空所有自定义参数
 */
void Recipe::clearCustomParams()
{
    if (!customParams_.isEmpty()) {
        customParams_.clear();
        setModified(true);
    }
}

// ========== 文件操作 ==========

/**
 * @brief 将方案保存到文件
 * @param filePath 目标文件路径
 * @return 保存成功返回true，失败返回false
 * @details 保存后自动更新修改时间并清除修改标志
 */
bool Recipe::saveToFile(const QString& filePath)
{
    // 序列化方案
    QJsonObject json = serialize();
    QJsonDocument doc(json);

    // 打开文件写入
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("保存方案文件失败: %1").arg(filePath));
        return false;
    }

    // 写入格式化的JSON内容
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    // 更新文件路径和时间戳
    filePath_ = filePath;
    info_.modifyTime = QDateTime::currentMSecsSinceEpoch();
    isModified_ = false;  // 保存后清除修改标志

    LOG_INFO(QString("保存方案: %1 -> %2").arg(info_.name).arg(filePath));

    emit saved(filePath);
    return true;
}

/**
 * @brief 从文件加载方案
 * @param filePath 源文件路径
 * @return 加载成功返回true，失败返回false
 */
bool Recipe::loadFromFile(const QString& filePath)
{
    // 打开方案文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("打开方案文件失败: %1").arg(filePath));
        return false;
    }

    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();

    // 解析JSON文档
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("方案文件格式错误: %1").arg(filePath));
        return false;
    }

    // 反序列化方案配置
    if (!deserialize(doc.object())) {
        LOG_ERROR(QString("反序列化方案失败: %1").arg(filePath));
        return false;
    }

    // 更新文件路径和状态
    filePath_ = filePath;
    isModified_ = false;  // 新加载的方案标记为未修改

    LOG_INFO(QString("加载方案: %1 <- %2").arg(info_.name).arg(filePath));

    emit loaded(filePath);
    return true;
}

// ========== 目录管理 ==========

/**
 * @brief 获取方案所在目录
 * @return 方案文件的绝对目录路径，未保存则返回空字符串
 */
QString Recipe::recipeDirectory() const
{
    if (filePath_.isEmpty()) {
        return QString();
    }

    QFileInfo fileInfo(filePath_);
    return fileInfo.absolutePath();
}

/**
 * @brief 获取模板库目录路径
 * @return 模板库目录的绝对路径
 * @details 模板库用于存储形状匹配模板、AI模型等资源文件
 */
QString Recipe::modelLibraryPath() const
{
    QString recipeDir = recipeDirectory();
    if (recipeDir.isEmpty()) {
        return QString();
    }

    // 模板库存储在方案目录下的 models 子目录
    return QDir(recipeDir).filePath("models");
}

/**
 * @brief 获取标定数据目录路径
 * @return 标定数据目录的绝对路径
 * @details 标定数据用于存储相机标定结果等
 */
QString Recipe::calibrationPath() const
{
    QString recipeDir = recipeDirectory();
    if (recipeDir.isEmpty()) {
        return QString();
    }

    // 标定数据存储在方案目录下的 calibration 子目录
    return QDir(recipeDir).filePath("calibration");
}

/**
 * @brief 确保方案目录结构存在
 * @return 创建成功返回true，失败返回false
 * @details 自动创建方案目录、模板库目录、标定目录
 */
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

    // 确保标定目录存在
    QString calibDir = calibrationPath();
    if (!calibDir.isEmpty()) {
        QDir calibPath(calibDir);
        if (!calibPath.exists()) {
            if (!calibPath.mkpath(".")) {
                LOG_ERROR(QString("无法创建标定目录: %1").arg(calibDir));
                return false;
            }
            LOG_INFO(QString("创建标定目录: %1").arg(calibDir));
        }
    }

    return true;
}

// ========== 序列化 ==========

/**
 * @brief 将方案序列化为JSON对象
 * @return 包含方案完整配置的JSON对象
 * @details 序列化内容包括：
 *          - 格式标识（用于版本兼容）
 *          - 方案信息（名称、描述、作者、时间戳、标签等）
 *          - 工具链配置
 *          - 自定义参数
 */
QJsonObject Recipe::serialize() const
{
    QJsonObject json;

    // 格式标识 - 用于版本兼容性检查
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

    // 标签数组
    QJsonArray tagsArray;
    for (const QString& tag : info_.tags) {
        tagsArray.append(tag);
    }
    infoObj["tags"] = tagsArray;

    json["info"] = infoObj;

    // 工具链配置
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

/**
 * @brief 从JSON对象反序列化方案配置
 * @param json 包含方案配置的JSON对象
 * @return 反序列化成功返回true，失败返回false
 * @details 按照序列化的逆过程恢复方案状态，包含版本兼容性检查
 */
bool Recipe::deserialize(const QJsonObject& json)
{
    // 检查格式类型
    QString formatType = json["formatType"].toString();
    if (formatType != "VisionForgeRecipe") {
        LOG_WARNING(QString("未知的方案格式类型: %1").arg(formatType));
    }

    // 检查格式版本
    QString formatVersion = json["formatVersion"].toString();
    if (formatVersion != "1.0") {
        LOG_WARNING(QString("未知的方案格式版本: %1").arg(formatVersion));
    }

    // 解析方案信息
    if (json.contains("info")) {
        QJsonObject infoObj = json["info"].toObject();

        info_.name = infoObj["name"].toString();
        info_.description = infoObj["description"].toString();
        info_.author = infoObj["author"].toString();
        info_.createTime = infoObj["createTime"].toVariant().toLongLong();
        info_.modifyTime = infoObj["modifyTime"].toVariant().toLongLong();
        info_.version = infoObj["version"].toString("1.0.0");
        info_.productCode = infoObj["productCode"].toString();

        // 解析标签列表
        info_.tags.clear();
        QJsonArray tagsArray = infoObj["tags"].toArray();
        for (const QJsonValue& val : tagsArray) {
            info_.tags.append(val.toString());
        }
    }

    // 解析工具链配置
    if (json.contains("toolChain") && toolChain_) {
        if (!toolChain_->deserialize(json["toolChain"].toObject())) {
            LOG_WARNING("反序列化工具链失败");
        }
    }

    // 解析自定义参数
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

/**
 * @brief 导出方案为文本报告
 * @return 格式化的方案报告文本
 * @details 生成人类可读的方案配置摘要，包含：
 *          - 方案基本信息
 *          - 工具链工具列表
 *          - 自定义参数列表
 */
QString Recipe::exportAsText() const
{
    QString text;
    QTextStream stream(&text);

    // 报告头部
    stream << "========================================\n";
    stream << "VisionForge 方案报告\n";
    stream << "========================================\n\n";

    // 基本信息部分
    stream << "【方案信息】\n";
    stream << "  名称: " << info_.name << "\n";
    stream << "  描述: " << info_.description << "\n";
    stream << "  作者: " << info_.author << "\n";
    stream << "  版本: " << info_.version << "\n";
    stream << "  产品编号: " << info_.productCode << "\n";

    // 标签信息
    if (!info_.tags.isEmpty()) {
        stream << "  标签: " << info_.tags.join(", ") << "\n";
    }

    // 时间信息
    QDateTime createDt = QDateTime::fromMSecsSinceEpoch(info_.createTime);
    QDateTime modifyDt = QDateTime::fromMSecsSinceEpoch(info_.modifyTime);
    stream << "  创建时间: " << createDt.toString("yyyy-MM-dd HH:mm:ss") << "\n";
    stream << "  修改时间: " << modifyDt.toString("yyyy-MM-dd HH:mm:ss") << "\n";
    stream << "\n";

    // 工具链信息部分
    stream << "【工具链】\n";
    if (toolChain_) {
        stream << "  工具数量: " << toolChain_->toolCount() << "\n";
        stream << "  工具列表:\n";

        // 遍历并列出所有工具
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

    // 自定义参数部分
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

/**
 * @brief 验证方案配置有效性
 * @return 验证错误信息列表，为空表示验证通过
 * @details 检查项：
 *          - 方案名称非空
 *          - 工具链非空
 */
QStringList Recipe::validate() const
{
    QStringList errors;

    // 检查方案名称
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

/**
 * @brief 设置方案修改状态
 * @param modified 是否已修改
 * @details 当修改状态变化时发送modified信号
 */
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
