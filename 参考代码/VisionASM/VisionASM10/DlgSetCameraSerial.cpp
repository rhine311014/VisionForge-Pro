// DlgSetCameraSerial.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSetCameraSerial.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
#include "vsBaseVisionAlign.h"
//#include "vsGrab.h"
#include "vsCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCameraSerial dialog


CDlgSetCameraSerial::CDlgSetCameraSerial(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetCameraSerial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetCameraSerial)
	m_strHelpInfo = _T("");
	m_nPlatformIndex = 0;
	//}}AFX_DATA_INIT
	m_pParent = (CVisionASMDlg *)pParent;
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_nPrevPlatformIndex = -1;
	m_paulOldSN = NULL;
	m_pvdwInitErrCode = NULL;

	m_bNotEnterApp = FALSE; // Mali 2013.9.18

	m_pGuiDisplay = NULL;
	m_pCamera = NULL;

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;

	InitializeCriticalSection(&m_csResource);
}


CDlgSetCameraSerial::~CDlgSetCameraSerial()
{
	if (m_paulOldSN != NULL)
	{
		delete[] m_paulOldSN;
		m_paulOldSN = NULL;
	}

	if (m_paulOldPosInfo != NULL)
	{
		delete[] m_paulOldPosInfo;
		m_paulOldPosInfo = NULL;
	}

	DeleteCriticalSection(&m_csResource);
}

void CDlgSetCameraSerial::SetInitErrorCode( std::vector<DWORD>* pvdwInitErrCode )
{
	m_pvdwInitErrCode = pvdwInitErrCode;
}

 //配置相机共享方式
