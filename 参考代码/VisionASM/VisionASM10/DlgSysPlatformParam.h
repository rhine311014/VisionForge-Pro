#if !defined(AFX_DLGSYSPLATFORMPARAM_H__5862E621_CA9F_4F66_B324_E6B87F162131__INCLUDED_)
#define AFX_DLGSYSPLATFORMPARAM_H__5862E621_CA9F_4F66_B324_E6B87F162131__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysPlatformParam.h : header file
//
#include "vsAlignToolDef.h"
#include "KeyBoard.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformParam dialog
class CDlgSysEnvironSetting;
class CDlgSysPlatformParam : public CDialog
{
// Construction
public:
	CDlgSysPlatformParam(CWnd* pParent = NULL);   // standard constructor
	CDlgSysPlatformParam(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CDlgSysPlatformParam();

// Dialog Data
	//{{AFX_DATA(CDlgSysPlatformParam)
 	enum { IDD = IDD_SYS_PLATFORM_PARAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysPlatformParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysPlatformParam)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CPlatformInfo			*m_pPlatformInfo;
	CCamPlatformInfo		*m_pCamPlatformInfo;
	CDlgSysEnvironSetting	*m_pParent;

	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	vcBaseVisionAlign *m_pVisionASM;
	BOOL			m_bCamPlatformInfoEnable;

	virtual void UpdateDialogLanguage() = 0;
	virtual BOOL IsStringNumerical(LPCTSTR lpszVal);
	
public:
	virtual void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	virtual void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	virtual CWnd* SetParentWnd(CDlgSysEnvironSetting *pParent);
	virtual void SetVisionASMRef(vcBaseVisionAlign* pVisionASM);
	virtual void InitDlgItem() = 0;
	virtual void UpdateDlgData(BOOL bSaveAndValidate = TRUE) = 0;
	virtual BOOL SetPlatformInfo(const CPlatformInfo* platformInfo) = 0;
	virtual CPlatformInfo* GetPlatformInfo() const;
	virtual BOOL SetCamPlatformInfo(const CCamPlatformInfo* platformInfo);
	virtual CCamPlatformInfo* GetCamPlatformInfo() const;

	virtual void SetCamPlatformInfoEnable(BOOL bEnable);
	virtual BOOL GetCamPlatformInfoEnable();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSPLATFORMPARAM_H__5862E621_CA9F_4F66_B324_E6B87F162131__INCLUDED_)
