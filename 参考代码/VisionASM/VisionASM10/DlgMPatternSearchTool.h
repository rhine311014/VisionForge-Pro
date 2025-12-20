#if !defined(AFX_DLGMPATTERNSEARCHTOOLSETTING_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_)
#define AFX_DLGMPATTERNSEARCHTOOLSETTING_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPatternSearchToolSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMPatternSearchTool dialog
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
class CDlgMPatternSearchTool : public CDialog
{
// Construction
public:
	CDlgMPatternSearchTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
	cpImage		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

// Dialog Data
	//{{AFX_DATA(CDlgMPatternSearchTool)
	enum { IDD = IDD_DLG_MPATTERNSEARCH_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetMask;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	CComboBox	m_comboSpeed;
	CComboBox	m_comboAccuracy;
	int		m_nSearchNum;
	double	m_dAcceptThre;
	double	m_dConfusionThre;
	BOOL	m_bEnableMask;
	double	m_dAngleHigh;
	double	m_dAngleLow;
	BOOL	m_bCoordSortEnable;
	double  m_dCoordSortGap;
	CComboBox m_comboCoordSortMethod;
	int		m_nCoordSortMethod;
	int		m_nCoordSortOrderX;
	int		m_nCoordSortOrderY;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMPatternSearchTool)
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
	//{{AFX_MSG(CDlgMPatternSearchTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnSetMaskImage();
	afx_msg void OnChkEnableMask();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAccuracyMode();
	afx_msg void OnSelchangeComboSpeedMode();
	afx_msg void OnBtnSetCenter();
	afx_msg void OnSetfocusEdtAngleLow();
	afx_msg void OnSetfocusEdtAngleHigh();
	afx_msg void OnSetfocusEdtAcceptThre();
	afx_msg void OnSetfocusEdtConfusionThre();
	afx_msg void OnBtnFullImageSearch();
	afx_msg void OnSetfocusEdtSearchNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	int		m_nPatSpeedMode;
	int		m_nPatAccuracyMode;
	void UpdateDialogLanguage();
public:
	BOOL m_bEnableMark2;
	afx_msg void OnBnClickedChkEnableMark2();
	afx_msg void OnCbnSelchangeComboCoordSortMethod();
	afx_msg void OnEnSetfocusEdtCoordSortGap();
	int m_nImageChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPATTERNSEARCHTOOLSETTING_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_)
