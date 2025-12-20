#pragma once


// CDlgSearchFailWarning 对话框

class CDlgSearchFailWarning : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchFailWarning)

public:
	CDlgSearchFailWarning(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearchFailWarning();

// 对话框数据
	enum { IDD = IDD_DLG_SEARCH_FAIL_WARN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
