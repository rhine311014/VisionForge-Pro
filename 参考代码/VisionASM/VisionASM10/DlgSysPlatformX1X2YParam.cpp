// DlgSysPlatformX1X2YParam.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysPlatformX1X2YParam.h"
#include "DlgSysEnvironSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformX1X2YParam dialog


CDlgSysPlatformX1X2YParam::CDlgSysPlatformX1X2YParam(CWnd* pParent /*=NULL*/)
	: CDlgSysPlatformParam(CDlgSysPlatformX1X2YParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysPlatformX1X2YParam)
	m_nCameraIndex = 0;
	m_strX1CoordX = _T("");
	m_strX1CoordY = _T("");
	m_strX2CoordX = _T("");
	m_strYCoordX = _T("");
	m_strX2CoordY = _T("");
	m_strYCoordY = _T("");
	//}}AFX_DATA_INIT
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
}

void CDlgSysPlatformX1X2YParam::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_PLATFORM_X1X2Y_PARAM") == strDlgID)
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

void CDlgSysPlatformX1X2YParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysPlatformX1X2YParam)	
	DDX_Control(pDX, IDC_COMBO_UVW_SLIDER_TYPE, m_comboUVWSliderType);
	DDX_Control(pDX, IDC_COMBO_Y_DRIVE_DIRECTION, m_comboYDriveDirection);
	DDX_Control(pDX, IDC_COMBO_X2_DRIVE_DIRECTION, m_comboX2DriveDirection);
	DDX_Control(pDX, IDC_COMBO_X1_DRIVE_DIRECTION, m_comboX1DriveDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_Y_COORD_TYPE, m_comboCamAxisYDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_X_COORD_TYPE, m_comboCamAxisXDirection);
	DDX_Radio(pDX, IDC_RADIO_CAMERA0, m_nCameraIndex);
	DDX_Text(pDX, IDC_EDIT_X1_COORD_X, m_strX1CoordX);
	DDX_Text(pDX, IDC_EDIT_X1_COORD_Y, m_strX1CoordY);
	DDX_Text(pDX, IDC_EDIT_X2_COORD_X, m_strX2CoordX);
	DDX_Text(pDX, IDC_EDIT_X2_COORD_Y, m_strX2CoordY);
	DDX_Text(pDX, IDC_EDIT_Y_COORD_X, m_strYCoordX);
	DDX_Text(pDX, IDC_EDIT_Y_COORD_Y, m_strYCoordY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysPlatformX1X2YParam, CDialog)
	//{{AFX_MSG_MAP(CDlgSysPlatformX1X2YParam)
	ON_BN_CLICKED(IDC_RADIO_CAMERA0, OnCameraChange)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_X_COORD_TYPE, OnSelchangeComboCamXCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_Y_COORD_TYPE, OnSelchangeComboCamYCoordType)
	ON_EN_SETFOCUS(IDC_EDIT_X1_COORD_X, OnSetfocusEditX1CoordX)
	ON_EN_SETFOCUS(IDC_EDIT_X1_COORD_Y, OnSetfocusEditX1CoordY)
	ON_EN_SETFOCUS(IDC_EDIT_X2_COORD_X, OnSetfocusEditX2CoordX)
	ON_EN_SETFOCUS(IDC_EDIT_X2_COORD_Y, OnSetfocusEditX2CoordY)
	ON_EN_SETFOCUS(IDC_EDIT_Y_COORD_X, OnSetfocusEditYCoordX)
	ON_EN_SETFOCUS(IDC_EDIT_Y_COORD_Y, OnSetfocusEditYCoordY)
	ON_BN_CLICKED(IDC_RADIO_CAMERA1, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA2, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA3, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA4, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA5, OnCameraChange)
	ON_EN_KILLFOCUS(IDC_EDIT_X1_COORD_X, OnKillfocusEditX1CoordX)
	ON_EN_KILLFOCUS(IDC_EDIT_X1_COORD_Y, OnKillfocusEditX1CoordY)
	ON_EN_KILLFOCUS(IDC_EDIT_X2_COORD_X, OnKillfocusEditX2CoordX)
	ON_EN_KILLFOCUS(IDC_EDIT_X2_COORD_Y, OnKillfocusEditX2CoordY)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_COORD_X, OnKillfocusEditYCoordX)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_COORD_Y, OnKillfocusEditYCoordY)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_UVW_SLIDER_TYPE, &CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboUvwSliderType)
	ON_CBN_SELCHANGE(IDC_COMBO_X1_DRIVE_DIRECTION, &CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboX1DriveDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_X2_DRIVE_DIRECTION, &CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboX2DriveDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_Y_DRIVE_DIRECTION, &CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboYDriveDirection)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformX1X2YParam message handlers

