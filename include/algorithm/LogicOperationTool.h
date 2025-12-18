/**
 * @file LogicOperationTool.h
 * @brief 逻辑运算工具 - 对多个输入进行逻辑运算
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QMap>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 逻辑运算类型
 */
enum class LogicType {
    AND,        // 与运算
    OR,         // 或运算
    NOT,        // 非运算 (单输入)
    XOR,        // 异或运算
    NAND,       // 与非运算
    NOR         // 或非运算
};

/**
 * @brief 逻辑输入项
 */
struct LogicInput {
    QString name;           // 输入名称
    QString sourceToolId;   // 来源工具ID
    QString valueKey;       // 值键名
    bool inverted = false;  // 是否取反
    bool enabled = true;    // 是否启用
    bool currentValue = false;  // 当前值 (运行时)
};

/**
 * @brief 逻辑运算结果
 */
struct LogicResult {
    bool result = false;        // 运算结果
    int trueCount = 0;          // 真值数量
    int falseCount = 0;         // 假值数量
    int totalInputs = 0;        // 总输入数
    QString expression;         // 运算表达式描述
};

/**
 * @brief 逻辑运算工具
 *
 * 支持对多个布尔输入进行逻辑运算:
 * - AND: 所有输入为真则结果为真
 * - OR: 任一输入为真则结果为真
 * - NOT: 单输入取反
 * - XOR: 奇数个输入为真则结果为真
 * - NAND: AND的取反
 * - NOR: OR的取反
 */
class LogicOperationTool : public VisionTool
{
    Q_OBJECT

public:
    explicit LogicOperationTool(QObject* parent = nullptr);
    virtual ~LogicOperationTool() = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::LogicOperation; }
    QString toolName() const override { return "逻辑运算"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    // ========== 参数设置 ==========

    /**
     * @brief 设置逻辑运算类型
     */
    void setLogicType(LogicType type);
    LogicType logicType() const { return logicType_; }

    /**
     * @brief 添加逻辑输入
     */
    void addInput(const LogicInput& input);

    /**
     * @brief 移除逻辑输入
     */
    void removeInput(int index);

    /**
     * @brief 清空所有输入
     */
    void clearInputs();

    /**
     * @brief 设置指定输入
     */
    void setInput(int index, const LogicInput& input);

    /**
     * @brief 获取所有输入
     */
    const std::vector<LogicInput>& inputs() const { return inputs_; }

    /**
     * @brief 设置输入值 (运行时)
     */
    void setInputValue(const QString& key, bool value);
    void setInputValues(const QMap<QString, bool>& values);

    // ========== 结果获取 ==========

    /**
     * @brief 获取运算结果
     */
    LogicResult result() const { return result_; }

    // ========== 序列化 ==========
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

signals:
    /**
     * @brief 运算完成信号
     */
    void operationCompleted(bool result);

private:
    /**
     * @brief 执行逻辑运算
     */
    bool executeLogic();

    /**
     * @brief 生成运算表达式描述
     */
    QString generateExpression() const;

    /**
     * @brief 绘制结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

private:
    LogicType logicType_;               // 逻辑运算类型
    std::vector<LogicInput> inputs_;    // 输入列表
    QMap<QString, bool> inputValues_;   // 输入值映射
    LogicResult result_;                // 运算结果
};

} // namespace Algorithm
} // namespace VisionForge
