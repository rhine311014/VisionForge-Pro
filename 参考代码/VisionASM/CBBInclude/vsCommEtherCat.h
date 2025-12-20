#pragma once

#include "vsComm.h"

#ifdef VS_COMM_ETHERCAT_EXPORTS
#define VS_COMM_ETHERCAT_API _declspec(dllexport)
#else
#define VS_COMM_ETHERCAT_API _declspec(dllimport)
#endif

#define  BytesNumber 256


typedef struct tagEtherCATCommInfo : vcCommInfo
{
	unsigned int  m_unSerialNumBoard;
	unsigned int m_unIndexChannel;

	tagEtherCATCommInfo()
	{
		m_unSerialNumBoard = 0;
		m_unIndexChannel = 0;
	}

	tagEtherCATCommInfo(const tagEtherCATCommInfo &etherCATCommInfo)
	{
		m_unSerialNumBoard = etherCATCommInfo.m_unSerialNumBoard;
		m_unIndexChannel = etherCATCommInfo.m_unIndexChannel;
	}

	tagEtherCATCommInfo & operator= (const tagEtherCATCommInfo &etherCATCommInfo)
	{
		if (this == &etherCATCommInfo)
		{
			return *this;
		}
		m_unSerialNumBoard = etherCATCommInfo.m_unSerialNumBoard;
		m_unIndexChannel = etherCATCommInfo.m_unIndexChannel;

		return *this;
	}
	virtual CommType GetCommType()
	{
		return eEtherCat;
	}

}vcCommEtherCatInfo;

class VS_COMM_ETHERCAT_API vcCommEtherCat: public vcComm
{
public:
	vcCommEtherCat(void);
	virtual ~vcCommEtherCat(void);


	virtual CommType GetCommType();											// 通信类型
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

	vector<unsigned int> GetAllBoardsSerialNum();

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


private:
	HANDLE  m_hDriver;
	HANDLE m_hChannel;

	unsigned int m_unWaitTimeOut;


	//unsigned char[] InToByteArr(int nInput,bool bDWord);

};