BOOL CDlgSetCameraSerial::ConfigShareCamInfo(CArray<CString, CString> *paulSN)
{
	
	if (m_nPlatformNum==1)
	{
		
		return TRUE;
	}
	
	std::vector<std::vector<CShareCamType>> vAllCameraShareInfo;
	vAllCameraShareInfo.resize(m_nPlatformNum);
	for (int i=0;i<m_nPlatformNum;i++)
	{
		int nCamNum=m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
		vAllCameraShareInfo.at(i).resize(nCamNum);
		for (int k=0;k<nCamNum;k++)
		{
			vAllCameraShareInfo.at(i).at(k)=eNoShare;
		}
	}
	std::vector<CShareCamInfo> vShareCamInfo;
	for (int i=0;i<m_nPlatformNum;i++)
	{
		int nCamNum = m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
		for (int j=0;j<nCamNum;j++)
		{
			vShareCamInfo.clear();
			if (vAllCameraShareInfo.at(i).at(j)==eShareTar)
			{
				continue;
			}
			CShareCamInfo shareCamInfo;
			shareCamInfo.m_nPlatformIndex=i;
			shareCamInfo.m_nCamIndex=j;
			vShareCamInfo.push_back(shareCamInfo);
			for (int k=i+1;k<m_nPlatformNum;k++)
			{
				for (int n=0;n<m_pParent->m_vpVisionASM.at(k)->GetCameraNum();n++)
				{
					if (paulSN[i].GetAt(j)==paulSN[k].GetAt(n))
					{
						shareCamInfo.m_nPlatformIndex=k;
						shareCamInfo.m_nCamIndex=n;
						vShareCamInfo.push_back(shareCamInfo);
						vAllCameraShareInfo.at(k).at(n)=eShareTar;
						break;
					}
				}
			}
			if (vShareCamInfo.size()>1)
			{
				
				CameraParam camParam;

				m_pParent->m_vpVisionASM.at(i)->GetCameraParam(j, camParam);
				camParam.m_eShareCamType=eShareSrc;
				camParam.m_nShareCamNum=vShareCamInfo.size()-1;
				camParam.m_vShareCamInfo.clear();
				for (int g=1;g<vShareCamInfo.size();g++)
				{
					camParam.m_vShareCamInfo.push_back(vShareCamInfo.at(g));
				}
				m_pParent->m_vpVisionASM.at(i)->SetCameraParam(j, camParam);

				
				for (int m=1;m<vShareCamInfo.size();m++)
				{
					int nPlatIndex=vShareCamInfo.at(m).m_nPlatformIndex;
					int nCamIdx=vShareCamInfo.at(m).m_nCamIndex;
					m_pParent->m_vpVisionASM.at(nPlatIndex)->GetCameraParam(nCamIdx, camParam);
					
					camParam.m_eShareCamType=eShareTar;
					camParam.m_nShareCamNum=1;
					camParam.m_vShareCamInfo.clear();
					camParam.m_vShareCamInfo.push_back(vShareCamInfo.at(0));
					
					m_pParent->m_vpVisionASM.at(nPlatIndex)->SetCameraParam(nCamIdx, camParam);
				}
				
			}
			else
			{
				CameraParam camParam;

				m_pParent->m_vpVisionASM.at(i)->GetCameraParam(j, camParam);
				camParam.m_eShareCamType=eNoShare;
				camParam.m_nShareCamNum=0;
				camParam.m_vShareCamInfo.clear();
				m_pParent->m_vpVisionASM.at(i)->SetCameraParam(j, camParam);
			}
		}
	}
	return TRUE;
}
BOOL CDlgSetCameraSerial::SaveAndInvalidateAll()
{
	CameraParam camParam;
	int i = 0, j = 0;
	int nCamNum = 0;
	int nPosNum = 0;
	CString strHelpInfo, strPlatform, strCamera, strInfo;
	BOOL bSucceed = TRUE;

	UpdateData(TRUE);
	// 检查各平台各相机通道序列号是否重复
	//CArray<unsigned long, unsigned long> *paulSN = NULL;
	CArray<CString, CString> *paulSN = NULL;
	CString strNewSN;
	int nMinValidSNIdx = (m_aulBusCamSN.GetSize() > 0) ? 0 : 1;
	int nCurSel = -1;
	//paulSN = new CArray<unsigned long, unsigned long>[m_nPlatformNum];
	paulSN = new CArray<CString, CString>[m_nPlatformNum];
	for (j=0; j<m_nPlatformNum; j++)
	{
		nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
		paulSN[j].SetSize(nCamNum);
		strCamera.Empty();
		for(i=0; i<nCamNum; i++)
		{
			if (j == m_nPlatformIndex)
			{
				m_pParent->m_vpVisionASM.at(j)->GetCameraParam(i, camParam);
				nCurSel = m_vpcmbNewSN.at(i)->GetCurSel();
				if (nCurSel >= nMinValidSNIdx)
				{
					m_vpcmbNewSN.at(i)->GetLBText(nCurSel, strNewSN);
				}
				else
				{
					strNewSN.Empty();
				}
				//camParam.m_lSerialNum = _tcstoul(strNewSN, NULL, 10);
				camParam.m_strDeviceID = strNewSN;
				m_pParent->m_vpVisionASM.at(j)->SetCameraParam(i, camParam);
			}
			else
			{
				m_pParent->m_vpVisionASM.at(j)->GetCameraParam(i, camParam);
			}
			//paulSN[j].SetAt(i, camParam.m_lSerialNum);
			paulSN[j].SetAt(i, camParam.m_strDeviceID);
		}
	}

	// 通过检查总线上所有相机被选中的次数来检查各平台各相机通道序列号是否重复
	int nBusCamNum = m_aulBusCamSN.GetSize();
	if (nBusCamNum < 1)
	{
		m_strHelpInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STC_NO_CAMERA_AVAILABLE);	//_T("没有任何可用相机！");
		
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = m_strHelpInfo;
		UpdateStatusBar();

		delete[] paulSN;
		paulSN = NULL;
		return FALSE;
	}
	if (IsShareCameraSystem())
	{
		int nBusCamNum = m_aulBusCamSN.GetSize();

		CString strHelpInfo;
		for (int n=0; n<nBusCamNum; n++)
		{
			for (int j=0; j<m_nPlatformNum; j++)
			{
				int nSameCnt = 0;
				CString strInfo;
				CString strTemp;
				CString strCamera;
				int nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
				for (int i=0; i<nCamNum; i++)
				{				
					if (paulSN[j].GetAt(i) == m_aulBusCamSN.GetAt(n))
					{
						nSameCnt++;
						strTemp.Format(_T("CH%d、"), i+1);
						strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), j+1, strTemp);
						strCamera += strInfo;
					}
				}
				if ((nSameCnt > 1) && !strCamera.IsEmpty())
				{
					strCamera = strCamera.Left(strCamera.GetLength()-2);
					strHelpInfo += strCamera + GetSysInfoString(m_psaSysInfoStrings,IDS_STC_CAMERA_SN_SAME) + _T("\n"); //_T("相机序列号相同！\n")
				}

			}

		}
		if (!strHelpInfo.IsEmpty())
		{
			m_strHelpInfo = strHelpInfo;
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = m_strHelpInfo;
			UpdateStatusBar();
			delete[] paulSN;
			paulSN = NULL;
			
			return FALSE;
		}

		BOOL bIsDestroyed = FALSE;
		for (j=0; j<m_nPlatformNum; j++)
		{
			int nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
			for(i=0; i<nCamNum; i++)
			{
				CameraParam camParam=m_vPreCameraParam[j].at(i);
				int bShareTarget=FALSE;
				if (camParam.m_eShareCamType==eShareTar)
				{
					bShareTarget=TRUE;
				}
				else
				{
					bShareTarget=FALSE;
				}
				bIsDestroyed = m_pParent->m_vpVisionASM.at(j)->DestroyPreShareCamera(i,bShareTarget);
				ASSERT(bIsDestroyed);
				m_pParent->m_vpVisionASM.at(j)->UninitPreShareCamera(i,bShareTarget);
			}
		}

		
		if (!ConfigShareCamInfo(paulSN))
		{
			delete[] paulSN;
			paulSN = NULL;
			
			return FALSE;
		}
		for (int i=0;i<m_nPlatformNum;i++)
		{
			m_pParent->m_vpVisionASM.at(i)->ReCreateInitCameras();

		}
	}
	else
	{
		int nSameCnt = 0;
		CString strTemp;
		for (int n=0; n<nBusCamNum; n++)
		{
			nSameCnt = 0;
			strCamera.Empty();
			for (j=0; j<m_nPlatformNum; j++)
			{
				nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
				for (i=0; i<nCamNum; i++)
				{				
					if (paulSN[j].GetAt(i) == m_aulBusCamSN.GetAt(n))
					{
						nSameCnt++;
						strTemp.Format(_T("CH%d、"), i+1);
						if (m_nPlatformNum == 1)
						{
							strInfo = strTemp;
						}
						else
						{
							strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), j+1, strTemp);
						}
						strCamera += strInfo;
					}
				}
			}
			if ((nSameCnt > 1) && !strCamera.IsEmpty())
			{
				strCamera = strCamera.Left(strCamera.GetLength()-2);
				strHelpInfo += strCamera + GetSysInfoString(m_psaSysInfoStrings,IDS_STC_CAMERA_SN_SAME) + _T("\n"); //_T("相机序列号相同！\n")
			}
		}
		if (!strHelpInfo.IsEmpty())
		{
			m_strHelpInfo = strHelpInfo;
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = m_strHelpInfo;
			UpdateStatusBar();
			delete[] paulSN;
			paulSN = NULL;
			
			return FALSE;
		}
	}
	
	
	delete[] paulSN;
	paulSN = NULL;



	// 初始化所有平台的所有相机
	// 1. 所有scPTGrab对象与相机分离
	BOOL bDestroyed = FALSE;
	for (j=0; j<m_nPlatformNum; j++)
	{
		nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
		for(i=0; i<nCamNum; i++)
		{
			bDestroyed = m_pParent->m_vpVisionASM.at(j)->DestroyCamera(i);
			ASSERT(bDestroyed);
		}
	}
	
	for (j=0; j<m_nPlatformNum; j++)
	{
		nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
		strCamera.Empty();
		BOOL bInitCamFailed = FALSE, bInitCamParamFailed = FALSE;
		for(i=0; i<nCamNum; i++)
		{
			if (!m_pParent->m_vpVisionASM.at(j)->InitCamera(i))
			{
				bInitCamFailed = TRUE;
				m_pvdwInitErrCode->at(j) |= (0x00000001<<i);
				strInfo.Format(_T("CH%d、"), i+1);
				strCamera += strInfo;
			}
			else 
			{
				if (!m_pParent->m_vpVisionASM.at(j)->UpdateCameraStatus(i))
				{
					bInitCamParamFailed = TRUE;
					m_pvdwInitErrCode->at(j) |= (0x00000001<<i);
				}
				else
				{
					m_pvdwInitErrCode->at(j) &= ~(0x00000001<<i);
				}
			}
		}
		// 修改错误状态
		if (bInitCamFailed)
		{
			m_pvdwInitErrCode->at(j) |= INIT_CAMERA_FAILED;
		}
		else
		{
			m_pvdwInitErrCode->at(j) &= ~INIT_CAMERA_FAILED;
		}
		if (bInitCamParamFailed)
		{
			m_pvdwInitErrCode->at(j) |= INIT_CAMERA_PARAM_FAILED;
		}
		else
		{
			m_pvdwInitErrCode->at(j) &= ~INIT_CAMERA_PARAM_FAILED;
		}
		if (!bInitCamFailed)
		{
			m_pParent->m_vpVisionASM.at(j)->SaveCameraInfo();
		}
		
		if (!strCamera.IsEmpty())
		{
			strCamera = strCamera.Left(strCamera.GetLength()-2);
			if (m_nPlatformNum == 1)
			{
				strPlatform.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), strCamera, GetSysInfoString(m_psaSysInfoStrings,IDS_STC_INIT_FAILED));
			}
			else
			{
				strPlatform.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_CAMERA_SSS_SSS), j+1, strCamera, GetSysInfoString(m_psaSysInfoStrings,IDS_STC_INIT_FAILED));// 平台号从1开始
			}
			strHelpInfo += strPlatform + _T("\n");	// 初始化失败
		}
	}

	// 当前平台位置信息
	CPosInfo posInfo;
	{
		nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosNum();
		for (i=0; i<nPosNum; i++)
		{
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosInfo(i, posInfo);
			int nCurSel = m_vpcmbNewPosCamIndex.at(i)->GetCurSel();
			int bEnable = *m_vpbPosEnable.at(i);

			posInfo.m_nCamIndex = nCurSel;
			posInfo.m_bPosEnable = bEnable;			
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPosInfo(i, posInfo);		
		}
	}

	for (j=0; j<m_nPlatformNum; j++)
	{
		m_pParent->m_vpVisionASM.at(j)->SavePosInfo();
	}

	if (!strHelpInfo.IsEmpty())
	{
		m_strHelpInfo = strHelpInfo;		
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = m_strHelpInfo;
		UpdateStatusBar();
		
		// 修复部分相机初始化失败此时 保存退出，初始化成功的相机已被m_vpVisionASM占用，然后进行采集时，之前成功的相机也无法采集bug

		for (j=0; j<m_nPlatformNum; j++)
		{
			nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
			for(i=0; i<nCamNum; i++)
			{
				bDestroyed = m_pParent->m_vpVisionASM.at(j)->DestroyCamera(i);
				ASSERT(bDestroyed);
			}
		}

		return FALSE;
	}
	return TRUE;
}

void CDlgSetCameraSerial::UpdateDialogLanguage()
{
//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));
	
	vcXMLConfigurator xml;
	CString strXMLPath;
	
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle")); 
	xml.EnterElem();
	
	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));
		
		if(_T("IDD_DLG_SET_CAMERA_SERIAL") == strDlgID)
		{
			int nID;
			CString strValue = xml.GetAttribute(_T("Value"));
			SetWindowText(strValue);
			
			xml.EnterElem();	   
			while(xml.FindElem())
			{
				xml.GetAttribute(_T("Id"),  nID);
				strValue = xml.GetAttribute(_T("Value"));
				
				if(GetDlgItem(nID) != NULL)
				{
					GetDlgItem(nID)->SetWindowText(strValue);
				}	  
			}
			xml.LeaveElem();
		}  
	}
	xml.LeaveElem();	
}

 //判断软件系统是否是相机共享系统
