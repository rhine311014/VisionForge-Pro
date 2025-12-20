#pragma once


// CDlgLineToLineDistanceTool 对话框
class CDlgSearch;
class CDlgCornerSearchCheckInspectTool;
class CDlgLineToLineDistanceTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLineToLineDistanceTool)

public:
	CDlgLineToLineDistanceTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLineToLineDistanceTool();

// 对话框数据
	enum { IDD = IDD_DLG_LINE_TO_LINE_DISTANCE_INSPECT };

	CDlgSearch		*m_pDlgParent;
	CDlgCornerSearchCheckInspectTool		*m_pDlgParent2;
	CStringArray*	m_psaSysInfoStrings;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void UpdateDialogLanguage();
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
	int m_nLineOutLineNum1;
	int m_nLinePolarityMode1;
	double m_dLineContrastThre1;
	long m_lLineFilterHalfWidth1;
	BOOL m_bSortByScore1;
	int m_nLineCaliperNum1;
	BOOL m_bEnableLineMask_1;

	int m_nLineOutLineNum2;
	double m_dLineContrastThre2;
	long m_lLineFilterHalfWidth2;
	int m_nLinePolarityMode2;
	BOOL m_bSortByScore2;
	BOOL m_bEnableLineMask_2;
	int m_nLineCaliperNum2;
	BOOL m_bShowFitPoint1;
	BOOL m_bShowFitPoint2;
	double m_dPixelRatio;
	BOOL m_bEnableMax1;
	double m_dDistanceMax1;
	BOOL m_bEnableMin1;
	double m_dDistanceMin1;
	int m_nDistanceMode;
	int m_nDistanceType;
	BOOL m_bEnableLineDistanceTool;
	double m_dInspectDistanceValue;
	afx_msg void OnBnClickedCheckEnableDistanceInspect();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedCheckShowFitpoint2();
	CString m_strInspectInfo;
	BOOL m_bEanbleOKNG_Decision;
	afx_msg void OnBnClickedBtnGetTrainImage();
	afx_msg void OnEnSetfocusEdtLineCalipernum1();
	afx_msg void OnEnSetfocusEdtLineOutlineNum1();
	afx_msg void OnEnSetfocusEdtLineContrastThre1();
	afx_msg void OnEnSetfocusEdtLineHalfWidthValue();
	afx_msg void OnEnSetfocusEdtLineCalipernum2();
	afx_msg void OnEnSetfocusEdtLineOutlineNum2();
	afx_msg void OnEnSetfocusEdtLineContrastThre2();
	afx_msg void OnEnSetfocusEdtLineHalfWidthValue2();
	afx_msg void OnEnSetfocusEdtPixelRatio();
	afx_msg void OnEnSetfocusEdtDistanceMax1();
	afx_msg void OnEnSetfocusEdtDistanceMin1();
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnBnClickedCheckLineMask2();
	BOOL m_bEnableDualEdge;
	BOOL m_bEnableDualEdge2;
	CString m_strDistanceLabel;
	afx_msg void OnEnSetfocusEditInspectdisLabel();
	BOOL m_bShowCaliper_1;
	BOOL m_bShowCaliper_2;
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedChkShowLineCaliper2();
	afx_msg void OnEnChangeEdtLineCalipernum1();
	afx_msg void OnEnChangeEdtLineCalipernum2();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth1();
	afx_msg void OnEnSetfocusEditLineFilterHalfWidth2();
};
