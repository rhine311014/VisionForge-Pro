#pragma once

#include "KeyBoard.h"


// CDlgProductOffsetSetting 对话框

class CDlgProductOffsetSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgProductOffsetSetting)

public:
	CDlgProductOffsetSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProductOffsetSetting();

// 对话框数据
	enum { IDD = IDD_DLG_PRODUCT_OFFSET_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }

    void UpdateDialogLanguage();
	CStringArray	*m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }


	virtual BOOL OnInitDialog();

// 	CComboBox m_ProductOffsetXDirection;
// 	CComboBox m_ProductOffsetYDirection;
// 	CComboBox m_ProductOffsetDDirection;
// 	
// 	CComboBox m_ProductOffsetXSetting;
// 	CComboBox m_ProductOffsetYSetting;
// 	CComboBox m_ProductOffsetDSetting;

// 	int m_nProductOffsetXDirection;
// 	int m_nProductOffsetYDirection;
// 	int m_nProductOffsetDDirection;
// 
// 	int m_nProductOffsetXSetting;
// 	int m_nProductOffsetYSetting;
// 	int m_nProductOffsetDSetting;


	BOOL m_bEnableProductOffsetXDirection;
	BOOL m_bEnableProductOffsetYDirection;
	BOOL m_bEnableProductOffsetDDirection;

	BOOL m_bEnableProductOffsetXSetting;
	BOOL m_bEnableProductOffsetYSetting;
	BOOL m_bEnableProductOffsetDSetting;


// 	afx_msg void OnSelchangeComboProductOffsetXDirection();
// 	afx_msg void OnSelchangeComboProductOffsetYDirection();
// 	afx_msg void OnSelchangeComboProductOffsetDDirection();
// 	afx_msg void OnSelchangeComboProductOffsetXSetting();
// 	afx_msg void OnSelchangeComboProductOffsetYSetting();
// 	afx_msg void OnSelchangeComboProductOffsetDSetting();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	BOOL bEnableProductOffsetXDirectionOld;
	BOOL bEnableProductOffsetYDirectionOld;
	BOOL bEnableProductOffsetDDirectionOld;

	BOOL bEnableProductOffsetXSettingOld;
	BOOL bEnableProductOffsetYSettingOld;
	BOOL bEnableProductOffsetDSettingOld;
	afx_msg void OnBnClickedCheckProductOffsetXDirection();

	afx_msg void OnBnClickedCheckProductOffsetYDirection();
	afx_msg void OnBnClickedCheckProductOffsetDDirection();
	afx_msg void OnBnClickedCheckProductOffsetXSetting();
	afx_msg void OnBnClickedCheckProductOffsetYSetting();
	afx_msg void OnBnClickedCheckProductOffsetDSetting();
};
