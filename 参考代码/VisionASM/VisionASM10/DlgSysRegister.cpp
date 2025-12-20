// DlgSysRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysRegister.h"
#include "afxdialogex.h"
#include "base64.h"
#include "svXMLConfigurator.h"


// CDlgSysRegister 对话框

IMPLEMENT_DYNAMIC(CDlgSysRegister, CDialog)

CDlgSysRegister::CDlgSysRegister(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysRegister::IDD, pParent)
	, m_strRegisterCode(_T(""))
	, m_strID(_T("16100"))
{
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bRegisterSuccess = FALSE;

	m_pParent = NULL;
}

CDlgSysRegister::~CDlgSysRegister()
{
}

void CDlgSysRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_strRegisterCode);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
}


BEGIN_MESSAGE_MAP(CDlgSysRegister, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT_CODE, &CDlgSysRegister::OnEnSetfocusEditCode)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CDlgSysRegister::OnBnClickedBtnRegister)
	ON_EN_SETFOCUS(IDC_EDIT_ID, &CDlgSysRegister::OnEnSetfocusEditId)
	ON_BN_CLICKED(IDOK, &CDlgSysRegister::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSysRegister::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSysRegister 消息处理程序





void CDlgSysRegister::OnEnSetfocusEditCode()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CODE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CODE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CODE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("[输入注册码]%s"),strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
	}
}


void CDlgSysRegister::OnBnClickedBtnRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strLog;
	
	{
		CString strExpiredate;
		vcAuthorizeTool authorizeTool;
		if (!m_pParent->m_AuthorizeUsbTool.Register(m_strRegisterCode,strExpiredate))
		{
			m_bRegisterSuccess = FALSE;
			AfxMessageBox(_T("注册失败!"));
			strLog.Format(_T("点击[注册]，注册失败"));
			g_opLogFile.SaveLogText(strLog,2);
			return;
		}

		m_bRegisterSuccess = TRUE;
		CString strInfo;
		strInfo = _T("注册成功！注册日期：") + strExpiredate;
		AfxMessageBox(strInfo);
		strLog.Format(_T("点击[注册]，注册成功"));
		g_opLogFile.SaveLogText(strLog,2);
	}
}



void CDlgSysRegister::OnEnSetfocusEditId()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ID)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ID, strInput);

		if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ID, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("输入[ID号]%s"),strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
	}
}


BOOL CDlgSysRegister::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	{
		m_pParent->m_AuthorizeUsbTool.GetUseDongleID(m_strID);
	}

	UpdateData(FALSE);

	UpdateDialogLanguage();

	CString strLog;
	strLog.Format(_T("进入[软件注册]界面"));
	g_opLogFile.SaveLogText(strLog,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSysRegister::UpdateDialogLanguage()
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

		if(_T("IDD_SYS_SOFT_REGISTER") == strDlgID)
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

void CDlgSysRegister::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SysPlatformInfo locSysPlatformInfo;
	for (int i=0; i<m_pParent->m_nPlatformNum; i++)
	{
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(i, locSysPlatformInfo);			
		locSysPlatformInfo.m_bRegisterSuccess = m_bRegisterSuccess;
		m_pParent->m_SystemConfigure.SetSysPlatformInfo(i, locSysPlatformInfo);
	}
	CString strLog;
	strLog.Format(_T("点击[退出]退出软件注册界面"));
	g_opLogFile.SaveLogText(strLog,1);
	CDialog::OnOK();
}


void CDlgSysRegister::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[取消]退出软件注册界面"));
	g_opLogFile.SaveLogText(strLog,1);
	CDialog::OnCancel();
}
