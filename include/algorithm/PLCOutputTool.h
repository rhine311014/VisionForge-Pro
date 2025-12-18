/**
 * @file PLCOutputTool.h
 * @brief PLC输出工具
 * @details 将检测结果输出到PLC寄存器/线圈
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <memory>

// 前向声明，避免算法层依赖通信层头文件
namespace VisionForge {
namespace Comm {
    class PLCComm;
}
}

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 输出映射项
 */
struct OutputMapping {
    QString name;               // 映射名称
    QString sourceKey;          // 数据源键名
    int address = 0;            // PLC地址
    bool isBit = false;         // 是否为位地址
    double scale = 1.0;         // 缩放系数
    int offset = 0;             // 偏移量
    bool enabled = true;        // 是否启用

    OutputMapping() = default;
    OutputMapping(const QString& n, const QString& key, int addr, bool bit = false)
        : name(n), sourceKey(key), address(addr), isBit(bit) {}
};

/**
 * @class PLCOutputTool
 * @brief PLC输出工具
 *
 * 支持的功能:
 * - 将检测结果写入PLC寄存器
 * - 将OK/NG结果写入PLC线圈
 * - 批量写入多个地址
 * - 数据类型转换和缩放
 */
class PLCOutputTool : public VisionTool
{
    Q_OBJECT

public:
    explicit PLCOutputTool(QObject* parent = nullptr);
    ~PLCOutputTool() override = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::PLCOutput; }
    QString toolName() const override { return "PLC输出"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== PLC连接设置 ==========

    /**
     * @brief 设置PLC通信对象
     */
    void setPLCComm(std::shared_ptr<Comm::PLCComm> plc);

    /**
     * @brief 获取PLC通信对象
     */
    std::shared_ptr<Comm::PLCComm> plcComm() const { return plcComm_; }

    // ========== 输出映射 ==========

    /**
     * @brief 添加输出映射
     */
    void addMapping(const OutputMapping& mapping);

    /**
     * @brief 移除输出映射
     */
    void removeMapping(int index);

    /**
     * @brief 清空所有映射
     */
    void clearMappings();

    /**
     * @brief 获取所有映射
     */
    const std::vector<OutputMapping>& mappings() const { return mappings_; }

    /**
     * @brief 修改映射
     */
    void setMapping(int index, const OutputMapping& mapping);

    // ========== 输入数据 ==========

    /**
     * @brief 设置输入数据
     */
    void setInputValue(const QString& key, const QVariant& value);
    void setInputValues(const QMap<QString, QVariant>& values);

    /**
     * @brief 设置OK/NG结果
     */
    void setJudgeResult(bool isOK) { judgeResult_ = isOK; }

    // ========== 结果地址设置 ==========

    /**
     * @brief 设置结果输出地址 (OK/NG)
     */
    void setResultAddress(int address) { resultAddress_ = address; emit paramChanged(); }
    int resultAddress() const { return resultAddress_; }

    /**
     * @brief 设置触发完成地址
     */
    void setCompleteAddress(int address) { completeAddress_ = address; emit paramChanged(); }
    int completeAddress() const { return completeAddress_; }

    /**
     * @brief 设置是否输出结果
     */
    void setOutputResult(bool output) { outputResult_ = output; emit paramChanged(); }
    bool outputResult() const { return outputResult_; }

    /**
     * @brief 设置是否输出完成信号
     */
    void setOutputComplete(bool output) { outputComplete_ = output; emit paramChanged(); }
    bool outputComplete() const { return outputComplete_; }

signals:
    void outputCompleted(bool success);
    void outputError(const QString& error);

private:
    bool writeRegister(int address, int value);
    bool writeCoil(int address, bool value);

private:
    std::shared_ptr<Comm::PLCComm> plcComm_;
    std::vector<OutputMapping> mappings_;
    QMap<QString, QVariant> inputValues_;
    bool judgeResult_;
    int resultAddress_;
    int completeAddress_;
    bool outputResult_;
    bool outputComplete_;
};

} // namespace Algorithm
} // namespace VisionForge
