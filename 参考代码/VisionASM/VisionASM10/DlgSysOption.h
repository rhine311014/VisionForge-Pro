#pragma once

#include "DlgSysOptionSetting.h"
#include "DlgSysOptionLightSetting.h"
#include "Layout.h"
#include "afxwin.h"

// CDlgSysOption 对话框
class CVisionASMDlg;

class CDlgSysOption : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSysOption)

public:
	CDlgSysOption(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysOption();

	// 对话框数据
	enum { IDD = IDD_SYS_OPTION };

	// 是否是新系统
	BOOL m_bSelNewSystem;

	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	// 初始化窗口
	void InitDlgItem();
	void DeInitDlgItem();

	// 初始化UI
	void InitUIStyle();


	// 创建/销毁选项窗口
	void CreateOptionDialog();
	void DestroyOptionDialog();

	// 初始化窗口布局
	void CreateDlgLayout();
	void DestroyDlgLayout();



	// 选项窗口
	CDlgSysOptionSetting*		m_pDlgSysOptionSetting;			// 基础设置
	CDlgSysOptionLightSetting*  m_pDlgSysOptionLightSetting;    // 光源设置

	// 窗口布局
	CLayout* m_pOptionPlaneLayout; // 选项板布局
	CLayout* m_pButtonPlaneLayout; // 按钮板布局
	CLayout* m_pOptionSettingLayout; // 基础设置窗口布局
	CLayout* m_pOptionLightSettingLayout; // 光源设置窗口布局

	// 父窗口
	CVisionASMDlg* m_pParent;


	//选项配置
	CSystemOptionConfig m_SysOptionConfig;

	CStringArray *m_psaSysInfoStrings;


public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedRdOptBaseSetting();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);


	int m_nRdOption;
	CXPStyleButtonST m_btnPanel;
	CThemeHelperST	m_themeHelper;
};