BOOL  CDlgSetCameraSerial::IsShareCameraSystem()
{
	BOOL bShareCam=FALSE;
	SysPlatformInfo platformInfo;
	for (int i=0;i<m_pParent->m_vpVisionASM.size();i++)
	{
		m_pParent->m_vpVisionASM.at(i)->GetSysPlatformInfo(platformInfo);
		if (platformInfo.m_bShareCamSystem)
		{
			bShareCam=TRUE;
			break;
		}
	}
	return bShareCam;
}
void CDlgSetCameraSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetCameraSerial)
	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Text(pDX, IDC_EDIT_CH0_OLD_SERIAL_NUMBER, m_strOldSN0);
	DDX_Text(pDX, IDC_EDIT_CH1_OLD_SERIAL_NUMBER, m_strOldSN1);
	DDX_Text(pDX, IDC_EDIT_CH2_OLD_SERIAL_NUMBER, m_strOldSN2);
	DDX_Text(pDX, IDC_EDIT_CH3_OLD_SERIAL_NUMBER, m_strOldSN3);
	DDX_Text(pDX, IDC_EDIT_CH4_OLD_SERIAL_NUMBER, m_strOldSN4);
	DDX_Text(pDX, IDC_EDIT_CH5_OLD_SERIAL_NUMBER, m_strOldSN5);
	DDX_Text(pDX, IDC_EDIT_CH6_OLD_SERIAL_NUMBER, m_strOldSN6);
	DDX_Text(pDX, IDC_EDIT_CH7_OLD_SERIAL_NUMBER, m_strOldSN7);
	DDX_Text(pDX, IDC_EDIT_POS0_OLD_CAM_INDEX, m_strOldPosCamIndex0);
	DDX_Text(pDX, IDC_EDIT_POS1_OLD_CAM_INDEX, m_strOldPosCamIndex1);
	DDX_Text(pDX, IDC_EDIT_POS2_OLD_CAM_INDEX, m_strOldPosCamIndex2);
	DDX_Text(pDX, IDC_EDIT_POS3_OLD_CAM_INDEX, m_strOldPosCamIndex3);
	DDX_Text(pDX, IDC_EDIT_POS4_OLD_CAM_INDEX, m_strOldPosCamIndex4);
	DDX_Text(pDX, IDC_EDIT_POS5_OLD_CAM_INDEX, m_strOldPosCamIndex5);
	DDX_Text(pDX, IDC_EDIT_POS6_OLD_CAM_INDEX, m_strOldPosCamIndex6);
	DDX_Text(pDX, IDC_EDIT_POS7_OLD_CAM_INDEX, m_strOldPosCamIndex7);
	DDX_Check(pDX, IDC_CHECK_POS0_ENABLE, m_bPosEnable0);
	DDX_Check(pDX, IDC_CHECK_POS1_ENABLE, m_bPosEnable1);
	DDX_Check(pDX, IDC_CHECK_POS2_ENABLE, m_bPosEnable2);
	DDX_Check(pDX, IDC_CHECK_POS3_ENABLE, m_bPosEnable3);
	DDX_Check(pDX, IDC_CHECK_POS4_ENABLE, m_bPosEnable4);
	DDX_Check(pDX, IDC_CHECK_POS5_ENABLE, m_bPosEnable5);
	DDX_Check(pDX, IDC_CHECK_POS6_ENABLE, m_bPosEnable6);
	DDX_Check(pDX, IDC_CHECK_POS7_ENABLE, m_bPosEnable7);
	DDX_Control(pDX, IDC_CMB_SERIAL_NUMBER, m_comboSN);
	DDX_Control(pDX, IDC_CMB_CH0_NEW_SERIAL_NUMBER, m_comboNewSN0);
	DDX_Control(pDX, IDC_CMB_CH1_NEW_SERIAL_NUMBER, m_comboNewSN1);
	DDX_Control(pDX, IDC_CMB_CH2_NEW_SERIAL_NUMBER, m_comboNewSN2);
	DDX_Control(pDX, IDC_CMB_CH3_NEW_SERIAL_NUMBER, m_comboNewSN3);
	DDX_Control(pDX, IDC_CMB_CH4_NEW_SERIAL_NUMBER, m_comboNewSN4);
	DDX_Control(pDX, IDC_CMB_CH5_NEW_SERIAL_NUMBER, m_comboNewSN5);
	DDX_Control(pDX, IDC_CMB_CH6_NEW_SERIAL_NUMBER, m_comboNewSN6);
	DDX_Control(pDX, IDC_CMB_CH7_NEW_SERIAL_NUMBER, m_comboNewSN7);
	DDX_Control(pDX, IDC_CMB_POS0_NEW_CAM_INDEX, m_comboNewPosCamIndex0);
	DDX_Control(pDX, IDC_CMB_POS1_NEW_CAM_INDEX, m_comboNewPosCamIndex1);
	DDX_Control(pDX, IDC_CMB_POS2_NEW_CAM_INDEX, m_comboNewPosCamIndex2);
	DDX_Control(pDX, IDC_CMB_POS3_NEW_CAM_INDEX, m_comboNewPosCamIndex3);
	DDX_Control(pDX, IDC_CMB_POS4_NEW_CAM_INDEX, m_comboNewPosCamIndex4);
	DDX_Control(pDX, IDC_CMB_POS5_NEW_CAM_INDEX, m_comboNewPosCamIndex5);
	DDX_Control(pDX, IDC_CMB_POS6_NEW_CAM_INDEX, m_comboNewPosCamIndex6);
	DDX_Control(pDX, IDC_CMB_POS7_NEW_CAM_INDEX, m_comboNewPosCamIndex7);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM5, m_btnShowPlatform5);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM6, m_btnShowPlatform6);
	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	DDX_Text(pDX, IDC_STATIC_HELP_INFO, m_strHelpInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetCameraSerial, CDialog)
	//{{AFX_MSG_MAP(CDlgSetCameraSerial)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM5, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM6, OnPlatformChange)
	ON_BN_CLICKED(IDC_BTN_GRAB, OnBtnGrab)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_CMB_CH0_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh0NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_CH1_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh1NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_CH2_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh2NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_CH3_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh3NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_CH4_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh4NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_CH5_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh5NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_CH6_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh6NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_CH7_NEW_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbCh7NewSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_SERIAL_NUMBER, &CDlgSetCameraSerial::OnCbnSelchangeCmbSerialNumber)
	ON_CBN_SELCHANGE(IDC_CMB_POS0_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos0NewCamIndex)
	ON_CBN_SELCHANGE(IDC_CMB_POS1_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos1NewCamIndex)
	ON_CBN_SELCHANGE(IDC_CMB_POS2_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos2NewCamIndex)
	ON_CBN_SELCHANGE(IDC_CMB_POS3_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos3NewCamIndex)
	ON_CBN_SELCHANGE(IDC_CMB_POS4_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos4NewCamIndex)
	ON_CBN_SELCHANGE(IDC_CMB_POS5_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos5NewCamIndex)
	ON_CBN_SELCHANGE(IDC_CMB_POS6_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos6NewCamIndex)
	ON_CBN_SELCHANGE(IDC_CMB_POS7_NEW_CAM_INDEX, &CDlgSetCameraSerial::OnCbnSelchangeCmbPos7NewCamIndex)
	ON_BN_CLICKED(IDC_CHECK_POS0_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos0Enable)
	ON_BN_CLICKED(IDC_CHECK_POS1_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos1Enable)
	ON_BN_CLICKED(IDC_CHECK_POS2_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos2Enable)
	ON_BN_CLICKED(IDC_CHECK_POS3_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos3Enable)
	ON_BN_CLICKED(IDC_CHECK_POS4_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos4Enable)
	ON_BN_CLICKED(IDC_CHECK_POS5_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos5Enable)
	ON_BN_CLICKED(IDC_CHECK_POS6_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos6Enable)
	ON_BN_CLICKED(IDC_CHECK_POS7_ENABLE, &CDlgSetCameraSerial::OnBnClickedCheckPos7Enable)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCameraSerial message handlers

LRESULT CDlgSetCameraSerial::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			EnterCriticalSection(&m_csResource);
			m_pGuiDisplay->SetImage(m_ImageInput, TRUE);					
			LeaveCriticalSection(&m_csResource);
		}
		break;
	default:
		break;
	}
