// DlgSysOptionSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysOptionSetting.h"
#include "svXMLConfigurator.h"
#include "VisionASMDLG.h"
#include "base64.h"
#include "DlgSystemInfo.h"
#include "DlgSearchGrapchSetting.h"
#include "DlgSysResStatistics.h"
#include "vsBaseVisionAlign.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysOptionSetting dialog


CDlgSysOptionSetting::CDlgSysOptionSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysOptionSetting::IDD, pParent)
	, m_nCommTimeout(0)
	, m_nCommReSendTimes(0)
	, m_nCommXYUnitExp(0)
	, m_nCommDUnitExp(0)
	, m_bMainframeSetAlignParam(FALSE)
	, m_nCommDelay(0)
	, m_bShowSepTargetPos(FALSE)
	//, m_bShowProductInfo(FALSE)
	//, m_bShowOffsetDirection(FALSE)
    , m_bChangeProductSize2um(FALSE)
	, m_bShowGamma(FALSE)
	, m_bCheckModeOnly(FALSE)
	, m_nRdWsLayoutType(0)
	, m_bChkShowFullScreen(FALSE)
	, m_nLoginTimeSet(0)
	, m_bAllSteoage(FALSE)
	, m_bMainframeSetBendParam(TRUE)
{
	//{{AFX_DATA_INIT(CDlgSysOptionSetting)
	m_bSysKeyboardEnabled = FALSE;
	m_nLanguage = 0;
	m_bStartUpShowCamSNSetting = FALSE;
	//}}AFX_DATA_INIT
	m_pParent = (CVisionASMDlg *)pParent;
	m_psaSysInfoStrings = NULL;

	if (!m_SysOptionConfig.LoadSystemOption())
	{
		m_SysOptionConfig.m_bSysKeyboardEnabled = FALSE;
		m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	}

	switch (m_SysOptionConfig.m_lgidLanguage)
	{
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		m_nLanguage = 0;
		break;
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_nLanguage = 1;
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_nLanguage = 2;
		break;
	default:
		break;
	}

	m_bSysKeyboardEnabled = m_SysOptionConfig.m_bSysKeyboardEnabled;
	m_bStartUpShowCamSNSetting = m_SysOptionConfig.m_bStartUpShowCamSNSetting;
	m_TempSysOptionConfig = m_SysOptionConfig;
	m_bSelNewSystem = FALSE;
	m_bKillFocusEnabled = TRUE;
	m_bChange2um = FALSE;
	m_pDlgPlatformLayoutMode = NULL;
	m_bShowSysResStatistics = true;

	m_bShowWSLayout = true;
	m_bEnableWSLayout = true;

	m_bShowOtherSet = true;
	m_bEnableOtherSet = true;

}

void CDlgSysOptionSetting::SetSystemOptionConfig(CSystemOptionConfig& systemOptionConfig)
{
	m_SysOptionConfig = systemOptionConfig;
	m_TempSysOptionConfig = m_SysOptionConfig;

	switch (m_SysOptionConfig.m_lgidLanguage)
	{
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		m_nLanguage = 0;
		break;
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_nLanguage = 1;
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_nLanguage = 2;
		break;
	default:
		break;
	}

	m_bSysKeyboardEnabled = m_SysOptionConfig.m_bSysKeyboardEnabled;
	m_bStartUpShowCamSNSetting = m_SysOptionConfig.m_bStartUpShowCamSNSetting;
}

void CDlgSysOptionSetting::GetSystemOptionConfig(CSystemOptionConfig& systemOptionConfig)
{
	UpdateDlgData(TRUE);
	systemOptionConfig = m_SysOptionConfig;
}


void CDlgSysOptionSetting::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_OPTION_SETTING") == strDlgID)
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

void CDlgSysOptionSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysOptionSetting)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	DDX_Check(pDX, IDC_CHECK_SYS_KEYBOARD, m_bSysKeyboardEnabled);
	DDX_Radio(pDX, IDC_RADIO_CHINESE_SIMPLIFIED, m_nLanguage);
	DDX_Check(pDX, IDC_CHECK_CAMERA_SN_SETTING, m_bStartUpShowCamSNSetting);
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_COMBO_CAMERA_MODE, m_comboCameraMode);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_MODE, m_comboDisplayMode);
	DDX_Control(pDX, IDC_COMBO_SHOW_RUN_COMMAND_DETAIL, m_comboShowRunCommandDetail);
	DDX_Text(pDX, IDC_EDIT_COMM_TIMEOUT, m_nCommTimeout);
	DDX_Text(pDX, IDC_EDIT_COMM_RE_SEND_TIMES, m_nCommReSendTimes);
	DDX_Text(pDX, IDC_EDIT_COMM_X_Y_UNIT_EXP, m_nCommXYUnitExp);
	DDX_Text(pDX, IDC_EDIT_COMM_D_UNIT_EXP, m_nCommDUnitExp);
	DDX_Control(pDX, IDC_COMBO_COMM_DATA_LEN_TYPE, m_comboCommDataLenType);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_INDEX, m_comboSystemIndex);
	DDX_Control(pDX, IDC_STATIC_NOTE, m_labelNote);
	DDX_Check(pDX, IDC_CHECK_MAINFRAME_SET_ALIGN_PARAM, m_bMainframeSetAlignParam);
	DDX_Text(pDX, IDC_EDIT_COMM_DELAY, m_nCommDelay);
	DDX_Check(pDX, IDC_CHECK_SHOW_SEP_TARGET_POS, m_bShowSepTargetPos);
	//DDX_Check(pDX, IDC_CHECK_SHOW_PRODUCT_INFO, m_bShowProductInfo);
	//DDX_Check(pDX, IDC_CHECK_SHOW_OFFSET_DIRECTION, m_bShowOffsetDirection);
	DDX_Check(pDX, IDC_CHECK_CHANGE_TO_UM, m_bChange2um);
	DDX_Check(pDX, IDC_CHK_CHANGESIZE_TO_UM, m_bChangeProductSize2um);
	DDX_Check(pDX, IDC_CHECK_SHOW_GAMMA, m_bShowGamma);
	DDX_Check(pDX, IDC_CHECK_CHECK_MODE_ONLY, m_bCheckModeOnly);
	DDX_Radio(pDX, IDC_RD_WS_LAYOUT_TYPE1, m_nRdWsLayoutType);
	DDX_Check(pDX, IDC_CHECK_SHOW_FULL_SCREEN, m_bChkShowFullScreen);
	DDX_Text(pDX, IDC_EDIT_LOGIN_TIME_SET, m_nLoginTimeSet);
	//DDV_MinMaxInt(pDX, m_nLoginTimeSet, 0, 3600);
	DDX_Check(pDX, IDC_CHK_ALL_STORAGE, m_bAllSteoage);
	DDX_Check(pDX, IDC_CHECK_MAINFRAME_SET_BEND_PARAM, m_bMainframeSetBendParam);
}


