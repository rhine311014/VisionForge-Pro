/**
 * @file ToolChain.h
 * @brief 视觉工具链管理
 * @details 管理视觉工具的执行序列，支持保存/加载配置
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "base/ImageData.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Core {

/**
 * @brief 工具链节点
 */
struct ToolChainNode {
    std::unique_ptr<Algorithm::VisionTool> tool;  // 工具实例（智能指针管理生命周期）
    QString id;                         // 节点ID
    QString name;                       // 显示名称
    bool enabled;                       // 是否启用

    ToolChainNode()
        : tool(nullptr)
        , enabled(true)
    {}

    // 接管工具所有权的构造函数
    ToolChainNode(std::unique_ptr<Algorithm::VisionTool> t, const QString& nodeId, const QString& nodeName)
        : tool(std::move(t))
        , id(nodeId)
        , name(nodeName)
        , enabled(true)
    {}

    // 支持移动语义
    ToolChainNode(ToolChainNode&& other) noexcept = default;
    ToolChainNode& operator=(ToolChainNode&& other) noexcept = default;

    // 禁止拷贝
    ToolChainNode(const ToolChainNode&) = delete;
    ToolChainNode& operator=(const ToolChainNode&) = delete;
};

/**
 * @brief 工具链执行结果
 */
struct ToolChainResult {
    bool success;                               // 是否成功
    QString errorMessage;                       // 错误消息
    int failedToolIndex;                        // 失败工具的索引（-1表示无失败）
    QList<Algorithm::ToolResult> toolResults;   // 各工具的执行结果
    Base::ImageData::Ptr finalImage;            // 最终输出图像
    double totalExecutionTime;                  // 总执行时间(毫秒)

    ToolChainResult()
        : success(false)
        , failedToolIndex(-1)
        , totalExecutionTime(0.0)
    {}
};

/**
 * @brief 工具链类
 *
 * 管理多个视觉工具的有序执行
 */
class ToolChain : public QObject {
    Q_OBJECT

public:
    explicit ToolChain(QObject* parent = nullptr);
    ~ToolChain();

    // ========== 工具链管理 ==========

    /**
     * @brief 添加工具到链尾
     * @param tool 工具实例（工具链负责管理其生命周期）
     * @param name 显示名称
     * @return 节点ID
     */
    QString addTool(Algorithm::VisionTool* tool, const QString& name = QString());

    /**
     * @brief 在指定位置插入工具
     * @param index 插入位置
     * @param tool 工具实例
     * @param name 显示名称
     * @return 节点ID
     */
    QString insertTool(int index, Algorithm::VisionTool* tool, const QString& name = QString());

    /**
     * @brief 移除工具
     * @param nodeId 节点ID
     * @return true如果成功
     */
    bool removeTool(const QString& nodeId);

    /**
     * @brief 根据索引移除工具
     * @param index 工具索引
     * @return true如果成功
     */
    bool removeToolAt(int index);

    /**
     * @brief 移动工具位置
     * @param fromIndex 源索引
     * @param toIndex 目标索引
     * @return true如果成功
     */
    bool moveTool(int fromIndex, int toIndex);

    /**
     * @brief 清空工具链
     */
    void clear();

    /**
     * @brief 获取工具数量
     */
    int toolCount() const { return static_cast<int>(nodes_.size()); }

    /**
     * @brief 根据索引获取工具
     */
    Algorithm::VisionTool* getTool(int index) const;

    /**
     * @brief 根据ID获取工具
     */
    Algorithm::VisionTool* getTool(const QString& nodeId) const;

    /**
     * @brief 获取节点
     */
    const ToolChainNode* getNode(int index) const;

    /**
     * @brief 获取节点ID
     */
    QString getNodeId(int index) const;

    /**
     * @brief 查找节点索引
     */
    int findNodeIndex(const QString& nodeId) const;

    /**
     * @brief 设置节点启用状态
     */
    void setNodeEnabled(const QString& nodeId, bool enabled);

    /**
     * @brief 设置节点名称
     */
    void setNodeName(const QString& nodeId, const QString& name);

    // ========== 工具链执行 ==========

    /**
     * @brief 执行工具链
     * @param input 输入图像
     * @param result 执行结果
     * @return true如果全部成功
     */
    bool execute(const Base::ImageData::Ptr& input, ToolChainResult& result);

    /**
     * @brief 执行到指定工具
     * @param input 输入图像
     * @param untilIndex 执行到的索引（包含）
     * @param result 执行结果
     * @return true如果成功
     */
    bool executeUntil(const Base::ImageData::Ptr& input, int untilIndex, ToolChainResult& result);

    /**
     * @brief 执行单个工具（调试用）
     * @param input 输入图像
     * @param index 工具索引
     * @param result 工具结果
     * @return true如果成功
     */
    bool executeSingle(const Base::ImageData::Ptr& input, int index, Algorithm::ToolResult& result);

    // ========== 序列化 ==========

    /**
     * @brief 序列化工具链配置
     */
    QJsonObject serialize() const;

    /**
     * @brief 反序列化工具链配置
     */
    bool deserialize(const QJsonObject& json);

    /**
     * @brief 保存到文件
     */
    bool saveToFile(const QString& filePath) const;

    /**
     * @brief 从文件加载
     */
    bool loadFromFile(const QString& filePath);

    // ========== 属性 ==========

    /**
     * @brief 获取工具链名称
     */
    QString name() const { return name_; }

    /**
     * @brief 设置工具链名称
     */
    void setName(const QString& name);

    /**
     * @brief 获取描述
     */
    QString description() const { return description_; }

    /**
     * @brief 设置描述
     */
    void setDescription(const QString& description);

signals:
    /**
     * @brief 工具链结构改变信号
     */
    void structureChanged();

    /**
     * @brief 工具执行开始信号
     * @param index 工具索引
     * @param name 工具名称
     */
    void toolExecutionStarted(int index, const QString& name);

    /**
     * @brief 工具执行完成信号
     * @param index 工具索引
     * @param success 是否成功
     */
    void toolExecutionFinished(int index, bool success);

    /**
     * @brief 工具链执行进度信号
     * @param current 当前进度
     * @param total 总数
     */
    void executionProgress(int current, int total);

    /**
     * @brief 名称改变信号
     */
    void nameChanged(const QString& name);

private:
    QString generateNodeId();

private:
    QString name_;                      // 工具链名称
    QString description_;               // 描述
    std::vector<ToolChainNode> nodes_;  // 工具节点列表（使用std::vector支持移动语义）
    int nodeIdCounter_;                 // 节点ID计数器
};

} // namespace Core
} // namespace VisionForge
