#include "StdAfx.h"
#include "LogFileSave.h"
#include "vsVisionAlignDef.h"

CLogFileSave g_opLogFile(GetLogDirecotryPath() + _T("\\OperationLog.txt")); //?
CLogFileSave g_opBendLogFile(GetLogDirecotryPath() + _T("\\BendOperationLog.txt")); //折弯参数操作日志

#define MAXOPERATIONLOGCOUNT	200
CLogFileSave::CLogFileSave(CString strFullPath, long lFileSizeMax)
{
	m_strLogFilePath = strFullPath;
	m_lFileSizeMax = lFileSizeMax;
	m_bEnableLogFileSave = FALSE;
	InitializeCriticalSection(&m_csResourceOperationLogInfo);
}


CLogFileSave::~CLogFileSave(void)
{
	DeleteCriticalSection(&m_csResourceOperationLogInfo);
}


void CLogFileSave::SetLogFilePath(CString strFullPath)
{
	m_strLogFilePath = strFullPath;
}

void CLogFileSave::SetLogFileSizeMax(long lFileSizeMax)
{
	m_lFileSizeMax = lFileSizeMax;
}

void CLogFileSave::EnableLogFileSave(BOOL bEnable)
{
	m_bEnableLogFileSave = bEnable;
}

bool CLogFileSave::SaveLogText(CString strLogText, int nLayer)
{
	//if (!m_bEnableLogFileSave)
	//{
	//	return false;
	//}
	//FILE *stream;
	CString strLogTime;
	SYSTEMTIME stLogTime; 
	GetLocalTime(&stLogTime);
	strLogTime.Format(_T("%02d/%02d %02d:%02d:%02d:%03d"), stLogTime.wMonth,stLogTime.wDay,stLogTime.wHour,stLogTime.wMinute,stLogTime.wSecond,stLogTime.wMilliseconds);
	CString strSplitChar = _T("-");
	EnterCriticalSection(&m_csResourceOperationLogInfo);
	if (m_saOperationLogInfo.size() > MAXOPERATIONLOGCOUNT)
	{
		m_saOperationLogInfo.pop_front();
	}
	if (nLayer > 0)
	{
		CString strLogType = _T("操作");
		m_saOperationLogInfo.push_back(strLogTime + strSplitChar + strLogType + strSplitChar + strLogText);
	}
	else
	{
		CString strLogType = _T("系统");
		m_saOperationLogInfo.push_back(strLogTime + strSplitChar + strLogType + strSplitChar + strLogText);
	}
	LeaveCriticalSection(&m_csResourceOperationLogInfo);
	strLogTime.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d:%03d]："),
		stLogTime.wYear,stLogTime.wMonth,stLogTime.wDay,stLogTime.wHour,stLogTime.wMinute,stLogTime.wSecond,stLogTime.wMilliseconds);

	for (long i=0; i<nLayer; i++)
	{
		strLogText = _T("....")+strLogText;
	}

	strLogText = strLogTime + strLogText + _T("\r\n");

	CFileStatus statusFile;
	CFile::GetStatus(m_strLogFilePath,statusFile);
	if (statusFile.m_size > m_lFileSizeMax*1024*1024)
	{
		::DeleteFile(m_strLogFilePath);
	}

	HANDLE  hFile = CreateFile(
		m_strLogFilePath,			//创建或打开的文件或设备的名称
		GENERIC_WRITE,				//文件访问权限,写
		0,							//共享模式,这里设置0防止其他进程打开文件或设备
		NULL,						//SECURITY_ATTRIBUTES结构，安全描述，NULL代表默认安全级别
		OPEN_ALWAYS,				//对于存在或不存在的设置执行的操作，始终创建
		FILE_FLAG_WRITE_THROUGH,	//设置文件的属性，里面有高速缓存的选项
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	SetFilePointer(hFile,0,0,FILE_END);

	DWORD dwNumOfWrite = 0;
	WriteFile(hFile, strLogText.GetBuffer(), strLogText.GetLength()*sizeof(char), &dwNumOfWrite, NULL);


	CloseHandle(hFile);

	return true;
}
void CLogFileSave::GetOperationLogInfo(deque<CString>& saOperationLogInfo)
{
	EnterCriticalSection(&m_csResourceOperationLogInfo);
	saOperationLogInfo = m_saOperationLogInfo;
	LeaveCriticalSection(&m_csResourceOperationLogInfo);
}
void CLogFileSave::ClearOperationLogInfo()
{
	EnterCriticalSection(&m_csResourceOperationLogInfo);
	m_saOperationLogInfo.clear();
	LeaveCriticalSection(&m_csResourceOperationLogInfo);
}

bool CLogFileSave::SaveLogTimeAndText(SYSTEMTIME stLogTime, CString strLogText, int nLayer)
{
	if (!m_bEnableLogFileSave)
	{
		return false;
	}

	CString strLogTime;
	strLogTime.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d:%03d]："),
		stLogTime.wYear,stLogTime.wMonth,stLogTime.wDay,stLogTime.wHour,stLogTime.wMinute,stLogTime.wSecond,stLogTime.wMilliseconds);

	for (long i=0; i<nLayer; i++)
	{
		strLogText = _T("...")+strLogText;
	}

	strLogText = strLogTime + strLogText + _T("\r\n");

	CFileStatus statusFile;
	CFile::GetStatus(m_strLogFilePath,statusFile);
	if (statusFile.m_size > m_lFileSizeMax*1024*1024)
	{
		::DeleteFile(m_strLogFilePath);
	}

	HANDLE  hFile = CreateFile(
		m_strLogFilePath,			//创建或打开的文件或设备的名称
		GENERIC_WRITE,				//文件访问权限,写
		0,							//共享模式,这里设置0防止其他进程打开文件或设备
		NULL,						//SECURITY_ATTRIBUTES结构，安全描述，NULL代表默认安全级别
		OPEN_ALWAYS,				//对于存在或不存在的设置执行的操作，始终创建
		FILE_FLAG_WRITE_THROUGH,	//设置文件的属性，里面有高速缓存的选项
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	SetFilePointer(hFile,0,0,FILE_END);

	DWORD dwNumOfWrite = 0;
	WriteFile(hFile, strLogText.GetBuffer(), strLogText.GetLength()*sizeof(char), &dwNumOfWrite, NULL);

	CloseHandle(hFile);

	return true;
}

bool CLogFileSave::SaveLogText(CString strLogHead, CString strLogText, int nLayer)
{
	if (!m_bEnableLogFileSave)
	{
		return false;
	}

	strLogText += strLogHead;
	return SaveLogText(strLogText);
}

bool CLogFileSave::SaveLogText(CString strLogHead, BOOL bRegType, long nRegAdrr, std::vector<int> vnReceivedData, int nLayer)
{
	if (!m_bEnableLogFileSave)
	{
		return false;
	}

	CString strLogText = strLogHead;

	long nRegAdrrStep = 2;
	if (bRegType)
	{
		nRegAdrrStep = 2;
	}
	else
	{
		nRegAdrrStep = 1;
	}

	for (long i=0; i<vnReceivedData.size(); i++)
	{
		CString strData;
		strData.Format(_T(" Addr%d:%d "), nRegAdrr+i*nRegAdrrStep, vnReceivedData[i]);
		strLogText += strData;
	}

	return SaveLogText(strLogText);
}