BOOL CDlgSysPlatformX1X2YParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);

	CString strComboUvwSliderType;
	m_comboUVWSliderType.GetWindowText(strComboUvwSliderType);
	strComboUvwSliderTypeOld = strComboUvwSliderType;

	CString strComboX1DriveDirection;
	m_comboX1DriveDirection.GetWindowText(strComboX1DriveDirection);
	strComboX1DriveDirectionOld = strComboX1DriveDirection;

	CString strComboX2DriveDirection;
	m_comboX2DriveDirection.GetWindowText(strComboX2DriveDirection);
	strComboX2DriveDirectionOld = strComboX2DriveDirection;

	CString strComboYDriveDirection;
	m_comboYDriveDirection.GetWindowText(strComboYDriveDirection);
	strComboYDriveDirectionOld = strComboYDriveDirection;

	CString strComboCamXCoordType;
	m_comboCamAxisXDirection.GetWindowText(strComboCamXCoordType);
	strComboCamXCoordTypeOld = strComboCamXCoordType;

	CString strComboCamYCoordType;
	m_comboCamAxisXDirection.GetWindowText(strComboCamYCoordType);
	strComboCamYCoordTypeOld = strComboCamYCoordType;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysPlatformX1X2YParam::InitDlgItem()
{
	m_comboUVWSliderType.ResetContent();
	m_comboUVWSliderType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SLIDER_NO_MOVE)/*_T("固定滑块")*/);	//类型1
	m_comboUVWSliderType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SLIDER_MOVE)/*_T("移动滑块")*/);	//类型2

	m_comboX1DriveDirection.ResetContent();
	m_comboX1DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboX1DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	
	m_comboX2DriveDirection.ResetContent();
	m_comboX2DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboX2DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	
	m_comboYDriveDirection.ResetContent();
	m_comboYDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboYDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));		//_T("向上")
	
	m_comboCamAxisXDirection.ResetContent();
	m_comboCamAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboCamAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	
	m_comboCamAxisYDirection.ResetContent();
	m_comboCamAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboCamAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));	//_T("向上")

	if (m_bCamPlatformInfoEnable)
	{
		int nCamNum = m_pCamPlatformInfo->GetCamNum();
		for (int i=0; i<6; i++)
		{
			if (i < nCamNum)
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		for (int i=0; i<6; i++)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
		}
		GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
		
	}

	UpdateDlgByAuthorize();

	::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysPlatformX1X2YParam::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	CPlatformX1X2YInfo *pPlatformX1X2YInfo = (CPlatformX1X2YInfo*)m_pPlatformInfo;
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		pPlatformX1X2YInfo->m_nPlatformUVWSliderType = (m_comboUVWSliderType.GetCurSel() == 0) ? eFixedSlider : eShiftSlider;
		pPlatformX1X2YInfo->m_nPlatformX1CoordType = (m_comboX1DriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformX1X2YInfo->m_nPlatformX2CoordType = (m_comboX2DriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformX1X2YInfo->m_nPlatformYCoordType = (m_comboYDriveDirection.GetCurSel() == 0) ? eDirectNegative : eDirectPositive;
		pPlatformX1X2YInfo->m_X1Pos = CCoordPos(atof(m_strX1CoordX), atof(m_strX1CoordY), 0.0);
		pPlatformX1X2YInfo->m_X2Pos = CCoordPos(atof(m_strX2CoordX), atof(m_strX2CoordY), 0.0);
		pPlatformX1X2YInfo->m_YPos = CCoordPos(atof(m_strYCoordX), atof(m_strYCoordY), 0.0);
	}
	else
	{
		if (m_bCamPlatformInfoEnable)
		{
			int nCamNum = m_pCamPlatformInfo->GetCamNum();
			for (int i=0; i<6; i++)
			{
				if (i < nCamNum)
				{
					GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_SHOW);
				}
				else
				{
					GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
				}
			}
		}
		else
		{
			for (int i=0; i<6; i++)
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
			}
			GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);

		}

		m_comboUVWSliderType.SetCurSel((pPlatformX1X2YInfo->m_nPlatformUVWSliderType == eFixedSlider) ? 0 : 1);
		m_comboX1DriveDirection.SetCurSel((pPlatformX1X2YInfo->m_nPlatformX1CoordType == eDirectPositive) ? 0 : 1);
		m_comboX2DriveDirection.SetCurSel((pPlatformX1X2YInfo->m_nPlatformX2CoordType == eDirectPositive) ? 0 : 1);
		m_comboYDriveDirection.SetCurSel((pPlatformX1X2YInfo->m_nPlatformYCoordType == eDirectPositive) ? 1 : 0);
		m_strX1CoordX.Format(_T("%.3f"), pPlatformX1X2YInfo->m_X1Pos.GetPosX());
		m_strX1CoordY.Format(_T("%.3f"), pPlatformX1X2YInfo->m_X1Pos.GetPosY());
		m_strX2CoordX.Format(_T("%.3f"), pPlatformX1X2YInfo->m_X2Pos.GetPosX());
		m_strX2CoordY.Format(_T("%.3f"), pPlatformX1X2YInfo->m_X2Pos.GetPosY());
		m_strYCoordX.Format(_T("%.3f"), pPlatformX1X2YInfo->m_YPos.GetPosX());
		m_strYCoordY.Format(_T("%.3f"), pPlatformX1X2YInfo->m_YPos.GetPosY());


		if (m_bCamPlatformInfoEnable)
		{
			// 相机平台动态显示和设置
			if (m_pCamPlatformInfo->GetCamPlatformType() == eCamPlatformSepFix)
			{
				GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
				for (int i=0; i<6; i++)
				{
					GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
				}
			}
			else if (m_pCamPlatformInfo->GetCamPlatformType() == eCamPlatformSepX)
			{
				GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
				int nCamNum = m_pCamPlatformInfo->GetCamNum();
				for (int i=0; i<6; i++)
				{
					if (i < nCamNum)
					{
						GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_SHOW);
					}
					else
					{
						GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
					}
				}
			
				m_comboCamAxisXDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
			}
			else if (m_pCamPlatformInfo->GetCamPlatformType() == eCamPlatformSepXY)
			{
				GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_SHOW);
				int nCamNum = m_pCamPlatformInfo->GetCamNum();
				for (int i=0; i<6; i++)
				{
					if (i < nCamNum)
					{
						GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_SHOW);
					}
					else
					{
						GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
					}
				}			

				m_comboCamAxisXDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
				m_comboCamAxisYDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
			}
		}


		UpdateData(FALSE);
	}
}

