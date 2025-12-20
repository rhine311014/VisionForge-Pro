//#if !defined(AFX_DLGMQUICKPICKCORNERSEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_)
//#define AFX_DLGMQUICKPICKCORNERSEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//
//#include "svGuiDisplay.h"
//#include "svImage.h"
//#include "BtnST.h"
//#include "afxwin.h"
//
//#ifndef BIF_NEWDIALOGSTYLE
//#define BIF_NEWDIALOGSTYLE 0x00000040
//#endif //BIF_NEWDIALOGSTYLE
//#ifndef BIF_NONEWFOLDERBUTTON 
//#define BIF_NONEWFOLDERBUTTON  0x00000200
//#endif //BIF_NEWDIALOGSTYLE
//
//// CDlgMQuickPickCornerSearchTool 对话框
//class CDlgSearch;
//class CDlgMQuickPickCornerSearchTool : public CDialog
//{
//	//DECLARE_DYNAMIC(CDlgMQuickPickCornerSearchTool)
//
//public:
//	CDlgMQuickPickCornerSearchTool(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CDlgMQuickPickCornerSearchTool();
//
//// 对话框数据
//	enum { IDD = IDD_DLG_MQUICKPICKCORNERSEARCH_TOOL };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//
//	DECLARE_MESSAGE_MAP()
//};
//
//#endif // !defined(AFX_DLGQUICKSEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_)
//

#if !defined(AFX_DLGMQUICKPICKCORNERSEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_)
#define AFX_DLGMQUICKPICKCORNERSEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMQuickSearchToolSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMQuickCornerSearchTool dialog
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
class CDlgMQuickPickCornerSearchTool : public CDialog
{
	// Construction
public:
	CDlgMQuickPickCornerSearchTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch *m_pDlgParent;
	scGuiDisplay m_GuiPatternDisplay;
	CStringArray *m_psaSysInfoStrings;
	cpImage	m_imageMask;

	// Dialog Data
	//{{AFX_DATA(CDlgMQuickCornerSearchTool)
	enum { IDD = IDD_DLG_MQUICKPICKCORNERSEARCH_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	long	m_lNoiseThre;
	long	m_lTotalMag;
	double	m_dGrau;
	BOOL	m_bEnableMask;
	BOOL	m_bGetFourResultEnable;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	double	m_dSizeHigh;
	double	m_dSizeLow;
	BOOL	m_bCoordSortEnable;
	double  m_dCoordSortGap;
	CComboBox m_comboCoordSortMethod;
	int		m_nCoordSortMethod;
	int		m_nCoordSortOrderX;
	int		m_nCoordSortOrderY;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMQuickCornerSearchTool)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
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
	//{{AFX_MSG(CDlgMQuickCornerSearchTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkEnableMask();
	afx_msg void OnBtnSetCenter();
	afx_msg void OnSetfocusEdtGrau();
	afx_msg void OnSetfocusEdtNoisethre();
	afx_msg void OnSetfocusEdtTotalMag();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtSizeLow();
	afx_msg void OnSetfocusEdtSizeHigh();
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnKillfocusEdtGrau();
	afx_msg void OnSetfocusEdtSearchNum();
	afx_msg void OnCliperChange();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int      m_nLineOutLineNum[4];
	double   m_dLineContrastThre[4];
	long     m_lLineFilterHalfWidth[4];
	int      m_nLinePolarityMode[4];
	int      m_nLineCaliperNum[4];
	BOOL	 m_bSortByScore[4];
	BOOL	 m_bEnableDualEdge[4];
	BOOL	 m_bEnableLineMask[4];
	BOOL	 m_bShowFitPoint[4];
	BOOL	 m_bShowCaliper[4];

	int      m_nPreCliperIndex;
protected:
	CThemeHelperST		m_themeHelper;

	void UpdateDialogLanguage();
public:
	BOOL m_bEnableMark2;
	afx_msg void OnBnClickedChkEnableMark2();
	afx_msg void OnCbnSelchangeComboCoordSortMethod();
	afx_msg void OnEnSetfocusEdtCoordSortGap2();

	afx_msg void OnEnSetfocusEdtLineCalipernum1();
	afx_msg void OnEnSetfocusEdtLineOutlineNum1();
	afx_msg void OnEnSetfocusEdtLineContrastThre1();
	afx_msg void OnBnClickedChkGetFourResult();


	int m_nCliperIndex;
	int m_nLinePolarityMode1;
	int m_nLineCaliperNum1;
	int m_nLineOutLineNum1;
	int m_dLineContrastThre1;


	BOOL m_bSortByScore1;
	BOOL m_bEnableDualEdge1;
	BOOL m_bEnableLineMask_1;
	BOOL m_bShowFitPoint_1;
	afx_msg void OnBnClickedCheckSortByScore1();
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedChkEnableDualEdge();
	BOOL m_bShowCaliper_1;
	afx_msg void OnBnClickedChkShowLineCaliper();
	afx_msg void OnChangeEdtLineCalipernum1();
	long m_lLineFilterHalfWidth1;
	int m_nImageChannel;
	BOOL m_bEnable24CorLineCombine;
	BOOL m_bEnable13ProjectTo24Combine;
	BOOL m_bEnableOutput13MidCombine;
	afx_msg void OnBnClickedChkOutput13Mid();
	afx_msg void OnBnClickedChk24CorLineCombine();
	afx_msg void OnBnClickedChkShowHideSearchRect();
	// 夹爪1屏蔽区域启用
	BOOL m_bSearchHideRectVisible1;
	afx_msg void OnBnClickedChkShowHideSearchRect1();
	// 夹爪2屏蔽区域
	BOOL m_bSearchHideRectVisible2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQUICKSEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_)















