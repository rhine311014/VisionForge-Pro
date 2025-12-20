#pragma once
#include "CommProRegBase.h"

// 基恩士KV（KV-700/1000）系列PLC的寄存器网口PLCLink方式


// 支持的寄存器数据(或地址)类型	
// RegDataType regDataType;
// {
// 		// DM
// 		regDataType.m_nDataTypeIndex = 0;
// 		regDataType.m_strDataTypeName = _T("DM");
// 		regDataType.m_strDataTypeVal = _T("DM");
// 		m_vRegDataTypes.push_back(regDataType);
// 
// 		// EM
// 		regDataType.m_nDataTypeIndex = 1;
// 		regDataType.m_strDataTypeName = _T("EM");
// 		regDataType.m_strDataTypeVal = _T("EM");
// 		m_vRegDataTypes.push_back(regDataType);
//
// 		// ZF
// 		regDataType.m_nDataTypeIndex = 2;
// 		regDataType.m_strDataTypeName = _T("ZF");
// 		regDataType.m_strDataTypeVal = _T("ZF");
// 		m_vRegDataTypes.push_back(regDataType);
// }


#pragma pack(push,8)
class VS_COMM_API CCommProRegKeyenceSocketPCLink : public CCommProRegBase
{
public:
	CCommProRegKeyenceSocketPCLink(void);
	~CCommProRegKeyenceSocketPCLink(void);

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
};

#pragma pack(pop)