BOOL CDlgSysPlatformX1X2YParam::SetPlatformInfo( const CPlatformInfo* platformInfo )
{
	if (platformInfo->GetPlatformType() != ePlatformX1X2Y)
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
		m_pPlatformInfo = new CPlatformX1X2YInfo(*((CPlatformX1X2YInfo*)platformInfo));
	}
	return TRUE;
}


void CDlgSysPlatformX1X2YParam::OnCameraChange() 
{
	int nCameraIndexOld = m_nCameraIndex;
	UpdateData(TRUE);
	CString strVal[] = {_T("CH0"), _T("CH1"),_T("CH2"), _T("CH3"), _T("CH4"), _T("CH5")};
	if (nCameraIndexOld != m_nCameraIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[相机平台坐标设置]由%s到%s\n"),strVal[nCameraIndexOld],strVal[m_nCameraIndex]);
		g_opLogFile.SaveLogText(strLog,3);
	}
	m_comboCamAxisXDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
	m_comboCamAxisYDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
}

void CDlgSysPlatformX1X2YParam::OnSelchangeComboCamXCoordType() 
{
	//m_comboCamAxisXDirection
	CString strComboCamXCoordType;
	m_comboCamAxisXDirection.GetWindowText(strComboCamXCoordType);

	if (strComboCamXCoordTypeOld != strComboCamXCoordType)
	{
		CString strLog;
		strLog.Format(_T("修改[X轴正方向]由%s到%s\n"),strComboCamXCoordTypeOld,strComboCamXCoordType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboCamXCoordTypeOld = strComboCamXCoordType;
	}
	UpdateData(TRUE);
	PlatformAxisDirectType eDirection = (m_comboCamAxisXDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
	m_pCamPlatformInfo->SetCamDirectX(m_nCameraIndex, eDirection);
}

void CDlgSysPlatformX1X2YParam::OnSelchangeComboCamYCoordType() 
{
	CString strComboCamYCoordType;
	m_comboCamAxisYDirection.GetWindowText(strComboCamYCoordType);

	if (strComboCamYCoordTypeOld != strComboCamYCoordType)
	{
		CString strLog;
		strLog.Format(_T("修改[Y轴正方向]由%s到%s\n"),strComboCamXCoordTypeOld,strComboCamYCoordType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboCamYCoordTypeOld = strComboCamYCoordType;
	}
	UpdateData(TRUE);
	PlatformAxisDirectType eDirection = (m_comboCamAxisYDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
	m_pCamPlatformInfo->SetCamDirectY(m_nCameraIndex, eDirection);
}

void CDlgSysPlatformX1X2YParam::OnSetfocusEditX1CoordX() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X1_COORD_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X1_COORD_X, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X1_COORD_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[X1轴连接点坐标][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditX1CoordX();
	}
}

void CDlgSysPlatformX1X2YParam::OnSetfocusEditX1CoordY() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X1_COORD_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X1_COORD_Y, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X1_COORD_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[X1轴连接点坐标][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditX1CoordY();
	}
}

void CDlgSysPlatformX1X2YParam::OnSetfocusEditX2CoordX() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X2_COORD_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X2_COORD_X, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X2_COORD_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[X2轴连接点坐标][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditX2CoordX();
	}
}

void CDlgSysPlatformX1X2YParam::OnSetfocusEditX2CoordY() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X2_COORD_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X2_COORD_Y, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X2_COORD_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[X2轴连接点坐标][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditX2CoordY();
	}
}

