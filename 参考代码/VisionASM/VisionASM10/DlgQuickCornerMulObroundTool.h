#pragma once


// CDlgPatternCornerMulObroundTool dialog
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"


#define DLG_QUICK_LINE_NUM 2
#define DLG_QUICK_OBROUND_NUM 2


class CDlgSearch;
class CDlgQuickCornerMulObroundTool : public CDialog
{
	// Construction
public:
	CDlgQuickCornerMulObroundTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

	// Dialog Data
	//{{AFX_DATA(CDlgPatternCornerMulObroundTool)
	enum { IDD = IDD_DLG_QUICK_CORNER_MUIOBROUND_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	long	m_lNoiseThre;
	long	m_lTotalMag;
	double	m_dGrau;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	int		m_nPreObroundIndex;
	int		m_nTempCircleInwardMode[DLG_QUICK_OBROUND_NUM];
	int		m_nTempCirclePolarityMode[DLG_QUICK_OBROUND_NUM];
	int		m_nTempCircleCaliperNum[DLG_QUICK_OBROUND_NUM];
	double	m_dTempCircleContrastThre[DLG_QUICK_OBROUND_NUM];
	long	m_lTempCircleFilterHalfWidth[DLG_QUICK_OBROUND_NUM];
	int		m_nTempCircleOutLineNum[DLG_QUICK_OBROUND_NUM];
	BOOL	m_bTempSortByScore1[DLG_QUICK_OBROUND_NUM];
	BOOL	m_bTempShowCaliper[DLG_QUICK_OBROUND_NUM];
	BOOL	m_bTempLineEnableDualEdge[DLG_QUICK_OBROUND_NUM];
	int		m_nObroundIndex;
	int		m_nCircleInwardMode;
	int		m_nCirclePolarityMode;
	int		m_nCircleCaliperNum;
	double	m_dCircleContrastThre;
	long	m_lCircleFilterHalfWidth;
	int		m_nCircleOutLineNum;
	BOOL	m_bSortByScore1;
	CButton m_checkSortByScore1;
	BOOL	m_bEnableMulObround;
	int		m_nObroundOutputResult;
	BOOL	m_bShowCaliper;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPatternCornerMulObroundTool)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

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

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPatternCornerMulObroundTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSetCenter();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnChangeEdtCircleCalipernum();
	afx_msg void OnRadCircleInwardMode0();
	afx_msg void OnRadCircleInwardMode1();
	afx_msg void OnSetfocusEdtCircleCalipernum();
	afx_msg void OnSetfocusEdtCircleContrastThre();
	afx_msg void OnSetfocusEdtCircleOutlineNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();

public:
	int		 m_nPreLineIndex;
	int      m_nTempLineCaliperNum[DLG_QUICK_LINE_NUM];
	int		 m_nTempLineOutLineNum[DLG_QUICK_LINE_NUM];
	double   m_dTempLineContrastThre[DLG_QUICK_LINE_NUM];
	int      m_nTempLinePolarityMode[DLG_QUICK_LINE_NUM];
	long     m_lTempLineFilterHalfWidth[DLG_QUICK_LINE_NUM];
	BOOL	 m_bTempLineSortByScore[DLG_QUICK_LINE_NUM];
	BOOL	 m_bTempEnableShowLinePoint[DLG_QUICK_LINE_NUM];
	BOOL	 m_bTempLineEnableMask[DLG_QUICK_LINE_NUM];
	afx_msg void OnBnClickedRadioLineIndex1();
	int m_nLineIndex;
	int m_nLineCaliperNum;
	int m_nLineOutLineNum;
	double m_dLineContrastThre;
	int m_nLinePolarityMode;
	BOOL m_bLineSortByScore;
	CButton m_checkLineSortByScore;
	BOOL m_bLineEnableMask;
	CButton m_checkLineEnableMask;
	BOOL m_bEnableShowLinePoint;
	CButton m_checkShowLinePoint;
	long m_lLineFilterHalfWidth;
	afx_msg void OnEnSetfocusEdtLineCalipernum();
	afx_msg void OnEnSetfocusEdtLineOutlineNum();
	afx_msg void OnEnSetfocusEdtLineContrastThre();
	afx_msg void OnEnChangeEdtLineCalipernum();
	afx_msg void OnBnClickedCheckLineShowLinePoint();
	afx_msg void OnBnClickedCheckLineSortByScore();
	afx_msg void OnBnClickedRadLinePolarityMode1();
	afx_msg void OnBnClickedCheckLineEnableMask();
	afx_msg void OnBnClickedRadioObroundIndex1();
	afx_msg void OnBnClickedRadioObroundIndex2();

	
	afx_msg void OnBnClickedChkMulObround();
	afx_msg void OnBnClickedRadCircleResultCentreCircle();
	afx_msg void OnBnClickedRadCircleResultRightCircle();
	afx_msg void OnBnClickedRadCircleResultLeftCircle();
	
	afx_msg void OnBnClickedChkShowLineCaliper();
	BOOL m_bLineEnableDualEdge;
	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnKillfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoise();
	afx_msg void OnKillfocusEdtNoise();
	afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnKillfocusEdtTotalMag();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditCircleFilterHalfWidth1();
	int m_nImageChannel;
};

