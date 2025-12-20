// DlgSysPlatformXY1Y2Param.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysPlatformXY1Y2Param.h"
#include "DlgSysEnvironSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformXY1Y2Param dialog


CDlgSysPlatformXY1Y2Param::CDlgSysPlatformXY1Y2Param(CWnd* pParent /*=NULL*/)
	: CDlgSysPlatformParam(CDlgSysPlatformXY1Y2Param::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysPlatformXY1Y2Param)
	m_nCameraIndex = 0;
	m_strXCoordX = _T("");
	m_strXCoordY = _T("");
	m_strY1CoordX = _T("");
	m_strY2CoordX = _T("");
	m_strY1CoordY = _T("");
	m_strY2CoordY = _T("");
	//}}AFX_DATA_INIT
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
}

void CDlgSysPlatformXY1Y2Param::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_PLATFORM_XY1Y2_PARAM") == strDlgID)
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

void CDlgSysPlatformXY1Y2Param::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysPlatformXY1Y2Param)
	DDX_Control(pDX, IDC_COMBO_UVW_SLIDER_TYPE, m_comboUVWSliderType);
	DDX_Control(pDX, IDC_COMBO_Y2_DRIVE_DIRECTION, m_comboY2DriveDirection);
	DDX_Control(pDX, IDC_COMBO_Y1_DRIVE_DIRECTION, m_comboY1DriveDirection);
	DDX_Control(pDX, IDC_COMBO_X_DRIVE_DIRECTION, m_comboXDriveDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_Y_COORD_TYPE, m_comboCamAxisYDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_X_COORD_TYPE, m_comboCamAxisXDirection);
	DDX_Radio(pDX, IDC_RADIO_CAMERA0, m_nCameraIndex);
	DDX_Text(pDX, IDC_EDIT_X_COORD_X, m_strXCoordX);
	DDX_Text(pDX, IDC_EDIT_X_COORD_Y, m_strXCoordY);
	DDX_Text(pDX, IDC_EDIT_Y1_COORD_X, m_strY1CoordX);
	DDX_Text(pDX, IDC_EDIT_Y1_COORD_Y, m_strY1CoordY);
	DDX_Text(pDX, IDC_EDIT_Y2_COORD_X, m_strY2CoordX);
	DDX_Text(pDX, IDC_EDIT_Y2_COORD_Y, m_strY2CoordY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysPlatformXY1Y2Param, CDialog)
	//{{AFX_MSG_MAP(CDlgSysPlatformXY1Y2Param)
	ON_BN_CLICKED(IDC_RADIO_CAMERA0, OnCameraChange)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_X_COORD_TYPE, OnSelchangeComboCamXCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_Y_COORD_TYPE, OnSelchangeComboCamYCoordType)
	ON_EN_SETFOCUS(IDC_EDIT_X_COORD_X, OnSetfocusEditXCoordX)
	ON_EN_SETFOCUS(IDC_EDIT_X_COORD_Y, OnSetfocusEditXCoordY)
	ON_EN_SETFOCUS(IDC_EDIT_Y1_COORD_X, OnSetfocusEditY1CoordX)
	ON_EN_SETFOCUS(IDC_EDIT_Y1_COORD_Y, OnSetfocusEditY1CoordY)
	ON_EN_SETFOCUS(IDC_EDIT_Y2_COORD_X, OnSetfocusEditY2CoordX)
	ON_EN_SETFOCUS(IDC_EDIT_Y2_COORD_Y, OnSetfocusEditY2CoordY)
	ON_BN_CLICKED(IDC_RADIO_CAMERA1, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA2, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA3, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA4, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA5, OnCameraChange)
	ON_EN_KILLFOCUS(IDC_EDIT_X_COORD_X, OnKillfocusEditXCoordX)
	ON_EN_KILLFOCUS(IDC_EDIT_X_COORD_Y, OnKillfocusEditXCoordY)
	ON_EN_KILLFOCUS(IDC_EDIT_Y1_COORD_X, OnKillfocusEditY1CoordX)
	ON_EN_KILLFOCUS(IDC_EDIT_Y1_COORD_Y, OnKillfocusEditY1CoordY)
	ON_EN_KILLFOCUS(IDC_EDIT_Y2_COORD_X, OnKillfocusEditY2CoordX)
	ON_EN_KILLFOCUS(IDC_EDIT_Y2_COORD_Y, OnKillfocusEditY2CoordY)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_UVW_SLIDER_TYPE, &CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboUvwSliderType)
	ON_CBN_SELCHANGE(IDC_COMBO_X_DRIVE_DIRECTION, &CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboXDriveDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_Y1_DRIVE_DIRECTION, &CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboY1DriveDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_Y2_DRIVE_DIRECTION, &CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboY2DriveDirection)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformXY1Y2Param message handlers

