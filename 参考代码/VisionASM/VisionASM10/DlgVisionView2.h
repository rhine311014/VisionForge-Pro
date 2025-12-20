#pragma once

#include "vsBaseVisionAlign.h"
#include "VisionASMDlg.h"
#include "Label.h"

#include "svGuiText.h"
#include "svGuiCross.h"

#include "Layout.h"
#include "DlgVisionRun.h"
#include "afxwin.h"


// CDlgVisionView2 对话框

class CDlgVisionView2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgVisionView2)

public:
	CDlgVisionView2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVisionView2();

// 对话框数据
	enum { IDD = IDD_DLG_VISION_VIEW2 };


protected:
	void InitUIStyle();           // 设置UI风格
	void InitTabRunInfo();		  // 初始化运行信息Tab工位标签
	void InitUILayout();          // 整个窗口布局设置
	CLayout* InitTitleLayout();       // 初始化标题栏
	
protected:
	CLayout* m_pMainLayout;       // 整个窗口布局对象类型1 （水平布局）
	CLayout* m_pMainLayout2;	  // 整个窗口布局对象类型2	（垂直布局）

	CLayout* m_pCurMainLayout;	  // 当前整个窗口显示的布局类型
	int		 m_nCurMainLayout;	  // 当前整个窗口显示布局类型索引
	
protected:
	void InitDisplayLayout();     // 设置Display布局显示
	void InitDisplayLayoutPos1(); // 一个位置时布局
	void InitDisplayLayoutPos2(); // 两个位置时布局
	void InitDisplayLayoutPos3(); // 三个位置时布局
	void InitDisplayLayoutPos4(); // 四个位置时布局
	void InitDisplayLayoutPos5(); // 五个位置时布局
	void InitDisplayLayoutPos6(); // 六个位置时布局
	void InitDisplayLayoutPos7(); // 七个位置时布局
	void InitDisplayLayoutPos8(); // 八个位置时布局
protected:
	std::vector<CLayout*> m_vpDisplayLayout;	// 窗口全部显示时, Display窗口布局对象 种类
	int m_nDisplayShowMode;                     // 窗口全部显示时, 布局显示类型

protected:
	void InitPosShowMode();            // 初始化位置显示模式
	std::vector<CLayout*> m_vpChkPosDisplayLayout;	// 当选中单个位置显示时
public:
	int m_nPosShowMode;			       // RaidioButton位置显示模式 
	int m_nPrePosShowMode;
	int m_nVisionRunWidth;
public:
	CLayout* m_pViewDisplay;			// 显示display区域
	CLayout* m_pCurDisplayLayout;      // 当前显示布局情况

protected:
	CLayout* m_pTabLayout;				// 工位共享时Tab页自适应布局

public:

	int GetVisionRunWidth();
	void SetVisionRunWidth(int nWidth);				  // 设置运行窗口宽度
	int GetViewLayoutType();
	void SetViewLayoutType(int nViewLayoutType);      // 设置ViewPosDisplay布局类型

	int GetCurMainLayoutIndex();
	void SetCurMainLayoutIndex(int nIndex);           // 设置当前整个窗口布局显示类型


	void UpdateViewLayout();						  // 更新ViewPosDisplay布局显示
	void UpdateMainLayout();
protected:
	void UpdatePosShowLayout(int nChkId);       // 更新位置显示模式
	void UpdateGuiPos();						// 当窗口大小变化时, 更新GUI位置
public:
	void SetAllPosBtnHide();		  // 设置所有位置Radio按钮隐藏
	void SetSelPosModeBtnVisible();  // 根据位置数, 设置位置显示选择按钮控件显示

protected:
	CLayout* m_pPosBtnLayout;		// PosRadioBUtton布局, 位置控制显示
	CLayout* m_pTitleLayout;		// 标题栏布局

	CFont		m_cfontWSName;       // 工位选择按钮字体
