/**
 * @file KeyencePCLinkComm.h
 * @brief 基恩士PC-LINK上位链路协议通信类
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "comm/PLCComm.h"
#include <QTcpSocket>

namespace VisionForge {
namespace Comm {

/**
 * @brief 基恩士软元件代码
 */
namespace KeyenceDeviceCode {
    constexpr const char* DM = "DM";     // 数据存储器
    constexpr const char* CM = "CM";     // 控制存储器
    constexpr const char* TM = "TM";     // 临时存储器
    constexpr const char* EM = "EM";     // 扩展数据存储器
    constexpr const char* FM = "FM";     // 文件寄存器
    constexpr const char* ZF = "ZF";     // 文件寄存器 (Z)
    constexpr const char* R = "R";       // 内部继电器
    constexpr const char* B = "B";       // 链接继电器
    constexpr const char* MR = "MR";     // 内部辅助继电器
    constexpr const char* LR = "LR";     // 锁存继电器
    constexpr const char* CR = "CR";     // 控制继电器
    constexpr const char* T = "T";       // 定时器 (当前值)
    constexpr const char* TC = "TC";     // 定时器 (触点)
    constexpr const char* C = "C";       // 计数器 (当前值)
    constexpr const char* CC = "CC";     // 计数器 (触点)
}

/**
 * @brief 基恩士软元件类型
 */
enum class KeyenceDeviceType {
    DM,     // 数据存储器
    CM,     // 控制存储器
    TM,     // 临时存储器
    EM,     // 扩展数据存储器
    FM,     // 文件寄存器
    ZF,     // 文件寄存器 (Z)
    R,      // 内部继电器
    B,      // 链接继电器
    MR,     // 内部辅助继电器
    LR,     // 锁存继电器
    CR,     // 控制继电器
    T,      // 定时器 (当前值)
    TC,     // 定时器 (触点)
    C,      // 计数器 (当前值)
    CC,     // 计数器 (触点)
};

/**
 * @class KeyencePCLinkComm
 * @brief 基恩士PC-LINK上位链路协议通信类
 *
 * 支持基恩士KV系列PLC的PC-LINK上位链路通信
 * 使用ASCII命令格式，通过TCP/Ethernet通信
 *
 * 命令格式:
 * - 读取: RD <设备>.U <点数>\r  (例: RD DM0.U 10\r)
 * - 写入: WR <设备>.U <数据...>\r (例: WR DM0.U 100 200\r)
 * - 读取位: RD <设备>\r (例: RD MR0\r)
 * - 写入位: WR <设备> <1/0>\r (例: WR MR0 1\r)
 *
 * 响应格式:
 * - 成功: <数据>\r\n 或 OK\r\n
 * - 失败: E0\r\n (命令错误), E1\r\n (数据错误)
 */
class KeyencePCLinkComm : public PLCComm
{
    Q_OBJECT

public:
    explicit KeyencePCLinkComm(QObject* parent = nullptr);
    ~KeyencePCLinkComm() override;

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
    PLCResult readDevice(KeyenceDeviceType deviceType, int address, int count);

    /**
     * @brief 写入指定软元件
     * @param deviceType 软元件类型
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    PLCResult writeDevice(KeyenceDeviceType deviceType, int address, const std::vector<int>& values);

    /**
     * @brief 读取位软元件
     * @param deviceType 软元件类型 (R, MR, CR等)
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readBitDevice(KeyenceDeviceType deviceType, int address, int count);

    /**
     * @brief 写入位软元件
     * @param deviceType 软元件类型
     * @param address 起始地址
     * @param values 写入值
     * @return 写入结果
     */
    PLCResult writeBitDevice(KeyenceDeviceType deviceType, int address, const std::vector<bool>& values);

private:
    /**
     * @brief 发送命令并接收响应
     */
    QString sendCommand(const QString& command);

    /**
     * @brief 获取软元件代码字符串
     */
    QString getDeviceCode(KeyenceDeviceType deviceType) const;

    /**
     * @brief 判断是否为位软元件
     */
    bool isBitDevice(KeyenceDeviceType deviceType) const;

    /**
     * @brief 解析响应数据
     */
    PLCResult parseResponse(const QString& response, bool isBitDevice = false);

    /**
     * @brief 检查响应是否成功
     */
    bool checkResponseOK(const QString& response, QString& errorMsg);

private:
    QTcpSocket* socket_;

    // 默认软元件类型 (用于基类接口)
    KeyenceDeviceType defaultWordDevice_;
    KeyenceDeviceType defaultBitDevice_;
};

} // namespace Comm
} // namespace VisionForge
