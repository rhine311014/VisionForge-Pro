#pragma once

#include "KeyBoard.h"

// CCDlgPCBSetting 对话框

class CCDlgPCBSetting : public CDialog
{
	DECLARE_DYNAMIC(CCDlgPCBSetting)

public:
	CCDlgPCBSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCDlgPCBSetting();

// 对话框数据
	enum { IDD = IDD_DLG_PCB_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPCBInsPreX;
	CString m_strPCBInsPreY;
	CString m_strPCBInsPreD;
	int m_nPCBInsPreType;
	BOOL m_bPCBShowInspectResult;
	BOOL m_bChange2um;
    int m_nOldPCBInsPreType;
    void UpdateDialogLanguage();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditPcbInspectXPrecision();
	afx_msg void OnSetfocusEditPcbInspectXPrecision();
	afx_msg void OnKillfocusEditPcbInspectYPrecision();
	afx_msg void OnSetfocusEditPcbInspectYPrecision();
	afx_msg void OnKillfocusEditPcbInspectDPrecision();
	afx_msg void OnSetfocusEditPcbInspectDPrecision();

public:
	CStringArray	*m_psaSysInfoStrings;
//	void UpdateDialogLanguage();
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	BOOL			m_bKillFocusEnabled;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
    void OnInsPreTypeChange();
	BOOL	m_bShowPCB4To3Align;        //iyo

    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBnClickedCheckPcb4to3align();
    afx_msg void OnBnClickedCheckShowInspectResult();
    afx_msg void OnBnClickedRadioCenterInspect();
    afx_msg void OnBnClickedRadioForePointInspect();
    afx_msg void OnBnClickedRadioThreePointInspect();

	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
};
