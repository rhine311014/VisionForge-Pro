#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgPatternLineSearchTool 对话框

class CDlgSearch;
class CDlgPatternLineSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternLineSearchTool)

public:
	CDlgPatternLineSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternLineSearchTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// 对话框数据
	enum { IDD = IDD_DLG_PATTERNLINESEARCH_TOOL };

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
	double	m_dLineContrastThre1;
	long	m_lLineFilterHalfWidth1;
	int		m_nLineOutLineNum1;
	int		m_nLinePolarityMode1;


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

	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();
public:
	BOOL m_bSortByScore1;
	CButton m_checkSortByScore1;
	CComboBox	m_comboLineAngleType;
	int		m_nLineAngleType;
	afx_msg void OnCbnSelchangeComboLineAngleType();

	BOOL m_bEnableLineMask;
	CButton m_checkLineMask;
	BOOL m_bEnableShowLinePoint;
	CButton m_checkShowLinePoint;
	afx_msg void OnBnClickedCheckEnableLineMask();
	afx_msg void OnBnClickedCheckShowLinePoint();
	BOOL m_bShowCaliper_1;
	afx_msg void OnBnClickedChkShowLineCaliper();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	BOOL m_bEnableDualEdge;
	int m_nImageChannel;
};
