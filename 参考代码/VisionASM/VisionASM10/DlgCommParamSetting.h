#pragma once
#include "afxwin.h"
#include "Keyboard.h"
#include "vsVisionAlignDef.h"

// CDlgCommParamSetting 对话框
class CDlgSysEnvironSetting;
class CDlgCommParamSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommParamSetting)

public:
	CDlgCommParamSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCommParamSetting();

// 对话框数据
	enum { IDD = IDD_DLG_COMM_PARAM_SETTING };
public:
	BOOL	m_bSysKeyboardEnabled;
	int		m_nLanguage;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetParentWnd(CDlgSysEnvironSetting *pParent) { m_pParent = pParent; }
	CCommOptionInfo GetCommOptionInfo()const;
	void SetCommOptionInfo(const CCommOptionInfo &commOptionInfo);
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void SetVisionASMRef(vcBaseVisionAlign *pVisionASM){m_pVisionASM = pVisionASM;}
protected:
	CStringArray *m_psaSysInfoStrings;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	CDlgSysEnvironSetting	*m_pParent;
	void UpdateDialogLanguage();
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	void InitDlgItem();

	CCommOptionInfo m_commOptionInfo;
	vcBaseVisionAlign *m_pVisionASM;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nCommXUnitExp;
	int m_nCommYUnitExp;
	int m_nCommDUnitExp;
	afx_msg void OnEnSetfocusEditCommXUnitExp();
	afx_msg void OnEnKillfocusEditCommXUnitExp();
	afx_msg void OnEnSetfocusEditCommYUnitExp();
	afx_msg void OnEnKillfocusEditCommYUnitExp();
	afx_msg void OnEnSetfocusEditCommDUnitExp();
	afx_msg void OnEnKillfocusEditCommDUnitExp();
	int m_nCommDelay;
	int m_nCommTimeout;
	int m_nCommReSendTimes;
	CComboBox m_comboCommDataLenType;
	afx_msg void OnEnSetfocusEditCommTimeout();
	afx_msg void OnEnKillfocusEditCommTimeout();
	afx_msg void OnEnSetfocusEditCommReSendTimes();
	afx_msg void OnEnKillfocusEditCommReSendTimes();
	afx_msg void OnEnSetfocusEditCommDelay();
	afx_msg void OnEnKillfocusEditCommDelay();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboCommDataLenType();
	BOOL m_bUsePlatformComm;
	afx_msg void OnBnClickedChkUsePlatformCommSetting();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用


	bool m_bShowCommParam;
	bool m_bEnableCommParam;
};
