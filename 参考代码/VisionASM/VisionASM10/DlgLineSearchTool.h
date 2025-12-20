#if !defined(AFX_DLGLINESEARCHTOOLSETTING_H__DEC9A236_3947_4D90_83C4_398DA372A3D5__INCLUDED_)
#define AFX_DLGLINESEARCHTOOLSETTING_H__DEC9A236_3947_4D90_83C4_398DA372A3D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLineSearchToolSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLineSearchTool dialog
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
class CDlgLineSearchTool : public CDialog
{
// Construction
public:
	CDlgLineSearchTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch	*m_pDlgParent;
	scGuiDisplay		m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// Dialog Data
	//{{AFX_DATA(CDlgLineSearchTool)
	enum { IDD = IDD_DLG_LINESEARCH_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnGetTrainImage;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	int		m_nLineCaliperNum1;
	double	m_dLineContrastThre1;
	long	m_lLineFilterHalfWidth1;
	int		m_nLineOutLineNum1;
	long	m_lNoiseThre;
	long	m_lTotalMag;
	int		m_nLinePolarityMode1;
	double	m_dGrau;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	BOOL m_bSortByScore1;
	CButton m_checkSortByScore1;
	CComboBox	m_comboLineAngleType;
	int		m_nLineAngleType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLineSearchTool)
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
	//{{AFX_MSG(CDlgLineSearchTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnChangeEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoisethre();
	afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();
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
	afx_msg void OnCbnSelchangeComboLineAngleType();
	BOOL m_bEnableLineMask;
	CButton m_checkLineMask;
	BOOL m_bEnableShowLinePoint;
	CButton m_checkShowLinePoint;
	afx_msg void OnBnClickedCheckEnableLineMask();
	afx_msg void OnBnClickedCheckShowLinePoint();
	BOOL m_bShowCaliper;
	afx_msg void OnBnClickedChkShowLineCaliper();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
	BOOL m_bEnableDualEdge;
	int m_nImageChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINESEARCHTOOLSETTING_H__DEC9A236_3947_4D90_83C4_398DA372A3D5__INCLUDED_)
