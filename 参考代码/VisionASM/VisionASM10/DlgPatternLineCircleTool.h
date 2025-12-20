// #pragma once
// 
// 
// // CDlgPatternLineCircleTool 对话框
// 
// class CDlgPatternLineCircleTool : public CDialog
// {
// 	DECLARE_DYNAMIC(CDlgPatternLineCircleTool)
// 
// public:
// 	CDlgPatternLineCircleTool(CWnd* pParent = NULL);   // 标准构造函数
// 	virtual ~CDlgPatternLineCircleTool();
// 
// // 对话框数据
// 	enum { IDD = IDD_DLG_PATTERN_LINE_CIRCLE };
// 
// protected:
// 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
// 
// 	DECLARE_MESSAGE_MAP()
// };


#pragma once

// CDlgPatternLineCircleTool 对话框

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"


class CDlgSearch;
class CDlgPatternLineCircleTool : public CDialog
{
	// Construction
public:
	CDlgPatternLineCircleTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPatternLineCircleTool();

	// 对话框数据
	enum { IDD = IDD_DLG_PATTERN_LINE_CIRCLE_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;

// 	double m_dGrau;
// 	long	m_lNoiseThre;
// 	long	m_lTotalMag;
	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	CComboBox	m_comboSpeed;
	CComboBox	m_comboAccuracy;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	BOOL	m_bEnableMask;

	// Circle
	int		m_nCircleCaliperNum;
	int		m_nCircleOutLineNum;
	double	m_dCircleContrastThre;
	int		m_nCirclePolarityMode;
	int		m_nCircleInwardMode;
	long	m_lCircleFilterHalfWidth;
	BOOL	m_bCircleSortByScore;
	CButton m_CircleCheckSortByScore;

	// Line
	int		m_nLineCaliperNum1;
	double	m_dLineContrastThre1;
	int		m_nLineOutLineNum1;
	int		m_nLinePolarityMode1;
	long	m_lLineFilterHalfWidth1;
	BOOL	m_bLineSortByScore1;
	CButton m_LineCheckSortByScore1;
	CComboBox	m_comboLineAngleType;
	int		m_nLineAngleType;

	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dConfusionThre;
// 	double	m_dAngleHigh;
// 	double	m_dAngleLow;


	CDlgSearch	*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;


public:

	void UpdateControlDisplay();			
	void UpdateTrainData(bool bValidate);	
	void UpdateSearchData(bool bValidate);
	void CheckDlgDataValidation();
	void UpdateDialogLanguage();

protected:

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	afx_msg void OnBtnFullImageSearch();

// 	afx_msg void OnSetfocusEdtGrau();
// 	afx_msg void OnSetfocusEdtNoisethre();
// 	afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnSelchangeComboAccuracyMode();
	afx_msg void OnSelchangeComboSpeedMode();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();

	// Circle
	afx_msg void OnChangeEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleOutlineNum();
	afx_msg void OnSetfocusEdtCircleContrastThre();
	afx_msg void OnRadCircleInwardMode0();
	afx_msg void OnRadCircleInwardMode1();

	// Line
	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();
	afx_msg void OnCbnSelchangeComboLineAngleType();

	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtConfusionThre();
// 	afx_msg void OnSetfocusEdtAngleLow();
// 	afx_msg void OnSetfocusEdtAngleHigh();

protected:
	CThemeHelperST		m_themeHelper;
public:
	BOOL m_bEnableShowLinePoint;
	CButton m_checkShowLinePoint;
	afx_msg void OnBnClickedCheckShowLinePoint();

	BOOL m_bEnableShowLinePoint2;
	CButton m_checkShowLinePoint2;
	afx_msg void OnBnClickedCheckShowLinePoint2();

	afx_msg void OnBnClickedChkShowCircleCaliper();
	afx_msg void OnBnClickedChkShowLineCaliper();
	BOOL m_bShowCircleCaliper;
	BOOL m_bShowLineCaliper;
	BOOL m_bEnableCircleDualEdge;
	BOOL m_bEnableLineDualEdge;
	BOOL m_bEnableCircleMask;
	BOOL m_bEnableLineMask;
	afx_msg void OnBnClickedCheckCircleMask();
	afx_msg void OnBnClickedCheckLineMask();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	int m_nImageChannel;
};

