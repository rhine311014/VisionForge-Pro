#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgPatternCircleLineSearchTool 对话框

class CDlgSearch;
class CDlgQuickCircle4LineSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgQuickCircle4LineSearchTool)

public:
	CDlgQuickCircle4LineSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuickCircle4LineSearchTool();

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// 对话框数据
	enum { IDD = IDD_DLG_QUICK_CIRCLE_4LINESEARCH_TOOL };

	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;

	int		m_nLineCaliperNum1;
	double	m_dLineContrastThre1;
	long	m_lLineFilterHalfWidth1;
	int		m_nLineOutLineNum1;
	int		m_nLinePolarityMode1;

	//圆参数
	int		m_nCircleInwardMode;
	int		m_nCirclePolarityMode;
	int		m_nCircleCaliperNum;
	double	m_dCircleContrastThre;
	int		m_nCircleOutLineNum;
	BOOL	m_bCircleSortByScore1;
	long	m_lCircleFilterHalfWidth;


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

	// 卡尺的参数和界面之间的数据交互
	void UpdateTrainLineData(bool bValidate);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
//	afx_msg void OnBtnSetCenter();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAcceptThre();
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
	int m_nCurFindLineIndex;
	afx_msg void OnBnClickedRadioFindlineIndex1();
	afx_msg void OnBnClickedRadioFindlineIndex2();
	afx_msg void OnBnClickedRadioFindlineIndex3();
	afx_msg void OnBnClickedRadioFindlineIndex4();
	afx_msg void OnEnChangeEdtCircleCalipernum();
	afx_msg void OnEnSetfocusEdtCircleCalipernum();
	afx_msg void OnEnSetfocusEdtCircleOutlineNum();
	afx_msg void OnEnSetfocusEdtCircleContrastThre();
	afx_msg void OnBnClickedRadCircleInwardMode0();
	afx_msg void OnBnClickedRadCircleInwardMode1();
	CButton m_checkCircleSortByScore1;
	CButton m_checkShowFitCirclePoint;
	BOOL m_bEnableShowFitCirclePoint;
	afx_msg void OnBnClickedCheckShowFitCirclePoint();
	afx_msg void OnBnClickedCheckShowFitLinePoint();
	CButton m_checkShowFitLinePoint;
	BOOL m_bEnableShowFitLinePoint;
	CComboBox m_combSearchOutLineAngleType;
	afx_msg void OnCbnSelchangeComboSearchOutLineAngleType();
	int m_nSearchOutLineAngleType;
	int m_nSearchOutPointType;
	afx_msg void OnBnClickedRadSearchOutPointType0();
	afx_msg void OnBnClickedRadSearchOutPointType1();
	afx_msg void OnBnClickedRadSearchOutPointType2();
	afx_msg void OnBnClickedRadSearchOutPointType3();
	BOOL m_bEnableLineAngleJudge;
	double m_dJudgeLineAngle;
	afx_msg void OnBnClickedCheckEnableAngleJudge();
	afx_msg void OnEnSetfocusEditLineAngleJudge();
	afx_msg void OnEnSetfocusEdtGrau();
	afx_msg void OnEnKillfocusEdtGrau();
	afx_msg void OnEnSetfocusEdtNoisethre();
	afx_msg void OnEnSetfocusEdtTotalMag();
	afx_msg void OnEnSetfocusEdtSizeLow();
	afx_msg void OnEnSetfocusEdtSizeHigh();
	long m_lTotalMag;
	long m_lNoiseThre;
	double m_dGrau;
	BOOL m_bEnableLine;
	afx_msg void OnBnClickedCheckEnableLine();
	// 启用两线夹角判断
	BOOL m_bEnable2LineAngleJudge;
	// 90°附近角度范围
	double m_dAngleRange;
	afx_msg void OnEnSetfocusEditAngleRange();
	afx_msg void OnEnSetfocusEdtCircleOutlineNum1();
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowCircleCaliper();
	afx_msg void OnBnClickedCheckCircleMask();
	afx_msg void OnBnClickedCheckLineMask();
	BOOL m_bShowCircleCaliper;
	BOOL m_bShowLineCaliper1;
	BOOL m_bEnableCircleDualEdge;
	BOOL m_bEnableLineDualEdge;
	BOOL m_bEnableCircleMask;
	BOOL m_bEnableLineMask;
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	int m_nImageChannel;
};
