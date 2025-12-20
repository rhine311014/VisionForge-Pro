#pragma once

#include "vsCommIOBase.h"
#include <map>

#define eIO 4


typedef enum tagIOGPIOU9H7BType
{
	//eGPIOTypeUnknown = -1,	// 未知类型	
	eGPIOTypeU9H7B = 0,
}IOGPIOU9H7BType;

class VS_COMM_IO_API  vcCommIOGPIOU9H7B: public vcCommIOBase
{
public:
	vcCommIOGPIOU9H7B(void);
	virtual ~vcCommIOGPIOU9H7B(void);


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

	virtual bool SetOutputState(int nIOIndex, bool bState);
	virtual bool SetOutputState(int nStartIO, int nIONum, std::vector<bool> vbWriteData);

	virtual bool GetOutputState(int nIOIndex , bool& bState);
	virtual bool GetOutputState(int nStartIO, int nIONum, std::vector<bool> &vbReadData);

	virtual bool GetInputState(int nIOIndex , bool& bState);
	virtual bool GetInputState(long nStartIO, int nIONum ,std::vector<bool> &vbReadData);

	//设置、读取GPIO模式
	bool GetIOMode(int nIOIndex , bool& bState);
	bool GetIOMode(int nStartIO, int nIONum, std::vector<bool> &vbReadData);
	//不支持设置IO模式，直接返回false
	bool SetIOMode(int nIOIndex , bool& bState);
	bool SetIOMode(long nStartIO, int nIONum ,std::vector<bool> &vbReadData);

	//设置GPIO的模式，PNP或NPN
	bool SetGPOMode(bool bIsPNP);
	bool GetGPOMode(bool &bIsPNP);

private:


	//unsigned char[] InToByteArr(int nInput,bool bDWord);

	std::vector<DWORD> m_vdwMask;

	bool m_bIsOpen;

	//winio相关接口
};