// 	switch (message)
// 	{
// 	case WM_CAMERA_ARRIVAL:
// 	case WM_CAMERA_REMOVAL:
// 		{
// 			// 查询总线上所有相机的序列号
// 			int i = 0, j = 0;
// 			unsigned int nBusCamNum = 0;
// 			vcGrab::GetBusCameraCount(&nBusCamNum);
// 			unsigned int *pSerialNum = new unsigned int[nBusCamNum];
// 			vcGrab::EnumerateBusCameras(pSerialNum, &nBusCamNum);
// 			
// 			for (j=0; j<6; j++)
// 			{
// 				m_vpcmbNewSN.at(j)->ResetContent();
// 			}
// 			if (nBusCamNum == 0)
// 			{
// 				for (j=0; j<6; j++)
// 				{
// 					m_vpcmbNewSN.at(j)->AddString(_T("------------"));
// 				}
// 			}
// 			CString strSN;
// 			m_aulBusCamSN.SetSize(nBusCamNum);
// 			for (i=0; i<nBusCamNum; i++)
// 			{
// 				m_aulBusCamSN.SetAt(i, (unsigned long)pSerialNum[i]);
// 				strSN.Format(_T("%lu"), (unsigned long)pSerialNum[i]);
// 				for (j=0; j<6; j++)
// 				{
// 					m_vpcmbNewSN.at(j)->AddString(strSN);
// 				}
// 			}
// 			delete pSerialNum;
// 			pSerialNum = NULL;
// 
// 			// 更新相机序列号选择
// 			m_nPrevPlatformIndex = -1;
// 			OnPlatformChange();
// 		}
// 		break;
// 	default:
// 		break;
// 	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgSetCameraSerial::OnInitDialog() 
{
	CString strLog;
	strLog.Format(_T("进入[设置相机通道]"));
	g_opLogFile.SaveLogText(strLog,1);
	strLog.Format(_T("点击[工位1]进行设置"));
	g_opLogFile.SaveLogText(strLog,2);
	//m_nPrevPlatformIndex = 0;

	CDialog::OnInitDialog();
    UpdateDialogLanguage();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	if (m_pParent->m_SysOptionConfig.m_bPlatformCustomNameEnable)
	{
		for (int i = 0; i < MaxPlatformCustomNameNum && i < MaxPlatformNum; i++)
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->SetWindowText(m_pParent->m_SysOptionConfig.m_strPlatformCustomName[i]);
		}
	}

	m_pGuiDisplay = new scGuiDisplay;
	m_pGuiDisplay->Create(IDC_STATIC_DISPLAY, this);
	m_pGuiDisplay->SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_pGuiDisplay->SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_pGuiDisplay->SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_pGuiDisplay->SetLanguage(1);
		break;
	}
//	m_pGuiDisplay->SetBackgroundColor(RGB(0,0,0));

	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	int nPartWidth[2] = {20, -1};
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	m_pStatusBar->SetParts(2, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);
	
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	
	UpdateStatusBar();
	
	// 设置按钮
	int nColorOffset = -20;
	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172, 168, 153), TRUE);
// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172, 168, 153), TRUE);
// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);

	m_btnApply.SetThemeHelper(&m_themeHelper);
	//	m_btnApply.SetIcon(IDI_ICON_APPLY_48);
	// 	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172, 168, 153), TRUE);
	// 	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);


// 
// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
//  	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);

	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(150, 150, 150), TRUE);

	CString strText;
	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM1, strText);
	m_btnShowPlatform1.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform1.SetIcon(m_pParent->m_anIDPlatformIcon[0], (int)BTNST_AUTO_DARKER);
// 	m_btnShowPlatform1.SetAlign(CButtonST::ST_ALIGN_VERT);
//	m_btnShowPlatform1.SetFlat(TRUE);
 	m_btnShowPlatform1.SetWindowText(strText);
//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	

	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM2, strText);
 	m_btnShowPlatform2.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform2.SetIcon(m_pParent->m_anIDPlatformIcon[1], (int)BTNST_AUTO_DARKER);
// 	m_btnShowPlatform2.SetAlign(CButtonST::ST_ALIGN_VERT);
// // 	m_btnShowPlatform2.SetFlat(FALSE);
 	m_btnShowPlatform2.SetWindowText(strText);
// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
//  	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	
	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM3, strText);
 	m_btnShowPlatform3.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform3.SetIcon(m_pParent->m_anIDPlatformIcon[2], (int)BTNST_AUTO_DARKER);
// 	m_btnShowPlatform3.SetAlign(CButtonST::ST_ALIGN_VERT);
// // 	m_btnShowPlatform3.SetFlat(FALSE);
 	m_btnShowPlatform3.SetWindowText(strText);
// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
//  	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	
	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM4, strText);
 	m_btnShowPlatform4.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform4.SetIcon(m_pParent->m_anIDPlatformIcon[3], (int)BTNST_AUTO_DARKER);
// 	m_btnShowPlatform4.SetAlign(CButtonST::ST_ALIGN_VERT);
// // 	m_btnShowPlatform4.SetFlat(FALSE);
 	m_btnShowPlatform4.SetWindowText(strText);
// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
//  	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);

	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM5, strText);
	m_btnShowPlatform5.SetThemeHelper(&m_themeHelper);
	m_btnShowPlatform5.SetWindowText(strText);

	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM6, strText);
	m_btnShowPlatform6.SetThemeHelper(&m_themeHelper);
	m_btnShowPlatform6.SetWindowText(strText);

	
	// 设置对话框位置
	CRect rcDlg;
	GetWindowRect(&rcDlg);
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int nDlgWidth = rcDlg.Width();
	int nDlgHeight = rcDlg.Height();
	rcDlg.left = (nScreenWidth - nDlgWidth) / 2;
	rcDlg.top = (nScreenHeight - nDlgHeight) / 2;
	rcDlg.right = rcDlg.left + nDlgWidth;
	rcDlg.bottom = rcDlg.top + nDlgHeight;
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	MoveWindow(rcDlg, FALSE);
	ShowWindow(SW_SHOW);

	m_nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();

	if (m_pParent!=NULL)
	{
	/*	for (int j=0;j<m_nPlatformNum;j++)
		{
			m_pParent->m_vpVisionASM.at(j)->SetIsConfigShareCamera(TRUE);
		}*/

		m_vPreCameraParam.resize(m_nPlatformNum);
		for (int i=0;i<m_nPlatformNum;i++)
		{
			int nCam=m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
			for (int j=0;j<nCam;j++)
			{
				CameraParam camParam;
				m_pParent->m_vpVisionASM.at(i)->GetCameraParam(j,camParam);
				m_vPreCameraParam[i].push_back(camParam);
			}
		}

		
	}



//	m_paulOldSN = new CArray<unsigned long, unsigned long>[m_nPlatformNum];
	m_paulOldSN = new CArray<CString, CString>[m_nPlatformNum];
	m_paulOldPosInfo = new CArray<CPosInfo, CPosInfo>[m_nPlatformNum];
	m_vpOldMainWnd.resize(m_nPlatformNum, NULL);
	int i = 0;
	for (i=0; i<MaxPlatformNum; i++)
	{
		if (i < m_nPlatformNum)
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);
		}
	}
	if (m_nPlatformNum <= 1)
	{
		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);	
	}
	
	// 初始化控制板
	CRect rcPanel, rcItem1, rcItem2;
	GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
	ScreenToClient(rcPanel);

 	int nButtonSizeWidth = 80;
