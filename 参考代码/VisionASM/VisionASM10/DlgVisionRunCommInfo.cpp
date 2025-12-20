// DlgVisionRunCommInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVisionRunCommInfo.h"
#include "afxdialogex.h"
#include "HorizontalLayout.h"
#include "VerticalLayout.h"
#include "DlgVisionRun.h"

// CDlgVisionRunCommInfo 对话框

IMPLEMENT_DYNAMIC(CDlgVisionRunCommInfo, CDialogEx)

CDlgVisionRunCommInfo::CDlgVisionRunCommInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVisionRunCommInfo::IDD, pParent)
	, m_nRadioCommandMode(0)
{
	m_pMainLayout = NULL;
	m_pVisionASM = NULL;
	m_bUpdatelogCommCommand = FALSE;
}

CDlgVisionRunCommInfo::~CDlgVisionRunCommInfo()
{
	if (m_pMainLayout)
	{
		delete m_pMainLayout;
		m_pMainLayout = NULL;
	}
}

void CDlgVisionRunCommInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMM_COMMAND, m_editLog);
	DDX_Radio(pDX, IDC_RADIO_COMMAND_DETAIL, m_nRadioCommandMode);
	DDX_Control(pDX, IDC_RADIO_COMMAND_DETAIL, m_rdCommDetail);
	DDX_Control(pDX, IDC_RADIO_COMMAND_SIMPLE, m_rdCommSimple);
	DDX_Control(pDX, IDC_BTN_COMMAND_CLEAR, m_btnCommClear);
}


BEGIN_MESSAGE_MAP(CDlgVisionRunCommInfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgVisionRunCommInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgVisionRunCommInfo::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_COMMAND_DETAIL, &CDlgVisionRunCommInfo::OnBnClickedRadioCommandDetail)
	ON_BN_CLICKED(IDC_RADIO_COMMAND_SIMPLE, &CDlgVisionRunCommInfo::OnBnClickedRadioCommandSimple)
	ON_BN_CLICKED(IDC_BTN_COMMAND_CLEAR, &CDlgVisionRunCommInfo::OnBnClickedBtnCommandClear)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgVisionRunCommInfo 消息处理程序


void CDlgVisionRunCommInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgVisionRunCommInfo::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgVisionRunCommInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitLayout();
	InitUIStyle();

	SetTimer(0,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgVisionRunCommInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgVisionRunCommInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pMainLayout)
	{
		m_pMainLayout->OnSize();
	}
}


void CDlgVisionRunCommInfo::InitUIStyle()
{
	 m_btnCommClear.SetThemeHelper(&m_themeHelpSt);
	 m_rdCommDetail.SetThemeHelper(&m_themeHelpSt);
	 m_rdCommSimple.SetThemeHelper(&m_themeHelpSt);

}

void CDlgVisionRunCommInfo::InitLayout()
{
	m_pMainLayout = new CVerticalLayout;

	// 间隙
	CLayout* pSegLayout = new CHorizontalLayout;
	// button
	CLayout* pBtnLayout = new CHorizontalLayout;
	pBtnLayout->AddWnd(GetDlgItem(IDC_RADIO_COMMAND_DETAIL), 70, 35, CLayout::eSizePolicy_Fixed);
	pBtnLayout->AddWnd(GetDlgItem(IDC_RADIO_COMMAND_SIMPLE), 70, 35, CLayout::eSizePolicy_Fixed);
	pBtnLayout->AddWnd(GetDlgItem(IDC_BTN_COMMAND_CLEAR), 70, 35, CLayout::eSizePolicy_Fixed);
	
	m_pMainLayout->AddLayout(pSegLayout, 0, 10, CLayout::eSizePolicy_Fixed);
	m_pMainLayout->AddLayout(pBtnLayout, 0, 40, CLayout::eSizePolicy_Fixed);
	m_pMainLayout->AddWnd(GetDlgItem(IDC_EDIT_COMM_COMMAND), 0,0, CLayout::eSizePolicy_Expand);
	m_pMainLayout->SetParent(this);
}

void CDlgVisionRunCommInfo::SetParentWnd(CDlgVisionRun* pParentWnd)
{
	m_pParent = pParentWnd;
}

void CDlgVisionRunCommInfo::SetVisionASMRef( vcBaseVisionAlign* pVisionASM )
{
	 m_pVisionASM = pVisionASM;
}


void CDlgVisionRunCommInfo::SetSimpleCommShow()
{
	m_nRadioCommandMode = 1;
	UpdateData(FALSE);
	UpdateLogCommCommand();
}


void CDlgVisionRunCommInfo::UpdateLogCommCommand()
{
	if (m_pParent->m_pParent->m_SysOptionConfig.m_nShowRunCommandDetail == 0)
	{
		m_editLog.SetWindowText(_T(""));
		m_editLog.LineScroll(m_editLog.GetLineCount());
		return;
	}

	UpdateData(TRUE);

	int i = 0;
	m_strLogCommandSimple.Empty();
	m_strLogCommandDetail.Empty();
	for(i = 0; i < m_saCommCommandSimple.size(); i++)
	{
		m_strLogCommandSimple += m_saCommCommandSimple.at(i) + _T("\r\n");

	}

	for (i=0;i<m_saCommCommandDetail.size();i++)
	{
		m_strLogCommandDetail += m_saCommCommandDetail.at(i) + _T("\r\n");
	}

	if (m_nRadioCommandMode == 0)
	{
		m_editLog.SetWindowText(m_strLogCommandDetail);
		m_editLog.LineScroll(m_editLog.GetLineCount());
	}
	else
	{
		m_editLog.SetWindowText(m_strLogCommandSimple);
		m_editLog.LineScroll(m_editLog.GetLineCount());
	}

	UpdateData(FALSE);
}


void CDlgVisionRunCommInfo::GetCommCommandInfo()
{
	if (m_pVisionASM)
	{
		m_pVisionASM->GetCommCommandInfo(m_saCommCommandSimple, m_saCommCommandDetail);
		m_bUpdatelogCommCommand = TRUE;
	}
}


void CDlgVisionRunCommInfo::OnBnClickedRadioCommandDetail()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[通信信息]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);

	UpdateLogCommCommand();
}


void CDlgVisionRunCommInfo::OnBnClickedRadioCommandSimple()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[指令显示]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);

	UpdateLogCommCommand();
}


void CDlgVisionRunCommInfo::OnBnClickedBtnCommandClear()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[指令清除]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	//PostMessage(WM_UPDATE_CLEAR_COMM_COMMAND_INFO);

	Sleep(100);

	m_pVisionASM->ClearCommCommandInfo();
	m_pVisionASM->GetCommCommandInfo(m_saCommCommandSimple, m_saCommCommandDetail);
	UpdateLogCommCommand();
	
}


void CDlgVisionRunCommInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( (nIDEvent == 0) && (m_pVisionASM != NULL) )
	{
		if (m_bUpdatelogCommCommand)	//hk20180130
		{
			m_bUpdatelogCommCommand = FALSE;
			UpdateLogCommCommand();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgVisionRunCommInfo::UpdateDialogLanguage()
{
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent && m_pParent->m_pParent &&m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,0));
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

		if(_T("IDD_DLG_VISION_RUN_COMM_INFO") == strDlgID)
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