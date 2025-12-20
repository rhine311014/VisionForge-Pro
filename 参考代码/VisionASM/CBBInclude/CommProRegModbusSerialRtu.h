#pragma once
#include "CommProRegBase.h"

// Modbus的寄存器串口RTU协议


// 支持的寄存器数据(或地址)类型	
// RegDataType regDataType;
// {
// 		// D
// 		regDataType.m_nDataTypeIndex = 0;
// 		regDataType.m_strDataTypeName = _T("D");
// 		regDataType.m_strDataTypeVal = _T("");
// 		m_vRegDataTypes.push_back(regDataType);
// }


#pragma pack(push,8)
class VS_COMM_API CCommProRegModbusSerialRtu :
	public CCommProRegBase
{
public:
	CCommProRegModbusSerialRtu(void);
	~CCommProRegModbusSerialRtu(void);

public:
	// 打包写寄存器为通信字符串
	virtual BOOL PackWriteRegInfo( CString& strInfo, long nRegAddr, int nWriteNum, std::vector<double> vWriteData );
	// 打包写寄存器为通信字符串
	virtual BOOL PackWriteRegInfo( CString& strInfo, long nRegAddr, int nWriteNum, std::vector<int> vWriteData );
	// 打包读寄存器为通信字符串
	virtual BOOL PackReadRegInfo( CString& strInfo, long nRegAddr, int nReadNum );

	// 解析写寄存器的响应字符串
	virtual BOOL AnalyzeAswWriteReg( CString strAsw );
	// 解析读寄存器的响应字符串
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<double> &vReceiveData );
	// 解析读寄存器的响应字符串
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<int> &vReceiveData );

	//////////////////////////////////////////////////////////////////////////
	virtual BOOL PackWriteRegInfo( CString& strInfo, long nRegAddr, int nWriteNum, std::vector<int> vWriteData, BOOL bDWORD);
	// 打包读寄存器为通信字符串
	virtual BOOL PackReadRegInfo( CString& strInfo, long nRegAddr, int nReadNum, BOOL bDWORD);

	// 解析写寄存器的响应字符串
	virtual BOOL AnalyzeAswWriteReg( CString strAsw, BOOL bDWORD);
	// 解析读寄存器的响应字符串
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<int> &vReceiveData, BOOL bDWORD );
	//////////////////////////////////////////////////////////////////////////

public:
	// 打包写IO寄存器为通信字符串
	virtual BOOL PackWriteIORegInfo( CString& strInfo, long nRegAddr, BOOL bOpen );
	// 解析写寄存器的响应字符串
	virtual BOOL AnalyzeAswWriteIOReg( CString strAsw );	

	// 打包读IO寄存器为通信字符串
	virtual BOOL PackReadIORegInfo( CString& strInfo, long nRegAddr );
	// 解析读IO寄存器的响应字符串
	virtual BOOL AnalyzeAswReadIOReg( CString strAsw,  int& nReceiveData );

	// 打包读IO寄存器为通信字符串
	virtual BOOL PackReadIORegInfo( CString& strInfo, long nRegAddr,int nReadNum);
	// 解析读IO寄存器的响应字符串
	virtual BOOL AnalyzeAswReadIOReg(CString strAsw,int nReadNum, std::vector<int> &vReceiveData);
};

#pragma pack(pop)