void CDlgSysPlatformX1X2YParam::OnSetfocusEditYCoordX() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_COORD_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_COORD_X, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_COORD_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[Y轴连接点坐标][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditYCoordX();
	}
}

void CDlgSysPlatformX1X2YParam::OnSetfocusEditYCoordY() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_COORD_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_COORD_Y, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_COORD_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[Y轴连接点坐标][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditYCoordY();
	}
}

void CDlgSysPlatformX1X2YParam::OnKillfocusEditX1CoordX() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X1_COORD_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformX1X2YInfo*)m_pPlatformInfo)->GetX1Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosX);
			SetDlgItemText(IDC_EDIT_X1_COORD_X, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X1_COORD_X, strVal);
		}
	}
}

void CDlgSysPlatformX1X2YParam::OnKillfocusEditX1CoordY() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X1_COORD_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformX1X2YInfo*)m_pPlatformInfo)->GetX1Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosY);
			SetDlgItemText(IDC_EDIT_X1_COORD_Y, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X1_COORD_Y, strVal);
		}
	}
}

void CDlgSysPlatformX1X2YParam::OnKillfocusEditX2CoordX() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X2_COORD_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformX1X2YInfo*)m_pPlatformInfo)->GetX2Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosX);
			SetDlgItemText(IDC_EDIT_X2_COORD_X, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X2_COORD_X, strVal);
		}
	}
}

