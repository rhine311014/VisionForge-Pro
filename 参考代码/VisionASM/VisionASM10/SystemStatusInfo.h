#pragma once
#include <vector>
#include "DlgSysStatusInfo.h"

//class CSystemStatusInfo;
//
//class cTimer
//{
//public:
//	typedef CMap<UINT,UINT,CSystemStatusInfo*,CSystemStatusInfo*> CTimerMap;
//	typedef CMap<UINT,UINT,UINT,UINT> CTimerIdEventMap;
//public:
//	cTimer();
//	virtual ~cTimer();
//
//	void SetTimer(UINT_PTR nIDEvent,UINT nElapse,CSystemStatusInfo* pOwner);//设置定时器，nElapse表示时间间隔，sz表示要提示的内容
//
//	void KillTimer(UINT_PTR nIDEvent);//销毁该实例的定时器
//
//	static CTimerMap m_sTimeMap;//静态数据成员，映射表类，用于保存所有的定时器信息
//
//	static CTimerIdEventMap m_sTimeIdEventMap;
//
//	static void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);//静态成员函数，用于处理定时器的消息
//
//};

class SystemStatusInfoUiThread :public CWinThread
{
public:
	SystemStatusInfoUiThread(void);
	virtual ~SystemStatusInfoUiThread(void);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CDialogEx* GetMainFrameDlg()const{ return m_pDlg;}
	void SetParentWndPos(CRect rc);
	void SetOwner(LPVOID pOnwer);
private:	
	CDlgSysStatusInfo* m_pDlg;
	CRect m_ParentWndPos;
};

class CVisionASMDlg;
class CSystemStatusInfo
{
public:
	CSystemStatusInfo(void);
	virtual ~CSystemStatusInfo(void);
public:
	
	std::vector<CString>& GetDiskName(){return m_vStrDriveName;}
	BOOL GetDiskUsedRate(CString strDriveName,double& usedRate)const;
	double GetSysMemoryUsedRate(){return m_dSysMemoryUsedRate;}
	double GetProcessMemoryUsedAmount_M()const{return m_dProcessMemoryUsedAmount_M;}
	double GetCpuUsedRate()const{return m_dCpuUsedRate;}
	int GetHandleUsedCount()const{return m_nHandleUsedCount;}
	int GetGDIUsedCount()const{return m_nGDIUsedCount;}

	BOOL BeginMonitor(LPVOID pOwner,CRect parentWndPos);
	BOOL SuspendMonitor();
	BOOL ResumeMonitor();
	BOOL StopMonitor();
	BOOL ShowUIWindow(BOOL bShow);
	CVisionASMDlg* m_pOwner;
	
	BOOL CacuDriveName();//获取系统各磁盘名
	BOOL CacuDriveUsedInfo();//计算各磁盘使用率
	BOOL CacuMemoryUsedInfo();//计算系统内存使用率和进程内存使用量
	BOOL CacuCpuUsedInfo();//计算CPU使用率
	BOOL CacuHandleUsedCount();//计算进程使用句柄数
	BOOL CacuGDIUsedCount();//计算进程GDI使用数

	void PostMessageUpdateSystemStatusInfo();//发送消息给UI线程刷新界面

	BOOL IsSysMemoryUsedRateWarning();
	BOOL IsDiskUsedRateWarning(CString strDriveName);
	BOOL IsCpuUsedRateWarning();
	BOOL IsHandleUsedCountWarning();
	BOOL IsGDIUsedCountWarning();
protected:
	
	DWORD64 DiffFileTime(FILETIME timePre, FILETIME timeNow);//计算间隔内CPU时间
	
	static UINT MonitorFunc(LPVOID pParam);//线程函数
	static UINT UpdateShowFunc(LPVOID pParam);//显示刷新函数
	
private:
	
	CWinThread* m_pMonitorThread;//统计线程
	CWinThread* m_pUiThread;//显示UI线程
	BOOL m_bExitMonitorThread;//是否结束统计线程

	HANDLE m_hEvent;
	BOOL m_bMoniterThreadSuspend;//统计线程是否暂停

	CWinThread* m_pUpdateShowThread;//显示线程
	BOOL m_bUpdateShowThreadSuspend;//显示线程是否暂停
	BOOL m_bExitUpdateShowThread;//是否结束显示线程

private:
	std::vector<CString> m_vStrDriveName;
	std::vector<double> m_vdDriveUsedRate;
	double m_dSysMemoryUsedRate;
	double m_dProcessMemoryUsedAmount_M;
	double m_dCpuUsedRate;
	int m_nHandleUsedCount;
	int m_nGDIUsedCount;
	/*cTimer m_Timer;*/
	
};

