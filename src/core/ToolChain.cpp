/**
 * @file ToolChain.cpp
 * @brief 工具链实现
 */

#include "core/ToolChain.h"
#include "algorithm/ToolFactory.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QElapsedTimer>

namespace VisionForge {
namespace Core {

ToolChain::ToolChain(QObject* parent)
    : QObject(parent)
    , name_("新建工具链")
    , nodeIdCounter_(0)
{
}

ToolChain::~ToolChain()
{
    clear();
}

// ========== 工具链管理 ==========

QString ToolChain::addTool(Algorithm::VisionTool* tool, const QString& name)
{
    if (!tool) {
        LOG_ERROR("添加工具失败：工具指针为空");
        return QString();
    }

    QString nodeId = generateNodeId();
    QString nodeName = name.isEmpty() ? tool->toolName() : name;

    // 接管工具所有权
    nodes_.push_back(ToolChainNode(std::unique_ptr<Algorithm::VisionTool>(tool), nodeId, nodeName));

    LOG_DEBUG(QString("添加工具到链尾: %1 [%2]").arg(nodeName).arg(nodeId));

    emit structureChanged();
    return nodeId;
}

QString ToolChain::insertTool(int index, Algorithm::VisionTool* tool, const QString& name)
{
    if (!tool) {
        LOG_ERROR("插入工具失败：工具指针为空");
        return QString();
    }

    if (index < 0 || index > static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("插入工具失败：索引越界 %1").arg(index));
        return QString();
    }

    QString nodeId = generateNodeId();
    QString nodeName = name.isEmpty() ? tool->toolName() : name;

    // 接管工具所有权
    nodes_.insert(nodes_.begin() + index, ToolChainNode(std::unique_ptr<Algorithm::VisionTool>(tool), nodeId, nodeName));

    LOG_DEBUG(QString("插入工具到位置 %1: %2 [%3]").arg(index).arg(nodeName).arg(nodeId));

    emit structureChanged();
    return nodeId;
}

bool ToolChain::removeTool(const QString& nodeId)
{
    int index = findNodeIndex(nodeId);
    if (index < 0) {
        LOG_ERROR(QString("移除工具失败：未找到节点 %1").arg(nodeId));
        return false;
    }

    return removeToolAt(index);
}

bool ToolChain::removeToolAt(int index)
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("移除工具失败：索引越界 %1").arg(index));
        return false;
    }

    LOG_DEBUG(QString("移除工具: %1 [%2]").arg(nodes_[index].name).arg(nodes_[index].id));

    // unique_ptr会自动删除工具实例
    nodes_.erase(nodes_.begin() + index);

    emit structureChanged();
    return true;
}

bool ToolChain::moveTool(int fromIndex, int toIndex)
{
    if (fromIndex < 0 || fromIndex >= static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("移动工具失败：源索引越界 %1").arg(fromIndex));
        return false;
    }

    if (toIndex < 0 || toIndex >= static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("移动工具失败：目标索引越界 %1").arg(toIndex));
        return false;
    }

    if (fromIndex == toIndex) {
        return true;
    }

    // 使用移动语义重新排列节点
    ToolChainNode node = std::move(nodes_[fromIndex]);
    nodes_.erase(nodes_.begin() + fromIndex);
    nodes_.insert(nodes_.begin() + toIndex, std::move(node));

    LOG_DEBUG(QString("移动工具: %1 -> %2").arg(fromIndex).arg(toIndex));

    emit structureChanged();
    return true;
}

void ToolChain::clear()
{
    // unique_ptr会自动删除所有工具实例
    nodes_.clear();

    LOG_DEBUG("工具链已清空");

    emit structureChanged();
}

Algorithm::VisionTool* ToolChain::getTool(int index) const
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        return nullptr;
    }
    return nodes_[index].tool.get();
}

Algorithm::VisionTool* ToolChain::getTool(const QString& nodeId) const
{
    int index = findNodeIndex(nodeId);
    if (index < 0) {
        return nullptr;
    }
    return nodes_[index].tool.get();
}

const ToolChainNode* ToolChain::getNode(int index) const
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        return nullptr;
    }
    return &nodes_[index];
}

QString ToolChain::getNodeId(int index) const
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        return QString();
    }
    return nodes_[index].id;
}

