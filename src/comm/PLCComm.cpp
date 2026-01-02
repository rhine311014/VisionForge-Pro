/**
 * @file PLCComm.cpp
 * @brief PLC通信基类实现文件
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @details
 * 本文件实现了PLCComm基类的构造函数、析构函数以及默认的双字和浮点数读写方法。
 * 同时提供了PLC通信实例的工厂函数createPLCComm()。
 *
 * ## 主要实现内容
 * - 双字(32位)寄存器的读写：通过组合/拆分两个16位寄存器实现
 * - 浮点数寄存器的读写：使用IEEE 754格式进行数据转换
 * - 工厂函数：根据协议类型创建对应的通信实例
 *
 * ## 数据格式说明
 * ### 双字格式（32位整数）
 * - 低字（地址N）：bits 0-15
 * - 高字（地址N+1）：bits 16-31
 * - 组合公式：value = (highWord << 16) | (lowWord & 0xFFFF)
 *
 * ### 浮点数格式（IEEE 754单精度）
 * - 符号位：1位
 * - 指数位：8位
 * - 尾数位：23位
 * - 存储顺序：低字在前，高字在后
 */

#include "comm/PLCComm.h"
#include "comm/ModbusTCPComm.h"
#include "comm/MitsubishiMCComm.h"
#include "comm/KeyencePCLinkComm.h"
#include "base/Logger.h"
#include <cstring>

