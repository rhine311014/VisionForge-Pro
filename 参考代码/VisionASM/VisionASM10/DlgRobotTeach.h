#pragma once
#include "svGuiDisplay.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "KeyBoard.h"
#include "DlgRobotTeachPos.h"
#include "DlgRobotTeachSG.h"
#include "afxcmn.h"
#include "LogFileSave.h"

// CDlgRobotTeach 对话框

class CDlgRobotTeach : public CDialog
{
	DECLARE_DYNAMIC(CDlgRobotTeach)

public:
	CDlgRobotTeach(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRobotTeach();

// 对话框数据
	enum { IDD = IDD_DLG_ROBOT_TEACH };



	CThemeHelperST		m_themeHelper;
	CWnd *m_pVAPrevMainWnd;		
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;
	vcXYDVisionAlign *m_pVisionASM;
	CStringArray	*m_psaSysInfoStrings;

	BOOL m_bIsGrabbing;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;						// 对象display
	std::vector<scGuiDisplay*> m_vpGuiTargetDisplay;			// 虚拟对象display
	std::vector<scGuiInteractiveContainer*> m_vpGuiInteractiveContainer;	
	std::vector<scGuiInteractiveContainer*> m_vpGuiTargetInteractiveContainer;	
	std::vector<scGuiCross*> m_vpGuiCross;			// 十字GUI
	std::vector<scGuiText*> m_vpGuiText;
	std::vector<scGuiText*> m_vpGuiTargetText;

	CMarkImagePos                      m_mpObjectMarkImagePos;      // 实时对象定位结果
	CMarkImagePos                      m_mpVirtualMarkImagePos;     // 虚拟对象定位结果

	std::vector<CMarkImagePos*>		   m_vpmpCalibMarkImagePos;

	// 对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos0;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos1;
	// 虚拟对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos0;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos1;


	std::vector<BOOL>				m_vbCalibTargetPosSuccess;
	BOOL							m_bCalibTargetPosSuccess;

	BOOL							m_bCommGetPlatformAxisPosSuccess;

	BOOL m_bCalcuVirtualObjectSuccess;
	std::vector<CCoordPos>			m_vObjectToVirtualOffsets;


	BOOL m_bDisplayCross;

	std::vector<CCoordPos> m_vMarkCorrdPos;
	std::vector<double> m_vdPreShutter;
	std::vector<PTTriggerMode> m_vPrevTriggerMode;
	std::vector<BOOL> m_vPrevGrabStatus;

	CSliderCtrl	m_sliderShutter0;
	CSliderCtrl	m_sliderShutter1;

	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void UpdateStatusBar();
	BOOL SetGUI();

	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	bool UpdateGuiDisplay();

	std::vector<CCoordPos> GetManualSearchResult();
	void UpdateBtnLiveGrab();
	void UpdateShutterDisplay();

	BOOL m_bStopNow;
	int m_nEvent;
	BOOL m_bExitThread;
	CWinThread* m_pThreadReceiveData;
	HANDLE m_hEventReceiveThreadExit;

	static  UINT ThreadReceiveData(LPVOID pParam);     // 工作线程
	void ThreadReceiveFunc();	// 工作线程函数

	void SearchObjectPos(int nPosIndex);
	void SearchTargetPos(int nPosIndex);

	void SearchObjectPos1(int nPosIndex);
	void SearchTargetPos1(int nPosIndex);

	void CommGetPlatformAxisPos();
	void UpdateDialogLanguage();

	// 机器人示教范围信息
	CRobotTeachRangeInfo m_RobotTeachRangeInfo;
	CRobotTeachInfo		 m_RobotTeachInfo;

	// 当前工作模式
	RobotMod	m_eCurTempRobotMod;
	RobotMod	m_eCurRobotMod;
	RobotAutoControl	m_eCurTempAutoControl;
	RobotAutoControl	m_eCurAutoControl;
	// 手动移动速度
	int			m_nCurTempMSpeed;
	int			m_nCurMSpeed;
	BOOL		m_bCurMSpeedValid;

	// 当前机器人位置
	RobotPos	m_rpCurPos;
	BOOL		m_bCurPosValid;

	RobotInchAxis m_eCurInchAxis;
	RobotInchType m_eCurInchType;


	// 机器人点位信息
	int			  m_nCurPosIndex;		// 当前的位置索引
	std::vector<int> m_vnPosIndex;		// 位置点编号
	std::vector<BOOL> m_vbPosValid;		// 位置点是否有效
	std::vector<RobotPos> m_vrpPosPos;	// 位置点的位置信息

	// 机器人整数常量信息
	int			  m_nCurSGIntIndex;			// 当前的整数常量索引
	std::vector<int> m_vnSGIntIndex;		// 整数常量编号
	std::vector<BOOL> m_vbSGIntValid;		// 整数常量是否有效
	std::vector<int> m_vnSGInt;				// 整数常量信息
	std::vector<int> m_vnCurTempSGInt;		// 整数常量信息

	// 机器人实数常量信息
	int			  m_nCurSGRealIndex;		// 当前的实数常量索引
	std::vector<int> m_vnSGRealIndex;		// 实数常量编号
	std::vector<BOOL> m_vbSGRealValid;		// 实数常量是否有效
	std::vector<double> m_vdSGReal;			// 实数常量信息
	std::vector<double> m_vdCurTempSGReal;		// 实数常量信息


	// 机器人点位信息字符串队列
	std::vector<CString*> m_vstrPosX;
	std::vector<CString*> m_vstrPosY;
	std::vector<CString*> m_vstrPosR;
	std::vector<CString*> m_vstrPosZ;


	std::vector<CDlgRobotTeachPos*>  m_vpDlgRobotTeachPos; 
	int m_nPrevTabIndex;

	CDlgRobotTeachSG*	m_pDlgRobotTeachSG;

	CString m_strFormatStringFloat;
public:
	//////////////////////////////////////////////////////////////////////////
	// Yamaha 机器人
	// 获取示教信息
	BOOL RobotGetTeachInfo();
	BOOL RobotGetPosMSpeed();
	// 启动手动模式
	BOOL RobotSetModManual();
	// 启动自动运行模式
	BOOL RobotSetAutoControlRun();


	// 1）机器人工作模式查询、设定（手动模式manual、自动模式auto等）；
	BOOL RobotGetMod();
	BOOL RobotSetMod();

	// 2）机器人自动模式下程序执行操作（自动模式下的运行Run、重置Reset、停止stop等）；
	BOOL RobotSetAutoControl();

	// 3）机器人手动移动速度查询、设定（mspeed；
	BOOL RobotGetMSpeed();
	BOOL RobotSetMSpeed();

	// 4）机器人移动命令（inch X、Y、Z等)；
	BOOL RobotInch();

	// 5）机器人当前位置查询（WHRXY）；
	BOOL RobotGetPosWHRXY();

	// 6）设置机器人示教点位（TEACH 200）；
	BOOL RobotTeachPos();

	// 7）机器人点位查询（Read P200）；
	BOOL RobotReadPos();

	// 8）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）.
	BOOL RobotMovePos();

	// 9）执行中断处理（ETX(03)）
	BOOL RobotETX();

	// 10）保存示教信息到机器人
	BOOL RobotSaveTeachInfo();

	// 11）设置机器人点位
	BOOL RobotSetPos();


	// 3）机器人常量查询设置（SGI）
	BOOL RobotGetSGI();
	BOOL RobotSetSGI();

	// 3）机器人常量查询设置（SGR）
	BOOL RobotGetSGR();
	BOOL RobotSetSGR();

	// 3）机器人常量查询设置（SGI）
	BOOL RobotGetSGISGR();
	BOOL RobotSetSGISGR();

	// 10）设置某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) = 20 [cr/lf]）
	// 11）查询某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) [cr/lf]）
	// 12) 设置某个变量的值
	// 13）设置某个变量的值
	// 14）主机器人轴移动至可进行绝对式原点复位的位置(ABSADJ X-)
	// 15）主机器人轴的绝对式原点复位(ABSRESET k)



public:
	cpImage m_image0;
	cpImage m_image1;

protected:
	int m_nType;			// 0：目标Mark；1：对象Mark

	void InitDlgItem();
	void SetItemFont();				// 设置Edit和Button控件字体显示

	void UpdateDlgItem(BOOL bEnable);

	void UpdateDlgData(BOOL bSave);									// 更新数据
	void UpdateDlgDataMod(BOOL bSave);								// 更新机器人工作模式
	void UpdateDlgDataMSeed(BOOL bSave);							// 更新手动速度
	void UpdateDlgDataCurPos(BOOL bSave);							// 更新当前位置
	void UpdateDlgDataTeachPosPos(BOOL bSave, int nPosIndex);		// 更新示教点位置

	void ReOpenComm(); // 重置通信
public:
	CRITICAL_SECTION    m_csResourceComm;
	CString			m_strLogCommandDetail;
	deque<CString>	m_saCommCommandDetail;
	void AddCommCommandInfo(CString strCmd );				// 添加日志
	void UpdateLogCommCommand();							// 显示日志

	CFont				m_MyFont;
	CFont				m_MyBtnFont;

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }

