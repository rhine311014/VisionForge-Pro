#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"

// CDlgCombinedPatternSearchTool 对话框

class CDlgSearch;
class CDlgCombinedPatternSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCombinedPatternSearchTool)

public:
	CDlgCombinedPatternSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCombinedPatternSearchTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	scGuiDisplay	m_GuiPatternDisplay2;
	cpImage		m_imageMask;
	cpImage		m_imageMask1;
	CStringArray*	m_psaSysInfoStrings;
	
// 对话框数据
	enum { IDD = IDD_DLG_COMBINED_PATTERNSEARCH_TOOL };

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
	BOOL	m_bEnableMask1;
	double	m_dAngleHigh;
	double	m_dAngleLow;



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

protected:
	CThemeHelperST		m_themeHelper;

	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnGetTrainImage();
	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnSetCenter();
	afx_msg void OnBnClickedBtnSetMaskImage();
	afx_msg void OnBnClickedChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboAccuracyMode();
	afx_msg void OnCbnSelchangeComboSpeedMode();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtConfusionThre();
	afx_msg void OnBnClickedBtnFullImageSearch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnSetMaskImage2();
	afx_msg void OnBnClickedChkEnableMask2();
	
	int m_nPostion;
	double m_dAcceptThre2;
	double m_dConfusionThre2;
	afx_msg void OnEnSetfocusEdtAcceptThre3();
	afx_msg void OnEnSetfocusEdtConfusionThre2();
	int m_nImageChannel;
};