BEGIN_MESSAGE_MAP(CDlgSysOptionSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysOptionSetting)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDIT_COMM_TIMEOUT, &CDlgSysOptionSetting::OnEnSetfocusEditCommTimeout)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_TIMEOUT, &CDlgSysOptionSetting::OnEnKillfocusEditCommTimeout)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_RE_SEND_TIMES, &CDlgSysOptionSetting::OnEnSetfocusEditCommReSendTimes)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_RE_SEND_TIMES, &CDlgSysOptionSetting::OnEnKillfocusEditCommReSendTimes)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_X_Y_UNIT_EXP, &CDlgSysOptionSetting::OnEnSetfocusEditCommXYUnitExp)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_X_Y_UNIT_EXP, &CDlgSysOptionSetting::OnEnKillfocusEditCommXYUnitExp)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_D_UNIT_EXP, &CDlgSysOptionSetting::OnEnSetfocusEditCommDUnitExp)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_D_UNIT_EXP, &CDlgSysOptionSetting::OnEnKillfocusEditCommDUnitExp)
	ON_BN_CLICKED(IDC_CHECK_SYS_KEYBOARD, &CDlgSysOptionSetting::OnBnClickedCheckSysKeyboard)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BNT_REGISTER, &CDlgSysOptionSetting::OnBnClickedBntRegister)
	ON_EN_KILLFOCUS(IDC_EDIT_COMM_DELAY, &CDlgSysOptionSetting::OnEnKillfocusEditCommDelay)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_DELAY, &CDlgSysOptionSetting::OnEnSetfocusEditCommDelay)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_INFO, &CDlgSysOptionSetting::OnBnClickedBtnSystemInfo)
	ON_BN_CLICKED(IDC_BTN_CROSS_SETTING, &CDlgSysOptionSetting::OnBnClickedBtnCrossSetting)
	ON_BN_CLICKED(IDC_RADIO_CHINESE_SIMPLIFIED, &CDlgSysOptionSetting::OnBnClickedRadioChineseSimplified)
	ON_BN_CLICKED(IDC_CHECK_CAMERA_SN_SETTING, &CDlgSysOptionSetting::OnBnClickedCheckCameraSnSetting)
	ON_BN_CLICKED(IDC_RADIO_ENGLISH, &CDlgSysOptionSetting::OnBnClickedRadioChineseSimplified)
	ON_BN_CLICKED(IDC_RADIO_CHINESE_TRADITIONAL, &CDlgSysOptionSetting::OnBnClickedRadioChineseSimplified)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, &CDlgSysOptionSetting::OnCbnSelchangeComboDisplayMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOW_RUN_COMMAND_DETAIL, &CDlgSysOptionSetting::OnCbnSelchangeComboShowRunCommandDetail)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_DATA_LEN_TYPE, &CDlgSysOptionSetting::OnCbnSelchangeComboCommDataLenType)
	ON_CBN_SELCHANGE(IDC_COMBO_SYSTEM_INDEX, &CDlgSysOptionSetting::OnCbnSelchangeComboSystemIndex)
	ON_BN_CLICKED(IDC_CHECK_MAINFRAME_SET_ALIGN_PARAM, &CDlgSysOptionSetting::OnBnClickedCheckMainframeSetAlignParam)
	//ON_BN_CLICKED(IDC_CHECK_SHOW_PRODUCT_INFO, &CDlgSysOptionSetting::OnBnClickedCheckShowProductInfo)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SEP_TARGET_POS, &CDlgSysOptionSetting::OnBnClickedCheckShowSepTargetPos)
	ON_BN_CLICKED(IDCANCEL, &CDlgSysOptionSetting::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK_CHANGE_TO_UM, &CDlgSysOptionSetting::OnBnClickedCheckChangeToUm)
    ON_BN_CLICKED(IDC_CHK_CHANGESIZE_TO_UM, &CDlgSysOptionSetting::OnBnClickedChkChangesizeToUm)
	ON_BN_CLICKED(IDC_BTN_SYS_RES_STATISTICS, &CDlgSysOptionSetting::OnBnClickedBtnSysResStatistics)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDIT_LOGIN_TIME_SET, &CDlgSysOptionSetting::OnEnKillfocusEditLoginTimeSet)
	ON_EN_SETFOCUS(IDC_EDIT_LOGIN_TIME_SET, &CDlgSysOptionSetting::OnEnSetfocusEditLoginTimeSet)
	ON_BN_CLICKED(IDC_CHECK_MAINFRAME_SET_BEND_PARAM, &CDlgSysOptionSetting::OnBnClickedCheckMainframeSetBendParam)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysOptionSetting message handlers

