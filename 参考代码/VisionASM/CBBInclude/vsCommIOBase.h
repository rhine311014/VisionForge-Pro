#pragma once

#include "vsComm.h"
#include <map>

#define eIO 4

#ifdef VS_COMM_IO_EXPORTS 
#define VS_COMM_IO_API _declspec(dllexport)
#else
#define VS_COMM_IO_API _declspec(dllimport)
#endif

typedef enum tagOutputDefaultState
{
	eOutputDefault_Close = 0,	// 默认闭	
	eOutputDefault_Open = 1,	// 默认开

}OutputDefaultState;
typedef struct tagIOCommInfo : vcCommInfo
{
	WORD	m_wCardType;
	WORD	m_wCardID;


	tagIOCommInfo()
	{
		m_wCardType = 16;
		m_wCardID = 0;
	}

	tagIOCommInfo(const tagIOCommInfo &ioCommInfo)
	{
		m_wCardType = ioCommInfo.m_wCardType;
		m_wCardID = ioCommInfo.m_wCardID;
	}

	tagIOCommInfo & operator= (const tagIOCommInfo &ioCommInfo)
	{
		if (this == &ioCommInfo)
		{
			return *this;
		}
		m_wCardType = ioCommInfo.m_wCardType;
		m_wCardID = ioCommInfo.m_wCardID;

		return *this;
	}
	virtual CommType GetCommType()
	{
		return (CommType)eIO;
	}

}vcIOCommInfo;


class VS_COMM_IO_API  vcCommIOBase: public vcComm
{
public:
	vcCommIOBase(void);
	virtual ~vcCommIOBase(void);


	virtual CommType GetCommType();											// 通信类型

	virtual int	GetCardBrandType() = 0;											// 卡品牌类型

	// 打开连接
	virtual bool Open(vcCommInfo *commInfo) = 0;	

	// 关闭连接
	virtual bool Close() = 0;	

	// 是否连接
	virtual bool IsValid() = 0;	

	// 设置超时
	virtual void SetTimeout(int nTimeout) = 0;	

	// 设置输出开机默认状态
	virtual void SetOutputInitState(OutputDefaultState state) = 0;

	// 获取是否可以设置开机默认输出状态
	virtual bool GetIsOutputInitEnable() = 0;

	virtual bool SetOutputState(int nIOIndex , bool bState) = 0;
	virtual bool SetOutputState(int nStartIO, int nIONum, std::vector<bool> vbWriteData) = 0;

	virtual bool GetOutputState(int nIOIndex , bool& bState) = 0;
	virtual bool GetOutputState(int nStartIO, int nIONum, std::vector<bool> &vbReadData) = 0;

	virtual bool GetInputState(int nIOIndex , bool& bState) = 0;
	virtual bool GetInputState(long nStartIO, int nIONum ,std::vector<bool> &vbReadData) = 0;

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


};

