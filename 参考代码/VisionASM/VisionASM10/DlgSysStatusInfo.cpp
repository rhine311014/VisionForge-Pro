// DlgSysStatusInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysStatusInfo.h"
#include "afxdialogex.h"
#include "SystemStatusInfo.h"
#include "VisionASMDLG.h"

// CDlgSysStatusInfo 对话框

IMPLEMENT_DYNAMIC(CDlgSysStatusInfo, CDialogEx)

CDlgSysStatusInfo::CDlgSysStatusInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSysStatusInfo::IDD, pParent)
{
	m_pOwner = NULL;
}

CDlgSysStatusInfo::~CDlgSysStatusInfo()
{
}

void CDlgSysStatusInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SYS_CPU_INFO, m_labelCpuUse);
	DDX_Control(pDX, IDC_STATIC_SYS_MEMORY_INFO, m_labelMemUse);
	DDX_Control(pDX, IDC_STATIC_SYS_DISK_INFO, m_labelDiskUse);
}


BEGIN_MESSAGE_MAP(CDlgSysStatusInfo, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgSysStatusInfo 消息处理程序


LRESULT CDlgSysStatusInfo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_SYSTEM_RESOURCE_INFO:
		{
			UpdateSystemResourceInfoText();
		}
		break;
	case WM_SHOW_SYSTEM_RESOURCE_INFO:
		{
			//GetDlgItem(IDC_STATIC_SYS_CPU_INFO)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_STATIC_SYS_MEMORY_INFO)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_STATIC_SYS_DISK_INFO)->ShowWindow(SW_SHOW);
			ShowWindow(SW_SHOW);
	
		}
		break;
	case WM_HIDE_SYSTEM_RESOURCE_INFO:
		{
			//GetDlgItem(IDC_STATIC_SYS_CPU_INFO)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_SYS_MEMORY_INFO)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_SYS_DISK_INFO)->ShowWindow(SW_HIDE);
			ShowWindow(SW_HIDE);
			
		}
		break;
	case WM_QUIT_SYSTEM_RESOURCE_INFO_UI:
		{
			DestroyWindow();
		}
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}
void CDlgSysStatusInfo::SetOwner(LPVOID pOwner)
{
	m_pOwner = (CSystemStatusInfo*)pOwner;
}
void CDlgSysStatusInfo::UpdateSystemResourceInfoText()
{
	BOOL bWarningCpu = FALSE;
	BOOL bWarningMem = FALSE;
	BOOL bWarningDisk = FALSE;
	CString strSysInfoCpu = _T("");
	CString strSysInfoMem = _T("");
	CString strSysInfoDisk = _T("Disk:");

	CString strTmp = _T("");

	double SysMemPercent = m_pOwner->GetSysMemoryUsedRate();
	strSysInfoMem.Format(_T("Mem:%.0f%%"),SysMemPercent*100);
	if (((m_pOwner->m_pOwner)->m_SysOptionConfig.m_bThreValid)&&(SysMemPercent > (m_pOwner->m_pOwner)->m_SysOptionConfig.m_dMaxMemUseThre))
	{
		bWarningMem = TRUE;
		
	}
	double CpuPercent = m_pOwner->GetCpuUsedRate();
	strSysInfoCpu.Format(_T("Cpu:%.0f%%"),CpuPercent*100);
	if (((m_pOwner->m_pOwner)->m_SysOptionConfig.m_bThreValid)&& (CpuPercent>(m_pOwner->m_pOwner)->m_SysOptionConfig.m_dMaxCpuUseThre))
	{
		bWarningCpu = TRUE;
	}

	double DiskPercent = 0.0;
	CString strDiskWarning;
	std::vector<CString> DiskName = m_pOwner->GetDiskName();
	for (int i=0;i<DiskName.size()&&i<3;i++)
	{
		m_pOwner->GetDiskUsedRate(DiskName.at(i),DiskPercent);
		if (((m_pOwner->m_pOwner)->m_SysOptionConfig.m_bThreValid)&&(DiskPercent>(m_pOwner->m_pOwner)->m_SysOptionConfig.m_dMaxDiskUseThre))
		{
			bWarningDisk = TRUE;
		}
		strTmp.Format(_T("%s%.0f%% "),DiskName.at(i).Left(DiskName.at(i).Find('\\')),DiskPercent*100);
		strSysInfoDisk = strSysInfoDisk + strTmp;
	}
	COLORREF TmpColor = RGB(54,164,186);
	COLORREF TmpColor2 =RGB(247,214,170);
	m_labelCpuUse.SetRedraw(FALSE);
	m_labelCpuUse.SetWindowText(strSysInfoCpu);
	m_labelCpuUse.SetRedraw(TRUE);
	if (bWarningCpu)
	{
		//m_labelCpuUse.SetTextColor(RGB(255,0,0));
		m_labelCpuUse.SetTextColor(TmpColor2);
		m_labelCpuUse.FlashText(TRUE);
	}
	else
	{
		m_labelCpuUse.SetTextColor(RGB(255,255,255));
		m_labelCpuUse.FlashText(FALSE);
	}	
	//m_labelCpuUse.SetBkColor(RGB(71,71,170),RGB(74,74,180),CLabel::Gradient);
	//m_labelCpuUse.SetBkColor(RGB(0,0,250),RGB(0,0,250),CLabel::Gradient);

	m_labelCpuUse.SetBkColor(TmpColor);

	m_labelCpuUse.SetFontBold(TRUE);
	m_labelCpuUse.SetFontSize(10);

	m_labelMemUse.SetRedraw(FALSE);
	m_labelMemUse.SetWindowText(strSysInfoMem);
	m_labelMemUse.SetRedraw(TRUE);
	if (bWarningMem)
	{
		//m_labelMemUse.SetTextColor(RGB(255,0,0));

		m_labelMemUse.SetTextColor(TmpColor2);
		m_labelMemUse.FlashText(TRUE);
	}
	else
	{
		m_labelMemUse.SetTextColor(RGB(255,255,255));
		m_labelMemUse.FlashText(FALSE);
	}	
	//m_labelMemUse.SetBkColor(RGB(74,74,180),RGB(77,77,190),CLabel::Gradient);		
	//m_labelMemUse.SetBkColor(RGB(0,0,250),RGB(0,0,250),CLabel::Gradient);

	
	m_labelMemUse.SetBkColor(TmpColor);

	m_labelMemUse.SetFontBold(TRUE);
	m_labelMemUse.SetFontSize(10);

	m_labelDiskUse.SetRedraw(FALSE);
	m_labelDiskUse.SetWindowText(strSysInfoDisk);
	m_labelDiskUse.SetRedraw(TRUE);
	if (bWarningDisk)
	{
		//m_labelDiskUse.SetTextColor(RGB(255,0,0));
		m_labelDiskUse.SetTextColor(TmpColor2);
		m_labelDiskUse.FlashText(TRUE);
	}
	else
	{
		m_labelDiskUse.SetTextColor(RGB(255,255,255));
		m_labelDiskUse.FlashText(FALSE);
	}	
	//m_labelDiskUse.SetBkColor(RGB(77,77,190),RGB(80,80,200),CLabel::Gradient);	
	//m_labelDiskUse.SetBkColor(RGB(0,0,250),RGB(0,0,250),CLabel::Gradient);

	m_labelMemUse.SetBkColor(TmpColor);

	m_labelDiskUse.SetFontBold(TRUE);
	m_labelDiskUse.SetFontSize(10);


}


