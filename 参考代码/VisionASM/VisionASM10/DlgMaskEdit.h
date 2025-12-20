#if !defined(AFX_DLGMASKEDIT_H__E911C925_5EEE_4874_8904_DF07B6C8EC07__INCLUDED_)
#define AFX_DLGMASKEDIT_H__E911C925_5EEE_4874_8904_DF07B6C8EC07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMaskEdit.h : header file
//
#include "svGuiMaskEdit.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMaskEdit dialog

class CDlgMaskEdit : public CDialog
{
// Construction
public:
	CDlgMaskEdit(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgMaskEdit)
	enum { IDD = IDD_DLG_MASK_EDIT };
	CXPStyleButtonST m_btnApply;
	CXPStyleButtonST m_btnCancel;
	//}}AFX_DATA

	scGuiMaskEdit m_GuiMaskEdit;
	CStringArray* m_psaSysInfoStrings;
	LANGID		  m_lgidLanguage;
	void SetSysInfoStringsRef(CStringArray* psaSysInfoStrings);
	void UpdateDialogLanguage();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMaskEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMaskEdit)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
protected:
	CThemeHelperST		m_themeHelper;
    virtual void OnCancel();
    virtual void OnOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMASKEDIT_H__E911C925_5EEE_4874_8904_DF07B6C8EC07__INCLUDED_)
