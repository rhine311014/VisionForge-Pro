/**
 * @file PLCComm.cpp
 * @brief PLC通信基类实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "comm/PLCComm.h"
#include "comm/ModbusTCPComm.h"
#include "comm/MitsubishiMCComm.h"
#include "comm/KeyencePCLinkComm.h"
#include "base/Logger.h"
#include <cstring>

namespace VisionForge {
namespace Comm {

PLCComm::PLCComm(QObject* parent)
    : QObject(parent)
{
}

PLCComm::~PLCComm()
{
}

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
    for (int i = 0; i < count && i * 2 + 1 < static_cast<int>(result.intValues.size()); ++i) {
        int lowWord = result.intValues[i * 2];
        int highWord = result.intValues[i * 2 + 1];
        int dword = (highWord << 16) | (lowWord & 0xFFFF);
        dwordResult.intValues.push_back(dword);
    }

    return dwordResult;
}

PLCResult PLCComm::writeDWords(int address, const std::vector<int>& values)
{
    // 将32位值拆分为两个16位寄存器
    std::vector<int> words;
    for (int value : values) {
        words.push_back(value & 0xFFFF);           // 低16位
        words.push_back((value >> 16) & 0xFFFF);   // 高16位
    }

    return writeRegisters(address, words);
}

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
        int lowWord = result.intValues[i * 2];
        int highWord = result.intValues[i * 2 + 1];
        unsigned int combined = ((unsigned int)highWord << 16) | ((unsigned int)lowWord & 0xFFFF);

        float floatValue;
        std::memcpy(&floatValue, &combined, sizeof(float));
        floatResult.doubleValues.push_back(static_cast<double>(floatValue));
    }

    return floatResult;
}

PLCResult PLCComm::writeFloats(int address, const std::vector<double>& values)
{
    // 将浮点数转换为两个16位寄存器
    std::vector<int> words;
    for (double value : values) {
        float floatValue = static_cast<float>(value);
        unsigned int intValue;
        std::memcpy(&intValue, &floatValue, sizeof(float));

        words.push_back(intValue & 0xFFFF);           // 低16位
        words.push_back((intValue >> 16) & 0xFFFF);   // 高16位
    }

    return writeRegisters(address, words);
}

std::shared_ptr<PLCComm> createPLCComm(PLCProtocol protocol)
{
    switch (protocol) {
    case PLCProtocol::ModbusTCP:
        return std::make_shared<ModbusTCPComm>();

    case PLCProtocol::MitsubishiMC_TCP:
    case PLCProtocol::MitsubishiMC_UDP:
        return std::make_shared<MitsubishiMCComm>();

    case PLCProtocol::KeyencePCLink:
        return std::make_shared<KeyencePCLinkComm>();

    default:
        LOG_ERROR(QString("不支持的PLC协议: %1").arg(static_cast<int>(protocol)));
        return nullptr;
    }
}

} // namespace Comm
} // namespace VisionForge
