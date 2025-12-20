#pragma once


// CDlgObroundMeasureInspectTool 对话框
class CDlgSearch;
class CDlgCircleCornerSearchCheckInspectTool;
class CDlgObroundMeasureInspectTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgObroundMeasureInspectTool)
public:
	CDlgObroundMeasureInspectTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgObroundMeasureInspectTool();

	// 对话框数据
	enum { IDD = IDD_DLG_OBROUND_MEASURE_INSPECT_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	CDlgSearch*									m_pDlgParent;
	CDlgCircleCornerSearchCheckInspectTool*		m_pDlgParent2;
	int m_nObroundCaliperNum;
	int m_nObroundOutLineNum;
	double m_dObroundContrastThre;
	int m_nObroundPolarityMode;
	int m_nObroundInwardMode;
	long m_lObroundFilterHalfWidth;
	BOOL m_bObroundSortByScore;
	CButton m_checkObroundSortByScore;
	BOOL m_bShowObroundFitPoint;

	double	m_dPixelRatio;



// 	BOOL	m_bEnableMaxCircleRadius1;
// 	double	m_dDistanceMaxCircleRadius1;
// 	BOOL	m_bEnableMinCircleRadius1;
// 	double	m_dDistanceMinCircleRadius1;
// 	BOOL m_bEnableMaxCircleRadius2;
// 	double m_dDistanceMaxCircleRadius2;
// 	BOOL m_bEnableMinCircleRadius2;
// 	double m_dDistanceMinCircleRadius2;


// 	BOOL	m_bEnableMax1;
// 	double	m_dDistanceMax1;
// 	BOOL	m_bEnableMin1;
// 	double	m_dDistanceMin1;
// 
// 	BOOL	m_bEnableMax2;
// 	double	m_dDistanceMax2;
// 	BOOL	m_bEnableMin2;
// 	double	m_dDistanceMin2;

	CComboBox	m_comboDefectType;
	int		m_nBinaryThre;
	int  	m_nMinAcceptArea;
	int 	m_nMaxAcceptArea;
	double	m_dMaxDefectArea;

	BOOL	m_bEnableDistCheck;
	double	m_dCurRadius;
	double	m_dCurDistance1;
	double	m_dCurDistance2;
	BOOL	m_bEnableBlobInspect;

	CStringArray*	m_psaSysInfoStrings;

public:
	// 检查对话框上的参数设置的有效性，并将无效参数设置成默认值
	void CheckDlgDataValidation();

	// 更新控件显示
	void UpdateControlDisplay();	
	afx_msg void OnEnSetfocusEdtObroundCalipernum();
	afx_msg void OnEnSetfocusEdtObroundOutlineNum();
	afx_msg void OnEnSetfocusEdtObroundContrastThre();
// 	afx_msg void OnEnSetfocusEdtDistanceMax1();
// 	afx_msg void OnEnSetfocusEdtDistanceMin1();
// 	afx_msg void OnEnSetfocusEdtDistanceMax2();
// 	afx_msg void OnEnSetfocusEdtDistanceMin2();
// 	afx_msg void OnEnSetfocusEdtDistanceMaxCircleRadius1();
// 	afx_msg void OnEnSetfocusEdtDistanceMinCircleRadius1();
	afx_msg void OnEnSetfocusEdtBinaryThre();
	afx_msg void OnEnSetfocusEdtMaxArea();
	afx_msg void OnEnSetfocusEdtMinArea();
	afx_msg void OnEnSetfocusEdtMaxDefectArea();
	virtual BOOL OnInitDialog();
	void UpdateDialogLanguage();
	afx_msg void OnEnChangeEdtObroundCalipernum();
	afx_msg void OnBnClickedRadObroundInwardMode0();
	afx_msg void OnBnClickedCheckShowObroundFitpoint();
	afx_msg void OnBnClickedRadObroundInwardMode1();
	CString m_strSizeLabel1;
	CString m_strDistLabel1;
	CString m_strDistLabel2;
	afx_msg void OnEnSetfocusEdtPixelRatio();
	afx_msg void OnEnSetfocusEditInspectdisLabel2();
	afx_msg void OnEnSetfocusEditInspectdisLabel3();
	afx_msg void OnEnSetfocusEditInspectdisLabel4();
	afx_msg void OnEnSetfocusEditInspectdisLabel1();
	double m_dCurBlobArea;
	BOOL m_bEnableDispLineSeg;
	double m_dAreaScaleRatio;
	afx_msg void OnEnSetfocusEdtAreaScaleRatio();
	BOOL m_bEnableSizeCheck;
	BOOL m_bEnableDispLineSeg2;
	BOOL m_bSizeCheckUseToTool;
	BOOL m_bDistCheckUseToTool;
	BOOL m_bBlobInspectUseToTool;
	
	CString m_strSizeLabel2;
	CString m_strCurSizeResult;
	CString m_strCurDistResult;
	CString m_strCurInspectResult;
	afx_msg void OnEnSetfocusEditInspectdisLabel6();
	double m_dCurRadius2;
	afx_msg void OnEnSetfocusEditCircleRadiusBase1();
	afx_msg void OnEnSetfocusEditCircleRadiusFlu1();
	afx_msg void OnEnSetfocusEditCircleRadiusBase2();
	afx_msg void OnEnSetfocusEditCircleRadiusFlu2();
	afx_msg void OnEnSetfocusEditDistanceBase1();
	afx_msg void OnEnSetfocusEditDistanceFlu1();
	afx_msg void OnEnSetfocusEditDistanceBase2();
	afx_msg void OnEnSetfocusEditDistanceFlu2();
	BOOL m_bEnableCircleRadius1;
	BOOL m_bEnableCircleRadius2;
	double m_dDistanceCircleRadiusBase1;
	double m_dDistanceCircleRadiusBase2;
	double m_dDistanceCircleRadiusFlu1;
	double m_dDistanceCircleRadiusFlu2;
	BOOL m_bEnableDistance1;
	BOOL m_bEnableDistance2;
	double m_dDistanceBase1;
	double m_dDistanceBase2;
	double m_dDistanceFlu1;
	double m_dDistanceFlu2;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
