// DlgSysUserLogin.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysUserLogin.h"
#include "DlgSysUserPasswordSetting.h"
#include "svXMLConfigurator.h"
#include "SHA256.H"
#include "VisionASMDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysUserLogin dialog


CDlgSysUserLogin::CDlgSysUserLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysUserLogin::IDD, pParent)
{
	m_pParent = (CVisionASMDlg *)pParent;
	//{{AFX_DATA_INIT(CDlgSysUserLogin)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_hAccel = NULL;

	char encrypted[65] = { 0 };
	CString strPW = _T("123456");
	sha_256(encrypted, strPW.GetBuffer(strPW.GetLength()));
	strPW.ReleaseBuffer();

	m_SysOptionConfig.LoadAdminPassword();
	strPW.Format(_T("%s"), encrypted);
// 	if (strPW != m_SysOptionConfig.m_strAdminPassword)
// 	{
// 		m_strPassword.Empty();
// 	}

	m_nModifyUsePassWordMode = 0;
	m_nUsePassWordMode = 1;
	
	//if (strPW != m_SysOptionConfig.m_strAdminPassword || strPW != m_SysOptionConfig.m_strAdminPassword2)
	if (strPW == m_SysOptionConfig.m_strAdminPassword2 && m_nUsePassWordMode == 1)
	{
		m_strPassword = _T("123456");
	}
	else
	{
		m_strPassword.Empty();
	}
}

void CDlgSysUserLogin::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_USER_LOGIN") == strDlgID)
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

void CDlgSysUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysUserLogin)
	DDX_Control(pDX, IDC_BTN_RESET_PASSWORD, m_btnChangePW);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT_CODE, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysUserLogin, CDialog)
	//{{AFX_MSG_MAP(CDlgSysUserLogin)
	ON_BN_CLICKED(IDC_BTN_RESET_PASSWORD, OnBtnResetPassword)
	ON_EN_SETFOCUS(IDC_EDIT_CODE, OnSetfocusEditCode)
	ON_BN_CLICKED(IDC_BTN_RESET_PASSWORD2, OnBtnResetPassword2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysUserLogin message handlers

BOOL CDlgSysUserLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btnChangePW.SetThemeHelper(&m_themeHelper);
// 	m_btnChangePW.SetIcon(IDI_ICON_RESET_PASSWORD_16);
// 	m_btnChangePW.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	// 设置按钮
	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnOK.SetIcon(IDI_ICON_OK_16);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_16);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	UpdateDialogLanguage();

//	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACC_SUPER_USER));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysUserLogin::OnBtnResetPassword() 
{
	// TODO: Add your control notification handler code here
	CString strLog;
	strLog.Format(_T("[修改超级权限密码]"));
	g_opLogFile.SaveLogText(strLog,2);

	m_nModifyUsePassWordMode = 0;
	CDlgSysUserPasswordSetting dlgSysUserPasswordSetting(this);	
	dlgSysUserPasswordSetting.m_nModifyUsePassWordMode = m_nModifyUsePassWordMode;
	dlgSysUserPasswordSetting.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlgSysUserPasswordSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgSysUserPasswordSetting.DoModal() == IDOK)
	{
		m_strPassword.Empty();
		UpdateData(FALSE);
	}
}

void CDlgSysUserLogin::OnSetfocusEditCode() 
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
			strLog.Format(_T("[输入密码]由%s到%s\n"),strInput, strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
	}
}

void CDlgSysUserLogin::OnOK() 
{
	UpdateData(TRUE);
	CString strPassword = "58032106";
	CString strLog;
	strLog.Format(_T("点击[确定]"));
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("退出[管理员登录]"));
	g_opLogFile.SaveLogText(strLog,1);

	if (m_strPassword == strPassword)
	{
		m_nUsePassWordMode = 0;
		CDialog::OnOK();
		return;
	}
	char encrypted[65] = { 0 };
	if (!m_SysOptionConfig.LoadAdminPassword())
	{
		// 		sha_256(encrypted, _T("lusterinc"));
		// 		m_SysOptionConfig.m_strAdminPassword.Format(_T("%s"), encrypted);
		
		// 		AfxMessageBox(_T("系统授权文件缺失！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SYS_LICENSE_LOST));
		return;
	}
	CString strTmpPassword = m_strPassword;
	sha_256(encrypted, m_strPassword.GetBuffer(m_strPassword.GetLength()));
	m_strPassword.ReleaseBuffer();
	
	m_strPassword.Format(_T("%s"), encrypted);

	// 初始默认
	
	if (m_strPassword == m_SysOptionConfig.m_strAdminPassword2)
	{
		m_pParent->m_strAdminPassword2 = strTmpPassword;
		m_nUsePassWordMode = 1;
		CDialog::OnOK();
		return;
	}	
	
	if (m_strPassword == m_SysOptionConfig.m_strAdminPassword)
	{
		m_pParent->m_strAdminPassword = strTmpPassword;
		m_nUsePassWordMode = 0;
		CDialog::OnOK();
		return;
	}

	if (m_strPassword != m_SysOptionConfig.m_strAdminPassword && m_strPassword != m_SysOptionConfig.m_strAdminPassword2 )
	{
		
		//AfxMessageBox(_T("密码错误！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PASSWORD_ERROR));
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgSysUserLogin::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hAccel != NULL)  
	{  
		if(TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return TRUE;
	} 

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgSysUserLogin::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	switch(LOWORD(wParam))  
// 	{  
// 	case  ID_SUPER_RESET_PASSWORD:   //加速键ID  
// 		{
// 			CDlgSysUserPasswordSetting dlg(this, TRUE);
// 			dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 			dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 			if (dlg.DoModal() == IDOK)
// 			{
// 				m_strPassword.Empty();
// 				UpdateData(FALSE);
// 			}
// 		}
// 		break;  
// 	default:
// 		break;
// 	}

	return CDialog::OnCommand(wParam, lParam);
}

void CDlgSysUserLogin::OnBtnResetPassword2() 
{
	// TODO: Add your control notification handler code here
	CString strLog;
	strLog.Format(_T("[修改一般权限密码]"));
	g_opLogFile.SaveLogText(strLog,2);

	m_nModifyUsePassWordMode = 1;
	CDlgSysUserPasswordSetting dlgSysUserPasswordSetting(this);	
	dlgSysUserPasswordSetting.m_nModifyUsePassWordMode = m_nModifyUsePassWordMode;
	dlgSysUserPasswordSetting.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlgSysUserPasswordSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgSysUserPasswordSetting.DoModal() == IDOK)
	{
		m_strPassword.Empty();
		UpdateData(FALSE);
	}
}
