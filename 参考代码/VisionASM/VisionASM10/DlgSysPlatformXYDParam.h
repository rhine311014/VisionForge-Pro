#if !defined(AFX_DLGSYSPLATFORMXYDPARAM_H__59040C84_6A10_4F19_9C26_BBF32C1D30DF__INCLUDED_)
#define AFX_DLGSYSPLATFORMXYDPARAM_H__59040C84_6A10_4F19_9C26_BBF32C1D30DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysPlatformXYDParam.h : header file
//
#include "DlgSysPlatformParam.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformXYDParam dialog

class CDlgSysPlatformXYDParam : public CDlgSysPlatformParam
{
// Construction
public:
	CDlgSysPlatformXYDParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysPlatformXYDParam)
	enum { IDD = IDD_SYS_PLATFORM_XYD_PARAM };
	CComboBox	m_comboRDriveType;
	CComboBox	m_comboRDriveLineType;
	CComboBox	m_comboYDriveDirection;
	CComboBox	m_comboXDriveDirection;
	CComboBox	m_comboRDriveDirection;
	CComboBox	m_comboCamAxisYDirection;
	CComboBox	m_comboCamAxisXDirection;
	CComboBox	m_comboCamPlatformAxisYDirection;
	CComboBox	m_comboCamPlatformAxisXDirection;
	int		m_nCameraIndex;
	CString	m_strRotationLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysPlatformXYDParam)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysPlatformXYDParam)
	virtual BOOL OnInitDialog();
	afx_msg void OnCameraChange();
	afx_msg void OnSelchangeComboCamXCoordType();
	afx_msg void OnSelchangeComboCamYCoordType();
	afx_msg void OnSelchangeComboCamXCoordType2();
	afx_msg void OnSelchangeComboCamYCoordType2();
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
	CString strComboCamXCoordTypeOld;
	CString strComboCamYCoordTypeOld;
	CString strComboXCoordTypeOld;
	CString strComboYCoordTypeOld;
	CString strComboRCoordTypeOld;
	CString strComboDriveTypeOld;
	CString strComboDriveLineTypeOld;
	afx_msg void OnCbnSelchangeComboDriveType();
	afx_msg void OnCbnSelchangeComboXCoordType();
	afx_msg void OnCbnSelchangeComboYCoordType();
	afx_msg void OnCbnSelchangeComboRDirection();
	afx_msg void OnCbnSelchangeComboDriveLineType();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

	bool m_bshowPlatformParamSet;
	bool m_bEnablePlatformParamSet;

	bool m_bshowCamPlatformParamSet;
	bool m_bEnableCamPlatformParamSet;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSPLATFORMXYDPARAM_H__59040C84_6A10_4F19_9C26_BBF32C1D30DF__INCLUDED_)
