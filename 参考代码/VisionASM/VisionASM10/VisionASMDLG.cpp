// VisionASMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "VisionASMDlg.h"

// #include "SystemConfigure.h"
#include "vsBaseVisionAlign.h"
//#include "DlgVisionView.h"
#include "DlgVisionView2.h"
#include "DlgVisionRun.h"
#include "vsXYDVisionAlign.h"
#include "svXMLConfigurator.h"
#include "DlgSetCameraSerial.h"
#include "DlgSysUserLogin.h"
#include "DlgSysEnvironSetting.h"
//#include "DlgSysCalibrateParam.h"
//#include "DlgSysAlignerParam.h"
#include "DlgSysCommSetting.h"
#include "DlgSysOptionSetting.h"
#include "DlgOperationInfo.h"	//zzc报警
#include "OperationInfoUiThread.h"
#include "DlgSysKind.h"
#include "DlgCalibTargetPos.h"
//#include "Language.h"
#include "base64.h"
#include "DlgSysOption.h"
#include "LightController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionASMDlg dialog

CVisionASMDlg::CVisionASMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisionASMDlg::IDD, pParent)
	//, m_nRadioDisplayMode(0)
{
	//{{AFX_DATA_INIT(CVisionASMDlg)
	m_nPlatformDispIdx = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


// 	m_pStatusBar = NULL;
	m_psaSysInfoStrings = NULL;
	m_pDlgVisionView = NULL;
	m_pDlgVisionRun = NULL;
	m_nPlatformNum = 0;
	m_bIsRunning = FALSE;
// 	m_hIconSucceed = NULL;
// 	m_hIconFailed = NULL;
// 	m_bStatusBarFlag = FALSE;
	m_bAuthorized = FALSE;
	m_nUsePassWordMode = 1;
	m_strTitleShow = _T("");
	m_bShowBottomCtrlPannel = TRUE;
	m_nPlatformDispIdx = 0;
	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;


	m_pMainLayout = NULL;				 // 窗口布局
	m_pBtnArea = NULL;
	m_pCurViewLayout = NULL;

	m_pOperationInfoUiThread = NULL;


	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strCurWorkDir = szPath;
	m_strCurWorkDir = m_strCurWorkDir.Left(m_strCurWorkDir.ReverseFind('\\'));
	
	//if(!m_SystemConfigure.LoadSystemConfigInfo())
	//{
	//	m_SystemConfigure.SaveSystemConfigInfo();
	//	CString strInfo = _T("载入Custom.xml失败,软件恢复默认配置！");
	//	AfxMessageBox(strInfo);
	//}

	m_pSplash = NULL;
	// 软件启动界面
	m_pSplash = new CSplashScreenEx();
	m_pSplash->Create(this,_T("视觉软件启动中……"),0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	// 设置启动界面BMP图像
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_pSplash->SetBitmap(IDB_SPLASH_VISION_INIT_ENG,255,0,255);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_pSplash->SetBitmap(IDB_SPLASH_VISION_INIT,255,0,255);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_pSplash->SetBitmap(IDB_SPLASH_VISION_INIT,255,0,255);
		break;
	}

	m_pSplash->SetTextFont("Impact",120,CSS_TEXT_NORMAL);
	m_pSplash->SetTextRect(CRect(10,220,300,104));
	m_pSplash->SetTextColor(RGB(0,0,255));
	m_pSplash->SetTextFormat(DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	m_pSplash->Show();
	CString strLog;
	strLog.Format(_T("软件启动中……\n"));
	m_pSplash->SetText(strLog);
	Sleep(10);

	if (!m_SysOptionConfig.LoadSystemOption())
	{
	//	m_SysOptionConfig.m_bSysKeyboardEnabled = FALSE;
		m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
		m_SysOptionConfig.SaveSystemOption();
	}
	strLog.Format(_T("载入软件配置……\n"));
	m_pSplash->SetText(strLog);
	Sleep(10);

	CLightController::GetInstance().OpenConnect(m_SysOptionConfig.m_sysOptLightConfig);

	strLog.Format(_T("光源初始化……\n"));
	m_pSplash->SetText(strLog);

	if (!m_SystemCompanyInfo.LoadCompanyInfo())
	{
		m_SystemCompanyInfo.SaveCompanyInfo();
	}

	m_pDlgEnvironSetting = NULL;
	m_pDlgOptionSetting = NULL;
	m_pDlgSysKind = NULL;

	m_bShowOtherView = TRUE;
	m_eWSShowMode =  esmMode0;
	
	// 相机初始化
	CString  strInfo = "Camera InitSystem Begin \n";
	AlignToolLogRecord(strInfo, 0);

	strLog.Format(_T("相机初始化中……\n"));
	m_pSplash->SetText(strLog);
	Sleep(10);

	scPTGrab2::InitSystem();

	// 相机初始化完成
	strInfo = "Camera InitSystem End \n";
	AlignToolLogRecord(strInfo, 0);

	strLog.Format(_T("相机初始化完成\n"));
	m_pSplash->SetText(strLog);
	Sleep(10);
	vcBaseVisionAlign::CreateManualSearchSection();

	m_vpSuspendResumeWorkThreadObject.clear();
	m_vbStartSuspendResume.clear();

	m_bPasswordReEnter = TRUE;
	m_nSeconds = 0;

	m_strAdminPassword = _T("visionsmart");
	m_strAdminPassword2 = _T("123456");
}

void CVisionASMDlg::InitLayoutUI()
{
	// 标题栏
	CLayout* pTitleLayout = InitTitleLayout();
	// 显示与区域
	// 功能按钮
	CLayout* pBtnLayout = InitFuncBtnLayout();
	m_pBtnArea = pBtnLayout;

	m_pMainLayout = new CVerticalLayout;
	m_pMainLayout->AddLayout(pTitleLayout, 0, 35, CLayout::eSizePolicy_Fixed, 0, 0, 0, 0);
	m_pMainLayout->AddWnd(GetDlgItem(IDC_STATIC_VIEW), 0, 0, CLayout::eSizePolicy_Expand, 0, 0 ,0, 2);
	m_pMainLayout->AddLayout(pBtnLayout, 0, 55, CLayout::eSizePolicy_Fixed);
	m_pMainLayout->SetParent(this);
}

CLayout* CVisionASMDlg::InitTitleLayout()
{
	// 工位按钮水平布局
	CHorizontalLayout* pWSLayout = new CHorizontalLayout;
	// 先给布局加一个弹簧
	CHorizontalLayout* pSpring = new CHorizontalLayout;
	pWSLayout->AddLayout(pSpring, 0, 0, CLayout::eSizePolicy_Expand);
	CXPStyleButtonST* pButtonArray[] = { &m_btnShowPlatformAll, &m_btnShowPlatform1, &m_btnShowPlatform2, &m_btnShowPlatform3, &m_btnShowPlatform4, &m_btnShowPlatform5, &m_btnShowPlatform6 };
	for (int i = 0; i < sizeof(pButtonArray)/sizeof(CXPStyleButtonST*); i++)
	{
		pWSLayout->AddWnd(pButtonArray[i], 55, 0, CLayout::eSizePolicy_Fixed);
		if (i > m_nPlatformNum)
		{
			pButtonArray[i]->ShowWindow(SW_HIDE);
		}
	}
	pWSLayout->AddWnd(&m_btnSetDisPlayMode, 75, 0, CLayout::eSizePolicy_Fixed);

	// 标题栏
	CHorizontalLayout* pTitleLayout = new CHorizontalLayout;
	pTitleLayout->AddWnd(&m_labelLogo, 50, 0, CLayout::eSizePolicy_Fixed, 15, 3, 5, 3);
	pTitleLayout->AddWnd(&m_labelTitle, 0, 0, CLayout::eSizePolicy_Expand, 0, 0, 0, 0);
	pTitleLayout->AddLayout(pWSLayout, 386, 0, CLayout::eSizePolicy_Fixed, 0, 0, 0, 0);

  	// 整个标题栏布局
	CVerticalLayout* pTitleBar = new CVerticalLayout;
	pTitleBar->AddWnd(&m_labelTitleBar, 0, 0, CLayout::eSizePolicy_Expand, 0, 0, 0, -17);
	pTitleBar->AddLayout(pTitleLayout, 0, 0, CLayout::eSizePolicy_Expand, 0, -15, 0, 1);


	return pTitleBar;
}

CLayout* CVisionASMDlg::InitShowViewLayout()
{
	// 显示与区域
	CHorizontalLayout* pShowViewRunLayout = new CHorizontalLayout;
	pShowViewRunLayout->AddWnd(GetDlgItem(IDC_STATIC_VIEW), 0, 0, CLayout::eSizePolicy_Expand);
	return pShowViewRunLayout;
}

CLayout* CVisionASMDlg::InitFuncBtnLayout()
{
	CHorizontalLayout* pBtnLayout = new CHorizontalLayout;
	CXPStyleButtonST* pButtonArray[] = {&m_btnEnvironmentSet, &m_btnProductManage, &m_btnOptionSet, &m_btnRun, &m_btnLogin, &m_btnExit, &m_btnShutDownWindows};

	for (int i = 0; i < sizeof(pButtonArray) / sizeof(CXPStyleButtonST*); i++)
	{
		pBtnLayout->AddWnd(pButtonArray[i], 0, 0, CLayout::eSizePolicy_Preferred);
	}
	
	return pBtnLayout;
}


void CVisionASMDlg::InitUIStyle()
{
	 InitTitleStyle();
	 InitFunBtnStyle();
}


void CVisionASMDlg::InitTitleStyle()
{

	COLORREF TmpColor = RGB(54,164,186);

	//m_labelTitleBar.SetBkColor(RGB(0, 0, 255));
	//m_labelTitle.SetBkColor(RGB(0, 0, 255));
	m_labelTitleBar.SetBkColor(TmpColor);
	m_labelTitle.SetBkColor(TmpColor);

	m_labelTitle.SetTextColor(RGB(255, 255, 255));
	m_labelTitle.SetFontSize(14);
	m_labelTitle.SetFontBold(TRUE);
	m_labelTitle.SetText(_T("视觉对位系统 V1.99.2.0"));

	m_cfWSButton.CreateFont(14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("黑体"));					// lpszFacename)
	CXPStyleButtonST* pButtonArray[] = { &m_btnSetDisPlayMode,&m_btnShowPlatformAll, &m_btnShowPlatform1, &m_btnShowPlatform2, &m_btnShowPlatform3, &m_btnShowPlatform4, &m_btnShowPlatform5, &m_btnShowPlatform6 };
	for (int i = 0; i < sizeof(pButtonArray)/sizeof(CXPStyleButtonST*); i++)
	{
		pButtonArray[i]->SetFont(&m_cfWSButton);
		pButtonArray[i]->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
		pButtonArray[i]->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
		pButtonArray[i]->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	}
	for (int i =0 ; i < m_nPlatformNum; i++)
	{
		SysPlatformInfo spi;
		m_SystemConfigure.GetSysPlatformInfo(i, spi);
		if (spi.m_bPlatformShareShow)
		{
			pButtonArray[i+1]->EnableWindow(FALSE);
		}
	}
}


void CVisionASMDlg::InitFunBtnStyle()
{
	// 功能按钮
	m_cfFunButton.CreateFont(20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("黑体"));					// lpszFacename)

	CXPStyleButtonST* pButtonArray[] = {&m_btnEnvironmentSet, &m_btnProductManage, &m_btnOptionSet, &m_btnRun, &m_btnLogin, &m_btnExit, &m_btnShutDownWindows};

	for (int i = 0; i < sizeof(pButtonArray) / sizeof(CXPStyleButtonST*); i++)
	{
		pButtonArray[i]->SetFont(&m_cfFunButton);
		if (pButtonArray[i] != &m_btnRun)
		{
			pButtonArray[i]->SetThemeHelper(&m_themeHelper);
		}
	}
}


void CVisionASMDlg::InitUIData()
{
	InitTitleData();
	InitFunBtnData();
}


void CVisionASMDlg::InitTitleData()
{
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_SystemCompanyInfo.m_strSysCompanyFilePath  = m_strCurWorkDir + _T("\\CompanyENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_SystemCompanyInfo.m_strSysCompanyFilePath  = m_strCurWorkDir + _T("\\Company.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_SystemCompanyInfo.m_strSysCompanyFilePath  = m_strCurWorkDir + _T("\\Company.xml");
		break;
	}


	// 加载公司信息
	if (!m_SystemCompanyInfo.LoadCompanyInfo())
	{
		m_SystemCompanyInfo.SaveCompanyInfo();
	}

	if(m_SystemCompanyInfo.m_nFontSize > 5)
	{
		m_labelTitle.SetFontSize(m_SystemCompanyInfo.m_nFontSize);
	}
	else
	{
		m_labelTitle.SetFontSize(17);
	}


	m_hIconLogo =  (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strCurWorkDir +_T("\\")+ m_SystemCompanyInfo.m_strCompanyLogo, IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if (m_hIconLogo != NULL)
	{
		m_labelLogo.SetBitmap(m_hIconLogo);
	}
	else
	{
		m_labelLogo.ShowWindow(SW_HIDE);
	}

	CString strTitle;
	// 3空格占位
	if (m_hIconLogo == NULL)
	{
		strTitle = _T("");
	}
	else
	{
		strTitle = _T("   ");
	}

	// 公司名称
	if (m_SystemCompanyInfo.m_strCompanyTitle.IsEmpty())
	{		
	}
	else
	{
		strTitle += m_SystemCompanyInfo.m_strCompanyTitle;
	}

	// 软件名称
	if (m_SystemCompanyInfo.m_strSoftwareTitle.IsEmpty())
	{
		strTitle += GetSysInfoString(m_psaSysInfoStrings,IDS_MB_STR_SOFTWARE_TITLE);
	}
	else
	{
		strTitle += m_SystemCompanyInfo.m_strSoftwareTitle;
	}

	// 版本前空格占位
	if (m_SystemCompanyInfo.m_strVersionDateFront.IsEmpty())
	{		
	}
	else
	{
		strTitle += m_SystemCompanyInfo.m_strVersionDateFront;
	}

	// 版本日期
	if (m_SystemCompanyInfo.m_bVersionDateCustomEnable)
	{		
		strTitle += m_SystemCompanyInfo.m_strVersionDate;		
	}
	else
	{
		//		strTitle += GetSysInfoString(m_psaSysInfoStrings,IDS_MB_STR_VERSION_DATE) + m_SystemConfigure.GetSoftwareVersion();	//hk20180130
		strTitle += m_SystemConfigure.GetSoftwareVersion();
	}

	// 版本后空格占位
	if (m_SystemCompanyInfo.m_strVersionDateBack.IsEmpty())
	{		
	}
	else
	{
		strTitle += m_SystemCompanyInfo.m_strVersionDateBack;
	}

	// 软件注释说明
	if (m_SystemCompanyInfo.m_strCompanyNote.IsEmpty())
	{		
	}
	else
	{
		strTitle += m_SystemCompanyInfo.m_strCompanyNote;
	}
	CString strTmp = (m_nUsePassWordMode==1)?GetSysInfoString(m_psaSysInfoStrings,IDS_GENERAL_AUTHORITY):GetSysInfoString(m_psaSysInfoStrings,IDS_SUPER_AUTHORITY);
	m_labelTitle.SetWindowText(strTitle + strTmp);
	m_strTitleShow = strTitle;
}

void CVisionASMDlg::InitFunBtnData()
{
	m_btnEnvironmentSet.SetWindowText(GetDlgCtrlString(IDC_BTN_ENVIRONMENT_SET));
	m_btnProductManage.SetWindowText(GetDlgCtrlString(IDC_BTN_PRODUCT_MANAGE));
	m_btnOptionSet.SetWindowText(GetDlgCtrlString(IDC_BTN_OPTION_SET));
	m_btnOptionSet.SetWindowText(GetDlgCtrlString(IDC_BTN_OPTION_SET));

	m_btnRun.SetWindowText(GetDlgCtrlString(IDC_BTN_ALIGNER_RUN));
	
	m_btnLogin.SetWindowText(GetDlgCtrlString(IDC_BTN_USER_LOGIN));
	m_btnExit.SetWindowText(GetDlgCtrlString(IDCANCEL));
	m_btnShutDownWindows.SetWindowText(GetDlgCtrlString(IDC_BTN_CLOSE_WINDOWS));

	//HICON icon[7];
	//icon[0]=AfxGetApp()->LoadIcon(IDI_ICON_SETTING1);
	//icon[1]=AfxGetApp()->LoadIcon(IDI_ICON_PRODUCTMANAGE1);
	//icon[2]=AfxGetApp()->LoadIcon(IDI_ICON_OPTION1);
	//icon[3]=AfxGetApp()->LoadIcon(IDI_ICON_RUN1);
	//icon[4]=AfxGetApp()->LoadIcon(IDI_ICON_ADMINLOAD1);
	//icon[5]=AfxGetApp()->LoadIcon(IDI_ICON_EXIT1);
	//icon[6]=AfxGetApp()->LoadIcon(IDI_ICON_SHUT1);
	
	
	//icon[6]=AfxGetApp()->LoadIcon(IDI_ICON4);
	//icon[7]=AfxGetApp()->LoadIcon(IDI_ICON4);

	m_btnEnvironmentSet.SetIcon(IDI_ICON_SETTING1);
	m_btnProductManage.SetIcon(IDI_ICON_PRODUCTMANAGE1);
	m_btnOptionSet.SetIcon(IDI_ICON_OPTION1);
	m_btnRun.SetIcon(IDI_ICON_RUN1);
	m_btnLogin.SetIcon(IDI_ICON_ADMINLOAD1);
	m_btnExit.SetIcon(IDI_ICON_EXIT1);
	m_btnShutDownWindows.SetIcon(IDI_ICON_SHUT1);

	//m_btnEnvironmentSet.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);
	//m_btnProductManage.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);
	//m_btnOptionSet.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);
	//m_btnRun.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);
	//m_btnLogin.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);
	//m_btnExit.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);
	//m_btnShutDownWindows.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);


	//m_btnProductManage.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnProductManage.ico
	//m_btnRun.SetIcon(icon[0]);

}


void CVisionASMDlg::InitRunWindow()
{
	m_pDlgVisionRun = new CDlgVisionRun[m_nPlatformNum];
	SysPlatformInfo locSysPlatformInfo;

	for (int i = 0; i < m_nPlatformNum; i++)
	{
		m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
		//m_pDlgVisionRun[i].SetCameraNumber(locSysPlatformInfo.m_nPositionNum);
		m_pDlgVisionRun[i].SetParentWnd(this);
		//m_pDlgVisionRun[i].SetSpecial1Platform(FALSE);
		m_pDlgVisionRun[i].Create(IDD_DLG_VISION_RUN);
		m_pDlgVisionRun[i].ShowWindow(SW_HIDE);
		CString strID;
		if (m_SysOptionConfig.m_bPlatformCustomNameEnable)
		{
			strID = m_SysOptionConfig.m_strPlatformCustomName[i];
		}
		else
		{			
			strID.Format("%d", i+1);	// 平台号从1开始
		}
		m_pDlgVisionRun[i].SetPlatformID(strID);	
		m_pDlgVisionRun[i].SetPlatformID(i);			
		m_pDlgVisionRun[i].EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	}
}

void CVisionASMDlg::InitViewWindow()
{
	m_vnVisionViewPos.resize(m_nPlatformNum);
	m_pDlgVisionView = new CDlgVisionView2[m_nPlatformNum];

	SysPlatformInfo locSysPlatformInfo;
	for (int i = 0; i < m_nPlatformNum; i++)
	{
		if (!m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo))
		{
			CString strInfo;
			strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_LOAD_CONFIG_INFO));
			AfxMessageBox(strInfo,MB_SYSTEMMODAL);
		}
		if (locSysPlatformInfo.m_bPlatformShareShow)
		{
			m_pDlgVisionView[locSysPlatformInfo.m_nShareShowPlatformIndex].AddVisionRunDlg(&m_pDlgVisionRun[i]);
		}
		else
		{
			m_pDlgVisionView[i].AddVisionRunDlg(&m_pDlgVisionRun[i]);
		}
	}
	for (int i=0; i<m_nPlatformNum; i++)
	{
		if (!m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo))
		{
			// 关闭启动界面
			//CSplashWnd::HideSplashScreen();
			CString strInfo;
			strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_LOAD_CONFIG_INFO));
			AfxMessageBox(strInfo,MB_SYSTEMMODAL);
		}

		m_vnVisionViewPos[i] = i;
		m_pDlgVisionView[i].SetPlatformID(i);
		m_pDlgVisionView[i].SetCameraNumber(locSysPlatformInfo.m_nCamNum, locSysPlatformInfo.m_nPositionNum, locSysPlatformInfo.m_nShowMode, /*locSysPlatformInfo.m_nPositionShowMode,*/ locSysPlatformInfo.m_nDisplayArrayMode);
		m_pDlgVisionView[i].SetParentWnd(this);	
		m_pDlgVisionView[i].SetVisionRunDlg(&m_pDlgVisionRun[i]);
	

		m_pDlgVisionView[i].Create(IDD_DLG_VISION_VIEW2, this);
		m_pDlgVisionView[i].ShowWindow(SW_SHOW);

		CString strID;
		if (m_SysOptionConfig.m_bPlatformCustomNameEnable)
		{
			strID = m_SysOptionConfig.m_strPlatformCustomName[i];
			m_pDlgVisionView[i].SetPlatformID(m_SysOptionConfig.m_strPlatformCustomName[i]);
		}
		else
		{			
			strID.Format("%d", i+1);	// 平台号从1开始
			m_pDlgVisionView[i].SetPlatformID(GetDlgCtrlString(IDC_STATIC_WS)+strID);
		}

		
		m_pDlgVisionView[i].SetAllPosBtnHide();
		if (!locSysPlatformInfo.m_bPlatformShareShow)
		{
			m_vpNotShareVisionView.push_back(&m_pDlgVisionView[i]);
		}
		//m_pDlgVisionView[i].SetAlignerInfoAlignment(5);
	}
	//m_pDlgVisionView[0].SetAlignerInfoAlignment(6);
}

void CVisionASMDlg::InitSingleViewLayout()
{
	for (int i = 0; i < m_nPlatformNum; i++)
	{
		CLayout*  pSinglePlatView = new CHorizontalLayout;
		pSinglePlatView->AddWnd(&m_pDlgVisionView[i]);
		pSinglePlatView->SetParent(GetDlgItem(IDC_STATIC_VIEW));
		m_vpSingleViewLayout.push_back(pSinglePlatView);
	}
}

 // 初始化视图布局
void CVisionASMDlg::InitViewLayout()
{
	 switch(m_vpNotShareVisionView.size()/*m_nPlatformNum*/)
	 {
	 case 1:
		 InitViewLayoutPlatformNum1();
		 break;
	 case 2:
		 InitViewLayoutPlatformNum2();
		 break;
	 case 3:
		 InitViewLayoutPlatformNum3();
		 break;
	 case 4:
		 InitViewLayoutPlatformNum4();
		 break;
	 case 5:
		 InitViewLayoutPlatformNum5();
		 break;
	 case 6:
		 InitViewLayoutPlatformNum6();
		 break;
	 }

	 if (m_nViewShowMode >= m_vpViewLayout.size())
	 {
		 m_nViewShowMode = 0;
	 }
	 m_vpViewLayout[m_nViewShowMode]->OnSize();
}

void CVisionASMDlg::InitViewLayoutPlatformNum1()
{
	// 第一种布局类型
	CHorizontalLayout* pViewLayout = new CHorizontalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0]);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);
}

void CVisionASMDlg::InitViewLayoutPlatformNum2()
{
	// 第一种布局类型	水平排列
	CLayout* pViewLayout = new CHorizontalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0]);
	pViewLayout->AddWnd(m_vpNotShareVisionView[1]);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

	// 第二种布局类型	垂直排列
	pViewLayout = new CVerticalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0]);
	pViewLayout->AddWnd(m_vpNotShareVisionView[1]);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);


}


void CVisionASMDlg::InitViewLayoutPlatformNum3()
{
	// 第一种布局类型	水平排列
	CLayout* pViewLayout = new CHorizontalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0]);
	pViewLayout->AddWnd(m_vpNotShareVisionView[1]);
	pViewLayout->AddWnd(m_vpNotShareVisionView[2]);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

	// 第二种布局类型	垂直排列
	pViewLayout = new CVerticalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0]);
	pViewLayout->AddWnd(m_vpNotShareVisionView[1]);
	pViewLayout->AddWnd(m_vpNotShareVisionView[2]);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);


	// 第三种布局类型	左1右2
	pViewLayout = new CHorizontalLayout;
	CLayout* pColLayout1 = new CVerticalLayout;
	pColLayout1->AddWnd(m_vpNotShareVisionView[1]);
	pColLayout1->AddWnd(m_vpNotShareVisionView[2]);
	pViewLayout->AddWnd(m_vpNotShareVisionView[0]);
	pViewLayout->AddLayout(pColLayout1);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

	// 第二种布局类型	左2右1
	pViewLayout = new CHorizontalLayout;
	pColLayout1 = new CVerticalLayout;
	pColLayout1->AddWnd(m_vpNotShareVisionView[0]);
	pColLayout1->AddWnd(m_vpNotShareVisionView[1]);
	pViewLayout->AddLayout(pColLayout1);
	pViewLayout->AddWnd(m_vpNotShareVisionView[2]);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);


	// 第五种  上2下1  | | |	
	//                 |   |
	pViewLayout = new CVerticalLayout;
	CLayout* pRowLayout1 = new CHorizontalLayout;
	pRowLayout1->AddWnd(m_vpNotShareVisionView[0]);
	pRowLayout1->AddWnd(m_vpNotShareVisionView[1]);
	pViewLayout->AddLayout(pRowLayout1);
	pViewLayout->AddWnd(m_vpNotShareVisionView[2]);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

	// 第六种 上1 下2
	pViewLayout = new CVerticalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0]);
	pRowLayout1 = new CHorizontalLayout;
	pRowLayout1->AddWnd(m_vpNotShareVisionView[1]);
	pRowLayout1->AddWnd(m_vpNotShareVisionView[2]);
	pViewLayout->AddLayout(pRowLayout1);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);
}


void CVisionASMDlg::InitViewLayoutPlatformNum4()
{
	// 第一种两行两列
	CLayout* pViewLayout = new CVerticalLayout;
	CLayout* pRowLayout1 = new CHorizontalLayout;
	CLayout* pRowLayout2 = new CHorizontalLayout;
	pRowLayout1->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout1->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout2->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout2->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pRowLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pRowLayout2, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

	// 第二种一行4列
	pViewLayout = new CHorizontalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

	// 第三种4行一列
	pViewLayout = new CVerticalLayout;
	pViewLayout->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);
}


void CVisionASMDlg::InitViewLayoutPlatformNum5()
{
	 // 第一种 上3下2
	CLayout* pViewLayout = new CVerticalLayout;
	CLayout* pRowLayout1 = new CHorizontalLayout;
	CLayout* pRowLayout2 = new CHorizontalLayout;
	pRowLayout1->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout1->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout1->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout2->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout2->AddWnd(m_vpNotShareVisionView[4], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pRowLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pRowLayout2, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);


	// 第二种 上2下3
	pViewLayout = new CVerticalLayout;
	pRowLayout1 = new CHorizontalLayout;
	pRowLayout2 = new CHorizontalLayout;
	pRowLayout1->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout1->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout2->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout2->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pRowLayout2->AddWnd(m_vpNotShareVisionView[4], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pRowLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pRowLayout2, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

	// 第三种 左2 右3
	pViewLayout = new CHorizontalLayout;
	CLayout* pColLayout1 = new CVerticalLayout;
	CLayout* pColLayout2 = new CVerticalLayout;
	pColLayout1->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout1->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[4], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout2, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);


	// 第四种 左3 右2
	pViewLayout = new CHorizontalLayout;
	pColLayout1 = new CVerticalLayout;
	pColLayout2 = new CVerticalLayout;
	pColLayout1->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout1->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout1->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[4], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout2, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);

}


