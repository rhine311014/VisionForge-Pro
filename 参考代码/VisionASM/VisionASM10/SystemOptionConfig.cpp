// SystemOptionConfig.cpp: implementation of the CSystemOptionConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemOptionConfig.h"
#include "svXMLConfigurator.h"
#include "base64.h"
#include "vsEn_Decrypt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// 获取CPU处理器信息
void GetCpuInfo(CString &chProcessorName,CString &chProcessorType,DWORD &dwNum,DWORD &dwMaxClockSpeed)  
{  
	CString strPath=_T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");//注册表子键路径  
	CRegKey regkey;//定义注册表类对象  
	LONG lResult;//LONG型变量－反应结果  
	lResult=regkey.Open(HKEY_LOCAL_MACHINE,LPCTSTR(strPath),KEY_ALL_ACCESS); //打开注册表键  
	if (lResult!=ERROR_SUCCESS)  
	{  
		return;  
	}  
	CHAR chCPUName[50] = {0};  
	DWORD dwSize=50;   

	//获取ProcessorNameString字段值  
	if (ERROR_SUCCESS == regkey.QueryStringValue(_T("ProcessorNameString"),chCPUName,&dwSize))  
	{  
		chProcessorName = chCPUName;  
	}  

	//查询CPU主频  
	DWORD dwValue;  
	if (ERROR_SUCCESS == regkey.QueryDWORDValue(_T("~MHz"),dwValue))  
	{  
		dwMaxClockSpeed = dwValue;  
	}  
	regkey.Close();//关闭注册表  

	//获取CPU核心数目  
	SYSTEM_INFO si;  
	memset(&si,0,sizeof(SYSTEM_INFO));  
	GetSystemInfo(&si);  
	dwNum = si.dwNumberOfProcessors;  

	switch (si.dwProcessorType)  
	{  
	case PROCESSOR_INTEL_386:  
		{  
			chProcessorType.Format(_T("Intel 386 processor"));  
		}  
		break;  
	case PROCESSOR_INTEL_486:  
		{  
			chProcessorType.Format(_T("Intel 486 Processor"));  
		}  
		break;  
	case PROCESSOR_INTEL_PENTIUM:  
		{  
			chProcessorType.Format(_T("Intel Pentium Processor"));  
		}  
		break;  
	case PROCESSOR_INTEL_IA64:  
		{  
			chProcessorType.Format(_T("Intel IA64 Processor"));  
		}  
		break;  
	case PROCESSOR_AMD_X8664:  
		{  
			chProcessorType.Format(_T("AMD X8664 Processor"));  
		}  
		break;  
	default:  
		chProcessorType.Format(_T("未知"));  
		break;  
	}  
}  

void Getcpuidex(INT32 CPUInfo[4], INT32 InfoType, INT32 ECXValue)
{
#ifdef _WIN64
	__cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
#else
	if (NULL==CPUInfo)    return;
	_asm{
		// load. 读取参数到寄存器
		mov edi, CPUInfo;    // 准备用edi寻址CPUInfo
		mov eax, InfoType;
		mov ecx, ECXValue;
		// CPUID
		cpuid;
		// save. 将寄存器保存到CPUInfo
		mov    [edi], eax;
		mov    [edi+4], ebx;
		mov    [edi+8], ecx;
		mov    [edi+12], edx;
	}
#endif
}

void Getcpuid(INT32 CPUInfo[4], INT32 InfoType)
{
	Getcpuidex(CPUInfo, InfoType, 0);
}

// 取得CPU商标（Brand）
//
// result: 成功时返回字符串的长度（一般为48）。失败时返回0。
// pbrand: 接收商标信息的字符串缓冲区。至少为49字节。
int cpu_getbrand(char* pbrand)
{
	INT32 dwBuf[4];
	if (NULL==pbrand)    return 0;
	// Function 0x80000000: Largest Extended Function Number
	Getcpuid(dwBuf, 0x80000000);
	if (dwBuf[0] < 0x80000004)    return 0;
	// Function 80000002h,80000003h,80000004h: Processor Brand String
	Getcpuid((INT32*)&pbrand[0], 0x80000002);    // 前16个字符
	Getcpuid((INT32*)&pbrand[16], 0x80000003);    // 中间16个字符
	Getcpuid((INT32*)&pbrand[32], 0x80000004);    // 最后16个字符
	pbrand[48] = '\0';
	return 48;
}

tagSystemOptionConfig::tagSystemOptionConfig()
{
	m_strSysConfigFilePath = _T("");
	m_strAdminPassword = _T("2437e696e6a3aeb6e965f78f86f5e2e966a78b870d075a21a3ea69afe218c973");//visionsmart
	m_strAdminPassword2 = _T("8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92");//123456 
	m_strRestDate = _T("NzMw");                //720 剩余两年时间
	m_strSysRegID = _T("16100");
	m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	m_bSysKeyboardEnabled = FALSE;
	m_bStartUpShowCamSNSetting = FALSE;
	m_nCurSystemIndex = 0;				// 当前系统的编号(0、1、2……)
	//m_nCameraMode = 0;					// 相机模式：0，4：3；1，3：4；2，6：5；3，5：6
	m_nDisplayMode = 0;					// 图像显示模式：0，实时显示；1，帧显示
	m_nDisplayIndex = 5;				// 实时显示时显示第n帧
	m_nCommXYUnitExp = 4;				// 串口XY单位幂4
	m_nCommDUnitExp = 5;				// 串口D单位幂5
	m_nCommDataLenType = 0;				// 串口数据长度：0：实际长度；1：固定长度；
	m_nCommDelay = 0;					// 发送延时
	m_nCommTimeOut = 30;				// 串口超时
	m_nCommReSendTimes = 3;				// 串口重复次数
	m_nShowRunCommandDetail = 0;		// 命令记录：0，不记录命令；
	m_bMainframeSetBendParam = FALSE;
	m_bMainframeSetAlignParam = TRUE;
	m_bShowSepTargetPos = FALSE;
	m_bCommRecDelayEnable = FALSE;		// 是否通信延时
	m_nCommRecDelayTime = 0;		// 通信延时时间毫秒
	//m_nShowMode	   = 0;					// // 主界面显示方式：2行3列，3行2列

	//m_bShowProductInfo = FALSE;
	//m_bShowOffsetDirection=FALSE;
	//m_bShowCircleTool = FALSE;
	//m_bShowCornerTool = FALSE;			// 显示找角工具的两条直线
    //m_bShowLineTool   = FALSE;
	//m_bShowSearchRect=FALSE;

	m_bChange2um = FALSE;
    m_bChangeProductSize2um = FALSE;
	m_bAllSteoage = FALSE;
	m_bShowSearchResult = TRUE;

	m_bShowGamma = FALSE;
	m_bCheckModeOnly = FALSE;
	m_bCheckFPMI = FALSE;
	CString chProcessorName = _T("");
	CString chProcessorType = _T("");
	DWORD dwNum = 0;
	DWORD dwMaxClockSpeed = 0;
//	GetCpuInfo(chProcessorName, chProcessorType, dwNum, dwMaxClockSpeed); // 获取CPU处理器信息

	char chCPU[256];
	cpu_getbrand(chCPU);// 获取CPU处理器信息
	chProcessorName = chCPU;
	if (chProcessorName.Find(_T("847E")) >= 0)
	{
		m_bCommRecDelayEnable = FALSE;		// 是否通信延时
		m_nCommRecDelayTime = 0;		// 通信延时时间毫秒
	}
	else
	{
		m_bCommRecDelayEnable = FALSE;		// 是否通信延时
		m_nCommRecDelayTime = 0;		// 通信延时时间毫秒
	}

	// 目标和对象相机分离时，关联标定目标位置所用的模板是来自目标模板还是对象模板。
	// 0：用目标模板关联标定目标位置，默认值
	// 1：用对象模板关联标定目标位置
	m_nSepCamCalibTargetModelType	= 0;  

	m_eTempPlatformType = ePlatformXY1Y2;
	m_pTempPlatformInfo = NULL;
	switch(m_eTempPlatformType)
	{
	case ePlatformXY1Y2:
		{
			m_pTempPlatformInfo = new CPlatformXY1Y2Info;
		}
		break;
	case ePlatformX1X2Y:
		{
			m_pTempPlatformInfo = new CPlatformX1X2YInfo;
		}
		break;
	default:
		{

		}
	}

	m_bPlatformCustomNameEnable = FALSE;
	m_bIsShowCalibrateEvaluate = FALSE;

	int i = 0;
	for (i = 0; i < MaxPlatformCustomNameNum; i++)
	{
		m_strPlatformCustomName[i].Format(_T("工位%d"), i+1);
	}


	m_TarSearchResultCrossSize.nLineColor = RGB(255,0,0);
	m_TarSearchResultCrossSize.nLineLength = 150;
	m_TarSearchResultCrossSize.nRotation = 0;
	m_TarSearchResultCrossSize.m_bShowDefortGui = TRUE;

	m_ObjSearchResultCrossSize.nLineColor = RGB(0,255,0);
	m_ObjSearchResultCrossSize.nLineLength = 100;
	m_ObjSearchResultCrossSize.nRotation = 45;
	m_ObjSearchResultCrossSize.m_bShowDefortGui = TRUE;

	m_bGetSysResourceUse = TRUE;
	m_bThreValid = TRUE;
	m_dMaxCpuUseThre = 0.5;
	m_dMaxDiskUseThre = 0.90;
	m_dMaxMemUseThre = 0.8;
	m_nMaxHandleUseThre = 1500;
	m_nMaxGDIUseThre = 1500;

	m_dCpuThreMaxValue=0.7;
	m_dDiskThreMaxValue=0.90;
	m_dMemThreMaxValue=0.8;
	m_nHandleThreMaxValue=5000;
	m_nGDIThreMaxValue=5000;

	m_nPlatformLayoutType = 0;
	m_bShowFullScreen = false;

	m_nLoginTimeSet = 120;

	m_bSaveVDBPostMessage = 1;

}
tagSystemOptionConfig::~tagSystemOptionConfig()
{
	if (m_pTempPlatformInfo != NULL)
	{
		delete m_pTempPlatformInfo;
		m_pTempPlatformInfo = NULL;
	}
}

