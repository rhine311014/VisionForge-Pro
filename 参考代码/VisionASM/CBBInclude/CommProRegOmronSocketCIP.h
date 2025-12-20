#pragma once
#include "CommProRegBase.h"

// 欧姆龙PLC的寄存器网口CIP协议


// 支持的寄存器数据(或地址)类型	
//RegDataType regDataType;
//{
//	// D
//	regDataType.m_nDataTypeIndex = 0;
//	regDataType.m_strDataTypeName = _T("DM");
//	regDataType.m_strDataTypeVal = _T("82");// char val = 82;
//	m_vRegDataTypes.push_back(regDataType);
//}


#pragma pack(push,8)
class VS_COMM_API CCommProRegOmronSocketCIP:
	public CCommProRegBase
{
public:
	CCommProRegOmronSocketCIP(void);
	virtual ~CCommProRegOmronSocketCIP(void);

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

	//////////////////////////////////////////////////////////////////////////
	//打包写寄存器为通信字符串（WORD：PLC2字节；DWORD：PLC4字节）
	virtual BOOL PackWriteRegInfo( CString& strInfo, CString strRegTag, int nWriteNum, std::vector<int> vWriteData, BOOL bDWORD);
	//打包写寄存器为通信字符串（BOOL类型，PLC2字节）
	virtual BOOL PackWriteRegInfo( CString& strInfo, CString strRegTag, int nWriteNum, std::vector<bool> vWriteData);
	//打包写寄存器为通信字符串（REAL = TRUE（Real类型）、PLC4字节；REAL = FALSE（LReal类型）、PLC8字节）
	virtual BOOL PackWriteRegInfo( CString& strInfo, CString strRegTag, int nWriteNum, std::vector<double> vWriteData, BOOL bREAL);

	// 打包读寄存器为通信字符串
	virtual BOOL PackReadRegInfo( CString& strInfo, CString strRegTag, int nReadNum, BOOL bDWORD);


	// 解析写寄存器的响应字符串
	virtual BOOL AnalyzeAswWriteReg( CString strAsw, BOOL bDWORD);

	// 解析读寄存器的响应字符串（WORD：PLC2字节；DWORD：PLC4字节）
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<int> &vReceiveData, BOOL bDWORD );
	// 解析读寄存器的响应字符串（BOOL类型，PLC2字节）
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<bool> &vReceiveData);
	// 解析读寄存器的响应字符串（REAL = TRUE（Real类型）、PLC4字节；REAL = FALSE（LReal类型）、PLC8字节）
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<double> &vReceiveData, BOOL bREAL );

	//////////////////////////////////////////////////////////////////////////

	// 打包寄存器握手或注册为通信字符串
	virtual BOOL PackShakeHandsRegInfo( CString& strInfo);
	// 解析寄存器握手或注册的响应字符串
	virtual BOOL AnalyzeShakeHandsReg(CString strAsw);

	// 设置与PLC通信的会话句柄（Omron CIP协议专用）
	virtual BOOL SetSessionHandle(CString strSessionHandle);
	// 获取与PLC通信的会话句柄（Omron CIP协议专用）
	virtual BOOL GetSessionHandle(CString& strSessionHandle);

	// 打包寄存器注销为通信字符串
	virtual BOOL PackUnShakeHandsRegInfo( CString& strInfo);


};

#pragma pack(pop)