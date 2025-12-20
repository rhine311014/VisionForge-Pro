#pragma once
#include "Keyboard.h"
#include "afxwin.h"

// CDlgTargetFixOffsetSetting 对话框

class CDlgTargetFixOffsetSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTargetFixOffsetSetting)

public:
	CDlgTargetFixOffsetSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTargetFixOffsetSetting();

// 对话框数据
	enum { IDD = IDD_DLG_TARGET_FIX_OFFSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStringArray	*m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	void UpdateDialogLanguage();

	BOOL m_bEnableTargetFixOffset;
// 	double m_dTargetFixOffsetX;
// 	double m_dTargetFixOffsetY;
// 	double m_dTargetFixOffsetD;
	CString m_strTargetFixOffsetX;
	CString m_strTargetFixOffsetY;
	CString m_strTargetFixOffsetD;
	BOOL m_bKillFocusEnabled;
	CStringArray m_saTarFixOffsetX;
	CStringArray m_saTarFixOffsetY;
	CStringArray m_saTarFixOffsetD;
	afx_msg void OnEnSetfocusEditTargetFixOffsetX();
	afx_msg void OnEnKillfocusEditTargetFixOffsetX();
	afx_msg void OnEnSetfocusEditTargetFixOffsetY();
	afx_msg void OnEnKillfocusEditTargetFixOffsetY();
	afx_msg void OnEnSetfocusEditTargetFixOffsetD();
	afx_msg void OnEnKillfocusEditTargetFixOffsetD();
	virtual BOOL OnInitDialog();
	CComboBox m_comboAlnTarFixOffsetIndex;
	int m_nPrevIndex;
	afx_msg void OnCbnSelchangeCmbTarFixOffsetIndex();
	afx_msg void OnBnClickedOk();
	int m_nTargetOffsetType;
};
