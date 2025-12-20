// DlgSysEnvironSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysEnvironSetting.h"
#include "VisionASMDlg.h"

#include "DlgSysSerialCommSetting.h"
#include "DlgSysSocketCommSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysEnvironSetting dialog


CDlgSysEnvironSetting::CDlgSysEnvironSetting(CWnd* pParent)
	: CDialog(CDlgSysEnvironSetting::IDD, pParent)
	, m_nRadioEnvironType(0)
{
	//{{AFX_DATA_INIT(CDlgSysEnvironSetting)
	//m_nPlatformIndex = 0;
	//}}AFX_DATA_INIT
	
	m_pParent = (CVisionASMDlg *)pParent;
	
	m_nPlatformIndex = m_pParent->m_nPlatformDispIdx-1;
	if (m_nPlatformIndex<0)
	{
		m_nPlatformIndex = 0;
	}
	m_nPrevPlatformIndex = m_nPlatformIndex;
	m_nPrevTabIndex = 0;
	m_pGuiDisplay = NULL;
//	m_paOldGuiDisplay = NULL;

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;

	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;

	m_SysOptionConfig.LoadSystemOption();

	m_scRectForCommDlg.SetRect(0,0,10,10);
	m_nFitImage = 0;
}

CDlgSysEnvironSetting::~CDlgSysEnvironSetting()
{
	
}

void CDlgSysEnvironSetting::InitDlgItem()
{
	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	rcClient.right = rcClient.right - 100;
	int nPartWidth[4] = {20, 300, 600, -1};
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	m_pStatusBar->SetParts(4, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);

	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	UpdateStatusBar();

	GetClientRect(rcClient);

	// 界面Butten
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

	m_btnApply.SetThemeHelper(&m_themeHelper);
	//	m_btnApply.SetIcon(IDI_ICON_APPLY_48);
	// 	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172, 168, 153), TRUE);
	// 	m_btnApply.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);

	m_btnEnvironCam.SetThemeHelper(&m_themeHelper);
	m_btnEnvironComm.SetThemeHelper(&m_themeHelper);
	m_btnEnvironPlatform.SetThemeHelper(&m_themeHelper);
	m_btnEnvironLog.SetThemeHelper(&m_themeHelper);
	m_btnEnvironSearchTool.SetThemeHelper(&m_themeHelper);
	m_btnEnvironLayout.SetThemeHelper(&m_themeHelper);
	m_btnEnvironAuthorizeSet.SetThemeHelper(&m_themeHelper);
	int nColorOffset = -20;
	CString strText;

	// 
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	//  	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);

	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(150, 150, 150), TRUE);

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

	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM5, strText);
	m_btnShowPlatform5.SetThemeHelper(&m_themeHelper);
	m_btnShowPlatform5.SetWindowText(strText);

	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM6, strText);
	m_btnShowPlatform6.SetThemeHelper(&m_themeHelper);
	m_btnShowPlatform6.SetWindowText(strText);


	// 调整平台选择按钮
	int i = 0;
	int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

	for (i=0; i<MaxPlatformNum; i++)
	{
		if (i<nPlatformNum)
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);
		}
	}
	if (nPlatformNum <= 1)
	{
		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
	}

	m_pGuiDisplay = new scGuiDisplay;
	m_pGuiDisplay->Create(IDC_STATIC_DISPLAY, this);
	m_pGuiDisplay->SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	m_pGuiDisplay->RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DRAGSTOP);
	//m_pGuiDisplay->SetAutoFit(FALSE);

	// 语言参数设置
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
// 	m_pGuiDisplay = new scGuiDisplay;
// 	m_pGuiDisplay->Create(IDC_STATIC_DISPLAY, this);
// 	m_pGuiDisplay->SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
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
//	m_pGuiDisplay->RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DRAGSTOP);


// 	m_vpGuiCross.resize(1);
// 	scGuiStaticContainer* pSContainer = m_pGuiDisplay->GetStaticContainer();
// 
// 	CString strKey;
// 	m_vpGuiCross.at(0) = new scGuiCross;
// 	m_vpGuiCross.at(0)->SetLineColor(RGB(0,255,0));
// 	strKey.Format(_T("ViewGuiCross%d"),0);
// 	pSContainer->AddItem(m_vpGuiCross.at(0), strKey);
// 
// 	m_pGuiDisplay->SetStaticContainer(pSContainer);



	m_vpGuiCross.resize(1);
	scGuiInteractiveContainer* pIContainer = m_pGuiDisplay->GetInteractiveContainer();

	CString strKey;
	m_vpGuiCross.at(0) = new scGuiCross;
	m_vpGuiCross.at(0)->SetLineColor(RGB(0,255,0));
	strKey.Format(_T("ViewGuiCross%d"),0);
	pIContainer->AddItem(m_vpGuiCross.at(0), strKey);

	m_pGuiDisplay->SetInteractiveContainer(pIContainer);

	// 	m_tabSheet.InsertItem(0, GetSysInfoString(m_psaSysInfoStrings,IDS_TAB_CAMERA_SETTING));		//_T("相机设置")
	// 	m_tabSheet.InsertItem(1, GetSysInfoString(m_psaSysInfoStrings,IDS_TAB_PLATFORM_SETTING));		//_T("平台设置")
	// 	m_tabSheet.InsertItem(2, _T("通信"));		//_T("平台设置")
	// 	m_tabSheet.InsertItem(3, GetSysInfoString(m_psaSysInfoStrings,IDS_TAB_DATA_RECORD_SETTING));	//_T("记录设置")
// 
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_CAMERA);		//_T("相机")
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_PLATFORM);	//_T("平台")
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_COMM);		//_T("通信")
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_LOG);		//_T("日志")
// 
// 	CRect rcTabSheet;
// 	m_tabSheet.GetClientRect(rcTabSheet);
// 	rcTabSheet.top	  += 20;   
// 	rcTabSheet.bottom -= 4;
// 	rcTabSheet.left	  += 2;
// 	rcTabSheet.right  -= 4;
// 
// 
	// 初始化控制板
	CRect rcPanel, rcItem1, rcItem2;
	GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
	ScreenToClient(rcPanel);

	int nButtonSizeWidth = 80;
//	int nButtonSizeHeight = 50;
	int nButtonSizeHeight = 45;
	int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
	int nStatusBarHeight = 25;
	GetClientRect(rcClient);
	rcItem1.SetRect(rcClient.right-rcPanel.Width(), rcClient.top, rcClient.right, rcClient.bottom);
	GetDlgItem(IDC_BTN_PANEL)->MoveWindow(rcItem1);
	rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSizeWidth)/2, rcItem1.top + nIntendY, 0, 0);
	rcItem2.right = rcItem2.left + nButtonSizeWidth;
	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

	//  	rcItem2.SetRect(rcItem1.left, rcItem1.top + nIntendY, 0, 0);
	//  	rcItem2.right = rcItem2.left + rcItem1.Width();
	//  	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

	// nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	for (i=0; i<nPlatformNum; i++)
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


	CRect rcRadioPannel;
	GetDlgItem(IDC_STATIC_RADIO_PANEL)->GetWindowRect(rcRadioPannel);
	rcItem2.left = rcClient.left;
	rcItem2.right = rcItem1.left + 1;
	rcItem2.top = rcClient.top;
	rcItem2.bottom = rcRadioPannel.Height();
	GetDlgItem(IDC_STATIC_RADIO_PANEL)->MoveWindow(rcItem2);

	CRect rcStatusBar;
	GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rcStatusBar);
	rcStatusBar.right = rcItem1.left -1;
	GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(rcStatusBar);


	// 初始化相机参数设置界面
	CRect rcImgRoi;
	GetDlgItem(IDC_TAB)->GetWindowRect(rcImgRoi);
	ScreenToClient(rcImgRoi);
	rcItem2.left =  rcItem1.left - rcImgRoi.Width() - 1;
	rcItem2.right = rcItem1.left ;
	rcItem2.top = rcItem2.bottom + 5;
	rcItem2.bottom = rcClient.bottom - nStatusBarHeight;

	m_vpVAPrevMainWnd.resize(nPlatformNum, NULL);
	for (i=0; i<nPlatformNum; i++)
	{
		m_vpVAPrevMainWnd.at(i) = m_pParent->m_vpVisionASM.at(i)->SetMainWnd(this);
	}
	m_dlgSysImgRoiParam.SetParentWnd(this);
	m_dlgSysImgRoiParam.EnableSysKeyboard(m_bSysKeyboardEnabled);
	m_dlgSysImgRoiParam.SetSysInfoStringsRef(m_psaSysInfoStrings);
	m_dlgSysImgRoiParam.SetCameraDisplay(m_pGuiDisplay);	// 注意先后顺序，应先调用SetCameraDisplay，再调SetVisionASMRef
	m_dlgSysImgRoiParam.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	m_dlgSysImgRoiParam.Create(IDD_SYS_IMGROI_PARAM, this);
	m_dlgSysImgRoiParam.MoveWindow(rcItem2);

	CRect rcDisp;
	GetDlgItem(IDC_STATIC_DISPLAY)->GetWindowRect(rcDisp);
	ScreenToClient(rcDisp);	
	rcItem2.right = rcItem2.left ;
	rcItem2.left = rcClient.left + 10;
