#pragma once
#include "CommProRegBase.h"

// 三菱FX系列PLC的寄存器网口MC（A兼容1E帧）二进制协议(FX-3U-ENET-ADP网口模块使用)


// 支持的寄存器数据(或地址)类型	
// RegDataType regDataType;
// {
// 		// D
// 		regDataType.m_nDataTypeIndex = 0;
// 		regDataType.m_strDataTypeName = _T("D");
// 		regDataType.m_strDataTypeVal = _T("A8");
// 		m_vRegDataTypes.push_back(regDataType);
// }


#pragma pack(push,8)
class VS_COMM_API CCommProRegMitsFXSocketBinaryMC1E :
	public CCommProRegBase
{
public:
	CCommProRegMitsFXSocketBinaryMC1E(void);
	~CCommProRegMitsFXSocketBinaryMC1E(void);

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