void CVisionASMDlg::InitViewLayoutPlatformNum6()
{
	// 第一种情况 2行3列
	CLayout* pViewLayout = new CVerticalLayout;
	CLayout* pColLayout1 = new CHorizontalLayout;
	CLayout* pColLayout2 = new CHorizontalLayout;
	pColLayout1->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout1->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout1->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[4], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[5], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout2, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);


	// 第一种情况 3行2列
	pViewLayout = new CVerticalLayout;
	pColLayout1 = new CHorizontalLayout;
	pColLayout2 = new CHorizontalLayout;
	CLayout* pColLayout3 = new CHorizontalLayout;
	pColLayout1->AddWnd(m_vpNotShareVisionView[0], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout1->AddWnd(m_vpNotShareVisionView[1], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[2], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout2->AddWnd(m_vpNotShareVisionView[3], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout3->AddWnd(m_vpNotShareVisionView[4], 0, 0, CLayout::eSizePolicy_Preferred);
	pColLayout3->AddWnd(m_vpNotShareVisionView[5], 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout1, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout2, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->AddLayout(pColLayout3, 0, 0, CLayout::eSizePolicy_Preferred);
	pViewLayout->SetParent(GetDlgItem(IDC_STATIC_VIEW));
	m_vpViewLayout.push_back(pViewLayout);
}


void CVisionASMDlg::UpdateViewLayoutShow(int nViewDisplayId)
{
	// 全工位显示

	if (nViewDisplayId==0)
	{
		for (int i =0; i < m_nPlatformNum; i++)
		{
			SysPlatformInfo spi;
			m_SystemConfigure.GetSysPlatformInfo(i, spi);
			if (!spi.m_bPlatformShareShow)
			{
				m_pDlgVisionView[i].ShowWindow(SW_SHOW);
			}
			else
			{
				m_pDlgVisionView[i].ShowWindow(SW_HIDE);
			}
		}

		if (m_nViewShowMode >= m_vpViewLayout.size())
		{
			m_nViewShowMode = 0;
		}

		m_pCurViewLayout = m_vpViewLayout[m_nViewShowMode];
	}
	else if(nViewDisplayId > 0 || nViewDisplayId <= m_nPlatformNum)
	{
		for (int i =0; i < m_nPlatformNum; i++)
		{
			m_pDlgVisionView[i].ShowWindow(SW_HIDE);
		}
		m_pDlgVisionView[nViewDisplayId-1].ShowWindow(SW_SHOW);

		m_pCurViewLayout = m_vpSingleViewLayout[nViewDisplayId-1];
	}

	m_pCurViewLayout->OnSize();
	UpdateWindow();
}


void CVisionASMDlg::UpdateAllViewLayout()
{
	m_nViewShowMode = m_SysOptionConfig.m_nPlatformLayoutType;
	UpdateViewLayoutShow(m_nPlatformDispIdx);
	//Invalidate();
}



void CVisionASMDlg::UpdateViewDisplayLayout(int nPlatformId)
{
	CPlatformLayoutInfo layoutInfo;
	m_vpVisionASM[nPlatformId]->GetPlatformLayoutInfo(layoutInfo);

	int nViewLayoutType = m_pDlgVisionView[nPlatformId].GetViewLayoutType();
	int nVisionRunWidth = m_pDlgVisionView[nPlatformId].GetVisionRunWidth();
	int nViewRunLayoutType = m_pDlgVisionView[nPlatformId].GetCurMainLayoutIndex();
	if (nViewLayoutType != layoutInfo.m_nViewLayoutType 
		|| nVisionRunWidth != layoutInfo.m_nViewVisionRunWidth
		|| nViewRunLayoutType != layoutInfo.m_nViewMainLayoutType)
	{
		m_pDlgVisionView[nPlatformId].SetViewLayoutType(layoutInfo.m_nViewLayoutType);
		m_pDlgVisionView[nPlatformId].SetVisionRunWidth(layoutInfo.m_nViewVisionRunWidth);
		m_pDlgVisionView[nPlatformId].SetCurMainLayoutIndex(layoutInfo.m_nViewMainLayoutType);
		m_pDlgVisionView[nPlatformId].UpdateViewLayout();
		m_pDlgVisionView[nPlatformId].UpdateMainLayout();
	}
}



void CVisionASMDlg::EnumCamera()
{
	CString strLog;
	int nAllCamNumber = 0;	// 所有平台的相机数量
	int nAllPosNumber = 0;	// 所有平台的位置数量

	m_nPlatformNum = m_SystemConfigure.GetPlatformNum();
	SysPlatformInfo locSysPlatformInfo;
	for (int i=0; i<m_nPlatformNum; i++)
	{
		if (m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo))
		{
			nAllCamNumber += locSysPlatformInfo.m_nCamNum;
			nAllPosNumber += locSysPlatformInfo.m_nPositionNum;
		}
	}


	// 相机启动
	CString  strInfo = "Camera Begin Sleep\n";
	AlignToolLogRecord(strInfo, 0);
	int nCheckDelayTime = vcGrab::GetCheckDelayTime();
	//Sleep(nCheckDelayTime); // 延时2秒
	//===================================================================
	//查询总线上的相机数量是否满足要求
	unsigned int nBusCamNum = 0;
	//scPTGrab2::GetBusCameraCount(&nBusCamNum);
	vcGrab::GetBusCameraCount(&nBusCamNum);
	strInfo.Format("The -1 time  GetBusCameraCount number is %d\n",nBusCamNum);
	AlignToolLogRecord(strInfo, 0);
	int nLoop  = 0;
	while (nLoop++<5 && nBusCamNum < nAllCamNumber)
	{
		Sleep(1000); // 延时1秒
		//scPTGrab2::GetBusCameraCount(&nBusCamNum);
		vcGrab::GetBusCameraCount(&nBusCamNum);
		strInfo.Format("The %d time  GetBusCameraCount number is %d\n",nLoop,nBusCamNum);
		AlignToolLogRecord(strInfo, 0);
	}

	if (nBusCamNum < nAllCamNumber)
	{
		Sleep(nCheckDelayTime);  // 延时2秒

		vcGrab::GetBusCameraCount(&nBusCamNum);
		strInfo.Format("The %d time  GetBusCameraCount number is %d\n",nLoop,nBusCamNum);
		AlignToolLogRecord(strInfo, 0);
	}

	if(nBusCamNum > 0)
	{
		PTCameraInfo *pPTCameraInfo = new PTCameraInfo[nBusCamNum];
		scPTGrab2::EnumerateBusCameras(pPTCameraInfo, &nBusCamNum);

		if (pPTCameraInfo != NULL)
		{
			delete[] pPTCameraInfo;
			pPTCameraInfo = NULL;
		}
	}

	//===============================================================================
	strInfo.Format("Camera Begin Sleep End\n");
	AlignToolLogRecord(strInfo, 0);

	strLog.Format(_T("相机枚举延时完成\n"));
	g_opLogFile.SaveLogText(strLog,0);
}



void CVisionASMDlg::EnableDlgItem(BOOL bEnable)
{

	m_vpVisionASM.at(0)->SetSystemOptionConfig(&m_SysOptionConfig);
	if (m_nUsePassWordMode == 1 && m_SysOptionConfig.m_bCheckFPMI)
	{
		//
		//m_SysOptionConfig.m_nMinute;
		// 获取时间	
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		//将时间都转化为分钟数，
		int m_StopMin = (tm.wDay-1) * 1440 + tm.wHour * 60 + tm.wMinute;
		//不跨月的情况
		if (m_SysOptionConfig.m_nMinute <= m_StopMin && m_SysOptionConfig.m_nMinute !=-1)
		{

			if (m_StopMin - m_SysOptionConfig.m_nMinute>m_SysOptionConfig.m_nMinuteFPMI)
			{
				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
			}

		}
		else if ((44640 - m_SysOptionConfig.m_nMinute) + m_StopMin< m_SysOptionConfig.m_nMinuteFPMI)
		{

			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
	}

	//GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_ALIGNER_RUN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_USER_LOGIN)->EnableWindow(bEnable);
	GetDlgItem(IDCANCEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->EnableWindow(bEnable);
	//GetDlgItem(IDC_BTN_REGISTER)->EnableWindow(bEnable);
}


BOOL CVisionASMDlg::CameraSerialNumConfig(std::vector<DWORD>* pvdwInitErrCode)
{
	CDlgSetCameraSerial dlgSetCamSerial(this);
	dlgSetCamSerial.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	dlgSetCamSerial.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgSetCamSerial.SetInitErrorCode(pvdwInitErrCode);

	if (dlgSetCamSerial.DoModal() == IDOK)
	{
		return TRUE;
	}
	else
	{
		// Mali 2013.9.18
		if (dlgSetCamSerial.m_bNotEnterApp)
		{
            this->OnCancel();
		}

		return FALSE;
	}
} 


HICON CVisionASMDlg::CreateGrayscaleIcon(HICON hIcon)
{
	HICON		hGrayIcon = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	BITMAP		bmp;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	ICONINFO	csII, csGrayII;
	BOOL		bRetValue = FALSE;
	
	bRetValue = ::GetIconInfo(hIcon, &csII);
	if (bRetValue == FALSE)	return NULL;
	
	hMainDC = ::GetDC(NULL);
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMainDC == NULL || hMemDC1 == NULL || hMemDC2 == NULL)	return NULL;
	
	if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
	{
		DWORD	dwWidth = csII.xHotspot*2;
		DWORD	dwHeight = csII.yHotspot*2;
		
		csGrayII.hbmColor = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
		if (csGrayII.hbmColor)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);
			
			//::BitBlt(hMemDC2, 0, 0, dwWidth, dwHeight, hMemDC1, 0, 0, SRCCOPY);
			
			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;
			BYTE		byNewPixel = 0;
			
			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC1, dwLoopX, dwLoopY);
					
					byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));
					if (crPixel)	::SetPixel(hMemDC2, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
				} // for
			} // for
			
			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);
			
			csGrayII.hbmMask = csII.hbmMask;
			
			csGrayII.fIcon = TRUE;
			hGrayIcon = ::CreateIconIndirect(&csGrayII);
		} // if
		
		::DeleteObject(csGrayII.hbmColor);
		//::DeleteObject(csGrayII.hbmMask);
	} // if
	
	::DeleteObject(csII.hbmColor);
	::DeleteObject(csII.hbmMask);
	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);
	
	return hGrayIcon;
} // End of CreateGrayscaleIcon

void CVisionASMDlg::UpdateDialogLanguage()
{
//	return;
	
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir = m_strCurWorkDir;
	
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
		
		if(_T("IDD_VISIONASM_DIALOG2") == strDlgID)
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

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_SystemCompanyInfo.m_strSysCompanyFilePath  = m_strCurWorkDir + _T("\\CompanyENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_SystemCompanyInfo.m_strSysCompanyFilePath  = m_strCurWorkDir + _T("\\Company.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_SystemCompanyInfo.m_strSysCompanyFilePath  = m_strCurWorkDir + _T("\\Company.xml");
		break;
	}
	if (!m_SystemCompanyInfo.LoadCompanyInfo())
	{
		m_SystemCompanyInfo.SaveCompanyInfo();
	}
	CString strMsg;
	strMsg.Empty();
	AddTitleWarningMsg(strMsg);

	if (m_bShowBottomCtrlPannel)
	{		
		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->SetWindowText(GetDlgCtrlString(IDC_BTN_SHOW_CTRL_PANNEL));	//_T("隐藏控制面板"));
	}
	else
	{
		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->SetWindowText(GetDlgCtrlString(IDC_BTN_SHOW_CTRL_PANNEL));	//_T("显示控制面板"));
	}
	if (m_bAuthorized)
	{
		GetDlgItem(IDC_BTN_USER_LOGIN)->SetWindowText(GetDlgCtrlString(IDC_BTN_USER_LOGIN));	//_T("管理员注销"));
	}
	else
	{
		GetDlgItem(IDC_BTN_USER_LOGIN)->SetWindowText(GetDlgCtrlString(IDC_BTN_USER_LOGIN));	//_T("管理员登录"));
	}
		
}

CString CVisionASMDlg::GetDlgCtrlString(int nCtrlID)
{
	CString strCtrl;
	strCtrl.Empty();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir = m_strCurWorkDir;

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

		if(_T("IDD_VISIONASM_DIALOG2") == strDlgID)
		{
			
			CString strValue = xml.GetAttribute(_T("Value"));
			//SetWindowText(strValue);

			xml.EnterElem();	   
			while(xml.FindElem())
			{
				int nID;
				xml.GetAttribute(_T("Id"),  nID);
				if (nID == nCtrlID)
				{
					strCtrl = xml.GetAttribute(_T("Value"));

					return strCtrl;
				}


			}
			xml.LeaveElem();
		}  
	}
	xml.LeaveElem();

 return strCtrl;

}

HWND CVisionASMDlg::GetOperationDlgHwnd()
{
	return m_pOperationInfoUiThread->GetOperationDlgPointer()->GetSafeHwnd();
}
void CVisionASMDlg::UpdateLanguageResource()
{
	EnterCriticalSection(&m_CSLanguage);
	if (m_psaSysInfoStrings != NULL)
	{
		if (m_SysOptionConfig.m_lgidLanguage == (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0)))
		{
			return;
		}
		else
		{
			delete m_psaSysInfoStrings;
		}
	}
	m_psaSysInfoStrings = new CStringArray();
	
	TCHAR aModuleFileName[MAX_PATH];
	GetModuleFileName(NULL, aModuleFileName, MAX_PATH);
	CString strModuleFileName(aModuleFileName);
	int nIndex = strModuleFileName.ReverseFind('\\');
	CString strCurWorkDir = strModuleFileName.Left(nIndex);
	
	CString strLanguageFilePath;
	vcXMLConfigurator xml;
	
	switch (m_SysOptionConfig.m_lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strLanguageFilePath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		if (!xml.Load(strLanguageFilePath))
		{
			AfxMessageBox(_T("System file lost or damaged!\n") + strLanguageFilePath);
//			CSplashWnd::HideSplashScreen();
			DestroyWindow();
			exit(ERROR_FILE_NOT_FOUND);
		}
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strLanguageFilePath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		if (!xml.Load(strLanguageFilePath))
		{
			AfxMessageBox(_T("系統文件缺失或損壞！\n") + strLanguageFilePath);
//			CSplashWnd::HideSplashScreen();
			DestroyWindow();
			exit(ERROR_FILE_NOT_FOUND);
		}
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strLanguageFilePath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		if (!xml.Load(strLanguageFilePath))
		{
			AfxMessageBox(_T("系统文件缺失或损坏！\n") + strLanguageFilePath);
//			CSplashWnd::HideSplashScreen();
			DestroyWindow();
			exit(ERROR_FILE_NOT_FOUND);
		}
		break;
	}
	xml.ResetDocElemPos();
	xml.FindElem(_T("ResourceBundle"));
	xml.EnterElem();
	xml.FindElem(_T("StringTable"));
	if(_T("StringTable") == CString(xml.GetAttribute(_T("Key"))))
	{
		xml.EnterElem();
		m_psaSysInfoStrings->RemoveAll();
		m_psaSysInfoStrings->SetSize(300, 5);
		int nID = 0;
		while(xml.FindElem())
		{
			xml.GetAttribute(_T("Id"), nID);
			m_psaSysInfoStrings->SetAtGrow(nID, xml.GetAttribute(_T("Value")));
		}
		xml.LeaveElem();
	}
	xml.LeaveElem();
	m_psaSysInfoStrings->FreeExtra();
	
	CString strLanguage;
	strLanguage.Format(_T("%d"), m_SysOptionConfig.m_lgidLanguage);
	m_psaSysInfoStrings->SetAt(0, strLanguage);

	LeaveCriticalSection(&m_CSLanguage);
}

// void CVisionASMDlg::UpdateLanguageResource()
// {
// 	if (m_psaSysInfoStrings != NULL)
// 	{
// 		if (m_SysOptionConfig.m_lgidLanguage == (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0)))
// 		{
// 			return;
// 		}
// 		else
// 		{
// 			delete m_psaSysInfoStrings;
// 		}
// 	}
// 
// 	m_psaSysInfoStrings = new CStringArray();
// 	m_psaSysInfoStrings->RemoveAll();
// 	
// 	int nSize = STRINGNUM;
// 	m_psaSysInfoStrings->SetSize(nSize, 5);
// 	int nID = 0;
// 	for(int i = 0; i < nSize; i++)
// 	{
// 		nID = _tcstol(g_StringTable[i][0], NULL, 10);
// 		m_psaSysInfoStrings->SetAtGrow(nID, g_StringTable[i][2]);
// 	}
// 
// }

BOOL CVisionASMDlg::DestroyWindow() 
{
//	vcBaseVisionAlign::EndRescanBusThread();

//	AfxMessageBox(_T("BOOL CVisionASMDlg::DestroyWindow()"));


	m_AuthorizeUsbTool.DisableAuthorizeCheck();
	int i=0; 
	for(int i = 0; i < m_vpSuspendResumeWorkThreadObject.size(); i++)
	{
		m_vbExitSuspendResumeThread[i] = TRUE;

		if (WaitForSingleObject((m_vpSuspendResumeWorkThread[i])->m_hThread, WAIT_THREAD_QUIT_TIME) == WAIT_TIMEOUT)
		{
			TerminateThread((m_vpSuspendResumeWorkThread[i])->m_hThread, 0);
		}
		m_vpSuspendResumeWorkThread[i] = NULL;
	}
	DeleteCriticalSection(&m_CSLanguage);
	
	for (i=0; i<m_vpVisionASM.size(); i++)
	{
		if (m_vpVisionASM.at(i) != NULL)
		{
			m_vpVisionASM.at(i)->DestroyWorkThread();
		}
	}


	for (i=0; i<m_vpVisionASM.size(); i++)
	{
		if (m_vpVisionASM.at(i) != NULL)
		{
			delete m_vpVisionASM.at(i);
			m_vpVisionASM.at(i) = NULL;
		}
	}

	
//	AfxMessageBox(_T("CDialog::DestroyWindow()"));
	
// 	if (m_pStatusBar != NULL)
// 	{
// 		delete m_pStatusBar;
// 		m_pStatusBar = NULL;
// 	}

	ExitGetSystemResourceInfo();

	for(int i=0; i<m_vpSuspendResumeWorkThreadObject.size(); i++)
	{
		delete m_vpSuspendResumeWorkThreadObject.at(i);
		m_vpSuspendResumeWorkThreadObject.at(i) = NULL;

	}

	if (m_pDlgVisionView != NULL)
	{
		for (int i=0; i<m_nPlatformNum; i++)
		{
			m_pDlgVisionView[i].DestroyWindow();
		}
		delete[] m_pDlgVisionView;
		m_pDlgVisionView = NULL;
	}

	if (m_pDlgVisionRun != NULL)
	{
		for (int i=0; i<m_nPlatformNum; i++)
		{
			m_pDlgVisionRun[i].DestroyWindow();
		}
		delete[] m_pDlgVisionRun;
		m_pDlgVisionRun = NULL;
	}
	if (m_pOperationInfoUiThread != NULL)
	{
		CDlgOperationInfo* pDlg = m_pOperationInfoUiThread->GetOperationDlgPointer();
		if (pDlg != NULL && pDlg->GetSafeHwnd())
		{
			::PostThreadMessage(m_pOperationInfoUiThread->m_nThreadID, WM_QUIT, 0, 0);
			if(WaitForSingleObject(m_pOperationInfoUiThread->m_hThread, 2000) == WAIT_TIMEOUT)
			{
				TerminateThread(m_pOperationInfoUiThread->m_hThread,0);
				delete m_pOperationInfoUiThread;	
			}
		}
		m_pOperationInfoUiThread = NULL;	
	}
	CString strLog;
	strLog.Format(_T("销毁对位视图窗口成功\n"));
	g_opLogFile.SaveLogText(strLog,0);

	if (m_psaSysInfoStrings != NULL)
	{
		delete m_psaSysInfoStrings;
		m_psaSysInfoStrings = NULL;
	}
	strLog.Format(_T("销毁系统信息成功\n"));
	g_opLogFile.SaveLogText(strLog,0);
// 	if (m_hIconSucceed != NULL)
// 	{
// 		DestroyIcon(m_hIconSucceed);
// 		m_hIconSucceed = NULL;
// 	}
// 	if (m_hIconFailed != NULL)
// 	{
// 		DestroyIcon(m_hIconFailed);
// 		m_hIconFailed = NULL;
// 	}


	if (m_pMainLayout)
	{
		delete m_pMainLayout;
		m_pMainLayout = NULL;
	}

	for (int i = 0; i < m_vpViewLayout.size(); i++)
	{
		if (m_vpViewLayout[i])
		{
			delete m_vpViewLayout[i];
		}
	}
	m_vpViewLayout.clear();


	for (int i = 0; i < m_vpSingleViewLayout.size(); i++)
	{
		if (m_vpSingleViewLayout[i])
		{
			delete m_vpSingleViewLayout[i];
		}
	}
	m_vpSingleViewLayout.clear();

	if (m_hIconLampRunning != NULL)
	{
		DestroyIcon(m_hIconLampRunning);
		m_hIconLampRunning = NULL;
	}
	if (m_hIconLampStopped != NULL)
	{
		DestroyIcon(m_hIconLampStopped);
		m_hIconLampStopped = NULL;
	}
	if (m_hIconLogo != NULL)
	{
		DeleteObject(m_hIconLogo);
		m_hIconLogo = NULL;
	}
	strLog.Format(_T("销毁图标成功\n"));
	g_opLogFile.SaveLogText(strLog,0);

	scPTGrab2::FreeSystem();
	strLog.Format(_T("释放系统成功\n"));
	g_opLogFile.SaveLogText(strLog,0);

	vcBaseVisionAlign::DeleteManualSearchSection();
	strLog.Format(_T("视觉软件退出\n"));
	g_opLogFile.SaveLogText(strLog,0);
	strLog.Format(_T("**************************************************\n"));
	g_opLogFile.SaveLogText(strLog,0);
	
	return CDialog::DestroyWindow();
}

// void CVisionASMDlg::UpdateStatusBar()
// {
// 	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
// 	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
// 	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
// }

void CVisionASMDlg::UpdateInitErrInfoDisplay()
{
	if ((m_vdwInitErrCode.size() < 1) || (m_vdwInitErrCode.size() != m_nPlatformNum))
	{
		return;
	}
	if (m_pDlgVisionView == NULL)
	{
		return;
	}
	
	DWORD dwErrCode = 0;
	for (int i=0; i<m_nPlatformNum; i++)
	{
		dwErrCode = m_vdwInitErrCode.at(i);
		if (dwErrCode == INIT_SUCCEED)
		{
			m_pDlgVisionView[i].UpdateStatusBar(TRUE, NULL, NULL);			
		}
		else if ((dwErrCode & INIT_CAMERA_FAILED) == INIT_CAMERA_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("相机初始化失败"), _T("请检查相机连接"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_INIT_FAILED), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_CHECK_CAMERA_CONNECTION));
		}
		else if ((dwErrCode & INIT_CAMERA_PARAM_FAILED) == INIT_CAMERA_PARAM_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("相机状态设置失败"), _T("请重新设置相机参数"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_STATUS_SET_FAILED), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_SET_CAMERA_STATUS_AGAIN));
		}
		else if ((dwErrCode & INIT_COMMU_FAILED) == INIT_COMMU_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("打开通信接口失败"), _T("请重新设置通信参数"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_COMM_PORT_FAILED), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_SET_COMM_PARAMS_AGAIN));
		}
		else if ((dwErrCode & INIT_VISION_ALIGN_TOOL_FAILED) == INIT_VISION_ALIGN_TOOL_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("当前视觉对位工具初始化失败"), _T("请重新标定，或重新设置当前产品"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_INIT_CUR_VISION_ALIGN_TOOL), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_SET_CALIB_OR_PRODUCT_PARAMS));
		}
		else if ((dwErrCode & INIT_CREATE_THREAD_FAILED) == INIT_CREATE_THREAD_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("工作线程创建失败"), NULL);
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_CREATE_WORK_THREAD), NULL);
		}
		else
		{
			m_pDlgVisionView[i].UpdateStatusBar(TRUE, NULL, NULL);
		}
	}
}

BOOL CVisionASMDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	if (pMsg->message == WM_UPDATE_SINGLE_PLATFORM_INFO)
	{
		int nPlatformIndex = pMsg->wParam;
		UpdateSinglePlatformInfo(nPlatformIndex);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CVisionASMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionASMDlg)
	DDX_Control(pDX, IDC_STATIC_LOGO, m_labelLogo);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_labelTitle);
	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM_ALL, m_nPlatformDispIdx);
	DDX_Control(pDX, IDC_BTN_ALIGNER_RUN, m_btnRun);
	DDX_Control(pDX, IDC_BTN_USER_LOGIN, m_btnLogin);
	DDX_Control(pDX, IDC_BTN_ENVIRONMENT_SET, m_btnEnvironmentSet);
	DDX_Control(pDX, IDC_BTN_PRODUCT_MANAGE, m_btnProductManage);
	DDX_Control(pDX, IDC_BTN_OPTION_SET, m_btnOptionSet);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_BTN_CLOSE_WINDOWS, m_btnShutDownWindows);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM_ALL, m_btnShowPlatformAll);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM5, m_btnShowPlatform5);
	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM6, m_btnShowPlatform6);

	//	DDX_Control(pDX, IDC_BTN_LAMP, m_btnRunLamp);
	//	DDX_Control(pDX, IDC_BTN_CALIBRATION_SET, m_btnCalibrationSet);
	//	DDX_Control(pDX, IDC_BTN_COMMUNICATION_SET, m_btnCommSet);
	//	DDX_Control(pDX, IDC_BTN_SHOW_CTRL_PANNEL, m_btnShowCtrlPannel);
	//	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	// 	DDX_Control(pDX, IDC_TAB_VIEW0, m_tabSheet0);
	// 	DDX_Control(pDX, IDC_TAB_VIEW1, m_tabSheet1);
	// 	DDX_Control(pDX, IDC_TAB_VIEW2, m_tabSheet2);
	// 	DDX_Control(pDX, IDC_TAB_VIEW3, m_tabSheet3);
	// 	DDX_Control(pDX, IDC_TAB_VIEW4, m_tabSheet4);
	// 	DDX_Control(pDX, IDC_TAB_VIEW5, m_tabSheet5);
	//}}AFX_DATA_MAP
	//DDX_Radio(pDX, IDC_RADIO_DISPLAY_POS0, m_nRadioDisplayMode);
	DDX_Control(pDX, IDC_STATIC_TITLE_BAR, m_labelTitleBar);
	DDX_Control(pDX, IDC_BTN_DISPLAY_MODE_SET, m_btnSetDisPlayMode);
}

BEGIN_MESSAGE_MAP(CVisionASMDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionASMDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_USER_LOGIN, OnBtnUserLogin)
	ON_BN_CLICKED(IDC_BTN_ENVIRONMENT_SET, OnBtnEnvironmentSet)
	ON_BN_CLICKED(IDC_BTN_CALIBRATION_SET, OnBtnCalibrationSet)
	ON_BN_CLICKED(IDC_BTN_PRODUCT_MANAGE, OnBtnProductManage)
//	ON_BN_CLICKED(IDC_BTN_COMMUNICATION_SET, OnBtnCommunicationSet)
	ON_BN_CLICKED(IDC_BTN_OPTION_SET, OnBtnOptionSet)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnRadioShowPlatform1)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnRadioShowPlatform2)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnRadioShowPlatform3)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnRadioShowPlatform4)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM5, OnRadioShowPlatform5)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM6, OnRadioShowPlatform6)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM_ALL, OnRadioShowPlatformAll)
	ON_BN_CLICKED(IDC_BTN_ALIGNER_RUN, OnBtnAlignerRun)
	ON_BN_CLICKED(IDC_BTN_CLOSE_WINDOWS, OnBtnShutDownWindows)
	ON_BN_CLICKED(IDC_BTN_SHOW_CTRL_PANNEL, OnBtnShowCtrlPannel)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()

	//}}AFX_MSG_MAP
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS0, &CVisionASMDlg::OnBnClickedRadioDisplayPos0)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS1, &CVisionASMDlg::OnBnClickedRadioDisplayPos1)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS2, &CVisionASMDlg::OnBnClickedRadioDisplayPos2)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS3, &CVisionASMDlg::OnBnClickedRadioDisplayPos3)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS4, &CVisionASMDlg::OnBnClickedRadioDisplayPos4)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS5, &CVisionASMDlg::OnBnClickedRadioDisplayPos5)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS6, &CVisionASMDlg::OnBnClickedRadioDisplayPos6)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS7, &CVisionASMDlg::OnBnClickedRadioDisplayPos7)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS8, &CVisionASMDlg::OnBnClickedRadioDisplayPos8)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS9, &CVisionASMDlg::OnBnClickedRadioDisplayPos9)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS10, &CVisionASMDlg::OnBnClickedRadioDisplayPos10)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS11, &CVisionASMDlg::OnBnClickedRadioDisplayPos11)
// 	ON_BN_CLICKED(IDC_RADIO_DISPLAY_POS12, &CVisionASMDlg::OnBnClickedRadioDisplayPos12)
//	ON_BN_CLICKED(IDC_BTN_SHOW_OTHER_VIEW, &CVisionASMDlg::OnBnClickedBtnShowOtherView)
// 	ON_BN_CLICKED(IDC_BTN_TEST, &CVisionASMDlg::OnBnClickedBtnTest)
// 	ON_BN_CLICKED(IDC_BTN_MANUAL_TARGET_SEARCH2, &CVisionASMDlg::OnBnClickedBtnManualTargetSearch2)
// 	ON_BN_CLICKED(IDC_BTN_MANUAL_OBJECT_SEARCH2, &CVisionASMDlg::OnBnClickedBtnManualObjectSearch2)
ON_BN_CLICKED(IDC_BTN_DISPLAY_MODE_SET, &CVisionASMDlg::OnBnClickedBtnDisplayModeSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionASMDlg message handlers

BOOL CVisionASMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strLog;
	strLog.Format(_T("**************************************************\n"));
	g_opLogFile.SaveLogText(strLog,0);
	strLog.Format(_T("视觉软件启动\n"));
	g_opLogFile.SaveLogText(strLog,0);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// 初始化开始
	CString strInfo;
	SysPlatformInfo locSysPlatformInfo;
	strLog.Format(_T("视觉软件初始化开始\n"));
	g_opLogFile.SaveLogText(strLog,0);

	InitializeCriticalSection(&m_CSLanguage);
	m_nPlatformNum = m_SystemConfigure.GetPlatformNum();
	//设置当前默认布局, 根据平台个数进行默认布局
	m_nViewShowMode = 0;

	
	// 软件启动界面
	strLog.Format(_T("加载基本信息中……\n"));
	m_pSplash->SetText(strLog);
	Sleep(10);

	UpdateLanguageResource();
	UpdateDialogLanguage();
	InitLayoutUI();
	InitUIStyle();
	InitUIData();
	// 枚举相机
	EnumCamera();
	strLog.Format(_T("加载基本信息完成\n"));
	g_opLogFile.SaveLogText(strLog,0);
	m_pSplash->SetText(strLog);
	Sleep(2000);


	strLog.Format(_T("创建平台对位视图中……\n"));
	m_pSplash->SetText(strLog);
	Sleep(10);

	// 初始化运行窗口
	InitRunWindow();
	// 创建视图窗口
	InitViewWindow();
	// 初始化视图布局方式
	InitViewLayout();
	// 初始化单个工位显示
	InitSingleViewLayout();
	// 设置当前工位布局
	m_pCurViewLayout = m_vpViewLayout[m_nViewShowMode];
	strLog.Format(_T("创建平台对位视图完成\n"));
	g_opLogFile.SaveLogText(strLog,0);
	m_pSplash->SetText(strLog);
	Sleep(10);
	// 添加平台共享通信信息，即设置所有的共享通信信息
	InitPlatformCameraShareInfo();

	//// 关闭启动界面
	//CSplashWnd::HideSplashScreen();
	m_pSplash->Hide();
	
	// 创建视觉对位工具	/加载配置文件	 ***************************
	InitPlatformVisionASM();
	strLog.Format(_T("创建视觉对位工具完成\n"));
	g_opLogFile.SaveLogText(strLog,0);



	// 创建操作界面
	InitOperationInfoWindow();
	// 相机初始化检查和相机序列号设置
	InitCameraSerialSetting();
	//如果按照相机数显示, 首先确保相机设置完成后再进行初始化DisplayGUI
	InitPlatformVisionASMGui();
	
	// 界面显示进行初始化调整
	UpdateAllViewLayout();

	strLog.Format(_T("视觉软件初始化完成\n"));
	g_opLogFile.SaveLogText(strLog,0);

	
	//创建开始/停止响应线程 QW
	InitSuspendResumeWorkThread();
	Sleep(200);
	// 开始运行
	OnBtnAlignerRun();
	UpdateInitErrInfoDisplay();
	//// 关闭启动界面
	//CSplashWnd::HideSplashScreen();
	// 初始化授权信息
	InitAuthorize();
	// 初始化完成
	strInfo = "InitDialog End \n";
	AlignToolLogRecord(strInfo, 0);
    //该功能为数据拷贝功能，即把二维码存储文件从存储路径拷贝到上传路径，使用时请相应修改（报错路径的配置）
    if(0)
    {
	    CWinThread* pThreadUpload = ::AfxBeginThread(CVisionASMDlg::ThreadUpload,this);
    }
	
	// 每个工位都进行产品信息和标定信息是否一致检查(临时措施，防止软件崩溃导致的产品、标定索引信息不一致问题)
	if (0)
	{
		CheckProductDataCalibDataConsistency();
	}
	
	
	RECT rc = {0, 0, 0, 0}; 
	if (m_SysOptionConfig.m_bShowFullScreen)
	{
		int width = GetSystemMetrics ( SM_CXSCREEN ); 
		int height= GetSystemMetrics ( SM_CYSCREEN );
		rc.right = width;
		rc.bottom = height;
	}
	else
	{
		SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID) &rc, 0); 
	}
	MoveWindow(&rc, TRUE);
	AfxGetMainWnd()->UpdateWindow();
	InitGetSystemResourceInfo();
//	ShowWindow(SW_MAXIMIZE);
//	SetWindowPos(&wndNoTopMost, 0, 0, 1024, 768, SWP_SHOWWINDOW);
//	CenterWindow(CWnd:: GetDesktopWindow( )); 

//	UpdateLanguageResource();
//	UpdateDialogLanguage();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVisionASMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVisionASMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVisionASMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


int CVisionASMDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// 	// TODO: Add your specialized creation code here
// 	m_pStatusBar=new CStatusBarCtrl;
// 	RECT wndRect;
// 	GetClientRect(&wndRect);
// 	wndRect.top = wndRect.bottom - 20;
// 	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, wndRect, this, IDC_STATUS_BAR_CTRL);
// 	
// 	int nPartWidth[3]= {20, 300, -1};//分割尺寸
// 	m_pStatusBar->SetParts(3, nPartWidth); //分割状态栏
// 	
// 	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
// 	m_hIconSucceed   =(HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16),IMAGE_ICON,16,16,0);   
// 	m_hIconFailed   =(HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16),IMAGE_ICON,16,16,0);   
// 	
// 	UpdateStatusBar();
	
	return 0;
}

void CVisionASMDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pMainLayout)
	{
		m_pMainLayout->OnSize();
	}

	if (m_pCurViewLayout)
	{
	   m_pCurViewLayout->OnSize();
	}

	//MoveDlgItem();
}