//	rcItem2.top = rcItem2.bottom + 5;
	rcItem2.bottom = rcClient.bottom - nStatusBarHeight;
	GetDlgItem(IDC_STATIC_DISPLAY)->MoveWindow(rcItem2);
	m_pGuiDisplay->MoveWindow(rcItem2);


	rcItem2.left = rcClient.left + 10;
	rcItem2.right = rcItem1.left ;
//	rcItem2.top = rcItem2.bottom + 5;
	rcItem2.bottom = rcClient.bottom - nStatusBarHeight;


	CRect rcItemPlatform;
	rcItemPlatform.left = rcItem2.left;
	rcItemPlatform.right = rcItem2.left + rcItem2.Width()/2;
	rcItemPlatform.top = rcItem2.top;
	rcItemPlatform.bottom = rcItem2.bottom;

	// 初始化平台参数设置界面
	CDlgSysPlatformParam *pDlg = NULL;
	CPlatformInfo *pPlatformInfo = NULL;
	m_vpDlgSysPlatformParam.resize(nPlatformNum);
	for (i=0; i<nPlatformNum; i++)
	{
		pPlatformInfo = m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo();
		switch (pPlatformInfo->GetPlatformType())
		{
		case ePlatformXYD:
			pDlg = new CDlgSysPlatformXYDParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
// 		case ePlatformXYPD:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
// 			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
		case ePlatformX1X2Y:
			pDlg = new CDlgSysPlatformX1X2YParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
			pDlg->Create(IDD_SYS_PLATFORM_X1X2Y_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		case ePlatformXY1Y2:
			pDlg = new CDlgSysPlatformXY1Y2Param;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
			pDlg->Create(IDD_SYS_PLATFORM_XY1Y2_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		case ePlatformYDPX1X2:
			pDlg = new CDlgSysPlatformYDPX1X2Param;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
			pDlg->Create(IDD_SYS_PLATFORM_YX1X2PD_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		case ePlatformYX1X2PD:
			pDlg = new CDlgSysPlatformYX1X2PDParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
			pDlg->Create(IDD_SYS_PLATFORM_YDPX1X2_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
// 		case ePlatformX:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
// 			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformXY:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
// 			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformXD:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
// 			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformYD:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
// 			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
		default:
			pDlg = new CDlgSysPlatformXYDParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetPlatformInfo());
			pDlg->SetCamPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetCamPlatformInfo());
			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		}
		m_vpDlgSysPlatformParam.at(i) = pDlg;
	}

	rcItemPlatform.left = rcItem2.left + rcItem2.Width()/2;
	rcItemPlatform.right = rcItem2.right;
	rcItemPlatform.top = rcItem2.top;
	rcItemPlatform.bottom = rcItem2.bottom;

	CDlgSysPlatformParam *pDlgMid = NULL;
	CPlatformInfo *pMidPlatformInfo = NULL;
	m_vpDlgSysMidPlatformParam.resize(nPlatformNum);
	for (i=0; i<nPlatformNum; i++)
	{
		pMidPlatformInfo = m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo();
		switch (pMidPlatformInfo->GetPlatformType())
		{
		case ePlatformXYD:
			pDlg = new CDlgSysPlatformXYDParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
			pDlg->SetCamPlatformInfoEnable(FALSE);
			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
// 		case ePlatformXYPD:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
// 			pDlg->SetCamPlatformInfoEnable(FALSE);
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformXDPY:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
// 			pDlg->SetCamPlatformInfoEnable(FALSE);
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformXDPY1:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
// 			pDlg->SetCamPlatformInfoEnable(FALSE);
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
		case ePlatformX1X2Y:
			pDlg = new CDlgSysPlatformX1X2YParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
			pDlg->SetCamPlatformInfoEnable(FALSE);
			pDlg->Create(IDD_SYS_PLATFORM_X1X2Y_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		case ePlatformXY1Y2:
			pDlg = new CDlgSysPlatformXY1Y2Param;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
			pDlg->SetCamPlatformInfoEnable(FALSE);
			pDlg->Create(IDD_SYS_PLATFORM_XY1Y2_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		case ePlatformYDPX1X2:
			pDlg = new CDlgSysPlatformYDPX1X2Param;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
			pDlg->SetCamPlatformInfoEnable(FALSE);
			pDlg->Create(IDD_SYS_PLATFORM_YDPX1X2_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		case ePlatformYX1X2PD:
			pDlg = new CDlgSysPlatformYX1X2PDParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
			pDlg->SetCamPlatformInfoEnable(FALSE);
			pDlg->Create(IDD_SYS_PLATFORM_YX1X2PD_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
// 		case ePlatformX:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
// 			pDlg->SetCamPlatformInfoEnable(FALSE);
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformXY:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
// 			pDlg->SetCamPlatformInfoEnable(FALSE);
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformXD:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
// 			pDlg->SetCamPlatformInfoEnable(FALSE);
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
// 		case ePlatformYD:
// 			pDlg = new CDlgSysPlatformXYDParam;
// 			pDlg->SetParentWnd(this);
// 			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
// 			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
// 			pDlg->SetCamPlatformInfoEnable(FALSE);
// 			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
// 			pDlg->MoveWindow(rcItemPlatform);
// 			break;
		default:
			pDlg = new CDlgSysPlatformXYDParam;
			pDlg->SetParentWnd(this);
			pDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
			pDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
			pDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
			pDlg->SetPlatformInfo(m_pParent->m_vpVisionASM.at(i)->GetMidPlatformInfo());
			pDlg->SetCamPlatformInfoEnable(FALSE);
			pDlg->Create(IDD_SYS_PLATFORM_XYD_PARAM, this);
			pDlg->MoveWindow(rcItemPlatform);
			break;
		}
		m_vpDlgSysMidPlatformParam.at(i) = pDlg;
	}

	// 初始化数据记录参数设置界面
	m_dlgSysDataRecord.SetParentWnd(this);
	m_dlgSysDataRecord.EnableSysKeyboard(m_bSysKeyboardEnabled);
	m_dlgSysDataRecord.SetSysInfoStringsRef(m_psaSysInfoStrings);
	m_dlgSysDataRecord.SetSysDataRecord(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysDataRecord());
	m_dlgSysDataRecord.SetPlatformIndex(m_nPlatformIndex);
	m_dlgSysDataRecord.m_bTempStartUpShowCamSN = m_SysOptionConfig.m_bStartUpShowCamSNSetting;
	m_dlgSysDataRecord.m_bTempSysKeyboard = m_SysOptionConfig.m_bSysKeyboardEnabled;
	m_dlgSysDataRecord.Create(IDD_SYS_DATA_RECORD, this);
	m_dlgSysDataRecord.MoveWindow(rcItem2);


	// 初始化通信界面参数

	CRect rcItemComm;

	int nWidth = 400;
	int nHeight = 380;
	rcItemComm.left = rcItem2.left;
	rcItemComm.right = rcItem2.left  +nWidth;
	rcItemComm.top = rcItem2.top;
	rcItemComm.bottom = rcItemComm.top + nHeight ;


	CDlgCommTypeSetting *pCommTypeSettingDlg = NULL;
	for (i=0; i<m_vpdlgCommTypeSetting.size(); i++)
	{
		if (m_vpdlgCommTypeSetting.at(i) != NULL)
		{
			delete m_vpdlgCommTypeSetting.at(i);
			m_vpdlgCommTypeSetting.at(i) = NULL;
		}
	}
	m_vpdlgCommTypeSetting.resize(nPlatformNum);

	for (i=0; i<m_vpdlgCommTypeSetting.size(); i++)
	{
		pCommTypeSettingDlg = new CDlgCommTypeSetting();
		pCommTypeSettingDlg->SetParentWnd(this);

		pCommTypeSettingDlg->SetParentWnd(this);
		pCommTypeSettingDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
		pCommTypeSettingDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);

		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(i)->GetVSconfig(vsConfig);
		pCommTypeSettingDlg->SetCCommParamInfo(vsConfig->m_CommParamInfo);
		pCommTypeSettingDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
		pCommTypeSettingDlg->Create(IDD_SYS_COMM_PARAM_SETTING,this);
		pCommTypeSettingDlg->MoveWindow(rcItemComm);
		pCommTypeSettingDlg->ShowWindow(SW_HIDE);

		m_vpdlgCommTypeSetting.at(i) = pCommTypeSettingDlg;

	}

	//初始化通信参数界面
	rcItemComm.left = rcItem2.left ;//rcItem2.Width()/4;
	rcItemComm.right = rcItem2.right +nWidth;
	rcItemComm.top = rcItem2.top + nHeight;
	/*rcItemComm.bottom = rcItem2.bottom;*/
	rcItemComm.bottom =rcItemComm.top +nHeight-180;
	m_dlgCommParamSetting.SetParentWnd(this);
	m_dlgCommParamSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
	m_dlgCommParamSetting.EnableSysKeyboard(m_bSysKeyboardEnabled);
	CCommOptionInfo commOptionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCommOptionInfoXML(commOptionInfo);
	m_dlgCommParamSetting.SetCommOptionInfo(commOptionInfo);
	m_dlgCommParamSetting.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	m_dlgCommParamSetting.Create(IDD_DLG_COMM_PARAM_SETTING,this);
	m_dlgCommParamSetting.MoveWindow(rcItemComm);
	
	SysPlatformInfo platformInfo;
	CDlgSysBaseCommSetting *pCommSettingDlg = NULL;
	for (i=0; i<m_vpCommSettingDlg.size(); i++)
	{
		if (m_vpCommSettingDlg.at(i) != NULL)
		{
			delete m_vpCommSettingDlg.at(i);
			m_vpCommSettingDlg.at(i) = NULL;
		}
	}
	m_vpCommSettingDlg.resize(nPlatformNum);

	
	rcItemComm.left = rcItem2.left  +nWidth;
	rcItemComm.right = rcItem2.left + 2*nWidth;//rcItem2.Width()/4;
	rcItemComm.top = rcItem2.top;
	rcItemComm.bottom = rcItem2.bottom;

	m_scRectForCommDlg = rcItemComm;

	for (i=0; i<nPlatformNum; i++)
	{
		// m_pParent->m_SystemConfigure.GetSysPlatformInfo(i, platformInfo);
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
		pCommSettingDlg->MoveWindow(rcItemComm);
		pCommSettingDlg->ShowWindow(SW_HIDE);
		pCommSettingDlg->SetVisible(FALSE);
		m_vpCommSettingDlg.at(i) = pCommSettingDlg;
	}




	// 初始化平台选项参数界面
	m_dlgPlatformOptionParam.SetParentWnd(this);
	m_dlgPlatformOptionParam.SetSysInfoStringsRef(m_psaSysInfoStrings);
	m_dlgPlatformOptionParam.EnableSysKeyboard(m_bSysKeyboardEnabled);
	m_dlgPlatformOptionParam.SetDlgSearchToolData(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetDlgSearchToolData());
	//lzk
	m_dlgPlatformOptionParam.SetAlignerTypeInfo(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo());//lzk
	CPlatformOptionInfo optionInfo;
	//获取该平台的平台选项并设置给对话框
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	m_dlgPlatformOptionParam.SetPlatformOptionInfo(optionInfo);
	m_dlgPlatformOptionParam.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	m_dlgPlatformOptionParam.Create(IDD_SYS_PLATFORM_OPTION_PARAM,this);
	m_dlgPlatformOptionParam.MoveWindow(rcItem2);
	
	// 权限设置界面
	CDlgAuthorizeSet *pDlgAuthorizeSet = NULL;
	for (i=0; i<m_vpAuthorizeSetDlg.size(); i++)
	{
		if (m_vpAuthorizeSetDlg.at(i) != NULL)
		{
			delete m_vpAuthorizeSetDlg.at(i);
			m_vpAuthorizeSetDlg.at(i) = NULL;
		}
	}
	m_vpAuthorizeSetDlg.resize(nPlatformNum);

	for (i=0; i<nPlatformNum; i++)
	{
		// m_pParent->m_SystemConfigure.GetSysPlatformInfo(i, platformInfo);
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(i)->GetVSconfig(vsConfig);
		pDlgAuthorizeSet = NULL;

		pDlgAuthorizeSet = new CDlgAuthorizeSet();
		pDlgAuthorizeSet->SetParentWnd(this);
		pDlgAuthorizeSet->SetVisionASMRef(m_pParent->m_vpVisionASM.at(i));
		pDlgAuthorizeSet->SetSysInfoStringsRef(m_psaSysInfoStrings);
		pDlgAuthorizeSet->m_cTreeAuthorizeShow = vsConfig->m_cTreeAuthorizeManagement;
		pDlgAuthorizeSet->Create(IDD_SYS_AUTHORIZE_SET, this);
		
		
		pDlgAuthorizeSet->MoveWindow(rcItem2);
		pDlgAuthorizeSet->ShowWindow(SW_HIDE);

		m_vpAuthorizeSetDlg.at(i) = pDlgAuthorizeSet;
	}
	
	// 初始化布局参数界面
	CPlatformLayoutInfo layoutInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformLayoutInfo(layoutInfo);

	m_dlgSysLayoutParamSetting.SetParentWnd(this);
	m_dlgSysLayoutParamSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
	m_dlgSysLayoutParamSetting.EnableSysKeyboard(m_bSysKeyboardEnabled);
	m_dlgSysLayoutParamSetting.SetPlatformLayoutInfo(layoutInfo);
	m_dlgSysLayoutParamSetting.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	m_dlgSysLayoutParamSetting.Create(IDD_DLG_SYS_LAYOUT_PARAM_SETTING,this);
	m_dlgSysLayoutParamSetting.MoveWindow(rcItem2);


	//m_tabSheet.SetCurSel(0);
	m_nPrevTabIndex = m_nRadioEnvironType;

	m_dlgSysImgRoiParam.ShowWindow(TRUE);
	UpdateData(FALSE);	
}

void CDlgSysEnvironSetting::MoveDlgItem()
{
	if (GetDlgItem(IDC_BTN_PANEL)->GetSafeHwnd() != NULL)
	{
		// 创建状态栏
		CRect rcClient;
		GetClientRect(rcClient);

		BOOL bRepaint = FALSE;

		m_pStatusBar->MoveWindow(rcClient.left,rcClient.bottom+20,rcClient.Width()-400,20);

		// 调整平台选择按钮
		int i = 0;
		int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
		ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

		for (i=0; i<MaxPlatformNum; i++)
		{
			if (i<nPlatformNum)
			{
				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);
			}
		}
		if (nPlatformNum <= 1)
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
		}


		// 初始化控制板
		CRect rcPanel, rcItem1, rcItem2;
		GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
		ScreenToClient(rcPanel);

		int nButtonSizeWidth = 80;
//		int nButtonSizeHeight = 50;
		int nButtonSizeHeight = 45;
		int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
		int nStatusBarHeight = 25;
		GetClientRect(rcClient);
		rcItem1.SetRect(rcClient.right-rcPanel.Width(), rcClient.top, rcClient.right, rcClient.bottom);
		GetDlgItem(IDC_BTN_PANEL)->MoveWindow(rcItem1);
		rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSizeWidth)/2, rcItem1.top + nIntendY, 0, 0);
		rcItem2.right = rcItem2.left + nButtonSizeWidth;
		rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

		//  	rcItem2.SetRect(rcItem1.left, rcItem1.top + nIntendY, 0, 0);
		//  	rcItem2.right = rcItem2.left + rcItem1.Width();
		//  	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;

		// nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
		for (i=0; i<nPlatformNum; i++)
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



		CRect rcRadioPannel;
		GetDlgItem(IDC_STATIC_RADIO_PANEL)->GetWindowRect(rcRadioPannel);
		rcItem2.left = rcClient.left;
		rcItem2.right = rcItem1.left + 1;
		rcItem2.top = rcClient.top;
		rcItem2.bottom = rcRadioPannel.Height();
		GetDlgItem(IDC_STATIC_RADIO_PANEL)->MoveWindow(rcItem2);

		m_pStatusBar->MoveWindow(rcClient.left,rcClient.bottom+20,rcClient.Width()-rcItem1.Width(),20);
// 		CRect rcStatusBar;
// 		GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rcStatusBar);
// 		ScreenToClient(rcStatusBar);
// 		rcStatusBar.right = 100;
//		GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(rcStatusBar);
//		m_pStatusBar->MoveWindow(rcStatusBar);

		// 初始化相机参数设置界面
		CRect rcImgRoi;
		GetDlgItem(IDC_TAB)->GetWindowRect(rcImgRoi);
		ScreenToClient(rcImgRoi);
		rcItem2.left =  rcItem1.left - rcImgRoi.Width() - 1;
		rcItem2.right = rcItem1.left ;
		rcItem2.top = rcItem2.bottom + 5;
		rcItem2.bottom = rcClient.bottom - nStatusBarHeight;

		m_dlgSysImgRoiParam.MoveWindow(rcItem2);

		CRect rcDisp;
		GetDlgItem(IDC_STATIC_DISPLAY)->GetWindowRect(rcDisp);
		ScreenToClient(rcDisp);	
		rcItem2.right = rcItem2.left ;
		rcItem2.left = rcClient.left + 10;
//		rcItem2.top = rcItem2.bottom + 5;
		rcItem2.bottom = rcClient.bottom - nStatusBarHeight;
		GetDlgItem(IDC_STATIC_DISPLAY)->MoveWindow(rcItem2);
		m_pGuiDisplay->MoveWindow(rcItem2);


		rcItem2.left = rcClient.left + 10;
		rcItem2.right = rcItem1.left ;
//		rcItem2.top = rcItem2.bottom + 5;
		rcItem2.bottom = rcClient.bottom - nStatusBarHeight;


		CRect rcItemPlatform;
		rcItemPlatform.left = rcItem2.left;
		rcItemPlatform.right = rcItem2.left + rcItem2.Width()/2;
		rcItemPlatform.top = rcItem2.top;
		rcItemPlatform.bottom = rcItem2.bottom;

		// 初始化平台参数设置界面
		CDlgSysPlatformParam *pDlg = NULL;
		CPlatformInfo *pPlatformInfo = NULL;
		for (i=0; i<nPlatformNum; i++)
		{
			pDlg = m_vpDlgSysPlatformParam.at(i);
			pDlg->MoveWindow(rcItemPlatform);
		}

		rcItemPlatform.left = rcItem2.left + rcItem2.Width()/2;
		rcItemPlatform.right = rcItem2.right;
		rcItemPlatform.top = rcItem2.top;
		rcItemPlatform.bottom = rcItem2.bottom;

		// 初始化中间平台参数设置界面
		CDlgSysPlatformParam *pDlgMid = NULL;
		CPlatformInfo *pMidPlatformInfo = NULL;
		for (i=0; i<nPlatformNum; i++)
		{
			pDlg = m_vpDlgSysMidPlatformParam.at(i);
			pDlg->MoveWindow(rcItemPlatform);
		}

		// 初始化数据记录参数设置界面
		m_dlgSysDataRecord.MoveWindow(rcItem2);


		// 初始化通信界面参数
		SysPlatformInfo platformInfo;
		CDlgSysBaseCommSetting *pCommSettingDlg = NULL;

		CRect rcItemComm;
		int nWidth = 400;
		int nHeight = 380;
		rcItemComm.left = rcItem2.left;
		rcItemComm.right = rcItem2.left + nWidth;//rcItem2.Width()/4;
		rcItemComm.top = rcItem2.top;
		rcItemComm.bottom = rcItemComm.top + nHeight;

		for (i=0; i<m_vpdlgCommTypeSetting.size(); i++)
		{
			m_vpdlgCommTypeSetting.at(i)->MoveWindow(rcItemComm);
		}
		
		rcItemComm.left =  rcItem2.left;
		rcItemComm.right = rcItem2.left + nWidth;//rcItem2.Width()/4;
		rcItemComm.top = rcItem2.top + nHeight;
		rcItemComm.bottom =rcItemComm.top +nHeight-180;

		m_dlgCommParamSetting.MoveWindow(rcItemComm);


		rcItemComm.left = rcItem2.left + nWidth;
		rcItemComm.right = rcItem2.left + 3*nWidth;;//rcItem2.Width()/4;
		rcItemComm.top = rcItem2.top;
		rcItemComm.bottom = rcItem2.bottom;

		m_scRectForCommDlg =rcItemComm; 
		for (i=0; i<nPlatformNum; i++)
		{
			pCommSettingDlg = m_vpCommSettingDlg.at(i);
			pCommSettingDlg->MoveWindow(rcItemComm);
		}


		// 初始化平台选项界面参数
		m_dlgPlatformOptionParam.MoveWindow(rcItem2);
		
		CDlgAuthorizeSet *pDlgAuthorizeSet = NULL;
		for (i=0; i<nPlatformNum; i++)
		{
			pDlgAuthorizeSet = m_vpAuthorizeSetDlg.at(i);
			pDlgAuthorizeSet->MoveWindow(rcItem2);
		}
		
		// 初始化布局界面窗口大小
		m_dlgSysLayoutParamSetting.MoveWindow(rcItem2);
	}
}

