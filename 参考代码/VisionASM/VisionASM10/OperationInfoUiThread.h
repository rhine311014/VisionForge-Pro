#pragma once
#include "DlgOperationInfo.h"


// COperationInfoUiThread
class CVisionASMDlg;
class COperationInfoUiThread : public CWinThread
{
	DECLARE_DYNCREATE(COperationInfoUiThread)

public:
	COperationInfoUiThread();           // 动态创建所使用的受保护的构造函数
	virtual ~COperationInfoUiThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CDlgOperationInfo*	m_pDlgOperationInfo;
protected:
	DECLARE_MESSAGE_MAP()

public:
	CDlgOperationInfo* GetOperationDlgPointer();
	BOOL	SetOperationDlgShown(BOOL boShow);
	CWnd*	SetParentWndPointer(CWnd* pParent);					//设置父窗口指针
	void	SetVisionASMRef(std::vector<vcBaseVisionAlign*> vpVisionASM);	//获取视觉指针
	void	SetPlatformNum(int nPlatformNum);								//设置平台数量
	void	InitDlgItem();													//初始化窗口控件
	void	SetCurrentWorkPath(CString strFilePath);						//设置报警文件路径
	void	SetOperationLogFilePath(CString strFilePath);					//设置操作日志文件路径
	void	SetOperationDlgCenterWindow();									//设置窗口居中
};


