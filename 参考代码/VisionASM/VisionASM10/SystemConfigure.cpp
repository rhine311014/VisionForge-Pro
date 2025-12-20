// SystemConfigure.cpp: implementation of the CSystemConfigure class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemConfigure.h"
#include "SystemOptionConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystemConfigure::CSystemConfigure()
{
	m_nSystemNum = 1;
	m_nCurSystemIndex = 0;


	m_nPlatformNum = 0;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath += _T("\\Custom.xml");

	m_strXMLFilePath = strPath;

	m_strSoftwareStandardVersion = "1.0.0";
	m_strSoftwareProjectVersion = "1.1.0";
	m_strSoftwareVersion = GetSoftwareVersion();

	// 根据项目，进行定制性的初始化
	ConfigCustomPlatformInfo();

}

CSystemConfigure::~CSystemConfigure()
{

}

// 软件版本
CString	CSystemConfigure::GetSoftwareVersion()
{	
	m_strSoftwareVersion = m_strSoftwareStandardVersion + "." + m_strSoftwareProjectVersion;
	return m_strSoftwareVersion;
}

// 标准软件版本
CString CSystemConfigure::GetSoftwareStandardVersion()
{
	return m_strSoftwareStandardVersion;
}

// 项目软件版本
CString CSystemConfigure::GetSoftwareProjectVersion()
{
	return m_strSoftwareProjectVersion;
}

// 获取系统数量
int CSystemConfigure::GetSystemNum() const
{
	return m_nSystemNum;
}

// 获取系统信息
BOOL CSystemConfigure::GetSystemInfo(int nIndex, CSystemInfo& systemInfo)
{
	if (nIndex < 0 || nIndex>=m_nSystemNum || nIndex>=m_vSystemInfo.size())
	{
		return FALSE;
	}

	systemInfo = m_vSystemInfo[nIndex];
	return TRUE;
}

// 获取当前系统编号
int CSystemConfigure::GetCurSystemIndex()
{
	return m_nCurSystemIndex;
}



// 根据项目，进行定制性的初始化	
void CSystemConfigure::ConfigCustomPlatformInfo()
{
	// 标准软件版本
	m_strSoftwareStandardVersion = _T("V1.100.1");

	//////////////////////////////////////////////////////////////////////////
	// 从选项配置文件中获取当前系统的编号
	CSystemOptionConfig sysOptionConfig;
	if (!sysOptionConfig.LoadSystemOption())
	{
		sysOptionConfig.SaveSystemOption();
	};

	m_nCurSystemIndex = sysOptionConfig.m_nCurSystemIndex;
	if(0)
	{

		//////////////////////////////////////////////////////////////////////////

		// 项目软件版本
		m_strSoftwareProjectVersion = _T("测试专用");

		// 系统数量
		m_nSystemNum = 1;

		// 系统信息
		CSystemInfo systemInfo;

		////////////////////////////////////////////
		// 系统1
		systemInfo.m_strSystemName = _T("------");
		systemInfo.m_nSystemIndex = 0;
		systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
		systemInfo.m_nSystemPlatformNum = 1;
		m_vSystemInfo.push_back(systemInfo);
		{
			// 平台数量
			m_nPlatformNum = systemInfo.m_nSystemPlatformNum;

			// 平台信息
			SysPlatformInfo sysPlatformInfo;

			// 第1个平台的信息
			sysPlatformInfo.m_nPlatformIndex			= 0;
			sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
			sysPlatformInfo.m_nCamNum					= 1;
			sysPlatformInfo.m_nPositionNum				= 2;
			sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

			sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

			//sysPlatformInfo.m_bPlatformPick = TRUE;
			sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

			sysPlatformInfo.m_bFixCode = TRUE;
			sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
			sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
			sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
			sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
			sysPlatformInfo.m_bShareCamSystem = TRUE;
			sysPlatformInfo.m_bRegisterSuccess			= FALSE;
			sysPlatformInfo.m_bShareCommEnable = TRUE;
			sysPlatformInfo.m_nShareCommIndex = 1;
			sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

			sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			m_vSysPlatformInfo.push_back(sysPlatformInfo);


			//// 第2个平台的信息
			//sysPlatformInfo.m_nPlatformIndex			= 1;
			//sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			//sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
			//sysPlatformInfo.m_nCamNum					= 1;
			//sysPlatformInfo.m_nPositionNum				= 2;
			//sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			//sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			//sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			//sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			//sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			//sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

			//sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

			//sysPlatformInfo.m_bPlatformPick = FALSE;
			//sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

			//sysPlatformInfo.m_bFixCode = TRUE;
			//sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
			//sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
			//sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			//sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
			//sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
			//sysPlatformInfo.m_bShareCamSystem = TRUE;
			//sysPlatformInfo.m_bRegisterSuccess			= FALSE;
			//sysPlatformInfo.m_bShareCommEnable = TRUE;
			//sysPlatformInfo.m_nShareCommIndex = 1;
			//sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

			//sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			//m_vSysPlatformInfo.push_back(sysPlatformInfo);

			return;

		}
	}

	if(0)
	{

		//////////////////////////////////////////////////////////////////////////

		// 项目软件版本
		m_strSoftwareProjectVersion = _T("--------");

		// 系统数量
		m_nSystemNum = 5;

		// 系统信息
		CSystemInfo systemInfo;

		////////////////////////////////////////////
		// 系统1
		systemInfo.m_strSystemName = _T("折弯工位");
		systemInfo.m_nSystemIndex = 0;
		systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
		systemInfo.m_nSystemPlatformNum = 1;
		m_vSystemInfo.push_back(systemInfo);
		{
			// 平台数量
			m_nPlatformNum = 1;

			// 平台信息
			SysPlatformInfo sysPlatformInfo;

			// 第2个平台的信息
			sysPlatformInfo.m_nPlatformIndex			= 0;
			sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
			sysPlatformInfo.m_nCamNum					= 1;
			sysPlatformInfo.m_nPositionNum				= 2;
			sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

			sysPlatformInfo.m_bFixCode = TRUE;
			sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
			sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			sysPlatformInfo.m_bShareCamSystem = TRUE;
			sysPlatformInfo.m_bRegisterSuccess			= FALSE;

			sysPlatformInfo.m_bShowBendPathCalc = TRUE;

			sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			m_vSysPlatformInfo.push_back(sysPlatformInfo);

		}

		// 系统2
		systemInfo.m_strSystemName = _T("FOAM贴附");
		systemInfo.m_nSystemIndex = 1;
		systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
		systemInfo.m_nSystemPlatformNum = 1;
		m_vSystemInfo.push_back(systemInfo);
		{
			// 平台数量
			m_nPlatformNum = 1;
			// 平台信息
			SysPlatformInfo sysPlatformInfo;
			// 第1个平台的信息
			sysPlatformInfo.m_nPlatformIndex			= 0;
			sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
			sysPlatformInfo.m_nCamNum					= 1;
			sysPlatformInfo.m_nPositionNum				= 4;
			sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

			sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
			sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = TRUE;

			sysPlatformInfo.m_bFixCode = TRUE;
			sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
			sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			sysPlatformInfo.m_bShareCamSystem = TRUE;
			sysPlatformInfo.m_bRegisterSuccess			= FALSE;

			sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;// 不启用多段

			sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			m_vSysPlatformInfo.push_back(sysPlatformInfo);

		}	
		// 系统3
		systemInfo.m_strSystemName = _T("折弯检测");
		systemInfo.m_nSystemIndex = 2;
		systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
		systemInfo.m_nSystemPlatformNum = 1;
		m_vSystemInfo.push_back(systemInfo);
		{
			// 平台数量
			m_nPlatformNum = 1;

			// 平台信息
			SysPlatformInfo sysPlatformInfo;

			// 第1个平台的信息
			sysPlatformInfo.m_nPlatformIndex			= 0;
			sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
			sysPlatformInfo.m_nCamNum					= 1;
			sysPlatformInfo.m_nPositionNum				= 2;
			sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

			sysPlatformInfo.m_bFixCode = TRUE;
			sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
			sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			sysPlatformInfo.m_bShareCamSystem = TRUE;
			sysPlatformInfo.m_bRegisterSuccess			= FALSE;
			// 启用多段  2段触发
			sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;
			sysPlatformInfo.m_bTargetCalibUseBoard = TRUE;
			sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			m_vSysPlatformInfo.push_back(sysPlatformInfo);



		}

		// 系统3
		systemInfo.m_strSystemName = _T("拾取工位");
		systemInfo.m_nSystemIndex = 3;
		systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
		systemInfo.m_nSystemPlatformNum = 1;
		m_vSystemInfo.push_back(systemInfo);
		{
			// 平台数量
			m_nPlatformNum = 1;

			// 平台信息
			SysPlatformInfo sysPlatformInfo;

			// 第1个平台的信息
			sysPlatformInfo.m_nPlatformIndex			= 0;
			sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
			sysPlatformInfo.m_nCamNum					= 1;
			sysPlatformInfo.m_nPositionNum				= 2;
			sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;


			sysPlatformInfo.m_bPlatformPick = TRUE;
			sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

			sysPlatformInfo.m_bFixCode = TRUE;
			sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
			sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
			sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
			//sysPlatformInfo.m_bShareCamSystem = TRUE;
			sysPlatformInfo.m_bRegisterSuccess			= FALSE;
			//sysPlatformInfo.m_bShareCommEnable = TRUE;
			//sysPlatformInfo.m_nShareCommIndex = 1;
			//sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

			sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			m_vSysPlatformInfo.push_back(sysPlatformInfo);


		}
		// 系统5
		systemInfo.m_strSystemName = _T("华星折弯上料");
		systemInfo.m_nSystemIndex = 4;
		systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
		systemInfo.m_nSystemPlatformNum = 1;
		m_vSystemInfo.push_back(systemInfo);
		{
			// 平台数量
			m_nPlatformNum = 1;

			// 平台信息
			SysPlatformInfo sysPlatformInfo;

			// 第1个平台的信息
			sysPlatformInfo.m_nPlatformIndex			= 0;
			sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
			sysPlatformInfo.m_nCamNum					= 1;
			sysPlatformInfo.m_nPositionNum				= 2;
			sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

			sysPlatformInfo.m_bPlatformPick = FALSE;
			sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

			sysPlatformInfo.m_bFixCode = TRUE;
			sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
			sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
			sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
			sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
		
			sysPlatformInfo.m_bRegisterSuccess			= FALSE;


			sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			m_vSysPlatformInfo.push_back(sysPlatformInfo);

		}

		return;

	}
	if(0)
	{

		//////////////////////////////////////////////////////////////////////////

		// 项目软件版本
		m_strSoftwareProjectVersion = _T("测试专用");

		// 系统数量
		m_nSystemNum = 1;

		// 系统信息
		CSystemInfo systemInfo;

		////////////////////////////////////////////
		// 系统1
		systemInfo.m_strSystemName = _T("拾取工位");
		systemInfo.m_nSystemIndex = 0;
		systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
		systemInfo.m_nSystemPlatformNum = 1;
		m_vSystemInfo.push_back(systemInfo);
		{
			// 平台数量
			m_nPlatformNum = 1;

			// 平台信息
			// 平台信息
			SysPlatformInfo sysPlatformInfo;

			// 第1个平台的信息
			sysPlatformInfo.m_nPlatformIndex			= 0;
			sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
			sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
			sysPlatformInfo.m_nCamNum					= 1;
			sysPlatformInfo.m_nPositionNum				= 2;
			sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
			sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
			sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
			sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
			sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
			sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

			sysPlatformInfo.m_bPlatformPick = TRUE;
			sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

			sysPlatformInfo.m_bFixCode = TRUE;
			sysPlatformInfo.m_bEnableKeyenceCode = TRUE;

			sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;//启用多段

			sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
			sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
			sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
			//sysPlatformInfo.m_bShareCamSystem = TRUE;
			sysPlatformInfo.m_bRegisterSuccess			= FALSE;
			//sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
			//sysPlatformInfo.m_bShareCommEnable = TRUE;
			//sysPlatformInfo.m_nShareCommIndex = 1;
			//sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

			sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
			m_vSysPlatformInfo.push_back(sysPlatformInfo);



			
		return;

	}
	}
	//////////////////////////////////////////////////////////////////////
	//公司名称
	typedef enum enumSysCompanyName
	{
		eCOMPANY_TEMP			= -1,	 // 临时
		eCOMPANY_VisionSmart	= 0 ,    // 博视
		eCOMPANY_ChengYi		= 1 ,    // 诚亿
		eCOMPANY_FengHua_2		= 2 ,    // 风华二部
		eCOMPANY_FuHeDa			= 3 ,    // 福和达		
		eCOMPANY_SanZuan		= 5 ,    // 三钻		
		eCOMPANY_XinSanLi		= 6 ,    // 鑫三力
		eCOMPANY_KaiDaYang		= 7 ,    // 凯达扬
		eCOMPANY_SiWei			= 8 ,    // 四维
		eCOMPANY_JingYunDa		= 9 ,    // 精运达
		eCOMPANY_DingJing		= 10,    // 鼎晶
		eCOMPANY_ShengDeXin		= 11,    // 盛德鑫
		eCOMPANY_JinTuo			= 12,    // 劲拓
		eCOMPANY_JiYin			= 13,    // 集银
		eCOMPANY_ShenKeDa		= 15,    // 深科达
		eCOMPANY_LianDe			= 16,    // 联得		
		eCOMPANY_NuoFeng		= 17,    // 诺峰
		eCOMPANY_XinYiHe		= 18,    // 鑫义和
		eCOMPANY_JiTe		    = 19,    // 吉特
		eCOMPANY_ZhiYun			= 20,    // 智云
		eCOMPANY_ShiNuoWei		= 21,    // 仕诺微
		eCOMPANY_FengHua_1		= 22,    // 风华一部
		eCOMPANY_FengHua_5		= 23,    // 风华五部
		eCOMPANY_XianHe			= 25,    // 先河
		eCOMPANY_HuaXinJingGong	= 26,    // 华鑫精工
		eCOMPANY_DeShang		= 27,    // 德尚(艾森)
		eCOMPANY_XinKe		    = 28,    // 信科
		eCOMPANY_SanQi			= 29,    // 三齐
		eCOMPANY_AnDeRui		= 30,    // 安德瑞
		eCOMPANY_ZhenLiDa		= 31,    // 振力达(江西振鑫)
		eCOMPANY_PuTianDa		= 32,    // 普天达
		eCOMPANY_ZhongYiHeng	= 33,    // 中易恒
		eCOMPANY_DaChuan		= 35,    // 大川
		eCOMPANY_ChengDa		= 36,    // 成大
		eCOMPANY_ChunHe         = 37,    // 春和自动化
		eCOMPANY_YiTian			= 38,	 // 易天
		eCOMPANY_WeiChuangDa	= 39,	 // 威创达
		eCOMPANY_BaLingLianHe	= 40,    // 深圳八零联合
		eCOMPANY_DaChengXin		= 41,    // 深圳达程鑫科技
		eCOMPANY_ShiZongRuiDi   = 42,    // 世宗瑞迪
		eCOMPANY_ShenzhenSiyuan = 43,	 // 深圳思远
		eCoMPANY_TengShenJingMing = 44,  //  腾盛自动化
	}SysCompanyName;


	SysCompanyName nCompanyName; // 公司编号

    nCompanyName = eCoMPANY_TengShenJingMing;
	//nCompanyName = eCOMPANY_TEMP;
	
	if (eCOMPANY_TEMP == nCompanyName)
	{

		int nProjectIndex = 20;
		switch(nProjectIndex)
		{

			// 腾盛精密-重庆京东方折弯机自动线
		case 20:  // 控制器1：Tray盘拾取&预对位1&预对位2_U9H7x64_三平台五相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.20.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台五相机视觉对位系统-X1X2Y");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						
					}


					return;
				}
			}
			break;
			// 北京测试机台-软件配置
		case 1:
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("1.1.0.0.0(Test)");

					// 系统数量
					m_nSystemNum = 6;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("全景拍照");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);



