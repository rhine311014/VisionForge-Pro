#pragma once
#include "afxcmn.h"
#include "KeyBoard.h"
#include "SystemOptionConfig.h"
#include "VisionASMDLG.h"

// CDlgSystemInfo 对话框

class CDlgSystemInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgSystemInfo)

public:
	CDlgSystemInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSystemInfo();

// 对话框数据
	enum { IDD = IDD_DLG_SYSTEM_INFO };

public:
	CVisionASMDlg *m_pParent;
	CString		  m_strSimpleSystemInfo;

public:
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	void UpdateDialogLanguage();
protected:
	//CThemeHelperST		m_themeHelper;
	CStringArray		*m_psaSysInfoStrings;
	BOOL				m_bSysKeyboardEnabled;
	CKeyBoard			m_ctrlKeyboard;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSaveSystemInfo();
	CTreeCtrl m_ctrlProductTree;
	void	UpdateDlgSystemInfoTree();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSaveSimpleSystemInfo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
