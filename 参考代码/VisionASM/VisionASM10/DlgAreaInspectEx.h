
#pragma once

// DlgAreaInspectEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgAreaInspectEx dialog
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"


class CDlgSearch;
class CDlgAreaInspectEx : public CDialog
{

	// Construction
public:
	CDlgAreaInspectEx(CWnd* pParent = NULL);   // standard constructor

	CDlgSearch		*m_pDlgParent;
//	scGuiDisplay	m_GuiPatternDisplay;
//	scImageGray		m_imageMask;
	CStringArray*	m_psaSysInfoStrings;

	// Dialog Data
	//{{AFX_DATA(CDlgAreaInspectEx)
	enum { IDD = IDD_DLG_AREAINSPECT_TOOLEX };
	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnGetTrainImage;
	//}}AFX_DATA
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAreaInspectEx)
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
public:
	std::vector<double>     m_dMinMean; //最小亮度
	std::vector<double>     m_dMaxMean; //最大亮度

	std::vector<double>     m_dMinStdDev; //最小方差
	std::vector<double>     m_dMaxStdDev; //最大方差

	std::vector<BOOL>       m_bMeanInvalid;  //是否使用亮度检测
	std::vector<BOOL>       m_bStdDevInvalid;  //是否使用方差检测

	std::vector<BOOL>       m_bMeanInvert;     //亮度检测结果取反
	std::vector<BOOL>       m_bStdDevInvert;   //方差检测结果取反
	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAreaInspectEx)
	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	virtual BOOL OnInitDialog();
 	afx_msg void OnBtnFullImageSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	//int		m_nDefectType;
	int          m_nCurToolIndex;

	void UpdateDialogLanguage();
public:	
	CComboBox m_SelAreaIndex;
	int m_nInsToolNum;
	double m_dCurMean;
	double m_dCurStdDev;
	double m_dCurMinMean;
	double m_dCurMaxMean;
	BOOL m_bMeanNotInvalid;
	BOOL m_bCurMeanInvert;
	double m_dCurMinStdDev;
	double m_dCurMaxStdDev;
	BOOL m_bCurStdDevNotInvalid;
	BOOL m_bCurStdDevInvert;
	int m_nCurLogicIndex;
	afx_msg void OnCbnSelchangeComboAreaIndex();
	afx_msg void OnEnSetfocusEdtInstoolNum();
	afx_msg void OnEnSetfocusEdtMinMean();
	afx_msg void OnEnSetfocusEdtMaxMean();
	afx_msg void OnEnSetfocusEdtMinStddev();
	afx_msg void OnEnSetfocusEdtMaxStddev();
    afx_msg void OnEnKillfocusEdtInstoolNum();
    afx_msg void OnEnChangeEdtInstoolNum();
	int m_nImageChannel;

	afx_msg void OnBtnSetCurToAll();
	int m_nColNum;
	int m_nRowNum;
	afx_msg void OnEnSetfocusEdtColNum();
	afx_msg void OnEnSetfocusEdtRowNum();
	afx_msg void OnBtnAutoArrange();
};