BOOL CDlgSysStatusInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitDlgItem();
	// TODO:  在此添加额外的初始化
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgSysStatusInfo::InitDlgItem()
{
	CRect rc,rc1,rc2,rc3,rcTmp;
	GetClientRect(rc);

	m_labelCpuUse.GetWindowRect(rcTmp);
	ScreenToClient(rcTmp);
	rc1=rcTmp;
	rc1.right=rc1.left+rc.Width()*1.1/5;
	m_labelCpuUse.MoveWindow(rc1);
	//m_labelCpuUse.SetBkColor(RGB(71,71,170),RGB(74,74,180),CLabel::Gradient);	
	//m_labelCpuUse.SetBkColor(RGB(0,0,250),RGB(0,0,250),CLabel::Gradient);
	COLORREF TmpColor = RGB(54,164,186);
	m_labelCpuUse.SetBkColor(TmpColor);

	m_labelCpuUse.SetFontBold(TRUE);
	m_labelCpuUse.SetFontSize(10);

	m_labelMemUse.GetWindowRect(rcTmp);
	ScreenToClient(rcTmp);
	rc2.left=rc1.right;
	rc2.right = rc2.left + rc.Width()*1.2/5;
	rc2.top=rc1.top;
	rc2.bottom = rc2.top + rcTmp.Height();
	m_labelMemUse.MoveWindow(rc2);
	//m_labelMemUse.SetBkColor(RGB(74,74,180),RGB(77,77,190),CLabel::Gradient);	
	m_labelMemUse.SetBkColor(RGB(0,0,250),RGB(0,0,250),CLabel::Gradient);

	m_labelMemUse.SetBkColor(TmpColor);

	m_labelMemUse.SetFontBold(TRUE);
	m_labelMemUse.SetFontSize(10);

	m_labelDiskUse.GetWindowRect(rcTmp);
	ScreenToClient(rcTmp);
	rc3.left=rc2.right;
	rc3.right = rc3.left + rc.Width()*2.7/5;
	rc3.top=rc2.top;
	rc3.bottom=rc3.top+rcTmp.Height();
	m_labelDiskUse.MoveWindow(rc3);
	//m_labelDiskUse.SetBkColor(RGB(77,77,190),RGB(80,80,200),CLabel::Gradient);	
	//m_labelDiskUse.SetBkColor(RGB(0,0,250),RGB(0,0,250),CLabel::Gradient);

	m_labelDiskUse.SetBkColor(TmpColor);

	m_labelDiskUse.SetFontBold(TRUE);
	m_labelDiskUse.SetFontSize(10);
}

BOOL CDlgSysStatusInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgSysStatusInfo::OnDestroy()
{	
	CDialogEx::OnDestroy();	
	PostQuitMessage(0);
	// TODO: 在此处添加消息处理程序代码
}


BOOL CDlgSysStatusInfo::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DestroyWindow();

}
