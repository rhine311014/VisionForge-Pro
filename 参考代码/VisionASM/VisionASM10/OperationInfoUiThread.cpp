// OperationInfoUiThread.cpp : 实现文件
//

#include "stdafx.h"
#include "OperationInfoUiThread.h"


// COperationInfoUiThread

IMPLEMENT_DYNCREATE(COperationInfoUiThread, CWinThread)

COperationInfoUiThread::COperationInfoUiThread()
{
	m_pDlgOperationInfo = new CDlgOperationInfo;
}

COperationInfoUiThread::~COperationInfoUiThread()
{
}

BOOL COperationInfoUiThread::InitInstance()
{
	if (m_pDlgOperationInfo != NULL)
	{
		m_pDlgOperationInfo->Create(IDD_DLG_OPERATION_INFO, NULL);
		m_pDlgOperationInfo->CenterWindow();
		m_pDlgOperationInfo->ShowWindow(SW_HIDE);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

int COperationInfoUiThread::ExitInstance()
{
	if (m_pDlgOperationInfo->GetSafeHwnd())
	{
		m_pDlgOperationInfo->ShowWindow(SW_HIDE);
		m_pDlgOperationInfo->DestroyWindow();
	}
	if (m_pDlgOperationInfo!=NULL)
	{
		delete m_pDlgOperationInfo;
		m_pDlgOperationInfo = NULL;
	}
	return CWinThread::ExitInstance();
}

CDlgOperationInfo* COperationInfoUiThread::GetOperationDlgPointer()
{
	if (m_pDlgOperationInfo == NULL)
	{
		return NULL;
	}

	return m_pDlgOperationInfo;
}

BOOL COperationInfoUiThread::SetOperationDlgShown(BOOL boShow)
{
	if (m_pDlgOperationInfo == NULL)
	{
		return FALSE;
	}

	m_pDlgOperationInfo->ShowWindow(boShow);
	return TRUE;
}

CWnd* COperationInfoUiThread::SetParentWndPointer(CWnd* pParent)
{
	return m_pDlgOperationInfo->SetParentWndPointer((CVisionASMDlg*)pParent);
}

void COperationInfoUiThread::SetVisionASMRef(std::vector<vcBaseVisionAlign*> vpVisionASM)
{
	m_pDlgOperationInfo->SetVisionASMRef(vpVisionASM);
}

void COperationInfoUiThread::SetPlatformNum(int nPlatformNum)
{
	m_pDlgOperationInfo->SetPlatformNum(nPlatformNum);
}

void COperationInfoUiThread::InitDlgItem()
{
	m_pDlgOperationInfo->InitDlgItem();
}

void COperationInfoUiThread::SetCurrentWorkPath(CString strFilePath)
{
	m_pDlgOperationInfo->SetCurrentWorkPath(strFilePath);
}

void COperationInfoUiThread::SetOperationLogFilePath(CString strFilePath)
{
	m_pDlgOperationInfo->SetOperationLogFilePath(strFilePath);
}

void COperationInfoUiThread::SetOperationDlgCenterWindow()
{
	m_pDlgOperationInfo->CenterWindow();
}

BEGIN_MESSAGE_MAP(COperationInfoUiThread, CWinThread)
END_MESSAGE_MAP()


// COperationInfoUiThread 消息处理程序
