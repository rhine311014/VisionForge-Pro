// CommProtocolProcess.cpp: implementation of the CCommProtocolProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisionASM.h"
#include "CommProtocolProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommProtocol::CCommProtocol()
{
	m_pMidPlatformInfo = NULL;
	m_eMidPlatformType = ePlatformUnknown;
	m_eCmdParamOrderType = eCPOrderXYD;
	m_eProtocolType = eProCmdFast;
	m_eExProtocolType = eExProDefault;
	m_nPlatformIndex = 0;
	m_nPLCBaseRegAddr = 0;
	m_nPLCIDRegAddr   = 70;
	m_nPLCTempBaseRegAddr = 0;
	m_pCommProRegBase = NULL;
	m_bRegType = TRUE;
	m_nRegDataTypeIndex = 0;
	m_bEnableCommMultiCalibExtension = FALSE;
	m_bEnableCommMutiTarget=FALSE;

	m_nIDLength = 17;
	m_bEnableIDRaed = FALSE;	
	m_bIDInvert = FALSE; 
}

CCommProtocol::~CCommProtocol()
{
	if (m_pMidPlatformInfo != NULL)
	{
		delete m_pMidPlatformInfo;
		m_pMidPlatformInfo = NULL;
	}

	m_eMidPlatformType = ePlatformUnknown;

	if (m_pCommProRegBase != NULL)
	{
		delete m_pCommProRegBase;
		m_pCommProRegBase = NULL;
	}
}

// 设置、获取通信协议参数
void CCommProtocol::SetCommProtocolParam(CommProtocolParam param)
{
	m_CommProtocolParam = param;
}

CommProtocolParam  CCommProtocol::GetCommProtocolParam()
{
	return m_CommProtocolParam;
}


// 设置、获取系统选项配置
void CCommProtocol::SetSystemOptionConfig(SystemOptionConfig systemOptionConfig)
{
	m_SystemOptionCofing = systemOptionConfig;
}

SystemOptionConfig CCommProtocol::GetSystemOptionConfig()
{
	return m_SystemOptionCofing;
}


// 设置、获取中间平台
void CCommProtocol::SetMidPlatformInfo(PlatformType ePlatformType, CPlatformInfo* pPlatformInfo)
{
	if (pPlatformInfo == NULL)
	{
		return;
	}

	if (m_pMidPlatformInfo != NULL)
	{
		delete m_pMidPlatformInfo;
		m_pMidPlatformInfo = NULL;
	}
	
	m_eMidPlatformType = ePlatformType;

	switch(m_eMidPlatformType)
	{
	case ePlatformXYD:
		{
			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
		}
		break;
// 	case ePlatformXYPD:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
// 	case ePlatformXD:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
// 	case ePlatformXY:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
// 	case ePlatformX:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
// 	case ePlatformY:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
// 	case ePlatformXDPY:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
// 	case ePlatformXDPY1:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
// 	case ePlatformYD:
// 		{
// 			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
// 		}
// 		break;
	case ePlatformXY1Y2:
		{
			m_pMidPlatformInfo = new CPlatformXY1Y2Info(*(CPlatformXY1Y2Info*)pPlatformInfo);
		}
		break;
	case ePlatformX1X2Y:
		{
			m_pMidPlatformInfo = new CPlatformX1X2YInfo(*(CPlatformX1X2YInfo*)pPlatformInfo);
		}
		break;
	default:
		{
			m_pMidPlatformInfo = new CPlatformXYDInfo(*(CPlatformXYDInfo*)pPlatformInfo);
		}
	}

}
CPlatformInfo* CCommProtocol::GetMidPlatformInfo()
{
	return m_pMidPlatformInfo;
}

// 设置、获取平台序号
void CCommProtocol::SetPlatformIndex(int nPlatformIndex)
{
	m_nPlatformIndex = nPlatformIndex;
}

int CCommProtocol::GetPlatformIndex()
{
	return m_nPlatformIndex;
}

// 设置、获取PLC寄存器基地址
void CCommProtocol::SetPLCBaseRegAddr(long nBaseRegAddr)
{
	m_nPLCBaseRegAddr = nBaseRegAddr;
}