BOOL CDlgSysOptionSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlgItem();
	UpdateDlgData(FALSE);
	UpdateDialogLanguage();

	MoveWindow(m_pParent->m_rcViewRect);

	CString strComboDisplayMode;
	m_comboDisplayMode.GetWindowText(strComboDisplayMode);
	strComboDisplayModeOld = strComboDisplayMode;

	CString strComboShowRunCommandDetail;
	m_comboShowRunCommandDetail.GetWindowText(strComboShowRunCommandDetail);
	strComboShowRunCommandDetailOld = strComboShowRunCommandDetail;

	CString strComboCommDataLenType;
	m_comboCommDataLenType.GetWindowText(strComboCommDataLenType);
	strComboCommDataLenTypeOld = strComboCommDataLenType;

	CString strComboSystemIndex;
	m_comboSystemIndex.GetWindowText(strComboSystemIndex);
	strComboSystemIndexOld = strComboSystemIndex;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysOptionSetting::InitDlgItem()
{
	//m_comboCameraMode.ResetContent();
	//m_comboCameraMode.AddString(_T("4:3"));	
	//m_comboCameraMode.AddString(_T("3:4"));	
	//m_comboCameraMode.AddString(_T("6:5"));	
	//m_comboCameraMode.AddString(_T("5:6"));	

	CString str;
	m_comboDisplayMode.ResetContent();
	m_comboDisplayMode.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_LIVE_DISPLAY)/*_T("实时显示")*/);
	m_comboDisplayMode.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_FRAME_VALID)/*_T("帧有效")*/);	

	m_comboShowRunCommandDetail.ResetContent();
	m_comboShowRunCommandDetail.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_NOT_SHOW_RUN_COMMANDDETAIL)/*_T("不显示记录")*/);	
	m_comboShowRunCommandDetail.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SHOW_RUN_COMMANDDETAIL)/*_T("显示记录")*/);	

	m_comboCommDataLenType.ResetContent();
	m_comboCommDataLenType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COMM_DATA_LEN_CHANGE)/*_T("变化长度")*/);
	m_comboCommDataLenType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COMM_DATA_LEN_SAME)/*_T("固定长度")*/);		

	m_labelNote.SetBkColor(RGB(0,255,255));
	m_labelNote.SetFontSize(11);
	m_labelNote.SetFontBold(TRUE);
	m_labelNote.SetText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_EXIT_SOFTWARE_AND_REOPEN)/*_T("注:如果选择新的视觉系统后,则必须退出软件，重新打开软件！！！")*/);
	
	UpdateDlgByAuthorize();

	// 视觉系统选择
	m_comboSystemIndex.ResetContent();
	int nSystemNum = m_pParent->m_SystemConfigure.GetSystemNum();
	SysPlatformInfo locSysPlatformInfo;	
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(0, locSysPlatformInfo);
	CSystemInfo systemInfo;
	int i = 0;	
	for(i = 0; i < nSystemNum; i++)
	{
		if (!m_pParent->m_SystemConfigure.GetSystemInfo(i, systemInfo))
		{
			break;
		}
		m_comboSystemIndex.AddString(systemInfo.m_strSystemName);
	}

	int nPlatformNum = systemInfo.m_nSystemPlatformNum;

	if (nSystemNum > 1)
	{	
		GetDlgItem(IDC_STATIC_GRP_SYSTEM_INFO)->ShowWindow(m_bShowSysResStatistics);
		GetDlgItem(IDC_STATIC_SYSTEM_INDEX)->ShowWindow(m_bShowSysResStatistics);
		GetDlgItem(IDC_COMBO_SYSTEM_INDEX)->ShowWindow(m_bShowSysResStatistics);
		GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(m_bShowSysResStatistics);
	}
	else
	{
		GetDlgItem(IDC_STATIC_GRP_SYSTEM_INFO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SYSTEM_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_SYSTEM_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(SW_HIDE);
	}

	{
		GetDlgItem(IDC_BNT_REGISTER)->ShowWindow(m_bShowOtherSet);
		GetDlgItem(IDC_REST_DATE)->ShowWindow(SW_HIDE);
	}


	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);

	BOOL bRepaint = FALSE;
	m_btnOK.SetThemeHelper(&m_themeHelper);
	m_btnCancel.SetThemeHelper(&m_themeHelper);
	m_btnApply.SetThemeHelper(&m_themeHelper);

	int nColorOffset = -20;
	CString strText;

	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(150, 150, 150), TRUE);

	// 初始化控制板
	CRect rcPanel, rcItem1, rcItem2;
	GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
	ScreenToClient(rcPanel);

	int nButtonSizeWidth = 80;
	int nButtonSizeHeight = 45;
	int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
	int nStatusBarHeight = 20;
	GetClientRect(rcClient);
	rcItem1.SetRect(rcClient.right-rcPanel.Width(), rcClient.top, rcClient.right, rcClient.bottom);
	GetDlgItem(IDC_BTN_PANEL)->MoveWindow(rcItem1);
	rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSizeWidth)/2, rcItem1.top + nIntendY, 0, 0);
	rcItem2.right = rcItem2.left + nButtonSizeWidth;
	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

	rcItem2.top = rcItem1.bottom-nIntendY-nButtonSizeHeight;
	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;
	GetDlgItem(IDC_BTN_APPLY)->MoveWindow(rcItem2);
	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
	GetDlgItem(IDCANCEL)->MoveWindow(rcItem2);
	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
	GetDlgItem(IDOK)->MoveWindow(rcItem2);

	InitPlatformModeShow();
}

