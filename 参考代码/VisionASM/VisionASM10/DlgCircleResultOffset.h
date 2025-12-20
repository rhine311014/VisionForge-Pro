#pragma once
#include "KeyBoard.h"

// CDlgCircleResultOffset 对话框
class CDlgSearch;
class CDlgCircleResultOffset : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCircleResultOffset)

public:
	CDlgCircleResultOffset(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCircleResultOffset();

// 对话框数据
	enum { IDD = IDD_DLG_CIRCLE_RESULT_OFFSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int m_nCircleCenterMoveMode;
    int m_nCircleCenterMoveModeX;
    int m_nCircleCenterMoveModeY;
    double m_dCircleCenterMoveLengthX;
    double m_dCircleCenterMoveLengthY;
    double m_dCircleCenterMoveRadiusMulX;
    double m_dCircleCenterMoveRadiusMulY;
    double m_dCircleCenterMoveAngle;
    double m_dCircleCenterMoveRadiusMul;
public:
    CStringArray	*m_psaSysInfoStrings;
    void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
    void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
    BOOL			m_bSysKeyboardEnabled;
    CKeyBoard		m_ctrlKeyboard;
    afx_msg void OnEnSetfocusEdtXByOffset();
    afx_msg void OnEnSetfocusEdtXByRadius();
    afx_msg void OnEnSetfocusEdtYByOffset();
    afx_msg void OnEnSetfocusEdtYByRadius();
    afx_msg void OnEnSetfocusEdtAngle();
    afx_msg void OnEnSetfocusEdtRadius();
    virtual BOOL OnInitDialog();
    void UpdateDialogLanguage();
    CDlgSearch		*m_pDlgParent;
};