public:
	void SetVisionRunDlg(CWnd* pVisionRunDlg);
	void AddVisionRunDlg(CWnd* pVisionRunDlg);
	std::vector<scGuiDisplay*>  m_vpGuiDisplay;		// GUI显示控件
protected:
	CDlgVisionRun*   m_pDlgVisionRun;		    // 运行信息窗口

	std::vector<CDlgVisionRun*>	m_vpDlgVisionRun; //运行窗口信息


protected:
	CVisionASMDlg*				m_pParent;				// 父窗口
	vcBaseVisionAlign*			m_pVisionASM;		// 视觉对位工具
	CStatusBarCtrl*				m_pStatusBar;			// 状态栏
	

	std::vector<scGuiText*>		m_vpGuiText;
	std::vector<scGuiCross*>	m_vpGuiCross;
	std::vector<BOOL>			m_vbGuiCrossVisible;

	HICON					m_hIconSucceed;			// 成功
	HICON					m_hIconFailed;			// 失败
	BOOL					m_bStatusBarFlag;		// 状态信息 成功/失败
	CString					m_strStatusBarInfo1;	// 状态信息1
	CString					m_strStatusBarInfo2;	// 状态信息2

	BOOL					m_bOtherPlatformStatusBarFlag;		// 状态信息 成功/失败
	CString					m_strOtherPlatformStatusBarInfo1;	// 状态信息1
	CString					m_strOtherPlatformStatusBarInfo2;	// 状态信息2


	AlignerProcessInfo		m_AlignerProcessInfo;	// 实时对位信息
	int	m_nCamNum;       // 相机数
	int m_nPositionNum;  // 位置数
	int m_nShowMode;      // 0:按照位置数 1:按照相机数
	int m_nShowDisplayNum; // 显示位置数
	int m_nArrayMode[MAXPOSNUM];			//hk20180130
	int m_nFitImage;                        //控制第一次图片显示为适应屏幕
	//BOOL m_nFitImageBool;                        //控制图片适应适应屏幕

	CString m_strPlatformID;		// platform ID
	int	m_nPlatformID;

	CLabel	m_labelStatus;
	CLabel	m_labelOtherStatus;

	void UpdateStatusBar();
	void UpdateOtherPlatformStatusBar();

public:
	void UpdateStatusBar( BOOL bFlag, LPCTSTR lpszInfo1, LPCTSTR lpszInfo2 );
	void UpdateOtherPlatformStatusBar( BOOL bFlag, LPCTSTR lpszInfo1, LPCTSTR lpszInfo2 );

	void UpdateDialogLanguage();
	void UpdateShowLanguage();
	CWnd* SetParentWnd(CWnd* pParent);

	void SetCameraNumber(int nCamNum, int nPositionNum, int nShowMode,/* int nPositionShowMode, */int nArrayMode[MAXPOSNUM]);
	int GetCameraNumber() { return m_nCamNum; }
	void SetVisionASMRef(vcBaseVisionAlign* pVisionASM);

	void UpdateCrossInfo();
	void SetPlatformID(LPCTSTR strID);
	CString GetPlatformIDString();
	void UpdatePlatformID();

	void SetPlatformID(int nID);
	int  GetPlatformID();

	// 更新手动按钮搜索按钮显示
	void UpdateManualSearchButton();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnDisplayVision();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnManualTargetSearch();
	afx_msg void OnBnClickedBtnManualObjectSearch();
	afx_msg void OnBnClickedRadioDisplayPos0();

	CXPStyleButtonST m_btnDisplayVisionRun;
	CThemeHelperST	 m_themeHelper;			// 主题助手，CXPStyleButtonST使用

	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);

	// 设置nPos位置的对象或者目标模板；
	void SetSearchTool(int nPos,BOOL bObjectSearchTool=TRUE);
	void SetCameraShutter(int nPosIndex);


public:
	CTabCtrl m_tabRunInfo;
	afx_msg void OnTcnSelchangeTabRunInfo(NMHDR *pNMHDR, LRESULT *pResult);
};
