// vsSerialComm.h: interface for the vcSerialComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVSERIALCOMM_H__67B55171_96E0_47E3_8D99_0386C5B18517__INCLUDED_)
#define AFX_SVSERIALCOMM_H__67B55171_96E0_47E3_8D99_0386C5B18517__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vsComm.h"
#include <deque>
#include <vector>
using namespace std;

#pragma pack(push,8)

// 串口奇偶校验
typedef enum tagSerialParity
{
	eNOPARITY = 0,		// 不校验
		eODDPARITY,			// 奇校验
		eEVENPARITY,		// 偶校验
		eMARKPARITY,        // 标记校验
		eSPACEPARITY        // 空格校验
}SerialParity;

// 串口停止位
typedef enum tagSerialStopBits
{
	eONESTOPBIT = 0,	// 1位
		eONE5STOPBITS,		// 1.5位
		eTWOSTOPBITS        // 2为
}SerialStopBits;

// 串口通信数据信息
typedef struct tagSerialCommInfo : public vcCommInfo
{
	unsigned int	m_uiComPort;			// 串口号，从0开始
	unsigned int	m_uiBaudRate;           // 波特率
	SerialParity	m_Parity;               // 奇偶校验
	SerialStopBits  m_StopBits;				// 停止位
	unsigned int	m_uiDataBits;           // 数据位
	CString			m_strCommStop;          // 通信终止符
	CString			m_strCmdStop;			// 协议命令终止符
	long            m_lRegAddress;	        // 寄存器首地址
	long		    m_lTempRegAddress;		// 临时寄存器地址（二维码、一次发送多个对位位置等）
	
	long		   m_lWriteRegMaxNum;		// 单次最大写入寄存器数量
	long		   m_lReadRegMaxNum;		// 单次最大读取寄存器数量
	tagSerialCommInfo()
	{
		m_uiComPort			= 0;
		m_uiBaudRate		= 9600;
		m_Parity			= eNOPARITY;
		m_StopBits			= eONESTOPBIT;
		m_uiDataBits		= 8;
		m_strCommStop		= _T("");
		m_strCmdStop		= _T("");
		m_lRegAddress = 100;
		m_lTempRegAddress = 200;

		m_lWriteRegMaxNum	= 100;
		m_lReadRegMaxNum	= 100;
	}
	
	tagSerialCommInfo(const tagSerialCommInfo &serailCommInfo)
	{
		m_uiComPort			= serailCommInfo.m_uiComPort;
		m_uiBaudRate		= serailCommInfo.m_uiBaudRate;
		m_Parity			= serailCommInfo.m_Parity;
		m_StopBits			= serailCommInfo.m_StopBits;
		m_uiDataBits		= serailCommInfo.m_uiDataBits;
		m_strCommStop		= serailCommInfo.m_strCommStop;
		m_strCmdStop		= serailCommInfo.m_strCmdStop;
		m_lRegAddress		= serailCommInfo.m_lRegAddress;
		m_lTempRegAddress	= serailCommInfo.m_lTempRegAddress;

		m_lWriteRegMaxNum	= serailCommInfo.m_lWriteRegMaxNum;
		m_lReadRegMaxNum	= serailCommInfo.m_lReadRegMaxNum;
	}
	
	tagSerialCommInfo& operator= (const tagSerialCommInfo &serailCommInfo)
	{
		if (this == &serailCommInfo)
		{
			return *this;
		}
		
		m_uiComPort			= serailCommInfo.m_uiComPort;
		m_uiBaudRate		= serailCommInfo.m_uiBaudRate;
		m_Parity			= serailCommInfo.m_Parity;
		m_StopBits			= serailCommInfo.m_StopBits;
		m_uiDataBits		= serailCommInfo.m_uiDataBits;
		m_strCommStop		= serailCommInfo.m_strCommStop;
		m_strCmdStop		= serailCommInfo.m_strCmdStop;
		m_lRegAddress		= serailCommInfo.m_lRegAddress;
		m_lTempRegAddress	= serailCommInfo.m_lTempRegAddress;

		m_lWriteRegMaxNum	= serailCommInfo.m_lWriteRegMaxNum;
		m_lReadRegMaxNum	= serailCommInfo.m_lReadRegMaxNum;
		
		return *this;
	}
	
	bool operator==(const tagSerialCommInfo &serailCommInfo) const
	{
		if (m_uiComPort != serailCommInfo.m_uiComPort) return false;
		if (m_uiBaudRate != serailCommInfo.m_uiBaudRate) return false;
		if (m_Parity != serailCommInfo.m_Parity) return false;
		if (m_StopBits != serailCommInfo.m_StopBits) return false;
		if (m_uiDataBits != serailCommInfo.m_uiDataBits) return false;
		if (m_strCommStop != serailCommInfo.m_strCommStop) return false;
		if (m_strCmdStop != serailCommInfo.m_strCmdStop) return false;
		if (m_lRegAddress != serailCommInfo.m_lRegAddress) return false;
		if (m_lTempRegAddress != serailCommInfo.m_lTempRegAddress) return false;

		return true;
	}

	virtual CommType GetCommType()
	{
		return eSerial;
	}
	
}vcSerialCommInfo;

