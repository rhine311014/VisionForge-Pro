/**
 * @file VisionProject.cpp
 * @brief 视觉项目核心实现文件
 * @details 本文件实现了VisionForge视觉检测系统的项目管理核心功能，包括：
 *          - 项目的创建、打开、保存、关闭等生命周期管理
 *          - 主工具链和子工具链的管理
 *          - 相机设备的关联管理
 *          - 项目配置的JSON序列化与反序列化
 *          项目文件采用.vfp扩展名，使用JSON格式存储配置信息
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-01-02
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
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

/**
 * @brief 构造函数 - 初始化视觉项目对象
 * @param parent 父对象指针，用于Qt对象树内存管理
 * @details 初始化项目状态标志，创建默认主工具链，
 *          并连接工具链结构变化信号以追踪修改状态
 */
VisionProject::VisionProject(QObject* parent)
    : QObject(parent)
    , isOpened_(false)      // 项目初始为未打开状态
    , isModified_(false)    // 项目初始为未修改状态
    , mainToolChain_(nullptr)
    , camera_(nullptr)
{
    // 创建主工具链，作为项目的核心检测流程
    mainToolChain_ = new ToolChain(this);
    mainToolChain_->setName("主工具链");

    // 连接主工具链的结构变化信号到修改状态追踪
    // 当工具链发生任何结构性变化时，自动标记项目为已修改
    connect(mainToolChain_, &ToolChain::structureChanged,
            this, [this]() { setModified(true); });
}

/**
 * @brief 析构函数 - 清理项目资源
 * @details 关闭项目并释放主工具链内存
 */
VisionProject::~VisionProject()
{
    // 关闭项目，释放相关资源
    close();

    // 释放主工具链
    if (mainToolChain_) {
        delete mainToolChain_;
        mainToolChain_ = nullptr;
    }
}

// ========== 项目管理 ==========

/**
 * @brief 创建新项目
 * @param projectPath 项目目录路径
 * @param projectName 项目名称
 * @return 创建成功返回true，失败返回false
 * @details 执行以下步骤：
 *          1. 关闭当前已打开的项目
 *          2. 创建项目目录
 *          3. 初始化项目配置信息
 *          4. 保存项目文件
 *          5. 发送项目打开信号
 */
bool VisionProject::createNew(const QString& projectPath, const QString& projectName)
{
    // 步骤1：关闭现有项目，确保资源正确释放
    close();

    // 步骤2：创建项目目录
    QDir dir;
    if (!dir.mkpath(projectPath)) {
        LOG_ERROR(QString("创建项目目录失败: %1").arg(projectPath));
        return false;
    }

    // 步骤3：设置项目配置，初始化基本信息
    config_ = ProjectConfig();
    config_.projectName = projectName;
    config_.projectPath = projectPath;
    config_.author = qgetenv("USERNAME");  // 从环境变量获取当前用户名作为作者
    config_.createTime = QDateTime::currentMSecsSinceEpoch();  // 记录创建时间戳
    config_.modifyTime = config_.createTime;  // 初始修改时间等于创建时间

    // 设置项目文件路径（目录路径 + 项目名 + .vfp扩展名）
    projectFilePath_ = QDir(projectPath).filePath(projectName + ".vfp");

    // 步骤4：保存项目文件
    if (!save()) {
        LOG_ERROR("保存新项目失败");
        return false;
    }

    // 步骤5：更新项目状态
    isOpened_ = true;
    isModified_ = false;

    LOG_INFO(QString("创建新项目: %1").arg(projectFilePath_));

    // 发送项目已打开信号，通知UI更新
    emit projectOpened(projectPath);
    return true;
}

/**
 * @brief 打开已存在的项目
 * @param projectFilePath 项目文件完整路径（.vfp文件）
 * @return 打开成功返回true，失败返回false
 * @details 从指定的项目文件加载配置，恢复项目状态
 */
bool VisionProject::open(const QString& projectFilePath)
{
    // 关闭当前已打开的项目
    close();

    // 从文件加载项目配置
    if (!loadFromFile(projectFilePath)) {
        LOG_ERROR(QString("加载项目失败: %1").arg(projectFilePath));
        return false;
    }

    // 更新项目状态
    projectFilePath_ = projectFilePath;
    isOpened_ = true;
    isModified_ = false;

    LOG_INFO(QString("打开项目: %1").arg(projectFilePath));

    // 发送项目已打开信号
    emit projectOpened(config_.projectPath);
    return true;
}

/**
 * @brief 保存当前项目
 * @return 保存成功返回true，失败返回false
 * @details 将当前项目配置序列化并保存到项目文件
 */
