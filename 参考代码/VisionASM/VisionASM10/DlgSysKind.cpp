// DlgSysKind.cpp : 实现文件
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysKind.h"
#include "afxdialogex.h"


// CDlgSysKind 对话框

IMPLEMENT_DYNAMIC(CDlgSysKind, CDialog)

CDlgSysKind::CDlgSysKind(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysKind::IDD, pParent)
	, m_nRadioKind(0)
	, m_nPreRadioKind(0)
	, m_nPlatformIndex(0)
{
	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;

	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;

	m_pDlgKindCalibSetting = NULL;
	m_pDlgKindAlignView = NULL;
	m_pDlgKindAlignSetting = NULL;
	m_pDlgKindAlignTarget = NULL;
	m_pDlgKindAlignObject = NULL;
	m_pDlgKindAlignVirtualObject = NULL;
	m_pDlgKindAlignCalibTarget = NULL;
	m_pDlgKindAlignInspect = NULL;
	m_nPrevPlatformIdx = 0;
 	m_pParent = (CVisionASMDlg *)pParent;
 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
 	CTotalProductData *pTotalProduct = NULL;
	CTotalCalibData* pTotalCalib = NULL;
 	for (int i=0; i<nPlatformNum; i++)
 	{
 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
 		m_vpTotalProductData.push_back(pTotalProduct);

		pTotalCalib = m_pParent->m_vpVisionASM.at(i)->GetTotalCalibData();
		m_vpTotalCalibData.push_back(pTotalCalib);
 	}
	m_nPlatformIndex = m_pParent->m_nPlatformDispIdx-1;
	if (m_nPlatformIndex<0)
	{
		m_nPlatformIndex = 0;
	}
	if ( m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData()->GetCount()<1)
	{
		m_nPlatformIndex = 0;
	}
	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_bSinglePlatformProductManageEnable = FALSE;
	m_bInitCreateDlg = FALSE;

	m_bShowTargetTemplateSet = false;
	m_bEnableTargetTemplateSet = false;
	m_bShowObjectTemplateSet = false;
	m_bEnableObjectTemplateSet = false;
	m_bShowVirtualObjectTemplateSet = false;
	m_bEnableVirtualObjectTemplateSet = false;
	m_bShowCalibTargetTemplateSet = false;
	m_bEnableCalibTargetTemplateSet = false;
	m_bShowInspectTemplateSet = false;
	m_bEnableInspectTemplateSet = false;
}

CDlgSysKind::~CDlgSysKind()
{
}

void CDlgSysKind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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
	DDX_Control(pDX, IDC_RADIO_ALIGN, m_btnAlign);
	DDX_Control(pDX, IDC_RADIO_CALIB, m_btnCalib);
	DDX_Control(pDX, IDC_RADIO_KIND_SETTING, m_btnKindSetting);
	DDX_Control(pDX, IDC_RADIO_KIND_TARGET, m_btnKindTarget);
	DDX_Control(pDX, IDC_RADIO_KIND_OBJECT, m_btnKindObject);
	DDX_Control(pDX, IDC_RADIO_KIND_VIRTUAL_OBJECT, m_btnKindVirtualObject);
	DDX_Control(pDX, IDC_RADIO_KIND_CALIB_TARGET, m_btnKindCalibTarget);
	DDX_Control(pDX, IDC_RADIO_KIND_INSPECT, m_btnKindInspect);

	DDX_Control(pDX, IDC_TAB_KIND_ALIGN, m_tabSheet);
	DDX_Radio(pDX, IDC_RADIO_ALIGN, m_nRadioKind);
	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
}


BEGIN_MESSAGE_MAP(CDlgSysKind, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_KIND_ALIGN, &CDlgSysKind::OnTcnSelchangeTabKindAlign)
// 	ON_BN_CLICKED(IDC_RADIO_ALIGN, &CDlgSysKind::OnBnClickedRadioAlign)
// 	ON_BN_CLICKED(IDC_RADIO_CALIB, &CDlgSysKind::OnBnClickedRadioCalib)

	ON_BN_CLICKED(IDC_RADIO_ALIGN, &CDlgSysKind::OnBnClickedRadioKind)
	ON_BN_CLICKED(IDC_RADIO_CALIB, &CDlgSysKind::OnBnClickedRadioKind)
	ON_BN_CLICKED(IDC_RADIO_KIND_SETTING, &CDlgSysKind::OnBnClickedRadioKind)
	ON_BN_CLICKED(IDC_RADIO_KIND_TARGET, &CDlgSysKind::OnBnClickedRadioKind)
	ON_BN_CLICKED(IDC_RADIO_KIND_OBJECT, &CDlgSysKind::OnBnClickedRadioKind)
	ON_BN_CLICKED(IDC_RADIO_KIND_VIRTUAL_OBJECT, &CDlgSysKind::OnBnClickedRadioKind)
	ON_BN_CLICKED(IDC_RADIO_KIND_CALIB_TARGET, &CDlgSysKind::OnBnClickedRadioKind)
	ON_BN_CLICKED(IDC_RADIO_KIND_INSPECT, &CDlgSysKind::OnBnClickedRadioKind)

	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, &CDlgSysKind::OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, &CDlgSysKind::OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, &CDlgSysKind::OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, &CDlgSysKind::OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM5, &CDlgSysKind::OnPlatformChange)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM6, &CDlgSysKind::OnPlatformChange)
	ON_WM_SIZE()
//	ON_BN_CLICKED(IDC_BTN_PANEL, &CDlgSysKind::OnBnClickedBtnPanel)
// 	ON_BN_CLICKED(IDOK, &CDlgSysKind::OnBnClickedOk)
// 	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgSysKind::OnBnClickedBtnApply)

END_MESSAGE_MAP()


// CDlgSysKind 消息处理程序


BOOL CDlgSysKind::OnInitDialog()
{
	CDialog::OnInitDialog();
//	m_bSysKeyboardEnabled = FALSE;

	//m_pParent = (CVisionASMDlg *)pParent;
	//int nPlatformNum = m_pParent->m_vpVisionASM.size();
	//CTotalProductData *pTotalProduct = NULL;
	//for (int i=0; i<nPlatformNum; i++)
	//{
	//	pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
	//	m_vpTotalProductData.push_back(pTotalProduct);
	//}

	CString strModuelDir;
	GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
	strModuelDir.ReleaseBuffer();
	m_strModuleDir = strModuelDir.Left(strModuelDir.ReverseFind('\\'));   

	InitDlgItem();
	UpdateDialogLanguage();
	if (m_pParent->m_SysOptionConfig.m_bPlatformCustomNameEnable)
	{
		for (int i = 0; i < MaxPlatformCustomNameNum && i < MaxPlatformNum; i++)
		{
			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->SetWindowText(m_pParent->m_SysOptionConfig.m_strPlatformCustomName[i]);
		}
	}

	InitSearchToolChanged();//初始化所有的模板工具修改标记
	// TODO:  在此添加额外的初始化
	flag1 = 0;
	flag2 = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSysKind::InitDlgItem()
{
	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && (m_nPlatformIndex<  m_pParent->m_vpVisionASM.size()) && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{

			CString strNodeName = _T("");
			vsConfig->GetCheckedByItem(eProductAuthorize_TargetTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowTargetTemplateSet,m_bEnableTargetTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_ObjectTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowObjectTemplateSet,m_bEnableObjectTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_VirtualObjectTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowVirtualObjectTemplateSet,m_bEnableVirtualObjectTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_CalibTargetTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCalibTargetTemplateSet,m_bEnableCalibTargetTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_InspectTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowInspectTemplateSet,m_bEnableInspectTemplateSet);


		}
	}
//	ShowWindow(SW_MAXIMIZE);
	int i = 0;

	// 调整控制板
	int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

	for (i=0; i<MaxPlatformNum; i++)
	{
		if (i < nPlatformNum)
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


	// 设置按钮
	m_btnOK.SetThemeHelper(&m_themeHelper);
	// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
	// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172, 168, 153), TRUE);
	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);

	m_btnCancel.SetThemeHelper(&m_themeHelper);
	//	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
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


	m_btnShowPlatform1.SetCheck(1);

	m_btnAlign.SetThemeHelper(&m_themeHelper);
	m_btnCalib.SetThemeHelper(&m_themeHelper);
	m_btnKindSetting.SetThemeHelper(&m_themeHelper);
	m_btnKindTarget.SetThemeHelper(&m_themeHelper);
	m_btnKindObject.SetThemeHelper(&m_themeHelper);
	m_btnKindVirtualObject.SetThemeHelper(&m_themeHelper);
	m_btnKindCalibTarget.SetThemeHelper(&m_themeHelper);
	m_btnKindInspect.SetThemeHelper(&m_themeHelper);

	// 创建状态栏
	int nPartWidth[3] = {20, 300, -1};
	CRect rcClient;
	GetClientRect(rcClient);

	int nStatusBarHeight = 0;
// 	rcClient.top = rcClient.bottom - nStatusBarHeight;
// 	m_pStatusBar = new CStatusBarCtrl;
// 	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
// 	m_pStatusBar->SetParts(3, nPartWidth);
// 	m_pStatusBar->ShowWindow(SW_SHOW);
// 
// 	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
// 	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
// 	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
// 	UpdateStatusBar();


	GetClientRect(rcClient);

	// 初始化控制板
	CRect rcPanel, rcItem1, rcItem2;
	GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
	ScreenToClient(rcPanel);

	int nButtonSizeWidth = 80;
//	int nButtonSizeHeight = 50;
	int nButtonSizeHeight = 45;
	int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
	nStatusBarHeight = 0;
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


	CRect rcKindPannel;
	GetDlgItem(IDC_STATIC_KIND_PANEL)->GetWindowRect(rcKindPannel);
	rcItem2.left = rcClient.left;
	rcItem2.right = rcItem1.left + 1;
	rcItem2.top = rcClient.top;
	rcItem2.bottom = rcKindPannel.Height();
	GetDlgItem(IDC_STATIC_KIND_PANEL)->MoveWindow(rcItem2);

	// 修改新选择的平台的显示界面
	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);


	m_pDlgKindAlignView = new CDlgSysKindAlignView;
	m_pDlgKindAlignView->m_pParent = m_pParent;
	m_pDlgKindAlignView->m_pParentKind = this;
	m_pDlgKindAlignView->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
	m_pDlgKindAlignView->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
	m_pDlgKindAlignView->Create(IDD_SYS_KIND_ALIGN_VIEW, this);
	m_pDlgKindAlignView->MoveWindow(rcItem2);

