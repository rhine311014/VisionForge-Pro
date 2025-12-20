#pragma once
#include "afxwin.h"


// CDlgCircleDoubleMeasureInspectTool 对话框
class CDlgSearch;
class CDlgCircleCornerSearchCheckInspectTool;
class CDlgCircleDoubleMeasureInspectTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCircleDoubleMeasureInspectTool)
public:
	CDlgCircleDoubleMeasureInspectTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCircleDoubleMeasureInspectTool();

// 对话框数据
	enum { IDD = IDD_DLG_CIRCLE_DOUBLE_MEASURE_INSPECT_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	CDlgSearch*									m_pDlgParent;
	CDlgCircleCornerSearchCheckInspectTool*		m_pDlgParent2;
	int m_nCircleCaliperNum;
	int m_nCircleOutLineNum;
	double m_dCircleContrastThre;
	int m_nCirclePolarityMode;
	int m_nCircleInwardMode;
	long m_lCircleFilterHalfWidth;
	BOOL m_bCircleSortByScore;
	CButton m_checkCircleSortByScore;
	BOOL m_bShowCircleFitPoint;

	int m_nCircleCaliperNumTmp[2];
	int m_nCircleOutLineNumTmp[2];
	double m_dCircleContrastThreTmp[2];
	int m_nCirclePolarityModeTmp[2];
	int m_nCircleInwardModeTmp[2];
	long m_lCircleFilterHalfWidthTmp[2];
	BOOL m_bCircleSortByScoreTmp[2];
	CButton m_checkCircleSortByScoreTmp[2];
	BOOL m_bShowCircleFitPointTmp[2];


	
 	CString  m_strDistLabel1Tmp[3];
// 	BOOL m_bEnableMax1Tmp[3];
// 	double m_dDistanceMax1Tmp[3];
// 	BOOL m_bEnableMin1Tmp[3];
// 	double m_dDistanceMin1Tmp[3];

	BOOL m_bEnableDistance1Tmp[3];
	double m_dDistanceBase1Tmp[3];
	double m_dDistanceFlu1Tmp[3];

	CString  m_strDistLabel2Tmp[3];
// 	BOOL m_bEnableMax2Tmp[3];
// 	double m_dDistanceMax2Tmp[3];
// 	BOOL m_bEnableMin2Tmp[3];
// 	double m_dDistanceMin2Tmp[3];

	BOOL m_bEnableDistance2Tmp[3];
	double m_dDistanceBase2Tmp[3];
	double m_dDistanceFlu2Tmp[3];


	int      m_nPreCicleIndex;
	int      m_nPreCicleIndex2;
	double	m_dPixelRatio;
	
	BOOL m_bEnableSizeCheck;
	BOOL m_bEnableDispLineSeg2;
	BOOL m_bBlobInspectUseToTool;
	BOOL m_bDistCheckUseToTool;
	BOOL m_bSizeCheckUseToTool;

	CString m_strSizeLabel2;
	CString m_strSizeLabel3;

// 	BOOL	m_bEnableMaxCircleRadius1;
// 	double	m_dDistanceMaxCircleRadius1;
// 	BOOL	m_bEnableMinCircleRadius1;
// 	double	m_dDistanceMinCircleRadius1;

	
// 	BOOL m_bEnableMaxCircleRadius2;
// 	double m_dDistanceMaxCircleRadius2;
// 	BOOL m_bEnableMinCircleRadius2;
// 	double m_dDistanceMinCircleRadius2;
	
// 	BOOL m_bEnableMaxCircleRadius3;
// 	double m_dDistanceMaxCircleRadius3;
// 	BOOL m_bEnableMinCircleRadius3;
// 	double m_dDistanceMinCircleRadius3;

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

	void UpdateCircleTrainData(bool bValidate);

	void UpdateCircleSearchData(bool bValidate);

	void UpdateCircleMeasureData();
	afx_msg void OnEnSetfocusEdtCircleCalipernum();
	afx_msg void OnEnSetfocusEdtCircleOutlineNum();
	afx_msg void OnEnSetfocusEdtCircleContrastThre();	
// 	afx_msg void OnEnSetfocusEdtDistanceMaxCircleRadius1();
// 	afx_msg void OnEnSetfocusEdtDistanceMinCircleRadius1();
// 	afx_msg void OnEnSetfocusEdtDistanceMaxCircleRadius2();
// 	afx_msg void OnEnSetfocusEdtDistanceMinCircleRadius2();
// 	afx_msg void OnEnSetfocusEdtDistanceMaxCircleRadius3();
// 	afx_msg void OnEnSetfocusEdtDistanceMinCircleRadius3();
// 	afx_msg void OnEnSetfocusEdtDistanceMax1();
// 	afx_msg void OnEnSetfocusEdtDistanceMin1();
// 	afx_msg void OnEnSetfocusEdtDistanceMax2();
// 	afx_msg void OnEnSetfocusEdtDistanceMin2();
	afx_msg void OnEnSetfocusEdtBinaryThre();
	afx_msg void OnEnSetfocusEdtMaxArea();
	afx_msg void OnEnSetfocusEdtMinArea();
	afx_msg void OnEnSetfocusEdtMaxDefectArea();
	virtual BOOL OnInitDialog();
	void UpdateDialogLanguage();
	afx_msg void OnEnChangeEdtCircleCalipernum();
	afx_msg void OnBnClickedRadCircleInwardMode0();
	afx_msg void OnBnClickedCheckShowCircleFitpoint();
	afx_msg void OnBnClickedRadCircleInwardMode1();
	CString m_strSizeLabel1;
	CString m_strDistLabel1;
	CString m_strDistLabel2;
	afx_msg void OnEnSetfocusEdtPixelRatio();
	afx_msg void OnEnSetfocusEditInspectdisLabel2();
	afx_msg void OnEnSetfocusEditInspectdisLabel3();
	afx_msg void OnEnSetfocusEditInspectdisLabel4();
	afx_msg void OnEnSetfocusEditInspectdisLabel1();
	double m_dCurBlobArea;
	afx_msg void OnBnClickedRadioCircle1();
	BOOL m_nCircleIndex;
	//int m_nCircleMeasureIndex;
	afx_msg void OnBnClickedRadioCircle2();
	afx_msg void OnBnClickedRadioCircleMeasure1();
	afx_msg void OnBnClickedRadioCircleMeasure2();
	BOOL m_bEnableDispLineSeg;
	double m_dAreaScaleRatio;
	afx_msg void OnEnSetfocusEdtAreaScaleRatio();
	
	CString m_strCurSizeResult;
	CString m_strCurDistResult;
	CString m_strCurInspectResult;
	double m_dCurRadius2;
	double m_dCurRadius3;
	afx_msg void OnEnSetfocusEditInspectdisLabel5();
	
	int m_nDistanceType;
	CComboBox m_comboDistanceType;
	afx_msg void OnCbnSelchangeComboDistancetype();
	CComboBox m_CComboCircleDistance;
	int m_nCircleIndexDistance;
	afx_msg void OnCbnSelchangeComboCircleControlOption();
	double m_dDistanceCircleRadiusBase1;
	double m_dDistanceCircleRadiusBase2;
	double m_dDistanceCircleRadiusBase3;
	double m_dDistanceCircleRadiusFlu1;
	double m_dDistanceCircleRadiusFlu2;
	double m_dDistanceCircleRadiusFlu3;
	double m_dDistanceBase1;
	double m_dDistanceBase2;
	double m_dDistanceFlu1;
	double m_dDistanceFlu2;
	BOOL m_bEnableCircleRadius1;
	BOOL m_bEnableCircleRadius2;
	BOOL m_bEnableCircleRadius3;
	BOOL m_bEnableDistance1;
	BOOL m_bEnableDistance2;
	afx_msg void OnEnSetfocusEditCircleRadiusBase1();
	afx_msg void OnEnSetfocusEditCircleRadiusFlu1();
	afx_msg void OnEnSetfocusEditCircleRadiusBase2();
	afx_msg void OnEnSetfocusEditCircleRadiusFlu2();
	afx_msg void OnEnSetfocusEditCircleRadiusBase3();
	afx_msg void OnEnSetfocusEditCircleRadiusFlu3();
	afx_msg void OnEnSetfocusEditDistanceBase1();
	afx_msg void OnEnSetfocusEditDistanceFlu1();
	afx_msg void OnEnSetfocusEditDistanceBase2();
	afx_msg void OnEnSetfocusEditDistanceFlu2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
