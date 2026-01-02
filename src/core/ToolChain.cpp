/**
 * @file ToolChain.cpp
 * @brief 视觉工具链核心实现文件
 * @details 本文件实现了VisionForge视觉检测系统的工具链管理功能，包括：
 *          - 工具的添加、插入、移除、移动等操作
 *          - 工具链的顺序执行、部分执行、单工具执行
 *          - 工具链配置的JSON序列化与反序列化
 *          - 工具链文件的保存与加载
 *          工具链是视觉检测流程的核心，按顺序执行各个视觉工具完成检测任务
 * @author VisionForge Team
 * @version 1.7.0
 * @date 2025-01-02
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 */

#include "core/ToolChain.h"
#include "algorithm/ToolFactory.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QElapsedTimer>

namespace VisionForge {
namespace Core {

/**
 * @brief 构造函数 - 初始化工具链对象
 * @param parent 父对象指针，用于Qt对象树内存管理
 * @details 初始化工具链名称和节点ID计数器
 */
ToolChain::ToolChain(QObject* parent)
    : QObject(parent)
    , name_("新建工具链")    // 默认工具链名称
    , nodeIdCounter_(0)      // 节点ID计数器初始化为0
{
}

/**
 * @brief 析构函数 - 清理工具链资源
 * @details 调用clear()释放所有工具节点
 */
ToolChain::~ToolChain()
{
    clear();
}

// ========== 工具链管理 ==========

/**
 * @brief 添加工具到工具链尾部
 * @param tool 要添加的视觉工具指针
 * @param name 工具节点名称，为空则使用工具默认名称
 * @return 新创建的节点ID，失败返回空字符串
 * @details 工具链接管工具的所有权，使用unique_ptr管理内存
 */
QString ToolChain::addTool(Algorithm::VisionTool* tool, const QString& name)
{
    // 验证工具指针有效性
    if (!tool) {
        LOG_ERROR("添加工具失败：工具指针为空");
        return QString();
    }

    // 生成唯一节点ID
    QString nodeId = generateNodeId();
    // 确定节点名称：优先使用传入名称，否则使用工具默认名称
    QString nodeName = name.isEmpty() ? tool->toolName() : name;

    // 使用unique_ptr接管工具所有权，添加到工具链尾部
    nodes_.push_back(ToolChainNode(std::unique_ptr<Algorithm::VisionTool>(tool), nodeId, nodeName));

    LOG_DEBUG(QString("添加工具到链尾: %1 [%2]").arg(nodeName).arg(nodeId));

    // 发送结构变化信号
    emit structureChanged();
    return nodeId;
}

/**
 * @brief 在指定位置插入工具
 * @param index 插入位置索引
 * @param tool 要插入的视觉工具指针
 * @param name 工具节点名称，为空则使用工具默认名称
 * @return 新创建的节点ID，失败返回空字符串
 * @details 在指定索引位置插入工具，后续工具自动后移
 */
QString ToolChain::insertTool(int index, Algorithm::VisionTool* tool, const QString& name)
{
    // 验证工具指针有效性
    if (!tool) {
        LOG_ERROR("插入工具失败：工具指针为空");
        return QString();
    }

    // 验证索引范围（允许在末尾插入，所以上限是size()）
    if (index < 0 || index > static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("插入工具失败：索引越界 %1").arg(index));
        return QString();
    }

    // 生成唯一节点ID和名称
    QString nodeId = generateNodeId();
    QString nodeName = name.isEmpty() ? tool->toolName() : name;

    // 在指定位置插入工具节点
    nodes_.insert(nodes_.begin() + index, ToolChainNode(std::unique_ptr<Algorithm::VisionTool>(tool), nodeId, nodeName));

    LOG_DEBUG(QString("插入工具到位置 %1: %2 [%3]").arg(index).arg(nodeName).arg(nodeId));

    // 发送结构变化信号
    emit structureChanged();
    return nodeId;
}

/**
 * @brief 按节点ID移除工具
 * @param nodeId 要移除的节点ID
 * @return 移除成功返回true，失败返回false
 */
bool ToolChain::removeTool(const QString& nodeId)
{
    // 查找节点索引
    int index = findNodeIndex(nodeId);
    if (index < 0) {
        LOG_ERROR(QString("移除工具失败：未找到节点 %1").arg(nodeId));
        return false;
    }

    // 调用按索引移除的方法
    return removeToolAt(index);
}

/**
 * @brief 按索引位置移除工具
 * @param index 要移除的工具索引
 * @return 移除成功返回true，失败返回false
 * @details unique_ptr会自动释放工具实例内存
 */
bool ToolChain::removeToolAt(int index)
{
    // 验证索引范围
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("移除工具失败：索引越界 %1").arg(index));
        return false;
    }

    LOG_DEBUG(QString("移除工具: %1 [%2]").arg(nodes_[index].name).arg(nodes_[index].id));

    // 从vector中删除节点，unique_ptr会自动释放工具内存
    nodes_.erase(nodes_.begin() + index);

    // 发送结构变化信号
    emit structureChanged();
    return true;
}

