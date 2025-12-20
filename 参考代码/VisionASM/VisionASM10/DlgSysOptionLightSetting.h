#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "KeyBoard.h"


// CDlgSysOptionLightSetting 对话框
class CVisionASMDlg;
class CDlgSysOptionLightSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSysOptionLightSetting)

public:
	CDlgSysOptionLightSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysOptionLightSetting();

	// 对话框数据
	enum { IDD = IDD_SYS_OPTION_LIGHT_SETTING };

	// 保存数据
	void GetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig);
	void SetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig);
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


	// 初始化GUI
	void InitDlgItem();

	// 更新界面数据
	void UpdateDlgData(int nIndex, bool bSaveAndValidate);

	// 更新控件显示状态
	void UpdateShowStatus(int nIndex);

	// 光源控制器参数
	SystemOptionLightConfig m_SystemOptionLightConfig;
	//SystemOptionLightConfig m_tempSystemOptionLightConfig;

	bool m_bShowLightControl;
	bool m_bEnableLightControl;

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	void UpdateDlgByAuthorize();//Summer0131 权限管理
	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);//Summer0131 显示隐藏控件
	CVisionASMDlg *m_pParent;

	// 当前光源控制器序号
	int m_nRdLightControl;
	BOOL m_bChkEnable;

	CComboBox m_cbbLightType;
	CComboBox m_cbbCommType;

	CStatic m_stSerialPort;
	CStatic m_stBaudRate;
	CComboBox m_cbbBaudRate;
	CComboBox m_cbbSerialPort;

	CStatic m_stIPAddr;
	CStatic m_stPort;
	int m_nEditPort;
	CIPAddressCtrl m_ipAddressCtrl;


	CStatic m_stCurChannel[MAXLightChannelNum];
	CStatic m_stMaxChannel[MAXLightChannelNum];
	CStatic m_stMinChannel[MAXLightChannelNum];
	CSliderCtrl m_sliderChannel[MAXLightChannelNum];

	afx_msg void OnBnClickedRdLightControl1();
	afx_msg void OnCbnSelchangeComboLightType();
	afx_msg void OnCbnSelchangeComboCommType();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedChkEnable();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnOpenAll();
	afx_msg void OnBnClickedBtnOpenChannel1();
	afx_msg void OnBnClickedBtnOpenChannel2();
	afx_msg void OnBnClickedBtnOpenChannel3();
	afx_msg void OnBnClickedBtnOpenChannel4();
	afx_msg void OnBnClickedBtnOpenChannel5();
	afx_msg void OnBnClickedBtnOpenChannel6();
	afx_msg void OnBnClickedBtnOpenChannel7();
	afx_msg void OnBnClickedBtnOpenChannel8();
	afx_msg void OnBnClickedBtnCloseAll();

	CString m_strEditName[8];
	afx_msg void OnEnSetfocusEditPort();
	afx_msg void OnSetfocusIpaddress();

	//bool m_bAllOpenLightControl[MAXLightChannelNum];	//Summer0216 光源控制器  启用
	//bool m_bAllEnableLightControl[MAXLightChannelNum];	//Summer0216 光源控制器  打开
	afx_msg void OnEnKillfocusEditPort();
	afx_msg void OnBnClickedButtonLightCtrlRename();
	int m_nMaxChannelSum;
	afx_msg void OnEnSetfocusEditMaxChannelSum();
	afx_msg void OnEnKillfocusEditMaxChannelSum();
};
