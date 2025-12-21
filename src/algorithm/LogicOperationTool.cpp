/**
 * @file LogicOperationTool.cpp
 * @brief 逻辑运算工具实现
 */

#include "algorithm/LogicOperationTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>

namespace VisionForge {
namespace Algorithm {

LogicOperationTool::LogicOperationTool(QObject* parent)
    : VisionTool(parent)
    , logicType_(LogicType::AND)
{
    setDisplayName(toolName());
}

bool LogicOperationTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    result_ = LogicResult();

    // 执行逻辑运算
    bool logicResult = executeLogic();

    result_.result = logicResult;
    result_.expression = generateExpression();

    // 设置输出
    output.success = true;
    output.executionTime = timer.elapsed();
    output.setValue("result", logicResult);
    output.setValue("trueCount", result_.trueCount);
    output.setValue("falseCount", result_.falseCount);
    output.setValue("totalInputs", result_.totalInputs);
    output.setValue("expression", result_.expression);

    // 绘制结果
    if (input && !input->isEmpty()) {
        cv::Mat resultImage = drawResults(input->mat());
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }
        setDebugImage(output.outputImage);
    }

    QString logicName;
    switch (logicType_) {
    case LogicType::AND:  logicName = "AND"; break;
    case LogicType::OR:   logicName = "OR"; break;
    case LogicType::NOT:  logicName = "NOT"; break;
    case LogicType::XOR:  logicName = "XOR"; break;
    case LogicType::NAND: logicName = "NAND"; break;
    case LogicType::NOR:  logicName = "NOR"; break;
    }

    setStatusText(QString("%1: %2 (%3真/%4假)")
                 .arg(logicName)
                 .arg(logicResult ? "TRUE" : "FALSE")
                 .arg(result_.trueCount)
                 .arg(result_.falseCount));

    LOG_DEBUG(QString("逻辑运算完成: %1 = %2, 耗时: %3ms")
             .arg(logicName).arg(logicResult).arg(output.executionTime));

    emit operationCompleted(logicResult);
    return true;
}

bool LogicOperationTool::executeLogic()
{
    // 收集启用的输入值
    std::vector<bool> values;
    int trueCount = 0;
    int falseCount = 0;

    for (auto& inp : inputs_) {
        if (!inp.enabled) continue;

        // 获取输入值
        bool value = false;
        if (!inp.valueKey.isEmpty() && inputValues_.contains(inp.valueKey)) {
            value = inputValues_[inp.valueKey];
        } else if (inputValues_.contains(inp.name)) {
            value = inputValues_[inp.name];
        }

        // 应用取反
        if (inp.inverted) {
            value = !value;
        }

        inp.currentValue = value;
        values.push_back(value);

        if (value) {
            trueCount++;
        } else {
            falseCount++;
        }
    }

    result_.trueCount = trueCount;
    result_.falseCount = falseCount;
    result_.totalInputs = static_cast<int>(values.size());

    // 如果没有输入，返回默认值
    if (values.empty()) {
        LOG_WARNING("逻辑运算没有有效输入");
        return false;
    }

    // 执行逻辑运算
    bool result = false;

    switch (logicType_) {
    case LogicType::AND:
        result = true;
        for (bool v : values) {
            result = result && v;
        }
        break;

    case LogicType::OR:
        result = false;
        for (bool v : values) {
            result = result || v;
        }
        break;

    case LogicType::NOT:
        // NOT只取第一个输入
        result = !values[0];
        break;

    case LogicType::XOR:
        // XOR: 奇数个真值时结果为真
        result = (trueCount % 2) == 1;
        break;

    case LogicType::NAND:
        result = true;
        for (bool v : values) {
            result = result && v;
        }
        result = !result;
        break;

    case LogicType::NOR:
        result = false;
        for (bool v : values) {
            result = result || v;
        }
        result = !result;
        break;
    }

    return result;
}

QString LogicOperationTool::generateExpression() const
{
    QStringList parts;
    for (const auto& inp : inputs_) {
        if (!inp.enabled) continue;

        QString part = inp.name;
        if (inp.inverted) {
            part = "!" + part;
        }
        part += QString("(%1)").arg(inp.currentValue ? "T" : "F");
        parts.append(part);
    }

    QString op;
    switch (logicType_) {
    case LogicType::AND:  op = " AND "; break;
    case LogicType::OR:   op = " OR "; break;
    case LogicType::NOT:  op = "NOT "; break;
    case LogicType::XOR:  op = " XOR "; break;
    case LogicType::NAND: op = " NAND "; break;
    case LogicType::NOR:  op = " NOR "; break;
    }

    if (logicType_ == LogicType::NOT) {
        return op + (parts.isEmpty() ? "?" : parts.first());
    }

    return parts.join(op) + " = " + (result_.result ? "TRUE" : "FALSE");
}

