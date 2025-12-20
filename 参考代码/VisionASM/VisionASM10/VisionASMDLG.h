// VisionASMDlg.h : header file
//

#if !defined(AFX_VisionASMDLG_H__B504C75E_E5C2_4AB6_8531_DFEC30B5A7F7__INCLUDED_)
#define AFX_VisionASMDLG_H__B504C75E_E5C2_4AB6_8531_DFEC30B5A7F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SystemOptionConfig.h"
#include "SystemConfigure.h"
#include "svGuiDisplay.h"
#include "svGuiRect.h"
#include "KeyBoard.h"
#include "Splash.h"
#include "SplashScreenEx.h"
#include "Label.h"
#include "SystemCompanyInfo.h"
#include "vsAuthorizeTool.h"
#include "LogFileSave.h"
#include "SystemStatusInfo.h"
#include "HorizontalLayout.h"
#include "VerticalLayout.h"
#include "afxwin.h"

//#include "vsBaseVisionAlign.h"
// #include "DlgVisionView.h"
//#include "HaspInfo.h"


typedef std::vector<scGuiDisplay*> VpGuiDisplay;
typedef std::vector<scGuiRect*>	VpGuiRect;



#define MaxPlatformNum	6	// 最大的平台数量

// 主界面的工位显示模式
typedef enum enumWSShowMode
{
	esmMode0 = 0,		// 单工位2位置、单工位6位置、单工位8位置的显示模式（图像显示+信息显示，上下结构）
	esmMode1 = 1,		// 单工位4位置、双工位4位置（图像显示+信息显示，左右结构）
	esmMode2,			// 双工位6位置、三工位、四工位（全工位图像显示，不显示信息；单工位图像显示+信息显示（上下结构））
	esmMode3			// 其他
}WSShowMode;


#define WM_CHANGEPLATFORM								WM_USER + 900

/////////////////////////////////////////////////////////////////////////////
// CVisionASMDlg dialog
class CDlgOperationInfo;	//zzc运行信息
class COperationInfoUiThread;	//zzc线程窗口
class CDlgSysEnvironSetting;
class CDlgSysKind;
class CDlgSysOptionSetting;
class CDlgVisionView;
class CDlgVisionView2;
class CDlgVisionRun;

class CSuspendResumeWorkThreadObject
{
public:
	CSuspendResumeWorkThreadObject(){m_pOwner =NULL;m_nPlatformIndex = -1; };
	~CSuspendResumeWorkThreadObject(){};
public:
	void*	m_pOwner;				// 调用对象
	int		m_nPlatformIndex;		// 用于平台的序号
};
// class CSystemConfigure;
class vcBaseVisionAlign;
class CVisionASMDlg : public CDialog
{
// Construction
public:
	CVisionASMDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVisionASMDlg)
	enum { IDD = IDD_VISIONASM_DIALOG2 };

protected:
	void InitLayoutUI();			// 初始化界面
	CLayout* InitTitleLayout();		// 初始化标题栏
	CLayout* InitShowViewLayout();	// 初始化显示区域
	CLayout* InitFuncBtnLayout();	// 初始化功能按钮区
protected:
	void InitUIStyle();				// 初始化界面风格
	void InitTitleStyle();			// 初始化标题栏显示风格
	void InitFunBtnStyle();			// 初始化功能按钮显示风格
protected:
	void InitUIData();              // 初始化界面显示数据
	void InitTitleData();			// 初始化标题栏显示数据
	void InitFunBtnData();			// 初始化功能按钮显示数据
protected:
	void InitRunWindow();           // 初始化运行窗口
	void InitViewWindow();          // 初始化视图窗口
	void InitViewLayout();          // 初始化视图布局
	void InitSingleViewLayout();    // 初始化单个工位时布局
	void InitViewLayoutPlatformNum1(); // 平台数为1的时候, 视图布局
	void InitViewLayoutPlatformNum2(); // 平台数为2的时候, 视图布局
	void InitViewLayoutPlatformNum3(); // 平台数为3的时候, 视图布局
	void InitViewLayoutPlatformNum4(); // 平台数为4的时候, 视图布局
	void InitViewLayoutPlatformNum5(); // 平台数为5的时候, 视图布局
	void InitViewLayoutPlatformNum6(); // 平台数为6的时候, 视图布局
