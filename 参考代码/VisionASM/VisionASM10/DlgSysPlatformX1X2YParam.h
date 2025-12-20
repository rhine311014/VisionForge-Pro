#if !defined(AFX_DLGSYSPLATFORMX1X2YPARAM_H__E4C0330B_EA5B_4111_AF11_D877BDAC6C6B__INCLUDED_)
#define AFX_DLGSYSPLATFORMX1X2YPARAM_H__E4C0330B_EA5B_4111_AF11_D877BDAC6C6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysPlatformX1X2YParam.h : header file
//
#include "DlgSysPlatformParam.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformX1X2YParam dialog

class CDlgSysPlatformX1X2YParam : public CDlgSysPlatformParam
{
// Construction
public:
	CDlgSysPlatformX1X2YParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysPlatformX1X2YParam)
	enum { IDD = IDD_SYS_PLATFORM_X1X2Y_PARAM };
	CComboBox	m_comboUVWSliderType;
	CComboBox	m_comboYDriveDirection;
	CComboBox	m_comboX2DriveDirection;
	CComboBox	m_comboX1DriveDirection;
	CComboBox	m_comboCamAxisYDirection;
	CComboBox	m_comboCamAxisXDirection;
	int		m_nCameraIndex;
	CString	m_strX1CoordX;
	CString	m_strX1CoordY;
	CString	m_strX2CoordX;
	CString	m_strX2CoordY;
	CString	m_strYCoordX;
	CString	m_strYCoordY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysPlatformX1X2YParam)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysPlatformX1X2YParam)
	virtual BOOL OnInitDialog();
	afx_msg void OnCameraChange();
	afx_msg void OnSelchangeComboCamXCoordType();
	afx_msg void OnSelchangeComboCamYCoordType();
	afx_msg void OnSetfocusEditX1CoordX();
	afx_msg void OnSetfocusEditX1CoordY();
	afx_msg void OnSetfocusEditX2CoordX();
	afx_msg void OnSetfocusEditX2CoordY();
	afx_msg void OnSetfocusEditYCoordX();
	afx_msg void OnSetfocusEditYCoordY();
	afx_msg void OnKillfocusEditX1CoordX();
	afx_msg void OnKillfocusEditX1CoordY();
	afx_msg void OnKillfocusEditX2CoordX();
	afx_msg void OnKillfocusEditX2CoordY();
	afx_msg void OnKillfocusEditYCoordX();
	afx_msg void OnKillfocusEditYCoordY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void UpdateDialogLanguage();

public:
	void InitDlgItem();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	virtual BOOL SetPlatformInfo(const CPlatformInfo* platformInfo);
	CString strComboUvwSliderTypeOld;
	CString strComboX1DriveDirectionOld;
	CString strComboX2DriveDirectionOld;
	CString strComboYDriveDirectionOld;
	CString strComboCamXCoordTypeOld;
	CString strComboCamYCoordTypeOld;
	afx_msg void OnCbnSelchangeComboUvwSliderType();
	afx_msg void OnCbnSelchangeComboX1DriveDirection();
	afx_msg void OnCbnSelchangeComboX2DriveDirection();
	afx_msg void OnCbnSelchangeComboYDriveDirection();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSPLATFORMX1X2YPARAM_H__E4C0330B_EA5B_4111_AF11_D877BDAC6C6B__INCLUDED_)