tagSystemOptionConfig::tagSystemOptionConfig(const tagSystemOptionConfig& systemOptionConfig)
{
	m_strSysConfigFilePath = systemOptionConfig.m_strSysConfigFilePath;
	m_strAdminPassword = systemOptionConfig.m_strAdminPassword;
	m_lgidLanguage = systemOptionConfig.m_lgidLanguage;
	m_bSysKeyboardEnabled = systemOptionConfig.m_bSysKeyboardEnabled;
	m_bStartUpShowCamSNSetting = systemOptionConfig.m_bStartUpShowCamSNSetting;

	m_nCurSystemIndex  = systemOptionConfig.m_nCurSystemIndex;			// 当前系统的编号(0、1、2……)

	//m_nCameraMode = systemOptionConfig.m_nCameraMode;					// 相机模式：0，4：3；1，3：4；2，6：5；3，5：6
	m_nDisplayMode = systemOptionConfig.m_nDisplayMode;					// 图像显示模式：0，实时显示；1，帧显示
	m_nDisplayIndex = systemOptionConfig.m_nDisplayIndex;				// 实时显示时显示第n帧
	m_nCommXYUnitExp = systemOptionConfig.m_nCommXYUnitExp;				// 串口XY单位幂4
	m_nCommDUnitExp = systemOptionConfig.m_nCommDUnitExp;				// 串口D单位幂5
	m_nCommDataLenType = systemOptionConfig.m_nCommDataLenType;				// 串口数据长度：0：实际长度；1：固定长度；
	m_nCommDelay = systemOptionConfig.m_nCommDelay;
	m_nCommTimeOut = systemOptionConfig.m_nCommTimeOut;				// 串口超时
	m_nCommReSendTimes = systemOptionConfig.m_nCommReSendTimes;				// 串口重复次数
	m_nShowRunCommandDetail = systemOptionConfig.m_nShowRunCommandDetail;		// 命令记录：0，不记录命令；
	
	m_bMainframeSetAlignParam = systemOptionConfig.m_bMainframeSetAlignParam;
	m_bCommRecDelayEnable = systemOptionConfig.m_bCommRecDelayEnable;		// 是否通信延时
	m_nCommRecDelayTime = systemOptionConfig.m_nCommRecDelayTime;		// 通信延时时间毫秒
	m_bShowSepTargetPos = systemOptionConfig.m_bShowSepTargetPos;

	m_bMainframeSetBendParam = systemOptionConfig.m_bMainframeSetBendParam;

	//m_nShowMode	   = systemOptionConfig.m_nShowMode;					// 主界面显示方式：2行3列，3行2列

	//m_bShowProductInfo = systemOptionConfig.m_bShowProductInfo;
	//m_bShowOffsetDirection = systemOptionConfig.m_bShowOffsetDirection;
	//m_bShowCircleTool   = systemOptionConfig.m_bShowCircleTool;
	//m_bShowCornerTool	= systemOptionConfig.m_bShowCornerTool;
	// m_bShowLineTool   = systemOptionConfig.m_bShowLineTool;

	//m_bShowSearchRect=systemOptionConfig.m_bShowSearchRect;

	m_bChange2um			= systemOptionConfig.m_bChange2um;
    m_bChangeProductSize2um			= systemOptionConfig.m_bChangeProductSize2um;
	m_bAllSteoage			= systemOptionConfig.m_bAllSteoage;
	m_bShowSearchResult	= systemOptionConfig.m_bShowSearchResult;
	m_bShowGamma		= systemOptionConfig.m_bShowGamma;
	m_bCheckModeOnly = systemOptionConfig.m_bCheckModeOnly;
	
	// 目标和对象相机分离时，关联标定目标位置所用的模板是来自目标模板还是对象模板。
	// 0：用目标模板关联标定目标位置，默认值
	// 1：用对象模板关联标定目标位置
	m_nSepCamCalibTargetModelType = systemOptionConfig.m_nSepCamCalibTargetModelType;

	m_bGetSysResourceUse = systemOptionConfig.m_bGetSysResourceUse;
	m_bThreValid = systemOptionConfig.m_bThreValid;
	m_dMaxCpuUseThre = systemOptionConfig.m_dMaxCpuUseThre;
	m_dMaxDiskUseThre = systemOptionConfig.m_dMaxDiskUseThre;
	m_dMaxMemUseThre = systemOptionConfig.m_dMaxMemUseThre;
	m_nMaxHandleUseThre = systemOptionConfig.m_nMaxHandleUseThre;
	m_nMaxGDIUseThre = systemOptionConfig.m_nMaxGDIUseThre;

	m_dCpuThreMaxValue=systemOptionConfig.m_dCpuThreMaxValue;
	m_dDiskThreMaxValue=systemOptionConfig.m_dDiskThreMaxValue;
	m_dMemThreMaxValue=systemOptionConfig.m_dMemThreMaxValue;
	m_nHandleThreMaxValue=systemOptionConfig.m_nHandleThreMaxValue;
	m_nGDIThreMaxValue=systemOptionConfig.m_nGDIThreMaxValue;

	m_eTempPlatformType = systemOptionConfig.m_eTempPlatformType;
	m_TarSearchResultCrossSize = systemOptionConfig.m_TarSearchResultCrossSize;
	m_ObjSearchResultCrossSize = systemOptionConfig.m_ObjSearchResultCrossSize;

	m_nPlatformLayoutType = systemOptionConfig.m_nPlatformLayoutType;
	m_bShowFullScreen = systemOptionConfig.m_bShowFullScreen;
	m_nLoginTimeSet = systemOptionConfig.m_nLoginTimeSet;

	m_bSaveVDBPostMessage = systemOptionConfig.m_bSaveVDBPostMessage;
	m_pTempPlatformInfo = NULL;
	if (systemOptionConfig.m_pTempPlatformInfo != NULL)
	{
		switch(m_eTempPlatformType)
		{
		case ePlatformXY1Y2:
			{
				m_pTempPlatformInfo = new CPlatformXY1Y2Info(*(CPlatformXY1Y2Info*)systemOptionConfig.m_pTempPlatformInfo);
			}
			break;
		case ePlatformX1X2Y:
			{
				m_pTempPlatformInfo = new CPlatformX1X2YInfo(*(CPlatformX1X2YInfo*)systemOptionConfig.m_pTempPlatformInfo);
			}
			break;
		default:
			{

			}
		}
	}

	m_bPlatformCustomNameEnable = systemOptionConfig.m_bPlatformCustomNameEnable;
	m_bIsShowCalibrateEvaluate = systemOptionConfig.m_bIsShowCalibrateEvaluate;
	int i = 0;
	for (i = 0; i < MaxPlatformCustomNameNum; i++)
	{
		m_strPlatformCustomName[i] = systemOptionConfig.m_strPlatformCustomName[i];
	}

	m_sysOptLightConfig = systemOptionConfig.m_sysOptLightConfig;
}

