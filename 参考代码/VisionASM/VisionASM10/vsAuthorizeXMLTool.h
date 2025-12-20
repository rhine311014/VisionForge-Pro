#pragma once


//授权状态
//授权有效包含两种状态：1）eAuthorizeXMLValid；2）eAuthorizeXMLWarning
enum AuthorizeXMLStatus
{
	eAuthorizeXMLValid = 0,			//授权有效
	eAuthorizeXMLError,				//授权读错误
	eAuthorizeXMLWarning,			//授权过期警告
	eAuthorizeXMLExpire,			//授权过期
};

class vcAuthorizeXMLTool
{
public:
	vcAuthorizeXMLTool();
	~vcAuthorizeXMLTool();

	void SetOwner(CWnd* pWnd);										// 设置消息接收窗口

	void SetWarningDayNumber(int nDayNumber = 3);					// 设置警告天数，设置为0时，则不警告

	void EnableAuthorizeCheck();									// 开启授权检测线程（消息接收窗口不能为NULL）
	void DisableAuthorizeCheck();									// 关闭授权检测线程

	AuthorizeXMLStatus CheckAuthorize(CString& strRegDate);			// 手动单次执行授权检测，正常返回TRUE；否则返回FALSE

	BOOL Register(CString strRegisterCode, CString strRegID, CString& strRegDate);				// 输入注册信息，更新加密锁注册信息

	BOOL GetUseDongleID(CString& strDongleID);

	BOOL GetSystemRegisterCode(CString& strRegisterCode);

	BOOL GetSystemRegID(CString& strRegID);

	BOOL GetSystemRestDate(CString& strRestDate);

	BOOL GetSystemRestSecond(CString& strRestSecond);

	BOOL GetSystemLastRundate(CString& strLastRunDate);

	BOOL GetExpiredate(CString& strExpiredate);

	CString GetCurrenttime();

public:
	BOOL LoadSystemExpireDate();
	BOOL SaveSystemExpireDate();
	BOOL LoadSystemRegID();
	BOOL SaveSystemRegID();
	BOOL LoadSystemRestDate();
	BOOL SaveSystemRestDate();
	BOOL LoadSystemRestSecond();
	BOOL SaveSystemRestSecond();
	BOOL SaveSystemLastRundate();
	BOOL LoadSystemLastRundate();
	CString m_strSysConfigFilePath;		// 系统配置文件全路径
	CString m_strSysExpireDate;         // 系统过期日期
	CString m_strSysRegID;              // 系统注册ID
	CString m_strRestDate;              // 剩余时间
	CString m_strRestSecond;            // 剩余时间
	CString m_strLastRunDate;           // 最后一次运行日期

	BOOL CreatAuthorizeCheckThread();								// 创建监控软件过期线程
	BOOL CloseAuthorizeCheckThread();								// 关闭监控软件过期线程

protected:
	static UINT AuthorizeCheckThread(LPVOID pParam);				// 监控软件是否过期线程函数

	BOOL CheckDateTimeIsValid(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond);

private:
	CWnd*							m_pWndOwner;					// 父窗口

	CWinThread*						m_pAuthorizeCheckThread;		// 授权信息线程
	BOOL							m_bExitAuthorizeCheckThread;	// 授权信息线程是否退出的标志

	int								m_nWarningDayNumber;			// 警告天数

	CString GetLastDateOfDir();			//获取文件夹下最新日期
};