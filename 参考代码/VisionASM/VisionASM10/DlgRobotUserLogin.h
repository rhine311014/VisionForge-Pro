#pragma once
#include "KeyBoard.h"
#include "SystemOptionConfig.h"

// CDlgRobotUserLogin 对话框

class CDlgRobotUserLogin : public CDialog
{
	DECLARE_DYNAMIC(CDlgRobotUserLogin)

public:
	CDlgRobotUserLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRobotUserLogin();

// 对话框数据
	enum { IDD = IDD_DLG_ROBOT_USER_LOGIN };
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
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysUserLogin)
	virtual void OnOK();
	afx_msg void OnSetfocusEditCode();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CThemeHelperST		m_themeHelper;
	CStringArray		*m_psaSysInfoStrings;
	BOOL				m_bSysKeyboardEnabled;
	CKeyBoard			m_ctrlKeyboard;
	CSystemOptionConfig	m_SysOptionConfig;
	HACCEL m_hAccel;

	void UpdateDialogLanguage();

public:
	CString				m_strRobotPassword;

public:
	void SetRobotPassword(CString strRobotPassword);
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
};
