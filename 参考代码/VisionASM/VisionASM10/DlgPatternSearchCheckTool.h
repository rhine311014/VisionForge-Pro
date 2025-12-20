#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"

// CDlgPatternSearchCheckTool 对话框

class CDlgSearch;
class CDlgPatternSearchCheckTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternSearchCheckTool)

public:
	CDlgPatternSearchCheckTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternSearchCheckTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	scGuiDisplay	m_GuiPatternDisplayCheck1;
	scGuiDisplay	m_GuiPatternDisplayCheck2;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;
	
// 对话框数据
	enum { IDD = IDD_DLG_COMBINED_PATTERNSEARCH_TOOL };

	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;

	CComboBox	m_comboSpeed;
	CComboBox	m_comboAccuracy;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	BOOL	m_bEnableCheckRegion1;
	BOOL	m_bEnableCheckRegion2;


	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dConfusionThre;
	double m_dAcceptThre2;
	double m_dConfusionThre2;
	int		m_nImageChannel;




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
	afx_msg void OnEnSetfocusEdtCheckAcceptThre();
	afx_msg void OnEnSetfocusEdtCheckConfusionThre();
	afx_msg void OnBnClickedCheckRegion1Enable();
	afx_msg void OnBnClickedCheckRegion2Enable();
};
