#if !defined(AFX_DLGSYSPLATFORMXY1Y2PARAM_H__8A313BE9_C501_4E4C_A8F3_69D584F63590__INCLUDED_)
#define AFX_DLGSYSPLATFORMXY1Y2PARAM_H__8A313BE9_C501_4E4C_A8F3_69D584F63590__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysPlatformXY1Y2Param.h : header file
//
#include "DlgSysPlatformParam.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformXY1Y2Param dialog

class CDlgSysPlatformXY1Y2Param : public CDlgSysPlatformParam
{
// Construction
public:
	CDlgSysPlatformXY1Y2Param(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysPlatformXY1Y2Param)
	enum { IDD = IDD_SYS_PLATFORM_XY1Y2_PARAM };
	CComboBox	m_comboUVWSliderType;
	CComboBox	m_comboY2DriveDirection;
	CComboBox	m_comboY1DriveDirection;
	CComboBox	m_comboXDriveDirection;
	CComboBox	m_comboCamAxisYDirection;
	CComboBox	m_comboCamAxisXDirection;
	int		m_nCameraIndex;
	CString	m_strXCoordX;
	CString	m_strXCoordY;
	CString	m_strY1CoordX;
	CString	m_strY1CoordY;
	CString	m_strY2CoordX;
	CString	m_strY2CoordY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysPlatformXY1Y2Param)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysPlatformXY1Y2Param)
	virtual BOOL OnInitDialog();
	afx_msg void OnCameraChange();
	afx_msg void OnSelchangeComboCamXCoordType();
	afx_msg void OnSelchangeComboCamYCoordType();
	afx_msg void OnSetfocusEditXCoordX();
	afx_msg void OnSetfocusEditXCoordY();
	afx_msg void OnSetfocusEditY1CoordX();
	afx_msg void OnSetfocusEditY1CoordY();
	afx_msg void OnSetfocusEditY2CoordX();
	afx_msg void OnSetfocusEditY2CoordY();
	afx_msg void OnKillfocusEditXCoordX();
	afx_msg void OnKillfocusEditXCoordY();
	afx_msg void OnKillfocusEditY1CoordX();
	afx_msg void OnKillfocusEditY1CoordY();
	afx_msg void OnKillfocusEditY2CoordX();
	afx_msg void OnKillfocusEditY2CoordY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void UpdateDialogLanguage();
	
public:
	void InitDlgItem();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	virtual BOOL SetPlatformInfo(const CPlatformInfo* platformInfo);
	CString strComboUvwSliderTypeOld;
	CString strComboXDriveDirectionOld;
	CString strComboY1DriveDirectionOld;
	CString strComboY2DriveDirectionOld;
	CString strComboCamXCoordTypeOld;
	CString strComboCamYCoordTypeOld;
	afx_msg void OnCbnSelchangeComboUvwSliderType();
	afx_msg void OnCbnSelchangeComboXDriveDirection();
	afx_msg void OnCbnSelchangeComboY1DriveDirection();
	afx_msg void OnCbnSelchangeComboY2DriveDirection();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSPLATFORMXY1Y2PARAM_H__8A313BE9_C501_4E4C_A8F3_69D584F63590__INCLUDED_)
