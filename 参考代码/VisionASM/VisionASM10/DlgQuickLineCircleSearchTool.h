// #pragma once
// 
// 
// // CDlgQuickLineCircleSearchTool 对话框
// 
// class CDlgQuickLineCircleSearchTool : public CDialog
// {
// 	DECLARE_DYNAMIC(CDlgQuickLineCircleSearchTool)
// 
// public:
// 	CDlgQuickLineCircleSearchTool(CWnd* pParent = NULL);   // 标准构造函数
// 	virtual ~CDlgQuickLineCircleSearchTool();
// 
// // 对话框数据
// 	enum { IDD = IDD_DLG_QUICK_LINE_CIRCLE_TOOL };
// 
// protected:
// 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
// 
// 	DECLARE_MESSAGE_MAP()
// };


#pragma once

// CDlgQuickLineCircleTool 对话框


#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"


class CDlgSearch;
class CDlgQuickLineCircleTool : public CDialog
{
	// Construction
public:
	CDlgQuickLineCircleTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgQuickLineCircleTool();

	// 对话框数据
	enum { IDD = IDD_DLG_QUICK_LINE_CIRCLE_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;

	BOOL m_bEnableMark2;

	double m_dGrau;
	long	m_lNoiseThre;
	long	m_lTotalMag;
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
	BOOL m_bEnableShowLinePoint;
	CButton m_checkShowLinePoint;
	BOOL m_bEnableCircle;

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
	BOOL m_bEnableShowLinePoint2;
	CButton m_checkShowLinePoint2;
	BOOL m_bEnableLine;

	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dAngleHigh;
	double	m_dAngleLow;


	CDlgSearch	*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

	//}}AFX_DATA


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

	afx_msg void OnBnClickedCheckEnableMark2();
	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoisethre();
	afx_msg void OnSetfocusEdtTotalMag();

	// Circle
	afx_msg void OnChangeEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleOutlineNum();
	afx_msg void OnSetfocusEdtCircleContrastThre();
	afx_msg void OnRadCircleInwardMode0();
	afx_msg void OnRadCircleInwardMode1();
	afx_msg void OnBnClickedCheckShowLinePoint();
	afx_msg void OnBnClickedCheckEnableCircle();

	// Line
	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();
	afx_msg void OnCbnSelchangeComboLineAngleType();
	afx_msg void OnBnClickedCheckShowLinePoint2();
	afx_msg void OnBnClickedCheckEnableLine();

	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();

protected:
	CThemeHelperST		m_themeHelper;
	
public:
	BOOL m_bShowCircleCaliper;
	afx_msg void OnBnClickedChkShowCircleCaliper();
	BOOL m_bShowLineCaliper;
	afx_msg void OnBnClickedCheckLineMask();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	BOOL m_bEnableCircleMask;
	BOOL m_bEnableDualEdge;
	int m_nImageChannel;
};

