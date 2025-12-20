#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"

// CDlgQuickSearchCheckTool 对话框
class CDlgSearch;
class CDlgQuickSearchCheckTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgQuickSearchCheckTool)

public:
	CDlgQuickSearchCheckTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuickSearchCheckTool();


	CDlgSearch *m_pDlgParent;
	scGuiDisplay m_GuiPatternDisplay;
	scGuiDisplay	m_GuiPatternDisplayCheck1;
	scGuiDisplay	m_GuiPatternDisplayCheck2;
	CStringArray *m_psaSysInfoStrings;
	cpImage	m_imageMask;

// 对话框数据
	enum { IDD = IDD_DLG_COMBINED_QUICKSEARCH_TOOL };

	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;

	double	m_dGrau;
	double	m_dGrau1;
	long	m_lNoiseThre;
	long	m_lNoiseThre1;
	long	m_lTotalMag;
	long	m_lTotalMag1;
	BOOL	m_bEnableMask;
	BOOL	m_bEnableCheckRegion1;
	BOOL	m_bEnableCheckRegion2;

	double	m_dAcceptThre;
	double	m_dAcceptThre2;
	int		m_nSearchNum;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	double	m_dSizeHigh;
	double	m_dSizeLow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	// 更新控件显示
	void UpdateControlDisplay();			
	// 定位工具训练数据与界面之间的数据获取，
	// bValidate:true,定位工具从界面获取参数;false,界面从定位工具获取训练参数值
	void UpdateTrainData(bool bValidate);	
	// 定位工具搜索数据与界面之间的数据获取，
	// bValidate:true,定位工具从界面获取参数;false,界面从定位工具获取搜索参数值
	void UpdateSearchData(bool bValidate);
	// 检查对话框上的参数设置的有效性，并将无效参数设置成默认值
	void CheckDlgDataValidation();

	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	void UpdateDialogLanguage();
public:
	afx_msg void OnBnClickedBtnGetTrainImage();
	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnSetMaskImage();
	afx_msg void OnBnClickedChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSetCenter();
	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnKillfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoisethre();
	afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtSizeLow();
	afx_msg void OnSetfocusEdtSizeHigh();
	afx_msg void OnBnClickedBtnFullImageSearch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetfocusEdtGrau1();
	afx_msg void OnKillfocusEdtGrau1();
	afx_msg void OnSetfocusEdtNoisethre1();
	afx_msg void OnSetfocusEdtTotalMag1();
	afx_msg void OnEnSetfocusEdtAcceptThreCheck();
	afx_msg void OnBnClickedCheckRegion1Enable();
	afx_msg void OnBnClickedCheckRegion2Enable();
	int m_nImageChannel;
};