void CCommProtocol::SetPLCWZRegAddr(long nWZRegAddr)
{
	m_nPLCWZRegAddr = nWZRegAddr;
}
void CCommProtocol::SetPLCIDRegAddr(long nBaseRegAddr3)
{
	m_nPLCIDRegAddr = nBaseRegAddr3;
}
long CCommProtocol::GetPLCBaseRegAddr()
{
	return m_nPLCBaseRegAddr;
}

long CCommProtocol::GetPLCWZRegAddr()
{
	return m_nPLCWZRegAddr;
}
long CCommProtocol::GetPLCIDRegAddr()
{
	return m_nPLCIDRegAddr;
}

void CCommProtocol::SetPLCIDLength(int nLength)
{
	m_nIDLength = nLength;
}
int CCommProtocol::GetPLCIDLength()
{
	return m_nIDLength;
}
void CCommProtocol::SetEnableIDRead(BOOL bEnable)
{
	m_bEnableIDRaed = bEnable;
}
BOOL CCommProtocol::GetIsEnableIDRead()
{
	return m_bEnableIDRaed;
}

void CCommProtocol::SetEnableIDInvert(BOOL bEnable)
{
	m_bIDInvert = bEnable;
}
BOOL CCommProtocol::GetIsEnableIDInvert()
{
	return m_bIDInvert;
}


// 设置、获取PLC临时寄存器基地址
void CCommProtocol::SetPLCTempBaseRegAddr(long nTempBaseRegAddr)
{
	m_nPLCTempBaseRegAddr = nTempBaseRegAddr;
}

long CCommProtocol::GetPLCTempBaseRegAddr()
{
	return m_nPLCTempBaseRegAddr;
}

void CCommProtocol::SetPLCRegType(bool bRegType)
{
	m_bRegType = bRegType;
}

bool CCommProtocol::GetPLCRegType()
{
	return m_bRegType;
}

// 设置、获取协议类型
void CCommProtocol::SetPLCRegDataTypeIndex(int nRegDataTypeIndex)
{
	m_nRegDataTypeIndex = nRegDataTypeIndex;
	if (IsPlcProtocolType())
	{
		if (m_pCommProRegBase != NULL)
		{
			m_pCommProRegBase->SetCurRegDataTypeIndex(nRegDataTypeIndex);
		}
	}
}

int CCommProtocol::GetPLCRegDataTypeIndex()
{
	if (IsPlcProtocolType())
	{
		if (m_pCommProRegBase != NULL)
		{
			return m_pCommProRegBase->GetCurRegDataTypeIndex();
		}
	}

	return m_nRegDataTypeIndex;
}

// 设置、获取协议类型
void CCommProtocol::SetProtocolType(ProtocolType ptProtocolType)
{
	m_eProtocolType = ptProtocolType;
}
ProtocolType CCommProtocol::GetProtocolType()
{
	return m_eProtocolType;
}

// 设置、获取扩展协议类型
void CCommProtocol::SetExProtocolType(ExProtocolType ptExProtocolType)
{
	m_eExProtocolType = ptExProtocolType;
}

ExProtocolType CCommProtocol::GetExProtocolType()
{
	return m_eExProtocolType;
}

BOOL CCommProtocol::IsPlcProtocolType()
{
	return FALSE;
}

BOOL CCommProtocol::SetServerCommInfo(vcCommInfo *commInfo)
{
	if (!IsPlcProtocolType())
	{
		return FALSE;
	}

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}

	return m_pCommProRegBase->SetServerCommInfo(commInfo);

}

BOOL CCommProtocol::SetLocalCommInfo(vcCommInfo *commInfo)
{
	if (!IsPlcProtocolType())
	{
		return FALSE;
	}

	if (m_pCommProRegBase == NULL)
	{
		return FALSE;
	}
	
	return m_pCommProRegBase->SetLocalCommInfo(commInfo);
}

void CCommProtocol::SetCmdParamOrderType(CmdParamOrderType cpOrderType)
{
	m_eCmdParamOrderType = cpOrderType;
}

CmdParamOrderType CCommProtocol::GetCmdParamOrderType()
{
	return m_eCmdParamOrderType;
}

void CCommProtocol::SetEnableCommMultiCalibExtension(BOOL bEnable)
{
	m_bEnableCommMultiCalibExtension = bEnable;
}

void CCommProtocol::SetEnableCommMutiTarget(BOOL bEnable)
{
	m_bEnableCommMutiTarget = bEnable;
}

