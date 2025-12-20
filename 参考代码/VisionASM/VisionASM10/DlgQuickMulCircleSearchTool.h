#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"

// CDlgQuickMulCircleSearchTool 对话框

#define DLG_QUICK_CIRCLE_NUM 2

class CDlgSearch;
class CDlgQuickMulCircleSearchTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuickMulCircleSearchTool)

public:
	CDlgQuickMulCircleSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuickMulCircleSearchTool();

// 对话框数据
	enum { IDD = IDD_DLG_QUICKMULCIRCLESEARCH_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

	int      m_nPreCircleIndex;
	int      m_nTempCircleCaliperNum[DLG_QUICK_CIRCLE_NUM];
	int      m_nTempCircleOutLineNum[DLG_QUICK_CIRCLE_NUM];
	double   m_dTempCircleContrastThre[DLG_QUICK_CIRCLE_NUM];
	int      m_nTempCirclePolarityMode[DLG_QUICK_CIRCLE_NUM];
	int		 m_nTempCircleInwardMode[DLG_QUICK_CIRCLE_NUM];
	long     m_lTempCircleFilterHalfWidth[DLG_QUICK_CIRCLE_NUM];
	BOOL	 m_bTempCircleSortByScore[DLG_QUICK_CIRCLE_NUM];
	BOOL	 m_bTempEnableShowCirclePoint[DLG_QUICK_CIRCLE_NUM];
	BOOL	 m_bTempShowCircleCaliper[DLG_QUICK_CIRCLE_NUM];
	BOOL	 m_bTempEnableCircleMask[DLG_QUICK_CIRCLE_NUM];
	BOOL	m_bTempEnableDualEdge[DLG_QUICK_CIRCLE_NUM];

public:
	void UpdateDialogLanguage();

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

	void UpdateSearchResult();

public:
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;
	CThemeHelperST		m_themeHelper;

	double m_dGrau;
	long m_lNoiseThre;
	long m_lTotalMag;
	BOOL m_bEnableMask;

	int m_nLineAngleType;
	CComboBox m_comboLineAngleType;

	int m_nCircleIndex;
	int m_nCircleCaliperNum;
	int m_nCircleOutLineNum;
	double m_dCircleContrastThre;
	int m_nCirclePolarityMode;
	int m_nCircleInwardMode;
	BOOL m_bCircleSortByScore;
	CButton m_checkCircleSortByScore;
	BOOL m_bEnableShowCirclePoint;
	CButton m_checkShowCirclePoint;
	long	m_lCircleFilterHalfWidth;

	double m_dAcceptThre;
	int m_nSearchNum;
	double m_dAngleLow;
	double m_dAngleHigh;

	virtual BOOL OnInitDialog();

	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoisethre();
	afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnBnClickedChkEnableMask();
	afx_msg void OnBnClickedBtnSetMaskImage();
	afx_msg void OnBnClickedBtnGetTrainImage();

	afx_msg void OnEnChangeEdtCircleCalipernum();
	afx_msg void OnBnClickedRadCirclePolarityMode0();
	afx_msg void OnBnClickedRadCircleInwardMode0();
	afx_msg void OnBnClickedCheckSortByScore1();
	afx_msg void OnBnClickedCheckShowLinePoint();
	afx_msg void OnBnClickedRadioCircleIndex1();

	afx_msg void OnCbnSelchangeComboLineAngleType();

	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnFullImageSearch();

	afx_msg void OnSetfocusEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleOutlineNum();
	afx_msg void OnSetfocusEdtCircleContrastThre();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	BOOL m_bShowCircleCaliper;
	afx_msg void OnBnClickedChkShowCircleCaliper();
	BOOL m_bEnableCircleMask;
	afx_msg void OnBnClickedCheckLineMask();
	BOOL m_bEnableDualEdge;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	int m_nImageChannel;
	BOOL m_bEnableInspect;
	double m_dPixelRatio;
	BOOL m_bEnableMax1;
	BOOL m_bEnableMin1;
	double m_dDistanceMax1;
	double m_dDistanceMin1;
	BOOL m_bEnableMax2;
	BOOL m_bEnableMin2;
	double m_dDistanceMax2;
	double m_dDistanceMin2;
	BOOL m_bEnableMax3;
	BOOL m_bEnableMin3;
	double m_dDistanceMax3;
	double m_dDistanceMin3;
	double m_dFactor_a;
	double m_dFactor_b;
	double m_dCenterDistance;
	double m_dRadius1;
	double m_dRadius2;
	afx_msg void OnEnSetfocusEdtPixelRatio();
	afx_msg void OnEnSetfocusEdtDistanceMax1();
	afx_msg void OnEnSetfocusEdtDistanceMin1();
	afx_msg void OnEnSetfocusEdtDistanceMax2();
	afx_msg void OnEnSetfocusEdtDistanceMin3();
	afx_msg void OnEnSetfocusEdtDistanceMax3();
	afx_msg void OnEnSetfocusEdtDistanceMin4();
	afx_msg void OnEnSetfocusEdtDistanceMax4();
	afx_msg void OnEnSetfocusEdtDistanceMin5();
};
