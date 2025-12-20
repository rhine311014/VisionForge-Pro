#pragma once


// DlgHistogramForSpecicalPick 对话框
class CDlgSearch;
class DlgHistogramForSpecicalPick : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHistogramForSpecicalPick)

public:
	DlgHistogramForSpecicalPick(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgHistogramForSpecicalPick();
	CDlgSearch	*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;
// 对话框数据
	enum { IDD = IDD_DLG_HISTOGRAM_FOR_SPECIAL_PICK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dHeadXoffset;
	double m_dHeadYOffset;
	int m_nHeadLength;
	int m_nHeadWidth;

	double m_dTailXoffset;
	double m_dTailYOffset;
	int m_nTailLength;
	int m_nTailWidth;

	int m_MeanHighHead;
	int m_nHeadGreyValue;
	int m_nTailGreyValue;
	afx_msg void OnEnSetfocusEdtHeadOffsetX();
	afx_msg void OnSetfocusEdtLineHeadOffsetY();
	afx_msg void OnEnSetfocusEdtHeadLength();
	afx_msg void OnEnSetfocusEdtHeadWidth();
	afx_msg void OnEnSetfocusEdtTailOffsetY();
	afx_msg void OnEnSetfocusEdtTailOffsetX();
	afx_msg void OnEnSetfocusEdtTailLength();
	afx_msg void OnEnSetfocusEdtTailWidth();

	void CheckDlgDataValidation();
	void UpdateDialogLanguage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
