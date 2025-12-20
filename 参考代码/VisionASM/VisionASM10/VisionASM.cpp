// VisionASM.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VisionASM.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
//#include "HaspInfo.h"
#include "HaspLoginCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <io.h>  
#include <fcntl.h>

#include <DbgHelp.h>  
#pragma comment(lib,"DbgHelp.lib")

void CreateDumpFile(CString lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  
{  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
	// Dump信息
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
	dumpInfo.ExceptionPointers = pException;  
	dumpInfo.ThreadId = GetCurrentThreadId();  
	dumpInfo.ClientPointers = TRUE;  
	// 写入dump文件
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  
	CloseHandle(hDumpFile);  
}  

// 处理Unhandled Excepiton 的回调函数
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)  
{   
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString strDmpName;
	strDmpName.Format("%02d%02d_%02d%02d%02d.dmp",time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	CreateDumpFile(strDmpName,pException);  
	return EXCEPTION_EXECUTE_HANDLER;  
}
void InitConsoleWindow()
{
	AllocConsole();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle,_O_TEXT);
	FILE * hf = _fdopen( hCrt, "w" );
	*stdout = *hf;
}

/////////////////////////////////////////////////////////////////////////////
// CVisionASMApp

BEGIN_MESSAGE_MAP(CVisionASMApp, CWinApp)
	//{{AFX_MSG_MAP(CVisionASMApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionASMApp construction

CVisionASMApp::CVisionASMApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

// 	for (long i=0; i<20; i++)	//最多延时10S
// 	{
// 		string stringID;
// 		if (FindDongleID(stringID) == 0)
// 		{
// 			Sleep(5000);
// 			break;
// 		}
// 
// 		Sleep(500);
// 	}

	for (long i=0; i<20; i++)	//最多延时10S
	{
		// 查询加密锁是否存在
		if (CheckDongle())
		{
			break;
		}

		Sleep(500);
	}
	
	
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

	/*void * addr = (void*)GetProcAddress(LoadLibraryA("kernel32.dll"),"SetUnhandledExceptionFilter");
	if (addr)
	{    
		unsigned char code[16];
		int size = 0;
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD oldFlag, tmpFlag;
		VirtualProtect(addr, size, PAGE_READWRITE, &oldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, oldFlag, &tmpFlag);
	}*/

//	InitConsoleWindow();
	// 判断该软件是否授权
	CString strExeDir;
	GetModuleFileName(NULL, strExeDir.GetBuffer(MAX_PATH), MAX_PATH);
	strExeDir.ReleaseBuffer();
	strExeDir = strExeDir.Left(strExeDir.ReverseFind('\\'));

	CreateDirectoryPro(strExeDir + _T("\\VSConfig"));
	
	// 禁止软件在C盘中启动，避免磁盘包含打开时，且本地存图时，导致整体内存占用累计递增崩溃bug
	if(strExeDir.Left(2)==_T("C:"))
	{
		CString strInfo = _T("禁止C盘内启动程序,请将软件目录改为D盘！");
		AfxMessageBox(strInfo,MB_SYSTEMMODAL);
		exit(0);
	}

	HINSTANCE hInstance = NULL;

	BOOL bAuthorized = FALSE;
	CSystemConfigure sysConfig;
