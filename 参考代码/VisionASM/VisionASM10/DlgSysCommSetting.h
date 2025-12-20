#if !defined(AFX_DLGSYSCOMMTEST_H__BDABE35E_59C6_41B7_A2B1_D6CC04353B1C__INCLUDED_)
#define AFX_DLGSYSCOMMTEST_H__BDABE35E_59C6_41B7_A2B1_D6CC04353B1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysCommTest.h : header file
//
#include "DlgSysBaseCommSetting.h"
#include "vsComm.h"

#include <VECTOR>
/////////////////////////////////////////////////////////////////////////////
// CDlgSysCommSetting dialog
#define IDD_SYS_COMM_CONFIG             1025

class CVisionASMDlg;
class CDlgSysCommSetting : public CDialog
{
// Construction
public:
	CDlgSysCommSetting(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysCommSetting)
	enum { IDD = IDD_SYS_COMM_CONFIG };
	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnShowPlatform1;
	CXPStyleButtonST	m_btnShowPlatform2;
	CXPStyleButtonST	m_btnShowPlatform3;
	CXPStyleButtonST	m_btnShowPlatform4;
	CXPStyleButtonST	m_btnPanel;
	int		m_nPlatformIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysCommSetting)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysCommSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnPlatformChange();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	CVisionASMDlg *m_pParent;
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	int m_nPrevPlatformIndex;

	CStringArray	*m_psaSysInfoStrings;
	std::vector<CDlgSysBaseCommSetting*> m_vpCommSettingDlg;

	void InitDlgItem();
	void UpdateDialogLanguage();

public:
	BOOL		m_bSysKeyboardEnabled;
	CKeyBoard	m_ctrlKeyboard;

	BOOL		m_bStatusBarFlag;
	CString		m_strStatusBarInfo1;
	CString		m_strStatusBarInfo2;

	void UpdateStatusBar();

	virtual void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	virtual void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSCOMMTEST_H__BDABE35E_59C6_41B7_A2B1_D6CC04353B1C__INCLUDED_)
