#pragma once
#include "Layout.h"

// CDlgViewPosLayoutMode 对话框

class CDlgViewPosLayoutMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgViewPosLayoutMode)

public:
	CDlgViewPosLayoutMode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgViewPosLayoutMode();

// 对话框数据
	enum { IDD = IDD_DLG_VIEW_POS_LAYOUT_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void SetLayoutType(int nLayoutType);
	void SetPosNum(int nPosNum);

public:
	void UpdateLayout(int nLayoutType, int nPosNum);
	bool LayoutIsValid();  // 布局是否有效

protected:
	void FreeResource();
private:
	int m_nLayoutType; // 布局类型 暂定1-6
	int m_nPosNum;	   // 设置位置数


	int m_nSTWidth;
	int m_nSTHeigh;
protected:
	void InitLayout();			// 初始化布局
	void InitLayoutPos1();	   	// 位置数1布局
	void InitLayoutPos2();		// 位置数2布局
	void InitLayoutPos3();		// 位置数3布局
	void InitLayoutPos4();		// 位置数4布局
	void InitLayoutPos6();		// 位置数6布局
	void InitLayoutPos8();		// 位置数8布局

	void SetControlHide();     // 设置空间隐藏
	void SetControlVisible(); // 根据位置数， 设置控件显示

private:
	CLayout* m_pMainLayout;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CStringArray* m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) {  m_psaSysInfoStrings = psaSysInfoStrings; };
	void UpdateDialogLanguage();			// 更新语言包
};
