#pragma once


// CDlgProductShutter 对话框

class CDlgSearch;


class CDlgProductShutter : public CDialog
{
	DECLARE_DYNAMIC(CDlgProductShutter)

public:
	CDlgProductShutter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProductShutter();

// 对话框数据
	enum { IDD = IDD_DLG_PRODUCT_SHUTTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CSliderCtrl m_sliderProductShutter;

	CSliderCtrl	m_sliderGain;
	CSliderCtrl	m_sliderGamma;

	virtual BOOL OnInitDialog();

public:

	CDlgSearch *m_pParent;
	CameraParamSet		m_dProductShutter;
	CameraParamSet	m_dEnvironmentShutter;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnReset();
	CameraParamSet m_dProductShutterOld;

	CameraParamEnable   m_bEnableSearchShutter;			// 产品设置中曝光
};