// 	int nButtonSizeHeight = 50;
	int nButtonSizeHeight = 45;
	int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
	int nStatusBarHeight = 20;
	GetClientRect(rcClient);
	rcItem1.SetRect(rcClient.right-rcPanel.Width(), rcClient.top, rcClient.right, rcClient.bottom-nStatusBarHeight);
	GetDlgItem(IDC_BTN_PANEL)->MoveWindow(rcItem1);
	rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSizeWidth)/2, rcItem1.top + nIntendY, 0, 0);
	rcItem2.right = rcItem2.left + nButtonSizeWidth;
	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

	//  	rcItem2.SetRect(rcItem1.left, rcItem1.top + nIntendY, 0, 0);
	//  	rcItem2.right = rcItem2.left + rcItem1.Width();
	//  	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

	// nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	for (i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->MoveWindow(rcItem2);
		rcItem2.OffsetRect(0, rcItem2.Height() + nIntervalY);
	}

	rcItem2.top = rcItem1.bottom-nIntendY-nButtonSizeHeight;
	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;
	GetDlgItem(IDC_BTN_APPLY)->MoveWindow(rcItem2);
	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
	GetDlgItem(IDCANCEL)->MoveWindow(rcItem2);
	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
	GetDlgItem(IDOK)->MoveWindow(rcItem2);

	//  	CRect rcCancel;
	//  	GetDlgItem(IDCANCEL)->GetWindowRect(rcCancel);
	//  	rcItem2.left = rcItem1.left+(rcItem1.Width()-rcCancel.Width())/2;
	//  	rcItem2.right = rcItem2.left + rcCancel.Width();
	//  	rcItem2.top = rcItem1.bottom-nIntendYOK-rcCancel.Height();
	//  	rcItem2.bottom = rcItem2.top + rcCancel.Height();
	//  	GetDlgItem(IDCANCEL)->MoveWindow(rcItem2);
	//  	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalYOK);
	//  	GetDlgItem(IDOK)->MoveWindow(rcItem2);

	
	// 将控件变量放到向量中，方便使用
	m_vpcmbNewSN.push_back(&m_comboNewSN0);
	m_vpcmbNewSN.push_back(&m_comboNewSN1);
	m_vpcmbNewSN.push_back(&m_comboNewSN2);
	m_vpcmbNewSN.push_back(&m_comboNewSN3);
	m_vpcmbNewSN.push_back(&m_comboNewSN4);
	m_vpcmbNewSN.push_back(&m_comboNewSN5);
	m_vpcmbNewSN.push_back(&m_comboNewSN6);
	m_vpcmbNewSN.push_back(&m_comboNewSN7);

	m_vpstrOldSN.push_back(&m_strOldSN0);
	m_vpstrOldSN.push_back(&m_strOldSN1);
	m_vpstrOldSN.push_back(&m_strOldSN2);
	m_vpstrOldSN.push_back(&m_strOldSN3);
	m_vpstrOldSN.push_back(&m_strOldSN4);
	m_vpstrOldSN.push_back(&m_strOldSN5);
	m_vpstrOldSN.push_back(&m_strOldSN6);
	m_vpstrOldSN.push_back(&m_strOldSN7);

	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex0);
	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex1);
	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex2);
	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex3);
	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex4);
	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex5);
	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex6);
	m_vpcmbNewPosCamIndex.push_back(&m_comboNewPosCamIndex7);

	m_vpbPosEnable.push_back(&m_bPosEnable0);
	m_vpbPosEnable.push_back(&m_bPosEnable1);
	m_vpbPosEnable.push_back(&m_bPosEnable2);
	m_vpbPosEnable.push_back(&m_bPosEnable3);
	m_vpbPosEnable.push_back(&m_bPosEnable4);
	m_vpbPosEnable.push_back(&m_bPosEnable5);
	m_vpbPosEnable.push_back(&m_bPosEnable6);
	m_vpbPosEnable.push_back(&m_bPosEnable7);

	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex0);
	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex1);
	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex2);
	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex3);
	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex4);
	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex5);
	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex6);
	m_vpstrOldPosCamIndex.push_back(&m_strOldPosCamIndex7);



	// 查询总线上所有相机的序列号
	int j=0;
	unsigned int nBusCamNum = 0;
//	vcGrab scPt;
	scPTGrab2::GetBusCameraCount(&nBusCamNum);
	//vcGrab::GetBusCameraCount(&nBusCamNum);
	//unsigned int *pSerialNum = new unsigned int[nBusCamNum];

	PTCameraInfo *pPTCameraInfo = new PTCameraInfo[nBusCamNum];
	scPTGrab2::EnumerateBusCameras(pPTCameraInfo, &nBusCamNum);

	for (j=0; j<8; j++)
	{
		m_vpcmbNewSN.at(j)->ResetContent();
	}
	m_comboSN.ResetContent();

	for (j=0; j<8; j++)
	{
		m_vpcmbNewPosCamIndex.at(j)->ResetContent();
	}

	if (nBusCamNum == 0)
	{
		for (j=0; j<8; j++)
		{
			m_vpcmbNewSN.at(j)->AddString(_T("***"));
		}
	}
	CString strSN;
	m_aulBusCamSN.SetSize(nBusCamNum);
	for (i=0; i<nBusCamNum; i++)
	{
// 		m_aulBusCamSN.SetAt(i, (unsigned long)pSerialNum[i]);
// 		strSN.Format(_T("%lu"), (unsigned long)pSerialNum[i]);
		CString strID = _T("");
		strID = pPTCameraInfo[i].m_cDeviceID;
		m_aulBusCamSN.SetAt(i, strID);
		strSN = strID;
		for (j=0; j<8; j++)
		{
			m_vpcmbNewSN.at(j)->AddString(strSN);
		}

		m_comboSN.AddString(strSN);
	}

	delete pPTCameraInfo;
	pPTCameraInfo = NULL;

