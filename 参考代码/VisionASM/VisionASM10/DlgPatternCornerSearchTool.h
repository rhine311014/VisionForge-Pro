#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgPatternCornerSearchTool 对话框

class CDlgSearch;
class CDlgPatternCornerSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternCornerSearchTool)

public:
	CDlgPatternCornerSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternCornerSearchTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// 对话框数据
	enum { IDD = IDD_DLG_PATTERNCORNERSEARCH_TOOL };

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
	double	m_dLineContrastThre1;
	double	m_dLineContrastThre2;
	long	m_lLineFilterHalfWidth1;
	long	m_lLineFilterHalfWidth2;
	int		m_nLineOutLineNum1;
	int		m_nLineOutLineNum2;
	int		m_nLinePolarityMode1;
	int		m_nLinePolarityMode2;
    int     m_nOldLinePolarityMode1;
    int     m_nOldLinePolarityMode2;


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
    void OnLinePolarityChnage1();
    void OnLinePolarityChnage2();
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

	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	void UpdateDialogLanguage();
public:
    int		m_nPatSpeedMode;
    int		m_nPatAccuracyMode;
    int     m_nOldPatSpeedMode;
    int     m_nOldAccuracyMode;
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
	afx_msg void OnEnSetfocusEditAngleBase();
	afx_msg void OnEnSetfocusEditAngleFlu();
	afx_msg void OnBnClickedAngleCheck();
	afx_msg void OnBnClickedAngleCorCheck();
    void OnLineCorModeChange();
	// 夹角真值
	double m_dAngleBaseValue;
	// 夹角波动范围
	double m_dAngleFluValue;
	// 是否启用夹角超限判断
	CButton m_bAngleCheckEnabled;
	// 是否启用角度修正
	CButton m_bAngleCorCheckEnaled;
	// 选择修正的直线
	int m_nLineCorModeSelect;
    int m_nOldLineCorModeSelect;
	//是否启用夹角判断
	BOOL m_bEnableAngleCheck;
	BOOL m_bEnaleAngleCorCheck;
	BOOL m_bEnableDualEdge;
	BOOL m_bEnableDualEdge2;
    afx_msg void OnBnClickedCheckSortByScore1();
    afx_msg void OnBnClickedCheckSortByScore2();
    afx_msg void OnBnClickedChkEnableDualEdge();
    afx_msg void OnBnClickedChkEnableDualEdge2();
    afx_msg void OnBnClickedRadLineCorMode1();
    afx_msg void OnBnClickedRadLineCorMode2();
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper2();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_2;
	int m_nImageChannel;
};
