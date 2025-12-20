#pragma once
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"

// CDlgDmCodeCalibrateTool 对话框
class CDlgSearch;
class CDlgDmCodeCalibrateTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgDmCodeCalibrateTool)

public:
	CDlgDmCodeCalibrateTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDmCodeCalibrateTool();

// 对话框数据
	enum { IDD = IDD_DLG_DMCODE_CALIBRATE_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDlgSearch	*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;
	CXPStyleButtonST	m_btnGetTrainImage;
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
protected:
	CThemeHelperST		m_themeHelper;
	void UpdateDialogLanguage();
	void OnDmcodeMethodChange();
private:
	int m_nDmCodeMaxNum;
	int m_nDecodeMaxTime;
	double m_dGridThre;
	double m_dGridSize;
	BOOL m_bRecode;
	BOOL m_bShowOutputPoints;
public:
	
	afx_msg void OnBnClickedBtnGetTrainImage();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusEditValDmcodemaxnum();
	afx_msg void OnEnSetfocusEditValDmcodemaxtime();
	afx_msg void OnEnSetfocusEditValDmcodegridthre();
	afx_msg void OnEnSetfocusEditValDmcodegridsize();
	afx_msg void OnBnClickedBtnDmcodeFullimage();
	afx_msg void OnBnClickedBtnCornersFullimage();
	afx_msg void OnBnClickedBtnTrain();
	int m_nDecodeMethod;
	int m_nDmCodeMirror;
	int m_nSampleLevel;
	int m_nMinBarSize;
	int m_nMaxBarSize;
	afx_msg void OnEnSetfocusEditSampleLevel();
	afx_msg void OnEnSetfocusEditMinBarSize();
	afx_msg void OnEnSetfocusEditMaxBarSize();
	afx_msg void OnBnClickedRdoDmcodeMethodDefault();
	afx_msg void OnBnClickedRdoDecodeMethodVsb();
	int m_nImageChannel;
};