void CVisionASMDlg::OnBtnUserLogin() 
{
	CString strText;
	CString strLog,strTmp;
	GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowTextA(strTmp);
	strLog.Format(_T("点击[%s]，进入[管理员登录]\n"),strTmp);
	g_opLogFile.SaveLogText(strLog,1);
//	GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowText(strText);
// 	if (m_bAuthorized)
// 	{
// 		if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_ADMIN_LOGOUT_CONFIRM), MB_YESNO) == IDYES)	//_T("注销管理员登录？")
// 		{
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_LOGIN));	//"管理员登录");
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->EnableWindow(FALSE);
// //			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->EnableWindow(FALSE);
// 			m_bAuthorized = FALSE;
// 
// //			m_btnLogin.SetIcon(IDI_ICON_ADMIN_LOAD_48);
// 		}
// 	}
// 	else
	{
		CDlgSysUserLogin dlgSysUserLogin(this);
		dlgSysUserLogin.SetSysInfoStringsRef(m_psaSysInfoStrings);
		dlgSysUserLogin.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);

		if(m_bPasswordReEnter == FALSE)
		{
			if (m_nUsePassWordMode==0)
			{
				dlgSysUserLogin.m_strPassword = m_strAdminPassword;
			} 
			else if (m_nUsePassWordMode==1)
			{
				dlgSysUserLogin.m_strPassword = m_strAdminPassword2;
			}
		}

		if (dlgSysUserLogin.DoModal() == IDOK)
		{
			//GetDlgItem(IDC_BTN_USER_LOGIN)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_LOGOUT));	//_T("管理员注销")

			m_nUsePassWordMode = dlgSysUserLogin.m_nUsePassWordMode;

			//if (m_nUsePassWordMode == 1)
			{
				//m_nSeconds = 0;
				SetTimer(2,1000,NULL);
			}

			CString strTmp = (m_nUsePassWordMode==1)?GetSysInfoString(m_psaSysInfoStrings,IDS_GENERAL_AUTHORITY):GetSysInfoString(m_psaSysInfoStrings,IDS_SUPER_AUTHORITY);
			m_labelTitle.SetWindowText(m_strTitleShow + strTmp);
			
			COLORREF TmpColor = RGB(54,164,186);
			//m_labelTitle.SetBkColor(RGB(0,0,255)/*,RGB(80,80,200),CLabel::Gradient*/);
			
			m_labelTitle.SetBkColor(TmpColor);

			m_labelTitle.SetTextColor(RGB(255,255,255));
			m_labelTitle.SetFontBold(TRUE);
			int NSize= (m_SystemCompanyInfo.m_nFontSize > 5)?m_SystemCompanyInfo.m_nFontSize:17;
			m_labelTitle.SetFontSize(NSize);
			//if (dlgSysUserLogin.m_nUsePassWordMode == 0)
			m_vpVisionASM.at(0)->SetSystemOptionConfig(&m_SysOptionConfig);
			if (m_nUsePassWordMode == 1 && m_SysOptionConfig.m_bCheckFPMI)
			{
				//
				//m_SysOptionConfig.m_nMinute;
				// 获取时间	
				SYSTEMTIME tm;
				GetLocalTime(&tm);
				//将时间都转化为分钟数，
				int m_StopMin = (tm.wDay-1) * 1440 + tm.wHour * 60 + tm.wMinute;
				//不跨月的情况
				if (m_SysOptionConfig.m_nMinute <= m_StopMin && m_SysOptionConfig.m_nMinute !=-1)
				{
					
					if (m_StopMin - m_SysOptionConfig.m_nMinute>m_SysOptionConfig.m_nMinuteFPMI)
					{
						GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
					}
					else
					{
						GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
					}

				}
				else if ((44640 - m_SysOptionConfig.m_nMinute) + m_StopMin< m_SysOptionConfig.m_nMinuteFPMI)
				{

					GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
				}
				else
				{
				    GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
				}
			}
			else
			{
				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
			}

			
			{
				//GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(TRUE);
				
			}
			/*			else
			{
			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(FALSE);

			}*/			
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
//			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->EnableWindow(TRUE);
			m_bAuthorized = TRUE;			
			
			int i = 0;
			for(i = 0; i < m_nPlatformNum; i++)
			{
				if (m_bAuthorized)
				{
					m_pDlgVisionRun[i].UpdateUserType(m_nUsePassWordMode, TRUE);
				}
				else
				{
					m_pDlgVisionRun[i].UpdateUserType(m_nUsePassWordMode, FALSE);
				}
			}

			//m_btnLogin.SetIcon(IDI_ICON_ADMIN_EXIT_48);
		}
	}

}

void CVisionASMDlg::OnBtnEnvironmentSet() 
{
	EnableDlgItem(FALSE);
	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(m_rcViewRect);
	m_rcViewRect.top -= 1;
	m_rcViewRect.bottom = m_pBtnArea->m_rtLayout.bottom;
//	ScreenToClient(m_rcViewRect);

	CString strLog,strtemp;
	GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowText(strtemp);
	strLog.Format(_T("点击[%s]，进入%s界面\n"),strtemp,strtemp);
	g_opLogFile.SaveLogText(strLog,1);

	// 环境变量设置，配置信息实时生效
	CDlgSysEnvironSetting dlgEnvironSet(this);
	dlgEnvironSet.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgEnvironSet.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	for (int i = 0, nSize = m_vpVisionASM.size(); i < nSize; ++i)
	{
		vcBaseVisionAlign* pVisionASM = m_vpVisionASM.at(i);
		SysPlatformInfo sysPlatformInfo;
		pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bCameraHardTriggerMode)
		{
			pVisionASM->FreezeAllCameras();
		}
	}
	if (dlgEnvironSet.DoModal() == IDOK)
	{
		for (int i=0; i<m_nPlatformNum; i++)
		{
			if (m_vpVisionASM.at(i)->InitAlignTool())
			{
				m_vdwInitErrCode.at(i) &= ~INIT_VISION_ALIGN_TOOL_FAILED;
			}
			else
			{
				m_vdwInitErrCode.at(i) |= INIT_VISION_ALIGN_TOOL_FAILED;
			}
			if (m_SysOptionConfig.m_TarSearchResultCrossSize.m_bShowDefortGui)
			{
				//m_vpVisionASM.at(i)->ShowAllGuiAlnTargetImageMarkPosGraphics(TRUE);
				
			}
			// SY
			//m_vpVisionASM.at(i)->ShowAllGuiTargetImageMarkPosToDisplay(TRUE);

			m_vpVisionASM.at(i)->m_cVisionAlignGui.SetAllPosSearchResult(eTargetSearch);

			m_pDlgVisionRun[i].UpdateDlgParamItem();//更新主界面补偿图示

			m_pDlgVisionRun[i].UpdateDlgProductInfo();//更新主界面产品信息控件

			m_vdwInitErrCode.at(i) &= ~INIT_CAMERA_PARAM_FAILED;
			for (int j=0; j<m_vpVisionASM.at(i)->GetCameraNum(); j++)
			{
				if (!m_vpVisionASM.at(i)->UpdateCameraStatus(j))
				{
					m_vdwInitErrCode.at(i) |= INIT_CAMERA_PARAM_FAILED;
					break;
				}
			}

			// Mali 2013.7.28
            //m_vpVisionASM.at(i)->InitAlignTool();

			m_pDlgVisionView[i].UpdateCrossInfo();
		}
		//UpdateInitErrInfoDisplay();

		if (!m_SysOptionConfig.LoadSystemOption())
		{
			m_SysOptionConfig.m_bSysKeyboardEnabled = TRUE;
			m_SysOptionConfig.SaveSystemOption();
		}
	}

	for (int i = 0, nSize = m_vpVisionASM.size(); i < nSize; ++i)
	{
		vcBaseVisionAlign* pVisionASM = m_vpVisionASM.at(i);
		SysPlatformInfo sysPlatformInfo;
		pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bCameraHardTriggerMode)
		{
			pVisionASM->GrabAllCameras();
		}
	}


	for (int i=0; i<m_nPlatformNum; i++)
	{
		// 判断通信是否开启
		if (!m_vpVisionASM.at(i)->IsCommOpen())
		{
			// 如果没有开启，则重新开启
			if (m_vpVisionASM.at(i)->OpenComm())
			{
				m_vdwInitErrCode.at(i) &= ~INIT_COMMU_FAILED;
			}			
		}
	}
	UpdateInitErrInfoDisplay();

	EnableDlgItem(TRUE);
}

void CVisionASMDlg::OnBtnCalibrationSet() 
{
// 	CDlgSysCalibrateParam dlgCalibrateParam(this);
// 	dlgCalibrateParam.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 	dlgCalibrateParam.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
// 	if (dlgCalibrateParam.DoModal() == IDOK)
// 	{
// 		for (int i=0; i<m_nPlatformNum; i++)
// 		{
// 			if (m_vpVisionASM.at(i)->UpdateCurVisionASMTool())
// 			{
// 				m_vdwInitErrCode.at(i) &= ~INIT_VISION_ALIGN_TOOL_FAILED;
// 			}
// 			else
// 			{
// 				m_vdwInitErrCode.at(i) |= INIT_VISION_ALIGN_TOOL_FAILED;
// 			}
// 
// 			
// // 			if (m_SysOptionConfig.m_TarSearchResultCrossSize.m_bShowDefortGui)
// // 			{
// // 				m_vpVisionASM.at(i)->ShowAllGuiAlnTargetImageMarkPosGraphics(TRUE);
// // 			}
// // 			// SY
// // 			m_vpVisionASM.at(i)->ShowAllGuiTargetImageMarkPosToDisplay(TRUE);
// 			m_vpVisionASM.at(i)->m_cVisionAlignGui.SetAllPosSearchResult(eTargetSearch);
// 		}
// 
// 		UpdateInitErrInfoDisplay();
// 	}
// 	CString strLog;
// 	strLog.Format(_T("点击主界面[调校设置]，进入调校设置界面\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
}

void CVisionASMDlg::OnBtnProductManage() 
{
// 	CDlgSysAlignerParam dlgAlignerParam(this);
// 	dlgAlignerParam.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 	dlgAlignerParam.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
// 	dlgAlignerParam.DoModal();
// 	for (int i=0; i<m_nPlatformNum; i++)
// 	{
// 		if (m_vpVisionASM.at(i)->UpdateCurVisionASMTool(FALSE))
// 		{
// 			m_vdwInitErrCode.at(i) &= ~INIT_VISION_ALIGN_TOOL_FAILED;
// 		}
// 		else
// 		{
// 			m_vdwInitErrCode.at(i) |= INIT_VISION_ALIGN_TOOL_FAILED;
// 		}
// 		m_pDlgVisionView[i].UpdateCurProductAlignInfo();
// 	}
// 	UpdateInitErrInfoDisplay();
	CString strLog;
	strLog.Format(_T("点击[产品设置]，进入产品设置界面\n"));
	g_opLogFile.SaveLogText(strLog,1);
	//strLog.Format(_T("打开[产品浏览]"),);
	//g_opLogFile.SaveLogText(strLog,2);

	EnableDlgItem(FALSE);
// 	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(m_rcViewRect);
// 	m_rcViewRect.top = m_rcViewRect.top -1;

	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(m_rcViewRect);
	m_rcViewRect.top -= 1;
	m_rcViewRect.bottom = m_pBtnArea->m_rtLayout.bottom;

//	ScreenToClient(m_rcViewRect);

//	m_pDlgVisionRun[0].ShowWindow(SW_HIDE);

	CDlgSysKind dlgKind(this);
	dlgKind.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgKind.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	dlgKind.DoModal();
// 	m_pDlgSysKind = new CDlgSysKind(this);
// 	m_pDlgSysKind->Create(IDD_SYS_KIND, this);
// 	m_pDlgSysKind->ShowWindow(SW_SHOW);
	//dlgKind.ShowWindow(SW_SHOW);
	for (int i=0; i<m_nPlatformNum; i++)
	{
		if (m_vpVisionASM.at(i)->UpdateCurVisionASMTool(TRUE, TRUE))
		{
			m_vdwInitErrCode.at(i) &= ~INIT_VISION_ALIGN_TOOL_FAILED;
		}
		else
		{
			m_vdwInitErrCode.at(i) |= INIT_VISION_ALIGN_TOOL_FAILED;
		}
		m_pDlgVisionView[i].SetVisionASMRef(m_vpVisionASM.at(i));
		m_pDlgVisionRun[i].UpdateCurProductAlignInfo();

        
// 		if (m_SysOptionConfig.m_TarSearchResultCrossSize.m_bShowDefortGui)
// 		{
// 			m_vpVisionASM.at(i)->ShowAllGuiAlnTargetImageMarkPosGraphics(TRUE);
// 		}
// 		// SY
// 		m_vpVisionASM.at(i)->ShowAllGuiTargetImageMarkPosToDisplay(TRUE);
		m_vpVisionASM.at(i)->m_cVisionAlignGui.SetAllPosSearchResult(eTargetSearch);
	}

	UpdateInitErrInfoDisplay();
	EnableDlgItem(TRUE);
	//if (m_nUsePassWordMode == 0)
	{
		//2022/4/7
		//GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(TRUE);

			

	}
	//else
	//{
	//	GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(FALSE);

	//}	
}

void CVisionASMDlg::ExecuteSinglePlatformProductManage(int nPlatformIndex)
{
	m_nSinglePlatformIndex = nPlatformIndex;
	SetTimer(1, 10, 0);
}
// 单平台产品管理
void CVisionASMDlg::SinglePlatformProductManage(int nPlatformIndex)
{

	int i = nPlatformIndex;	

	m_vpVisionASM.at(i)->SinglePlatformProductManage(TRUE);
	SysPlatformInfo sysPlatformInfo;
	m_vpVisionASM.at(i)->GetSysPlatformInfo(sysPlatformInfo);
	if (m_bIsRunning)
	{
		m_vpVisionASM.at(i)->StopNow(TRUE);
		for (int j=0; j<m_vpVisionASM.at(i)->GetCameraNum(); j++)
		{
			if (m_vpVisionASM.at(i)->IsCameraOnline(j) && m_vpVisionASM.at(i)->IsCameraGrabbing(j))
			{
				m_vpVisionASM.at(i)->FreezeCamera(j);
			}
		}
		m_vpVisionASM.at(i)->m_bEnableSingleFrameShowMode = FALSE;
		// 触发模式下，修改产品模板需要将相机设置为连续采集模式，完成修改后，恢复相机为触发模式 -- Begin
		if (sysPlatformInfo.m_bCameraHardTriggerMode)
		{
			m_vpVisionASM.at(i)->SetCameraTriggerMode(ePTNotTrigger);
		}
	}

	//EnableDlgItem(FALSE);
	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(m_rcViewRect);
	m_rcViewRect.bottom = m_pBtnArea->m_rtLayout.bottom;
	//	ScreenToClient(m_rcViewRect);

	//	m_pDlgVisionRun[0].ShowWindow(SW_HIDE);

	CDlgSysKind dlgKind(this);
	dlgKind.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgKind.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	dlgKind.m_bSinglePlatformProductManageEnable = TRUE;
	dlgKind.m_nPlatformIndex = i;
	if (dlgKind.DoModal() == IDOK)
	{
		//AfxMessageBox(_T("OK"));
	}
	else
	{
		//AfxMessageBox(_T("Cancel"));
	}


	if (m_vpVisionASM.at(i)->UpdateCurVisionASMTool(TRUE, TRUE))
	{
		m_vdwInitErrCode.at(i) &= ~INIT_VISION_ALIGN_TOOL_FAILED;
	}
	else
	{
		m_vdwInitErrCode.at(i) |= INIT_VISION_ALIGN_TOOL_FAILED;
	}
	WPARAM wParam;
	wParam = i;
	PostMessage(WM_UPDATE_SINGLE_PLATFORM_INFO, wParam, 0);

	if (m_bIsRunning)
	{	
		m_vpVisionASM.at(i)->m_bEnableSingleFrameShowMode = TRUE;
		for (int j=0; j<m_vpVisionASM.at(i)->GetCameraNum(); j++)
		{
			if (!m_vpVisionASM.at(i)->IsCameraGrabbing(j))
			{
				m_vpVisionASM.at(i)->GrabCamera(j);
			}
		}
		m_vpVisionASM.at(i)->StopNow(FALSE);
		// 触发模式下，修改产品模板需要将相机设置为连续采集模式，完成修改后，恢复相机为触发模式 -- End
		if (sysPlatformInfo.m_bCameraHardTriggerMode)
		{
			m_vpVisionASM.at(i)->SetCameraTriggerMode(ePTHardwareTrigger);
		}
	}
	m_vpVisionASM.at(i)->SinglePlatformProductManage(FALSE);

}

void CVisionASMDlg::UpdateSinglePlatformInfo(int nPlatformIndex)
{
	int i = nPlatformIndex;	
	// m_pDlgVisionView[i].UpdateCurProductAlignInfo();
	m_pDlgVisionRun[i].UpdateCurProductAlignInfo();

	DWORD dwErrCode = 0;
	{
		dwErrCode = m_vdwInitErrCode.at(i);
		if (dwErrCode == INIT_SUCCEED)
		{
			m_pDlgVisionView[i].UpdateStatusBar(TRUE, NULL, NULL);			
		}
		else if ((dwErrCode & INIT_CAMERA_FAILED) == INIT_CAMERA_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("相机初始化失败"), _T("请检查相机连接"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_INIT_FAILED), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_CHECK_CAMERA_CONNECTION));
		}
		else if ((dwErrCode & INIT_CAMERA_PARAM_FAILED) == INIT_CAMERA_PARAM_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("相机状态设置失败"), _T("请重新设置相机参数"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_STATUS_SET_FAILED), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_SET_CAMERA_STATUS_AGAIN));
		}
		else if ((dwErrCode & INIT_COMMU_FAILED) == INIT_COMMU_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("打开通信接口失败"), _T("请重新设置通信参数"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_COMM_PORT_FAILED), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_SET_COMM_PARAMS_AGAIN));
		}
		else if ((dwErrCode & INIT_VISION_ALIGN_TOOL_FAILED) == INIT_VISION_ALIGN_TOOL_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("当前视觉对位工具初始化失败"), _T("请重新标定，或重新设置当前产品"));
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_INIT_CUR_VISION_ALIGN_TOOL), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLEASE_SET_CALIB_OR_PRODUCT_PARAMS));
		}
		else if ((dwErrCode & INIT_CREATE_THREAD_FAILED) == INIT_CREATE_THREAD_FAILED)
		{
			//m_pDlgVisionView[i].UpdateStatusBar(FALSE, _T("工作线程创建失败"), NULL);
			m_pDlgVisionView[i].UpdateStatusBar(FALSE, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_CREATE_WORK_THREAD), NULL);
		}
		else
		{
			m_pDlgVisionView[i].UpdateStatusBar(TRUE, NULL, NULL);
		}
	}
}


void CVisionASMDlg::OnBtnCommunicationSet() 
{
	// TODO: Add your control notification handler code here
	CDlgSysCommSetting dlgCommTest(this);
	dlgCommTest.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgCommTest.EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
	if (dlgCommTest.DoModal() == IDOK)
	{
		for (int i=0; i<m_nPlatformNum; i++)
		{
			m_vdwInitErrCode.at(i) &= ~INIT_COMMU_FAILED;
			if (!m_vpVisionASM.at(i)->IsCommOpen())
			{
				if (!m_vpVisionASM.at(i)->OpenComm())
				{
					m_vdwInitErrCode.at(i) |= INIT_COMMU_FAILED;
				}
			}
		}
		UpdateInitErrInfoDisplay();
	}
}

void CVisionASMDlg::OnBtnOptionSet() 
{
	CString strLog;
	strLog.Format(_T("点击[选项]，进入选项界面\n"));
	g_opLogFile.SaveLogText(strLog,1);

	EnableDlgItem(FALSE);
// 	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(m_rcViewRect);
// 	m_rcViewRect.top = m_rcViewRect.top -1;

	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(m_rcViewRect);
	m_rcViewRect.top -= 1;
	m_rcViewRect.bottom = m_pBtnArea->m_rtLayout.bottom;

//	ScreenToClient(m_rcViewRect);
	m_AuthorizeUsbTool.SuspendAuthorizeCheck();

	//CDlgSysOptionSetting dlgSysOptionSetting(this);
	//dlgSysOptionSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
	CDlgSysOption dlgSysOptionSetting(this);
	dlgSysOptionSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);

	LANGID lgidOld = m_SysOptionConfig.m_lgidLanguage;

	//BOOL bShowProductInfo = m_SysOptionConfig.m_bShowProductInfo;
    BOOL bChangeProduct2um = m_SysOptionConfig.m_bChangeProductSize2um;
	BOOL m_bAllSteoage = m_SysOptionConfig.m_bAllSteoage;
	//BOOL bShowOffsetDirection=m_SysOptionConfig.m_bShowOffsetDirection;
    //BOOL bShowPCB = m_SysOptionConfig.m_bShowPCB;
	BOOL bGetSysResourceInfo = m_SysOptionConfig.m_bGetSysResourceUse;

	bool bChangeShowFullScreen = m_SysOptionConfig.m_bShowFullScreen;

	if (dlgSysOptionSetting.DoModal() == IDOK)
	{
		if (dlgSysOptionSetting.m_bSelNewSystem == TRUE)
		{
			EnableDlgItem(FALSE);			
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

			for(int i = 0; i < m_nPlatformNum; i++)
			{
				m_pDlgVisionRun[i].EnableDlgItem(FALSE);			
				m_pDlgVisionRun[i].UpdateUserType(m_nUsePassWordMode, FALSE);
				m_pDlgVisionRun[i].UpdateDlgParamItem();
			}

			return;
		}

		if (!m_SysOptionConfig.LoadSystemOption())
		{
			//m_SysOptionConfig.m_bSysKeyboardEnabled = FALSE;
			m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
			m_SysOptionConfig.SaveSystemOption();
		}

		for (int i=0; i<m_nPlatformNum; i++)
		{
			m_vpVisionASM.at(i)->SetSystemOptionConfig(&m_SysOptionConfig);
			m_vpVisionASM.at(i)->UpdateCommOptionInfo();
		}

		//UpdateManualSearchButton();
		for (int i=0; i<m_nPlatformNum; i++)
		{
			m_pDlgVisionView[i].UpdateManualSearchButton();
			m_pDlgVisionRun[i].UpdateDlgParamItem();
			m_pDlgVisionRun[i].UpdateAlignPrecisionInfo();
//			m_pDlgVisionRun[i].UpdateAlignPrecisionInfo2();
			m_pDlgVisionRun[i].UpdateAlignOffsetInfo();
            m_pDlgVisionRun[i].EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);
            
		}


		// 如果变换了语言选项，则切换语言
		if (m_SysOptionConfig.m_lgidLanguage != lgidOld)
		{
			UpdateLanguageResource();
			for (int i=0; i<m_nPlatformNum; i++)
			{
				m_pDlgVisionView[i].UpdateDialogLanguage();
				m_pDlgVisionRun[i].UpdateDialogLanguage();
				m_pDlgVisionRun[i].UpdateCurProductAlignInfo();
				m_pDlgVisionRun[i].UpdateDlgProductInfo();//更新主界面产品信息控件
				CString strID;
				strID.Format("%d", i+1);	// 平台号从1开始
				m_pDlgVisionView[i].SetPlatformID(GetDlgCtrlString(IDC_STATIC_WS)+strID);
				m_pDlgVisionRun[i].SetPlatformID(strID);
				m_vpVisionASM.at(i)->SetSysInfoStrings(m_psaSysInfoStrings);

                //m_vpVisionASM[i]->SetAllShowGuiObjectImageMarkPos();

				m_vpVisionASM[i]->m_cVisionAlignGui.Changelanguage();

				if(m_pOperationInfoUiThread && m_pOperationInfoUiThread->m_pDlgOperationInfo)
				{
					m_pOperationInfoUiThread->m_pDlgOperationInfo->UpdateDialogLanguage();
					//m_pOperationInfoUiThread->m_pDlgOperationInfo->UpdateCurDlgLanguage();
				}
				
//                 m_vpVisionASM[i]->SetAllShowGuiTargetImageMarkPos();
//                 m_vpVisionASM[i]->SetAllShowGuiVirtualObjectImageMarkPos();
//                 m_vpVisionASM[i]->SetAllShowGuiInspectImageMarkPos();
//                 m_vpVisionASM[i]->SetAllShowGuiCalibTargetImageMarkPos();
			}
			UpdateDialogLanguage();
		}
		
		//if ((m_SysOptionConfig.m_bShowOffsetDirection != bShowOffsetDirection))
		{

			for (int i=0; i<m_nPlatformNum; i++)
			{    
				//m_pDlgVisionRun[i].MoveDlgItem(FALSE);  
                m_pDlgVisionRun[i].UpdateDlgParamItem();
                //m_pDlgVisionRun[i].MoveDlgItem(FALSE); 
                //m_pDlgVisionRun[i].Invalidate();
			}

		}

        if (m_SysOptionConfig.m_bChangeProductSize2um != bChangeProduct2um)
        {

            for (int i=0; i<m_nPlatformNum; i++)
            {

                m_pDlgVisionRun[i].UpdateCurProductAlignInfo();
                  
            }

        }

		if (m_SysOptionConfig.m_bGetSysResourceUse!=bGetSysResourceInfo)
		{
			UpdateGetSystemResourceInfo();
		}

		if (m_SysOptionConfig.m_bShowFullScreen != bChangeShowFullScreen)
		{
			RECT rc = {0, 0, 0, 0}; 
			if (m_SysOptionConfig.m_bShowFullScreen)
			{
				int width = GetSystemMetrics ( SM_CXSCREEN ); 
				int height= GetSystemMetrics ( SM_CYSCREEN );
				rc.right = width;
				rc.bottom = height;
			}
			else
			{
				SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID) &rc, 0); 
			}
			MoveWindow(&rc, TRUE);
		}
	}
	EnableDlgItem(TRUE);

	UpdateInitErrInfoDisplay();

	m_AuthorizeUsbTool.ResumeAuthorizeCheck();
	for (int i=0; i<m_nPlatformNum; i++)
	{
		if (m_pDlgVisionView!=NULL)
		{
			m_pDlgVisionView[i].UpdateShowLanguage();
		}
	}

	// 界面显示进行初始化调整
	UpdateAllViewLayout();
//	InitDlgItemSpecial();

}

