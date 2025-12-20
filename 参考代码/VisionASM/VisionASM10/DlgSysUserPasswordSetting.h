#if !defined(AFX_DLGSYSUSERPASSWORDSETTING_H__04347BF8_3329_4EEC_BF5A_941513D3521B__INCLUDED_)
#define AFX_DLGSYSUSERPASSWORDSETTING_H__04347BF8_3329_4EEC_BF5A_941513D3521B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysUserPasswordSetting.h : header file
//
#include "KeyBoard.h"
#include "SystemOptionConfig.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysUserPasswordSetting dialog
class CDlgSysUserLogin;
class CDlgSysUserPasswordSetting : public CDialog
{
// Construction
public:
	CDlgSysUserPasswordSetting(CWnd* pParent = NULL, BOOL bSuperAdmin = FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysUserPasswordSetting)
	enum { IDD = IDD_SYS_USER_PASSWORD_SETTING };
	CXPStyleButtonST m_btnOK;
	CXPStyleButtonST m_btnCancel;
	CString	m_strOldCode;
	CString	m_strNewCode;
	CString	m_strNewCodeConfirm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysUserPasswordSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysUserPasswordSetting)
	virtual void OnOK();
	afx_msg void OnSetfocusEditOldCode();
	afx_msg void OnSetfocusEditNewCode();
	afx_msg void OnSetfocusEditNewCodeConfirm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	CStringArray		*m_psaSysInfoStrings;
	BOOL				m_bSysKeyboardEnabled;
	CKeyBoard			m_ctrlKeyboard;
	CSystemOptionConfig	m_SysOptionConfig;
	BOOL				m_bSuperAdmin;
	void UpdateDialogLanguage();
public:
	int					m_nModifyUsePassWordMode;	//0超级权限、1一般权限
	CDlgSysUserLogin	*m_pParent;
public:
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSUSERPASSWORDSETTING_H__04347BF8_3329_4EEC_BF5A_941513D3521B__INCLUDED_)