// 						// 平台数量
// 						m_nPlatformNum = 1;
// 
// 						// 平台信息
// 						SysPlatformInfo sysPlatformInfo;
// 
// 						// 第1个平台的信息
// 						sysPlatformInfo.m_nPlatformIndex			= 0;
// 						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
// 						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
// 						sysPlatformInfo.m_nCamNum					= 1;
// 						sysPlatformInfo.m_nPositionNum				= 2;
// 						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
// 						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
// 						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
// 						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
// 						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
// 						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
// 
// 						sysPlatformInfo.m_bTargetObjectCamSeparate  = FALSE;
// 
// 						sysPlatformInfo.m_bPlatformPick				= TRUE;
// 						sysPlatformInfo.m_ePickCamPlatformType		= ePickCamPlatformSepFix;
// 
// 						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
// 
// 						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
// 
// 						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
// 						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					// 系统2
					systemInfo.m_strSystemName = _T("单相机两位置");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType    = eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType    = eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate  = FALSE;
						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bPlatformPick				= FALSE;
						sysPlatformInfo.m_ePickCamPlatformType		= ePickCamPlatformSepFix;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE; 

						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					// 系统3
					systemInfo.m_strSystemName = _T("两相机四位置不分离");
					systemInfo.m_nSystemIndex = 2;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType    = eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType    = eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate  = FALSE;

						sysPlatformInfo.m_bPlatformPick				= FALSE;
						sysPlatformInfo.m_ePickCamPlatformType		= ePickCamPlatformSepFix;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE; 

						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					// 系统4
					systemInfo.m_strSystemName = _T("四相机四位置分离");
					systemInfo.m_nSystemIndex = 3;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 4;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType    = eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType    = eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate  = TRUE;

						sysPlatformInfo.m_bPlatformPick				= FALSE;
						sysPlatformInfo.m_ePickCamPlatformType		= ePickCamPlatformSepFix;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					// 系统5
					systemInfo.m_strSystemName = _T("六相机八位置分离");
					systemInfo.m_nSystemIndex = 4;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 6;
						sysPlatformInfo.m_nPositionNum				= 8;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType    = eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType    = eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate  = TRUE;

						sysPlatformInfo.m_bPlatformPick				= FALSE;
						sysPlatformInfo.m_ePickCamPlatformType		= ePickCamPlatformSepFix;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					// 系统6
					systemInfo.m_strSystemName = _T("1相机视觉对位系统");
					systemInfo.m_nSystemIndex = 5;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum =1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 8;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						//sysPlatformInfo.m_eAlignerTargetMoveType	= AlignerTargetMoveType::eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate	= TRUE;

						//sysPlatformInfo.m_bTargetCalibUseBoard = TRUE;

						//sysPlatformInfo.m_bShareCamSystem			= TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;


		case 2:
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("1.1.0.0.0(Test)");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息：对象平台1
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 4;
						sysPlatformInfo.m_nPositionNum				= 8;
						sysPlatformInfo.m_nShowMode					= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate  = TRUE;

						sysPlatformInfo.m_bObjectInDifferentComputer = TRUE;
						sysPlatformInfo.m_bTargetInDifferentComputer = FALSE;

						sysPlatformInfo.m_bShareCamSystem			 = TRUE;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);



						// 第2个平台的信息：对象平台2
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 4;
						sysPlatformInfo.m_nPositionNum				= 8;
						sysPlatformInfo.m_nShowMode					= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate  = TRUE;

						sysPlatformInfo.m_bObjectInDifferentComputer = TRUE;
						sysPlatformInfo.m_bTargetInDifferentComputer = FALSE;

						sysPlatformInfo.m_bShareCamSystem			 = TRUE;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);




						// 第3个平台的信息：目标平台1
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 4;
						sysPlatformInfo.m_nPositionNum				= 8;
						sysPlatformInfo.m_nShowMode					= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;


						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;

						sysPlatformInfo.m_bObjectInDifferentComputer = FALSE;
						sysPlatformInfo.m_bTargetInDifferentComputer = TRUE;

						sysPlatformInfo.m_bShareCamSystem			 = TRUE;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);



						// 第4个平台的信息：目标平台2
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 4;
						sysPlatformInfo.m_nPositionNum				= 8;
						sysPlatformInfo.m_nShowMode					= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;

						sysPlatformInfo.m_bObjectInDifferentComputer = FALSE;
						sysPlatformInfo.m_bTargetInDifferentComputer = TRUE;

						sysPlatformInfo.m_bShareCamSystem			 = TRUE;

						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;

		case 3:
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("1.1.0.0.0(Test)");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息：对象平台1
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_nShowMode					= 0;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;


						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);



					}


					return;
				}
			}
			break;

		case 4:  // 控制器7：折弯精度检1&折弯精度检2&折弯R角检_U9H7x64_三平台六相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.24.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台六相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		case 5:  // 控制器2：FOAM拾取1&FOAM拾取2&FOAM贴附1&FOAM贴附2_U9H7x64_四平台五相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.21.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台五相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					

					}

					return;
				}
			}
			break;
		case 6:  // 控制器4&5：折弯上料1&折弯上料2&折弯对位1&折弯对位2_U9H7x64_四平台四相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.23.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;
					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统-XY1Y2");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = FALSE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						//// 第2个平台的信息
						//sysPlatformInfo.m_nPlatformIndex			= 1;
						//sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						//sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						//sysPlatformInfo.m_nCamNum					= 2;
						//sysPlatformInfo.m_nPositionNum				= 2;
						//sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						//sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						//sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						//sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						//sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						//sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						//sysPlatformInfo.m_bPlatformPick = FALSE;
						//sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						//sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						//sysPlatformInfo.m_bFixCode = TRUE;
						//sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						//sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						//sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						//sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						//sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						//sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						//sysPlatformInfo.m_bShareCommEnable = FALSE;
						//sysPlatformInfo.m_nShareCommIndex = 1;
						//sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						//sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						//m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}
					return;
				}
			}
			break;

		}
	}
	
	if (eCoMPANY_TengShenJingMing == nCompanyName)
	{
		int nProjectIndex = 36;//
		switch(nProjectIndex)
		{

		// 腾盛精密-武汉天马折弯机
		case 1:// 上料拾取
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.1.2");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双吸头拾取1+上料2+上料3视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;


						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						//sysPlatformInfo.m_bShareCommEnable = TRUE;
						//sysPlatformInfo.m_nShareCommIndex = 1;
						//sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 0;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息

						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;
						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		case 2:// foam贴附
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.7");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					systemInfo.m_strSystemName = _T("foam胶贴附目标关联视觉对位系统");//用的是这个系统
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 3;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						// 启用多段
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;

						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 3;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;


						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}




					return;
				}
			}
			break;
		case 3:// 折弯对位
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.5");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("折弯对位视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bShowBendPathCalc = FALSE;
						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;


						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShowBendPathCalc = TRUE;

						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShowBendPathCalc = FALSE;

						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShowBendPathCalc = TRUE;

						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		case 4:// 检测
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.7");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("折弯检测视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						// 启用多段  2段触发
						sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;



						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_bTargetCalibUseBoard = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;

						sysPlatformInfo.m_bTargetCalibUseBoard = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;

						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 5:// foam上料+贴附对位---这个系统没使用
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.5");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("foam胶贴附目标关联视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

					
						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 3;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						// 启用多段
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;


						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 3;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;


						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					// 系统2
					systemInfo.m_strSystemName = _T("foam胶贴附目标标定视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 3;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bTarObjCamSepAutoCalibTar = TRUE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						// 启用多段
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;

						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 3;
						sysPlatformInfo.m_nPositionNum				= 4;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;


						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		// 腾盛精密-武汉华星折弯机自动线
		case 10:  // 控制器1：Tray盘拾取&传送带拾取_U8H8x64_两平台两相机视觉对位系统_腾盛精密_CYQ_20211023
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.10.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("Tray拾取&传送带拾取视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 11:  // 控制器2：喷码上料1&喷码上料2_U8H8x64_两平台四相机视觉对位系统_腾盛精密_CYQ_20211023
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.11.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("喷码上料1&喷码上料2视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 12:  // 控制器3：FOAM拾取1&FOAM拾取2&FOAM贴附1&FOAM贴附2_U8H8x64_四平台五相机视觉对位系统_腾盛精密_CYQ_20211023
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.12.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("FOAM拾取1&FOAM拾取2&FOAM贴附1&FOAM贴附2视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 13:  // 控制器4：撕膜检测1&撕膜检测2&二维码检测1&二维码检测2_U8H7x64_四平台四相机视觉对位系统_腾盛精密_CYQ_20211023
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.13.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("撕膜检测1&撕膜检测2&二维码检测1&二维码检测2视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 14:  // 控制器5&6：折弯上料1&折弯上料2&折弯对位1&折弯对位2_U9H7x64_四平台四相机视觉对位系统_腾盛精密_CYQ_20211023
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.14.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("折弯上料1&折弯上料2&折弯对位1&折弯对位2视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 15:  // 控制器8：本压检测1&本压检测2&折弯精度检测1&折弯R胶检测1_U9H7x64_四平台六相机视觉对位系统_腾盛精密_CYQ_20211023
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.15.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("本压检测1&本压检测2&折弯精度检测1&折弯R胶检测1视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 16:  // 控制器9：NG料盘检测&下料装盘_U8H7x64_两平台两相机视觉对位系统_腾盛精密_CYQ_20211023
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.2.16.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("NG料盘检测&下料装盘视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = FALSE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;

		// 腾盛精密-重庆京东方折弯机自动线
		case 20:  // 控制器1：Tray盘拾取&预对位1&预对位2_U9H7x64_三平台五相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.20.1.1");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台五相机视觉对位系统-X1X2Y");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("三平台五相机视觉对位系统-XY1Y2");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		case 21:  // 控制器2：FOAM拾取1&FOAM拾取2&FOAM贴附1&FOAM贴附2_U9H7x64_四平台五相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.21.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台五相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 22:  // 控制器3：PSA拾取1&PSA拾取2&PSA贴附1&PSA贴附2&撕膜检测1&撕膜检测2_U9H7x64_六平台八相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.22.1.1");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("六平台八相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 6;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 6;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第5个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 4;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 3;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第6个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 5;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 3;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("六平台六相机视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 6;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 6;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第5个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 4;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 3;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第6个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 5;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 3;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 23:  // 控制器4&5：折弯上料1&折弯上料2&折弯对位1&折弯对位2_U9H7x64_四平台四相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.23.1.1");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统-X1X2Y");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统-XY1Y2");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}
					return;
				}
			}
			break;
		case 24:  // 控制器7：折弯精度检1&折弯精度检2&折弯R角检_U9H7x64_三平台六相机视觉对位系统_腾盛精密_CYQ_20220124
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.3.24.1.1");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台六相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;

			// 腾盛精密-绵阳京东方M2\B12折弯机自动线(25~30)
		case 25:  // 控制器1：Tray盘拾取&预对位1&预对位2_U9H7x64_三平台五相机视觉对位系统_腾盛精密_LJJ_20230328
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.25.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台五相机视觉对位系统-XYD");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		case 26:  // 控制器2：FOAM拾取1&FOAM拾取2&FOAM贴附1&FOAM贴附2_U9H7x64_四平台五相机视觉对位系统_腾盛精密_LJJ_20230328
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.26.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台五相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 27:  // 控制器3：PSA拾取1&PSA拾取2&PSA贴附1&PSA贴附2_U9H7x64_四平台六相机视觉对位系统_腾盛精密_LJJ_20230328
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.27.1.14");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台六相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		case 28:  // 控制器4&5：折弯上料1&折弯上料2&折弯对位1&折弯对位2_U9H7x64_四平台四相机视觉对位系统_腾盛精密_LJJ_20230328
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.28.1.14");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统-X1X2Y");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统-XY1Y2");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}
					return;
				}
			}
			break;
		case 29:  // 控制器7：本压1&本压2&撕膜检测1&撕膜检测2_U9H7x64_四平台六相机视觉对位系统_腾盛精密_LJJ_20230328
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.29.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台六相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;
						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 0;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 0;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}
					return;
				}
			}
			break;
		case 30:  // 控制器8：折弯精度检1&折弯精度检2&折弯R角检1&折弯R角检2_U9H7x64_四平台八相机视觉对位系统_腾盛精密_LJJ_20230328
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.30.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台八相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;

			// 腾盛精密-重庆京东方B12折弯机5条新线(31~36)  钉钉号：202305081724000139881
		case 31:  // 控制器1：Panel拾取&撕膜上料1&撕膜上料2_U10H10x64_三平台五相机视觉对位系统_腾盛精密_LJJ_20230713
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.31.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台五相机视觉对位系统-XYD");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}
			}
			break;
		case 32:  // 控制器2：FOAM拾取1&FOAM拾取2&FOAM贴附_U10H10x64_三平台四相机视觉对位系统_腾盛精密_LJJ_20230713
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.32.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = FALSE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 33: // 控制器3：FOAM拾取1&FOAM拾取2&FOAM贴附&撕膜检测_U10H10x64_四平台六相机视觉对位系统_腾盛精密_LJJ_20230713
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.33.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台六相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 34:  // 控制器4&5&6：折弯上料1&折弯上料2&折弯对位1&折弯对位2_U10H10x64_四平台四相机视觉对位系统_腾盛精密_LJJ_20230713
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.34.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统-XY1Y2");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}
					return;
				}
			}
			break;
		case 35:  // 控制器8：本压1&本压2&下料检测&撕膜检测&撕膜检测_U10H10x64_五平台七相机视觉对位系统_腾盛精密_LJJ_20230828
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.35.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("五平台七相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 5;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 5;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;
						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 0;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 0;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = TRUE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第5个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 4;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 1;
						sysPlatformInfo.m_nPositionNum				= 1;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						//sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}
					return;
				}
			}
			break;
		case 36:  // 控制器9：折弯精度检1&折弯精度检2&折弯R角检1&折弯R角检2_U10H10x64_四平台八相机视觉对位系统_腾盛精密_LJJ_20230713
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.36.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台八相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXYD;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetCalibUseBoard      = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = FALSE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = TRUE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;

		case 37:  //B12测试 曲面屏只对位Y
			{
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("44.4.34.1.14");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统-XY1Y2");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 0;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 1;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformXY1Y2;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = FALSE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 2;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformY;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 1;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex			= 3;
						sysPlatformInfo.m_ePlatformType				= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType			= ePlatformY;
						sysPlatformInfo.m_nCamNum					= 2;
						sysPlatformInfo.m_nPositionNum				= 2;
						sysPlatformInfo.m_eCamPlatformType			= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType		= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend			= FALSE;
						sysPlatformInfo.m_eAlignerObjectType		= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bShowBendPathCalc         = TRUE;

						sysPlatformInfo.m_bFixCode = TRUE;
						sysPlatformInfo.m_bEnableKeyenceCode = TRUE;
						sysPlatformInfo.m_bEnableMulRegTrigger = TRUE;
						sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = TRUE;
						sysPlatformInfo.m_bEnableMultiCalibExtension = FALSE;
						sysPlatformInfo.m_nMultiCalibExtensionMaxNum = 1;
						sysPlatformInfo.m_bShareCamSystem = TRUE;
						sysPlatformInfo.m_bRegisterSuccess			= FALSE;
						sysPlatformInfo.m_bShareCommEnable = TRUE;
						sysPlatformInfo.m_nShareCommIndex = 2;
						sysPlatformInfo.m_bShareCommPlatformParallel = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}
					return;
				}
			}
		default:
			{
				return;
			}
			break;	
		}
	}

	// 2.风华
	if (eCOMPANY_FengHua_2 == nCompanyName)
	{
		int nProjectIndex = 205;
		switch(nProjectIndex)
		{
		case 1:	// 2.1 风华COG/FOG（双平台三相机全自动-XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.1 风华COG/FOG（双平台3相机全自动）
				// 日期：2016.01.07
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.1.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectVirtual;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 2: // 2.2 风华背光液晶贴合（单平台四相机背光液晶贴合-XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.2 风华背光液晶贴合
				// 日期：2016.01.07
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.2.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformX1X2Y;
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 3: // 2.3 风华COG单平台双相机玻璃上料-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.3 风华COG单平台2相机玻璃上料
				// 日期：2016.01.07
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.3.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectVirtual;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}		  		 
			break;
		case 4: // 2.4 风华COG双平台四相机预压-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.4 风华COG双平台4相机预压
				// 日期：2016.01.07
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.4.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectVirtual;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;	
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 5: // 2.5 风华FOG之单平台单相机FPC上料-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.5 风华FOG之单平台单相机FPC上料
				// 日期：2016.01.07
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.5.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}	
			break;
		case 6: //  2.6 风华TFOG单平台单相机EC机-XD+Y1平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.6 风华TFOG单平台单相机EC机-XD+Y1平台
				// 日期：2016.02.17
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.6.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("EC清洗机上料系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXDPY1;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 7: // 2.7 风华TFOG双平台四相机玻璃FPC上料-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.7 风华TFOG双平台4相机玻璃FPC上料
				// 日期：2016.02.17
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.7.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectVirtual;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 8: // 2.8 风华TFOG双系统双平台六相机预压-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.8 风华TFOG双系统双平台6相机预压
				// 日期：2016.02.17
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.8.0");


					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("双平台6相机视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 3;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 3;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}

			}
			break;
		case 9: // 2.9 风华TFOG双平台双相机本压-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.9 风华TFOG双平台2相机本压
				// 日期：2016.01.07
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.9.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 10: // 2.10 风华FOG双平台双相机玻璃FPC上料-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.10 风华FOG双平台双相机玻璃FPC上料
				// 日期：2016.03.06
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.10.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 11: // 2.11 风华FOG单平台双相机预压-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.11 风华FOG单平台双相机预压
				// 日期：2016.02.17
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.11.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 12: // 2.12 风华TFOG双系统单平台三相机预压-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.12 风华TFOG双系统单平台3相机预压
				// 日期：2016.02.17
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.12.0");


					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台3相机视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						sysPlatformInfo.m_nCamNum				= 3;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 13: // 2.13 风华一部三平台三相机玻璃研磨清洗上下料-XYD平台
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.13 风华一部三平台三相机玻璃研磨清洗上下料
				// 日期：2016.02.17
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.13.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台三相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick			= TRUE;
						sysPlatformInfo.m_ePickCamPlatformType	= ePickCamPlatformSepFix;


						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}

			}
			break;
		case 14: // 2.14 风华二部三平台四相机FOG-XYD平台（工位1：2相机2位置XYD预压；工位2：玻璃上料1相机2位置XD；工位3：玻璃上料1相机2位置XYD；）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.14 风华二部三平台四相机FOG（工位1：2相机2位置XYD预压；工位2：玻璃上料1相机2位置XD；工位3：玻璃上料1相机2位置XYD；）
				// 日期：2017.01.03
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.14.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压2相机2位置XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick			= FALSE;
						sysPlatformInfo.m_ePickCamPlatformType	= ePickCamPlatformSepFix;


						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



						// 第2个平台的信息（玻璃上料1相机2位置XD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（玻璃上料1相机2位置XYD）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 15: // 2.15 风华三平台四相机FOG上料预压-XYD平台（工位1：单相机预压1；工位2：单相机预压2；工位3：双相机玻璃上料；）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.15 风华三平台四相机FOG上料预压（工位1：单相机预压1；工位2：单相机预压2；工位3：双相机玻璃上料；）
				// 日期：2017.01.09
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.15.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 16: // 2.16 风华四平台四相机背光压焊（工位1：单相机X平台焊接1；工位2：单相机X平台焊接2；工位3：单相机X平台焊接3；工位3：单相机X平台焊接4；）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.16 风华四平台四相机背光压焊（工位1：单相机X平台焊接1；工位2：单相机X平台焊接2；工位3：单相机X平台焊接3；工位3：单相机X平台焊接4；）
				// 日期：2017.01.09
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.16.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformX;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformX;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
	
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);


						m_vSysPlatformInfo.push_back(sysPlatformInfo);



						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformX;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 3;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformX;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 18: // 2.18 风华单平台单相机背光压焊机上料-XYD平台（工位1：单相机XYD平台上料；）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.18 风华单平台单相机背光压焊机上料（工位1：单相机XYD平台上料；）
				// 日期：2017.01.09
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.18.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 19: // 2.19 风华三平台四相机COG单预压双上料-XY+D平台（工位1：双相机XY+D预压平台；工位2：玻璃上料1相机2位置XD；工位3：玻璃上料1相机2位置XYD；）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.19 风华三平台四相机COG单预压双上料-XY+D平台（工位1：双相机XY+D预压平台；工位2：玻璃上料1相机2位置XD；工位3：玻璃上料1相机2位置XYD；）
				// 日期：2017.06.23
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.19.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机视觉对位系统-XY+D平台");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料1相机2位置XD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（IC或FPC上料1相机2位置XYD）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}
			}
			break;
		case 201: // 2.201 风华FOG双系统
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.201 风华FOG双系统
				//系统1：双平台四相机预压-XY+D平台[平台1：预压：目标对象分离。两个相机固定，从上向下拍玻璃；一个相机移动一次拍摄两个位置，从下向上拍摄FPC，XY+D平台][平台2：ACF贴合上料：一个相机固定拍摄两个位置XYD，从上向下拍摄玻璃，XYD平台]
				//系统2：双平台两相机预压-XY+D平台[平台1：预压：目标对象不分离。一个相机移动一次拍摄两个位置，从下向上拍摄FPC和玻璃，XY+D平台][平台2：ACF贴合上料：一个相机固定拍摄两个位置XYD，从上向下拍摄玻璃，XYD平台]
				// 创建人：李伟
				// 日期：2018.01.05
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.201.0");


					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 3;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("双平台双相机视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}

			}
			break;
		case 202: 
			{

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.201.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					systemInfo.m_strSystemName = _T("双平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



		
		

					return;
				}

			}
			break;	
		case 203: // 2.203 风华双平台双相机FOF玻璃清洗机-XYD（清洗机120P，QBD-120P）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.203 风华双平台双相机FOF玻璃清洗机-XYD（清洗机120P，QBD-120P）
				// 系统1：双平台双相机FOF玻璃清洗机-XYD[平台1：玻璃清洗上料：一个相机固定拍摄两个位置XYD，从下向上拍摄玻璃，XYD平台][平台2：玻璃清洗上料：一个相机固定拍摄两个位置XYD，从下向上拍摄玻璃，XYD平台]
				// 创建人：fumin
				// 日期：2018.06.05
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.203.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}

			}
			break;
		case 204: // 2.204 风华四平台四相机FOF玻璃上料XYD平台_FPC上料XY+D平台（QBD-120P）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.204 风华四平台四相机FOF玻璃上料XYD平台_FPC上料XY+D平台
				//系统1：四平台四相机FOF玻璃上料XYD平台_FPC上料XY+D平台
				//       [平台1：玻璃上料1：一个相机固定拍摄两个位置XYD，从下向上拍摄玻璃，XYD平台]
				//       [平台2：玻璃上料2：一个相机固定拍摄两个位置XYD，从下向上拍摄玻璃，XYD平台]
				//       [平台3：FPC上料1：一个相机移动拍摄XY平台上FPC的两个位置，从上向下拍摄FPC，预压旋转D轴调整角度，XY+D平台]
				//       [平台4：FPC上料2：一个相机移动拍摄XY平台上FPC的两个位置，从上向下拍摄FPC，预压旋转D轴调整角度，XY+D平台]
				// 创建人：fumin
				// 日期：2018.06.05
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.204.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台四相机FOF玻璃上料XYD平台_FPC上料XY+D平台");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 3;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}

			}
			break;
		case 205: // 2.205 风华单平台四(二)相机FOF预压-XY+D（QBD-120P）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.205 风华单平台四(二)相机FOF预压-XY+D
				//系统1：单平台四相机预压-XY+D平台[平台1：预压：目标对象分离。两个相机固定，从上向下拍玻璃；2个相机拍摄两个位置，从下向上拍摄FPC，XY+D平台]
				//系统2：单平台双相机预压-XY+D平台[平台1：预压：目标对象不分离。一个相机移动一次拍摄两个位置，从下向上拍摄FPC和玻璃，XY+D平台]
				// 创建人：fumin
				// 日期：2018.06.05
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.205.0");


					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台四相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}



					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台双相机视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					return;
				}

			}
			break;		
		case 206: // 2.206 风华双平台双相机FOF本压上料-XYD（QBD-120P）
			{
				//////////////////////////////////////////////////////////////////////////
				// 2.206 风华双平台双相机FOF本压上料-XYD（QBD-120P）
				// 系统1：双平台双相机FOF本压上料-XYD[平台1：玻璃本压上料：一个相机固定拍摄两个位置XYD，从下向上拍摄玻璃，XYD平台][平台2：玻璃本压上料：一个相机固定拍摄两个位置XYD，从下向上拍摄玻璃，XYD平台]
				// 创建人：fumin
				// 日期：2018.06.05
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("2.206.0");


					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}

			}
			break;
		default:
			{
				return;
			}
			break;		
		}
	}

	// 6.鑫三力
	if (eCOMPANY_XinSanLi == nCompanyName)
	{
		int nProjectIndex = 162;
		switch(nProjectIndex)
		{
		case 1:	// 6.1鑫三力COG/FOG（单平台单相机玻璃上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.1鑫三力COG/FOG（单平台单相机玻璃上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.1.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃上料XYD平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 2: // 6.2鑫三力COG/FOG（单平台双相机预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.2鑫三力COG/FOG（单平台双相机预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.2.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台双相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 3: // 6.3鑫三力COG/FOG（单平台四相机预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.3鑫三力COG/FOG（单平台四相机预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.3.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台四相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}		  		 
			break;
		case 4: // 6.4 鑫三力COG/FOG（双平台三相机上料预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.4 鑫三力COG/FOG（双平台三相机上料预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.4.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 5: // 6.5 鑫三力COG/FOG（双平台五相机上料预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.5 鑫三力COG/FOG（双平台五相机上料预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.5.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台五相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}	
			break;
		case 6: // 6.6 鑫三力COG/FOG（双平台双相机上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.6 鑫三力COG/FOG（双平台双相机上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.6.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 7: // 6.7 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.7 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.7.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第3个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 8: // 6.8鑫三力COG/FOG（单平台单相机玻璃上料XD+Y平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.8鑫三力COG/FOG（单平台单相机玻璃上料XD+Y平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.8.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃上料XDPY平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 9: // 6.9鑫三力COG/FOG（单平台双相机预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.9鑫三力COG/FOG（单平台双相机预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.9.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台双相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 10: // 6.10鑫三力COG/FOG（单平台四相机预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.10鑫三力COG/FOG（单平台四相机预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.10.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台四相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 11: // 6.11 鑫三力COG/FOG（双平台三相机上料预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.11 鑫三力COG/FOG（双平台三相机上料预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.11.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 12: // 6.12 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.12 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.12.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台五相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 13: // 6.13 鑫三力COG/FOG（双平台双相机上料XD+Y平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.13 鑫三力COG/FOG（双平台双相机上料XD+Y平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.13.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 14: // 6.14 鑫三力COG/FOG（三平台四相机预压双上料XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.14 鑫三力COG/FOG（三平台四相机预压双上料XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.14.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
			

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第3个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXDPY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 15:// 6.15 鑫三力COG/FOG（单平台单相机IC上料XY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.15 鑫三力COG/FOG（单平台单相机IC上料XY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.15.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台单相机IC上料XY平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 16: // 6.16 鑫三力COG/FOG（双平台三相机预压XY+D平台_IC上料XY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.16 鑫三力COG/FOG（双平台三相机预压XY+D平台_IC上料XY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.16.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("双平台三相机预压_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第1个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}


			}
			break;
		case 17: // 6.17 鑫三力COG/FOG（三平台三相机玻璃上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.17 鑫三力COG/FOG（三平台三相机玻璃上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.17.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("三平台三相机玻璃上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 18: // 6.18 鑫三力COG/FOG（三平台三相机预压XY+D平台_IC上料XY平台_玻璃上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.18 鑫三力COG/FOG（三平台三相机预压XY+D平台_IC上料XY平台_玻璃上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.18.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("三平台三相机预压_IC_玻璃上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 19: // 6.19 鑫三力COG/FOG（单平台双相机上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.19 鑫三力COG/FOG（单平台双相机上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.19.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}

			}
			break;
		case 20: // 6.20 鑫三力COG/FOG（双平台四相机双预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.20 鑫三力COG/FOG（双平台四相机双预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.20.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("双平台四相机双预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}

			}
			break;
		case 21: // 6.21 鑫三力COG/FOG（三平台四相机FPC双相机XYD上料_单相机玻璃双上料XYD）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.21 鑫三力COG/FOG（三平台四相机FPC双相机XYD上料_单相机玻璃双上料XYD）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.21.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("三平台四相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
				

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
				
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 22: // 6.22 鑫三力COG/FOG（双平台双相机XD+Y上料_IC上料XY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.22 鑫三力COG/FOG（双平台双相机XD+Y上料_IC上料XY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.22.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("双平台双相机XYD_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC XY上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		

					}


					return;
				}
			}
			break;
		case 23: // 6.23 鑫三力COG/FOG（双平台三相机XD+Y上料_FPC双相机上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.23 鑫三力COG/FOG（双平台三相机XD+Y上料_FPC双相机上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.23.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("双平台三相机XD+Y_FPC双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（双相机FPC XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		

					}


					return;
				}

			}
			break;
		case 24: // 6.24 鑫三力背光贴合（单平台四相机背光贴合系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.24 鑫三力背光贴合（单平台四相机背光贴合系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.24.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("单平台四相机背光贴合视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 25: // 6.25 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XY+D）
			{

				//////////////////////////////////////////////////////////////////////////
				// 6.25 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XY+D）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.25.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台3相机预压XY+D视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXDPY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统3
					systemInfo.m_strSystemName = _T("双平台5相机预压XY+D平台视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
		

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


					}




					return;
				}

			}
			break;
		case 26: // 6.26 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XYD）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.26 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XYD）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.26.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台3相机预压XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
				

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
		

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("双平台5相机预压XYD平台视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
			

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


					}


					return;
				}

			}
			break;
		case 27:// 6.27 鑫三力FOG/COG（单相机玻璃拾取视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.27 鑫三力FOG/COG（单相机玻璃拾取视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.27.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃拾取XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



					}


					return;
				}
			}
			break;
		case 28: // 6.28 鑫三力FOG/COG（三相机三平台玻璃拾取_玻璃上料_玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.28 鑫三力FOG/COG（三相机三平台玻璃拾取_玻璃上料_玻璃上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.28.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃拾取XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
	
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
		

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



					}


					return;
				}

			}
			break;
		case 29: // 6.29 鑫三力FOG/COG（四相机三平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.29 鑫三力FOG/COG（四相机三平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.29.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压_双玻璃拾取视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
	

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
	

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
		

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



					}


					return;
				}
			}
			break;
		case 30: // 6.30 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.30 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.30.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压_玻璃上料_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
			

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（IC XY上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



					}


					return;
				}
			}
			break;
		case 31: // 6.31 鑫三力COG/FOG（双平台双相机：X+XYD上料、X+XYD上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.31 鑫三力COG/FOG（双平台双相机：X+XYD上料、X+XYD上料）
				// 日期：2017.03.31
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.31.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


						// 第2个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}

					return;
				}
			}
			break;
		case 32: // 6.32 鑫三力COG/FOG（双平台三相机：XY+D预压、X+XYD上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.32 鑫三力COG/FOG（双平台三相机：XY+D预压、X+XYD上料）
				// 日期：2017.04.01
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.32.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


						// 第2个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}

					return;
				}


			}
			break;
		case 33: // 6.33 鑫三力COG/FOG（单平台单相机：X+XYD上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.33 鑫三力COG/FOG（单平台单相机：X+XYD上料）
				// 日期：2017.04.01
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.33.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}

					return;
				}
			}
			break;
		case 34: // 6.34 鑫三力FOG/COG（五相机四平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.34 鑫三力FOG/COG（五相机四平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
				// （双系统双平台全自动视觉对位系统XY+D）
				// 系统1：平台1：双相机预压XY+D；     平台2：单相机玻璃上料XD+Y
				// 系统2：平台1：单相机本压上料XD+Y； 平台2：单相机IC上料XYD
				// 日期：2017.02.04
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.34.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1   平台1：双相机预压XY+D；     平台2：单相机玻璃上料XD+Y
					systemInfo.m_strSystemName = _T("双平台3相机预压XY+D_上料XD+Y视觉系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料XD+Y）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统2 平台1：单相机本压上料XD+Y； 平台2：单相机IC上料XYD
					systemInfo.m_strSystemName = _T("双平台2相机本压上料XD+Y_IC上料XYD视觉系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（本压上料XD+Y）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（IC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}
			}
			break;
		case 35: // 6.35 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.35 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.35.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第3个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 36: // 6.36 鑫三力COG/FOG（双平台三相机预压XYD平台，上料XDPY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.36 鑫三力COG/FOG（双平台三相机预压XYD平台，上料XDPY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.36.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	



					}


					return;
				}
			}
			break;
		case 37: // 6.37 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.37 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.37.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台两相机玻璃上料_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo; 

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（IC XY上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



					}


					return;
				}
			}
			break;
		case 38: // 6.38 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.38 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.38.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机预压_玻璃上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


					}


					return;
				}
			}
			break;
		case 39: // 6.39 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.39 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.39.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台五相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 40: // 6.40 鑫三力COG/FOG（三平台五相机上料XYD平台_双本压校正XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.40 鑫三力COG/FOG（三平台五相机上料XYD平台_双本压校正XYD平台）
				// 日期：2017.11.06
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.40.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台五相机上料校正视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 41:
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.41 鑫三力FOG/COG（平台1：双相机预压XY+D；     平台2：单相机XD+Y玻璃上料   平台3：本压单相机XYD上料）
				// 日期：2017.11.01
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.41.0");
					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;
					////////////////////////////////////////////
					// 系统1   平台1：双相机预压XY+D；     平台2：单相机XD+Y玻璃上料   平台3：本压单相机XYD上料
					systemInfo.m_strSystemName = _T("三平台4相机预压XY+D_XD+Y上料_本压XYD系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						// sysPlatformInfo.m_eProtocolType			= eProRegKeyenceSerialPCLink;
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC/FPC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXDPY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						// sysPlatformInfo.m_eProtocolType			= eProRegKeyenceSerialPCLink;
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC/FPC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						// sysPlatformInfo.m_eProtocolType			= eProRegKeyenceSerialPCLink;
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}




					return;
				}

			}
			break;
			//////////////////////////////////////////////////////////////////////////
			//读写寄存器
		case 101:	// 6.1鑫三力COG/FOG（单平台单相机玻璃上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.1鑫三力COG/FOG（单平台单相机玻璃上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.101.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃上料XYD平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						

						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 102: // 6.2鑫三力COG/FOG（单平台双相机预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.2鑫三力COG/FOG（单平台双相机预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.102.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台双相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 103: // 6.3鑫三力COG/FOG（单平台四相机预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.3鑫三力COG/FOG（单平台四相机预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.103.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台四相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}		  		 
			break;
		case 104: // 6.4 鑫三力COG/FOG（双平台三相机上料预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.4 鑫三力COG/FOG（双平台三相机上料预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.104.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 105: // 6.5 鑫三力COG/FOG（双平台五相机上料预压XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.5 鑫三力COG/FOG（双平台五相机上料预压XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.105.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台五相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}	
			break;
		case 106: // 6.6 鑫三力COG/FOG（双平台双相机上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.6 鑫三力COG/FOG（双平台双相机上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.106.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 107: // 6.7 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.7 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.107.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
	

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第3个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 108: // 6.8鑫三力COG/FOG（单平台单相机玻璃上料XD+Y平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.8鑫三力COG/FOG（单平台单相机玻璃上料XD+Y平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.108.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃上料XDPY平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 109: // 6.9鑫三力COG/FOG（单平台双相机预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.9鑫三力COG/FOG（单平台双相机预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.109.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台双相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 110: // 6.10鑫三力COG/FOG（单平台四相机预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.10鑫三力COG/FOG（单平台四相机预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.110.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台四相机预压平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 111: // 6.11 鑫三力COG/FOG（双平台三相机上料预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.11 鑫三力COG/FOG（双平台三相机上料预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.111.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 112: // 6.12 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.12 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.112.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台五相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 113: // 6.13 鑫三力COG/FOG（双平台双相机上料XD+Y平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.13 鑫三力COG/FOG（双平台双相机上料XD+Y平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.113.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 114: // 6.14 鑫三力COG/FOG（三平台四相机预压双上料XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.14 鑫三力COG/FOG（三平台四相机预压双上料XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.114.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第3个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 115:// 6.15 鑫三力COG/FOG（单平台单相机IC上料XY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.15 鑫三力COG/FOG（单平台单相机IC上料XY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.115.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台单相机IC上料XY平台视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
				

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 116: // 6.16 鑫三力COG/FOG（双平台三相机预压XY+D平台_IC上料XY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.16 鑫三力COG/FOG（双平台三相机预压XY+D平台_IC上料XY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.116.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("双平台三相机预压_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第1个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}


			}
			break;
		case 117: // 6.17 鑫三力COG/FOG（三平台三相机玻璃上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.17 鑫三力COG/FOG（三平台三相机玻璃上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.117.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("三平台三相机玻璃上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;
		case 118: // 6.18 鑫三力COG/FOG（三平台三相机预压XY+D平台_IC上料XY平台_玻璃上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.18 鑫三力COG/FOG（三平台三相机预压XY+D平台_IC上料XY平台_玻璃上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.118.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("三平台三相机预压_IC_玻璃上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 119: // 6.19 鑫三力COG/FOG（单平台双相机上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.19 鑫三力COG/FOG（单平台双相机上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.119.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}

			}
			break;
		case 120: // 6.20 鑫三力COG/FOG（双平台四相机双预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.20 鑫三力COG/FOG（双平台四相机双预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.120.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("双平台四相机双预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}

			}
			break;
		case 121: // 6.21 鑫三力COG/FOG（三平台四相机FPC双相机XYD上料_单相机玻璃双上料XYD）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.21 鑫三力COG/FOG（三平台四相机FPC双相机XYD上料_单相机玻璃双上料XYD）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.121.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("三平台四相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 122: // 6.22 鑫三力COG/FOG（双平台双相机XD+Y上料_IC上料XY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.22 鑫三力COG/FOG（双平台双相机XD+Y上料_IC上料XY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.122.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("双平台双相机XYD_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
				

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC XY上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		

					}


					return;
				}
			}
			break;
		case 123: // 6.23 鑫三力COG/FOG（双平台三相机XD+Y上料_FPC双相机上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.23 鑫三力COG/FOG（双平台三相机XD+Y上料_FPC双相机上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.123.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("双平台三相机XD+Y_FPC双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
				
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（双相机FPC XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		

					}


					return;
				}

			}
			break;
		case 124: // 6.24 鑫三力背光贴合（单平台四相机背光贴合系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.24 鑫三力背光贴合（单平台四相机背光贴合系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.124.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("单平台四相机背光贴合视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
					

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}
			}
			break;
		case 125: // 6.25 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XY+D）
			{

				//////////////////////////////////////////////////////////////////////////
				// 6.25 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XY+D）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.125.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台3相机预压XY+D视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
					
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
			

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统3
					systemInfo.m_strSystemName = _T("双平台5相机预压XY+D平台视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
	
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


					}




					return;
				}

			}
			break;
		case 126: // 6.26 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XYD）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.26 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XYD）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.126.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台3相机预压XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第1个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("双平台5相机预压XYD平台视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
	
						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第1个平台的信息（预压平台）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


					}


					return;
				}

			}
			break;
		case 127:// 6.27 鑫三力FOG/COG（单相机玻璃拾取视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.27 鑫三力FOG/COG（单相机玻璃拾取视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.27.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃拾取XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



					}


					return;
				}
			}
			break;
		case 128: // 6.28 鑫三力FOG/COG（三相机三平台玻璃拾取_玻璃上料_玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.28 鑫三力FOG/COG（三相机三平台玻璃拾取_玻璃上料_玻璃上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.128.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机玻璃拾取XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



					}


					return;
				}

			}
			break;
		case 129: // 6.29 鑫三力FOG/COG（四相机三平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.29 鑫三力FOG/COG（四相机三平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.129.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压_双玻璃拾取视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);



					}


					return;
				}
			}
			break;
		case 130: // 6.30 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.30 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.130.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压_玻璃上料_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（IC XY上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



					}


					return;
				}
			}
			break;
		case 131: // 6.31 鑫三力COG/FOG（双平台双相机：X+XYD上料、X+XYD上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.31 鑫三力COG/FOG（双平台双相机：X+XYD上料、X+XYD上料）
				// 日期：2017.03.31
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.131.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台双相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


						// 第2个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}

					return;
				}
			}
			break;
		case 132: // 6.32 鑫三力COG/FOG（双平台三相机：XY+D预压、X+XYD上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.32 鑫三力COG/FOG（双平台三相机：XY+D预压、X+XYD上料）
				// 日期：2017.04.01
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.132.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


						// 第2个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}

					return;
				}


			}
			break;
		case 133: // 6.33 鑫三力COG/FOG（单平台单相机：X+XYD上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.33 鑫三力COG/FOG（单平台单相机：X+XYD上料）
				// 日期：2017.04.01
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.133.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台单相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（X+XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectBench;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}

					return;
				}
			}
			break;
		case 134: // 6.34 鑫三力FOG/COG（五相机四平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.34 鑫三力FOG/COG（五相机四平台XYD预压_XD+Y玻璃上料_XD+Y玻璃上料视觉对位系统）
				// （双系统双平台全自动视觉对位系统XY+D）
				// 系统1：平台1：双相机预压XY+D；     平台2：单相机玻璃上料XD+Y
				// 系统2：平台1：单相机本压上料XD+Y； 平台2：单相机IC上料XYD
				// 日期：2017.02.04
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.134.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1   平台1：双相机预压XY+D；     平台2：单相机玻璃上料XD+Y
					systemInfo.m_strSystemName = _T("双平台3相机预压XY+D_上料XD+Y视觉系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料XD+Y）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}

					////////////////////////////////////////////
					// 系统2 平台1：单相机本压上料XD+Y； 平台2：单相机IC上料XYD
					systemInfo.m_strSystemName = _T("双平台2相机本压上料XD+Y_IC上料XYD视觉系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（本压上料XD+Y）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（IC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}
			}
			break;
		case 135: // 6.35 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.35 鑫三力COG/FOG（三平台四相机预压双上料XYD平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.135.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第3个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 136: // 6.36 鑫三力COG/FOG（双平台三相机预压XYD平台，上料XDPY平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.36 鑫三力COG/FOG（双平台三相机预压XYD平台，上料XDPY平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.136.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	



					}


					return;
				}
			}
			break;
		case 137: // 6.37 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.37 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料_XY 芯片上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.137.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台两相机玻璃上料_IC上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo; 

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（IC XY上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



					}


					return;
				}
			}
			break;
		case 138: // 6.38 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料视觉对位系统）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.38 鑫三力FOG/COG（三相机双平台XY+D预压_XD+Y玻璃上料视觉对位系统）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.138.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台三相机预压_玻璃上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（玻璃上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


					}


					return;
				}
			}
			break;
		case 139: // 6.39 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.39 鑫三力COG/FOG（双平台五相机上料预压XY+D平台）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.139.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("双平台五相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 140: // 6.40 鑫三力COG/FOG（三平台五相机上料XYD平台_双本压校正XYD平台）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.40 鑫三力COG/FOG（三平台五相机上料XYD平台_双本压校正XYD平台）
				// 日期：2017.11.06
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.140.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台五相机上料校正视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（传送带上玻璃拾取）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;
		case 141:
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.41 鑫三力FOG/COG（平台1：双相机预压XY+D；     平台2：单相机XD+Y玻璃上料   平台3：本压单相机XYD上料）
				// 日期：2017.11.01
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.141.0");
					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;
					////////////////////////////////////////////
					// 系统1   平台1：双相机预压XY+D；     平台2：单相机XD+Y玻璃上料   平台3：本压单相机XYD上料
					systemInfo.m_strSystemName = _T("三平台4相机预压XY+D_XD+Y上料_本压XYD系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC/FPC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXDPY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC/FPC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}




					return;
				}

			}
			break;
		case 142: // 6.142 鑫三力COG/FOG（四平台六相机双预压XY+D平台，双IC上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.142 鑫三力COG/FOG（四平台六相机双预压XY+D平台，双IC上料）
				// 日期：2017.02.04
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.142.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("四平台六相机双预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第2个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						// 第3个平台的信息（IC上料）
						sysPlatformInfo.m_nPlatformIndex = 3;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}


					return;
				}

			}
			break;
		case 143: // 6.143 鑫三力COG/FOG（双系统4相机2V2---三相机2V2）
			{
				//////////////////////////////////////////////////////////////////////////
				//  6.143 鑫三力COG/FOG（双系统4相机2V2---三相机2V2）
				// 日期：2018.05.30
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.143.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("单平台四相机预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);							

					}

					systemInfo.m_strSystemName = _T("单平台三相机预压视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 3;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);							

					}


					return;
				}

			}
			break;
		case 144: // 6.144 鑫三力FPC拾取上料（单平台单相机大视野拾取）
			{
				//////////////////////////////////////////////////////////////////////////
				//   6.144 鑫三力FPC拾取上料（单平台单相机大视野拾取）
				// 日期：2018.05.30
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.144.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("单平台单相机拾取视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XY+D预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = TRUE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);							

					}




					return;
				}

			}
			break;
		case 145: // 6.145 三平台三相机上料（XYD_XYD_XY）
			{
				//////////////////////////////////////////////////////////////////////////
				//   6.145 三平台三相机上料（XYD_XYD_XY）
				// 日期：2018.05.30
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.145.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("三平台三相机上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
						
						
						
						
						
						
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（XYD上料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息（XY上料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);

					}




					return;
				}

			}
			break;
		case 61:	// 6.61 四平台四相机COG/FOG绑定 设备型号5620
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.61 四平台四相机COG/FOG绑定
				// 平台1：玻璃上料1-XYD（XD+Y）;平台2：玻璃上料2-XYD（XD+Y）;平台3：本压上料1-XYD(X+YD);平台4：本压上料2-XYD(X+YD)
				// 日期：2018.11.07
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.61.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("四平台四相机XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息：玻璃上料1-XYD（XD+Y）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	


						// 第2个平台的信息：玻璃上料2-XYD（XD+Y）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	



						// 第3个平台的信息：本压上料1-XYD(X+YD)
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	



						// 第4个平台的信息：本压上料2-XYD(X+YD)
						sysPlatformInfo.m_nPlatformIndex = 3;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}
			}
			break;

		case 146:	// 6.146 鑫三力COG/FOG（系统1:单平台两相机ACF对位检测, 系统2：单平台单相机ACF对位检测）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.146 鑫三力COG/FOG（系统1:单平台两相机ACF对位检测, 系统2：单平台单相机ACF对位检测）
				// 日期：2018.11.15 GGY
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.146.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台两相机ACF对位检测视觉系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（单平台两相机ACF对位检测）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}


					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台单相机ACF对位检测视觉系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（单平台单相机ACF对位检测）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	
					}

					return;
				}
			}
			break;

		case 147: // 6.147 鑫三力5620 COF（四平台六相机预压预压上料上料（平台1：双相机预压XY+D，平台2：双相机预压XY+D，平台3：单相机FPC上料XYD，平台4：单相机FPC上料XYD））
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.147 鑫三力5620 COF（四平台六相机预压预压上料上料（平台1：双相机预压XY+D，平台2：双相机预压XY+D，平台3：单相机FPC上料XYD，平台4：单相机FPC上料XYD））
				// 日期：2018.12.11 GGY
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.147.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1：四平台六相机预压预压上料上料（平台1：双相机预压XY+D，平台2：双相机预压XY+D，平台3：单相机FPC上料XYD，平台4：单相机FPC上料XYD）
					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 4;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 4;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 
						

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息（单相机FPC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 
						

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第4个平台的信息（单相机FPC上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 3;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}
					return;
				}
			}
			break;

		case 148: // 6.148 鑫三力COF FB300（系统1：三平台四相机预压上料上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.148 鑫三力COF FB300（系统1：三平台四相机预压上料上料）
				// 日期：2018.12.11 GGY
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.148.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1：三平台四相机预压上料上料（平台1：双相机预压XY+D，平台2：单相机FPC上料XY，平台3：单相机玻璃上料XYD）
					systemInfo.m_strSystemName = _T("三平台四相机预压上料上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 
						

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息（单相机FPC上料XY）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息（单相机玻璃上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}
					return;
				}
			}
			break;

		case 149: // 6.149 鑫三力FOF FB400（系统1：两平台五相机预压上料、系统2：两平台三相机预压上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.149 鑫三力FOF FB400（系统1：两平台五相机预压上料、系统2：两平台三相机预压上料）
				// 日期：2018.12.11 GGY
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.149.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1：两平台五相机预压上料（平台1：四相机预压XY+D，平台2：单相机玻璃上料XYD）
					systemInfo.m_strSystemName = _T("两平台五相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（四相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息（单相机玻璃上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}


					////////////////////////////////////////////
					// 系统2：两平台三相机预压上料（平台1：两相机预压XY+D，平台2：单相机玻璃上料XYD）
					systemInfo.m_strSystemName = _T("两平台三相机预压上料视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);

						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}
					return;
				}
			}
			break;

		case 150: // 6.150 鑫三力2600 FOG（系统1：三平台四相机预压上料上料）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.150 鑫三力2600 FOG（系统1：三平台四相机预压上料上料）
				// 日期：2018.12.11 GGY
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.150.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1：三平台四相机预压上料上料（平台1：双相机预压XY+D，平台2：单相机玻璃上料XYD，平台3：单相机玻璃上料XYD）
					systemInfo.m_strSystemName = _T("三平台四相机预压上料上料视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压XY+D）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第2个平台的信息（单相机玻璃上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息（单相机玻璃上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;

						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 


						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}
					return;
				}
			}
			break;

		case 151: // 6.151 鑫三力FOG保密打样（系统1：单平台四相机预压、系统2：单平台两相机预压）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.151 鑫三力FOG保密打样（系统1：单平台四相机预压、系统2：单平台两相机预压）
				// 日期：2018.12.13 GGY
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.151.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1：单平台四相机预压（平台1：四相机预压XYD）
					systemInfo.m_strSystemName = _T("单平台四相机预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（四相机预压XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 
						

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}


					////////////////////////////////////////////
					// 系统2：单平台两相机预压（平台1：两相机预压XYD）
					systemInfo.m_strSystemName = _T("单平台两相机预压视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（四相机预压XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = TRUE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix; 

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}
					return;
				}
			}
			break;
		case 152: // 6.152 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XYD）
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.26 鑫三力FOG/COG（双系统双平台全自动视觉对位系统XYD）
				// 系统1：单平台四相机XYD
				// 系统2：单平台双相机XYD
				// 日期：2018.10.17
				//////////////////////////////////////////////////////////////////////////
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.152.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;


					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("单平台四相机预压XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（四相机预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);


					}

					////////////////////////////////////////////
					// 系统2
					systemInfo.m_strSystemName = _T("单平台双相机预压XYD视觉对位系统");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（双相机预压平台）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}

			}
			break;

		case 153: // 6.153 鑫三力上料XYD_XY_XYD
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.153 鑫三力上料
				// 日期：2018.10.17
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.153.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（玻璃上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



						// 第2个平台的信息（COF上料XYD_1）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	



						// 第3个平台的信息（COF上料XYD_2）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}

			}
			break;

		case 154: // 6.154 鑫三力COF冲切机  LD1510A
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.154 鑫三力上料
				// 日期：2018.10.20
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.154.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



						// 第2个平台的信息（上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}

			}
			break;

			//////////////////////////////////////////////////////////////////////////
			// 设备型号为2001，其中155、156、157为前到工序；158、159为后道工序

			// 155、156、157设备型号都是2001
		case 155: // 6.155 鑫三力双相机XYD上料  设备型号：2001
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.155 鑫三力双相机XYD上料
				// 日期：2018.10.25
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.155.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（双相机上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}

			}
			break;


			// 155、156、157设备型号都是2001
		case 156: // 6.156 鑫三力单平台单相机XY上料  设备型号：2001
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.155 鑫三力单相机XY平台上料
				// 日期：2018.10.25
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.156.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（双相机上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}

			}
			break;


			// 155、156、157设备型号都是2001
		case 157: // 6.157 鑫三力四相机XY+D预压  设备型号：2001
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.157 鑫三力四相机XY+D预压
				// 日期：2018.10.25
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.157.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 4;
						sysPlatformInfo.m_nPositionNum			= 4;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = TRUE;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}

			}
			break;

			// 设备型号为2001，158、159为后道工序
		case 158: // 6.158 鑫三力双平台双相机预压
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.158 鑫三力双平台双相机预压
				// 日期：2018.11.06
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.158.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（预压XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



						// 第2个平台的信息（预压XYD）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}

			}
			break;

			// 设备型号为2001，158、159为后道工序
		case 159: // 6.159 鑫三力双平台三相机上料
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.159 鑫三力双平台三相机上料
				// 日期：2018.11.06
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.159.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 2;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 2;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息（玻璃上料XYD）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);		



						// 第2个平台的信息（IC上料XY）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXY;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					return;
				}

			}
			break;
		case 160: // 6.160 鑫三力单平台六相机FOF
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.160 鑫三力单平台六相机FOF
				// 日期：2018.11.12
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.160.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 1;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 1;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 6;
						sysPlatformInfo.m_nPositionNum			= 8;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate=TRUE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);		

					}

					return;
				}

			}
			break;
		case 161: // 6.161 共享相机
			{
				//////////////////////////////////////////////////////////////////////////
				// 6.161 共享相机
				// 日期：2018.11.26
				//////////////////////////////////////////////////////////////////////////

				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.161.0");

					// 系统数量
					m_nSystemNum = 1;

					// 系统信息
					CSystemInfo systemInfo;		


					systemInfo.m_strSystemName = _T("视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;

						// 第1个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate=FALSE;
						sysPlatformInfo.m_bShareCamSystem=TRUE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;
						
						

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);		


						// 第2个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate=FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);


						// 第3个平台的信息
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate=FALSE;

						
						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);
						m_vSysPlatformInfo.push_back(sysPlatformInfo);
					}

					return;
				}

			}
			break;

		case 162:  // 6.162 鑫三力三平台四相机FOP
			{
				{
					// 项目软件版本
					m_strSoftwareProjectVersion = _T("6.162.0");

					// 系统数量
					m_nSystemNum = 2;

					// 系统信息
					CSystemInfo systemInfo;

					////////////////////////////////////////////
					// 系统1
					systemInfo.m_strSystemName = _T("三平台四相机上料预压视觉对位系统");
					systemInfo.m_nSystemIndex = 0;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;


						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（pannel入料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;
	
						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;


						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（FPC入料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;


						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}

					////////////////////////////////////////////
					// 系统2 -----标定方式启用以一个基准为基准的模式
					systemInfo.m_strSystemName = _T("三平台四相机上料预压视觉对位系统-位置1基准标定");
					systemInfo.m_nSystemIndex = 1;
					systemInfo.m_nSystemOffsetIndex = systemInfo.m_nSystemIndex <= 0 ? 0 : (m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemOffsetIndex + m_vSystemInfo[m_vSystemInfo.size() -1].m_nSystemPlatformNum);
					systemInfo.m_nSystemPlatformNum = 3;
					m_vSystemInfo.push_back(systemInfo);
					{
						// 平台数量
						m_nPlatformNum = 3;

						// 平台信息
						SysPlatformInfo sysPlatformInfo;


						// 第1个平台的信息（预压）
						sysPlatformInfo.m_nPlatformIndex = 0;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYPD;
						
						sysPlatformInfo.m_nCamNum				= 2;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetNoMove;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;
						
						sysPlatformInfo.m_bEnableObjectCalibBench = TRUE;
						sysPlatformInfo.m_nObjectCalibBenchIndex = 1;



						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;


						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						
						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（pannel入料）
						sysPlatformInfo.m_nPlatformIndex = 1;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;

						sysPlatformInfo.m_bEnableObjectCalibBench = TRUE;
						sysPlatformInfo.m_nObjectCalibBenchIndex = 1;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;


						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

						// 第2个平台的信息（FPC入料）
						sysPlatformInfo.m_nPlatformIndex = 2;
						sysPlatformInfo.m_ePlatformType			= ePlatformXYD;
						sysPlatformInfo.m_eMidPlatformType		= ePlatformXYD;
						
						sysPlatformInfo.m_nCamNum				= 1;
						sysPlatformInfo.m_nPositionNum			= 2;
						sysPlatformInfo.m_eCamPlatformType		= eCamPlatformSepFix;
						sysPlatformInfo.m_eAlignerTargetType	= eTargetOnline;
						sysPlatformInfo.m_eAlignerTargetMoveType	= eTargetMoveByPlatform;
						sysPlatformInfo.m_bBenchMarkAmend		= FALSE;
						sysPlatformInfo.m_eAlignerObjectType	= eObjectOnline;
						sysPlatformInfo.m_eAlignerObjectMoveType	= eObjectMoveByPlatform;


						sysPlatformInfo.m_bEnableObjectCalibBench = TRUE;
						sysPlatformInfo.m_nObjectCalibBenchIndex = 1;

						sysPlatformInfo.m_bTargetObjectCamSeparate = FALSE;


						sysPlatformInfo.m_bPlatformPick = FALSE;
						sysPlatformInfo.m_ePickCamPlatformType = ePickCamPlatformSepFix;


						sysPlatformInfo.m_bRegisterSuccess = FALSE;

						sysPlatformInfo.m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), sysPlatformInfo.m_nPlatformIndex + systemInfo.m_nSystemOffsetIndex);

						m_vSysPlatformInfo.push_back(sysPlatformInfo);	

					}


					return;
				}
			}
			break;

		default:
			{
				return;
			}
			break;
		}
	}
}
//
//BOOL CSystemConfigure::LoadPlatformInfo( LPCTSTR lpszPlatformName, SysPlatformInfo& sysPlatformInfo )
//{
//	if (lpszPlatformName == NULL)
//	{
//		return FALSE;
//	}
//
//	SysPlatformInfo sysTempPlatformInfo;
//	CString strElem = lpszPlatformName;
//	int nData = 0;
//
//	if (!m_XMLConfigurator.FindElem(strElem))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.EnterElem();
//
//	// 加载 PlatformType
//	if (!m_XMLConfigurator.FindElem(_T("PlatType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_ePlatformType = (PlatformType)nData;
//
//	// 加载 MidPlatformType
//	if (!m_XMLConfigurator.FindElem(_T("MidPlatType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eMidPlatformType = (PlatformType)nData;
//
//	// 加载 CommType
//	if (!m_XMLConfigurator.FindElem(_T("CommType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eCommType = (CommType)nData;
//
//	// 加载 ShareCommEnable
//	if (m_XMLConfigurator.FindElem(_T("ShareCommEnable")))
//	{
//		m_XMLConfigurator.GetElemData(nData);
//		sysTempPlatformInfo.m_bShareCommEnable = (BOOL)nData;
//	}
//
//	// 加载 ShareCommIndex
//	if (m_XMLConfigurator.FindElem(_T("ShareCommIndex")))
//	{
//		m_XMLConfigurator.GetElemData(nData);
//		sysTempPlatformInfo.m_nShareCommIndex = nData;
//	}
//	
//	// 加载 ShareCommPlatformParallel
//	if (m_XMLConfigurator.FindElem(_T("ShareCommPlatformParallel")))
//	{
//		m_XMLConfigurator.GetElemData(nData);
//		sysTempPlatformInfo.m_bShareCommPlatformParallel = (BOOL)nData;
//	}
//
//	// 加载 CameraNum
//	if (!m_XMLConfigurator.FindElem(_T("CamNum")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nCamNum);
//
//	// 加载 PositionNum
//	if (!m_XMLConfigurator.FindElem(_T("PosNum")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nPositionNum);
//
//	// 加载 ObjectPositionNum
//	if (m_XMLConfigurator.FindElem(_T("ObjectPositionNum")))
//	{
//		m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nObjectPositionNum);
//	}
//
//	// 加载 TargetPositionNum
//	if (m_XMLConfigurator.FindElem(_T("TargetPositionNum")))
//	{
//		m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nTargetPositionNum);
//	}
//
//	// 加载 TargetPositionVirtualMode
//	if (m_XMLConfigurator.FindElem(_T("TargetPositionVirtualMode")))
//	{
//		m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nTargetPositionVirtualMode);
//	}
//
//	// 加载 ObjectPositionVirtualMode
//	if (m_XMLConfigurator.FindElem(_T("ObjectPositionVirtualMode")))
//	{
//		m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nObjectPositionVirtualMode);
//	}
//	
//	// 加载 EnableTargetPositionVirtualMode
//	if (m_XMLConfigurator.FindElem(_T("EnableTargetPositionVirtualMode")))
//	{
//		m_XMLConfigurator.GetElemData(nData);
//		sysTempPlatformInfo.m_bEnableTargetPositionVirtualMode = (BOOL)nData;
//	}
//
//	// 加载 EnableObjectPositionVirtualMode
//	if (m_XMLConfigurator.FindElem(_T("EnableObjectPositionVirtualMode")))
//	{
//		m_XMLConfigurator.GetElemData(nData);
//		sysTempPlatformInfo.m_bEnableObjectPositionVirtualMode = (BOOL)nData;
//	}
//
//	// 加载 TargetPositionDirectionMode
//	if (m_XMLConfigurator.FindElem(_T("TargetPositionDirectionMode")))
//	{
//		m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_bTargetPositionDirectionMode);
//	}
//
//	// 加载 ObjectPositionDirectionMode
//	if (m_XMLConfigurator.FindElem(_T("ObjectPositionDirectionMode")))
//	{
//		m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_bObjectPositionDirectionMode);
//	}
//
//	// PositionShowMode
//	if (m_XMLConfigurator.FindElem(_T("PositionShowMode")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nPositionShowMode);	
//	}
//
//    // ShowMode
//    if (m_XMLConfigurator.FindElem(_T("ShowMode")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nShowMode);	
//    }
//
//	// 加载 CamPlatformType
//	if (!m_XMLConfigurator.FindElem(_T("CamPlatType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eCamPlatformType = (CamPlatformType)nData;
//
//	// 加载 AlignerTargetType
//	if (!m_XMLConfigurator.FindElem(_T("AlignerTargetType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eAlignerTargetType = (AlignerTargetType)nData;
//
//	// 加载 AlignerTargetMoveType
//	if (!m_XMLConfigurator.FindElem(_T("AlignerTargetMoveType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eAlignerTargetMoveType = (AlignerTargetMoveType)nData;
//
//	// 加载 BenchMarkAmend
//	if (!m_XMLConfigurator.FindElem(_T("BenchMarkAmend")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_bBenchMarkAmend = (BOOL)nData;
//
//	// 加载 AlignerObjectType
//	if (!m_XMLConfigurator.FindElem(_T("AlignerObjectType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eAlignerObjectType = (AlignerObjectType)nData;
//
//	// 加载 AlignerObjectMoveType
//	if (!m_XMLConfigurator.FindElem(_T("AlignerObjectMoveType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eAlignerObjectMoveType = (AlignerObjectMoveType)nData;
//
//	// 加载 AlignerMode
//	if (!m_XMLConfigurator.FindElem(_T("AlignerMode")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eAlignerMode = (AlignerMode)nData;
//
//    // TargetUseFixedVirtualCoordinates
//    if (m_XMLConfigurator.FindElem(_T("TargetUseFixedVirtualCoordinates")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bTargetUseFixedVirtualCoordinates = (BOOL)nData;
//    }
//
//    // TargetCalibUseBoard
//    if (m_XMLConfigurator.FindElem(_T("TargetCalibUseBoard")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bTargetCalibUseBoard = (BOOL)nData;
//    }
//
//    // 加载 EnableEightShapeAlign
//    if (m_XMLConfigurator.FindElem(_T("EnableEightShapeAlign")))
//    {
//        m_XMLConfigurator.GetElemData(nData);
//        sysTempPlatformInfo.m_bEnableEightShapeAlign = (BOOL)nData;
//    }
//
//	// 加载 CmdParamOrderType
//	if (!m_XMLConfigurator.FindElem(_T("CmdParamOrderType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eCmdParamOrderType = (CmdParamOrderType)nData;
//
//	// 加载 ProtocolType
//	if (!m_XMLConfigurator.FindElem(_T("ProtocolType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eProtocolType = (ProtocolType)nData;
//
//    // ExProtocolType
//    if (m_XMLConfigurator.FindElem(_T("ExProtocolType")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_eExProtocolType = (ExProtocolType)nData;
//    }
//
//	// 加载 RegDataTypeIndex
//	if (!m_XMLConfigurator.FindElem(_T("RegDataTypeIndex")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_nRegDataTypeIndex = nData;
//
//
//	// 加载 MotionControlType
//	if (!m_XMLConfigurator.FindElem(_T("MotionControlType")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_eMotionControlType = (MotionControlType)nData;
//
//    // PlcBaseRegAddr
//    if (m_XMLConfigurator.FindElem(_T("PlcBaseRegAddr")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_nPlcBaseRegAddr = nData;
//    }
//
//    // RegiterType
//    if (m_XMLConfigurator.FindElem(_T("RegiterType")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bRegiterType = (BOOL)nData;
//    }
//
//    // LoadSingleProductSearchToolEnable
//    if (m_XMLConfigurator.FindElem(_T("LoadSingleProductSearchToolEnable")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bLoadSingleProductSearchToolEnable = (BOOL)nData;
//    }
//
//
//	// 加载 TargetObjectCamSeparate
//	if (!m_XMLConfigurator.FindElem(_T("TargetObjectCamSeparate")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_bTargetObjectCamSeparate = (BOOL)nData;
//
//
//	// 加载 TarObjCamSepNoAxisCalib
//	if (!m_XMLConfigurator.FindElem(_T("TarObjCamSepNoAxisCalib")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_bTarObjCamSepNoAxisCalib = (BOOL)nData;
//
//	// 加载 TarObjCamSepAutoCalibTar
//	if (!m_XMLConfigurator.FindElem(_T("TarObjCamSepAutoCalibTar")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_bTarObjCamSepAutoCalibTar = (BOOL)nData;
//	
//	// 加载 TarCamWithDiffObjCam
//	if (!m_XMLConfigurator.FindElem(_T("TarCamWithDiffObjCam")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_bTarCamWithDiffObjCam = (BOOL)nData;
//
//	// 加载 TargetGuanLianUse4Points
//	if (m_XMLConfigurator.FindElem(_T("TargetGuanLianUse4Points")))
//	{
//		m_XMLConfigurator.GetElemData(nData);
//		sysTempPlatformInfo.m_bTargetGuanLianUse4Points = (BOOL)nData;
//	}
//
//	// 加载 UseAlignFAHLex
//	if (m_XMLConfigurator.FindElem(_T("UseAlignFAHLex")))
//	{
//		m_XMLConfigurator.GetElemData(nData);
//		sysTempPlatformInfo.m_bUseAlignFAHLex = (BOOL)nData;
//	}
//
//    // PlatformPick
//    if (m_XMLConfigurator.FindElem(_T("PlatformPick")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bPlatformPick = (BOOL)nData;
//    }
//    //TargetPick
//    if(m_XMLConfigurator.FindElem(_T("TargetPick")))//Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);
//        sysTempPlatformInfo.m_bTargetPick = (BOOL)nData;
//    }
//    // PickCamPlatformType
//    if (m_XMLConfigurator.FindElem(_T("PickCamPlatformType")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_ePickCamPlatformType = (PickCamPlatformType)nData;
//    }
//
//    // DiffPosWithSameCalibInfoEnabled
//    if (m_XMLConfigurator.FindElem(_T("DiffPosWithSameCalibInfoEnabled")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bDiffPosWithSameCalibInfoEnabled = (BOOL)nData;
//    }
//
//    // DiffPosWithSameSearchResultEnabled
//    if (m_XMLConfigurator.FindElem(_T("DiffPosWithSameSearchResultEnabled")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bDiffPosWithSameSearchResultEnabled = (BOOL)nData;
//    }
//
//	// 加载 EnableTargetBench
//	if (!m_XMLConfigurator.FindElem(_T("EnableTargetBench")))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.GetElemData(nData);
//	sysTempPlatformInfo.m_bEnableTargetBench = (BOOL)nData;
//
//    // EnableObjectCalibBench
//    if (m_XMLConfigurator.FindElem(_T("EnableObjectCalibBench")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bEnableObjectCalibBench = (BOOL)nData;
//    }
//
//    // ObjectCalibBenchIndex
//    if (m_XMLConfigurator.FindElem(_T("ObjectCalibBenchIndex")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nObjectCalibBenchIndex);	
//    }					
//
//    // EnableMultiCalibExtension
//    if (m_XMLConfigurator.FindElem(_T("EnableMultiCalibExtension")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bEnableMultiCalibExtension = (BOOL)nData;
//    }
//
//    // MultiCalibExtensionNum
//    if (m_XMLConfigurator.FindElem(_T("MultiCalibExtensionMaxNum")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(sysTempPlatformInfo.m_nMultiCalibExtensionMaxNum);	
//    }
//
//    // InspectEnable
//    if (m_XMLConfigurator.FindElem(_T("InspectEnable")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bInspectEnable = (BOOL)nData;
//    }
//
//    // VirtualObjectEnable
//    if (m_XMLConfigurator.FindElem(_T("VirtualObjectEnable")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bVirtualObjectEnable = (BOOL)nData;
//    }
//
//    // CalibTargetEnable
//    if (m_XMLConfigurator.FindElem(_T("CalibTargetEnable")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bCalibTargetEnable = (BOOL)nData;
//    }
//
//    // CameraHardTriggerMode
//    if (m_XMLConfigurator.FindElem(_T("CameraHardTriggerMode")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bCameraHardTriggerMode = (BOOL)nData;
//    }
//
//    // ShareCamSystem
//    if (m_XMLConfigurator.FindElem(_T("ShareCamSystem")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bShareCamSystem = (BOOL)nData;
//    }
//
//	// ObjectInDifferentComputer
//	if (m_XMLConfigurator.FindElem(_T("ObjectInDifferentComputer")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(nData);	
//		sysTempPlatformInfo.m_bObjectInDifferentComputer = (BOOL)nData;
//	}
//	// TargetInDifferentComputer
//	if (m_XMLConfigurator.FindElem(_T("TargetInDifferentComputer")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(nData);	
//		sysTempPlatformInfo.m_bTargetInDifferentComputer = (BOOL)nData;
//	}
//
//	// CalibSendAbsPos
//	if (m_XMLConfigurator.FindElem(_T("CalibSendAbsPos")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(nData);	
//		sysTempPlatformInfo.m_bCalibSendAbsPos = (BOOL)nData;
//	}
//
//	// CalibSendRelativePos
//	if (m_XMLConfigurator.FindElem(_T("CalibSendRelativePos")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(nData);	
//		sysTempPlatformInfo.m_bCalibSendRelativePos = (BOOL)nData;
//	}
//	
//	// ProductFaChd
//	if (m_XMLConfigurator.FindElem(_T("ProductFaChd")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(nData);	
//		sysTempPlatformInfo.m_bProductFaChd = (BOOL)nData;
//	}
//	// LoadSingleProductSearchToolEnableFaChd
//	if (m_XMLConfigurator.FindElem(_T("LoadSingleProductSearchToolEnableFaChd")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(nData);	
//		sysTempPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd = (BOOL)nData;
//	}
//
//    // CalibrateChangeXDirection
//    if (m_XMLConfigurator.FindElem(_T("CalibrateChangeXDirection")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bCalibrateChangeXDirection = (BOOL)nData;
//    }
//
//    // CalibrateChangeYDirection
//    if (m_XMLConfigurator.FindElem(_T("CalibrateChangeYDirection")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bCalibrateChangeYDirection = (BOOL)nData;
//    }
//
//    // CalibrateChangeDDirection
//    if (m_XMLConfigurator.FindElem(_T("CalibrateChangeDDirection")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bCalibrateChangeDDirection = (BOOL)nData;
//    }
//
//	// EnableDataRecordTime
//	if (m_XMLConfigurator.FindElem(_T("EnableDataRecordTime")))// Level 4
//	{
//		m_XMLConfigurator.GetElemData(nData);	
//		sysTempPlatformInfo.m_bEnableDataRecordTime = (BOOL)nData;
//	}
//
//    // RegisterSuccess
//    if (m_XMLConfigurator.FindElem(_T("RegisterSuccess")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bRegisterSuccess = (BOOL)nData;
//    }
//
//    // UseSpecialComm
//    if (m_XMLConfigurator.FindElem(_T("UseSpecialComm")))// Level 4
//    {
//        m_XMLConfigurator.GetElemData(nData);	
//        sysTempPlatformInfo.m_bUseSpecialComm = (BOOL)nData;
//    }
//
//    // DisplayArrayMode
//    if (m_XMLConfigurator.FindElem("DisplayArrayMode"))// Level 4
//    {
//        m_XMLConfigurator.EnterElem();
//
//        CString strDisplayArrayMode;
//        for (int nDisplayArrayModeIndex=0; nDisplayArrayModeIndex<MAXPOSNUM; nDisplayArrayModeIndex++)	//hk20180130
//        {
//            strDisplayArrayMode.Format(_T("DisplayArrayMode%d"), nDisplayArrayModeIndex);
//            if (m_XMLConfigurator.FindElem(strDisplayArrayMode))// Level 5
//            {
//                m_XMLConfigurator.GetElemData(nData);	
//                sysTempPlatformInfo.m_nDisplayArrayMode[nDisplayArrayModeIndex] = nData;
//            }
//            else
//            {
//                sysTempPlatformInfo.m_nDisplayArrayMode[nDisplayArrayModeIndex] = nDisplayArrayModeIndex;
//            }							
//        }
//
//        m_XMLConfigurator.LeaveElem();
//    }
//
//	// 加载 PlatformDetailSavePath
//	if (!m_XMLConfigurator.FindElem(_T("PlatformDetailSavePath")))
//	{
//		return FALSE;
//	}
//	sysTempPlatformInfo.m_strPlatformDetailSavePath = m_XMLConfigurator.GetElemData();
//
//	m_XMLConfigurator.LeaveElem();
//	sysPlatformInfo = sysTempPlatformInfo;
//	return TRUE;
//}
//
//BOOL CSystemConfigure::SavePlatformInfo( LPCTSTR lpszPlatformName, SysPlatformInfo sysPlatformInfo )
//{
//	if (lpszPlatformName == NULL)
//	{
//		return FALSE;
//	}
//	
//	CString strElem = lpszPlatformName;
//	int nData = 0;
//	if (!m_XMLConfigurator.AddElem(strElem))
//	{
//		return FALSE;
//	}
//	m_XMLConfigurator.EnterElem();
//
//	// 保存 PlatformType
//	if (!m_XMLConfigurator.AddElem(_T("PlatType"), (int)sysPlatformInfo.m_ePlatformType))
//	{
//		return FALSE;
//	}
//
//	// 保存 MidPlatformType
//	if (!m_XMLConfigurator.AddElem(_T("MidPlatType"), (int)sysPlatformInfo.m_eMidPlatformType))
//	{
//		return FALSE;
//	}
//
//	// 保存 CommType
//	if (!m_XMLConfigurator.AddElem(_T("CommType"), (int)sysPlatformInfo.m_eCommType))
//	{
//		return FALSE;
//	}
//
//	// 保存 ShareCommEnable
//	if (!m_XMLConfigurator.AddElem(_T("ShareCommEnable"), (int)sysPlatformInfo.m_bShareCommEnable))
//	{
//		return FALSE;
//	}
//
//	// 保存 ShareCommIndex
//	if (!m_XMLConfigurator.AddElem(_T("ShareCommIndex"), (int)sysPlatformInfo.m_nShareCommIndex))
//	{
//		return FALSE;
//	}
//
//	// 保存 ShareCommPlatformParallel
//	if (!m_XMLConfigurator.AddElem(_T("ShareCommPlatformParallel"), (int)sysPlatformInfo.m_bShareCommPlatformParallel))
//	{
//		return FALSE;
//	}
//
//
//	// 保存 CameraNum
//	if (!m_XMLConfigurator.AddElem(_T("CamNum"), sysPlatformInfo.m_nCamNum))
//	{
//		return FALSE;
//	}
//
//	// 保存 PositionNum
//	if (!m_XMLConfigurator.AddElem(_T("PosNum"), sysPlatformInfo.m_nPositionNum))
//	{
//		return FALSE;
//	}
//
//	// 保存 ObjectPositionNum
//	if (!m_XMLConfigurator.AddElem(_T("ObjectPositionNum"), sysPlatformInfo.m_nObjectPositionNum))
//	{
//		return FALSE;
//	}
//
//	// 保存 TargetPositionNum
//	if (!m_XMLConfigurator.AddElem(_T("TargetPositionNum"), sysPlatformInfo.m_nTargetPositionNum))
//	{
//		return FALSE;
//	}
//
//	// 保存 TargetPositionVirtualMode
//	if (!m_XMLConfigurator.AddElem(_T("TargetPositionVirtualMode"), sysPlatformInfo.m_nTargetPositionVirtualMode))
//	{
//		return FALSE;
//	}
//
//	// 保存 ObjectPositionVirtualMode
//	if (!m_XMLConfigurator.AddElem(_T("ObjectPositionVirtualMode"), sysPlatformInfo.m_nObjectPositionVirtualMode))
//	{
//		return FALSE;
//	}
//
//	// 保存 EnableTargetPositionVirtualMode
//	if (!m_XMLConfigurator.AddElem(_T("EnableTargetPositionVirtualMode"), sysPlatformInfo.m_bEnableTargetPositionVirtualMode))
//	{
//		return FALSE;
//	}
//
//	// 保存 EnableObjectPositionVirtualMode
//	if (!m_XMLConfigurator.AddElem(_T("EnableObjectPositionVirtualMode"), sysPlatformInfo.m_bEnableObjectPositionVirtualMode))
//	{
//		return FALSE;
//	}
//
//	// 保存 TargetPositionDirectionMode
//	if (!m_XMLConfigurator.AddElem(_T("TargetPositionDirectionMode"), sysPlatformInfo.m_bTargetPositionDirectionMode))
//	{
//		return FALSE;
//	}
//
//	// 保存 ObjectPositionDirectionMode
//	if (!m_XMLConfigurator.AddElem(_T("ObjectPositionDirectionMode"), sysPlatformInfo.m_bObjectPositionDirectionMode))
//	{
//		return FALSE;
//	}
//
//    // PositionShowMode
//    if (!m_XMLConfigurator.AddElem(_T("PositionShowMode"), sysPlatformInfo.m_nPositionShowMode))
//    {
//        return FALSE;
//    }
//
//	// ShowMode
//	if (!m_XMLConfigurator.AddElem(_T("ShowMode"), sysPlatformInfo.m_nShowMode))
//	{
//		return FALSE;
//	}
//
//	// 保存 CamPlatformType
//	if (!m_XMLConfigurator.AddElem(_T("CamPlatType"), (int)sysPlatformInfo.m_eCamPlatformType))
//	{
//		return FALSE;
//	}
//
//	// 保存 AlignerTargetType
//	if (!m_XMLConfigurator.AddElem(_T("AlignerTargetType"), (int)sysPlatformInfo.m_eAlignerTargetType))
//	{
//		return FALSE;
//	}
//
//
//	// 保存 AlignerTargetMoveType
//	if (!m_XMLConfigurator.AddElem(_T("AlignerTargetMoveType"), (int)sysPlatformInfo.m_eAlignerTargetMoveType))
//	{
//		return FALSE;
//	}
//
//	// 保存 BenchMarkAmend
//	if (!m_XMLConfigurator.AddElem(_T("BenchMarkAmend"), (int)sysPlatformInfo.m_bBenchMarkAmend))
//	{
//		return FALSE;
//	}
//
//	// 保存 AlignerObjectType
//	if (!m_XMLConfigurator.AddElem(_T("AlignerObjectType"), (int)sysPlatformInfo.m_eAlignerObjectType))
//	{
//		return FALSE;
//	}
//
//	// 保存 AlignerObjectMoveType
//	if (!m_XMLConfigurator.AddElem(_T("AlignerObjectMoveType"), (int)sysPlatformInfo.m_eAlignerObjectMoveType))
//	{
//		return FALSE;
//	}
//
//	// 保存 AlignerMode
//	if (!m_XMLConfigurator.AddElem(_T("AlignerMode"), (int)sysPlatformInfo.m_eAlignerMode))
//	{
//		return FALSE;
//	}
//
//    // TargetCalibUseBoard
//    if (!m_XMLConfigurator.AddElem(_T("TargetCalibUseBoard"), (int)sysPlatformInfo.m_bTargetCalibUseBoard))
//    {
//        return FALSE;
//    }
//
//    // TargetUseFixedVirtualCoordinates
//    if (!m_XMLConfigurator.AddElem(_T("TargetUseFixedVirtualCoordinates"), (int)sysPlatformInfo.m_bTargetUseFixedVirtualCoordinates))
//    {
//        return FALSE;
//    }
//
//    // EnableEightShapeAlign
//    if (!m_XMLConfigurator.AddElem(_T("EnableEightShapeAlign"), (int)sysPlatformInfo.m_bEnableEightShapeAlign))
//    {
//        return FALSE;
//    }
//
//	// 保存 CmdParamOrderType
//	if (!m_XMLConfigurator.AddElem(_T("CmdParamOrderType"), (int)sysPlatformInfo.m_eCmdParamOrderType))
//	{
//		return FALSE;
//	}
//
//	// 保存 ProtocolType
//	if (!m_XMLConfigurator.AddElem(_T("ProtocolType"), (int)sysPlatformInfo.m_eProtocolType))
//	{
//		return FALSE;
//	}
//
//    // ExProtocolType
//    if (!m_XMLConfigurator.AddElem(_T("ExProtocolType"), (int)sysPlatformInfo.m_eExProtocolType))
//    {
//        return FALSE;
//    }
//
//	// 保存 RegDataTypeIndex
//	if (!m_XMLConfigurator.AddElem(_T("RegDataTypeIndex"), sysPlatformInfo.m_nRegDataTypeIndex))
//	{
//		return FALSE;
//	}
//
//	// 保存 MotionControlType
//	if (!m_XMLConfigurator.AddElem(_T("MotionControlType"), (int)sysPlatformInfo.m_eMotionControlType))
//	{
//		return FALSE;
//	}
//	
//    // PlcBaseRegAddr
//    if (!m_XMLConfigurator.AddElem(_T("PlcBaseRegAddr"), sysPlatformInfo.m_nPlcBaseRegAddr))
//    {
//        return FALSE;
//    }
//
//    // RegiterType
//    if (!m_XMLConfigurator.AddElem(_T("RegiterType"), (int)sysPlatformInfo.m_bRegiterType))
//    {
//        return FALSE;
//    }
//
//    // LoadSingleProductSearchToolEnable
//    if (!m_XMLConfigurator.AddElem(_T("LoadSingleProductSearchToolEnable"), (int)sysPlatformInfo.m_bLoadSingleProductSearchToolEnable))
//    {
//        return FALSE;
//    }
//
//	// 保存 TargetObjectCamSeparate
//	if (!m_XMLConfigurator.AddElem(_T("TargetObjectCamSeparate"), (int)sysPlatformInfo.m_bTargetObjectCamSeparate))
//	{
//		return FALSE;
//	}
//
//	// 保存 TarObjCamSepNoAxisCalib
//	if (!m_XMLConfigurator.AddElem(_T("TarObjCamSepNoAxisCalib"), (int)sysPlatformInfo.m_bTarObjCamSepNoAxisCalib))
//	{
//		return FALSE;
//	}
//
//	// 保存 TarObjCamSepAutoCalibTar
//	if (!m_XMLConfigurator.AddElem(_T("TarObjCamSepAutoCalibTar"), (int)sysPlatformInfo.m_bTarObjCamSepAutoCalibTar))
//	{
//		return FALSE;
//	}
//
//	// 保存 TarCamWithDiffObjCam
//	if (!m_XMLConfigurator.AddElem(_T("TarCamWithDiffObjCam"), (int)sysPlatformInfo.m_bTarCamWithDiffObjCam))
//	{
//		return FALSE;
//	}
//
//	// 保存 TargetGuanLianUse4Points
//	if (!m_XMLConfigurator.AddElem(_T("TargetGuanLianUse4Points"), (int)sysPlatformInfo.m_bTargetGuanLianUse4Points))
//	{
//		return FALSE;
//	}
//
//	// 保存 UseAlignFAHLex
//	if (!m_XMLConfigurator.AddElem(_T("UseAlignFAHLex"), (int)sysPlatformInfo.m_bUseAlignFAHLex))
//	{
//		return FALSE;
//	}
//
//    // PlatformPick
//    if (!m_XMLConfigurator.AddElem(_T("PlatformPick"), (int)sysPlatformInfo.m_bPlatformPick))
//    {
//        return FALSE;
//    }
//    
//    if (!m_XMLConfigurator.AddElem(_T("TargetPick"),(int)sysPlatformInfo.m_bTargetPick))
//    {
//        return FALSE;
//    }
//    
//    // PickCamPlatformType
//    if (!m_XMLConfigurator.AddElem(_T("PickCamPlatformType"), (int)sysPlatformInfo.m_ePickCamPlatformType))
//    {
//        return FALSE;
//    }
//
//    // DiffPosWithSameCalibInfoEnabled
//    if (!m_XMLConfigurator.AddElem(_T("DiffPosWithSameCalibInfoEnabled"), (int)sysPlatformInfo.m_bDiffPosWithSameCalibInfoEnabled))
//    {
//        return FALSE;
//    }
//
//    // DiffPosWithSameSearchResultEnabled
//    if (!m_XMLConfigurator.AddElem(_T("DiffPosWithSameSearchResultEnabled"), (int)sysPlatformInfo.m_bDiffPosWithSameSearchResultEnabled))
//    {
//        return FALSE;
//    }
//
//	// 保存 EnableTargetBench
//	if (!m_XMLConfigurator.AddElem(_T("EnableTargetBench"), (int)sysPlatformInfo.m_bEnableTargetBench))
//	{
//		return FALSE;
//	}
//
//    // EnableObjectCalibBench
//    if (!m_XMLConfigurator.AddElem(_T("EnableObjectCalibBench"), (int)sysPlatformInfo.m_bEnableObjectCalibBench))
//    {
//        return FALSE;
//    }
//
//    // ObjectCalibBenchIndex
//    if (!m_XMLConfigurator.AddElem(_T("ObjectCalibBenchIndex"), sysPlatformInfo.m_nObjectCalibBenchIndex))
//    {
//        return FALSE;
//    }
//
//    // EnableMultiCalibExtension
//    if (!m_XMLConfigurator.AddElem(_T("EnableMultiCalibExtension"), (int)sysPlatformInfo.m_bEnableMultiCalibExtension))
//    {
//        return FALSE;
//    }
//
//    // MultiCalibExtensionNum
//    if (!m_XMLConfigurator.AddElem(_T("MultiCalibExtensionMaxNum"), sysPlatformInfo.m_nMultiCalibExtensionMaxNum))
//    {
//        return FALSE;
//    }
//
//    // InspectEnable
//    if (!m_XMLConfigurator.AddElem(_T("InspectEnable"), (int)sysPlatformInfo.m_bInspectEnable))
//    {
//        return FALSE;
//    }
//
//    // VirtualObjectEnable
//    if (!m_XMLConfigurator.AddElem(_T("VirtualObjectEnable"), (int)sysPlatformInfo.m_bVirtualObjectEnable))
//    {
//        return FALSE;
//    }
//
//    // CalibTargetEnable
//    if (!m_XMLConfigurator.AddElem(_T("CalibTargetEnable"), (int)sysPlatformInfo.m_bCalibTargetEnable))
//    {
//        return FALSE;
//    }
//
//    // CameraHardTriggerMode
//    if (!m_XMLConfigurator.AddElem(_T("CameraHardTriggerMode"), (int)sysPlatformInfo.m_bCameraHardTriggerMode))
//    {
//        return FALSE;
//    }
//
//    // ShareCamSystem
//    if (!m_XMLConfigurator.AddElem(_T("ShareCamSystem"), (int)sysPlatformInfo.m_bShareCamSystem))
//    {
//        return FALSE;
//    }
//
//	// ObjectInDifferentComputer
//	if (!m_XMLConfigurator.AddElem(_T("ObjectInDifferentComputer"), (int)sysPlatformInfo.m_bObjectInDifferentComputer))
//	{
//		return FALSE;
//	}
//	// TargetInDifferentComputer
//	if (!m_XMLConfigurator.AddElem(_T("TargetInDifferentComputer"), (int)sysPlatformInfo.m_bTargetInDifferentComputer))
//	{
//		return FALSE;
//	}
//
//	// CalibSendAbsPos
//	if (!m_XMLConfigurator.AddElem(_T("CalibSendAbsPos"), (int)sysPlatformInfo.m_bCalibSendAbsPos))
//	{
//		return FALSE;
//	}
//
//	// CalibSendRelativePos
//	if (!m_XMLConfigurator.AddElem(_T("CalibSendRelativePos"), (int)sysPlatformInfo.m_bCalibSendRelativePos))
//	{
//		return FALSE;
//	}
//	
//	// ProductFaChd
//	if (!m_XMLConfigurator.AddElem(_T("ProductFaChd"), (int)sysPlatformInfo.m_bProductFaChd))// Level 4
//	{
//		return FALSE;
//	}
//
//	// LoadSingleProductSearchToolEnableFaChd
//	if (!m_XMLConfigurator.AddElem(_T("LoadSingleProductSearchToolEnableFaChd"), (int)sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd))
//	{
//		return FALSE;
//	}
//
//    //CalibrateChangeXDirection
//    if (!m_XMLConfigurator.AddElem(_T("CalibrateChangeXDirection"), (int)sysPlatformInfo.m_bCalibrateChangeXDirection))
//    {
//    }
//
//    //CalibrateChangeYDirection
//    if (!m_XMLConfigurator.AddElem(_T("CalibrateChangeYDirection"), (int)sysPlatformInfo.m_bCalibrateChangeYDirection))
//    {
//        return FALSE;
//    }
//
//    //CalibrateChangeDDirection
//    if (!m_XMLConfigurator.AddElem(_T("CalibrateChangeDDirection"), (int)sysPlatformInfo.m_bCalibrateChangeDDirection))
//    {
//        return FALSE;
//    }
//
//	//EnableDataRecordTime
//	if (!m_XMLConfigurator.AddElem(_T("EnableDataRecordTime"), (int)sysPlatformInfo.m_bEnableDataRecordTime))
//	{
//		return FALSE;
//	}
//
//
//
//    // RegisterSuccess
//    if (!m_XMLConfigurator.AddElem(_T("RegisterSuccess"), (int)sysPlatformInfo.m_bRegisterSuccess))
//    {
//        return FALSE;
//    }
//
//    // UseSpecialComm
//    if (!m_XMLConfigurator.AddElem(_T("UseSpecialComm"), (int)sysPlatformInfo.m_bUseSpecialComm))
//    {
//        return FALSE;
//    }
//
//    // DisplayArrayMode
//    if (!m_XMLConfigurator.AddElem("DisplayArrayMode"))
//    {
//        return FALSE;
//    }
//    m_XMLConfigurator.EnterElem();
//    CString strDisplayArrayMode;
//    for (int nDisplayArrayModeIndex=0; nDisplayArrayModeIndex<MAXPOSNUM; nDisplayArrayModeIndex++)	
//    {
//        strDisplayArrayMode.Format(_T("DisplayArrayMode%d"), nDisplayArrayModeIndex);
//        if (!m_XMLConfigurator.AddElem(strDisplayArrayMode, sysPlatformInfo.m_nDisplayArrayMode[nDisplayArrayModeIndex]))
//        {
//            return FALSE;
//        }				
//    }
//    m_XMLConfigurator.LeaveElem();
//
//	// 保存 PlatformDetailSavePath
//	if (!m_XMLConfigurator.AddElem(_T("PlatformDetailSavePath"), sysPlatformInfo.m_strPlatformDetailSavePath))
//	{
//		return FALSE;
//	}
//
//	m_XMLConfigurator.LeaveElem();
//	return TRUE;
//}