BOOL CCommProtocol::GetEnableCommMultiCalibExtension()
{
	return m_bEnableCommMultiCalibExtension;
}

BOOL CCommProtocol::GetEnableCommMutiTarget()
{
	return m_bEnableCommMutiTarget;
}

/////////////////////////////// 解析接收到的数据////////////////////////////////
// 解析收到的系统工作指令类型
SysWorkCmdType CCommProtocol::AnalyzeSysWorkCmdReceived( CString strCmd )
{
	if (IsSysCalibrateCmd(strCmd))
	{
		return eCmdCalibrate;
	}
	if (IsSysResetCmd(strCmd))
	{
		return eCmdResetFRST;
	}
// 
// 	if (IsOnLineAlignCmd(strCmd))
// 	{
// 		return eCmdOnlineAlign;
// 	}

	if (IsOnLineAlignCmdFACO(strCmd))
	{
		return eCmdOnlineAlignFACO;
	}

	if (IsOnLineAlignCmdFAAL(strCmd))
	{
		return eCmdAutoAlignFAAL;
	}

	if (IsOnLineAlignCmdFALG(strCmd))
	{
		return eCmdOnceAlignMoveFALG;
	}

    if(IsCalibMarkSearchCmd(strCmd))
	{
		return eCmdCalibSearch;
	}

	if (IsTargetMarkSearchCmd(strCmd))
	{
		return eCmdTargetSearch;
	}

	if (IsTargetMarkSearchCmdFTGT(strCmd))
	{
		return eCmdTargetSearchFTGT;
	}

	if (IsTargetMarkSearchCmdFTGC(strCmd))
	{
		return eCmdTargetSearchFTGC;
	}

	if (IsObjectMarkSearchCmd(strCmd))
	{
		return eCmdObjectSearch;
	}

	if (IsVirtualMarkSearchCmd(strCmd))
	{
		return eCmdVirtualSearch;
	}

	if (IsGetVirtualAndObjectMarkRelationShipCmd(strCmd))
	{
		return eCmdCntObAndVb;
	}

	if (IsGetAlignResult(strCmd))
	{
		return eCmdGetAlignResult;
	}

	if (IsSetTargetBench(strCmd))
	{
		return eCmdSetTargetBench;
	}

	return eCmdUnknow;

}

SysWorkCmdType CCommProtocol::AnalyzeSysWorkCmdReceived(std::vector<int> vnReceivedData)
{
	return eCmdUnknow;
}

// 解析收到的“指定平台各轴移动到的绝对位置”响应（&TAR00\）
CmdAnsweredStatus CCommProtocol::AnalyzeAswDrivePlatformMoveToAbsPos( CString strAsw )
{
	int nLength;
	TCHAR char1, char2;
	CString strInfo1, strInfo2;
	
	nLength = strlen(strAsw);

	if (nLength<0 || nLength!=7) return eAswCmdErr;
    
    char1 = strAsw.GetAt(0);
	char2 = strAsw.GetAt(nLength-1);
	strInfo1 = strAsw.Mid(1,3);
    strInfo2 = strAsw.Mid(4,2);

	if(char1!='&') return eAswCmdErr;
    if(char2!='\\') return eAswCmdErr;
    if(strInfo1!=_T("TAR")) return eAswCmdErr;

	CmdAnsweredStatus cmdAnsweredStatus = (CmdAnsweredStatus)(atoi(strInfo2));
   
 	return cmdAnsweredStatus;
}

