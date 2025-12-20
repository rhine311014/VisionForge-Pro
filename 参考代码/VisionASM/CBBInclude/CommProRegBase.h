#pragma once
// PLC寄存器操作基类
#include <vector>
#include "limits.h"
#include "vsComm.h"
#include "CommProRegDef.h"

#pragma pack(push,8)

// 寄存器数据(或地址)类型
typedef struct tagRegDataType
{
	int		m_nDataTypeIndex;		// 寄存器数据(或地址)类型序号
	CString m_strDataTypeName;		// 寄存器数据(或地址)类型名称，用于显示
	CString m_strDataTypeVal;		// 寄存器数据(或地址)类型值，用于实际协议通信
	
	tagRegDataType()
	{
		m_nDataTypeIndex = 0;
		m_strDataTypeName = _T("");
		m_strDataTypeVal = _T("");
	}
}RegDataType;

//class vcSocketCommInfo;
class VS_COMM_API CCommProRegBase
{
public:
	CCommProRegBase(void);
	virtual ~CCommProRegBase(void);

public:
	// 获取支持的寄存器数据(或地址)类型
	virtual std::vector<RegDataType> GetAvailableRegDataTypes();
	// 获取当前寄存器数据(或地址)类型
	virtual BOOL GetCurRegDataType(RegDataType& regDataType);

	// 设置当前寄存器数据(或地址)类型的序号
	virtual BOOL SetCurRegDataTypeIndex(int nCurRegDataTypeIndex);
	// 获取当前寄存器数据(或地址)类型的序号
	virtual int GetCurRegDataTypeIndex();
	
	// 获取是否支持设置PLC站号
	virtual BOOL GetPlcDeviceAddrEnable();

	// 设置PLC站号
	virtual BOOL SetPlcDeviceAddr(int nAddr);
	// 获取PLC站号
	virtual int GetPlcDeviceAddr();

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
	virtual BOOL PackWriteIORegInfo( CString& strInfo, long nRegAddr, BOOL bOpen);
	// 解析写寄存器的响应字符串
	virtual BOOL AnalyzeAswWriteIOReg( CString strAsw );	
	
	// 打包读IO寄存器为通信字符串
	virtual BOOL PackReadIORegInfo( CString& strInfo, long nRegAddr);
	// 解析读IO寄存器的响应字符串
	virtual BOOL AnalyzeAswReadIOReg( CString strAsw, int& nReceiveData );

	// 打包读IO寄存器为通信字符串
	virtual BOOL PackReadIORegInfo( CString& strInfo, long nRegAddr,int nReadNum);
	// 解析读IO寄存器的响应字符串
	virtual BOOL AnalyzeAswReadIOReg(CString strAsw,int nReadNum, std::vector<int> &vReceiveData);

	// 打包寄存器握手为通信字符串
	virtual BOOL PackShakeHandsRegInfo( CString& strInfo);
	// 解析寄存器握手的响应字符串
	virtual BOOL AnalyzeShakeHandsReg(CString strAsw);

	// 设置与PLC通信的会话句柄（Omron CIP协议专用）
	virtual BOOL SetSessionHandle(CString strSessionHandle);
	// 获取与PLC通信的会话句柄（Omron CIP协议专用）
	virtual BOOL GetSessionHandle(CString& strSessionHandle);

	//////////////////////////////////////////////////////////////////////////
	virtual BOOL PackWriteRegInfo( CString& strInfo, CString strRegTag, int nWriteNum, std::vector<int> vWriteData, BOOL bDWORD);
	// 打包读寄存器为通信字符串
	virtual BOOL PackReadRegInfo( CString& strInfo, CString strRegTag, int nReadNum, BOOL bDWORD);

	//打包写寄存器为通信字符串（BOOL类型，PLC2字节）
	virtual BOOL PackWriteRegInfo( CString& strInfo, CString strRegTag, int nWriteNum, std::vector<bool> vWriteData);
	//打包写寄存器为通信字符串（REAL = TRUE（Real类型）、PLC4字节；REAL = FALSE（LReal类型）、PLC8字节）
	virtual BOOL PackWriteRegInfo( CString& strInfo, CString strRegTag, int nWriteNum, std::vector<double> vWriteData, BOOL bREAL);

	// 解析读寄存器的响应字符串（BOOL类型，PLC2字节）
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<bool> &vReceiveData);
	// 解析读寄存器的响应字符串（REAL = TRUE（Real类型）、PLC4字节；REAL = FALSE（LReal类型）、PLC8字节）
	virtual BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<double> &vReceiveData, BOOL bREAL );

	// 打包寄存器注销为通信字符串
	virtual BOOL PackUnShakeHandsRegInfo( CString& strInfo);

public:
	// 设置服务器（PLC）通信信息
	virtual BOOL SetServerCommInfo(vcCommInfo *commInfo);	
	// 设置本地（PC）通信信息
	virtual BOOL SetLocalCommInfo(vcCommInfo *commInfo);

public:
	 CString CalcCrcFast(CString puchMsg);
	 CString CalcFCS(CString strData); // FCS校验
	 CString CalcLRC(CString puchMsg); // LRC校验
protected:
	std::vector<RegDataType> m_vRegDataTypes;
	int						 m_nCurRegDataTypeIndex;
	vcCommInfo*				 m_pServerCommInfo;
	vcCommInfo*				 m_pLocalCommInfo;
	bool					 m_bPlcDeviceAddrEnable;//是否支持设置PLC站号
	int						 m_nPlcDeviceAddr;	//PLC站号
	CString					 m_strSessionHandle;
};

#pragma pack(pop)