BOOL CSystemConfigure::LoadSystemConfigInfo()
{
	return LoadSystemConfigInfo(m_strXMLFilePath);
	/*m_XMLConfigurator.Load(m_strXMLFilePath);

	m_XMLConfigurator.ResetDocElemPos();
	if (!m_XMLConfigurator.FindElem(_T("SysConfig")))
	{
	return FALSE;
	}
	m_XMLConfigurator.EnterElem();
	if (!m_XMLConfigurator.FindElem(_T("WSNum")))
	{
	return FALSE;
	}
	int nPlatformNum = 1;
	m_XMLConfigurator.GetElemData(nPlatformNum);
	if ((nPlatformNum < 1) || (nPlatformNum >= 16))
	{
	return FALSE;
	}
	SetPlatformNum(nPlatformNum);

	CString strPlatformName = _T("");
	BOOL bAllSecceed = TRUE;
	for (int i=0; i<nPlatformNum; i++)
	{
	strPlatformName.Format(_T("WS%d"), i);
	bAllSecceed = bAllSecceed && LoadPlatformInfo(strPlatformName, m_vSysPlatformInfo.at(i));
	}
	return bAllSecceed;*/
}

BOOL CSystemConfigure::SaveSystemConfigInfo()
{
	return SaveSystemConfigInfo(m_strXMLFilePath);
	/*if (m_nPlatformNum != m_vSysPlatformInfo.size())
	{
	return FALSE;
	}

	m_XMLConfigurator.ResetDocElemPos();
	if (m_XMLConfigurator.FindElem(_T("SysConfig")))
	{
	m_XMLConfigurator.DeleteElem();
	}
	m_XMLConfigurator.AddElem(_T("SysConfig"));
	m_XMLConfigurator.EnterElem();

	m_XMLConfigurator.AddElem(_T("WSNum"), m_nPlatformNum);

	CString strPlatformName = _T("");
	BOOL bAllSecceed = TRUE;
	for (int i=0; i<m_nPlatformNum; i++)
	{
	strPlatformName.Format(_T("WS%d"), i);
	bAllSecceed = bAllSecceed && SavePlatformInfo(strPlatformName, m_vSysPlatformInfo.at(i));
	}

	return bAllSecceed && m_XMLConfigurator.Save(m_strXMLFilePath);*/
}

