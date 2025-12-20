// DlgSysPlatformYXPDParam.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysPlatformYX1X2PDParam.h"
#include "DlgSysEnvironSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformYX1X2PDParam dialog


CDlgSysPlatformYX1X2PDParam::CDlgSysPlatformYX1X2PDParam(CWnd* pParent /*=NULL*/)
	: CDlgSysPlatformParam(CDlgSysPlatformYX1X2PDParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysPlatformYX1X2PDParam)
	m_strRotationLength = _T("");
	//}}AFX_DATA_INIT
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
}


void CDlgSysPlatformYX1X2PDParam::InitDlgItem()
{
	m_comboCH0AxisXDirection.ResetContent();
	m_comboCH0AxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboCH0AxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	
	m_comboCH1AxisXDirection.ResetContent();
	m_comboCH1AxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboCH1AxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	
	m_comboYDriveDirection.ResetContent();
	m_comboYDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboYDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));		//_T("向上")
	
	m_comboRDriveDirection.ResetContent();
	m_comboRDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CLOCKWISE));		//_T("顺时针")
	m_comboRDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COUNTER_CLOCKWISE));//_T("逆时针")
	
	m_comboRDriveType.ResetContent();
	m_comboRDriveType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DIRECTIVE_DRIVE));		//_T("直接驱动")
	m_comboRDriveType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINEAR_DRIVE));			//_T("直线驱动")
	UpdateDlgByAuthorize();
	::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysPlatformYX1X2PDParam::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	CPlatformYDPX1X2Info *pPlatformYDPX1X2Info = (CPlatformYDPX1X2Info*)m_pPlatformInfo;
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		pPlatformYDPX1X2Info->m_nPlatformXCoordType[0] = (m_comboCH0AxisXDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformYDPX1X2Info->m_nPlatformXCoordType[1] = (m_comboCH1AxisXDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformYDPX1X2Info->m_nPlatformYCoordType = (m_comboYDriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformYDPX1X2Info->m_nPlatformDCoordType = (m_comboRDriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformYDPX1X2Info->m_nPlatformDDriveType = (PlatformDDriveType)m_comboRDriveType.GetCurSel();
		pPlatformYDPX1X2Info->m_dPlatformRotationLength = atof(m_strRotationLength);
	}
	else
	{
		m_comboCH0AxisXDirection.SetCurSel((pPlatformYDPX1X2Info->m_nPlatformXCoordType[0] == eDirectPositive) ? 0 : 1);
		m_comboCH1AxisXDirection.SetCurSel((pPlatformYDPX1X2Info->m_nPlatformXCoordType[1] == eDirectPositive) ? 0 : 1);
		m_comboYDriveDirection.SetCurSel((pPlatformYDPX1X2Info->m_nPlatformYCoordType == eDirectPositive) ? 0 : 1);
		m_comboRDriveDirection.SetCurSel((pPlatformYDPX1X2Info->m_nPlatformDCoordType == eDirectPositive) ? 0 : 1);
		m_comboRDriveType.SetCurSel((int)pPlatformYDPX1X2Info->m_nPlatformDDriveType);
		m_strRotationLength.Format(_T("%.3f"), pPlatformYDPX1X2Info->m_dPlatformRotationLength);
		UpdateData(FALSE);
	}
}

BOOL CDlgSysPlatformYX1X2PDParam::SetPlatformInfo( const CPlatformInfo* platformInfo )
{
	if (platformInfo->GetPlatformType() != ePlatformYX1X2PD)
	{
		return FALSE;
	}
	if (m_pPlatformInfo != NULL)
	{
		delete m_pPlatformInfo;
		m_pPlatformInfo = NULL;
	}
	if (platformInfo != NULL)
	{
		m_pPlatformInfo = new CPlatformYX1X2PDInfo(*((CPlatformYX1X2PDInfo*)platformInfo));
	}
	return TRUE;
}

void CDlgSysPlatformYX1X2PDParam::UpdateDialogLanguage()
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
	atof(strXMLPath);
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
		
		if(_T("IDD_SYS_PLATFORM_YX1X2PD_PARAM") == strDlgID)
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

void CDlgSysPlatformYX1X2PDParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysPlatformYX1X2PDParam)
	DDX_Control(pDX, IDC_COMBO_DRIVE_TYPE, m_comboRDriveType);
	DDX_Control(pDX, IDC_COMBO_Y_COORD_TYPE, m_comboYDriveDirection);
	DDX_Control(pDX, IDC_COMBO_R_DIRECTION, m_comboRDriveDirection);
	DDX_Control(pDX, IDC_COMBO_CH0X_COORD_TYPE, m_comboCH0AxisXDirection);
	DDX_Control(pDX, IDC_COMBO_CH1X_COORD_TYPE, m_comboCH1AxisXDirection);
	DDX_Text(pDX, IDC_EDIT_ROTATION_LENGTH, m_strRotationLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysPlatformYX1X2PDParam, CDialog)
	//{{AFX_MSG_MAP(CDlgSysPlatformYX1X2PDParam)
	ON_EN_SETFOCUS(IDC_EDIT_ROTATION_LENGTH, OnSetfocusEditRotationLength)
	ON_EN_KILLFOCUS(IDC_EDIT_ROTATION_LENGTH, OnKillfocusEditRotationLength)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_CH0X_COORD_TYPE, &CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboCh0xCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_CH1X_COORD_TYPE, &CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboCh1xCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_Y_COORD_TYPE, &CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboYCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_R_DIRECTION, &CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboRDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE_TYPE, &CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboDriveType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformYX1X2PDParam message handlers

BOOL CDlgSysPlatformYX1X2PDParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);
	
	CString strComboX1DriveDirection;
	m_comboCH0AxisXDirection.GetWindowText(strComboX1DriveDirection);
	strComboX1DriveDirectionOld = strComboX1DriveDirection;

	CString strComboX2DriveDirection;
	m_comboCH1AxisXDirection.GetWindowText(strComboX2DriveDirection);
	strComboX2DriveDirectionOld = strComboX2DriveDirection;

	CString strComboYDriveDirection;
	m_comboYDriveDirection.GetWindowText(strComboYDriveDirection);
	strComboYDriveDirectionOld = strComboYDriveDirection;

	CString strComboDriveType;
	m_comboRDriveType.GetWindowText(strComboDriveType);
	strComboDriveTypeOld = strComboDriveType;

	CString strComboRDirection;
	m_comboRDriveDirection.GetWindowText(strComboRDirection);
	strComboRCoordTypeOld = strComboRDirection;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysPlatformYX1X2PDParam::OnSetfocusEditRotationLength() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ROTATION_LENGTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[平台旋转中心到θ轴的垂直距离]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditRotationLength();
	}
}

