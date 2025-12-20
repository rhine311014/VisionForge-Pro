#pragma once
#include "CommProRegBase.h"

// 欧姆龙PLC的寄存器网口Fins协议


// 支持的寄存器数据(或地址)类型	
//RegDataType regDataType;
//{
//	// D
//	regDataType.m_nDataTypeIndex = 0;
//	regDataType.m_strDataTypeName = _T("DM");
//	regDataType.m_strDataTypeVal = _T("82");// char val = 82;
//	m_vRegDataTypes.push_back(regDataType);
//
//	regDataType.m_nDataTypeIndex = 1;
//	regDataType.m_strDataTypeName = _T("EM");
//	regDataType.m_strDataTypeVal = _T("98");// char val = 98;
//	m_vRegDataTypes.push_back(regDataType);
//}


#pragma pack(push,8)
class VS_COMM_API CCommProRegOmronSocketFins :
	public CCommProRegBase
{
public:
	CCommProRegOmronSocketFins(void);
	~CCommProRegOmronSocketFins(void);

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

	// 打包寄存器握手为通信字符串
	virtual BOOL PackShakeHandsRegInfo( CString& strInfo);
	// 解析寄存器握手的响应字符串
	virtual BOOL AnalyzeShakeHandsReg(CString strAsw);
};

#pragma pack(pop)