// 加载所有系统配置信息
BOOL CSystemConfigure::LoadSystemConfigInfo(CString strFilePathName)
{


	vcXMLConfigurator m_XMLConfigurator;
	if (!m_XMLConfigurator.Load(strFilePathName))
	{
		return FALSE;
	};

	std::vector<CSystemInfo> tmp_vSystemInfo;
	std::vector<SysPlatformInfo> tmp_vSysPlatformInfo;

	tmp_vSystemInfo.clear();
	tmp_vSysPlatformInfo.clear();

	m_XMLConfigurator.ResetDocElemPos();
	if (!m_XMLConfigurator.FindElem(_T("SysConfig"))) 	// Level 1
	{
		return FALSE;
	}

	m_XMLConfigurator.EnterElem();

	// 软件版本	
	if (!m_XMLConfigurator.FindElem(_T("SoftwareVersion")))// Level 2
	{
		return FALSE;
	}
	m_strSoftwareVersion = m_XMLConfigurator.GetElemData();


	// 标准版本	
	if (!m_XMLConfigurator.FindElem(_T("SoftwareStandardVersion")))// Level 2
	{
		return FALSE;
	}
	m_strSoftwareStandardVersion = m_XMLConfigurator.GetElemData();

	// 项目版本	
	if (!m_XMLConfigurator.FindElem(_T("SoftwareStandardVersion")))// Level 2
	{
		return FALSE;
	}
	m_strSoftwareStandardVersion = m_XMLConfigurator.GetElemData();

	// 系统数量
	if (!m_XMLConfigurator.FindElem(_T("SystemNum")))// Level 2
	{
		return FALSE;
	}
	m_XMLConfigurator.GetElemData(m_nSystemNum);

	// 当前系统序号
	if (!m_XMLConfigurator.FindElem(_T("CurSystemIndex")))// Level 2
	{
		return FALSE;		
	}
	m_XMLConfigurator.GetElemData(m_nCurSystemIndex);

	// 系统
	CString strInfo;
	int nData = 0;
	int nSystemIndex = 0;
	int nPlatformIndex = 0;
	for (nSystemIndex = 0; nSystemIndex < m_nSystemNum; nSystemIndex++)
	{
		CSystemInfo systemInfo;
		// 系统
		strInfo.Format(_T("Sys%d"), nSystemIndex);		// Level 2
		m_XMLConfigurator.AddElem(strInfo);
		if (!m_XMLConfigurator.FindElem(strInfo))// Level 2
		{
			return FALSE;
		}

		m_XMLConfigurator.EnterElem();
		{
			// 系统名称
			if (!m_XMLConfigurator.FindElem(_T("SystemName")))// Level 3
			{
				return FALSE;		
			}
			systemInfo.m_strSystemName = m_XMLConfigurator.GetElemData();

			// 系统编号
			if (!m_XMLConfigurator.FindElem(_T("SystemIndex")))// Level 3
			{
				return FALSE;		
			}
			m_XMLConfigurator.GetElemData(systemInfo.m_nSystemIndex);

			// 系统偏移量编号
			if (!m_XMLConfigurator.FindElem(_T("SystemOffsetIndex")))// Level 3
			{
				return FALSE;		
			}
			m_XMLConfigurator.GetElemData(systemInfo.m_nSystemOffsetIndex);

			// 系统包含的平台数
			if (!m_XMLConfigurator.FindElem(_T("SystemPlatformNum")))// Level 3
			{
				return FALSE;		
			}
			m_XMLConfigurator.GetElemData(systemInfo.m_nSystemPlatformNum);

			// 平台信息
			for (nPlatformIndex = 0; nPlatformIndex < systemInfo.m_nSystemPlatformNum; nPlatformIndex++)
			{
				SysPlatformInfo sysPlatformInfo;

				// 平台
				strInfo.Format(_T("WS%d"), nPlatformIndex);
				if (!m_XMLConfigurator.FindElem(strInfo))// Level 3
				{
					return FALSE;		
				}

				m_XMLConfigurator.EnterElem();
				{
					
					// 平台相关
					{
						// 平台名称
						if (m_XMLConfigurator.FindElem(_T("PlatName")))// Level 4
						{
							sysPlatformInfo.m_strPlatformName = m_XMLConfigurator.GetElemData();	
						}

						// 平台序号
						if (m_XMLConfigurator.FindElem(_T("PlatIndex")))// Level 4
						{
							m_XMLConfigurator.GetElemData(sysPlatformInfo.m_nPlatformIndex);	
						}

						// PlatformType
						if (m_XMLConfigurator.FindElem(_T("PlatType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_ePlatformType = (PlatformType)nData;
						}

						// MidPlatformType
						if (m_XMLConfigurator.FindElem(_T("MidPlatType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_eMidPlatformType = (PlatformType)nData;
						}
					}

					// 通信相关 后续调整到界面设置
					{

						// 加载 ShareCommEnable
						if (m_XMLConfigurator.FindElem(_T("ShareCommEnable")))
						{
							m_XMLConfigurator.GetElemData(nData);
							sysPlatformInfo.m_bShareCommEnable = (BOOL)nData;
						}

						// 加载 ShareCommIndex
						if (m_XMLConfigurator.FindElem(_T("ShareCommIndex")))
						{
							m_XMLConfigurator.GetElemData(nData);
							sysPlatformInfo.m_nShareCommIndex = nData;
						}

						// 加载 ShareCommPlatformParallel
						if (m_XMLConfigurator.FindElem(_T("ShareCommPlatformParallel")))
						{
							m_XMLConfigurator.GetElemData(nData);
							sysPlatformInfo.m_bShareCommPlatformParallel = (BOOL)nData;
						}

						
					}


					// 相机位置相关
					{
						// CameraNum
						if (m_XMLConfigurator.FindElem(_T("CamNum")))// Level 4
						{
							m_XMLConfigurator.GetElemData(sysPlatformInfo.m_nCamNum);	
						}

						// PositionNum
						if (m_XMLConfigurator.FindElem(_T("PosNum")))// Level 4
						{
							m_XMLConfigurator.GetElemData(sysPlatformInfo.m_nPositionNum);	
						}

						// CamPlatformType
						if (m_XMLConfigurator.FindElem(_T("CamPlatType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_eCamPlatformType = (CamPlatformType)nData;
						}

						// CameraHardTriggerMode
						if (m_XMLConfigurator.FindElem(_T("CameraHardTriggerMode")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bCameraHardTriggerMode = (BOOL)nData;
						}

						// ShareCamSystem
						if (m_XMLConfigurator.FindElem(_T("ShareCamSystem")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bShareCamSystem = (BOOL)nData;
						}

					}

					// 对位相关
					{
						// AlignerTargetType
						if (m_XMLConfigurator.FindElem(_T("AlignerTargetType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_eAlignerTargetType = (AlignerTargetType)nData;
						}

						// AlignerTargetMoveType
						if (m_XMLConfigurator.FindElem(_T("AlignerTargetMoveType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_eAlignerTargetMoveType = (AlignerTargetMoveType)nData;
						}

						// BenchMarkAmend
						if (m_XMLConfigurator.FindElem(_T("BenchMarkAmend")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bBenchMarkAmend = (BOOL)nData;
						}

						// AlignerObjectType
						if (m_XMLConfigurator.FindElem(_T("AlignerObjectType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_eAlignerObjectType = (AlignerObjectType)nData;
						}

						// AlignerObjectMoveType
						if (m_XMLConfigurator.FindElem(_T("AlignerObjectMoveType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_eAlignerObjectMoveType = (AlignerObjectMoveType)nData;
						}

						// TargetObjectCamSeparate
						if (m_XMLConfigurator.FindElem(_T("TargetObjectCamSeparate")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bTargetObjectCamSeparate = (BOOL)nData;
						}

						// TargetCalibUseBoard
						if (m_XMLConfigurator.FindElem(_T("TargetCalibUseBoard")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bTargetCalibUseBoard = (BOOL)nData;
						}

						// TarObjCamSepNoAxisCalib
						if (m_XMLConfigurator.FindElem(_T("TarObjCamSepNoAxisCalib")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bTarObjCamSepNoAxisCalib = (BOOL)nData;
						}

						// TarObjCamSepAutoCalibTar
						if (m_XMLConfigurator.FindElem(_T("TarObjCamSepAutoCalibTar")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bTarObjCamSepAutoCalibTar = (BOOL)nData;
						}

						// TarCamWithDiffObjCam
// 						if (m_XMLConfigurator.FindElem(_T("TarCamWithDiffObjCam")))// Level 4
// 						{
// 							m_XMLConfigurator.GetElemData(nData);	
// 							sysPlatformInfo.m_bTarCamWithDiffObjCam = (BOOL)nData;
// 						}

						// PlatformPick
						if (m_XMLConfigurator.FindElem(_T("PlatformPick")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bPlatformPick = (BOOL)nData;
						}
						//TargetPick
						if(m_XMLConfigurator.FindElem(_T("TargetPick")))//Level 4
						{
							m_XMLConfigurator.GetElemData(nData);
							sysPlatformInfo.m_bTargetPick = (BOOL)nData;
						}
						// PickCamPlatformType
						if (m_XMLConfigurator.FindElem(_T("PickCamPlatformType")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_ePickCamPlatformType = (PickCamPlatformType)nData;
						}

// 						// PlatformGlue
// 						if (m_XMLConfigurator.FindElem(_T("PlatformGlue")))// Level 4
// 						{
// 							m_XMLConfigurator.GetElemData(nData);	
// 							sysPlatformInfo.m_bPlatformGlue = (BOOL)nData;
// 						}

// 						// GlueCamPlatformType
// 						if (m_XMLConfigurator.FindElem(_T("GlueCamPlatformType")))// Level 4
// 						{
// 							m_XMLConfigurator.GetElemData(nData);	
// 							sysPlatformInfo.m_eGlueCamPlatformType = (GlueCamPlatformType)nData;
// 						}

						// EnableTargetBench
						if (m_XMLConfigurator.FindElem(_T("EnableTargetBench")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bEnableTargetBench = (BOOL)nData;
						}

						// EnableObjectCalibBench
						if (m_XMLConfigurator.FindElem(_T("EnableObjectCalibBench")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bEnableObjectCalibBench = (BOOL)nData;
						}

						// ObjectCalibBenchIndex
						if (m_XMLConfigurator.FindElem(_T("ObjectCalibBenchIndex")))// Level 4
						{
							m_XMLConfigurator.GetElemData(sysPlatformInfo.m_nObjectCalibBenchIndex);	
						}					

						// EnableMultiCalibExtension
						if (m_XMLConfigurator.FindElem(_T("EnableMultiCalibExtension")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bEnableMultiCalibExtension = (BOOL)nData;
						}

						// MultiCalibExtensionNum
						if (m_XMLConfigurator.FindElem(_T("MultiCalibExtensionMaxNum")))// Level 4
						{
							m_XMLConfigurator.GetElemData(sysPlatformInfo.m_nMultiCalibExtensionMaxNum);	
						}

						// 加载 UseAlignFAHLex
						if (m_XMLConfigurator.FindElem(_T("UseAlignFAHLex")))
						{
							m_XMLConfigurator.GetElemData(nData);
							sysPlatformInfo.m_bUseAlignFAHLex = (BOOL)nData;
						}
					}

					// 产品相关
					{
						// LoadSingleProductSearchToolEnable
						if (m_XMLConfigurator.FindElem(_T("LoadSingleProductSearchToolEnable")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bLoadSingleProductSearchToolEnable = (BOOL)nData;
						}

						// ProductFaChd
						if (m_XMLConfigurator.FindElem(_T("ProductFaChd")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bProductFaChd = (BOOL)nData;
						}

						// LoadSingleProductSearchToolEnableFaChd
						if (m_XMLConfigurator.FindElem(_T("LoadSingleProductSearchToolEnableFaChd")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd = (BOOL)nData;
						}

						// PlatformDetailSavePath
						if (m_XMLConfigurator.FindElem(_T("PlatformDetailSavePath")))// Level 4
						{
							sysPlatformInfo.m_strPlatformDetailSavePath = m_XMLConfigurator.GetElemData();	
						}

					}

					// 显示相关
					{

						// ShowMode
						if (m_XMLConfigurator.FindElem(_T("ShowMode")))// Level 4
						{
							m_XMLConfigurator.GetElemData(sysPlatformInfo.m_nShowMode);	
						}

						// DisplayArrayMode
						if (m_XMLConfigurator.FindElem("DisplayArrayMode"))// Level 4
						{
							m_XMLConfigurator.EnterElem();

							CString strDisplayArrayMode;
							for (int nDisplayArrayModeIndex=0; nDisplayArrayModeIndex<MAXPOSNUM; nDisplayArrayModeIndex++)	//hk20180130
							{
								strDisplayArrayMode.Format(_T("DisplayArrayMode%d"), nDisplayArrayModeIndex);
								if (m_XMLConfigurator.FindElem(strDisplayArrayMode))// Level 5
								{
									m_XMLConfigurator.GetElemData(nData);	
									sysPlatformInfo.m_nDisplayArrayMode[nDisplayArrayModeIndex] = nData;
								}
								else
								{
									sysPlatformInfo.m_nDisplayArrayMode[nDisplayArrayModeIndex] = nDisplayArrayModeIndex;
								}							
							}

							m_XMLConfigurator.LeaveElem();
						}

						// ObjectInDifferentComputer
						if (m_XMLConfigurator.FindElem(_T("ObjectInDifferentComputer")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bObjectInDifferentComputer = (BOOL)nData;
						}
						// TargetInDifferentComputer
						if (m_XMLConfigurator.FindElem(_T("TargetInDifferentComputer")))// Level 4
						{
							m_XMLConfigurator.GetElemData(nData);	
							sysPlatformInfo.m_bTargetInDifferentComputer = (BOOL)nData;
						}
					}

					
					// RegisterSuccess
					if (m_XMLConfigurator.FindElem(_T("RegisterSuccess")))// Level 4
					{
						m_XMLConfigurator.GetElemData(nData);	
						sysPlatformInfo.m_bRegisterSuccess = (BOOL)nData;
					}


				}				
				m_XMLConfigurator.LeaveElem();			
				tmp_vSysPlatformInfo.push_back(sysPlatformInfo);
			}
		}
		m_XMLConfigurator.LeaveElem();
		tmp_vSystemInfo.push_back(systemInfo);
	}

	m_vSysPlatformInfo = tmp_vSysPlatformInfo;
	m_vSystemInfo = tmp_vSystemInfo;
	return TRUE;
}

// 保存所有系统配置信息到指定目录
BOOL CSystemConfigure::SaveSystemConfigInfo(CString strFilePathName)
{
	CString strInfo;
	strInfo             = _T("");

	vcXMLConfigurator m_XMLConfigurator;
	m_XMLConfigurator.SetDoc(_T(""));	

	m_XMLConfigurator.AddElem(_T("SysConfig"));	// Level 1
	m_XMLConfigurator.EnterElem();

	// 软件版本	
	m_XMLConfigurator.AddElem(_T("SoftwareVersion"), GetSoftwareVersion());	// Level 2

	// 标准版本	
	m_XMLConfigurator.AddElem(_T("SoftwareStandardVersion"), m_strSoftwareStandardVersion);	// Level 2

	// 项目版本	
	m_XMLConfigurator.AddElem(_T("SoftwareProjectVersion"), m_strSoftwareProjectVersion);// Level 2

	// 系统数量
	m_XMLConfigurator.AddElem(_T("SystemNum"), m_nSystemNum);// Level 2

	// 当前系统序号
	m_XMLConfigurator.AddElem(_T("CurSystemIndex"), m_nCurSystemIndex);// Level 2


	// 系统
	int nSystemIndex = 0;
	int nPlatformIndex = 0;
	CSystemInfo systemInfo;
	SysPlatformInfo sysPlatformInfo;
	for (nSystemIndex = 0; nSystemIndex < m_nSystemNum && nSystemIndex < m_vSystemInfo.size(); nSystemIndex++)
	{
		// 获取系统信息
		GetSystemInfo(nSystemIndex, systemInfo);	

		// 系统
		strInfo.Format(_T("Sys%d"), nSystemIndex);		// Level 2
		m_XMLConfigurator.AddElem(strInfo);

		m_XMLConfigurator.EnterElem();
		// 系统名称
		m_XMLConfigurator.AddElem(_T("SystemName"), systemInfo.m_strSystemName);// Level 3

		// 系统编号
		m_XMLConfigurator.AddElem(_T("SystemIndex"), systemInfo.m_nSystemIndex);// Level 3

		// 系统偏移量编号
		m_XMLConfigurator.AddElem(_T("SystemOffsetIndex"), systemInfo.m_nSystemOffsetIndex);// Level 3

		// 系统包含的平台数
		m_XMLConfigurator.AddElem(_T("SystemPlatformNum"), systemInfo.m_nSystemPlatformNum);// Level 3


		// 平台信息
		for (nPlatformIndex = 0; nPlatformIndex < systemInfo.m_nSystemPlatformNum; nPlatformIndex++)
		{
			// 获取平台信息
			GetSysPlatformInfo(nSystemIndex, nPlatformIndex, sysPlatformInfo);

			// 平台
			strInfo.Format(_T("WS%d"), nPlatformIndex);							// Level 3
			m_XMLConfigurator.AddElem(strInfo);

			m_XMLConfigurator.EnterElem();

			// 平台相关
			{
				// 平台名称
				m_XMLConfigurator.AddElem(_T("PlatName"), sysPlatformInfo.m_strPlatformName); // Level 4

				// 平台序号
				m_XMLConfigurator.AddElem(_T("PlatIndex"), sysPlatformInfo.m_nPlatformIndex); // Level 4

				// PlatformType
				m_XMLConfigurator.AddElem(_T("PlatType"), (int)sysPlatformInfo.m_ePlatformType); // Level 4


				// MidPlatformType
				m_XMLConfigurator.AddElem(_T("MidPlatType"), (int)sysPlatformInfo.m_eMidPlatformType); // Level 4
			}

			// 通信相关 后续调整到界面设置
			{

				m_XMLConfigurator.AddElem(_T("ShareCommEnable"), (int)sysPlatformInfo.m_bShareCommEnable); // Level 4

				// ShareCommIndex
				m_XMLConfigurator.AddElem(_T("ShareCommIndex"), sysPlatformInfo.m_nShareCommIndex); // Level 4

				// ShareCommPlatformParallel
				m_XMLConfigurator.AddElem(_T("ShareCommPlatformParallel"), (int)sysPlatformInfo.m_bShareCommPlatformParallel); // Level 4

			}


			// 相机位置相关
			{
				// CameraNum
				m_XMLConfigurator.AddElem(_T("CamNum"), sysPlatformInfo.m_nCamNum); // Level 4

				// PositionNum
				m_XMLConfigurator.AddElem(_T("PosNum"), sysPlatformInfo.m_nPositionNum); // Level 4

				// PositionNum
				m_XMLConfigurator.AddElem(_T("PosNum"), sysPlatformInfo.m_nPositionNum); // Level 4

				// CameraHardTriggerMode
				m_XMLConfigurator.AddElem(_T("CameraHardTriggerMode"), (int)sysPlatformInfo.m_bCameraHardTriggerMode); // Level 4

				// ShareCamSystem
				m_XMLConfigurator.AddElem(_T("ShareCamSystem"), (int)sysPlatformInfo.m_bShareCamSystem); // Level 4

			}
			
			// 对位相关
			{
				// AlignerTargetType
				m_XMLConfigurator.AddElem(_T("AlignerTargetType"), (int)sysPlatformInfo.m_eAlignerTargetType); // Level 4


				// AlignerTargetMoveType
				m_XMLConfigurator.AddElem(_T("AlignerTargetMoveType"), (int)sysPlatformInfo.m_eAlignerTargetMoveType); // Level 4

				// BenchMarkAmend
				m_XMLConfigurator.AddElem(_T("BenchMarkAmend"), (int)sysPlatformInfo.m_bBenchMarkAmend); // Level 4

				// AlignerObjectType
				m_XMLConfigurator.AddElem(_T("AlignerObjectType"), (int)sysPlatformInfo.m_eAlignerObjectType); // Level 4

				// AlignerObjectMoveType
				m_XMLConfigurator.AddElem(_T("AlignerObjectMoveType"), (int)sysPlatformInfo.m_eAlignerObjectMoveType); // Level 4
				
				// TargetObjectCamSeparate
				m_XMLConfigurator.AddElem(_T("TargetObjectCamSeparate"), (int)sysPlatformInfo.m_bTargetObjectCamSeparate); // Level 4

				// TargetCalibUseBoard
				m_XMLConfigurator.AddElem(_T("TargetCalibUseBoard"), (int)sysPlatformInfo.m_bTargetCalibUseBoard); // Level 4

				// TarObjCamSepNoAxisCalib
				m_XMLConfigurator.AddElem(_T("TarObjCamSepNoAxisCalib"), (int)sysPlatformInfo.m_bTarObjCamSepNoAxisCalib); // Level 4

				// TarObjCamSepAutoCalibTar
				m_XMLConfigurator.AddElem(_T("TarObjCamSepAutoCalibTar"), (int)sysPlatformInfo.m_bTarObjCamSepAutoCalibTar); // Level 4

				// TarCamWithDiffObjCam
//				m_XMLConfigurator.AddElem(_T("TarCamWithDiffObjCam"), (int)sysPlatformInfo.m_bTarCamWithDiffObjCam); // Level 4

				// PlatformPick
				m_XMLConfigurator.AddElem(_T("PlatformPick"), (int)sysPlatformInfo.m_bPlatformPick); // Level 4
				m_XMLConfigurator.AddElem(_T("TargetPick"),(int)sysPlatformInfo.m_bTargetPick);
				// PickCamPlatformType
				m_XMLConfigurator.AddElem(_T("PickCamPlatformType"), (int)sysPlatformInfo.m_ePickCamPlatformType); // Level 4

// 				// PlatformGlue
// 				m_XMLConfigurator.AddElem(_T("PlatformGlue"), (int)sysPlatformInfo.m_bPlatformGlue); // Level 4
// 				// GlueCamPlatformType
// 				m_XMLConfigurator.AddElem(_T("GlueCamPlatformType"), (int)sysPlatformInfo.m_eGlueCamPlatformType); // Level 4

				// EnableTargetBench
				m_XMLConfigurator.AddElem(_T("EnableTargetBench"), (int)sysPlatformInfo.m_bEnableTargetBench); // Level 4

				// EnableObjectCalibBench
				m_XMLConfigurator.AddElem(_T("EnableObjectCalibBench"), (int)sysPlatformInfo.m_bEnableObjectCalibBench); // Level 4

				// ObjectCalibBenchIndex
				m_XMLConfigurator.AddElem(_T("ObjectCalibBenchIndex"), sysPlatformInfo.m_nObjectCalibBenchIndex); // Level 4

				// EnableMultiCalibExtension
				m_XMLConfigurator.AddElem(_T("EnableMultiCalibExtension"), (int)sysPlatformInfo.m_bEnableMultiCalibExtension); // Level 4

				// MultiCalibExtensionNum
				m_XMLConfigurator.AddElem(_T("MultiCalibExtensionMaxNum"), sysPlatformInfo.m_nMultiCalibExtensionMaxNum); // Level 4

				// UseAlignFAHLex
				m_XMLConfigurator.AddElem(_T("UseAlignFAHLex"), (int)sysPlatformInfo.m_bUseAlignFAHLex); // Level 4
			}
			
			// 产品相关
			{
				// LoadSingleProductSearchToolEnable
				m_XMLConfigurator.AddElem(_T("LoadSingleProductSearchToolEnable"), (int)sysPlatformInfo.m_bLoadSingleProductSearchToolEnable); // Level 4
				// ProductFaChd
				m_XMLConfigurator.AddElem(_T("ProductFaChd"), (int)sysPlatformInfo.m_bProductFaChd); // Level 4
				// LoadSingleProductSearchToolEnableFaChd
				m_XMLConfigurator.AddElem(_T("LoadSingleProductSearchToolEnableFaChd"), (int)sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd); // Level 4
				// PlatformDetailSavePath
				m_XMLConfigurator.AddElem(_T("PlatformDetailSavePath"), sysPlatformInfo.m_strPlatformDetailSavePath); // Level 4
			}

		   // 显示相关
			{

				// ShowMode
				m_XMLConfigurator.AddElem(_T("ShowMode"), sysPlatformInfo.m_nShowMode); // Level 4

				// DisplayArrayMode
				m_XMLConfigurator.AddElem("DisplayArrayMode");// Level 4
				m_XMLConfigurator.EnterElem();
				CString strDisplayArrayMode;
				for (int nDisplayArrayModeIndex=0; nDisplayArrayModeIndex<MAXPOSNUM; nDisplayArrayModeIndex++)	
				{
					strDisplayArrayMode.Format(_T("DisplayArrayMode%d"), nDisplayArrayModeIndex);
					m_XMLConfigurator.AddElem(strDisplayArrayMode, sysPlatformInfo.m_nDisplayArrayMode[nDisplayArrayModeIndex]);// Level 5					
				}
				m_XMLConfigurator.LeaveElem();


				// ObjectInDifferentComputer
				m_XMLConfigurator.AddElem(_T("ObjectInDifferentComputer"), (int)sysPlatformInfo.m_bObjectInDifferentComputer); // Level 4

				// TargetInDifferentComputer
				m_XMLConfigurator.AddElem(_T("TargetInDifferentComputer"), (int)sysPlatformInfo.m_bTargetInDifferentComputer); // Level 4

			}
			
			// RegisterSuccess
			m_XMLConfigurator.AddElem(_T("RegisterSuccess"), (int)sysPlatformInfo.m_bRegisterSuccess); // Level 4


			m_XMLConfigurator.LeaveElem();
		}

		m_XMLConfigurator.LeaveElem();

	}
	m_XMLConfigurator.LeaveElem();

	BOOL bRetVal = m_XMLConfigurator.Save(strFilePathName);
	return bRetVal;
}

BOOL CSystemConfigure::GetSysPlatformInfo( int nIndex, SysPlatformInfo& sysPlatformInfo )
{
	if (m_nCurSystemIndex < 0 ||m_nCurSystemIndex >= m_vSystemInfo.size())
	{
		m_nCurSystemIndex = 0;
	}

	int nSize = m_vSysPlatformInfo.size();
	if ((nIndex < 0) || (nIndex >= nSize))
	{
		return FALSE;
	}
	else
	{
		if ((nIndex < 0) || (nIndex >= m_vSystemInfo[m_nCurSystemIndex].m_nSystemPlatformNum))
		{
			return FALSE;
		}
		sysPlatformInfo = m_vSysPlatformInfo.at(m_vSystemInfo[m_nCurSystemIndex].m_nSystemOffsetIndex + nIndex);
		return TRUE;
	}


// 	int nSize = m_vSysPlatformInfo.size();
// 	if ((nIndex < 0) || (nIndex >= nSize))
// 	{
// 		return FALSE;
// 	}
// 	else
// 	{
// 		sysPlatformInfo = m_vSysPlatformInfo.at(nIndex);
// 		return TRUE;
// 	}
}

BOOL CSystemConfigure::SetSysPlatformInfo( int nIndex, SysPlatformInfo sysPlatformInfo )
{
// 	int nSize = m_vSysPlatformInfo.size();
// 	if ((nIndex < 0) || (nIndex >= nSize))
// 	{
// 		return FALSE;
// 	}
// 	else
// 	{
// 		m_vSysPlatformInfo.at(nIndex) = sysPlatformInfo;
// 		return TRUE;
// 	}

	if (m_nCurSystemIndex < 0 ||m_nCurSystemIndex >= m_vSystemInfo.size())
	{
		m_nCurSystemIndex = 0;
	}

	int nSize = m_vSysPlatformInfo.size();
	if ((nIndex < 0) || (nIndex >= nSize))
	{
		return FALSE;
	}
	else
	{
		if ((nIndex < 0) || (nIndex >= m_vSystemInfo[m_nCurSystemIndex].m_nSystemPlatformNum))
		{
			return FALSE;
		}
		m_vSysPlatformInfo.at(m_vSystemInfo[m_nCurSystemIndex].m_nSystemOffsetIndex + nIndex) = sysPlatformInfo;
		return TRUE;
	}
}

// 获取指定系统的指定平台配置信息
BOOL CSystemConfigure::GetSysPlatformInfo(int nSystemIndex, int nPlatformIndex, SysPlatformInfo& sysPlatformInfo)
{
	if (nSystemIndex < 0 || nSystemIndex >= m_vSystemInfo.size())
	{
		return FALSE;
	}

	if (nPlatformIndex < 0 || m_vSystemInfo.at(nSystemIndex).m_nSystemOffsetIndex + nPlatformIndex >= m_vSysPlatformInfo.size())
	{
		return FALSE;
	}

	sysPlatformInfo = m_vSysPlatformInfo.at(m_vSystemInfo.at(nSystemIndex).m_nSystemOffsetIndex + nPlatformIndex);

	return TRUE;
}


// 获取平台数
int CSystemConfigure::GetPlatformNum()
{
	if (m_nCurSystemIndex < 0 ||m_nCurSystemIndex >= m_vSystemInfo.size())
	{
		m_nCurSystemIndex = 0;
	}
	
	int nPlatformNum = 0;
	nPlatformNum = m_vSystemInfo[m_nCurSystemIndex].m_nSystemPlatformNum;


	return nPlatformNum;
}

void CSystemConfigure::SetPlatformNum( int nNum )
{
	if ((nNum < 0) || (nNum >= 16))
	{
		return;
	}
	int nOldNum = m_vSysPlatformInfo.size();
	m_vSysPlatformInfo.resize(nNum);
	m_nPlatformNum = nNum;

	if (nNum > nOldNum)
	{
		for (int i=nOldNum; i<nNum; i++)
		{
			m_vSysPlatformInfo.at(i).m_strPlatformDetailSavePath.Format(_T("\\VSConfig\\WS%d"), i); 
		}
	}
}
