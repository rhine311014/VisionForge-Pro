#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgPatternBlobSearchTool 对话框

class CDlgSearch;
class CDlgPatternBlobSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternBlobSearchTool)

public:
	CDlgPatternBlobSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternBlobSearchTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// 对话框数据
	enum { IDD = IDD_DLG_PATTERNBLOBSEARCH_TOOL };

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
	BOOL    m_bPatternSearchEnable;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	int 	m_nBlobMode;				
	long	m_lBlobGrayThre;		
	long	m_lBlobAreaMinThre;	   
	long	m_lBlobAreaMaxThre;	   




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
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();
public:
	

	afx_msg void OnBnClickedChkEnablePatternsearch();
	afx_msg void OnEnSetfocusEdtThreshold();
	afx_msg void OnEnSetfocusEdtBlobMinArea();
	afx_msg void OnEnSetfocusEdtBlobMaxArea();
	BOOL m_bCoordSortEnable;
	double m_dCoordSortGap;
	CComboBox m_comboCoordSortMethod;
	int		m_nCoordSortMethod;
	int		m_nCoordSortOrderX;
	int		m_nCoordSortOrderY;
	afx_msg void OnCbnSelchangeComboCoordSortMethod();
	afx_msg void OnEnSetfocusEdtCoordSortGap();
	int m_nImageChannel;
};
