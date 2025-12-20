#if !defined(AFX_DLGSYSPLATFORMYXPDPARAM_H__533F6FAC_6EFE_477E_938C_FC14EC8EC2F4__INCLUDED_)
#define AFX_DLGSYSPLATFORMYXPDPARAM_H__533F6FAC_6EFE_477E_938C_FC14EC8EC2F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysPlatformYXPDParam.h : header file
//
#include "DlgSysPlatformParam.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformYX1X2PDParam dialog

class CDlgSysPlatformYX1X2PDParam : public CDlgSysPlatformParam
{
// Construction
public:
	CDlgSysPlatformYX1X2PDParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysPlatformYX1X2PDParam)
	enum { IDD = IDD_SYS_PLATFORM_YX1X2PD_PARAM };
	CComboBox	m_comboRDriveType;
	CComboBox	m_comboYDriveDirection;
	CComboBox	m_comboRDriveDirection;
	CComboBox	m_comboCH0AxisXDirection;
	CComboBox	m_comboCH1AxisXDirection;
	CString	m_strRotationLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysPlatformYX1X2PDParam)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysPlatformYX1X2PDParam)
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
	afx_msg void OnEnChangeEditRotationLength();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSPLATFORMYXPDPARAM_H__533F6FAC_6EFE_477E_938C_FC14EC8EC2F4__INCLUDED_)
