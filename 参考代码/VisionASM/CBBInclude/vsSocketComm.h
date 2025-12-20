// vsSocketComm.h: interface for the vcSocketComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVSOCKETCOMM_H__498F0959_C1C3_42C8_AF03_FD2D9F40617F__INCLUDED_)
#define AFX_SVSOCKETCOMM_H__498F0959_C1C3_42C8_AF03_FD2D9F40617F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment (lib, "wsock32.lib")
#include "vsComm.h"
#include <winsock.h>
#include <vector>

#pragma pack(push,8)

// Socket类别
typedef enum tagSocketType
{
	eSTUnknown = -1,	// 未知类型	
		eSTServer,			// 服务端
		eSTClient			// 客户端
}SocketType;

typedef enum tagSocketProtoType
{
	eSPTUnknown = -1,		// 未知类型	
	eSPTProtoTCP,			// The Transmission Control Protocol (TCP)
	eSPTProtoUDP			// The User Datagram Protocol (UDP)
}SocketProtoType;

// 网络通信数据信息
typedef struct tagSocketCommInfo : public vcCommInfo
{
	// 网络通信部分
	SocketType     m_socketType;			// Socket类型，服务端或客户端
	SocketProtoType m_socketProtoType;		// Socket通信协议类型，TCP/UDP
	unsigned char  m_byAddr[4];				// IP地址
	unsigned int   m_uiPort;				// 端口
	unsigned int   m_uiListenNum;			// 服务端监听个数
	CString		   m_strCommStop;           // 通信终止符
	CString		   m_strCmdStop;			// 协议命令终止符
	long           m_lRegAddress;			// 寄存器首地址
	long		   m_lTempRegAddress;		// 临时寄存器地址（二维码、一次发送多个对位位置等）
	
	long		   m_lWriteRegMaxNum;		// 单次最大写入寄存器数量
	long		   m_lReadRegMaxNum;		// 单次最大读取寄存器数量
	long          m_lRegAddress3;			// ID寄存器首地址
	long          m_lWZRegAddress;			// 寄存器首地址
	CString       m_szGetName;
	double        m_dWZW;
	double        m_dWZR;
	double        m_dWZY1;
	double        m_dWZY2;
	double        m_dWZY3;
	double        m_dWZZ1;
	double        m_dWZZ2;
	double        m_dWZZ3;

	//	
	int	 m_nIDLength;
	BOOL m_bEnableIDRaed;
	BOOL m_bIDInvert;
	
	tagSocketCommInfo()
	{
		m_socketType		= eSTUnknown;
		m_socketProtoType	= eSPTProtoTCP;
		m_uiPort			= 2000;
		m_uiListenNum		= 10;			// 默认为10个监听端
		m_strCommStop		= _T("");
		m_strCmdStop		= _T("");
		memset(m_byAddr, 0, sizeof(BYTE)*4);
		m_lRegAddress       = 100;
		m_lTempRegAddress   = 200;

		m_lWriteRegMaxNum	= 100;
		m_lReadRegMaxNum	= 100;
		
		m_lRegAddress3      = 150;
		m_lWZRegAddress     = 100;
		m_szGetName         = _T("");
		m_dWZW                =0;
		m_dWZR                =0;
		m_dWZY1               =0;
		m_dWZY2               =0;
		m_dWZY3               =0;
		m_dWZZ1               =0;
		m_dWZZ2               =0;
		m_dWZZ3               =0;

		m_nIDLength          = 17;
		m_bEnableIDRaed      = FALSE;
		m_bIDInvert          = FALSE;
	}
	
	tagSocketCommInfo(const tagSocketCommInfo &socketCommInfo)
	{
		m_socketType	= socketCommInfo.m_socketType;
		m_socketProtoType = socketCommInfo.m_socketProtoType;
		m_uiPort		= socketCommInfo.m_uiPort;
		m_uiListenNum	= socketCommInfo.m_uiListenNum;
		m_strCommStop	= socketCommInfo.m_strCommStop;
		m_strCmdStop	= socketCommInfo.m_strCmdStop;
		for (int i=0; i<4; i++)
		{
			m_byAddr[i] = socketCommInfo.m_byAddr[i];
		}
		m_lRegAddress   = socketCommInfo.m_lRegAddress;
		m_lTempRegAddress = socketCommInfo.m_lTempRegAddress;

		m_lWriteRegMaxNum	= socketCommInfo.m_lWriteRegMaxNum;
		m_lReadRegMaxNum	= socketCommInfo.m_lReadRegMaxNum;
		
		m_lWZRegAddress   = socketCommInfo.m_lWZRegAddress;
		m_lRegAddress3   = socketCommInfo.m_lRegAddress3;
		m_lTempRegAddress = socketCommInfo.m_lTempRegAddress;

		m_szGetName           =socketCommInfo.m_szGetName;
		m_dWZW                =socketCommInfo.m_dWZW;
		m_dWZR                =socketCommInfo.m_dWZR;
		m_dWZY1               =socketCommInfo.m_dWZY1;
		m_dWZY2               =socketCommInfo.m_dWZY2;
		m_dWZY3               =socketCommInfo.m_dWZY2;
		m_dWZZ1               =socketCommInfo.m_dWZZ1;
		m_dWZZ2               =socketCommInfo.m_dWZZ2;
		m_dWZZ3               =socketCommInfo.m_dWZZ3;

		m_nIDLength          = socketCommInfo.m_nIDLength;
		m_bEnableIDRaed      = socketCommInfo.m_bEnableIDRaed;
		m_bIDInvert          = socketCommInfo.m_bIDInvert;
	}
	
	tagSocketCommInfo & operator= (const tagSocketCommInfo &socketCommInfo)
	{
		if (this == &socketCommInfo)
		{
			return *this;
		}
		
		m_socketType	= socketCommInfo.m_socketType;
		m_socketProtoType = socketCommInfo.m_socketProtoType;
		m_uiPort		= socketCommInfo.m_uiPort;
		m_uiListenNum	= socketCommInfo.m_uiListenNum;
		m_strCommStop	= socketCommInfo.m_strCommStop;
		m_strCmdStop	= socketCommInfo.m_strCmdStop;

		for (int i=0; i<4; i++)
		{
			m_byAddr[i] = socketCommInfo.m_byAddr[i];
		}

		m_lRegAddress   = socketCommInfo.m_lRegAddress;
		m_lTempRegAddress = socketCommInfo.m_lTempRegAddress;

		m_lWriteRegMaxNum	= socketCommInfo.m_lWriteRegMaxNum;
		m_lReadRegMaxNum	= socketCommInfo.m_lReadRegMaxNum;		
		
		m_lWZRegAddress   = socketCommInfo.m_lWZRegAddress;
		m_lRegAddress3   = socketCommInfo.m_lRegAddress3;
		m_lTempRegAddress = socketCommInfo.m_lTempRegAddress;
		m_szGetName           =socketCommInfo.m_szGetName;
		m_dWZW                =socketCommInfo.m_dWZW;
		m_dWZR                =socketCommInfo.m_dWZR;
		m_dWZY1               =socketCommInfo.m_dWZY1;
		m_dWZY2               =socketCommInfo.m_dWZY2;
		m_dWZY3               =socketCommInfo.m_dWZY2;
		m_dWZZ1               =socketCommInfo.m_dWZZ1;
		m_dWZZ2               =socketCommInfo.m_dWZZ2;
		m_dWZZ3               =socketCommInfo.m_dWZZ3;

		m_nIDLength          = socketCommInfo.m_nIDLength;
		m_bEnableIDRaed      = socketCommInfo.m_bEnableIDRaed;
		m_bIDInvert          = socketCommInfo.m_bIDInvert;

		return *this;
	}
	
	bool operator==(const tagSocketCommInfo &socketCommInfo) const
	{
		if (m_socketType != socketCommInfo.m_socketType) return false;
		if (m_socketProtoType != socketCommInfo.m_socketProtoType) return false;
		if (m_byAddr[0] != socketCommInfo.m_byAddr[0]) return false;
		if (m_byAddr[1] != socketCommInfo.m_byAddr[1]) return false;
		if (m_byAddr[2] != socketCommInfo.m_byAddr[2]) return false;
		if (m_byAddr[3] != socketCommInfo.m_byAddr[3]) return false;
		if (m_uiPort != socketCommInfo.m_uiPort) return false;
		if (m_uiListenNum != socketCommInfo.m_uiListenNum) return false;
		if (m_strCommStop != socketCommInfo.m_strCommStop) return false;
		if (m_strCmdStop != socketCommInfo.m_strCmdStop) return false;
		if (m_lRegAddress != socketCommInfo.m_lRegAddress) return false;
		if (m_lTempRegAddress != socketCommInfo.m_lTempRegAddress) return false;

		return true;
	}

	virtual CommType GetCommType()
	{
		return eSocket;
	}
	
}vcSocketCommInfo;