//
// 串口通信类
// 
class VS_COMM_API vcSerialComm : public vcComm
{
public:
	vcSerialComm();
	virtual ~vcSerialComm();

	virtual CommType GetCommType();											// 通信类型

	virtual bool Open(vcCommInfo *commInfo);								// 打开连接
	virtual bool Close();													// 关闭连接
	virtual bool SendData(CString strData);									// 发送数据
	virtual bool StartRecieveData();										// 开始接收数据
	virtual bool StopRecieveData();											// 停止接收数据
	virtual CommStatus RecieveData(CString &strData);								// 接收数据
	virtual CommStatus RecieveData2(CString &strData, CString &strFrontData);		// 接收数据
	virtual bool ClearRecievedData();										// 清除接收到的指令和未处理的缓存数据
	virtual bool IsValid();													// 是否连接
	virtual bool SetPlatformIndex(int nPlatformIndex);						// 设置平台序号
	virtual void SetProtocolType(ProtocolType ptProtocolType);				// 设置协议类型
	virtual ProtocolType GetProtocolType();									// 获取通信协议类型
	void AlignLogRecord(CString strlog);
	virtual void SetTimeout(int nTimeout);									// 设置超时
	
	vcSerialCommInfo GetSerialCommInfo();						// 获取Open时所用的连接信息

private:
	int ReadBlock(BYTE *pStr, unsigned int uiLength);						// 读数据
	int WriteBlock(BYTE *pStr, unsigned int uiLength);						// 写数据
	static ULONG WINAPI CommThread(PVOID pParam);							// 串口处理线程函数

private:
	HANDLE				m_hFile;			// 文件句柄
	bool				m_bConnected;		// 连接状态
	HANDLE				m_hThreadComm;		// 处理线程句柄
	HANDLE				m_hEvtThreadExit;	// 线程退出事件
	OVERLAPPED			m_osWrite;			// 异步写结构体
	OVERLAPPED			m_osRead;			// 异步读结构体

// 	bool				m_bDataRecieved;	// 接收是否完成
	CString				m_strRecieve;		// 接收数据
	int					m_RecLen;
	int					m_len;
// 	CString				m_strAppend;		// 追加字符
	vcSerialCommInfo	m_SerialCommInfo;	// 信息
	deque<CString>		m_RecievedMsgArray; // 接收命令队列
	CRITICAL_SECTION	m_csRecieve;			

	int					m_nPlatformIndex;	// 平台序号
	ProtocolType		m_ptProtocolType;	// 通信协议类型

	HANDLE				m_phReceiveEvents[3];	// [0]:接收事件;[1]:停止采集事件;[3]:线程出错事件
	int					m_nTimeout;				// 超时
	bool				m_bClientRecStop;		// 连接状态
};
#pragma pack(pop)

#endif // !defined(AFX_SVSERIALCOMM_H__67B55171_96E0_47E3_8D99_0386C5B18517__INCLUDED_)
