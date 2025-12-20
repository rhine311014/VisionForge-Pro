#pragma once
#include "Keyboard.h"
#include "Label.h"
#include "DlgSysOptionSetting.h"
#include "SystemStatusInfo.h"
#include "afxwin.h"
#include "ComboBoxColor.h"
// CDlgSysResStatistics 对话框

class CDlgSysResStatistics : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSysResStatistics)

public:
	CDlgSysResStatistics(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysResStatistics();

// 对话框数据
	enum { IDD = IDD_DLG_RES_STATISTICS };
public:
	BOOL	m_bSysKeyboardEnabled;
	int		m_nLanguage;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void SetParent(CDlgSysOptionSetting* pParent);
protected:
	CStringArray *m_psaSysInfoStrings;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	void UpdateDialogLanguage();
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	void InitDlgItem();
	void UpdateSystemResourceInfoText();
private:
	CDlgSysOptionSetting* m_pParent;
	CSystemStatusInfo*  m_pSysStatusInfo;
	CLabel m_labelCpuUse;
	CLabel m_labelMemUse;
/*	CLabel m_labelDiskUse;*/	
	CLabel m_labelHandleUse;
	CLabel m_labelGDIUse;
	static UINT CacuFunc(LPVOID pParam);//线程函数
	BOOL m_bExitCacuThread;
	CWinThread* m_pCacuThread;//统计线程
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_bGetSysResourceUse;
	double m_dMaxCpuUseThre;
	double m_dMaxDiskUseThre;
	double m_dMaxMemUseThre;
	int m_nMaxHandleUseThre;
	int m_nMaxGDIUseThre;
	afx_msg void OnBnClickedCheckGetSysResourceUse();
	afx_msg void OnEnSetfocusEditCpuUseThre();
	afx_msg void OnEnKillfocusEditCpuUseThre();
	afx_msg void OnEnSetfocusEditMemUseThre();
	afx_msg void OnEnKillfocusEditMemUseThre();
	afx_msg void OnEnSetfocusEditDiskUseThre();
	afx_msg void OnEnKillfocusEditDiskUseThre();
	afx_msg void OnEnSetfocusEditHandleUseThre();
	afx_msg void OnEnKillfocusEditHandleUseThre();
	afx_msg void OnEnSetfocusEditGdiUseThre();
	afx_msg void OnEnKillfocusEditGdiUseThre();
	CComboBoxColor m_cmbDiskUse;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL m_bThreValid;
	afx_msg void OnBnClickedChkThreValid();
};
