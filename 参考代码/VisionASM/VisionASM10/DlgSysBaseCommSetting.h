#if !defined(AFX_DLGSYSBASECOMMSETTING_H__40A2E796_391F_4720_8C17_0C8E93746033__INCLUDED_)
#define AFX_DLGSYSBASECOMMSETTING_H__40A2E796_391F_4720_8C17_0C8E93746033__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysBaseCommSetting.h : header file
//
#include "KeyBoard.h"
#include "vsComm.h"
#include "vsBaseVisionAlign.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysBaseCommSetting dialog
class CDlgSysCommSetting;
class CDlgSysEnvironSetting;
class CDlgSysBaseCommSetting : public CDialog
{
// Construction
public:
	CDlgSysBaseCommSetting(CWnd* pParent = NULL);   // standard constructor
	CDlgSysBaseCommSetting(UINT nIDTemplate, CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CDlgSysBaseCommSetting)
	enum { IDD = IDD_SYS_BASE_COMM_CONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysBaseCommSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysBaseCommSetting)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	//CDlgSysCommSetting *m_pParent;
	CDlgSysEnvironSetting *m_pParent;

	BOOL m_bIsCommOpen;
	BOOL m_bExitThread;
	CWinThread* m_pThreadReceiveData;
	HANDLE m_hEventReceiveThreadExit;
	vcBaseVisionAlign *m_pVisionASM;

	HICON m_hIconOpen;
	HICON m_hIconClosed;

	virtual void UpdateDialogLanguage() = 0;

	BOOL m_bVisible;

public:
	virtual void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	virtual void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	
	virtual void InitDlgItem() = 0;
	virtual void UpdateDlgData(BOOL bSaveAndValidate = TRUE) = 0;

	virtual void SetParentWnd(CWnd *pParent) { m_pParent = (CDlgSysEnvironSetting *)pParent;  /*m_pParent = (CDlgSysCommSetting *)pParent;*/}
	virtual void SetVisionASMRef(vcBaseVisionAlign *pVisionASM) { m_pVisionASM = pVisionASM; }
	virtual void ReportCommStatus(BOOL bStatusConnected) = 0;

	virtual void SetVisible(BOOL bVisible){ m_bVisible = bVisible; };
	virtual BOOL IsVisible(){ return m_bVisible;};

	virtual void UpdateCommStatus() = 0;

	virtual void UpdateControlShow(BOOL bVisible) = 0;
	virtual void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	virtual void UpdateDlgByAuthorize() = 0;	// 根据权限刷新界面控件显示隐藏、启用禁用
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSBASECOMMSETTING_H__40A2E796_391F_4720_8C17_0C8E93746033__INCLUDED_)