// 	delete pSerialNum;
// 	pSerialNum = NULL;

	// 设置对位工具对象的主窗口（消息响应窗口）
	for (j=0; j<m_nPlatformNum; j++)
	{
		m_vpOldMainWnd.at(j) = m_pParent->m_vpVisionASM.at(j)->SetMainWnd(this);
	}

	// 保存旧相机序列号设置
	int nCamNum = 0;
	CameraParam camParam;
	CString strOldSN;
	for (j=0; j<m_nPlatformNum; j++)
	{
		nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
		m_paulOldSN[j].SetSize(nCamNum);
		for (i=0; i<nCamNum; i++)
		{
			m_pParent->m_vpVisionASM.at(j)->GetCameraParam(i, camParam);
			//m_paulOldSN[j].SetAt(i, camParam.m_lSerialNum);
			m_paulOldSN[j].SetAt(i, camParam.m_strDeviceID);
		}
	}

	// 保存旧的位置信息
	int nPosNum = 0;
	CPosInfo posInfo;
	for (j=0; j<m_nPlatformNum; j++)
	{
		nPosNum = m_pParent->m_vpVisionASM.at(j)->GetPosNum();
		m_paulOldPosInfo[j].SetSize(nPosNum);
		for (i=0; i<nPosNum; i++)
		{
			m_pParent->m_vpVisionASM.at(j)->GetPosInfo(i, posInfo);
			//m_paulOldSN[j].SetAt(i, camParam.m_lSerialNum);
			m_paulOldPosInfo[j].SetAt(i, posInfo);
		}
	}

	
	OnPlatformChange();

	// 显示相机初始化提示信息
	if (nBusCamNum < 1)
	{
		m_strHelpInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STC_NO_CAMERA_AVAILABLE);	//_T("没有任何可用相机！");
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = m_strHelpInfo;
		UpdateStatusBar();

		UpdateData(FALSE);
		return TRUE;
	}
	int nCurSel = -1;
	CString strNewSN, strCamera, strInfo, strPlatform, strHelpInfo;
	for (j=0; j<m_nPlatformNum; j++)
	{
		if ((m_pvdwInitErrCode->at(j) & INIT_CAMERA_FAILED) == INIT_CAMERA_FAILED)
		{
			nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
			strInfo.Empty();
			for(int i=0; i<nCamNum; i++)
			{
				if ( ((m_pvdwInitErrCode->at(j)>>i) & 0x00000001) == 0x00000001 )	// 逐个检查相机是否初始化失败
				{
					strInfo.Format(_T("CH%d、"), i+1);
					strCamera += strInfo;
				}
			}
			strCamera = strCamera.Left(strCamera.GetLength()-2);

			if (m_nPlatformNum == 1)
			{
				strPlatform.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), strCamera, GetSysInfoString(m_psaSysInfoStrings,IDS_STC_INIT_FAILED) + _T("\n"));// 平台号从1开始
			}
			else
			{
				strPlatform.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_CAMERA_SSS_SSS), j+1, strCamera, GetSysInfoString(m_psaSysInfoStrings,IDS_STC_INIT_FAILED) + _T("\n"));
			}

			strHelpInfo += strPlatform;
		}
	}
	if (!strHelpInfo.IsEmpty())
	{
		m_strHelpInfo = strHelpInfo;

		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = m_strHelpInfo;
		UpdateStatusBar();

		UpdateData(FALSE);
	}

	m_comboNewSN0.GetWindowText(strcomboNewSN0Old);
	m_comboNewSN1.GetWindowText(strcomboNewSN1Old);
	m_comboNewSN2.GetWindowText(strcomboNewSN2Old);
	m_comboNewSN3.GetWindowText(strcomboNewSN3Old);
	m_comboNewSN4.GetWindowText(strcomboNewSN4Old);
	m_comboNewSN5.GetWindowText(strcomboNewSN5Old);
	m_comboNewSN6.GetWindowText(strcomboNewSN6Old);
	m_comboNewSN7.GetWindowText(strcomboNewSN7Old);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetCameraSerial::OnPlatformChange()
{
	UpdateData(TRUE);
	if ((m_nPlatformIndex == m_nPrevPlatformIndex) || (m_nPlatformIndex >= m_nPlatformNum) || (m_nPlatformIndex < 0))
	{
		return;
	}

	// 保存前一平台的序列号设置
	int nMinValidSNIdx = (m_aulBusCamSN.GetSize() > 0) ? 0 : 1;
	int i = 0;
	int nCamNum = 0;
	int nPosNum = 0;
	CameraParam camParam;
	CPosInfo posInfo;
	CString strNewSN;
	if ((m_nPrevPlatformIndex >= 0) && (m_nPrevPlatformIndex < m_nPlatformNum))
	{
		nCamNum = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->GetCameraNum();
		for (i=0; i<nCamNum; i++)
		{
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->GetCameraParam(i, camParam);
			int nCurSel = m_vpcmbNewSN.at(i)->GetCurSel();
			if (nCurSel >= nMinValidSNIdx)
			{
				m_vpcmbNewSN.at(i)->GetLBText(nCurSel, strNewSN);
			}
			else
			{
				strNewSN.Empty();
			}
			//camParam.m_lSerialNum = _tcstoul(strNewSN, NULL, 10);
			camParam.m_strDeviceID = strNewSN;

			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetCameraParam(i, camParam);		
		}

		nPosNum = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->GetPosNum();
		for (i=0; i<nPosNum; i++)
		{
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->GetPosInfo(i, posInfo);
			int nCurSel = m_vpcmbNewPosCamIndex.at(i)->GetCurSel();
			int bEnable = *m_vpbPosEnable.at(i);

			posInfo.m_nCamIndex = nCurSel;
			posInfo.m_bPosEnable = bEnable;			
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetPosInfo(i, posInfo);		
		}


	}

	// 显示新平台的序列号设置
	nCamNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
	for (i=0; i<8; i++)
	{
		if (i < nCamNum)
		{
			GetDlgItem(IDC_STATIC_SN_CH0+i)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_STATIC_GRP_CAM_SN_CH0+i)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_STATIC_OLD_SN_CH0+i)->ShowWindow(SW_SHOW);
 			GetDlgItem(IDC_EDIT_CH0_OLD_SERIAL_NUMBER+i)->ShowWindow(SW_SHOW);
//			GetDlgItem(IDC_STATIC_NEW_SN_CH0+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMB_CH0_NEW_SERIAL_NUMBER+i)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_STATIC_SN_CH0+i)->ShowWindow(SW_HIDE);
//			GetDlgItem(IDC_STATIC_GRP_CAM_SN_CH0+i)->ShowWindow(SW_HIDE);
//			GetDlgItem(IDC_STATIC_OLD_SN_CH0+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_CH0_OLD_SERIAL_NUMBER+i)->ShowWindow(SW_HIDE);
//			GetDlgItem(IDC_STATIC_NEW_SN_CH0+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMB_CH0_NEW_SERIAL_NUMBER+i)->ShowWindow(SW_HIDE);
		}
	}

	// 显示新平台的位置信息设置
	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosNum();
	for (i=0; i<8; i++)
	{
		if (i < nPosNum)
		{
			GetDlgItem(IDC_STATIC_POS0+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_POS0_OLD_CAM_INDEX+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMB_POS0_NEW_CAM_INDEX+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CHECK_POS0_ENABLE+i)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_STATIC_POS0+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_POS0_OLD_CAM_INDEX+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMB_POS0_NEW_CAM_INDEX+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHECK_POS0_ENABLE+i)->ShowWindow(SW_HIDE);
		}
	}

	for(i=0; i<nCamNum; i++)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraParam(i, camParam);
		//strNewSN.Format(_T("%lu"), camParam.m_lSerialNum);
		//m_vpstrOldSN.at(i)->Format(_T("%lu"), m_paulOldSN[m_nPlatformIndex].GetAt(i));
		strNewSN = camParam.m_strDeviceID;
		m_vpstrOldSN.at(i)->Format(_T("%s"),m_paulOldSN[m_nPlatformIndex].GetAt(i));
		int nIdxFound = m_vpcmbNewSN.at(i)->FindStringExact(-1, strNewSN);
		if (nIdxFound != CB_ERR)
		{
			m_vpcmbNewSN.at(i)->SetCurSel(nIdxFound);
		}
		else
		{
			m_vpcmbNewSN.at(i)->SetCurSel(0);
		}
	}

	int j = 0;
	for(i=0; i<nPosNum; i++)
	{
		m_vpcmbNewPosCamIndex.at(i)->ResetContent();
		for(j = 0; j < nCamNum; j++)
		{
			CString strCamIndex;
			//strCamIndex.Format(_T("CH%d通道"), j+1);
			strCamIndex.Format(_T("CH%d"), j+1);
			m_vpcmbNewPosCamIndex.at(i)->AddString(strCamIndex);
		}
	}

	for(i=0; i<nPosNum; i++)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosInfo(i, posInfo);
		//strNewSN.Format(_T("%lu"), camParam.m_lSerialNum);
		//m_vpstrOldSN.at(i)->Format(_T("%lu"), m_paulOldSN[m_nPlatformIndex].GetAt(i));
		//m_vpstrOldPosCamIndex.at(i)->Format(_T("CH%d通道"),m_paulOldPosInfo[m_nPlatformIndex].GetAt(i).m_nCamIndex+1);
		m_vpstrOldPosCamIndex.at(i)->Format(_T("CH%d"),m_paulOldPosInfo[m_nPlatformIndex].GetAt(i).m_nCamIndex+1);
		m_vpcmbNewPosCamIndex.at(i)->SetCurSel(posInfo.m_nCamIndex);
		*m_vpbPosEnable.at(i) = posInfo.m_bPosEnable;
	}

	CString strLog;
	if (m_nPrevPlatformIndex != -1)
	{
		//m_nPrevPlatformIndex = 0;
		strLog.Format(_T("完成[工位%d]设置"),m_nPrevPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("点击[工位%d]进行设置"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
	}
	//strLog.Format(_T("完成[工位%d]设置"),m_nPrevPlatformIndex+1);
	//g_opLogFile.SaveLogText(strLog,2);
	//strLog.Format(_T("点击[工位%d]进行设置"),m_nPlatformIndex+1);
	//g_opLogFile.SaveLogText(strLog,2);

	UpdateData(FALSE);

	m_nPrevPlatformIndex = m_nPlatformIndex;
}

void CDlgSetCameraSerial::OnOK()
{
	// 释放
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
		//scPTGrab2::DeleteCamera(m_pCamera);
		m_pCamera = NULL;
	}


	if (!SaveAndInvalidateAll())
	{
		UpdateData(FALSE);
		return;
	}
	CString strLog;
	strLog.Format(_T("完成[工位%d]设置\n"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("保存退出[设置相机通道]"));
	g_opLogFile.SaveLogText(strLog,1);

	CDialog::OnOK();
}

void CDlgSetCameraSerial::OnCancel() 
{

	// 释放
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
		//scPTGrab2::DeleteCamera(m_pCamera);
		m_pCamera = NULL;
	}

	// 恢复旧相机序列号
	CPosInfo posInfo;
	
	for (int j=0; j<m_nPlatformNum; j++)
	{
		CameraParam camParam;
		int nCamNum = m_pParent->m_vpVisionASM.at(j)->GetCameraNum();
		for (int i=0; i<nCamNum; i++)
		{
			m_pParent->m_vpVisionASM.at(j)->GetCameraParam(i, camParam);
			//camParam.m_lSerialNum = m_paulOldSN[j].GetAt(i);
			camParam.m_strDeviceID = m_paulOldSN[j].GetAt(i);
			m_pParent->m_vpVisionASM.at(j)->SetCameraParam(i, camParam);
			m_pParent->m_vpVisionASM.at(j)->SaveCameraInfo();
		}

		int nPosNum = m_pParent->m_vpVisionASM.at(j)->GetPosNum();
		for (int i=0; i<nPosNum; i++)
		{
			m_pParent->m_vpVisionASM.at(j)->GetPosInfo(i, posInfo);
			posInfo.m_bPosEnable = m_paulOldPosInfo[j].GetAt(i).m_bPosEnable;
			posInfo.m_nCamIndex = m_paulOldPosInfo[j].GetAt(i).m_nCamIndex;
			m_pParent->m_vpVisionASM.at(j)->SetPosInfo(i, posInfo);
			m_pParent->m_vpVisionASM.at(j)->SavePosInfo();
		}
	}

	CString strLog;
	strLog.Format(_T("完成[工位%d]设置\n"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("不保存退出[设置相机通道]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	
	CDialog::OnCancel();
}

BOOL CDlgSetCameraSerial::DestroyWindow() 
{
	// 还原视觉对位工具对象的主窗口（消息响应窗口）
	for (int j=0; j<m_nPlatformNum; j++)
	{
		m_pParent->m_vpVisionASM.at(j)->SetMainWnd(m_vpOldMainWnd.at(j));
	}

	// 释放
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
		//scPTGrab2::DeleteCamera(m_pCamera);
		m_pCamera = NULL;
	}

	if (m_pGuiDisplay)
	{
		delete m_pGuiDisplay;
		m_pGuiDisplay = NULL;
	}
	
	if (m_pStatusBar != NULL)
	{
		delete m_pStatusBar;
		m_pStatusBar = NULL;
	}
	if (m_hIconSucceed != NULL)
	{
		DestroyIcon(m_hIconSucceed);
		m_hIconSucceed = NULL;
	}
	if (m_hIconFailed != NULL)
	{
		DestroyIcon(m_hIconFailed);
		m_hIconFailed = NULL;
	}
	
	return CDialog::DestroyWindow();
}

void CDlgSetCameraSerial::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_bNotEnterApp = TRUE;	 // Mali 2013.9.18
	CDialog::OnClose();
}

void WINAPI CDlgSetCameraSerial::GrabImageFinishedCallback( void* pOwner, PTStatus status, unsigned char* pImage, long lWidth, long lHeight, PTPixelFormat pixelFormat )
{
	if (ePTGrabOK!=status)
	{
		return;
	}
	
	if (NULL == pOwner || pImage == NULL)
	{
		return ;
	}    
	
	CDlgSetCameraSerial* pMain = (CDlgSetCameraSerial*)(pOwner);
	
	if(NULL == pMain)
	{
		return;
	}
    
	// 	cpImage srcImage(pImage, lWidth, lHeight, 1);
	// 	pMain->ImageProcess(0, srcImage);

	if (ePTGrey8 == pixelFormat)
	{
		CamPixelFormat eFormat;
		int nFormat = 0;

		scPTGrab2* pPTGrab = pMain->m_pCamera; 
		if(pPTGrab->GetCamPixelFormat(nFormat))
		{
			eFormat = CamPixelFormat(nFormat);

			if(eFormat == eCamBayer_BG8 || eFormat == eCamBayer_GB8 ||eFormat == eCamBayer_RG8 ||eFormat == eCamBayer_GR8)
			{
				cpImage srcImage( lWidth, lHeight,epPackedRGB24, 1);
				cpImage tmpImage(pImage, lWidth, lHeight,epGray8, 1);	
				cpBayerFilter ff;
				ff.Bayer2RGB24(tmpImage,srcImage,(epBayerType)eFormat);					
				pMain->ImageProcess(0, srcImage);
			}
			else if(eFormat == eCamGray)
			{
				cpImage srcImage(pImage, lWidth, lHeight,epGray8, 1);		
				pMain->ImageProcess(0, srcImage);
			}
			else
			{
				cpImage srcImage(pImage, lWidth, lHeight,epGray8, 1);		
				pMain->ImageProcess(0, srcImage);
			}
		}
	}
	else if (ePTRGB24 == pixelFormat || ePTRGBA32 == pixelFormat)
	{

		cpImage srcImage(pImage, lWidth, lHeight,epPackedRGB24, 1);	
		pMain->ImageProcess(0, srcImage);
	}
}
void CDlgSetCameraSerial::ImageProcess(int nIndex, cpImage srcImage)
{ 	
	// 连续采集处理	
	EnterCriticalSection(&m_csResource);
	m_ImageInput = srcImage;
	LeaveCriticalSection(&m_csResource);
	
	::PostMessage(GetSafeHwnd(), WM_UPDATE_DISPLAY_IMAGE, nIndex, 0);	
}

void CDlgSetCameraSerial::OnBtnGrab() 
{
	// TODO: Add your control notification handler code here
	//

	//

	UpdateData(TRUE);
	if (m_comboSN.GetCount() < 1)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_NO_CAMERA);// _T("没有可用相机！");
		UpdateStatusBar();

		return;
	}

	CString strNewSN;
	int nCurSel =  m_comboSN.GetCurSel();
	if (nCurSel < 0)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_NO_CAMERA);//_T("没有可用相机！");
		UpdateStatusBar();
		return;
	}
	m_comboSN.GetLBText(nCurSel, strNewSN);
	if (strNewSN.IsEmpty())
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_NO_CAMERA);//_T("没有可用相机！");
		UpdateStatusBar();
		return;
	}
	
	CString strText;
	GetDlgItem(IDC_BTN_GRAB)->GetWindowText(strText);
	//if (strText == _T("采集")) // 
	if (strText == GetSysInfoString(m_psaSysInfoStrings,IDS_STR_GRAB))
	{
		CString strLog;
		strLog.Format(_T("点击[%s],开始采集"),strText);
		g_opLogFile.SaveLogText(strLog,3);
		// 释放
		if (m_pCamera != NULL)
		{
			delete m_pCamera;
			//scPTGrab2::DeleteCamera(m_pCamera);
			m_pCamera = NULL;
		}

		// 初始化、开始采集
		m_pCamera = new scPTGrab2;
		//m_pCamera = scPTGrab2::CreateCamera();
		m_pCamera->SetOwner(this, GrabImageFinishedCallback);

 		if (!m_pCamera->InitFromSerial(strNewSN))
 		{
 			// 初始化失败
 			// 退出
 			m_bStatusBarFlag = FALSE;
 			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_INIT_FAILED);	//_T("相机初始化失败！");
 			UpdateStatusBar();
 
 			return;
 		}
		
		m_pCamera->Grab();
		GetDlgItem(IDC_CMB_SERIAL_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_FREEZE)/*_T("停止")*/);

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_GRAB_SUCCESS);	//_T("相机采集成功！");
		UpdateStatusBar();
	}
	else if (strText == GetSysInfoString(m_psaSysInfoStrings,IDS_STR_FREEZE)/*_T("停止")*/)
	{
		CString strLog;
		strLog.Format(_T("点击[%s],停止采集"),strText);
		g_opLogFile.SaveLogText(strLog,3);
		// 停止采集
		m_pCamera->Freeze();
		GetDlgItem(IDC_CMB_SERIAL_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_GRAB)/*_T("采集")*/);

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CAMERA_FREEZE);//_T("相机停止采集！");
		UpdateStatusBar();
	}

}