void CDlgSysOptionSetting::MoveDlgItem()
{
	if (GetDlgItem(IDC_BTN_PANEL)->GetSafeHwnd() != NULL)
	{
		CRect rcClient;
		GetClientRect(rcClient);

		// 初始化控制板
		CRect rcPanel, rcItem1, rcItem2;
		GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
		ScreenToClient(rcPanel);

		int nButtonSizeWidth = 80;
		int nButtonSizeHeight = 50;
		int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
		int nStatusBarHeight = 20;
		GetClientRect(rcClient);
		rcItem1.SetRect(rcClient.right-rcPanel.Width(), rcClient.top, rcClient.right, rcClient.bottom);
		GetDlgItem(IDC_BTN_PANEL)->MoveWindow(rcItem1);
		rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSizeWidth)/2, rcItem1.top + nIntendY, 0, 0);
		rcItem2.right = rcItem2.left + nButtonSizeWidth;
		rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

		rcItem2.top = rcItem1.bottom-nIntendY-nButtonSizeHeight;
		rcItem2.bottom = rcItem2.top + nButtonSizeHeight;
		GetDlgItem(IDC_BTN_APPLY)->MoveWindow(rcItem2);
		rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
		GetDlgItem(IDCANCEL)->MoveWindow(rcItem2);
		rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
		GetDlgItem(IDOK)->MoveWindow(rcItem2);
	}
}

void CDlgSysOptionSetting::UpdateDlgData(BOOL bSaveAndValidate /* = TRUE */)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		switch (m_nLanguage)
		{
		case 0:
			m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
			break;
		case 1:
			m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
			break;
		case 2:
			m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL);
			break;
		}
		m_SysOptionConfig.m_bSysKeyboardEnabled = m_bSysKeyboardEnabled;
		m_SysOptionConfig.m_bStartUpShowCamSNSetting = m_bStartUpShowCamSNSetting;

		//m_SysOptionConfig.m_nCameraMode = m_comboCameraMode.GetCurSel();
		m_SysOptionConfig.m_nDisplayMode = m_comboDisplayMode.GetCurSel();
		m_SysOptionConfig.m_nShowRunCommandDetail = m_comboShowRunCommandDetail.GetCurSel();

		m_SysOptionConfig.m_nCommDelay = m_nCommDelay;
		m_SysOptionConfig.m_nCommTimeOut = m_nCommTimeout;
		m_SysOptionConfig.m_nCommReSendTimes = m_nCommReSendTimes;
		m_SysOptionConfig.m_nCommXYUnitExp = m_nCommXYUnitExp;
		m_SysOptionConfig.m_nCommDUnitExp = m_nCommDUnitExp;
		m_SysOptionConfig.m_nCommDataLenType = m_comboCommDataLenType.GetCurSel();

		m_SysOptionConfig.m_bMainframeSetAlignParam = m_bMainframeSetAlignParam;
		m_SysOptionConfig.m_bShowSepTargetPos = m_bShowSepTargetPos;

		//m_SysOptionConfig.m_bShowProductInfo = m_bShowProductInfo;
		//m_SysOptionConfig.m_bShowOffsetDirection = m_bShowOffsetDirection;
		m_SysOptionConfig.m_bChange2um			 = m_bChange2um;
        m_SysOptionConfig.m_bChangeProductSize2um			 = m_bChangeProductSize2um;
		m_SysOptionConfig.m_bAllSteoage			 = m_bAllSteoage;
		m_SysOptionConfig.m_bShowGamma    = m_bShowGamma;
		m_SysOptionConfig.m_bCheckModeOnly    = m_bCheckModeOnly;

		m_SysOptionConfig.m_nCurSystemIndex = m_comboSystemIndex.GetCurSel();

	    m_SysOptionConfig.m_nPlatformLayoutType = m_nRdWsLayoutType;
		
		m_SysOptionConfig.m_bShowFullScreen = m_bChkShowFullScreen;

		m_SysOptionConfig.m_nLoginTimeSet = m_nLoginTimeSet;

		m_SysOptionConfig.m_bMainframeSetBendParam = m_bMainframeSetBendParam;
	}
	else
	{
		switch (m_SysOptionConfig.m_lgidLanguage)
		{
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			m_nLanguage = 0;
			break;
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_nLanguage = 1;
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_nLanguage = 2;
			break;
		default:
			break;
		}
		m_bSysKeyboardEnabled = m_SysOptionConfig.m_bSysKeyboardEnabled;
		m_bStartUpShowCamSNSetting = m_SysOptionConfig.m_bStartUpShowCamSNSetting;

		//m_comboCameraMode.SetCurSel(m_SysOptionConfig.m_nCameraMode);
		m_comboDisplayMode.SetCurSel(m_SysOptionConfig.m_nDisplayMode);
		m_comboShowRunCommandDetail.SetCurSel(m_SysOptionConfig.m_nShowRunCommandDetail);

		m_nCommDelay = m_SysOptionConfig.m_nCommDelay;
		m_nCommTimeout = m_SysOptionConfig.m_nCommTimeOut;
		m_nCommReSendTimes = m_SysOptionConfig.m_nCommReSendTimes;
		m_nCommXYUnitExp = m_SysOptionConfig.m_nCommXYUnitExp;
		m_nCommDUnitExp = m_SysOptionConfig.m_nCommDUnitExp;
		m_comboCommDataLenType.SetCurSel(m_SysOptionConfig.m_nCommDataLenType);

		m_bMainframeSetAlignParam = m_SysOptionConfig.m_bMainframeSetAlignParam;
		m_bShowSepTargetPos       = m_SysOptionConfig.m_bShowSepTargetPos;
		//m_bShowProductInfo = m_SysOptionConfig.m_bShowProductInfo;
		//m_bShowOffsetDirection=m_SysOptionConfig.m_bShowOffsetDirection;

		m_bChange2um		   = m_SysOptionConfig.m_bChange2um;
        m_bChangeProductSize2um		   = m_SysOptionConfig.m_bChangeProductSize2um;
		m_bAllSteoage			 = m_SysOptionConfig.m_bAllSteoage;
		m_bShowGamma    = m_SysOptionConfig.m_bShowGamma;
		m_bCheckModeOnly    = m_SysOptionConfig.m_bCheckModeOnly;

		m_comboSystemIndex.SetCurSel(m_SysOptionConfig.m_nCurSystemIndex);
		m_nRdWsLayoutType = m_SysOptionConfig.m_nPlatformLayoutType;
		m_bChkShowFullScreen = m_SysOptionConfig.m_bShowFullScreen;

		m_nLoginTimeSet = m_SysOptionConfig.m_nLoginTimeSet;

		m_bMainframeSetBendParam = m_SysOptionConfig.m_bMainframeSetBendParam;

		UpdateData(FALSE);
	}
}

