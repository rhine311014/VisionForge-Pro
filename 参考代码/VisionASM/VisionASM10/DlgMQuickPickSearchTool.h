#pragma once
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgMQuickPickSearchTool 对话框
class CDlgSearch;
class CDlgMQuickPickSearchTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgMQuickPickSearchTool)

public:
	CDlgMQuickPickSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMQuickPickSearchTool();
	CDlgSearch *m_pDlgParent;
	scGuiDisplay m_GuiPatternDisplay;
	CStringArray *m_psaSysInfoStrings;
	cpImage	m_imageMask;
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

// 对话框数据
	enum { IDD = IDD_DLG_MQUICKPICKSEARCH_TOOL };

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
	//{{AFX_MSG(CDlgMQuickSearchTool)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	void UpdateDialogLanguage();
public:
	BOOL m_bEnableMark2;
	afx_msg void OnBnClickedChkEnableMark2();
	afx_msg void OnCbnSelchangeComboCoordSortMethod();
	afx_msg void OnEnSetfocusEdtCoordSortGap2();
	int m_nImageChannel;
	afx_msg void OnBnClickedChkShowHideSearch1Rect();
	afx_msg void OnBnClickedChkShowHideSearch2Rect();
	BOOL m_bSearchHideRectVisible1;
	BOOL m_bSearchHideRectVisible2;
};