public:
	void UpdateViewLayoutShow(int nViewDisplayId);   // 更新工位切换视图窗口位置显示
	void UpdateAllViewLayout();                      // 更新所有工位布局显示
	void UpdateViewDisplayLayout(int nPlatformId);	  // 更新View视图种Display布局

// 主界面布局
private:
	CLayout*	m_pMainLayout;		// 主窗口布局			 
	CLayout*    m_pBtnArea;			// 功能按钮区域
// 视图布局
private:
	int  m_nViewShowMode;                        // 工位布局类型
	std::vector<CLayout*> m_vpViewLayout;        // 多个工位同时显示, 不同类型的布局				 
	std::vector<CLayout*> m_vpSingleViewLayout;  // 单个工位显示布局
	CLayout*    m_pCurViewLayout;                // 当前界面显示的布局
private:
	CFont		m_cfWSButton;       // 工位选择按钮字体
	CFont       m_cfFunButton;      // 功能按钮字体

public:

	int	m_nPlatformDispIdx;					    // 当前显示的工位索引
	CXPStyleButtonST m_btnRun;					// 运行按钮
	CXPStyleButtonST m_btnLogin;				// 登录按钮
	CXPStyleButtonST m_btnEnvironmentSet;		// 系统设置
	CXPStyleButtonST m_btnProductManage;		// 产品管理
	CXPStyleButtonST m_btnOptionSet;			// 选项
	CXPStyleButtonST m_btnExit;					// 退出
	CXPStyleButtonST m_btnShutDownWindows;		// 关机

	CXPStyleButtonST m_btnShowPlatformAll;		// 显示全部工位
	CXPStyleButtonST m_btnShowPlatform1;		// 显示工位1
	CXPStyleButtonST m_btnShowPlatform2;		// 显示工位2
	CXPStyleButtonST m_btnShowPlatform3;		// 显示工位3
	CXPStyleButtonST m_btnShowPlatform4;		// 显示工位4
	CXPStyleButtonST m_btnShowPlatform5;		// 显示工位5
	CXPStyleButtonST m_btnShowPlatform6;		// 显示工位6

	CXPStyleButtonST m_btnSetDisPlayMode;		// 帧有效、采集图像切换

	CLabel		m_labelTitleBar;				// 标题栏
	CLabel		m_labelTitle;					// 显示标题
	CStatic		m_labelLogo;					// logo图标

//	CXPStyleButtonST m_btnCalibrationSet;		// 标定设置
//	CXPStyleButtonST m_btnRunLamp;				// 暂时不用
//	CXPStyleButtonST m_btnCommSet;				// 暂时不用
//	CXPStyleButtonST m_btnShowCtrlPannel;		// 暂时不用
//	CXPStyleButtonST	m_btnPanel;				// 暂时不用
// 	CTabCtrl	m_tabSheet0;					// 暂时不用
// 	CTabCtrl	m_tabSheet1;					// 暂时不用
// 	CTabCtrl	m_tabSheet2;					// 暂时不用
// 	CTabCtrl	m_tabSheet3;					// 暂时不用
// 	CTabCtrl	m_tabSheet4;					// 暂时不用
// 	CTabCtrl	m_tabSheet5;					// 暂时不用

	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionASMDlg)
public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVisionASMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnUserLogin();
	afx_msg void OnBtnEnvironmentSet();
	afx_msg void OnBtnCalibrationSet();
	afx_msg void OnBtnProductManage();
	afx_msg void OnBtnCommunicationSet();
	afx_msg void OnBtnOptionSet();
	afx_msg void OnRadioShowPlatform1();
	afx_msg void OnRadioShowPlatform2();
	afx_msg void OnRadioShowPlatform3();
	afx_msg void OnRadioShowPlatform4();
	afx_msg void OnRadioShowPlatform5();
	afx_msg void OnRadioShowPlatform6();
	afx_msg void OnRadioShowPlatformAll();
	afx_msg void OnBtnAlignerRun();
	virtual void OnCancel();
	afx_msg void OnBtnShutDownWindows();
	afx_msg void OnBtnShowCtrlPannel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


// 多线程停止/运行
public:
	std::vector<BOOL>			m_vbStartSuspendResume;		  // 程序运行状态
	std::vector<CWinThread*>    m_vpSuspendResumeWorkThread;  // 线程对象
	std::vector<BOOL>           m_vbExitSuspendResumeThread;  // 线程运行状态
	std::vector<CSuspendResumeWorkThreadObject*> m_vpSuspendResumeWorkThreadObject;	// 线程参数对象
	static UINT VisionSuspendResumeWorkThread(LPVOID lpParam); // 线程执行函数

