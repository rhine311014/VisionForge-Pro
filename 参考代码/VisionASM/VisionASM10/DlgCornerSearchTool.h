#if !defined(AFX_DLGCORNERSEARCHTOOLfORCORNERSETTING_H__FD052144_C0F9_4940_AE83_7A8B5CEC816D__INCLUDED_)
#define AFX_DLGCORNERSEARCHTOOLSETTING_H__FD052144_C0F9_4940_AE83_7A8B5CEC816D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCornerSearchToolSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCornerSearchTool dialog
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x00000040
#endif //BIF_NEWDIALOGSTYLE
#ifndef BIF_NONEWFOLDERBUTTON 
#define BIF_NONEWFOLDERBUTTON  0x00000200
#endif //BIF_NEWDIALOGSTYLE

class CDlgSearch;
class CDlgCornerSearchTool : public CDialog
{
// Construction
public:
	CDlgCornerSearchTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch	*m_pDlgParent;
	scGuiDisplay		m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// Dialog Data
	//{{AFX_DATA(CDlgCornerSearchTool)
	enum { IDD = IDD_DLG_CORNERSEARCH_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	int		m_nLineCaliperNum1;
	int		m_nLineCaliperNum2;
	double	m_dLineContrastThre1;
	double	m_dLineContrastThre2;
	long	m_lLineFilterHalfWidth1;
	long	m_lLineFilterHalfWidth2;
	int		m_nLineOutLineNum1;
	int		m_nLineOutLineNum2;
	long	m_lNoiseThre;
	long	m_lTotalMag;
	int		m_nLinePolarityMode1;
	int		m_nLinePolarityMode2;
    int     m_nOldLinePolarityMode1;
    int     m_nOldLinePolarityMode2;
	double	m_dGrau;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCornerSearchTool)
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

    void OnLinePolarityChnage1();
    void OnLinePolarityChnage2();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCornerSearchTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnChangeEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoisethre();
	afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();
	afx_msg void OnSetfocusEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineContrastThre2();
	afx_msg void OnSetfocusEdtLineOutlineNum2();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnSetfocusEdtSearchNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	void UpdateDialogLanguage();
public:
	afx_msg void OnEnChangeEdtAcceptThre();
	BOOL m_bSortByScore1;
	BOOL m_bSortByScore2;
	CButton m_checkSortByScore1;
	CButton m_checkSortByScore2;
	
	BOOL m_bEnableDualEdge;
	BOOL m_bEnableDualEdge2;
	BOOL m_bEnableLineMask_1;
	BOOL m_bShowFitPoint_1;
	BOOL m_bEnableLineMask_2;
	BOOL m_bShowFitPoint_2;
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedCheckLineMask2();
	afx_msg void OnBnClickedCheckShowFitpoint2();
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
	afx_msg void OnEnSetfocusEditAngleBase();
	afx_msg void OnEnSetfocusEditAngleFlu();
	afx_msg void OnBnClickedAngleCheck();
	afx_msg void OnBnClickedAngleCorCheck();
    afx_msg void OnBnClickedRadLinePolarityMode01();
    afx_msg void OnBnClickedRadLinePolarityMode11();
    afx_msg void OnBnClickedRadLinePolarityMode21();
    afx_msg void OnBnClickedRadLinePolarityMode02();
    afx_msg void OnBnClickedRadLinePolarityMode12();
    afx_msg void OnBnClickedRadLinePolarityMode22();
    afx_msg void OnBnClickedCheckSortByScore1();
    afx_msg void OnBnClickedCheckSortByScore2();
    afx_msg void OnBnClickedChkEnableDualEdge();
    afx_msg void OnBnClickedChkEnableDualEdge2();
    afx_msg void OnBnClickedRadLineCorMode1();
    afx_msg void OnBnClickedRadLineCorMode2();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_2;
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper2();

	int m_nImageChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCORNERSEARCHTOOLSETTING_H__FD052144_C0F9_4940_AE83_7A8B5CEC816D__INCLUDED_)
