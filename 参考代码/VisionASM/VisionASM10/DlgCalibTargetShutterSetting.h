#pragma once

#include "vsBaseVisionAlign.h"
#include "afxcmn.h"
// CDlgCalibTargetShutterSetting 对话框


class CDlgCalibTargetShutterSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalibTargetShutterSetting)

public:
	CDlgCalibTargetShutterSetting(CWnd* pParent =NULL);   // 标准构造函数
	virtual ~CDlgCalibTargetShutterSetting();

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
	
	CSliderCtrl m_sliderDmcodeShutter;
	double		m_dDmCodeShutter;
	double		m_dEnvironmentShutter;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	double m_dDmCodeShutterOld;
	virtual BOOL OnInitDialog();

	vcBaseVisionAlign *m_pVisionASM;
	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);

	int m_nPosIndex;
};
