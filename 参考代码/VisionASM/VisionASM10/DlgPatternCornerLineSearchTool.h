#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"
// CDlgPatternCornerLineSearchTool 对话框
class CDlgSearch;
class CDlgPatternCornerLineSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternCornerLineSearchTool)

public:
	CDlgPatternCornerLineSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternCornerLineSearchTool();

// 对话框数据
	enum { IDD = IDD_DLG_PATTERN_CORNER_LINE_SEARCH_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;


	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;

	CThemeHelperST		m_themeHelper;

	
	CComboBox	m_comboLineAngleType;
	////////////////////////////////
	CComboBox	m_comboSpeed;
	CComboBox	m_comboAccuracy;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dConfusionThre;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;

	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;

	//----------------------------
	CButton m_checkSortByScore1;
	CButton m_checkEnableLineMask1;
	CButton m_checkShowLinePoint1;
	CButton m_checkSortByScore3;
	CButton m_checkEnableLineMask3;
	CButton m_checkShowLinePoint3;


	int m_iRDFindCornerLine1;
	int m_iEditLineCaliperNum1;
	int m_iEditLineOutllneNum1;
	double m_iEditLineContrastThre1;
	int m_iRdLinePolarityMode1;
	int m_iRdEdgeProperty1;
	long	m_lLineFilterHalfWidth1;

	int m_iEditLineCaliperNum3;
	int m_iEditLineOutlineNum3;
	double m_iEditLineContrastThre3;
	int m_iRdLinePolarityMode3;
	int m_iRdSingleEdgeProperty3;
	long	m_lLineFilterHalfWidth3;
	// 找线参数
	int		m_nLineCaliperNum[3];
	double	m_dLineContrastThre[3];
	long	m_lLineFilterHalfWidth[3];
	int		m_nLineOutLineNum[3];
	int		m_nLinePolarityMode[3];
	int     m_nLineEdgeProperty[3];
	BOOL m_bSortByScore[3];
	BOOL m_bEnableLineMask[3];
	BOOL m_bEnableShowLinePoint[3];
	BOOL m_bEnableShowFitPoint[3];
	BOOL m_bShowCaliper[3];
	BOOL m_bEnableDualEdge[3];
	// 找线类型
	int m_nLineAngleType;

	///////////////////////////////

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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

	void UpdateDialogLanguage();

	void UpdateGuiLineParam(bool bUpdate, int iLine);
protected:
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


	afx_msg void OnCbnSelchangeComboLineAngleType();

	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineCalipernum1();

	afx_msg void OnSetfocusEdtLineContrastThre1();

	afx_msg void OnSetfocusEdtLineOutlineNum1();

	afx_msg void OnBnClickedCheckEnableLineMask1();
	afx_msg void OnBnClickedCheckShowLinePoint1();




	afx_msg void OnChangeEdtLineCalipernum3();
	afx_msg void OnSetfocusEdtLineCalipernum3();
	afx_msg void OnSetfocusEdtLineContrastThre3();
	afx_msg void OnSetfocusEdtLineOutlineNum3();
	afx_msg void OnBnClickedCheckEnableLineMask3();
	afx_msg void OnBnClickedCheckShowLinePoint3();

public:
	
	afx_msg void OnBnClickedRdLineCaliper1();
	afx_msg void OnBnClickedRdLineCaliper2();

	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper3();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth3();
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_3;
	BOOL m_bEnableDualEdge_1;
	BOOL m_bEnableDualEdge_3;
	int m_nImageChannel;
};