int ToolChain::findNodeIndex(const QString& nodeId) const
{
    for (size_t i = 0; i < nodes_.size(); ++i) {
        if (nodes_[i].id == nodeId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void ToolChain::setNodeEnabled(const QString& nodeId, bool enabled)
{
    int index = findNodeIndex(nodeId);
    if (index < 0) {
        LOG_ERROR(QString("设置节点启用状态失败：未找到节点 %1").arg(nodeId));
        return;
    }

    nodes_[index].enabled = enabled;

    LOG_DEBUG(QString("设置节点 %1 启用状态: %2").arg(nodeId).arg(enabled ? "启用" : "禁用"));
}

void ToolChain::setNodeName(const QString& nodeId, const QString& name)
{
    int index = findNodeIndex(nodeId);
    if (index < 0) {
        LOG_ERROR(QString("设置节点名称失败：未找到节点 %1").arg(nodeId));
        return;
    }

    nodes_[index].name = name;

    LOG_DEBUG(QString("设置节点 %1 名称: %2").arg(nodeId).arg(name));
}

// ========== 工具链执行 ==========

bool ToolChain::execute(const Base::ImageData::Ptr& input, ToolChainResult& result)
{
    QElapsedTimer timer;
    timer.start();

    result = ToolChainResult();

    if (!input || input->isEmpty()) {
        result.success = false;
        result.errorMessage = "输入图像为空";
        LOG_ERROR(result.errorMessage);
        return false;
    }

    if (nodes_.empty()) {
        result.success = false;
        result.errorMessage = "工具链为空";
        LOG_ERROR(result.errorMessage);
        return false;
    }

    LOG_INFO(QString("开始执行工具链: %1 (%2个工具)").arg(name_).arg(static_cast<int>(nodes_.size())));

    // 执行工具链
    Base::ImageData::Ptr currentImage = input;
    result.toolResults.reserve(static_cast<int>(nodes_.size()));

    for (size_t i = 0; i < nodes_.size(); ++i) {
        const ToolChainNode& node = nodes_[i];

        // 跳过禁用的工具
        if (!node.enabled) {
            LOG_DEBUG(QString("跳过禁用的工具: %1").arg(node.name));
            continue;
        }

        emit toolExecutionStarted(static_cast<int>(i), node.name);
        emit executionProgress(static_cast<int>(i) + 1, static_cast<int>(nodes_.size()));

        Algorithm::ToolResult toolResult;
        bool success = node.tool->process(currentImage, toolResult);

        result.toolResults.append(toolResult);

        emit toolExecutionFinished(static_cast<int>(i), success);

        if (!success) {
            result.success = false;
            result.errorMessage = QString("工具 '%1' 执行失败: %2")
                                .arg(node.name)
                                .arg(toolResult.errorMessage);
            result.failedToolIndex = static_cast<int>(i);

            LOG_ERROR(result.errorMessage);
            return false;
        }

        // 更新当前图像为输出图像
        if (toolResult.outputImage && !toolResult.outputImage->isEmpty()) {
            currentImage = toolResult.outputImage;
        }

        LOG_DEBUG(QString("工具 %1 执行成功，耗时 %2 ms")
                 .arg(node.name)
                 .arg(toolResult.executionTime, 0, 'f', 2));
    }

    // 执行成功
    result.success = true;
    result.finalImage = currentImage;
    result.totalExecutionTime = timer.elapsed();

    LOG_INFO(QString("工具链执行完成，总耗时 %1 ms").arg(result.totalExecutionTime));

    return true;
}

bool ToolChain::executeUntil(const Base::ImageData::Ptr& input, int untilIndex, ToolChainResult& result)
{
    if (untilIndex < 0 || untilIndex >= static_cast<int>(nodes_.size())) {
        result.success = false;
        result.errorMessage = QString("索引越界: %1").arg(untilIndex);
        LOG_ERROR(result.errorMessage);
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    result = ToolChainResult();

    if (!input || input->isEmpty()) {
        result.success = false;
        result.errorMessage = "输入图像为空";
        LOG_ERROR(result.errorMessage);
        return false;
    }

    LOG_INFO(QString("执行工具链到索引 %1: %2").arg(untilIndex).arg(name_));

    // 执行到指定索引
    Base::ImageData::Ptr currentImage = input;

    for (int i = 0; i <= untilIndex; ++i) {
        const ToolChainNode& node = nodes_[static_cast<size_t>(i)];

        if (!node.enabled) {
            continue;
        }

        emit toolExecutionStarted(i, node.name);

        Algorithm::ToolResult toolResult;
        bool success = node.tool->process(currentImage, toolResult);

        result.toolResults.append(toolResult);

        emit toolExecutionFinished(i, success);

        if (!success) {
            result.success = false;
            result.errorMessage = QString("工具 '%1' 执行失败: %2")
                                .arg(node.name)
                                .arg(toolResult.errorMessage);
            result.failedToolIndex = i;
            LOG_ERROR(result.errorMessage);
            return false;
        }

        if (toolResult.outputImage && !toolResult.outputImage->isEmpty()) {
            currentImage = toolResult.outputImage;
        }
    }

    result.success = true;
    result.finalImage = currentImage;
    result.totalExecutionTime = timer.elapsed();

    return true;
}

bool ToolChain::executeSingle(const Base::ImageData::Ptr& input, int index, Algorithm::ToolResult& result)
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        result.success = false;
        result.errorMessage = QString("索引越界: %1").arg(index);
        LOG_ERROR(result.errorMessage);
        return false;
    }

    const ToolChainNode& node = nodes_[static_cast<size_t>(index)];

    if (!node.enabled) {
        result.success = false;
        result.errorMessage = QString("工具 '%1' 已禁用").arg(node.name);
        LOG_WARNING(result.errorMessage);
        return false;
    }

    LOG_DEBUG(QString("执行单个工具: %1").arg(node.name));

    return node.tool->process(input, result);
}

// ========== 序列化 ==========

QJsonObject ToolChain::serialize() const
{
    QJsonObject json;

    json["name"] = name_;
    json["description"] = description_;
    json["version"] = "1.0";

    // 序列化工具节点
    QJsonArray toolsArray;
    for (const ToolChainNode& node : nodes_) {
        QJsonObject nodeObj;
        nodeObj["id"] = node.id;
        nodeObj["name"] = node.name;
        nodeObj["enabled"] = node.enabled;
        nodeObj["toolType"] = static_cast<int>(node.tool->toolType());
        nodeObj["toolName"] = node.tool->toolName();
        nodeObj["params"] = node.tool->serializeParams();

        toolsArray.append(nodeObj);
    }
    json["tools"] = toolsArray;

    return json;
}

bool ToolChain::deserialize(const QJsonObject& json)
{
    // 清空现有工具链
    clear();

    // 加载基本信息
    if (json.contains("name")) {
        name_ = json["name"].toString();
    }

    if (json.contains("description")) {
        description_ = json["description"].toString();
    }

    // 加载工具节点
    if (!json.contains("tools")) {
        LOG_ERROR("反序列化失败：缺少tools字段");
        return false;
    }

    QJsonArray toolsArray = json["tools"].toArray();

    for (const QJsonValue& value : toolsArray) {
        QJsonObject nodeObj = value.toObject();

        // 创建工具实例
        int toolType = nodeObj["toolType"].toInt();
        Algorithm::VisionTool* tool = Algorithm::ToolFactory::instance().createTool(
            static_cast<Algorithm::VisionTool::ToolType>(toolType)
        );

        if (!tool) {
            LOG_ERROR(QString("创建工具失败：类型 %1").arg(toolType));
            continue;
        }

        // 恢复参数
        if (nodeObj.contains("params")) {
            tool->deserializeParams(nodeObj["params"].toObject());
        }

        // 添加到工具链
        QString nodeId = nodeObj["id"].toString();
        QString nodeName = nodeObj["name"].toString();
        bool enabled = nodeObj.value("enabled").toBool(true);

        // 接管工具所有权
        ToolChainNode newNode(std::unique_ptr<Algorithm::VisionTool>(tool), nodeId, nodeName);
        newNode.enabled = enabled;
        nodes_.push_back(std::move(newNode));
    }

    LOG_INFO(QString("工具链加载完成: %1 (%2个工具)").arg(name_).arg(static_cast<int>(nodes_.size())));

    emit structureChanged();
    return true;
}

bool ToolChain::saveToFile(const QString& filePath) const
{
    QJsonObject json = serialize();
    QJsonDocument doc(json);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("保存工具链失败：无法打开文件 %1").arg(filePath));
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("工具链已保存到: %1").arg(filePath));
    return true;
}

bool ToolChain::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("加载工具链失败：无法打开文件 %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("加载工具链失败：无效的JSON格式 %1").arg(filePath));
        return false;
    }

    bool success = deserialize(doc.object());

    if (success) {
        LOG_INFO(QString("工具链已从文件加载: %1").arg(filePath));
    }

    return success;
}

// ========== 属性 ==========

void ToolChain::setName(const QString& name)
{
    if (name_ != name) {
        name_ = name;
        emit nameChanged(name);
    }
}

void ToolChain::setDescription(const QString& description)
{
    description_ = description;
}

// ========== 私有方法 ==========

QString ToolChain::generateNodeId()
{
    return QString("node_%1").arg(++nodeIdCounter_);
}

} // namespace Core
} // namespace VisionForge
