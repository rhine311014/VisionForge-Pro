#if !defined(AFX_DLGPROCIRCLESEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_)
#define AFX_DLGPROCIRCLESEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProCircleSearchToolSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProCircleSearchTool dialog
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
class CDlgProCircleSearchTool : public CDialog
{
// Construction
public:
	CDlgProCircleSearchTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch *m_pDlgParent;
	scGuiDisplay m_GuiPatternDisplay;
	CStringArray *m_psaSysInfoStrings;
	cpImage	m_imageMask;

// Dialog Data
	//{{AFX_DATA(CDlgProCircleSearchTool)
	enum { IDD = IDD_DLG_PROCIRCLESEARCH_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	int		m_nCirclePolarityMode;
	long	m_lNoiseThre;
	long	m_lTotalMag;
	double	m_dGrau;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProCircleSearchTool)
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
	//{{AFX_MSG(CDlgProCircleSearchTool)
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
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnKillfocusEdtGrau();
	afx_msg void OnSetfocusEdtSearchNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	void UpdateDialogLanguage();
public:
	int m_nImageChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROCIRCLESEARCHTOOLSETTING_H__6AA12E6D_AE79_4FA3_8F5C_5DA2B54D13D8__INCLUDED_)
