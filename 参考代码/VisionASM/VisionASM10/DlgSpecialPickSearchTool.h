#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DlgQuickSearchTool.h"
#include "DlgPatternSearchTool.h"
#include "DlgFind4LineParamSet.h"
#include "DlgBlobSearchParamSet.h"
#include "DlgHistogramForSpecicalPick.h"
// CDlgSpecialPickSearchTool 对话框
class CDlgSearch;
class CDlgSpecialPickSearchTool : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSpecialPickSearchTool)

public:
	CDlgSpecialPickSearchTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSpecialPickSearchTool();

// 对话框数据
	enum { IDD = IDD_DLG_SPECIAL_PICK_SEARCH_TOOL };


	CDlgSearch		*m_pDlgParent;

	CXPStyleButtonST m_btnMarkTrain;
	CXPStyleButtonST m_btnMarkGetTrainImage;
	CStringArray*	m_psaSysInfoStrings;
	CThemeHelperST		m_themeHelper;
	// tabCtl
	CTabCtrl m_cTabCtl;
	int m_TabIndex;

	CDlgQuickSearchTool         m_DlgQuickSearchTool_Rough; // 粗定位：几何DLg
	CDlgPatternSearchTool       m_DlgPatternSearchTool_Rough;// 粗定位：区域DLg
	CDlgBlobSearchParamSet		m_DlgBlobSearchParamSet_Rough;

	CDlgFind4LineParamSet		m_DlgFind4LineParamSet;		// 精定位: 矩形 或多标记点
	CDlgBlobSearchParamSet		m_DlgBlobSearchParamSet;


	CDlgQuickSearchTool         m_DlgQuickSearchTool_HeadTail; // 头尾检查：几何DLg
	CDlgPatternSearchTool       m_DlgPatternSearchTool_HeadTail;// 头尾检查：区域DLg
	DlgHistogramForSpecicalPick m_DlgHistogramForSpecicalPick; // 头尾检查：灰度直方图DLg

	// 精定位 固定点、区域、等等 后续添加

	// 正反、混料后续添加


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	virtual BOOL OnInitDialog();

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


	afx_msg void OnCbnSelchangeCmbRouchSearchtoolType();
	int m_nRoughSearchToolTypeIndex;
	afx_msg void OnCbnSelchangeCmbAccurateSearchToolType();
	int m_nAccurateSearchToolTypeIndex;
	afx_msg void OnCbnSelchangeCmbHeadTailCheckToolType();
	int m_nHeadTailCheckToolTypeIndex;

	void InitDlgItem();
	void UpdateDlgItem();
	void RemoveImage();
	void ClearScreen();
	void SetGuiPatternDisplayImage(BOOL bCopy=TRUE);

	CComboBox m_cmbRouchSearchtoolType;

	BOOL m_bEnableAccurateLoation; // 启用精定位
	BOOL m_bEnableHeadTailCheck;  //启用头尾检查

	CComboBox m_cmbAccurateSearchType;

	CComboBox m_cmbHeadTailCheckToolType;

	BOOL	m_bCoordSortEnable;
	double  m_dCoordSortGap;
	CComboBox m_comboCoordSortMethod;
	int		m_nCoordSortMethod;
	int		m_nCoordSortOrderX;
	int		m_nCoordSortOrderY;



	afx_msg void OnBnClickedBtnMarkGettrainimage();
	afx_msg void OnBnClickedBtnMarkTrain();
	
	afx_msg void OnBnClickedChkEnableAccurateLocation();
	afx_msg void OnBnClickedChkEnableHeadTailCheck();

	afx_msg void OnCbnSelchangeComboCoordSortMethod();
	afx_msg void OnEnSetfocusEdtCoordSortGap2();
	int m_nHeadTailFailTpye;

	CComboBox m_comboHeadTailFailTpe;
	//int m_nHeadTailFailTpye;

	void UpdateSearchResult();  
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nImageChannel;
};