void CDlgSysOptionSetting::OnOK() 
{
// 	UpdateData(TRUE);
// 	switch (m_nLanguage)
// 	{
// 	case 0:
// 		m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
// 		break;
// 	case 1:
// 		m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
// 		break;
// 	case 2:
// 		m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL);
// 		break;
// 	}
// 	m_SysOptionConfig.m_bSysKeyboardEnabled = m_bSysKeyboardEnabled;
// 	m_SysOptionConfig.m_bStartUpShowCamSNSetting = m_bStartUpShowCamSNSetting;

	UpdateDlgData(TRUE);


	// 检查当前视觉系统是否改变
	if (m_SysOptionConfig.m_nCurSystemIndex != m_TempSysOptionConfig.m_nCurSystemIndex)
	{
		CString strInfo;
		strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHOOSE_THE_SOFTWARE);//.Format(_T("确定改变当前视觉系统,选择新的视觉系统吗？\r\n选择“是”,则确定选择新的视觉系统,然后必须退出软件，重新打开软件；\r\n选择“否”,则仍选择当前的视觉系统。"));
		int nMBReturn = AfxMessageBox(strInfo, MB_YESNO);	
		if (nMBReturn == IDYES)
		{
			m_bSelNewSystem = TRUE;
		}
		else
		{
			return;
		}
	}




	CString strLog;
	strLog.Format(_T("点击[确定]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("保存退出\n"));
	g_opLogFile.SaveLogText(strLog,1);

	m_SysOptionConfig.SaveSystemOption();
	CDialog::OnOK();
}

BOOL CDlgSysOptionSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgSysOptionSetting::IsStringNumerical( LPCTSTR lpszVal )
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
			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgSysOptionSetting::InitPlatformModeShow()
{
	 int nArrCtrlID[] = {IDC_ST_LAYOUT_TYPE1, IDC_ST_LAYOUT_TYPE2, IDC_ST_LAYOUT_TYPE3, IDC_ST_LAYOUT_TYPE4, IDC_ST_LAYOUT_TYPE5, IDC_ST_LAYOUT_TYPE6};
	 int nArrRdCtrlID[] = {IDC_RD_WS_LAYOUT_TYPE1, IDC_RD_WS_LAYOUT_TYPE2, IDC_RD_WS_LAYOUT_TYPE3, IDC_RD_WS_LAYOUT_TYPE4, IDC_RD_WS_LAYOUT_TYPE5, IDC_RD_WS_LAYOUT_TYPE6};
	 int nLayoutTypeCount = sizeof(nArrCtrlID)/sizeof(int);
	 m_pDlgPlatformLayoutMode = new CDlgPlatformLayoutMode[nLayoutTypeCount];
	 m_vpWsTypeLayout.resize(nLayoutTypeCount, NULL);
	 int nPlatformNum = m_pParent->m_vpNotShareVisionView.size()/*m_pParent->m_vpVisionASM.size()*/;

	 for (int i =0; i <nLayoutTypeCount; i++)
	 {
		 m_pDlgPlatformLayoutMode[i].Create(IDD_DLG_PLATFORM_LAYOUT_MODE, GetDlgItem(nArrCtrlID[i]));
		 m_pDlgPlatformLayoutMode[i].SetSysInfoStringsRef(m_psaSysInfoStrings);
		 m_pDlgPlatformLayoutMode[i].UpdateDialogLanguage();
		 m_pDlgPlatformLayoutMode[i].ShowWindow(m_bShowWSLayout);
		 m_vpWsTypeLayout[i] = new CHorizontalLayout;
		 m_vpWsTypeLayout[i]->AddWnd(&m_pDlgPlatformLayoutMode[i]);
		 m_vpWsTypeLayout[i]->SetParent(GetDlgItem(nArrCtrlID[i]));
		 m_vpWsTypeLayout[i]->OnSize();
		 m_pDlgPlatformLayoutMode[i].UpdateShow(i, nPlatformNum);
		 GetDlgItem(nArrRdCtrlID[i])->EnableWindow(m_pDlgPlatformLayoutMode[i].LayoutIsValid() && m_bEnableWSLayout);
	 }

}

void CDlgSysOptionSetting::OnEnSetfocusEditCommTimeout()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_TIMEOUT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_TIMEOUT, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_TIMEOUT, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[超时(毫秒)]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommTimeout();
	}	
}

