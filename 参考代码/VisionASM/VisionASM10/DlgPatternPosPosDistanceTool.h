#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DlgDonleQuicksearchTool 对话框
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x00000040
#endif //BIF_NEWDIALOGSTYLE
#ifndef BIF_NONEWFOLDERBUTTON 
#define BIF_NONEWFOLDERBUTTON  0x00000200
#endif //BIF_NEWDIALOGSTYLE
class CDlgSearch;
class CDlgPatternPosPosDistanceTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPatternPosPosDistanceTool)

public:
	CDlgPatternPosPosDistanceTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternPosPosDistanceTool();

	//训练对话框有关参数传递
	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	scGuiDisplay	m_GuiPatternDisplay2;
	scGuiDisplay	m_GuiPatternDisplay3;
	scImageGray		m_imageMask;			//工具1掩膜设置
	scImageGray		m_imageMask2;			//工具1掩膜设置
	scImageGray		m_imageMask3;			//工具2掩膜设置
	CStringArray*	m_psaSysInfoStrings;


// 对话框数据
	enum { IDD = IDD_DLG_PATTERNPOSPOSDISTATNCE_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask1;
	CXPStyleButtonST	m_btnSetMask2;
	CXPStyleButtonST	m_btnSetCenter1;
	CXPStyleButtonST	m_btnSetCenter2;
	CXPStyleButtonST	m_btnGetTrainImage;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

	void UpdateSearchResult();
	DECLARE_MESSAGE_MAP()
protected:
	CThemeHelperST		m_themeHelper;
	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;

	int		m_nPatSpeedMode1;
	int		m_nPatAccuracyMode1;

	int		m_nPatSpeedMode2;
	int		m_nPatAccuracyMode2;
	void UpdateDialogLanguage();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboPrecision1();
	afx_msg void OnCbnSelchangeComboSpeed1();
	afx_msg void OnCbnSelchangeComboPrecision2();
	afx_msg void OnCbnSelchangeComboSpeed2();
	afx_msg void OnBnClickedBtnGetImg();
	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnSetMark1();
	afx_msg void OnBnClickedBtnSetMark2();
	afx_msg void OnBnClickedBtnTool1Searchall();
	afx_msg void OnBnClickedBtnTool2Searchall();
	afx_msg void OnBnClickedBtnSetMask1();
	afx_msg void OnBnClickedBtnSetMask2();

	afx_msg void OnSetfocusEdtAngleLow1();
	afx_msg void OnSetfocusEdtAngleHigh1();
	afx_msg void OnSetfocusEdtAcceptThre1();
	afx_msg void OnSetfocusEdtConfusionThre1();
	afx_msg void OnSetfocusEdtSearchNum1();
	afx_msg void OnSetfocusEdtAngleLow2();
	afx_msg void OnSetfocusEdtAngleHigh2();
	afx_msg void OnSetfocusEdtAcceptThre2();
	afx_msg void OnSetfocusEdtConfusionThre2();
	afx_msg void OnSetfocusEdtSearchNum2();
public:
	// 工具角度下限值
	double m_dAngleLow;
	// 工具角度上限值
	double m_dAngleHigh;
	// 工具启用标记点
	BOOL m_bEnableMask;
	// 工具相似度初步筛选
	double m_dAcceptThre;
	// 工具最终相似度筛选
	double m_dConfusionThre;
	// 工具搜索个数
	int m_nSearchNum;
	// 工具精度设置
	CComboBox m_comboAccuracy;
	// 工具速度设置
	CComboBox m_comboSpeed;

	// 工具1角度下限值
	double m_dAngleLow1;
	// 工具1角度上限值
	double m_dAngleHigh1;
	// 工具2角度下限值
	double m_dAngleLow2;
	// 工具2角度上限值
	double m_dAngleHigh2;
	// 工具1启用标记点
	BOOL m_bEnableMask1;
	// 工具2启用标记点
	BOOL m_bEnableMask2;
	// 工具1相似度初步筛选
	double m_dAcceptThre1;
	// 工具2初步相似度筛选
	double m_dAcceptThre2;
	// 工具1最终相似度筛选
	double m_dConfusionThre1;
	// 工具2最终相似度筛选
	double m_dConfusionThre2;
	// 工具1搜索个数
	int m_nSearchNum1;
	// 工具2搜索个数
	int m_nSearchNum2;
	// 工具1精度设置
	CComboBox m_comboAccuracy1;
	// 工具2精度设置
	CComboBox m_comboAccuracy2;
	// 工具1速度设置
	CComboBox m_comboSpeed1;
	// 工具2速度设置
	CComboBox m_comboSpeed2;
	double m_dPixelRatio;
	BOOL m_bFirstSearchToolConst;
	BOOL m_bSecondSearchToolConst;
	BOOL m_bEnableMax1;
	BOOL m_bEnableMax3;
	BOOL m_bEnableMax2;

	BOOL m_bEnableMin1;
	BOOL m_bEnableMin2;
	BOOL m_bEnableMin3;
	
	double m_dDistanceMin1;
	double m_dDistanceMin2;
	double m_dDistanceMin3;

	double m_dDistanceMax1;
	double m_dDistanceMax2;
	double m_dDistanceMax3;

	double m_dDistance1;
	double m_dDistance2;
	double m_dDistance3;
	afx_msg void OnBnClickedCheckEnableMask1();
	afx_msg void OnBnClickedCheckEnablemask2();
	afx_msg void OnBnClickedCheckFirstSerchToolConstPoint();
	afx_msg void OnBnClickedCheckSecondSerchToolConstPoint();
	afx_msg void OnBnClickedCheckDistanceMax1();
	afx_msg void OnBnClickedCheckDistanceMin1();
	afx_msg void OnEnSetfocusEdtPixelRatio();
	afx_msg void OnEnSetfocusEdtDistanceMax1();
	afx_msg void OnEnSetfocusEdtDistanceMin1();
	afx_msg void OnBnClickedCheckDistanceMax2();
	afx_msg void OnBnClickedCheckDistanceMin3();
	afx_msg void OnBnClickedCheckDistanceMax3();
	afx_msg void OnBnClickedCheckDistanceMin4();
	afx_msg void OnEnSetfocusEdtDistanceMax2();
	afx_msg void OnEnSetfocusEdtDistanceMin3();
	afx_msg void OnEnSetfocusEdtDistanceMax3();
	afx_msg void OnEnSetfocusEdtDistanceMin4();


	BOOL m_bEnablePosPosDist;
	afx_msg void OnBnClickedCheckEnablePosposDist();
	afx_msg void OnBnClickedBtnSetMask();
	afx_msg void OnBnClickedCheckEnableMask();
	afx_msg void OnBnClickedBtnSetCenter();
	afx_msg void OnCbnSelchangeComboPrecision();
	afx_msg void OnCbnSelchangeComboSpeed();
	afx_msg void OnEnSetfocusEditLow();
	afx_msg void OnEnSetfocusEditHigh();
	BOOL m_bEnableFirstCoarsePattern;
	afx_msg void OnBnClickedCheckEnableFirstCoarsePattern();
	afx_msg void OnBnClickedBtnFullImageSearch();
};
