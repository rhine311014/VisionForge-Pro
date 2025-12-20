#pragma once

#include "vsHaspInfo.h"

#define WAIT_CHECK_THREAD_QUIT_TIME						10000
#define WAIT_CHECK_THREAD_SUSPEND_TIME					10000

#define WM_UPDATE_AUTHORIZE								WM_USER + 500
#define WM_UPDATE_AUTHORIZE_VALID						WM_UPDATE_AUTHORIZE + 0			//授权正常
#define WM_UPDATE_AUTHORIZE_READ_ERROR					WM_UPDATE_AUTHORIZE + 1			//读加密锁授权信息失败
#define WM_UPDATE_AUTHORIZE_WRITE_ERROR					WM_UPDATE_AUTHORIZE + 2			//写加密锁授权信息失败
#define WM_UPDATE_AUTHORIZE_LOST_INFO					WM_UPDATE_AUTHORIZE + 3			//授权信息为空
#define WM_UPDATE_AUTHORIZE_FORMAT_ERROR				WM_UPDATE_AUTHORIZE + 4			//授权信息格式错误
#define WM_UPDATE_AUTHORIZE_WARNING						WM_UPDATE_AUTHORIZE + 5			//距离授权日期天数小于设置的警告天数,参数1返回剩余天数
#define WM_UPDATE_AUTHORIZE_EXPIRE						WM_UPDATE_AUTHORIZE + 6			//授权已过期
#define WM_UPDATE_AUTHORIZE_EXCEPTION					WM_UPDATE_AUTHORIZE + 7			//授权信息异常:(1)当前日期小于授权生成日期;(2)当前日期小于上次读锁日期

//授权状态
//授权有效包含两种状态：1）eAuthorizeValid；2）eAuthorizeWarning
enum AuthorizeStatus
{
	eAuthorizeUnkown = -1,			//未知授权状态
	eAuthorizeValid = 0,			//授权有效
	eAuthorizeReadError,			//授权读错误
	eAuthorizeWriteError,		    //授权写错误
	eAuthorizeLostInfo,				//授权信息丢失
	eAuthorizeFormatError,			//授权信息格式错误
	eAuthorizeWarning,				//授权过期警告
	eAuthorizeExpire,				//授权过期
	eAuthorizeException,			//授权信息异常
};

typedef struct tagAuthorizeInfo
{
	AuthorizeStatus m_AuthorizeStatus;//授权状态
	CString m_strAuthorizeInfo;		//读取加密锁授权信息
	CString m_strGenerateDate;		//注册信息生成日期
	CString m_strExpireDate;		//注册信息授权日期
	CString m_strLastRunDateTime;	//软件上次运行日期时间
	CTime	  m_timeCurrent;		//当前时间(授权检测时)
	CTimeSpan m_spanPast;			//注册信息生成到当前时间间隔
	CTimeSpan m_spanExcept;			//软件上次运行到当前时间间隔
	CTimeSpan m_spanRemain;			//当前到授权日期0点的时间间隔

	tagAuthorizeInfo()
	{
		Reset();
	}

	void Reset()
	{
		m_AuthorizeStatus = eAuthorizeUnkown;
		m_strAuthorizeInfo = _T("");
		m_strGenerateDate = _T("");
		m_strExpireDate = _T("");
		m_strLastRunDateTime = _T("");
		m_timeCurrent = 0;
		m_spanPast = 0;
		m_spanExcept = 0;
		m_spanRemain = 0;
	}
}AuthorizeInfo;

class HaspCheck_API vcAuthorizeTool
{
public:
	vcAuthorizeTool();
	~vcAuthorizeTool();

	void SetOwner(CWnd* pWnd);										// 设置消息接收窗口

	void SetWarningDayNumber(int nDayNumber = 3);					// 设置警告天数，设置为0时，则不警告
	void SetCheckCycleTime(int nSecond = 600);						// 设置检测授权的周期时间，默认600秒,10分钟
	void SetUpdateCycleTime(int nSecond = 1800);					// 设置更新授权的周期时间，默认1800秒，30分钟
	void SetAuthorizeAbnormalCountMax(int nMax = 2);				// 设置允许最大连续授权信息读写异常次数

	void EnableAuthorizeCheck();									// 开启授权检测线程（消息接收窗口不能为NULL）
	void DisableAuthorizeCheck();									// 关闭授权检测线程

	BOOL SuspendAuthorizeCheck();									// 挂起授权检测线程
	BOOL ResumeAuthorizeCheck();									// 恢复授权检测线程

	AuthorizeStatus CheckAuthorize(CString& strRegDate);			// 手动单次执行授权检测，正常返回TRUE；否则返回FALSE

	BOOL Register(CString strReg, CString& strRegDate);				// 输入注册信息，更新加密锁注册信息

	BOOL GetUseDongleID(CString& strDongleID);						// 获取加密锁ID

protected:
	BOOL CreatAuthorizeCheckThread();								// 创建监控软件过期线程
	BOOL CloseAuthorizeCheckThread();								// 关闭监控软件过期线程
	static UINT AuthorizeCheckThread(LPVOID pParam);				// 监控软件是否过期线程函数

	BOOL GetLastRunDateTime(CString& strLastRunDateTime);			// 获取加密锁中保存的上次运行日期时间

	BOOL CheckDateTimeIsValid(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond);

	void AuthorizeCheckDelay();
	void PostMessageAuthorizeCheck(UINT Msg, int nDaysRemain = 0);

private:
	CWnd*							m_pWndOwner;					// 父窗口
	int								m_nWarningDayNumber;			// 警告天数
	int								m_nCheckCycleTime;				// 检查授权周期时间,S为单位
	int								m_nUpdateCycleTime;				// 更新授权周期时间,S为单位
	int								m_nAuthorizeAbnormalCount;		// 连续授权信息读写异常次数
	int								m_nAuthorizeAbnormalCountMax;	// 允许最大连续授权信息读写异常次数

	CWinThread*						m_pAuthorizeCheckThread;		// 授权信息线程
	BOOL							m_bExitAuthorizeCheckThread;	// 授权信息线程是否退出的标志
	BOOL							m_bExitAuthorizeCheckThreaded;	// 工作线程是否已经退出
	BOOL							m_bCurThreadWorkFinished;		// 工作线程当前工作是否结束
	BOOL							m_bSuspendAuthorizeCheckThread;	// 工作线程是否挂起
	BOOL							m_bStopNow;						// 是否立即停止当前工作

	AuthorizeInfo					m_AuthorizeInfo;				// 授权过程信息

	CRITICAL_SECTION                m_csDongle;						// 同步信号(访问加密锁)
};