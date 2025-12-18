/**
 * @file RangeJudgeTool.h
 * @brief 范围判定工具
 * @details 判断测量值是否在指定范围内，输出OK/NG结果
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 判定条件
 */
struct JudgeCondition {
    QString name;           // 条件名称
    QString sourceToolId;   // 数据来源工具ID
    QString valueKey;       // 值的键名
    double minValue = 0.0;  // 最小值
    double maxValue = 0.0;  // 最大值
    bool enabled = true;    // 是否启用
    bool result = false;    // 判定结果

    JudgeCondition() = default;
    JudgeCondition(const QString& n, double min, double max)
        : name(n), minValue(min), maxValue(max) {}
};

/**
 * @brief 判定结果
 */
struct JudgeResult {
    bool overallResult = false;         // 总体结果 (OK/NG)
    int passCount = 0;                  // 通过数量
    int failCount = 0;                  // 失败数量
    std::vector<JudgeCondition> details;// 详细结果
    QString description;                // 描述
};

/**
 * @class RangeJudgeTool
 * @brief 范围判定工具
 *
 * 支持的功能:
 * - 多条件组合判定 (AND/OR)
 * - 范围检查 (min <= value <= max)
 * - 比较检查 (>, <, ==, !=)
 * - 百分比容差
 */
class RangeJudgeTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 组合逻辑
     */
    enum CombineLogic {
        LogicAND,       // 所有条件都满足
        LogicOR         // 任一条件满足
    };
    Q_ENUM(CombineLogic)

    explicit RangeJudgeTool(QObject* parent = nullptr);
    ~RangeJudgeTool() override = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::RangeJudge; }
    QString toolName() const override { return "范围判定"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 判定设置 ==========

    /**
     * @brief 添加判定条件
     */
    void addCondition(const JudgeCondition& condition);

    /**
     * @brief 移除判定条件
     */
    void removeCondition(int index);

    /**
     * @brief 清空所有条件
     */
    void clearConditions();

    /**
     * @brief 获取所有条件
     */
    const std::vector<JudgeCondition>& conditions() const { return conditions_; }

    /**
     * @brief 修改条件
     */
    void setCondition(int index, const JudgeCondition& condition);

    /**
     * @brief 设置组合逻辑
     */
    void setCombineLogic(CombineLogic logic);
    CombineLogic combineLogic() const { return combineLogic_; }

    /**
     * @brief 设置输入值 (用于测试或手动输入)
     */
    void setInputValue(const QString& key, double value);
    void setInputValues(const QMap<QString, double>& values);

    /**
     * @brief 获取判定结果
     */
    const JudgeResult& result() const { return result_; }

signals:
    void judgementCompleted(bool result);

private:
    bool evaluateCondition(JudgeCondition& condition);
    cv::Mat drawResults(const cv::Mat& input) const;

private:
    std::vector<JudgeCondition> conditions_;
    CombineLogic combineLogic_;
    QMap<QString, double> inputValues_;
    JudgeResult result_;
};

} // namespace Algorithm
} // namespace VisionForge
