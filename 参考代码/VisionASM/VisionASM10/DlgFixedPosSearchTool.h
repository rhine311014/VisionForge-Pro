#if !defined(AFX_DLGFIXEDPOSSEARCHTOOLSETTING_H__95CAA6B4_EC4C_4DA3_8B1F_D43FAE6A30C8__INCLUDED_)
#define AFX_DLGFIXEDPOSSEARCHTOOLSETTING_H__95CAA6B4_EC4C_4DA3_8B1F_D43FAE6A30C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFixedPosSearchToolSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedPosSearchTool dialog
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
class CDlgFixedPosSearchTool : public CDialog
{
// Construction
public:
	CDlgFixedPosSearchTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch	*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;

	// Dialog Data
	//{{AFX_DATA(CDlgFixedPosSearchTool)
	enum { IDD = IDD_DLG_FIXEDPOSSEARCH_TOOL };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnSetCenter;
	CXPStyleButtonST	m_btnGetTrainImage;
	int		m_nFixedPosMode;
	BOOL m_bEnableSecondModelPoint;
	int		m_nFixedPosMode2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFixedPosSearchTool)
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
	//{{AFX_MSG(CDlgFixedPosSearchTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnRadFixedposMode0();
	afx_msg void OnRadFixedposMode1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSetCenter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	void UpdateDialogLanguage();

	
public:
	afx_msg void OnBnClickedRadFixedpos2Mode0();
	afx_msg void OnBnClickedRadFixedpos2Mode1();
	afx_msg void OnBnClickedBtnSetCenter2();
	afx_msg void OnBnClickedChkEnableMark2();
	int m_nImageChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFIXEDPOSSEARCHTOOLSETTING_H__95CAA6B4_EC4C_4DA3_8B1F_D43FAE6A30C8__INCLUDED_)