void CDlgSysPlatformX1X2YParam::OnKillfocusEditX2CoordY() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X2_COORD_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformX1X2YInfo*)m_pPlatformInfo)->GetX2Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosY);
			SetDlgItemText(IDC_EDIT_X2_COORD_Y, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X2_COORD_Y, strVal);
		}
	}
}

void CDlgSysPlatformX1X2YParam::OnKillfocusEditYCoordX() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_COORD_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformX1X2YInfo*)m_pPlatformInfo)->GetYPos();
			strVal.Format(_T("%.3f"), pos.m_dPosX);
			SetDlgItemText(IDC_EDIT_Y_COORD_X, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_COORD_X, strVal);
		}
	}
}

void CDlgSysPlatformX1X2YParam::OnKillfocusEditYCoordY() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_COORD_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformX1X2YInfo*)m_pPlatformInfo)->GetYPos();
			strVal.Format(_T("%.3f"), pos.m_dPosY);
			SetDlgItemText(IDC_EDIT_Y_COORD_Y, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_COORD_Y, strVal);
		}
	}
}

BOOL CDlgSysPlatformX1X2YParam::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboUvwSliderType()
{
	// TODO: 在此添加控件通知处理程序代码m_comboUVWSliderType
	CString strComboUvwSliderType;
	m_comboUVWSliderType.GetWindowText(strComboUvwSliderType);

	if (strComboUvwSliderTypeOld != strComboUvwSliderType)
	{
		CString strLog;
		strLog.Format(_T("修改[结构类型]由%s到%s\n"),strComboUvwSliderTypeOld,strComboUvwSliderType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboUvwSliderTypeOld = strComboUvwSliderType;
	}
}


void CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboX1DriveDirection()
{
	// TODO: 在此添加控件通知处理程序代码m_comboX1DriveDirection
	CString strComboX1DriveDirection;
	m_comboX1DriveDirection.GetWindowText(strComboX1DriveDirection);

	if (strComboX1DriveDirectionOld != strComboX1DriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[X1轴正方向]由%s到%s\n"),strComboX1DriveDirectionOld,strComboX1DriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboX1DriveDirectionOld = strComboX1DriveDirection;
	}
}


void CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboX2DriveDirection()
{
	// TODO: 在此添加控件通知处理程序代码m_comboX2DriveDirection
	CString strComboX2DriveDirection;
	m_comboX2DriveDirection.GetWindowText(strComboX2DriveDirection);

	if (strComboX2DriveDirectionOld != strComboX2DriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[X2轴正方向]由%s到%s\n"),strComboX2DriveDirectionOld,strComboX2DriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboX2DriveDirectionOld = strComboX2DriveDirection;
	}
}


void CDlgSysPlatformX1X2YParam::OnCbnSelchangeComboYDriveDirection()
{
	// TODO: 在此添加控件通知处理程序代码m_comboYDriveDirection
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

void CDlgSysPlatformX1X2YParam::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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

void CDlgSysPlatformX1X2YParam::UpdateDlgByAuthorize()
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
				SetGroupBoxStatus(IDC_STATIC_GRP_PLATFORM_UVW_SLIDER_TYPE,bEnable,bShow);
				SetGroupBoxStatus(IDC_STATIC_GRP_PLATFORM_COORD,bEnable,bShow);
				SetGroupBoxStatus(IDC_STATIC_GRP_PLATFORM_DETAIL,bEnable,bShow);

			}

		}
	}
}