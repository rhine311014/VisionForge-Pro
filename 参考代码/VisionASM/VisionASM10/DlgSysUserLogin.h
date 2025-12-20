#if !defined(AFX_DLGSYSUSERLOGIN_H__40E8A5C9_1D9C_4D28_8E09_D676AD92FEE6__INCLUDED_)
#define AFX_DLGSYSUSERLOGIN_H__40E8A5C9_1D9C_4D28_8E09_D676AD92FEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysUserLogin.h : header file
//
#include "KeyBoard.h"
#include "SystemOptionConfig.h"
#include "LogFileSave.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSysUserLogin dialog
class CVisionASMDlg;
class CDlgSysUserLogin : public CDialog
{
// Construction
public:
	CDlgSysUserLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysUserLogin)
	enum { IDD = IDD_SYS_USER_LOGIN };
	CXPStyleButtonST	m_btnChangePW;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnOK;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysUserLogin)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysUserLogin)
	afx_msg void OnBtnResetPassword();
	virtual void OnOK();
	afx_msg void OnSetfocusEditCode();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnResetPassword2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int					m_nModifyUsePassWordMode;	//0超级权限、1一般权限
	int					m_nUsePassWordMode;			//根据输入的密码确定使用的权限。0超级权限、1一般权限

	CVisionASMDlg *m_pParent;

protected:
	CThemeHelperST		m_themeHelper;
	CStringArray		*m_psaSysInfoStrings;
	BOOL				m_bSysKeyboardEnabled;
	CKeyBoard			m_ctrlKeyboard;
	CSystemOptionConfig	m_SysOptionConfig;
	HACCEL m_hAccel;

	void UpdateDialogLanguage();

public:
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSUSERLOGIN_H__40E8A5C9_1D9C_4D28_8E09_D676AD92FEE6__INCLUDED_)
