#if !defined(AFX_DLGSYSOPTIONSETTING_H__A3B16E5B_F2FB_44EF_9CC5_48E8AA3054B8__INCLUDED_)
#define AFX_DLGSYSOPTIONSETTING_H__A3B16E5B_F2FB_44EF_9CC5_48E8AA3054B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysOptionSetting.h : header file
//

#include "SystemOptionConfig.h"
#include "afxwin.h"
#include "KeyBoard.h"
#include "Label.h"
#include "vsEn_Decrypt.h"
#include "DlgSysRegister.h"
#include "DlgPlatformLayoutMode.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSysOptionSetting dialog

class CVisionASMDlg;
class CDlgSysOptionSetting : public CDialog
{
// Construction
public:
	CDlgSysOptionSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysOptionSetting)
	enum { IDD = IDD_SYS_OPTION_SETTING };
	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST m_btnApply;
	CXPStyleButtonST	m_btnPanel;
	BOOL	m_bSysKeyboardEnabled;
	int		m_nLanguage;
	BOOL	m_bStartUpShowCamSNSetting;
	//}}AFX_DATA

	void SetSystemOptionConfig(CSystemOptionConfig& systemOptionConfig);
	void GetSystemOptionConfig(CSystemOptionConfig& systemOptionConfig);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysOptionSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysOptionSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST	m_themeHelper;
	CStringArray *m_psaSysInfoStrings;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	void InitDlgItem();
	void MoveDlgItem();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void UpdateDialogLanguage();
	BOOL IsStringNumerical(LPCTSTR lpszVal);

	void InitPlatformModeShow();
private:
 	CDlgPlatformLayoutMode* m_pDlgPlatformLayoutMode;
 	std::vector<CLayout*> m_vpWsTypeLayout;

//
public:
	BOOL	m_bSelNewSystem;
	CVisionASMDlg *m_pParent;
	CSystemOptionConfig m_SysOptionConfig;
	CSystemOptionConfig m_TempSysOptionConfig;
public:
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	//CComboBox m_comboCameraMode;
	CComboBox m_comboDisplayMode;
	CComboBox m_comboShowRunCommandDetail;
	int m_nCommTimeout;
	int m_nCommReSendTimes;
	int m_nCommXYUnitExp;
	int m_nCommDUnitExp;
	CComboBox m_comboCommDataLenType;

	BOOL m_bSaveAlignPos;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEditCommTimeout();
	afx_msg void OnEnKillfocusEditCommTimeout();
	afx_msg void OnEnSetfocusEditCommReSendTimes();
	afx_msg void OnEnKillfocusEditCommReSendTimes();
	afx_msg void OnEnSetfocusEditCommXYUnitExp();
	afx_msg void OnEnKillfocusEditCommXYUnitExp();
	afx_msg void OnEnSetfocusEditCommDUnitExp();
	afx_msg void OnEnKillfocusEditCommDUnitExp();

	afx_msg void OnBnClickedCheckSysKeyboard();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CComboBox m_comboSystemIndex;
	CLabel	m_labelNote;
	BOOL m_bMainframeSetAlignParam;
	afx_msg void OnBnClickedBntRegister();

	int m_nCommDelay;
	afx_msg void OnEnKillfocusEditCommDelay();
	afx_msg void OnEnSetfocusEditCommDelay();

	BOOL m_bShowSepTargetPos;
	afx_msg void OnBnClickedBtnSystemInfo();

	//BOOL m_bShowProductInfo;
	//BOOL m_bShowOffsetDirection;//主界面显示补偿方向

	afx_msg void OnBnClickedBtnCrossSetting();
	afx_msg void OnBnClickedRadioChineseSimplified();
	afx_msg void OnBnClickedCheckCameraSnSetting();

	CString strComboDisplayModeOld;
	CString strComboShowRunCommandDetailOld;
	CString strComboCommDataLenTypeOld;
	CString strComboSystemIndexOld;

	afx_msg void OnCbnSelchangeComboDisplayMode();
	afx_msg void OnCbnSelchangeComboShowRunCommandDetail();
	afx_msg void OnCbnSelchangeComboCommDataLenType();
	afx_msg void OnCbnSelchangeComboSystemIndex();
	afx_msg void OnBnClickedCheckMainframeSetAlignParam();
	afx_msg void OnBnClickedCheckShowProductInfo();
	afx_msg void OnBnClickedCheckShowSepTargetPos();
	afx_msg void OnBnClickedCancel();

	BOOL m_bChange2um;
    BOOL m_bChangeProductSize2um;

    afx_msg void OnBnClickedCheckChangeToUm();
    afx_msg void OnBnClickedChkChangesizeToUm();

	BOOL m_bShowGamma;
	BOOL m_bCheckModeOnly;
	//BOOL m_bCheckDPMI;
	afx_msg void OnBnClickedBtnSysResStatistics();

	int m_nRdWsLayoutType;

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSearchGraphSetting();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

	bool m_bShowSysResStatistics;

	bool m_bShowWSLayout;
	bool m_bEnableWSLayout;

	bool m_bShowOtherSet;
	bool m_bEnableOtherSet;


	BOOL m_bChkShowFullScreen;
	int m_nLoginTimeSet;
	afx_msg void OnEnKillfocusEditLoginTimeSet();
	afx_msg void OnEnSetfocusEditLoginTimeSet();
	// 将数据文件和VDB图片分别整合到一个文件夹下
	BOOL m_bAllSteoage;
	afx_msg void OnBnClickedCheckMainframeSetBendParam();
	BOOL m_bMainframeSetBendParam;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSOPTIONSETTING_H__A3B16E5B_F2FB_44EF_9CC5_48E8AA3054B8__INCLUDED_)
