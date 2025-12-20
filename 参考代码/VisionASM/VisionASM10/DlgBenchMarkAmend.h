#if !defined(AFX_DLGBENCHMARKAMEND_H__8457D68F_3BE4_4133_A759_2014AD8FCA0D__INCLUDED_)
#define AFX_DLGBENCHMARKAMEND_H__8457D68F_3BE4_4133_A759_2014AD8FCA0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBenchMarkAmend.h : header file
//

#include "svGuiDisplay.h"
#include "svGuiCross.h"
#include "svGuiInteractiveContainer.h"
#include "vsBaseVisionAlign.h"
#include "LogFileSave.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBenchMarkAmend dialog

class CDlgBenchMarkAmend : public CDialog
{
// Construction
public:
	CDlgBenchMarkAmend(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBenchMarkAmend)
	enum { IDD = IDD_DLG_BENCHMARK };
	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnGrabImage;
	CXPStyleButtonST	m_btnResetMark;
	CXPStyleButtonST	m_btnAmendBenchMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBenchMarkAmend)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBenchMarkAmend)
	afx_msg void OnBtnSerialGrab();
	afx_msg void OnBtnBenchFixture();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnResetMark();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	CWnd *m_pVAPrevMainWnd;
	scGuiDisplay *m_pGuiDisplays;					// 相机采集图像显示控件
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;
	scGuiCross *m_pGuiCross;
	scGuiInteractiveContainer* *m_ppGuiInteractiveContainer;

	vcBaseVisionAlign *m_pVisionASM;
	BOOL m_bIsGrabbing;
	CStringArray *m_psaSysInfoStrings;
	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	void MoveDlgItem();
	void UpdateStatusBar();
	void UpdateDialogLanguage();

public:
	void SetVisionASMRef(vcBaseVisionAlign* pVisionASM);
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBENCHMARKAMEND_H__8457D68F_3BE4_4133_A759_2014AD8FCA0D__INCLUDED_)
