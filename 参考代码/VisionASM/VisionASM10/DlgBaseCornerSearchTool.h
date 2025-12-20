#pragma once

#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgBaseCornerSearchTool 对话框

class CDlgSearch;
class CDlgBaseCornerSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBaseCornerSearchTool)

public:
	CDlgBaseCornerSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBaseCornerSearchTool();

	CDlgSearch		*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;

// 对话框数据
	enum { IDD = IDD_DLG_BASECORNERSEARCH_TOOL };

	CXPStyleButtonST	m_btnTrain;
	CXPStyleButtonST	m_btnGetTrainImage;


	int		m_nLineCaliperNum1;
	int		m_nLineCaliperNum2;
	double	m_dLineContrastThre1;
	double	m_dLineContrastThre2;
	long	m_lLineFilterHalfWidth1;
	long	m_lLineFilterHalfWidth2;
	int		m_nLineOutLineNum1;
	int		m_nLineOutLineNum2;
	int		m_nLinePolarityMode1;
	int		m_nLinePolarityMode2;


public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	virtual BOOL OnInitDialog();

	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();

	afx_msg void OnChangeEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineContrastThre2();
	afx_msg void OnSetfocusEdtLineOutlineNum2();

	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;

	void UpdateDialogLanguage();
public:
	BOOL m_bSortByScore1;
	BOOL m_bSortByScore2;
	CButton m_checkSortByScore1;
	CButton m_checkSortByScore2;
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_2;
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper2();
	BOOL m_bEnableLineMask_1;
	BOOL m_bEnableLineMask_2;
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckLineMask2();
	BOOL m_bEnableDualEdge;
	BOOL m_bEnableDualEdge2;
	int m_nImageChannel;
};