public:
	std::vector<vcBaseVisionAlign*> m_vpVisionASM;	// 视觉对位工具
	CRITICAL_SECTION    criticalUpload;			   //上传临界区

	std::vector<DWORD>	m_vdwInitErrCode;		// 视觉对位工具初始化错误码
	CSystemOptionConfig m_SysOptionConfig;		// 系统选项信息
	CSystemConfigure	m_SystemConfigure;		// 系统配置信息
	CSystemCompanyInfo	m_SystemCompanyInfo;
	int					m_nPlatformNum;			// 平台个数
	int					m_anIDPlatformIcon[4];
	CFont				m_font;
	HICON				m_hIconLampRunning;		// 运行时状态图标
	HICON				m_hIconLampStopped;		// 停止时状态图标
	HBITMAP				m_hIconLogo;
	CString				m_strCurWorkDir;		// 应用程序所在目录
	CDlgVisionView2*	m_pDlgVisionView;		// 对位视图
	CDlgVisionRun*		m_pDlgVisionRun;
	std::vector<CDlgVisionView2*> m_vpNotShareVisionView; // 非共享视图
	BOOL				m_bAuthorized;			// 已授权
	int					m_nUsePassWordMode;		//根据输入的密码确定使用的权限。0超级权限、1一般权限
	CString				m_strTitleShow;			// 软件Title显示
	// 系统提示信息（其中第一个元素为语言种类(LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0))）
	CStringArray*		m_psaSysInfoStrings;
	BOOL				m_bIsRunning;			// 系统是否正在运行
	int					m_nPrevDispPltfrmIdx;	// 界面显示的前一个平台号
	CThemeHelperST		m_themeHelper;			// 主题助手，CXPStyleButtonST使用
	CRect				m_rcViewRect;
	CDlgSysEnvironSetting *m_pDlgEnvironSetting;
	CDlgSysOptionSetting  *m_pDlgOptionSetting;
	CDlgSysKind			*m_pDlgSysKind;
	COperationInfoUiThread*	m_pOperationInfoUiThread;		//zzc线程创建报警
	std::vector<int>	m_vnVisionViewPos;
	BOOL				m_bShowOtherView;
	WSShowMode			m_eWSShowMode;
	CRITICAL_SECTION    m_CSLanguage;
	CSystemStatusInfo	m_SystemStatusInfo;

	CSplashScreenEx		*m_pSplash;

// 	CStatusBarCtrl*		m_pStatusBar;			// 状态栏
// 	BOOL				m_bStatusBarFlag;		// 状态标识
// 	HICON				m_hIconSucceed;			// 成功图标
// 	HICON				m_hIconFailed;			// 失败图标
// 	CString				m_strStatusBarInfo1;	// 状态信息1
// 	CString				m_strStatusBarInfo2;	// 状态信息2
//	std::vector<CTabCtrl*>	m_vTabSheet;

public:
	void EnumCamera();             // 枚举相机
	HWND GetOperationDlgHwnd();
	// 重新选择语言时，更新各提示信息的内容
	void UpdateLanguageResource();
	// 显示相机序列号配置对话框，重新设置相机的序列号
	BOOL CameraSerialNumConfig(std::vector<DWORD>* pvdwInitErrCode); 
// 	// 更新状态栏信息
// 	void UpdateStatusBar();
	// 根据错误码m_vdwInitErrCode（初始值由InitVisionASM()返回），在状态栏上显示当前初始化错误提示信息 
	void UpdateInitErrInfoDisplay();			
	void UpdateGetSystemResourceInfo();
	void InitGetSystemResourceInfo();
	void ExitGetSystemResourceInfo();

	void InitCameraSerialSetting();		// 初始化相机序列号配置
	BOOL InitSuspendResumeWorkThread(); // 初始化挂起、唤醒工作线程
	void InitPlatformCameraShareInfo(); // 初始化工位相机共享信息
	void InitPlatformVisionASM();		// 初始化工位视觉对位工具
	void InitOperationInfoWindow();		// 初始化操作窗口
	void InitPlatformVisionASMGui();	// 设置VisionASMGUI
	void InitAuthorize();				// 初始化授权信息
	void CheckProductDataCalibDataConsistency();	// 工位产品信息和标定信息是否一致检查
	
	// 设置按钮可用状态
	void EnableDlgItem(BOOL bEnable);

	// 单平台产品管理
	void ExecuteSinglePlatformProductManage(int nPlatformIndex);
	void SinglePlatformProductManage(int nPlatformIndex);
	void UpdateSinglePlatformInfo(int nPlatformIndex);
	int m_nSinglePlatformIndex;

	// 授权管理
	vcAuthorizeTool m_AuthorizeUsbTool;
