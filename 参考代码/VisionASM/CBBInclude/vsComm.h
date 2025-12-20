// vsComm.h: interface for the vcComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVCOMM_H__DDBA1997_6C53_438E_A59E_5C68F458A435__INCLUDED_)
#define AFX_SVCOMM_H__DDBA1997_6C53_438E_A59E_5C68F458A435__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommProRegDef.h"
#include <deque>
#include <vector>
using namespace std;

#pragma pack(push,8)

// 串口通信输入/输出缓冲区设置
#define SERIAL_MAX_IN_BUFFER		4096
#define SERIAL_MAX_OUT_BUFFER		4096

// 网络通信输入/输出缓冲区设置
#define NETWORK_MAX_IN_BUFFER		4096
#define NETWORK_MAX_OUT_BUFFER		4096


// 通信类型
typedef enum tagCommType
{
	eCommUnknown = -1,	// 未知类型	
	eSerial,			// 串口
	eSocket,			// 网络
	eADS,				// ADS通信
	eEtherCat,			// EtherCat通信
}CommType;

// 指令响应的反馈状态
typedef enum enumCommStatus
{
	eCommSucceed = 0,   // 成功
	eCommErr=1,         // 通信错误
	eCommUnConnect,		// 通信未连接
	eCommStop,			// 通信停止
	eCommTimeOut,       // 通信超时
}CommStatus;

// 通信模块数据
typedef struct tagCommInfo
{
	virtual CommType GetCommType() = 0;	  // 获取通讯类型
	virtual ~tagCommInfo(){}
}vcCommInfo;


// 
// 通信基类
//

class VS_COMM_API vcComm  
{
public:
	vcComm();
	virtual ~vcComm();

	virtual CommType GetCommType() = 0;											// 通信类型

	virtual bool Open(vcCommInfo *commInfo) = 0;								// 打开连接
	virtual bool Close() = 0;													// 关闭连接
	virtual bool SendData(CString strData) = 0;									// 发送数据
	virtual bool StartRecieveData() = 0;										// 开始接收数据
	virtual bool StopRecieveData() = 0;											// 停止接收数据
	virtual CommStatus RecieveData(CString &strData) = 0;								// 接收数据
	virtual CommStatus RecieveData2(CString &strData, CString &strFrontData) = 0;		// 接收数据
	virtual CommStatus RecieveDataAll(CString &strData);								// 接收所有数据
	virtual CommStatus RecieveDataLength(CString &strData, int nMinimumLength);			// 接收所有数据(最少MinimumLength长度)
	virtual bool ClearRecievedData() = 0;										// 清除接收到的指令和未处理的缓存数据
	virtual bool IsValid() = 0;													// 是否连接
	virtual void SetTimeout(int nTimeout) = 0;									// 设置超时
	static bool SplitCmdMsg(CString strRecieve, CString strCommStop, CString strCmdStop,  
		vector<CString> &cmdArray, CString &strRemain);							// 有通信终止符的指令解析

	static bool SplitCmdMsgMitsQSocketAscii(CString strRecieve, /*CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);							// 三菱Q系列PLC的寄存器网口MC（E3）ASCII协议的指令解析
	static bool SplitCmdMsgMitsQSocketBinary(CString strRecieve, /*CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);							// 三菱Q系列PLC的寄存器网口MC（E3）二进制协议的指令解析
	static bool SplitCmdMsgModbusSocketTcp(CString strRecieve,/* CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);							// Modbus的寄存器网口TCP协议

	static bool SplitCmdMsgModbusSerialRtu(CString strRecieve,/* CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);							// Modbus的寄存器网口TCP协议

	// 基恩士PLC 
	static bool SplitCmdMsgKeneyceSocketAscii(CString strRecieve,/* CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);	

	static bool SplitCmdMsgKeneyceSocketBinary(CString strRecieve,/* CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);	

	//富士PLC
	static bool SplitCmdMsgFuJiSPHSocket(CString strRecieve, /*CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);							// 三菱Q系列PLC的寄存器网口MC（E3）二进制协议的指令解析

	// 恒河网口二进制
	static bool SplitCmdMsgYokogawaSocketBinary(CString strRecieve, /*CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);	

	//三菱FX系列PLC的寄存器网口MC（A兼容1E帧）二进制协议(FX-3U-ENET-ADP网口模块使用)
	static bool SplitCmdMsgMitsFXSocketBinaryMC1E(CString strRecieve, /*CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain, int nReadDataNumber);							

	// 欧姆龙PLC的寄存器网口Fins协议
	static bool SplitCmdMsgOmronSocketFins(CString strRecieve, /*CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);

	// 欧姆龙PLC的寄存器网口CIP协议
	static bool SplitCmdMsgOmronSocketCIP(CString strRecieve, /*CString strCommStop, CString strCmdStop,*/  
		vector<CString> &cmdArray, CString &strRemain);	


	virtual bool SetPlatformIndex(int nPlatformIndex) = 0;						// 设置平台序号
	virtual void SetProtocolType(ProtocolType ptProtocolType) = 0;				// 设置协议类型
	virtual ProtocolType GetProtocolType() = 0;									// 获取通信协议类型
	void AlignLogRecord(CString strlog, int nPlatformIndex);
	void AlignLogRecord1(CString strlog, int nPlatformIndex);
public:
	static CString CalcCrcFast(CString puchMsg);

protected:
	void	SetReadAdressNumber(int nReadNumber);
	int		GetReadAdressNumber();
	int		m_nReadDataNumber;
};
#pragma pack(pop)

#endif // !defined(AFX_SVCOMM_H__DDBA1997_6C53_438E_A59E_5C68F458A435__INCLUDED_)
