// DlgRobotUserLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgRobotUserLogin.h"
#include "afxdialogex.h"
#include "DlgSysUserPasswordSetting.h"
#include "svXMLConfigurator.h"
#include "SHA256.H"
#include "LogFileSave.h"

// CDlgRobotUserLogin 对话框

IMPLEMENT_DYNAMIC(CDlgRobotUserLogin, CDialog)

CDlgRobotUserLogin::CDlgRobotUserLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRobotUserLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRobotUserLogin)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
}

CDlgRobotUserLogin::~CDlgRobotUserLogin()
{

}

void CDlgRobotUserLogin::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_ROBOT_USER_LOGIN") == strDlgID)
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
		}  
	}
	xml.LeaveElem();
}

void CDlgRobotUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRobotUserLogin)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT_CODE, m_strPassword);
//	DDV_MaxChars(pDX, m_strPassword, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRobotUserLogin, CDialog)
	//{{AFX_MSG_MAP(CDlgRobotUserLogin)
	ON_EN_SETFOCUS(IDC_EDIT_CODE, OnSetfocusEditCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRobotUserLogin message handlers

BOOL CDlgRobotUserLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 设置按钮
	m_btnOK.SetThemeHelper(&m_themeHelper);
	// 	m_btnOK.SetIcon(IDI_ICON_OK_16);
	// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnCancel.SetThemeHelper(&m_themeHelper);
	// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_16);
	// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_HORIZ);

//	UpdateDialogLanguage();

	//	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACC_SUPER_USER));
	UpdateDialogLanguage();

	CString strLog;
	strLog.Format(_T("进入[机器人管理员登录]"));
	g_opLogFile.SaveLogText(strLog,2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRobotUserLogin::OnSetfocusEditCode() 
{
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CODE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CODE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowPasswordStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CODE, strOutput);
			CString strLog;
			strLog.Format(_T("[输入密码]为%s\n"),strOutput);
			g_opLogFile.SaveLogText(strLog,3);
		}
	}
}

void CDlgRobotUserLogin::OnOK() 
{
	UpdateData(TRUE);
	CString strLog;
	strLog.Format(_T("点击[确定]"));
	g_opLogFile.SaveLogText(strLog,3);
	strLog.Format(_T("退出[机器人管理员登录]"));
	g_opLogFile.SaveLogText(strLog,2);
	if (m_strPassword != m_strRobotPassword)
	{
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgRobotUserLogin::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;


	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgRobotUserLogin::SetRobotPassword( CString strRobotPassword )
{
	m_strRobotPassword = strRobotPassword;
}


// CDlgRobotUserLogin 消息处理程序