//	vcAuthorizeXMLTool m_AuthorizeXMLTool;
	void AddTitleWarningMsg(CString strMsg);   // 添加主题过期报警信息

protected:
	BOOL m_bShowBottomCtrlPannel;
	int m_nCtrlPannelHeight;

	// 根据语言选择更改界面信息
	void UpdateDialogLanguage();
	CString GetDlgCtrlString(int nCtrlID);
	// 备用，根据彩色图标创建一个灰度图标
	HICON CreateGrayscaleIcon(HICON hIcon);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	static UINT ThreadUpload(LPVOID lpParam);


private:
	// 单平台界面初始化特别处理
	//void InitDlgItemSpecial();
	// 图像显示
	//void ShowVisionView(int nViewIndex);
	//void ShowVisionViewMode0(int nViewIndex);	// Mode0显示模式,图像显示
	//void ShowVisionViewMode1(int nViewIndex);	// Mode1显示模式,图像显示
	//void ShowVisionViewMode2(int nViewIndex);	// Mode2显示模式,图像显示
	// 动态调整界面上各控件的位置
	//void MoveDlgItem(BOOL bMoveViewsOnly = FALSE);
	//void MoveDlgItemMode0(BOOL bMoveViewsOnly = FALSE);		// Mode0显示模式,动态调整界面上各控件的位置
	//void MoveDlgItemMode1(BOOL bMoveViewsOnly = FALSE);		// Mode1显示模式,动态调整界面上各控件的位置
	//void MoveDlgItemMode2(BOOL bMoveViewsOnly = FALSE);		// Mode2显示模式,动态调整界面上各控件的位置

	// 更新手动按钮搜索按钮显示
	//void UpdateManualSearchButton();
//	int m_nRadioDisplayMode;	// 显示模式，表示当前选中工位的相机图像显示方式，显示哪个位置的图像。
// 	afx_msg void OnBnClickedRadioDisplayPos0();
// 	afx_msg void OnBnClickedRadioDisplayPos1();
// 	afx_msg void OnBnClickedRadioDisplayPos2();
// 	afx_msg void OnBnClickedRadioDisplayPos3();
// 	afx_msg void OnBnClickedRadioDisplayPos4();
// 	afx_msg void OnBnClickedRadioDisplayPos5();
// 	afx_msg void OnBnClickedRadioDisplayPos6();
// 	afx_msg void OnBnClickedRadioDisplayPos7();
// 	afx_msg void OnBnClickedRadioDisplayPos8();
// 	afx_msg void OnBnClickedRadioDisplayPos9();
// 	afx_msg void OnBnClickedRadioDisplayPos10();
// 	afx_msg void OnBnClickedRadioDisplayPos11();
// 	afx_msg void OnBnClickedRadioDisplayPos12();

//	afx_msg void OnBnClickedBtnShowOtherView();
// 	afx_msg void OnBnClickedBtnTest();
// 	afx_msg void OnBnClickedBtnManualTargetSearch2();
// 	afx_msg void OnBnClickedBtnManualObjectSearch2();


public:
	BOOL m_bPasswordReEnter;	// 密码账号登陆后;密码是否在一定时间内过期;如果过期需要再次登陆时需要输入 m_bPasswordReEnter = TRUE;  如果没有过期 m_bPasswordReEnter = FALSE； 再次登陆时需要输入则不需要输入密码；
	int m_nSeconds;			// 成功登陆后，开始计秒数;m_nSeconds>m_nSecondsMax后登陆密码免输入超时；

	CString	m_strAdminPassword;			// 管理员密码
	CString	m_strAdminPassword2;		// 管理员密码

	afx_msg void OnBnClickedBtnDisplayModeSet();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VisionASMDLG_H__B504C75E_E5C2_4AB6_8531_DFEC30B5A7F7__INCLUDED_)
