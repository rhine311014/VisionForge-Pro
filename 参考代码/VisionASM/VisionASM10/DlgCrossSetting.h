#pragma once
#include "afxwin.h"
#include "SystemOptionConfig.h"
#include "KeyBoard.h"

class CDlgSearchGrapchSetting ;
// CDlgCrossSetting 对话框

class CDlgCrossSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgCrossSetting)

public:
	CDlgCrossSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCrossSetting();

// 对话框数据
	enum { IDD = IDD_DLG_CROSS_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void UpdateDlgdata(bool flag);
	void SetCrossSize(SearchResultCrossSize nCrossSize);
	void UpdateDialogLanguage();
	BOOL IsStringNumerical(LPCTSTR lpszVal);
    void OnLineColorChange();
	void OnTrainRectLineColorChange();
	void OnOtherUILineColorChange();
public:
	COLORREF						m_ColorDisplay;
	CRect							m_RectDisPlay;
	int								m_ShowColor;
	int								m_ShowLength;
	int								m_ShowWidth;
	BOOL							m_ShowStyle;
    int                             m_nOldLineColor;
    BOOL                            m_bOldShowStyle;
    int                             m_nOldShowLength;
    int                             m_nOldShowWidth;
	bool							m_bSysKeyboardEnabled;
	SearchResultCrossSize			m_Cross;
	CStringArray				   *m_psaSysInfoStrings;
	CDlgSearchGrapchSetting	       *m_pParent;
	CKeyBoard						m_ctrlKeyboard;
	
	int								m_nTrainRectShowColor;
	int								m_nTrainRectShowWidth;
	int								m_nOldTrainRectShowColor;
	int								m_nOldTrainRectShowWidth;

	COLORREF						m_TrainRectColorDisplay;
	CRect							m_TrainRectRectDisPlay;


	int								m_nOtherUIShowColor;
	int								m_nOtherUIShowWidth;
	int								m_nOldOtherUIShowColor;
	int								m_nOldOtherUIShowWidth;

	COLORREF						m_OtherUIColorDisplay;
	CRect							m_OtherUIRectDisPlay;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadRed();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedRadGreen();
	afx_msg void OnBnClickedRadBlue();
	afx_msg void OnBnClickedRadColorSetting();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditLineWidth();
	afx_msg void OnEnChangeEditLineLength();
	afx_msg void OnEnSetfocusEditLineWidth();
	afx_msg void OnEnSetfocusEditLineLength();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_bShowDefortGui;
	virtual BOOL DestroyWindow();
    afx_msg void OnBnClickedRadCrosszero();
    afx_msg void OnBnClickedRadCross45();
    afx_msg void OnBnClickedCheckShowDefortGui();
	BOOL m_bShowTrainRect;
	BOOL m_bShowOtherGui;

	afx_msg void OnBnClickedRadTrainRectRed();
	afx_msg void OnBnClickedRadTrainRectGreen();
	afx_msg void OnBnClickedRadTrainRectBlue();
	afx_msg void OnBnClickedRadTrainRectColorSetting();
	afx_msg void OnEnChangeEditTrainRectLineWidth();
	afx_msg void OnEnSetfocusEditTrainRectLineWidth();


	afx_msg void OnBnClickedRadOtherUIRed();
	afx_msg void OnBnClickedRadOtherUIGreen();
	afx_msg void OnBnClickedRadOtherUIBlue();
	afx_msg void OnBnClickedRadOtherUIColorSetting();
	afx_msg void OnEnChangeEditOtherUILineWidth();
	afx_msg void OnEnSetfocusEditOtherUILineWidth();


};
