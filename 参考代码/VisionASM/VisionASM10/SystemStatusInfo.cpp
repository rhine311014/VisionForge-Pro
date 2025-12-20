#include "stdafx.h"
#include "SystemStatusInfo.h"
#include "Psapi.h"
#include "VisionASMDLG.h"


//cTimer::CTimerMap cTimer::m_sTimeMap;
//cTimer::CTimerIdEventMap cTimer::m_sTimeIdEventMap;
//cTimer::cTimer()
//{
//
//}
//cTimer::~cTimer()
//{
//
//}
//
//void cTimer::SetTimer(UINT_PTR nIDEvent,UINT nElapse,CSystemStatusInfo* pOwner)//设置定时器，nElapse表示时间间隔，sz表示要提示的内容
//{
//	UINT_PTR idEvent = ::SetTimer(NULL,nIDEvent,nElapse,(TIMERPROC)TimerProc);
//	m_sTimeIdEventMap[nIDEvent] = idEvent;
//	m_sTimeMap[idEvent]=pOwner;
//}
//
//void cTimer::KillTimer(UINT_PTR nIDEvent)//销毁该实例的定时器
//{
//	::KillTimer(NULL,nIDEvent);
//	m_sTimeMap.RemoveKey(m_sTimeIdEventMap[nIDEvent]);
//	m_sTimeIdEventMap.RemoveKey(nIDEvent);
//}
//
//
//void CALLBACK cTimer::TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)//静态成员函数，用于处理定时器的消息
//{
//
//	m_sTimeMap[idEvent]->PostMessageUpdateSystemStatusInfo();
//	
//}