bool VisionProject::save()
{
    // 检查项目是否已打开
    if (!isOpened_) {
        LOG_WARNING("保存项目失败：项目未打开");
        return false;
    }

    // 检查项目文件路径有效性
    if (projectFilePath_.isEmpty()) {
        LOG_ERROR("保存项目失败：项目文件路径为空");
        return false;
    }

    // 更新修改时间戳
    config_.modifyTime = QDateTime::currentMSecsSinceEpoch();

    // 执行保存操作
    if (!saveToFile(projectFilePath_)) {
        LOG_ERROR(QString("保存项目失败: %1").arg(projectFilePath_));
        return false;
    }

    // 重置修改状态
    isModified_ = false;

    LOG_INFO(QString("保存项目: %1").arg(projectFilePath_));

    // 发送保存完成信号
    emit projectSaved(projectFilePath_);
    emit modifiedChanged(false);

    return true;
}

/**
 * @brief 另存为新项目文件
 * @param projectFilePath 新的项目文件路径
 * @return 保存成功返回true，失败返回false
 * @details 将当前项目以新的路径和名称保存，同时更新项目配置
 */
bool VisionProject::saveAs(const QString& projectFilePath)
{
    // 检查项目是否已打开
    if (!isOpened_) {
        LOG_WARNING("另存为失败：项目未打开");
        return false;
    }

    // 保存原路径用于失败时恢复
    QString oldPath = projectFilePath_;
    projectFilePath_ = projectFilePath;

    // 更新项目路径和名称配置
    QFileInfo fileInfo(projectFilePath);
    config_.projectPath = fileInfo.absolutePath();
    config_.projectName = fileInfo.baseName();

    // 执行保存操作
    if (!save()) {
        // 保存失败时恢复原路径
        projectFilePath_ = oldPath;
        return false;
    }

    LOG_INFO(QString("另存为: %1").arg(projectFilePath));

    return true;
}

/**
 * @brief 关闭当前项目
 * @details 释放所有项目资源，包括工具链、相机等，
 *          并重置项目状态到初始状态
 */
void VisionProject::close()
{
    // 如果项目未打开，直接返回
    if (!isOpened_) {
        return;
    }

    LOG_INFO(QString("关闭项目: %1").arg(config_.projectName));

    // 清理主工具链中的所有工具
    if (mainToolChain_) {
        mainToolChain_->clear();
    }

    // 清理并释放所有子工具链
    for (ToolChain* toolChain : subToolChains_) {
        delete toolChain;
    }
    subToolChains_.clear();

    // 清理相机资源
    if (camera_) {
        delete camera_;
        camera_ = nullptr;
    }

    // 重置项目配置和状态
    config_ = ProjectConfig();
    projectFilePath_.clear();
    isOpened_ = false;
    isModified_ = false;

    // 发送项目关闭信号
    emit projectClosed();
}

/**
 * @brief 设置项目修改状态
 * @param modified 是否已修改
 * @details 当修改状态发生变化时，发送modifiedChanged信号通知UI更新
 */
void VisionProject::setModified(bool modified)
{
    if (isModified_ != modified) {
        isModified_ = modified;
        emit modifiedChanged(modified);
    }
}

// ========== 工具链管理 ==========

/**
 * @brief 添加子工具链
 * @param name 子工具链名称
 * @return 创建的子工具链指针，如果已存在同名工具链则返回已存在的
 * @details 子工具链用于实现模块化的检测流程，可以被主工具链调用
 */
ToolChain* VisionProject::addSubToolChain(const QString& name)
{
    // 检查是否已存在同名子工具链
    if (subToolChains_.contains(name)) {
        LOG_WARNING(QString("子工具链已存在: %1").arg(name));
        return subToolChains_[name];
    }

    // 创建新的子工具链
    ToolChain* toolChain = new ToolChain(this);
    toolChain->setName(name);

    // 添加到子工具链集合
    subToolChains_[name] = toolChain;

    // 连接修改信号，追踪子工具链的变化
    connect(toolChain, &ToolChain::structureChanged,
            this, [this]() { setModified(true); });

    // 标记项目已修改
    setModified(true);

    LOG_INFO(QString("添加子工具链: %1").arg(name));

    return toolChain;
}

/**
 * @brief 移除子工具链
 * @param name 要移除的子工具链名称
 * @return 移除成功返回true，不存在则返回false
 */
