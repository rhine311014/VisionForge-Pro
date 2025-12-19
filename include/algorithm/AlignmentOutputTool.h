/**
 * @file AlignmentOutputTool.h
 * @brief 对位结果输出工具
 * @details 支持PLC/TCP/串口多种输出方式
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "algorithm/MultiPointAlignmentTool.h"
#include <QTcpSocket>
#include <QtSerialPort/QSerialPort>
#include <QJsonObject>
#include <memory>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 输出方式
 */
enum class OutputMethod {
    None = 0,
    PLC = 1,            ///< PLC输出
    TCP = 2,            ///< TCP输出
    Serial = 4,         ///< 串口输出
    All = 7             ///< 所有方式
};

// 支持位运算
inline OutputMethod operator|(OutputMethod a, OutputMethod b) {
    return static_cast<OutputMethod>(static_cast<int>(a) | static_cast<int>(b));
}
inline OutputMethod operator&(OutputMethod a, OutputMethod b) {
    return static_cast<OutputMethod>(static_cast<int>(a) & static_cast<int>(b));
}
inline bool hasFlag(OutputMethod value, OutputMethod flag) {
    return (static_cast<int>(value) & static_cast<int>(flag)) != 0;
}

/**
 * @brief PLC输出配置
 */
struct PLCOutputConfig {
    QString protocolType;       ///< 协议类型: "Modbus", "MC", "Keyence", "Omron"
    QString ipAddress;          ///< PLC IP地址
    int port;                   ///< 端口

    // 寄存器映射
    int offsetXRegister;        ///< X偏差寄存器地址
    int offsetYRegister;        ///< Y偏差寄存器地址
    int offsetThetaRegister;    ///< 角度偏差寄存器地址
    int resultRegister;         ///< 结果状态寄存器
    int triggerRegister;        ///< 触发完成寄存器

    // 数据格式
    double scaleFactor;         ///< 缩放因子（例：1000表示mm转um）
    bool signedValue;           ///< 是否带符号
    int dataType;               ///< 数据类型: 0=Int16, 1=Int32, 2=Float

    PLCOutputConfig()
        : port(502)
        , offsetXRegister(0)
        , offsetYRegister(2)
        , offsetThetaRegister(4)
        , resultRegister(6)
        , triggerRegister(7)
        , scaleFactor(1000.0)
        , signedValue(true)
        , dataType(0)
    {}

    QJsonObject toJson() const;
    static PLCOutputConfig fromJson(const QJsonObject& json);
};

/**
 * @brief TCP输出配置
 */
struct TCPOutputConfig {
    QString serverAddress;      ///< 服务器地址
    int port;                   ///< 端口
    QString dataFormat;         ///< 数据格式: "JSON", "CSV", "Binary", "Custom"
    QString customFormat;       ///< 自定义格式字符串
    QString delimiter;          ///< 分隔符
    bool addChecksum;           ///< 是否添加校验和
    QString endMarker;          ///< 结束标记
    int timeoutMs;              ///< 超时时间

    TCPOutputConfig()
        : port(5000)
        , dataFormat("JSON")
        , delimiter(",")
        , addChecksum(false)
        , endMarker("\r\n")
        , timeoutMs(1000)
    {}

    QJsonObject toJson() const;
    static TCPOutputConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 串口输出配置
 */
struct SerialOutputConfig {
    QString portName;           ///< 串口名称
    int baudRate;               ///< 波特率
    int dataBits;               ///< 数据位
    int stopBits;               ///< 停止位
    QString parity;             ///< 校验: "None", "Even", "Odd"
    QString dataFormat;         ///< 数据格式
    QString customFormat;       ///< 自定义格式
    QString endMarker;          ///< 结束标记
    int timeoutMs;              ///< 超时时间

    SerialOutputConfig()
        : baudRate(9600)
        , dataBits(8)
        , stopBits(1)
        , parity("None")
        , dataFormat("CSV")
        , endMarker("\r\n")
        , timeoutMs(1000)
    {}

    QJsonObject toJson() const;
    static SerialOutputConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 输出结果
 */
struct OutputResult {
    bool success;               ///< 总体是否成功
    bool plcSuccess;            ///< PLC输出是否成功
    bool tcpSuccess;            ///< TCP输出是否成功
    bool serialSuccess;         ///< 串口输出是否成功
    QString errorMessage;       ///< 错误信息
    double outputTime;          ///< 输出耗时(ms)

    OutputResult()
        : success(false)
        , plcSuccess(false)
        , tcpSuccess(false)
        , serialSuccess(false)
        , outputTime(0.0)
    {}
};

/**
 * @class AlignmentOutputTool
 * @brief 对位结果输出工具
 *
 * 功能：
 * - 支持PLC输出（Modbus、三菱MC、基恩士、欧姆龙）
 * - 支持TCP输出（JSON、CSV、二进制、自定义格式）
 * - 支持串口输出
 * - 可组合多种输出方式
 */
class AlignmentOutputTool : public VisionTool
{
    Q_OBJECT

public:
    explicit AlignmentOutputTool(QObject* parent = nullptr);
    ~AlignmentOutputTool() override;