LRESULT CDlgSysEnvironSetting::OnGuiDisplayDragStop(WPARAM wParam, LPARAM lParam)
{
	if (wParam == m_pGuiDisplay->GetID())
	{
		if (m_pGuiDisplay->GetImageCP() != NULL && m_pGuiDisplay->GetImageCP()->IsValid())
		{	
			if (m_vpGuiCross.at(0)->IsVisible() && m_vpGuiCross.at(0)->IsSelected())
			{
				double x;
				double y;
				m_vpGuiCross.at(0)->GetCenterXY(x, y);
				m_dlgSysImgRoiParam.m_dCrossX = x;
				m_dlgSysImgRoiParam.m_dCrossY = y;
				m_dlgSysImgRoiParam.m_strCrossX.Format(_T("%.01f"), m_dlgSysImgRoiParam.m_dCrossX);
				m_dlgSysImgRoiParam.m_strCrossY.Format(_T("%.01f"), m_dlgSysImgRoiParam.m_dCrossY);
				m_dlgSysImgRoiParam.UpdateData(FALSE);

				ImageRoi roi;
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetImageROIParam(m_dlgSysImgRoiParam.m_nCameraIndex, roi);
				roi.m_dCrossX = m_dlgSysImgRoiParam.m_dCrossX;
				roi.m_dCrossY = m_dlgSysImgRoiParam.m_dCrossY;
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetImageROIParam(m_dlgSysImgRoiParam.m_nCameraIndex, roi);
			}
		}
	}
	
	return 0 ;
}

