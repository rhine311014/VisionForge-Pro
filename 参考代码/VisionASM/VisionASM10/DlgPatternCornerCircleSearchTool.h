#pragma once


#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

#define LINE_NUM_TEMP 2

// CDlgPatternCornerCircleSearchTool 对话框

class CDlgSearch;
class CDlgPatternCornerCircleSearchTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgPatternCornerCircleSearchTool)

public:
	CDlgPatternCornerCircleSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPatternCornerCircleSearchTool();

// 对话框数据
	enum { IDD = IDD_DLG_PATTERNCORNERCIRCLESEARCH_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()

//public:

	CXPStyleButtonST m_btnSetMask;
	CXPStyleButtonST m_btnGetTrainImage;
	CXPStyleButtonST m_btnTrain;
	CXPStyleButtonST m_btnFullImageSearch;

	CComboBox m_comboAccuracy;
	CComboBox m_comboSpeed;
	int m_nPatSpeedMode;
	int m_nPatAccuracyMode;
	BOOL m_bEnableMask;
	double m_dAngleLow;
	double m_dAngleHigh;

	int m_nLineCaliperIndex;
	int m_nLineCaliperNum;
	int m_nLineOutLineNum;
	double m_dLineContrastThre;
	int m_nLinePolarityMode;
	long m_lLineFilterHalfWidth;
	BOOL m_bLineSortByScore;
	CButton m_checkLineSortByScore;
	BOOL m_bEnableLineMask;
	BOOL m_bShowLineFitPoint;

	int		m_nLineAngleType;
	CComboBox m_comboLineAngleType;

	int m_nCircleCaliperNum;
	int m_nCircleOutLineNum;
	double m_dCircleContrastThre;
	int m_nCirclePolarityMode;
	int m_nCircleInwardMode;
	long m_lCircleFilterHalfWidth;
	BOOL m_bCircleSortByScore;
	CButton m_checkCircleSortByScore;
	BOOL m_bShowCircleFitPoint;

	double m_dAcceptThre;
	double m_dConfusionThre;
	int m_nSearchNum;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedChkEnableMask();
	afx_msg void OnBnClickedBtnSetMaskImage();
	afx_msg void OnBnClickedBtnGetTrainImage();
	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnFullImageSearch();
	afx_msg void OnLineCaliperChange();
	afx_msg void OnBnClickedCheckLineMask();
	afx_msg void OnBnClickedCheckShowLineFitpoint();
	afx_msg void OnBnClickedCheckShowCircleFitpoint();

	afx_msg void OnKillfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnKillfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnEnChangeEdtLineCalipernum();
	afx_msg void OnKillfocusEdtLineCalipernum();
	afx_msg void OnSetfocusEdtLineCalipernum();
	afx_msg void OnKillfocusEdtLineOutlineNum();
	afx_msg void OnSetfocusEdtLineOutlineNum();
	afx_msg void OnKillfocusEdtLineContrastThre();
	afx_msg void OnSetfocusEdtLineContrastThre();

	afx_msg void OnEnChangeEdtCircleCalipernum();
	afx_msg void OnKillfocusEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleCalipernum();
	afx_msg void OnKillfocusEdtCircleOutlineNum();
	afx_msg void OnSetfocusEdtCircleOutlineNum();
	afx_msg void OnKillfocusEdtCircleContrastThre();
	afx_msg void OnSetfocusEdtCircleContrastThre();
	afx_msg void OnKillfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnKillfocusEdtConfusionThre();
	afx_msg void OnSetfocusEdtConfusionThre();
	afx_msg void OnKillfocusEdtSearchNum();
	afx_msg void OnSetfocusEdtSearchNum();



public:

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;
	CThemeHelperST		m_themeHelper;

	int      m_nPreLineCaliperIndex;
	int      m_nTempLineCaliperNum[LINE_NUM_TEMP];
	int      m_nTempLineOutLineNum[LINE_NUM_TEMP];
	double   m_dTempLineContrastThre[LINE_NUM_TEMP];
	int      m_nTempLinePolarityMode[LINE_NUM_TEMP];
	long     m_lTempLineFilterHalfWidth[LINE_NUM_TEMP];
	BOOL	 m_bTempLineSortByScore[LINE_NUM_TEMP];
	BOOL     m_bTempShowLineFitPoint[LINE_NUM_TEMP];
	BOOL     m_bTempShowLineCaliper[LINE_NUM_TEMP];
	BOOL	 m_bTempEnableLineMask[LINE_NUM_TEMP];
	BOOL	 m_bTempEnableLineDualEdge[LINE_NUM_TEMP];

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

	
	afx_msg void OnBnClickedChkShowLineCaliper();
	afx_msg void OnBnClickedChkShowCircleCaliper();
	BOOL m_bShowLineCaliper;
	BOOL m_bShowCircleCaliper;
	afx_msg void OnBnClickedRadCircleInwardMode1();
	afx_msg void OnBnClickedRadCircleInwardMode0();
	BOOL m_bEnableLineDualEdge;
	BOOL m_bEnableCircleDualEdge;
	BOOL m_bEnableCircleMask;
	afx_msg void OnBnClickedCheckCircleMask();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth();
	int m_nImageChannel;
};
