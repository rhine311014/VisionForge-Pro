#pragma once
#include "afxwin.h"

#include "svAngle.h"
#include "svVector.h"
#include "svLine.h"
#include "svLineSeg.h"
#include "svMeasure.h"
#include "KeyBoard.h"

// CDlgOffsetCalculate 对话框

class CDlgOffsetCalculate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOffsetCalculate)

public:
	CDlgOffsetCalculate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOffsetCalculate();

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }

	CStringArray	*m_psaSysInfoStrings;
	void UpdateDialogLanguage();
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	SysPlatformInfo m_platformInfo;
	void SetSysPlatformInfo(SysPlatformInfo platformInfo) {m_platformInfo = platformInfo;}
	
	CPlatformOptionInfo m_optionInfo;
	void SetPlatformOptionInfo(CPlatformOptionInfo& optionInfo) {m_optionInfo = optionInfo;}

// 对话框数据
	enum { IDD = IDD_DLG_OFFSET_CALCULATE };

	void ShowImage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_staticImage0;
	CStatic m_staticImage1;
	CStatic m_staticImage2;
	CStatic m_staticImage3;
	CStatic m_staticImage4;
	virtual BOOL OnInitDialog();
	int m_nRadioPosMode;
	afx_msg void OnBnClickedRadioPosMode1();
	afx_msg void OnBnClickedRadioPosMode2();
	double m_dObjectL1;
	double m_dObjectW1;
	double m_dTargetL2;
	double m_dTargetW2;
	double m_dF1;
	double m_dF2;
	CString m_strObjectL1;
	CString m_strObjectW1;
	CString m_strTargetL2;
	CString m_strTargetW2;
	CString m_strF1;
	CString m_strF2;
	BOOL m_bOffsetXYEnable;
	CEdit m_editAngleResult;
	afx_msg void OnBnClickedButtonOffsetCalculate();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckXyEnable();
	afx_msg void OnEnSetfocusEditL1();
	afx_msg void OnEnSetfocusEditW1();
	afx_msg void OnEnSetfocusEditL2();
	afx_msg void OnEnSetfocusEditW2();
	afx_msg void OnEnSetfocusEditF1();
	afx_msg void OnEnSetfocusEditF2();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditAngleResult();
	BOOL	m_bKillFocusEnabled;	//标识失去焦点
	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
	afx_msg void OnEnKillfocusEditL1();
	afx_msg void OnEnKillfocusEditW1();
	afx_msg void OnEnKillfocusEditL2();
	afx_msg void OnEnKillfocusEditW2();
	afx_msg void OnEnKillfocusEditF1();
	afx_msg void OnEnKillfocusEditF2();
	BOOL m_bEnableObjectPositionVirtualMode;
	BOOL m_bEnableTargetPositionVirtualMode;
	int m_nObjectPositionVirtualMode;
	int m_nTargetPositionVirtualMode;

	afx_msg void OnBnClickedCheckObjectPositionVitualMode();
	afx_msg void OnBnClickedCheckTargetPositionVitualMode();
	afx_msg void OnBnClickedRadioObject12positionVirtual();
	afx_msg void OnBnClickedRadioObject34positionVirtual();
	afx_msg void OnBnClickedRadioObject13positionVirtual();
	afx_msg void OnBnClickedRadioObject24positionVirtual();
	afx_msg void OnBnClickedRadioTarget56positionVirtual();
	afx_msg void OnBnClickedRadioTarget78positionVirtual();
	afx_msg void OnBnClickedRadioTarget57positionVirtual();
	afx_msg void OnBnClickedRadioTarget68positionVirtual();
};
