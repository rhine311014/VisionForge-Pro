#pragma once
#include "afxwin.h"


// CDlgBlobSearchParamSet 对话框
class CDlgSearch;

class CDlgBlobSearchParamSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBlobSearchParamSet)

public:
	CDlgBlobSearchParamSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBlobSearchParamSet();

// 对话框数据
	enum { IDD = IDD_DLG_BLOB_SEARCH_TOOL_FOR_SPECIAL_PICK };
	CDlgSearch		*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateDialogLanguage();
	int m_nBlobMode;
	long m_lBlobGrayThre;
	long m_lConnectivityMinPels;
	long m_lBlobAreaMinThre;
	long m_lBlobAreaMaxThre;
//	int		m_nBlobOutMode;
	afx_msg void OnSetfocusEdtBlobMinArea();
	afx_msg void OnSetfocusEdtBlobMaxArea();
	afx_msg void OnSetfocusEdtConnectMinPels();

	int m_nBlobSearchNums;

	afx_msg void OnEnSetfocusEdtBlobSearchNums();
	afx_msg void OnEnSetfocusEdtThreshold();
	CComboBox m_ComboOutput1;
	CComboBox m_ComboOutput2;
	int m_nOutPutPoint1;
	int m_nOutPutPoint2;
	int m_nImageChannel;
	void UpdateRadioButtonDisplay();
};