// 	if (0)
// 	{
// 		// 暂时先不创建
// 		m_pDlgKindAlignSetting = new CDlgSysKindAlignSetting;
// 		m_pDlgKindAlignSetting->m_pParent = m_pParent;
// 		m_pDlgKindAlignSetting->m_pParentKind = this;
// 		m_pDlgKindAlignSetting->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignSetting->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignSetting->Create(IDD_SYS_KIND_ALIGN_SETTING, &m_tabSheet);
// 		m_pDlgKindAlignSetting->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignTarget = new CDlgSysKindAlignTarget;
// 		m_pDlgKindAlignTarget->m_pParent = m_pParent;
// 		m_pDlgKindAlignTarget->m_pParentKind = this;
// 		m_pDlgKindAlignTarget->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignTarget->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignTarget->Create(IDD_SYS_KIND_ALIGN_TARGET, &m_tabSheet);
// 		m_pDlgKindAlignTarget->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignObject = new CDlgSysKindAlignObject;
// 		m_pDlgKindAlignObject->m_pParent = m_pParent;
// 		m_pDlgKindAlignObject->m_pParentKind = this;
// 		m_pDlgKindAlignObject->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignObject->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignObject->Create(IDD_SYS_KIND_ALIGN_OBJECT, &m_tabSheet);
// 		m_pDlgKindAlignObject->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignVirtualObject = new CDlgSysKindAlignVirtualObject;
// 		m_pDlgKindAlignVirtualObject->m_pParent = m_pParent;
// 		m_pDlgKindAlignVirtualObject->m_pParentKind = this;
// 		m_pDlgKindAlignVirtualObject->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignVirtualObject->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignVirtualObject->Create(IDD_SYS_KIND_ALIGN_VIRTUAL_OBJECT, &m_tabSheet);
// 		m_pDlgKindAlignVirtualObject->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignCalibTarget = new CDlgSysKindAlignCalibTarget;
// 		m_pDlgKindAlignCalibTarget->m_pParent = m_pParent;
// 		m_pDlgKindAlignCalibTarget->m_pParentKind = this;
// 		m_pDlgKindAlignCalibTarget->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignCalibTarget->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignCalibTarget->Create(IDD_SYS_KIND_ALIGN_CALIB_TARGET, &m_tabSheet);
// 		m_pDlgKindAlignCalibTarget->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignInspect = new CDlgSysKindAlignInspect;
// 		m_pDlgKindAlignInspect->m_pParent = m_pParent;
// 		m_pDlgKindAlignInspect->m_pParentKind = this;
// 		m_pDlgKindAlignInspect->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignInspect->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignInspect->Create(IDD_SYS_KIND_ALIGN_INSPECT, &m_tabSheet);
// 		m_pDlgKindAlignInspect->MoveWindow(rcTabSheet);
// 	}


	m_pDlgKindCalibSetting = new CDlgSysKindCalibSetting;
	m_pDlgKindCalibSetting->m_pParent = m_pParent;
	m_pDlgKindCalibSetting->m_pParentKind = this;
	m_pDlgKindCalibSetting->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
	m_pDlgKindCalibSetting->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
	m_pDlgKindCalibSetting->Create(IDD_SYS_KIND_CALIB_SETTING, this);
	m_pDlgKindCalibSetting->MoveWindow(rcItem2);

	m_rtView = rcItem2;

	m_btnAlign.SetCheck(1);

	/*m_tabSheet.SetCurSel(0);*/
	m_nPrevTabIndex = 0;
	m_pDlgKindAlignView->ShowWindow(TRUE);	
	//	ShowWindow(SW_MAXIMIZE);	

	if (m_bSinglePlatformProductManageEnable == TRUE)
	{
		for (i = 0; i < MaxPlatformNum; i++)
		{
			if (i == m_nPlatformIndex)
			{
				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->EnableWindow(SW_HIDE);
			}
		}
		UpdateData(FALSE);
		m_bInitCreateDlg = TRUE;
		OnPlatformChange();
		m_bInitCreateDlg = FALSE;
	}

	MoveWindow(m_pParent->m_rcViewRect);

	PostMessage(WM_SETFOCUS, NULL, NULL);
	UpdateData(FALSE);
	OnPlatformChange();
}