// 解析收到的“获取相机平台各轴绝对位置”响应（&TCGX1Y1X2Y2...XnYn00\）
CmdAnsweredStatus CCommProtocol::AnalyzeAswGetCamPlatformAbsPos( CString strAsw, std::vector<CPlatformXYAxisPos*>& vpCamPltAxisPos )
{	
	int nLength;
	TCHAR char1, char2;
	CString strInfo1, strInfo2;
	double dValue1, dValue2;

	nLength = strlen(strAsw);
	if (nLength<0) return eAswCmdErr;

	char1 = strAsw.GetAt(0);
	char2 = strAsw.GetAt(nLength-1);
	strInfo1 = strAsw.Mid(1,3);
	strInfo2 = strAsw.Mid(nLength-3, 2);
    
	if(char1!='&') return eAswCmdErr;
    if(char2!='\\') return eAswCmdErr;
    if(strInfo1!=_T("TCG")) return eAswCmdErr;
    if(strInfo2!=_T("00")) return (CmdAnsweredStatus)(atoi(strInfo2));

    int k=0;
	for (int i=0; i<vpCamPltAxisPos.size(); i++, k=k+2)
	{   
		if (NULL == vpCamPltAxisPos[i])
		{
			return eAswUnkown;
		}

        strInfo1 = strAsw.Mid(k*12+4, 12); 
        strInfo2 = strAsw.Mid((k+1)*12+4, 12); 
		
		dValue1 = atof(strInfo1);
		dValue2 = atof(strInfo2);

		vpCamPltAxisPos[i]->m_dPosX = dValue1/10000;
		vpCamPltAxisPos[i]->m_dPosY = dValue2/10000;
	}

	return eAswSucceed;
}

// 解析收到的“指定相机平台各轴移动到的绝对位置”响应（&TCR00\）
CmdAnsweredStatus CCommProtocol::AnalyzeAswDriveCamPlatformMoveToAbsPos( CString strAsw )
{
	int nLength;
	TCHAR char1, char2;
	CString strInfo1, strInfo2;
	
	nLength = strlen(strAsw);
	
	if (nLength<0 || nLength!=7) return eAswCmdErr;
    
    char1 = strAsw.GetAt(0);
	char2 = strAsw.GetAt(nLength-1);
	strInfo1 = strAsw.Mid(1,3);
    strInfo2 = strAsw.Mid(4,2);
	
	if(char1!='&') return eAswCmdErr;
    if(char2!='\\') return eAswCmdErr;
    if(strInfo1!=_T("TCR")) return eAswCmdErr;
	
	CmdAnsweredStatus cmdAnsweredStatus = (CmdAnsweredStatus)(atoi(strInfo2));
	
 	return cmdAnsweredStatus;
}

// 解析收到的“等待相机平台将相机由A位置移动至B位置”响应（&WAT00\）
CmdAnsweredStatus CCommProtocol::AnalyzeAswWaitCamPltMove( CString strAsw )
{
   	int nLength;
	TCHAR char1, char2;
	CString strInfo1, strInfo2;
	
	nLength = strlen(strAsw);
	
	if (nLength<0 || nLength!=7) return eAswCmdErr;
    
    char1 = strAsw.GetAt(0);
	char2 = strAsw.GetAt(nLength-1);
	strInfo1 = strAsw.Mid(1,3);
    strInfo2 = strAsw.Mid(4,2);
	
	if(char1!='&') return eAswCmdErr;
    if(char2!='\\') return eAswCmdErr;
    if(strInfo1!=_T("WAT")) return eAswCmdErr;
	
	CmdAnsweredStatus cmdAnsweredStatus = (CmdAnsweredStatus)(atoi(strInfo2));
	
 	return cmdAnsweredStatus;

}

// 解析是否系统标定指令（&CAE\）
BOOL CCommProtocol::IsSysCalibrateCmd( CString strCmd )
{
	int nLength;
    TCHAR char1, char2;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength!=5) return FALSE;
    
    char1 = strCmd.GetAt(0);
	char2 = strCmd.GetAt(nLength-1);
	strInfo = strCmd.Mid(1, 3);

	if(char1!='&') return FALSE;
    if(char2!='\\') return FALSE;
    if(strInfo!=_T("CAE")) return FALSE;
	
	return TRUE;
}

BOOL CCommProtocol::IsSysResetCmd(CString strCmd)
{
	return FALSE;
}
// 解析是否在线对位指令（&AAE\）
BOOL CCommProtocol::IsOnLineAlignCmd( CString strCmd )
{
	int nLength;
    TCHAR char1, char2;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength!=5) return FALSE;
    
    char1 = strCmd.GetAt(0);
	char2 = strCmd.GetAt(nLength-1);
	strInfo = strCmd.Mid(1, 3);
	
	if(char1!='&') return FALSE;
    if(char2!='\\') return FALSE;
    if(strInfo!=_T("AAE")) return FALSE;

	return TRUE;	
}
// 解析是否在线对位指令（&AAE\）
BOOL CCommProtocol::IsOnLineAlignCmdFACO( CString strCmd )
{
	return FALSE;	
}
// 解析是否在线对位指令（&AAE\）
BOOL CCommProtocol::IsOnLineAlignCmdFAAL( CString strCmd )
{
	return FALSE;	
}
// 解析是否在线对位指令（&AAE\）
BOOL CCommProtocol::IsOnLineAlignCmdFALG( CString strCmd )
{
	return FALSE;	
}
BOOL CCommProtocol::IsGetAlignResult(CString strCmd)
{
	return FALSE;
}

