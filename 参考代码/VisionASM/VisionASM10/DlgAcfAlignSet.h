#pragma once


// CDlgAcfAlignSet 对话框

class CDlgAcfAlignSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAcfAlignSet)

public:
	CDlgAcfAlignSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAcfAlignSet();

// 对话框数据
	enum { IDD = IDD_DLG_ACF_ALIGN_INSPECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	double m_dCurMinMean;
	double m_dCurMaxMean;
	double m_dCurMinStdDev;
	double m_dCurMaxStdDev;
	BOOL m_bMeanInvalid;
	BOOL m_bMeanInvert;
	BOOL m_bStdDevInvalid;
	BOOL m_bStdDevInvert;
	int m_nCurLogicIndex;
	BOOL m_nInpectTool;
};
