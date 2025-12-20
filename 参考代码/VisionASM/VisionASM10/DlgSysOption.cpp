// DlgSysOption.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysOption.h"
#include "afxdialogex.h"
#include "HorizontalLayout.h"
#include "VerticalLayout.h"
#include "VisionASMDLG.h"
#include "LightController.h"
// CDlgSysOption 对话框

IMPLEMENT_DYNAMIC(CDlgSysOption, CDialogEx)

	CDlgSysOption::CDlgSysOption(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSysOption::IDD, pParent)
	, m_nRdOption(0)
{
	m_pParent = (CVisionASMDlg*)pParent;

	m_pDlgSysOptionSetting = NULL;
	m_pDlgSysOptionLightSetting = NULL;

	m_pOptionPlaneLayout = NULL; // 选项板布局
	m_pButtonPlaneLayout = NULL; // 按钮板布局
	m_pOptionSettingLayout = NULL; // 基础设置窗口布局
	m_pOptionLightSettingLayout = NULL; // 光源设置窗口布局


	if (!m_SysOptionConfig.LoadSystemOption())
	{
		m_SysOptionConfig.m_bSysKeyboardEnabled = FALSE;
		m_SysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	}

	m_bSelNewSystem = FALSE;
}

CDlgSysOption::~CDlgSysOption()
{
}

void CDlgSysOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RD_OPT_BASE_SETTING, m_nRdOption);
	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
}


BEGIN_MESSAGE_MAP(CDlgSysOption, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSysOption::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSysOption::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgSysOption::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_RD_OPT_BASE_SETTING, &CDlgSysOption::OnBnClickedRdOptBaseSetting)
	ON_BN_CLICKED(IDC_RD_OPT_LIGHT_SETTING, &CDlgSysOption::OnBnClickedRdOptBaseSetting)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgSysOption 消息处理程序

// 初始化窗口
void CDlgSysOption::InitDlgItem()
{
	CreateOptionDialog();
	CreateDlgLayout();

	InitUIStyle();
}

// 释放窗口
void CDlgSysOption::DeInitDlgItem()
{
	DestroyDlgLayout();
	DestroyOptionDialog();
}

void CDlgSysOption::InitUIStyle()
{
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(150, 150, 150), TRUE);
	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(150, 150, 150), TRUE);
}

// 创建/销毁选项窗口
void CDlgSysOption::CreateOptionDialog()
{
	// 创建基础设置选项窗口
	m_pDlgSysOptionSetting = new CDlgSysOptionSetting;
	m_pDlgSysOptionSetting->m_pParent = (CVisionASMDlg *)this->GetParent();
	m_pDlgSysOptionSetting->SetSystemOptionConfig(m_SysOptionConfig);
	m_pDlgSysOptionSetting->SetSysInfoStringsRef(m_psaSysInfoStrings);
	m_pDlgSysOptionSetting->Create(IDD_SYS_OPTION_SETTING);
	m_pDlgSysOptionSetting->ShowWindow(SW_HIDE);


	// 创建光源设置选项窗口
	m_pDlgSysOptionLightSetting = new CDlgSysOptionLightSetting;
	m_pDlgSysOptionLightSetting->m_pParent = (CVisionASMDlg *)this->GetParent();
	m_pDlgSysOptionLightSetting->SetSystemOptionLightConfig(m_SysOptionConfig.m_sysOptLightConfig);
	m_pDlgSysOptionLightSetting->EnableSysKeyboard(m_SysOptionConfig.m_bSysKeyboardEnabled);//Summer0214
	m_pDlgSysOptionLightSetting->Create(IDD_SYS_OPTION_LIGHT_SETTING);
	m_pDlgSysOptionLightSetting->ShowWindow(SW_HIDE);


	// 设置初始化显示选项索引
	m_nRdOption = 0;
	m_pDlgSysOptionSetting->ShowWindow(SW_SHOW);
}
void CDlgSysOption::DestroyOptionDialog()
{
	if (m_pDlgSysOptionSetting && m_pDlgSysOptionSetting->GetSafeHwnd())
	{
		m_pDlgSysOptionSetting->DestroyWindow();
		delete m_pDlgSysOptionSetting;
		m_pDlgSysOptionSetting = NULL;
	}

	if (m_pDlgSysOptionLightSetting && m_pDlgSysOptionLightSetting->GetSafeHwnd())
	{
		m_pDlgSysOptionLightSetting->DestroyWindow();
		delete m_pDlgSysOptionLightSetting;
		m_pDlgSysOptionLightSetting = NULL;
	}

}

