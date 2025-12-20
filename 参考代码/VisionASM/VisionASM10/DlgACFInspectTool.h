#pragma once
#include "afxcmn.h"
#include "DlgQuickSearchTool.h"
#include "DlgPatternSearchTool.h"
#include "DlgACFInspectToolUI.h "
#include "DlgPatternCornerSearchToolForCorner.h"
#include "DlgCornerSearchToolForCorner.h"

// CDlgACFInspectTool 对话框
class CDlgSearch;
class CDlgACFInspectTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgACFInspectTool)

public:
	CDlgACFInspectTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgACFInspectTool();

	CDlgSearch		*m_pDlgParent;


// 对话框数据
	enum { IDD = IDD_DLG_ACF_INSPECT_TOOL };

	CXPStyleButtonST m_btnMarkTrain;
	CXPStyleButtonST m_btnMarkGetTrainImage;
	//CXPStyleButtonST m_btnMarkFullImageSearch;
	CStringArray*	m_psaSysInfoStrings;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CThemeHelperST		m_themeHelper;
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_cTabCtl;
	
	int m_TabIndex;
	BOOL m_bEnableLocation;
	CDlgQuickSearchTool         m_DlgQuickSearchTool;
	CDlgPatternSearchTool       m_DlgPatternSearchTool;
	CDlgPatternCornerSearchToolForCorner m_DlgPatternCornerSearchTool;	// 区域角定位
	CDlgCornerSearchToolForCorner		m_DlgCornerSearchTool;			// 几何角定位
	CDlgACFInspectToolUI		 m_DlgGrayBlobInspectTool;

	// 角定位找线工具参数
	int		m_nLineCaliperNum1;			// 卡尺个数
	int		m_nLineCaliperNum2;
	double	m_dLineContrastThre1;		// 对比度
	double	m_dLineContrastThre2;
	long	m_lLineFilterHalfWidth1;	// 滤波半宽
	long	m_lLineFilterHalfWidth2;
	int		m_nLineOutLineNum1;			// 忽略个数
	int		m_nLineOutLineNum2;
	int		m_nLinePolarityMode1;		// 极性
	int		m_nLinePolarityMode2;	
	int     m_nOldLinePolarityMode1;	// 原先极性
	int     m_nOldLinePolarityMode2;

	BOOL	m_bSortByScore1;			// 按分数排序
	BOOL	m_bSortByScore2;

	BOOL	m_bEnableLineMask_1;		// 找线工具掩膜
	BOOL	m_bEnableLineMask_2;
	BOOL	m_bShowFitPoint_1;			// 显示拟合点
	BOOL	m_bShowFitPoint_2;
	BOOL	m_bShowCaliper_1;			// 是否显示卡尺
	BOOL	m_bShowCaliper_2;











	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
public:
	 //更新控件显示
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

	void UpdateSearchResult();//将搜索的结果更新到界面上

	afx_msg void OnCbnSelchangeCmbSearchtoolType();
	int m_nSearchToolTypeIndex;

	void InitDlgItem();
	void UpdateDlgItem();
	void RemoveImage();
	void ClearScreen();
	void SetGuiPatternDisplayImage(BOOL bCopy=TRUE);
	CComboBox m_cmbSearchToolType;
	CComboBox m_cmbInspectType;
	afx_msg void OnBnClickedBtnMarkGettrainimage();
	afx_msg void OnBnClickedBtnMarkTrain();
	afx_msg void OnBnClickedChkEnableBasesearchCheck();
};