void CVisionASMDlg::OnRadioShowPlatform1() 
{
// 	UpdateData(TRUE);
// 	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
// 	{
// 		return;
// 	}
// 
// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 	ScreenToClient(rect);
// 
// 	m_vTabSheet.at(0)->MoveWindow(rect);
// 	CRect rcTabSheet;
// 	m_vTabSheet.at(0)->GetClientRect(rcTabSheet);
// 	rcTabSheet.DeflateRect(2,20,4,4);
// 
// 	m_pDlgVisionView[0].MoveWindow(rcTabSheet);
// 	
// 	for (int i=0; i<m_nPlatformNum; i++)
// 	{
// 		m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		m_pDlgVisionView[i].ShowWindow(SW_HIDE);
// 	}
// 	m_vTabSheet.at(0)->ShowWindow(SW_SHOW);
// 	m_pDlgVisionView[0].ShowWindow(SW_SHOW);
// //	m_pDlgVisionView[0].SetImageDisplayAlignment(2, 2);
// 	m_pDlgVisionView[0].SetAlignerInfoAlignment(5);
// 	
// 	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
	CString strLog;
	strLog.Format(_T("点击主界面[工位1]，显示工位1\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);
	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
	{
		return;
	}
	UpdateViewLayoutShow(m_nPlatformDispIdx);
	m_pDlgVisionView[m_nPlatformDispIdx-1].SetSelPosModeBtnVisible();

	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
}

void CVisionASMDlg::OnRadioShowPlatform2() 
{
// 	UpdateData(TRUE);
// 	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
// 	{
// 		return;
// 	}
// 	
// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 	ScreenToClient(rect);	
// 	m_vTabSheet.at(1)->MoveWindow(rect);
// 	CRect rcTabSheet;
// 	m_vTabSheet.at(1)->GetClientRect(rcTabSheet);
// 	rcTabSheet.DeflateRect(2,20,4,4);
// 	
// 	m_pDlgVisionView[1].MoveWindow(rcTabSheet);
// 	
// 	for (int i=0; i<m_nPlatformNum; i++)
// 	{
// 		m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		m_pDlgVisionView[i].ShowWindow(SW_HIDE);
// 	}
// 	m_vTabSheet.at(1)->ShowWindow(SW_SHOW);
// 	m_pDlgVisionView[1].ShowWindow(SW_SHOW);
// //	m_pDlgVisionView[1].SetImageDisplayAlignment(1, 2);
// 	m_pDlgVisionView[1].SetAlignerInfoAlignment(5);
// 	
// 	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
	CString strLog;
	strLog.Format(_T("点击主界面[工位2]，显示工位2\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);
	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
	{
		return;
	}

	UpdateViewLayoutShow(m_nPlatformDispIdx);
	m_pDlgVisionView[m_nPlatformDispIdx-1].SetSelPosModeBtnVisible();
	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
}

void CVisionASMDlg::OnRadioShowPlatform3() 
{
// 	UpdateData(TRUE);
// 	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
// 	{
// 		return;
// 	}
// 	
// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 	ScreenToClient(rect);	
// 	m_vTabSheet.at(2)->MoveWindow(rect);
// 	CRect rcTabSheet;
// 	m_vTabSheet.at(2)->GetClientRect(rcTabSheet);
// 	rcTabSheet.DeflateRect(2,20,4,4);
// 	
// 	m_pDlgVisionView[2].MoveWindow(rcTabSheet);
// 	
// 	for (int i=0; i<m_nPlatformNum; i++)
// 	{
// 		m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		m_pDlgVisionView[i].ShowWindow(SW_HIDE);
// 	}
// 	m_vTabSheet.at(2)->ShowWindow(SW_SHOW);
// 	m_pDlgVisionView[2].ShowWindow(SW_SHOW);
// //	m_pDlgVisionView[2].SetImageDisplayAlignment(2, 3);
// 	m_pDlgVisionView[2].SetAlignerInfoAlignment(5);
// 	
// 	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
	CString strLog;
	strLog.Format(_T("点击主界面[工位3]，显示工位3\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);
	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
	{
		return;
	}

	UpdateViewLayoutShow(m_nPlatformDispIdx);
	m_pDlgVisionView[m_nPlatformDispIdx-1].SetSelPosModeBtnVisible();
	//ShowVisionView(m_nPlatformDispIdx - 1);

	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
}

void CVisionASMDlg::OnRadioShowPlatform4() 
{
// 	UpdateData(TRUE);
// 	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
// 	{
// 		return;
// 	}
// 	
// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 	ScreenToClient(rect);	
// 	m_vTabSheet.at(3)->MoveWindow(rect);
// 	CRect rcTabSheet;
// 	m_vTabSheet.at(3)->GetClientRect(rcTabSheet);
// 	rcTabSheet.DeflateRect(2,20,4,4);
// 	
// 	m_pDlgVisionView[3].MoveWindow(rcTabSheet);
// 	
// 	for (int i=0; i<m_nPlatformNum; i++)
// 	{
// 		m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		m_pDlgVisionView[i].ShowWindow(SW_HIDE);
// 	}
// 	m_vTabSheet.at(3)->ShowWindow(SW_SHOW);
// 	m_pDlgVisionView[3].ShowWindow(SW_SHOW);
// 	m_pDlgVisionView[3].SetAlignerInfoAlignment(5);
// 	
// 	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
	CString strLog;
	strLog.Format(_T("点击主界面[工位4]，显示工位4\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);
	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
	{
		return;
	}

	UpdateViewLayoutShow(m_nPlatformDispIdx);
	m_pDlgVisionView[m_nPlatformDispIdx-1].SetSelPosModeBtnVisible();
	//ShowVisionView(m_nPlatformDispIdx - 1);

	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
}

void CVisionASMDlg::OnRadioShowPlatform5() 
{
	CString strLog;
	strLog.Format(_T("点击主界面[工位5]，显示工位5\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);
	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
	{
		return;
	}

	UpdateViewLayoutShow(m_nPlatformDispIdx);

	//ShowVisionView(m_nPlatformDispIdx - 1);
	m_pDlgVisionView[m_nPlatformDispIdx-1].SetSelPosModeBtnVisible();

	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
}

void CVisionASMDlg::OnRadioShowPlatform6() 
{
	CString strLog;
	strLog.Format(_T("点击主界面[工位6]，显示工位6\n"));
	g_opLogFile.SaveLogText(strLog,1);
	UpdateData(TRUE);
	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
	{
		return;
	}

	UpdateViewLayoutShow(m_nPlatformDispIdx);
	m_pDlgVisionView[m_nPlatformDispIdx-1].SetSelPosModeBtnVisible();

	//ShowVisionView(m_nPlatformDispIdx - 1);

	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
}

void CVisionASMDlg::OnRadioShowPlatformAll() 
{
// 	UpdateData(TRUE);
// 	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
// 	{
// 		return;
// 	}
// 	
// 	MoveDlgItem();
// 	for (int i=0; i<m_nPlatformNum; i++)
// 	{
// 		m_pDlgVisionView[i].ShowWindow(SW_SHOW);
//  		m_pDlgVisionView[i].SetAlignerInfoAlignment(5);
// 	}
// 
// 	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;GetDlgItem(IDCANCEL)->GetWindowText(strTmp);
	CString strLog,strTmp;
	GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->GetWindowText(strTmp);
	strLog.Format(_T("点击[%s]，显示%s\n"),strTmp,strTmp);
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);
	if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
	{
		return;
	}

	UpdateViewLayoutShow(m_nPlatformDispIdx);

	for (int i = 0; i < m_nPlatformNum; i++)
	{
		m_pDlgVisionView[i].SetAllPosBtnHide();
	}
	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
}

void CVisionASMDlg::OnBtnShowCtrlPannel() 
{
	CString strLog;
	strLog.Format(_T("点击主界面[隐藏设置]，进入隐藏设置界面\n"));
	g_opLogFile.SaveLogText(strLog,1);

	CRect rcView, rcClient, rcItem;
	GetClientRect(rcClient);
	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
	ScreenToClient(rcView);
	int nIndent = rcView.top - rcClient.top;
	int nStatusBar = 20;

	if (m_bShowBottomCtrlPannel)
	{
		rcView.bottom = rcClient.bottom - nIndent;
		GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(rcView);

		GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcItem);
		ScreenToClient(rcItem);
		GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(rcItem.left, rcItem.top, rcItem.Width(), rcClient.bottom-rcItem.top);

// 		GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rcItem);
// 		ScreenToClient(rcItem);
// 		GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(rcItem.left, rcItem.top, rcItem.Width(), rcClient.bottom-rcItem.top);

		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->SetWindowText(GetDlgCtrlString(IDC_BTN_SHOW_CTRL_PANNEL));	//_T("显示控制面板"));
		m_bShowBottomCtrlPannel = FALSE;

//		m_btnShowCtrlPannel.SetIcon(IDI_ICON_SHOW_CTRL_PANNEL_48);
	}
	else
	{
		rcView.bottom = rcClient.bottom - m_nCtrlPannelHeight;
		GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(rcView);

		GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcItem);
		ScreenToClient(rcItem);
		GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(rcItem.left, rcItem.top, rcItem.Width(), rcClient.bottom-rcItem.top);
		
// 		GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rcItem);
// 		ScreenToClient(rcItem);
// 		GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(rcItem.left, rcItem.top, rcItem.Width(), rcClient.bottom-rcItem.top-nStatusBar);
		
		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->SetWindowText(GetDlgCtrlString(IDC_BTN_SHOW_CTRL_PANNEL));	//_T("隐藏控制面板"));
		m_bShowBottomCtrlPannel = TRUE;

//		m_btnShowCtrlPannel.SetIcon(IDI_ICON_HIDE_CTRL_PANNEL_48);
	}
	
	int nShowHide = m_bShowBottomCtrlPannel ? SW_SHOW : SW_HIDE;

	GetDlgItem(IDC_STATIC_SEPARATOR)->ShowWindow(nShowHide);
// 	m_pStatusBar->ShowWindow(nShowHide);

	GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_USER_LOGIN)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->ShowWindow(nShowHide);
//	GetDlgItem(IDC_BTN_COMMUNICATION_SET)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_OPTION_SET)->ShowWindow(nShowHide);
	
	GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(nShowHide);
	GetDlgItem(IDCANCEL)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->ShowWindow(nShowHide);

 	//MoveDlgItem(TRUE);
}
UINT CVisionASMDlg::VisionSuspendResumeWorkThread(LPVOID lpParam)
{
	CSuspendResumeWorkThreadObject* pSuspendResumeWorkThreadObject = (CSuspendResumeWorkThreadObject*)lpParam;
	if (pSuspendResumeWorkThreadObject == NULL)
	{
		return 0;
	}
	CVisionASMDlg* pDlg = (CVisionASMDlg*) pSuspendResumeWorkThreadObject->m_pOwner;
	if (pDlg ==  NULL)
	{
		return 0;
	}
	int i = pSuspendResumeWorkThreadObject->m_nPlatformIndex;
	
	while (FALSE == pDlg->m_vbExitSuspendResumeThread.at(i))
	{
		if (pDlg->m_vbExitSuspendResumeThread.at(i))
			break; // 跳出while循环体
		if (pDlg->m_vbStartSuspendResume.at(i))
		{
			if (pDlg->m_bIsRunning)
			{
				SysPlatformInfo sysPlatformInfo;
				pDlg->m_vpVisionASM.at(i)->GetSysPlatformInfo(sysPlatformInfo);
				if (!sysPlatformInfo.m_bCameraHardTriggerMode)
				{
					for (int j=0; j<pDlg->m_vpVisionASM.at(i)->GetCameraNum(); j++)
					{
						if (pDlg->m_vpVisionASM.at(i)->GetCameraShareType(j) != eShareTar)
						{
							if (pDlg->m_vpVisionASM.at(i)->IsCameraOnline(j) && pDlg->m_vpVisionASM.at(i)->IsCameraGrabbing(j))
							{
								pDlg->m_vpVisionASM.at(i)->FreezeCamera(j);

								CPlatformOptionInfo optionInfo;
								pDlg->m_vpVisionASM.at(i)->GetPlatformOptionInfo(optionInfo);
							
								if (optionInfo.m_bEnableSearchShutter == TRUE)
								{
									// 确保实时采集为环境设置中的曝光时间
									Sleep(10);
									pDlg->m_vpVisionASM.at(i)->UpdateCameraShutter(j);
								}
							}
						}

						
					}
				}
				pDlg->m_vpVisionASM.at(i)->m_bEnableSingleFrameShowMode = FALSE;

				pDlg->m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc = pDlg->m_vpVisionASM.at(i)->SuspendWorkThread();
				{
					CString strLog;
					strLog.Format(_T("工位%d停止工作结果:%d\n"), i+1, pDlg->m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc);
					g_opLogFile.SaveLogText(strLog,1);
				}


				pDlg->m_vbStartSuspendResume.at(i) = FALSE;
			}
			else
			{
				{
					CString strLog;
					strLog.Format(_T("连续采集图像%d开始\n"), i);
					g_opLogFile.SaveLogText(strLog,1);
				}
				pDlg->m_vpVisionASM.at(i)->m_bEnableSingleFrameShowMode = TRUE;

				for (int j=0; j<pDlg->m_vpVisionASM.at(i)->GetCameraNum(); j++)
				{
					if (pDlg->m_vpVisionASM.at(i)->GetCameraShareType(j) != eShareTar)
					{
						if (!pDlg->m_vpVisionASM.at(i)->IsCameraGrabbing(j))
						{
							pDlg->m_vpVisionASM.at(i)->GrabCamera(j);
						}
					}
					
				}

				{
					CString strLog;
					strLog.Format(_T("连续采集图像%d结束\n"), i);
					g_opLogFile.SaveLogText(strLog,1);
				}

				pDlg->m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc = pDlg->m_vpVisionASM.at(i)->ResumeWorkThread();

				{
					CString strLog;
					strLog.Format(_T("工位%d恢复工作结果%d\n"), i+1, pDlg->m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc);
					g_opLogFile.SaveLogText(strLog,1);
				}

				if (pDlg->m_vpVisionASM.at(i)->IsCommOpen())
				{
					pDlg->m_vdwInitErrCode.at(i) &= ~INIT_COMMU_FAILED;
				}
				pDlg->m_vbStartSuspendResume.at(i) = FALSE;
			}
			
		}
		Sleep(10);
	}
	return 0;
}
void CVisionASMDlg::OnBtnAlignerRun() 
{
	BeginWaitCursor();
	BOOL bSucceed = TRUE;
	BOOL bTmp = TRUE;
	// 运行或停止运行默认切换到一般权限
	m_nUsePassWordMode = 1;
	CString strTmp = (m_nUsePassWordMode==1)? GetSysInfoString(m_psaSysInfoStrings,IDS_GENERAL_AUTHORITY):GetSysInfoString(m_psaSysInfoStrings,IDS_SUPER_AUTHORITY);
	m_labelTitle.SetWindowText(m_strTitleShow + strTmp);
	m_labelTitle.SetBkColor(RGB(0,0,255)/*,RGB(80,80,200),CLabel::Gradient*/);
	COLORREF TmpColor = RGB(54,164,186);
	m_labelTitle.SetBkColor(TmpColor);
	m_labelTitle.SetTextColor(RGB(255,255,255));
	m_labelTitle.SetFontBold(TRUE);
	int NSize= (m_SystemCompanyInfo.m_nFontSize > 5)?m_SystemCompanyInfo.m_nFontSize:17;
	m_labelTitle.SetFontSize(NSize);
	strTmp = (m_SysOptionConfig.m_nDisplayMode==1)? _T("帧有效"):_T("实时有效");
	GetDlgItem(IDC_BTN_DISPLAY_MODE_SET)->SetWindowText(strTmp);
	if (m_bIsRunning)
	{
		{
			CString strLog;
			strLog.Format(_T("按下按钮-点击[停止]\n"));
			g_opLogFile.SaveLogText(strLog,1);
		}
		m_bAuthorized = FALSE;
		for(int i = 0; i < m_nPlatformNum; i++)
		{
			m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc = FALSE; 
			m_vbStartSuspendResume.at(i) = TRUE;
		}
		for(int i = 0; i < m_nPlatformNum; i++)
		{
			bTmp &= m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc;
		}
		bSucceed = bTmp;
		scTimer stTimer;
		stTimer.Reset();
		while (!bSucceed)
		{
			if (stTimer.GetTimeMilli(FALSE)>=WAIT_THREAD_SUSPEND_TIME)
				break;
			Sleep(10);
			bTmp = TRUE;
			for (int i=0; i<m_nPlatformNum; i++)
			{
				bTmp &= m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc;
			}
			bSucceed = bTmp;

		}
		//for (int i=0; i<m_nPlatformNum; i++)
		//{
		//	bSucceed &= m_vpVisionASM.at(i)->SuspendWorkThread();
		//	SysPlatformInfo sysPlatformInfo;
		//	m_vpVisionASM.at(i)->GetSysPlatformInfo(sysPlatformInfo);
		//	if (!sysPlatformInfo.m_bCameraHardTriggerMode)
		//	{
		//		for (int j=0; j<m_vpVisionASM.at(i)->GetCameraNum(); j++)
		//		{
		//			if (m_vpVisionASM.at(i)->IsCameraOnline(j) && m_vpVisionASM.at(i)->IsCameraGrabbing(j))
		//			{
		//				m_vpVisionASM.at(i)->FreezeCamera(j);

		//				if (m_SysOptionConfig.m_bEnableSearchShutter == TRUE)
		//				{
		//					// 确保实时采集为环境设置中的曝光时间
		//					Sleep(10);
		//					m_vpVisionASM.at(i)->UpdateCameraShutter(j);
		//				}
		//			}
		//		}
		//	}

		//	m_vpVisionASM.at(i)->m_bEnableSingleFrameShowMode = FALSE;
		//}

		if (bSucceed)
		{
			GetDlgItem(IDC_BTN_ALIGNER_RUN)->SetWindowText(GetDlgCtrlString(IDC_BTN_ALIGNER_RUN));	//_T("运行")
			//m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255, 0, 0), TRUE);
			//m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 0, 0), TRUE);
			//m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(255, 0, 0), TRUE);
			
			COLORREF TmpColor = RGB(201,91,69);
			m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_OUT, TmpColor, TRUE);
			m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_IN, TmpColor, TRUE);
			m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,TmpColor, TRUE);

			m_btnRun.SetIcon(IDI_ICON_RUN1);


			m_bIsRunning = FALSE;
//			m_btnRun.SetIcon(IDI_ICON_RUN_48);

// 			m_btnRunLamp.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255, 255, 0), TRUE);
// 			m_btnRunLamp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 0), TRUE);
// 			m_btnRunLamp.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(255, 255, 0), TRUE);

			int i = 0;
			for(i = 0; i < m_nPlatformNum; i++)
			{
				m_pDlgVisionRun[i].EnableDlgItem(TRUE);
				if (m_bAuthorized)
				{
					m_pDlgVisionRun[i].UpdateUserType(m_nUsePassWordMode, TRUE);
				}
				else
				{
					m_pDlgVisionRun[i].UpdateUserType(m_nUsePassWordMode, FALSE);
				}
			}

			GetDlgItem(IDC_BTN_USER_LOGIN)->EnableWindow(TRUE);
			
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->EnableWindow(TRUE);
			//GetDlgItem(IDC_BTN_REGISTER)->EnableWindow(TRUE);
			if (m_bAuthorized)
			{
				//if (m_nUsePassWordMode == 0)
				{
					GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(TRUE);
				}
				/*else
				{
				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(FALSE);

				}*/
				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
				//				GetDlgItem(IDC_BTN_COMMUNICATION_SET)->EnableWindow(TRUE);
			}


// 			m_bStatusBarFlag = TRUE;
// 			m_strStatusBarInfo1.Empty();
// 			m_strStatusBarInfo2.Empty();
		}
		else
		{
// 			m_bStatusBarFlag = FALSE;
// 			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP);	//_T("停止失败");
// 			UpdateStatusBar();
		}
		CString strLog;
		strLog.Format(_T("点击[停止]完成\n"));
		g_opLogFile.SaveLogText(strLog,1);
	}
	else
	{
		{
			CString strLog;
			strLog.Format(_T("点击[运行]开始\n"));
			g_opLogFile.SaveLogText(strLog,1);
		}

		for(int i = 0; i < m_nPlatformNum; i++)
		{
			m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc = FALSE; 
			m_vbStartSuspendResume.at(i) = TRUE;
		}
		for(int i = 0; i < m_nPlatformNum; i++)
		{
			bTmp &= m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc;
		}
		bSucceed = bTmp;
		scTimer stTimer;
		stTimer.Reset();
		while (!bSucceed)
		{
			if (stTimer.GetTimeMilli(FALSE)>=WAIT_THREAD_SUSPEND_TIME)
				break;
			Sleep(10);
			bTmp = TRUE;
			for (int i=0; i<m_nPlatformNum; i++)
			{
				bTmp &= m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc;
			}
			bSucceed = bTmp;

		}



//		for (int i=0; i<m_nPlatformNum; i++)
//		{
//
//			//m_vpVisionASM.at(i)->m_bEnableSingleFrameShowMode = TRUE;
//
//			//for (int j=0; j<m_vpVisionASM.at(i)->GetCameraNum(); j++)
//			//{
//				//if (!m_vpVisionASM.at(i)->IsCameraGrabbing(j))
//				//{
//					//m_vpVisionASM.at(i)->GrabCamera(j);
//				//}
//			//}
//
//			bSucceed &= m_vpVisionASM.at(i)->ResumeWorkThread();
////			/*bSucceed &= */m_vpVisionASM.at(i)->GrabAllCameras();
//			if (m_vpVisionASM.at(i)->IsCommOpen())
//			{
//				m_vdwInitErrCode.at(i) &= ~INIT_COMMU_FAILED;
//			}
//
//		}

		if (bSucceed)
		{
			GetDlgItem(IDC_BTN_ALIGNER_RUN)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP));	//_T("停止"));
			//m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0, 255, 0), TRUE);
			//m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 255, 0), TRUE);
			//m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0, 255, 0), TRUE);

			COLORREF TmpColor = RGB(54,164,186);
			m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_OUT, TmpColor, TRUE);
			m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_IN, TmpColor, TRUE);
			m_btnRun.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,TmpColor, TRUE);

//			m_btnRun.SetIcon(IDI_ICON_STOP_48);
			m_btnRun.SetIcon(IDI_ICON_STOP1);

			m_bIsRunning = TRUE;

			int i = 0;
			for(i = 0; i < m_nPlatformNum; i++)
			{
				m_pDlgVisionRun[i].EnableDlgItem(FALSE);			
				m_pDlgVisionRun[i].UpdateUserType(m_nUsePassWordMode, FALSE);
				m_pDlgVisionRun[i].UpdateAlignPrecisionInfo();
//				m_pDlgVisionRun[i].UpdateAlignPrecisionInfo2();
				m_pDlgVisionRun[i].UpdateAlignOffsetInfo();
			}
// 
// 			m_btnRunLamp.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0, 255, 0), TRUE);
// 			m_btnRunLamp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 255, 0), TRUE);
// 			m_btnRunLamp.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0, 255, 0), TRUE);

			GetDlgItem(IDC_BTN_USER_LOGIN)->EnableWindow(FALSE);
			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->EnableWindow(FALSE);
			//GetDlgItem(IDC_BTN_REGISTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_CALIBRATION_SET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->EnableWindow(FALSE);
//			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_OPTION_SET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);

// 			m_bStatusBarFlag = TRUE;
// 			m_strStatusBarInfo1.Empty();
// 			m_strStatusBarInfo2.Empty();
// 			UpdateStatusBar();

			for (int i=0; i<m_nPlatformNum; i++)
			{
				m_pDlgVisionView[i].UpdateStatusBar(TRUE, _T(""), _T(""));
			}

		}
		else
		{
// 			m_bStatusBarFlag = FALSE;
// 			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_RUN);	//_T("运行失败");
// 			UpdateStatusBar();
			for (int i=0; i<m_nPlatformNum; i++)
			{
				m_pDlgVisionView[i].UpdateStatusBar(FALSE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_RUN_FAILE)/* _T("开始失败")*/, _T(""));
			}
		}
		CString strLog;
		strLog.Format(_T("点击[运行]完成\n"));
		g_opLogFile.SaveLogText(strLog,1);


		/*for (int i=0; i<m_nPlatformNum; i++)
		{
			{
				CString strLog;
				strLog.Format(_T("连续采集图像%d开始\n"), i);
				g_opLogFile.SaveLogText(strLog,1);
			}
			m_vpVisionASM.at(i)->m_bEnableSingleFrameShowMode = TRUE;

			for (int j=0; j<m_vpVisionASM.at(i)->GetCameraNum(); j++)
			{
				if (!m_vpVisionASM.at(i)->IsCameraGrabbing(j))
				{
					m_vpVisionASM.at(i)->GrabCamera(j);
				}
			}

			{
				CString strLog;
				strLog.Format(_T("连续采集图像%d结束\n"), i);
				g_opLogFile.SaveLogText(strLog,1);
			}
		}*/
	}
	EndWaitCursor();
	
}

void CVisionASMDlg::OnCancel() 
{
	CString strLog,strTmp;
	GetDlgItem(IDCANCEL)->GetWindowText(strTmp);
	strLog.Format(_T("点击[%s]\n"),strTmp);
	g_opLogFile.SaveLogText(strLog,1);
	// TODO: Add extra cleanup here
	if (IDNO == AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_EXIT_SOFTWARE_OR_NOT)/*_T("确定要退出软件吗？")*/, MB_YESNO | MB_SYSTEMMODAL))	//_T("确定要关机吗？")
	{
		strLog.Format(_T("点击[取消]，未退出软件\n"));
		g_opLogFile.SaveLogText(strLog,1);
		return ;
	}
	strLog.Format(_T("确定退出视觉软件\n"));
	g_opLogFile.SaveLogText(strLog,1);

	CDialog::OnCancel();
}

void CVisionASMDlg::OnBtnShutDownWindows() 
{
	CString strLog;
	strLog.Format(_T("点击[关机]键\n"));
	g_opLogFile.SaveLogText(strLog,1);
	if (IDNO == AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SHUT_DOWN_WINDOWS_CONFIRM), MB_YESNO | MB_SYSTEMMODAL))	//_T("确定要关机吗？")
	{
		strLog.Format(_T("点击[取消]，未关机\n"));
		g_opLogFile.SaveLogText(strLog,1);
		return ;
	}
	
	// 关机
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	DWORD dwVersion;
	
	dwVersion = GetVersion();         // 得到WINDOWS NT或Win32的版本号
	
	if(dwVersion < 0x80000000)        // 用于判断WIN系列，从而设置相应的权限
	{
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
		ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
	}
	else
	{
		ExitWindowsEx(EWX_FORCE | EWX_SHUTDOWN, 0);
	}
	strLog.Format(_T("关闭计算机\n"));
	g_opLogFile.SaveLogText(strLog,1);
}






BOOL CVisionASMDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}




void CVisionASMDlg::AddTitleWarningMsg(CString strMsg)
{

	CString strTitle;
	// 3空格占位
	if (m_hIconLogo == NULL)
	{
		strTitle = _T("");
	}
	else
	{
		strTitle = _T("   ");
	}

	// 公司名称
	if (m_SystemCompanyInfo.m_strCompanyTitle.IsEmpty())
	{		
	}
	else
	{
		strTitle += m_SystemCompanyInfo.m_strCompanyTitle;
	}

	// 软件名称
	if (m_SystemCompanyInfo.m_strSoftwareTitle.IsEmpty())
	{
		strTitle += GetSysInfoString(m_psaSysInfoStrings,IDS_MB_STR_SOFTWARE_TITLE);
	}
	else
	{
		strTitle += m_SystemCompanyInfo.m_strSoftwareTitle;
	}

	// 5空格占位
	strTitle += _T("  ");

	if (strMsg.GetLength()==0)
	{
// 		// 版本日期
// 		if (m_SystemCompanyInfo.m_strVersionDate.IsEmpty())
// 		{		
// 		}
// 		else
// 		{
// 			strTitle += m_SystemCompanyInfo.m_strVersionDate;
// 		}
// 
// 		// 软件注释说明
// 		if (m_SystemCompanyInfo.m_strCompanyNote.IsEmpty())
// 		{		
// 		}
// 		else
// 		{
// 			strTitle += m_SystemCompanyInfo.m_strCompanyNote;
// 		}

		// 版本前空格占位
		if (m_SystemCompanyInfo.m_strVersionDateFront.IsEmpty())
		{		
		}
		else
		{
			strTitle += m_SystemCompanyInfo.m_strVersionDateFront;
		}

		// 版本日期
		if (m_SystemCompanyInfo.m_bVersionDateCustomEnable)
		{		
			strTitle += m_SystemCompanyInfo.m_strVersionDate;		
		}
		else
		{
			//		strTitle += GetSysInfoString(m_psaSysInfoStrings,IDS_MB_STR_VERSION_DATE) + m_SystemConfigure.GetSoftwareVersion();	//hk20180130
			strTitle += m_SystemConfigure.GetSoftwareVersion();
		}

		// 版本后空格占位
		if (m_SystemCompanyInfo.m_strVersionDateBack.IsEmpty())
		{		
		}
		else
		{
			strTitle += m_SystemCompanyInfo.m_strVersionDateBack;
		}

		// 软件注释说明
		if (m_SystemCompanyInfo.m_strCompanyNote.IsEmpty())
		{		
		}
		else
		{
			strTitle += m_SystemCompanyInfo.m_strCompanyNote;
		}
	}
	else
	{
//加两个空格空位
	//strTitle +=_T("  ");
	strTitle +=strMsg; 
	}

	CString strTmp = (m_nUsePassWordMode==1)? GetSysInfoString(m_psaSysInfoStrings,IDS_GENERAL_AUTHORITY):GetSysInfoString(m_psaSysInfoStrings,IDS_SUPER_AUTHORITY);
	m_labelTitle.SetWindowText(strTitle + strTmp);
	m_labelTitle.SetBkColor(RGB(0,0,255)/*,RGB(80,80,200),CLabel::Gradient*/);
	COLORREF TmpColor = RGB(54,164,186);
	m_labelTitle.SetBkColor(TmpColor);
	m_labelTitle.SetTextColor(RGB(255,255,255));
	m_labelTitle.SetFontBold(TRUE);
	int NSize= (m_SystemCompanyInfo.m_nFontSize > 5)?m_SystemCompanyInfo.m_nFontSize:17;
	m_labelTitle.SetFontSize(NSize);
}

void CVisionASMDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 1)
	{
		KillTimer(1);
		SinglePlatformProductManage(m_nSinglePlatformIndex);
	}
	else if(nIDEvent == 2)
	{
		m_nSeconds ++;

		// 在设定m_nSecondsMax = 60s 内; 无需再次输入登陆密码；超过后登陆框密码
		if (m_nSeconds<= m_SysOptionConfig.m_nLoginTimeSet)
		{
			m_bPasswordReEnter = FALSE;
		}
		else
		{
			m_nSeconds = 0;
			m_bPasswordReEnter = TRUE;
			KillTimer(2);
		}
		// 超过一定时间后，默认退出 免重新输入密码状态
		if(m_nSeconds >m_SysOptionConfig.m_nLoginTimeSet*5 && m_nSeconds < INT_MAX -1)
		{
			m_nSeconds = 0;
			m_bPasswordReEnter = TRUE;
			KillTimer(2);
		}

	}
	CDialog::OnTimer(nIDEvent);
}