//	sysConfig.LoadSystemConfigInfo
	if (0)
	{
		int nPlatformNum = sysConfig.GetPlatformNum();
		if (nPlatformNum == 1)
		{	
			// vsa1000       
			lpFunc Func1;
			hInstance = LoadLibrary(strExeDir + _T("\\vsa1000.dll"));		
			if (NULL != hInstance)
			{
				Func1 = (lpFunc) GetProcAddress(hInstance, "fnvsa1000");
				if (Func1 != NULL)
				{
					if (Func1() == vas1000)
					{
						bAuthorized = TRUE;
					}
				}
			}

		}
		else
		{
			// vsa2000       
			lpFunc Func1;
			hInstance = LoadLibrary(strExeDir + _T("\\vsa2000.dll"));		
			if (NULL != hInstance)
			{
				Func1 = (lpFunc) GetProcAddress(hInstance, "fnvsa2000");
				if (Func1 != NULL)
				{
					if (Func1() == vas2000)
					{
						bAuthorized = TRUE;
					}
				}
			}
		}

		if (!bAuthorized)
		{
			if (NULL != hInstance)
			{
				FreeLibrary(hInstance);
				hInstance = NULL;    
			}   

			AfxMessageBox(_T("License file not found or Authorization failed!"),MB_SYSTEMMODAL);
			exit(ERROR_INVALID_ACCESS);
		}
		else
		{
			if (NULL != hInstance)
			{
				FreeLibrary(hInstance);
				hInstance = NULL;    
			}   
		}
	}


	
// 	CSystemConfigure sysConfig;
// 	sysConfig.LoadSystemConfigInfo();
// 	int nPlatformNum = sysConfig.GetPlatformNum();
// 	if (nPlatformNum == 1)
// 	{
// 		SysPlatformInfo platformInfo;
// 		sysConfig.GetSysPlatformInfo(0, platformInfo);
// 		if ((platformInfo.m_nCamNum > 0) && (platformInfo.m_nCamNum <= 2))
// 		{
// 			// VA3000/3100            
// 			hInstance = LoadLibrary(strExeDir + _T("\\VA3000.dll"));			
// 		}
// 		else if ((platformInfo.m_nCamNum >= 3) && (platformInfo.m_nCamNum <= 6))
// 		{
// 			// VA4000/4100
// 			hInstance = LoadLibrary(strExeDir + _T("\\VA4000.dll"));			
// 		}
// 	}
// 	else if ((nPlatformNum >= 2) && (nPlatformNum <= 4))
// 	{
// 		// VA5000/5100
//        	hInstance = LoadLibrary(strExeDir + _T("\\VA5000.dll"));
// 	}
// 	
// 	if (NULL == hInstance)
// 	{
// 		AfxMessageBox(_T("License file not found or Authorization failed!")); 
// 		exit(ERROR_INVALID_ACCESS);
// 	}
// 	else
// 	{
//         FreeLibrary(hInstance);
// 		hInstance = NULL;       
// 	}
// 

	// 判断是否已有实例在运行
	m_hMutexAppRunning = CreateMutex(NULL, true, "Vision Already Running");   

	if (GetLastError() == ERROR_ALREADY_EXISTS)   
	{  
// 		CSystemOptionConfig sysOptionConfig;
// 		sysOptionConfig.LoadSystemOption();
// 		
// 		TCHAR aModuleFileName[MAX_PATH];
// 		GetModuleFileName(NULL, aModuleFileName, MAX_PATH);
// 		CString strModuleFileName(aModuleFileName);
// 		int nIndex = strModuleFileName.ReverseFind('\\');
// 		CString strCurWorkDir = strModuleFileName.Left(nIndex);
// 		
// 		CString strLanguageFilePath;
// 		scXMLConfigurator xml;
// 		
// 		switch (sysOptionConfig.m_lgidLanguage)
// 		{
// 		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
// 			strLanguageFilePath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
// 			if (!xml.Load(strLanguageFilePath))
// 			{
// 				AfxMessageBox(_T("System file lost or damaged!\n") + strLanguageFilePath);
// 				exit(ERROR_FILE_NOT_FOUND);
// 			}
// 			break;
// 		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
// 			strLanguageFilePath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
// 			if (!xml.Load(strLanguageFilePath))
// 			{
// 				AfxMessageBox(_T("系統文件缺失或損壞！\n") + strLanguageFilePath);
// 				exit(ERROR_FILE_NOT_FOUND);
// 			}
// 			break;
// 		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
// 		default:
// 			strLanguageFilePath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
// 			if (!xml.Load(strLanguageFilePath))
// 			{
// 				AfxMessageBox(_T("系统文件缺失或损坏！\n") + strLanguageFilePath);
// 				exit(ERROR_FILE_NOT_FOUND);
// 			}
// 			break;
// 		}
//		
//		CString strInfo = _T("另一个实例已经在运行！");
// 		xml.ResetDocElemPos();
// 		xml.FindElem(_T("ResourceBundle"));
// 		xml.EnterElem();
// 		xml.FindElem(_T("StringTable"));
// 		if(_T("StringTable") == xml.GetAttribute(_T("Key")))
// 		{
// 			xml.EnterElem();
// 			int nID = 0;
// 			while(xml.FindElem())
// 			{
// 				xml.GetAttribute(_T("Id"), nID);
// 				if (nID == IDS_MB_ANOTHER_PROCESS_IS_RUNNING)
// 				{
// 					strInfo = xml.GetAttribute(_T("Value"));
// 					break;
// 				}
// 			}
// 		}
//
//		AfxMessageBox(strInfo);

		CString strInfo = _T("另一个实例已经在运行！");
		AfxMessageBox(strInfo,MB_SYSTEMMODAL);
		exit(ERROR_ALREADY_EXISTS);
	}
}

