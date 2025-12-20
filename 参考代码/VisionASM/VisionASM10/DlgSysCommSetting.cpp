// DlgSysCommTest.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysCommSetting.h"
#include "VisionASMDlg.h"
#include "DlgSysSerialCommSetting.h"
#include "DlgSysSocketCommSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysCommSetting dialog


CDlgSysCommSetting::CDlgSysCommSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysCommSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysCommSetting)
	m_nPlatformIndex = 0;
	//}}AFX_DATA_INIT

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_nPrevPlatformIndex = m_nPlatformIndex;
	m_pParent = (CVisionASMDlg *)pParent;
}


void CDlgSysCommSetting::InitDlgItem()
{
	// 创建状态栏
	int nStatusBarHeight = 20;
	int nPartWidth[3] = {20, 300, -1};
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	m_pStatusBar->SetParts(3, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);
	
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	UpdateStatusBar();
	
	BOOL bRepaint = FALSE;
	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), bRepaint);
// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172, 168, 153), bRepaint);
// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), bRepaint);

	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), bRepaint);
// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172, 168, 153), bRepaint);
// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), bRepaint);

	int nColorOffset = -20;
	CString strText;
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	
	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM1, strText);
	//	m_btnShowPlatform1.SetThemeHelper(&m_themeHelper);
	// 	m_btnShowPlatform1.SetIcon(m_pParent->m_anIDPlatformIcon[0], (int)BTNST_AUTO_DARKER);
	// 	m_btnShowPlatform1.SetAlign(CButtonST::ST_ALIGN_VERT);
	//	m_btnShowPlatform1.SetFlat(TRUE);
	m_btnShowPlatform1.SetWindowText(strText);
	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	
	
	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM2, strText);
	// 	m_btnShowPlatform2.SetThemeHelper(&m_themeHelper);
	// 	m_btnShowPlatform2.SetIcon(m_pParent->m_anIDPlatformIcon[1], (int)BTNST_AUTO_DARKER);
	// 	m_btnShowPlatform2.SetAlign(CButtonST::ST_ALIGN_VERT);
	// // 	m_btnShowPlatform2.SetFlat(FALSE);
	m_btnShowPlatform2.SetWindowText(strText);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	
	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM3, strText);
	// 	m_btnShowPlatform3.SetThemeHelper(&m_themeHelper);
	// 	m_btnShowPlatform3.SetIcon(m_pParent->m_anIDPlatformIcon[2], (int)BTNST_AUTO_DARKER);
	// 	m_btnShowPlatform3.SetAlign(CButtonST::ST_ALIGN_VERT);
	// // 	m_btnShowPlatform3.SetFlat(FALSE);
	m_btnShowPlatform3.SetWindowText(strText);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	
	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM4, strText);
	// 	m_btnShowPlatform4.SetThemeHelper(&m_themeHelper);
	// 	m_btnShowPlatform4.SetIcon(m_pParent->m_anIDPlatformIcon[3], (int)BTNST_AUTO_DARKER);
	// 	m_btnShowPlatform4.SetAlign(CButtonST::ST_ALIGN_VERT);
	// // 	m_btnShowPlatform4.SetFlat(FALSE);
	m_btnShowPlatform4.SetWindowText(strText);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	
