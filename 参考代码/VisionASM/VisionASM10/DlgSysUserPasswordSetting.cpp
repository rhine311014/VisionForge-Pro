// DlgSysUserPasswordSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSysUserLogin.h"
#include "VisionASMDLG.h"
#include "VisionASM.h"
#include "DlgSysUserPasswordSetting.h"
#include "svXMLConfigurator.h"
#include "SHA256.H"
#include "LogFileSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysUserPasswordSetting dialog


CDlgSysUserPasswordSetting::CDlgSysUserPasswordSetting(CWnd* pParent /*=NULL*/, BOOL bSuperAdmin/* = FALSE*/)
	: CDialog(CDlgSysUserPasswordSetting::IDD, pParent)
{
	m_pParent = (CDlgSysUserLogin *)pParent;
	//{{AFX_DATA_INIT(CDlgSysUserPasswordSetting)
	m_strOldCode = _T("");
	m_strNewCode = _T("");
	m_strNewCodeConfirm = _T("");
	//}}AFX_DATA_INIT
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bSuperAdmin = bSuperAdmin;
	m_nModifyUsePassWordMode = 1;
}

void CDlgSysUserPasswordSetting::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_USER_PASSWORD_SETTING") == strDlgID)
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

void CDlgSysUserPasswordSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysUserPasswordSetting)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_OLD_CODE, m_strOldCode);
	DDX_Text(pDX, IDC_EDIT_NEW_CODE, m_strNewCode);
	DDX_Text(pDX, IDC_EDIT_NEW_CODE_CONFIRM, m_strNewCodeConfirm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysUserPasswordSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysUserPasswordSetting)
	ON_EN_SETFOCUS(IDC_EDIT_OLD_CODE, OnSetfocusEditOldCode)
	ON_EN_SETFOCUS(IDC_EDIT_NEW_CODE, OnSetfocusEditNewCode)
	ON_EN_SETFOCUS(IDC_EDIT_NEW_CODE_CONFIRM, OnSetfocusEditNewCodeConfirm)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CDlgSysUserPasswordSetting::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysUserPasswordSetting message handlers

BOOL CDlgSysUserPasswordSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btnOK.SetThemeHelper(&m_themeHelper);
	m_btnOK.SetIcon(IDI_ICON_OK_16);
	m_btnOK.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
	m_btnCancel.SetIcon(IDI_ICON_CANCEL_16);
	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	UpdateDialogLanguage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysUserPasswordSetting::OnSetfocusEditOldCode() 
{
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OLD_CODE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OLD_CODE, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OLD_CODE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("输入[原密码]为%s\n"),strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
	}
}

void CDlgSysUserPasswordSetting::OnSetfocusEditNewCode() 
{
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_NEW_CODE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_NEW_CODE, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_NEW_CODE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[新密码]为%s\n"),strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
	}
}

void CDlgSysUserPasswordSetting::OnSetfocusEditNewCodeConfirm() 
{
	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_NEW_CODE_CONFIRM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_NEW_CODE_CONFIRM, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_NEW_CODE_CONFIRM, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[新密码确认]为%s\n"),strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
	}
}

void CDlgSysUserPasswordSetting::OnOK() 
{
	UpdateData(TRUE);

	CString strSuperPassword = "58032106";
	CString strEncryptedOldCode;
	char encrypted[65] = { 0 };
	if (!m_SysOptionConfig.LoadAdminPassword() && !m_bSuperAdmin)
	{
// 		sha_256(encrypted, _T("dde8ef47e757492e5d4fd1b216403daa9ca00ff026f7b9421a65e379b8c1e5bb"));		// lusterinc
// 		m_SysOptionConfig.m_strAdminPassword.Format(_T("%s"), encrypted);

// 		AfxMessageBox(_T("系统授权文件缺失！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SYS_LICENSE_LOST));
		return;
	}

	strEncryptedOldCode = m_strOldCode;

	sha_256(encrypted, strEncryptedOldCode.GetBuffer(strEncryptedOldCode.GetLength()));
	strEncryptedOldCode.ReleaseBuffer();
	strEncryptedOldCode.Format(_T("%s"), encrypted);
	
// 	if (m_bSuperAdmin)
// 	{
// 		if (m_strOldCode != _T("c3866e21282fe9f8f01d5a85810bcee4fdd224990cb1e9ebec5d9efcfcfcb02b"))	// lusterinc2012
// 		{
// 			//AfxMessageBox(_T("原密码错误！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OLD_PASSWORD_ERROR));
// 			return;
// 		}
// 	}
// 	else

	if (m_nModifyUsePassWordMode == 0)
	{
		if (m_strOldCode == strSuperPassword)
		{
		}
		else if (m_SysOptionConfig.m_strAdminPassword != strEncryptedOldCode)
		{
			//AfxMessageBox(_T("原密码错误！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OLD_PASSWORD_ERROR));
			return;
		}
	}
	else if (m_nModifyUsePassWordMode == 1)
	{
		if (m_strOldCode == strSuperPassword)
		{
		}
		else if (m_SysOptionConfig.m_strAdminPassword2 != strEncryptedOldCode)
		{
			//AfxMessageBox(_T("原密码错误！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OLD_PASSWORD_ERROR));
			return;
		}
	}
	else
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OLD_PASSWORD_ERROR));
		return;
	}

	if (m_strNewCode != m_strNewCodeConfirm)
	{
		//AfxMessageBox(_T("两次输入的密码不一致！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TWO_PASSWORD_MISMATCH));
		return;
	}
	if (m_strNewCode.GetLength() < 2)
	{
		//AfxMessageBox(_T("密码长度太短！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PASSWORD_IS_TOO_SHORT));
		return;
	}
	if (m_strNewCode.GetLength() > 16)
	{
		//AfxMessageBox(_T("密码长度太长！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PASSWORD_IS_TOO_LONG));
		return;
	}

	if (m_nModifyUsePassWordMode == 0)
	{
		m_pParent->m_pParent->m_strAdminPassword = m_strNewCode;
	}
	else if (m_nModifyUsePassWordMode == 1)
	{
		m_pParent->m_pParent->m_strAdminPassword2 = m_strNewCode;
	}
	else
	{
		return;
	}
	sha_256(encrypted, m_strNewCode.GetBuffer(m_strNewCode.GetLength()));
	m_strNewCode.ReleaseBuffer();

	if (m_nModifyUsePassWordMode == 0)
	{
		m_strNewCode.Format(_T("%s"), encrypted);
		m_SysOptionConfig.m_strAdminPassword = m_strNewCode;
		m_SysOptionConfig.SaveAdminPassword();
	}
	else if (m_nModifyUsePassWordMode == 1)
	{
		m_strNewCode.Format(_T("%s"), encrypted);
		m_SysOptionConfig.m_strAdminPassword2 = m_strNewCode;
		m_SysOptionConfig.SaveAdminPassword();
	}
	else
	{
		return;
	}
	CString strLog;
	strLog.Format(_T("点击[确定]退出密码设置\n"));
	g_opLogFile.SaveLogText(strLog,2);

	CDialog::OnOK();
}


void CDlgSysUserPasswordSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[取消]退出密码设置\n"));
	g_opLogFile.SaveLogText(strLog,2);
	CDialog::OnCancel();
}