LRESULT CVisionASMDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_AUTHORIZE_VALID:
		{
			CString strmsg;
			strmsg = _T("");
			AddTitleWarningMsg(strmsg);

			SysPlatformInfo locSysPlatformInfo;
			for (int i=0; i<m_nPlatformNum; i++)
			{
				m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = TRUE;
				m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
		}
		break;
	case WM_UPDATE_AUTHORIZE_EXPIRE:
		{
			CString strmsg;
			strmsg= GetSysInfoString(m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE);
			AddTitleWarningMsg(strmsg);

			SysPlatformInfo locSysPlatformInfo;
			for (int i=0; i<m_nPlatformNum; i++)
			{
				m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
		}
		break;
	case WM_UPDATE_AUTHORIZE_WARNING:
		{
			CString strmsg;
			strmsg.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_EXPIRE_DATE_COMMING),wParam);
			AddTitleWarningMsg(strmsg);

			SysPlatformInfo locSysPlatformInfo;
			for (int i=0; i<m_nPlatformNum; i++)
			{
				m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = TRUE;
				m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
		}
		break;
	case WM_UPDATE_AUTHORIZE_READ_ERROR:
	case WM_UPDATE_AUTHORIZE_WRITE_ERROR:
	case WM_UPDATE_AUTHORIZE_LOST_INFO:
	case WM_UPDATE_AUTHORIZE_EXCEPTION:
	case WM_UPDATE_AUTHORIZE_FORMAT_ERROR:
		{
			CString strmsg;
			strmsg= GetSysInfoString(m_psaSysInfoStrings,IDS_STR_REGISTER_INFO_LOST);
			AddTitleWarningMsg(strmsg);

			SysPlatformInfo locSysPlatformInfo;
			for (int i=0; i<m_nPlatformNum; i++)
			{
				m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
				locSysPlatformInfo.m_bRegisterSuccess = FALSE;
				m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
				m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
			}
		}
		break;
	case WM_CHANGEPLATFORM:
		{
			m_nPlatformDispIdx = wParam;
			if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
			{
				break;
			}
			UpdateViewLayoutShow(m_nPlatformDispIdx);
			if (m_nPlatformDispIdx > 0)
			{
				m_pDlgVisionView[m_nPlatformDispIdx-1].SetSelPosModeBtnVisible();
			}
			
			m_pDlgVisionView[m_nPrevDispPltfrmIdx].SetAllPosBtnHide();
			
			m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;

// 
// 			if (0 == wParam)
// 			{
// 				
// 				;
// 			}
// 			else if (1 == wParam)
// 			{
// 				if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
// 				{
// 					break;
// 				}
// 
// 				ShowVisionView(m_nPlatformDispIdx - 1);
// 
// 				m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
// 			}
// 			else if(2==wParam)
// 			{
// 				if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
// 				{
// 					break;
// 				}
// 
// 				ShowVisionView(m_nPlatformDispIdx - 1);
// 
// 				m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
// 			}
//             else if(3==wParam)
//             {
//                 if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
//                 {
//                     break;
//                 }
// 
//                 ShowVisionView(m_nPlatformDispIdx - 1);
// 
//                 m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
//             }
//             else if(4==wParam)
//             {
//                 if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
//                 {
//                     break;
//                 }
// 
//                 ShowVisionView(m_nPlatformDispIdx - 1);
// 
//                 m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
//             }
//             else if(5==wParam)
//             {
//                 if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
//                 {
//                     break;
//                 }
// 
//                 ShowVisionView(m_nPlatformDispIdx - 1);
// 
//                 m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
//             }
//             else if(6==wParam)
//             {
//                 if (m_nPlatformDispIdx == m_nPrevDispPltfrmIdx)
//                 {
//                     break;
//                 }
// 
//                 ShowVisionView(m_nPlatformDispIdx - 1);
// 
//                 m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
//             }
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CVisionASMDlg::UpdateGetSystemResourceInfo()
{
	if (m_SysOptionConfig.m_bGetSysResourceUse)
	{
		//CRect rc;
		//GetWindowRect(rc);
		//m_SystemStatusInfo.BeginMonitor(this,rc);
		////m_SystemStatusInfo.ResumeMonitor();
		m_SystemStatusInfo.ShowUIWindow(TRUE);
	}
	else
	{
		//m_SystemStatusInfo.StopMonitor();
		////m_SystemStatusInfo.SuspendMonitor();
		m_SystemStatusInfo.ShowUIWindow(FALSE);
	}
}

void CVisionASMDlg::InitGetSystemResourceInfo()
{
	//if (m_SysOptionConfig.m_bGetSysResourceUse)
	//{
		CRect rc;
		GetWindowRect(rc);
		rc.right -= 400;
		m_SystemStatusInfo.BeginMonitor(this,rc);
	//}
	
	UpdateGetSystemResourceInfo();
}

void CVisionASMDlg::ExitGetSystemResourceInfo()
{
	m_SystemStatusInfo.StopMonitor();
}

void CVisionASMDlg::InitCameraSerialSetting()
{
	CString strLog;
	if (m_SysOptionConfig.m_bStartUpShowCamSNSetting)
	{
		//
		for (int i=0; i<m_nPlatformNum; i++)
		{
			int nCamNum = m_vpVisionASM.at(i)->GetCameraNum();
			for (int j=0; j<nCamNum; j++)
			{
				m_vpVisionASM.at(i)->DestroyCamera(j);
			}
		}
		if (!CameraSerialNumConfig(&m_vdwInitErrCode))	// 如果用户点击取消，则系统运行在离线状态
		{
			for (int i=0; i<m_nPlatformNum; i++)
			{
				int nCamNum = m_vpVisionASM.at(i)->GetCameraNum();
				for (int j=0; j<nCamNum; j++)
				{
					m_vpVisionASM.at(i)->DestroyCamera(j);
				}
			}
		}
	}
	else
	{
		for (int i=0; i<m_nPlatformNum; i++)
		{
			if ((m_vdwInitErrCode.at(i) & INIT_CAMERA_FAILED) == INIT_CAMERA_FAILED)
			{
				for (int k=0; k<m_nPlatformNum; k++)
				{
					int nCamNum = m_vpVisionASM.at(k)->GetCameraNum();
					for (int j=0; j<nCamNum; j++)
					{
						m_vpVisionASM.at(k)->DestroyCamera(j);
					}
				}

				if (!CameraSerialNumConfig(&m_vdwInitErrCode))	// 如果用户点击取消，则系统运行在离线状态
				{
					for (int i=0; i<m_nPlatformNum; i++)
					{
						int nCamNum = m_vpVisionASM.at(i)->GetCameraNum();
						for (int j=0; j<nCamNum; j++)
						{
							m_vpVisionASM.at(i)->DestroyCamera(j);
						}
					}
				}
				break;
			}
		}
	}

	for (int i=0;i<m_vpVisionASM.size();i++)
	{
		m_vpVisionASM.at(i)->UpdateShareCamTransferSame();
	}

	strLog.Format(_T("相机初始化检查和相机序列号设置完成\n"));
	g_opLogFile.SaveLogText(strLog,0);
}

BOOL CVisionASMDlg::InitSuspendResumeWorkThread()
{
	CSuspendResumeWorkThreadObject* pSuspendResumeWorkThreadObject = NULL;
	for(int i = 0; i < m_nPlatformNum; i++)
	{		
		pSuspendResumeWorkThreadObject = new CSuspendResumeWorkThreadObject;
		pSuspendResumeWorkThreadObject->m_nPlatformIndex = i;
		pSuspendResumeWorkThreadObject->m_pOwner = this;
		m_vpSuspendResumeWorkThreadObject.push_back(pSuspendResumeWorkThreadObject);
		m_vbStartSuspendResume.push_back(FALSE);
	}


	CString strLog;
	BOOL bExitSuspendResumeWorkThread;  // 搜素结果线程是否退出
	CWinThread*	pSuspendResumeWorkThread = NULL;  
	m_vbExitSuspendResumeThread.resize(m_nPlatformNum, FALSE);
	m_vpSuspendResumeWorkThread.clear();
	for (int i=0; i<m_nPlatformNum; i++)
	{
		m_vpVisionASM.at(i)->SuspendResumeWorkThreadSucc = FALSE;

		bExitSuspendResumeWorkThread = FALSE; 

		pSuspendResumeWorkThread = NULL;  
		pSuspendResumeWorkThread = AfxBeginThread(VisionSuspendResumeWorkThread,  // 平台工作线程执行函数
			m_vpSuspendResumeWorkThreadObject[i],	// 传入线程的参数
			THREAD_PRIORITY_NORMAL,					// 线程的优先级,一般设置为 0 .让它和主线程具有共同的优先级
			0,										// 指定新创建的线程的栈的大小.如果为 0,新创建的线程具有和主线程一样的大小的栈 
			0,										// CREATE_SUSPENDED : 线程创建以后,会处于挂起状态,直到调用:ResumeThread；0：线程创建后立即运行
			NULL									// 指向一个 SECURITY_ATTRIBUTES 的结构体,用它来标志新创建线程的安全性.如果为 NULL，那么新创建的线程就具有和主线程一样的安全性
			);

		m_vpSuspendResumeWorkThread.push_back(pSuspendResumeWorkThread);
		if (NULL == pSuspendResumeWorkThread)
		{
			strLog.Format(_T("开始停止控制线程创建失败\n"));
			g_opLogFile.SaveLogText(strLog,0);
			return FALSE;
		}

		if (pSuspendResumeWorkThread->m_hThread == INVALID_HANDLE_VALUE)
		{
			strLog.Format(_T("开始停止控制线程创建失败1\n"));
			g_opLogFile.SaveLogText(strLog,0);
			return FALSE;
		}
	}

	return TRUE;
}

void CVisionASMDlg::InitPlatformCameraShareInfo()
{
	SysPlatformInfo locSysPlatformInfo;
	vcBaseVisionAlign::m_ShareCommInfoArray.ClearPlatShareCommInfo();
	for (int i=0; i<m_nPlatformNum; i++)
	{
		m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
		if(locSysPlatformInfo.m_bShareCommEnable)
		{
			vcBaseVisionAlign::AddPlatShareComm(
				locSysPlatformInfo.m_nPlatformIndex, locSysPlatformInfo.m_nShareCommIndex, locSysPlatformInfo.m_bShareCommEnable);
		}
	}
}

void CVisionASMDlg::InitPlatformVisionASM()
{
	m_vdwInitErrCode.resize(m_nPlatformNum, 0);
	SysPlatformInfo locSysPlatformInfo;
	for (int i=0; i<m_nPlatformNum; i++)
	{
		m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
		vcBaseVisionAlign *pVisionASM = NULL;
		switch (locSysPlatformInfo.m_ePlatformType)
		{
		case ePlatformXYD:
			pVisionASM = new vcXYDVisionAlign();
			break;
// 		case ePlatformXYPD:
// 			pVisionASM = new vcXYDVisionAlign();
// 			break;
// 		case ePlatformX1X2Y:
// 			pVisionASM = new vcX1X2YVisionAlign();
// 			break;
// 		case ePlatformXY1Y2:
// 			pVisionASM = new vcXY1Y2VisionAlign();
// 			break;
// 		case ePlatformYX1X2PD:
// 			pVisionASM = new vcYX1X2PDVisionAlign();
// 			break;
// 		case ePlatformYDPX1X2:
// 			pVisionASM = new vcYDPX1X2VisionAlign();
// 			break;
		default:
			pVisionASM = new vcXYDVisionAlign();
			break;
		}

		if (pVisionASM != NULL)
		{
			pVisionASM->SetSystemOptionConfig(&m_SysOptionConfig);
			pVisionASM->SetSysPlatformInfo(locSysPlatformInfo);
			pVisionASM->SetSysInfoStrings(m_psaSysInfoStrings);
			pVisionASM->SetSysStatusInfo(&m_SystemStatusInfo);

			pVisionASM->SetMainWnd(&m_pDlgVisionView[i]);
			pVisionASM->SetMainRunWnd(&m_pDlgVisionRun[i]);
			pVisionASM->SetMainFrameWnd(this);

// 			if (m_pOperationInfoUiThread->GetOperationDlgPointer() != NULL)
// 			{
// 				pVisionASM->SetOperatingInfoShowWnd(m_pOperationInfoUiThread->GetOperationDlgPointer());	//zzc 运行信息窗口
// 			}

			// 共享通信，将源通信接口的对位流程VisionAlign设置到共享接口的对位流程VisionAlign
			if (eShareCommTar == vcBaseVisionAlign::m_ShareCommInfoArray.GetShareCommType(locSysPlatformInfo.m_nPlatformIndex))
			{			
				int nShareCommSrcPlatformIndex = vcBaseVisionAlign::m_ShareCommInfoArray.GetShareCommSrcPlatformIndex(locSysPlatformInfo.m_nPlatformIndex);
				if (nShareCommSrcPlatformIndex >= 0 && nShareCommSrcPlatformIndex < m_vpVisionASM.size())
				{
					pVisionASM->SetVisionAlignOther(m_vpVisionASM.at(nShareCommSrcPlatformIndex));
				}				
			}

			if (!pVisionASM->LoadVisionASMConfig())
			{
				// 关闭启动界面
				//CSplashWnd::HideSplashScreen();

				CString strInfo;
				strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_LOAD_VISION_ALIGN_TOOL));
				AfxMessageBox(strInfo,MB_SYSTEMMODAL);
			}

			MotionControlType TmpMotionControlType = eMCNo;
			CVisionASMConfig *vsConfig;
			pVisionASM->GetVSconfig(vsConfig);
			if (NULL != vsConfig)
			{
				TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
			}

			if ((TmpMotionControlType == eMCYamahaRobot || TmpMotionControlType == eMCYamahaRobot340) 
				&& !pVisionASM->LoadRobotTeachRangeInfo())
			{
				CString strInfo;
				strInfo.Format(_T("加载机器人示教范围信息失败！"));
				//AfxMessageBox(strInfo);
				//pVisionASM->SaveRobotTeachRangeInfo();
			}

			if (!pVisionASM->LoadTotalCalibDataInfo())
			{
				if(!pVisionASM->LoadTotalCalibDataInfoOld())
				{
					// 关闭启动界面
					//CSplashWnd::HideSplashScreen();

					CString strInfo;
					strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), i+1, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_T0_LOAD_PRODUCT_INFO));
					AfxMessageBox(strInfo,MB_SYSTEMMODAL);
				}
			}

			if (!pVisionASM->LoadTotalProductDataInfo())
			{
				if(!pVisionASM->LoadTotalProductDataInfoOld())
				{
					// 关闭启动界面
					//CSplashWnd::HideSplashScreen();

					CString strInfo;
					strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), i+1, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_T0_LOAD_PRODUCT_INFO));
					AfxMessageBox(strInfo,MB_SYSTEMMODAL);
				}
			}

			m_vdwInitErrCode.at(i) = pVisionASM->InitVisionASM();
		}
		m_vpVisionASM.push_back(pVisionASM);
	}// End for
}

void CVisionASMDlg::InitOperationInfoWindow()
{
	m_pOperationInfoUiThread = new COperationInfoUiThread;		//zzc 线程创建运行信息窗口
	m_pOperationInfoUiThread->SetParentWndPointer(this);		//对话框设置ASMDlg为父窗口
	for (int i = 0; i < m_nPlatformNum; i++)
	{
		m_vpVisionASM[i]->SetOperatingInfoShowWnd(m_pOperationInfoUiThread->GetOperationDlgPointer());
	}

	m_pOperationInfoUiThread->SetVisionASMRef(m_vpVisionASM);			//zzc设置视觉指针，可确定读取的文件路径
	m_pOperationInfoUiThread->SetPlatformNum(m_nPlatformNum);			//设置平台数量，通过平台数量，可确定如何初始化
	m_pOperationInfoUiThread->SetCurrentWorkPath(m_strCurWorkDir);		//设置当前执行路径，为了找到语言文件
	m_pOperationInfoUiThread->CreateThread();
}

void CVisionASMDlg::InitPlatformVisionASMGui()
{
	for (int i=0; i<m_nPlatformNum; i++)
	{
		vcBaseVisionAlign *pVisionASM = m_vpVisionASM.at(i);

		CPlatformLayoutInfo layoutInfo;
		pVisionASM->GetPlatformLayoutInfo(layoutInfo);

		m_pDlgVisionView[i].SetVisionASMRef(pVisionASM);
		m_pDlgVisionView[i].SetVisionRunWidth(layoutInfo.m_nViewVisionRunWidth);
		m_pDlgVisionView[i].SetViewLayoutType(layoutInfo.m_nViewLayoutType);
		m_pDlgVisionView[i].SetCurMainLayoutIndex(layoutInfo.m_nViewMainLayoutType);
		m_pDlgVisionView[i].UpdateViewLayout();

		m_pDlgVisionRun[i].SetVisionASMRef(pVisionASM);
		m_pDlgVisionRun[i].UpdateDlgProductInfo();

		// GUI
		pVisionASM->m_cVisionAlignGui.SetVisionRef(pVisionASM);
		pVisionASM->m_cVisionAlignGui.SetVisionDisplay();
		pVisionASM->m_cVisionAlignGui.Changelanguage();

// 		pVisionASM->m_cVisionAlignGui.SetAllPosSearchResult(eObjectSearch);
// 		pVisionASM->m_cVisionAlignGui.SetAllPosSearchResult(eTargetSearch);


		// 关联/标定GUI
		pVisionASM->AddAllGuiCalibrateGraphicsToDisplay();
		pVisionASM->InitAlignerSizeInfo();
	}
}

void CVisionASMDlg::InitAuthorize()
{
	SysPlatformInfo locSysPlatformInfo;
	m_SystemConfigure.GetSysPlatformInfo(0, locSysPlatformInfo);
	int	nWarningDayNumber = 20;			// 警告天数
	m_AuthorizeUsbTool.SetOwner(this);
	m_AuthorizeUsbTool.SetWarningDayNumber(nWarningDayNumber);

	CString strRegDate;
	AuthorizeStatus status = m_AuthorizeUsbTool.CheckAuthorize(strRegDate);
	BOOL bRegisterSuccess = FALSE;
	if ( (status == eAuthorizeValid) || (status == eAuthorizeWarning) )
	{
		bRegisterSuccess = TRUE;
	}
	else
	{
		bRegisterSuccess = FALSE;
	}
	for (int i=0; i<m_nPlatformNum; i++)
	{
		m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);
		locSysPlatformInfo.m_bRegisterSuccess = bRegisterSuccess;
		m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
		m_vpVisionASM.at(i)->SetSysPlatformInfo(locSysPlatformInfo);
	}

	m_AuthorizeUsbTool.EnableAuthorizeCheck();
}

UINT CVisionASMDlg::ThreadUpload(LPVOID lpParam)
{
	CVisionASMDlg* pDlg = (CVisionASMDlg*)lpParam;
	CString strSavePath;
	CString strUploadPath;
	CString strTmpFilePath;
	CString strWorkType;
	int nTimeInterval;
	int nSeconds = 0;
	SYSTEMTIME sys;
	CString strLogPath;
	CString strRecord;
	CString strTime;
	strLogPath.Format(_T("D:\\UploadLog.txt"));
	FILE *fileLog;
	while(1)
	{
		pDlg->m_vpVisionASM.at(0)->LoadQRCodeOption();

		strUploadPath = pDlg->m_vpVisionASM.at(0)->m_stUploadData.strUploadPath;
		strTmpFilePath =strUploadPath;
		strSavePath = pDlg->m_vpVisionASM.at(0)->m_stUploadData.strSavePath;
		strWorkType = pDlg->m_vpVisionASM.at(0)->m_stUploadData.strWorkType;
		nTimeInterval = pDlg->m_vpVisionASM.at(0)->m_stUploadData.nTimeInterval;

		CFileFind fileFind;
		BOOL bFind;
		BOOL bDir;
		bFind = FALSE;
		bDir = FALSE;
		bFind = fileFind.FindFile(strUploadPath);
		while(bFind)
		{
			bFind = fileFind.FindNextFile();
			bDir = fileFind.IsDirectory();
			if(bDir) break;
		}
		if(!bDir)
		{
			if(!CreateDirectory(strUploadPath,NULL))
			{
				fileLog = fopen(strLogPath,_T("a+t"));
				GetLocalTime(&sys);
				strTime.Format(_T("[%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
				strRecord.Format(_T("%s 创建文件夹失败\n"),strTime);
				fwrite(strRecord,sizeof(TCHAR),strRecord.GetLength(),fileLog);
				fclose(fileLog);
			}
		}

		if (nTimeInterval<=5)     //如果时间设置成0，则1秒上传一次
		{
			nSeconds = 5 * 60 * 1000;
		} 
		else if (nTimeInterval>=10)
		{
			nSeconds = 10 * 60 * 1000;
		}
		else
		{
			nSeconds = nTimeInterval * 60 * 1000;
		}
		Sleep(nSeconds);
		GetLocalTime(&sys);
		CString str;
		str.Format(_T("\\%s-2DCodeRecord.csv"),strWorkType);
		strSavePath += str;
		str.Empty();
		str.Format(_T("\\%s-%04d%02d%02d%02d%02d%02d"),strWorkType,sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
		strTmpFilePath += str + _T(".tmp");
		strUploadPath += str + _T(".csv");
		str.Empty();

		EnterCriticalSection(&pDlg->criticalUpload);

		if(PathFileExists(strSavePath))
		{
			if(rename(strSavePath,strTmpFilePath) == 0)
			{
				fileLog = fopen(strLogPath,_T("a+t"));
				GetLocalTime(&sys);
				strTime.Format(_T("[%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
				strRecord.Format(_T("%s strSavePath :%s,===========>strTmpFilePath :%s\n"),strTime,strSavePath,strTmpFilePath);
				fwrite(strRecord,sizeof(TCHAR),strRecord.GetLength(),fileLog);
				fclose(fileLog);
			}
			else
			{
				fileLog = fopen(strLogPath,_T("a+t"));
				GetLocalTime(&sys);
				strTime.Format(_T("[%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
				strRecord.Format(_T("%s 移动文件失败\n"),strTime);
				fwrite(strRecord,sizeof(TCHAR),strRecord.GetLength(),fileLog);
				fclose(fileLog);
			}
			if(rename(strTmpFilePath,strUploadPath) == 0)
			{
				fileLog = fopen(strLogPath,_T("a+t"));
				GetLocalTime(&sys);
				strTime.Format(_T("[%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
				strRecord.Format(_T("%s strTmpFilePath :%s,===========>strUploadPath :%s\n"),strTime,strTmpFilePath,strUploadPath);
				fwrite(strRecord,sizeof(TCHAR),strRecord.GetLength(),fileLog);
				fclose(fileLog);
			}
			else
			{
				fileLog = fopen(strLogPath,_T("a+t"));
				GetLocalTime(&sys);
				strTime.Format(_T("[%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
				strRecord.Format(_T("%s 移动文件失败\n"),strTime);
				fwrite(strRecord,sizeof(TCHAR),strRecord.GetLength(),fileLog);
				fclose(fileLog);
			}
			
			fileLog = fopen(strLogPath,_T("a+t"));
			GetLocalTime(&sys);
			strTime.Format(_T("[%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
			strRecord.Format(_T("%s 上传文件成功\n"),strTime);
			fwrite(strRecord,sizeof(TCHAR),strRecord.GetLength(),fileLog);
			fclose(fileLog);
		}
		else
		{
			fileLog = fopen(strLogPath,_T("a+t"));
			GetLocalTime(&sys);
			strTime.Format(_T("[%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
			strRecord.Format(_T("%s 没有可上传文件\n"),strTime);
			fwrite(strRecord,sizeof(TCHAR),strRecord.GetLength(),fileLog);
			fclose(fileLog);
		}
		strSavePath.Empty();
		strTmpFilePath.Empty();
		strUploadPath.Empty();
		LeaveCriticalSection(&pDlg->criticalUpload);
	}
	

	return 1;
}

void CVisionASMDlg::CheckProductDataCalibDataConsistency()
{
	for (int i=0; i<m_nPlatformNum; i++)
	{
		vcBaseVisionAlign *pVisionASM = m_vpVisionASM.at(i);
		// 产品信息和标定信息是否一致检查
		if (pVisionASM != NULL && !pVisionASM->CheckProductDataCalibDataConsistency())
		{
			CString strInfo;
			strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_PLATFORM_DDD_SSS), i+1, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_T0_CHECK_PRODUCT_CALIB_DIFF));
			AfxMessageBox(strInfo,MB_SYSTEMMODAL);
			g_opLogFile.SaveLogText(strInfo,1);
			strInfo =_T("软件退出！");
			g_opLogFile.SaveLogText(strInfo,1); 
			EndDialog(IDCANCEL);
			break;
		}
	}
}






// void CVisionASMDlg::MoveDlgItem(BOOL bMoveViewsOnly)
// {
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	int cx = rcClient.Width();
// 	int cy = rcClient.Height();
// 	
// 	if (GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetSafeHwnd() != NULL)
// 	{
// 		int nStatusBar = 2;	// 如果使用状态栏，请改成相应高度
// 		int nFrame = 100 + nStatusBar;
// 		int nDeltaX = 0;
// 		int nDeltaY = 0;
// 		int nBtnInterVal = 6;
// 		int nGRPIntend = 8;
// 
// 		CRect rect, rect1;
// 		if (!bMoveViewsOnly)
// 		{
// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->MoveWindow(
// 				rect.left, cy - nFrame, rect.Width(),  rect.Height());
// 			
// 			nDeltaY = cy - nFrame - rect.top;
// 
// 			GetDlgItem(IDC_STATIC_VIEW_SELECT)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 
// 			nDeltaX = cx - rect.Width() - 2 - rect.left;
// 			
// 			// 显示背景
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 				rect.left, rect.top, rect.Width() + nDeltaX,  rect.Height() + nDeltaY);		
// 
// 			// 竖直分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(), cy);
// 
// 			// 水平分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect1);
// 			ScreenToClient(rect1);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect1.left,  rect.Height());	
// 			
// 			// 系统设置区
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 			
// 			// 退出和关机
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 			
// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect1);
// 			ScreenToClient(rect1);
// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 				rect1.left + nGRPIntend, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 				rect1.right - rect.Width() - 8, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 		
// // 			// 状态栏
// // 			GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(
// // 				rect.left, cy - nStatusBar, cx - 2 * rect.left,  rect.Height());
// 
// 			// 显示控制区调整
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			ScreenToClient(rect1);
// 			GetDlgItem(IDC_STATIC_VIEW_SELECT)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			if (m_bShowBottomCtrlPannel)
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW_SELECT)->MoveWindow(
// 					cx - rect.Width() - 2, rect.top, rect.Width(),  rect1.bottom - rect.top);
// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 					cx - rect.Width() - 2, rect.top, rect.Width(),  rect1.bottom - rect.top);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW_SELECT)->MoveWindow(
// 					cx - rect.Width() - 2, rect.top, rect.Width(),  rect.Height());
// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 					cx - rect.Width() - 2, rect.top, rect.Width(),  rect.Height());
// 			}
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_STATIC_VIEW_SELECT)->GetWindowRect(rect1);
// 			ScreenToClient(rect1);
// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->MoveWindow(
// 				rect.left + nDeltaX, rect1.bottom - nBtnInterVal - rect.Height() - nStatusBar, rect.Width(),  rect.Height());
// 
// 			// 运行/停止控制区调整
// // 			GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 				rect.left + nDeltaX, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// // 			// 状态栏调整
// // 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// // 			ScreenToClient(rect1);
// // 			m_pStatusBar->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			m_pStatusBar->MoveWindow(rect.left, rect.top, rect1.right-rect.left, rect.Height());
// 		}
// 
// 		if (m_pDlgVisionView != NULL)
// 		{		
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			int nWidth = 0, nHeight = 0, nInterval = 3;
// 			
// 			UpdateData(TRUE);
// 			if (m_nPlatformDispIdx <= 0)
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					nWidth = rect.Width();
// 					nHeight = rect.Height();
// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					break;
// 				case 2:
// // 					nWidth = (rect.Width()-nInterval)/2;
// // 					nHeight = rect.Height();
// // 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-nInterval)/2;
// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					m_pDlgVisionView[1].MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					break;
// 				case 3:
// // 					nWidth = (rect.Width()-2*nInterval)/3;
// // 					nHeight = rect.Height();
// // 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// // 					m_pDlgVisionView[2].MoveWindow(rect.left + (nWidth+nInterval)*2, rect.top, nWidth, nHeight);
// 
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-2*nInterval)/3;
// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					m_pDlgVisionView[1].MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
//  					m_pDlgVisionView[2].MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 					break;
// 				case 4:
// 				default:
// 					nWidth = (rect.Width()-nInterval)/2;
// 					nHeight = (rect.Height()-nInterval)/2;
// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 					m_pDlgVisionView[2].MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					m_pDlgVisionView[3].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					break;
// 				}
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				nWidth = rect.Width();
// 				nHeight = rect.Height();
// 				m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					if ( i == (m_nPlatformDispIdx - 1) )
// 					{
// 						m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 					}
// 					else
// 					{
// 						m_pDlgVisionView[i].ShowWindow(SW_HIDE);
// 					}
// 				}
// 
// 			}
// 		}
// 	}	
// }

// 


// void CVisionASMDlg::InitDlgItemSpecial()
// {
// 	GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->ShowWindow(SW_HIDE);
// //	GetDlgItem(IDC_STATIC_SEPARATOR2)->ShowWindow(SW_HIDE);
// 	m_bShowBottomCtrlPannel = TRUE;
//  
// 	// 隐藏右侧控制面板
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	
// 	CRect rcItem;
// 	GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcItem);
// 	ScreenToClient(rcItem);
// 	int nIndent = rcItem.left - rcClient.left;
// 	rcItem.right = rcClient.right - nIndent;
// //	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(rcItem);
// 	GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 
// 	GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// }

// void CVisionASMDlg::ShowVisionView(int nViewIndex)
// {
// 	if (m_eWSShowMode == esmMode2)
// 	{
// 		ShowVisionView4Platform(nViewIndex);
// 		return;
// 	}
// 
// 	if (m_vnVisionViewPos.size() <= 0)
// 	{
// 		return;
// 	}
// 	// 交换位置
// 	int nViewPos, nSelViewPos;
// 	int i = 0; 
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{
// 		if (m_vnVisionViewPos[i] == nViewIndex)
// 		{
// 			nSelViewPos = i;
// 			break;
// 		}
// 	}
// 
// 	if (i == m_vnVisionViewPos.size())
// 	{
// 		return;
// 	}
// 
// 	swap(m_vnVisionViewPos[0], m_vnVisionViewPos[nSelViewPos]);
// 
// 
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == 0)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(6);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(5);
// 		}
// 	}
// 
// 	MoveDlgItem(TRUE);	
// 
// 	// 显示运行信息
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == 0)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_HIDE);
// 		}
// 	}
// 
// 
// 	// 更新主界面其他信息
// 	nViewPos = m_vnVisionViewPos[0];
// 	CString strPlatformID;
// 	strPlatformID = m_pDlgVisionView[nViewPos].GetPlatformIDString();
// 	GetDlgItem(IDC_STATIC_WS)->SetWindowText(strPlatformID);
// 
// 	SysPlatformInfo platformInfo;
// 	int nPosNum = 0;
// 	m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 	nPosNum = m_vpVisionASM.at(nViewPos)->GetCameraNum();
// 	//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 	nPosNum = platformInfo.m_nPositionNum;
// 
// 	// 	if (platformInfo.m_nCamNum == 1)
// 	// 	{
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 	// 	}
// 	// 	else 
// 	if (nPosNum == 1)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 2)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 3)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 4)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);
// 	}	
// 	m_nRadioDisplayMode = m_pDlgVisionView[nViewPos].GetDisplayMode();
// 
// 	UpdateData(FALSE);
// 
// }

// 图像显示



// void CVisionASMDlg::OnBnClickedRadioDisplayPos0()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置1]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// 
// }


// void CVisionASMDlg::OnBnClickedRadioDisplayPos1()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置2]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }


// void CVisionASMDlg::OnBnClickedRadioDisplayPos2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置3]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }


// void CVisionASMDlg::OnBnClickedRadioDisplayPos3()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置4]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }


// void CVisionASMDlg::OnBnClickedRadioDisplayPos4()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置1,2]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }


// void CVisionASMDlg::OnBnClickedRadioDisplayPos5()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置3,4]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }


// void CVisionASMDlg::OnBnClickedRadioDisplayPos6()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置1,2,3,4]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }


// void CVisionASMDlg::OnBnClickedRadioDisplayPos7()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置7]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	UpdateData(TRUE);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }

// void CVisionASMDlg::OnBnClickedRadioDisplayPos8()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置6]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }

// void CVisionASMDlg::OnBnClickedRadioDisplayPos9()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置7]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }

// void CVisionASMDlg::OnBnClickedRadioDisplayPos10()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置8]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }

// void CVisionASMDlg::OnBnClickedRadioDisplayPos11()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[位置5,6,7,8]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }

// void CVisionASMDlg::OnBnClickedRadioDisplayPos12()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	CString strLog;
// 	strLog.Format(_T("点击显示位置[全部位置]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].SetDisplayMode(m_nRadioDisplayMode);
// }


// void CVisionASMDlg::OnBnClickedBtnManualTargetSearch2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击主界面[手动目标搜索]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 
// 	m_pDlgVisionView[nViewIndex].OnBnClickedBtnManualTargetSearch();
// }


// void CVisionASMDlg::OnBnClickedBtnManualObjectSearch2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString strLog;
// 	strLog.Format(_T("点击主界面[手动对象搜索]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 	int nViewIndex;
// 	if (m_eWSShowMode == esmMode0
// 		|| m_eWSShowMode == esmMode1)
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		nViewIndex = max(0, m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		nViewIndex = m_vnVisionViewPos[0];
// 	}
// 	m_pDlgVisionView[nViewIndex].OnBnClickedBtnManualObjectSearch();
// }

// void CVisionASMDlg::OnBnClickedBtnShowOtherView()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	m_bShowOtherView = !m_bShowOtherView;
// 	if (m_bShowOtherView)
// 	{
// 		GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->SetWindowText(_T("∨"));
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->SetWindowText(_T("∧"));
// 	}
// 
// 	MoveDlgItem(TRUE);
// }


// void CVisionASMDlg::OnBnClickedBtnTest()
// {
	// TODO: 在此添加控件通知处理程序代码
//	CString strLog;
// 	strLog.Format(_T("点击主界面[测试模式]\n"));
// 	g_opLogFile.SaveLogText(strLog,1);
// 
// 	CDlgCalibTargetPos dlg;
// 
// 	dlg.SetVisionAlignerRef(m_vpVisionASM[0]);
// 
// 	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 	if (dlg.DoModal() == IDOK)
// 	{
// 	}
// }	


// void CVisionASMDlg::InitDlgItemSpecial()
// {
// 	if (m_eWSShowMode == esmMode0)
// 	{
// 		// (m_nPlatformNum == 1 && nAllPosNumber == 8) 
// 		// (m_nPlatformNum == 1 && nAllPosNumber == 2) 
// 		// (m_nPlatformNum == 1 && nAllPosNumber == 6)
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->ShowWindow(SW_HIDE);
// 		//	GetDlgItem(IDC_STATIC_SEPARATOR2)->ShowWindow(SW_HIDE);
// 		m_bShowBottomCtrlPannel = TRUE;
// 
// 		// 隐藏右侧控制面板
// 		CRect rcClient;
// 		GetClientRect(rcClient);
// 
// 		CRect rcItem;
// 		GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcItem);
// 		ScreenToClient(rcItem);
// 		int nIndent = rcItem.left - rcClient.left;
// 		rcItem.right = rcClient.right - nIndent;
// 		//	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(rcItem);
// 		GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 
// 		GetDlgItem(IDC_STATIC_SEPARATOR2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->ShowWindow(SW_HIDE);
// 
// 		// 主显示工位1
// 		int nPosNum = 0;
// 		SysPlatformInfo platformInfo;
// 		//对象和目标不在同一台控制器时，对象端和目标端初始化时的位置显示
// 		for(int i=0;i<m_vnVisionViewPos.size();i++)
// 		{
// 			int nViewPos = m_vnVisionViewPos.at(i);
// 			m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 			nPosNum = platformInfo.m_nPositionNum;;
// 			if (nPosNum == 8 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 8 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 6 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 6 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 4 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(5);	
// 			}
// 			else if (nPosNum == 4 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(4);	
// 			}
// 			if (nPosNum == 2 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(1);	
// 			}
// 			else if (nPosNum == 2 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(0);	
// 			}
// 
// 		}
// 		// 只显示工位1
// 		ShowVisionView(0);
// 	}
// 	else if (m_eWSShowMode == esmMode1)
// 	{
// 		// 		(m_nPlatformNum == 1 && nAllPosNumber == 4)
// 		// 		(m_nPlatformNum == 2 && nAllPosNumber == 4)
// 
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->ShowWindow(SW_HIDE);
// 		//	GetDlgItem(IDC_STATIC_SEPARATOR2)->ShowWindow(SW_HIDE);
// 		m_bShowBottomCtrlPannel = TRUE;
// 
// 		// 隐藏右侧控制面板
// 		CRect rcClient;
// 		GetClientRect(rcClient);
// 
// 		CRect rcItem;
// 		GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcItem);
// 		ScreenToClient(rcItem);
// 		int nIndent = rcItem.left - rcClient.left;
// 		rcItem.right = rcClient.right - nIndent;
// 		//	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(rcItem);
// 		GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 
// 		// 主显示工位1
// 		int nPosNum = 0;
// 		SysPlatformInfo platformInfo;
// 		//对象和目标不在同一台控制器时，对象端和目标端初始化时的位置显示
// 		for(int i=0;i<m_vnVisionViewPos.size();i++)
// 		{
// 			int nViewPos = m_vnVisionViewPos.at(i);
// 			m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 			nPosNum = platformInfo.m_nPositionNum;;
// 			if (nPosNum == 8 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 8 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 6 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 6 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 4 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(5);	
// 			}
// 			else if (nPosNum == 4 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(4);	
// 			}
// 			if (nPosNum == 2 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(1);	
// 			}
// 			else if (nPosNum == 2 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(0);	
// 			}
// 		}
// 		ShowVisionView(0);
// 	} 
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		// 		(m_nPlatformNum == 2 && nAllPosNumber == 6)
// 		// 		(m_nPlatformNum == 2 && nAllPosNumber == 8)
// 		// 		m_nPlatformNum == 3
// 		// 		m_nPlatformNum == 4
// 		//		m_nPlatformNum == 5
// 		// 		m_nPlatformNum == 6
// 
// 		int i = 0;
// 		for (i=0; i<MaxPlatformNum; i++)
// 		{
// 			if (i < m_nPlatformNum)
// 			{
// 				//GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);
// 			}
// 		}	
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->ShowWindow(SW_HIDE);
// 		//	GetDlgItem(IDC_STATIC_SEPARATOR2)->ShowWindow(SW_HIDE);
// 		m_bShowBottomCtrlPannel = TRUE;
// 
// 		// 隐藏右侧控制面板
// 		CRect rcClient;
// 		GetClientRect(rcClient);
// 
// 		CRect rcItem;
// 		GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcItem);
// 		ScreenToClient(rcItem);
// 		int nIndent = rcItem.left - rcClient.left;
// 		rcItem.right = rcClient.right - nIndent;
// 		//	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(rcItem);
// 		GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 
// 		GetDlgItem(IDC_STATIC_SEPARATOR2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->ShowWindow(SW_HIDE);
// 		// 主显示工位1
// 		int nPosNum = 0;
// 		SysPlatformInfo platformInfo;
// 		//对象和目标不在同一台控制器时，对象端和目标端初始化时的位置显示
// 		for(int i=0;i<m_vnVisionViewPos.size();i++)
// 		{
// 			int nViewPos = m_vnVisionViewPos.at(i);
// 			m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 			nPosNum = platformInfo.m_nPositionNum;;
// 			if (nPosNum == 8 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 8 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 6 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 6 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 4 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(5);	
// 			}
// 			else if (nPosNum == 4 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(4);	
// 			}
// 			if (nPosNum == 2 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(1);	
// 			}
// 			else if (nPosNum == 2 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(0);	
// 			}
// 		}
// 		// 显示全部工位
// 		ShowVisionView(m_nPlatformDispIdx - 1);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->ShowWindow(SW_HIDE);
// 		//	GetDlgItem(IDC_STATIC_SEPARATOR2)->ShowWindow(SW_HIDE);
// 		m_bShowBottomCtrlPannel = TRUE;
// 
// 		// 隐藏右侧控制面板
// 		CRect rcClient;
// 		GetClientRect(rcClient);
// 
// 		CRect rcItem;
// 		GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcItem);
// 		ScreenToClient(rcItem);
// 		int nIndent = rcItem.left - rcClient.left;
// 		rcItem.right = rcClient.right - nIndent;
// 		//	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(rcItem);
// 		GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 		// 主显示工位1
// 		int nPosNum = 0;
// 		SysPlatformInfo platformInfo;
// 		//对象和目标不在同一台控制器时，对象端和目标端初始化时的位置显示
// 		for(int i=0;i<m_vnVisionViewPos.size();i++)
// 		{
// 			int nViewPos = m_vnVisionViewPos.at(i);
// 			m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 			nPosNum = platformInfo.m_nPositionNum;;
// 			if (nPosNum == 8 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 8 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 6 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(11);	
// 			}
// 			else if (nPosNum == 6 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(6);	
// 			}
// 			if (nPosNum == 4 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(5);	
// 			}
// 			else if (nPosNum == 4 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(4);	
// 			}
// 			if (nPosNum == 2 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(1);	
// 			}
// 			else if (nPosNum == 2 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 			{
// 				m_pDlgVisionView[nViewPos].SetDisplayMode(0);	
// 			}
// 		}
// 		// 主显示工位1
// 		ShowVisionView(0);
// 	}
// 
// }


// void CVisionASMDlg::ShowVisionView(int nViewIndex)
// {
// 	if (m_eWSShowMode == esmMode0)
// 	{
// 		ShowVisionViewMode0(nViewIndex);
// 	}
// 	else if (m_eWSShowMode == esmMode1)
// 	{
// 		ShowVisionViewMode1(nViewIndex);
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		ShowVisionViewMode2(nViewIndex);
// 	}
// 	else
// 	{
// 		ShowVisionViewMode1(nViewIndex);
// 	}
// 	
// 	UpdateManualSearchButton();
// }

// 更新手动按钮搜索按钮显示
// void  CVisionASMDlg::UpdateManualSearchButton()
// {
// 	if (m_eWSShowMode == esmMode0)
// 	{
// 		if (m_SysOptionConfig.m_bManualAfTarSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_HIDE);
// 		}
// 
// 		if (m_SysOptionConfig.m_bManualAfObjSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 	else if (m_eWSShowMode == esmMode1)
// 	{
// 		if (m_SysOptionConfig.m_bManualAfTarSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_HIDE);
// 		}
// 
// 		if (m_SysOptionConfig.m_bManualAfObjSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		if (m_SysOptionConfig.m_bManualAfTarSearchFailShowButton)
// 		{
// 			if (m_nPlatformDispIdx == 0)
// 			{
// 				GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_HIDE);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_SHOW);
// 			}
// 
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_HIDE);
// 		}
// 
// 		if (m_SysOptionConfig.m_bManualAfObjSearchFailShowButton)
// 		{
// 			if (m_nPlatformDispIdx == 0)
// 			{
// 				GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_HIDE);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_SHOW);
// 			}
// 
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 	else
// 	{
// 		if (m_SysOptionConfig.m_bManualAfObjSearchFailShowButton)
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_HIDE);
// 		}
// 	}
// }


// Mode0显示模式,图像显示
// void CVisionASMDlg::ShowVisionViewMode0(int nViewIndex)
// {
// 	if (m_vnVisionViewPos.size() <= 0)
// 	{
// 		return;
// 	}
// 	// 交换位置
// 	int nViewPos, nSelViewPos;
// 	int i = 0; 
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{
// 		if (m_vnVisionViewPos[i] == nViewIndex)
// 		{
// 			nSelViewPos = i;
// 			break;
// 		}
// 	}
// 
// 	if (i == m_vnVisionViewPos.size())
// 	{
// 		return;
// 	}
// 
// 	swap(m_vnVisionViewPos[0], m_vnVisionViewPos[nSelViewPos]);
// 
// 
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == 0)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(6);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(5);
// 		}
// 	}
// 
// 	MoveDlgItem(TRUE);	
// 
// 	// 显示运行信息
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == 0)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_HIDE);
// 		}
// 	}
// 
// 
// 	// 更新主界面其他信息
// 	nViewPos = m_vnVisionViewPos[0];
// 	if (m_SysOptionConfig.m_bPlatformCustomNameEnable)
// 	{
// 		GetDlgItem(IDC_STATIC_WS)->SetWindowText(m_SysOptionConfig.m_strPlatformCustomName[nViewPos]);
// 	}
// 	else
// 	{
// 		CString strPlatformID,strCtrlID;
// 		strPlatformID = m_pDlgVisionView[nViewPos].GetPlatformIDString();
// 		//strPlatformID.Format("%s%d",GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_WS),m_pDlgVisionView[nViewPos].GetPlatformID());
// 		strCtrlID = GetDlgCtrlString(IDC_STATIC_WS);
// 		strPlatformID.Format("%d",m_pDlgVisionView[nViewPos].GetPlatformID()+1);
// 		strCtrlID += strPlatformID;
// 		GetDlgItem(IDC_STATIC_WS)->SetWindowText(strCtrlID);
// 	}
// 
// 
// 	SysPlatformInfo platformInfo;
// 	int nPosNum = 0;
// 	m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 	nPosNum = m_vpVisionASM.at(nViewPos)->GetCameraNum();
// 	//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 	nPosNum = platformInfo.m_nPositionNum;
// 
// 	// 	if (platformInfo.m_nCamNum == 1)
// 	// 	{
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 	// 	}
// 	// 	else 
// 	if (nPosNum == 1 || (platformInfo.m_nPositionShowMode == 1) )
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 2)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 3)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 4)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}	
// 	else if (nPosNum == 6)	//hk20180130
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(TRUE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_SHOW);
// 	}
// 	else if (nPosNum == 8)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(TRUE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_SHOW);
// 	}	
// 	//对象和目标不在同一台控制器时，对象端和目标端位置选择单选按钮的禁用
// 	if (nPosNum == 8 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 8 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	if (nPosNum == 6 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 6 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	if (nPosNum == 4 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 4 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	if (nPosNum == 2 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 2 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	m_nRadioDisplayMode = m_pDlgVisionView[nViewPos].GetDisplayMode();
// 
// 
// 	if (platformInfo.m_nShowMode == 1)
// 	{
// 		GetDlgItem(IDC_STATIC_DISPLAY_MODE)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 
// 	UpdateData(FALSE);
// 
// }
// Mode1显示模式,图像显示
// void CVisionASMDlg::ShowVisionViewMode1(int nViewIndex)
// {
// 	if (m_vnVisionViewPos.size() <= 0)
// 	{
// 		return;
// 	}
// 	// 交换位置
// 	int nViewPos, nSelViewPos;
// 	int i = 0; 
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{
// 		if (m_vnVisionViewPos[i] == nViewIndex)
// 		{
// 			nSelViewPos = i;
// 			break;
// 		}
// 	}
// 
// 	if (i == m_vnVisionViewPos.size())
// 	{
// 		return;
// 	}
// 
// 	swap(m_vnVisionViewPos[0], m_vnVisionViewPos[nSelViewPos]);
// 
// 
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == 0)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(6);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(5);
// 		}
// 	}
// 
// 	MoveDlgItem(TRUE);	
// 
// 	// 显示运行信息
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == 0)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_HIDE);
// 		}
// 	}
// 
// 
// 	// 更新主界面其他信息
// 	nViewPos = m_vnVisionViewPos[0];
// 	if (m_SysOptionConfig.m_bPlatformCustomNameEnable)
// 	{
// 		GetDlgItem(IDC_STATIC_WS)->SetWindowText(m_SysOptionConfig.m_strPlatformCustomName[nViewPos]);
// 	}
// 	else
// 	{
// 		CString strPlatformID,strCtrlID;
// 		strPlatformID = m_pDlgVisionView[nViewPos].GetPlatformIDString();
// 	// 	strPlatformID.Format("%d",m_pDlgVisionView[nViewPos].GetPlatformID());
// 	// 	strPlatformID= GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_WS)+strPlatformID;
// 		strCtrlID = GetDlgCtrlString(IDC_STATIC_WS);
// 		strPlatformID.Format("%d",m_pDlgVisionView[nViewPos].GetPlatformID()+1);
// 		strCtrlID += strPlatformID;
// 		GetDlgItem(IDC_STATIC_WS)->SetWindowText(strCtrlID);
// 	}
// 
// 
// 	SysPlatformInfo platformInfo;
// 	int nPosNum = 0;
// 	m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 	nPosNum = m_vpVisionASM.at(nViewPos)->GetCameraNum();
// 	//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 	nPosNum = platformInfo.m_nPositionNum;
// 
// 	// 	if (platformInfo.m_nCamNum == 1)
// 	// 	{
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 	// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 	// 	}
// 	// 	else 
// 	if (nPosNum == 1 || (platformInfo.m_nPositionShowMode == 1) )
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 2)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 3)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 4)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 	else if (nPosNum == 6)	//hk20180130
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(TRUE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_SHOW);
// 	}
// 	else if (nPosNum == 8)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(TRUE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_SHOW);
// 	}
// 	//对象和目标不在同一台控制器时，对象端和目标端位置选择单选按钮的禁用	
// 	if (nPosNum == 8 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 8 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	if (nPosNum == 6 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 6 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	if (nPosNum == 4 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 4 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	if (nPosNum == 2 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	else if (nPosNum == 2 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 	{
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 	}
// 	m_nRadioDisplayMode = m_pDlgVisionView[nViewPos].GetDisplayMode();
// 
// 	if (platformInfo.m_nShowMode == 1)
// 	{
// 		GetDlgItem(IDC_STATIC_DISPLAY_MODE)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 	}
// 
// 	UpdateData(FALSE);
// 
// }	
// Mode2显示模式,图像显示
// void CVisionASMDlg::ShowVisionViewMode2(int nViewIndex)
// {
// 	if (m_vnVisionViewPos.size() <= 0 )
// 	{
// 		return;
// 	}
// 
// 	m_nPlatformDispIdx = nViewIndex + 1;
// 	m_nPrevDispPltfrmIdx = m_nPlatformDispIdx;
// 	UpdateData(FALSE);
// 
// 	// 交换位置
// 	int nViewPos = 0, nSelViewPos = 0;
// 	int i = 0; 
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == nViewIndex)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(6);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionView[nViewPos].SetAlignerInfoAlignment(5);
// 		}
// 	}
// 
// 	MoveDlgItem(TRUE);	
// 
// 	// 显示运行信息
// 	for (i = 0; i < m_vnVisionViewPos.size(); i++)
// 	{		
// 		if (i == nViewIndex)
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			nViewPos = m_vnVisionViewPos[i];
// 			m_pDlgVisionRun[nViewPos].ShowWindow(SW_HIDE);
// 		}
// 	}
// 
// 	if (nViewIndex == -1)	// 如果显示所有平台
// 	{
// 
// 		GetDlgItem(IDC_STATIC_WS_ICON)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_STATIC_WS)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_STATIC_DISPLAY_MODE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 
// 		GetDlgItem(IDC_STATIC_WS_ICON)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_WS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_DISPLAY_MODE)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->ShowWindow(SW_HIDE);
// 
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 
// 		for (i = 0; i < MaxPlatformNum; i ++)
// 		{
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);			
// 		}
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->ShowWindow(SW_HIDE);
// 
// 
// 		GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_HIDE);
// 
// 		GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_HIDE);
// 
// 
// 		GetDlgItem(IDC_STATIC_SEPARATOR5)->ShowWindow(SW_HIDE);
// 
// 
// 		m_nPlatformDispIdx = nViewIndex + 1;
// 
// 		UpdateData(FALSE);
// 
// 	}
// 	else	// 显示某个工位时
// 	{
// 		// 更新主界面其他信息
// 		nViewPos = m_vnVisionViewPos[nViewIndex];
// 		if (m_SysOptionConfig.m_bPlatformCustomNameEnable)
// 		{
// 			GetDlgItem(IDC_STATIC_WS)->SetWindowText(m_SysOptionConfig.m_strPlatformCustomName[nViewPos]);
// 		}
// 		else
// 		{
// 			CString strPlatformID,strCtrlID;
// 			strPlatformID = m_pDlgVisionView[nViewPos].GetPlatformIDString();
// 
// 			strCtrlID = GetDlgCtrlString(IDC_STATIC_WS);
// 			strPlatformID.Format("%d",m_pDlgVisionView[nViewPos].GetPlatformID()+1);
// 			strCtrlID += strPlatformID;
// 			GetDlgItem(IDC_STATIC_WS)->SetWindowText(strCtrlID);
// 		}
// 
// 
// 		SysPlatformInfo platformInfo;
// 		int nPosNum = 0;
// 		m_vpVisionASM.at(nViewPos)->GetSysPlatformInfo(platformInfo);
// 		nPosNum = m_vpVisionASM.at(nViewPos)->GetCameraNum();
// 		//		nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		nPosNum = platformInfo.m_nPositionNum;
// 
// 		// 	if (platformInfo.m_nCamNum == 1)
// 		// 	{
// 		// 		GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 		// 		GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 		// 		GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 		// 		GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 		// 		GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 		// 		GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 		// 		GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 		// 	}
// 		// 	else 
// 
// 
// 		GetDlgItem(IDC_STATIC_WS_ICON)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_STATIC_WS)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_STATIC_DISPLAY_MODE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->EnableWindow(TRUE);
// 
// 		GetDlgItem(IDC_STATIC_WS_ICON)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STATIC_WS)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STATIC_DISPLAY_MODE)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STATIC_SHOW_PLATFORM)->ShowWindow(SW_SHOW);
// 		if (nPosNum == 1 || (platformInfo.m_nPositionShowMode == 1) )
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 		}
// 		else if (nPosNum == 2)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 		}
// 		else if (nPosNum == 3)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 		}
// 		else if (nPosNum == 4)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 
// 
// 
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 		}
//         else if (nPosNum == 6)
//         {
//             GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(TRUE);
// 
// 
// 
//             GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_SHOW);
//         }
//         else if (nPosNum == 8)
//         {
//             GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(TRUE);
// 
// 
// 
//             GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_SHOW);
//             GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_SHOW);
//         }
// 		//对象和目标不在同一台控制器时，对象端和目标端位置选择单选按钮的禁用	
// 		if (nPosNum == 8 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		else if (nPosNum == 8 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		if (nPosNum == 6 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		else if (nPosNum == 6 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		if (nPosNum == 4 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		else if (nPosNum == 4 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		if (nPosNum == 2 && platformInfo.m_bTargetInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		else if (nPosNum == 2 && platformInfo.m_bObjectInDifferentComputer && platformInfo.m_bTargetObjectCamSeparate)
// 		{
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->EnableWindow(FALSE);
// 		}
// 		for (i = 0; i < MaxPlatformNum; i ++)
// 		{
// 			if (i < m_nPlatformNum)
// 			{
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
// 			} 
// 			else
// 			{
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_RADIO_SHOW_PLATFORM1+i)->ShowWindow(SW_HIDE);
// 			}
// 
// 		}
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->ShowWindow(SW_SHOW);
// 
// 		GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->ShowWindow(SW_SHOW);
// 
// 		GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->ShowWindow(SW_SHOW);
// 
// 		GetDlgItem(IDC_STATIC_SEPARATOR5)->ShowWindow(SW_SHOW);
// 
// 		m_nRadioDisplayMode = m_pDlgVisionView[nViewPos].GetDisplayMode();
// 
// 		if (platformInfo.m_nShowMode == 1)
// 		{
// 			GetDlgItem(IDC_STATIC_DISPLAY_MODE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS0)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS1)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS3)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS4)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS5)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS6)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS7)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS8)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS9)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS10)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS11)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_RADIO_DISPLAY_POS12)->ShowWindow(SW_HIDE);
// 		}
// 
// 		UpdateData(FALSE);
// 	}
// 
// }

