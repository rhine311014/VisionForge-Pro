#pragma once

#include <vector>
#include "vsComm.h"

#pragma pack(push,8)
class CCommProADSEx;
// 网络通信数据信息
typedef struct tagADSCommInfo : vcCommInfo
{
	// 网络通信部分
	unsigned char  m_byAddr[6];				// IP地址
	unsigned int   m_uiPort;				// 端口
	long           m_lRegAddress;			// 寄存器首地址
	long		   m_lTempRegAddress;		// 临时寄存器地址（二维码、一次发送多个对位位置等）

	tagADSCommInfo()
	{
		m_uiPort			= 851;
		m_byAddr[0]			= 192;
		m_byAddr[1]			= 168;
		m_byAddr[2]			= 1;
		m_byAddr[3]			= 1;
		m_byAddr[4]			= 1;
		m_byAddr[5]			= 1;
		m_lRegAddress       = 100;
		m_lTempRegAddress   = 200;
	}

	tagADSCommInfo(const tagADSCommInfo &adsCommInfo)
	{

		m_uiPort		= adsCommInfo.m_uiPort;
		for (int i=0; i<6; i++)
		{
			m_byAddr[i] = adsCommInfo.m_byAddr[i];
		}
		m_lRegAddress   = adsCommInfo.m_lRegAddress;
		m_lTempRegAddress = adsCommInfo.m_lTempRegAddress;

	}

	tagADSCommInfo & operator= (const tagADSCommInfo &adsCommInfo)
	{
		if (this == &adsCommInfo)
		{
			return *this;
		}

		m_uiPort = adsCommInfo.m_uiPort;
		
		for (int i=0; i<6; i++)
		{
			m_byAddr[i] = adsCommInfo.m_byAddr[i];
		}

		m_lRegAddress   = adsCommInfo.m_lRegAddress;
		m_lTempRegAddress = adsCommInfo.m_lTempRegAddress;
	
		return *this;
	}
	virtual CommType GetCommType()
	{
		return eADS;
	}

}vcADSCommInfo;


class VS_COMM_API CCommProADS: public vcComm
{
public:
	CCommProADS(void);
	virtual ~CCommProADS(void);

	virtual CommType GetCommType();											// 通信类型

public:
	// 打开连接
	virtual bool Open(vcCommInfo *commInfo);	
	// 关闭连接
	virtual bool Close();		
	// 是否连接
	virtual bool IsValid();		
	// 设置超时
	virtual void SetTimeout(int nTimeout);	

	virtual bool SendData(long nRegAddr, int nDataNum, std::vector<int> vWriteData, bool bDWord);
	virtual bool RecieveData(long nRegAddr, int nDataNum ,std::vector<int> &vReadData, bool bDWord);

public:
	virtual bool SendData(CString strData);									// 发送数据
	virtual bool StartRecieveData();										// 开始接收数据
	virtual bool StopRecieveData();											// 停止接收数据
	virtual CommStatus RecieveData(CString &strData);								// 接收数据
	virtual CommStatus RecieveData2(CString &strData, CString &strFrontData);		// 接收数据
	virtual bool ClearRecievedData();										// 清除接收到的指令和未处理的缓存数据
	virtual bool SetPlatformIndex(int nPlatformIndex);						// 设置平台序号
	virtual void SetProtocolType(ProtocolType ptProtocolType);				// 设置协议类型
	virtual ProtocolType GetProtocolType();									// 获取通信协议类型


public:
	vcADSCommInfo	m_CommInfo;
private:
	CCommProADSEx*	m_pCommProADS;
	
};

#pragma pack(pop)