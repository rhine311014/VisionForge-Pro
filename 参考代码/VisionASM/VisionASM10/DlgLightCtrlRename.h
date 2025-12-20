#pragma once


// CDlgLightCtrlRename 对话框

class CDlgLightCtrlRename : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLightCtrlRename)

public:
	CDlgLightCtrlRename(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLightCtrlRename();

// 对话框数据
	enum { IDD = IDD_DLG_LIGHT_CTRL_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strLightCtrlName1;
	CString m_strLightCtrlName2;
	CString m_strLightCtrlName3;
	CString m_strLightCtrlName4;
	CString m_strLightCtrlName5;
	CString m_strLightCtrlName6;
	CString m_strLightCtrlName7;
	CString m_strLightCtrlName8;

	void GetSystemOptionLightName(SystemOptionLightConfig& systemOptionLightConfig);
	void SetSystemOptionLightName(SystemOptionLightConfig& systemOptionLightConfig);
};
