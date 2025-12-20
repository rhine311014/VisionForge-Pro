#if !defined(AFX_DLGSYSSERIALCOMMTEST_H__A0EE5E1F_F388_4B3A_95C4_C3FB031E44E1__INCLUDED_)
#define AFX_DLGSYSSERIALCOMMTEST_H__A0EE5E1F_F388_4B3A_95C4_C3FB031E44E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysSerialCommTest.h : header file
//

#include "vsSerialComm.h"
#include "DlgSysBaseCommSetting.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysSerialCommSetting dialog

class CDlgSysCommSetting;
class CDlgSysSerialCommSetting : public CDlgSysBaseCommSetting
{
// Construction
public:
	CDlgSysSerialCommSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysSerialCommSetting)
	enum { IDD = IDD_SYS_SERIAL_COMM_CONFIG };
	CXPStyleButtonST	m_btnSend;
	CXPStyleButtonST	m_btnCommSwitch;
	CXPStyleButtonST	m_btnSDClear;
	CXPStyleButtonST	m_btnRDClear;
	CComboBox	m_comboCommStopChar;
	CComboBox	m_comboCmdStopChar;
	CStatic	m_ctrlCommLamp;
	CComboBox	m_comboStopBits;
	CComboBox	m_comboComPort;
	CComboBox	m_comboDataBits;
	CComboBox	m_comboParity;
	CComboBox	m_comboBaudRate;
	CString	m_strDataReceived;
	CString m_strDataReceivedOnce;
	CString	m_strDataSending;
	CString m_strRegAddress;
	CString m_strTempRegAddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysSerialCommSetting)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysSerialCommSetting)
	afx_msg void OnButtonCleanRxdata();
	afx_msg void OnButtonManualsend();
	afx_msg void OnButtonCleanTxdata();
	afx_msg void OnButtonCloseComm();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditTxdata();
	afx_msg void OnSelchangeComboCommPort();
	afx_msg void OnSelchangeComboCommData();
	afx_msg void OnSelchangeComboCommCommStopChar();
	afx_msg void OnSelchangeComboCommCmdStopChar();
	afx_msg void OnSelchangeComboCommCheck();
	afx_msg void OnSelchangeComboCommBaud();
	afx_msg void OnSelchangeComboCommStop();
	afx_msg void OnKillfocusEdtSerialRegAddress();
	afx_msg void OnSetfocusEdtSerialRegAddress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	vcSerialCommInfo *m_pCommInfo;

	virtual void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	virtual void InitDlgItem();
	virtual void UpdateDialogLanguage();
	virtual void ReportCommStatus(BOOL bStatusConnected);
	virtual BOOL TestReceiveCommRepeat(CString& strInfo, CommStatus& nCommStatus);
	virtual BOOL TestIsSuccessReceived(CString& strReceivedInfo,CString strReadInfo);
public:
	static  UINT ThreadReceiveData(LPVOID pParam);     // 工作线程
	void UpdateReceiveData();
	virtual void UpdateCommStatus();
	
	int m_nRegType;
	CString strCommPortOld;
	CString strCommBaudOld;
	CString strCommCheckOld;
	CString strCommDataOld;
	CString strCommStopOld;
	CString strCmdStopCharOld;
	CString strCommStopCharOld;
	BOOL m_bRadioWordMode;        //字节类型：FALSE-单字节,TRUE-双字节
	int m_nEditReadRegAddrValue;
	int m_nEditWriteRegAddrValue;
	int m_nEditWriteRegValue;
	int m_nEditReadRegValue;
	afx_msg void OnEnSetfocusEditReadRegAddrValue();
	afx_msg void OnEnSetfocusEditWriteRegAddrValue();
	afx_msg void OnEnSetfocusEditWriteRegValue();
	afx_msg void OnBnClickedButtonReadReg();
	afx_msg void OnBnClickedButtonWriteReg();
	CXPStyleButtonST m_btnReadReg;
	CXPStyleButtonST m_btnWriteReg;
	afx_msg void OnEnKillfocusEditReadRegAddrValue();
	afx_msg void OnEnKillfocusEditWriteRegAddrValue();
	//afx_msg void OnBnClickedRadioRegChange();
	afx_msg void OnKillfocusEdtTempRegAddress();
	afx_msg void OnSetfocusEdtTempRegAddress();
	long m_lWriteRegMaxNum;
	long m_lReadRegMaxNum;
	afx_msg void OnEnSetfocusEdtWriteRegMaxnum();
	afx_msg void OnEnSetfocusEdtReadRegMaxnum();

	virtual void UpdateControlShow(BOOL bVisible);
	//virtual void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

	bool m_bshowCommParamSet;
	bool m_bEnableCommParamSet;

	bool m_bshowCommTestSet;
	bool m_bEnableCommTestSet;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSSERIALCOMMTEST_H__A0EE5E1F_F388_4B3A_95C4_C3FB031E44E1__INCLUDED_)
