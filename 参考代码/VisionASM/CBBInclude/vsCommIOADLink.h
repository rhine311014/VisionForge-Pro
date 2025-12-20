#pragma once

#include "vsCommIOBase.h"
#include <map>

#define eIO 4


typedef enum tagIOCardType
{
	eCardTypeUnknown = -1,	// 未知类型	
	ePCI7432 = 16,			// PCI_7432
	ePCI7433 = 17,			// PCI_7433
	ePCI7434 = 18,			// PCI_7434
}IOCardType;


//typedef struct tagIOCardTypeInfo
//{
//	tagIOCardTypeInfo()
//	{
//		m_mapIOCardTypeInfo.insert(std::map<IOCardType,CString>::value_type(ePCI7432 , "PCI_7432"));
//		m_mapIOCardTypeInfo.insert(std::map<IOCardType,CString>::value_type(ePCI7433 , "PCI_7433"));
//		m_mapIOCardTypeInfo.insert(std::map<IOCardType,CString>::value_type(ePCI7434 , "PCI_7434"));
//
//	}
//	std::map<IOCardType , CString> m_mapIOCardTypeInfo;
//}vcIOCardTypeInfo;


class VS_COMM_IO_API  vcCommIOADLink: public vcCommIOBase
{
public:
	vcCommIOADLink(void);
	virtual ~vcCommIOADLink(void);


	virtual CommType GetCommType();											// 通信类型

	virtual int	GetCardBrandType();											// 卡品牌类型

	// 打开连接
	virtual bool Open(vcCommInfo *commInfo);	

	// 关闭连接
	virtual bool Close();	

	// 是否连接
	virtual bool IsValid();	

	// 设置超时
	virtual void SetTimeout(int nTimeout);	
	// 设置输出开机默认状态
	virtual void SetOutputInitState(OutputDefaultState state) ;

	// 获取是否可以设置开机默认输出状态
	virtual bool GetIsOutputInitEnable() ;

	virtual bool SetOutputState(int nIOIndex , bool bState);
	virtual bool SetOutputState(int nStartIO, int nIONum, std::vector<bool> vbWriteData);

	virtual bool GetOutputState(int nIOIndex , bool& bState);
	virtual bool GetOutputState(int nStartIO, int nIONum, std::vector<bool> &vbReadData);

	virtual bool GetInputState(int nIOIndex , bool& bState);
	virtual bool GetInputState(long nStartIO, int nIONum ,std::vector<bool> &vbReadData);

private:

	unsigned int m_unWaitTimeOut;

	short	m_hCard;
	//unsigned char[] InToByteArr(int nInput,bool bDWord);

	std::vector<DWORD> m_vdwMask;

};

