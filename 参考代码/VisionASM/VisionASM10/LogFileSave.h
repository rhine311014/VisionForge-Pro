#pragma once

#include <vector>

class CLogFileSave
{
public:
	CLogFileSave(CString strFullPath, long lFileSizeMax = 5);
	virtual ~CLogFileSave(void);


	//设置日志文件路径
	void SetLogFilePath(CString strFullPath);

	//设置日志文件最大尺寸，单位MB
	void SetLogFileSizeMax(long lFileSizeMax);

	//设置是否保存日志
	void EnableLogFileSave(BOOL bEnable);

	//写入日志文件
	bool SaveLogText(CString strLogText, int nLayer = 0);
	bool SaveLogTimeAndText(SYSTEMTIME stLogTime, CString strLogText, int nLayer = 0);
	bool SaveLogText(CString strLogHead, CString strLogText, int nLayer = 0);
	bool SaveLogText(CString strLogHead, BOOL bRegType, long nRegAdrr, std::vector<int> vnReceivedData, int nLayer = 0);

	void GetOperationLogInfo(deque<CString>& saOperationLogInfo);	//获得操作记录
	void ClearOperationLogInfo();									//清空操作记录
private:
	CString	m_strLogFilePath;

	long m_lFileSizeMax;

	BOOL m_bEnableLogFileSave;
	deque<CString>		m_saOperationLogInfo;
	CRITICAL_SECTION	m_csResourceOperationLogInfo;		//zzc 操作记录临界区
};

extern CLogFileSave g_opLogFile;
extern CLogFileSave g_opBendLogFile;

