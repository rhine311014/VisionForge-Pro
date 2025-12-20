// DlgSysPlatformXYDParam.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysPlatformXYDParam.h"
#include "DlgSysEnvironSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformXYDParam dialog


CDlgSysPlatformXYDParam::CDlgSysPlatformXYDParam(CWnd* pParent /*=NULL*/)
	: CDlgSysPlatformParam(CDlgSysPlatformXYDParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysPlatformXYDParam)
	m_nCameraIndex = 0;
	m_strRotationLength = _T("");

	m_bshowPlatformParamSet = true;
	m_bEnablePlatformParamSet = true; 

	m_bshowCamPlatformParamSet= true;
	m_bEnableCamPlatformParamSet= true;
	//}}AFX_DATA_INIT
}

void CDlgSysPlatformXYDParam::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_PLATFORM_XYD_PARAM") == strDlgID)
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

void CDlgSysPlatformXYDParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysPlatformXYDParam)
	DDX_Control(pDX, IDC_COMBO_DRIVE_TYPE, m_comboRDriveType);
	DDX_Control(pDX, IDC_COMBO_DRIVE_LINE_TYPE, m_comboRDriveLineType);
	DDX_Control(pDX, IDC_COMBO_Y_COORD_TYPE, m_comboYDriveDirection);
	DDX_Control(pDX, IDC_COMBO_X_COORD_TYPE, m_comboXDriveDirection);
	DDX_Control(pDX, IDC_COMBO_R_DIRECTION, m_comboRDriveDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_Y_COORD_TYPE, m_comboCamAxisYDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_X_COORD_TYPE, m_comboCamAxisXDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_Y_COORD_TYPE2, m_comboCamPlatformAxisYDirection);
	DDX_Control(pDX, IDC_COMBO_CAM_X_COORD_TYPE2, m_comboCamPlatformAxisXDirection);
	DDX_Radio(pDX, IDC_RADIO_CAMERA0, m_nCameraIndex);
	DDX_Text(pDX, IDC_EDIT_ROTATION_LENGTH, m_strRotationLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysPlatformXYDParam, CDialog)
	//{{AFX_MSG_MAP(CDlgSysPlatformXYDParam)
	ON_BN_CLICKED(IDC_RADIO_CAMERA0, OnCameraChange)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_X_COORD_TYPE, OnSelchangeComboCamXCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_Y_COORD_TYPE, OnSelchangeComboCamYCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_X_COORD_TYPE2, OnSelchangeComboCamXCoordType2)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_Y_COORD_TYPE2, OnSelchangeComboCamYCoordType2)
	ON_EN_SETFOCUS(IDC_EDIT_ROTATION_LENGTH, OnSetfocusEditRotationLength)
	ON_BN_CLICKED(IDC_RADIO_CAMERA1, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA2, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA3, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA4, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA5, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA6, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA7, OnCameraChange)
	ON_EN_KILLFOCUS(IDC_EDIT_ROTATION_LENGTH, OnKillfocusEditRotationLength)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE_TYPE, &CDlgSysPlatformXYDParam::OnCbnSelchangeComboDriveType)
	ON_CBN_SELCHANGE(IDC_COMBO_X_COORD_TYPE, &CDlgSysPlatformXYDParam::OnCbnSelchangeComboXCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_Y_COORD_TYPE, &CDlgSysPlatformXYDParam::OnCbnSelchangeComboYCoordType)
	ON_CBN_SELCHANGE(IDC_COMBO_R_DIRECTION, &CDlgSysPlatformXYDParam::OnCbnSelchangeComboRDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE_LINE_TYPE, &CDlgSysPlatformXYDParam::OnCbnSelchangeComboDriveLineType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformXYDParam message handlers

BOOL CDlgSysPlatformXYDParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);
	
	CString strComboCamXCoordType;
	m_comboCamAxisXDirection.GetWindowText(strComboCamXCoordType);
	strComboCamXCoordTypeOld = strComboCamXCoordType;

	CString strComboCamYCoordType;
	m_comboCamAxisXDirection.GetWindowText(strComboCamYCoordType);
	strComboCamYCoordTypeOld = strComboCamYCoordType;

	CString strComboXCoordType;
	m_comboXDriveDirection.GetWindowText(strComboXCoordType);
	strComboXCoordTypeOld = strComboXCoordType;

	CString strComboYCoordType;
	m_comboYDriveDirection.GetWindowText(strComboYCoordType);
	strComboYCoordTypeOld = strComboYCoordType;

	CString strComboRDirection;
	m_comboRDriveDirection.GetWindowText(strComboRDirection);
	strComboRCoordTypeOld = strComboRDirection;

	CString strComboDriveType;
	m_comboRDriveType.GetWindowText(strComboDriveType);
	strComboDriveTypeOld = strComboDriveType;

	CString strComboDriveLineType;
	m_comboRDriveLineType.GetWindowText(strComboDriveLineType);
	strComboDriveLineTypeOld = strComboDriveLineType;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysPlatformXYDParam::InitDlgItem()
{
	UpdateDlgByAuthorize();
	m_comboXDriveDirection.ResetContent();
	m_comboXDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboXDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")

	m_comboYDriveDirection.ResetContent();
	m_comboYDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboYDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));		//_T("向上")

	m_comboRDriveDirection.ResetContent();
	m_comboRDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CLOCKWISE));		//_T("顺时针")
	m_comboRDriveDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COUNTER_CLOCKWISE));//_T("逆时针")

	m_comboRDriveType.ResetContent();
	m_comboRDriveType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DIRECTIVE_DRIVE));		//_T("直接驱动")
	m_comboRDriveType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINEAR_DRIVE));			//_T("直线驱动")

	m_comboRDriveLineType.ResetContent();
	m_comboRDriveLineType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_D_DRIVE_LINE_TYPE_FIXED));		//_T("连接点固定在直线轴上")
	m_comboRDriveLineType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_D_DRIVE_LINE_TYPE_SHIFT));		//_T("连接点相对直线轴移动")


	m_comboCamAxisXDirection.ResetContent();
	m_comboCamAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboCamAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	m_comboCamAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_NONE));
	m_comboCamAxisYDirection.ResetContent();
	m_comboCamAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboCamAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));	//_T("向上")
	m_comboCamAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_NONE));

	m_comboCamPlatformAxisXDirection.ResetContent();
	m_comboCamPlatformAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_RIGHT));	//_T("向右")
	m_comboCamPlatformAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_LEFT));	//_T("向左")
	m_comboCamPlatformAxisXDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_NONE));
	m_comboCamPlatformAxisYDirection.ResetContent();
	m_comboCamPlatformAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_DOWN));	//_T("向下")
	m_comboCamPlatformAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_UP));	//_T("向上")
	m_comboCamPlatformAxisYDirection.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_POINTING_NONE));
	if (m_bCamPlatformInfoEnable)
	{
		
		for (int i=0; i<8; i++)
		{
		
			GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(m_bshowPlatformParamSet);
	
		}

		
		//GetDlgItem(IDC_RADIO_CAMERA_6)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_RADIO_CAMERA_7)->ShowWindow(SW_SHOW);
	}
	else
	{
		for (int i=0; i<8; i++)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
		}
		//GetDlgItem(IDC_RADIO_CAMERA_6)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_RADIO_CAMERA_7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_CAM_PLATFORM_AXIS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_GRP_PLATFORM_COORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_X_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_X_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_R_DIRECTION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_R_DIRECTION)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_GRP_PLATFORM_DETAIL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DRIVE_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_DRIVE_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DRIVE_LINE_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_DRIVE_LINE_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ROTATION_LENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ROTATION_LENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MM)->ShowWindow(SW_HIDE);
		
	}

	

	::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysPlatformXYDParam::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	CPlatformXYDInfo *pPlatformXYDInfo = (CPlatformXYDInfo*)m_pPlatformInfo;
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		pPlatformXYDInfo->m_nPlatformXCoordType = (m_comboXDriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformXYDInfo->m_nPlatformYCoordType = (m_comboYDriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformXYDInfo->m_nPlatformDCoordType = (m_comboRDriveDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
		pPlatformXYDInfo->m_nPlatformDDriveType = (PlatformDDriveType)m_comboRDriveType.GetCurSel();
		pPlatformXYDInfo->m_nPlatformDDriveLineType = (PlatformDDriveLineType)m_comboRDriveLineType.GetCurSel();
		pPlatformXYDInfo->m_dPlatformRotationLength = atof(m_strRotationLength);
	}
	else
	{

		if (m_bCamPlatformInfoEnable)
		{
			int nCamNum = m_pVisionASM->GetPosNum();
			for (int i=0; i<8; i++)
			{
                if (i<nCamNum)
                {
                    GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(m_bshowPlatformParamSet);
                }
                else
                {
					GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
                }
			}

			//GetDlgItem(IDC_RADIO_CAMERA_6)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_RADIO_CAMERA_7)->ShowWindow(SW_SHOW);
		}
		else
		{
			for (int i=0; i<8; i++)
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
			}
			//GetDlgItem(IDC_RADIO_CAMERA_6)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_RADIO_CAMERA_7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE2)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CAM_PLATFORM_AXIS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_GRP_PLATFORM_COORD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_X_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_X_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_R_DIRECTION)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_R_DIRECTION)->ShowWindow(SW_HIDE);


			GetDlgItem(IDC_STATIC_GRP_PLATFORM_DETAIL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_DRIVE_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_DRIVE_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_DRIVE_LINE_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_DRIVE_LINE_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_ROTATION_LENGTH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_ROTATION_LENGTH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MM)->ShowWindow(SW_HIDE);


		}


		m_comboXDriveDirection.SetCurSel((pPlatformXYDInfo->m_nPlatformXCoordType == eDirectPositive) ? 0 : 1);
		m_comboYDriveDirection.SetCurSel((pPlatformXYDInfo->m_nPlatformYCoordType == eDirectPositive) ? 0 : 1);
		m_comboRDriveDirection.SetCurSel((pPlatformXYDInfo->m_nPlatformDCoordType == eDirectPositive) ? 0 : 1);
		m_comboRDriveType.SetCurSel((int)pPlatformXYDInfo->m_nPlatformDDriveType);
		m_comboRDriveLineType.SetCurSel((int)pPlatformXYDInfo->m_nPlatformDDriveLineType);
		m_strRotationLength.Format(_T("%.6f"), pPlatformXYDInfo->m_dPlatformRotationLength);

		GetDlgItem(IDC_EDIT_ROTATION_LENGTH)->EnableWindow(pPlatformXYDInfo->m_nPlatformDDriveType == eDriveLine);
		GetDlgItem(IDC_COMBO_DRIVE_LINE_TYPE)->EnableWindow(pPlatformXYDInfo->m_nPlatformDDriveType == eDriveLine);

		BOOL bShowXY =TRUE;
		int nShowXY = SW_HIDE;
		if (NULL !=m_pVisionASM)
		{
			CVisionASMConfig *vsConfig;
			m_pVisionASM->GetVSconfig(vsConfig);
			if (NULL !=vsConfig)
			{
				bool bShow = false;
				bool bEnable = false;
				CString strNodeName = _T("");

				// 相机平台参数
				bool bFind = vsConfig->GetCheckedByItem(ePlatformAuthorize_Camplatform,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
				if (bFind)
				{
					bShowXY = bShowXY && bShow;
				}
			}
		}
		if (bShowXY)
		{
			nShowXY = SW_SHOW;
		}

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
                GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE2)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE2)->ShowWindow(SW_HIDE);

                GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_STC_CAM_PLATFORM_AXIS)->ShowWindow(SW_HIDE);
				
				for (int i=0; i<8; i++)
				{
					GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
				}
				//GetDlgItem(IDC_RADIO_CAMERA_6)->ShowWindow(SW_HIDE);
				//GetDlgItem(IDC_RADIO_CAMERA_7)->ShowWindow(SW_HIDE);
			}
			else if (m_pCamPlatformInfo->GetCamPlatformType() == eCamPlatformSepX)
			{
				GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(nShowXY);
				GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(nShowXY);
				GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(nShowXY);
				GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(nShowXY);
				GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE2)->ShowWindow(nShowXY);
				GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);

                GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE2)->ShowWindow(nShowXY);
                GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE2)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_STC_CAM_PLATFORM_AXIS)->ShowWindow(nShowXY);
				int nCamNum = m_pVisionASM->GetPosNum();
				for (int i=0; i<8; i++)
				{
                    if (i<nCamNum)
                    {
                        GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(nShowXY);
                    }
                    else
                    {
                        GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
                    }
						

				}
				//GetDlgItem(IDC_RADIO_CAMERA_6)->ShowWindow(SW_SHOW);
				//GetDlgItem(IDC_RADIO_CAMERA_7)->ShowWindow(SW_SHOW);
				m_comboCamAxisXDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
			}
			else if (m_pCamPlatformInfo->GetCamPlatformType() == eCamPlatformSepXY)
			{
				GetDlgItem(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG)->ShowWindow(nShowXY);
				GetDlgItem(IDC_STATIC_GRP_NONAME)->ShowWindow(nShowXY);
				GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE)->ShowWindow(nShowXY);
				GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE)->ShowWindow(nShowXY);
				GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE)->ShowWindow(nShowXY);
				GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE)->ShowWindow(nShowXY);
				GetDlgItem(IDC_COMBO_CAM_X_COORD_TYPE2)->ShowWindow(nShowXY);
				GetDlgItem(IDC_COMBO_CAM_Y_COORD_TYPE2)->ShowWindow(nShowXY);
                GetDlgItem(IDC_STATIC_CAM_X_COORD_TYPE2)->ShowWindow(nShowXY);
                GetDlgItem(IDC_STATIC_CAM_Y_COORD_TYPE2)->ShowWindow(nShowXY);
                GetDlgItem(IDC_STC_CAM_PLATFORM_AXIS)->ShowWindow(nShowXY);
				int nCamNum = m_pVisionASM->GetPosNum();
				for (int i=0; i<8; i++)
				{
                    if (i<nCamNum)
                    {
                        GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(nShowXY);
                    }
                    else
                    {
						GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
                    }

				}			
				//GetDlgItem(IDC_RADIO_CAMERA_6)->ShowWindow(SW_SHOW);
				//GetDlgItem(IDC_RADIO_CAMERA_7)->ShowWindow(SW_SHOW);

				if(m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive)
				{
					m_comboCamAxisXDirection.SetCurSel(0);
				}
				else if (m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectNegative)
				{
					m_comboCamAxisXDirection.SetCurSel(1);
				}
				else
				{
					m_comboCamAxisXDirection.SetCurSel(2);
				}
				if(m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectPositive)
				{
					m_comboCamAxisYDirection.SetCurSel(0);
				}
				else if (m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectNegative)
				{
					m_comboCamAxisYDirection.SetCurSel(1);
				}
				else
				{
					m_comboCamAxisYDirection.SetCurSel(2);
				}

				if(m_pCamPlatformInfo->GetCamPlatformAxisDirectX(m_nCameraIndex) == eDirectPositive)
				{
					m_comboCamPlatformAxisXDirection.SetCurSel(0);
				}
				else if (m_pCamPlatformInfo->GetCamPlatformAxisDirectX(m_nCameraIndex) == eDirectNegative)
				{
					m_comboCamPlatformAxisXDirection.SetCurSel(1);
				}
				else
				{
					m_comboCamPlatformAxisXDirection.SetCurSel(2);
				}
				if(m_pCamPlatformInfo->GetCamPlatformAxisDirectY(m_nCameraIndex) == eDirectPositive)
				{
					m_comboCamPlatformAxisYDirection.SetCurSel(0);
				}
				else if (m_pCamPlatformInfo->GetCamPlatformAxisDirectY(m_nCameraIndex) == eDirectNegative)
				{
					m_comboCamPlatformAxisYDirection.SetCurSel(1);
				}
				else
				{
					m_comboCamPlatformAxisYDirection.SetCurSel(2);
				}

				//m_comboCamAxisXDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
				//m_comboCamAxisYDirection.SetCurSel((m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectPositive) ? 0 : 1);
			}
		}

		UpdateData(FALSE);
	}
}

