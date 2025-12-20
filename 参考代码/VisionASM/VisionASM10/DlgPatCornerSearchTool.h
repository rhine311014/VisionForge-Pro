#if !defined(AFX_DLGPATCORNERSEARCHTOOLSETTING_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_)
#define AFX_DLGPATCORNERSEARCHTOOLSETTING_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGPATCORNERSEARCHTOOLSETTING.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPatCornerSearchTool dialog
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
class CDlgPatCornerSearchTool : public CDialog
{
// Construction
public:
	CDlgPatCornerSearchTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	scGuiDisplay	m_GuiPatternDisplay2;
	cpImage		m_imageMask2;
	CStringArray*	m_psaSysInfoStrings;

// Dialog Data
	//{{AFX_DATA(CDlgPatCornerSearchTool)
	enum { IDD = IDD_DLG_PATCORNERSEARCH_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetMask2;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	CComboBox	m_comboSpeed;
	CComboBox	m_comboAccuracy;
	CComboBox	m_comboSpeed2;
	CComboBox	m_comboAccuracy2;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	BOOL	m_bEnableMask2;
	double	m_dAngleHigh2;
	double	m_dAngleLow2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPatCornerSearchTool)
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
	//{{AFX_MSG(CDlgPatCornerSearchTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	afx_msg void OnBtnSetMaskImage2();
	afx_msg void OnChkEnableMask2();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAccuracyMode();
	afx_msg void OnSelchangeComboSpeedMode();
	afx_msg void OnSelchangeComboAccuracyMode2();
	afx_msg void OnSelchangeComboSpeedMode2();
	afx_msg void OnBtnSetCenter();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAngleLow2();
	afx_msg void OnSetfocusEdtAngleHigh2();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnSetfocusEdtSearchNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	int		m_nPatSpeedMode2;
	int		m_nPatAccuracyMode2;
	void UpdateDialogLanguage();
public:
	int m_nImageChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPATCORNERSEARCHTOOLSETTING_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_)