LRESULT CDlgSysEnvironSetting::OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam)
{
	CString strStatus = " ";
	
	if (wParam == m_pGuiDisplay->GetID())
	{
		if (m_pGuiDisplay->GetImageCP() != NULL)
		{
			if (m_pGuiDisplay->GetImageCP()->IsValid())
			{
				m_pGuiDisplay->SetAutoFit(FALSE);
				long nWidth  = m_pGuiDisplay->GetImageCP()->Width();
				long nHeight = m_pGuiDisplay->GetImageCP()->Height();
				
				// 获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标,低16位存储的是X坐标
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				
				// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
				double dImageX, dImageY;
				m_pGuiDisplay->TransformPos(x,y,dImageX,dImageY);
				
				// 获取对应像素灰度值
				BYTE R,G,B;
				m_pGuiDisplay->GetImagePixelValue(LONG(dImageX),LONG(dImageY),R,G,B);
				if(dImageX<0||dImageY<0||dImageY>=nHeight||dImageX>=nWidth)
				{
					R = 0;
					G = 0;
					B = 0;
				}
				
				strStatus.Format("(%.0f,%.0f)  (%d,%d,%d)", dImageX, dImageY, R, G, B);
			}
		}	
	}
	
	m_pStatusBar->SetText(strStatus,3,0);
	
	return 0 ;
}

void CDlgSysEnvironSetting::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSysEnvironSetting::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_ENVIRONMENT_SETTING") == strDlgID)
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

void CDlgSysEnvironSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysEnvironSetting)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM5, m_btnShowPlatform5);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM6, m_btnShowPlatform6);
	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	DDX_Control(pDX, IDC_TAB, m_tabSheet);
	DDX_Control(pDX, IDC_RADIO_ENVIRON_CAM, m_btnEnvironCam);	
	DDX_Control(pDX, IDC_RADIO_ENVIRON_COMM, m_btnEnvironComm);
	DDX_Control(pDX, IDC_RADIO_ENVIRON_PLATFORM, m_btnEnvironPlatform);
	DDX_Control(pDX, IDC_RADIO_ENVIRON_LOG, m_btnEnvironLog);
	DDX_Control(pDX,IDC_RADIO_ENVIRON_SEARCHTOOL,m_btnEnvironSearchTool);
	DDX_Control(pDX,IDC_RADIO_ENVIRON_LAYOUT,m_btnEnvironLayout);
	DDX_Control(pDX, IDC_RADIO_ENVIRON_AUTHORIZESET, m_btnEnvironAuthorizeSet);
	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_ENVIRON_CAM, m_nRadioEnvironType);
}

BEGIN_MESSAGE_MAP(CDlgSysEnvironSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysEnvironSetting)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM5, OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM6, OnPlatformChange)
	ON_MESSAGE(WM_SV_DISPLAY_MOUSEMOVE,OnGuiDisplayMouseMove)
	ON_MESSAGE(WM_SV_DISPLAY_DRAGSTOP,OnGuiDisplayDragStop)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_ENVIRON_CAM, &CDlgSysEnvironSetting::OnBnClickedRadioEnvironCam)
	ON_BN_CLICKED(IDC_RADIO_ENVIRON_COMM, &CDlgSysEnvironSetting::OnBnClickedRadioEnvironComm)
	ON_BN_CLICKED(IDC_RADIO_ENVIRON_PLATFORM, &CDlgSysEnvironSetting::OnBnClickedRadioEnvironPlatform)
	ON_BN_CLICKED(IDC_RADIO_ENVIRON_LOG, &CDlgSysEnvironSetting::OnBnClickedRadioEnvironLog)
	ON_BN_CLICKED(IDC_RADIO_ENVIRON_SEARCHTOOL,&CDlgSysEnvironSetting::OnBnClickedRadioEnvironSearchTool)
	ON_BN_CLICKED(IDC_RADIO_ENVIRON_AUTHORIZESET, &CDlgSysEnvironSetting::OnBnClickedRadioEnvironAuthorizeset)
	ON_BN_CLICKED(IDC_RADIO_ENVIRON_LAYOUT,&CDlgSysEnvironSetting::OnBnClickedRadioEnvironLayout)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysEnvironSetting message handlers