BOOL CDlgSysPlatformXYDParam::SetPlatformInfo( const CPlatformInfo* platformInfo )
{
	if (platformInfo->GetPlatformType() != ePlatformXYD
		&& platformInfo->GetPlatformType() != ePlatformXDPY
		&& platformInfo->GetPlatformType() != ePlatformXDPY1
		&& platformInfo->GetPlatformType() != ePlatformXYPD
		&& platformInfo->GetPlatformType() != ePlatformXY
		&& platformInfo->GetPlatformType() != ePlatformX
		&& platformInfo->GetPlatformType() != ePlatformY )
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
		m_pPlatformInfo = new CPlatformXYDInfo(*((CPlatformXYDInfo*)platformInfo));
	}
	return TRUE;
}


void CDlgSysPlatformXYDParam::OnCameraChange() 
{
	int nCameraIndexOld = m_nCameraIndex;
	UpdateData(TRUE);
	CString strVal[] = {_T("CH0"), _T("CH1"),_T("CH2"), _T("CH3"), _T("CH4"), _T("CH5"),_T("CH6"),_T("CH7")};
	if (nCameraIndexOld != m_nCameraIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[相机平台坐标设置]由%s到%s\n"),strVal[nCameraIndexOld],strVal[m_nCameraIndex]);
		g_opLogFile.SaveLogText(strLog,3);
	}
	UpdateData(TRUE);
	if(m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectPositive)
	{
		m_comboCamAxisXDirection.SetCurSel(0);
	}
	else if (m_pCamPlatformInfo->GetCamDirectX(m_nCameraIndex) == eDirectNegative)
	{
		m_comboCamAxisXDirection.SetCurSel(1);
	}
	else
	{
		m_comboCamAxisXDirection.SetCurSel(2);
	}
	if(m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectPositive)
	{
		m_comboCamAxisYDirection.SetCurSel(0);
	}
	else if (m_pCamPlatformInfo->GetCamDirectY(m_nCameraIndex) == eDirectNegative)
	{
		m_comboCamAxisYDirection.SetCurSel(1);
	}
	else
	{
		m_comboCamAxisYDirection.SetCurSel(2);
	}


	if(m_pCamPlatformInfo->GetCamPlatformAxisDirectX(m_nCameraIndex) == eDirectPositive)
	{
		m_comboCamPlatformAxisXDirection.SetCurSel(0);
	}
	else if (m_pCamPlatformInfo->GetCamPlatformAxisDirectX(m_nCameraIndex) == eDirectNegative)
	{
		m_comboCamPlatformAxisXDirection.SetCurSel(1);
	}
	else
	{
		m_comboCamPlatformAxisXDirection.SetCurSel(2);
	}
	if(m_pCamPlatformInfo->GetCamPlatformAxisDirectY(m_nCameraIndex) == eDirectPositive)
	{
		m_comboCamPlatformAxisYDirection.SetCurSel(0);
	}
	else if (m_pCamPlatformInfo->GetCamPlatformAxisDirectY(m_nCameraIndex) == eDirectNegative)
	{
		m_comboCamPlatformAxisYDirection.SetCurSel(1);
	}
	else
	{
		m_comboCamPlatformAxisYDirection.SetCurSel(2);
	}
}