void CDlgSysOptionSetting::OnEnKillfocusEditCommTimeout()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommTimeout = (m_nCommTimeout<0) ? 0 : (m_nCommTimeout);
		UpdateData(FALSE);
	}
}

void CDlgSysOptionSetting::OnEnSetfocusEditCommReSendTimes()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_RE_SEND_TIMES)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_RE_SEND_TIMES, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_RE_SEND_TIMES, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[重发次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommReSendTimes();
	}	
}

void CDlgSysOptionSetting::OnEnKillfocusEditCommReSendTimes()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommReSendTimes = (m_nCommReSendTimes<0) ? 0 : (m_nCommReSendTimes);
		UpdateData(FALSE);
	}
}

void CDlgSysOptionSetting::OnEnSetfocusEditCommXYUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_X_Y_UNIT_EXP)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_X_Y_UNIT_EXP, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_X_Y_UNIT_EXP, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[XY单位幂]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommXYUnitExp();
	}	
}

void CDlgSysOptionSetting::OnEnKillfocusEditCommXYUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommXYUnitExp = (m_nCommXYUnitExp<1) ? 1 : (m_nCommXYUnitExp);
		UpdateData(FALSE);
	}
}

void CDlgSysOptionSetting::OnEnSetfocusEditCommDUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_D_UNIT_EXP)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_D_UNIT_EXP, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_D_UNIT_EXP, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[θ单位幂]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommDUnitExp();
	}	
}

void CDlgSysOptionSetting::OnEnKillfocusEditCommDUnitExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommDUnitExp = (m_nCommDUnitExp<1) ? 1 : (m_nCommDUnitExp);
		UpdateData(FALSE);
	}
}

void CDlgSysOptionSetting::OnBnClickedCheckSysKeyboard()
{
	// TODO: 在此添加控件通知处理程序代码m_bSysKeyboardEnabled
	BOOL bSysKeyboardEnabledOld = m_bSysKeyboardEnabled;
	UpdateData(TRUE);
	if (bSysKeyboardEnabledOld != m_bSysKeyboardEnabled)
	{
		CString strVal = m_bSysKeyboardEnabled ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[开启屏幕键盘]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}
}

void CDlgSysOptionSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}

void CDlgSysOptionSetting::OnBnClickedBntRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent==NULL)
	{
		return;
	}

	CString strLog;
	strLog.Format(_T("进行[软件注册]\n"));
	g_opLogFile.SaveLogText(strLog,2);

	SysPlatformInfo locSysPlatformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(0, locSysPlatformInfo);

	CDlgSysRegister dlgSysRegister;
	dlgSysRegister.m_pParent = m_pParent;
	dlgSysRegister.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgSysRegister.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	dlgSysRegister.m_bRegisterSuccess = locSysPlatformInfo.m_bRegisterSuccess;

	if (dlgSysRegister.DoModal() == IDOK)
	{		
		{
			//获取注册日期
			CString strRegDate;
			AuthorizeStatus status = m_pParent->m_AuthorizeUsbTool.CheckAuthorize(strRegDate);

			GetDlgItem(IDC_REST_DATE)->SetWindowText(strRegDate);
		}
	}
}

void CDlgSysOptionSetting::OnEnKillfocusEditCommDelay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nCommDelay = (m_nCommDelay<0) ? 0 : (m_nCommDelay);
		UpdateData(FALSE);
	}
}

void CDlgSysOptionSetting::OnEnSetfocusEditCommDelay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_COMM_DELAY)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_COMM_DELAY, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_COMM_DELAY, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[延时(毫秒)]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCommDelay();
	}	
}

void CDlgSysOptionSetting::OnBnClickedBtnSystemInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击进入[系统信息]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	CDlgSystemInfo dlgSystemInfo;
	dlgSystemInfo.m_pParent = m_pParent;
	dlgSystemInfo.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgSystemInfo.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	if (dlgSystemInfo.DoModal() == IDOK)
	{
	}

}

void CDlgSysOptionSetting::OnBnClickedRadioChineseSimplified()
{
	// TODO: 在此添加控件通知处理程序代码m_nLanguage
	int nLanguageOld = m_nLanguage;
	UpdateData(TRUE);
	CString strLog;
	if (nLanguageOld != m_nLanguage)
	{
		CString strVal[] = {_T("简体中文"),_T("英文"),_T("繁體中文")};
		strLog.Format(_T("修改[语言设置]由%s到%s\n"), strVal[nLanguageOld],strVal[m_nLanguage]);
		g_opLogFile.SaveLogText(strLog,2);
	}

}

void CDlgSysOptionSetting::OnBnClickedCheckCameraSnSetting()
{
	// TODO: 在此添加控件通知处理程序代码m_bStartUpShowCamSNSetting
	BOOL bStartUoShowCanSNSettingOld = m_bStartUpShowCamSNSetting;
	UpdateData(TRUE);
	if (bStartUoShowCanSNSettingOld != m_bStartUpShowCamSNSetting)
	{
		CString strVal = m_bStartUpShowCamSNSetting ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[启动时总是显示相机序列号设置界面]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}
}

