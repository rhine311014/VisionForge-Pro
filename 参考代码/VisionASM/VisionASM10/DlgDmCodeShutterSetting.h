#pragma once

#include "vsBaseVisionAlign.h"
#include "afxcmn.h"
// CDlgDmCodeShutterSetting 对话框

class CDlgCalibTargetDmcodeParam;
class CDlgDmCodeShutterSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgDmCodeShutterSetting)

public:
	CDlgDmCodeShutterSetting(CWnd* pParent =NULL);   // 标准构造函数
	virtual ~CDlgDmCodeShutterSetting();

// 对话框数据
	enum { IDD = IDD_DLG_DMCODE_SHUTTER_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CStringArray	*m_psaSysInfoStrings;
public:
	void UpdateDialogLanguage();
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	afx_msg void OnBnClickedBtnReset();
	CDlgCalibTargetDmcodeParam* m_pParent;
	CSliderCtrl m_sliderDmcodeShutter;
	double		m_dDmCodeShutter;
	double		m_dEnvironmentShutter;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	double m_dDmCodeShutterOld;
	virtual BOOL OnInitDialog();
};
