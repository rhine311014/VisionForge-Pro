#if !defined(AFX_DLGAREAINSPECT_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_)
#define AFX_DLGAREAINSPECT_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAreaInspect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgAreaInspect dialog
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
class CDlgAreaInspect : public CDialog
{
	// Construction
public:
	CDlgAreaInspect(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch		*m_pDlgParent;
	scGuiDisplay	m_GuiPatternDisplay;
//	scImageGray		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;
    cpImage	m_imageMask;
	// Dialog Data
	//{{AFX_DATA(CDlgPatternSearchTool)
	enum { IDD = IDD_DLG_AREAINSPECT_TOOL };
	CXPStyleButtonST	m_btnTrain;
// 	CXPStyleButtonST	m_btnSetMask;
// 	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
 	CComboBox	m_comboDefectType;
// 	CComboBox	m_comboAccuracy;
 	int		m_nBinaryThre;
 	int  	m_nMinAcceptArea;
 	int 	m_nMaxAcceptArea;
 	double	m_dMaxDefectArea;
 	double	m_dDefectArea;
 	int 	m_nMaxGreyVal;

    double	m_dAcceptThre;
    long	m_lNoiseThre;
    long	m_lTotalMag;
    double	m_dGrau;
    BOOL	m_bEnableMask;
    double	m_dAngleHigh;
    double	m_dAngleLow;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPatternSearchTool)
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

	void UpdateSearchResult();

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPatternSearchTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	virtual BOOL OnInitDialog();
 	afx_msg void OnSelchangeComboDefectType();
 	afx_msg void OnBtnFullImageSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	int		m_nDefectType;

	void UpdateDialogLanguage();
public:
	afx_msg void OnEnSetfocusEdtBinaryThre();
	afx_msg void OnEnSetfocusEdtMinArea();
	afx_msg void OnEnSetfocusEdtMaxArea();
	afx_msg void OnEnSetfocusEdtMaxDefectArea();
    BOOL m_bEnableBaseSearch;
    afx_msg void OnEnSetfocusEdtGrau();
    afx_msg void OnEnSetfocusEdtNoisethre();
    afx_msg void OnEnSetfocusEdtTotalMag();
    afx_msg void OnEnSetfocusEdtAcceptThre();
    afx_msg void OnEnSetfocusEdtAngleLow();
    afx_msg void OnEnSetfocusEdtAngleHigh();
    afx_msg void OnBnClickedChkEnableMask();
    afx_msg void OnBnClickedBtnSetMaskImage();
    afx_msg void OnBnClickedBtnSetCenter();
    afx_msg void OnBnClickedCheckEnableBaseSearch();
	int m_nImageChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAREAINSPECT_H__9E83C67C_E77B_43B6_ABBD_90BF3E5F3D95__INCLUDED_)