bool VisionProject::removeSubToolChain(const QString& name)
{
    // 检查子工具链是否存在
    if (!subToolChains_.contains(name)) {
        LOG_WARNING(QString("子工具链不存在: %1").arg(name));
        return false;
    }

    // 从集合中移除并释放内存
    ToolChain* toolChain = subToolChains_.take(name);
    delete toolChain;

    // 标记项目已修改
    setModified(true);

    LOG_INFO(QString("移除子工具链: %1").arg(name));

    return true;
}

/**
 * @brief 获取子工具链
 * @param name 子工具链名称
 * @return 子工具链指针，不存在则返回nullptr
 */
ToolChain* VisionProject::getSubToolChain(const QString& name)
{
    return subToolChains_.value(name, nullptr);
}

/**
 * @brief 获取所有子工具链名称
 * @return 子工具链名称列表
 */
QStringList VisionProject::getSubToolChainNames() const
{
    return subToolChains_.keys();
}

// ========== 相机管理 ==========

/**
 * @brief 设置项目关联的相机
 * @param camera 相机接口指针
 * @details 如果已有相机关联，会先释放原有相机
 */
void VisionProject::setCamera(HAL::ICamera* camera)
{
    // 释放原有相机
    if (camera_) {
        delete camera_;
    }

    // 设置新相机
    camera_ = camera;

    // 标记项目已修改
    setModified(true);

    LOG_INFO("设置相机");
}

// ========== 序列化 ==========

/**
 * @brief 将项目序列化为JSON对象
 * @return 包含项目完整配置的JSON对象
 * @details 序列化内容包括：
 *          - 项目配置信息（名称、路径、作者、时间等）
 *          - 主工具链配置
 *          - 所有子工具链配置
 *          - 格式版本信息
 */
QJsonObject VisionProject::serialize() const
{
    QJsonObject json;

    // 序列化项目配置信息
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

    // 序列化主工具链
    if (mainToolChain_) {
        json["mainToolChain"] = mainToolChain_->serialize();
    }

    // 序列化所有子工具链
    QJsonObject subToolChainsObj;
    for (auto it = subToolChains_.constBegin(); it != subToolChains_.constEnd(); ++it) {
        subToolChainsObj[it.key()] = it.value()->serialize();
    }
    json["subToolChains"] = subToolChainsObj;

    // 添加格式版本标识，用于后续版本兼容
    json["formatVersion"] = "1.0";

    return json;
}

/**
 * @brief 从JSON对象反序列化项目配置
 * @param json 包含项目配置的JSON对象
 * @return 反序列化成功返回true，失败返回false
 * @details 按照序列化的逆过程恢复项目状态
 */
bool VisionProject::deserialize(const QJsonObject& json)
{
    // 检查格式版本兼容性
    QString formatVersion = json["formatVersion"].toString();
    if (formatVersion != "1.0") {
        LOG_WARNING(QString("未知的项目格式版本: %1").arg(formatVersion));
    }

    // 反序列化项目配置信息
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

    // 反序列化主工具链
    if (json.contains("mainToolChain")) {
        if (mainToolChain_) {
            mainToolChain_->deserialize(json["mainToolChain"].toObject());
        }
    }

    // 反序列化所有子工具链
    if (json.contains("subToolChains")) {
        QJsonObject subToolChainsObj = json["subToolChains"].toObject();

        for (auto it = subToolChainsObj.constBegin(); it != subToolChainsObj.constEnd(); ++it) {
            QString name = it.key();
            ToolChain* toolChain = addSubToolChain(name);
            toolChain->deserialize(it.value().toObject());
        }
    }

    // 发送配置变化信号
    emit configChanged();

    return true;
}

// ========== 私有方法 ==========

/**
 * @brief 从文件加载项目配置
 * @param filePath 项目文件路径
 * @return 加载成功返回true，失败返回false
 * @details 读取JSON文件并调用deserialize进行反序列化
 */
bool VisionProject::loadFromFile(const QString& filePath)
{
    // 打开项目文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("打开项目文件失败: %1").arg(filePath));
        return false;
    }

    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();

    // 解析JSON文档
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("项目文件格式错误: %1").arg(filePath));
        return false;
    }

    // 调用反序列化方法恢复项目配置
    return deserialize(doc.object());
}

/**
 * @brief 将项目配置保存到文件
 * @param filePath 目标文件路径
 * @return 保存成功返回true，失败返回false
 * @details 调用serialize获取JSON对象，格式化后写入文件
 */
bool VisionProject::saveToFile(const QString& filePath)
{
    // 序列化项目配置
    QJsonObject json = serialize();
    QJsonDocument doc(json);

    // 创建并打开目标文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("创建项目文件失败: %1").arg(filePath));
        return false;
    }

    // 写入格式化的JSON内容（带缩进，便于阅读）
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

} // namespace Core
} // namespace VisionForge
