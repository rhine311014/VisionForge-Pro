
#pragma once

// DlgACFInspectToolUI.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// DlgACFInspectToolUI dialog
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"


class CDlgSearch;
class CDlgACFInspectToolUI : public CDialog
{
public:
	CDlgACFInspectToolUI(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgACFInspectToolUI();
	CDlgSearch		*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;
	enum { IDD = IDD_DLG_ACF_GRAY_BLOB_INSPECT_TOOL };

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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

	void UpdateSearchResult();
public:
	std::vector<double>     m_dMinMean; //最小亮度
	std::vector<double>     m_dMaxMean; //最大亮度

	std::vector<double>     m_dMinStdDev; //最小方差
	std::vector<double>     m_dMaxStdDev; //最大方差

	std::vector<BOOL>       m_bMeanInvalid;  //是否使用亮度检测
	std::vector<BOOL>       m_bStdDevInvalid;  //是否使用方差检测

	std::vector<BOOL>       m_bMeanInvert;     //亮度检测结果取反
	std::vector<BOOL>       m_bStdDevInvert;   //方差检测结果取反
	
protected:


	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	

	void UpdateDialogLanguage();
public:	
	CComboBox m_SelAreaIndex;
	int          m_nCurToolIndex;

	int m_nInsToolNum;
	double m_dCurMean;
	double m_dCurStdDev;
	double m_dCurMinMean;
	double m_dCurMaxMean;
	BOOL m_bMeanNotInvalid;
	BOOL m_bCurMeanInvert;
	double m_dCurMinStdDev;
	double m_dCurMaxStdDev;
	BOOL m_bCurStdDevNotInvalid;
	BOOL m_bCurStdDevInvert;

	afx_msg void OnCbnSelchangeComboAreaIndex();
	afx_msg void OnEnSetfocusEdtInstoolNum();
	afx_msg void OnEnSetfocusEdtMinMean();
	afx_msg void OnEnSetfocusEdtMaxMean();
	afx_msg void OnEnSetfocusEdtMinStddev();
	afx_msg void OnEnSetfocusEdtMaxStddev();
	afx_msg void OnEnKillfocusEdtInstoolNum();
	afx_msg void OnEnChangeEdtInstoolNum();

	afx_msg void OnBtnSetCurToAll();
	int m_nColNum;
	int m_nRowNum;
	
	afx_msg void OnEnSetfocusEdtColNum();
	afx_msg void OnEnSetfocusEdtRowNum();


	BOOL m_bCurEnableGrayInspect; // 当前灰度检测是否启用
	BOOL m_bCurEnableBlobInspect; // 当前Blob检测是否启用

	std::vector<BOOL>     m_vbCurEnableGrayInspect;  //是否使用亮度检测
	std::vector<BOOL>		m_vbCurEnableBlobInspect; // 当前Blob检测是否启用

	std::vector<BOOL>     m_vnColNum;  //等分列数目
	std::vector<BOOL>		m_vnRowNum; // 等分行数目
	std::vector<BOOL>		m_vnAllowNGNum; // 允许等分NG数目

	std::vector<int> m_vnDefectType;
	std::vector<int> m_vnBinaryThre;
	std::vector<int> m_vnMinAcceptArea;
	std::vector<int> m_vnMaxAcceptArea;
	std::vector<double> m_vdMaxDefectArea;

	CComboBox	m_comboDefectType;
	int		m_nDefectType;
	int	m_nBinaryThre;
	int  	m_nMinAcceptArea;
	int 	m_nMaxAcceptArea;
	double	m_dMaxDefectArea;

	double	m_dDefectArea;
	afx_msg void OnSelchangeComboDefectType();
	afx_msg void OnEnSetfocusEdtBinaryThre();
	afx_msg void OnEnSetfocusEdtMinArea();
	afx_msg void OnEnSetfocusEdtMaxArea();
	afx_msg void OnEnSetfocusEdtMaxDefectArea();
	
	int m_nAllowedNG;
	afx_msg void OnEnSetfocusEdtColNum2();
	double m_dmaxGrayValue;
};