void CDlgSysPlatformYX1X2PDParam::OnKillfocusEditRotationLength() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strVal);
		if (!IsStringNumerical(strVal))
		{
			strVal.Format(_T("%.3f"), ((CPlatformYX1X2PDInfo*)m_pPlatformInfo)->m_dPlatformRotationLength);
			SetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strVal);
		}
	}	
}

BOOL CDlgSysPlatformYX1X2PDParam::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboCh0xCoordType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboX1DriveDirection;
	m_comboCH0AxisXDirection.GetWindowText(strComboX1DriveDirection);

	if (strComboX1DriveDirectionOld != strComboX1DriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[X1轴正方向]由%s到%s\n"),strComboX1DriveDirectionOld,strComboX1DriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboX1DriveDirectionOld = strComboX1DriveDirection;
	}
}


void CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboCh1xCoordType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboX2DriveDirection;
	m_comboCH1AxisXDirection.GetWindowText(strComboX2DriveDirection);

	if (strComboX2DriveDirectionOld != strComboX2DriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[X2轴正方向]由%s到%s\n"),strComboX2DriveDirectionOld,strComboX2DriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboX2DriveDirectionOld = strComboX2DriveDirection;
	}
}


void CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboYCoordType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboYDriveDirection;
	m_comboYDriveDirection.GetWindowText(strComboYDriveDirection);

	if (strComboYDriveDirectionOld != strComboYDriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[Y轴正方向]由%s到%s\n"),strComboYDriveDirectionOld,strComboYDriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboYDriveDirectionOld = strComboYDriveDirection;
	}
}


void CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboRDirection()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboRDirection;
	m_comboRDriveDirection.GetWindowText(strComboRDirection);

	if (strComboRCoordTypeOld != strComboRDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[θ轴正方向]由%s到%s\n"),strComboRCoordTypeOld,strComboRDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboRCoordTypeOld = strComboRDirection;
	}
}


void CDlgSysPlatformYX1X2PDParam::OnCbnSelchangeComboDriveType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboDriveType;
	m_comboRDriveType.GetWindowText(strComboDriveType);

	if (strComboDriveTypeOld != strComboDriveType)
	{
		CString strLog;
		strLog.Format(_T("修改[θ轴驱动方式]由%s到%s\n"),strComboDriveTypeOld,strComboDriveType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboDriveTypeOld = strComboDriveType;
	}
}


void CDlgSysPlatformYX1X2PDParam::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
{
	CRect CRectGroupBox;
	// 检查是否为Group控件
	if(NULL== GetDlgItem(nID)) return;
	if (NULL== (CStatic*) GetDlgItem(nID)) return;
	GetDlgItem(nID)->GetWindowRect(&CRectGroupBox);
	ScreenToClient(&CRectGroupBox);

	int nShow = SW_SHOW;
	if (bShow)
	{
		nShow = SW_SHOW;
	}
	else
	{
		nShow = SW_HIDE;
	}

	// Group 控件本身也进行显示或隐藏
	GetDlgItem(nID)->EnableWindow(bEnable);
	GetDlgItem(nID)->ShowWindow(nShow);

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		CRect CRect2;
		pWnd->GetWindowRect(&CRect2);
		ScreenToClient(&CRect2);
		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) && CRectGroupBox.PtInRect(CRect2.BottomRight()))
		{
			pWnd->EnableWindow(bEnable);
			pWnd->ShowWindow(nShow);
		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgSysPlatformYX1X2PDParam::UpdateDlgByAuthorize()
{
	if (NULL !=m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			bool bShow = false;
			bool bEnable = false;
			CString strNodeName = _T("");
			// 对位平台参数
			bool bFind = vsConfig->GetCheckedByItem(ePlatformAuthorize_ParamSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_PLATFORM_COORD,bEnable,bShow);
				SetGroupBoxStatus(IDC_STATIC_GRP_PLATFORM_DETAIL,bEnable,bShow);

			}

		}
	}
}