/**
 * @file MitsubishiMCComm.h
 * @brief 三菱MC协议通信类
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "comm/PLCComm.h"
#include <QTcpSocket>
#include <QUdpSocket>

namespace VisionForge {
namespace Comm {

/**
 * @brief 三菱MC协议子命令 (3E帧二进制格式)
 * 子命令用于区分位软元件和字软元件的操作
 */
namespace MCSubCommand {
    // 批量读写子命令
    constexpr quint16 Word = 0x0000;    // 字软元件操作 (D, W, R等)
    constexpr quint16 Bit = 0x0001;     // 位软元件操作 (M, X, Y, B等)
}

/**
 * @brief 三菱软元件代码 (二进制)
 */
namespace MCDeviceCode {
    constexpr quint8 D = 0xA8;    // 数据寄存器
    constexpr quint8 M = 0x90;    // 内部继电器
    constexpr quint8 X = 0x9C;    // 输入
    constexpr quint8 Y = 0x9D;    // 输出
    constexpr quint8 W = 0xB4;    // 链接寄存器
    constexpr quint8 B = 0xA0;    // 链接继电器
    constexpr quint8 R = 0xAF;    // 文件寄存器
    constexpr quint8 ZR = 0xB0;   // 文件寄存器扩展
    constexpr quint8 T = 0xC1;    // 定时器触点
    constexpr quint8 TN = 0xC2;   // 定时器当前值
    constexpr quint8 C = 0xC3;    // 计数器触点
    constexpr quint8 CN = 0xC5;   // 计数器当前值
    constexpr quint8 ST = 0xC8;   // 累计定时器触点
    constexpr quint8 STN = 0xC9;  // 累计定时器当前值
}

/**
 * @class MitsubishiMCComm
 * @brief 三菱MC协议通信类
 *
 * 支持Q/L/iQ-R系列PLC的MC协议通信
 * 支持3E帧格式 (二进制)
 */
class MitsubishiMCComm : public PLCComm
{
    Q_OBJECT

public:
    explicit MitsubishiMCComm(QObject* parent = nullptr);
    ~MitsubishiMCComm() override;

    // PLCComm接口实现
    bool connect(const PLCConfig& config) override;
    void disconnect() override;
    bool isConnected() const override;

    PLCResult readRegisters(int address, int count) override;
    PLCResult writeRegisters(int address, const std::vector<int>& values) override;
    PLCResult readCoils(int address, int count) override;
    PLCResult writeCoils(int address, const std::vector<bool>& values) override;

    /**
     * @brief 读取指定软元件
     * @param deviceType 软元件类型
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readDevice(MitsubishiDeviceType deviceType, int address, int count);

    /**
     * @brief 写入指定软元件
     * @param deviceType 软元件类型
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    PLCResult writeDevice(MitsubishiDeviceType deviceType, int address, const std::vector<int>& values);

    /**
     * @brief 读取位软元件
     * @param deviceType 软元件类型 (M, X, Y, B等)
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readBitDevice(MitsubishiDeviceType deviceType, int address, int count);

    /**
     * @brief 写入位软元件
     * @param deviceType 软元件类型
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    PLCResult writeBitDevice(MitsubishiDeviceType deviceType, int address, const std::vector<bool>& values);

private:
    /**
     * @brief 发送请求并接收响应
     */
    QByteArray sendRequest(const QByteArray& request);

    /**
     * @brief 构建3E帧请求
     * @param command 命令
     * @param subCommand 子命令
     * @param data 数据部分
     * @return 完整请求帧
     */
    QByteArray build3EFrame(quint16 command, quint16 subCommand, const QByteArray& data);

    /**
     * @brief 解析响应
     */
    PLCResult parseResponse(const QByteArray& response, bool isBitDevice = false);

    /**
     * @brief 获取软元件代码
     */
    quint8 getDeviceCode(MitsubishiDeviceType deviceType) const;

    /**
     * @brief 判断是否为位软元件
     */
    bool isBitDevice(MitsubishiDeviceType deviceType) const;

    /**
     * @brief 构建软元件地址数据
     */
    QByteArray buildDeviceAddress(MitsubishiDeviceType deviceType, int address);

private:
    QTcpSocket* socket_;
    bool useTcp_;

    // MC协议参数
    quint8 networkNo_;           // 网络号
    quint8 pcNo_;                // PC号
    quint16 destModuleIo_;       // 目标模块IO号
    quint8 destModuleStation_;   // 目标模块站号
    quint16 monitoringTimer_;    // 监视定时器

    // 默认软元件类型 (用于基类接口)
    MitsubishiDeviceType defaultWordDevice_;
    MitsubishiDeviceType defaultBitDevice_;
};

} // namespace Comm
} // namespace VisionForge
