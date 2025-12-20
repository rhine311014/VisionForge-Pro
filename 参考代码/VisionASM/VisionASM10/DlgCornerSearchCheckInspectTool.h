#pragma once
#include "afxcmn.h"
#include "DlgPatternCornerSearchToolForCorner.h"
#include "DlgQuickSearchTool.h"
#include "DlgCornerSearchToolForCorner.h"
#include "DlgPatternSearchTool.h"
#include "DlgLineToLineDistanceTool.h"
#include "DlgCircleToLineDistanceTool.h"

#include "afxwin.h"


// CDlgCornerSearchCheckInspectTool 对话框
class CDlgSearch;
class CDlgCornerSearchCheckInspectTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgCornerSearchCheckInspectTool)

public:
	CDlgCornerSearchCheckInspectTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCornerSearchCheckInspectTool();

	CDlgSearch		*m_pDlgParent;


// 对话框数据
	enum { IDD = IDD_DLG_CORNERSEARCH_CHECK_INSPECT_TOOL };

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
	BOOL m_bEnableCorner;

	CDlgPatternCornerSearchToolForCorner m_DlgPatternCornerSearchTool;
	CDlgCornerSearchToolForCorner m_DlgCornerSearchTool;
	CDlgQuickSearchTool         m_DlgQuickSearchTool;
	CDlgPatternSearchTool       m_DlgPatternSearchTool;
	CDlgLineToLineDistanceTool	m_DlgLineToLineDistanceTool1;
	CDlgLineToLineDistanceTool	m_DlgLineToLineDistanceTool2;
	CDlgCircleToLineDistanceTool	m_DlgCircleToLineDistanceTool;

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
	afx_msg void OnCbnSelchangeCmbSearchtoolType();
	int m_nSearchToolTypeIndex;
	afx_msg void OnCbnSelchangeCmbInspectType();
	int m_nInspectTypeIndex;

	void InitDlgItem();
	void UpdateDlgItem();
	void RemoveImage();
	void ClearScreen();
	void SetGuiPatternDisplayImage(BOOL bCopy=TRUE);
	CComboBox m_cmbSearchToolType;
	CComboBox m_cmbInspectType;
	afx_msg void OnBnClickedBtnMarkGettrainimage();
	afx_msg void OnBnClickedBtnMarkTrain();
	BOOL m_bUseInspectTool;
	afx_msg void OnBnClickedChkUseinspect();
	BOOL m_bInspectAfterCorner;
//	afx_msg void OnBnSetfocusCheckEnableCorner();
	afx_msg void OnBnClickedCheckEnableCorner();
};