void CDlgSysOptionSetting::OnCbnSelchangeComboDisplayMode()
{
	// TODO: 在此添加控件通知处理程序代码m_comboDisplayMode
	CString strComboDisplayMode;
	//m_comboDisplayMode.GetWindowText(strComboDisplayMode);
	int select = m_comboDisplayMode.GetCurSel();	
	m_comboDisplayMode.GetLBText(select,strComboDisplayMode);

	if (strComboDisplayModeOld != strComboDisplayMode)
	{
		CString strLog;
		strLog.Format(_T("修改[图像显示]由%s到%s\n"),strComboDisplayModeOld,strComboDisplayMode);
		g_opLogFile.SaveLogText(strLog,2);
		strComboDisplayModeOld = strComboDisplayMode;
	}
}

void CDlgSysOptionSetting::OnCbnSelchangeComboShowRunCommandDetail()
{
	// TODO: 在此添加控件通知处理程序代码m_comboShowRunCommandDetail  strComboShowRunCommandDetailOld
	CString strComboShowRunCommandDetail;
	//m_comboShowRunCommandDetail.GetWindowText(strComboShowRunCommandDetail);
	int select = m_comboShowRunCommandDetail.GetCurSel();
	m_comboShowRunCommandDetail.GetLBText(select,strComboShowRunCommandDetail);
	if (strComboShowRunCommandDetailOld != strComboShowRunCommandDetail)
	{
		CString strLog;
		strLog.Format(_T("修改[命令记录]由%s到%s\n"),strComboShowRunCommandDetailOld,strComboShowRunCommandDetail);
		g_opLogFile.SaveLogText(strLog,2);
		strComboShowRunCommandDetailOld = strComboShowRunCommandDetail;
	}
}

void CDlgSysOptionSetting::OnCbnSelchangeComboCommDataLenType()
{
	// TODO: 在此添加控件通知处理程序代码m_comboCommDataLenType  strComboCommDataLenTypeOld
	CString strComboCommDataLenType;
	//m_comboCommDataLenType.GetWindowText(strComboCommDataLenType);
	int select = m_comboCommDataLenType.GetCurSel();
	m_comboCommDataLenType.GetLBText(select,strComboCommDataLenType);
	if (strComboCommDataLenTypeOld != strComboCommDataLenType)
	{
		CString strLog;
		strLog.Format(_T("修改[数据长度类型]由%s到%s\n"),strComboCommDataLenTypeOld, strComboCommDataLenType);
		g_opLogFile.SaveLogText(strLog,2);
		strComboCommDataLenTypeOld = strComboCommDataLenType;
	}
}

void CDlgSysOptionSetting::OnCbnSelchangeComboSystemIndex()
{
	// TODO: 在此添加控件通知处理程序代码m_comboSystemIndex strComboSystemIndexOld
	CString strComboSystemIndex;
	//m_comboSystemIndex.GetWindowText(strComboSystemIndex);
	int select = m_comboSystemIndex.GetCurSel();
	m_comboSystemIndex.GetLBText(select,strComboSystemIndex);
	if (strComboSystemIndexOld != strComboSystemIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[当前视觉系统]由%s到%s\n"),strComboSystemIndexOld, strComboSystemIndex);
		g_opLogFile.SaveLogText(strLog,2);
		strComboSystemIndexOld = strComboSystemIndex;
	}
}

void CDlgSysOptionSetting::OnBnClickedCheckMainframeSetAlignParam()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bMainframeSetAlignParamOld = m_bMainframeSetAlignParam;
	UpdateData(TRUE);
	if (bMainframeSetAlignParamOld != m_bMainframeSetAlignParam)
	{
		CString strVal = m_bMainframeSetAlignParam ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[允许主界面设置对位参数]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}
}

//void CDlgSysOptionSetting::OnBnClickedCheckShowProductInfo()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	BOOL bShowProductInfoOld = m_bShowProductInfo;
//	UpdateData(TRUE);
//	if (bShowProductInfoOld != m_bShowProductInfo)
//	{
//		CString strVal = m_bShowProductInfo ? _T("开启状态"):_T("未开启状态");
//		CString strLog;
//		strLog.Format(_T("修改[显示产品信息]为%s\n"),strVal);
//		g_opLogFile.SaveLogText(strLog,2);
//	}
//}

void CDlgSysOptionSetting::OnBnClickedCheckShowSepTargetPos()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bShowSepTargetPosOld = m_bShowSepTargetPos;
	UpdateData(TRUE);
	if (bShowSepTargetPosOld != m_bShowSepTargetPos)
	{
		CString strVal = m_bShowSepTargetPos ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[显示目标虚拟Mark位置]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}
}

void CDlgSysOptionSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码m_btnCancel
	CString strLog;
	strLog.Format(_T("点击[取消]"));
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("不保存退出"));
	g_opLogFile.SaveLogText(strLog,1);
	CDialog::OnCancel();
}

void CDlgSysOptionSetting::OnBnClickedBtnCrossSetting()
{
	CDlgSearchGrapchSetting dlg;
	
	dlg.m_bSysKeyboardEnabled = m_bSysKeyboardEnabled;
	dlg.m_TarCross = m_SysOptionConfig.m_TarSearchResultCrossSize;
	dlg.m_ObjCross = m_SysOptionConfig.m_ObjSearchResultCrossSize;
	dlg.m_psaSysInfoStrings = m_psaSysInfoStrings;

    CString strLog;
    strLog.Format(_T("点击[搜索图形设置]"));
    g_opLogFile.SaveLogText(strLog,2);

	//dlg.SetCross(nWidth,nLength,nColor,(int)nRotation);
	if( IDOK == dlg.DoModal())
	{
 		m_SysOptionConfig.m_TarSearchResultCrossSize = dlg.m_TarCross;
 		m_SysOptionConfig.m_ObjSearchResultCrossSize = dlg.m_ObjCross;

	}


}

