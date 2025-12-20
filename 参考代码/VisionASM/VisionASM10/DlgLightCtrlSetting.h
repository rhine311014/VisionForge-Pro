#pragma once


// CDlgLightCtrlSetting 对话框

class CDlgLightCtrlSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLightCtrlSetting)

public:
	CDlgLightCtrlSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLightCtrlSetting();

	// 对话框数据
	enum { IDD = IDD_DLG_LIGHT_CTRL_SETTING };


	void SetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig);
	void GetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	void InitDlgItem();

	void UpdateDlgData(int nIndex, bool bSaveAndValidate);

	void UpdateShowStatus(int nIndex);

	// 光源控制器参数
	SystemOptionLightConfig m_SystemOptionLightConfig;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRdLightControl1();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	int m_nRdLightControl;
	CStatic m_stCurChannel[MAXLightChannelNum];
	CStatic m_stMaxChannel[MAXLightChannelNum];
	CStatic m_stMinChannel[MAXLightChannelNum];
	CSliderCtrl m_sliderChannel[MAXLightChannelNum];
	CString m_strEditName[MAXLightChannelNum];
};
