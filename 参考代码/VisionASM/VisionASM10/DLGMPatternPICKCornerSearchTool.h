//#pragma once
//
//
//// DLGMPatternPICKCornerSearchTool 对话框
//
//class DLGMPatternPICKCornerSearchTool : public CDialogEx
//{
//	DECLARE_DYNAMIC(DLGMPatternPICKCornerSearchTool)
//
//public:
//	DLGMPatternPICKCornerSearchTool(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~DLGMPatternPICKCornerSearchTool();
//
//// 对话框数据
//	enum { IDD = IDD_DLG_MPATTERNPICKCORNERSEARCH_TOOL };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//
//	DECLARE_MESSAGE_MAP()
//};

#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgPatternCornerSearchTool 对话框

class CDlgSearch;
class CDlgMPatternPickCornerSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMPatternPickCornerSearchTool)

public:
	CDlgMPatternPickCornerSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMPatternPickCornerSearchTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

	// 对话框数据
	enum { IDD = IDD_DLG_MPATTERNPICKCORNERSEARCH_TOOL };

	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	CComboBox	m_comboSpeed;
	CComboBox	m_comboAccuracy;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dConfusionThre;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	int      m_nPreCliperIndex;
	int		 m_nCliperIndex;
	int      m_nLineOutLineNum[4];
	double   m_dLineContrastThre[4];
	long     m_lLineFilterHalfWidth[4];
	int      m_nLinePolarityMode[4];
	int      m_nLineCaliperNum[4];
	BOOL	 m_bSortByScore[4];
	BOOL	 m_bEnableDualEdge[4];
	BOOL	 m_bEnableLineMask[4];
	BOOL	 m_bShowFitPoint[4];
	BOOL	 m_bShowCaliper[4];

	int		m_nLineCaliperNum1;
	double	m_dLineContrastThre1;
	long	m_lLineFilterHalfWidth1;
	int		m_nLineOutLineNum1;
	int		m_nLinePolarityMode1;
	BOOL	m_bSortByScore1;		// TrainData
	BOOL	m_bEnableDualEdge1;		// TrainData
	BOOL	m_bEnableLineMask_1;	// TrainGui
	BOOL	m_bShowFitPoint_1;		// SearchGui
	BOOL    m_bShowCaliper_1;		// TrainGui

	BOOL	m_bCoordSortEnable;
	double  m_dCoordSortGap;
	CComboBox m_comboCoordSortMethod;
	int		m_nCoordSortMethod;
	int		m_nCoordSortOrderX;
	int		m_nCoordSortOrderY;

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAccuracyMode();
	afx_msg void OnSelchangeComboSpeedMode();
	//	afx_msg void OnBtnSetCenter();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtConfusionThre();
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnSetfocusEdtSearchNum();

	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();
	afx_msg void OnCbnSelchangeComboCoordSortMethod();
	afx_msg void OnEnSetfocusEdtCoordSortGap2();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();
public:
	afx_msg void OnCaliperChange();


	afx_msg void OnBnClickedCheckSortByScore1();
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedChkEnableDualEdge();

	afx_msg void OnBnClickedChkShowLineCaliper();
	int m_nImageChannel;
	BOOL m_bEnable24CorLineCombine;
	BOOL m_bEnable13ProjectTo24Combine;
	BOOL m_bEnableOutput13MidCombine;
	afx_msg void OnBnClickedChkOutput13Mid();
	afx_msg void OnBnClickedChk24CorLineCombine();
	BOOL m_bGetFourResultEnable;
	afx_msg void OnBnClickedChkGetFourResult2();
	// 夹爪1屏蔽区域启用
	BOOL m_bSearchHideRectVisible1;
	// 夹爪2屏蔽区域
	BOOL m_bSearchHideRectVisible2;
	afx_msg void nBnClickedChkShowHideSearchRect();
	afx_msg void OnBnClickedChkShowHideSearchRect1();
	afx_msg void OnBnClickedChkShowAcfAlignInspect();


	//ACF检测参数
	double m_dCurMinMean;
	double m_dCurMaxMean;
	double m_dCurMinStdDev;
	double m_dCurMaxStdDev;

	BOOL	 m_bMeanInvalid;
	BOOL	 m_bStdDevInvalid;
	BOOL	 m_bMeanInvert;
	BOOL	 m_bStdDevInvert;
	BOOL	 m_nInpectTool;
	int m_nCurLogicIndex;

};