void CDlgSetCameraSerial::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
}



void CDlgSetCameraSerial::OnCbnSelchangeCmbCh0NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN0;
	m_comboNewSN0.GetWindowText(strcomboNewSN0);

	if (strcomboNewSN0Old != strcomboNewSN0)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH1通道]由%s到%s\n"),strcomboNewSN0Old,strcomboNewSN0);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN0Old = strcomboNewSN0;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbCh1NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN1;
	m_comboNewSN1.GetWindowText(strcomboNewSN1);

	if (strcomboNewSN1Old != strcomboNewSN1)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH2通道]由%s到%s\n"),strcomboNewSN1Old,strcomboNewSN1);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN1Old = strcomboNewSN1;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbCh2NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN2;
	m_comboNewSN2.GetWindowText(strcomboNewSN2);

	if (strcomboNewSN2Old != strcomboNewSN2)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH3通道]由%s到%s\n"),strcomboNewSN2Old,strcomboNewSN2);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN2Old = strcomboNewSN2;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbCh3NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN3;
	m_comboNewSN3.GetWindowText(strcomboNewSN3);

	if (strcomboNewSN3Old != strcomboNewSN3)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH4通道]由%s到%s\n"),strcomboNewSN3Old,strcomboNewSN3);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN3Old = strcomboNewSN3;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbCh4NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN4;
	m_comboNewSN4.GetWindowText(strcomboNewSN4);

	if (strcomboNewSN4Old != strcomboNewSN4)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH5通道]由%s到%s\n"),strcomboNewSN4Old,strcomboNewSN4);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN4Old = strcomboNewSN4;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbCh5NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN5;
	m_comboNewSN5.GetWindowText(strcomboNewSN5);

	if (strcomboNewSN5Old != strcomboNewSN5)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH6通道]由%s到%s\n"),strcomboNewSN5Old,strcomboNewSN5);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN5Old = strcomboNewSN5;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbCh6NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN6;
	m_comboNewSN6.GetWindowText(strcomboNewSN6);

	if (strcomboNewSN6Old != strcomboNewSN6)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH7通道]由%s到%s\n"),strcomboNewSN6Old,strcomboNewSN6);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN6Old = strcomboNewSN6;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbCh7NewSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewSN7;
	m_comboNewSN7.GetWindowText(strcomboNewSN7);

	if (strcomboNewSN7Old != strcomboNewSN7)
	{
		CString strLog;
		strLog.Format(_T("修改[相机通道设置][CH8通道]由%s到%s\n"),strcomboNewSN7Old,strcomboNewSN7);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewSN7Old = strcomboNewSN7;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboSN;
	m_comboSN.GetWindowText(strcomboSN);

	if (strcomboSNOld != strcomboSN)
	{
		CString strLog;
		strLog.Format(_T("修改[相机ID]由%s到%s\n"),strcomboSNOld,strcomboSN);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboSNOld = strcomboSN;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos0NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex0;
	m_comboNewPosCamIndex0.GetWindowText(strcomboNewPosCamIndex0);
	if (strcomboNewPosCamIndex0Old.IsEmpty())
	{
		strcomboNewPosCamIndex0Old = _T("CH1");
	}

	if (strcomboNewPosCamIndex0Old != strcomboNewPosCamIndex0)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置1]由%s到%s\n"),strcomboNewPosCamIndex0Old,strcomboNewPosCamIndex0);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex0Old = strcomboNewPosCamIndex0;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos1NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex1;
	m_comboNewPosCamIndex1.GetWindowText(strcomboNewPosCamIndex1);
	if (strcomboNewPosCamIndex1Old.IsEmpty())
	{
		strcomboNewPosCamIndex1Old = _T("CH1");
	}
	if (strcomboNewPosCamIndex1Old != strcomboNewPosCamIndex1)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置2]由%s到%s\n"),strcomboNewPosCamIndex1Old,strcomboNewPosCamIndex1);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex1Old = strcomboNewPosCamIndex1;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos2NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex2;
	m_comboNewPosCamIndex2.GetWindowText(strcomboNewPosCamIndex2);
	if (strcomboNewPosCamIndex2Old.IsEmpty())
	{
		strcomboNewPosCamIndex2Old = _T("CH1");
	}
	if (strcomboNewPosCamIndex2Old != strcomboNewPosCamIndex2)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置3]由%s到%s\n"),strcomboNewPosCamIndex2Old,strcomboNewPosCamIndex2);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex2Old = strcomboNewPosCamIndex2;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos3NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex3;
	m_comboNewPosCamIndex3.GetWindowText(strcomboNewPosCamIndex3);
	if (strcomboNewPosCamIndex3Old.IsEmpty())
	{
		strcomboNewPosCamIndex3Old = _T("CH1");
	}
	if (strcomboNewPosCamIndex3Old != strcomboNewPosCamIndex3)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置4]由%s到%s\n"),strcomboNewPosCamIndex3Old,strcomboNewPosCamIndex3);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex3Old = strcomboNewPosCamIndex3;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos4NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex4;
	m_comboNewPosCamIndex4.GetWindowText(strcomboNewPosCamIndex4);
	if (strcomboNewPosCamIndex4Old.IsEmpty())
	{
		strcomboNewPosCamIndex4Old = _T("CH1");
	}
	if (strcomboNewPosCamIndex4Old != strcomboNewPosCamIndex4)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置5]由%s到%s\n"),strcomboNewPosCamIndex4Old,strcomboNewPosCamIndex4);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex4Old = strcomboNewPosCamIndex4;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos5NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex5;
	m_comboNewPosCamIndex5.GetWindowText(strcomboNewPosCamIndex5);
	if (strcomboNewPosCamIndex5Old.IsEmpty())
	{
		strcomboNewPosCamIndex5Old = _T("CH1");
	}
	if (strcomboNewPosCamIndex5Old != strcomboNewPosCamIndex5)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置6]由%s到%s\n"),strcomboNewPosCamIndex5Old,strcomboNewPosCamIndex5);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex5Old = strcomboNewPosCamIndex5;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos6NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex6;
	m_comboNewPosCamIndex6.GetWindowText(strcomboNewPosCamIndex6);
	if (strcomboNewPosCamIndex6Old.IsEmpty())
	{
		strcomboNewPosCamIndex6Old = _T("CH1");
	}
	if (strcomboNewPosCamIndex6Old != strcomboNewPosCamIndex6)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置7]由%s到%s\n"),strcomboNewPosCamIndex6Old,strcomboNewPosCamIndex6);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex6Old = strcomboNewPosCamIndex6;
	}
}