BOOL CDlgSysEnvironSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	InitDlgItem();
	UpdateDialogLanguage();
	if (m_pParent->m_SysOptionConfig.m_bPlatformCustomNameEnable)
	{
		for (int i = 0; i < MaxPlatformCustomNameNum && i < MaxPlatformNum; i++)
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->SetWindowText(m_pParent->m_SysOptionConfig.m_strPlatformCustomName[i]);
		}
	}

	GetDlgItem(IDC_RADIO_ENVIRON_AUTHORIZESET)->ShowWindow(m_pParent->m_nUsePassWordMode==0);
	GetDlgItem(IDC_RADIO_ENVIRON_AUTHORIZESET)->EnableWindow(m_pParent->m_nUsePassWordMode==0);

	CString strLog;
	strLog.Format(_T("进入工位%d[相机]设置页面\n"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	MoveWindow(m_pParent->m_rcViewRect);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysEnvironSetting::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
// 	UpdateData(TRUE);
// 	int nCurSel = m_tabSheet.GetCurSel();
// 	if (nCurSel == m_nPrevTabIndex)
// 	{
// 		return;
// 	}
// 	
// 	CString strItem;
// 	TCITEM tcItem;
// 	CHAR chChar[256];
// 	
// 	//  Get the current tab item text.
// 	tcItem.mask = TCIF_TEXT;
// 	tcItem.pszText = chChar;
// 	tcItem.cchTextMax = 256;
// 	
// 	m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
// 	strItem = tcItem.pszText;
// 	
// 	if (TABSHEET_ITEM_CAMERA == strItem)
// 	{
// 		m_dlgSysImgRoiParam.ShowWindow(SW_HIDE);
// 	}
// 	else if (TABSHEET_ITEM_PLATFORM == strItem)
// 	{
// 		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);
// 	}
// 	else if (TABSHEET_ITEM_COMM == strItem)
// 	{
// 		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);
// 	}
// 	else if (TABSHEET_ITEM_LOG == strItem)
// 	{
// 		m_dlgSysDataRecord.ShowWindow(SW_HIDE);
// 	}
// 	
// 	//  Get the current tab item text.
// 	tcItem.mask = TCIF_TEXT;
// 	tcItem.pszText = chChar;
// 	tcItem.cchTextMax = 256;
// 	m_tabSheet.GetItem(nCurSel, &tcItem);
// 	strItem = tcItem.pszText;
// 	
// 	if (TABSHEET_ITEM_CAMERA == strItem)
// 	{
// 		m_dlgSysImgRoiParam.ShowWindow(SW_SHOW);
// 	}
// 	else if (TABSHEET_ITEM_PLATFORM == strItem)
// 	{
// 		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
// 	}
// 	else if (TABSHEET_ITEM_COMM == strItem)
// 	{
// 		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
// 	}
// 	else if (TABSHEET_ITEM_LOG == strItem)
// 	{
// 		m_dlgSysDataRecord.ShowWindow(SW_SHOW);
// 	}
// 	
// 	
// 	m_nPrevTabIndex = nCurSel;
// 	
// 	switch(m_nPrevTabIndex)
// 	{
// 	case 0:
// 		m_dlgSysImgRoiParam.ShowWindow(SW_HIDE);
// 		break;
// 	case 1:
// 		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);
// 		break;
// 	case 2:
// 		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);
// 		break;
// 	case 3:
// 		m_dlgSysDataRecord.ShowWindow(SW_HIDE);
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	switch(nCurSel)
// 	{
// 	case 0:
// 		m_dlgSysImgRoiParam.ShowWindow(SW_SHOW);
// 		break;
// 	case 1:
// 		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
// 		break;
// 	case 2:
// 		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
// 		break;
// 	case 3:
// 		m_dlgSysDataRecord.ShowWindow(SW_SHOW);
// 		break;
// 	default:
// 		break;
// 	}
// 	m_nPrevTabIndex = nCurSel;

	*pResult = 0;
}

void CDlgSysEnvironSetting::OnPlatformChange() 
{
	UpdateData(TRUE);
	if (m_nPlatformIndex == m_nPrevPlatformIndex)
	{
		return;
	}
	
	BeginWaitCursor();
	// 更新相机参数
	m_dlgSysImgRoiParam.UpdateDlgData(TRUE);
	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	vcBaseVisionAlign *pPrevVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex);
	if (m_dlgSysImgRoiParam.IsCameraGrabing())	// 更新GUIDisplay
	{
		pPrevVisionASM->FreezeAllCameras();
//		pPrevVisionASM->FreezeCamera(m_dlgSysImgRoiParam.m_nCameraIndex);
		if ( (m_dlgSysImgRoiParam.m_nCameraIndex >= pVisionASM->GetPosNum()) 
			|| (m_dlgSysImgRoiParam.m_nCameraIndex < 0) )
		{
			m_dlgSysImgRoiParam.m_nCameraIndex = 0;
		}
		pVisionASM->GrabPosCamera(m_dlgSysImgRoiParam.m_nCameraIndex);
	}
	m_dlgSysImgRoiParam.SetVisionASMRef(pVisionASM);
	m_dlgSysImgRoiParam.UpdateDlgData(FALSE);

	// 更新平台参数
	m_vpDlgSysPlatformParam.at(m_nPrevPlatformIndex)->UpdateDlgData(TRUE);
	CPlatformInfo *pPlatformInfo = m_vpDlgSysPlatformParam.at(m_nPrevPlatformIndex)->GetPlatformInfo();
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetPlatformInfo(pPlatformInfo);

	CCamPlatformInfo *pCamPlatformInfo = m_vpDlgSysPlatformParam.at(m_nPrevPlatformIndex)->GetCamPlatformInfo();
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetCamPlatformInfo(pCamPlatformInfo);
	m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->UpdateDlgData(FALSE);

	// 更新中间平台参数
	m_vpDlgSysMidPlatformParam.at(m_nPrevPlatformIndex)->UpdateDlgData(TRUE);
	pPlatformInfo = m_vpDlgSysMidPlatformParam.at(m_nPrevPlatformIndex)->GetPlatformInfo();
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetMidPlatformInfo(pPlatformInfo);
	m_vpDlgSysMidPlatformParam.at(m_nPlatformIndex)->UpdateDlgData(FALSE);


	if (m_nPrevTabIndex == 2)
	{
		m_vpDlgSysPlatformParam.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);

		m_vpDlgSysMidPlatformParam.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
		m_vpDlgSysMidPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
	}

	// 更新通信参数
	m_vpCommSettingDlg.at(m_nPrevPlatformIndex)->UpdateDlgData(TRUE);
	m_vpCommSettingDlg.at(m_nPlatformIndex)->UpdateDlgData(FALSE);
	//更新通信类型参数
	m_vpdlgCommTypeSetting.at(m_nPrevPlatformIndex)->UpdateDlgData(TRUE);
	m_vpdlgCommTypeSetting.at(m_nPlatformIndex)->UpdateDlgData(FALSE);
	
	if (m_nPrevTabIndex == 1)
	{
		m_vpCommSettingDlg.at(m_nPrevPlatformIndex)->SetVisible(FALSE);
		m_vpCommSettingDlg.at(m_nPlatformIndex)->SetVisible(TRUE);

		m_vpCommSettingDlg.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);

		m_vpCommSettingDlg.at(m_nPlatformIndex)->UpdateCommStatus();


		//更新通信类型参数

		m_vpdlgCommTypeSetting.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
		m_vpdlgCommTypeSetting.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);



	}

	if (m_nPrevTabIndex == 6)
	{
		//更新权限界面
		m_vpAuthorizeSetDlg.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
		m_vpAuthorizeSetDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
	}