cv::Mat LogicOperationTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    // 绘制结果背景
    cv::Scalar bgColor = result_.result ?
                         cv::Scalar(0, 128, 0) : cv::Scalar(0, 0, 200);
    cv::rectangle(result, cv::Point(10, 10), cv::Point(250, 60), bgColor, -1);

    // 绘制结果文字
    QString resultText = result_.result ? "TRUE" : "FALSE";
    cv::putText(result, resultText.toStdString(),
               cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.2,
               cv::Scalar(255, 255, 255), 2);

    // 绘制逻辑类型
    QString logicName;
    switch (logicType_) {
    case LogicType::AND:  logicName = "AND"; break;
    case LogicType::OR:   logicName = "OR"; break;
    case LogicType::NOT:  logicName = "NOT"; break;
    case LogicType::XOR:  logicName = "XOR"; break;
    case LogicType::NAND: logicName = "NAND"; break;
    case LogicType::NOR:  logicName = "NOR"; break;
    }

    cv::putText(result, logicName.toStdString(),
               cv::Point(170, 50), cv::FONT_HERSHEY_SIMPLEX, 0.6,
               cv::Scalar(255, 255, 255), 1);

    // 绘制输入列表
    int y = 90;
    for (const auto& inp : inputs_) {
        if (!inp.enabled) continue;

        cv::Scalar color = inp.currentValue ?
                          cv::Scalar(0, 200, 0) : cv::Scalar(0, 0, 255);

        QString text = QString("%1%2: %3")
                      .arg(inp.inverted ? "!" : "")
                      .arg(inp.name)
                      .arg(inp.currentValue ? "TRUE" : "FALSE");

        cv::putText(result, text.toStdString(),
                   cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                   color, 1);
        y += 25;
    }

    // 绘制表达式
    cv::putText(result, result_.expression.toStdString(),
               cv::Point(10, result.rows - 20), cv::FONT_HERSHEY_SIMPLEX, 0.4,
               cv::Scalar(255, 255, 0), 1);

    return result;
}

void LogicOperationTool::setLogicType(LogicType type)
{
    if (logicType_ != type) {
        logicType_ = type;
        emit paramChanged();
    }
}

void LogicOperationTool::addInput(const LogicInput& input)
{
    inputs_.push_back(input);
    emit paramChanged();
}

void LogicOperationTool::removeInput(int index)
{
    if (index >= 0 && index < static_cast<int>(inputs_.size())) {
        inputs_.erase(inputs_.begin() + index);
        emit paramChanged();
    }
}

void LogicOperationTool::clearInputs()
{
    inputs_.clear();
    emit paramChanged();
}

void LogicOperationTool::setInput(int index, const LogicInput& input)
{
    if (index >= 0 && index < static_cast<int>(inputs_.size())) {
        inputs_[index] = input;
        emit paramChanged();
    }
}

void LogicOperationTool::setInputValue(const QString& key, bool value)
{
    inputValues_[key] = value;
}

void LogicOperationTool::setInputValues(const QMap<QString, bool>& values)
{
    inputValues_ = values;
}

QJsonObject LogicOperationTool::serializeParams() const
{
    QJsonObject json;
    json["logicType"] = static_cast<int>(logicType_);

    QJsonArray inputArray;
    for (const auto& inp : inputs_) {
        QJsonObject inpObj;
        inpObj["name"] = inp.name;
        inpObj["sourceToolId"] = inp.sourceToolId;
        inpObj["valueKey"] = inp.valueKey;
        inpObj["inverted"] = inp.inverted;
        inpObj["enabled"] = inp.enabled;
        inputArray.append(inpObj);
    }
    json["inputs"] = inputArray;

    return json;
}

void LogicOperationTool::deserializeParams(const QJsonObject& json)
{
    logicType_ = static_cast<LogicType>(json.value("logicType").toInt(0));

    inputs_.clear();
    QJsonArray inputArray = json.value("inputs").toArray();
    for (const auto& inpVal : inputArray) {
        QJsonObject inpObj = inpVal.toObject();
        LogicInput inp;
        inp.name = inpObj["name"].toString();
        inp.sourceToolId = inpObj["sourceToolId"].toString();
        inp.valueKey = inpObj["valueKey"].toString();
        inp.inverted = inpObj["inverted"].toBool();
        inp.enabled = inpObj["enabled"].toBool(true);
        inputs_.push_back(inp);
    }
}

} // namespace Algorithm
} // namespace VisionForge