	BOOL IsStringNumerical(LPCTSTR lpszVal);
	BOOL IsStringOverMSpeed(LPCTSTR lpszVal);

	BOOL IsInchEnable(RobotInchAxis nAxis, RobotInchType nType); // 根据机器人当前位置以及预先设定好的范围，判断是否可以点动
	BOOL IsCurPosOutRange();	 // 根据机器人当前位置以及预先设定好的范围，判断当前位置是否超出了范围
	BOOL IsTeachPosOutRange();	 // 根据机器人当前位置以及预先设定好的示教点限制范围，判断当前位置是否超出了示教点限制范围


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CXPStyleButtonST m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnGrabImage;
	CXPStyleButtonST	m_btnSearchObject0Pos0;
	CXPStyleButtonST	m_btnSearchObject1Pos0;
	CXPStyleButtonST	m_btnSearchObject0Pos1;
	CXPStyleButtonST	m_btnSearchObject1Pos1;
	CXPStyleButtonST	m_btnSearchVirtualObject0Pos0;
	CXPStyleButtonST	m_btnSearchVirtualObject1Pos0;
	CXPStyleButtonST	m_btnSearchVirtualObject0Pos1;
	CXPStyleButtonST	m_btnSearchVirtualObject1Pos1;
	CXPStyleButtonST	m_btnCalcuVirtualObject;
	CXPStyleButtonST	m_btnCommGetPlatformAxisPos;
	CXPStyleButtonST	m_btnPosXAdd;
	CXPStyleButtonST	m_btnPosXSub;
	CXPStyleButtonST	m_btnPosYAdd;
	CXPStyleButtonST	m_btnPosYSub;
	CXPStyleButtonST	m_btnPosRAdd;
	CXPStyleButtonST	m_btnPosRSub;
	CXPStyleButtonST	m_btnPosZAdd;
	CXPStyleButtonST	m_btnPosZSub;
	CLabel	m_labelNote;
	afx_msg void OnBnClickedBtnSerialGrab();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSearchObject0Pos0();
	afx_msg void OnBnClickedBtnSearchObject1Pos0();
	afx_msg void OnBnClickedBtnSearchObject0Pos1();
	afx_msg void OnBnClickedBtnSearchObject1Pos1();
	afx_msg void OnBnClickedBtnSearchVirtualObject0Pos0();
	afx_msg void OnBnClickedBtnSearchVirtualObject1Pos0();
	afx_msg void OnBnClickedBtnSearchVirtualObject0Pos1();
	afx_msg void OnBnClickedBtnSearchVirtualObject1Pos1();
	afx_msg void OnBnClickedBtnCalcuVirtualObject();
	int m_nRadioCalibTargetPos;
	afx_msg void OnBnClickedBtnSearchTarget0MovePos1();
	afx_msg void OnBnClickedBtnCommGetPlatformAxisPos();
	afx_msg void OnBnClickedBtnSetMannualMod();
	afx_msg void OnBnClickedBtnSetAutoRun();
	afx_msg void OnBnClickedBtnGetTeachInfo();
	CString m_strMSpeed;
	CString m_strCurPosX;
	CString m_strCurPosY;
	CString m_strCurPosR;
	CString m_strCurPosZ;
	CString m_strPosX1;
	CString m_strPosY1;
	CString m_strPosR1;
	CString m_strPosZ1;
	CString m_strPosX2;
	CString m_strPosY2;
	CString m_strPosR2;
	CString m_strPosZ2;
	CString m_strPosX3;
	CString m_strPosY3;
	CString m_strPosR3;
	CString m_strPosZ3;
	CString m_strPosX4;
	CString m_strPosY4;
	CString m_strPosR4;
	CString m_strPosZ4;
	CString m_strPosX5;
	CString m_strPosY5;
	CString m_strPosR5;
	CString m_strPosZ5;
	afx_msg void OnBnClickedBtnSavePos1();
	afx_msg void OnBnClickedBtnSavePos2();
	afx_msg void OnBnClickedBtnSavePos3();
	afx_msg void OnBnClickedBtnSavePos4();
	afx_msg void OnBnClickedBtnSavePos5();
	afx_msg void OnBnClickedBtnMovePos1();
	afx_msg void OnBnClickedBtnMovePos2();
	afx_msg void OnBnClickedBtnMovePos3();
	afx_msg void OnBnClickedBtnMovePos4();
	afx_msg void OnBnClickedBtnMovePos5();
	afx_msg void OnBnClickedBtnPosXSub();
	afx_msg void OnBnClickedBtnPosXAdd();
	afx_msg void OnBnClickedBtnPosYSub();
	afx_msg void OnBnClickedBtnPosYAdd();
	afx_msg void OnBnClickedBtnPosRSub();
	afx_msg void OnBnClickedBtnPosZSub();
	afx_msg void OnBnClickedBtnPosZAdd();
	CEdit m_editLog;
	afx_msg void OnBnClickedBtnSetMspeed();
	afx_msg void OnEnSetfocusEditMspeed();
	afx_msg void OnEnKillfocusEditMspeed();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnEtx();
	afx_msg void OnBnClickedBtnReopenComm();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CTabCtrl m_tabSheet;
	afx_msg void OnTcnSelchangeTabTeachPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSaveTeachInfoToRobot();
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnGetPosMspeed();
	afx_msg void OnBnClickedBtnPosRAdd();
};