BOOL CCommProtocol::IsSetTargetBench(CString strCmd)
{
	return FALSE;
}
// 解析是否标定搜索指令（&CAG\）
BOOL CCommProtocol::IsCalibMarkSearchCmd(CString strCmd)
{
	int nLength;
    TCHAR char1, char2;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength!=5) return FALSE;
    
    char1 = strCmd.GetAt(0);
	char2 = strCmd.GetAt(nLength-1);
	strInfo = strCmd.Mid(1, 3);
	
	if(char1!='&') return FALSE;
    if(char2!='\\') return FALSE;
    if(strInfo!=_T("CAG")) return FALSE;
	
	return TRUE;
}

// 解析是否实时目标搜索指令（&TGG\）
BOOL CCommProtocol::IsTargetMarkSearchCmd( CString strCmd )
{
	int nLength;
    TCHAR char1, char2;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength!=5) return FALSE;
    
    char1 = strCmd.GetAt(0);
	char2 = strCmd.GetAt(nLength-1);
	strInfo = strCmd.Mid(1, 3);
	
	if(char1!='&') return FALSE;
    if(char2!='\\') return FALSE;
    if(strInfo!=_T("TGG")) return FALSE;

	return TRUE;
}
// 解析是否实时目标搜索指令（&TGG\）
BOOL CCommProtocol::IsTargetMarkSearchCmdFTGT( CString strCmd )
{
	return FALSE;
}

// 解析是否实时目标搜索指令（&TGG\），目标候补搜索
BOOL CCommProtocol::IsTargetMarkSearchCmdFTGC( CString strCmd )
{
	return FALSE;
}
// 解析是否实时对象搜索指令（&OBG\）
BOOL CCommProtocol::IsObjectMarkSearchCmd( CString strCmd )
{
	int nLength;
    TCHAR char1, char2;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength!=5) return FALSE;
    
    char1 = strCmd.GetAt(0);
	char2 = strCmd.GetAt(nLength-1);
	strInfo = strCmd.Mid(1, 3);
	
	if(char1!='&') return FALSE;
    if(char2!='\\') return FALSE;
    if(strInfo!=_T("OBG")) return FALSE;

	return TRUE;
}

// 解析是否虚拟对象搜索指令（&VBG\）
BOOL CCommProtocol::IsVirtualMarkSearchCmd( CString strCmd )
{
	int nLength;
    TCHAR char1, char2;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength!=5) return FALSE;
    
    char1 = strCmd.GetAt(0);
	char2 = strCmd.GetAt(nLength-1);
	strInfo = strCmd.Mid(1, 3);
	
	if(char1!='&') return FALSE;
    if(char2!='\\') return FALSE;
    if(strInfo!=_T("VBG")) return FALSE;

	return TRUE;
}

// 解析是否确定虚拟对象与实时对象关系指令（&COV\）
BOOL CCommProtocol::IsGetVirtualAndObjectMarkRelationShipCmd( CString strCmd )
{
	int nLength;
    TCHAR char1, char2;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength!=5) return FALSE;
    
    char1 = strCmd.GetAt(0);
	char2 = strCmd.GetAt(nLength-1);
	strInfo = strCmd.Mid(1, 3);
	
	if(char1!='&') return FALSE;
    if(char2!='\\') return FALSE;
    if(strInfo!=_T("COV")) return FALSE;
	
	return TRUE;

}

/////////////////////////////// 打包需要发送的数据//////////////////////////////

// <平台移动>

// 打包“获取平台各轴绝对位置”指令： TAG
BOOL CCommProtocol::PackGetPlatformAbsPos( CString& strData )
{    
    strData = _T("&TAG");	
   	return TRUE;
}

// <相机移动>