BOOL CDlgSysPlatformXY1Y2Param::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);
	
	m_comboUVWSliderType.GetWindowText(strComboUvwSliderTypeOld);
	m_comboXDriveDirection.GetWindowText(strComboXDriveDirectionOld);
	m_comboY1DriveDirection.GetWindowText(strComboY1DriveDirectionOld);
	m_comboY2DriveDirection.GetWindowText(strComboY2DriveDirectionOld);
	m_comboCamAxisXDirection.GetWindowText(strComboCamXCoordTypeOld);
	m_comboCamAxisYDirection.GetWindowText(strComboCamYCoordTypeOld);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysPlatformXY1Y2Param::InitDlgItem()
{
	m_comboUVWSliderType.ResetContent();
	m_comboUVWSliderType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SLIDER_NO_MOVE)/*_T("固定滑块")*/);	//类型1
	m_comboUVWSliderType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SLIDER_MOVE)/*_T("移动滑块")*/);	//类型2

	m_comboXDriveDirection.ResetContent();
	m_comboXDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboXDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	
	m_comboY1DriveDirection.ResetContent();
	m_comboY1DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboY1DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));		//_T("向上")
	
	m_comboY2DriveDirection.ResetContent();
	m_comboY2DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboY2DriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));		//_T("向上")
	
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

void CDlgSysPlatformXY1Y2Param::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	CPlatformXY1Y2Info *pPlatformXY1Y2Info = (CPlatformXY1Y2Info*)m_pPlatformInfo;
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		pPlatformXY1Y2Info->m_nPlatformUVWSliderType = (m_comboUVWSliderType.GetCurSel() == 0) ? eFixedSlider : eShiftSlider;
		pPlatformXY1Y2Info->m_nPlatformXCoordType = (m_comboXDriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformXY1Y2Info->m_nPlatformY1CoordType = (m_comboY1DriveDirection.GetCurSel() == 0) ? eDirectNegative : eDirectPositive;
		pPlatformXY1Y2Info->m_nPlatformY2CoordType = (m_comboY2DriveDirection.GetCurSel() == 0) ? eDirectNegative : eDirectPositive;
		pPlatformXY1Y2Info->m_XPos = CCoordPos(atof(m_strXCoordX), atof(m_strXCoordY), 0.0);
		pPlatformXY1Y2Info->m_Y1Pos = CCoordPos(atof(m_strY1CoordX), atof(m_strY1CoordY), 0.0);
		pPlatformXY1Y2Info->m_Y2Pos = CCoordPos(atof(m_strY2CoordX), atof(m_strY2CoordY), 0.0);
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

		m_comboUVWSliderType.SetCurSel((pPlatformXY1Y2Info->m_nPlatformUVWSliderType == eFixedSlider) ? 0 : 1);
		m_comboXDriveDirection.SetCurSel((pPlatformXY1Y2Info->m_nPlatformXCoordType == eDirectPositive) ? 0 : 1);
		m_comboY1DriveDirection.SetCurSel((pPlatformXY1Y2Info->m_nPlatformY1CoordType == eDirectPositive) ? 1 : 0);
		m_comboY2DriveDirection.SetCurSel((pPlatformXY1Y2Info->m_nPlatformY2CoordType == eDirectPositive) ? 1 : 0);
		m_strXCoordX.Format(_T("%.3f"), pPlatformXY1Y2Info->m_XPos.GetPosX());
		m_strXCoordY.Format(_T("%.3f"), pPlatformXY1Y2Info->m_XPos.GetPosY());
		m_strY1CoordX.Format(_T("%.3f"), pPlatformXY1Y2Info->m_Y1Pos.GetPosX());
		m_strY1CoordY.Format(_T("%.3f"), pPlatformXY1Y2Info->m_Y1Pos.GetPosY());
		m_strY2CoordX.Format(_T("%.3f"), pPlatformXY1Y2Info->m_Y2Pos.GetPosX());
		m_strY2CoordY.Format(_T("%.3f"), pPlatformXY1Y2Info->m_Y2Pos.GetPosY());

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

BOOL CDlgSysPlatformXY1Y2Param::SetPlatformInfo( const CPlatformInfo* platformInfo )
{
	if (platformInfo->GetPlatformType() != ePlatformXY1Y2)
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
		m_pPlatformInfo = new CPlatformXY1Y2Info(*((CPlatformXY1Y2Info*)platformInfo));
	}
	return TRUE;
}


void CDlgSysPlatformXY1Y2Param::OnCameraChange() 
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

	UpdateData(TRUE);
	m_comboCamAxisXDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
	m_comboCamAxisYDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
}

void CDlgSysPlatformXY1Y2Param::OnSelchangeComboCamXCoordType() 
{
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

void CDlgSysPlatformXY1Y2Param::OnSelchangeComboCamYCoordType() 
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

void CDlgSysPlatformXY1Y2Param::OnSetfocusEditXCoordX() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_COORD_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_COORD_X, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_COORD_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[X轴连接点坐标][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditXCoordX();
	}
}

void CDlgSysPlatformXY1Y2Param::OnSetfocusEditXCoordY() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_COORD_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_COORD_Y, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_COORD_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[X轴连接点坐标][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditXCoordY();
	}
}