void CDlgSysPlatformXYDParam::OnSelchangeComboCamXCoordType() 
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
	//PlatformAxisDirectType eDirection = (m_comboCamAxisXDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
	PlatformAxisDirectType eDirection;
	if(m_comboCamAxisXDirection.GetCurSel() == 0)
	{
		eDirection = eDirectPositive;
	}
	else if (m_comboCamAxisXDirection.GetCurSel() == 1)
	{
		eDirection = eDirectNegative;
	}
	else
	{
		eDirection = eDirectNone;
	}

	m_pCamPlatformInfo->SetCamDirectX(m_nCameraIndex, eDirection);
}

void CDlgSysPlatformXYDParam::OnSelchangeComboCamYCoordType() 
{
	CString strComboCamYCoordType;
	m_comboCamAxisYDirection.GetWindowText(strComboCamYCoordType);

	if (strComboCamYCoordTypeOld != strComboCamYCoordType)
	{
		CString strLog;
		strLog.Format(_T("修改[X轴正方向]由%s到%s\n"),strComboCamYCoordTypeOld,strComboCamYCoordType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboCamYCoordTypeOld = strComboCamYCoordType;
	}
	UpdateData(TRUE);
	//PlatformAxisDirectType eDirection = (m_comboCamAxisYDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
	PlatformAxisDirectType eDirection;
	if(m_comboCamAxisYDirection.GetCurSel() == 0)
	{
		eDirection = eDirectPositive;
	}
	else if (m_comboCamAxisYDirection.GetCurSel() == 1)
	{
		eDirection = eDirectNegative;
	}
	else
	{
		eDirection = eDirectNone;
	}
	m_pCamPlatformInfo->SetCamDirectY(m_nCameraIndex, eDirection);
}

void CDlgSysPlatformXYDParam::OnSelchangeComboCamXCoordType2() 
{
	UpdateData(TRUE);
	//PlatformAxisDirectType eDirection = (m_comboCamAxisXDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
	PlatformAxisDirectType eDirection;
	if(m_comboCamPlatformAxisXDirection.GetCurSel() == 0)
	{
		eDirection = eDirectPositive;
	}
	else if (m_comboCamPlatformAxisXDirection.GetCurSel() == 1)
	{
		eDirection = eDirectNegative;
	}
	else
	{
		eDirection = eDirectNone;
	}

	m_pCamPlatformInfo->SetCamPlatformAxisDirectX(m_nCameraIndex, eDirection);
}

void CDlgSysPlatformXYDParam::OnSelchangeComboCamYCoordType2() 
{
	UpdateData(TRUE);
	//PlatformAxisDirectType eDirection = (m_comboCamAxisYDirection.GetCurSel() == 0) ? eDirectPositive : eDirectNegative;
	PlatformAxisDirectType eDirection;
	if(m_comboCamPlatformAxisYDirection.GetCurSel() == 0)
	{
		eDirection = eDirectPositive;
	}
	else if (m_comboCamPlatformAxisYDirection.GetCurSel() == 1)
	{
		eDirection = eDirectNegative;
	}
	else
	{
		eDirection = eDirectNone;
	}
	m_pCamPlatformInfo->SetCamPlatformAxisDirectY(m_nCameraIndex, eDirection);
}
void CDlgSysPlatformXYDParam::OnSetfocusEditRotationLength() 
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

void CDlgSysPlatformXYDParam::OnKillfocusEditRotationLength() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strVal);
		if (!IsStringNumerical(strVal))
		{
			strVal.Format(_T("%.3f"), ((CPlatformXYDInfo*)m_pPlatformInfo)->m_dPlatformRotationLength);
			SetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strVal);
		}
		else
		{
			strVal.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ROTATION_LENGTH, strVal);
		}
	}
}

