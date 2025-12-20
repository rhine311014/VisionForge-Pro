#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"

// CDlgPatternCornerMulCircleSearchTool 对话框

#define DLG_PATTERN_CIRCLE_NUM 2
#define DLG_PATTERN_LINE_NUM 2

class CDlgSearch;
class CDlgPatternCornerMulCircleSearchTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgPatternCornerMulCircleSearchTool)

public:
	CDlgPatternCornerMulCircleSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternCornerMulCircleSearchTool();

	// 对话框数据
	enum { IDD = IDD_DLG_PATTERNCORNERMULCIRCLESEARCHTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

	int      m_nPreCircleIndex;
	int      m_nTempCircleCaliperNum[DLG_PATTERN_CIRCLE_NUM];
	int      m_nTempCircleOutLineNum[DLG_PATTERN_CIRCLE_NUM];
	double   m_dTempCircleContrastThre[DLG_PATTERN_CIRCLE_NUM];
	int      m_nTempCirclePolarityMode[DLG_PATTERN_CIRCLE_NUM];
	int		 m_nTempCircleInwardMode[DLG_PATTERN_CIRCLE_NUM];
	long     m_lTempCircleFilterHalfWidth[DLG_PATTERN_CIRCLE_NUM];
	BOOL	 m_bTempCircleSortByScore[DLG_PATTERN_CIRCLE_NUM];
	BOOL	 m_bTempEnableShowCirclePoint[DLG_PATTERN_CIRCLE_NUM];
	BOOL	 m_bTempShowCircleCaliper[DLG_PATTERN_CIRCLE_NUM];
	BOOL	 m_bTempEnableCircleMask[DLG_PATTERN_CIRCLE_NUM];

	int		 m_nPreLineIndex;
	int      m_nTempLineCaliperNum[DLG_PATTERN_LINE_NUM];
	int		 m_nTempLineOutLineNum[DLG_PATTERN_LINE_NUM];
	double   m_dTempLineContrastThre[DLG_PATTERN_LINE_NUM];
	int      m_nTempLinePolarityMode[DLG_PATTERN_LINE_NUM];
	long     m_lTempLineFilterHalfWidth[DLG_PATTERN_LINE_NUM];
	BOOL	 m_bTempLineSortByScore[DLG_PATTERN_LINE_NUM];
	BOOL	 m_bTempEnableShowLinePoint[DLG_PATTERN_LINE_NUM];
	BOOL	 m_bTempShowLineCaliper[DLG_PATTERN_LINE_NUM];
	BOOL	 m_bTempEnableLineDualEdge[DLG_PATTERN_LINE_NUM];
	BOOL	 m_bTempEnableCircleDualEdge[DLG_PATTERN_LINE_NUM];
	BOOL	 m_bTempEnableLineMask[DLG_PATTERN_LINE_NUM];
public:
	void UpdateDialogLanguage();

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


public:
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;
	CThemeHelperST		m_themeHelper;

	int m_iAccracy;
	int m_iCoarseScale;
	double m_dAngleLow;
	double m_dAngleHigh;


	BOOL m_bEnableMask;

	int m_nLineAngleType;
	CComboBox m_comboLineAngleType;
	CComboBox m_comboAccuracy;
	CComboBox m_comboSpeed;

	int m_nCircleIndex;
	int m_nCircleCaliperNum;
	int m_nCircleOutLineNum;
	double m_dCircleContrastThre;
	int m_nCirclePolarityMode;
	int m_nCircleInwardMode;
	BOOL m_bCircleSortByScore;
	CButton m_checkCircleSortByScore;
	BOOL m_bEnableShowCirclePoint;
	CButton m_checkShowCirclePoint;
	long m_lCircleFilterHalfWidth;

	int m_nLineIndex;
	int m_nLineCaliperNum;
	int m_nLineOutLineNum;
	double m_dLineContrastThre;
	int m_nLinePolarityMode;
	BOOL m_bLineSortByScore;
	CButton m_checkLineSortByScore;
	BOOL m_bEnableLineMask;
	CButton m_checkLineEnableMask;
	BOOL m_bEnableShowLinePoint;
	CButton m_checkShowLinePoint;


	double m_dAcceptThre;
	int m_nSearchNum;
	double m_dConfusionThre;
	long m_lLineFilterHalfWidth;
	virtual BOOL OnInitDialog();

	//afx_msg void OnSetfocusEdtGrau();
	//afx_msg void OnSetfocusEdtNoisethre();
	//afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnBnClickedChkEnableMask();
	afx_msg void OnBnClickedBtnSetMaskImage();
	afx_msg void OnBnClickedBtnGetTrainImage();

	afx_msg void OnEnChangeEdtCircleCalipernum();
	afx_msg void OnBnClickedRadCirclePolarityMode0();
	afx_msg void OnBnClickedRadCircleInwardMode0();
	afx_msg void OnBnClickedCheckSortByScore1();
	afx_msg void OnBnClickedCheckShowLinePoint();
	afx_msg void OnBnClickedRadioCircleIndex1();

	afx_msg void OnCbnSelchangeComboLineAngleType();

	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnFullImageSearch();

	afx_msg void OnSetfocusEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleOutlineNum();
	afx_msg void OnSetfocusEdtCircleContrastThre();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtSearchConfusionThre();
	afx_msg void OnEnSetfocusEdtConfusionThre();
	afx_msg void OnBnClickedRadioLineIndex1();
	//afx_msg void OnBnClickedRadioLineIndex2();
	afx_msg void OnEnSetfocusEdtLineCalipernum();
	afx_msg void OnEnSetfocusEdtLineOutlineNum();
	afx_msg void OnEnSetfocusEdtLineContrastThre();
	afx_msg void OnEnChangeEdtLineCalipernum();
	afx_msg void OnBnClickedCheckLineShowLinePoint();
	afx_msg void OnBnClickedCheckLineSortByScore();
	afx_msg void OnBnClickedRadLinePolarityMode1();
	afx_msg void OnBnClickedCheckLineEnableMask();
	BOOL m_bShowLineCaliper;
	BOOL m_bShowCircleCaliper;
	afx_msg void OnBnClickedChkShowLineCaliper();
	afx_msg void OnBnClickedChkShowCircleCaliper();
	BOOL m_bEnableLineDualEdge;
	BOOL m_bEnableCircleDualEdge;
	BOOL m_bEnableCircleMask;
	afx_msg void OnBnClickedCheckCircleMask();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	int m_nImageChannel;
};