void CDlgSysPlatformXY1Y2Param::OnSetfocusEditY1CoordX() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y1_COORD_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y1_COORD_X, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y1_COORD_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[Y1轴连接点坐标][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditY1CoordX();
	}
}

void CDlgSysPlatformXY1Y2Param::OnSetfocusEditY1CoordY() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y1_COORD_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y1_COORD_Y, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y1_COORD_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[Y1轴连接点坐标][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditY1CoordY();
	}
}

void CDlgSysPlatformXY1Y2Param::OnSetfocusEditY2CoordX() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y2_COORD_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y2_COORD_X, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y2_COORD_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[Y2轴连接点坐标][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditY2CoordX();
	}
}

void CDlgSysPlatformXY1Y2Param::OnSetfocusEditY2CoordY() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y2_COORD_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y2_COORD_Y, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y2_COORD_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[Y2轴连接点坐标][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditY2CoordY();
	}
}

// Kill Focus
void CDlgSysPlatformXY1Y2Param::OnKillfocusEditXCoordX() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_COORD_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformXY1Y2Info*)m_pPlatformInfo)->GetXPos();
			strVal.Format(_T("%.3f"), pos.m_dPosX);
			SetDlgItemText(IDC_EDIT_X_COORD_X, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_COORD_X, strVal);
		}
	}
}

void CDlgSysPlatformXY1Y2Param::OnKillfocusEditXCoordY() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_COORD_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformXY1Y2Info*)m_pPlatformInfo)->GetXPos();
			strVal.Format(_T("%.3f"), pos.m_dPosY);
			SetDlgItemText(IDC_EDIT_X_COORD_Y, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_COORD_Y, strVal);
		}
	}
}

void CDlgSysPlatformXY1Y2Param::OnKillfocusEditY1CoordX() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y1_COORD_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformXY1Y2Info*)m_pPlatformInfo)->GetY1Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosX);
			SetDlgItemText(IDC_EDIT_Y1_COORD_X, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y1_COORD_X, strVal);
		}
	}
}

void CDlgSysPlatformXY1Y2Param::OnKillfocusEditY1CoordY() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y1_COORD_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformXY1Y2Info*)m_pPlatformInfo)->GetY1Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosY);
			SetDlgItemText(IDC_EDIT_Y1_COORD_Y, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y1_COORD_Y, strVal);
		}
	}
}

void CDlgSysPlatformXY1Y2Param::OnKillfocusEditY2CoordX() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y2_COORD_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformXY1Y2Info*)m_pPlatformInfo)->GetY2Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosX);
			SetDlgItemText(IDC_EDIT_Y2_COORD_X, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y2_COORD_X, strVal);
		}
	}
}

void CDlgSysPlatformXY1Y2Param::OnKillfocusEditY2CoordY() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y2_COORD_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			CCoordPos pos = ((CPlatformXY1Y2Info*)m_pPlatformInfo)->GetY2Pos();
			strVal.Format(_T("%.3f"), pos.m_dPosY);
			SetDlgItemText(IDC_EDIT_Y2_COORD_Y, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y2_COORD_Y, strVal);
		}
	}
}

BOOL CDlgSysPlatformXY1Y2Param::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboUvwSliderType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboUvwSliderType;
	m_comboUVWSliderType.GetWindowText(strComboUvwSliderType);

	if (strComboUvwSliderTypeOld != strComboUvwSliderType)
	{
		CString strLog;
		strLog.Format(_T("修改[命令]由%s到%s\n"),strComboUvwSliderTypeOld,strComboUvwSliderType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboUvwSliderTypeOld = strComboUvwSliderType;
	}
}


void CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboXDriveDirection()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboXDriveDirection;
	m_comboXDriveDirection.GetWindowText(strComboXDriveDirection);

	if (strComboXDriveDirectionOld != strComboXDriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[X1正方向]由%s到%s\n"),strComboXDriveDirectionOld,strComboXDriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboXDriveDirectionOld = strComboXDriveDirection;
	}
}


void CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboY1DriveDirection()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboY1DriveDirection;
	m_comboY1DriveDirection.GetWindowText(strComboY1DriveDirection);

	if (strComboY1DriveDirectionOld != strComboY1DriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[X1正方向]由%s到%s\n"),strComboY1DriveDirectionOld,strComboY1DriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboY1DriveDirectionOld = strComboY1DriveDirection;
	}
}


void CDlgSysPlatformXY1Y2Param::OnCbnSelchangeComboY2DriveDirection()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboY2DriveDirection;
	m_comboY2DriveDirection.GetWindowText(strComboY2DriveDirection);

	if (strComboY2DriveDirectionOld != strComboY2DriveDirection)
	{
		CString strLog;
		strLog.Format(_T("修改[X1正方向]由%s到%s\n"),strComboY2DriveDirectionOld,strComboY2DriveDirection);
		g_opLogFile.SaveLogText(strLog,3);
		strComboY2DriveDirectionOld = strComboY2DriveDirection;
	}
}


void CDlgSysPlatformXY1Y2Param::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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

void CDlgSysPlatformXY1Y2Param::UpdateDlgByAuthorize()
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