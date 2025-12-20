#pragma once

#include "vsCommIOBase.h"
#include <map>

#define eIO 4


typedef enum tagIOCardTypeFY
{
	//eCardTypeUnknown = -1,	// 未知类型	
	ePCIFY6400 = 1,			// FY6400
}IOCardTypeFY;


class VS_COMM_IO_API  vcCommIOFY: public vcCommIOBase
{
public:
	vcCommIOFY(void);
	virtual ~vcCommIOFY(void);


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


	HANDLE	m_hCard;
	std::vector<int> m_vnMask;
};

