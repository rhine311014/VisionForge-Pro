#pragma once

#include "KeyBoard.h"

// CCDlgWeldInfo 对话框

class CCDlgWeldInfo : public CDialog
{
	DECLARE_DYNAMIC(CCDlgWeldInfo)

public:
	CCDlgWeldInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCDlgWeldInfo();

// 对话框数据
	enum { IDD = IDD_DLG_WELD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bEnableWeld;
	CString m_strWeldMovementRatioX;
	CString m_strWeldMovementRatioY;
	CString m_strWeldMovementRatioD;
	CString m_strWeldAmmendMoveRatioX;
	CString m_strWeldAmmendMoveRatioY;
	int m_nWeldTargetSearchTimes;
	int m_nWeldSearchBench;
	int m_nWeldAngleBench;
    int m_nOldWeldTargetSearchTimes;
    int m_nOldWeldSearchBench;
    int m_nOldWeldAngleBench;
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditWeldMovementRatioX();
	afx_msg void OnSetfocusEditWeldMovementRatioX();
	afx_msg void OnKillfocusEditWeldMovementRatioY();
	afx_msg void OnSetfocusEditWeldMovementRatioY();
	afx_msg void OnKillfocusEditWeldAmmendMovementRatioX();
	afx_msg void OnSetfocusEditWeldAmmendMovementRatioX();
	afx_msg void OnKillfocusEditWeldAmmendMovementRatioY();
	afx_msg void OnSetfocusEditWeldAmmendMovementRatioY();


public:
	CStringArray	*m_psaSysInfoStrings;
//	void UpdateDialogLanguage();
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
    void UpdateDialogLanguage();
    void OnTargetSearchTimesChange();
    void OnWeldSearchBenchChange();
    void OnWeldAngleBenchChange();

	BOOL			m_bKillFocusEnabled;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBnClickedCheckEnableWeld();
    afx_msg void OnBnClickedRadioWeldTargetMulSearch();
    afx_msg void OnBnClickedRadioWeldTargetOneSearch();
    afx_msg void OnBnClickedRadioWeldTargetZeroSearch();
    afx_msg void OnBnClickedRadioWeldObjectBench();
    afx_msg void OnBnClickedRadioWeldTargetBench();
    afx_msg void OnBnClickedRadioWeldNoBench();
    afx_msg void OnBnClickedRadioWeldObjectAngleBench();
    afx_msg void OnBnClickedRadioWeldTargetAngleBench();
    afx_msg void OnBnClickedRadioWeldNoAngleBench();

	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
	
	afx_msg void OnEnKillfocusEditWeldMovementRatioD();
	afx_msg void OnEnSetfocusEditWeldMovementRatioD();
};