//
// 网络通信类
//
class VS_COMM_API vcSocketComm : public vcComm
{
public:
	vcSocketComm();
	virtual ~vcSocketComm();

	virtual CommType GetCommType();											// 通信类型

public:
	//////////////////////////////////公共函数//////////////////////////////
	static  bool InitWSA();													// 通信库初始化
	static  bool UnInitWSA();												// 通信库退出
	static  bool GetHostName(CString &strHostIPAddr);						// 获取主机名字
	static  bool GetHostIPAddr(CString &strHostIPAddr);						// 获取主机IP地址

public:
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
	virtual void SetTimeout(int nTimeout);											// 设置超时

	bool GetLocalCommInfo(vcCommInfo *commInfo);								// 获取作为客户端时本地IP的地址

	vcSocketCommInfo GetSocketCommInfo();							// 获取Open时所用的连接信息

private:
	static ULONG  WINAPI ServerCommThread(PVOID pParam);					// 服务端通信处理线程函数
	static ULONG  WINAPI ClientCommThread(PVOID pParam);					// 客户端通信处理线程函数

private:
	SocketType			m_SocketType;		// SOCKET类型
	SOCKET				m_hSocket;			// SOCKET句柄
	bool				m_bConnected;		// 连接状态
	HANDLE 				m_hThreadComm;		// 处理线程
	HANDLE				m_hEvtThreadExit;	// 线程对出事件
	UINT				m_uPort;			// 端口
	CString				m_strServer;		// 服务端IP地址
	std::vector<SOCKET> m_hAcceptArray;		// 服务端接收客户端句柄数组
	std::vector<sockaddr_in>  m_vSockAddrIn;// UDP服务端接收客户端地址数组

// 	bool				m_bDataRecieved;	// 是否接收完成
	CString				m_strRecieve;		// 接收数据
// 	CString				m_strAppend;		// 追加字符
	vcSocketCommInfo	m_SocketCommInfo;	// 信息
	deque<CString>		m_RecievedMsgArray; // 接收命令队列
	CRITICAL_SECTION	m_csRecieve;			

	char				m_cBuf[NETWORK_MAX_IN_BUFFER];
	bool				m_bClientRecStop;		// 连接状态

	int					m_nPlatformIndex;	// 平台序号
	ProtocolType		m_ptProtocolType;	// 通信协议类型

	HANDLE				m_phReceiveEvents[3];	// [0]:接收事件;[1]:停止采集事件；[3]:线程出错事件
	int					m_nTimeout;			// 超时
	CRITICAL_SECTION	m_csAcceptArray;	// 接收临界区

	vcSocketCommInfo	m_LocalSocketCommInfo;	// 获取作为客户端时本地IP的地址

};
#pragma pack(pop)

#endif // !defined(AFX_SVSOCKETCOMM_H__498F0959_C1C3_42C8_AF03_FD2D9F40617F__INCLUDED_)
