#if !defined(AFX_DLGSYSPLATFORMYDPXPARAM_H__5CBC5763_0306_4FDE_9B80_01DBBA5C284C__INCLUDED_)
#define AFX_DLGSYSPLATFORMYDPXPARAM_H__5CBC5763_0306_4FDE_9B80_01DBBA5C284C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysPlatformYDPXParam.h : header file
//
#include "DlgSysPlatformParam.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformYDPX1X2Param dialog

class CDlgSysPlatformYDPX1X2Param : public CDlgSysPlatformParam
{
// Construction
public:
	CDlgSysPlatformYDPX1X2Param(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysPlatformYDPX1X2Param)
	enum { IDD = IDD_SYS_PLATFORM_YDPX1X2_PARAM };
	CComboBox	m_comboCH1AxisXDirection;
	CComboBox	m_comboCH0AxisXDirection;
	CComboBox	m_comboRDriveType;
	CComboBox	m_comboRDriveDirection;
	CComboBox	m_comboYDriveDirection;
	CString	m_strRotationLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysPlatformYDPX1X2Param)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysPlatformYDPX1X2Param)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditRotationLength();
	afx_msg void OnKillfocusEditRotationLength();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void UpdateDialogLanguage();
	
public:
	void InitDlgItem();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	virtual BOOL SetPlatformInfo(const CPlatformInfo* platformInfo);
	CString strComboX1DriveDirectionOld;
	CString strComboX2DriveDirectionOld;
	CString strComboYDriveDirectionOld;
	CString strComboDriveTypeOld;
	CString strComboRCoordTypeOld;
	afx_msg void OnCbnSelchangeComboCh0xCoordType();
	afx_msg void OnCbnSelchangeComboCh1xCoordType();
	afx_msg void OnCbnSelchangeComboYCoordType();
	afx_msg void OnCbnSelchangeComboRDirection();
	afx_msg void OnCbnSelchangeComboDriveType();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSPLATFORMYDPXPARAM_H__5CBC5763_0306_4FDE_9B80_01DBBA5C284C__INCLUDED_)