void CDlgSetCameraSerial::OnCbnSelchangeCmbPos7NewCamIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strcomboNewPosCamIndex7;
	m_comboNewPosCamIndex7.GetWindowText(strcomboNewPosCamIndex7);
	if (strcomboNewPosCamIndex7Old.IsEmpty())
	{
		strcomboNewPosCamIndex7Old = _T("CH1");
	}
	if (strcomboNewPosCamIndex7Old != strcomboNewPosCamIndex7)
	{
		CString strLog;
		strLog.Format(_T("修改[位置信息设置][位置8]由%s到%s\n"),strcomboNewPosCamIndex7Old,strcomboNewPosCamIndex7);
		g_opLogFile.SaveLogText(strLog,3);
		strcomboNewPosCamIndex7Old = strcomboNewPosCamIndex7;
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos0Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable0Old = m_bPosEnable0;

	UpdateData(TRUE);
	if (bPosEnable0Old != m_bPosEnable0)
	{
		CString strLog;
		CString strValue = m_bPosEnable0 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos1Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable1Old = m_bPosEnable1;

	UpdateData(TRUE);
	if (bPosEnable1Old != m_bPosEnable1)
	{
		CString strLog;
		CString strValue = m_bPosEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos2Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable2Old = m_bPosEnable2;

	UpdateData(TRUE);
	if (bPosEnable2Old != m_bPosEnable2)
	{
		CString strLog;
		CString strValue = m_bPosEnable2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置3]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos3Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable3Old = m_bPosEnable3;

	UpdateData(TRUE);
	if (bPosEnable3Old != m_bPosEnable3)
	{
		CString strLog;
		CString strValue = m_bPosEnable3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置4]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos4Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable4Old = m_bPosEnable4;

	UpdateData(TRUE);
	if (bPosEnable4Old != m_bPosEnable4)
	{
		CString strLog;
		CString strValue = m_bPosEnable4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置5]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos5Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable5Old = m_bPosEnable5;

	UpdateData(TRUE);
	if (bPosEnable5Old != m_bPosEnable5)
	{
		CString strLog;
		CString strValue = m_bPosEnable5 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置5]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos6Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable6Old = m_bPosEnable6;

	UpdateData(TRUE);
	if (bPosEnable6Old != m_bPosEnable6)
	{
		CString strLog;
		CString strValue = m_bPosEnable6 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置7]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSetCameraSerial::OnBnClickedCheckPos7Enable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPosEnable7Old = m_bPosEnable7;

	UpdateData(TRUE);
	if (bPosEnable7Old != m_bPosEnable7)
	{
		CString strLog;
		CString strValue = m_bPosEnable7 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[位置信息设置][位置8]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}
