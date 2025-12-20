#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "PatternChordSearchTool.h"

// CDlgPatternChordSearchTool 对话框
class CDlgSearch;
class CDlgPatternChordSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternChordSearchTool)

public:
	CDlgPatternChordSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternChordSearchTool();

	CDlgSearch	*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;
// 对话框数据
	enum { IDD = IDD_DLG_PATTERNCHORD_SEARCH_TOOL };

	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;

	CComboBox m_CoarseScale;
	CComboBox m_Accuracy;

	int     m_nCoarseScale;
	int     m_nAccuray;

	int		m_nSearchNum;
	double	m_dAcceptThre;
	double m_dConfusionThre;
	int		m_nChordInwardMode;
	int		m_nChordPolarityMode;
	int		m_nChordCaliperNum;
	double	m_dChordContrastThre;
	long	m_lCircleFilterHalfWidth;
	long	m_lLineFilterHalfWidth;
	int		m_nChordOutLineNum;
	BOOL	m_bSortByScore1;
	CButton m_checkSortByScore1;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CThemeHelperST		m_themeHelper;

	void UpdateDialogLanguage();

	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdtChordCalipernum();
	afx_msg void OnRadChordInwardMode0();
	afx_msg void OnRadChordInwardMode1();
	afx_msg void OnSetfocusEdtChordCalipernum();
	afx_msg void OnSetfocusEdtChordContrastThre();
	afx_msg void OnSetfocusEdtChordOutlineNum();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtConfusionThre();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnSetfocusEdtSearchNum();

	DECLARE_MESSAGE_MAP()

public:
	int m_nChordLineCaliperNum;
	int m_nLineOutlineNum;
	int m_nLineContrastThre;
	int m_nLinePolarity;
	BOOL m_bLineSortByScore;
	afx_msg void OnEnSetfocusEdtLineCalipernum();
	afx_msg void OnEnSetfocusEdtLineOutlineNum();
	afx_msg void OnEnSetfocusEdtLineContrastThre();
	CComboBox m_comboLineAngleType;
	BOOL m_bCheckCorrectAngle;
	int m_nLineAngleType;
	BOOL m_bEnableLineMask;
	BOOL m_bShowLinePoint;
	afx_msg void OnEnChangeEdtLineCalipernum();
	afx_msg void OnBnClickedCheckEnableLineMask();
	afx_msg void OnBnClickedCheckShowLinePoint();
	BOOL m_bShowCircleCaliper;
	BOOL m_bShowLineCaliper;
	afx_msg void OnBnClickedChkShowCircleCaliper();
	afx_msg void OnBnClickedChkShowLineCaliper();
	BOOL m_bShowCirclePoint;
	afx_msg void OnBnClickedCheckShowCirclePoint();
	afx_msg void OnBnClickedCheckCircleMask();
	BOOL m_bEnableCircleMask;
	BOOL m_bCircleEnableDualEdge;
	BOOL m_bLineEnableDualEdge;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	int m_nImageChannel;
	
	BOOL m_bEnableLineCircleCross;
	int	 m_nLineCircleCrossMode;
	afx_msg void OnBnClickedCheckEnableLineCircleCross();
	afx_msg void OnEnSetfocusEditFitLineDms();
	BOOL m_bEnableFitLineDMS;
	double m_dFitLineDMS;

	double m_dOffsetRatio;
	afx_msg void OnEnSetfocusEditOffsetRatio();
};