CVisionASMApp::~CVisionASMApp()
{
	if (m_hMutexAppRunning != NULL)
	{
		ReleaseMutex(m_hMutexAppRunning);   
		CloseHandle(m_hMutexAppRunning);
	}
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CVisionASMApp object

CVisionASMApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVisionASMApp initialization

BOOL CVisionASMApp::InitInstance()
{
// 	CCommandLineInfo cmdInfo; 
//     ParseCommandLine(cmdInfo); 
// 	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash); 
// 	
// 	// 设置启动界面BMP图像
// 	CSystemConfigure sysConfig;
// 	sysConfig.LoadSystemConfigInfo();

	CSystemOptionConfig sysOptionConfig;		// 系统选项信息
	if (!sysOptionConfig.LoadSystemOption())
	{
		//	m_SysOptionConfig.m_bSysKeyboardEnabled = FALSE;
		sysOptionConfig.m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
		sysOptionConfig.SaveSystemOption();
	}
	
// 	int nPlatformNum = sysConfig.GetPlatformNum();
// 	if (nPlatformNum == 1)
// 	{
// 		SysPlatformInfo platformInfo;
// 		sysConfig.GetSysPlatformInfo(0, platformInfo);
// 		if ((platformInfo.m_nCamNum > 0) && (platformInfo.m_nCamNum <= 2))
// 		{
// 			CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT);
// 		}
// 		else if ((platformInfo.m_nCamNum >= 3) && (platformInfo.m_nCamNum <= 6))
// 		{
// 			CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT);
// 		}
// 		else
// 		{
// 			CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT);
// 		}
// 	}
// 	else if ((nPlatformNum >= 2) && (nPlatformNum <= 4))
// 	{
// 		CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT);
// 	}
// 	else
// 	{
// 		CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT);
// 	}
// 	// 	// 开启启动界面
	// 	CSplashWnd::ShowSplashScreen();	
	
	
	// 设置启动界面BMP图像
	//CCommandLineInfo cmdInfo; 
 //   ParseCommandLine(cmdInfo); 
	//CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	//switch (sysOptionConfig.m_lgidLanguage)
	//{
	//case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
	//	CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT_ENG);
	//	break;
	//case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
	//	CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT);
	//	break;
	//case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	//default:
	//    CSplashWnd::SetSplashBMP(IDB_SPLASH_VISION_INIT);
	//	break;
	//}
	//
	//// 开启启动界面
	//CSplashWnd::ShowSplashScreen();

 	AfxEnableControlContainer();


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CVisionASMDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CVisionASMApp::PreTranslateMessage(MSG* pMsg) 
{
// 	if (CSplashWnd::PreTranslateAppMessage(pMsg))
// 	return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}