BOOL CDlgSysPlatformXYDParam::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysPlatformXYDParam::OnCbnSelchangeComboDriveType()
{
	// TODO: 在此添加控件通知处理程序代码m_comboRDriveType
	CString strComboDriveType;
	m_comboRDriveType.GetWindowText(strComboDriveType);

	if (strComboDriveTypeOld != strComboDriveType)
	{
		CString strLog;
		strLog.Format(_T("修改[θ轴驱动方式]由%s到%s\n"),strComboDriveTypeOld,strComboDriveType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboDriveTypeOld = strComboDriveType;
	}

	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_ROTATION_LENGTH)->EnableWindow((PlatformDDriveType)m_comboRDriveType.GetCurSel() == eDriveLine);
	GetDlgItem(IDC_COMBO_DRIVE_LINE_TYPE)->EnableWindow((PlatformDDriveType)m_comboRDriveType.GetCurSel() == eDriveLine);
}


void CDlgSysPlatformXYDParam::OnCbnSelchangeComboXCoordType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboXCoordType;
	m_comboXDriveDirection.GetWindowText(strComboXCoordType);

	if (strComboXCoordTypeOld != strComboXCoordType)
	{
		CString strLog;
		strLog.Format(_T("修改[X轴正方向]由%s到%s\n"),strComboXCoordTypeOld,strComboXCoordType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboXCoordTypeOld = strComboXCoordType;
	}
}