/**
 * @brief 移动工具位置
 * @param fromIndex 源位置索引
 * @param toIndex 目标位置索引
 * @return 移动成功返回true，失败返回false
 * @details 使用移动语义高效重新排列工具节点顺序
 */
bool ToolChain::moveTool(int fromIndex, int toIndex)
{
    // 验证源索引范围
    if (fromIndex < 0 || fromIndex >= static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("移动工具失败：源索引越界 %1").arg(fromIndex));
        return false;
    }

    // 验证目标索引范围
    if (toIndex < 0 || toIndex >= static_cast<int>(nodes_.size())) {
        LOG_ERROR(QString("移动工具失败：目标索引越界 %1").arg(toIndex));
        return false;
    }

    // 源和目标相同时无需移动
    if (fromIndex == toIndex) {
        return true;
    }

    // 使用移动语义重新排列节点
    // 步骤：先取出源节点，删除原位置，再插入目标位置
    ToolChainNode node = std::move(nodes_[fromIndex]);
    nodes_.erase(nodes_.begin() + fromIndex);
    nodes_.insert(nodes_.begin() + toIndex, std::move(node));

    LOG_DEBUG(QString("移动工具: %1 -> %2").arg(fromIndex).arg(toIndex));

    // 发送结构变化信号
    emit structureChanged();
    return true;
}

/**
 * @brief 清空工具链
 * @details 删除所有工具节点，unique_ptr自动释放内存
 */
void ToolChain::clear()
{
    // 清空节点容器，unique_ptr自动释放所有工具实例
    nodes_.clear();

    LOG_DEBUG("工具链已清空");

    // 发送结构变化信号
    emit structureChanged();
}

/**
 * @brief 按索引获取工具
 * @param index 工具索引
 * @return 工具指针，索引无效返回nullptr
 */
Algorithm::VisionTool* ToolChain::getTool(int index) const
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        return nullptr;
    }
    return nodes_[index].tool.get();
}

/**
 * @brief 按节点ID获取工具
 * @param nodeId 节点ID
 * @return 工具指针，未找到返回nullptr
 */
Algorithm::VisionTool* ToolChain::getTool(const QString& nodeId) const
{
    int index = findNodeIndex(nodeId);
    if (index < 0) {
        return nullptr;
    }
    return nodes_[index].tool.get();
}

/**
 * @brief 获取工具链节点
 * @param index 节点索引
 * @return 节点指针，索引无效返回nullptr
 */
const ToolChainNode* ToolChain::getNode(int index) const
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        return nullptr;
    }
    return &nodes_[index];
}

/**
 * @brief 获取节点ID
 * @param index 节点索引
 * @return 节点ID，索引无效返回空字符串
 */
QString ToolChain::getNodeId(int index) const
{
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        return QString();
    }
    return nodes_[index].id;
}

/**
 * @brief 查找节点索引
 * @param nodeId 要查找的节点ID
 * @return 节点索引，未找到返回-1
 * @details 遍历所有节点查找匹配的ID
 */
