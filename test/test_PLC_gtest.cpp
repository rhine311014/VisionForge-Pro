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
// 协议工厂测试 (3个测试用例)
// ============================================================

// 测试33: 协议工厂创建
TEST(PLCProtocolFactoryTest, CreateProtocol) {
    auto modbusProtocol = PLCProtocolFactory::create(ProtocolType::ModbusTCP);
    EXPECT_NE(modbusProtocol, nullptr);
    EXPECT_EQ(modbusProtocol->protocolType(), ProtocolType::ModbusTCP);

    auto mcProtocol = PLCProtocolFactory::create(ProtocolType::MitsubishiMC3E);
    EXPECT_NE(mcProtocol, nullptr);
    EXPECT_EQ(mcProtocol->protocolType(), ProtocolType::MitsubishiMC3E);

    auto unsupported = PLCProtocolFactory::create(ProtocolType::Unknown);
    EXPECT_EQ(unsupported, nullptr);
}

// 测试34: 支持的协议列表
TEST(PLCProtocolFactoryTest, SupportedProtocols) {
    auto protocols = PLCProtocolFactory::supportedProtocols();

    EXPECT_FALSE(protocols.empty());
    EXPECT_TRUE(std::find(protocols.begin(), protocols.end(), ProtocolType::ModbusTCP) != protocols.end());
    EXPECT_TRUE(std::find(protocols.begin(), protocols.end(), ProtocolType::MitsubishiMC3E) != protocols.end());
}

// 测试35: 协议支持检查
TEST(PLCProtocolFactoryTest, IsSupported) {
    EXPECT_TRUE(PLCProtocolFactory::isSupported(ProtocolType::ModbusTCP));
    EXPECT_TRUE(PLCProtocolFactory::isSupported(ProtocolType::MitsubishiMC3E));
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
// 主函数
// ============================================================

int main(int argc, char** argv) {
    // 初始化Qt应用程序 (用于信号槽和事件循环)
    QCoreApplication app(argc, argv);

    // 初始化Google Test
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