void CDlgSysPlatformXYDParam::OnCbnSelchangeComboYCoordType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strComboYCoordType;
	m_comboYDriveDirection.GetWindowText(strComboYCoordType);

	if (strComboYCoordTypeOld != strComboYCoordType)
	{
		CString strLog;
		strLog.Format(_T("修改[Y轴正方向]由%s到%s\n"),strComboYCoordTypeOld,strComboYCoordType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboYCoordTypeOld = strComboYCoordType;
	}
}


void CDlgSysPlatformXYDParam::OnCbnSelchangeComboRDirection()
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


void CDlgSysPlatformXYDParam::OnCbnSelchangeComboDriveLineType()
{
	// TODO: 在此添加控件通知处理程序代码m_comboRDriveLineType
	CString strComboDriveLineType;
	m_comboRDriveLineType.GetWindowText(strComboDriveLineType);

	if (strComboDriveLineTypeOld != strComboDriveLineType)
	{
		CString strLog;
		strLog.Format(_T("修改[θ轴直线驱动类型]由%s到%s\n"),strComboDriveLineTypeOld,strComboDriveLineType);
		g_opLogFile.SaveLogText(strLog,3);
		strComboDriveLineTypeOld = strComboDriveLineType;
	}
}

void CDlgSysPlatformXYDParam::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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

void CDlgSysPlatformXYDParam::UpdateDlgByAuthorize()
{
	if (NULL !=m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{

			CString strNodeName = _T("");
			// 对位平台参数
			bool bFind = vsConfig->GetCheckedByItem(ePlatformAuthorize_ParamSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bshowPlatformParamSet,m_bEnablePlatformParamSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_PLATFORM_COORD,m_bEnablePlatformParamSet,m_bshowPlatformParamSet);
				SetGroupBoxStatus(IDC_STATIC_GRP_PLATFORM_DETAIL,m_bEnablePlatformParamSet,m_bshowPlatformParamSet);
				
			}
			// 相机平台参数
			bFind = vsConfig->GetCheckedByItem(ePlatformAuthorize_Camplatform,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bshowCamPlatformParamSet,m_bEnableCamPlatformParamSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_CAM_PLATFORM_CONFIG,m_bEnableCamPlatformParamSet,m_bshowCamPlatformParamSet);

			}
		}
	}
}
