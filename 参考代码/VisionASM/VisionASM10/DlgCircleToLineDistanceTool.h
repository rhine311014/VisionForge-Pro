#pragma once


// CDlgCircleToLineDistanceTool 对话框
class CDlgSearch;
class CDlgCornerSearchCheckInspectTool;
class CDlgCircleToLineDistanceTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCircleToLineDistanceTool)

public:
	CDlgCircleToLineDistanceTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCircleToLineDistanceTool();

// 对话框数据
	enum { IDD = IDD_DLG_CIRCLE_TO_LINE_DISTANCE_INSPECT };

	CDlgSearch		*m_pDlgParent;
	CDlgCornerSearchCheckInspectTool		*m_pDlgParent2;
	CStringArray*	m_psaSysInfoStrings;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 更新控件显示
	void UpdateControlDisplay();	
	void UpdateDialogLanguage();
	// 检查对话框上的参数设置的有效性，并将无效参数设置成默认值
	void CheckDlgDataValidation();

	afx_msg void OnBnClickedCheckEnableDistanceInspect();
	afx_msg void OnEnChangeEdtLineCalipernum1();
	afx_msg void OnBnClickedChkShowLineCaliper1();
	afx_msg void OnBnClickedCheckShowFitpoint1();
	afx_msg void OnBnClickedCheckLineMask1();
	afx_msg void OnEnChangeEdtCircleCalipernum();
	afx_msg void OnBnClickedChkShowCircleCaliper();
	afx_msg void OnBnClickedCheckShowFitCirclePoint();
	afx_msg void OnBnClickedCheckCircleMask();
	afx_msg void OnBnClickedRadCircleInwardMode0();
	afx_msg void OnBnClickedRadCircleInwardMode1();
public:
	int m_nLineOutLineNum1;			//忽略数
	int m_nLinePolarityMode1;			//极性
	double m_dLineContrastThre1;		//对比度
	long m_lLineFilterHalfWidth1;		//滤波半宽
	BOOL m_bSortByScore1;				//按分数排序
	int m_nLineCaliperNum1;			//卡尺数
	BOOL m_bEnableLineMask_1;			//启用掩摸
	BOOL m_bShowFitPoint1;			//显示拟合点
	BOOL m_bEnableDualEdge;			//双极性
	BOOL m_bShowCaliper_1;			//显示卡尺
	BOOL m_bEnableLineDistanceTool;	//启用距离检测	
	double m_dPixelRatio;				//像素当量
	BOOL m_bEnableMax1;				//最大值管控
	double m_dDistanceMax1;			//最大值
	BOOL m_bEnableMin1;				//最小值管控
	double m_dDistanceMin1;			//最小值

	CString m_strInspectInfo;			//测量结果
	double m_dInspectDistanceValue;	//测量距离
	CString m_strDistanceLabel;		//测量结果标签
	BOOL m_bEanbleOKNG_Decision;		//测量结果失败用于定位失败

	int m_nCircleCaliperNum;			//卡尺数
	int m_nCircleOutLineNum;			//忽略数
	double m_dCircleContrastThre;		//对比度
	long m_lCircleFilterHalfWidth;		//滤波半款
	int m_nCirclePolarityMode;		//极性
	BOOL m_bEnableCircleDualEdge;		//启用双极性
	int m_nCircleInwardMode;			//向心
	BOOL m_bEnableCircleMask;			//启用掩摸
	BOOL m_bSortByScore2;				//按分数排序
	BOOL m_bEnableShowFitCirclePoint;	//显示拟合点
	CButton m_checkShowFitCirclePoint;//显示拟合点
	BOOL m_bShowCircleCaliper;		//显示卡尺
	
	afx_msg void OnEnSetfocusEdtLineCalipernum1();
	afx_msg void OnEnSetfocusEdtLineOutlineNum1();
	afx_msg void OnEnSetfocusEdtLineContrastThre1();
	afx_msg void OnEnSetfocusEdtLineFilterHalfwidth1();
	afx_msg void OnEnSetfocusEdtCircleCalipernum();
	afx_msg void OnEnSetfocusEdtCircleOutlineNum1();
	afx_msg void OnEnSetfocusEdtCircleContrastThre();
	afx_msg void OnEnSetfocusEdtCircleFilterHalfwidth1();
	afx_msg void OnEnSetfocusEdtPixelRatio();
	afx_msg void OnEnSetfocusEdtDistanceMax1();
	afx_msg void OnEnSetfocusEditInspectdisLabel();
	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedCheckEnableDistanceInspect1();
	// //显示圆
	BOOL m_bEnableCircleDistanceTool;
};
