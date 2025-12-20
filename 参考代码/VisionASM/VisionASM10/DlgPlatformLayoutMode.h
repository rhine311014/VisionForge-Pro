#pragma once
#include "Layout.h"

// CDlgPlatformLayoutMode 对话框

class CDlgPlatformLayoutMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPlatformLayoutMode)

public:
	CDlgPlatformLayoutMode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPlatformLayoutMode();

// 对话框数据
	enum { IDD = IDD_DLG_PLATFORM_LAYOUT_MODE };

public:
	void UpdateShow(int nLayoutType, int nPlatformNum);	  //更新显示
	bool LayoutIsValid();  // 判断当前布局是否有效

protected:
	void FreeResource();

	void InitLayout();					  // 初始化UI显示
	void InitLayoutPlatformNum1();		  // 初始化平台数为1的时候， 布局显示
	void InitLayoutPlatformNum2();		  // 初始化平台数为2的时候， 布局显示
	void InitLayoutPlatformNum3();		  // 初始化平台数为3的时候， 布局显示
	void InitLayoutPlatformNum4();		  // 初始化平台数为4的时候， 布局显示
	void InitLayoutPlatformNum5();		  // 初始化平台数为5的时候， 布局显示
	void InitLayoutPlatformNum6();		  // 初始化平台数为6的时候， 布局显示


	void SetStVisible();				   // 设置Static显示状态


private:
	CLayout* m_pMainLayout;
	int m_nLayoutType;
	int m_nPlatformNum;
	int m_nStWidth;
	int m_nStHeight;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CStringArray* m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) {  m_psaSysInfoStrings = psaSysInfoStrings; };
	void UpdateDialogLanguage();			// 更新语言包
};
