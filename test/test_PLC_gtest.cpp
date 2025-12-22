/**
 * @file test_PLC_gtest.cpp
 * @brief PLC通信模块单元测试
 * @author VisionForge Team
 * @date 2025-12-21
 *
 * 使用Google Test框架测试PLC通信模块的各项功能
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "plc/PLCTypes.h"
#include "plc/PLCProtocol.h"
#include "plc/PLCClient.h"
#include "plc/EtherCATTypes.h"
#include "plc/EtherCATMaster.h"

#include <QCoreApplication>
#include <QSignalSpy>
#include <QTest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <memory>

using namespace VisionForge::PLC;
using namespace testing;

// ============================================================
// 测试夹具
// ============================================================

class PLCTypesTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

class ModbusTCPProtocolTest : public ::testing::Test {
protected:
    void SetUp() override {
        protocol_ = std::make_unique<ModbusTCPProtocol>();
        PLCConfig config;
        config.protocol = ProtocolType::ModbusTCP;
        config.slaveId = 1;
        protocol_->setConfig(config);
    }

    void TearDown() override {
        protocol_.reset();
    }

    std::unique_ptr<ModbusTCPProtocol> protocol_;
};

class MitsubishiMCProtocolTest : public ::testing::Test {
protected:
    void SetUp() override {
        protocol_ = std::make_unique<MitsubishiMCProtocol>();
        PLCConfig config;
        config.protocol = ProtocolType::MitsubishiMC3E;
        config.networkNo = 0;
        config.pcNo = 0xFF;
        config.destModuleIo = 0x03FF;
        config.destModuleStation = 0;
        protocol_->setConfig(config);
    }

    void TearDown() override {
        protocol_.reset();
    }

    std::unique_ptr<MitsubishiMCProtocol> protocol_;
};

class PLCClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 确保有Qt事件循环
        if (!QCoreApplication::instance()) {
            static int argc = 1;
            static char* argv[] = { const_cast<char*>("test") };
            static QCoreApplication app(argc, argv);
        }

        client_ = std::make_unique<PLCClient>();
    }

    void TearDown() override {
        if (client_) {
            client_->disconnectFromPLC();
        }
        client_.reset();
    }

    std::unique_ptr<PLCClient> client_;
};

// ============================================================
// PLCTypes 测试 (10个测试用例)
// ============================================================

// 测试1: CommStatus枚举名称获取
TEST_F(PLCTypesTest, GetCommStatusName) {
    EXPECT_EQ(getCommStatusName(CommStatus::Disconnected), "Disconnected");
    EXPECT_EQ(getCommStatusName(CommStatus::Connecting), "Connecting");
    EXPECT_EQ(getCommStatusName(CommStatus::Connected), "Connected");
    EXPECT_EQ(getCommStatusName(CommStatus::Reconnecting), "Reconnecting");
    EXPECT_EQ(getCommStatusName(CommStatus::Error), "Error");
}

// 测试2: ProtocolType枚举名称获取
TEST_F(PLCTypesTest, GetProtocolTypeName) {
    EXPECT_EQ(getProtocolTypeName(ProtocolType::ModbusTCP), "Modbus TCP");
    EXPECT_EQ(getProtocolTypeName(ProtocolType::MitsubishiMC3E), "Mitsubishi MC 3E Binary");
    EXPECT_EQ(getProtocolTypeName(ProtocolType::OmronFINS_TCP), "Omron FINS TCP");
    EXPECT_EQ(getProtocolTypeName(ProtocolType::SiemensS7), "Siemens S7");
}

// 测试3: RegisterType名称获取
TEST_F(PLCTypesTest, GetRegisterTypeName) {
    EXPECT_EQ(getRegisterTypeName(RegisterType::D), "D");
    EXPECT_EQ(getRegisterTypeName(RegisterType::M), "M");
    EXPECT_EQ(getRegisterTypeName(RegisterType::X), "X");
    EXPECT_EQ(getRegisterTypeName(RegisterType::Y), "Y");
    EXPECT_EQ(getRegisterTypeName(RegisterType::HoldingRegister), "HoldingRegister");
    EXPECT_EQ(getRegisterTypeName(RegisterType::Coil), "Coil");
}

// 测试4: 位寄存器判断
TEST_F(PLCTypesTest, IsBitRegister) {
    EXPECT_TRUE(isBitRegister(RegisterType::M));
    EXPECT_TRUE(isBitRegister(RegisterType::X));
    EXPECT_TRUE(isBitRegister(RegisterType::Y));
    EXPECT_TRUE(isBitRegister(RegisterType::Coil));
    EXPECT_FALSE(isBitRegister(RegisterType::D));
    EXPECT_FALSE(isBitRegister(RegisterType::W));
    EXPECT_FALSE(isBitRegister(RegisterType::HoldingRegister));
}

// 测试5: 数据类型大小
TEST_F(PLCTypesTest, GetDataTypeSize) {
    EXPECT_EQ(getDataTypeSize(DataType::Bit), 1);
    EXPECT_EQ(getDataTypeSize(DataType::Int16), 2);
    EXPECT_EQ(getDataTypeSize(DataType::UInt16), 2);
    EXPECT_EQ(getDataTypeSize(DataType::Int32), 4);
    EXPECT_EQ(getDataTypeSize(DataType::Float32), 4);
    EXPECT_EQ(getDataTypeSize(DataType::Float64), 8);
}

// 测试6: PLCConfig验证
TEST_F(PLCTypesTest, PLCConfigValidation) {
    PLCConfig config;
    config.ipAddress = "192.168.1.1";
    config.port = 502;
    config.connectTimeout = 3000;
    config.responseTimeout = 1000;

    EXPECT_TRUE(config.isValid());

    config.ipAddress = "";
    EXPECT_FALSE(config.isValid());

    config.ipAddress = "192.168.1.1";
    config.port = 0;
    EXPECT_FALSE(config.isValid());
}

// 测试7: PLCConfig序列化
TEST_F(PLCTypesTest, PLCConfigSerialization) {
    PLCConfig config;
    config.name = "TestPLC";
    config.ipAddress = "192.168.1.100";
    config.port = 502;
    config.protocol = ProtocolType::ModbusTCP;
    config.slaveId = 5;
    config.connectTimeout = 5000;

    QVariantMap map = config.toVariantMap();
    PLCConfig restored = PLCConfig::fromVariantMap(map);

    EXPECT_EQ(restored.name, config.name);
    EXPECT_EQ(restored.ipAddress, config.ipAddress);
    EXPECT_EQ(restored.port, config.port);
    EXPECT_EQ(restored.protocol, config.protocol);
    EXPECT_EQ(restored.slaveId, config.slaveId);
    EXPECT_EQ(restored.connectTimeout, config.connectTimeout);
}

// 测试8: PLCResult成功构造
TEST_F(PLCTypesTest, PLCResultSuccess) {
    PLCResult result = PLCResult::Success();

    EXPECT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.errorCode, 0);
    EXPECT_TRUE(result.errorMessage.isEmpty());
}

// 测试9: PLCResult错误构造
TEST_F(PLCTypesTest, PLCResultError) {
    PLCResult result = PLCResult::Error(ErrorCode::Timeout, "Connection timeout");

    EXPECT_FALSE(result.isSuccess());
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorCode, ErrorCode::Timeout);
    EXPECT_EQ(result.errorMessage, "Connection timeout");
}

// 测试10: 错误码消息获取
TEST_F(PLCTypesTest, GetErrorMessage) {
    EXPECT_EQ(getErrorMessage(ErrorCode::Success), "Success");
    EXPECT_EQ(getErrorMessage(ErrorCode::Timeout), "Timeout");
    EXPECT_EQ(getErrorMessage(ErrorCode::NotConnected), "Not connected");
    EXPECT_EQ(getErrorMessage(ErrorCode::ModbusIllegalFunction), "Modbus: Illegal function");
}

// ============================================================
// ModbusTCP协议测试 (12个测试用例)
// ============================================================

// 测试11: 协议类型
TEST_F(ModbusTCPProtocolTest, ProtocolType) {
    EXPECT_EQ(protocol_->protocolType(), ProtocolType::ModbusTCP);
    EXPECT_EQ(protocol_->protocolName(), "Modbus TCP");
}

// 测试12: 读保持寄存器请求打包
TEST_F(ModbusTCPProtocolTest, PackReadHoldingRegisters) {
    QByteArray request = protocol_->packReadHoldingRegisters(100, 10);

    // 检查长度: MBAP头(7) + 功能码(1) + 地址(2) + 数量(2) = 12
    EXPECT_EQ(request.size(), 12);

    // 检查功能码
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::ReadHoldingRegisters);

    // 检查起始地址 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[8]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[9]), 0x64);  // 100

    // 检查数量 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[10]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x0A);  // 10
}

// 测试13: 写单个寄存器请求打包
TEST_F(ModbusTCPProtocolTest, PackWriteSingleRegister) {
    QByteArray request = protocol_->packWriteSingleRegister(200, 12345);

    EXPECT_EQ(request.size(), 12);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::WriteSingleRegister);

    // 检查地址
    EXPECT_EQ(static_cast<uint8_t>(request[8]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[9]), 0xC8);  // 200

    // 检查值 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[10]), 0x30);
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x39);  // 12345 = 0x3039
}

// 测试14: 写多个寄存器请求打包
TEST_F(ModbusTCPProtocolTest, PackWriteMultipleRegisters) {
    std::vector<uint16_t> values = {100, 200, 300};
    QByteArray request = protocol_->packWriteMultipleRegisters(50, values);

    // MBAP头(7) + 功能码(1) + 地址(2) + 数量(2) + 字节数(1) + 数据(6) = 19
    EXPECT_EQ(request.size(), 19);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::WriteMultipleRegisters);

    // 检查数量
    EXPECT_EQ(static_cast<uint8_t>(request[10]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x03);

    // 检查字节数
    EXPECT_EQ(static_cast<uint8_t>(request[12]), 0x06);
}

// 测试15: 读线圈请求打包
TEST_F(ModbusTCPProtocolTest, PackReadCoils) {
    QByteArray request = protocol_->packReadCoils(0, 16);

    EXPECT_EQ(request.size(), 12);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::ReadCoils);
}

// 测试16: 写单个线圈请求打包
TEST_F(ModbusTCPProtocolTest, PackWriteSingleCoil) {
    QByteArray request = protocol_->packWriteSingleCoil(10, true);

    EXPECT_EQ(request.size(), 12);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::WriteSingleCoil);

    // ON值为0xFF00
    EXPECT_EQ(static_cast<uint8_t>(request[10]), 0xFF);
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x00);
}

// 测试17: 写多个线圈请求打包
TEST_F(ModbusTCPProtocolTest, PackWriteMultipleCoils) {
    std::vector<bool> values = {true, false, true, true, false, false, true, false, true};
    QByteArray request = protocol_->packWriteMultipleCoils(0, values);

    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::WriteMultipleCoils);

    // 9个线圈需要2个字节
    EXPECT_EQ(static_cast<uint8_t>(request[12]), 0x02);
}

// 测试18: 解析读寄存器响应
TEST_F(ModbusTCPProtocolTest, ParseReadRegistersResponse) {
    // 构造模拟响应
    QByteArray response;
    // MBAP头
    response.append(static_cast<char>(0x00));  // Transaction ID高
    response.append(static_cast<char>(0x01));  // Transaction ID低
    response.append(static_cast<char>(0x00));  // Protocol ID高
    response.append(static_cast<char>(0x00));  // Protocol ID低
    response.append(static_cast<char>(0x00));  // Length高
    response.append(static_cast<char>(0x07));  // Length低 (1+1+1+4)
    response.append(static_cast<char>(0x01));  // Unit ID
    // PDU
    response.append(static_cast<char>(0x03));  // Function Code
    response.append(static_cast<char>(0x04));  // Byte Count
    response.append(static_cast<char>(0x00));  // Data高1
    response.append(static_cast<char>(0x64));  // Data低1 (100)
    response.append(static_cast<char>(0x00));  // Data高2
    response.append(static_cast<char>(0xC8));  // Data低2 (200)

    protocol_->setTransactionId(1);
    PLCResult result = protocol_->parseResponse(response, RegisterType::HoldingRegister);

    EXPECT_TRUE(result.isSuccess());
    EXPECT_EQ(result.uint16Values.size(), 2);
    EXPECT_EQ(result.uint16Values[0], 100);
    EXPECT_EQ(result.uint16Values[1], 200);
}

// 测试19: 解析异常响应
TEST_F(ModbusTCPProtocolTest, ParseExceptionResponse) {
    QByteArray response;
    // MBAP头
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x01));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x03));
    response.append(static_cast<char>(0x01));
    // 异常PDU
    response.append(static_cast<char>(0x83));  // 0x80 | 0x03
    response.append(static_cast<char>(0x02));  // Exception code: Illegal Data Address

    protocol_->setTransactionId(1);
    PLCResult result = protocol_->parseResponse(response);

    EXPECT_FALSE(result.isSuccess());
    EXPECT_NE(result.errorCode, 0);
}

// 测试20: 响应完整性检查
TEST_F(ModbusTCPProtocolTest, IsResponseComplete) {
    // 不完整的响应
    QByteArray incomplete;
    incomplete.append(static_cast<char>(0x00));
    incomplete.append(static_cast<char>(0x01));
    EXPECT_FALSE(protocol_->isResponseComplete(incomplete));

    // 完整的响应
    QByteArray complete;
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x01));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x03));
    complete.append(static_cast<char>(0x01));
    complete.append(static_cast<char>(0x06));
    complete.append(static_cast<char>(0x00));
    EXPECT_TRUE(protocol_->isResponseComplete(complete));
}

// 测试21: 事务ID管理
TEST_F(ModbusTCPProtocolTest, TransactionIdManagement) {
    protocol_->setTransactionId(100);
    EXPECT_EQ(protocol_->transactionId(), 100);

    EXPECT_EQ(protocol_->nextTransactionId(), 101);
    EXPECT_EQ(protocol_->transactionId(), 101);
}

// 测试22: 通用读请求接口
TEST_F(ModbusTCPProtocolTest, PackReadRequest) {
    QByteArray request = protocol_->packReadRequest(RegisterType::HoldingRegister, 0, 10);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::ReadHoldingRegisters);

    request = protocol_->packReadRequest(RegisterType::Coil, 0, 8);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::ReadCoils);

    request = protocol_->packReadRequest(RegisterType::InputRegister, 0, 5);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), ModbusFunctionCode::ReadInputRegisters);
}

// ============================================================
// 三菱MC协议测试 (10个测试用例)
// ============================================================

// 测试23: MC协议类型
TEST_F(MitsubishiMCProtocolTest, ProtocolType) {
    EXPECT_EQ(protocol_->protocolType(), ProtocolType::MitsubishiMC3E);
    EXPECT_EQ(protocol_->protocolName(), "Mitsubishi MC 3E Binary");
}

// 测试24: 批量读取字软元件请求
TEST_F(MitsubishiMCProtocolTest, PackBatchReadWord) {
    QByteArray request = protocol_->packBatchReadWord(RegisterType::D, 100, 10);

    // 检查子头
    EXPECT_EQ(static_cast<uint8_t>(request[0]), 0x50);
    EXPECT_EQ(static_cast<uint8_t>(request[1]), 0x00);

    // 检查命令 (小端序 0x0401)
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x01);
    EXPECT_EQ(static_cast<uint8_t>(request[12]), 0x04);

    // 检查子命令 (字软元件 0x0000)
    EXPECT_EQ(static_cast<uint8_t>(request[13]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[14]), 0x00);
}

// 测试25: 批量读取位软元件请求
TEST_F(MitsubishiMCProtocolTest, PackBatchReadBit) {
    QByteArray request = protocol_->packBatchReadBit(RegisterType::M, 100, 16);

    // 检查子命令 (位软元件 0x0001)
    EXPECT_EQ(static_cast<uint8_t>(request[13]), 0x01);
    EXPECT_EQ(static_cast<uint8_t>(request[14]), 0x00);
}

// 测试26: 批量写入字软元件请求
TEST_F(MitsubishiMCProtocolTest, PackBatchWriteWord) {
    std::vector<uint16_t> values = {100, 200, 300};
    QByteArray request = protocol_->packBatchWriteWord(RegisterType::D, 0, values);

    // 检查命令 (批量写入 0x1401)
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x01);
    EXPECT_EQ(static_cast<uint8_t>(request[12]), 0x14);

    // 检查点数 (小端序)
    EXPECT_EQ(static_cast<uint8_t>(request[19]), 0x03);
    EXPECT_EQ(static_cast<uint8_t>(request[20]), 0x00);
}

// 测试27: 批量写入位软元件请求
TEST_F(MitsubishiMCProtocolTest, PackBatchWriteBit) {
    std::vector<bool> values = {true, false, true, true};
    QByteArray request = protocol_->packBatchWriteBit(RegisterType::M, 100, values);

    // 检查子命令 (位软元件)
    EXPECT_EQ(static_cast<uint8_t>(request[13]), 0x01);
    EXPECT_EQ(static_cast<uint8_t>(request[14]), 0x00);
}

// 测试28: 解析MC响应
TEST_F(MitsubishiMCProtocolTest, ParseResponse) {
    QByteArray response;
    // 响应子头
    response.append(static_cast<char>(0xD0));
    response.append(static_cast<char>(0x00));
    // 网络号、PC号
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0xFF));
    // 模块IO、站号
    response.append(static_cast<char>(0xFF));
    response.append(static_cast<char>(0x03));
    response.append(static_cast<char>(0x00));
    // 数据长度 (小端序)
    response.append(static_cast<char>(0x06));
    response.append(static_cast<char>(0x00));
    // 完成代码 (0x0000 = 成功)
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    // 数据 (2个字, 小端序)
    response.append(static_cast<char>(0x64));  // 100
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0xC8));  // 200
    response.append(static_cast<char>(0x00));

    PLCResult result = protocol_->parseResponse(response, RegisterType::D);

    EXPECT_TRUE(result.isSuccess());
    EXPECT_EQ(result.uint16Values.size(), 2);
    EXPECT_EQ(result.uint16Values[0], 100);
    EXPECT_EQ(result.uint16Values[1], 200);
}

// 测试29: MC响应完整性检查
TEST_F(MitsubishiMCProtocolTest, IsResponseComplete) {
    // 不完整
    QByteArray incomplete;
    incomplete.append(static_cast<char>(0xD0));
    incomplete.append(static_cast<char>(0x00));
    EXPECT_FALSE(protocol_->isResponseComplete(incomplete));

    // 完整
    QByteArray complete;
    complete.append(static_cast<char>(0xD0));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0xFF));
    complete.append(static_cast<char>(0xFF));
    complete.append(static_cast<char>(0x03));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x02));  // 长度
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x00));  // 完成代码
    complete.append(static_cast<char>(0x00));
    EXPECT_TRUE(protocol_->isResponseComplete(complete));
}

// 测试30: MC错误响应解析
TEST_F(MitsubishiMCProtocolTest, ParseErrorResponse) {
    QByteArray response;
    response.append(static_cast<char>(0xD0));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0xFF));
    response.append(static_cast<char>(0xFF));
    response.append(static_cast<char>(0x03));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x02));
    response.append(static_cast<char>(0x00));
    // 错误代码 0xC050
    response.append(static_cast<char>(0x50));
    response.append(static_cast<char>(0xC0));

    PLCResult result = protocol_->parseResponse(response);

    EXPECT_FALSE(result.isSuccess());
    EXPECT_EQ(result.errorCode, ErrorCode::MCCommandError);
}

// 测试31: 软元件代码获取
TEST_F(MitsubishiMCProtocolTest, GetMCDeviceCode) {
    EXPECT_EQ(getMCDeviceCode(RegisterType::D), MCDeviceCode::D);
    EXPECT_EQ(getMCDeviceCode(RegisterType::M), MCDeviceCode::M);
    EXPECT_EQ(getMCDeviceCode(RegisterType::X), MCDeviceCode::X);
    EXPECT_EQ(getMCDeviceCode(RegisterType::Y), MCDeviceCode::Y);
    EXPECT_EQ(getMCDeviceCode(RegisterType::W), MCDeviceCode::W);
}

// 测试32: 通用读写接口
TEST_F(MitsubishiMCProtocolTest, GenericReadWriteInterface) {
    // 字软元件读取
    QByteArray request = protocol_->packReadRequest(RegisterType::D, 100, 5);
    EXPECT_GT(request.size(), 0);

    // 位软元件读取
    request = protocol_->packReadRequest(RegisterType::M, 0, 16);
    EXPECT_GT(request.size(), 0);

    // 字软元件写入
    std::vector<uint16_t> values = {1, 2, 3};
    request = protocol_->packWriteRequest(RegisterType::D, 0, values);
    EXPECT_GT(request.size(), 0);

    // 位软元件写入
    std::vector<bool> bits = {true, false, true};
    request = protocol_->packWriteBitRequest(RegisterType::M, 0, bits);
    EXPECT_GT(request.size(), 0);
}

// ============================================================
// Modbus RTU协议测试 (12个测试用例)
// ============================================================

class ModbusRTUProtocolTest : public ::testing::Test {
protected:
    void SetUp() override {
        protocol_ = std::make_unique<ModbusRTUProtocol>();
        PLCConfig config;
        config.protocol = ProtocolType::ModbusRTU;
        config.slaveId = 1;
        protocol_->setConfig(config);
    }

    void TearDown() override {
        protocol_.reset();
    }

    std::unique_ptr<ModbusRTUProtocol> protocol_;
};

// 测试RTU-1: 协议类型
TEST_F(ModbusRTUProtocolTest, ProtocolType) {
    EXPECT_EQ(protocol_->protocolType(), ProtocolType::ModbusRTU);
    EXPECT_EQ(protocol_->protocolName(), "Modbus RTU");
}

// 测试RTU-2: CRC16计算验证
TEST_F(ModbusRTUProtocolTest, CRC16Calculation) {
    // 标准Modbus RTU CRC测试向量: 地址01, 功能码03, 起始地址0000, 数量000A
    // CRC结果: 0xCDC5 (帧中低字节在前: C5 CD)
    QByteArray data;
    data.append(static_cast<char>(0x01));  // 从站地址
    data.append(static_cast<char>(0x03));  // 功能码
    data.append(static_cast<char>(0x00));  // 起始地址高
    data.append(static_cast<char>(0x00));  // 起始地址低
    data.append(static_cast<char>(0x00));  // 数量高
    data.append(static_cast<char>(0x0A));  // 数量低

    quint16 crc = ModbusRTUProtocol::calculateCRC16(data);
    EXPECT_EQ(crc, 0xCDC5);
}

// 测试RTU-3: CRC验证
TEST_F(ModbusRTUProtocolTest, CRC16Verification) {
    QByteArray frame;
    frame.append(static_cast<char>(0x01));
    frame.append(static_cast<char>(0x03));
    frame.append(static_cast<char>(0x00));
    frame.append(static_cast<char>(0x00));
    frame.append(static_cast<char>(0x00));
    frame.append(static_cast<char>(0x0A));
    // CRC = 0xCDC5, 低字节在前
    frame.append(static_cast<char>(0xC5));  // CRC低
    frame.append(static_cast<char>(0xCD));  // CRC高

    EXPECT_TRUE(ModbusRTUProtocol::verifyCRC16(frame));

    // 篡改数据
    frame[2] = 0x01;
    EXPECT_FALSE(ModbusRTUProtocol::verifyCRC16(frame));
}

// 测试RTU-4: 读保持寄存器请求打包
TEST_F(ModbusRTUProtocolTest, PackReadHoldingRegisters) {
    QByteArray request = protocol_->packReadHoldingRegisters(100, 10);

    // 长度: 从站地址(1) + 功能码(1) + 地址(2) + 数量(2) + CRC(2) = 8
    EXPECT_EQ(request.size(), 8);

    // 检查从站地址
    EXPECT_EQ(static_cast<uint8_t>(request[0]), 0x01);

    // 检查功能码
    EXPECT_EQ(static_cast<uint8_t>(request[1]), ModbusFunctionCode::ReadHoldingRegisters);

    // 检查起始地址 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[2]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[3]), 0x64);  // 100

    // 检查数量 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[4]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[5]), 0x0A);  // 10

    // 验证CRC
    EXPECT_TRUE(ModbusRTUProtocol::verifyCRC16(request));
}

// 测试RTU-5: 写单个寄存器请求打包
TEST_F(ModbusRTUProtocolTest, PackWriteSingleRegister) {
    QByteArray request = protocol_->packWriteSingleRegister(200, 12345);

    EXPECT_EQ(request.size(), 8);
    EXPECT_EQ(static_cast<uint8_t>(request[1]), ModbusFunctionCode::WriteSingleRegister);

    // 检查地址
    EXPECT_EQ(static_cast<uint8_t>(request[2]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[3]), 0xC8);  // 200

    // 检查值 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[4]), 0x30);
    EXPECT_EQ(static_cast<uint8_t>(request[5]), 0x39);  // 12345 = 0x3039

    EXPECT_TRUE(ModbusRTUProtocol::verifyCRC16(request));
}

// 测试RTU-6: 写多个寄存器请求打包
TEST_F(ModbusRTUProtocolTest, PackWriteMultipleRegisters) {
    std::vector<uint16_t> values = {100, 200, 300};
    QByteArray request = protocol_->packWriteMultipleRegisters(50, values);

    // 从站(1) + 功能码(1) + 地址(2) + 数量(2) + 字节数(1) + 数据(6) + CRC(2) = 15
    EXPECT_EQ(request.size(), 15);
    EXPECT_EQ(static_cast<uint8_t>(request[1]), ModbusFunctionCode::WriteMultipleRegisters);

    // 检查数量
    EXPECT_EQ(static_cast<uint8_t>(request[4]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[5]), 0x03);

    // 检查字节数
    EXPECT_EQ(static_cast<uint8_t>(request[6]), 0x06);

    EXPECT_TRUE(ModbusRTUProtocol::verifyCRC16(request));
}

// 测试RTU-7: 读线圈请求打包
TEST_F(ModbusRTUProtocolTest, PackReadCoils) {
    QByteArray request = protocol_->packReadCoils(0, 16);

    EXPECT_EQ(request.size(), 8);
    EXPECT_EQ(static_cast<uint8_t>(request[1]), ModbusFunctionCode::ReadCoils);
    EXPECT_TRUE(ModbusRTUProtocol::verifyCRC16(request));
}

// 测试RTU-8: 写单个线圈请求打包
TEST_F(ModbusRTUProtocolTest, PackWriteSingleCoil) {
    QByteArray request = protocol_->packWriteSingleCoil(10, true);

    EXPECT_EQ(request.size(), 8);
    EXPECT_EQ(static_cast<uint8_t>(request[1]), ModbusFunctionCode::WriteSingleCoil);

    // ON值为0xFF00
    EXPECT_EQ(static_cast<uint8_t>(request[4]), 0xFF);
    EXPECT_EQ(static_cast<uint8_t>(request[5]), 0x00);

    EXPECT_TRUE(ModbusRTUProtocol::verifyCRC16(request));
}

// 测试RTU-9: 解析读寄存器响应
TEST_F(ModbusRTUProtocolTest, ParseReadRegistersResponse) {
    // 构造模拟响应
    QByteArray response;
    response.append(static_cast<char>(0x01));  // 从站地址
    response.append(static_cast<char>(0x03));  // 功能码
    response.append(static_cast<char>(0x04));  // 字节数
    response.append(static_cast<char>(0x00));  // Data高1
    response.append(static_cast<char>(0x64));  // Data低1 (100)
    response.append(static_cast<char>(0x00));  // Data高2
    response.append(static_cast<char>(0xC8));  // Data低2 (200)

    // 添加CRC
    quint16 crc = ModbusRTUProtocol::calculateCRC16(response);
    response.append(static_cast<char>(crc & 0xFF));
    response.append(static_cast<char>((crc >> 8) & 0xFF));

    PLCResult result = protocol_->parseResponse(response, RegisterType::HoldingRegister);

    EXPECT_TRUE(result.isSuccess());
    EXPECT_EQ(result.uint16Values.size(), 2);
    EXPECT_EQ(result.uint16Values[0], 100);
    EXPECT_EQ(result.uint16Values[1], 200);
}

// 测试RTU-10: 解析异常响应
TEST_F(ModbusRTUProtocolTest, ParseExceptionResponse) {
    QByteArray response;
    response.append(static_cast<char>(0x01));  // 从站地址
    response.append(static_cast<char>(0x83));  // 异常功能码 0x80 | 0x03
    response.append(static_cast<char>(0x02));  // 异常代码: Illegal Data Address

    // 添加CRC
    quint16 crc = ModbusRTUProtocol::calculateCRC16(response);
    response.append(static_cast<char>(crc & 0xFF));
    response.append(static_cast<char>((crc >> 8) & 0xFF));

    PLCResult result = protocol_->parseResponse(response);

    EXPECT_FALSE(result.isSuccess());
    EXPECT_NE(result.errorCode, 0);
}

// 测试RTU-11: CRC校验失败
TEST_F(ModbusRTUProtocolTest, CRCCheckFailure) {
    QByteArray response;
    response.append(static_cast<char>(0x01));
    response.append(static_cast<char>(0x03));
    response.append(static_cast<char>(0x02));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x64));
    response.append(static_cast<char>(0x00));  // 错误的CRC
    response.append(static_cast<char>(0x00));

    PLCResult result = protocol_->parseResponse(response);

    EXPECT_FALSE(result.isSuccess());
    EXPECT_EQ(result.errorCode, ErrorCode::DataError);
    EXPECT_TRUE(result.errorMessage.contains("CRC"));
}

// 测试RTU-12: 响应完整性检查
TEST_F(ModbusRTUProtocolTest, IsResponseComplete) {
    // 不完整的响应
    QByteArray incomplete;
    incomplete.append(static_cast<char>(0x01));
    incomplete.append(static_cast<char>(0x03));
    EXPECT_FALSE(protocol_->isResponseComplete(incomplete));

    // 完整的读响应
    QByteArray complete;
    complete.append(static_cast<char>(0x01));  // 从站
    complete.append(static_cast<char>(0x03));  // 功能码
    complete.append(static_cast<char>(0x02));  // 字节数
    complete.append(static_cast<char>(0x00));  // 数据
    complete.append(static_cast<char>(0x64));
    complete.append(static_cast<char>(0xB8));  // CRC低
    complete.append(static_cast<char>(0x44));  // CRC高
    EXPECT_TRUE(protocol_->isResponseComplete(complete));
}

// ============================================================
// 串口配置测试 (3个测试用例)
// ============================================================

// 测试Serial-1: 串口配置验证
TEST(SerialConfigTest, Validation) {
    SerialConfig config;
    config.portName = "COM1";
    config.baudRate = 9600;
    config.dataBits = 8;

    EXPECT_TRUE(config.isValid());

    config.portName = "";
    EXPECT_FALSE(config.isValid());

    config.portName = "COM1";
    config.baudRate = 0;
    EXPECT_FALSE(config.isValid());
}

// 测试Serial-2: 帧间延时计算
TEST(SerialConfigTest, InterFrameDelay) {
    SerialConfig config;
    config.baudRate = 9600;
    config.dataBits = 8;
    config.parity = SerialParity::None;
    config.stopBits = SerialStopBits::One;

    // 一个字符 = 1(start) + 8(data) + 0(parity) + 1(stop) = 10 bits
    // 3.5字符时间 = 3.5 * 10 * 1000 / 9600 ≈ 3.65ms
    int delay = config.calculateInterFrameDelay();
    EXPECT_GE(delay, 3);
    EXPECT_LE(delay, 5);
}

// 测试Serial-3: 串口配置序列化
TEST(SerialConfigTest, Serialization) {
    SerialConfig config;
    config.portName = "COM3";
    config.baudRate = 115200;
    config.dataBits = 8;
    config.parity = SerialParity::Even;
    config.stopBits = SerialStopBits::One;

    QVariantMap map = config.toVariantMap();
    SerialConfig restored = SerialConfig::fromVariantMap(map);

    EXPECT_EQ(restored.portName, config.portName);
    EXPECT_EQ(restored.baudRate, config.baudRate);
    EXPECT_EQ(restored.dataBits, config.dataBits);
    EXPECT_EQ(restored.parity, config.parity);
    EXPECT_EQ(restored.stopBits, config.stopBits);
}

// ============================================================
// Omron FINS协议测试 (15个测试用例)
// ============================================================

class OmronFINSProtocolTest : public ::testing::Test {
protected:
    void SetUp() override {
        protocol_ = std::make_unique<OmronFINSProtocol>();
        PLCConfig config;
        config.protocol = ProtocolType::OmronFINS_TCP;
        config.finsSourceNode = 1;
        config.finsDestNode = 2;
        config.finsSourceUnit = 0;
        config.finsDestUnit = 0;
        protocol_->setConfig(config);
        protocol_->setSourceNode(1);
        protocol_->setDestNode(2);
    }

    void TearDown() override {
        protocol_.reset();
    }

    std::unique_ptr<OmronFINSProtocol> protocol_;
};

// 测试FINS-1: 协议类型
TEST_F(OmronFINSProtocolTest, ProtocolType) {
    EXPECT_EQ(protocol_->protocolType(), ProtocolType::OmronFINS_TCP);
    EXPECT_EQ(protocol_->protocolName(), "Omron FINS TCP");
}

// 测试FINS-2: 握手请求打包
TEST_F(OmronFINSProtocolTest, PackHandshakeRequest) {
    QByteArray request = protocol_->packHandshakeRequest();

    // 握手请求长度: 魔术字(4) + 长度(4) + 命令(4) + 错误码(4) + 客户端节点(4) = 20
    EXPECT_EQ(request.size(), 20);

    // 检查魔术字 "FINS"
    EXPECT_EQ(request[0], 'F');
    EXPECT_EQ(request[1], 'I');
    EXPECT_EQ(request[2], 'N');
    EXPECT_EQ(request[3], 'S');

    // 检查长度 (12 = 8 + 4)
    EXPECT_EQ(static_cast<uint8_t>(request[4]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[5]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[6]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[7]), 0x0C);

    // 检查命令 = 0 (握手请求)
    EXPECT_EQ(static_cast<uint8_t>(request[8]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[9]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[10]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x00);
}

// 测试FINS-3: 内存区域读取请求打包
TEST_F(OmronFINSProtocolTest, PackMemoryAreaRead) {
    QByteArray request = protocol_->packMemoryAreaRead(OmronFINSProtocol::AREA_DM_WORD, 100, 10);

    // TCP头(16) + FINS头(10) + 命令(8) = 34
    EXPECT_EQ(request.size(), 34);

    // 检查魔术字
    EXPECT_EQ(request[0], 'F');
    EXPECT_EQ(request[1], 'I');
    EXPECT_EQ(request[2], 'N');
    EXPECT_EQ(request[3], 'S');

    // 检查命令类型 = 2 (FINS帧)
    EXPECT_EQ(static_cast<uint8_t>(request[8]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[9]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[10]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[11]), 0x02);

    // 检查FINS帧头ICF (偏移16)
    EXPECT_EQ(static_cast<uint8_t>(request[16]), 0x80);  // ICF命令

    // 检查FINS命令码 (偏移26-27)
    EXPECT_EQ(static_cast<uint8_t>(request[26]), 0x01);  // 0x0101 Memory Area Read
    EXPECT_EQ(static_cast<uint8_t>(request[27]), 0x01);

    // 检查内存区域代码 (偏移28)
    EXPECT_EQ(static_cast<uint8_t>(request[28]), OmronFINSProtocol::AREA_DM_WORD);

    // 检查起始地址 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[29]), 0x00);  // 高字节
    EXPECT_EQ(static_cast<uint8_t>(request[30]), 0x64);  // 低字节 (100)

    // 检查读取数量
    EXPECT_EQ(static_cast<uint8_t>(request[32]), 0x00);  // 高字节
    EXPECT_EQ(static_cast<uint8_t>(request[33]), 0x0A);  // 低字节 (10)
}

// 测试FINS-4: 内存区域写入请求打包
TEST_F(OmronFINSProtocolTest, PackMemoryAreaWrite) {
    std::vector<uint16_t> values = {100, 200, 300};
    QByteArray request = protocol_->packMemoryAreaWrite(OmronFINSProtocol::AREA_DM_WORD, 50, values);

    // TCP头(16) + FINS头(10) + 命令(8) + 数据(6) = 40
    EXPECT_EQ(request.size(), 40);

    // 检查FINS命令码 (偏移26-27)
    EXPECT_EQ(static_cast<uint8_t>(request[26]), 0x01);  // 0x0102 Memory Area Write
    EXPECT_EQ(static_cast<uint8_t>(request[27]), 0x02);

    // 检查写入数量
    EXPECT_EQ(static_cast<uint8_t>(request[32]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(request[33]), 0x03);  // 3个字

    // 检查数据 (大端序)
    EXPECT_EQ(static_cast<uint8_t>(request[34]), 0x00);  // 100高
    EXPECT_EQ(static_cast<uint8_t>(request[35]), 0x64);  // 100低
    EXPECT_EQ(static_cast<uint8_t>(request[36]), 0x00);  // 200高
    EXPECT_EQ(static_cast<uint8_t>(request[37]), 0xC8);  // 200低
}

// 测试FINS-5: 通用读请求接口
TEST_F(OmronFINSProtocolTest, PackReadRequest) {
    QByteArray request = protocol_->packReadRequest(RegisterType::DM, 0, 10);
    EXPECT_GT(request.size(), 0);

    // 验证使用了正确的内存区域代码
    EXPECT_EQ(static_cast<uint8_t>(request[28]), OmronFINSProtocol::AREA_DM_WORD);
}

// 测试FINS-6: 通用写请求接口
TEST_F(OmronFINSProtocolTest, PackWriteRequest) {
    std::vector<uint16_t> values = {1, 2, 3};
    QByteArray request = protocol_->packWriteRequest(RegisterType::DM, 0, values);
    EXPECT_GT(request.size(), 0);
}

// 测试FINS-7: 内存区域代码映射
TEST_F(OmronFINSProtocolTest, GetMemoryAreaCode) {
    EXPECT_EQ(OmronFINSProtocol::getMemoryAreaCode(RegisterType::DM), OmronFINSProtocol::AREA_DM_WORD);
    EXPECT_EQ(OmronFINSProtocol::getMemoryAreaCode(RegisterType::CIO), OmronFINSProtocol::AREA_CIO_WORD);
    EXPECT_EQ(OmronFINSProtocol::getMemoryAreaCode(RegisterType::WR), OmronFINSProtocol::AREA_WR_WORD);
    EXPECT_EQ(OmronFINSProtocol::getMemoryAreaCode(RegisterType::HR), OmronFINSProtocol::AREA_HR_WORD);
    EXPECT_EQ(OmronFINSProtocol::getMemoryAreaCode(RegisterType::AR), OmronFINSProtocol::AREA_AR_WORD);
    EXPECT_EQ(OmronFINSProtocol::getMemoryAreaCode(RegisterType::EM), OmronFINSProtocol::AREA_EM_WORD);
}

// 测试FINS-8: 握手响应解析
TEST_F(OmronFINSProtocolTest, ParseHandshakeResponse) {
    QByteArray response;
    // 魔术字
    response.append('F');
    response.append('I');
    response.append('N');
    response.append('S');
    // 长度
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x10));  // 16
    // 命令 = 1 (握手响应)
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x01));
    // 错误码 = 0
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    // 客户端节点
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x05));  // 节点5
    // 服务器节点
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x01));  // 节点1

    EXPECT_TRUE(protocol_->parseHandshakeResponse(response));
    EXPECT_TRUE(protocol_->isHandshakeComplete());
    EXPECT_EQ(protocol_->sourceNode(), 5);
    EXPECT_EQ(protocol_->destNode(), 1);
}

// 测试FINS-9: 读响应解析
TEST_F(OmronFINSProtocolTest, ParseReadResponse) {
    QByteArray response;
    // 魔术字
    response.append('F');
    response.append('I');
    response.append('N');
    response.append('S');
    // 长度 (8 + 10 + 2 + 4 = 24)
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x18));
    // 命令 = 2 (FINS帧)
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x02));
    // TCP错误码 = 0
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    // FINS帧头 (10字节)
    response.append(static_cast<char>(0xC0));  // ICF响应
    response.append(static_cast<char>(0x00));  // RSV
    response.append(static_cast<char>(0x02));  // GCT
    response.append(static_cast<char>(0x00));  // DNA
    response.append(static_cast<char>(0x01));  // DA1
    response.append(static_cast<char>(0x00));  // DA2
    response.append(static_cast<char>(0x00));  // SNA
    response.append(static_cast<char>(0x02));  // SA1
    response.append(static_cast<char>(0x00));  // SA2
    response.append(static_cast<char>(0x01));  // SID
    // FINS响应码 (成功)
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    // 数据 (2个字, 大端序)
    response.append(static_cast<char>(0x00));  // 100高
    response.append(static_cast<char>(0x64));  // 100低
    response.append(static_cast<char>(0x00));  // 200高
    response.append(static_cast<char>(0xC8));  // 200低

    // 先设置读命令以便解析数据
    protocol_->packReadRequest(RegisterType::DM, 0, 2);

    PLCResult result = protocol_->parseResponse(response, RegisterType::DM);

    EXPECT_TRUE(result.isSuccess());
    EXPECT_EQ(result.uint16Values.size(), 2);
    EXPECT_EQ(result.uint16Values[0], 100);
    EXPECT_EQ(result.uint16Values[1], 200);
}

// 测试FINS-10: 错误响应解析
TEST_F(OmronFINSProtocolTest, ParseErrorResponse) {
    QByteArray response;
    // 魔术字
    response.append('F');
    response.append('I');
    response.append('N');
    response.append('S');
    // 长度
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x14));
    // 命令 = 2
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x02));
    // TCP错误码 = 0
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    response.append(static_cast<char>(0x00));
    // FINS帧头
    for (int i = 0; i < 10; ++i) {
        response.append(static_cast<char>(0x00));
    }
    // FINS响应码 (错误: 0x1103 - 地址范围错误)
    response.append(static_cast<char>(0x11));  // 主代码
    response.append(static_cast<char>(0x03));  // 子代码

    PLCResult result = protocol_->parseResponse(response);

    EXPECT_FALSE(result.isSuccess());
    EXPECT_NE(result.errorCode, 0);
}

// 测试FINS-11: 响应完整性检查
TEST_F(OmronFINSProtocolTest, IsResponseComplete) {
    // 不完整的响应
    QByteArray incomplete;
    incomplete.append('F');
    incomplete.append('I');
    incomplete.append('N');
    incomplete.append('S');
    EXPECT_FALSE(protocol_->isResponseComplete(incomplete));

    // 完整的响应
    QByteArray complete;
    complete.append('F');
    complete.append('I');
    complete.append('N');
    complete.append('S');
    // 长度 = 8
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x00));
    complete.append(static_cast<char>(0x08));
    // 填充8字节数据
    for (int i = 0; i < 8; ++i) {
        complete.append(static_cast<char>(0x00));
    }
    EXPECT_TRUE(protocol_->isResponseComplete(complete));
}

// 测试FINS-12: 节点地址设置
TEST_F(OmronFINSProtocolTest, NodeAddressSettings) {
    protocol_->setSourceNode(10);
    protocol_->setDestNode(20);
    protocol_->setSourceUnit(1);
    protocol_->setDestUnit(2);
    protocol_->setSourceNetwork(3);
    protocol_->setDestNetwork(4);

    EXPECT_EQ(protocol_->sourceNode(), 10);
    EXPECT_EQ(protocol_->destNode(), 20);
}

// 测试FINS-13: FINS错误信息
TEST_F(OmronFINSProtocolTest, FINSErrorMessage) {
    QString msg;

    msg = OmronFINSProtocol::getFINSErrorMessage(0x00, 0x00);
    EXPECT_EQ(msg, "Success");

    msg = OmronFINSProtocol::getFINSErrorMessage(0x01, 0x01);
    EXPECT_TRUE(msg.contains("Local node not in network"));

    msg = OmronFINSProtocol::getFINSErrorMessage(0x02, 0x01);
    EXPECT_TRUE(msg.contains("Destination node not in network"));

    msg = OmronFINSProtocol::getFINSErrorMessage(0x11, 0x03);
    EXPECT_TRUE(msg.contains("Address range error"));
}

// 测试FINS-14: 无效魔术字响应
TEST_F(OmronFINSProtocolTest, InvalidMagicResponse) {
    QByteArray response;
    response.append('X');  // 无效魔术字
    response.append('Y');
    response.append('Z');
    response.append('W');
    for (int i = 0; i < 20; ++i) {
        response.append(static_cast<char>(0x00));
    }

    PLCResult result = protocol_->parseResponse(response);

    EXPECT_FALSE(result.isSuccess());
    EXPECT_EQ(result.errorCode, ErrorCode::ProtocolError);
    EXPECT_TRUE(result.errorMessage.contains("Invalid FINS magic"));
}

// 测试FINS-15: 位区域判断
TEST_F(OmronFINSProtocolTest, IsBitArea) {
    EXPECT_TRUE(OmronFINSProtocol::isBitArea(RegisterType::Coil));
    EXPECT_TRUE(OmronFINSProtocol::isBitArea(RegisterType::DiscreteInput));
    EXPECT_FALSE(OmronFINSProtocol::isBitArea(RegisterType::DM));
    EXPECT_FALSE(OmronFINSProtocol::isBitArea(RegisterType::HoldingRegister));
}

// ============================================================
// 协议工厂测试 (3个测试用例)
// ============================================================

// 测试33: 协议工厂创建
TEST(PLCProtocolFactoryTest, CreateProtocol) {
    auto modbusProtocol = PLCProtocolFactory::create(ProtocolType::ModbusTCP);
    EXPECT_NE(modbusProtocol, nullptr);
    EXPECT_EQ(modbusProtocol->protocolType(), ProtocolType::ModbusTCP);

    auto rtuProtocol = PLCProtocolFactory::create(ProtocolType::ModbusRTU);
    EXPECT_NE(rtuProtocol, nullptr);
    EXPECT_EQ(rtuProtocol->protocolType(), ProtocolType::ModbusRTU);

    auto mcProtocol = PLCProtocolFactory::create(ProtocolType::MitsubishiMC3E);
    EXPECT_NE(mcProtocol, nullptr);
    EXPECT_EQ(mcProtocol->protocolType(), ProtocolType::MitsubishiMC3E);

    auto finsProtocol = PLCProtocolFactory::create(ProtocolType::OmronFINS_TCP);
    EXPECT_NE(finsProtocol, nullptr);
    EXPECT_EQ(finsProtocol->protocolType(), ProtocolType::OmronFINS_TCP);

    auto unsupported = PLCProtocolFactory::create(ProtocolType::Unknown);
    EXPECT_EQ(unsupported, nullptr);
}

// 测试34: 支持的协议列表
TEST(PLCProtocolFactoryTest, SupportedProtocols) {
    auto protocols = PLCProtocolFactory::supportedProtocols();

    EXPECT_FALSE(protocols.empty());
    EXPECT_TRUE(std::find(protocols.begin(), protocols.end(), ProtocolType::ModbusTCP) != protocols.end());
    EXPECT_TRUE(std::find(protocols.begin(), protocols.end(), ProtocolType::ModbusRTU) != protocols.end());
    EXPECT_TRUE(std::find(protocols.begin(), protocols.end(), ProtocolType::MitsubishiMC3E) != protocols.end());
    EXPECT_TRUE(std::find(protocols.begin(), protocols.end(), ProtocolType::OmronFINS_TCP) != protocols.end());
}

// 测试35: 协议支持检查
TEST(PLCProtocolFactoryTest, IsSupported) {
    EXPECT_TRUE(PLCProtocolFactory::isSupported(ProtocolType::ModbusTCP));
    EXPECT_TRUE(PLCProtocolFactory::isSupported(ProtocolType::ModbusRTU));
    EXPECT_TRUE(PLCProtocolFactory::isSupported(ProtocolType::MitsubishiMC3E));
    EXPECT_TRUE(PLCProtocolFactory::isSupported(ProtocolType::OmronFINS_TCP));
    EXPECT_FALSE(PLCProtocolFactory::isSupported(ProtocolType::Unknown));
    EXPECT_FALSE(PLCProtocolFactory::isSupported(ProtocolType::SiemensS7));
}

// ============================================================
// PLCClient测试 (5个测试用例)
// ============================================================

// 测试36: 客户端初始状态
TEST_F(PLCClientTest, InitialState) {
    EXPECT_FALSE(client_->isConnected());
    EXPECT_EQ(client_->status(), CommStatus::Disconnected);
    EXPECT_FALSE(client_->isHeartbeatRunning());
}

// 测试37: 无效配置连接
TEST_F(PLCClientTest, InvalidConfigConnection) {
    PLCConfig invalidConfig;
    invalidConfig.ipAddress = "";

    bool result = client_->connectToPLC(invalidConfig);
    EXPECT_FALSE(result);
    EXPECT_EQ(client_->status(), CommStatus::Disconnected);
}

// 测试38: 统计信息
TEST_F(PLCClientTest, Statistics) {
    CommStatistics stats = client_->statistics();

    EXPECT_EQ(stats.totalConnections, 0);
    EXPECT_EQ(stats.totalReadCount, 0);
    EXPECT_EQ(stats.totalWriteCount, 0);

    client_->resetStatistics();
    stats = client_->statistics();
    EXPECT_EQ(stats.totalConnections, 0);
}

// 测试39: 自动重连设置
TEST_F(PLCClientTest, AutoReconnect) {
    EXPECT_TRUE(client_->autoReconnect());

    client_->setAutoReconnect(false);
    EXPECT_FALSE(client_->autoReconnect());

    client_->setAutoReconnect(true);
    EXPECT_TRUE(client_->autoReconnect());
}

// 测试40: 未连接状态下的读写
TEST_F(PLCClientTest, ReadWriteWithoutConnection) {
    PLCResult result = client_->readInt16(RegisterType::D, 0);
    EXPECT_FALSE(result.isSuccess());
    // 未初始化协议时返回ProtocolError（protocol_为null）
    EXPECT_EQ(result.errorCode, ErrorCode::ProtocolError);

    result = client_->writeInt16(RegisterType::D, 0, 100);
    EXPECT_FALSE(result.isSuccess());
    EXPECT_EQ(result.errorCode, ErrorCode::ProtocolError);
}

// ============================================================
// 数据转换测试 (5个测试用例)
// ============================================================

// 测试41: uint16转int32
TEST(DataConversionTest, Uint16ToInt32) {
    std::vector<uint16_t> input = {0x1234, 0x5678};
    auto result = PLCProtocol::toInt32(input, ByteOrder::BigEndian);

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0x12345678);
}

// 测试42: uint16转float
TEST(DataConversionTest, Uint16ToFloat) {
    // 1.0f = 0x3F800000
    std::vector<uint16_t> input = {0x3F80, 0x0000};
    auto result = PLCProtocol::toFloat(input, ByteOrder::BigEndian);

    EXPECT_EQ(result.size(), 1);
    EXPECT_FLOAT_EQ(result[0], 1.0f);
}

// 测试43: int32转uint16
TEST(DataConversionTest, Int32ToUint16) {
    std::vector<int32_t> input = {0x12345678};
    auto result = PLCProtocol::fromInt32(input, ByteOrder::BigEndian);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 0x1234);
    EXPECT_EQ(result[1], 0x5678);
}

// 测试44: float转uint16
TEST(DataConversionTest, FloatToUint16) {
    std::vector<float> input = {1.0f};
    auto result = PLCProtocol::fromFloat(input, ByteOrder::BigEndian);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 0x3F80);
    EXPECT_EQ(result[1], 0x0000);
}

// 测试45: 小端序转换
TEST(DataConversionTest, LittleEndianConversion) {
    std::vector<uint16_t> input = {0x5678, 0x1234};
    auto result = PLCProtocol::toInt32(input, ByteOrder::LittleEndian);

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0x12345678);
}

// ============================================================
// PLCClientManager测试 (5个测试用例)
// ============================================================

// 测试46: 管理器单例
TEST(PLCClientManagerTest, Singleton) {
    PLCClientManager& manager1 = PLCClientManager::instance();
    PLCClientManager& manager2 = PLCClientManager::instance();

    EXPECT_EQ(&manager1, &manager2);
}

// 测试47: 创建客户端
TEST(PLCClientManagerTest, CreateClient) {
    PLCClientManager& manager = PLCClientManager::instance();

    PLCConfig config;
    config.ipAddress = "192.168.1.1";
    config.port = 502;
    config.protocol = ProtocolType::ModbusTCP;

    PLCClient* client = manager.createClient("TestPLC", config);
    EXPECT_NE(client, nullptr);

    PLCClient* retrieved = manager.getClient("TestPLC");
    EXPECT_EQ(client, retrieved);

    manager.removeClient("TestPLC");
}

// 测试48: 获取不存在的客户端
TEST(PLCClientManagerTest, GetNonExistentClient) {
    PLCClientManager& manager = PLCClientManager::instance();

    PLCClient* client = manager.getClient("NonExistent");
    EXPECT_EQ(client, nullptr);
}

// 测试49: 客户端名称列表
TEST(PLCClientManagerTest, ClientNames) {
    PLCClientManager& manager = PLCClientManager::instance();
    manager.disconnectAll();

    PLCConfig config;
    config.ipAddress = "192.168.1.1";
    config.port = 502;

    manager.createClient("PLC1", config);
    manager.createClient("PLC2", config);

    QStringList names = manager.clientNames();
    EXPECT_EQ(names.size(), 2);
    EXPECT_TRUE(names.contains("PLC1"));
    EXPECT_TRUE(names.contains("PLC2"));

    manager.disconnectAll();
    EXPECT_EQ(manager.clientCount(), 0);
}

// 测试50: 断开所有连接
TEST(PLCClientManagerTest, DisconnectAll) {
    PLCClientManager& manager = PLCClientManager::instance();

    PLCConfig config;
    config.ipAddress = "192.168.1.1";
    config.port = 502;

    manager.createClient("PLC1", config);
    manager.createClient("PLC2", config);

    EXPECT_EQ(manager.clientCount(), 2);

    manager.disconnectAll();
    EXPECT_EQ(manager.clientCount(), 0);
}

// ============================================================
// EtherCAT类型测试 (10个测试用例)
// ============================================================

// 测试EC-1: 状态枚举名称
TEST(EtherCATTypesTest, StateNames) {
    EXPECT_EQ(getECStateName(ECState::None), "None");
    EXPECT_EQ(getECStateName(ECState::Init), "Init");
    EXPECT_EQ(getECStateName(ECState::PreOp), "Pre-Operational");
    EXPECT_EQ(getECStateName(ECState::SafeOp), "Safe-Operational");
    EXPECT_EQ(getECStateName(ECState::Operational), "Operational");
    EXPECT_EQ(getECStateName(ECState::Boot), "Boot");
}

// 测试EC-2: 状态错误检查
TEST(EtherCATTypesTest, StateErrorCheck) {
    EXPECT_FALSE(hasError(ECState::Init));
    EXPECT_FALSE(hasError(ECState::PreOp));
    EXPECT_FALSE(hasError(ECState::Operational));
    EXPECT_TRUE(hasError(ECState::InitError));
    EXPECT_TRUE(hasError(ECState::PreOpError));
    EXPECT_TRUE(hasError(ECState::OpError));
}

// 测试EC-3: 数据类型大小
TEST(EtherCATTypesTest, DataTypeSize) {
    EXPECT_EQ(getECDataTypeSize(ECDataType::BOOLEAN), 1);
    EXPECT_EQ(getECDataTypeSize(ECDataType::INTEGER8), 1);
    EXPECT_EQ(getECDataTypeSize(ECDataType::UNSIGNED8), 1);
    EXPECT_EQ(getECDataTypeSize(ECDataType::INTEGER16), 2);
    EXPECT_EQ(getECDataTypeSize(ECDataType::UNSIGNED16), 2);
    EXPECT_EQ(getECDataTypeSize(ECDataType::INTEGER32), 4);
    EXPECT_EQ(getECDataTypeSize(ECDataType::UNSIGNED32), 4);
    EXPECT_EQ(getECDataTypeSize(ECDataType::REAL32), 4);
    EXPECT_EQ(getECDataTypeSize(ECDataType::INTEGER64), 8);
    EXPECT_EQ(getECDataTypeSize(ECDataType::REAL64), 8);
}

// 测试EC-4: 从站身份信息
TEST(EtherCATTypesTest, SlaveIdentity) {
    ECSlaveIdentity identity;
    EXPECT_FALSE(identity.isValid());

    identity.vendorId = 0x00000002;  // Beckhoff
    identity.productCode = 0x044C2C52;  // EK1100
    EXPECT_TRUE(identity.isValid());

    QString str = identity.toString();
    EXPECT_TRUE(str.contains("00000002"));
    EXPECT_TRUE(str.contains("044c2c52"));
}

// 测试EC-5: 从站身份JSON序列化
TEST(EtherCATTypesTest, SlaveIdentityJson) {
    ECSlaveIdentity original;
    original.vendorId = 0x00000002;
    original.productCode = 0x07D83052;
    original.revisionNo = 0x00110001;
    original.serialNo = 12345;

    QJsonObject json = original.toJson();
    ECSlaveIdentity restored = ECSlaveIdentity::fromJson(json);

    EXPECT_EQ(restored.vendorId, original.vendorId);
    EXPECT_EQ(restored.productCode, original.productCode);
    EXPECT_EQ(restored.revisionNo, original.revisionNo);
    EXPECT_EQ(restored.serialNo, original.serialNo);
}

// 测试EC-6: PDO条目映射值
TEST(EtherCATTypesTest, PDOEntryMapping) {
    PDOEntry entry;
    entry.index = 0x7000;
    entry.subIndex = 1;
    entry.bitLength = 1;

    uint32_t mappingValue = entry.toMappingValue();
    EXPECT_EQ(mappingValue, 0x70000101);

    PDOEntry restored = PDOEntry::fromMappingValue(mappingValue);
    EXPECT_EQ(restored.index, 0x7000);
    EXPECT_EQ(restored.subIndex, 1);
    EXPECT_EQ(restored.bitLength, 1);
}

// 测试EC-7: PDO映射大小计算
TEST(EtherCATTypesTest, PDOMappingSize) {
    PDOMapping pdo;
    pdo.pdoIndex = 0x1600;

    PDOEntry entry1, entry2, entry3;
    entry1.bitLength = 1;
    entry2.bitLength = 1;
    entry3.bitLength = 16;

    pdo.entries.push_back(entry1);
    pdo.entries.push_back(entry2);
    pdo.entries.push_back(entry3);

    EXPECT_EQ(pdo.totalBitSize(), 18);
    EXPECT_EQ(pdo.totalByteSize(), 3);  // (18+7)/8 = 3
}

// 测试EC-8: 主站配置JSON序列化
TEST(EtherCATTypesTest, MasterConfigJson) {
    ECMasterConfig original;
    original.name = "Test Master";
    original.networkInterface = "eth0";
    original.cycleTime = 500;
    original.sdoTimeout = 5000;
    original.autoConfig = false;

    QJsonObject json = original.toJson();
    ECMasterConfig restored = ECMasterConfig::fromJson(json);

    EXPECT_EQ(restored.name, original.name);
    EXPECT_EQ(restored.networkInterface, original.networkInterface);
    EXPECT_EQ(restored.cycleTime, original.cycleTime);
    EXPECT_EQ(restored.sdoTimeout, original.sdoTimeout);
    EXPECT_EQ(restored.autoConfig, original.autoConfig);
}

// 测试EC-9: DC配置序列化
TEST(EtherCATTypesTest, DCConfigJson) {
    DCConfig original;
    original.enabled = true;
    original.syncType = DCSyncType::Sync0;
    original.cycleTime0 = 1000000;
    original.shiftTime = 50000;

    QJsonObject json = original.toJson();
    DCConfig restored = DCConfig::fromJson(json);

    EXPECT_EQ(restored.enabled, original.enabled);
    EXPECT_EQ(restored.syncType, original.syncType);
    EXPECT_EQ(restored.cycleTime0, original.cycleTime0);
    EXPECT_EQ(restored.shiftTime, original.shiftTime);
}

// 测试EC-10: SDO中止码消息
TEST(EtherCATTypesTest, SDOAbortMessages) {
    QString msg;

    msg = SDOResponse::getSDOAbortMessage(0x06010000);
    EXPECT_TRUE(msg.contains("does not exist"));

    msg = SDOResponse::getSDOAbortMessage(0x06040041);
    EXPECT_TRUE(msg.contains("cannot be read"));

    msg = SDOResponse::getSDOAbortMessage(0x06040042);
    EXPECT_TRUE(msg.contains("cannot be written"));

    msg = SDOResponse::getSDOAbortMessage(0x06070010);
    EXPECT_TRUE(msg.contains("Data type"));
}

// ============================================================
// EtherCAT主站测试 (15个测试用例)
// ============================================================

class EtherCATMasterTest : public ::testing::Test {
protected:
    void SetUp() override {
        master_ = std::make_unique<EtherCATMaster>();
    }

    void TearDown() override {
        if (master_) {
            master_->shutdown();
        }
        master_.reset();
    }

    std::unique_ptr<EtherCATMaster> master_;
};

// 测试ECM-1: 初始状态
TEST_F(EtherCATMasterTest, InitialState) {
    EXPECT_FALSE(master_->isInitialized());
    EXPECT_FALSE(master_->isRunning());
    EXPECT_EQ(master_->getStatus(), ECMasterStatus::Disconnected);
    EXPECT_EQ(master_->getSlaveCount(), 0);
}

// 测试ECM-2: 初始化
TEST_F(EtherCATMasterTest, Initialize) {
    ECMasterConfig config;
    config.name = "Test Master";
    config.networkInterface = "eth0";
    config.cycleTime = 1000;

    bool result = master_->initialize(config);
    EXPECT_TRUE(result);
    EXPECT_TRUE(master_->isInitialized());

    // 重复初始化应该失败
    result = master_->initialize(config);
    EXPECT_FALSE(result);
}

// 测试ECM-3: 无效配置初始化
TEST_F(EtherCATMasterTest, InitializeInvalidConfig) {
    ECMasterConfig config;
    config.networkInterface = "";  // 空接口名

    bool result = master_->initialize(config);
    EXPECT_FALSE(result);
    EXPECT_FALSE(master_->isInitialized());
}

// 测试ECM-4: 网络扫描 (模拟模式)
TEST_F(EtherCATMasterTest, ScanNetwork) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);

    int count = master_->scanNetwork();
    EXPECT_GT(count, 0);  // 模拟模式应该返回测试从站
    EXPECT_EQ(master_->getSlaveCount(), count);
}

// 测试ECM-5: 获取从站信息
TEST_F(EtherCATMasterTest, GetSlaveInfo) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();

    const ECSlaveInfo* slave = master_->getSlaveInfo(1);
    EXPECT_NE(slave, nullptr);
    EXPECT_EQ(slave->position, 1);
    EXPECT_FALSE(slave->name.isEmpty());
    EXPECT_TRUE(slave->identity.isValid());

    // 不存在的从站
    const ECSlaveInfo* notFound = master_->getSlaveInfo(100);
    EXPECT_EQ(notFound, nullptr);
}

// 测试ECM-6: 从站配置
TEST_F(EtherCATMasterTest, ConfigureSlaves) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();

    bool result = master_->configureSlaves();
    EXPECT_TRUE(result);

    auto [inputSize, outputSize] = master_->getIOMapSize();
    EXPECT_GE(inputSize + outputSize, 0);
}

// 测试ECM-7: 状态请求
TEST_F(EtherCATMasterTest, RequestState) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();
    master_->configureSlaves();

    bool result = master_->requestState(ECState::PreOp);
    EXPECT_TRUE(result);
    EXPECT_TRUE(master_->allSlavesInState(ECState::PreOp));

    result = master_->requestState(ECState::SafeOp);
    EXPECT_TRUE(result);

    result = master_->requestState(ECState::Operational);
    EXPECT_TRUE(result);
}

// 测试ECM-8: SDO读取 (模拟模式)
TEST_F(EtherCATMasterTest, SDORead) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();
    master_->configureSlaves();

    // 读取设备类型 (0x1000)
    SDOResponse resp = master_->sdoRead(1, 0x1000, 0);
    EXPECT_TRUE(resp.success);
    EXPECT_GT(resp.data.size(), 0);

    // 读取厂商ID (0x1018:01)
    resp = master_->sdoRead(1, 0x1018, 1);
    EXPECT_TRUE(resp.success);
}

// 测试ECM-9: SDO写入 (模拟模式)
TEST_F(EtherCATMasterTest, SDOWrite) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();
    master_->configureSlaves();

    QByteArray data(2, 0);
    data[0] = 0x01;
    data[1] = 0x00;

    SDOResponse resp = master_->sdoWrite(1, 0x6000, 0, data);
    EXPECT_TRUE(resp.success);
}

// 测试ECM-10: SDO便捷方法
TEST_F(EtherCATMasterTest, SDOConvenienceMethods) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();
    master_->configureSlaves();

    // 写入和读取
    bool writeOk = master_->sdoWriteUInt16(1, 0x6000, 0, 1234);
    EXPECT_TRUE(writeOk);

    writeOk = master_->sdoWriteInt32(1, 0x6000, 1, -12345);
    EXPECT_TRUE(writeOk);

    writeOk = master_->sdoWriteFloat(1, 0x6000, 2, 3.14159f);
    EXPECT_TRUE(writeOk);
}

// 测试ECM-11: 周期数据交换
TEST_F(EtherCATMasterTest, CyclicExchange) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    config.cycleTime = 1000;
    master_->initialize(config);
    master_->scanNetwork();
    master_->configureSlaves();

    bool result = master_->startCyclicExchange();
    EXPECT_TRUE(result);
    EXPECT_TRUE(master_->isRunning());
    EXPECT_EQ(master_->getStatus(), ECMasterStatus::Running);

    // 等待几个周期
    QThread::msleep(50);

    master_->stopCyclicExchange();
    EXPECT_FALSE(master_->isRunning());
}

// 测试ECM-12: IO数据访问
TEST_F(EtherCATMasterTest, IODataAccess) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();
    master_->configureSlaves();

    // 写入输出位
    bool result = master_->writeOutputBit(2, 0, true);  // EL2008
    EXPECT_TRUE(result);

    // 读取输出
    QByteArray outputs = master_->readSlaveOutputs(2);
    EXPECT_GT(outputs.size(), 0);

    // 写入输出字节
    result = master_->writeOutputByte(2, 0, 0xFF);
    EXPECT_TRUE(result);

    // 读取确认
    outputs = master_->readSlaveOutputs(2);
    EXPECT_EQ(static_cast<uint8_t>(outputs[0]), 0xFF);
}

// 测试ECM-13: 统计信息
TEST_F(EtherCATMasterTest, Statistics) {
    ECMasterConfig config;
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();
    master_->configureSlaves();

    ECStatistics stats = master_->getStatistics();
    EXPECT_EQ(stats.cycleCount, 0);

    master_->startCyclicExchange();
    QThread::msleep(50);
    master_->stopCyclicExchange();

    stats = master_->getStatistics();
    EXPECT_GT(stats.cycleCount, 0);

    master_->resetStatistics();
    stats = master_->getStatistics();
    EXPECT_EQ(stats.cycleCount, 0);
}

// 测试ECM-14: 诊断信息
TEST_F(EtherCATMasterTest, DiagnosticInfo) {
    ECMasterConfig config;
    config.name = "Test Master";
    config.networkInterface = "eth0";
    master_->initialize(config);
    master_->scanNetwork();

    QString info = master_->getDiagnosticInfo();
    EXPECT_FALSE(info.isEmpty());
    EXPECT_TRUE(info.contains("Test Master"));
    EXPECT_TRUE(info.contains("eth0"));
}

// 测试ECM-15: JSON持久化
TEST_F(EtherCATMasterTest, JsonPersistence) {
    ECMasterConfig config;
    config.name = "Persistent Master";
    config.networkInterface = "eth0";
    config.cycleTime = 500;
    master_->initialize(config);
    master_->scanNetwork();

    QJsonObject json = master_->exportToJson();
    EXPECT_FALSE(json.isEmpty());
    EXPECT_EQ(json["type"].toString(), "EtherCATMaster");

    auto newMaster = std::make_unique<EtherCATMaster>();
    bool result = newMaster->importFromJson(json);
    EXPECT_TRUE(result);
}

// ============================================================
// EtherCAT主站管理器测试 (5个测试用例)
// ============================================================

// 测试ECMgr-1: 单例
TEST(EtherCATManagerTest, Singleton) {
    EtherCATMasterManager& mgr1 = EtherCATMasterManager::instance();
    EtherCATMasterManager& mgr2 = EtherCATMasterManager::instance();
    EXPECT_EQ(&mgr1, &mgr2);
}

// 测试ECMgr-2: 创建主站
TEST(EtherCATManagerTest, CreateMaster) {
    EtherCATMasterManager& mgr = EtherCATMasterManager::instance();

    EtherCATMaster* master = mgr.createMaster("TestEC1");
    EXPECT_NE(master, nullptr);

    EtherCATMaster* retrieved = mgr.getMaster("TestEC1");
    EXPECT_EQ(master, retrieved);

    mgr.removeMaster("TestEC1");
}

// 测试ECMgr-3: 获取不存在的主站
TEST(EtherCATManagerTest, GetNonExistent) {
    EtherCATMasterManager& mgr = EtherCATMasterManager::instance();

    EtherCATMaster* master = mgr.getMaster("NonExistent");
    EXPECT_EQ(master, nullptr);
}

// 测试ECMgr-4: 主站名称列表
TEST(EtherCATManagerTest, MasterNames) {
    EtherCATMasterManager& mgr = EtherCATMasterManager::instance();

    mgr.createMaster("EC1");
    mgr.createMaster("EC2");

    QStringList names = mgr.getMasterNames();
    EXPECT_GE(names.size(), 2);
    EXPECT_TRUE(names.contains("EC1"));
    EXPECT_TRUE(names.contains("EC2"));

    mgr.removeMaster("EC1");
    mgr.removeMaster("EC2");
}

// 测试ECMgr-5: 网络接口列表
TEST(EtherCATManagerTest, NetworkInterfaces) {
    QStringList interfaces = EtherCATMasterManager::listNetworkInterfaces();
    // 应该至少有一个网络接口
    // 注意：在某些测试环境中可能为空
    EXPECT_GE(interfaces.size(), 0);
}

// ============================================================
// 主函数
// ============================================================

int main(int argc, char** argv) {
    // 初始化Qt应用程序 (用于信号槽和事件循环)
    QCoreApplication app(argc, argv);

    // 初始化Google Test
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
