#if !defined(AFX_DLGCORNERSEARCHTOOLfORCORNERSETTING_H__FD052144_C0F9_4940_AE83_7A8B5CEC816D__INCLUDED_)
#define AFX_DLGCORNERSEARCHTOOLSETTING_H__FD052144_C0F9_4940_AE83_7A8B5CEC816D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIdReaderQrCodeToolSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIdReaderDmCodeTool dialog
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
class CDlgIdReaderDmCodeTool : public CDialog
{
// Construction
public:
	CDlgIdReaderDmCodeTool(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch	*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;

// Dialog Data
	//{{AFX_DATA(CDlgIdReaderDmCodeTool)
	enum { IDD = IDD_TOOL_ID_READER_DM_CODE_TOOL };

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIdReaderDmCodeTool)
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
	//{{AFX_MSG(CDlgIdReaderDmCodeTool)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnFullImageSearch();
	virtual BOOL OnInitDialog();


	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	void UpdateDialogLanguage();
public:
	int m_nDecodeMethod;
	int m_nAppMode;
	int m_nDiscreteFlag;
	int m_nDistortionFlag;
	int m_nMirrorMode;
	int m_nPolarityMode;
	int m_nRectangleFlag;
	int m_nCodeMaxNum;
	int m_nMinBarSize;
	int m_nMaxBarSize;
	int m_nSampleLevel;
	int m_nTimeOut;
	int m_nDebugFlag;
	int m_nAdvanceParam;
	int m_nAdvanceParam2;
	int		m_nPatSpeedMode;
	CComboBox	m_comboSpeed;

	afx_msg void OnBnClickedRadioDecodeMethod();
	afx_msg void OnBnClickedRadioAppMode();
	afx_msg void OnBnClickedRadioDiscreteFlag();
	afx_msg void OnBnClickedRadioDistortionFlag();
	afx_msg void OnBnClickedRadioMirrorMode();
	afx_msg void OnBnClickedRadioPolarityMode();
	afx_msg void OnBnClickedRadioRectangleFlag();
	afx_msg void OnEnSetfocusEditCodemaxnum();
	afx_msg void OnEnKillfocusEditCodemaxnum();
	afx_msg void OnEnSetfocusEditMinbarsize();
	afx_msg void OnEnKillfocusEditMinbarsize();
	afx_msg void OnEnSetfocusEditMaxbarsize();
	afx_msg void OnEnKillfocusEditMaxbarsize();
	afx_msg void OnEnSetfocusEditSamplelevel();
	afx_msg void OnEnKillfocusEditSamplelevel();
	afx_msg void OnEnSetfocusEditWaittime();
	afx_msg void OnEnKillfocusEditWaittime();
	afx_msg void OnEnSetfocusEditDebugFlag();
	afx_msg void OnEnKillfocusEditDebugFlag();
	afx_msg void OnEnSetfocusEditAdvanceParam();
	afx_msg void OnEnKillfocusEditAdvanceParam();
	afx_msg void OnEnSetfocusEditAdvanceParam2();
	afx_msg void OnEnKillfocusEditAdvanceParam2();
	afx_msg void OnSelchangeComboSpeedMode();

	CString m_strInput;
	CString m_strOutput;
	int m_nCodeType;

	int m_nImageChannel;
	afx_msg void OnBnClickedRadio();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCORNERSEARCHTOOLSETTING_H__FD052144_C0F9_4940_AE83_7A8B5CEC816D__INCLUDED_)
