/**
 * @file RangeJudgeTool.cpp
 * @brief 范围判定工具实现
 */

#include "algorithm/RangeJudgeTool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>

namespace VisionForge {
namespace Algorithm {

RangeJudgeTool::RangeJudgeTool(QObject* parent)
    : VisionTool(parent)
    , combineLogic_(LogicAND)
{
    setDisplayName(toolName());
}

bool RangeJudgeTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    result_ = JudgeResult();
    result_.details = conditions_;

    // 评估每个条件
    int passCount = 0;
    int failCount = 0;

    for (auto& condition : result_.details) {
        if (!condition.enabled) {
            continue;
        }

        condition.result = evaluateCondition(condition);

        if (condition.result) {
            passCount++;
        } else {
            failCount++;
        }
    }

    result_.passCount = passCount;
    result_.failCount = failCount;

    // 根据组合逻辑计算总体结果
    int enabledCount = 0;
    for (const auto& c : conditions_) {
        if (c.enabled) enabledCount++;
    }

    if (enabledCount == 0) {
        result_.overallResult = true;  // 无条件默认通过
    } else if (combineLogic_ == LogicAND) {
        result_.overallResult = (failCount == 0);
    } else {  // LogicOR
        result_.overallResult = (passCount > 0);
    }

    result_.description = result_.overallResult ? "OK" : "NG";

    // 设置输出
    output.success = true;
    output.executionTime = timer.elapsed();
    output.setValue("result", result_.overallResult);
    output.setValue("passCount", passCount);
    output.setValue("failCount", failCount);
    output.setValue("description", result_.description);

    // 绘制结果
    if (input && !input->isEmpty()) {
        cv::Mat resultImage = drawResults(input->mat());
        output.outputImage = std::make_shared<Base::ImageData>(resultImage);
        setDebugImage(output.outputImage);
    }

    setStatusText(QString("%1 (通过:%2 失败:%3)")
                 .arg(result_.description)
                 .arg(passCount)
                 .arg(failCount));

    LOG_DEBUG(QString("范围判定完成: %1, 耗时: %2ms")
             .arg(result_.description).arg(output.executionTime));

    emit judgementCompleted(result_.overallResult);
    return true;
}

bool RangeJudgeTool::evaluateCondition(JudgeCondition& condition)
{
    // 获取输入值
    double value = 0.0;

    if (!condition.valueKey.isEmpty() && inputValues_.contains(condition.valueKey)) {
        value = inputValues_[condition.valueKey];
    } else if (inputValues_.contains(condition.name)) {
        value = inputValues_[condition.name];
    } else {
        // 如果没有找到对应的值，默认失败
        LOG_WARNING(QString("未找到判定条件的输入值: %1").arg(condition.name));
        return false;
    }

    // 判断是否在范围内
    bool inRange = (value >= condition.minValue && value <= condition.maxValue);

    return inRange;
}

cv::Mat RangeJudgeTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 绘制总体结果
    cv::Scalar bgColor = result_.overallResult ?
                         cv::Scalar(0, 128, 0) : cv::Scalar(0, 0, 200);
    cv::rectangle(result, cv::Point(10, 10), cv::Point(200, 60), bgColor, -1);

    QString resultText = result_.overallResult ? "OK" : "NG";
    cv::putText(result, resultText.toStdString(),
               cv::Point(70, 50), cv::FONT_HERSHEY_SIMPLEX, 1.5,
               cv::Scalar(255, 255, 255), 3);

    // 绘制详细条件
    int y = 90;
    for (const auto& cond : result_.details) {
        if (!cond.enabled) continue;

        cv::Scalar color = cond.result ?
                          cv::Scalar(0, 200, 0) : cv::Scalar(0, 0, 255);

        QString text = QString("%1: [%.2f, %.2f] %3")
                      .arg(cond.name)
                      .arg(cond.minValue)
                      .arg(cond.maxValue)
                      .arg(cond.result ? "PASS" : "FAIL");

        cv::putText(result, text.toStdString(),
                   cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                   color, 1);
        y += 25;
    }

    return result;
}

void RangeJudgeTool::addCondition(const JudgeCondition& condition)
{
    conditions_.push_back(condition);
    emit paramChanged();
}

void RangeJudgeTool::removeCondition(int index)
{
    if (index >= 0 && index < static_cast<int>(conditions_.size())) {
        conditions_.erase(conditions_.begin() + index);
        emit paramChanged();
    }
}

void RangeJudgeTool::clearConditions()
{
    conditions_.clear();
    emit paramChanged();
}

void RangeJudgeTool::setCondition(int index, const JudgeCondition& condition)
{
    if (index >= 0 && index < static_cast<int>(conditions_.size())) {
        conditions_[index] = condition;
        emit paramChanged();
    }
}

void RangeJudgeTool::setCombineLogic(CombineLogic logic)
{
    if (combineLogic_ != logic) {
        combineLogic_ = logic;
        emit paramChanged();
    }
}

void RangeJudgeTool::setInputValue(const QString& key, double value)
{
    inputValues_[key] = value;
}

void RangeJudgeTool::setInputValues(const QMap<QString, double>& values)
{
    inputValues_ = values;
}

QJsonObject RangeJudgeTool::serializeParams() const
{
    QJsonObject json;
    json["combineLogic"] = static_cast<int>(combineLogic_);

    QJsonArray condArray;
    for (const auto& cond : conditions_) {
        QJsonObject condObj;
        condObj["name"] = cond.name;
        condObj["sourceToolId"] = cond.sourceToolId;
        condObj["valueKey"] = cond.valueKey;
        condObj["minValue"] = cond.minValue;
        condObj["maxValue"] = cond.maxValue;
        condObj["enabled"] = cond.enabled;
        condArray.append(condObj);
    }
    json["conditions"] = condArray;

    return json;
}

void RangeJudgeTool::deserializeParams(const QJsonObject& json)
{
    combineLogic_ = static_cast<CombineLogic>(json.value("combineLogic").toInt(0));

    conditions_.clear();
    QJsonArray condArray = json.value("conditions").toArray();
    for (const auto& condVal : condArray) {
        QJsonObject condObj = condVal.toObject();
        JudgeCondition cond;
        cond.name = condObj["name"].toString();
        cond.sourceToolId = condObj["sourceToolId"].toString();
        cond.valueKey = condObj["valueKey"].toString();
        cond.minValue = condObj["minValue"].toDouble();
        cond.maxValue = condObj["maxValue"].toDouble();
        cond.enabled = condObj["enabled"].toBool(true);
        conditions_.push_back(cond);
    }
}

} // namespace Algorithm
} // namespace VisionForge