void CDlgSysOptionSetting::OnBnClickedCheckChangeToUm()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);

    CString strVal = m_bChange2um ? _T("开启状态"):_T("未开启状态");
    CString strLog;
    strLog.Format(_T("修改[切换精度补偿为um]为%s\n"),strVal);
    g_opLogFile.SaveLogText(strLog,2);
}

void CDlgSysOptionSetting::OnBnClickedChkChangesizeToUm()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);

    CString strVal = m_bChangeProductSize2um ? _T("开启状态"):_T("未开启状态");
    CString strLog;
    strLog.Format(_T("修改[切换产品尺寸为um]为%s\n"),strVal);
    g_opLogFile.SaveLogText(strLog,2);
}

void CDlgSysOptionSetting::OnBnClickedBtnSysResStatistics()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgSysResStatistics dlg;
	dlg.SetParent(this);
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlg.DoModal();
}

void CDlgSysOptionSetting::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pDlgPlatformLayoutMode)
	{
		for (int i = 0; i< m_vpWsTypeLayout.size(); i++)
		{

			m_pDlgPlatformLayoutMode[i].DestroyWindow();
		}
		delete[] m_pDlgPlatformLayoutMode;
		m_pDlgPlatformLayoutMode = NULL;
	}

	for (int i = 0; i< m_vpWsTypeLayout.size(); i++)
	{
		if (m_vpWsTypeLayout[i])
		{
			delete m_vpWsTypeLayout[i];
		}
	}
	m_vpWsTypeLayout.clear();
}

void CDlgSysOptionSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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

void CDlgSysOptionSetting::UpdateDlgByAuthorize()
{
	// 系统选项仅仅通过第一工位的权限管理进行设置
	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && m_pParent->m_vpVisionASM.at(0))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(0)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			bool bShow = false;
			bool bEnable = false;
			CString strNodeName = _T("");
			// 语言设置
			bool bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_Language,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_LANGUAGE,bEnable,bShow);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_StartSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_START_UP_SETTING,bEnable,bShow);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_KeyBoard,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_SYS_KEYBOARD,bEnable,bShow);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_LogShowSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_INFO_DISP_MODE,bEnable,bShow);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_SystemSwitch,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowSysResStatistics,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_SYSTEM_INFO,bEnable,m_bShowSysResStatistics);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_CheckModeSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_CHECK_SETTING,bEnable,bShow);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_Interface,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_MAIN_DISPLAY_SEETING,bEnable,bShow);
				SetGroupBoxStatus(IDC_STATIC_OTHER_SEETING,bEnable,bShow);
				GetDlgItem(IDC_BTN_CROSS_SETTING)->EnableWindow(bEnable);
				GetDlgItem(IDC_BTN_CROSS_SETTING)->ShowWindow(bShow);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_WSlayout,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowWSLayout,m_bEnableWSLayout);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_LAYOUT_SETTING,m_bEnableWSLayout,m_bShowWSLayout);
			}

			bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_OtherSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowOtherSet,m_bEnableOtherSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_COMM,m_bEnableOtherSet,m_bShowOtherSet);
				GetDlgItem(IDC_BTN_SYS_RES_STATISTICS)->EnableWindow(m_bEnableOtherSet);
				GetDlgItem(IDC_BTN_SYS_RES_STATISTICS)->ShowWindow(m_bShowOtherSet);
				GetDlgItem(IDC_BTN_SYSTEM_INFO)->EnableWindow(m_bEnableOtherSet);
				GetDlgItem(IDC_BTN_SYSTEM_INFO)->ShowWindow(m_bShowOtherSet);

				GetDlgItem(IDC_REST_DATE)->EnableWindow(m_bEnableOtherSet);
				GetDlgItem(IDC_REST_DATE)->ShowWindow(m_bShowOtherSet);
				GetDlgItem(IDC_BNT_REGISTER)->EnableWindow(m_bEnableOtherSet);
				GetDlgItem(IDC_BNT_REGISTER)->ShowWindow(m_bShowOtherSet);
			}
		}
	}
}


void CDlgSysOptionSetting::OnEnKillfocusEditLoginTimeSet()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		// 限制在0-3600之间
		m_nLoginTimeSet = (m_nLoginTimeSet<0) ? 0 : (m_nLoginTimeSet);
		m_nLoginTimeSet = (m_nLoginTimeSet>3600) ? 3600 : (m_nLoginTimeSet);
		UpdateData(FALSE);
	}
}


void CDlgSysOptionSetting::OnEnSetfocusEditLoginTimeSet()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LOGIN_TIME_SET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LOGIN_TIME_SET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LOGIN_TIME_SET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[登录有效时间(秒)]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditLoginTimeSet();
	}	
}


void CDlgSysOptionSetting::OnBnClickedCheckMainframeSetBendParam()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bMainframeSetBendParamOld = m_bMainframeSetBendParam;
	UpdateData(TRUE);
	if (bMainframeSetBendParamOld != m_bMainframeSetBendParam)
	{
		CString strVal = m_bMainframeSetBendParam ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[允许主界面设置折弯参数]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,2);
	}
}