namespace VisionForge {
namespace Comm {

/**
 * @brief 构造函数
 * @param parent 父对象指针
 *
 * @details
 * 初始化PLC通信基类。默认状态：
 * - 协议类型：Unknown
 * - 超时时间：3000毫秒
 */
PLCComm::PLCComm(QObject* parent)
    : QObject(parent)
{
}

/**
 * @brief 析构函数
 *
 * @details
 * 虚析构函数，确保派生类能正确释放资源。
 * 派生类的disconnect()应在各自的析构函数中调用。
 */
PLCComm::~PLCComm()
{
}

/**
 * @brief 读取双字寄存器（32位整数）
 * @param address 起始寄存器地址
 * @param count 读取的双字数量
 * @return PLCResult 包含读取状态和转换后的32位整数值
 *
 * @details
 * 实现逻辑：
 * 1. 调用readRegisters()读取 count*2 个16位寄存器
 * 2. 将每两个连续的16位值组合成一个32位整数
 *
 * ## 字节序处理
 * 采用小端序（Little-Endian）：
 * - 地址N：存储低16位（bits 0-15）
 * - 地址N+1：存储高16位（bits 16-31）
 *
 * ## 组合计算
 * @code
 * int dword = (highWord << 16) | (lowWord & 0xFFFF);
 * @endcode
 *
 * @note 如果读取的寄存器数量不足，只返回能完整组合的双字
 */
PLCResult PLCComm::readDWords(int address, int count)
{
    // 默认实现：读取2*count个字寄存器，然后组合成双字
    PLCResult result = readRegisters(address, count * 2);
    if (!result.isSuccess()) {
        return result;
    }

    PLCResult dwordResult;
    dwordResult.status = CommStatus::Success;

    // 将两个16位寄存器组合成一个32位值
    // 字节序：低字在前（地址N），高字在后（地址N+1）
    for (int i = 0; i < count && i * 2 + 1 < static_cast<int>(result.intValues.size()); ++i) {
        int lowWord = result.intValues[i * 2];       // 低16位
        int highWord = result.intValues[i * 2 + 1];  // 高16位
        int dword = (highWord << 16) | (lowWord & 0xFFFF);  // 组合成32位值
        dwordResult.intValues.push_back(dword);
    }

    return dwordResult;
}

/**
 * @brief 写入双字寄存器（32位整数）
 * @param address 起始寄存器地址
 * @param values 要写入的32位整数数组
 * @return PLCResult 包含写入操作的状态
 *
 * @details
 * 实现逻辑：
 * 1. 将每个32位整数拆分为两个16位值
 * 2. 调用writeRegisters()写入所有16位值
 *
 * ## 拆分计算
 * @code
 * lowWord = value & 0xFFFF;           // 提取低16位
 * highWord = (value >> 16) & 0xFFFF;  // 提取高16位
 * @endcode
 *
 * ## 写入顺序
 * 对于32位值写入地址N：
 * - 地址N：写入低16位
 * - 地址N+1：写入高16位
 */
PLCResult PLCComm::writeDWords(int address, const std::vector<int>& values)
{
    // 将32位值拆分为两个16位寄存器
    std::vector<int> words;
    for (int value : values) {
        words.push_back(value & 0xFFFF);           // 低16位，存入地址N
        words.push_back((value >> 16) & 0xFFFF);   // 高16位，存入地址N+1
    }

    return writeRegisters(address, words);
}

/**
 * @brief 读取浮点数寄存器（IEEE 754单精度）
 * @param address 起始寄存器地址
 * @param count 读取的浮点数数量
 * @return PLCResult 包含读取状态和转换后的浮点数值（存于doubleValues）
 *
 * @details
 * 实现逻辑：
 * 1. 调用readRegisters()读取 count*2 个16位寄存器
 * 2. 将每两个连续的16位值组合成32位
 * 3. 使用memcpy将32位值解释为IEEE 754浮点数
 *
 * ## IEEE 754单精度浮点数格式
 * - 位31：符号位（0=正，1=负）
 * - 位30-23：指数（偏移127）
 * - 位22-0：尾数
 *
 * ## 类型转换
 * 使用memcpy进行类型双关(type punning)，避免违反严格别名规则：
 * @code
 * float floatValue;
 * std::memcpy(&floatValue, &combined, sizeof(float));
 * @endcode
 *
 * @note 返回值存储在doubleValues中，以提供更高精度的表示
 */
PLCResult PLCComm::readFloats(int address, int count)
{
    // 读取2*count个字寄存器，然后转换为浮点数
    PLCResult result = readRegisters(address, count * 2);
    if (!result.isSuccess()) {
        return result;
    }

    PLCResult floatResult;
    floatResult.status = CommStatus::Success;

    // 将两个16位寄存器组合成一个32位浮点数
    for (int i = 0; i < count && i * 2 + 1 < static_cast<int>(result.intValues.size()); ++i) {
        int lowWord = result.intValues[i * 2];       // 低16位
        int highWord = result.intValues[i * 2 + 1];  // 高16位

        // 组合成32位无符号整数
        unsigned int combined = ((unsigned int)highWord << 16) | ((unsigned int)lowWord & 0xFFFF);

        // 使用memcpy将32位值解释为IEEE 754浮点数
        // 这种方式符合C++标准，避免未定义行为
        float floatValue;
        std::memcpy(&floatValue, &combined, sizeof(float));

        // 转换为double存储，保持精度
        floatResult.doubleValues.push_back(static_cast<double>(floatValue));
    }

    return floatResult;
}

/**
 * @brief 写入浮点数寄存器
 * @param address 起始寄存器地址
 * @param values 要写入的浮点数数组（double类型）
 * @return PLCResult 包含写入操作的状态
 *
 * @details
 * 实现逻辑：
 * 1. 将double转换为float（可能损失精度）
 * 2. 使用memcpy将float转换为32位整数表示
 * 3. 拆分为两个16位值并写入
 *
 * ## 精度警告
 * - double范围：约 +/-1.7E308
 * - float范围：约 +/-3.4E38
 * 超出float范围的值会被截断或变为无穷大。
 *
 * ## 写入顺序
 * 对于浮点数写入地址N：
 * - 地址N：低16位
 * - 地址N+1：高16位
 */
PLCResult PLCComm::writeFloats(int address, const std::vector<double>& values)
{
    // 将浮点数转换为两个16位寄存器
    std::vector<int> words;
    for (double value : values) {
        // 将double转换为float（注意精度损失）
        float floatValue = static_cast<float>(value);

        // 使用memcpy获取float的二进制表示
        unsigned int intValue;
        std::memcpy(&intValue, &floatValue, sizeof(float));

        // 拆分为两个16位值
        words.push_back(intValue & 0xFFFF);           // 低16位
        words.push_back((intValue >> 16) & 0xFFFF);   // 高16位
    }

    return writeRegisters(address, words);
}

/**
 * @brief PLC通信实例工厂函数
 * @param protocol 要创建的PLC协议类型
 * @return 对应协议的PLCComm智能指针
 *
 * @details
 * 根据传入的协议类型，创建并返回相应的PLC通信实例。
 *
 * ## 支持的协议映射
 * | 协议类型 | 创建的实例类型 |
 * |---------|--------------|
 * | ModbusTCP | ModbusTCPComm |
 * | MitsubishiMC_TCP | MitsubishiMCComm |
 * | MitsubishiMC_UDP | MitsubishiMCComm |
 * | KeyencePCLink | KeyencePCLinkComm |
 * | 其他 | nullptr（并记录错误日志） |
 *
 * ## 错误处理
 * 对于不支持的协议类型，函数会：
 * 1. 记录错误日志
 * 2. 返回nullptr
 *
 * @note 调用方应检查返回值是否为nullptr
 */
std::shared_ptr<PLCComm> createPLCComm(PLCProtocol protocol)
{
    switch (protocol) {
    case PLCProtocol::ModbusTCP:
        // 创建Modbus TCP通信实例
        return std::make_shared<ModbusTCPComm>();

    case PLCProtocol::MitsubishiMC_TCP:
    case PLCProtocol::MitsubishiMC_UDP:
        // 创建三菱MC协议通信实例
        // TCP和UDP模式使用同一个类，通过配置区分
        return std::make_shared<MitsubishiMCComm>();

    case PLCProtocol::KeyencePCLink:
        // 创建基恩士PC-Link协议通信实例
        return std::make_shared<KeyencePCLinkComm>();

    default:
        // 不支持的协议类型，记录错误并返回nullptr
        LOG_ERROR(QString("不支持的PLC协议: %1").arg(static_cast<int>(protocol)));
        return nullptr;
    }
}

} // namespace Comm
} // namespace VisionForge
