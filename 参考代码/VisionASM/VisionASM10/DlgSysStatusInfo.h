#pragma once
#include "Label.h"
// CDlgSysStatusInfo 对话框
#define WM_UPDATE_SYSTEM_RESOURCE_INFO WM_USER + 1000 + 100
#define WM_SHOW_SYSTEM_RESOURCE_INFO WM_USER + 1000+ 101
#define WM_HIDE_SYSTEM_RESOURCE_INFO WM_USER + 1000 + 102
#define WM_QUIT_SYSTEM_RESOURCE_INFO_UI WM_USER + + 1000 + 103
class CSystemStatusInfo;
class CDlgSysStatusInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSysStatusInfo)

public:
	CDlgSysStatusInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysStatusInfo();

// 对话框数据
	enum { IDD = IDD_DLG_SYS_STATUS_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetOwner(LPVOID pOwner);
	void UpdateSystemResourceInfoText();

protected:
	void InitDlgItem();
private:
	CSystemStatusInfo* m_pOwner;

	CLabel m_labelCpuUse;
	CLabel m_labelMemUse;
	CLabel m_labelDiskUse;

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
};