SystemStatusInfoUiThread::SystemStatusInfoUiThread(void)
{
	m_pDlg = new CDlgSysStatusInfo;
}
SystemStatusInfoUiThread::~SystemStatusInfoUiThread(void)
{
	if (m_pDlg->GetSafeHwnd())
	{
		m_pDlg->DestroyWindow();
	}
	if (m_pDlg!=NULL)
	{
		delete m_pDlg;
		m_pDlg = NULL;
		m_pMainWnd = NULL;
	}


}
BOOL SystemStatusInfoUiThread::InitInstance()
{

	if (m_pDlg != NULL)
	{
		m_pDlg->Create(IDD_DLG_SYS_STATUS_INFO,NULL);
		m_pMainWnd = m_pDlg;
		CRect rc,rcTmp;
		m_pDlg->GetWindowRect(rcTmp);
		rc.right = m_ParentWndPos.right;
		rc.left = rc.right - rcTmp.Width();
		rc.top = m_ParentWndPos.top;
		rc.bottom = rc.top+rcTmp.Height();
		m_pDlg->MoveWindow(rc);
		m_pDlg->ShowWindow(SW_SHOW);
		m_pDlg->UpdateWindow();
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
int SystemStatusInfoUiThread::ExitInstance()
{

	if (m_pDlg->GetSafeHwnd())
	{
		m_pDlg->DestroyWindow();
	}
	if (m_pDlg!=NULL)
	{
		delete m_pDlg;
		m_pDlg = NULL;
		m_pMainWnd = NULL;
	}


	return CWinThread::ExitInstance();//调用基类中的该函数
}

void SystemStatusInfoUiThread::SetParentWndPos(CRect rc)
{
	m_ParentWndPos = rc;	
}

void SystemStatusInfoUiThread::SetOwner(LPVOID pOnwer)
{
	m_pDlg->SetOwner(pOnwer);
}
CSystemStatusInfo::CSystemStatusInfo(void)
{
	m_dSysMemoryUsedRate = 0.0;
	m_dProcessMemoryUsedAmount_M = 0.0;
	m_dCpuUsedRate = 0.0;
	m_nHandleUsedCount = 0;
	m_nGDIUsedCount = 0;

	m_pMonitorThread =NULL;
	m_pUiThread = NULL;
	m_bExitMonitorThread = FALSE;
	
	m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_bMoniterThreadSuspend = TRUE;

	m_pUpdateShowThread = NULL;
	m_bUpdateShowThreadSuspend = TRUE;
	m_bExitUpdateShowThread = FALSE;
}


CSystemStatusInfo::~CSystemStatusInfo(void)
{
	StopMonitor();
	CloseHandle(m_hEvent);
}


UINT CSystemStatusInfo::MonitorFunc(LPVOID pParam)
{
	CSystemStatusInfo* pOwner = (CSystemStatusInfo*)pParam;
	if (pOwner!=NULL)
	{
		pOwner->CacuDriveName();
		while(!pOwner->m_bExitMonitorThread)
		{
			pOwner->CacuDriveUsedInfo();
			pOwner->CacuMemoryUsedInfo();
			pOwner->CacuCpuUsedInfo();
			pOwner->CacuHandleUsedCount();
			pOwner->CacuGDIUsedCount();
			Sleep(100);
			/*pOwner->PostMessageUpdateSystemStatusInfo();
			Sleep(1000);*/
			
		}
	}
	return 0;
	
}

UINT CSystemStatusInfo::UpdateShowFunc(LPVOID pParam)//显示刷新函数
{
	CSystemStatusInfo* pOwner = (CSystemStatusInfo*)pParam;
	if (pOwner!=NULL)
	{
		while(!pOwner->m_bExitUpdateShowThread)
		{
			
			pOwner->PostMessageUpdateSystemStatusInfo();
			Sleep(1500);
		}
	}
	return 0;
}

BOOL CSystemStatusInfo::StopMonitor()
{
	if (m_pMonitorThread)
	{
		if (m_bMoniterThreadSuspend == TRUE)
		{
			m_pMonitorThread->ResumeThread();
			m_bMoniterThreadSuspend = FALSE;
		}
		
		m_bExitMonitorThread = TRUE;
		if(WaitForSingleObject(m_pMonitorThread->m_hThread,5000)==WAIT_TIMEOUT)
		{
			TerminateThread(m_pMonitorThread->m_hThread,0);
		}
		m_pMonitorThread = NULL;
		m_bMoniterThreadSuspend = TRUE;
	}
	if (m_pUpdateShowThread)
	{
		if (m_bUpdateShowThreadSuspend == TRUE)
		{
			m_pUpdateShowThread->ResumeThread();
			m_bUpdateShowThreadSuspend = FALSE;
		}

		m_bExitUpdateShowThread = TRUE;
		if(WaitForSingleObject(m_pUpdateShowThread->m_hThread,5000)==WAIT_TIMEOUT)
		{
			TerminateThread(m_pUpdateShowThread->m_hThread,0);
		}
		m_pUpdateShowThread = NULL;
		m_bUpdateShowThreadSuspend = TRUE;
	}
	Sleep(200);
	if (m_pUiThread !=NULL)
	{
		CDialogEx* pDlg = ((SystemStatusInfoUiThread*)m_pUiThread)->GetMainFrameDlg();
		if (pDlg != NULL && pDlg->GetSafeHwnd())
		{
			::PostThreadMessage(m_pUiThread->m_nThreadID, WM_QUIT,0,0);
			/*::PostMessage(pDlg->GetSafeHwnd(),WM_QUIT_SYSTEM_RESOURCE_INFO_UI,0,0);*/

			if(WaitForSingleObject(m_pUiThread->m_hThread,10000)==WAIT_TIMEOUT)
			{
				TerminateThread(m_pUiThread->m_hThread,0);
				delete m_pUiThread;
			}
			if (m_pUiThread!=NULL)
			{
				m_pUiThread = NULL;
			}
		}
	}
	//m_Timer.KillTimer(0);
	return TRUE;
}

BOOL CSystemStatusInfo::CacuDriveName()
{
	TCHAR szBuf[100];
	memset(szBuf,0,100);
	CString strDrive;
	DWORD len = GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);
	m_vStrDriveName.clear();
	for (TCHAR* s=szBuf;*s;s+=_tcslen(s)+1)
	{
		strDrive = s;
		m_vStrDriveName.push_back(strDrive);
	}
	m_vdDriveUsedRate.resize(m_vStrDriveName.size(),0.0);
	return TRUE;
}

BOOL CSystemStatusInfo::CacuDriveUsedInfo()
{
	DWORD64 qwFreeBytes,qwFreeBytesToCaller,qwTotalBytes;
	BOOL bAllOK = TRUE;
	BOOL bResult = FALSE;
	CString strDrive;
	for (int i=0;i<m_vStrDriveName.size()&&i<m_vdDriveUsedRate.size();i++)
	{
		strDrive = m_vStrDriveName.at(i);
		bResult = GetDiskFreeSpaceEx(strDrive
										,(PULARGE_INTEGER)&qwFreeBytesToCaller
										,(PULARGE_INTEGER)&qwTotalBytes
										,(PULARGE_INTEGER)&qwFreeBytes);

		if (!bResult)
		{
			bAllOK = FALSE;
			continue;
		}
		m_vdDriveUsedRate.at(i)=1-(double)qwFreeBytes/(double)qwTotalBytes;
	}	
	return bAllOK;
}

BOOL CSystemStatusInfo::GetDiskUsedRate(CString strDrive,double& usePercent)const
{
	CString strLogicDrive = strDrive.Left(strDrive.Find('\\')+1);
	for (int i=0;i<m_vStrDriveName.size();i++)
	{
		if (strLogicDrive == m_vStrDriveName.at(i))
		{
			usePercent = m_vdDriveUsedRate.at(i);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CSystemStatusInfo::CacuMemoryUsedInfo()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	BOOL bResult = GlobalMemoryStatusEx(&statex);
	if (!bResult)
	{
		return FALSE;
	}
	DWORDLONG usePhys = statex.ullTotalPhys - statex.ullAvailPhys;
	m_dSysMemoryUsedRate = (double)usePhys/(double)statex.ullTotalPhys;

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	DWORD pid = GetCurrentProcessId();
	HANDLE handle;
	handle = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if (!handle)
	{
		return FALSE;
	}
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));
	PSAPI_WORKING_SET_INFORMATION workSet;
	memset(&workSet,0,sizeof(workSet));
	bResult = QueryWorkingSet(handle,&workSet,sizeof(workSet));
	if(bResult || (!bResult) && GetLastError() == ERROR_BAD_LENGTH)
	{
		int nSize = sizeof(workSet.NumberOfEntries) + workSet.NumberOfEntries*sizeof(workSet.WorkingSetInfo);
		char* pBuf = new char[nSize];
		if (!pBuf)
		{
			CloseHandle(handle);
			return FALSE;
		}
		QueryWorkingSet(handle,pBuf,nSize);
		PSAPI_WORKING_SET_BLOCK* pFirst = (PSAPI_WORKING_SET_BLOCK*)(pBuf + sizeof(workSet.NumberOfEntries));
		DWORD64 dwMem = 0;
		for (ULONG_PTR nMemEntryCnt = 0;nMemEntryCnt<workSet.NumberOfEntries;nMemEntryCnt++,pFirst++)
		{
			if (pFirst->Shared == 0)
			{
				dwMem += si.dwPageSize;
			}
		}
		delete pBuf;
		pBuf = NULL;
		if (workSet.NumberOfEntries>0)
		{
			m_dProcessMemoryUsedAmount_M = dwMem/(1024*1024);
		}
	}
	else
	{
		CloseHandle(handle);
		return FALSE;
	}
	CloseHandle(handle);
	return TRUE;
}

BOOL CSystemStatusInfo::CacuHandleUsedCount()
{
	DWORD pid = GetCurrentProcessId();
	HANDLE handle;
	handle = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if (!handle)
	{
		return FALSE;
	}
	DWORD nHandleCount = 0;
	if(!GetProcessHandleCount(handle,&nHandleCount))
	{
		CloseHandle(handle);
		return FALSE;
	}
	m_nHandleUsedCount = (int)nHandleCount;
	CloseHandle(handle);
	return TRUE;
}

BOOL CSystemStatusInfo::CacuGDIUsedCount()
{
	DWORD pid = GetCurrentProcessId();
	HANDLE handle;
	handle = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if (!handle)
	{
		return FALSE;
	}
	DWORD nGDICount = GetGuiResources(handle,GR_GDIOBJECTS);
	m_nGDIUsedCount = (int)nGDICount;
	CloseHandle(handle);
	return TRUE;
}
BOOL CSystemStatusInfo::CacuCpuUsedInfo()
{
	BOOL res;
	FILETIME preIdleTime,preKernelTime,preUserTime;
	FILETIME idleTime,kernelTime,userTime;
	res = GetSystemTimes(&idleTime,&kernelTime,&userTime);
	if(!res)
		return FALSE;
	preIdleTime = idleTime;
	preKernelTime = kernelTime;
	preUserTime = userTime;
	WaitForSingleObject(m_hEvent,1000);
	res = GetSystemTimes(&idleTime,&kernelTime,&userTime); 
	if(!res)
		return FALSE;
	int idle = (int)DiffFileTime(preIdleTime,idleTime);
	int kernel = (int)DiffFileTime(preKernelTime,kernelTime);
	int user = (int)DiffFileTime(preUserTime,userTime);
	m_dCpuUsedRate = (double)(kernel+user-idle)/(double)(kernel+user);
	return TRUE;
}

DWORD64 CSystemStatusInfo::DiffFileTime(FILETIME timePre, FILETIME timeNow)
{

	DWORD64 a = timePre.dwHighDateTime << sizeof(DWORD)*8 | timePre.dwLowDateTime;

	DWORD64 b = timeNow.dwHighDateTime << sizeof(DWORD)*8 | timeNow.dwLowDateTime;

	return   b - a;

}

void CSystemStatusInfo::PostMessageUpdateSystemStatusInfo()
{
	int i = 0;
	if (m_pUiThread ==NULL)
	{
		return;
	}
	CDialogEx* pDlg = ((SystemStatusInfoUiThread*)m_pUiThread)->GetMainFrameDlg();
	if (pDlg == NULL || !pDlg->GetSafeHwnd())
	{
		return;
	}
	PostMessage(pDlg->m_hWnd, WM_UPDATE_SYSTEM_RESOURCE_INFO,0,0);
	
}

BOOL CSystemStatusInfo::BeginMonitor(LPVOID pOwner,CRect parentWndPos)
{
	StopMonitor();
	m_pOwner = (CVisionASMDlg*)pOwner;
	if (m_pOwner == NULL)
	{
		return FALSE;
	}

 	m_pUiThread = new SystemStatusInfoUiThread;
	((SystemStatusInfoUiThread*)m_pUiThread)->SetOwner(this);
	((SystemStatusInfoUiThread*)m_pUiThread)->SetParentWndPos(parentWndPos);
	m_pUiThread->CreateThread();
	Sleep(200);
	m_bExitMonitorThread = FALSE;
	m_pMonitorThread = AfxBeginThread(MonitorFunc,this);
	m_bMoniterThreadSuspend = FALSE;

	m_bExitUpdateShowThread = FALSE;
	m_pUpdateShowThread = AfxBeginThread(UpdateShowFunc,this);
	m_bUpdateShowThreadSuspend = FALSE;
	//m_Timer.SetTimer(0,1000,this);
	return TRUE;
}

BOOL CSystemStatusInfo::SuspendMonitor()
{
	if (m_pMonitorThread == NULL)
	{
		return FALSE;
	}
	if (m_bMoniterThreadSuspend == FALSE)
	{
		m_pMonitorThread->SuspendThread();
		m_bMoniterThreadSuspend = TRUE;
		//m_Timer.KillTimer(0);
	}
	if (m_bUpdateShowThreadSuspend == FALSE)
	{
		m_pUpdateShowThread->SuspendThread();
		m_bUpdateShowThreadSuspend = TRUE;
	}
	
	if (m_pUiThread ==NULL)
	{
		return FALSE;
	}
	CDialogEx* pDlg = ((SystemStatusInfoUiThread*)m_pUiThread)->GetMainFrameDlg();
	if (pDlg == NULL || !pDlg->GetSafeHwnd())
	{
		return FALSE;
	}
	::PostMessage(pDlg->m_hWnd, WM_HIDE_SYSTEM_RESOURCE_INFO,0,0);
	
	return TRUE;
}
BOOL CSystemStatusInfo::ResumeMonitor()
{
	if (m_pMonitorThread == NULL)
	{
		return FALSE;
	}
	if (m_bMoniterThreadSuspend == TRUE)
	{
		m_pMonitorThread->ResumeThread();
		m_bMoniterThreadSuspend = FALSE;
		//m_Timer.SetTimer(0,1000,this);
	}
	if (m_bUpdateShowThreadSuspend == TRUE)
	{
		m_pUpdateShowThread->ResumeThread();
		m_bUpdateShowThreadSuspend = FALSE;
	}
	
	if (m_pUiThread ==NULL)
	{
		return FALSE;
	}
	CDialogEx* pDlg = ((SystemStatusInfoUiThread*)m_pUiThread)->GetMainFrameDlg();
	if (pDlg == NULL || !pDlg->GetSafeHwnd())
	{
		return FALSE;
	}
	::PostMessage(pDlg->m_hWnd, WM_SHOW_SYSTEM_RESOURCE_INFO,0,0);
	return TRUE;
}

BOOL CSystemStatusInfo::ShowUIWindow(BOOL bShow)
{
	if (m_pUiThread ==NULL)
	{
		return FALSE;
	}
	CDialogEx* pDlg = ((SystemStatusInfoUiThread*)m_pUiThread)->GetMainFrameDlg();
	if (pDlg == NULL || !pDlg->GetSafeHwnd())
	{
		return FALSE;
	}
	if (bShow)
	{
		//m_Timer.SetTimer(0,1000,this);
		::PostMessage(pDlg->m_hWnd, WM_SHOW_SYSTEM_RESOURCE_INFO,0,0);
		if (m_bUpdateShowThreadSuspend == TRUE)
		{
			m_pUpdateShowThread->ResumeThread();
			m_bUpdateShowThreadSuspend = FALSE;
		}
	}
	else
	{
		//m_Timer.KillTimer(0);
		::PostMessage(pDlg->m_hWnd, WM_HIDE_SYSTEM_RESOURCE_INFO,0,0);
		if (m_bUpdateShowThreadSuspend == FALSE)
		{
			m_pUpdateShowThread->SuspendThread();
			m_bUpdateShowThreadSuspend = TRUE;
		}
	}
	return TRUE;	
}

BOOL CSystemStatusInfo::IsSysMemoryUsedRateWarning()
{
	if (m_pMonitorThread==NULL)
	{
		return FALSE;
	}
	if ((m_pOwner->m_SysOptionConfig.m_bThreValid)&&(m_dSysMemoryUsedRate > m_pOwner->m_SysOptionConfig.m_dMaxMemUseThre))
	{
		return TRUE;
	}
	return FALSE;
	
}
BOOL CSystemStatusInfo::IsDiskUsedRateWarning(CString strDriveName)
{
	if (m_pMonitorThread==NULL)
	{
		return FALSE;
	}
	double dDiskPercent =0.0;
	GetDiskUsedRate(strDriveName,dDiskPercent);
	if ((m_pOwner->m_SysOptionConfig.m_bThreValid) && (dDiskPercent>m_pOwner->m_SysOptionConfig.m_dMaxDiskUseThre))
	{
		return TRUE;
	}
	return FALSE;
	
}

BOOL CSystemStatusInfo::IsCpuUsedRateWarning()
{
	if (m_pMonitorThread==NULL)
	{
		return FALSE;
	}
	if ((m_pOwner->m_SysOptionConfig.m_bThreValid)&&(m_dCpuUsedRate > m_pOwner->m_SysOptionConfig.m_dMaxCpuUseThre))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CSystemStatusInfo::IsHandleUsedCountWarning()
{
	if (m_pMonitorThread==NULL)
	{
		return FALSE;
	}
	if ((m_pOwner->m_SysOptionConfig.m_bThreValid)&&(m_nHandleUsedCount > m_pOwner->m_SysOptionConfig.m_nMaxHandleUseThre))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CSystemStatusInfo::IsGDIUsedCountWarning()
{
	if (m_pMonitorThread==NULL)
	{
		return FALSE;
	}
	if ((m_pOwner->m_SysOptionConfig.m_bThreValid)&&(m_nGDIUsedCount > m_pOwner->m_SysOptionConfig.m_nMaxGDIUseThre))
	{
		return TRUE;
	}
	return FALSE;
}