int ToolChain::findNodeIndex(const QString& nodeId) const
{
    for (size_t i = 0; i < nodes_.size(); ++i) {
        if (nodes_[i].id == nodeId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/**
 * @brief 设置节点启用状态
 * @param nodeId 节点ID
 * @param enabled 是否启用
 * @details 禁用的节点在执行时会被跳过
 */
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

/**
 * @brief 设置节点名称
 * @param nodeId 节点ID
 * @param name 新名称
 */
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

/**
 * @brief 执行完整工具链
 * @param input 输入图像数据
 * @param result 输出执行结果
 * @return 执行成功返回true，失败返回false
 * @details 执行算法流程：
 *          1. 验证输入图像和工具链有效性
 *          2. 按顺序执行每个启用的工具
 *          3. 每个工具的输出图像作为下一个工具的输入
 *          4. 任一工具失败则整体失败
 *          5. 统计总执行时间
 */
bool ToolChain::execute(const Base::ImageData::Ptr& input, ToolChainResult& result)
{
    // 创建计时器统计执行时间
    QElapsedTimer timer;
    timer.start();

    // 初始化结果结构
    result = ToolChainResult();

    // 验证输入图像有效性
    if (!input || input->isEmpty()) {
        result.success = false;
        result.errorMessage = "输入图像为空";
        LOG_ERROR(result.errorMessage);
        return false;
    }

    // 验证工具链非空
    if (nodes_.empty()) {
        result.success = false;
        result.errorMessage = "工具链为空";
        LOG_ERROR(result.errorMessage);
        return false;
    }

    LOG_INFO(QString("开始执行工具链: %1 (%2个工具)").arg(name_).arg(static_cast<int>(nodes_.size())));

    // 执行工具链 - 流水线模式，每个工具的输出作为下一个的输入
    Base::ImageData::Ptr currentImage = input;
    result.toolResults.reserve(static_cast<int>(nodes_.size()));

    for (size_t i = 0; i < nodes_.size(); ++i) {
        const ToolChainNode& node = nodes_[i];

        // 跳过禁用的工具
        if (!node.enabled) {
            LOG_DEBUG(QString("跳过禁用的工具: %1").arg(node.name));
            continue;
        }

        // 发送工具开始执行信号
        emit toolExecutionStarted(static_cast<int>(i), node.name);
        // 发送执行进度信号
        emit executionProgress(static_cast<int>(i) + 1, static_cast<int>(nodes_.size()));

        // 执行当前工具
        Algorithm::ToolResult toolResult;
        bool success = node.tool->process(currentImage, toolResult);

        // 保存工具执行结果
        result.toolResults.append(toolResult);

        // 发送工具执行完成信号
        emit toolExecutionFinished(static_cast<int>(i), success);

        // 处理执行失败情况
        if (!success) {
            result.success = false;
            result.errorMessage = QString("工具 '%1' 执行失败: %2")
                                .arg(node.name)
                                .arg(toolResult.errorMessage);
            result.failedToolIndex = static_cast<int>(i);

            LOG_ERROR(result.errorMessage);
            return false;
        }

        // 更新当前图像为输出图像（用于下一个工具）
        // 如果工具没有输出图像，继续使用之前的图像
        if (toolResult.outputImage && !toolResult.outputImage->isEmpty()) {
            currentImage = toolResult.outputImage;
        }

        LOG_DEBUG(QString("工具 %1 执行成功，耗时 %2 ms")
                 .arg(node.name)
                 .arg(toolResult.executionTime, 0, 'f', 2));
    }

    // 所有工具执行成功，设置结果
    result.success = true;
    result.finalImage = currentImage;
    result.totalExecutionTime = timer.elapsed();

    LOG_INFO(QString("工具链执行完成，总耗时 %1 ms").arg(result.totalExecutionTime));

    return true;
}

/**
 * @brief 执行工具链到指定索引位置
 * @param input 输入图像数据
 * @param untilIndex 执行到的工具索引（包含）
 * @param result 输出执行结果
 * @return 执行成功返回true，失败返回false
 * @details 用于调试时执行部分工具链，查看中间结果
 */
bool ToolChain::executeUntil(const Base::ImageData::Ptr& input, int untilIndex, ToolChainResult& result)
{
    // 验证目标索引有效性
    if (untilIndex < 0 || untilIndex >= static_cast<int>(nodes_.size())) {
        result.success = false;
        result.errorMessage = QString("索引越界: %1").arg(untilIndex);
        LOG_ERROR(result.errorMessage);
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    result = ToolChainResult();

    // 验证输入图像
    if (!input || input->isEmpty()) {
        result.success = false;
        result.errorMessage = "输入图像为空";
        LOG_ERROR(result.errorMessage);
        return false;
    }

    LOG_INFO(QString("执行工具链到索引 %1: %2").arg(untilIndex).arg(name_));

    // 执行到指定索引（包含该索引的工具）
    Base::ImageData::Ptr currentImage = input;

    for (int i = 0; i <= untilIndex; ++i) {
        const ToolChainNode& node = nodes_[static_cast<size_t>(i)];

        // 跳过禁用的工具
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

        // 更新当前图像
        if (toolResult.outputImage && !toolResult.outputImage->isEmpty()) {
            currentImage = toolResult.outputImage;
        }
    }

    result.success = true;
    result.finalImage = currentImage;
    result.totalExecutionTime = timer.elapsed();

    return true;
}

/**
 * @brief 执行单个工具
 * @param input 输入图像数据
 * @param index 要执行的工具索引
 * @param result 输出执行结果
 * @return 执行成功返回true，失败返回false
 * @details 用于单独测试某个工具的执行效果
 */
bool ToolChain::executeSingle(const Base::ImageData::Ptr& input, int index, Algorithm::ToolResult& result)
{
    // 验证索引有效性
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        result.success = false;
        result.errorMessage = QString("索引越界: %1").arg(index);
        LOG_ERROR(result.errorMessage);
        return false;
    }

    const ToolChainNode& node = nodes_[static_cast<size_t>(index)];

    // 检查工具是否启用
    if (!node.enabled) {
        result.success = false;
        result.errorMessage = QString("工具 '%1' 已禁用").arg(node.name);
        LOG_WARNING(result.errorMessage);
        return false;
    }

    LOG_DEBUG(QString("执行单个工具: %1").arg(node.name));

    // 执行工具处理
    return node.tool->process(input, result);
}

// ========== 序列化 ==========

/**
 * @brief 将工具链序列化为JSON对象
 * @return 包含工具链完整配置的JSON对象
 * @details 序列化内容包括：
 *          - 工具链基本信息（名称、描述、版本）
 *          - 所有工具节点（ID、名称、启用状态、类型、参数）
 */
QJsonObject ToolChain::serialize() const
{
    QJsonObject json;

    // 序列化工具链基本信息
    json["name"] = name_;
    json["description"] = description_;
    json["version"] = "1.0";

    // 序列化所有工具节点
    QJsonArray toolsArray;
    for (const ToolChainNode& node : nodes_) {
        QJsonObject nodeObj;
        nodeObj["id"] = node.id;
        nodeObj["name"] = node.name;
        nodeObj["enabled"] = node.enabled;
        nodeObj["toolType"] = static_cast<int>(node.tool->toolType());
        nodeObj["toolName"] = node.tool->toolName();
        // 序列化工具特定参数
        nodeObj["params"] = node.tool->serializeParams();

        toolsArray.append(nodeObj);
    }
    json["tools"] = toolsArray;

    return json;
}

/**
 * @brief 从JSON对象反序列化工具链配置
 * @param json 包含工具链配置的JSON对象
 * @return 反序列化成功返回true，失败返回false
 * @details 按照序列化的逆过程恢复工具链状态，
 *          使用ToolFactory根据类型创建工具实例
 */
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

    // 遍历并创建每个工具节点
    for (const QJsonValue& value : toolsArray) {
        QJsonObject nodeObj = value.toObject();

        // 使用工厂模式根据类型创建工具实例
        int toolType = nodeObj["toolType"].toInt();
        Algorithm::VisionTool* tool = Algorithm::ToolFactory::instance().createTool(
            static_cast<Algorithm::VisionTool::ToolType>(toolType)
        );

        if (!tool) {
            LOG_ERROR(QString("创建工具失败：类型 %1").arg(toolType));
            continue;
        }

        // 恢复工具参数
        if (nodeObj.contains("params")) {
            tool->deserializeParams(nodeObj["params"].toObject());
        }

        // 恢复节点属性并添加到工具链
        QString nodeId = nodeObj["id"].toString();
        QString nodeName = nodeObj["name"].toString();
        bool enabled = nodeObj.value("enabled").toBool(true);

        // 创建节点并接管工具所有权
        ToolChainNode newNode(std::unique_ptr<Algorithm::VisionTool>(tool), nodeId, nodeName);
        newNode.enabled = enabled;
        nodes_.push_back(std::move(newNode));
    }

    LOG_INFO(QString("工具链加载完成: %1 (%2个工具)").arg(name_).arg(static_cast<int>(nodes_.size())));

    // 发送结构变化信号
    emit structureChanged();
    return true;
}

/**
 * @brief 将工具链保存到文件
 * @param filePath 目标文件路径
 * @return 保存成功返回true，失败返回false
 */
bool ToolChain::saveToFile(const QString& filePath) const
{
    // 序列化工具链
    QJsonObject json = serialize();
    QJsonDocument doc(json);

    // 打开文件写入
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("保存工具链失败：无法打开文件 %1").arg(filePath));
        return false;
    }

    // 写入格式化的JSON
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("工具链已保存到: %1").arg(filePath));
    return true;
}

