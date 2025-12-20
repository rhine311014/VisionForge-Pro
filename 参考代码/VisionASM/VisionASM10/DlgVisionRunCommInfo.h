#pragma once
#include "Layout.h"
#include "afxwin.h"
#include "XPStyleButtonST.h"
// CDlgVisionRunCommInfo 对话框

class CDlgVisionRun;
class CDlgVisionRunCommInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVisionRunCommInfo)

public:
	CDlgVisionRunCommInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVisionRunCommInfo();

// 对话框数据
	enum { IDD = IDD_DLG_VISION_RUN_COMM_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void UpdateLogCommCommand();
	void GetCommCommandInfo();

	void SetVisionASMRef( vcBaseVisionAlign* pVisionASM );
	void SetParentWnd(CDlgVisionRun* pParentWnd);

	void SetSimpleCommShow();

	void UpdateDialogLanguage();			// 更新语言包

protected:
	void InitUIStyle();		// 初始化控件风格
	void InitLayout();		// 初始化布局

private:
	CLayout* m_pMainLayout;

	CDlgVisionRun* m_pParent;
	vcBaseVisionAlign* m_pVisionASM;

	CString m_strLogCommandSimple;
	CString m_strLogCommandDetail;

	deque<CString>	m_saCommCommandSimple;
	deque<CString>	m_saCommCommandDetail;
	bool m_bUpdatelogCommCommand;

public:
	CEdit m_editLog;
	int m_nRadioCommandMode;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRadioCommandDetail();
	afx_msg void OnBnClickedRadioCommandSimple();
	afx_msg void OnBnClickedBtnCommandClear();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CXPStyleButtonST m_rdCommDetail;
	CXPStyleButtonST m_rdCommSimple;
	CXPStyleButtonST m_btnCommClear;
	CThemeHelperST	 m_themeHelpSt;
	
};
