#pragma once
#include <windows.h>

class  vcMutex
{
public:
	vcMutex()
	{
		::InitializeCriticalSection(&m_cs);
	}
	~vcMutex()
	{
		::DeleteCriticalSection(&m_cs);
	}

	void lock()
	{
		::EnterCriticalSection(&m_cs);
	}

	void unLock()
	{
		::LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
};

class  vcMutexLock
{
public:
	vcMutexLock(vcMutex* pMutex)
	{
		m_pMutex = pMutex;
		m_pMutex->lock();
	}

	~vcMutexLock()
	{
		m_pMutex->unLock();
	}
private:
	vcMutex* m_pMutex;
};