// 	// 调整 Control Panel
// 	int i = 0;
// 	int nButtonSize = 75;
// 	int nFrame = 0;
// 	int nIntendY = 10, nIntervalY = 6;
// 	CRect rcItem1, rcItem2;
// 
// 	GetDlgItem(IDC_STATIC_VIEW_SELECT)->GetWindowRect(rcItem2);
// 	ScreenToClient(rcItem2);
// 	nFrame = rcItem2.Width();
// 
// 	GetClientRect(rcClient);
// 	rcItem1.SetRect(rcClient.right-nFrame, rcClient.top, rcClient.right, rcClient.bottom-nStatusBarHeight/*-nGRPIndent*/);
// 	GetDlgItem(IDC_STATIC_VIEW_SELECT)->MoveWindow(rcItem1);
// 	rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSize)/2, rcItem1.top + nIntendY, 0, 0);
// 	rcItem2.right = rcItem2.left + nButtonSize;
// 	rcItem2.bottom = rcItem2.top + nButtonSize;
// 	for (i=0; i<m_pParent->m_nPlatformNum; i++)
// 	{
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->MoveWindow(rcItem2);
// 		rcItem2.OffsetRect(0, rcItem2.Height() + nIntervalY);
// 	}
// 	rcItem2.top = rcItem1.bottom-nIntendY-nButtonSize;
// 	rcItem2.bottom = rcItem2.top + nButtonSize;
// 	GetDlgItem(IDCANCEL)->MoveWindow(rcItem2);
// 	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
// 	GetDlgItem(IDOK)->MoveWindow(rcItem2);

	// 调整 Control Panel
	int i = 0;
	int nButtonSizeWidth = 75;
	int nButtonSizeHeight = 30;
	int nFrame = 0;
	int nIntendY = 0, nIntervalY = 0, nIntendYOK = 6,nIntervalYOK = 6;
	CRect rcItem1, rcItem2;
	
	GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcItem2);
	ScreenToClient(rcItem2);
	nFrame = rcItem2.Width();
	
	
	GetClientRect(rcClient);
	rcItem1.SetRect(rcClient.right-nFrame, rcClient.top, rcClient.right, rcClient.bottom-nStatusBarHeight/*-nGRPIndent*/);
	GetDlgItem(IDC_BTN_PANEL)->MoveWindow(rcItem1);
	// 	rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSize)/2, rcItem1.top + nIntendY, 0, 0);
	// 	rcItem2.right = rcItem2.left + nButtonSize;
	// 	rcItem2.bottom = rcItem2.top + nButtonSize;
	rcItem2.SetRect(rcItem1.left, rcItem1.top + nIntendY, 0, 0);
	rcItem2.right = rcItem2.left + rcItem1.Width();
	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;
	for (i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->MoveWindow(rcItem2);
		rcItem2.OffsetRect(0, rcItem2.Height() + nIntervalY);
	}
	
	CRect rcCancel;
	GetDlgItem(IDCANCEL)->GetWindowRect(rcCancel);
	rcItem2.left = rcItem1.left+(rcItem1.Width()-rcCancel.Width())/2;
	rcItem2.right = rcItem2.left + rcCancel.Width();
	rcItem2.top = rcItem1.bottom-nIntendYOK-rcCancel.Height();
	rcItem2.bottom = rcItem2.top + rcCancel.Height();
	GetDlgItem(IDCANCEL)->MoveWindow(rcItem2);
	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalYOK);
	GetDlgItem(IDOK)->MoveWindow(rcItem2);
	
	// 创建并显示通信设置子窗口
	CRect rcDisp;
	SysPlatformInfo platformInfo;
	CDlgSysBaseCommSetting *pCommSettingDlg = NULL;
	GetDlgItem(IDC_STATIC_FRAME)->GetWindowRect(rcDisp);
	ScreenToClient(rcDisp);
	for (i=0; i<m_vpCommSettingDlg.size(); i++)
	{
		if (m_vpCommSettingDlg.at(i) != NULL)
		{
			delete m_vpCommSettingDlg.at(i);
			m_vpCommSettingDlg.at(i) = NULL;
		}
	}
	m_vpCommSettingDlg.resize(m_pParent->m_nPlatformNum);

	for (i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(i, platformInfo);
		
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(i)->GetVSconfig(vsConfig);
		pCommSettingDlg = NULL;
		if (vsConfig->m_CommParamInfo.m_eCommType == eSerial)
		{
			pCommSettingDlg = new CDlgSysSerialCommSetting();
			pCommSettingDlg->SetParentWnd(this);
			pCommSettingDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pCommSettingDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pCommSettingDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pCommSettingDlg->Create(IDD_SYS_SERIAL_COMM_CONFIG, this);
		}
		else if (vsConfig->m_CommParamInfo.m_eCommType == eSocket)
		{
			pCommSettingDlg = new CDlgSysSocketCommSetting();
			pCommSettingDlg->SetParentWnd(this);
			pCommSettingDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pCommSettingDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pCommSettingDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pCommSettingDlg->Create(IDD_SYS_SOCKET_COMM_CONFIG, this);
		}
		pCommSettingDlg->MoveWindow(rcDisp);
		m_vpCommSettingDlg.at(i) = pCommSettingDlg;
	}

	for (i=0; i<4; i++)
	{
		if (i < m_pParent->m_nPlatformNum)
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);
		}
	}
	if (m_pParent->m_nPlatformNum <= 1)
	{
		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
	}


	for (i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		if (i == m_nPlatformIndex)
		{
			m_vpCommSettingDlg.at(i)->SetVisible(TRUE);
			m_vpCommSettingDlg.at(i)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_vpCommSettingDlg.at(i)->SetVisible(FALSE);
			m_vpCommSettingDlg.at(i)->ShowWindow(SW_HIDE);
		}
	}
}

void CDlgSysCommSetting::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_COMM_CONFIG") == strDlgID)
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

void CDlgSysCommSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysCommSetting)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysCommSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysCommSetting)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnPlatformChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysCommSetting message handlers

BOOL CDlgSysCommSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlgItem();
	UpdateDialogLanguage();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysCommSetting::OnPlatformChange() 
{
	UpdateData(TRUE);
	if (m_nPrevPlatformIndex == m_nPlatformIndex)
	{
		return;
	}

	for (int i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		if (i == m_nPlatformIndex)
		{
			m_vpCommSettingDlg.at(i)->SetVisible(TRUE);
			m_vpCommSettingDlg.at(i)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_vpCommSettingDlg.at(i)->SetVisible(FALSE);
			m_vpCommSettingDlg.at(i)->ShowWindow(SW_HIDE);
		}
	}

	m_nPrevPlatformIndex = m_nPlatformIndex;
}

BOOL CDlgSysCommSetting::DestroyWindow() 
{
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
	
	for (int i=0; i<m_vpCommSettingDlg.size(); i++)
	{
		if (m_vpCommSettingDlg.at(i) != NULL)
		{
			m_vpCommSettingDlg.at(i)->DestroyWindow();
			delete m_vpCommSettingDlg.at(i);
			m_vpCommSettingDlg.at(i) = NULL;
		}
	}
	m_vpCommSettingDlg.clear();

	return CDialog::DestroyWindow();
}

void CDlgSysCommSetting::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSysCommSetting::OnOK() 
{
	for (int i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		m_vpCommSettingDlg.at(i)->UpdateDlgData(TRUE);
		m_pParent->m_vpVisionASM.at(i)->SaveCommInfo();
	}

	CDialog::OnOK();
}

void CDlgSysCommSetting::OnCancel() 
{
	for (int i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		m_pParent->m_vpVisionASM.at(i)->LoadCommInfo();
	}
	
	CDialog::OnCancel();
}