void CDlgSysKind::MoveDlgItem()
{
	if (GetDlgItem(IDC_BTN_PANEL)->GetSafeHwnd() != NULL)
	{		
		int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();

		// Kind 界面大小
		CRect rcClient;
		GetClientRect(rcClient);
		// 调整 Control Panel
		int i;
		// 初始化控制板
		CRect rcPanel, rcItem1, rcItem2;
		GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
		ScreenToClient(rcPanel);

		int nButtonSizeWidth = 80;
//		int nButtonSizeHeight = 50;
		int nButtonSizeHeight = 45;
		int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
		int nStatusBarHeight = 0;
		GetClientRect(rcClient);

		// 右侧按钮区大小
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


		CRect rcKindPannel;
		GetDlgItem(IDC_STATIC_KIND_PANEL)->GetWindowRect(rcKindPannel);
		rcItem2.left = rcClient.left + 1;
		rcItem2.right = rcItem1.left + 1;
		rcItem2.top = rcClient.top;
		rcItem2.bottom = rcKindPannel.Height();
		GetDlgItem(IDC_STATIC_KIND_PANEL)->MoveWindow(rcItem2);

		CRect rcKindAlignTab;
		GetDlgItem(IDC_TAB_KIND_ALIGN)->GetWindowRect(rcKindAlignTab);
		rcItem2.left = rcClient.left + 1;
		rcItem2.right = rcItem1.left ;
		rcItem2.top = rcItem2.bottom + 5;
		rcItem2.bottom = rcClient.bottom - nStatusBarHeight;
		GetDlgItem(IDC_TAB_KIND_ALIGN)->MoveWindow(rcItem2);

		m_pDlgKindCalibSetting->MoveWindow(rcItem2);

// 		CRect rcTabSheet;
// 		m_tabSheet.GetClientRect(rcTabSheet);
// 		rcTabSheet.top	  += 22;   
// 		rcTabSheet.bottom -= 4;
// 		rcTabSheet.left	  += 2;
// 		rcTabSheet.right  -= 4;

		if(m_pDlgKindAlignView != NULL)
		{
			m_pDlgKindAlignView->MoveWindow(rcItem2);
		}
		if(m_pDlgKindAlignSetting != NULL)
		{
			m_pDlgKindAlignSetting->MoveWindow(rcItem2);
		}
		if(m_pDlgKindAlignTarget != NULL ) 
		{
			m_pDlgKindAlignTarget->MoveWindow(rcItem2);
		}
		if(m_pDlgKindAlignObject != NULL)
		{
			m_pDlgKindAlignObject->MoveWindow(rcItem2);
		}
		if(m_pDlgKindAlignVirtualObject != NULL)
		{
			m_pDlgKindAlignVirtualObject->MoveWindow(rcItem2);
		}
		if(m_pDlgKindAlignCalibTarget)
		{
			m_pDlgKindAlignCalibTarget->MoveWindow(rcItem2);
		}
		if (m_pDlgKindAlignInspect != NULL)
		{
			m_pDlgKindAlignInspect->MoveWindow(rcItem2);
		}

		m_rtView = rcItem2;
	}
	PostMessage(WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKind::UpdateStatusBar()
{

}


void CDlgSysKind::OnTcnSelchangeTabKindAlign(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	int nCurSel = m_tabSheet.GetCurSel();
	if (nCurSel == m_nPrevTabIndex)
	{
		return;
	}

	CString strItem;
	TCITEM tcItem;
	CHAR chChar[256];

	//  Get the current tab item text.
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = chChar;
	tcItem.cchTextMax = 256;

	m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
	strItem = tcItem.pszText;

	if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_VIEW) == strItem)
	{
		m_pDlgKindAlignView->ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format(_T("关闭产品%d[产品浏览]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_SETTING) == strItem)
	{
		m_pDlgKindAlignSetting->ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format(_T("关闭产品%d[产品设置]\n"),m_pDlgKindAlignSetting->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_TARGET) == strItem)
	{
		m_pDlgKindAlignTarget->ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format(_T("关闭产品%d[目标模板设置]\n"),m_pDlgKindAlignTarget->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_OBJECT) == strItem)
	{
		m_pDlgKindAlignObject->ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format(_T("关闭产品%d[对象模板设置]\n"),m_pDlgKindAlignObject->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_VIRTUAL_OBJECT) == strItem)
	{
		m_pDlgKindAlignVirtualObject->ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format(_T("关闭产品%d[虚拟对象模板设置]\n"),m_pDlgKindAlignVirtualObject->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_CALIB_TARGET) == strItem)
	{
		m_pDlgKindAlignCalibTarget->ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format(_T("关闭产品%d[关联模板设置]\n"),m_pDlgKindAlignCalibTarget->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_INSPECT) == strItem)
	{
		m_pDlgKindAlignInspect->ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format(_T("关闭产品%d[检测设置]\n"),m_pDlgKindAlignInspect->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
	}

	//  Get the current tab item text.
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = chChar;
	tcItem.cchTextMax = 256;
	m_tabSheet.GetItem(nCurSel, &tcItem);
	strItem = tcItem.pszText;

	if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_VIEW) == strItem)
	{
		if (nCurSel != m_nPrevTabIndex )
		{
			m_pDlgKindAlignView->UpdateDlgTempData(FALSE);
		}
		CString strLog;
		strLog.Format(_T("打开产品%d[产品浏览]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);

		m_pDlgKindAlignView->ShowWindow(SW_SHOW);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_SETTING) == strItem)
	{
		CheckDlgAlignSettingIsCreated();
		if(m_pDlgKindAlignSetting->m_nPlatformIndex != m_nPlatformIndex)
		{
			m_pDlgKindAlignSetting->m_nPlatformIndex = m_nPlatformIndex;
			m_pDlgKindAlignSetting->OnPlatformChange();
		}
		CString strLog;
		strLog.Format(_T("打开产品%d[产品设置]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);

		m_pDlgKindAlignSetting->ShowWindow(SW_SHOW);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_TARGET) == strItem)
	{
		CheckDlgAlignTargetIsCreated();
		if(m_pDlgKindAlignTarget->m_nPlatformIndex != m_nPlatformIndex)
		{
			m_pDlgKindAlignTarget->m_nPlatformIndex = m_nPlatformIndex;
			m_pDlgKindAlignTarget->OnPlatformChange();
		}
		CString strLog;
		strLog.Format(_T("打开产品%d[目标模板设置]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
		m_pDlgKindAlignTarget->EnableWindowByAuthorizeset(m_bEnableTargetTemplateSet);
		m_pDlgKindAlignTarget->ShowWindow(SW_SHOW);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_OBJECT) == strItem)
	{
		CheckDlgAlignObjectIsCreated();
		if(m_pDlgKindAlignObject->m_nPlatformIndex != m_nPlatformIndex)
		{
			m_pDlgKindAlignObject->m_nPlatformIndex = m_nPlatformIndex;
			m_pDlgKindAlignObject->OnPlatformChange();
		}
		CString strLog;
		strLog.Format(_T("打开产品%d[对象模板设置]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
		m_pDlgKindAlignObject->EnableWindowByAuthorizeset(m_bEnableObjectTemplateSet);
		m_pDlgKindAlignObject->ShowWindow(SW_SHOW);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_VIRTUAL_OBJECT) == strItem)
	{
		CheckDlgAlignVirtualObjectIsCreated();
		if(m_pDlgKindAlignVirtualObject->m_nPlatformIndex != m_nPlatformIndex)
		{
			m_pDlgKindAlignVirtualObject->m_nPlatformIndex = m_nPlatformIndex;
			m_pDlgKindAlignVirtualObject->OnPlatformChange();
		}
		CString strLog;
		strLog.Format(_T("打开产品%d[虚拟对象模板设置]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
		m_pDlgKindAlignVirtualObject->EnableWindowByAuthorizeset(m_bEnableVirtualObjectTemplateSet);
		m_pDlgKindAlignVirtualObject->ShowWindow(SW_SHOW);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_CALIB_TARGET) == strItem)
	{
		CheckDlgAlignCalibTargetIsCreated();
		if(m_pDlgKindAlignCalibTarget->m_nPlatformIndex != m_nPlatformIndex)
		{
			m_pDlgKindAlignCalibTarget->m_nPlatformIndex = m_nPlatformIndex;
			m_pDlgKindAlignCalibTarget->OnPlatformChange();
		}
		CString strLog;
		strLog.Format(_T("打开产品%d[关联模板设置]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
		m_pDlgKindAlignCalibTarget->EnableWindowByAuthorizeset(m_bEnableCalibTargetTemplateSet);
		m_pDlgKindAlignCalibTarget->ShowWindow(SW_SHOW);
	}
	else if (GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_INSPECT) == strItem)
	{
		CheckDlgAlignInspectIsCreated();
		if(m_pDlgKindAlignInspect->m_nPlatformIndex != m_nPlatformIndex)
		{
			m_pDlgKindAlignInspect->m_nPlatformIndex = m_nPlatformIndex;
			m_pDlgKindAlignInspect->OnPlatformChange();
		}
		CString strLog;
		strLog.Format(_T("打开产品%d[检测模板]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1);
		g_opLogFile.SaveLogText(strLog,3);
		m_pDlgKindAlignInspect->EnableWindowByAuthorizeset(m_bEnableInspectTemplateSet);
		m_pDlgKindAlignInspect->ShowWindow(SW_SHOW);

	}

	m_nPrevTabIndex = nCurSel;

	*pResult = 0;
}


void CDlgSysKind::OnBnClickedRadioAlign()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString strLog;
	//strLog.Format(_T("打开工位%d产品\n"),m_nPlatformIndex+1);
	//g_opLogFile.SaveLogText(strLog,2);
	int nCurSel = m_tabSheet.GetCurSel();
	CString strItem;
	TCITEM tcItem;
	CHAR chChar[256];
	//  Get the current tab item text.
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = chChar;
	tcItem.cchTextMax = 256;
	m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
	strItem = tcItem.pszText;
	CString strLog;
	strLog.Format(_T("关闭[标定]产品%d\n"),m_pDlgKindCalibSetting->m_nCurProductIdx+1);
	g_opLogFile.SaveLogText(strLog,3);
	strLog.Format(_T("关闭[工位%d][标定]\n"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("打开[工位%d][产品]\n"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("打开产品%d[%s]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1,strItem);
	g_opLogFile.SaveLogText(strLog,3);

	UpdateData(TRUE);
	m_tabSheet.ShowWindow(SW_SHOW);
	if(m_pDlgKindAlignView != NULL)
	{
		m_pDlgKindAlignView->UpdateCalibDlgData(FALSE);
	}
	if(m_pDlgKindAlignSetting != NULL)
	{
		m_pDlgKindAlignSetting->UpdateCalibDlgData(FALSE);
	}
	if(m_pDlgKindCalibSetting != NULL)
	{
		m_pDlgKindCalibSetting->ShowWindow(SW_HIDE);
	}
}


void CDlgSysKind::OnBnClickedRadioCalib()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	int nCurSel = m_tabSheet.GetCurSel();
	CString strItem;
	TCITEM tcItem;
	CHAR chChar[256];
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = chChar;
	tcItem.cchTextMax = 256;
	m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
	strItem = tcItem.pszText;
	strLog.Format(_T("关闭[%s]"),strItem);
	g_opLogFile.SaveLogText(strLog,3);
	strLog.Format(_T("关闭[工位%d][产品]\n"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("打开[工位%d][标定]\n"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("打开产品%d[标定]\n"),m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex()+1);
	g_opLogFile.SaveLogText(strLog,3);


	UpdateData(TRUE);

	bool bRet = false;
	if(m_pDlgKindAlignObject != NULL)
	{
		bRet = bRet || m_pDlgKindAlignObject->m_bCopyObjectToCalib;
	}
	if(m_pDlgKindAlignVirtualObject != NULL)
	{
		bRet = bRet || m_pDlgKindAlignVirtualObject->m_bCopyObjectToCalib;
	}
	if(m_pDlgKindAlignCalibTarget != NULL)
	{
		bRet = bRet || m_pDlgKindAlignCalibTarget->m_bCopyObjectToCalib;
	}
	if (bRet)
	{
		m_pDlgKindCalibSetting->UpdateDlgPatternData(FALSE);
	}
	m_pDlgKindCalibSetting->SetCurProductIndex(m_pDlgKindAlignView->m_nCurProductIdx);

	m_pDlgKindCalibSetting->ShowWindow(SW_SHOW);
	m_tabSheet.ShowWindow(SW_HIDE);
}



void CDlgSysKind::OnBnClickedRadioKind()
{
	UpdateData(TRUE);
	if (m_nPreRadioKind == m_nRadioKind)
	{
		return;
	}

	CString strLog;
	//CString strArrItem[] = {_T("产品浏览"), _T("标定设置"), _T("产品设置"),_T("目标模板"), _T("对象模板"), _T("虚拟对象模板"), _T("关联模板"), _T("检测模板")};
	CWnd*	pKindView[] = {m_pDlgKindAlignView, m_pDlgKindCalibSetting, m_pDlgKindAlignSetting, m_pDlgKindAlignTarget, m_pDlgKindAlignObject, m_pDlgKindAlignVirtualObject, m_pDlgKindAlignCalibTarget, m_pDlgKindAlignInspect};

	int nCurProductIdx = m_vpTotalProductData[m_nPlatformIndex]->GetCurProductIndex();

	// 关闭前一个界面
	pKindView[m_nPreRadioKind]->ShowWindow(SW_HIDE);
	strLog.Format(_T("关闭产品%d[%s]\n"),nCurProductIdx+1, GetOptionText(m_nPreRadioKind));
	g_opLogFile.SaveLogText(strLog,3);

	// 打开新的界面
	strLog.Format(_T("打开产品%d[%s]\n"),nCurProductIdx+1, GetOptionText(m_nRadioKind));
	g_opLogFile.SaveLogText(strLog,3);


	switch(m_nRadioKind)
	{
	case 0:
		{
			m_pDlgKindAlignView->ShowWindow(SW_SHOW);
			if(m_pDlgKindAlignView->IsProductChanged())
			{
				m_pDlgKindAlignView->UpdateDlgTempData(FALSE);
			}
		}
		break;
	case 1:
		{
			m_pDlgKindCalibSetting->ShowWindow(SW_SHOW);
			m_tabSheet.ShowWindow(SW_HIDE);

			bool bRet = false;
			if(m_pDlgKindAlignObject != NULL)
			{
				bRet = bRet || m_pDlgKindAlignObject->m_bCopyObjectToCalib;
			}
			if(m_pDlgKindAlignVirtualObject != NULL)
			{
				bRet = bRet || m_pDlgKindAlignVirtualObject->m_bCopyObjectToCalib;
			}
			if(m_pDlgKindAlignCalibTarget != NULL)
			{
				bRet = bRet || m_pDlgKindAlignCalibTarget->m_bCopyObjectToCalib;
			}
			if (bRet)
			{
				m_pDlgKindCalibSetting->UpdateDlgPatternData(FALSE);
			}
			//	m_pDlgKindCalibSetting->SetCurProductIndex(m_pDlgKindAlignView->m_nCurProductIdx);

		}
		break;
	case 2:
		{
			CheckDlgAlignSettingIsCreated();
			if(m_pDlgKindAlignSetting->m_nPlatformIndex != m_nPlatformIndex)
			{
				m_pDlgKindAlignSetting->m_nPlatformIndex = m_nPlatformIndex;
				m_pDlgKindAlignSetting->OnPlatformChange();
			}
			m_pDlgKindAlignSetting->ShowWindow(SW_SHOW);
		}
		break;
	case 3:
		{
			CheckDlgAlignTargetIsCreated();
			if(m_pDlgKindAlignTarget->m_nPlatformIndex != m_nPlatformIndex)
			{
				m_pDlgKindAlignTarget->m_nPlatformIndex = m_nPlatformIndex;
				m_pDlgKindAlignTarget->OnPlatformChange();
			}
			m_pDlgKindAlignTarget->EnableWindowByAuthorizeset(m_bEnableTargetTemplateSet);
			m_pDlgKindAlignTarget->ShowWindow(SW_SHOW);
		}
		break;
	case 4:
		{
			CheckDlgAlignObjectIsCreated();
			if(m_pDlgKindAlignObject->m_nPlatformIndex != m_nPlatformIndex)
			{
				m_pDlgKindAlignObject->m_nPlatformIndex = m_nPlatformIndex;
				m_pDlgKindAlignObject->OnPlatformChange();
			}
			m_pDlgKindAlignObject->EnableWindowByAuthorizeset(m_bEnableObjectTemplateSet);
			m_pDlgKindAlignObject->ShowWindow(SW_SHOW);
		}
		break;
	case 5:
		{
			CheckDlgAlignVirtualObjectIsCreated();
			if(m_pDlgKindAlignVirtualObject->m_nPlatformIndex != m_nPlatformIndex)
			{
				m_pDlgKindAlignVirtualObject->m_nPlatformIndex = m_nPlatformIndex;
				m_pDlgKindAlignVirtualObject->OnPlatformChange();
			}
			m_pDlgKindAlignVirtualObject->EnableWindowByAuthorizeset(m_bEnableVirtualObjectTemplateSet);
			m_pDlgKindAlignVirtualObject->ShowWindow(SW_SHOW);
		}
		break;
	case 6:
		{
			CheckDlgAlignCalibTargetIsCreated();
			if(m_pDlgKindAlignCalibTarget->m_nPlatformIndex != m_nPlatformIndex)
			{
				m_pDlgKindAlignCalibTarget->m_nPlatformIndex = m_nPlatformIndex;
				m_pDlgKindAlignCalibTarget->OnPlatformChange();
			}
			m_pDlgKindAlignCalibTarget->EnableWindowByAuthorizeset(m_bEnableCalibTargetTemplateSet);
			m_pDlgKindAlignCalibTarget->ShowWindow(SW_SHOW);
		}
		break;
	case 7:
		{
			CheckDlgAlignInspectIsCreated();
			if(m_pDlgKindAlignInspect->m_nPlatformIndex != m_nPlatformIndex)
			{
				m_pDlgKindAlignInspect->m_nPlatformIndex = m_nPlatformIndex;
				m_pDlgKindAlignInspect->OnPlatformChange();
			}
			m_pDlgKindAlignInspect->EnableWindowByAuthorizeset(m_bEnableInspectTemplateSet);
			m_pDlgKindAlignInspect->ShowWindow(SW_SHOW);
		}
		break;
	}

	m_nPreRadioKind = m_nRadioKind;
}



void CDlgSysKind::OnPlatformChange() 
{
	CString strLog;
	UpdateData(TRUE);
	if (m_nPlatformIndex == m_nPrevPlatformIdx)
	{
		return;
	}
	//CString strArrItem[] = {_T("产品浏览"), _T("标定设置"), _T("产品设置"),_T("目标模板"), _T("对象模板"), _T("虚拟对象模板"), _T("关联模板"), _T("检测模板")};
	strLog.Format(_T("切换[工位%d]%s到[工位%d]%s\n"),m_nPrevPlatformIdx+1, GetOptionText(m_nRadioKind), m_nPlatformIndex+1, GetOptionText(m_nRadioKind));
	g_opLogFile.SaveLogText(strLog,3);

	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && (m_nPlatformIndex<  m_pParent->m_vpVisionASM.size()) && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{

			CString strNodeName = _T("");
			vsConfig->GetCheckedByItem(eProductAuthorize_TargetTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowTargetTemplateSet,m_bEnableTargetTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_ObjectTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowObjectTemplateSet,m_bEnableObjectTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_VirtualObjectTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowVirtualObjectTemplateSet,m_bEnableVirtualObjectTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_CalibTargetTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCalibTargetTemplateSet,m_bEnableCalibTargetTemplateSet);

			vsConfig->GetCheckedByItem(eProductAuthorize_InspectTemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowInspectTemplateSet,m_bEnableInspectTemplateSet);


		}
	}

	// fumin 2018.06.06 
	// 第一次调用OnPlatformChange()不调用创建窗口（产品设置、目标模板、对象模板等），第二次调用时创建，其他再次调用也不创建
	if (!m_bInitCreateDlg)
	{
		CheckAllDlgIsCreated();
		m_bInitCreateDlg = TRUE;
	}

	m_pDlgKindAlignView->m_nPlatformIndex = m_nPlatformIndex;
	m_pDlgKindAlignView->OnPlatformChange();

	if(m_pDlgKindAlignSetting != NULL)
	{
		m_pDlgKindAlignSetting->m_nPlatformIndex = m_nPlatformIndex;
		m_pDlgKindAlignSetting->OnPlatformChange();
	}

	if(m_pDlgKindAlignTarget != NULL)
	{
		m_pDlgKindAlignTarget->m_nPlatformIndex = m_nPlatformIndex;
		m_pDlgKindAlignTarget->EnableWindowByAuthorizeset(m_bEnableTargetTemplateSet);
		m_pDlgKindAlignTarget->OnPlatformChange();	
	}

	if(m_pDlgKindAlignObject != NULL)
	{
		m_pDlgKindAlignObject->m_nPlatformIndex = m_nPlatformIndex;
		m_pDlgKindAlignObject->EnableWindowByAuthorizeset(m_bEnableObjectTemplateSet);
		m_pDlgKindAlignObject->OnPlatformChange();
	}

	if(m_pDlgKindAlignVirtualObject != NULL)
	{
		m_pDlgKindAlignVirtualObject->m_nPlatformIndex = m_nPlatformIndex;
		m_pDlgKindAlignVirtualObject->EnableWindowByAuthorizeset(m_bEnableVirtualObjectTemplateSet);
		m_pDlgKindAlignVirtualObject->OnPlatformChange();	
	}

	if(m_pDlgKindAlignCalibTarget != NULL)
	{
		m_pDlgKindAlignCalibTarget->m_nPlatformIndex = m_nPlatformIndex;
		m_pDlgKindAlignCalibTarget->EnableWindowByAuthorizeset(m_bEnableCalibTargetTemplateSet);
		m_pDlgKindAlignCalibTarget->OnPlatformChange();
	}

	if(m_pDlgKindAlignInspect != NULL)
	{
		m_pDlgKindAlignInspect->m_nPlatformIndex = m_nPlatformIndex;
		m_pDlgKindAlignInspect->EnableWindowByAuthorizeset(m_bEnableInspectTemplateSet);
		m_pDlgKindAlignInspect->OnPlatformChange();
	}

	m_pDlgKindCalibSetting->m_nPlatformIndex = m_nPlatformIndex;
	m_pDlgKindCalibSetting->OnPlatformChange();

	m_nPrevPlatformIdx = m_nPlatformIndex;

	//工位切换,不同工位模板位置数不一，所以重新初始化模板标识符,再将模板工具标识符修改为false
	InitSearchToolChanged();
	SetProductSearchToolChanged(false);
	SetCalibObjectSearchToolChanged(false);

// 	int nCurSel = m_tabSheet.GetCurSel();
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
// 	CString strLog,strAlign,strCalib;
// 	m_btnAlign.GetWindowText(strAlign);
// 	m_btnCalib.GetWindowText(strCalib);
// 	if (m_nRadioKind == 0)
// 	{
// 		strLog.Format(_T("关闭[%s]\n"),strItem);
// 		g_opLogFile.SaveLogText(strLog,3);
// 		strLog.Format(_T("打开[工位%d][%s]\n"),m_nPlatformIndex+1,strAlign);
// 		g_opLogFile.SaveLogText(strLog,2);
// 		strLog.Format(_T("打开产品%d[%s]\n"),m_pDlgKindAlignView->m_nCurProductIdx+1,strItem);
// 		g_opLogFile.SaveLogText(strLog,3);
// 	}
				
}

BOOL CDlgSysKind::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_pDlgKindAlignView != NULL)
	{
		delete m_pDlgKindAlignView;
		m_pDlgKindAlignView = NULL;
	}

	if (m_pDlgKindAlignSetting != NULL)
	{
		delete m_pDlgKindAlignSetting;
		m_pDlgKindAlignSetting = NULL;
	}


	if (m_pDlgKindAlignTarget != NULL)
	{
		delete m_pDlgKindAlignTarget;
		m_pDlgKindAlignTarget = NULL;
	}


	if (m_pDlgKindAlignObject != NULL)
	{
		delete m_pDlgKindAlignObject;
		m_pDlgKindAlignObject = NULL;
	}

	if (m_pDlgKindAlignVirtualObject != NULL)
	{
		delete m_pDlgKindAlignVirtualObject;
		m_pDlgKindAlignVirtualObject = NULL;
	}

	if (m_pDlgKindAlignCalibTarget != NULL)
	{
		delete m_pDlgKindAlignCalibTarget;
		m_pDlgKindAlignCalibTarget = NULL;
	}

	if (m_pDlgKindCalibSetting != NULL)
	{
		delete m_pDlgKindCalibSetting;
		m_pDlgKindCalibSetting = NULL;
	}

	if (m_pDlgKindAlignInspect != NULL)
	{
		delete m_pDlgKindAlignInspect;
		m_pDlgKindAlignInspect = NULL;
	}

	if (m_pStatusBar != NULL)
	{
		delete m_pStatusBar;
		m_pStatusBar = NULL;
	}

	return CDialog::DestroyWindow();
}


void CDlgSysKind::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}


void CDlgSysKind::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pDlgKindAlignView->IsProductChanged() || m_pDlgKindCalibSetting->IsProductChanged())
	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			CString strLog;
		
			strLog.Format(_T("关闭[%s]"),GetOptionText(m_nRadioKind));
			g_opLogFile.SaveLogText(strLog,3);				
			
			strLog.Format(_T("[不保存]选择[保存]退出\n"));
			g_opLogFile.SaveLogText(strLog,2);
			strLog.Format(_T("退出[产品设置]\n"));
			g_opLogFile.SaveLogText(strLog,1);

			if (!m_pDlgKindAlignView->OnMyOK())
			{
				return;
			}

// 			if (!m_pDlgKindCalibSetting->OnMyOK())
// 			{
// 				return;
// 			}
		}
		else if (nMBReturn == IDCANCEL)
		{
			CString strLog;
			if (!m_pDlgKindCalibSetting->IsWindowVisible())
			{
				int nCurSel = m_tabSheet.GetCurSel();
				CString strItem;
				TCITEM tcItem;
				CHAR chChar[256];
				tcItem.mask = TCIF_TEXT;
				tcItem.pszText = chChar;
				tcItem.cchTextMax = 256;
				m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
				strItem = tcItem.pszText;
				strLog.Format(_T("关闭[%s]"),strItem);
				g_opLogFile.SaveLogText(strLog,3);				
			}
			else{
				strLog.Format(_T("关闭[标定]产品%d\n"),m_pDlgKindCalibSetting->m_nCurProductIdx+1);
				g_opLogFile.SaveLogText(strLog,3);
			}
			strLog.Format(_T("[不保存]选择[取消]退出\n"));
			g_opLogFile.SaveLogText(strLog,2);
			strLog.Format(_T("退出[产品设置]\n"));
			g_opLogFile.SaveLogText(strLog,1);

			return;
		}
		else
		{
			CString strLog;
			if (!m_pDlgKindCalibSetting->IsWindowVisible())
			{
				int nCurSel = m_tabSheet.GetCurSel();
				CString strItem;
				TCITEM tcItem;
				CHAR chChar[256];
				tcItem.mask = TCIF_TEXT;
				tcItem.pszText = chChar;
				tcItem.cchTextMax = 256;
				m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
				strItem = tcItem.pszText;
				strLog.Format(_T("关闭[%s]"),strItem);
				g_opLogFile.SaveLogText(strLog,3);				
			}
			else{
				strLog.Format(_T("关闭[标定]产品%d\n"),m_pDlgKindCalibSetting->m_nCurProductIdx+1);
				g_opLogFile.SaveLogText(strLog,3);
			}
			strLog.Format(_T("[不保存]选择[不保存]退出\n"));
			g_opLogFile.SaveLogText(strLog,2);
			strLog.Format(_T("退出[产品设置]\n"));
			g_opLogFile.SaveLogText(strLog,1);
			if (!LoadSearchToolInfo())
			{
				return;
			}
			if (!m_pDlgKindAlignView->CancelProductChange())
			{
				return;
			}

			if (!m_pDlgKindCalibSetting->CancelProductChange())
			{
				return;
			}
			
		}
	}
	else
	{
	    CString strLog;
// 		if (!m_pDlgKindCalibSetting->IsWindowVisible())
// 		{
// 			int nCurSel = m_tabSheet.GetCurSel();
// 			CString strItem;
// 			TCITEM tcItem;
// 			CHAR chChar[256];
// 			tcItem.mask = TCIF_TEXT;
// 			tcItem.pszText = chChar;
// 			tcItem.cchTextMax = 256;
// 			m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
// 			strItem = tcItem.pszText;
// 			strLog.Format(_T("关闭[%s]"),strItem);
// 			g_opLogFile.SaveLogText(strLog,3);				
// 		}
// 		else{
// 			strLog.Format(_T("关闭[标定]产品%d\n"),m_pDlgKindCalibSetting->m_nCurProductIdx+1);
// 			g_opLogFile.SaveLogText(strLog,3);
// 		}

		strLog.Format(_T("关闭产品%d[%s]\n"),m_pDlgKindCalibSetting->m_nCurProductIdx+1, GetOptionText(m_nRadioKind));
		g_opLogFile.SaveLogText(strLog,3);

		strLog.Format(_T("[不保存]退出\n"));
		g_opLogFile.SaveLogText(strLog,2);
		strLog.Format(_T("退出[产品设置]\n"));
		g_opLogFile.SaveLogText(strLog,1);
		
		if (!m_pDlgKindAlignView->OnMyCancel())
		{
			return;
		}

// 		if (!m_pDlgKindCalibSetting->OnMyCancel())
// 		{
// 			return;
// 		}
	}

// 	if (!m_pDlgKindAlignView->OnMyCancel())
// 	{
// 		return;
// 	}
// 
// 	if (!m_pDlgKindCalibSetting->OnMyCancel())
// 	{
// 		return;
// 	}

	CDialog::OnCancel();
}


void CDlgSysKind::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_pDlgKindAlignView != NULL)
	{
		if (!m_pDlgKindAlignView->OnMyOK())
		{
			return;
		}
	}

	CString strLog;
// 	if (!m_pDlgKindCalibSetting->IsWindowVisible())
// 	{
// 		int nCurSel = m_tabSheet.GetCurSel();
// 		CString strItem;
// 		TCITEM tcItem;
// 		CHAR chChar[256];
// 		tcItem.mask = TCIF_TEXT;
// 		tcItem.pszText = chChar;
// 		tcItem.cchTextMax = 256;
// 		m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
// 		strItem = tcItem.pszText;
// 		strLog.Format(_T("关闭[%s]"),strItem);
// 		g_opLogFile.SaveLogText(strLog,3);				
// 	}
// 
// // 	if(m_pDlgKindCalibSetting != NULL)
// // 	{
// // 		if (!m_pDlgKindCalibSetting->OnMyOK())
// // 		{
// // 			return;
// // 		}
// // 	}
// 
// 	if (m_pDlgKindCalibSetting->IsWindowVisible())
// 	{
// 		CString strLog;
// 		strLog.Format(_T("关闭[标定]产品%d\n"),m_pDlgKindCalibSetting->m_nCurProductIdx+1);
// 		g_opLogFile.SaveLogText(strLog,3);
// 	}

	//CString strArrItem[] = {_T("产品浏览"), _T("标定设置"), _T("产品设置"),_T("目标模板"), _T("对象模板"), _T("虚拟对象模板"), _T("关联模板"), _T("检测模板")};
	strLog.Format(_T("关闭[%s]\n"),GetOptionText(m_nRadioKind));
	g_opLogFile.SaveLogText(strLog,3);

	strLog.Format(_T("[保存]退出\n"));
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("退出[产品设置]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	CDialog::OnOK();
}


BOOL CDlgSysKind::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


// void CDlgSysKind::OnBnClickedBtnPanel()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// }
// 
// 
// void CDlgSysKind::OnBnClickedOk()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CDialog::OnOK();
// }
// 
// 
// void CDlgSysKind::OnBnClickedBtnApply()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// }
void CDlgSysKind::UpdateDialogLanguage()
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

		if(_T("IDD_SYS_KIND") == strDlgID)
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

void CDlgSysKind::CheckDlgAlignSettingIsCreated()
{

	if(m_pDlgKindAlignSetting == NULL)
	{
// 		CRect rcTabSheet;
// 		m_tabSheet.GetClientRect(rcTabSheet);
// 		rcTabSheet.top	  += 22;   
// 		rcTabSheet.bottom -= 4;
// 		rcTabSheet.left	  += 2;
// 		rcTabSheet.right  -= 4;

		m_pDlgKindAlignSetting = new CDlgSysKindAlignSetting;
		m_pDlgKindAlignSetting->m_pParent = m_pParent;
		m_pDlgKindAlignSetting->m_pParentKind = this;
		m_pDlgKindAlignSetting->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
		m_pDlgKindAlignSetting->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
		m_pDlgKindAlignSetting->Create(IDD_SYS_KIND_ALIGN_SETTING, this);
		m_pDlgKindAlignSetting->MoveWindow(m_rtView);


	}
}
void CDlgSysKind::CheckDlgAlignTargetIsCreated()
{

	if(m_pDlgKindAlignTarget == NULL)
	{
// 		CRect rcTabSheet;
// 		m_tabSheet.GetClientRect(rcTabSheet);
// 		rcTabSheet.top	  += 22;   
// 		rcTabSheet.bottom -= 4;
// 		rcTabSheet.left	  += 2;
// 		rcTabSheet.right  -= 4;

		m_pDlgKindAlignTarget = new CDlgSysKindAlignTarget;
		m_pDlgKindAlignTarget->m_pParent = m_pParent;
		m_pDlgKindAlignTarget->m_pParentKind = this;
		m_pDlgKindAlignTarget->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
		m_pDlgKindAlignTarget->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
		m_pDlgKindAlignTarget->Create(IDD_SYS_KIND_ALIGN_TARGET, this);
		m_pDlgKindAlignTarget->MoveWindow(m_rtView);


	}
}
void CDlgSysKind::CheckDlgAlignObjectIsCreated()
{

	if(m_pDlgKindAlignObject == NULL)
	{
// 		CRect rcTabSheet;
// 		m_tabSheet.GetClientRect(rcTabSheet);
// 		rcTabSheet.top	  += 22;   
// 		rcTabSheet.bottom -= 4;
// 		rcTabSheet.left	  += 2;
// 		rcTabSheet.right  -= 4;

		m_pDlgKindAlignObject = new CDlgSysKindAlignObject;
		m_pDlgKindAlignObject->m_pParent = m_pParent;
		m_pDlgKindAlignObject->m_pParentKind = this;
		m_pDlgKindAlignObject->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
		m_pDlgKindAlignObject->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
		m_pDlgKindAlignObject->Create(IDD_SYS_KIND_ALIGN_OBJECT, this);
		m_pDlgKindAlignObject->MoveWindow(m_rtView);



	}
}
void CDlgSysKind::CheckDlgAlignVirtualObjectIsCreated()
{

	if(m_pDlgKindAlignVirtualObject == NULL)
	{
// 		CRect rcTabSheet;
// 		m_tabSheet.GetClientRect(rcTabSheet);
// 		rcTabSheet.top	  += 22;   
// 		rcTabSheet.bottom -= 4;
// 		rcTabSheet.left	  += 2;
// 		rcTabSheet.right  -= 4;

		m_pDlgKindAlignVirtualObject = new CDlgSysKindAlignVirtualObject;
		m_pDlgKindAlignVirtualObject->m_pParent = m_pParent;
		m_pDlgKindAlignVirtualObject->m_pParentKind = this;
		m_pDlgKindAlignVirtualObject->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
		m_pDlgKindAlignVirtualObject->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
		m_pDlgKindAlignVirtualObject->Create(IDD_SYS_KIND_ALIGN_VIRTUAL_OBJECT, this);
		m_pDlgKindAlignVirtualObject->MoveWindow(m_rtView);


	}
}
void CDlgSysKind::CheckDlgAlignCalibTargetIsCreated()
{

	if(m_pDlgKindAlignCalibTarget == NULL)
	{
// 		CRect rcTabSheet;
// 		m_tabSheet.GetClientRect(rcTabSheet);
// 		rcTabSheet.top	  += 22;   
// 		rcTabSheet.bottom -= 4;
// 		rcTabSheet.left	  += 2;
// 		rcTabSheet.right  -= 4;

		m_pDlgKindAlignCalibTarget = new CDlgSysKindAlignCalibTarget;
		m_pDlgKindAlignCalibTarget->m_pParent = m_pParent;
		m_pDlgKindAlignCalibTarget->m_pParentKind = this;
		m_pDlgKindAlignCalibTarget->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
		m_pDlgKindAlignCalibTarget->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
		m_pDlgKindAlignCalibTarget->Create(IDD_SYS_KIND_ALIGN_CALIB_TARGET, this);
		m_pDlgKindAlignCalibTarget->MoveWindow(m_rtView);


	}
}

void CDlgSysKind::CheckDlgAlignInspectIsCreated()
{
	if(m_pDlgKindAlignInspect == NULL)
	{
// 		CRect rcTabSheet;
// 		m_tabSheet.GetClientRect(rcTabSheet);
// 		rcTabSheet.top	  += 22;   
// 		rcTabSheet.bottom -= 4;
// 		rcTabSheet.left	  += 2;
// 		rcTabSheet.right  -= 4;

		m_pDlgKindAlignInspect = new CDlgSysKindAlignInspect;
		m_pDlgKindAlignInspect->m_pParent = m_pParent;
		m_pDlgKindAlignInspect->m_pParentKind = this;
		m_pDlgKindAlignInspect->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
		m_pDlgKindAlignInspect->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
		m_pDlgKindAlignInspect->Create(IDD_SYS_KIND_ALIGN_INSPECT, this);
		m_pDlgKindAlignInspect->MoveWindow(m_rtView);
	}

}

void CDlgSysKind::CheckAllDlgIsCreated()
{
	CheckDlgAlignSettingIsCreated();
	CheckDlgAlignTargetIsCreated();
	CheckDlgAlignObjectIsCreated();
	CheckDlgAlignVirtualObjectIsCreated();
	CheckDlgAlignCalibTargetIsCreated();
	CheckDlgAlignInspectIsCreated();
}
// 初始化所有定位工具修改标识符//切换平台时也调用
bool CDlgSysKind::InitSearchToolChanged()
{
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
	int nCurProductIdx = pTotalProduct->GetCurProductIndex();
	if ((nCurProductIdx < 0) || (nCurProductIdx >= m_vpTotalProductData.at(m_nPlatformIndex)->GetCount()))
	{
		return FALSE;
	}
	CProductData *pProduct = pTotalProduct->GetItemPtr(nCurProductIdx);
	if (pProduct == NULL)
	{
		return FALSE;
	}
	CCalibData pCalibData = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData()->GetItem(nCurProductIdx);
	
	SysPlatformInfo platformInfo;
	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	pVisionASM->GetSysPlatformInfo(platformInfo);
	//初始化目标改变标识符
	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{
		m_vvbAlnTargetSearchToolChanged.resize(pProduct->m_pAlignerParam->m_vTargetPosPatInfo.size());
		for (int k=0; k<pProduct->m_vvpAlnTargetSearchToolRepo.size(); k++)
		{
			m_vvbAlnTargetSearchToolChanged[k].resize(pProduct->m_pAlignerParam->m_vTargetPosPatInfo[k].m_nPatNum, false);
		}
	}

	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		//初始化对象改变标识符
		m_vvbAlnObjectSearchToolChanged.resize(pProduct->m_pAlignerParam->m_vObjectPosPatInfo.size());
		for (int k=0; k<pProduct->m_vvpAlnObjectSearchToolRepo.size(); k++)
		{
			m_vvbAlnObjectSearchToolChanged[k].resize(pProduct->m_pAlignerParam->m_vObjectPosPatInfo[k].m_nPatNum, false);
		}
	}

	//初始化虚拟对象改变标识符
	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	{
		//m_vvbAlnVirtualObjectSearchToolChanged.resize(pProduct->m_pAlignerParam->GetVirtualObjectMarkPatternNum());
		m_vvbAlnVirtualObjectSearchToolChanged.resize(pProduct->m_pAlignerParam->GetObjectMarkPatternNum());
		for (int k=0; k<pProduct->m_vvpAlnVirtualObjectSearchToolRepo.size(); k++)
		{
			m_vvbAlnVirtualObjectSearchToolChanged[k].resize(VIRTUALOBJECTCANDIDATENUM, false);
		}
	}

	//初始化关联改变标识符
	if ((platformInfo.m_bTargetObjectCamSeparate == TRUE) 
		|| (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
	{
		m_vvbAlnCalibTargetSearchToolChanged.resize(pProduct->m_pAlignerParam->GetCalibTargetMarkPatternNum());
		for (int k=0; k<pProduct->m_vvpAlnCalibTargetSearchToolRepo.size(); k++)
		{
			m_vvbAlnCalibTargetSearchToolChanged[k].resize(ALNCALIBTARGETCANDIDATENUM, false);
		}	
	}

	//初始化检测改变标识符
	//if (TRUE == platformInfo.m_bInspectEnable)
	{
		m_vvbAlnInspectSearchToolChanged.resize(pProduct->m_pAlignerParam->GetInspectMarkPatternNum());
		for (int k=0; k<pProduct->m_vvpAlnInspectSearchToolRepo.size(); k++)
		{
			m_vvbAlnInspectSearchToolChanged[k].resize(INSPECTCANDIDATENUM, false);
		}
	}

	m_vvbAlnCalibObjectSearchToolChanged.resize(platformInfo.m_nPositionNum);
	for (int k=0; k<pCalibData.m_vvpCalibObjectSearchToolRepo.size(); k++)
	{
		m_vvbAlnCalibObjectSearchToolChanged[k].resize(1, false);
	}
	
	return TRUE;
}

bool CDlgSysKind::SetProductSearchToolChanged(bool bSetState)
{
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
	int nCurProductIdx = pTotalProduct->GetCurProductIndex();
	if ((nCurProductIdx < 0) || (nCurProductIdx >= m_vpTotalProductData.at(m_nPlatformIndex)->GetCount()))
	{
		return FALSE;
	}
	CProductData *pProduct = pTotalProduct->GetItemPtr(nCurProductIdx);
	if (pProduct == NULL)
	{
		return FALSE;
	}

	SysPlatformInfo platformInfo;
	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	pVisionASM->GetSysPlatformInfo(platformInfo);

	CPlatformOptionInfo optionInfo;
	pVisionASM->GetPlatformOptionInfo(optionInfo);
	//更改目标改变标识符
	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{

		for (int i=0; i<m_vvbAlnTargetSearchToolChanged.size(); i++)
		{
			for(int j=0; j<m_vvbAlnTargetSearchToolChanged[i].size(); j++)
			{
				m_vvbAlnTargetSearchToolChanged[i][j] = bSetState;
			}
		}
	}

	//更改对象改变标识符
	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		for (int i=0; i<m_vvbAlnObjectSearchToolChanged.size(); i++)
		{
			for(int j=0; j<m_vvbAlnObjectSearchToolChanged[i].size(); j++)
			{
				m_vvbAlnObjectSearchToolChanged[i][j] = bSetState;
			}
		}
	}

	//更改虚拟对象改变标识符
	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	{
		for (int i=0; i<m_vvbAlnVirtualObjectSearchToolChanged.size(); i++)
		{
			for(int j=0; j<m_vvbAlnVirtualObjectSearchToolChanged[i].size(); j++)
			{
				m_vvbAlnVirtualObjectSearchToolChanged[i][j] = bSetState;
			}
		}
	}

	//更改关联改变标识符
	if (platformInfo.m_bTargetObjectCamSeparate == TRUE)
	{
		for (int i=0; i<m_vvbAlnCalibTargetSearchToolChanged.size(); i++)
		{
			for(int j=0; j<m_vvbAlnCalibTargetSearchToolChanged[i].size(); j++)
			{
				m_vvbAlnCalibTargetSearchToolChanged[i][j] = bSetState;
			}
		}
	}

	//更改检测改变标识符
	//if (TRUE == optionInfo.m_bInspectEnable)
	{
		for (int i=0; i<m_vvbAlnInspectSearchToolChanged.size(); i++)
		{
			for(int j=0; j<m_vvbAlnInspectSearchToolChanged[i].size(); j++)
			{
				m_vvbAlnInspectSearchToolChanged[i][j] = bSetState;
			}
		}
	}
	return true;
}

bool CDlgSysKind::SetCalibObjectSearchToolChanged(bool bSetState)
{
	int nCalibIndex = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData()->GetCurCalibIndex();

	if (nCalibIndex<0 || nCalibIndex>=m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData()->GetCount())
	{
		return FALSE;
	}
	
	for (int i=0; i<m_vvbAlnCalibObjectSearchToolChanged.size(); i++)
	{
		for(int j=0; j<m_vvbAlnCalibObjectSearchToolChanged[i].size(); j++)
		{
			m_vvbAlnCalibObjectSearchToolChanged[i][j] = bSetState;
		}
	}
	return true;
}

BOOL CDlgSysKind::LoadSearchTool(SearchMode searchMode,CString strDir, std::vector<VpSearchTool> &vvpSearchTool,std::vector<VbSearchToolChanged> vvbSearchToolChanged)
{
	if(vvpSearchTool.size() != vvbSearchToolChanged.size())
	{
		return false;//工具数量应该与工具修改标志数量一致
	}
	else
	{
		for (int i = 0;i < vvbSearchToolChanged.size();i++)
		{
			if(vvbSearchToolChanged[i].size() != vvpSearchTool[i].size())
				return false;//工具数量应该与工具修改标志数量一致
		}
	}

	bool bSucceed =true;
	//工具路径
	CString strPatternDir;
	CString strTemp;
	//当前工具的位置数
	int nPatternNum;
	//int i,j;

	switch(searchMode)
	{
	case eTargetSearch:
		{
			strPatternDir = strDir+ _T("\\TargetModels");
		}
		break;
	case eVirtualSearch:
		{
			strPatternDir = strDir+ _T("\\VirtualObjectModels");
		}
		break;
	case eObjectSearch:
		{
			strPatternDir = strDir+ _T("\\ObjectModels");
		}
		break;
	case eCalibTargetSearch:
		{
			strPatternDir = strDir+ _T("\\CalibTargetModels");
		}
		break;
	case  eInspectSearch:
		{
			strPatternDir = strDir+ _T("\\InspectModels");
		}
		break;
	case  eCalibSearch:
		{
			strPatternDir = strDir+ _T("\\CalibModels");
		}
		break;
	default:
		return false;
	}
	// 赋值临时变量
	nPatternNum = vvpSearchTool.size();
	//CBaseSearchTool* pBaseSearchTool;		//声明工具
	//for (i=0;i<nPatternNum;i++)
	//{
	//	for (j=0;j<vvbSearchToolChanged.at(i).size();j++)
	//	{
	//		if (vvbSearchToolChanged.at(i).at(j))
	//		{
	//			if (vvpSearchTool.at(i).at(j)!=NULL)
	//			{
	//				delete vvpSearchTool.at(i).at(j);
	//				vvpSearchTool.at(i).at(j)=NULL;
	//			}
	//			
	//			strTemp.Format(_T("\\Pos%d_M%d"),i,j);
	//			strPatternDir = strPatternDir + strTemp;
	//			// 从XML文件中获取定位工具类型
	//			CSearchToolType cSearchToolType = CBaseSearchTool::GetSearchToolTypeFromFile(strPatternDir);
	//			if (cSearchToolType == eSearchToolUnknown)
	//			{
	//				continue;
	//			}
	//			// 根据定位工具类型创建定位工具
	//			pBaseSearchTool = NULL;
	//			if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->CreateSearchTool(cSearchToolType,&pBaseSearchTool))
	//			{
	//				bSucceed = false;
	//				continue;
	//			}
	//			if (NULL == pBaseSearchTool || FALSE == pBaseSearchTool->LoadModelFromFile(strPatternDir))
	//			{
	//				if (pBaseSearchTool!=NULL)
	//				{
	//					delete pBaseSearchTool;
	//					pBaseSearchTool = NULL;
	//				}
	//				bSucceed = false;
	//				continue;
	//			}
	//			vvpSearchTool.at(i).at(j) = pBaseSearchTool;
	//			
	//		}
	//	}
	//}

	//线程加载目标模板
	HANDLE hSearchToolThread[MAXPOSNUM];
	CInitSearchToolThreadParam InitSearchToolThreadParam[MAXPOSNUM];
	for (int j=0; j<nPatternNum; j++)
	{
		InitSearchToolThreadParam[j]=CInitSearchToolThreadParam(m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx),strPatternDir,vvpSearchTool.at(j).size(),j,vvbSearchToolChanged.at(j));
		hSearchToolThread[j] = CreateThread(NULL,0,vcBaseVisionAlign::InitSearchToolRepositoryThread,&InitSearchToolThreadParam[j],0,NULL);
	}

	//等待线程加载所有模板后，程序继续运行，默认等待1.5分钟
	int nReturn = WaitForMultipleObjects(nPatternNum,hSearchToolThread,TRUE,INITSEARCHTOOLTHREAD_WAITINGTIME);
	for(int i = 0; i < nPatternNum; i++ )
	{	
		CloseHandle(hSearchToolThread[i]);    // 关闭所有线程句柄并释放内存
	}
	if(nReturn == WAIT_TIMEOUT)
	{
		//线程加载超时
		return false;
	}

	//开始将线程保存的模板，从线程参数TargetSearchToolParam中读出
	for (int j=0; j<nPatternNum; j++)
	{
		for (int i=0; i<vvpSearchTool[j].size(); i++)
		{
			if (vvbSearchToolChanged[j][i])
			{
				if (vvpSearchTool[j][i]!=NULL)
				{
					delete vvpSearchTool[j][i];
					vvpSearchTool[j][i] = NULL;
				}
				if(InitSearchToolThreadParam[j].GetSearchToolInitResult(i))
				{

					vvpSearchTool[j][i] =InitSearchToolThreadParam[j].m_vpAlnSearchToolRepo[i];
				}
			}

		}
	}
	return bSucceed;
}
BOOL CDlgSysKind::LoadSearchToolInfo()
{
	BOOL bSucceed = TRUE;
	bSucceed = bSucceed && m_pDlgKindAlignView->LoadSearchToolInfo();
	bSucceed = bSucceed && m_pDlgKindCalibSetting->LoadSearchToolInfo();

	if (!bSucceed)
	{
		MessageBox(_T("加载定位工具失败"));
	}
	return bSucceed;
}


CString CDlgSysKind::GetOptionText(int nOptIndex)
{
	CString strArrItem[] = {_T("产品浏览"), _T("标定设置"), _T("产品设置"),_T("目标模板"), _T("对象模板"), _T("虚拟对象模板"), _T("关联模板"), _T("检测模板")};
	if (nOptIndex < 0 || nOptIndex >= 8)
	{
		return _T("产品未知选项");
	}

	return strArrItem[nOptIndex];
}





// 保存产品
// bool CDlgSysKind::SaveProduct(int nPlatformIdx, int nProductIdx)
// {
// 	if (!SaveAlignProduct(nPlatformIdx, nProductIdx))
// 	{
// 		return false;
// 	}
// 
// 	if (!SaveCalibProduct(nPlatformIdx, nProductIdx))
// 	{
// 		return false;
// 	}
// 
// 	if (!SaveAlignProductTotalData(nPlatformIdx, nProductIdx))
// 	{
// 		return false;
// 	}
// 
// 	if (!SaveCalibProductTotalData(nPlatformIdx, nProductIdx))
// 	{
// 		return false;
//  	}
// 
// 	return true;
// }
// 
// 保存产品数据
// bool CDlgSysKind::SaveAlignProduct(int nPlatformIdx, int nProductIdx)
// {
// 
// 	CString strLog;
// 	if (!SaveAlignProductData(nPlatformIdx, nProductIdx))
// 	{
// 		strLog.Format(_T("工位[%d]产品[%d]保存数据失败"), nPlatformIdx+1, nProductIdx+1);
// 		return false;
// 	}
// 
// 	if (!SaveAlignProductSearchTool(nPlatformIdx, nProductIdx))
// 	{
// 		strLog.Format(_T("工位[%d]产品[%d]保存定位失败"), nPlatformIdx+1, nProductIdx+1);
// 		return false;
// 	}
// 
// 	return true;
// }
// 
// 
// 保存产品参数数据
// bool CDlgSysKind::SaveAlignProductData(int nPlatformIdx, int nProductIdx)
// {
// 	return m_pParent->m_vpVisionASM[nPlatformIdx]->SaveProductDataInfo(nProductIdx);
// }
// 
// 保存产品模板数据
// bool CDlgSysKind::SaveAlignProductSearchTool(int nPlatformIdx, int nProductIdx)
// {
// 	bool bSucceed = true;
// 	CString strDir, strTemp;
// 	SysPlatformInfo platformInfo;
// 	CPlatformOptionInfo optionInfo;
// 	CTotalProductData* pTotalProduct = m_vpTotalProductData[nPlatformIdx];
// 	bool bProdNewCreate = m_pDlgKindAlignView->m_bProdNewCreate;
// 	bool bProdPatternChanged = m_pDlgKindAlignView->m_bProdPatternChanged;
// 	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 
// 
// 	pVisionASM->GetSysPlatformInfo(platformInfo);
// 	pVisionASM->GetPlatformOptionInfo(optionInfo);
// 	
// 	strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 	strTemp.Format(_T("\\Jobs\\%03d"), m_nPrevProductIdx+1);
// 	strDir += strTemp;        
// 
// 	int nPatternNum = platformInfo.m_nPositionNum;
// 	CBaseSearchTool *pSearchTool = NULL;
// 
// 	int j = 0, i = 0;
// 	CProductData *pProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 
// 	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
// 	{
// 		// 保存目标Mark模板
// 		if (bProdNewCreate || bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		{
// 			bSucceed =bSucceed && SaveSearchTool(eTargetSearch,strDir,pProduct->m_vvpAlnTargetSearchToolRepo, m_vvbAlnTargetSearchToolChanged);
// 		}
// 	}
// 	else
// 	{
// 		RemoveDirectoryPro(strDir + _T("\\TargetModels"));
// 	}
// 
// 
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
// 		// 保存虚拟对象模板
// 		if (bProdNewCreate || bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		{
// 			bSucceed = bSucceed && SaveSearchTool(eVirtualSearch,strDir,pProduct->m_vvpAlnVirtualObjectSearchToolRepo,m_vvbAlnVirtualObjectSearchToolChanged);
// 		}
// 	}
// 	else
// 	{
// 		RemoveDirectoryPro(strDir + _T("\\VirtualObjectModels"));
// 	}
// 
// 
// 
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		// 保存对象模板
// 		if (bProdNewCreate || bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		{
// 			bSucceed = bSucceed && SaveSearchTool(eObjectSearch,strDir,pProduct->m_vvpAlnObjectSearchToolRepo,m_vvbAlnObjectSearchToolChanged);
// 		}
// 	}
// 	else
// 	{
// 		RemoveDirectoryPro(strDir + _T("\\ObjectModels"));
// 	}
// 
// 	if (platformInfo.m_bTargetObjectCamSeparate == TRUE
// 		|| (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
// 	{
// 		// 保存关联模板
// 		if (bProdNewCreate || bProdPatternChanged/*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		{
// 			bSucceed = bSucceed && SaveSearchTool(eCalibTargetSearch,strDir,pProduct->m_vvpAlnCalibTargetSearchToolRepo,m_vvbAlnCalibTargetSearchToolChanged);
// 		}
// 	}
// 	else
// 	{
// 		RemoveDirectoryPro(strDir + _T("\\CalibTargetModels"));
// 	}
// 
// 	if (TRUE == optionInfo.m_bInspectEnable)
// 	{
// 		// 保存检测模板
// 		if (bProdNewCreate || bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		{
// 			bSucceed = bSucceed && SaveSearchTool(eInspectSearch,strDir,pProduct->m_vvpAlnInspectSearchToolRepo,m_vvbAlnInspectSearchToolChanged);
// 		}
// 	}
// 	else
// 	{
// 		RemoveDirectoryPro(strDir + _T("\\InspectModels"));
// 	}
// 	return true;
// }
// 
// 
// 保存所有产品概要文件
// bool CDlgSysKind::SaveAlignProductTotalData(int nPlatfromIdx, int nCurProductIdx)
// {
// 	m_vpTotalProductData[nPlatfromIdx]->SetCurProductIndex(nCurProductIdx);
// 	return m_pParent->m_vpVisionASM[nPlatfromIdx]->SaveTotalProductDataInfo(FALSE);
// }
// 
// 保存产品数据
// bool CDlgSysKind::SaveCalibProduct(int nPlatformIdx, int nProductIdx)
// {
// 	CString strLog;
// 	if (!SaveAlignProductData(nPlatformIdx, nProductIdx))
// 	{
// 		strLog.Format(_T("工位[%d]标定[%d]保存数据失败"), nPlatformIdx+1, nProductIdx+1);
// 		return false;
// 	}
// 
// 	if (!SaveAlignProductSearchTool(nPlatformIdx, nProductIdx))
// 	{
// 		strLog.Format(_T("工位[%d]标定[%d]保存定位失败"), nPlatformIdx+1, nProductIdx+1);
// 		return false;
// 	}
// 
// 	return true;
// }
// 
// 
// 保存产品参数数据
// bool CDlgSysKind::SaveCalibProductData(int nPlatformIdx, int nProductIdx)
// {
// 	return m_pParent->m_vpVisionASM[nPlatformIdx]->SaveCalibDataInfo(nProductIdx);
// }
// 
// 保存产品模板数据
// bool CDlgSysKind::SaveCalibProductSearchTool(int nPlatformIdx, int nProductIdx)
// {
// 	// 保存产品信息到磁盘
// 	bool bProdNewCreate = m_pDlgKindAlignView->m_bProdNewCreate;
// 	bool bProdPatternChanged = m_pDlgKindCalibSetting->m_bProdPatternChanged;
// 
// 	CString strDir, strTemp;
// 	SysPlatformInfo platformInfo;
// 	CTotalCalibData* pTotalCalib = m_vpTotalCalibData[nPlatformIdx];
// 	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(nPlatformIdx);
// 	pVisionASM->GetSysPlatformInfo(platformInfo);
// 
// 	strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 	strTemp.Format(_T("\\JobCalibs\\%03d"), nPlatformIdx+1);
// 	strDir += strTemp;
// 
// 	CCalibData *pCalibData = pTotalCalib->GetItemPtr(nProductIdx);
// 
// 	
// 	if (bProdNewCreate || bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 	{
// 		if(!SaveSearchTool(eCalibSearch,strDir,pCalibData->m_vvpCalibObjectSearchToolRepo, m_vvbAlnCalibObjectSearchToolChanged))
// 		{
// 			return false;
// 		}
// 	}			
// 	return true;
// }
// 
// 
// 保存所有产品概要文件
// bool CDlgSysKind::SaveCalibProductTotalData(int nPlatfromIdx, int nCurProductIdx)
// {
// 	m_vpTotalCalibData[nPlatfromIdx]->SetCurCalibIndex(nCurProductIdx);
// 	return m_pParent->m_vpVisionASM[nPlatfromIdx]->SaveTotalCalibDataInfo(FALSE);
// }
// 
// 
// bool CDlgSysKind::SaveSearchTool(SearchMode searchMode,CString strDir, std::vector<VpSearchTool> vvpSearchTool, std::vector<VbSearchToolChanged> vvbSearchToolChanged)
// {
// 	if(vvpSearchTool.size() != vvbSearchToolChanged.size())
// 	{
// 		return false;//工具数量应该与工具修改标志数量一致
// 	}
// 	else
// 	{
// 		for (int i = 0;i < vvbSearchToolChanged.size();i++)
// 		{
// 			if(vvbSearchToolChanged[i].size() != vvpSearchTool[i].size())
// 			{
// 				return false;//工具数量应该与工具修改标志数量一致	
// 			}
// 		}
// 	}
// 	bool bSucceed =true;
// 	//工具路径
// 	CString strPatternDir;
// 	//工具保存失败
// 	CString strSaveFail;
// 	//工具保存超时
// 	CString strSaveTimeout;
// 	//临时字符串
// 	CString strTemp;
// 	//当前工具的位置数
// 	int nPosNum = 0;
// 
// 	int i,j;
// 	switch(searchMode)
// 	{
// 	case eTargetSearch:
// 		{
// 			strPatternDir = strDir+ _T("\\TargetModels");
// 			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TARGET_MARK_PATTERN_SAVE_TIMEOUT);
// 			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TARGET_MARK_PATTEN_SAVE_FAILED);
// 		}
// 		break;
// 	case eVirtualSearch:
// 		{
// 			strPatternDir = strDir+ _T("\\VirtualObjectModels");
// 			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_VIRTUAL_OBJ_MARK_PATTERN_SAVE_TIMEOUT);
// 			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_VIRTUAL_OBJ_MARK_PATTERN_SAVE_FAILED);
// 		}
// 		break;
// 	case eObjectSearch:
// 		{
// 			strPatternDir = strDir+ _T("\\ObjectModels");
// 			strSaveTimeout =  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_TIMEOUT);
// 			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED);
// 		}
// 		break;
// 	case eCalibTargetSearch:
// 		{
// 			strPatternDir = strDir+ _T("\\CalibTargetModels");
// 			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_TARGET_MARK_PATTEN_SAVE_TIMEOUT);
// 			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_TARGET_MARK_PATTEN_SAVE_FAILED);
// 		}
// 		break;
// 	case  eInspectSearch:
// 		{
// 			strPatternDir = strDir+ _T("\\InspectModels");
// 			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INSPECT_MARK_PATTERN_SAVE_TIMEOUT);
// 			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INSPECT_MARK_PATTEN_SAVE_FAILED);
// 		}
// 		break;
// 	case  eCalibSearch:
// 		{
// 			strPatternDir = strDir+ _T("\\CalibModels");
// 			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_PATTERN_SAVE_TIMEOUT);
// 			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_PATTERN_SAVE_FAILED);
// 		}
// 		break;
// 	default:
// 		return false;
// 	}
// 	// 赋值临时变量
// 
// 	nPosNum = vvpSearchTool.size();
// 	CreateDirectoryPro(strPatternDir);
// 
// 	//线程
// 	HANDLE SearchToolSaveModelThread[MAXPOSNUM];
// 	//线程传递函数
// 	CSaveSearchToolRepositoryThreadParam SaveSearchToolThreadParam[MAXPOSNUM];
// 	for (j=0; j<nPosNum; j++)
// 	{
// 		SaveSearchToolThreadParam[j] =CSaveSearchToolRepositoryThreadParam(strPatternDir,j,vvpSearchTool[j],vvbSearchToolChanged[j]);//赋值线程所需的传递函数
// 		SearchToolSaveModelThread[j] = CreateThread(NULL,0,vcBaseVisionAlign::SaveSearchToolRepositoryThread,&SaveSearchToolThreadParam[j],0,NULL);//创建线程
// 	}
// 	int nReturn = WaitForMultipleObjects(nPosNum,SearchToolSaveModelThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
// 	for (j=0; j<nPosNum; j++)
// 	{
// 		CloseHandle(SearchToolSaveModelThread[j]);    // 关闭所有线程句柄并释放内存
// 	}
// 
// 
// 	if(nReturn==WAIT_TIMEOUT)
// 	{
// 		//保存超时
// 		bSucceed = false;
// 		for (j=0; j<nPosNum; j++)
// 		{
// 			if(SaveSearchToolThreadParam[j].GetIsSaveTimeOut())
// 			{
// 				strTemp.Format(_T("Pos%d_Thread%s"),j,strSaveTimeout);
// 				AfxMessageBox(strTemp);
// 			}
// 		}
// 
// 	}
// 	else
// 	{
// 		//开始判断哪些模板没有保存成功
// 		for (j=0; j<vvpSearchTool.size(); j++)
// 		{
// 			for(i = 0; i<vvpSearchTool[j].size(); i++)
// 			{
// 				if(SaveSearchToolThreadParam[j].GetSearchToolSaveResult(i)==FALSE)
// 				{
// 					strTemp.Format(_T("Pos%d_M%d%s"), j, i, strSaveFail);
// 					AfxMessageBox(strTemp);
// 					bSucceed = FALSE;
// 				}
// 			}
// 		}
// 	}
// 	return bSucceed;
// }


// 
// void CDlgSysKind::InitDlgItem()
// {
// 	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && (m_nPlatformIndex<  m_pParent->m_vpVisionASM.size()) && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
// 	{
// 		CVisionASMConfig *vsConfig;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetVSconfig(vsConfig);
// 		if (NULL !=vsConfig)
// 		{
// 
// 			CString strNodeName = _T("");
// 			vsConfig->GetCheckedByItem(eProductAuthorize_TemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowTemplateSet,m_bEnableTemplateSet);
// 
// 
// 		}
// 	}
// //	ShowWindow(SW_MAXIMIZE);
// 	int i = 0;
// 
// 	// 调整控制板
// 	int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
// 	ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());
// 
// 	for (i=0; i<MaxPlatformNum; i++)
// 	{
// 		if (i < nPlatformNum)
// 		{
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 	if (nPlatformNum <= 1)
// 	{
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
// 	}
// 
// 
// 	// 设置按钮
// 	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnApply.SetThemeHelper(&m_themeHelper);
// 
// 	int nColorOffset = -20;
// 	CString strText;
// 
// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(150, 150, 150), TRUE);
// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(150, 150, 150), TRUE);
// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(150, 150, 150), TRUE);
// 
// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM1, strText);
// 	m_btnShowPlatform1.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform1.SetWindowText(strText);
// 
// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM2, strText);
// 	m_btnShowPlatform2.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform2.SetWindowText(strText);
// 
// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM3, strText);
// 	m_btnShowPlatform3.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform3.SetWindowText(strText);
// 
// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM4, strText);
// 	m_btnShowPlatform4.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform4.SetWindowText(strText);
// 
// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM5, strText);
// 	m_btnShowPlatform5.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform5.SetWindowText(strText);
// 
// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM6, strText);
// 	m_btnShowPlatform6.SetThemeHelper(&m_themeHelper);
// 	m_btnShowPlatform6.SetWindowText(strText);
// 
// 
// 	m_btnShowPlatform1.SetCheck(1);
// 
// 	m_btnAlign.SetThemeHelper(&m_themeHelper);
// 	m_btnCalib.SetThemeHelper(&m_themeHelper);
// 	m_btnKindSetting.SetThemeHelper(&m_themeHelper);
// 	m_btnKindTarget.SetThemeHelper(&m_themeHelper);
// 	m_btnKindObject.SetThemeHelper(&m_themeHelper);
// 	m_btnKindVirtualObject.SetThemeHelper(&m_themeHelper);
// 	m_btnKindCalibTarget.SetThemeHelper(&m_themeHelper);
// 	m_btnKindInspect.SetThemeHelper(&m_themeHelper);
// 
// 	// 创建状态栏
// 	int nPartWidth[3] = {20, 300, -1};
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 
// 	int nStatusBarHeight = 0;
// 
// 	GetClientRect(rcClient);
// 
// 	// 初始化控制板
// 	CRect rcPanel, rcItem1, rcItem2;
// 	GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rcPanel);
// 	ScreenToClient(rcPanel);
// 
// 	int nButtonSizeWidth = 80;
// //	int nButtonSizeHeight = 50;
// 	int nButtonSizeHeight = 45;
// 	int nIntendY = 10, nIntervalY = 10, nIntendYOK = 6,nIntervalYOK = 6;
// 	nStatusBarHeight = 0;
// 	GetClientRect(rcClient);
// 	rcItem1.SetRect(rcClient.right-rcPanel.Width(), rcClient.top, rcClient.right, rcClient.bottom-nStatusBarHeight);
// 	GetDlgItem(IDC_BTN_PANEL)->MoveWindow(rcItem1);
// 	rcItem2.SetRect(rcItem1.left+(rcItem1.Width()-nButtonSizeWidth)/2, rcItem1.top + nIntendY, 0, 0);
// 	rcItem2.right = rcItem2.left + nButtonSizeWidth;
// 	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;
// 
// 	for (i=0; i<nPlatformNum; i++)
// 	{
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->MoveWindow(rcItem2);
// 		rcItem2.OffsetRect(0, rcItem2.Height() + nIntervalY);
// 	}
// 
// 	rcItem2.top = rcItem1.bottom-nIntendY-nButtonSizeHeight;
// 	rcItem2.bottom = rcItem2.top + nButtonSizeHeight;
// 	GetDlgItem(IDC_BTN_APPLY)->MoveWindow(rcItem2);
// 	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
// 	GetDlgItem(IDCANCEL)->MoveWindow(rcItem2);
// 	rcItem2.OffsetRect(0, -rcItem2.Height()-nIntervalY);
// 	GetDlgItem(IDOK)->MoveWindow(rcItem2);
// 
// 	CRect rcKindPannel;
// 	GetDlgItem(IDC_STATIC_KIND_PANEL)->GetWindowRect(rcKindPannel);
// 	rcItem2.left = rcClient.left;
// 	rcItem2.right = rcItem1.left + 1;
// 	rcItem2.top = rcClient.top;
// 	rcItem2.bottom = rcKindPannel.Height();
// 	GetDlgItem(IDC_STATIC_KIND_PANEL)->MoveWindow(rcItem2);
// 
// 
// 	CRect rcKindAlignTab;
// 	GetDlgItem(IDC_TAB_KIND_ALIGN)->GetWindowRect(rcKindAlignTab);
// 	rcItem2.left = rcClient.left + 1;
// 	rcItem2.right = rcItem1.left ;
// 	rcItem2.top = rcItem2.bottom + 5;
// 	rcItem2.bottom = rcClient.bottom - nStatusBarHeight;
// 	GetDlgItem(IDC_TAB_KIND_ALIGN)->MoveWindow(rcItem2);
// 
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_VIEW));		
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_SETTING));	
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_TARGET));		
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_OBJECT));
// 	
// 	// 修改新选择的平台的显示界面
// 	SysPlatformInfo platformInfo;
// 	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
// 
// 	//if(TRUE == platformInfo.m_bVirtualObjectEnable)
// 	{
// 		m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_VIRTUAL_OBJECT));
// 	}
// 	
// 	//if(TRUE == platformInfo.m_bCalibTargetEnable)
// 	{
// 		m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_CALIB_TARGET));
// 	}
// 	
// 	//if(TRUE == platformInfo.m_bInspectEnable)
// 	{
// 		m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), GetSysInfoString(m_psaSysInfoStrings,TABSHEET_ITEM_ALIGN_INSPECT));
// 	}
// 
// 	CRect rcTabSheet;
// 	m_tabSheet.GetClientRect(rcTabSheet);
// 	rcTabSheet.top	  += 22;   
// 	rcTabSheet.bottom -= 4;
// 	rcTabSheet.left	  += 2;
// 	rcTabSheet.right  -= 4;
// 
// 
// 	m_pDlgKindAlignView = new CDlgSysKindAlignView;
// 	m_pDlgKindAlignView->m_pParent = m_pParent;
// 	m_pDlgKindAlignView->m_pParentKind = this;
// 	m_pDlgKindAlignView->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 	m_pDlgKindAlignView->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 	m_pDlgKindAlignView->Create(IDD_SYS_KIND_ALIGN_VIEW, &m_tabSheet);
// 	m_pDlgKindAlignView->MoveWindow(rcTabSheet);
// 
// 	if (0)
// 	{
// 		// 暂时先不创建
// 		m_pDlgKindAlignSetting = new CDlgSysKindAlignSetting;
// 		m_pDlgKindAlignSetting->m_pParent = m_pParent;
// 		m_pDlgKindAlignSetting->m_pParentKind = this;
// 		m_pDlgKindAlignSetting->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignSetting->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignSetting->Create(IDD_SYS_KIND_ALIGN_SETTING, &m_tabSheet);
// 		m_pDlgKindAlignSetting->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignTarget = new CDlgSysKindAlignTarget;
// 		m_pDlgKindAlignTarget->m_pParent = m_pParent;
// 		m_pDlgKindAlignTarget->m_pParentKind = this;
// 		m_pDlgKindAlignTarget->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignTarget->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignTarget->Create(IDD_SYS_KIND_ALIGN_TARGET, &m_tabSheet);
// 		m_pDlgKindAlignTarget->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignObject = new CDlgSysKindAlignObject;
// 		m_pDlgKindAlignObject->m_pParent = m_pParent;
// 		m_pDlgKindAlignObject->m_pParentKind = this;
// 		m_pDlgKindAlignObject->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignObject->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignObject->Create(IDD_SYS_KIND_ALIGN_OBJECT, &m_tabSheet);
// 		m_pDlgKindAlignObject->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignVirtualObject = new CDlgSysKindAlignVirtualObject;
// 		m_pDlgKindAlignVirtualObject->m_pParent = m_pParent;
// 		m_pDlgKindAlignVirtualObject->m_pParentKind = this;
// 		m_pDlgKindAlignVirtualObject->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignVirtualObject->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignVirtualObject->Create(IDD_SYS_KIND_ALIGN_VIRTUAL_OBJECT, &m_tabSheet);
// 		m_pDlgKindAlignVirtualObject->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignCalibTarget = new CDlgSysKindAlignCalibTarget;
// 		m_pDlgKindAlignCalibTarget->m_pParent = m_pParent;
// 		m_pDlgKindAlignCalibTarget->m_pParentKind = this;
// 		m_pDlgKindAlignCalibTarget->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignCalibTarget->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignCalibTarget->Create(IDD_SYS_KIND_ALIGN_CALIB_TARGET, &m_tabSheet);
// 		m_pDlgKindAlignCalibTarget->MoveWindow(rcTabSheet);
// 
// 		m_pDlgKindAlignInspect = new CDlgSysKindAlignInspect;
// 		m_pDlgKindAlignInspect->m_pParent = m_pParent;
// 		m_pDlgKindAlignInspect->m_pParentKind = this;
// 		m_pDlgKindAlignInspect->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 		m_pDlgKindAlignInspect->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 		m_pDlgKindAlignInspect->Create(IDD_SYS_KIND_ALIGN_INSPECT, &m_tabSheet);
// 		m_pDlgKindAlignInspect->MoveWindow(rcTabSheet);
// 	}
// 
// 
// 	m_pDlgKindCalibSetting = new CDlgSysKindCalibSetting;
// 	m_pDlgKindCalibSetting->m_pParent = m_pParent;
// 	m_pDlgKindCalibSetting->m_pParentKind = this;
// 	m_pDlgKindCalibSetting->SetSysInfoStringsRef(m_pParent->m_psaSysInfoStrings);
// 	m_pDlgKindCalibSetting->EnableSysKeyboard(m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 	m_pDlgKindCalibSetting->Create(IDD_SYS_KIND_CALIB_SETTING, this);
// 	m_pDlgKindCalibSetting->MoveWindow(rcItem2);
// 
// 
// 
// 	m_tabSheet.SetCurSel(0);
// 	m_nPrevTabIndex = 0;
// 	m_pDlgKindAlignView->ShowWindow(TRUE);	
// //	ShowWindow(SW_MAXIMIZE);	
// 
// 	if (m_bSinglePlatformProductManageEnable == TRUE)
// 	{
// 		for (i = 0; i < MaxPlatformNum; i++)
// 		{
// 			if (i == m_nPlatformIndex)
// 			{
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->EnableWindow(SW_HIDE);
// 			}
// 		}
// 		UpdateData(FALSE);
// 		m_bInitCreateDlg = TRUE;
// 		OnPlatformChange();
// 		m_bInitCreateDlg = FALSE;
// 	}
// 
// 	MoveWindow(m_pParent->m_rcViewRect);
// 
// 	PostMessage(WM_SETFOCUS, NULL, NULL);
// 	UpdateData(FALSE);
// 	OnPlatformChange();
// }
