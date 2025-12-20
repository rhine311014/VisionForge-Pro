#pragma once

#ifdef VS_COMM_EXPORTS 
#define VS_COMM_API _declspec(dllexport)
#else
#define VS_COMM_API _declspec(dllimport)
#endif



// 协议类型
typedef enum enumProtocolType
{
	eProUnknown							= -1,				// 未知的通信协议
	eProCmdFast							= 0,				// 通用指令Fast通用无协议
	eProCmdVS							= 1,				// 通用指令VisionSmart通用无协议
	eProCmdYamaha						= 2,				// 通用指令雅马哈机器人无协议
	eProCmdEpson						= 3,				// 通用指令通用无协议预留1
	eProCmdReserved2					= 4,				// 通用指令通用无协议预留2
	eProCmdReserved3					= 5,				// 通用指令通用无协议预留3
	eProRegBase							= 10,				// PLC链接，大于该枚举值的都是PLC链接协议，直接读取存储PLC寄存器
	eProRegModbusSerialAscii			= 11,				// Modbus的寄存器串口ASCII协议
	eProRegModbusSerialRtu				= 12,				// Modbus的寄存器串口RTU协议
	eProRegModbusSocketTcp				= 13,				// Modbus的寄存器网口TCP协议
	eProRegModbusReserved1				= 14,				// Modbus的寄存器预留1
	eProRegModbusReserved2				= 15,				// Modbus的寄存器预留2
	eProRegPanasonicSerialMewtocol		= 21,				// 松下FP系列PLC的寄存器串口Mewtocol协议(一般使用这种)
	eProRegPanasonicSocketMewtocol		= 22,				// 松下FP系列PLC的寄存器网口Mewtocol协议
	eProRegPanasonicReserved1			= 23,				// 松下FP系列PLC的寄存器预留1
	eProRegPanasonicReserved2			= 24,				// 松下FP系列PLC的寄存器预留2
	eProRegPanasonicReserved3			= 25,				// 松下FP系列PLC的寄存器预留3
	eProRegMitsubishiQSerialAscii		= 31,				// 三菱Q系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	eProRegMitsubishiQSerialBinary		= 32,				// 三菱Q系列PLC的寄存器串口MC(格式5)二进制协议(一般使用这种)
	eProRegMitsubishiQSocketAscii		= 33,				// 三菱Q系列PLC的寄存器网口MC（E3）ASCII协议(一般使用这种)
	eProRegMitsubishiQSocketBinary		= 34,				// 三菱Q系列PLC的寄存器网口MC（E3）二进制协议(一般使用这种)
	eProRegMitsubishiQSocketReserved1	= 35,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）ASCII协议(一般使用这种)
	eProRegMitsubishiQSocketAsciiUDP	= 35,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）ASCII协议(一般使用这种)
	eProRegMitsubishiQSocketReserved2	= 36,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议(一般使用这种)	eProRegMitsubishiQSocketBinaryUDP	= 36,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议(一般使用这种)
	eProRegMitsubishiQSocketBinaryUDP	= 36,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议(一般使用这种)	eProRegMitsubishiQSocketBinaryUDP	= 36,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议(一般使用这种)
	eProRegMitsubishiFXSerialAscii		= 41,				// 三菱FX系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	eProRegMitsubishiFXSocketBinaryMC1E	= 42,				// 三菱FX系列PLC的寄存器网口MC（A兼容1E帧）二进制协议(FX-3U-ENET-ADP网口模块使用)
	eProRegMitsubishiFXReserved2		= 43,				// 三菱FX系列PLC的寄存器预留2
	eProRegMitsubishiFXReserved3		= 44,				// 三菱FX系列PLC的寄存器预留3
	eProRegMitsubishiFXReserved4		= 45,				// 三菱FX系列PLC的寄存器预留4
	eProRegYokogawaSocketAscii			= 51,				// 横河PLC的寄存器网口ASCII协议（PC-Link(以太网ASCII码)，一般用这种）
	eProRegYokogawaSocketBinary			= 52,				// 横河PLC的寄存器网口Binary协议（PC-Link(以太网二进制)）
	eProRegYokogawaPCLinkAscii			= 53,				// 横河PLC的寄存器网口PC-Link(ASCII码)
	eProRegYokogawaReserved1			= 54,				// 横河PLC的寄存器预留1
	eProRegYokogawaReserved2			= 55,				// 横河PLC的寄存器预留2

	eProRegKeyenceSerialAscii			= 61,				// 基恩士KV系列PLC的寄存器串口ASCII协议
	eProRegKeyenceSerialPCLink			= 62,				// 基恩士KV系列PLC的寄存器串口上位链路协议
	eProRegKeyenceSocketAscii			= 63,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）ASCII协议   (QnA兼容3E)
	eProRegKeyenceSocketBinary			= 64,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）二进制协议  (QnA兼容3E)
	eProRegKeyenceSocketPCLink			= 65,				// 基恩士KV系列PLC的寄存器网口上位链路协议
	eProRegKeyenceReserved1				= 66,				// 基恩士PLC的寄存器预留1

	eProRegYaskawa						= 71,				// 安川PLC的寄存器协议
	eProRegReserved1					= 81,				// PLC的寄存器操作协议预留1
	eProRegReserved2					= 82,				// PLC的寄存器操作协议预留2
	eProRegReserved3					= 83,				// PLC的寄存器操作协议预留3
	eProRegReserved4					= 84,				// PLC的寄存器操作协议预留4
	eProRegReserved5					= 85,				// PLC的寄存器操作协议预留5

	eProRegFujiSPH                      = 91,               // 富士MICREX-SX SPH系列 PLC的寄存器协议
	eProRegOmronSerialHostLink			= 101,				// 欧姆龙PLC的寄存器串口HostLink协议
	eProRegOmronSocketFins				= 102,				// 欧姆龙PLC的寄存器网口Fins协议
	eProRegOmronSocketCIP				= 103,				// 欧姆龙PLC的寄存器网口CIP协议

	eProRegDeltaModbusSerialAscii		= 106,				// Delta(台达)Modbus的寄存器串口ASCII协议
	eProRegDeltaModbusSerialRtu			= 107,				// Delta(台达)Modbus的寄存器串口RTU协议
	eProRegDeltaModbusSocketTcp			= 108,				// Delta(台达)Modbus的寄存器网口TCP协议
	eProRegDeltaModbusReserved1			= 109,				// Delta(台达)Modbus的寄存器预留1
	eProRegDeltaModbusReserved2			= 110,				// Delta(台达)Modbus的寄存器预留2

	eProRegSiemensModbusSerialAscii		= 116,				// Siemens(西门子)Modbus的寄存器预留1
	eProRegSiemensModbusSerialRtu		= 117,				// Siemens(西门子)Modbus的寄存器预留2
	eProRegSiemensModbusSocketTcp		= 118,				// Siemens(西门子)Modbus的寄存器网口TCP协议
	eProRegSiemensModbusReserved1		= 119,				// Siemens(西门子)Modbus的寄存器预留3
	eProRegSiemensModbusReserved2		= 120,				// Siemens(西门子)Modbus的寄存器预留4

}ProtocolType;

// 扩展通信协议类型；
typedef enum enumExProtocolType
{
	eExProDefault						= 0,				// 原通信协议
	eExProReserved1						= 1,			    // 扩展通用指令Fast通用无协议，及相应的寄存器通信指令
	eExProReserved2						= 2,			    // 扩展通用指令2
	eExProReserved3						= 3,			    // 扩展通用指令3
	eExProReserved4						= 4,			    // 扩展通用指令4
	eExProReserved5						= 5,			    // 扩展通用指令5

}ExProtocolType;