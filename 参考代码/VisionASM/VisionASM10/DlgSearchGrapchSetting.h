#pragma once
#include "afxcmn.h"
#include "SystemOptionConfig.h"
#include "DlgCrossSetting.h"

// CDlgSearchGrapchSetting 对话框

class CDlgSearchGrapchSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchGrapchSetting)

public:
	CDlgSearchGrapchSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearchGrapchSetting();

// 对话框数据
	enum { IDD = IDD_DLG_SEARCH_GRAPH_SETTING };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitDlgItem();
	void UpdataDlgdata(BOOL flag);
	void UpdateDialogLanguage();
	bool m_bSysKeyboardEnabled;
	CTabCtrl m_tab;
	CStringArray *m_psaSysInfoStrings;
	SearchResultCrossSize m_TarCross;
	SearchResultCrossSize m_ObjCross;
	CDlgCrossSetting * m_pDlgTarCross;
	CDlgCrossSetting * m_pDlgObjCross;
    int m_nPrevIndex;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
    virtual void OnCancel();
};
