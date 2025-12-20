#pragma once


// CDlgFind4LineParamSet 对话框
class CDlgSearch;

class CDlgFind4LineParamSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFind4LineParamSet)

public:
	CDlgFind4LineParamSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFind4LineParamSet();

	CDlgSearch		*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;


// 对话框数据
	enum { IDD = IDD_DLG_RECT_FOR_SPECIAL_PICK };


	//找线参数
	int		m_nLineCaliperNum1;
	int		m_nLineCaliperNum2;
	int		m_nLineCaliperNum3;
	int		m_nLineCaliperNum4;
	double	m_dLineContrastThre1;
	double	m_dLineContrastThre2;
	double	m_dLineContrastThre3;
	double	m_dLineContrastThre4;
	long	m_lLineFilterHalfWidth1;
	long	m_lLineFilterHalfWidth2;
	long	m_lLineFilterHalfWidth3;
	long	m_lLineFilterHalfWidth4;
	int		m_nLineOutLineNum1;
	int		m_nLineOutLineNum2;
	int		m_nLineOutLineNum3;
	int		m_nLineOutLineNum4;
	int		m_nLinePolarityMode1;
	int		m_nLinePolarityMode2;
	int		m_nLinePolarityMode3;
	int		m_nLinePolarityMode4;


	BOOL m_bSortByScore1;
	BOOL m_bSortByScore2;
	BOOL m_bSortByScore3;
	BOOL m_bSortByScore4;
	CButton m_checkSortByScore1;
	CButton m_checkSortByScore2;
	CButton m_checkSortByScore3;
	CButton m_checkSortByScore4;

	BOOL m_bEnableLineMask_1;
	BOOL m_bShowFitPoint_1;
	BOOL m_bEnableLineMask_2;
	BOOL m_bShowFitPoint_2;
	BOOL m_bEnableLineMask_3;
	BOOL m_bShowFitPoint_3;
	BOOL m_bEnableLineMask_4;
	BOOL m_bShowFitPoint_4;
//	int m_nResultOutMode;
	CComboBox m_comboOuput1;
	CComboBox m_comboOuput2;
	int m_nResultOutMode1;
	int m_nResultOutMode2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnChangeEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineCalipernum1();
	afx_msg void OnSetfocusEdtLineContrastThre1();
	afx_msg void OnSetfocusEdtLineOutlineNum1();

	afx_msg void OnChangeEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineCalipernum2();
	afx_msg void OnSetfocusEdtLineContrastThre2();
	afx_msg void OnSetfocusEdtLineOutlineNum2();

	afx_msg void OnChangeEdtLineCalipernum3();
	afx_msg void OnSetfocusEdtLineCalipernum3();
	afx_msg void OnSetfocusEdtLineContrastThre3();
	afx_msg void OnSetfocusEdtLineOutlineNum3();

	afx_msg void OnChangeEdtLineCalipernum4();
	afx_msg void OnSetfocusEdtLineCalipernum4();
	afx_msg void OnSetfocusEdtLineContrastThre4();
	afx_msg void OnSetfocusEdtLineOutlineNum4();

	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedCheckLineMask2();
	afx_msg void OnBnClickedCheckShowFitpoint2();
	afx_msg void OnBnClickedCheckLineMask3();
	afx_msg void OnBnClickedCheckShowFitpoint3();
	afx_msg void OnBnClickedCheckLineMask4();
	afx_msg void OnBnClickedCheckShowFitpoint4();

	DECLARE_MESSAGE_MAP()

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
	void UpdateDialogLanguage();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_2;
	BOOL m_bShowCaliper_3;
	BOOL m_bShowCaliper_4;
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper2();
	afx_msg void OnBnClickedChkShowLineCaliper3();
	afx_msg void OnBnClickedChkShowLineCaliper4();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth3();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth4();
	BOOL m_bEnableDualEdge_1;
	BOOL m_bEnableDualEdge_2;
	BOOL m_bEnableDualEdge_3;
	BOOL m_bEnableDualEdge_4;
};