// 	CString strItem;
// 	TCITEM tcItem;
// 	CHAR chChar[256];	
// 	//  Get the current tab item text.
// 	tcItem.mask = TCIF_TEXT;
// 	tcItem.pszText = chChar;
// 	tcItem.cchTextMax = 256;
// 	m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
// 	strItem = tcItem.pszText;
// 
// 
// 	
// 	if (TABSHEET_ITEM_PLATFORM == strItem)
// 	{
// 		m_vpDlgSysPlatformParam.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
// 		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
// 	}
// 	
// 	
// 	// 更新通信
// 	if (TABSHEET_ITEM_COMM == strItem)
// 	{
// 		m_vpCommSettingDlg.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
// 		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
// 	}
	
	// 	if (m_tabSheet.GetCurSel() == 1)
	// 	{
	// 		m_vpDlgSysPlatformParam.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
	// 		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
	// 	}
	// 
	// 
	// 	// 更新通信
	// 	if (m_tabSheet.GetCurSel() == 2)
	// 	{
	// 		m_vpCommSettingDlg.at(m_nPrevPlatformIndex)->ShowWindow(SW_HIDE);
	// 		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
	// 	}



	// 更新数据记录设置
	m_dlgSysDataRecord.UpdateDlgData(TRUE); 
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetSysDataRecord(m_dlgSysDataRecord.GetSysDataRecord());
	m_dlgSysDataRecord.SetSysDataRecord(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysDataRecord());
	m_dlgSysDataRecord.SetPlatformIndex(m_nPlatformIndex);

	m_SysOptionConfig.m_bStartUpShowCamSNSetting = m_dlgSysDataRecord.m_bTempStartUpShowCamSN;
	m_dlgSysDataRecord.m_bTempStartUpShowCamSN = m_SysOptionConfig.m_bStartUpShowCamSNSetting;

	m_SysOptionConfig.m_bSysKeyboardEnabled = m_dlgSysDataRecord.m_bTempSysKeyboard;
	m_dlgSysDataRecord.m_bTempSysKeyboard = m_SysOptionConfig.m_bSysKeyboardEnabled;
    m_dlgSysDataRecord.UpdateDlgData(FALSE);
	//m_nRadioEnvironType
	CString strVal[] = {_T("相机"),_T("通信"),_T("平台"),_T("日志"),_T("平台选项"), _T("布局"),_T("权限管理")};
    CString strLog,strPrev;
	if (1)
	{
		strPrev = strVal[m_nPrevTabIndex];
	}	
	switch(m_nRadioEnvironType)
	{
	case 0:
		strLog.Format(_T("退出工位%d[%s]设置页面\n"),m_nPrevPlatformIndex+1,strPrev);
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("进入工位%d[相机]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 1:
		strLog.Format(_T("退出工位%d[%s]设置页面\n"),m_nPrevPlatformIndex+1,strPrev);
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("进入工位%d[通信]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;	
	case 2:
		strLog.Format(_T("退出工位%d[%s]设置页面\n"),m_nPrevPlatformIndex+1,strPrev);
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("进入工位%d[平台]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 3:
		strLog.Format(_T("退出工位%d[%s]设置页面\n"),m_nPrevPlatformIndex+1,strPrev);
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("进入工位%d[日志]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
    case 4:
        strLog.Format(_T("退出工位%d[%s]设置页面\n"),m_nPrevPlatformIndex+1,strPrev);
        g_opLogFile.SaveLogText(strLog,2);
        strLog.Format(_T("进入工位%d[平台选项]设置页面\n"),m_nPlatformIndex+1);
        g_opLogFile.SaveLogText(strLog,2);
        break;
	case 5:
		strLog.Format(_T("退出工位%d[%s]设置页面\n"),m_nPrevPlatformIndex+1,strPrev);
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("进入工位%d[布局]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 6:
		strLog.Format(_T("退出工位%d[%s]设置页面\n"),m_nPrevPlatformIndex+1,strPrev);
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("进入工位%d[权限管理]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	default:
		break;
	}

	//保存该平台的平台选项
	m_dlgPlatformOptionParam.UpdateDlgData(TRUE);
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetDlgSearchToolData(m_dlgPlatformOptionParam.GetDlgSearchToolData());
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetMutiTarget();
	//lzk
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetAlignerTypeInfo(m_dlgPlatformOptionParam.GetAlignerTypeInfo());
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetPlatformOptionInfo(m_dlgPlatformOptionParam.GetPlatformOptionInfo());

	

   //更新平台选项设置
	m_dlgPlatformOptionParam.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	m_dlgPlatformOptionParam.SetDlgSearchToolData(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetDlgSearchToolData());
	//lzk
	m_dlgPlatformOptionParam.SetAlignerTypeInfo(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo());//lzk

	CPlatformOptionInfo optionInfo;
	//获取该平台的平台选项并设置给对话框
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	m_dlgPlatformOptionParam.SetPlatformOptionInfo(optionInfo);
    m_dlgPlatformOptionParam.UpdateDlgData(FALSE);

	// 保存上一个平台布局信息
	m_dlgSysLayoutParamSetting.UpdateDlgData(TRUE);
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIndex)->SetPlatformLayoutInfo(m_dlgSysLayoutParamSetting.GetPlatformLayoutInfo());


	// 更新当前平台布局信息
	CPlatformLayoutInfo layoutInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformLayoutInfo(layoutInfo);
	m_dlgSysLayoutParamSetting.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	m_dlgSysLayoutParamSetting.SetPlatformLayoutInfo(layoutInfo);
	m_dlgSysLayoutParamSetting.UpdateDlgData(FALSE);

	//更新通信选项设置
	m_dlgCommParamSetting.UpdateDlgData(TRUE);
    m_dlgCommParamSetting.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	CCommOptionInfo commOptionInfo;
	//获取该平台的平台选项并设置给对话框
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCommOptionInfoXML(commOptionInfo);
	m_dlgCommParamSetting.SetCommOptionInfo(commOptionInfo);
	m_dlgCommParamSetting.UpdateDlgData(FALSE);

	EndWaitCursor();
	m_nPrevPlatformIndex = m_nPlatformIndex;
}

BOOL CDlgSysEnvironSetting::DestroyWindow()
{
	m_dlgSysImgRoiParam.DestroyWindow();
	m_dlgSysDataRecord.DestroyWindow();
	m_dlgPlatformOptionParam.DestroyWindow();
	m_dlgSysLayoutParamSetting.DestroyWindow();

	int nSize = m_vpDlgSysPlatformParam.size();
	int i=0;
	for (i=0; i<nSize; i++)
	{
		m_vpDlgSysPlatformParam.at(i)->DestroyWindow();
		delete m_vpDlgSysPlatformParam.at(i);
		m_vpDlgSysPlatformParam.at(i) = NULL;
	}
	m_vpDlgSysPlatformParam.clear();


	for (i=0; i<m_vpDlgSysMidPlatformParam.size(); i++)
	{
		m_vpDlgSysMidPlatformParam.at(i)->DestroyWindow();
		delete m_vpDlgSysMidPlatformParam.at(i);
		m_vpDlgSysMidPlatformParam.at(i) = NULL;
	}
	m_vpDlgSysMidPlatformParam.clear();
	
	for (i=0; i<m_vpCommSettingDlg.size(); i++)
	{
		if (m_vpCommSettingDlg.at(i) != NULL)
		{
			m_vpCommSettingDlg.at(i)->DestroyWindow();
			delete m_vpCommSettingDlg.at(i);
			m_vpCommSettingDlg.at(i) = NULL;
		}
	}
	m_vpCommSettingDlg.clear();
	m_dlgCommParamSetting.DestroyWindow();



	for (i=0; i<m_vpdlgCommTypeSetting.size(); i++)
	{
		if (m_vpdlgCommTypeSetting.at(i) != NULL)
		{
			m_vpdlgCommTypeSetting.at(i)->DestroyWindow();
			delete m_vpdlgCommTypeSetting.at(i);
			m_vpdlgCommTypeSetting.at(i) = NULL;
		}
	}
	m_vpdlgCommTypeSetting.clear();
	

	for (i=0; i<m_vpAuthorizeSetDlg.size(); i++)
	{
		if (m_vpAuthorizeSetDlg.at(i) != NULL)
		{
			m_vpAuthorizeSetDlg.at(i)->DestroyWindow();
			delete m_vpAuthorizeSetDlg.at(i);
			m_vpAuthorizeSetDlg.at(i) = NULL;
		}
	}
	m_vpAuthorizeSetDlg.clear();


	if (m_pGuiDisplay != NULL)
	{
		delete m_pGuiDisplay;
		m_pGuiDisplay = NULL;
	}

	for (i=0; i<m_vpGuiCross.size(); i++)
	{		
		if ( m_vpGuiCross.at(i))
		{
			delete m_vpGuiCross.at(i);
			m_vpGuiCross.at(i) = NULL;
		}		
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



	// 恢复各视觉对位工具的主窗口
	for (i=0; i<m_pParent->m_SystemConfigure.GetPlatformNum(); i++)
	{
		m_pParent->m_vpVisionASM.at(i)->SetMainWnd(m_vpVAPrevMainWnd.at(i));
	}

	return CDialog::DestroyWindow();
}

void CDlgSysEnvironSetting::OnOK() 
{
	UpdateData(TRUE);

	CDlgSysPlatformParam *pDlg = m_vpDlgSysPlatformParam.at(m_nPlatformIndex);
	pDlg->UpdateDlgData(TRUE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPlatformInfo(pDlg->GetPlatformInfo());
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCamPlatformInfo(pDlg->GetCamPlatformInfo());

	pDlg = m_vpDlgSysMidPlatformParam.at(m_nPlatformIndex);
	pDlg->UpdateDlgData(TRUE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetMidPlatformInfo(pDlg->GetPlatformInfo());


	m_dlgSysImgRoiParam.UpdateDlgData(TRUE);

	m_dlgSysDataRecord.UpdateDlgData(TRUE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetSysDataRecord(m_dlgSysDataRecord.GetSysDataRecord());
	
	m_vpdlgCommTypeSetting.at(m_nPlatformIndex)->UpdateDlgData(TRUE);
	m_vpCommSettingDlg.at(m_nPlatformIndex)->UpdateDlgData(TRUE);
//	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCommInfo();

	m_SysOptionConfig.m_bStartUpShowCamSNSetting = m_dlgSysDataRecord.m_bTempStartUpShowCamSN;
	m_SysOptionConfig.m_bSysKeyboardEnabled = m_dlgSysDataRecord.m_bTempSysKeyboard;
	
	m_SysOptionConfig.SaveSystemOption();


	m_dlgPlatformOptionParam.UpdateDlgData(TRUE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetDlgSearchToolData(m_dlgPlatformOptionParam.GetDlgSearchToolData());
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPlatformOptionInfo(m_dlgPlatformOptionParam.GetPlatformOptionInfo());
	//lzk
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlignerTypeInfo(m_dlgPlatformOptionParam.GetAlignerTypeInfo());

	// 更新布局显示
	m_dlgSysLayoutParamSetting.UpdateDlgData(TRUE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPlatformLayoutInfo(m_dlgSysLayoutParamSetting.GetPlatformLayoutInfo());


	m_dlgCommParamSetting.UpdateDlgData(TRUE);
	int i =0;
	int nSize = m_pParent->m_vpVisionASM.size();
	for (i=0; i<nSize; i++)
	{
		m_pParent->m_vpVisionASM.at(i)->FreezeAllCameras();
		m_pParent->m_vpVisionASM.at(i)->m_bExecuteGitCommit=FALSE;

		m_pParent->m_vpVisionASM.at(i)->SaveCameraInfo();
		m_pParent->m_vpVisionASM.at(i)->SaveJobCameraInfo();
		m_pParent->m_vpVisionASM.at(i)->SaveImageRoiInfo();
		m_pParent->m_vpVisionASM.at(i)->SavePlatformInfo();
		m_pParent->m_vpVisionASM.at(i)->SaveCamPlatformInfo();
		m_pParent->m_vpVisionASM.at(i)->SaveMidPlatformInfo();
		m_pParent->m_vpVisionASM.at(i)->SaveDataRecordInfo();
		m_pParent->m_vpVisionASM.at(i)->SaveSearchToolDataInfo();
		m_pParent->m_vpVisionASM.at(i)->SavePlatformOptionInfo();//保存该平台的平台选项
		m_pParent->m_vpVisionASM.at(i)->SaveCommOptionInfo();//保存该平台的通信选项
		m_pParent->m_vpVisionASM.at(i)->SavePlatformLayoutInfo(); // 保存平台布局配置
		m_pParent->m_vpVisionASM.at(i)->SaveAuthorizeManagement(); //保存平台权限关联设置
		//lzk
		m_pParent->m_vpVisionASM.at(i)->SaveAlignerTypeInfo();
//		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCommInfo();


		m_pParent->m_vpVisionASM.at(i)->m_bExecuteGitCommit=TRUE;
		m_pParent->m_vpVisionASM.at(i)->SaveCommInfo();

		m_pParent->m_vpVisionASM.at(i)->SetMutiTarget();
		m_pParent->UpdateViewDisplayLayout(i);

		if (m_pParent->m_vpVisionASM.at(i)->GetSysDataRecord().m_bDataRecordTime == FALSE)
		{
			// 删除时间文件结构，更新m_strDataRecord、m_strAlignProcessData和m_strCommunicationData的具体路径
			m_pParent->m_vpVisionASM.at(i)->UpdateDataRecordTxtAddr();

			// 挂起删除文件夹线程
			if (m_pParent->m_vpVisionASM.at(i)->m_bCheckRemoveFolderThreadSuspend == FALSE)
			{
				m_pParent->m_vpVisionASM.at(i)->m_pCheckRemoveFolderThread->SuspendThread();
				m_pParent->m_vpVisionASM.at(i)->m_bCheckRemoveFolderThreadSuspend = TRUE;
			}
			
		}
		else
		{
			// 增加时间文件结构，更新m_strDataRecord、m_strAlignProcessData和m_strCommunicationData的具体路径
			m_pParent->m_vpVisionASM.at(i)->UpdateDataRecordTxtAddrTime();

			// 恢复删除文件夹线程
			if (m_pParent->m_vpVisionASM.at(i)->m_bCheckRemoveFolderThreadSuspend == TRUE)
			{
				m_pParent->m_vpVisionASM.at(i)->m_pCheckRemoveFolderThread->ResumeThread();
				m_pParent->m_vpVisionASM.at(i)->m_bCheckRemoveFolderThreadSuspend = FALSE;
			}
			
		}

	}
	for (int g=0;g<nSize;g++)
	{
		m_pParent->m_vpVisionASM.at(g)->UpdateShareCamTransferSame();
	}

	CString strLog;
	switch(m_nRadioEnvironType)
	{
	case 0:
		strLog.Format(_T("退出工位%d[相机]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 1:
		strLog.Format(_T("退出工位%d[通信]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;	
	case 2:
		strLog.Format(_T("退出工位%d[平台]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 3:
		strLog.Format(_T("退出工位%d[日志]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
    case 4:
        strLog.Format(_T("退出工位%d[平台选项]设置页面"),m_nPlatformIndex+1);
        g_opLogFile.SaveLogText(strLog,2);
        break;
	case 5:
		strLog.Format(_T("退出工位%d[平台选项]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
    case 6:
        strLog.Format(_T("退出工位%d[权限管理]设置页面"),m_nPlatformIndex+1);
        g_opLogFile.SaveLogText(strLog,2);
        break;
	default:
		break;
	}
	strLog.Format(_T("[保存]退出系统设置页面\n"));
	g_opLogFile.SaveLogText(strLog,1);
	CDialog::OnOK();
}

void CDlgSysEnvironSetting::OnCancel() 
{
	int nSize = m_pParent->m_vpVisionASM.size();
	for (int i=0; i<nSize; i++)
	{
		m_pParent->m_vpVisionASM.at(i)->FreezeAllCameras();

		m_pParent->m_vpVisionASM.at(i)->LoadCameraInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadJobCameraInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadImageRoiInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadPlatformInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadCamPlatformInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadMidPlatformInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadDataRecordInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadSearchToolDataInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadPlatformOptionInfo();
		m_pParent->m_vpVisionASM.at(i)->LoadCommOptionInfo();
		//lzk
		m_pParent->m_vpVisionASM.at(i)->LoadAlignerTypeInfo();

		// 从文件配置载入权限关联配置；恢复修改过的数据为初始状态
		m_pParent->m_vpVisionASM.at(i)->LoadAuthorizeManagement();

		int nCamNum = m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
		for (int j=0; j<nCamNum; j++)
		{
			// Mali 2013.8.16
			// 恢复相机本身状态	
			m_pParent->m_vpVisionASM.at(i)->UpdateCameraStatus(j);
			
// 			// Mali 2013.8.16
// 			// 恢复旋转镜像状态
// 			m_pParent->m_vpVisionASM.at(i)->GetImageROIParam(j, imageRoi);
// 			m_pParent->m_vpVisionASM.at(i)->SetImageROIParam(j, imageRoi);

		}


		int nPosNum = m_pParent->m_vpVisionASM.at(i)->GetPosNum();
		ImageRoi imageRoi;
		for (int k=0; k<nPosNum; k++)
		{
			// Mali 2013.8.16
			// 恢复旋转镜像状态
			m_pParent->m_vpVisionASM.at(i)->GetPosImageROIParam(k, imageRoi);
			m_pParent->m_vpVisionASM.at(i)->SetPosImageROIParam(k, imageRoi);

		}

		m_pParent->m_vpVisionASM.at(i)->LoadCommInfo();

		m_pParent->m_vpVisionASM.at(i)->SetParamToXYDCommProtocol();
		m_pParent->m_vpVisionASM.at(i)->ReInitComm();

		m_pParent->m_vpVisionASM.at(i)->InitAlignTool();

	}

	for (int g=0;g<nSize;g++)
	{
		m_pParent->m_vpVisionASM.at(g)->UpdateShareCamTransferSame();
	}
	CString strLog;
	switch(m_nRadioEnvironType)
	{
	case 0:
		strLog.Format(_T("退出工位%d[相机]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 1:
		strLog.Format(_T("退出工位%d[通信]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;	
	case 2:
		strLog.Format(_T("退出工位%d[平台]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 3:
		strLog.Format(_T("退出工位%d[日志]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
    case 4:
        strLog.Format(_T("退出工位%d[平台选项]设置页面"),m_nPlatformIndex+1);
        g_opLogFile.SaveLogText(strLog,2);
        break;
	default:
		break;
	}
	strLog.Format(_T("[不保存]退出系统设置页面\n"));
	g_opLogFile.SaveLogText(strLog,1);
	CDialog::OnCancel();
}

BOOL CDlgSysEnvironSetting::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSysEnvironSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnviron()
{
	UpdateData(TRUE);
	int nCurSel = m_nRadioEnvironType;
	if (nCurSel == m_nPrevTabIndex)
	{
		return;
	}
	CString strLog;
	switch(m_nPrevTabIndex)
	{
	case 0:
		m_pGuiDisplay->ShowWindow(SW_HIDE);
		m_dlgSysImgRoiParam.ShowWindow(SW_HIDE);
		strLog.Format(_T("退出工位%d[相机]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 1:
		m_vpCommSettingDlg.at(m_nPlatformIndex)->SetVisible(FALSE);
		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);

		m_vpdlgCommTypeSetting.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);

		m_dlgCommParamSetting.ShowWindow(SW_HIDE);
		strLog.Format(_T("退出工位%d[通信]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;	
	case 2:
		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);
		m_vpDlgSysMidPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);
		strLog.Format(_T("退出工位%d[平台]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 3:
		m_dlgSysDataRecord.ShowWindow(SW_HIDE);
		strLog.Format(_T("退出工位%d[日志]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 4:
		m_dlgPlatformOptionParam.ShowWindow(SW_HIDE);
        strLog.Format(_T("退出工位%d[平台选项]设置页面"),m_nPlatformIndex+1);
        g_opLogFile.SaveLogText(strLog,2);
		break;
	case 5:
		m_dlgSysLayoutParamSetting.ShowWindow(SW_HIDE);
		strLog.Format(_T("退出工位%d[布局]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 6:
		{
			m_vpAuthorizeSetDlg.at(m_nPlatformIndex)->ShowWindow(SW_HIDE);
			strLog.Format(_T("退出工位%d[权限设置]设置页面"),m_nPlatformIndex+1);
			g_opLogFile.SaveLogText(strLog,2);
		}
		break;
	default:
		break;
	}

	switch(nCurSel)
	{
	case 0:
		m_pGuiDisplay->ShowWindow(SW_SHOW);
		m_dlgSysImgRoiParam.ShowWindow(SW_SHOW);

		strLog.Format(_T("进入工位%d[相机]设置页面\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);

		break;
	case 1:
		m_vpCommSettingDlg.at(m_nPlatformIndex)->SetVisible(TRUE);
		m_vpCommSettingDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);

		m_vpdlgCommTypeSetting.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
		
		m_dlgCommParamSetting.ShowWindow(SW_SHOW);
		m_vpCommSettingDlg.at(m_nPlatformIndex)->UpdateCommStatus();
		
		strLog.Format(_T("进入工位%d[通信]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);

		break;	
	case 2:
		m_vpDlgSysPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
		m_vpDlgSysMidPlatformParam.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);

		strLog.Format(_T("进入工位%d[平台]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);

		break;
	case 3:
		m_dlgSysDataRecord.ShowWindow(SW_SHOW);

		strLog.Format(_T("进入工位%d[日志]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);

		break;
	case 4:
		m_dlgPlatformOptionParam.ShowWindow(SW_SHOW);

        strLog.Format(_T("进入工位%d[平台选项]设置页面"),m_nPlatformIndex+1);
        g_opLogFile.SaveLogText(strLog,2);
		break;
	case 5:
		m_dlgSysLayoutParamSetting.ShowWindow(SW_SHOW);
		strLog.Format(_T("进入工位%d[布局]设置页面"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
		break;
	case 6:
		{
			m_vpAuthorizeSetDlg.at(m_nPlatformIndex)->ShowWindow(SW_SHOW);
			strLog.Format(_T("进入工位%d[权限管理]设置页面"),m_nPlatformIndex+1);
			g_opLogFile.SaveLogText(strLog,2);
		}
		break;
	default:
		break;
	}
	m_nPrevTabIndex = nCurSel;
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnvironCam()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedRadioEnviron();
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnvironComm()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedRadioEnviron();
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnvironPlatform()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedRadioEnviron();
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnvironLog()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedRadioEnviron();
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnvironSearchTool()
{
	OnBnClickedRadioEnviron();
}

void CDlgSysEnvironSetting::UpdateControlShow(BOOL bVisible)
{
	if (m_nPlatformIndex<m_vpCommSettingDlg.size())
	{
		if(NULL !=m_vpCommSettingDlg.at(m_nPlatformIndex))
		{
			m_vpCommSettingDlg.at(m_nPlatformIndex)->UpdateControlShow(bVisible);
		}
	}

	
}

BOOL CDlgSysEnvironSetting::ChangeCommType(const CommType eCommType)
{
	BOOL bCheck = (eCommType == eSocket || eCommType == eSerial);
	if (!bCheck)
	{
		return FALSE;
	}
	if (m_vpCommSettingDlg.size()==0 || m_nPlatformIndex <0 || m_nPlatformIndex >m_vpCommSettingDlg.size())
	{
		return FALSE;
	}

	if (NULL ==m_vpCommSettingDlg.at(m_nPlatformIndex))
	{
		return FALSE;
	}
	// 将当前工位的产品的通信设置界面重新初始化
	m_vpCommSettingDlg.at(m_nPlatformIndex)->DestroyWindow();
	delete m_vpCommSettingDlg.at(m_nPlatformIndex);
	m_vpCommSettingDlg.at(m_nPlatformIndex) = NULL;

	CDlgSysBaseCommSetting *pCommSettingDlg = NULL;
	if (eCommType == eSerial)
	{
		pCommSettingDlg = new CDlgSysSerialCommSetting();
		pCommSettingDlg->SetParentWnd(this);
		pCommSettingDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
		pCommSettingDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
		pCommSettingDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
		pCommSettingDlg->Create(IDD_SYS_SERIAL_COMM_CONFIG, this);
	}
	else if (eCommType == eSocket)
	{
		pCommSettingDlg = new CDlgSysSocketCommSetting();
		pCommSettingDlg->SetParentWnd(this);
		pCommSettingDlg->SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
		pCommSettingDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
		pCommSettingDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
		pCommSettingDlg->Create(IDD_SYS_SOCKET_COMM_CONFIG, this);
	}
	pCommSettingDlg->MoveWindow(&m_scRectForCommDlg);
	pCommSettingDlg->ShowWindow(SW_SHOW);
	pCommSettingDlg->SetVisible(TRUE);

	m_vpCommSettingDlg.at(m_nPlatformIndex) = pCommSettingDlg;

	// 同步将共享通信的工位的通信设置界面重新初始化进行同样处理
	for (int it=0;it<m_pParent->m_vpVisionASM.size();it++)
	{
		vcBaseVisionAlign* TmpBaseVisionAlign = m_pParent->m_vpVisionASM.at(it);
		if (NULL !=TmpBaseVisionAlign && 
			NULL !=TmpBaseVisionAlign->GetVisionAlignOther() && 
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex) == TmpBaseVisionAlign->GetVisionAlignOther() && 
			eShareCommTar == TmpBaseVisionAlign->GetShareCommInfoArray()->GetShareCommType(TmpBaseVisionAlign->GetSysPlatformIndex()))
		{

			TmpBaseVisionAlign->ChangeCommType(eCommType);

			if (NULL !=m_vpCommSettingDlg.at(it))
			{
				m_vpCommSettingDlg.at(it)->DestroyWindow();
				delete m_vpCommSettingDlg.at(it);
				m_vpCommSettingDlg.at(it) = NULL;
			}

			CDlgSysBaseCommSetting *pCommSettingDlg = NULL;
			if (eCommType == eSerial)
			{
				pCommSettingDlg = new CDlgSysSerialCommSetting();
				pCommSettingDlg->SetParentWnd(this);
				pCommSettingDlg->SetVisionASMRef(TmpBaseVisionAlign);
				pCommSettingDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
				pCommSettingDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
				pCommSettingDlg->Create(IDD_SYS_SERIAL_COMM_CONFIG, this);
			}
			else if (eCommType == eSocket)
			{
				pCommSettingDlg = new CDlgSysSocketCommSetting();
				pCommSettingDlg->SetParentWnd(this);
				pCommSettingDlg->SetVisionASMRef(TmpBaseVisionAlign);
				pCommSettingDlg->SetSysInfoStringsRef(m_psaSysInfoStrings);
				pCommSettingDlg->EnableSysKeyboard(m_bSysKeyboardEnabled);
				pCommSettingDlg->Create(IDD_SYS_SOCKET_COMM_CONFIG, this);
			}
			pCommSettingDlg->MoveWindow(&m_scRectForCommDlg);
			pCommSettingDlg->ShowWindow(SW_HIDE);
			pCommSettingDlg->SetVisible(FALSE);

			m_vpCommSettingDlg.at(it) = pCommSettingDlg;

			CVisionASMConfig *vsConfigTar;
			CVisionASMConfig *vsConfigSrc;
			TmpBaseVisionAlign->GetVSconfig(vsConfigTar);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetVSconfig(vsConfigSrc);
			if (NULL != vsConfigSrc && NULL != vsConfigTar)
			{
				vsConfigTar->m_CommParamInfo = vsConfigSrc ->m_CommParamInfo;
			}


		}
	}


	



	return TRUE;
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnvironAuthorizeset()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedRadioEnviron();
}

void CDlgSysEnvironSetting::OnBnClickedRadioEnvironLayout()
{
	 OnBnClickedRadioEnviron();
}