// 初始化窗口布局
void CDlgSysOption::CreateDlgLayout()
{
	// 初始化OptionPlane布局
	{
		// 选项按钮
		CLayout* pOptionButtonLayout = new CHorizontalLayout;
		pOptionButtonLayout->AddLayout(new CHorizontalLayout(), 30, 0, CLayout::eSizePolicy_Fixed);
		pOptionButtonLayout->AddWnd(GetDlgItem(IDC_RD_OPT_BASE_SETTING), 100, 0, CLayout::eSizePolicy_Fixed, 0, 18, 0, 18);
		pOptionButtonLayout->AddLayout(new CHorizontalLayout(), 30, 0, CLayout::eSizePolicy_Fixed);
		pOptionButtonLayout->AddWnd(GetDlgItem(IDC_RD_OPT_LIGHT_SETTING), 100, 0, CLayout::eSizePolicy_Fixed, 0, 18, 0, 18);

		// 选项按钮板
		m_pOptionPlaneLayout = new CVerticalLayout; 
		m_pOptionPlaneLayout->AddWnd(GetDlgItem(IDC_STATIC_RADIO_PANEL), 0, 80, CLayout::eSizePolicy_Fixed);
		m_pOptionPlaneLayout->AddLayout(pOptionButtonLayout, 0, 80, CLayout::eSizePolicy_Fixed, 0, -80, 0, 80);

		// 适应大小
		m_pOptionPlaneLayout->SetParent(this);
		m_pOptionPlaneLayout->OnSize();
	}

	// 初始化按钮板布局
	{
		// 功能按钮
		CLayout* pButtonButtonLayout = new CVerticalLayout;
		pButtonButtonLayout->AddLayout(new CVerticalLayout(), 0, 0, CLayout::eSizePolicy_Expand);
		pButtonButtonLayout->AddWnd(GetDlgItem(IDOK), 0, 45, CLayout::eSizePolicy_Fixed, 5, 0, 5, 0);
		pButtonButtonLayout->AddLayout(new CVerticalLayout(), 0, 30, CLayout::eSizePolicy_Fixed);
		pButtonButtonLayout->AddWnd(GetDlgItem(IDCANCEL), 0, 45, CLayout::eSizePolicy_Fixed, 5, 0, 5, 0);
		pButtonButtonLayout->AddLayout(new CVerticalLayout(), 0, 50, CLayout::eSizePolicy_Fixed);

		m_pButtonPlaneLayout = new CHorizontalLayout;
		m_pButtonPlaneLayout->AddLayout(new CHorizontalLayout(), 0, 0, CLayout::eSizePolicy_Expand);
		m_pButtonPlaneLayout->AddLayout(pButtonButtonLayout, 90, 0, CLayout::eSizePolicy_Fixed, 90, 0, -90, 0);
		m_pButtonPlaneLayout->AddWnd(GetDlgItem(IDC_BTN_PANEL), 90, 0, CLayout::eSizePolicy_Fixed, 0, 0, 0, 0);

		m_pButtonPlaneLayout->SetParent(this);
		m_pButtonPlaneLayout->OnSize();
	}
	// 基础设置窗口布局
	{
		m_pOptionSettingLayout = new CHorizontalLayout;
		m_pOptionSettingLayout->AddWnd(m_pDlgSysOptionSetting, 0, 0, CLayout::eSizePolicy_Expand, 1, 80, 90, 1, true);
		m_pOptionSettingLayout->SetParent(this);
		m_pOptionSettingLayout->OnSize();
	}
	// 光源设置窗口布局
	{
		m_pOptionLightSettingLayout = new CHorizontalLayout;
		m_pOptionLightSettingLayout->AddWnd(m_pDlgSysOptionLightSetting, 0, 0, CLayout::eSizePolicy_Expand, 1, 80, 90, 1, true);
		m_pOptionLightSettingLayout->SetParent(this);
		m_pOptionLightSettingLayout->OnSize();
	}
}


