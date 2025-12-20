#if !defined(AFX_DLGPRODUCTRENAME_H__6DDF011C_83BB_41C3_8903_E758A69F6818__INCLUDED_)
#define AFX_DLGPRODUCTRENAME_H__6DDF011C_83BB_41C3_8903_E758A69F6818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProductRename.h : header file
//
#include "KeyBoard.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgProductRename dialog
// class CDlgSysAlignerParam;
// class CDlgSysKindAlignView;
// class CDlgSysKindCalibSetting;
class CDlgProductRename : public CDialog
{
// Construction
public:
	CDlgProductRename(LPCTSTR lpszCurName, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProductRename)
	enum { IDD = IDD_DLG_PRODUCT_RENAME };
	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CString	m_strNewName;
	CString	m_strOldName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProductRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProductRename)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditNewName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	

	void				*m_pParent;
	CStringArray		*m_psaSysInfoStrings;
	BOOL				m_bSysKeyboardEnabled;
	CKeyBoard			m_ctrlKeyboard;

	void UpdateDialogLanguage();

public:
	int					m_nParentType;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	CString GetNewName() const { return m_strNewName; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRODUCTRENAME_H__6DDF011C_83BB_41C3_8903_E758A69F6818__INCLUDED_)
