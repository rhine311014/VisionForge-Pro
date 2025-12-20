#if !defined(AFX_DLGSYSENVIRONSETTING_H__B87A2DF4_CA44_475F_AACF_4CC0BE329EBA__INCLUDED_)
#define AFX_DLGSYSENVIRONSETTING_H__B87A2DF4_CA44_475F_AACF_4CC0BE329EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysEnvironSetting.h : header file
//

#define MaxPlatformNum	6	// 最大的平台数量

#include "DlgSysImgRoiParam.h"
#include "DlgSysPlatformXYDParam.h"
#include "DlgSysPlatformYDPX1X2Param.h"
#include "DlgSysPlatformYX1X2PDParam.h"
#include "DlgSysPlatformX1X2YParam.h"
#include "DlgSysPlatformXY1Y2Param.h"
#include "DlgSysDataRecord.h"
#include "DlgSysPlatformOptionParam.h"
#include "DlgCommParamSetting.h"
#include "DlgSysLayoutParamSetting.h"

#include "DlgSysBaseCommSetting.h"
#include "vsComm.h"
#include "DlgCommTypeSetting.h"
#include "DlgAuthorizeSet.h"
#include "SystemOptionConfig.h"

#include "svGuiCross.h"

#include "LogFileSave.h" 
/////////////////////////////////////////////////////////////////////////////
// CDlgSysEnvironSetting dialog


// #define TABSHEET_ITEM_CAMERA	_T("相机")
// #define TABSHEET_ITEM_PLATFORM	_T("平台")
// #define TABSHEET_ITEM_COMM		_T("通信")
// #define TABSHEET_ITEM_LOG		_T("系统")

class CVisionASMDlg;
class CDlgSysEnvironSetting : public CDialog
{
// Construction
public:
	CDlgSysEnvironSetting(CWnd* pParent);   // standard constructor
	~CDlgSysEnvironSetting();

// Dialog Data
	//{{AFX_DATA(CDlgSysEnvironSetting)
	enum { IDD = IDD_SYS_ENVIRONMENT_SETTING };
	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST m_btnApply;
	CXPStyleButtonST	m_btnShowPlatform1;
	CXPStyleButtonST	m_btnShowPlatform2;
	CXPStyleButtonST	m_btnShowPlatform3;
	CXPStyleButtonST	m_btnShowPlatform4;
	CXPStyleButtonST	m_btnShowPlatform5;
	CXPStyleButtonST	m_btnShowPlatform6;
	CXPStyleButtonST	m_btnPanel;
	CXPStyleButtonST	m_btnEnvironCam;
	CXPStyleButtonST	m_btnEnvironComm;
	CXPStyleButtonST	m_btnEnvironPlatform;
	CXPStyleButtonST	m_btnEnvironLog;
	CXPStyleButtonST	m_btnEnvironSearchTool;
	CXPStyleButtonST	m_btnEnvironLayout;
	CXPStyleButtonST	m_btnEnvironAuthorizeSet;

	CTabCtrl	m_tabSheet;
	int		m_nPlatformIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysEnvironSetting)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysEnvironSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPlatformChange();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayDragStop(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	int m_nPrevPlatformIndex;
	int m_nPrevTabIndex;

// 	CArray<scGuiDisplay*, scGuiDisplay*> *m_paOldGuiDisplay;

	CDlgSysDataRecord m_dlgSysDataRecord;
	CDlgSysImgRoiParam m_dlgSysImgRoiParam;
	CDlgSysPlatformOptionParam m_dlgPlatformOptionParam;
	CDlgCommParamSetting m_dlgCommParamSetting;
	CDlgSysLayoutParamSetting	m_dlgSysLayoutParamSetting;
	std::vector<CDlgCommTypeSetting*> m_vpdlgCommTypeSetting;

	std::vector<CDlgSysPlatformParam*> m_vpDlgSysPlatformParam;

	std::vector<CDlgSysPlatformParam*> m_vpDlgSysMidPlatformParam;

	std::vector<CDlgSysBaseCommSetting*> m_vpCommSettingDlg;

	std::vector<CDlgAuthorizeSet*> m_vpAuthorizeSetDlg;

	std::vector<CWnd*> m_vpVAPrevMainWnd;		// 各视觉对位工具的旧主窗口（消息发送目标窗口）

	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	int m_nFitImage;                        //控制第一次图片显示为适应屏幕

public:
	CVisionASMDlg *m_pParent;
	scGuiDisplay *m_pGuiDisplay;
	CSystemOptionConfig m_SysOptionConfig;
	std::vector<scGuiCross*> m_vpGuiCross;

public:
	// 系统选项相关
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	void UpdateDialogLanguage();

public:
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	void InitDlgItem();
	void MoveDlgItem();
	void UpdateStatusBar();
	void OnBnClickedRadioEnviron();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRadioEnvironCam();
	afx_msg void OnBnClickedRadioEnvironComm();
	afx_msg void OnBnClickedRadioEnvironPlatform();
	afx_msg void OnBnClickedRadioEnvironLog();
	afx_msg void OnBnClickedRadioEnvironSearchTool();
	afx_msg void OnBnClickedRadioEnvironLayout();
	afx_msg void OnBnClickedRadioEnvironAuthorizeset();
	int m_nRadioEnvironType;
	void UpdateControlShow(BOOL bVisible);

	CRect m_scRectForCommDlg; // 网口、串口通信显示区域
	BOOL ChangeCommType(const CommType eCommType); // 改变通信连接类型 
	afx_msg void OnStnClickedStaticRadioPanel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSENVIRONSETTING_H__B87A2DF4_CA44_475F_AACF_4CC0BE329EBA__INCLUDED_)
