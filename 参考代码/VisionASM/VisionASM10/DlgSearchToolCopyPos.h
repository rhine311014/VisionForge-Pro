#pragma once
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"

// CDlgSearchToolCopyPos 对话框

class CDlgSearchToolCopyPos : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchToolCopyPos)

public:
	CDlgSearchToolCopyPos(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearchToolCopyPos();

// 对话框数据
	enum { IDD = IDD_DLG_SEARCH_TOOL_COPY_POS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	// 系统选项相关
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
public:
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
public:
	int m_nPosNum;
	std::vector<BOOL>	m_vbCopyPos;

public:
	std::vector<CButton*>		m_vpCheckPos;
	CButton m_checkPos1;
	CButton m_checkPos2;
	CButton m_checkPos3;
	CButton m_checkPos4;
	CButton m_checkPos5;
	CButton m_checkPos6;
	CButton m_checkPos7;
	CButton m_checkPos8;
	int m_nCandidateIndex;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckPos1();
	afx_msg void OnBnClickedCheckPos2();
	afx_msg void OnBnClickedCheckPos3();
	afx_msg void OnBnClickedCheckPos4();
	afx_msg void OnBnClickedCheckPos5();
	afx_msg void OnBnClickedCheckPos6();
	afx_msg void OnBnClickedCheckPos7();
	afx_msg void OnBnClickedCheckPos8();
	afx_msg void OnBnClickedCancel();
	CComboBox m_comboCandidate;
	int m_nPosCNum;
	int m_nSrcCandidateIndex;
	afx_msg void OnCbnSelchangeComboCandidateIndex();
};