// void CVisionASMDlg::MoveDlgItem(BOOL bMoveViewsOnly)
// {
// 
// 	// 获取整个显示区域
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	int cx = rcClient.Width();
// 	int cy = rcClient.Height();
// 	if (cx < 200 || cy < 200)
// 	{
// 		return;
// 	}
// 
// 	if (GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetSafeHwnd() != NULL)
// 	{
// 
// 		if (m_eWSShowMode == esmMode2)
// 		{
// 			MoveDlgItem4Platform(bMoveViewsOnly);
// 
// 			return;
// 		}
// 
// 		int nStatusBar = 2;	// 如果使用状态栏，请改成相应高度
// 		int nFrame = 5 + nStatusBar;
// 		int nDeltaX = 0;
// 		int nDeltaY = 0;
// 		int nBtnInterVal = 6;
// 		int nGRPIntend = 8;
// 
// 		// 获取第一个平台信息
// 		SysPlatformInfo localSysPlatformInfo;
// 		m_SystemConfigure.GetSysPlatformInfo(0,localSysPlatformInfo);
// 
// 		CRect rect, rect1;
// 		if (!bMoveViewsOnly)
// 		{
// 
// 			// 计算标题栏位置，并移动
// 			// Title
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(
// 				0, 0, cx,  rect.Height());	
// 			
// 	
// 	
// // 		
// // 			// 计算按钮位置，并移动
// // 			// Button
// // 			// 系统设置区
// // 			nDeltaY = 8;
// // 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// // 			// 			ScreenToClient(rect);
// // 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// // 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_TEST)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_TEST)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			// 退出和关机
// // 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDCANCEL)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// // 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 
// 
// 			// 计算按钮位置，并移动
// 			int nBtnNum = 7;
// 			int nBtnWidth = (cx - nBtnInterVal*(nBtnNum+1))/nBtnNum;
// 
// 			nDeltaY = 5;
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 				nBtnInterVal*1 + nBtnWidth*0, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(SW_HIDE);
// 	
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 				nBtnInterVal*2 + nBtnWidth*1, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 				nBtnInterVal*3 + nBtnWidth*2, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 				nBtnInterVal*4 + nBtnWidth*3, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
// 	
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 				nBtnInterVal*5 + nBtnWidth*4, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 			// 退出和关机
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(SW_HIDE);
// 
// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 				nBtnInterVal*6 + nBtnWidth*5, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 				nBtnInterVal*7 + nBtnWidth*6, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 	
// 
// 			nDeltaY = rect.Height() + nDeltaY + nFrame;
// 
// 			// 按钮水平分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 				0, cy - nDeltaY, cx,  rect.Height());	
// 
// 
// 
// 			// 显示模式水平分隔符
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);		
// 			nDeltaY = rect.Height() - rect.bottom;
// 
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->MoveWindow(
// 				0, rect.top + nDeltaY + 2, cx,  rect.Height());	
// 
// 			if (1 == m_nPlatformNum && 2 == localSysPlatformInfo.m_nPositionNum)
// 			{
// 				// 获取标题栏位置
// 				CRect rcTitle;
// 				GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
// 				ScreenToClient(rcTitle);
// 
// 				// 获取按钮分隔符
// 				CRect rcSep;
// 				GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 				ScreenToClient(rcSep);
// 
// 				// 设置垂直分隔符位置
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 					rect.left, rect.top + nDeltaY, rect.Width(), rcSep.top - rect.top - nDeltaY);
// 
// 
// 
// 				// 获取显示模式水平分隔符位置
// 				CRect rcSep5;
// 				GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 				ScreenToClient(rcSep5);
// 
// 				// 获取垂直分隔符位置
// 				CRect rcSep2;
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 				ScreenToClient(rcSep2);
// 
// 				// 设置平台显示切换按钮位置
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 					2, rcSep.top - rect.Height() - 2, cx - 2 ,  rect.Height());
// 
// 				CRect rcBtnShow;
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 				ScreenToClient(rcBtnShow);
// 
// 
// 				// 设置全部的显示背景，除了标题栏和按钮栏
// 				GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 					0, rcTitle.bottom + 1, cx,  cy - rcTitle.bottom -1);
// 
// 				// 显示背景
// 				// dlgvisionrun
// 				// 设置产品信息显示对话框位置
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 					10, rcSep.top - rect.Height() , cx - 20,  rect.Height());
// 
// 				// 获取产品信息显示对话框位置
// 				CRect rcView2;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcView2);
// 				ScreenToClient(rcView2);
// 
// 				// 设置各平台显示的整体位置
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 					0, rcSep5.bottom + 1 , cx,  rcSep.top - rcSep5.bottom -  rcView2.Height() - 2);
// 
// 				// 获取各平台显示的整体位置
// 				CRect rcView;
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 				ScreenToClient(rcView);
// 
// 				// 设置各平台显示之间的水平分隔符
// 				CRect rcSep7;
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 				ScreenToClient(rcSep7);
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 					rcView.left, rcView.bottom, rcView.Width(),rcSep7.Height());
// 			}
// 			else
// 			{
// 				// 获取标题栏位置
// 				CRect rcTitle;
// 				GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
// 				ScreenToClient(rcTitle);
// 
// 				// 获取按钮分隔符
// 				CRect rcSep;
// 				GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 				ScreenToClient(rcSep);
// 
// 				// 获取产品信息显示对话框位置
// 				CRect rcDlgRun;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcDlgRun);
// 				ScreenToClient(rcDlgRun);
// 
// 				// 设置垂直分隔符位置
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 					cx - rcDlgRun.Width() - rect.Width(), rect.top + nDeltaY, rect.Width(), rcSep.top - rect.top - nDeltaY);	
// 			
// 
// 				// 获取显示模式水平分隔符位置
// 				CRect rcSep5;
// 				GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 				ScreenToClient(rcSep5);
// 
// 				// 获取垂直分隔符位置
// 				CRect rcSep2;
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 				ScreenToClient(rcSep2);
// 
// 				// 设置平台显示切换按钮位置
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 					2, rcSep.top - rect.Height() - 2 , rcSep2.left - 2 ,  rect.Height());
// 
// 				// 获取平台显示切换按钮位置
// 				CRect rcBtnShow;
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 				ScreenToClient(rcBtnShow);
// 
// 				// 设置全部的显示背景，除了标题栏和按钮栏
// 				GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 					0, rcTitle.bottom + 1, cx,  cy - rcTitle.bottom -1);
// 
// 
// 				// 显示背景
// 				// dlgvisionrun
// 				// 设置产品信息显示对话框位置
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 					rcSep2.right, rcTitle.bottom, cx - rcSep2.right - 2,  rcSep.top - rcTitle.bottom - 2);
// 
// 
// 				// 获取产品信息显示对话框位置
// 				CRect rcView2;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcView2);
// 				ScreenToClient(rcView2);
// 
// 				// 设置各平台显示的整体位置
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 					0, rcSep5.bottom + 1 , rcSep2.left - 1,  rcSep.top - rcSep5.bottom -  rcBtnShow.Height() - 2);
// 
// 				// 获取各平台显示的整体位置
// 				CRect rcView;
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 				ScreenToClient(rcView);
// 
// 				// 设置各平台显示之间的水平分隔符
// 				CRect rcSep7;
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 				ScreenToClient(rcSep7);
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 					rcView.left, rcView.top + rcView.Height()/2, rcView.Width(),rcSep7.Height());
// 
// 			}
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 			// 				rect.left, rect.top + nDeltaY, rect.Width(), cy - rect.top - nDeltaY);	
// 			// 
// 			// 
// 			// 
// 			// 			CRect rcSep5;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 			// 			ScreenToClient(rcSep5);
// 			// 
// 			// 			CRect rcSep2;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 			// 			ScreenToClient(rcSep2);
// 			// 
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 			// 				2, cy - rect.Height() - 2 , rcSep2.left - 2 ,  rect.Height());
// 			// 
// 			// 			CRect rcBtnShow;
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 			// 			ScreenToClient(rcBtnShow);
// 			// 
// 			// 			// 显示背景
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 			// 				0, rcSep5.bottom + 1 , rcSep2.left - 1,  cy - rcSep5.bottom -  rcBtnShow.Height() - 2);
// 			// 			
// 			// 			CRect rcSep;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 			// 			ScreenToClient(rcSep);
// 			// 
// 			// 			// 显示背景
// 			// 			GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 			// 				0, rcSep.bottom + 1, cx,  cy - rcSep.bottom -1);
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 			// 				rcSep2.right + 2, rcSep.bottom + 2 , cx - rcSep2.right - 2,  cy - rcSep.bottom - 2);
// 			// 
// 			// 			CRect rcView;
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 			// 			ScreenToClient(rcView);
// 			// 
// 			// 			CRect rcSep7;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 			// 			ScreenToClient(rcSep7);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 			// 				rcView.left, rcView.top + rcView.Height()/2, rcView.Width(),rcSep7.Height());
// 
// 
// 
// 
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 
// 			// 			// 竖直分隔符
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(), cy);
// 			// 
// 			// 			// 水平分隔符
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 			// 				rect.left, rect.top + nDeltaY, rect1.left,  rect.Height());	
// 
// 			// 			// 系统设置区
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			// 			// 			ScreenToClient(rect);
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 			// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 退出和关机
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(
// 			// 				rect.left, cy - nStatusBar, cx - 2 * rect.left,  rect.Height());
// 
// 			// 			// 显示控制区调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			if (m_bShowBottomCtrlPannel)
// 			// 			{
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 			}
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect1.bottom - nBtnInterVal - rect.Height() - nStatusBar, rect.Width(),  rect.Height());
// 
// 			// 运行/停止控制区调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_LAMP)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_LAMP)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			m_pStatusBar->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			m_pStatusBar->MoveWindow(rect.left, rect.top, rect1.right-rect.left, rect.Height());
// 
// 			if (m_pDlgVisionRun != NULL)
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 
// 				int i = 0, nViewIndex = 0;
// 				for(i = 0; i < m_nPlatformNum; i++)
// 				{
// 					if (i == 0)
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_SHOW);
// 					}
// 					else
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_HIDE);
// 					}
// 				}
// 			}
// 
// 		}
// 
// 		// 		if (m_pDlgVisionView != NULL)
// 		// 		{		
// 		// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 		// 			ScreenToClient(rect);
// 		// 			int nWidth = 0, nHeight = 0, nInterval = 3;
// 		// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 		// 			int i = 0;
// 		// 			CRect rcTabSheet;
// 		// 			UpdateData(TRUE);
// 		// 			if (m_nPlatformDispIdx <= 0)
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					// 					nWidth = rect.Width();
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = rect.Height();
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 2:
// 		// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 3:
// 		// 					// 					nWidth = (rect.Width()-2*nInterval)/3;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[2].MoveWindow(rect.left + (nWidth+nInterval)*2, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-2*nInterval)/3;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 4:
// 		// 				default:
// 		// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 				//if (m_nPlatformNum != 1)
// 		// 				{
// 		// 					for (int i=0; i<m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 					}
// 		// 				}
// 		// 
// 		// 				for (int i=0; i<m_nPlatformNum; i++)
// 		// 				{
// 		// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 		// 				}
// 		// 
// 		// 			}
// 		// 			else
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 
// 		// 					break;
// 		// 				case 2:
// 		// 				case 3:
// 		// 				case 4:
// 		// 				default:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 			}
// 		// 		}
// 
// 
// 
// 
// 		if (m_pDlgVisionView != NULL)
// 		{		
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			int nWidth = 0, nHeight = 0, nInterval = 5, nOtherWidth = 0;
// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 			int i = 0, nViewIndex = 0;
// 			CRect rcTabSheet;
// 			UpdateData(TRUE);
// 			if (m_bShowOtherView)
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					nWidth = rect.Width();
// 					nHeight = rect.Height();
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = rect.Height();
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,4,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 2:
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 3:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - nInterval)/2;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nOtherWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-2*nInterval)/3;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 4:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - 2*nInterval)/3;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval), rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(3);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval)*2, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 
// 					// 
// 					// 
// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;	
// 				default:
// 					break;
// 				}
// 
// 				//if (m_nPlatformNum != 1)
// 				// 				{
// 				// 					for (int i=0; i<m_nPlatformNum; i++)
// 				// 					{
// 				// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 				// 					}
// 				// 				}
// 
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 				}
// 
// 				if (m_nPlatformNum == 1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 				}
// 				else
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				case 2:
// 				case 3:
// 				case 4:
// 				default:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				}
// 
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 
// 			}
// 		}
// 
// 	}	
// }

// 动态调整界面上各控件的位置
// void CVisionASMDlg::MoveDlgItem(BOOL bMoveViewsOnly)
// {
// 	return;
// 	if (m_eWSShowMode == esmMode0)
// 	{
// 		MoveDlgItemMode0(bMoveViewsOnly);
// 	}
// 	else if (m_eWSShowMode == esmMode1)
// 	{
// 		MoveDlgItemMode1(bMoveViewsOnly);
// 	}
// 	else if (m_eWSShowMode == esmMode2)
// 	{
// 		MoveDlgItemMode2(bMoveViewsOnly);
// 	}
// 	else
// 	{
// 		MoveDlgItemMode1(bMoveViewsOnly);
// 	}
// }