tagSystemOptionConfig& tagSystemOptionConfig::operator = (const tagSystemOptionConfig& systemOptionConfig)
{
	if (this == &systemOptionConfig)
	{
		return *this;
	}
	m_strSysConfigFilePath = systemOptionConfig.m_strSysConfigFilePath;
	m_strAdminPassword = systemOptionConfig.m_strAdminPassword;
	m_lgidLanguage = systemOptionConfig.m_lgidLanguage;
	m_bSysKeyboardEnabled = systemOptionConfig.m_bSysKeyboardEnabled;
	m_bStartUpShowCamSNSetting = systemOptionConfig.m_bStartUpShowCamSNSetting;

	m_nCurSystemIndex  = systemOptionConfig.m_nCurSystemIndex;			// 当前系统的编号(0、1、2……)

	//m_nCameraMode = systemOptionConfig.m_nCameraMode;					// 相机模式：0，4：3；1，3：4；2，6：5；3，5：6
	m_nDisplayMode = systemOptionConfig.m_nDisplayMode;					// 图像显示模式：0，实时显示；1，帧显示
	m_nDisplayIndex = systemOptionConfig.m_nDisplayIndex;				// 实时显示时显示第n帧
	m_nCommXYUnitExp = systemOptionConfig.m_nCommXYUnitExp;				// 串口XY单位幂4
	m_nCommDUnitExp = systemOptionConfig.m_nCommDUnitExp;				// 串口D单位幂5
	m_nCommDataLenType = systemOptionConfig.m_nCommDataLenType;				// 串口数据长度：0：实际长度；1：固定长度；
	m_nCommDelay = systemOptionConfig.m_nCommDelay;
	m_nCommTimeOut = systemOptionConfig.m_nCommTimeOut;				// 串口超时
	m_nCommReSendTimes = systemOptionConfig.m_nCommReSendTimes;				// 串口重复次数
	m_nShowRunCommandDetail = systemOptionConfig.m_nShowRunCommandDetail;		// 命令记录：0，不记录命令；
	
	m_bMainframeSetAlignParam = systemOptionConfig.m_bMainframeSetAlignParam;
	m_bShowSepTargetPos = systemOptionConfig.m_bShowSepTargetPos;
	
	m_bCommRecDelayEnable = systemOptionConfig.m_bCommRecDelayEnable;		// 是否通信延时
	m_nCommRecDelayTime = systemOptionConfig.m_nCommRecDelayTime;		// 通信延时时间毫秒

	m_bMainframeSetBendParam = systemOptionConfig.m_bMainframeSetBendParam;

	//m_nShowMode	   = systemOptionConfig.m_nShowMode;			// 主界面显示方式：2行3列，3行2列

	//m_bShowProductInfo = systemOptionConfig.m_bShowProductInfo;
	//m_bShowOffsetDirection = systemOptionConfig.m_bShowOffsetDirection;
	//m_bShowCircleTool = systemOptionConfig.m_bShowCircleTool;
	//m_bShowCornerTool = systemOptionConfig.m_bShowCornerTool;
    //m_bShowLineTool   = systemOptionConfig.m_bShowLineTool;

	//m_bShowSearchRect=systemOptionConfig.m_bShowSearchRect;
	m_bChange2um			= systemOptionConfig.m_bChange2um;
    m_bChangeProductSize2um			= systemOptionConfig.m_bChangeProductSize2um;
	m_bAllSteoage			= systemOptionConfig.m_bAllSteoage;
	m_bShowSearchResult	= systemOptionConfig.m_bShowSearchResult;
	m_bShowGamma		= systemOptionConfig.m_bShowGamma;
	m_bCheckModeOnly = systemOptionConfig.m_bCheckModeOnly;
	
	// 目标和对象相机分离时，关联标定目标位置所用的模板是来自目标模板还是对象模板。
	// 0：用目标模板关联标定目标位置，默认值
	// 1：用对象模板关联标定目标位置
	m_nSepCamCalibTargetModelType = systemOptionConfig.m_nSepCamCalibTargetModelType;

	m_TarSearchResultCrossSize = systemOptionConfig.m_TarSearchResultCrossSize;
	m_ObjSearchResultCrossSize = systemOptionConfig.m_ObjSearchResultCrossSize;

	m_bGetSysResourceUse = systemOptionConfig.m_bGetSysResourceUse;
	m_bThreValid = systemOptionConfig.m_bThreValid;
	m_dMaxCpuUseThre = systemOptionConfig.m_dMaxCpuUseThre;
	m_dMaxDiskUseThre = systemOptionConfig.m_dMaxDiskUseThre;
	m_dMaxMemUseThre = systemOptionConfig.m_dMaxMemUseThre;
	m_nMaxHandleUseThre = systemOptionConfig.m_nMaxHandleUseThre;
	m_nMaxGDIUseThre = systemOptionConfig.m_nMaxGDIUseThre;

	m_dCpuThreMaxValue=systemOptionConfig.m_dCpuThreMaxValue;
	m_dDiskThreMaxValue=systemOptionConfig.m_dDiskThreMaxValue;
	m_dMemThreMaxValue=systemOptionConfig.m_dMemThreMaxValue;
	m_nHandleThreMaxValue=systemOptionConfig.m_nHandleThreMaxValue;
	m_nGDIThreMaxValue=systemOptionConfig.m_nGDIThreMaxValue;

	m_eTempPlatformType = systemOptionConfig.m_eTempPlatformType;

	m_nPlatformLayoutType = systemOptionConfig.m_nPlatformLayoutType;
	m_bShowFullScreen = systemOptionConfig.m_bShowFullScreen;
	m_nLoginTimeSet = systemOptionConfig.m_nLoginTimeSet;
	m_bSaveVDBPostMessage = systemOptionConfig.m_bSaveVDBPostMessage;
	if (m_pTempPlatformInfo != NULL)
	{
		delete m_pTempPlatformInfo;
		m_pTempPlatformInfo = NULL;
	}

	if (systemOptionConfig.m_pTempPlatformInfo != NULL)
	{
		switch(m_eTempPlatformType)
		{
		case ePlatformXY1Y2:
			{
				m_pTempPlatformInfo = new CPlatformXY1Y2Info(*(CPlatformXY1Y2Info*)systemOptionConfig.m_pTempPlatformInfo);
			}
			break;
		case ePlatformX1X2Y:
			{
				m_pTempPlatformInfo = new CPlatformX1X2YInfo(*(CPlatformX1X2YInfo*)systemOptionConfig.m_pTempPlatformInfo);
			}
			break;
		default:
			{

			}
		}
	}

	m_bPlatformCustomNameEnable = systemOptionConfig.m_bPlatformCustomNameEnable;
	m_bIsShowCalibrateEvaluate = systemOptionConfig.m_bIsShowCalibrateEvaluate;

	int i = 0;
	for (i = 0; i < MaxPlatformCustomNameNum; i++)
	{
		m_strPlatformCustomName[i] = systemOptionConfig.m_strPlatformCustomName[i];
	}

	m_sysOptLightConfig = systemOptionConfig.m_sysOptLightConfig;

	return *this;
}


CSystemOptionConfig::CSystemOptionConfig()
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath += _T("\\VSConfig\\Config.xml");
	m_strSysConfigFilePath = strPath;

//     m_TarSearchResultCrossSize.nLineColor = RGB(255,0,0);
//     m_TarSearchResultCrossSize.nLineLength = 3000;
//     m_TarSearchResultCrossSize.nRotation = 0;
// 
//     m_ObjSearchResultCrossSize.nLineColor = RGB(0,255,0);
//     m_ObjSearchResultCrossSize.nLineLength = 100;
//     m_ObjSearchResultCrossSize.nRotation = 45;
}

CSystemOptionConfig::~CSystemOptionConfig()
{
}

BOOL CSystemOptionConfig::LoadAdminPassword()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;

	if (xml.Load(m_strSysConfigFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();
			if (xml.FindElem(_T("Code")))
			{
				m_strAdminPassword = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("Code2")))
			{
				m_strAdminPassword2 = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}
			xml.LeaveElem();
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL CSystemOptionConfig::SaveAdminPassword()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysConfigFilePath);
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}
	xml.EnterElem();
	if (!xml.FindElem(_T("Code")))
	{
		xml.AddElem(_T("Code"));
	}
	xml.SetElemData(m_strAdminPassword);

	if(!xml.FindElem(_T("Code2")))
	{
		xml.AddElem(_T("Code2"));
	}
	xml.SetElemData(m_strAdminPassword2);

	xml.LeaveElem();
	
	return xml.Save(m_strSysConfigFilePath);
}

