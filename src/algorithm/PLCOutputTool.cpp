/**
 * @file PLCOutputTool.cpp
 * @brief PLC输出工具实现
 */

#include "algorithm/PLCOutputTool.h"
#include "comm/PLCComm.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>

namespace VisionForge {
namespace Algorithm {

PLCOutputTool::PLCOutputTool(QObject* parent)
    : VisionTool(parent)
    , judgeResult_(true)
    , resultAddress_(0)
    , completeAddress_(1)
    , outputResult_(true)
    , outputComplete_(true)
{
    setDisplayName(toolName());
}

bool PLCOutputTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    bool allSuccess = true;
    int writeCount = 0;
    int failCount = 0;

    // 检查PLC连接
    if (!plcComm_ || !plcComm_->isConnected()) {
        output.success = false;
        output.errorMessage = "PLC未连接";
        emit outputError(output.errorMessage);
        return false;
    }

    // 输出OK/NG结果
    if (outputResult_) {
        if (writeCoil(resultAddress_, judgeResult_)) {
            writeCount++;
            LOG_DEBUG(QString("写入结果: 地址%1 = %2")
                     .arg(resultAddress_).arg(judgeResult_ ? "OK" : "NG"));
        } else {
            failCount++;
            allSuccess = false;
        }
    }

    // 输出映射数据
    for (const auto& mapping : mappings_) {
        if (!mapping.enabled) continue;

        QVariant value = inputValues_.value(mapping.sourceKey);
        if (!value.isValid()) {
            LOG_WARNING(QString("未找到输入值: %1").arg(mapping.sourceKey));
            continue;
        }

        if (mapping.isBit) {
            // 位输出
            bool bitValue = value.toBool();
            if (writeCoil(mapping.address, bitValue)) {
                writeCount++;
            } else {
                failCount++;
                allSuccess = false;
            }
        } else {
            // 字输出 (带缩放和偏移)
            double numValue = value.toDouble();
            int intValue = static_cast<int>(numValue * mapping.scale + mapping.offset);

            if (writeRegister(mapping.address, intValue)) {
                writeCount++;
            } else {
                failCount++;
                allSuccess = false;
            }
        }
    }

    // 输出完成信号
    if (outputComplete_) {
        if (writeCoil(completeAddress_, true)) {
            writeCount++;
            LOG_DEBUG(QString("写入完成信号: 地址%1").arg(completeAddress_));
        } else {
            failCount++;
            allSuccess = false;
        }
    }

    // 设置输出
    output.success = allSuccess;
    output.executionTime = timer.elapsed();
    output.setValue("writeCount", writeCount);
    output.setValue("failCount", failCount);
    output.setValue("result", judgeResult_);

    if (!allSuccess) {
        output.errorMessage = QString("部分写入失败: %1/%2")
                             .arg(failCount).arg(writeCount + failCount);
    }

    // 传递输入图像
    output.outputImage = input;

    setStatusText(QString("写入%1项 %2")
                 .arg(writeCount)
                 .arg(allSuccess ? "成功" : "部分失败"));

    LOG_DEBUG(QString("PLC输出完成: %1项成功, %2项失败, 耗时: %3ms")
             .arg(writeCount).arg(failCount).arg(output.executionTime));

    emit outputCompleted(allSuccess);
    return allSuccess;
}

bool PLCOutputTool::writeRegister(int address, int value)
{
    if (!plcComm_) return false;

    std::vector<int> values = { value };
    Comm::PLCResult result = plcComm_->writeRegisters(address, values);

    if (!result.isSuccess()) {
        LOG_ERROR(QString("写入寄存器失败: 地址%1, 错误: %2")
                 .arg(address).arg(result.errorMessage));
        return false;
    }
    return true;
}

bool PLCOutputTool::writeCoil(int address, bool value)
{
    if (!plcComm_) return false;

    std::vector<bool> values = { value };
    Comm::PLCResult result = plcComm_->writeCoils(address, values);

    if (!result.isSuccess()) {
        LOG_ERROR(QString("写入线圈失败: 地址%1, 错误: %2")
                 .arg(address).arg(result.errorMessage));
        return false;
    }
    return true;
}

void PLCOutputTool::setPLCComm(std::shared_ptr<Comm::PLCComm> plc)
{
    plcComm_ = plc;
}

void PLCOutputTool::addMapping(const OutputMapping& mapping)
{
    mappings_.push_back(mapping);
    emit paramChanged();
}

void PLCOutputTool::removeMapping(int index)
{
    if (index >= 0 && index < static_cast<int>(mappings_.size())) {
        mappings_.erase(mappings_.begin() + index);
        emit paramChanged();
    }
}

void PLCOutputTool::clearMappings()
{
    mappings_.clear();
    emit paramChanged();
}

void PLCOutputTool::setMapping(int index, const OutputMapping& mapping)
{
    if (index >= 0 && index < static_cast<int>(mappings_.size())) {
        mappings_[index] = mapping;
        emit paramChanged();
    }
}

void PLCOutputTool::setInputValue(const QString& key, const QVariant& value)
{
    inputValues_[key] = value;
}

void PLCOutputTool::setInputValues(const QMap<QString, QVariant>& values)
{
    inputValues_ = values;
}

QJsonObject PLCOutputTool::serializeParams() const
{
    QJsonObject json;
    json["resultAddress"] = resultAddress_;
    json["completeAddress"] = completeAddress_;
    json["outputResult"] = outputResult_;
    json["outputComplete"] = outputComplete_;

    QJsonArray mappingArray;
    for (const auto& m : mappings_) {
        QJsonObject mObj;
        mObj["name"] = m.name;
        mObj["sourceKey"] = m.sourceKey;
        mObj["address"] = m.address;
        mObj["isBit"] = m.isBit;
        mObj["scale"] = m.scale;
        mObj["offset"] = m.offset;
        mObj["enabled"] = m.enabled;
        mappingArray.append(mObj);
    }
    json["mappings"] = mappingArray;

    return json;
}

void PLCOutputTool::deserializeParams(const QJsonObject& json)
{
    resultAddress_ = json.value("resultAddress").toInt(0);
    completeAddress_ = json.value("completeAddress").toInt(1);
    outputResult_ = json.value("outputResult").toBool(true);
    outputComplete_ = json.value("outputComplete").toBool(true);

    mappings_.clear();
    QJsonArray mappingArray = json.value("mappings").toArray();
    for (const auto& mVal : mappingArray) {
        QJsonObject mObj = mVal.toObject();
        OutputMapping m;
        m.name = mObj["name"].toString();
        m.sourceKey = mObj["sourceKey"].toString();
        m.address = mObj["address"].toInt();
        m.isBit = mObj["isBit"].toBool();
        m.scale = mObj["scale"].toDouble(1.0);
        m.offset = mObj["offset"].toInt();
        m.enabled = mObj["enabled"].toBool(true);
        mappings_.push_back(m);
    }
}

} // namespace Algorithm
} // namespace VisionForge