// Mode0显示模式,动态调整界面上各控件的位置
// void CVisionASMDlg::MoveDlgItemMode0(BOOL bMoveViewsOnly/* = FALSE*/)
// {
// 
// 	if (m_eWSShowMode != esmMode0)
// 	{
// 		return;
// 	}
// 
// 	// 获取整个显示区域
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	int cx = rcClient.Width();
// 	int cy = rcClient.Height();
// 	if (cx < 200 || cy < 200)
// 	{
// 		return;
// 	}
// 
// 	if (GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetSafeHwnd() != NULL)
// 	{
// 
// 		int nStatusBar = 2;	// 如果使用状态栏，请改成相应高度
// 		int nFrame = 5 + nStatusBar;
// 		int nDeltaX = 0;
// 		int nDeltaY = 0;
// 		int nBtnInterVal = 6;
// 		int nGRPIntend = 8;
// 
// 		// 获取第一个平台信息
// 		SysPlatformInfo localSysPlatformInfo;
// 		m_SystemConfigure.GetSysPlatformInfo(0,localSysPlatformInfo);
// 
// 		CRect rect, rect1;
// 		if (!bMoveViewsOnly)
// 		{
// 
// 			// 计算标题栏位置，并移动
// 			// Title
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(
// 				0, 0, cx,  rect.Height());	
// 
// 			{
// 				CRect rcRef, rcDest;
// 				// 手动对象搜索按钮
// 				GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcRef);
// 				ScreenToClient(rcRef);				
// 				GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				rcDest.left = rcRef.right - rect.Width() - 1*nBtnInterVal;
// 				rcDest.top = rect.top;
// 				rcDest.right = rcDest.left + rect.Width();
// 				rcDest.bottom = rcDest.top + rect.Height();
// 				GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->MoveWindow(rcDest);
// 
// 				// 手动目标搜索按钮
// 				GetDlgItem(IDC_BTN_MANUAL_OBJECT_SEARCH2)->GetWindowRect(rcRef);
// 				ScreenToClient(rcRef);				
// 				GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				rcDest.left = rcRef.left - rect.Width() - 1*nBtnInterVal;
// 				rcDest.top = rect.top;
// 				rcDest.right = rcDest.left + rect.Width();
// 				rcDest.bottom = rcDest.top + rect.Height();
// 				GetDlgItem(IDC_BTN_MANUAL_TARGET_SEARCH2)->MoveWindow(rcDest);
// 			}
// 
// 
// 			// 		
// 			// 			// 计算按钮位置，并移动
// 			// 			// Button
// 			// 			// 系统设置区
// 			// 			nDeltaY = 8;
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			// 			// 			ScreenToClient(rect);
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 			// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_TEST)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_TEST)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 退出和关机
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 
// 			//IDC_BTN_REGISTER
// 			// 计算按钮位置，并移动
// // 			if (localSysPlatformInfo.m_bExpireWarningEnabled)
// // 			{
// // 				int nBtnNum = 8;
// // 				int nBtnWidth = (cx - nBtnInterVal*(nBtnNum+1))/nBtnNum;
// // 
// // 				nDeltaY = 5;
// // 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// // 					nBtnInterVal*1 + nBtnWidth*0, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// // 					nBtnInterVal*2 + nBtnWidth*1, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// // 				// 			ScreenToClient(rect);
// // 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// // 				// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// // 					nBtnInterVal*3 + nBtnWidth*2, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// // 					nBtnInterVal*4 + nBtnWidth*3, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// // 					nBtnInterVal*5 + nBtnWidth*4, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				// 退出和关机
// // 				GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDCANCEL)->MoveWindow(
// // 					nBtnInterVal*6 + nBtnWidth*5, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// // 					nBtnInterVal*7 + nBtnWidth*6, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_REGISTER)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_REGISTER)->MoveWindow(
// // 					nBtnInterVal*8 + nBtnWidth*7, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// // 
// // 
// // 				nDeltaY = rect.Height() + nDeltaY + nFrame;
// // 			}
// // 			else
// 			{
// 				int nBtnNum = 7;
// 				int nBtnWidth = (cx - nBtnInterVal*(nBtnNum+1))/nBtnNum;
// 
// 				nDeltaY = 5;
// 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 					nBtnInterVal*1 + nBtnWidth*0, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 					nBtnInterVal*2 + nBtnWidth*1, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 				// 			ScreenToClient(rect);
// 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 				// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 					nBtnInterVal*3 + nBtnWidth*2, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 					nBtnInterVal*4 + nBtnWidth*3, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 					nBtnInterVal*5 + nBtnWidth*4, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				// 退出和关机
// 				GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDCANCEL)->MoveWindow(
// 					nBtnInterVal*6 + nBtnWidth*5, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 					nBtnInterVal*7 + nBtnWidth*6, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 				//GetDlgItem(IDC_BTN_REGISTER)->ShowWindow(SW_HIDE);
// 
// 
// 				nDeltaY = rect.Height() + nDeltaY + nFrame;
// 
// 			}
// 			
// 
// 			// 按钮水平分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 				0, cy - nDeltaY, cx,  rect.Height());	
// 
// 
// 
// 			// 显示模式水平分隔符
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);		
// 			nDeltaY = rect.Height() - rect.bottom;
// 
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->MoveWindow(
// 				0, rect.top + nDeltaY + 2, cx,  rect.Height());	
// 
// 			// if (m_eWSShowMode == esmMode0)
// 			{
// 				// 获取标题栏位置
// 				CRect rcTitle;
// 				GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
// 				ScreenToClient(rcTitle);
// 
// 				// 获取按钮分隔符
// 				CRect rcSep;
// 				GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 				ScreenToClient(rcSep);
// 
// 				// 设置垂直分隔符位置
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 					rect.left, rect.top + nDeltaY, rect.Width(), rcSep.top - rect.top - nDeltaY);
// 
// 
// 
// 				// 获取显示模式水平分隔符位置
// 				CRect rcSep5;
// 				GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 				ScreenToClient(rcSep5);
// 
// 				// 获取垂直分隔符位置
// 				CRect rcSep2;
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 				ScreenToClient(rcSep2);
// 
// 				// 设置平台显示切换按钮位置
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 					2, rcSep.top - rect.Height() - 2, cx - 2 ,  rect.Height());
// 
// 				CRect rcBtnShow;
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 				ScreenToClient(rcBtnShow);
// 
// 
// 				// 设置全部的显示背景，除了标题栏和按钮栏
// 				GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 					0, rcTitle.bottom + 1, cx,  cy - rcTitle.bottom -1);
// 
// 				// 显示背景
// 				// dlgvisionrun
// 				// 设置产品信息显示对话框位置
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 					5, rcSep.top - rect.Height() , cx - 10,  rect.Height());
// 
// 				// 获取产品信息显示对话框位置
// 				CRect rcView2;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcView2);
// 				ScreenToClient(rcView2);
// 
// 				// 设置各平台显示的整体位置
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 					0, rcSep5.bottom + 1 , cx,  rcSep.top - rcSep5.bottom -  rcView2.Height() - 2);
// 
// 				// 获取各平台显示的整体位置
// 				CRect rcView;
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 				ScreenToClient(rcView);
// 
// 				// 设置各平台显示之间的水平分隔符
// 				CRect rcSep7;
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 				ScreenToClient(rcSep7);
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 					rcView.left, rcView.bottom, rcView.Width(),rcSep7.Height());
// 			}			
// 		
// 
// 			if (m_pDlgVisionRun != NULL)
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 
// 				int i = 0, nViewIndex = 0;
// 				for(i = 0; i < m_nPlatformNum; i++)
// 				{
// 					if (i == 0)
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_SHOW);
// 					}
// 					else
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_HIDE);
// 					}
// 				}
// 			}
// 
// 		}
// 
// 		// 		if (m_pDlgVisionView != NULL)
// 		// 		{		
// 		// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 		// 			ScreenToClient(rect);
// 		// 			int nWidth = 0, nHeight = 0, nInterval = 3;
// 		// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 		// 			int i = 0;
// 		// 			CRect rcTabSheet;
// 		// 			UpdateData(TRUE);
// 		// 			if (m_nPlatformDispIdx <= 0)
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					// 					nWidth = rect.Width();
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = rect.Height();
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 2:
// 		// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 3:
// 		// 					// 					nWidth = (rect.Width()-2*nInterval)/3;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[2].MoveWindow(rect.left + (nWidth+nInterval)*2, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-2*nInterval)/3;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 4:
// 		// 				default:
// 		// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 				//if (m_nPlatformNum != 1)
// 		// 				{
// 		// 					for (int i=0; i<m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 					}
// 		// 				}
// 		// 
// 		// 				for (int i=0; i<m_nPlatformNum; i++)
// 		// 				{
// 		// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 		// 				}
// 		// 
// 		// 			}
// 		// 			else
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 
// 		// 					break;
// 		// 				case 2:
// 		// 				case 3:
// 		// 				case 4:
// 		// 				default:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 			}
// 		// 		}
// 
// 
// 		if (m_pDlgVisionView != NULL)
// 		{		
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			int nWidth = 0, nHeight = 0, nInterval = 5, nOtherWidth = 0;
// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 			int i = 0, nViewIndex = 0;
// 			CRect rcTabSheet;
// 			UpdateData(TRUE);
// 			if (m_bShowOtherView)
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					nWidth = rect.Width();
// 					nHeight = rect.Height();
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = rect.Height();
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,4,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 2:
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 3:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - nInterval)/2;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nOtherWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-2*nInterval)/3;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 4:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - 2*nInterval)/3;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval), rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(3);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval)*2, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 
// 					// 
// 					// 
// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;	
// 				default:
// 					break;
// 				}
// 
// 				//if (m_nPlatformNum != 1)
// 				// 				{
// 				// 					for (int i=0; i<m_nPlatformNum; i++)
// 				// 					{
// 				// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 				// 					}
// 				// 				}
// 
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 				}
// 
// 				if (m_nPlatformNum == 1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 				}
// 				else
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				case 2:
// 				case 3:
// 				case 4:
// 				default:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				}
// 
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 
// 			}
// 		}
// 
// 	}	
// }

// Mode1显示模式,动态调整界面上各控件的位置
// void CVisionASMDlg::MoveDlgItemMode1(BOOL bMoveViewsOnly/* = FALSE*/)
// {
// 
// 	if (m_eWSShowMode != esmMode1)
// 	{
// 		return;
// 	}
// 
// 	// 获取整个显示区域
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	int cx = rcClient.Width();
// 	int cy = rcClient.Height();
// 	if (cx < 200 || cy < 200)
// 	{
// 		return;
// 	}
// 
// 	if (GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetSafeHwnd() != NULL)
// 	{
// 
// 		int nStatusBar = 2;	// 如果使用状态栏，请改成相应高度
// 		int nFrame = 5 + nStatusBar;
// 		int nDeltaX = 0;
// 		int nDeltaY = 0;
// 		int nBtnInterVal = 6;
// 		int nGRPIntend = 8;
// 
// 		// 获取第一个平台信息
// 		SysPlatformInfo localSysPlatformInfo;
// 		m_SystemConfigure.GetSysPlatformInfo(0,localSysPlatformInfo);
// 
// 		CRect rect, rect1;
// 		if (!bMoveViewsOnly)
// 		{
// 
// 			// 计算标题栏位置，并移动
// 			// Title
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(
// 				0, 0, cx,  rect.Height());	
// 
// 
// 
// 			// 计算按钮位置，并移动
// // 			if (localSysPlatformInfo.m_bExpireWarningEnabled)
// // 			{
// // 				int nBtnNum = 8;
// // 				int nBtnWidth = (cx - nBtnInterVal*(nBtnNum+1))/nBtnNum;
// // 
// // 				nDeltaY = 5;
// // 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// // 					nBtnInterVal*1 + nBtnWidth*0, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// // 					nBtnInterVal*2 + nBtnWidth*1, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// // 				// 			ScreenToClient(rect);
// // 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// // 				// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// // 					nBtnInterVal*3 + nBtnWidth*2, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// // 					nBtnInterVal*4 + nBtnWidth*3, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// // 					nBtnInterVal*5 + nBtnWidth*4, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				// 退出和关机
// // 				GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDCANCEL)->MoveWindow(
// // 					nBtnInterVal*6 + nBtnWidth*5, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// // 					nBtnInterVal*7 + nBtnWidth*6, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_REGISTER)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_REGISTER)->MoveWindow(
// // 					nBtnInterVal*8 + nBtnWidth*7, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// // 
// // 
// // 				nDeltaY = rect.Height() + nDeltaY + nFrame;
// // 			}
// // 			else
// 			{
// 				int nBtnNum = 7;
// 				int nBtnWidth = (cx - nBtnInterVal*(nBtnNum+1))/nBtnNum;
// 
// 				nDeltaY = 5;
// 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 					nBtnInterVal*1 + nBtnWidth*0, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 					nBtnInterVal*2 + nBtnWidth*1, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 				// 			ScreenToClient(rect);
// 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 				// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 					nBtnInterVal*3 + nBtnWidth*2, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 					nBtnInterVal*4 + nBtnWidth*3, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 					nBtnInterVal*5 + nBtnWidth*4, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				// 退出和关机
// 				GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDCANCEL)->MoveWindow(
// 					nBtnInterVal*6 + nBtnWidth*5, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 					nBtnInterVal*7 + nBtnWidth*6, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 		//		GetDlgItem(IDC_BTN_REGISTER)->ShowWindow(SW_HIDE);
// 
// 
// 				nDeltaY = rect.Height() + nDeltaY + nFrame;
// 
// 			}
// 
// 			// 按钮水平分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 				0, cy - nDeltaY, cx,  rect.Height());	
// 
// 
// 
// 			// 显示模式水平分隔符
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);		
// 			nDeltaY = rect.Height() - rect.bottom;
// 
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->MoveWindow(
// 				0, rect.top + nDeltaY + 2, cx,  rect.Height());	
// 
// 			// if (m_eWSShowMode != esmMode1)
// 			{
// 				// 获取标题栏位置
// 				CRect rcTitle;
// 				GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
// 				ScreenToClient(rcTitle);
// 
// 				// 获取按钮分隔符
// 				CRect rcSep;
// 				GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 				ScreenToClient(rcSep);
// 
// 				// 获取产品信息显示对话框位置
// 				CRect rcDlgRun;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcDlgRun);
// 				ScreenToClient(rcDlgRun);
// 
// 				// 设置垂直分隔符位置
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 					cx - rcDlgRun.Width() - rect.Width(), rect.top + nDeltaY, rect.Width(), rcSep.top - rect.top - nDeltaY);	
// 
// 
// 				// 获取显示模式水平分隔符位置
// 				CRect rcSep5;
// 				GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 				ScreenToClient(rcSep5);
// 
// 				// 获取垂直分隔符位置
// 				CRect rcSep2;
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 				ScreenToClient(rcSep2);
// 
// 				// 设置平台显示切换按钮位置
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 					2, rcSep.top - rect.Height() - 2 , rcSep2.left - 2 ,  rect.Height());
// 
// 				// 获取平台显示切换按钮位置
// 				CRect rcBtnShow;
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 				ScreenToClient(rcBtnShow);
// 
// 				// 设置全部的显示背景，除了标题栏和按钮栏
// 				GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 					0, rcTitle.bottom + 1, cx,  cy - rcTitle.bottom -1);
// 
// 
// 				// 显示背景
// 				// dlgvisionrun
// 				// 设置产品信息显示对话框位置
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 					rcSep2.right, rcTitle.bottom, cx - rcSep2.right - 2,  rcSep.top - rcTitle.bottom - 2);
// 
// 
// 				// 获取产品信息显示对话框位置
// 				CRect rcView2;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcView2);
// 				ScreenToClient(rcView2);
// 
// 				// 设置各平台显示的整体位置
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 					0, rcSep5.bottom + 1 , rcSep2.left - 1,  rcSep.top - rcSep5.bottom -  rcBtnShow.Height() - 2);
// 
// 				// 获取各平台显示的整体位置
// 				CRect rcView;
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 				ScreenToClient(rcView);
// 
// 				// 设置各平台显示之间的水平分隔符
// 				CRect rcSep7;
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 				ScreenToClient(rcSep7);
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 					rcView.left, rcView.top + rcView.Height()/2, rcView.Width(),rcSep7.Height());
// 
// 			}
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 			// 				rect.left, rect.top + nDeltaY, rect.Width(), cy - rect.top - nDeltaY);	
// 			// 
// 			// 
// 			// 
// 			// 			CRect rcSep5;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 			// 			ScreenToClient(rcSep5);
// 			// 
// 			// 			CRect rcSep2;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 			// 			ScreenToClient(rcSep2);
// 			// 
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 			// 				2, cy - rect.Height() - 2 , rcSep2.left - 2 ,  rect.Height());
// 			// 
// 			// 			CRect rcBtnShow;
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 			// 			ScreenToClient(rcBtnShow);
// 			// 
// 			// 			// 显示背景
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 			// 				0, rcSep5.bottom + 1 , rcSep2.left - 1,  cy - rcSep5.bottom -  rcBtnShow.Height() - 2);
// 			// 			
// 			// 			CRect rcSep;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 			// 			ScreenToClient(rcSep);
// 			// 
// 			// 			// 显示背景
// 			// 			GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 			// 				0, rcSep.bottom + 1, cx,  cy - rcSep.bottom -1);
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 			// 				rcSep2.right + 2, rcSep.bottom + 2 , cx - rcSep2.right - 2,  cy - rcSep.bottom - 2);
// 			// 
// 			// 			CRect rcView;
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 			// 			ScreenToClient(rcView);
// 			// 
// 			// 			CRect rcSep7;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 			// 			ScreenToClient(rcSep7);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 			// 				rcView.left, rcView.top + rcView.Height()/2, rcView.Width(),rcSep7.Height());
// 
// 
// 
// 
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 
// 			// 			// 竖直分隔符
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(), cy);
// 			// 
// 			// 			// 水平分隔符
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 			// 				rect.left, rect.top + nDeltaY, rect1.left,  rect.Height());	
// 
// 			// 			// 系统设置区
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			// 			// 			ScreenToClient(rect);
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 			// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 退出和关机
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(
// 			// 				rect.left, cy - nStatusBar, cx - 2 * rect.left,  rect.Height());
// 
// 			// 			// 显示控制区调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			if (m_bShowBottomCtrlPannel)
// 			// 			{
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 			}
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect1.bottom - nBtnInterVal - rect.Height() - nStatusBar, rect.Width(),  rect.Height());
// 
// 			// 运行/停止控制区调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_LAMP)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_LAMP)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			m_pStatusBar->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			m_pStatusBar->MoveWindow(rect.left, rect.top, rect1.right-rect.left, rect.Height());
// 
// 			if (m_pDlgVisionRun != NULL)
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 
// 				int i = 0, nViewIndex = 0;
// 				for(i = 0; i < m_nPlatformNum; i++)
// 				{
// 					if (i == 0)
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_SHOW);
// 					}
// 					else
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_HIDE);
// 					}
// 				}
// 			}
// 
// 		}
// 
// 		// 		if (m_pDlgVisionView != NULL)
// 		// 		{		
// 		// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 		// 			ScreenToClient(rect);
// 		// 			int nWidth = 0, nHeight = 0, nInterval = 3;
// 		// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 		// 			int i = 0;
// 		// 			CRect rcTabSheet;
// 		// 			UpdateData(TRUE);
// 		// 			if (m_nPlatformDispIdx <= 0)
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					// 					nWidth = rect.Width();
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = rect.Height();
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 2:
// 		// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 3:
// 		// 					// 					nWidth = (rect.Width()-2*nInterval)/3;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[2].MoveWindow(rect.left + (nWidth+nInterval)*2, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-2*nInterval)/3;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 4:
// 		// 				default:
// 		// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 				//if (m_nPlatformNum != 1)
// 		// 				{
// 		// 					for (int i=0; i<m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 					}
// 		// 				}
// 		// 
// 		// 				for (int i=0; i<m_nPlatformNum; i++)
// 		// 				{
// 		// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 		// 				}
// 		// 
// 		// 			}
// 		// 			else
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 
// 		// 					break;
// 		// 				case 2:
// 		// 				case 3:
// 		// 				case 4:
// 		// 				default:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 			}
// 		// 		}
// 
// 
// 
// 
// 		if (m_pDlgVisionView != NULL)
// 		{		
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			int nWidth = 0, nHeight = 0, nInterval = 5, nOtherWidth = 0;
// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 			int i = 0, nViewIndex = 0;
// 			CRect rcTabSheet;
// 			UpdateData(TRUE);
// 			if (m_bShowOtherView)
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					nWidth = rect.Width();
// 					nHeight = rect.Height();
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = rect.Height();
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,4,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 2:
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 3:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - nInterval)/2;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nOtherWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-2*nInterval)/3;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 4:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - 2*nInterval)/3;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval), rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(3);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval)*2, rect.top + nHeight + nInterval, nOtherWidth, nHeight);
// 
// 					// 
// 					// 
// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;	
// 				default:
// 					break;
// 				}
// 
// 				//if (m_nPlatformNum != 1)
// 				// 				{
// 				// 					for (int i=0; i<m_nPlatformNum; i++)
// 				// 					{
// 				// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 				// 					}
// 				// 				}
// 
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 				}
// 
// 				if (m_nPlatformNum == 1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 				}
// 				else
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				case 2:
// 				case 3:
// 				case 4:
// 				default:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				}
// 
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 
// 			}
// 		}
// 
// 	}	
// }

