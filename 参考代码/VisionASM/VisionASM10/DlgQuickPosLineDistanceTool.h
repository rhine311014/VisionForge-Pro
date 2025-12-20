#pragma once
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgQuickPosLineDistanceTool 对话框

class CDlgSearch;

// CDlgQuickPosLineDistanceTool 对话框

class CDlgQuickPosLineDistanceTool : public CDialogEx
{
public:
	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;
	DECLARE_DYNAMIC(CDlgQuickPosLineDistanceTool)

	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;


	double m_dGrau;
	long	m_lNoiseThre;
	long	m_lTotalMag;
	BOOL	m_bEnableMask;

	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	
	//double	m_dAngleHigh;
	//double	m_dAngleLow;

	int		m_nLineCaliperNum1;
	int		m_nLineCaliperNum2;
	double	m_dLineContrastThre1;
	double	m_dLineContrastThre2;
	long	m_lLineFilterHalfWidth1;
	long	m_lLineFilterHalfWidth2;
	int		m_nLineOutLineNum1;
	int		m_nLineOutLineNum2;
	int		m_nLinePolarityMode1;
	int		m_nLinePolarityMode2;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	

	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
//	afx_msg void OnSelchangeComboAccuracyMode();
//	afx_msg void OnSelchangeComboSpeedMode();
	afx_msg void OnBtnSetCenter();
	//afx_msg void OnSetfocusEdtAngleLow();
	//afx_msg void OnSetfocusEdtAngleHigh();
	//afx_msg void OnSetfocusEdtAcceptThre();
	//afx_msg void OnSetfocusEdtConfusionThre();
	afx_msg void OnBtnFullImageSearch();
	

	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();

	afx_msg void OnChangeEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineContrastThre2();
	afx_msg void OnSetfocusEdtLineOutlineNum2();

	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoisethre();
	afx_msg void OnSetfocusEdtTotalMag();

	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();

	DECLARE_MESSAGE_MAP()

public:
	CDlgQuickPosLineDistanceTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuickPosLineDistanceTool();

// 对话框数据
	enum { IDD = IDD_DLG_QUICK_POSLINEDISTANCE_TOOL };
protected:
	CThemeHelperST		m_themeHelper;

	//int		m_nPatSpeedMode;
	//int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();
public:
	BOOL m_bSortByScore1;
	BOOL m_bSortByScore2;
	CButton m_checkSortByScore1;
	CButton m_checkSortByScore2;

	BOOL m_bEnableLineMask_1;
	BOOL m_bShowFitPoint_1;
	BOOL m_bEnableLineMask_2;
	BOOL m_bShowFitPoint_2;
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedCheckLineMask2();
	afx_msg void OnBnClickedCheckShowFitpoint2();

public:
	BOOL	m_bEnableSearchPos;
	BOOL	m_bEnableFindLine2;
	afx_msg void OnBnClickedCheckFindLine2();

	double	m_dPixelRatio;
	BOOL	m_bEnableMax1;
	BOOL	m_bEnableMax2;
	double	m_dDistanceMax1;
	double	m_dDistanceMax2;
	BOOL	m_bEnableMin1;
	BOOL	m_bEnableMin2;
	double	m_dDistanceMin1;
	double	m_dDistanceMin2;
	afx_msg void OnSetfocusEdtPixelRatio();
	afx_msg void OnSetfocusEdtDistanceMax1();
	afx_msg void OnSetfocusEdtDistanceMax2();
	afx_msg void OnSetfocusEdtDistanceMin1();
	afx_msg void OnSetfocusEdtDistanceMin2();

public:
	double	m_dDistance1;
	double	m_dDistance2;
	void UpdateSearchResult();
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_2;
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper2();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
	BOOL m_bEnableDualEdge_1;
	BOOL m_bEnableDualEdge_2;
	int m_nImageChannel;

};
