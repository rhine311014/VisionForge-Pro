#pragma once

#include "KeyBoard.h"
#include "SystemOptionConfig.h"
#include "vsEn_Decrypt.h"
#include "SHA256.H"
//#include "HaspInfo.h"
#include "VisionASMDLG.h"
// CDlgSysRegister 对话框

class CDlgSysRegister : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysRegister)

public:
	CDlgSysRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysRegister();

	CVisionASMDlg *m_pParent;

// 对话框数据
	enum { IDD = IDD_SYS_SOFT_REGISTER };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	void UpdateDialogLanguage();
protected:
	//CThemeHelperST		m_themeHelper;
	CStringArray		*m_psaSysInfoStrings;
	BOOL				m_bSysKeyboardEnabled;
	CKeyBoard			m_ctrlKeyboard;
	
public:
	BOOL                m_bRegisterSuccess;
	
public:
	afx_msg void OnEnSetfocusEditCode();
	afx_msg void OnBnClickedBtnRegister();
	CString m_strRegisterCode;
	CString m_strID;
	afx_msg void OnEnSetfocusEditId();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
