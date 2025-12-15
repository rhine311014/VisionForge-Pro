/**
 * @file VisionProject.cpp
 * @brief 视觉项目实现
 */

#include "core/VisionProject.h"
#include "base/Logger.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>

namespace VisionForge {
namespace Core {

VisionProject::VisionProject(QObject* parent)
    : QObject(parent)
    , isOpened_(false)
    , isModified_(false)
    , mainToolChain_(nullptr)
    , camera_(nullptr)
{
    // 创建主工具链
    mainToolChain_ = new ToolChain(this);
    mainToolChain_->setName("主工具链");

    // 连接修改信号
    connect(mainToolChain_, &ToolChain::structureChanged,
            this, [this]() { setModified(true); });
}

VisionProject::~VisionProject()
{
    close();

    if (mainToolChain_) {
        delete mainToolChain_;
        mainToolChain_ = nullptr;
    }
}

// ========== 项目管理 ==========

bool VisionProject::createNew(const QString& projectPath, const QString& projectName)
{
    // 关闭现有项目
    close();

    // 创建项目目录
    QDir dir;
    if (!dir.mkpath(projectPath)) {
        LOG_ERROR(QString("创建项目目录失败: %1").arg(projectPath));
        return false;
    }

    // 设置项目配置
    config_ = ProjectConfig();
    config_.projectName = projectName;
    config_.projectPath = projectPath;
    config_.author = qgetenv("USERNAME");
    config_.createTime = QDateTime::currentMSecsSinceEpoch();
    config_.modifyTime = config_.createTime;

    // 设置项目文件路径
    projectFilePath_ = QDir(projectPath).filePath(projectName + ".vfp");

    // 保存项目
    if (!save()) {
        LOG_ERROR("保存新项目失败");
        return false;
    }

    isOpened_ = true;
    isModified_ = false;

    LOG_INFO(QString("创建新项目: %1").arg(projectFilePath_));

    emit projectOpened(projectPath);
    return true;
}

bool VisionProject::open(const QString& projectFilePath)
{
    // 关闭现有项目
    close();

    // 加载项目文件
    if (!loadFromFile(projectFilePath)) {
        LOG_ERROR(QString("加载项目失败: %1").arg(projectFilePath));
        return false;
    }

    projectFilePath_ = projectFilePath;
    isOpened_ = true;
    isModified_ = false;

    LOG_INFO(QString("打开项目: %1").arg(projectFilePath));

    emit projectOpened(config_.projectPath);
    return true;
}

bool VisionProject::save()
{
    if (!isOpened_) {
        LOG_WARNING("保存项目失败：项目未打开");
        return false;
    }

    if (projectFilePath_.isEmpty()) {
        LOG_ERROR("保存项目失败：项目文件路径为空");
        return false;
    }

    // 更新修改时间
    config_.modifyTime = QDateTime::currentMSecsSinceEpoch();

    // 保存到文件
    if (!saveToFile(projectFilePath_)) {
        LOG_ERROR(QString("保存项目失败: %1").arg(projectFilePath_));
        return false;
    }

    isModified_ = false;

    LOG_INFO(QString("保存项目: %1").arg(projectFilePath_));

    emit projectSaved(projectFilePath_);
    emit modifiedChanged(false);

    return true;
}

bool VisionProject::saveAs(const QString& projectFilePath)
{
    if (!isOpened_) {
        LOG_WARNING("另存为失败：项目未打开");
        return false;
    }

    QString oldPath = projectFilePath_;
    projectFilePath_ = projectFilePath;

    // 更新项目路径
    QFileInfo fileInfo(projectFilePath);
    config_.projectPath = fileInfo.absolutePath();
    config_.projectName = fileInfo.baseName();

    if (!save()) {
        // 恢复原路径
        projectFilePath_ = oldPath;
        return false;
    }

    LOG_INFO(QString("另存为: %1").arg(projectFilePath));

    return true;
}

void VisionProject::close()
{
    if (!isOpened_) {
        return;
    }

    LOG_INFO(QString("关闭项目: %1").arg(config_.projectName));

    // 清理主工具链
    if (mainToolChain_) {
        mainToolChain_->clear();
    }

    // 清理子工具链
    for (ToolChain* toolChain : subToolChains_) {
        delete toolChain;
    }
    subToolChains_.clear();

    // 清理相机
    if (camera_) {
        delete camera_;
        camera_ = nullptr;
    }

    // 重置状态
    config_ = ProjectConfig();
    projectFilePath_.clear();
    isOpened_ = false;
    isModified_ = false;

    emit projectClosed();
}

void VisionProject::setModified(bool modified)
{
    if (isModified_ != modified) {
        isModified_ = modified;
        emit modifiedChanged(modified);
    }
}

// ========== 工具链管理 ==========

ToolChain* VisionProject::addSubToolChain(const QString& name)
{
    if (subToolChains_.contains(name)) {
        LOG_WARNING(QString("子工具链已存在: %1").arg(name));
        return subToolChains_[name];
    }

    ToolChain* toolChain = new ToolChain(this);
    toolChain->setName(name);

    subToolChains_[name] = toolChain;

    // 连接修改信号
    connect(toolChain, &ToolChain::structureChanged,
            this, [this]() { setModified(true); });

    setModified(true);

    LOG_INFO(QString("添加子工具链: %1").arg(name));

    return toolChain;
}

bool VisionProject::removeSubToolChain(const QString& name)
{
    if (!subToolChains_.contains(name)) {
        LOG_WARNING(QString("子工具链不存在: %1").arg(name));
        return false;
    }

    ToolChain* toolChain = subToolChains_.take(name);
    delete toolChain;

    setModified(true);

    LOG_INFO(QString("移除子工具链: %1").arg(name));

    return true;
}

ToolChain* VisionProject::getSubToolChain(const QString& name)
{
    return subToolChains_.value(name, nullptr);
}

QStringList VisionProject::getSubToolChainNames() const
{
    return subToolChains_.keys();
}

// ========== 相机管理 ==========

void VisionProject::setCamera(HAL::ICamera* camera)
{
    if (camera_) {
        delete camera_;
    }

    camera_ = camera;

    setModified(true);

    LOG_INFO("设置相机");
}

// ========== 序列化 ==========

QJsonObject VisionProject::serialize() const
{
    QJsonObject json;

    // 项目配置
    QJsonObject configObj;
    configObj["projectName"] = config_.projectName;
    configObj["projectPath"] = config_.projectPath;
    configObj["description"] = config_.description;
    configObj["author"] = config_.author;
    configObj["createTime"] = config_.createTime;
    configObj["modifyTime"] = config_.modifyTime;
    configObj["version"] = config_.version;
    configObj["cameraType"] = config_.cameraType;
    configObj["cameraParams"] = config_.cameraParams;
    configObj["autoSaveResults"] = config_.autoSaveResults;
    configObj["resultSavePath"] = config_.resultSavePath;
    configObj["maxHistoryCount"] = config_.maxHistoryCount;

    json["config"] = configObj;

    // 主工具链
    if (mainToolChain_) {
        json["mainToolChain"] = mainToolChain_->serialize();
    }

    // 子工具链
    QJsonObject subToolChainsObj;
    for (auto it = subToolChains_.constBegin(); it != subToolChains_.constEnd(); ++it) {
        subToolChainsObj[it.key()] = it.value()->serialize();
    }
    json["subToolChains"] = subToolChainsObj;

    // 格式版本
    json["formatVersion"] = "1.0";

    return json;
}

bool VisionProject::deserialize(const QJsonObject& json)
{
    // 检查格式版本
    QString formatVersion = json["formatVersion"].toString();
    if (formatVersion != "1.0") {
        LOG_WARNING(QString("未知的项目格式版本: %1").arg(formatVersion));
    }

    // 加载项目配置
    if (json.contains("config")) {
        QJsonObject configObj = json["config"].toObject();

        config_.projectName = configObj["projectName"].toString();
        config_.projectPath = configObj["projectPath"].toString();
        config_.description = configObj["description"].toString();
        config_.author = configObj["author"].toString();
        config_.createTime = configObj["createTime"].toVariant().toLongLong();
        config_.modifyTime = configObj["modifyTime"].toVariant().toLongLong();
        config_.version = configObj["version"].toString();
        config_.cameraType = configObj["cameraType"].toString();
        config_.cameraParams = configObj["cameraParams"].toObject();
        config_.autoSaveResults = configObj["autoSaveResults"].toBool();
        config_.resultSavePath = configObj["resultSavePath"].toString();
        config_.maxHistoryCount = configObj["maxHistoryCount"].toInt(100);
    }

    // 加载主工具链
    if (json.contains("mainToolChain")) {
        if (mainToolChain_) {
            mainToolChain_->deserialize(json["mainToolChain"].toObject());
        }
    }

    // 加载子工具链
    if (json.contains("subToolChains")) {
        QJsonObject subToolChainsObj = json["subToolChains"].toObject();

        for (auto it = subToolChainsObj.constBegin(); it != subToolChainsObj.constEnd(); ++it) {
            QString name = it.key();
            ToolChain* toolChain = addSubToolChain(name);
            toolChain->deserialize(it.value().toObject());
        }
    }

    emit configChanged();

    return true;
}

// ========== 私有方法 ==========

bool VisionProject::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("打开项目文件失败: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("项目文件格式错误: %1").arg(filePath));
        return false;
    }

    return deserialize(doc.object());
}

bool VisionProject::saveToFile(const QString& filePath)
{
    QJsonObject json = serialize();
    QJsonDocument doc(json);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("创建项目文件失败: %1").arg(filePath));
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

} // namespace Core
} // namespace VisionForge