void CDlgSysOption::DestroyDlgLayout()
{
	if(m_pOptionPlaneLayout)
	{
		delete m_pOptionPlaneLayout;
		m_pOptionPlaneLayout = NULL;
	}

	if(m_pButtonPlaneLayout)
	{
		delete m_pButtonPlaneLayout;
		m_pButtonPlaneLayout = NULL;
	}

	if(m_pOptionSettingLayout)
	{
		delete m_pOptionSettingLayout;
		m_pOptionSettingLayout = NULL;
	}

	if(m_pOptionLightSettingLayout)
	{
		delete m_pOptionLightSettingLayout;
		m_pOptionLightSettingLayout = NULL;
	}

}


BOOL CDlgSysOption::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	MoveWindow(m_pParent->m_rcViewRect);

	InitDlgItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgSysOption::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSysOption::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CSystemOptionConfig tmpSysOptionConfig;

	m_pDlgSysOptionSetting->GetSystemOptionConfig(tmpSysOptionConfig);
	m_pDlgSysOptionLightSetting->GetSystemOptionLightConfig(tmpSysOptionConfig.m_sysOptLightConfig);


	// 检查当前视觉系统是否改变
	if (m_SysOptionConfig.m_nCurSystemIndex != tmpSysOptionConfig.m_nCurSystemIndex)
	{
		CString strInfo;
		//strInfo = g_vsLanguage.GetString( IDS_STR_CHOOSE_THE_SOFTWARE);//.Format(_T("确定改变当前视觉系统,选择新的视觉系统吗？\r\n选择“是”,则确定选择新的视觉系统,然后必须退出软件，重新打开软件；\r\n选择“否”,则仍选择当前的视觉系统。"));
		strInfo.Format(_T("确定改变当前视觉系统,选择新的视觉系统吗？\r\n选择“是”,则确定选择新的视觉系统,然后必须退出软件，重新打开软件；\r\n选择“否”,则仍选择当前的视觉系统。"));
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


	m_SysOptionConfig = tmpSysOptionConfig;

	CLightController::GetInstance().OpenConnect(m_SysOptionConfig.m_sysOptLightConfig);

	CString strLog;
	strLog.Format(_T("点击[确定]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("保存退出\n"));
	g_opLogFile.SaveLogText(strLog,1);

	m_SysOptionConfig.SaveSystemOption();

	CDialogEx::OnOK();
}


void CDlgSysOption::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	// 恢复原来配置光源配置
	CLightController::GetInstance().OpenConnect(m_SysOptionConfig.m_sysOptLightConfig);

	CDialogEx::OnCancel();
}


void CDlgSysOption::OnBnClickedBtnApply()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSysOption::OnBnClickedRdOptBaseSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPreOptSel = m_nRdOption;
	UpdateData(TRUE);
	if (nPreOptSel == m_nRdOption)
	{
		return;
	}

	m_pDlgSysOptionSetting->ShowWindow(SW_HIDE);
	m_pDlgSysOptionLightSetting->ShowWindow(SW_HIDE);


	switch(m_nRdOption)
	{
	case 0:
		{
			m_pDlgSysOptionSetting->ShowWindow(SW_SHOW);
		}
		break;
	case 1:
		{
			m_pDlgSysOptionLightSetting->ShowWindow(SW_SHOW);
		}
		break;
	default:
		return;
	}

}


void CDlgSysOption::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	DeInitDlgItem();
}


void CDlgSysOption::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_pOptionPlaneLayout)
	{
		m_pOptionPlaneLayout->OnSize();
	}
	if(m_pButtonPlaneLayout)
	{
		m_pButtonPlaneLayout->OnSize();
	}
	if(m_pOptionSettingLayout)
	{
		m_pOptionSettingLayout->OnSize();
	}
	if(m_pOptionLightSettingLayout)
	{
		m_pOptionLightSettingLayout->OnSize();
	}
}
