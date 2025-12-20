#pragma once
#include "afxcmn.h"
#include "DlgPatternCornerSearchToolForCorner.h"
#include "DlgCornerSearchToolForCorner.h"
#include "DlgCircleMeasureInspectTool.h"
#include "DlgObroundMeasureInspectTool.h"
#include "DlgCircleDoubleMeasureInspectTool.h"


#include "afxwin.h"


// CDlgCircleCornerSearchCheckInspectTool 对话框
class CDlgSearch;
class CDlgCircleCornerSearchCheckInspectTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgCircleCornerSearchCheckInspectTool)

public:
	CDlgCircleCornerSearchCheckInspectTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCircleCornerSearchCheckInspectTool();

	CDlgSearch		*m_pDlgParent;


// 对话框数据
	enum { IDD = IDD_DLG_CIRCLECORNERSEARCH_CHECK_INSPECT_TOOL };

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

	CDlgPatternCornerSearchToolForCorner	m_DlgPatternCornerSearchTool;
	CDlgCornerSearchToolForCorner			m_DlgCornerSearchTool;
	CDlgCircleMeasureInspectTool			m_DlgCircleMeasureInspectTool;
	CDlgCircleDoubleMeasureInspectTool      m_DlgCircleDoubleMeasureInspectTool;
	CDlgObroundMeasureInspectTool			m_DlgObroundMeasureInspectTool;

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
	afx_msg void OnCbnSelchangeCmbInspectType();
	int m_nCircleTypeIndex;

	void InitDlgItem();
	void UpdateDlgItem();
	void RemoveImage();
	void ClearScreen();
	void SetGuiPatternDisplayImage(BOOL bCopy=TRUE);
	CComboBox m_cmbSearchToolType;
	CComboBox m_cmbInspectType;
	afx_msg void OnBnClickedBtnMarkGettrainimage();
	afx_msg void OnBnClickedBtnMarkTrain();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
