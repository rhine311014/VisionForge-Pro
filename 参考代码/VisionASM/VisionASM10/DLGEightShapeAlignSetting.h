#pragma once
#include "KeyBoard.h"

// CDLGEightShapeAlignSetting 对话框

class CDLGEightShapeAlignSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGEightShapeAlignSetting)

public:
	CDLGEightShapeAlignSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGEightShapeAlignSetting();

// 对话框数据
	enum { IDD = IDD_DLG_EIGHT_SHAPE_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStringArray	*m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	BOOL IsStringNumerical(LPCTSTR lpszVal);

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	BOOL m_bCheckEightShapeAlign;
	int m_nEightShapeAlignMode;
	double m_dEightShapeAngle;
	afx_msg void OnEnSetfocusEditEighitAngle();
	afx_msg void OnEnKillfocusEditEighitAngle();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckEightShapeAlign();
	afx_msg void OnBnClickedRadioPositiveEight();
};