// Mode2显示模式,动态调整界面上各控件的位置
// void CVisionASMDlg::MoveDlgItemMode2(BOOL bMoveViewsOnly/* = FALSE*/)
// {
// 	if (m_eWSShowMode != esmMode2)
// 	{
// 		return;
// 	}
// 
// 	// 获取整个显示区域
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	int cx = rcClient.Width();
// 	int cy = rcClient.Height();
// 	if (cx < 200 || cy < 200)
// 	{
// 		return;
// 	}
// 
// 	if (GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetSafeHwnd() != NULL)
// 	{
// 		int nStatusBar = 2;	// 如果使用状态栏，请改成相应高度
// 		int nFrame = 5 + nStatusBar;
// 		int nDeltaX = 0;
// 		int nDeltaY = 0;
// 		int nBtnInterVal = 6;
// 		int nGRPIntend = 8;
// 
// 		// 获取第一个平台信息
// 		SysPlatformInfo localSysPlatformInfo;
// 		m_SystemConfigure.GetSysPlatformInfo(0,localSysPlatformInfo);
// 
// 		CRect rect, rect1;
// 		if (!bMoveViewsOnly)
// 		{
// 
// 			// 计算标题栏位置，并移动
// 			// Title
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(
// 				0, 0, cx,  rect.Height());	
// 
// 
// 
// 			// 		
// 			// 			// 计算按钮位置，并移动
// 			// 			// Button
// 			// 			// 系统设置区
// 			// 			nDeltaY = 8;
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			// 			// 			ScreenToClient(rect);
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 			// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_TEST)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_TEST)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 退出和关机
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 				rect.left, cy - rect.Height() - nDeltaY, rect.Width(),  rect.Height());
// 
// 
// 			// 计算按钮位置，并移动
// // 			if (localSysPlatformInfo.m_bExpireWarningEnabled)
// // 			{
// // 				int nBtnNum = 8;
// // 				int nBtnWidth = (cx - nBtnInterVal*(nBtnNum+1))/nBtnNum;
// // 
// // 				nDeltaY = 5;
// // 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// // 					nBtnInterVal*1 + nBtnWidth*0, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// // 					nBtnInterVal*2 + nBtnWidth*1, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// // 				// 			ScreenToClient(rect);
// // 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// // 				// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// // 					nBtnInterVal*3 + nBtnWidth*2, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// // 					nBtnInterVal*4 + nBtnWidth*3, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// // 					nBtnInterVal*5 + nBtnWidth*4, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				// 退出和关机
// // 				GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(SW_HIDE);
// // 
// // 				GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDCANCEL)->MoveWindow(
// // 					nBtnInterVal*6 + nBtnWidth*5, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// // 					nBtnInterVal*7 + nBtnWidth*6, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_REGISTER)->GetWindowRect(rect);
// // 				ScreenToClient(rect);
// // 				GetDlgItem(IDC_BTN_REGISTER)->MoveWindow(
// // 					nBtnInterVal*8 + nBtnWidth*7, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// // 
// // 				GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// // 
// // 
// // 				nDeltaY = rect.Height() + nDeltaY + nFrame;
// // 			}
// // 			else
// 			{
// 				int nBtnNum = 7;
// 				int nBtnWidth = (cx - nBtnInterVal*(nBtnNum+1))/nBtnNum;
// 
// 				nDeltaY = 5;
// 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 					nBtnInterVal*1 + nBtnWidth*0, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_CALIBRATION_SET)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 					nBtnInterVal*2 + nBtnWidth*1, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 				// 			ScreenToClient(rect);
// 				// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 				// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 					nBtnInterVal*3 + nBtnWidth*2, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 					nBtnInterVal*4 + nBtnWidth*3, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 					nBtnInterVal*5 + nBtnWidth*4, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				// 退出和关机
// 				GetDlgItem(IDC_STATIC_GRP_EXIT)->ShowWindow(SW_HIDE);
// 
// 				GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDCANCEL)->MoveWindow(
// 					nBtnInterVal*6 + nBtnWidth*5, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 					nBtnInterVal*7 + nBtnWidth*6, cy - rect.Height() - nDeltaY, nBtnWidth,  rect.Height());
// 
// 				GetDlgItem(IDC_BTN_PANEL)->ShowWindow(SW_HIDE);
// 				//GetDlgItem(IDC_BTN_REGISTER)->ShowWindow(SW_HIDE);
// 
// 
// 				nDeltaY = rect.Height() + nDeltaY + nFrame;
// 
// 			}
// 
// 			// 按钮水平分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 				0, cy - nDeltaY, cx,  rect.Height());	
// 
// 
// 
// 			// 显示模式水平分隔符
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);		
// 			nDeltaY = rect.Height() - rect.bottom;
// 
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->MoveWindow(
// 				0, rect.top + nDeltaY + 2, cx,  rect.Height());	
// 
// 			//if (m_eWSShowMode == esmMode2)
// 			{
// 				// 获取标题栏位置
// 				CRect rcTitle;
// 				GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
// 				ScreenToClient(rcTitle);
// 
// 				// 获取按钮分隔符
// 				CRect rcSep;
// 				GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 				ScreenToClient(rcSep);
// 
// 				// 设置垂直分隔符位置
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 					rect.left, rect.top + nDeltaY, rect.Width(), rcSep.top - rect.top - nDeltaY);
// 
// 
// 
// 				// 获取显示模式水平分隔符位置
// 				CRect rcSep5;
// 				GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 				ScreenToClient(rcSep5);
// 
// 				// 获取垂直分隔符位置
// 				CRect rcSep2;
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 				ScreenToClient(rcSep2);
// 
// 				// 设置平台显示切换按钮位置
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 					2, rcSep.top - rect.Height() - 2, cx - 2 ,  rect.Height());
// 
// 				CRect rcBtnShow;
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 				ScreenToClient(rcBtnShow);
// 
// 
// 				// 设置全部的显示背景，除了标题栏
// 				GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 					0, rcTitle.bottom + 1, cx,  cy - rcTitle.bottom -1);
// 
// 				// 设置全部工位的显示背景，除了标题栏和按钮栏
// 				GetDlgItem(IDC_STATIC_VIEW3)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW3)->MoveWindow(
// 					0, rcTitle.bottom + 1, cx,  rcSep.top - rcTitle.bottom -1);
// 
// 				// 显示背景
// 				// dlgvisionrun
// 				// 设置产品信息显示对话框位置
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 					5, rcSep.top - rect.Height() , cx - 10,  rect.Height());
// 
// 				// 获取产品信息显示对话框位置
// 				CRect rcView2;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcView2);
// 				ScreenToClient(rcView2);
// 
// 				// 设置各平台显示的整体位置
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 					0, rcSep5.bottom + 1 , cx,  rcSep.top - rcSep5.bottom -  rcView2.Height() - 2);
// 
// 				// 获取各平台显示的整体位置
// 				CRect rcView;
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 				ScreenToClient(rcView);
// 
// 				// 设置各平台显示之间的水平分隔符
// 				CRect rcSep7;
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 				ScreenToClient(rcSep7);
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 					rcView.left, rcView.bottom, rcView.Width(),rcSep7.Height());
// 			}
// 
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 			// 				rect.left, rect.top + nDeltaY, rect.Width(), cy - rect.top - nDeltaY);	
// 			// 
// 			// 
// 			// 
// 			// 			CRect rcSep5;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 			// 			ScreenToClient(rcSep5);
// 			// 
// 			// 			CRect rcSep2;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 			// 			ScreenToClient(rcSep2);
// 			// 
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 			// 				2, cy - rect.Height() - 2 , rcSep2.left - 2 ,  rect.Height());
// 			// 
// 			// 			CRect rcBtnShow;
// 			// 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 			// 			ScreenToClient(rcBtnShow);
// 			// 
// 			// 			// 显示背景
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 			// 				0, rcSep5.bottom + 1 , rcSep2.left - 1,  cy - rcSep5.bottom -  rcBtnShow.Height() - 2);
// 			// 			
// 			// 			CRect rcSep;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 			// 			ScreenToClient(rcSep);
// 			// 
// 			// 			// 显示背景
// 			// 			GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 			// 				0, rcSep.bottom + 1, cx,  cy - rcSep.bottom -1);
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 			// 				rcSep2.right + 2, rcSep.bottom + 2 , cx - rcSep2.right - 2,  cy - rcSep.bottom - 2);
// 			// 
// 			// 			CRect rcView;
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 			// 			ScreenToClient(rcView);
// 			// 
// 			// 			CRect rcSep7;
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 			// 			ScreenToClient(rcSep7);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 			// 				rcView.left, rcView.top + rcView.Height()/2, rcView.Width(),rcSep7.Height());
// 
// 
// 
// 
// 			// 
// 			// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 
// 			// 			// 竖直分隔符
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(), cy);
// 			// 
// 			// 			// 水平分隔符
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 			// 				rect.left, rect.top + nDeltaY, rect1.left,  rect.Height());	
// 
// 			// 			// 系统设置区
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			// 			// 			ScreenToClient(rect);
// 			// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 			// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			// 退出和关机
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(
// 			// 				rect.left, cy - nStatusBar, cx - 2 * rect.left,  rect.Height());
// 
// 			// 			// 显示控制区调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			if (m_bShowBottomCtrlPannel)
// 			// 			{
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 			}
// 			// 			else
// 			// 			{
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 			// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 			}
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect1.bottom - nBtnInterVal - rect.Height() - nStatusBar, rect.Width(),  rect.Height());
// 
// 			// 运行/停止控制区调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_LAMP)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_LAMP)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			// 
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			m_pStatusBar->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			m_pStatusBar->MoveWindow(rect.left, rect.top, rect1.right-rect.left, rect.Height());
// 
// 			if (m_pDlgVisionRun != NULL)
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 
// 				int i = 0, nViewIndex = 0;
// 				for(i = 0; i < m_nPlatformNum; i++)
// 				{
// 					if (i == m_nPlatformDispIdx - 1)
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_SHOW);
// 					}
// 					else
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_HIDE);
// 					}
// 				}
// 			}
// 
// 		}
// 
// 		// 		if (m_pDlgVisionView != NULL)
// 		// 		{		
// 		// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 		// 			ScreenToClient(rect);
// 		// 			int nWidth = 0, nHeight = 0, nInterval = 3;
// 		// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 		// 			int i = 0;
// 		// 			CRect rcTabSheet;
// 		// 			UpdateData(TRUE);
// 		// 			if (m_nPlatformDispIdx <= 0)
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					// 					nWidth = rect.Width();
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = rect.Height();
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 2:
// 		// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 3:
// 		// 					// 					nWidth = (rect.Width()-2*nInterval)/3;
// 		// 					// 					nHeight = rect.Height();
// 		// 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					// 					m_pDlgVisionView[2].MoveWindow(rect.left + (nWidth+nInterval)*2, rect.top, nWidth, nHeight);
// 		// 
// 		// 					nWidth = rect.Width();
// 		// 					nHeight = (rect.Height()-2*nInterval)/3;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				case 4:
// 		// 				default:
// 		// 					nWidth = (rect.Width()-nInterval)/2;
// 		// 					nHeight = (rect.Height()-nInterval)/2;
// 		// 
// 		// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 		// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		// 
// 		// 					for (i = 0; i < m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 
// 		// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 				//if (m_nPlatformNum != 1)
// 		// 				{
// 		// 					for (int i=0; i<m_nPlatformNum; i++)
// 		// 					{
// 		// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 					}
// 		// 				}
// 		// 
// 		// 				for (int i=0; i<m_nPlatformNum; i++)
// 		// 				{
// 		// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 		// 				}
// 		// 
// 		// 			}
// 		// 			else
// 		// 			{
// 		// 				switch (m_nPlatformNum)
// 		// 				{
// 		// 				case 1:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,4,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 
// 		// 					break;
// 		// 				case 2:
// 		// 				case 3:
// 		// 				case 4:
// 		// 				default:
// 		// 					{
// 		// 						nWidth = rect.Width();
// 		// 						nHeight = rect.Height();
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 		// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 		// 						rcTabSheet.DeflateRect(2,20,4,4);
// 		// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 		// 						for (i=0; i<m_nPlatformNum; i++)
// 		// 						{
// 		// 							if ( i == (m_nPlatformDispIdx - 1) )
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 		// 							}
// 		// 							else
// 		// 							{
// 		// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 		// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 		// 							}
// 		// 						}
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 
// 		// 			}
// 		// 		}
// 
// 
// 
// 
// 		if (m_pDlgVisionView != NULL)
// 		{		
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			int nWidth = 0, nHeight = 0, nInterval = 5, nOtherWidth = 0, nOtherHeight = 0;
// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 			int i = 0, nViewIndex = 0;
// 			CRect rcTabSheet;
// 			UpdateData(TRUE);
// 			if (m_nPlatformDispIdx == 0)	// 显示所有工位
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW3)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					nWidth = rect.Width();
// 					nHeight = rect.Height();
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = rect.Height();
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,4,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 2:
// 					{
// 						SysPlatformInfo localSysPlatformInfo[2];
// 						m_SystemConfigure.GetSysPlatformInfo(0,localSysPlatformInfo[0]);
// 						m_SystemConfigure.GetSysPlatformInfo(1,localSysPlatformInfo[1]);
// 
// 						if (localSysPlatformInfo[0].m_nPositionNum == 4
// 							&& localSysPlatformInfo[1].m_nPositionNum == 4)
// 						{
// 							nWidth = (rect.Width()-nInterval)/2;					
// 							nHeight = rect.Height();
// 							nOtherWidth = (rect.Width()-nInterval)/2;	
// 							nOtherHeight = rect.Height();
// 
// 							nViewIndex = m_vnVisionViewPos.at(0);
// 							m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 							nViewIndex = m_vnVisionViewPos.at(1);
// 							m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 						}
// 						else
// 						{
// 							nWidth = rect.Width();					
// 							nHeight = (rect.Height()-nInterval)*3/5;
// 							nOtherWidth = rect.Width();	
// 							nOtherHeight = (rect.Height()-nInterval)*2/5;
// 
// 							nViewIndex = m_vnVisionViewPos.at(0);
// 							m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 							nViewIndex = m_vnVisionViewPos.at(1);
// 							m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						}
// 					}
// 
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 3:
// 					nWidth = (rect.Width()-nInterval)/2;					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width()-nInterval)/2;	
// 					nOtherHeight = (rect.Height()-nInterval)/2;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth+ nWidth, nOtherHeight);
// 
// 					// 					nWidth = rect.Width();
// 					// 					nHeight = (rect.Height()-2*nInterval)/3;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;
// 				case 4:
// 					nWidth = (rect.Width()-nInterval)/2;					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width()-nInterval)/2;	
// 					nOtherHeight = (rect.Height()-nInterval)/2;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 					nViewIndex = m_vnVisionViewPos.at(3);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 					// 
// 					// 
// 					// 					nWidth = (rect.Width()-nInterval)/2;
// 					// 					nHeight = (rect.Height()-nInterval)/2;
// 					// 
// 					// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 					// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					// 
// 					// 					for (i = 0; i < m_nPlatformNum; i++)
// 					// 					{
// 					// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 					// 						rcTabSheet.DeflateRect(2,20,4,4);
// 					// 
// 					// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					// 					}
// 					break;	
// 				case 5:
// 					if (0 == m_SysOptionConfig.m_nShowMode)			// 2行3列
// 					{
// 						nWidth = (rect.Width()-nInterval)/3;					
// 						nHeight = (rect.Height()-nInterval)/2;
// 						nOtherWidth = (rect.Width()-nInterval)/3;	
// 						nOtherHeight = (rect.Height()-nInterval)/2;
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						nViewIndex = m_vnVisionViewPos.at(1);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 						nViewIndex = m_vnVisionViewPos.at(2);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 						nViewIndex = m_vnVisionViewPos.at(3);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 						nViewIndex = m_vnVisionViewPos.at(4);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, 2*nOtherWidth, nOtherHeight);
// 					}
// 					else if (1 == m_SysOptionConfig.m_nShowMode)		// 3行2列
// 					{
// 						nWidth = (rect.Width()-nInterval)/2;					
// 						nHeight = (rect.Height()-nInterval)/3;
// 						nOtherWidth = (rect.Width()-nInterval)/2;	
// 						nOtherHeight = (rect.Height()-nInterval)/3;
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(1);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(2);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(3);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(4);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval + nHeight + nInterval, 2*nOtherWidth, nOtherHeight);
// 
// 					}
// 					else
// 					{
// 						nWidth = (rect.Width()-nInterval)/3;					
// 						nHeight = (rect.Height()-nInterval)/2;
// 						nOtherWidth = (rect.Width()-nInterval)/3;	
// 						nOtherHeight = (rect.Height()-nInterval)/2;
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						nViewIndex = m_vnVisionViewPos.at(1);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 						nViewIndex = m_vnVisionViewPos.at(2);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 						nViewIndex = m_vnVisionViewPos.at(3);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 						nViewIndex = m_vnVisionViewPos.at(4);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 					}
// 
// // 					nWidth = (rect.Width()-nInterval)/3;					
// // 					nHeight = (rect.Height()-nInterval)/2;
// // 					nOtherWidth = (rect.Width()-nInterval)/3;	
// // 					nOtherHeight = (rect.Height()-nInterval)/2;
// // 
// // 					nViewIndex = m_vnVisionViewPos.at(0);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					nViewIndex = m_vnVisionViewPos.at(1);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// // 					nViewIndex = m_vnVisionViewPos.at(2);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// // 					nViewIndex = m_vnVisionViewPos.at(3);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// // 					nViewIndex = m_vnVisionViewPos.at(4);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 					break;
// 				case 6:
// 					if (0 == m_SysOptionConfig.m_nShowMode)			// 2行3列
// 					{
// 						nWidth = (rect.Width()-nInterval)/3;					
// 						nHeight = (rect.Height()-nInterval)/2;
// 						nOtherWidth = (rect.Width()-nInterval)/3;	
// 						nOtherHeight = (rect.Height()-nInterval)/2;
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(1);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(2);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(3);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(4);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(5);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 					}
// 					else if (1 == m_SysOptionConfig.m_nShowMode)		// 3行2列
// 					{
// 						nWidth = (rect.Width()-nInterval)/2;					
// 						nHeight = (rect.Height()-nInterval)/3;
// 						nOtherWidth = (rect.Width()-nInterval)/2;	
// 						nOtherHeight = (rect.Height()-nInterval)/3;
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(1);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(2);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(3);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(4);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(5);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 					}
// 					else
// 					{
// 						nWidth = (rect.Width()-nInterval)/3;					
// 						nHeight = (rect.Height()-nInterval)/2;
// 						nOtherWidth = (rect.Width()-nInterval)/3;	
// 						nOtherHeight = (rect.Height()-nInterval)/2;
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(1);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(2);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(3);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(4);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 
// 						nViewIndex = m_vnVisionViewPos.at(5);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 					}
// // 					nWidth = (rect.Width()-nInterval)/3;					
// // 					nHeight = (rect.Height()-nInterval)/2;
// // 					nOtherWidth = (rect.Width()-nInterval)/3;	
// // 					nOtherHeight = (rect.Height()-nInterval)/2;
// // 
// // 					nViewIndex = m_vnVisionViewPos.at(0);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 
// // 					nViewIndex = m_vnVisionViewPos.at(1);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// // 
// // 					nViewIndex = m_vnVisionViewPos.at(2);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top, nOtherWidth, nOtherHeight);
// // 
// // 					nViewIndex = m_vnVisionViewPos.at(3);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// // 
// // 					nViewIndex = m_vnVisionViewPos.at(4);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// // 				
// // 					nViewIndex = m_vnVisionViewPos.at(5);
// // 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nWidth + nInterval + nWidth + nInterval, rect.top + nHeight + nInterval, nOtherWidth, nOtherHeight);
// 					break;
// 
// 				default:
// 					break;
// 				}
// 
// 				//if (m_nPlatformNum != 1)
// 				// 				{
// 				// 					for (int i=0; i<m_nPlatformNum; i++)
// 				// 					{
// 				// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 				// 					}
// 				// 				}
// 
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 				}
// 
// 				if (m_nPlatformNum == 1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 				}
// 				else
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_SHOW);
// 				}
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 			}
// 			else	// 显示单个工位
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_nPlatformDispIdx - 1;
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == m_nPlatformDispIdx - 1)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				case 2:
// 				case 3:
// 				case 4:
// 				default:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_nPlatformDispIdx - 1;
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == m_nPlatformDispIdx - 1)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				}
// 
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 
// 			}
// 		}
// 
// 	}
// }
// void CVisionASMDlg::MoveDlgItem(BOOL bMoveViewsOnly)
// {
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	int cx = rcClient.Width();
// 	int cy = rcClient.Height();
// 	if (cx < 200 || cy < 200)
// 	{
// 		return;
// 	}
// 
// 	if (GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetSafeHwnd() != NULL)
// 	{
// 		int nStatusBar = 2;	// 如果使用状态栏，请改成相应高度
// 		int nFrame = 15 + nStatusBar;
// 		int nDeltaX = 0;
// 		int nDeltaY = 0;
// 		int nBtnInterVal = 6;
// 		int nGRPIntend = 8;
// 		
// 		SysPlatformInfo localSysPlatformInfo;
// 		m_SystemConfigure.GetSysPlatformInfo(0,localSysPlatformInfo);
// 
// 		CRect rect, rect1;
// 		if (!bMoveViewsOnly)
// 		{
// 
// 			// Title
// 			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(
// 				0, 0, cx,  rect.Height());	
// 
// 			nDeltaY = rect.Height() - rect.bottom;
// 
// 			// Button
// 
// 			// 系统设置区
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_TEST)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_TEST)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			// 退出和关机
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 				rect.left, rect.top+ nDeltaY, rect.Width(),  rect.Height());
// 
// 
// 			// 水平分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 				0, rect.top + nDeltaY, cx,  rect.Height());	
// 
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR5)->MoveWindow(
// 				0, rect.top + nDeltaY, cx,  rect.Height());	
// 
// 			if (1 == m_nPlatformNum && 2 == localSysPlatformInfo.m_nPositionNum)
// 			{
// 
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 					rect.left, rect.top + nDeltaY, rect.Width(), cy - rect.top - nDeltaY);	
// 
// 
// 
// 				CRect rcSep5;
// 				GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 				ScreenToClient(rcSep5);
// 
// 				CRect rcSep2;
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 				ScreenToClient(rcSep2);
// 
// 
// 
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 					2, cy - rect.Height() - 2 , cx - 2 ,  rect.Height());
// 
// 				CRect rcBtnShow;
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 				ScreenToClient(rcBtnShow);
// 
// 				// 显示背景
// 				// dlgvisionrun
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 					10, cy - rect.Height() , cx - 20,  rect.Height());
// 
// 
// 				CRect rcView2;
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rcView2);
// 				ScreenToClient(rcView2);
// 
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 					0, rcSep5.bottom + 1 , cx,  cy - rcSep5.bottom -  rcView2.Height() - 2);
// 
// 				CRect rcSep;
// 				GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 				ScreenToClient(rcSep);
// 
// 				// 显示背景
// 				GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 					0, rcSep.bottom + 1, cx,  cy - rcSep.bottom -1);
// 
// 
// 				CRect rcView;
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 				ScreenToClient(rcView);
// 
// 				CRect rcSep7;
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 				ScreenToClient(rcSep7);
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 					rcView.left, rcView.top + rcView.Height(), rcView.Width(),rcSep7.Height());
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 					rect.left, rect.top + nDeltaY, rect.Width(), cy - rect.top - nDeltaY);	
// 
// 
// 
// 				CRect rcSep5;
// 				GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// 				ScreenToClient(rcSep5);
// 
// 				CRect rcSep2;
// 				GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// 				ScreenToClient(rcSep2);
// 
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// 					2, cy - rect.Height() - 2 , rcSep2.left - 2 ,  rect.Height());
// 
// 				CRect rcBtnShow;
// 				GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// 				ScreenToClient(rcBtnShow);
// 
// 				// 显示背景
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 					0, rcSep5.bottom + 1 , rcSep2.left - 1,  cy - rcSep5.bottom -  rcBtnShow.Height() - 2);
// 
// 				CRect rcSep;
// 				GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// 				ScreenToClient(rcSep);
// 
// 				// 显示背景
// 				GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// 					0, rcSep.bottom + 1, cx,  cy - rcSep.bottom -1);
// 
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 				GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// 					rcSep2.right + 2, rcSep.bottom + 2 , cx - rcSep2.right - 2,  cy - rcSep.bottom - 2);
// 
// 				CRect rcView;
// 				GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// 				ScreenToClient(rcView);
// 
// 				CRect rcSep7;
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// 				ScreenToClient(rcSep7);
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// 					rcView.left, rcView.top + rcView.Height()/2, rcView.Width(),rcSep7.Height());
// 
// 			}
// // 
// // 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// // 				rect.left, rect.top + nDeltaY, rect.Width(), cy - rect.top - nDeltaY);	
// // 
// // 
// // 
// // 			CRect rcSep5;
// // 			GetDlgItem(IDC_STATIC_SEPARATOR5)->GetWindowRect(rcSep5);
// // 			ScreenToClient(rcSep5);
// // 
// // 			CRect rcSep2;
// // 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcSep2);
// // 			ScreenToClient(rcSep2);
// // 
// // 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->MoveWindow(
// // 				2, cy - rect.Height() - 2 , rcSep2.left - 2 ,  rect.Height());
// // 
// // 			CRect rcBtnShow;
// // 			GetDlgItem(IDC_BTN_SHOW_OTHER_VIEW)->GetWindowRect(rcBtnShow);
// // 			ScreenToClient(rcBtnShow);
// // 
// // 			// 显示背景
// // 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// // 				0, rcSep5.bottom + 1 , rcSep2.left - 1,  cy - rcSep5.bottom -  rcBtnShow.Height() - 2);
// // 			
// // 			CRect rcSep;
// // 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rcSep);
// // 			ScreenToClient(rcSep);
// // 
// // 			// 显示背景
// // 			GetDlgItem(IDC_STATIC_VIEW1)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(
// // 				0, rcSep.bottom + 1, cx,  cy - rcSep.bottom -1);
// // 
// // 			GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(
// // 				rcSep2.right + 2, rcSep.bottom + 2 , cx - rcSep2.right - 2,  cy - rcSep.bottom - 2);
// // 
// // 			CRect rcView;
// // 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rcView);
// // 			ScreenToClient(rcView);
// // 
// // 			CRect rcSep7;
// // 			GetDlgItem(IDC_STATIC_SEPARATOR7)->GetWindowRect(rcSep7);
// // 			ScreenToClient(rcSep7);
// // 			GetDlgItem(IDC_STATIC_SEPARATOR7)->MoveWindow(
// // 				rcView.left, rcView.top + rcView.Height()/2, rcView.Width(),rcSep7.Height());
// 
// 
// 
// 
// // 
// // 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 
// // 			// 竖直分隔符
// // 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(), cy);
// // 
// // 			// 水平分隔符
// // 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect1);
// // 			ScreenToClient(rect1);
// // 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// // 				rect.left, rect.top + nDeltaY, rect1.left,  rect.Height());	
// 
// // 			// 系统设置区
// // 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// // 			// 			ScreenToClient(rect);
// // 			// 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// // 			// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			// 退出和关机
// // 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDCANCEL)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(
// 			// 				rect.left, cy - nStatusBar, cx - 2 * rect.left,  rect.Height());
// 
// // 			// 显示控制区调整
// // 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// // 			ScreenToClient(rect1);
// // 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			if (m_bShowBottomCtrlPannel)
// // 			{
// // 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// // 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// // 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 			}
// // 			else
// // 			{
// // 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// // 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// // 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 			}
// // 
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect1);
// // 			ScreenToClient(rect1);
// // 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->MoveWindow(
// // 				rect.left + nDeltaX, rect1.bottom - nBtnInterVal - rect.Height() - nStatusBar, rect.Width(),  rect.Height());
// 
// 			// 运行/停止控制区调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(
// 			// 				rect.left + nDeltaX, rect.top + nDeltaY, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_LAMP)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_LAMP)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// // 
// // 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			// 			// 状态栏调整
// 			// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			// 			ScreenToClient(rect1);
// 			// 			m_pStatusBar->GetWindowRect(rect);
// 			// 			ScreenToClient(rect);
// 			// 			m_pStatusBar->MoveWindow(rect.left, rect.top, rect1.right-rect.left, rect.Height());
// 
// 			if (m_pDlgVisionRun != NULL)
// 			{
// 				GetDlgItem(IDC_STATIC_VIEW2)->GetWindowRect(rect);
// 				ScreenToClient(rect);
// 
// 				int i = 0, nViewIndex = 0;
// 				for(i = 0; i < m_nPlatformNum; i++)
// 				{
// 					if (i == 0)
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_SHOW);
// 					}
// 					else
// 					{
// 						nViewIndex = m_vnVisionViewPos[i];
// 						m_pDlgVisionRun[nViewIndex].MoveWindow(rect);
// 						m_pDlgVisionRun[nViewIndex].ShowWindow(SW_HIDE);
// 					}
// 				}
// 			}
// 
// 		}
// 
// // 		if (m_pDlgVisionView != NULL)
// // 		{		
// // 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			int nWidth = 0, nHeight = 0, nInterval = 3;
// // 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// // 			int i = 0;
// // 			CRect rcTabSheet;
// // 			UpdateData(TRUE);
// // 			if (m_nPlatformDispIdx <= 0)
// // 			{
// // 				switch (m_nPlatformNum)
// // 				{
// // 				case 1:
// // 					// 					nWidth = rect.Width();
// // 					// 					nHeight = rect.Height();
// // 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 
// // 					nWidth = rect.Width();
// // 					nHeight = rect.Height();
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,4,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// // 					break;
// // 				case 2:
// // 					// 					nWidth = (rect.Width()-nInterval)/2;
// // 					// 					nHeight = rect.Height();
// // 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// // 
// // 					nWidth = rect.Width();
// // 					nHeight = (rect.Height()-nInterval)/2;
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,20,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// // 					break;
// // 				case 3:
// // 					// 					nWidth = (rect.Width()-2*nInterval)/3;
// // 					// 					nHeight = rect.Height();
// // 					// 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					// 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// // 					// 					m_pDlgVisionView[2].MoveWindow(rect.left + (nWidth+nInterval)*2, rect.top, nWidth, nHeight);
// // 
// // 					nWidth = rect.Width();
// // 					nHeight = (rect.Height()-2*nInterval)/3;
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,20,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// // 					break;
// // 				case 4:
// // 				default:
// // 					nWidth = (rect.Width()-nInterval)/2;
// // 					nHeight = (rect.Height()-nInterval)/2;
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,20,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// // 					break;
// // 				}
// // 
// // 				//if (m_nPlatformNum != 1)
// // 				{
// // 					for (int i=0; i<m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// // 					}
// // 				}
// // 
// // 				for (int i=0; i<m_nPlatformNum; i++)
// // 				{
// // 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// // 				}
// // 
// // 			}
// // 			else
// // 			{
// // 				switch (m_nPlatformNum)
// // 				{
// // 				case 1:
// // 					{
// // 						nWidth = rect.Width();
// // 						nHeight = rect.Height();
// // 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// // 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,4,4,4);
// // 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// // 						for (i=0; i<m_nPlatformNum; i++)
// // 						{
// // 							if ( i == (m_nPlatformDispIdx - 1) )
// // 							{
// // 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// // 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// // 							}
// // 							else
// // 							{
// // 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// // 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// // 							}
// // 						}
// // 					}
// // 
// // 					break;
// // 				case 2:
// // 				case 3:
// // 				case 4:
// // 				default:
// // 					{
// // 						nWidth = rect.Width();
// // 						nHeight = rect.Height();
// // 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// // 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,20,4,4);
// // 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// // 						for (i=0; i<m_nPlatformNum; i++)
// // 						{
// // 							if ( i == (m_nPlatformDispIdx - 1) )
// // 							{
// // 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// // 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// // 							}
// // 							else
// // 							{
// // 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// // 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// // 							}
// // 						}
// // 					}
// // 					break;
// // 				}
// // 
// // 			}
// // 		}
// 
// 
// 				
// 
// 		if (m_pDlgVisionView != NULL)
// 		{		
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			int nWidth = 0, nHeight = 0, nInterval = 5, nOtherWidth = 0;
// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 			int i = 0, nViewIndex = 0;
// 			CRect rcTabSheet;
// 			UpdateData(TRUE);
// 			if (m_bShowOtherView)
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					nWidth = rect.Width();
// 					nHeight = rect.Height();
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// // 					nWidth = rect.Width();
// // 					nHeight = rect.Height();
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,4,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// 					break;
// 				case 2:
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 
// // 					nWidth = rect.Width();
// // 					nHeight = (rect.Height()-nInterval)/2;
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,20,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// 					break;
// 				case 3:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - nInterval)/2;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nWidth + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + nOtherWidth + nInterval, rect.top + nWidth + nInterval, nOtherWidth, nHeight);
// 
// // 					nWidth = rect.Width();
// // 					nHeight = (rect.Height()-2*nInterval)/3;
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,20,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// 					break;
// 				case 4:
// 					nWidth = rect.Width();					
// 					nHeight = (rect.Height()-nInterval)/2;
// 					nOtherWidth = (rect.Width() - 2*nInterval)/3;
// 
// 					nViewIndex = m_vnVisionViewPos.at(0);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(1);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top + nWidth + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(2);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval), rect.top + nWidth + nInterval, nOtherWidth, nHeight);
// 					nViewIndex = m_vnVisionViewPos.at(3);
// 					m_pDlgVisionView[nViewIndex].MoveWindow(rect.left + (nOtherWidth + nInterval)*2, rect.top + nWidth + nInterval, nOtherWidth, nHeight);
// 
// // 
// // 
// // 					nWidth = (rect.Width()-nInterval)/2;
// // 					nHeight = (rect.Height()-nInterval)/2;
// // 
// // 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// // 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// // 
// // 					for (i = 0; i < m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// // 						rcTabSheet.DeflateRect(2,20,4,4);
// // 
// // 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// // 					}
// 					break;	
// 				default:
// 					break;
// 				}
// 
// 				//if (m_nPlatformNum != 1)
// // 				{
// // 					for (int i=0; i<m_nPlatformNum; i++)
// // 					{
// // 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// // 					}
// // 				}
// 
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 				}
// 
// 				if (m_nPlatformNum == 1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 				}
// 				else
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 							
// 						}
// 
// 					}
// 					break;
// 				case 2:
// 				case 3:
// 				case 4:
// 				default:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 
// 						nViewIndex = m_vnVisionViewPos.at(0);
// 						m_pDlgVisionView[nViewIndex].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{							
// 							if (i == 0)
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_SHOW);
// 							}
// 							else
// 							{
// 								nViewIndex = m_vnVisionViewPos.at(i);
// 								m_pDlgVisionView[nViewIndex].ShowWindow(SW_HIDE);
// 							}
// 
// 						}
// 
// 					}
// 					break;
// 				}
// 
// 				GetDlgItem(IDC_STATIC_SEPARATOR7)->ShowWindow(SW_HIDE);
// 
// 			}
// 		}
// 
// 	}	
// }
// 
// void CVisionASMDlg::MoveDlgItem(BOOL bMoveViewsOnly)
// {
// 	CRect rcClient;
// 	GetClientRect(rcClient);
// 	int cx = rcClient.Width();
// 	int cy = rcClient.Height();
// 	if (cx < 200 || cy < 200)
// 	{
// 		return;
// 	}
// 	
// 	if (GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetSafeHwnd() != NULL)
// 	{
// 		int nStatusBar = 2;	// 如果使用状态栏，请改成相应高度
// 		int nFrame = 15 + nStatusBar;
// 		int nDeltaX = 0;
// 		int nDeltaY = 0;
// 		int nBtnInterVal = 6;
// 		int nGRPIntend = 8;
// 
// 		CRect rect, rect1;
// 		if (!bMoveViewsOnly)
// 		{
// 
// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			nDeltaX = cx - rect.Width() - nFrame - rect.left;	
// 			
// 			// 显示背景
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			nDeltaY = cy - rect.Height() - 2 - rect.top;
// 
// 			GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(
// 				rect.left, rect.top, rect.Width() + nDeltaX,  rect.Height() + nDeltaY);	
// 			
// 
// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_GRP_SYS_CONFIG)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 
// 			// 竖直分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(), cy);
// 
// 			// 水平分隔符
// 			GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rect1);
// 			ScreenToClient(rect1);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_SEPARATOR)->MoveWindow(
// 				rect.left, rect.top + nDeltaY, rect1.left,  rect.Height());	
// 			
// 			// 系统设置区
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ENVIRONMENT_SET)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CALIBRATION_SET)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_PRODUCT_MANAGE)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// // 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_BTN_COMMUNICATION_SET)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_OPTION_SET)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_OPTION_SET)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_USER_LOGIN)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			
// 			// 退出和关机
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_STATIC_GRP_EXIT)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			
// 			GetDlgItem(IDCANCEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDCANCEL)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_CLOSE_WINDOWS)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 		
// // 			// 状态栏
// // 			GetDlgItem(IDC_STATUS_BAR_CTRL)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATUS_BAR_CTRL)->MoveWindow(
// // 				rect.left, cy - nStatusBar, cx - 2 * rect.left,  rect.Height());
// 
// 			// 显示控制区调整
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// 			ScreenToClient(rect1);
// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			if (m_bShowBottomCtrlPannel)
// 			{
// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 				GetDlgItem(IDC_BTN_PANEL)->MoveWindow(
// 					rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 			}
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM1)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM2)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM3)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM4)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_RADIO_SHOW_PLATFORM_ALL)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_PANEL)->GetWindowRect(rect1);
// 			ScreenToClient(rect1);
// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_SHOW_CTRL_PANNEL)->MoveWindow(
// 				rect.left + nDeltaX, rect1.bottom - nBtnInterVal - rect.Height() - nStatusBar, rect.Width(),  rect.Height());
// 
// 			// 运行/停止控制区调整
// // 			GetDlgItem(IDC_STATIC_VIEW_RUN)->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			GetDlgItem(IDC_STATIC_VIEW_RUN)->MoveWindow(
// // 				rect.left + nDeltaX, rect.top + nDeltaY, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_LAMP)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_LAMP)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem(IDC_BTN_ALIGNER_RUN)->MoveWindow(
// 				rect.left + nDeltaX, rect.top, rect.Width(),  rect.Height());
// 
// // 			// 状态栏调整
// // 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect1);
// // 			ScreenToClient(rect1);
// // 			m_pStatusBar->GetWindowRect(rect);
// // 			ScreenToClient(rect);
// // 			m_pStatusBar->MoveWindow(rect.left, rect.top, rect1.right-rect.left, rect.Height());
// 		}
// 
// 		if (m_pDlgVisionView != NULL)
// 		{		
// 			GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			int nWidth = 0, nHeight = 0, nInterval = 3;
// 			int nSpaceTop = 20, nSpaceLeft = 2, nSpaceRight = 20, nSpaceBottom = 20;
// 			int i = 0;
// 			CRect rcTabSheet;
// 			UpdateData(TRUE);
// 			if (m_nPlatformDispIdx <= 0)
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// // 					nWidth = rect.Width();
// // 					nHeight = rect.Height();
// // 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 
// 					nWidth = rect.Width();
// 					nHeight = rect.Height();
// 					
// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					
// 					for (i = 0; i < m_nPlatformNum; i++)
// 					{
// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 						rcTabSheet.DeflateRect(2,4,4,4);
// 						
// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					}
// 					break;
// 				case 2:
// // 					nWidth = (rect.Width()-nInterval)/2;
// // 					nHeight = rect.Height();
// // 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-nInterval)/2;
// 					
// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 
// 					for (i = 0; i < m_nPlatformNum; i++)
// 					{
// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 						rcTabSheet.DeflateRect(2,20,4,4);
// 						
// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					}
// 					break;
// 				case 3:
// // 					nWidth = (rect.Width()-2*nInterval)/3;
// // 					nHeight = rect.Height();
// // 					m_pDlgVisionView[0].MoveWindow(rect.left, rect.top, nWidth, nHeight);
// // 					m_pDlgVisionView[1].MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// // 					m_pDlgVisionView[2].MoveWindow(rect.left + (nWidth+nInterval)*2, rect.top, nWidth, nHeight);
// 
// 					nWidth = rect.Width();
// 					nHeight = (rect.Height()-2*nInterval)/3;
// 					
// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					m_vTabSheet.at(1)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + (nHeight+nInterval)*2, nWidth, nHeight);
// 
// 					for (i = 0; i < m_nPlatformNum; i++)
// 					{
// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 						rcTabSheet.DeflateRect(2,20,4,4);
// 						
// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					}
// 					break;
// 				case 4:
// 				default:
// 					nWidth = (rect.Width()-nInterval)/2;
// 					nHeight = (rect.Height()-nInterval)/2;
// 
// 					m_vTabSheet.at(0)->MoveWindow(rect.left, rect.top, nWidth, nHeight);
// 					m_vTabSheet.at(1)->MoveWindow(rect.left + nWidth + nInterval, rect.top, nWidth, nHeight);
// 					m_vTabSheet.at(2)->MoveWindow(rect.left, rect.top + nHeight + nInterval, nWidth, nHeight);
// 					m_vTabSheet.at(3)->MoveWindow(rect.left + nWidth + nInterval, rect.top + nHeight + nInterval, nWidth, nHeight);
// 		
// 					for (i = 0; i < m_nPlatformNum; i++)
// 					{
// 						m_vTabSheet.at(i)->GetClientRect(rcTabSheet);
// 						rcTabSheet.DeflateRect(2,20,4,4);
// 						
// 						m_pDlgVisionView[i].MoveWindow(rcTabSheet);
// 					}
// 					break;
// 				}
// 
// 				//if (m_nPlatformNum != 1)
// 				{
// 					for (int i=0; i<m_nPlatformNum; i++)
// 					{
// 						m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 					}
// 				}
// 
// 				for (int i=0; i<m_nPlatformNum; i++)
// 				{
// 					m_pDlgVisionView[i].ShowWindow(SW_SHOW);
// 				}
// 
// 			}
// 			else
// 			{
// 				switch (m_nPlatformNum)
// 				{
// 				case 1:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 						rcTabSheet.DeflateRect(2,4,4,4);
// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{
// 							if ( i == (m_nPlatformDispIdx - 1) )
// 							{
// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 							}
// 							else
// 							{
// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 							}
// 						}
// 					}
// 
// 					break;
// 				case 2:
// 				case 3:
// 				case 4:
// 				default:
// 					{
// 						nWidth = rect.Width();
// 						nHeight = rect.Height();
// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->MoveWindow(rect.left, rect.top, nWidth, nHeight);	
// 						m_vTabSheet.at(m_nPlatformDispIdx-1)->GetClientRect(rcTabSheet);
// 						rcTabSheet.DeflateRect(2,20,4,4);
// 						m_pDlgVisionView[m_nPlatformDispIdx-1].MoveWindow(rcTabSheet);
// 						for (i=0; i<m_nPlatformNum; i++)
// 						{
// 							if ( i == (m_nPlatformDispIdx - 1) )
// 							{
// 								m_vTabSheet.at(i)->ShowWindow(SW_SHOW);
// 								m_pDlgVisionView[i].ShowWindow(SW_SHOW);								
// 							}
// 							else
// 							{
// 								m_vTabSheet.at(i)->ShowWindow(SW_HIDE);
// 								m_pDlgVisionView[i].ShowWindow(SW_HIDE);								
// 							}
// 						}
// 					}
// 					break;
// 				}
// 
// 			}
// 		}
// 	}	
// }
// 


void CVisionASMDlg::OnBnClickedBtnDisplayModeSet()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_SysOptionConfig.m_nDisplayMode==0)
	{
		m_SysOptionConfig.m_nDisplayMode = 1;
		GetDlgItem(IDC_BTN_DISPLAY_MODE_SET)->SetWindowText(_T("帧有效"));
	}
	else if(m_SysOptionConfig.m_nDisplayMode == 1)
	{
		m_SysOptionConfig.m_nDisplayMode = 0;
		GetDlgItem(IDC_BTN_DISPLAY_MODE_SET)->SetWindowText(_T("实时显示"));
	}

	m_SysOptionConfig.SaveSystemOption();
}