// 打包“获取相机平台各轴绝对位置”指令： TCG
BOOL CCommProtocol::PackGetCamPlatformAbsPos( CString& strData )
{
    strData = _T("&TCG");
	return TRUE;
}

// 打包“指定相机平台各轴移动到的绝对位置”指令: TCR
BOOL CCommProtocol::PackDriveCamPlatformMoveToAbsPos( CString& strData, const std::vector<CPlatformXYAxisPos*> vpCamPltAxisPos )
{
	strData = _T("&TCR");

	CString strTemp1, strTemp2;
	CString strXPos, strYPos;
	double  xPos,   yPos;
	
	for (int i=0; i<vpCamPltAxisPos.size(); i++)
	{
		strXPos = _T("");
		strYPos = _T("");

		if (NULL == vpCamPltAxisPos[i])
		{
			return FALSE;
		}

		// X轴
        xPos = vpCamPltAxisPos[i]->m_dPosX;

		if (xPos>=0)
		{
			strTemp1 = _T("+");
		}
		else
		{
			strTemp1 = _T("-");
			xPos = -1*xPos;
		} 			
		
		xPos = xPos * 10000;		
		
		strTemp2.Format(_T("%011.0lf"), xPos);
		
		strXPos  = strTemp1 + strTemp2;

		// Y轴
        yPos = vpCamPltAxisPos[i]->m_dPosY;
		
		if (yPos>=0)
		{
			strTemp1 = _T("+");
		}
		else
		{
			strTemp1 = _T("-");
			yPos = -1*yPos;
		} 			
		
		yPos = yPos * 10000;		
		
		strTemp2.Format(_T("%011.0lf"), yPos);
		
		strYPos  = strTemp1 + strTemp2;

        strData = strData + strXPos + strYPos;
	}

	return TRUE;
}

//<其他>
// 标定/对位中，“等待相机平台将相机由A位置移动至B位置”指令: WAT（仅在单相机对位中用）
BOOL CCommProtocol::PackWaitCamPltMove( CString& strData )
{
    strData = _T("&WAT");
	return TRUE;
}


// 指令
// _T("TP,1234567890")		// 请求切换产品
BOOL CCommProtocol::IsSetProductNameCmd( CString strCmd )
{
	int nLength;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength<=3) return FALSE;
    
	strInfo = strCmd.Mid(0, 2);
    if(strInfo!=_T("TP")) return FALSE;
	
	return TRUE;
	
}

// 指令
// _T("TN,000")				// 请求切换产品
BOOL CCommProtocol::IsSetProductIndexCmd( CString strCmd )
{
	int nLength;
	CString strInfo;
	
    nLength = strCmd.GetLength();
    
	if (nLength<=0 || nLength<=3) return FALSE;
    
	strInfo = strCmd.Mid(0, 2);
    if(strInfo!=_T("TN")) return FALSE;
	
	return TRUE;
}

CmdAnsweredStatus CCommProtocol::AnalyzeAswSetProductName( CString strAsw, CString& strProductName )
{
	if (!IsSetProductNameCmd(strAsw))
	{
		return eAswUnkown;
	}
	
	strProductName = strAsw.Mid(3);
	if (strProductName.IsEmpty())
	{
		return eAswUnkown;
	}
	
	return eAswSucceed;
}

CmdAnsweredStatus CCommProtocol::AnalyzeAswSetProductIndex( CString strAsw, int& nProductIndex )
{
	if (!IsSetProductIndexCmd(strAsw))
	{
		return eAswUnkown;
	}
	
	CString strProductName;
	strProductName = strAsw.Mid(3);
	if (strProductName.IsEmpty())
	{
		return eAswUnkown;
	}
	
	nProductIndex = atoi(strProductName);
	
	return eAswSucceed;
}

BOOL CCommProtocol::IsStringNumerical( LPCTSTR lpszVal )
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i=0; i<nLen; i++)
	{
		ch = strVal.GetAt(i);
		if ((ch > '9') || ((ch < '0') && (ch != '.')))
		{
			if ((i != 0) && (i != nLen-1) && (ch=='E'||ch=='e'))
			{
				continue;
			}

			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}

			if((i > 0) && (strVal.GetAt(i-1) == 'E') && ((ch == '-') || (ch == '+')))
			{
				continue;
			}

			return FALSE;
		}
	}
	return TRUE;
}