/**
 * @brief 从文件加载工具链
 * @param filePath 源文件路径
 * @return 加载成功返回true，失败返回false
 */
bool ToolChain::loadFromFile(const QString& filePath)
{
    // 打开文件读取
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("加载工具链失败：无法打开文件 %1").arg(filePath));
        return false;
    }

    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();

    // 解析JSON文档
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        LOG_ERROR(QString("加载工具链失败：无效的JSON格式 %1").arg(filePath));
        return false;
    }

    // 反序列化工具链
    bool success = deserialize(doc.object());

    if (success) {
        LOG_INFO(QString("工具链已从文件加载: %1").arg(filePath));
    }

    return success;
}

// ========== 属性 ==========

/**
 * @brief 设置工具链名称
 * @param name 新名称
 */
void ToolChain::setName(const QString& name)
{
    if (name_ != name) {
        name_ = name;
        emit nameChanged(name);
    }
}

/**
 * @brief 设置工具链描述
 * @param description 新描述
 */
void ToolChain::setDescription(const QString& description)
{
    description_ = description;
}

// ========== 私有方法 ==========

/**
 * @brief 生成唯一节点ID
 * @return 格式为 "node_N" 的唯一ID字符串
 * @details 使用递增计数器确保ID唯一性
 */
QString ToolChain::generateNodeId()
{
    return QString("node_%1").arg(++nodeIdCounter_);
}

} // namespace Core
} // namespace VisionForge