    // ========== VisionTool 接口 ==========

    ToolType toolType() const override { return ToolType::PLCOutput; }
    QString toolName() const override { return "AlignmentOutput"; }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 配置管理 ==========

    /**
     * @brief 设置PLC配置
     */
    void setPLCConfig(const PLCOutputConfig& config);

    /**
     * @brief 获取PLC配置
     */
    PLCOutputConfig getPLCConfig() const;

    /**
     * @brief 设置TCP配置
     */
    void setTCPConfig(const TCPOutputConfig& config);

    /**
     * @brief 获取TCP配置
     */
    TCPOutputConfig getTCPConfig() const;

    /**
     * @brief 设置串口配置
     */
    void setSerialConfig(const SerialOutputConfig& config);

    /**
     * @brief 获取串口配置
     */
    SerialOutputConfig getSerialConfig() const;

    /**
     * @brief 设置输出方式
     */
    void setOutputMethod(OutputMethod method);

    /**
     * @brief 获取输出方式
     */
    OutputMethod getOutputMethod() const;

    // ========== 数据设置 ==========

    /**
     * @brief 设置对位结果
     */
    void setAlignmentResult(const AlignmentResult& result);

    /**
     * @brief 设置偏差值（手动设置）
     * @param x X偏差
     * @param y Y偏差
     * @param theta 角度偏差（度）
     */
    void setOffset(double x, double y, double theta);

    /**
     * @brief 设置OK/NG判定结果
     */
    void setJudgeResult(bool isOK);

    // ========== 输出执行 ==========

    /**
     * @brief 执行输出
     * @return 输出结果
     */
    OutputResult executeOutput();

    /**
     * @brief 输出到PLC
     * @return 是否成功
     */
    bool outputToPLC();

    /**
     * @brief 输出到TCP
     * @return 是否成功
     */
    bool outputToTCP();

    /**
     * @brief 输出到串口
     * @return 是否成功
     */
    bool outputToSerial();

    /**
     * @brief 获取最近输出结果
     */
    OutputResult getLastOutputResult() const;

    // ========== 连接管理 ==========

    /**
     * @brief 连接PLC
     * @return 是否成功
     */
    bool connectPLC();

    /**
     * @brief 断开PLC
     */
    void disconnectPLC();

    /**
     * @brief 检查PLC连接状态
     */
    bool isPLCConnected() const;

    /**
     * @brief 连接TCP服务器
     * @return 是否成功
     */
    bool connectTCP();

    /**
     * @brief 断开TCP
     */
    void disconnectTCP();

    /**
     * @brief 检查TCP连接状态
     */
    bool isTCPConnected() const;

    /**
     * @brief 打开串口
     * @return 是否成功
     */
    bool openSerial();

    /**
     * @brief 关闭串口
     */
    void closeSerial();

    /**
     * @brief 检查串口是否打开
     */
    bool isSerialOpen() const;

signals:
    /**
     * @brief 输出完成信号
     */
    void outputCompleted(const OutputResult& result);

    /**
     * @brief PLC连接状态变化
     */
    void plcConnectionChanged(bool connected);

    /**
     * @brief TCP连接状态变化
     */
    void tcpConnectionChanged(bool connected);

    /**
     * @brief 串口状态变化
     */
    void serialStateChanged(bool open);

private:
    // 格式化输出数据
    QByteArray formatData(const QString& format) const;
    QByteArray formatJSON() const;
    QByteArray formatCSV() const;
    QByteArray formatBinary() const;
    QByteArray formatCustom(const QString& format) const;

    // Modbus相关
    bool writeModbusRegisters(int startAddr, const QVector<quint16>& values);
    bool writeModbusFloat(int addr, float value);

    // 计算校验和
    quint16 calculateChecksum(const QByteArray& data) const;

private:
    PLCOutputConfig plcConfig_;
    TCPOutputConfig tcpConfig_;
    SerialOutputConfig serialConfig_;
    OutputMethod outputMethod_;

    // 输出数据
    double offsetX_;
    double offsetY_;
    double offsetTheta_;
    bool judgeResult_;
    AlignmentResult alignmentResult_;

    // 连接
    std::unique_ptr<QTcpSocket> tcpSocket_;
    std::unique_ptr<QSerialPort> serialPort_;
    std::unique_ptr<QTcpSocket> plcSocket_;  // PLC也用TCP（Modbus TCP等）

    OutputResult lastResult_;
};

} // namespace Algorithm
} // namespace VisionForge
