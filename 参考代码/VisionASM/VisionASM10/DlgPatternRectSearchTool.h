#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgPatternRectSearchTool 对话框

class CDlgSearch;
class CDlgPatternRectSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternRectSearchTool)

public:
	CDlgPatternRectSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternRectSearchTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// 对话框数据
	enum { IDD = IDD_DLG_PATTERNRECTSEARCH_TOOL };

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

	int		m_nLineCaliperNum1;
	int		m_nLineCaliperNum2;
	int		m_nLineCaliperNum3;
	int		m_nLineCaliperNum4;
	double	m_dLineContrastThre1;
	double	m_dLineContrastThre2;
	double	m_dLineContrastThre3;
	double	m_dLineContrastThre4;
	long	m_lLineFilterHalfWidth1;
	long	m_lLineFilterHalfWidth2;
	long	m_lLineFilterHalfWidth3;
	long	m_lLineFilterHalfWidth4;
	int		m_nLineOutLineNum1;
	int		m_nLineOutLineNum2;
	int		m_nLineOutLineNum3;
	int		m_nLineOutLineNum4;
	int		m_nLinePolarityMode1;
	int		m_nLinePolarityMode2;
	int		m_nLinePolarityMode3;
	int		m_nLinePolarityMode4;


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

	afx_msg void OnChangeEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineContrastThre2();
	afx_msg void OnSetfocusEdtLineOutlineNum2();

	afx_msg void OnChangeEdtLineCalipernum3();
	afx_msg void OnSetfocusEdtLineCalipernum3();
	afx_msg void OnSetfocusEdtLineContrastThre3();
	afx_msg void OnSetfocusEdtLineOutlineNum3();

	afx_msg void OnChangeEdtLineCalipernum4();
	afx_msg void OnSetfocusEdtLineCalipernum4();
	afx_msg void OnSetfocusEdtLineContrastThre4();
	afx_msg void OnSetfocusEdtLineOutlineNum4();

	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();
public:
	BOOL m_bSortByScore1;
	BOOL m_bSortByScore2;
	BOOL m_bSortByScore3;
	BOOL m_bSortByScore4;
	CButton m_checkSortByScore1;
	CButton m_checkSortByScore2;
	CButton m_checkSortByScore3;
	CButton m_checkSortByScore4;

	BOOL m_bEnableLineMask_1;
	BOOL m_bShowFitPoint_1;
	BOOL m_bEnableLineMask_2;
	BOOL m_bShowFitPoint_2;
	BOOL m_bEnableLineMask_3;
	BOOL m_bShowFitPoint_3;
	BOOL m_bEnableLineMask_4;
	BOOL m_bShowFitPoint_4;
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedCheckLineMask2();
	afx_msg void OnBnClickedCheckShowFitpoint2();
	afx_msg void OnBnClickedCheckLineMask3();
	afx_msg void OnBnClickedCheckShowFitpoint3();
	afx_msg void OnBnClickedCheckLineMask4();
	afx_msg void OnBnClickedCheckShowFitpoint4();
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper2();
	afx_msg void OnBnClickedChkShowLineCaliper3();
	afx_msg void OnBnClickedChkShowLineCaliper4();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth3();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth4();
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_2;
	BOOL m_bShowCaliper_3;
	BOOL m_bShowCaliper_4;
	BOOL m_bEnableDualEdge_1;
	BOOL m_bEnableDualEdge_2;
	BOOL m_bEnableDualEdge_3;
	BOOL m_bEnableDualEdge_4;
	int m_nResultOutMode;
	CComboBox m_comboOuput;

	int m_nImageChannel;
};