BOOL CSystemOptionConfig::LoadSystemOption()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;
	
	if (xml.Load(m_strSysConfigFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();
			int nData = 0;
			double dVal = 0;
			if (xml.FindElem(_T("Language")))
			{
				xml.GetElemData(nData);
				m_lgidLanguage = (LANGID)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("Code")))
			{
				m_strAdminPassword = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("Code2")))
			{
				m_strAdminPassword2 = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("CurSystemIndex")))
			{
				xml.GetElemData(nData);
				m_nCurSystemIndex = nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("SysKeyboardEnabled")))
			{
				xml.GetElemData(nData);
				m_bSysKeyboardEnabled = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("CameraEnabled")))
			{
				xml.GetElemData(nData);
				m_bStartUpShowCamSNSetting = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			//if (xml.FindElem(_T("CameraMode")))
			//{
			//	xml.GetElemData(nData);
			//	m_nCameraMode = nData;
			//}
			//else
			//{
			//	bSucceed = FALSE;
			//}

			if (xml.FindElem(_T("DisplayMode")))
			{
				xml.GetElemData(nData);
				m_nDisplayMode = nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("DisplayIndex")))
			{
				xml.GetElemData(nData);
				m_nDisplayIndex = nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("CommXYUnitExp")))
			{
				xml.GetElemData(nData);
				m_nCommXYUnitExp = nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("CommDUnitExp")))
			{
				xml.GetElemData(nData);
				m_nCommDUnitExp = nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("CommDataLenType")))
			{
				xml.GetElemData(nData);
				m_nCommDataLenType = nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("CommDelay")))
			{
				xml.GetElemData(nData);
				m_nCommDelay = nData;
			}
			else
			{
				//bSucceed = FALSE;
				m_nCommDelay = 0;
			}

			if (xml.FindElem(_T("CommTimeOut")))
			{
				xml.GetElemData(nData);
				m_nCommTimeOut = nData;
			}
			else
			{
				bSucceed = FALSE;
			}


			if (xml.FindElem(_T("CommReSendTimes")))
			{
				xml.GetElemData(nData);
				m_nCommReSendTimes = nData;
			}
			else
			{
				bSucceed = FALSE;
			}


			if (xml.FindElem(_T("ShowRunCommandDetail")))
			{
				xml.GetElemData(nData);
				m_nShowRunCommandDetail = nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("MainframeSetAlignParam")))
			{
				xml.GetElemData(nData);
				m_bMainframeSetAlignParam = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			//m_bMainframeSetBendParam
			if (xml.FindElem(_T("MainframeSetBendParam")))
			{
				xml.GetElemData(nData);
				m_bMainframeSetBendParam = (BOOL)nData;
			}
			//else
			//{
			//	bSucceed = FALSE;
			//}

			if (xml.FindElem(_T("ShowSepTargetPos")))
			{
				xml.GetElemData(nData);
				m_bShowSepTargetPos = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}
			

			//if (xml.FindElem(_T("ShowCircleTool")))
			//{
			//	xml.GetElemData(nData);
			//	m_bShowCircleTool = (BOOL)nData;
			//}
			//else
			//{
			//	bSucceed = FALSE;
			//}

			//if (xml.FindElem(_T("ShowCornerTool")))
			//{
			//	xml.GetElemData(nData);
			//	m_bShowCornerTool = (BOOL)nData;
			//}
			//else
			//{
			//	bSucceed = FALSE;
			//}

            //if (xml.FindElem(_T("ShowLineTool")))
            //{
            //    xml.GetElemData(nData);
            //    m_bShowLineTool = (BOOL)nData;
            //}
            //else
            //{
            //    bSucceed = FALSE;
            //}
			
			if (xml.FindElem(_T("Change2um")))
			{
				xml.GetElemData(nData);
				m_bChange2um = (BOOL)nData;
			}
			else
			{
				m_bChange2um = FALSE;
			}
            if (xml.FindElem(_T("ChangeProduct2um")))
            {
                xml.GetElemData(nData);
                m_bChangeProductSize2um = (BOOL)nData;
            }
            else
            {
                m_bChangeProductSize2um = FALSE;
            }
			if (xml.FindElem(_T("AllSteoage")))
			{
				xml.GetElemData(nData);
				m_bAllSteoage = (BOOL)nData;
			}
			else
			{
				m_bAllSteoage = FALSE;
			}

			if (xml.FindElem(_T("ShowSearchResult")))
			{
				xml.GetElemData(nData);
				m_bShowSearchResult = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}
			if (xml.FindElem(_T("ShowGamma")))
			{
				xml.GetElemData(nData);
				m_bShowGamma = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}
			if (xml.FindElem(_T("CheckModeOnly")))
			{
				xml.GetElemData(nData);
				m_bCheckModeOnly = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("CheckFPMI")))
			{
				xml.GetElemData(nData);
				m_bCheckFPMI = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			//if (xml.FindElem(_T("ShowMode")))
			//{
			//	xml.GetElemData(nData);
			//	m_nShowMode = nData;
			//}
			//else
			//{
			//	bSucceed = FALSE;
			//}

			//if (xml.FindElem(_T("ShowProductInfo")))
			//{
			//	xml.GetElemData(nData);
			//	m_bShowProductInfo = (BOOL)nData;
			//}
			//else
			//{
			//	m_bShowProductInfo = FALSE;
			//}
			
			//if (xml.FindElem(_T("ShowOffsetDirection")))
			//{
			//	xml.GetElemData(nData);
			//	m_bShowOffsetDirection = (BOOL)nData;
			//}
			//else
			//{
			//	m_bShowOffsetDirection = FALSE;
			//}

			//if (xml.FindElem(_T("ShowSearchRect")))
			//{
			//	xml.GetElemData(nData);
			//	m_bShowSearchRect = (BOOL)nData;
			//}
			//else
			//{
			//	m_bShowSearchRect = FALSE;
			//}
			
			// 布局类型
			if (xml.FindElem(_T("PlatformLayoutType")))
			{
				xml.GetElemData(nData);
				m_nPlatformLayoutType = nData;
			}
			else
			{
				m_nPlatformLayoutType = 0;
			}

			
			if (xml.FindElem(_T("ShowFullScreen")))
			{
				xml.GetElemData(nData);
				m_bShowFullScreen = (BOOL)nData;
			}
			else
			{
				m_bShowFullScreen = FALSE;
			}

			if (xml.FindElem(_T("nLoginTimeSet")))
			{
				xml.GetElemData(nData);
				m_nLoginTimeSet = nData;
			}
			else
			{
				m_nLoginTimeSet = 120;
			}

			if (xml.FindElem(_T("SaveVDBPostMessage")))
			{
				xml.GetElemData(nData);
				m_bSaveVDBPostMessage = (int)nData;
			}
			else
			{
				m_bSaveVDBPostMessage = 1;
			}

			if (xml.FindElem(_T("SepCamCalibTargetModelType")))
			{
				xml.GetElemData(nData);
				m_nSepCamCalibTargetModelType = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("EnableGetSystemResourceUse")))
			{
				xml.GetElemData(nData);
				m_bGetSysResourceUse = (BOOL)nData;
			}
			if (xml.FindElem(_T("IsThreValid")))
			{
				xml.GetElemData(nData);
				m_bThreValid = (BOOL)nData;
			}
			if (xml.FindElem(_T("MaxCpuPercentUseThre")))
			{
				xml.GetElemData(dVal);
				m_dMaxCpuUseThre = dVal;
			}
			if (xml.FindElem(_T("MaxDiskPercentUseThre")))
			{
				xml.GetElemData(dVal);
				m_dMaxDiskUseThre = dVal;
			}
			if (xml.FindElem(_T("MaxMemPercentUseThre")))
			{
				xml.GetElemData(dVal);
				m_dMaxMemUseThre = dVal;
			}
			if (xml.FindElem(_T("MaxHandleCountUseThre")))
			{
				xml.GetElemData(nData);
				m_nMaxHandleUseThre = nData;
			}
			if (xml.FindElem(_T("MaxGDICountUseThre")))
			{
				xml.GetElemData(nData);
				m_nMaxGDIUseThre = nData;
			}

			if (xml.FindElem(_T("CpuThreMaxValue")))
			{
				xml.GetElemData(dVal);
				m_dCpuThreMaxValue = dVal;
			}
			if (xml.FindElem(_T("DiskThreMaxValue")))
			{
				xml.GetElemData(dVal);
				m_dDiskThreMaxValue = dVal;
			}

			if (xml.FindElem(_T("MemThreMaxValue")))
			{
				xml.GetElemData(dVal);
				m_dMemThreMaxValue = dVal;
			}

			if (xml.FindElem(_T("HandleThreMaxValue")))
			{
				xml.GetElemData(nData);
				m_nHandleThreMaxValue = nData;
			}

			if (xml.FindElem(_T("GDIThreMaxValue")))
			{
				xml.GetElemData(nData);
				m_nGDIThreMaxValue = nData;
			}

			if (xml.FindElem(_T("PlatformCustomNameEnable")))
			{
				xml.GetElemData(nData);
				m_bPlatformCustomNameEnable = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("ShowCalibrateEvaluate")))
			{
				xml.GetElemData(nData);
				m_bIsShowCalibrateEvaluate = (BOOL)nData;
			}
			else
			{
				m_bIsShowCalibrateEvaluate = FALSE;
			}

			int i = 0;
			for(i = 0; i < MaxPlatformCustomNameNum; i++)
			{
				CString str;
				str.Format(_T("PlatformCustomName%d"), i+1);
				if (xml.FindElem(str))
				{
					m_strPlatformCustomName[i] = xml.GetElemData();
				}
				else
				{
					bSucceed = FALSE;
				}
			}

			if(xml.FindElem(_T("TarCrossSize")))
			{
				xml.EnterElem();
				if(xml.FindElem(_T("TarCrossWidth")))
				{
					xml.GetElemData(nData);
					m_TarSearchResultCrossSize.nLineWidth = nData;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("TarCrossColor")))
				{
					xml.GetElemData(nData);
					int b = nData >> 16;
					int g = (nData ^( b<<16)) >>8;
					int r = (nData ^ (b<<16) ^ (g<<8));
					m_TarSearchResultCrossSize.nLineColor = RGB(r,g,b);
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("TarCrossLength")))
				{
					xml.GetElemData(nData);
					m_TarSearchResultCrossSize.nLineLength = nData;
				}
				else
				{
                    bSucceed = FALSE;
				}
			
				if(xml.FindElem(_T("TarCrossRotation")))
				{
					xml.GetElemData(dVal);
					m_TarSearchResultCrossSize.nRotation = dVal;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ShowDefortTargetGui")))
				{
					xml.GetElemData(nData);
					m_TarSearchResultCrossSize.m_bShowDefortGui = (BOOL)nData;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ShowTrainRectGui")))
				{
					xml.GetElemData(nData);
					m_TarSearchResultCrossSize.m_bShowRectGui = (BOOL)nData;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ShowOtherGui")))
				{
					xml.GetElemData(nData);
					m_TarSearchResultCrossSize.m_bShowOtherGui = (BOOL)nData;
				}
				else
				{
					bSucceed = FALSE;
				}


				if(xml.FindElem(_T("TarTrainRectLineColor")))
				{
					xml.GetElemData(nData);
					int b = nData >> 16;
					int g = (nData ^( b<<16)) >>8;
					int r = (nData ^ (b<<16) ^ (g<<8));
					m_TarSearchResultCrossSize.nTrainRectLineColor = RGB(r,g,b);
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("TarTrainRectLineWidth")))
				{
					xml.GetElemData(nData);
					m_TarSearchResultCrossSize.nTrainRectLineWidth = nData;
				}
				else
				{
					bSucceed = FALSE;
				}


				if(xml.FindElem(_T("TarOtherUILineColor")))
				{
					xml.GetElemData(nData);
					int b = nData >> 16;
					int g = (nData ^( b<<16)) >>8;
					int r = (nData ^ (b<<16) ^ (g<<8));
					m_TarSearchResultCrossSize.nOtherGuiLineColor = RGB(r,g,b);
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("TarOtherUILineWidth")))
				{
					xml.GetElemData(nData);
					m_TarSearchResultCrossSize.nOtherUILineWidth = nData;
				}
				else
				{
					bSucceed = FALSE;
				}


				xml.LeaveElem();
			}
			else
			{
				bSucceed = FALSE;
			}

        	if(xml.FindElem(_T("ObjCrossSize")))
			{
				xml.EnterElem();
				if(xml.FindElem(_T("ObjCrossWidth")))
				{
					xml.GetElemData(nData);
					m_ObjSearchResultCrossSize.nLineWidth = nData;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ObjCrossColor")))
				{
					xml.GetElemData(nData);
					int b = nData >> 16;
					int g = (nData ^( b<<16)) >>8;
					int r = (nData ^ (b<<16) ^ (g<<8));
					m_ObjSearchResultCrossSize.nLineColor = RGB(r,g,b);
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ObjCrossLength")))
				{
					xml.GetElemData(nData);
					m_ObjSearchResultCrossSize.nLineLength = nData;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ObjCrossRotation")))
				{
					xml.GetElemData(dVal);
					m_ObjSearchResultCrossSize.nRotation = dVal;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ShowDefortObjectGui")))
				{
					xml.GetElemData(nData);
					m_ObjSearchResultCrossSize.m_bShowDefortGui = (BOOL)nData;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ShowTrainRectGui")))
				{
					xml.GetElemData(nData);
					m_ObjSearchResultCrossSize.m_bShowRectGui = (BOOL)nData;
				}
				else
				{
					bSucceed = FALSE;
				}

				if(xml.FindElem(_T("ShowOtherGui")))
				{
					xml.GetElemData(nData);
					m_ObjSearchResultCrossSize.m_bShowOtherGui = (BOOL)nData;
				}
				else
				{
					bSucceed = FALSE;
				}
				if(xml.FindElem(_T("ObjTrainRectLineColor")))
				{
					xml.GetElemData(nData);
					int b = nData >> 16;
					int g = (nData ^( b<<16)) >>8;
					int r = (nData ^ (b<<16) ^ (g<<8));
					m_ObjSearchResultCrossSize.nTrainRectLineColor = RGB(r,g,b);
				}
				else
				{
					bSucceed = FALSE;
				}
				if(xml.FindElem(_T("ObjTrainRectLineWidth")))
				{
					xml.GetElemData(nData);
					m_ObjSearchResultCrossSize.nTrainRectLineWidth = nData;
				}
				else
				{
					bSucceed = FALSE;
				}
				if(xml.FindElem(_T("ObjOtherUILineColor")))
				{
					xml.GetElemData(nData);
					int b = nData >> 16;
					int g = (nData ^( b<<16)) >>8;
					int r = (nData ^ (b<<16) ^ (g<<8));
					m_ObjSearchResultCrossSize.nOtherGuiLineColor = RGB(r,g,b);
				}
				else
				{
					bSucceed = FALSE;
				}
				if(xml.FindElem(_T("ObjOtherUILineWidth")))
				{
					xml.GetElemData(nData);
					m_ObjSearchResultCrossSize.nOtherUILineWidth = nData;
				}
				else
				{
					bSucceed = FALSE;
				}
				xml.LeaveElem();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("AlignOffsetByAreaGui")))
			{
				xml.EnterElem();
				if (!xml.FindElem(_T("LineWidth")))
				{
					bSucceed = FALSE;
				}
				else
				{
					xml.GetElemData(nData);
					m_AlignOffsetAreaGui.m_nLineWidth = nData;
				}
				if (!xml.FindElem(_T("LightLineWidth")))
				{
					bSucceed = FALSE;
				}
				else
				{
					xml.GetElemData(nData);
					m_AlignOffsetAreaGui.m_nLightLineWidth = nData;
				}
				if (!xml.FindElem(_T("NormalLineColor")))
				{
					bSucceed = FALSE;
				}
				else
				{
					xml.GetElemData(nData);
					m_AlignOffsetAreaGui.m_clrNormal = nData;
				}
				if (!xml.FindElem(_T("LightLineColor")))
				{
					bSucceed = FALSE;
				}
				else
				{
					xml.GetElemData(nData);
					m_AlignOffsetAreaGui.m_clrLight = nData;
				}
				xml.LeaveElem();
			}
			else
			{
				bSucceed = FALSE;
			}

			// 光源控制选项读取 "SystemOptionLightControl"
			{
				if (xml.FindElem(_T("SystemOptionLightControl")))
				{
					xml.EnterElem();
					{
						CString strTmp;
						if (xml.FindElem(_T("LightControlName"))) //光源控制器名称
						{
							xml.EnterElem();
							for (int i = 0; i < MAXLightControl; i++)
							{
								strTmp.Format(_T("LightCtrl%d_Name"), i+1);
								if (xml.FindElem(strTmp))
								{
									m_sysOptLightConfig.m_strLightCtrlName[i] = xml.GetElemData();
								}
							}
							xml.LeaveElem();
						}

						for (int i = 0; i < MAXLightControl; i++)
						{
							strTmp.Format(_T("LightControl%d"), i);
							if(xml.FindElem(strTmp))
							{
								// 读取光源参数
								xml.EnterElem();
								{
									// 是否使用
									if (xml.FindElem(_T("LightCtrlUsed")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_bLightCtrlUsed[i] = nData;
									}
									else
									{
										bSucceed = false;
									}


									// 光源类型
									if (xml.FindElem(_T("LightType")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nLightType = nData;
									}
									else
									{
										bSucceed = false;
									}

									// 连接超时
									if (xml.FindElem(_T("ConnectTimeOut")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nConnectTimeOut = nData;
									}
									else
									{
										bSucceed = false;
									}

									// 通信超时
									if (xml.FindElem(_T("CommTimeOut")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nCommTimeOut = nData;
									}
									else
									{
										bSucceed = false;
									}

									// 光源的通信类型
									if(xml.FindElem(_T("CommType")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nCommType = nData;
									}
									else
									{
										bSucceed = false;
									}

									// 光源串口号
									if(xml.FindElem(_T("SerialPort")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nSerialPort = nData;
									}
									else
									{
										bSucceed = false;
									}

									// 光源波特率
									if(xml.FindElem(_T("BaundRate")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nBaundRate = nData;
									}
									else
									{
										bSucceed = false;
									}


									// 光源IP
									if(xml.FindElem(_T("IPAddr")))
									{
										xml.EnterElem();
										{

											for (int k = 0; k < 4; k++)
											{
												strTmp.Format("IP%d", k);
												if (xml.FindElem(strTmp))
												{
													xml.GetElemData(nData);
													m_sysOptLightConfig.m_lightCtrlParam[i].m_szIP[k] = nData;
												}
												else
												{
													bSucceed = false;
												}
											}

										}
										xml.LeaveElem();
									}
									else
									{
										bSucceed = false;
									}


									// 光源端口
									if(xml.FindElem(_T("Port")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nPort = nData;
									}
									else
									{
										bSucceed = false;
									}

									// 光源最大串口号
									if(xml.FindElem(_T("MaxChannels")))
									{
										xml.GetElemData(nData);
										m_sysOptLightConfig.m_lightCtrlParam[i].m_nMaxChannels = nData;
									}
									else
									{
										bSucceed = false;
									}


									// 光源通道亮度值
									if (xml.FindElem(_T("Channel")))
									{
										xml.EnterElem();
										{
											for (int j = 0; j < MAXLightChannelNum; j++)
											{
												strTmp.Format(_T("Channel%d"), j);
												if (xml.FindElem(strTmp))
												{
													xml.GetElemData(nData);
													m_sysOptLightConfig.m_lightCtrlParam[i].m_nChannelValue[j] = nData;
												}
												else
												{
													bSucceed = false;
												}
											}
										}
										xml.LeaveElem();
									}

									// 光源通道是否打开
									if (xml.FindElem(_T("ChannelIsOpen")))
									{
										xml.EnterElem();
										{
											for (int j = 0; j < MAXLightChannelNum; j++)
											{
												strTmp.Format(_T("IsOpen%d"), j);
												if (xml.FindElem(strTmp))
												{
													xml.GetElemData(nData);
													m_sysOptLightConfig.m_lightCtrlParam[i].m_bChannelIsOpen[j] = nData;
												}
												else
												{
													bSucceed = false;
												}
											}
										}
										xml.LeaveElem();
									}

									// 通道名称
									if (xml.FindElem(_T("LightName")))
									{
										xml.EnterElem();
										{
											for (int j = 0; j < MAXLightChannelNum; j++)
											{
												strTmp.Format(_T("LightName%d"), j);
												if (xml.FindElem(strTmp))
												{
													m_sysOptLightConfig.m_lightCtrlParam[i].m_strName[j] = xml.GetElemData();
												}
												else
												{
													bSucceed = false;
												}
											}
										}
										xml.LeaveElem();
									}
								}
								xml.LeaveElem();
							}
						}
					}
					xml.LeaveElem();
				}
			}

			xml.LeaveElem();
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL CSystemOptionConfig::SaveSystemOption()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysConfigFilePath);
	xml.ResetDocElemPos();
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}

	xml.EnterElem();
	if (!xml.FindElem(_T("Language")))
	{
		xml.AddElem(_T("Language"));
	}
	xml.SetElemData((int)m_lgidLanguage);
 
	if (!xml.FindElem(_T("Code")))
	{
		xml.AddElem(_T("Code"));
	}
	xml.SetElemData(m_strAdminPassword);

	if(!xml.FindElem(_T("Code2")))
	{
		xml.AddElem(_T("Code2"));
	}
	xml.SetElemData(m_strAdminPassword2);

	if (!xml.FindElem(_T("CurSystemIndex")))
	{
		xml.AddElem(_T("CurSystemIndex"));
	}
	xml.SetElemData(m_nCurSystemIndex);


	if (!xml.FindElem(_T("SysKeyboardEnabled")))
	{
		xml.AddElem(_T("SysKeyboardEnabled"));
	}
	xml.SetElemData((int)m_bSysKeyboardEnabled);

	if (!xml.FindElem(_T("CameraEnabled")))
	{
		xml.AddElem(_T("CameraEnabled"));
	}
	xml.SetElemData((int)m_bStartUpShowCamSNSetting);


	//if (!xml.FindElem(_T("CameraMode")))
	//{
	//	xml.AddElem(_T("CameraMode"));
	//}
	//xml.SetElemData(m_nCameraMode);


	if (!xml.FindElem(_T("DisplayMode")))
	{
		xml.AddElem(_T("DisplayMode"));
	}
	xml.SetElemData(m_nDisplayMode);


	if (!xml.FindElem(_T("DisplayIndex")))
	{
		xml.AddElem(_T("DisplayIndex"));
	}
	xml.SetElemData(m_nDisplayIndex);



	if (!xml.FindElem(_T("CommXYUnitExp")))
	{
		xml.AddElem(_T("CommXYUnitExp"));
	}
	xml.SetElemData(m_nCommXYUnitExp);


	if (!xml.FindElem(_T("CommDUnitExp")))
	{
		xml.AddElem(_T("CommDUnitExp"));
	}
	xml.SetElemData(m_nCommDUnitExp);


	if (!xml.FindElem(_T("CommDataLenType")))
	{
		xml.AddElem(_T("CommDataLenType"));
	}
	xml.SetElemData(m_nCommDataLenType);

	if (!xml.FindElem(_T("CommDelay")))
	{
		xml.AddElem(_T("CommDelay"));
	}
	xml.SetElemData(m_nCommDelay);

	if (!xml.FindElem(_T("CommTimeOut")))
	{
		xml.AddElem(_T("CommTimeOut"));
	}
	xml.SetElemData(m_nCommTimeOut);


	if (!xml.FindElem(_T("CommReSendTimes")))
	{
		xml.AddElem(_T("CommReSendTimes"));
	}
	xml.SetElemData(m_nCommReSendTimes);


	if (!xml.FindElem(_T("ShowRunCommandDetail")))
	{
		xml.AddElem(_T("ShowRunCommandDetail"));
	}
	xml.SetElemData(m_nShowRunCommandDetail);


	if (!xml.FindElem(_T("MainframeSetAlignParam")))
	{
		xml.AddElem(_T("MainframeSetAlignParam"));
	}
	xml.SetElemData((int)m_bMainframeSetAlignParam);	

	//m_bMainframeSetBendParam
	if (!xml.FindElem(_T("MainframeSetBendParam")))
	{
		xml.AddElem(_T("MainframeSetBendParam"));
	}
	xml.SetElemData((int)m_bMainframeSetBendParam);	

	if (!xml.FindElem(_T("CommRecDelayEnable")))
	{
		xml.AddElem(_T("CommRecDelayEnable"));
	}
	xml.SetElemData((int)m_bCommRecDelayEnable);

	if (!xml.FindElem(_T("CommRecDelayTime")))
	{
		xml.AddElem(_T("CommRecDelayTime"));
	}
	xml.SetElemData(m_nCommRecDelayTime);

	if (!xml.FindElem(_T("SepCamCalibTargetModelType")))
	{
		xml.AddElem(_T("SepCamCalibTargetModelType"));
	}
	xml.SetElemData(m_nSepCamCalibTargetModelType);

	if (!xml.FindElem(_T("ShowSepTargetPos")))
	{
		xml.AddElem(_T("ShowSepTargetPos"));
	}
	xml.SetElemData((int)m_bShowSepTargetPos);

	//if (!xml.FindElem(_T("ShowCircleTool")))
	//{
	//	xml.AddElem(_T("ShowCircleTool"));
	//}
	//xml.SetElemData((int)m_bShowCircleTool);

	//if (!xml.FindElem(_T("ShowCornerTool")))
	//{
	//	xml.AddElem(_T("ShowCornerTool"));
	//}
	//xml.SetElemData((int)m_bShowCornerTool);

    //if (!xml.FindElem(_T("ShowLineTool")))
    //{
    //    xml.AddElem(_T("ShowLineTool"));
    //}
    //xml.SetElemData((int)m_bShowLineTool);
	
	if (!xml.FindElem(_T("Change2um")))
	{
		xml.AddElem(_T("Change2um"));
	}
	xml.SetElemData((int)m_bChange2um);
    if (!xml.FindElem(_T("ChangeProduct2um")))
    {
        xml.AddElem(_T("ChangeProduct2um"));
    }
    xml.SetElemData((int)m_bChangeProductSize2um);

	if (!xml.FindElem(_T("AllSteoage")))
	{
		xml.AddElem(_T("AllSteoage"));
	}
	xml.SetElemData((int)m_bAllSteoage);

	if (!xml.FindElem(_T("ShowSearchResult")))
	{
		xml.AddElem(_T("ShowSearchResult"));
	}
	xml.SetElemData((int)m_bShowSearchResult);

	if (!xml.FindElem(_T("ShowGamma")))
	{
		xml.AddElem(_T("ShowGamma"));
	}
	xml.SetElemData((int)m_bShowGamma);

	if (!xml.FindElem(_T("CheckModeOnly")))
	{
		xml.AddElem(_T("CheckModeOnly"));
	}
	xml.SetElemData((int)m_bCheckModeOnly);

	if (!xml.FindElem(_T("CheckFPMI")))
	{
		xml.AddElem(_T("CheckFPMI"));
	}
	xml.SetElemData((int)m_bCheckFPMI);

	//if (!xml.FindElem(_T("ShowMode")))
	//{
	//	xml.AddElem(_T("ShowMode"));
	//}
	//xml.SetElemData(m_nShowMode);
	
	//if (!xml.FindElem(_T("ShowProductInfo")))
	//{
	//	xml.AddElem(_T("ShowProductInfo"));
	//}
	//xml.SetElemData((int)m_bShowProductInfo);

	//if (!xml.FindElem(_T("ShowOffsetDirection")))
	//{
	//	xml.AddElem(_T("ShowOffsetDirection"));
	//}
	//xml.SetElemData((int)m_bShowOffsetDirection);

	//if (!xml.FindElem(_T("ShowSearchRect")))
	//{
	//	xml.AddElem(_T("ShowSearchRect"));
	//}
	//xml.SetElemData((int)m_bShowSearchRect);

	// 布局类型
	if (!xml.FindElem(_T("PlatformLayoutType")))
	{
		xml.AddElem(_T("PlatformLayoutType"));
	}
	xml.SetElemData((int)m_nPlatformLayoutType);


	// 布局类型
	if (!xml.FindElem(_T("ShowFullScreen")))
	{
		xml.AddElem(_T("ShowFullScreen"));
	}
	xml.SetElemData((int)m_bShowFullScreen);

	if (!xml.FindElem(_T("nLoginTimeSet")))
	{
		xml.AddElem(_T("nLoginTimeSet"));
	}
	xml.SetElemData(m_nLoginTimeSet);

	if (!xml.FindElem(_T("SaveVDBPostMessage")))
	{
		xml.AddElem(_T("SaveVDBPostMessage"));
	}
	xml.SetElemData((int)m_bSaveVDBPostMessage);

	if (!xml.FindElem(_T("EnableGetSystemResourceUse")))
	{
		xml.AddElem(_T("EnableGetSystemResourceUse"));
	}
	xml.SetElemData((int)m_bGetSysResourceUse);

	if (!xml.FindElem(_T("IsThreValid")))
	{
		xml.AddElem(_T("IsThreValid"));
	}
	xml.SetElemData((int)m_bThreValid);

	if (!xml.FindElem(_T("MaxMemPercentUseThre")))
	{
		xml.AddElem(_T("MaxMemPercentUseThre"));
	}
	xml.SetElemData(m_dMaxMemUseThre);

	if (!xml.FindElem(_T("MaxCpuPercentUseThre")))
	{
		xml.AddElem(_T("MaxCpuPercentUseThre"));
	}
	xml.SetElemData(m_dMaxCpuUseThre);

	if (!xml.FindElem(_T("MaxDiskPercentUseThre")))
	{
		xml.AddElem(_T("MaxDiskPercentUseThre"));
	}
	xml.SetElemData(m_dMaxDiskUseThre);

	if (!xml.FindElem(_T("MaxHandleCountUseThre")))
	{
		xml.AddElem(_T("MaxHandleCountUseThre"));
	}
	xml.SetElemData(m_nMaxHandleUseThre);

	if (!xml.FindElem(_T("MaxGDICountUseThre")))
	{
		xml.AddElem(_T("MaxGDICountUseThre"));
	}
	xml.SetElemData(m_nMaxGDIUseThre);


	if (!xml.FindElem(_T("CpuThreMaxValue")))
	{
		xml.AddElem(_T("CpuThreMaxValue"));
	}
	xml.SetElemData(m_dCpuThreMaxValue);

	if (!xml.FindElem(_T("DiskThreMaxValue")))
	{
		xml.AddElem(_T("DiskThreMaxValue"));
	}
	xml.SetElemData(m_dDiskThreMaxValue);

	if (!xml.FindElem(_T("MemThreMaxValue")))
	{
		xml.AddElem(_T("MemThreMaxValue"));
	}
	xml.SetElemData(m_dMemThreMaxValue);

	if (!xml.FindElem(_T("HandleThreMaxValue")))
	{
		xml.AddElem(_T("HandleThreMaxValue"));
	}
	xml.SetElemData(m_nHandleThreMaxValue);

	if (!xml.FindElem(_T("GDIThreMaxValue")))
	{
		xml.AddElem(_T("GDIThreMaxValue"));
	}
	xml.SetElemData(m_nGDIThreMaxValue);

	if (!xml.FindElem(_T("PlatformCustomNameEnable")))
	{
		xml.AddElem(_T("PlatformCustomNameEnable"));
	}
	xml.SetElemData((int)m_bPlatformCustomNameEnable);

	if (!xml.FindElem(_T("ShowCalibrateEvaluate")))
	{
		xml.AddElem(_T("ShowCalibrateEvaluate"));
	}
	xml.SetElemData((int)m_bIsShowCalibrateEvaluate);

	int i = 0;
	for(i = 0; i < MaxPlatformCustomNameNum; i++)
	{
		CString str;
		str.Format(_T("PlatformCustomName%d"), i+1);
		if (!xml.FindElem(str))
		{
			xml.AddElem(str);
		}
		xml.SetElemData(m_strPlatformCustomName[i]);
	}

	if (!xml.FindElem(_T("TarCrossSize")))
	{
		xml.AddElem(_T("TarCrossSize"));
	}
	xml.EnterElem();
	{
		if(!xml.FindElem(_T("TarCrossColor")))
		{
			xml.AddElem(_T("TarCrossColor"));
		}
		xml.SetElemData((int)m_TarSearchResultCrossSize.nLineColor);

		if(!xml.FindElem(_T("TarCrossWidth")))
		{
			xml.AddElem(_T("TarCrossWidth"));
		}
		xml.SetElemData((int)m_TarSearchResultCrossSize.nLineWidth);

		if(!xml.FindElem(_T("TarCrossLength")))
		{
			xml.AddElem(_T("TarCrossLength"));
		}
		xml.SetElemData((int)m_TarSearchResultCrossSize.nLineLength);	

		if(!xml.FindElem(_T("TarCrossRotation")))
		{
			xml.AddElem(_T("TarCrossRotation"));
		}
		xml.SetElemData((double)m_TarSearchResultCrossSize.nRotation);

		if(!xml.FindElem(_T("ShowDefortTargetGui")))
		{
			xml.AddElem(_T("ShowDefortTargetGui"));
		}
		xml.SetElemData((BOOL)m_TarSearchResultCrossSize.m_bShowDefortGui);

		if(!xml.FindElem(_T("ShowTrainRectGui")))
		{
			xml.AddElem(_T("ShowTrainRectGui"));
		}
		xml.SetElemData((BOOL)m_TarSearchResultCrossSize.m_bShowRectGui);

		if(!xml.FindElem(_T("ShowOtherGui")))
		{
			xml.AddElem(_T("ShowOtherGui"));
		}
		xml.SetElemData((BOOL)m_TarSearchResultCrossSize.m_bShowOtherGui);

		if(!xml.FindElem(_T("TarTrainRectLineColor")))
		{
			xml.AddElem(_T("TarTrainRectLineColor"));
		}
		xml.SetElemData((int)m_TarSearchResultCrossSize.nTrainRectLineColor);
		if(!xml.FindElem(_T("TarTrainRectLineWidth")))
		{
			xml.AddElem(_T("TarTrainRectLineWidth"));
		}
		xml.SetElemData((int)m_TarSearchResultCrossSize.nTrainRectLineWidth);
		if(!xml.FindElem(_T("TarOtherUILineColor")))
		{
			xml.AddElem(_T("TarOtherUILineColor"));
		}
		xml.SetElemData((int)m_TarSearchResultCrossSize.nOtherGuiLineColor);
		if(!xml.FindElem(_T("TarOtherUILineWidth")))
		{
			xml.AddElem(_T("TarOtherUILineWidth"));
		}
		xml.SetElemData((int)m_TarSearchResultCrossSize.nOtherUILineWidth);
	}
	xml.LeaveElem();
	
	if (!xml.FindElem(_T("ObjCrossSize")))
	{
		xml.AddElem(_T("ObjCrossSize"));
	}
	xml.EnterElem();
	{
		if(!xml.FindElem(_T("ObjCrossColor")))
		{
			xml.AddElem(_T("ObjCrossColor"));
		}
		xml.SetElemData((int)m_ObjSearchResultCrossSize.nLineColor);

		if(!xml.FindElem(_T("ObjCrossWidth")))
		{
			xml.AddElem(_T("ObjCrossWidth"));
		}
		xml.SetElemData((int)m_ObjSearchResultCrossSize.nLineWidth);

		if(!xml.FindElem(_T("ObjCrossLength")))
		{
			xml.AddElem(_T("ObjCrossLength"));
		}
		xml.SetElemData((int)m_ObjSearchResultCrossSize.nLineLength);	

		if(!xml.FindElem(_T("ObjCrossRotation")))
		{
			xml.AddElem(_T("ObjCrossRotation"));
		}
		xml.SetElemData((double)m_ObjSearchResultCrossSize.nRotation);

		if(!xml.FindElem(_T("ShowDefortObjectGui")))
		{
			xml.AddElem(_T("ShowDefortObjectGui"));
		}
		xml.SetElemData((BOOL)m_ObjSearchResultCrossSize.m_bShowDefortGui);

		if(!xml.FindElem(_T("ShowTrainRectGui")))
		{
			xml.AddElem(_T("ShowTrainRectGui"));
		}
		xml.SetElemData((BOOL)m_ObjSearchResultCrossSize.m_bShowRectGui);

		if(!xml.FindElem(_T("ShowOtherGui")))
		{
			xml.AddElem(_T("ShowOtherGui"));
		}
		xml.SetElemData((BOOL)m_ObjSearchResultCrossSize.m_bShowOtherGui);
		if(!xml.FindElem(_T("ObjTrainRectLineColor")))
		{
			xml.AddElem(_T("ObjTrainRectLineColor"));
		}
		xml.SetElemData((int)m_ObjSearchResultCrossSize.nTrainRectLineColor);
		if(!xml.FindElem(_T("ObjTrainRectLineWidth")))
		{
			xml.AddElem(_T("ObjTrainRectLineWidth"));
		}
		xml.SetElemData((int)m_ObjSearchResultCrossSize.nTrainRectLineWidth);
		if(!xml.FindElem(_T("ObjOtherUILineColor")))
		{
			xml.AddElem(_T("ObjOtherUILineColor"));
		}
		xml.SetElemData((int)m_ObjSearchResultCrossSize.nOtherGuiLineColor);
		if(!xml.FindElem(_T("ObjOtherUILineWidth")))
		{
			xml.AddElem(_T("ObjOtherUILineWidth"));
		}
		xml.SetElemData((int)m_ObjSearchResultCrossSize.nOtherUILineWidth);
	}
	xml.LeaveElem();
	if (!xml.FindElem(_T("AlignOffsetByAreaGui")))
	{
		xml.AddElem(_T("AlignOffsetByAreaGui"));
	}
	xml.EnterElem();
	if (!xml.FindElem(_T("LineWidth")))
	{
		xml.AddElem(_T("LineWidth"));
	}
	xml.SetElemData(m_AlignOffsetAreaGui.m_nLineWidth);
	if (!xml.FindElem(_T("LightLineWidth")))
	{
		xml.AddElem(_T("LightLineWidth"));
	}
	xml.SetElemData(m_AlignOffsetAreaGui.m_nLightLineWidth);
	if (!xml.FindElem(_T("NormalLineColor")))
	{
		xml.AddElem(_T("NormalLineColor"));
	}
	xml.SetElemData((int)m_AlignOffsetAreaGui.m_clrNormal);
	if (!xml.FindElem(_T("LightLineColor")))
	{
		xml.AddElem(_T("LightLineColor"));
	}
	xml.SetElemData((int)m_AlignOffsetAreaGui.m_clrLight);
	xml.LeaveElem();

	// 光源控制选项 "SystemOptionLightControl"
	{
		if (!xml.FindElem(_T("SystemOptionLightControl")))
		{
			xml.AddElem(_T("SystemOptionLightControl"));
		}

		xml.EnterElem();
		{
			CString strTmp;

			//光源控制器名称
			{
				if (!xml.FindElem(_T("LightControlName"))) 
				{
					xml.AddElem(_T("LightControlName"));
				}
				xml.EnterElem();
				for (int i = 0; i < MAXLightControl; i++)
				{
					strTmp.Format(_T("LightCtrl%d_Name"), i+1);
					if(!xml.FindElem(strTmp))
					{
						xml.AddElem(strTmp);
					}
					xml.SetElemData(m_sysOptLightConfig.m_strLightCtrlName[i]);
				}
				xml.LeaveElem();
			}

			for (int i = 0; i < MAXLightControl; i++)
			{
				strTmp.Format(_T("LightControl%d"), i);
				if(!xml.FindElem(strTmp))
				{
					xml.AddElem(strTmp);
				}

				// 保存光源参数
				xml.EnterElem();
				{
					// 是否使用
					if (!xml.FindElem(_T("LightCtrlUsed")))
					{
						xml.AddElem(_T("LightCtrlUsed"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_bLightCtrlUsed[i]);

					// 光源类型
					if (!xml.FindElem(_T("LightType")))
					{
						xml.AddElem(_T("LightType"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nLightType);

					// 连接超时
					if (!xml.FindElem(_T("ConnectTimeOut")))
					{
						xml.AddElem(_T("ConnectTimeOut"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nConnectTimeOut);

					// 通信超时
					if (!xml.FindElem(_T("CommTimeOut")))
					{
						xml.AddElem(_T("CommTimeOut"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nCommTimeOut);

					// 光源的通信类型
					if(!xml.FindElem(_T("CommType")))
					{
						xml.AddElem(_T("CommType"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nCommType);

					// 光源串口号
					if(!xml.FindElem(_T("SerialPort")))
					{
						xml.AddElem(_T("SerialPort"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nSerialPort);

					// 光源波特率
					if(!xml.FindElem(_T("BaundRate")))
					{
						xml.AddElem(_T("BaundRate"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nBaundRate);

					// 光源IP
					if(!xml.FindElem(_T("IPAddr")))
					{
						xml.AddElem(_T("IPAddr"));
					}
					xml.EnterElem();
					{
						for (int k = 0; k < 4; k++)
						{
							strTmp.Format(_T("IP%d"), k);
							if (!xml.FindElem(strTmp))
							{
								xml.AddElem(strTmp);
							}
							xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_szIP[k]);
						}

					}
					xml.LeaveElem();

					// 光源端口
					if(!xml.FindElem(_T("Port")))
					{
						xml.AddElem(_T("Port"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nPort);

					// 光源最大串口号
					if(!xml.FindElem(_T("MaxChannels")))
					{
						xml.AddElem(_T("MaxChannels"));
					}
					xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nMaxChannels);


					// 光源通道亮度值
					if (!xml.FindElem(_T("Channel")))
					{
						xml.AddElem(_T("Channel"));
					}
					xml.EnterElem();
					{
						for (int j = 0; j < MAXLightChannelNum; j++)
						{
							strTmp.Format(_T("Channel%d"), j);
							if (!xml.FindElem(strTmp))
							{
								xml.AddElem(strTmp);
							}
							xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_nChannelValue[j]);
						}
					}
					xml.LeaveElem();



					// 光源通道是否打开
					if (!xml.FindElem(_T("ChannelIsOpen")))
					{
						xml.AddElem(_T("ChannelIsOpen"));
					}
					xml.EnterElem();
					{
						for (int j = 0; j < MAXLightChannelNum; j++)
						{
							strTmp.Format(_T("IsOpen%d"), j);
							if (!xml.FindElem(strTmp))
							{
								xml.AddElem(strTmp);
							}
							xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_bChannelIsOpen[j]);
						}
					}
					xml.LeaveElem();


					//Summer0214 光源通道名称
					if (!xml.FindElem(_T("LightName")))
					{
						xml.AddElem(_T("LightName"));
					}
					xml.EnterElem();
					{
						for (int j = 0; j < MAXLightChannelNum; j++)
						{
							strTmp.Format(_T("LightName%d"), j);
							if (!xml.FindElem(strTmp))
							{
								xml.AddElem(strTmp);
							}
							xml.SetElemData(m_sysOptLightConfig.m_lightCtrlParam[i].m_strName[j]);
						}
					}
					xml.LeaveElem();

				}
				xml.LeaveElem();
			}
		}
		xml.LeaveElem();
	}

	xml.LeaveElem();

	return xml.Save(m_strSysConfigFilePath);
}