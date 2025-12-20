#pragma once

// CDlgPatternCircleSearchTool 对话框

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"


class CDlgSearch;
class CDlgPatternCircleSearchTool : public CDialog
{
	// Construction
public:
	CDlgPatternCircleSearchTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPatternCircleSearchTool();

	// 对话框数据
	enum { IDD = IDD_DLG_PATTERN_CIRCLESEARCH_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;

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

	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dConfusionThre;
	BOOL	m_bCoordSortEnable;
	double  m_dCoordSortGap;
	CComboBox m_comboCoordSortMethod;
	int		m_nCoordSortMethod;
	int		m_nCoordSortOrderX;
	int		m_nCoordSortOrderY;

	CDlgSearch	*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

    int m_nCircleCenterMoveMode;
    int m_nCircleCenterMoveModeX;
    int m_nCircleCenterMoveModeY;
    double m_dCircleCenterMoveLengthX;
    double m_dCircleCenterMoveLengthY;
    double m_dCircleCenterMoveRadiusMulX;
    double m_dCircleCenterMoveRadiusMulY;
    double m_dCircleCenterMoveAngle;
    double m_dCircleCenterMoveRadiusMul;

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
	afx_msg void OnBnClickedCheckLineMask();
	afx_msg void OnBnClickedCheckShowLinePoint();

	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtConfusionThre();

protected:
	CThemeHelperST		m_themeHelper;
public:
	BOOL m_bEnableShowLinePoint;
	CButton m_checkShowLinePoint;

	int m_nAuxiliaryPosType;
	afx_msg void OnCbnSelchangeComboCoordSortMethod();
	afx_msg void OnSetfocusEdtCoordSortGap();
    afx_msg void OnBnClickedBtnResultOffsetSetting();
    BOOL m_bEnableCircleResultOffset;
    afx_msg void OnBnClickedChkEnableCircleresultOffset();
	afx_msg void OnBnClickedChkShowCircleCaliper();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	BOOL m_bShowCircleCaliper;
	BOOL m_bEnableCircleMask;
	BOOL m_bEnableDualEdge;
	int m_nImageChannel;
};