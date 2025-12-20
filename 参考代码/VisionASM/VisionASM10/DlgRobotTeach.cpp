// DlgRobotTeach.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgRobotTeach.h"
#include "afxdialogex.h"


// CDlgRobotTeach 对话框

IMPLEMENT_DYNAMIC(CDlgRobotTeach, CDialog)

CDlgRobotTeach::CDlgRobotTeach(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRobotTeach::IDD, pParent)
	, m_nRadioCalibTargetPos(0)
	, m_strMSpeed(_T(""))
	, m_strCurPosX(_T(""))
	, m_strCurPosY(_T(""))
	, m_strCurPosR(_T(""))
	, m_strCurPosZ(_T(""))
{
	m_nType = 0;
	m_pVAPrevMainWnd = NULL;

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;

	m_vpGuiDisplay.clear();
	m_vpGuiTargetDisplay.clear();
	m_vpPrevGuiDisplays.clear();
	m_pVisionASM = NULL;

	m_psaSysInfoStrings = NULL;

	m_vpGuiCross.clear();

	m_bDisplayCross = FALSE;

	// 状态栏相关
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");

	m_vpGuiInteractiveContainer.clear();
	m_vpGuiTargetInteractiveContainer.clear();

	m_vMarkCorrdPos.clear();
	m_vdPreShutter.clear();
	m_vPrevGrabStatus.clear();
	m_vPrevTriggerMode.clear();

	m_bCalcuVirtualObjectSuccess = FALSE;
	m_bIsGrabbing = FALSE;

	m_pThreadReceiveData = NULL;
	m_bExitThread = FALSE;
	m_hEventReceiveThreadExit = NULL;

	m_bCommGetPlatformAxisPosSuccess = FALSE;

	m_nEvent = 0;
	m_bStopNow = FALSE;

	// 当前工作模式
	m_eCurRobotMod = eRmUnknown;
	m_eCurAutoControl = eRacUnknown;
	// 手动移动速度
	m_nCurMSpeed = 20;
	m_bCurMSpeedValid = FALSE;
	// 当前机器人位置
	m_bCurPosValid = FALSE;

	m_eCurInchAxis = eRiaX;
	m_eCurInchType = eRitAdd;

	m_nCurPosIndex = 0;

	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = FALSE;

	InitializeCriticalSection(&m_csResourceComm);

	m_pDlgRobotTeachSG = NULL;
	m_strFormatStringFloat = _T("%.03f");
}

CDlgRobotTeach::~CDlgRobotTeach()
{
	int i=0;
	for (i=0; i<m_vpGuiDisplay.size(); i++)
	{
		if (m_vpGuiDisplay.at(i))
		{
			delete m_vpGuiDisplay.at(i);
			m_vpGuiDisplay.at(i) = NULL;
		}
	}

	for (i=0; i<m_vpGuiTargetDisplay.size(); i++)
	{
		if (m_vpGuiTargetDisplay.at(i))
		{
			delete m_vpGuiTargetDisplay.at(i);
			m_vpGuiTargetDisplay.at(i) = NULL;
		}
	}

	for (i=0; i<m_vpGuiCross.size(); i++)
	{
		if (m_vpGuiCross.at(i))
		{
			delete m_vpGuiCross.at(i);
			m_vpGuiCross.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnObjectImageMarkPos0.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos0.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos0.at(i);
			m_vpGuiAlnObjectImageMarkPos0.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnObjectImageMarkPos1.size();i++)
	{
		if (m_vpGuiAlnObjectImageMarkPos1.at(i) != NULL)
		{
			delete m_vpGuiAlnObjectImageMarkPos1.at(i);
			m_vpGuiAlnObjectImageMarkPos1.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnTargetImageMarkPos0.size();i++)
	{
		if (m_vpGuiAlnTargetImageMarkPos0.at(i) != NULL)
		{
			delete m_vpGuiAlnTargetImageMarkPos0.at(i);
			m_vpGuiAlnTargetImageMarkPos0.at(i) = NULL;
		}
	}

	for(i=0; i< m_vpGuiAlnTargetImageMarkPos1.size();i++)
	{
		if (m_vpGuiAlnTargetImageMarkPos1.at(i) != NULL)
		{
			delete m_vpGuiAlnTargetImageMarkPos1.at(i);
			m_vpGuiAlnTargetImageMarkPos1.at(i) = NULL;
		}
	}

	for(i=0; i<m_vpGuiText.size();i++)
	{
		if (m_vpGuiText.at(i) != NULL)
		{
			delete m_vpGuiText.at(i);
			m_vpGuiText.at(i) = NULL;
		}
	}


	for(i=0; i<m_vpGuiTargetText.size();i++)
	{
		if (m_vpGuiTargetText.at(i) != NULL)
		{
			delete m_vpGuiTargetText.at(i);
			m_vpGuiTargetText.at(i) = NULL;
		}
	}

	for (i = 0; i < RobotTeachPosMaxPageNum; i++)
	{
		delete m_vpDlgRobotTeachPos[i];
		m_vpDlgRobotTeachPos[i] = NULL;
	}

	if (m_pDlgRobotTeachSG != NULL)
	{
		delete m_pDlgRobotTeachSG;
		m_pDlgRobotTeachSG = NULL;
	}

	m_pVisionASM = NULL;
	m_MyFont.DeleteObject();
	m_MyBtnFont.DeleteObject();
	DeleteCriticalSection(&m_csResourceComm);
}

void CDlgRobotTeach::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 	DDX_Control(pDX, IDOK, m_btnOK);
	// 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SERIAL_GRAB, m_btnGrabImage);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT0_MOVE_POS0, m_btnSearchObject0Pos0);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT1_MOVE_POS0, m_btnSearchObject1Pos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT0_MOVE_POS1, m_btnSearchObject0Pos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_OBJECT1_MOVE_POS1, m_btnSearchObject1Pos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_TARGET0_MOVE_POS0, m_btnSearchVirtualObject0Pos0);
	DDX_Control(pDX, IDC_BTN_SEARCH_TARGET1_MOVE_POS0, m_btnSearchVirtualObject1Pos0);
	DDX_Control(pDX, IDC_BTN_SEARCH_TARGET0_MOVE_POS1, m_btnSearchVirtualObject0Pos1);
	DDX_Control(pDX, IDC_BTN_SEARCH_TARGET1_MOVE_POS1, m_btnSearchVirtualObject1Pos1);
	DDX_Control(pDX, IDC_BTN_CALIB_TARGET_POS, m_btnCalcuVirtualObject);
	DDX_Control(pDX, IDC_BTN_COMM_GET_PLATFORM_AXIS_POS, m_btnCommGetPlatformAxisPos);
	DDX_Radio(pDX, IDC_RADIO_CALIB_TARGET_POS0, m_nRadioCalibTargetPos);
	DDX_Control(pDX, IDC_STATIC_NOTE_AXIS, m_labelNote);
	DDX_Text(pDX, IDC_EDIT_MSPEED, m_strMSpeed);
	DDX_Text(pDX, IDC_EDIT_CUR_POS_X, m_strCurPosX);
	DDX_Text(pDX, IDC_EDIT_CUR_POS_Y, m_strCurPosY);
	DDX_Text(pDX, IDC_EDIT_CUR_POS_R, m_strCurPosR);
	DDX_Text(pDX, IDC_EDIT_CUR_POS_Z, m_strCurPosZ);
	DDX_Text(pDX, IDC_EDIT_POS_X1, m_strPosX1);
	DDX_Text(pDX, IDC_EDIT_POS_Y1, m_strPosY1);
	DDX_Text(pDX, IDC_EDIT_POS_R1, m_strPosR1);
	DDX_Text(pDX, IDC_EDIT_POS_Z1, m_strPosZ1);
	DDX_Text(pDX, IDC_EDIT_POS_X2, m_strPosX2);
	DDX_Text(pDX, IDC_EDIT_POS_Y2, m_strPosY2);
	DDX_Text(pDX, IDC_EDIT_POS_R2, m_strPosR2);
	DDX_Text(pDX, IDC_EDIT_POS_Z2, m_strPosZ2);
	DDX_Text(pDX, IDC_EDIT_POS_X3, m_strPosX3);
	DDX_Text(pDX, IDC_EDIT_POS_Y3, m_strPosY3);
	DDX_Text(pDX, IDC_EDIT_POS_R3, m_strPosR3);
	DDX_Text(pDX, IDC_EDIT_POS_Z3, m_strPosZ3);
	DDX_Text(pDX, IDC_EDIT_POS_X4, m_strPosX4);
	DDX_Text(pDX, IDC_EDIT_POS_Y4, m_strPosY4);
	DDX_Text(pDX, IDC_EDIT_POS_R4, m_strPosR4);
	DDX_Text(pDX, IDC_EDIT_POS_Z4, m_strPosZ4);
	DDX_Text(pDX, IDC_EDIT_POS_X5, m_strPosX5);
	DDX_Text(pDX, IDC_EDIT_POS_Y5, m_strPosY5);
	DDX_Text(pDX, IDC_EDIT_POS_R5, m_strPosR5);
	DDX_Text(pDX, IDC_EDIT_POS_Z5, m_strPosZ5);
	DDX_Control(pDX, IDC_EDIT_COMM_COMMAND, m_editLog);
	DDX_Control(pDX, IDC_BTN_POS_X_ADD, m_btnPosXAdd);
	DDX_Control(pDX, IDC_BTN_POS_X_SUB, m_btnPosXSub);
	DDX_Control(pDX, IDC_BTN_POS_Y_ADD, m_btnPosYAdd);
	DDX_Control(pDX, IDC_BTN_POS_Y_SUB, m_btnPosYSub);
	DDX_Control(pDX, IDC_BTN_POS_R_ADD, m_btnPosRAdd);
	DDX_Control(pDX, IDC_BTN_POS_R_SUB, m_btnPosRSub);
	DDX_Control(pDX, IDC_BTN_POS_Z_ADD, m_btnPosZAdd);
	DDX_Control(pDX, IDC_BTN_POS_Z_SUB, m_btnPosZSub);
	DDX_Control(pDX, IDC_TAB_TEACH_POS, m_tabSheet);
}


BEGIN_MESSAGE_MAP(CDlgRobotTeach, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SERIAL_GRAB, &CDlgRobotTeach::OnBnClickedBtnSerialGrab)
	ON_BN_CLICKED(IDCANCEL, &CDlgRobotTeach::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgRobotTeach::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT0_MOVE_POS0, &CDlgRobotTeach::OnBnClickedBtnSearchObject0Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT1_MOVE_POS0, &CDlgRobotTeach::OnBnClickedBtnSearchObject1Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT0_MOVE_POS1, &CDlgRobotTeach::OnBnClickedBtnSearchObject0Pos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1, &CDlgRobotTeach::OnBnClickedBtnSearchObject1Pos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET0_MOVE_POS0, &CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject0Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET1_MOVE_POS0, &CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject1Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET0_MOVE_POS1, &CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject0Pos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET1_MOVE_POS1, &CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject1Pos1)
	ON_BN_CLICKED(IDC_BTN_CALIB_TARGET_POS, &CDlgRobotTeach::OnBnClickedBtnCalcuVirtualObject)
	ON_BN_CLICKED(IDC_BTN_COMM_GET_PLATFORM_AXIS_POS, &CDlgRobotTeach::OnBnClickedBtnCommGetPlatformAxisPos)
	ON_BN_CLICKED(IDC_BTN_SET_MANNUAL_MOD, &CDlgRobotTeach::OnBnClickedBtnSetMannualMod)
	ON_BN_CLICKED(IDC_BTN_SET_AUTO_RUN, &CDlgRobotTeach::OnBnClickedBtnSetAutoRun)
	ON_BN_CLICKED(IDC_BTN_GET_TEACH_INFO, &CDlgRobotTeach::OnBnClickedBtnGetTeachInfo)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS1, &CDlgRobotTeach::OnBnClickedBtnSavePos1)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS2, &CDlgRobotTeach::OnBnClickedBtnSavePos2)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS3, &CDlgRobotTeach::OnBnClickedBtnSavePos3)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS4, &CDlgRobotTeach::OnBnClickedBtnSavePos4)
	ON_BN_CLICKED(IDC_BTN_SAVE_POS5, &CDlgRobotTeach::OnBnClickedBtnSavePos5)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS1, &CDlgRobotTeach::OnBnClickedBtnMovePos1)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS2, &CDlgRobotTeach::OnBnClickedBtnMovePos2)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS3, &CDlgRobotTeach::OnBnClickedBtnMovePos3)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS4, &CDlgRobotTeach::OnBnClickedBtnMovePos4)
	ON_BN_CLICKED(IDC_BTN_MOVE_POS5, &CDlgRobotTeach::OnBnClickedBtnMovePos5)
	ON_BN_CLICKED(IDC_BTN_POS_X_SUB, &CDlgRobotTeach::OnBnClickedBtnPosXSub)
	ON_BN_CLICKED(IDC_BTN_POS_X_ADD, &CDlgRobotTeach::OnBnClickedBtnPosXAdd)
	ON_BN_CLICKED(IDC_BTN_POS_Y_SUB, &CDlgRobotTeach::OnBnClickedBtnPosYSub)
	ON_BN_CLICKED(IDC_BTN_POS_Y_ADD, &CDlgRobotTeach::OnBnClickedBtnPosYAdd)
	ON_BN_CLICKED(IDC_BTN_POS_R_SUB, &CDlgRobotTeach::OnBnClickedBtnPosRSub)
	ON_BN_CLICKED(IDC_BTN_POS_Z_SUB, &CDlgRobotTeach::OnBnClickedBtnPosZSub)
	ON_BN_CLICKED(IDC_BTN_POS_Z_ADD, &CDlgRobotTeach::OnBnClickedBtnPosZAdd)
	ON_BN_CLICKED(IDC_BTN_SET_MSPEED, &CDlgRobotTeach::OnBnClickedBtnSetMspeed)
	ON_EN_SETFOCUS(IDC_EDIT_MSPEED, &CDlgRobotTeach::OnEnSetfocusEditMspeed)
	ON_EN_KILLFOCUS(IDC_EDIT_MSPEED, &CDlgRobotTeach::OnEnKillfocusEditMspeed)
	ON_BN_CLICKED(IDC_BTN_ETX, &CDlgRobotTeach::OnBnClickedBtnEtx)
	ON_BN_CLICKED(IDC_BTN_REOPEN_COMM, &CDlgRobotTeach::OnBnClickedBtnReopenComm)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TEACH_POS, &CDlgRobotTeach::OnTcnSelchangeTabTeachPos)
	ON_BN_CLICKED(IDC_BTN_SAVE_TEACH_INFO_TO_ROBOT, &CDlgRobotTeach::OnBnClickedBtnSaveTeachInfoToRobot)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgRobotTeach::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_GET_POS_MSPEED, &CDlgRobotTeach::OnBnClickedBtnGetPosMspeed)
	ON_BN_CLICKED(IDC_BTN_POS_R_ADD, &CDlgRobotTeach::OnBnClickedBtnPosRAdd)
END_MESSAGE_MAP()


// CDlgRobotTeach 消息处理程序

void CDlgRobotTeach::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
{
	if (m_pVisionASM != NULL)
	{
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplays.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}

	m_pVisionASM = (vcXYDVisionAlign*)pVisionAligner;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		m_vpPrevGuiDisplays.resize(nNum);
		for (int i=0; i<nNum; i++)
		{
			m_vpPrevGuiDisplays.at(i) = m_pVisionASM->GetPosDisplay(i);
			m_pVisionASM->SetPosDisplay(i, NULL);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
	}
}

BOOL CDlgRobotTeach::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化界面
	// 	if (m_pVisionASM->GetCameraNum() != 4)
	// 	{
	// 		m_nType = 0;
	// 	}

	// 初始化控件和相关数据
	UpdateDialogLanguage();
	InitDlgItem();		

	// 查询机器人数据


	UpdateDlgData(FALSE);

	OnBnClickedBtnSerialGrab();

	m_pThreadReceiveData = AfxBeginThread(ThreadReceiveData, this);
	m_hEventReceiveThreadExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgRobotTeach::SetItemFont()
{
	m_MyFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));					// lpszFacename


	m_MyBtnFont.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));					// lpszFacename


	GetDlgItem(IDC_EDIT_MSPEED)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_CUR_POS_X)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_CUR_POS_Y)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_CUR_POS_R)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_CUR_POS_Z)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z1)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z3)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z4)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_X5)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Y5)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_R5)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_POS_Z5)->SetFont(&m_MyFont);

	GetDlgItem(IDC_BTN_POS_X_ADD)->SetFont(&m_MyBtnFont);
	GetDlgItem(IDC_BTN_POS_X_SUB)->SetFont(&m_MyBtnFont);
	GetDlgItem(IDC_BTN_POS_Y_ADD)->SetFont(&m_MyBtnFont);
	GetDlgItem(IDC_BTN_POS_Y_SUB)->SetFont(&m_MyBtnFont);
	GetDlgItem(IDC_BTN_POS_R_ADD)->SetFont(&m_MyBtnFont);
	GetDlgItem(IDC_BTN_POS_R_SUB)->SetFont(&m_MyBtnFont);
	GetDlgItem(IDC_BTN_POS_Z_ADD)->SetFont(&m_MyBtnFont);
	GetDlgItem(IDC_BTN_POS_Z_SUB)->SetFont(&m_MyBtnFont);

}
void CDlgRobotTeach::InitDlgItem()
{
	// 	// 	CRect cr;
	// 	// 	GetClientRect(&cr);//获取对话框客户区域大小
	// 	// 	ClientToScreen(&cr);//转换为荧幕坐标
	// 	int x= GetSystemMetrics(SM_CXSCREEN);//获取荧幕坐标的宽度，单位为像素
	// 	int y= GetSystemMetrics(SM_CYSCREEN);//获取荧幕坐标的高度，单位为像素
	// 	MoveWindow(40, 30, (x - 80), (y - 60));//设置窗口显示在屏幕中间

	int i = 0;

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		
		MotionControlType TmpMotionControlType = eMCNo;
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL != vsConfig)
		{
			TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
		}

		if (TmpMotionControlType == eMCYamahaRobot)
		{
			m_strFormatStringFloat = _T("%.02f");
		}
		else
		{
			m_strFormatStringFloat = _T("%.03f");
		}
		

		// 获取机器人示教范围信息
		m_pVisionASM->GetRobotTeachRangeInfo(m_RobotTeachRangeInfo);

		// 获取机器人的示教信息

		// 获取当前产品索引 
		int nCurProductIndex = 0;
		CTotalProductData *pTotalProduct = m_pVisionASM->GetTotalProuductData();
		if (pTotalProduct != NULL)
		{
			nCurProductIndex = pTotalProduct->GetCurProductIndex();
		}
		CProductData *pProductData = pTotalProduct->GetItemPtr(nCurProductIndex);

		if (pProductData != NULL)
		{
			m_RobotTeachInfo = pProductData->m_RobotTeachInfo;
		}
	}

	// 机器人整数常量信息
	m_vnSGIntIndex.resize(RobotTeachSGIntMaxNum);		// 整数常量编号
	m_vbSGIntValid.resize(RobotTeachSGIntMaxNum);		// 整数常量是否有效
	m_vnSGInt.resize(RobotTeachSGIntMaxNum);		// 整数常量的位置信息

	for(i = 0; i < RobotTeachSGIntMaxNum; i++)
	{
		m_vnSGIntIndex[i] = m_RobotTeachRangeInfo.m_nRobotTeachSGIntStartPoint + i;
	}
	m_vbSGIntValid = m_RobotTeachInfo.m_vbRobotTeachSGIntValid;
	m_vnSGInt = m_RobotTeachInfo.m_vnRobotTeachSGInt;
	m_vnCurTempSGInt = m_vnSGInt;

	// 机器人实数常量信息
	m_vnSGRealIndex.resize(RobotTeachSGRealMaxNum);		// 实数常量编号
	m_vbSGRealValid.resize(RobotTeachSGRealMaxNum);		// 实数常量是否有效
	m_vdSGReal.resize(RobotTeachSGRealMaxNum);			// 实数常量的位置信息

	for(i = 0; i < RobotTeachSGRealMaxNum; i++)
	{
		m_vnSGRealIndex[i] = m_RobotTeachRangeInfo.m_nRobotTeachSGRealStartPoint + i;
	}
	m_vbSGRealValid = m_RobotTeachInfo.m_vbRobotTeachSGRealValid;
	m_vdSGReal = m_RobotTeachInfo.m_vdRobotTeachSGReal;
	m_vdCurTempSGReal = m_vdSGReal;
	
	// 机器人点位信息
	m_vnPosIndex.resize(RobotTeachPosTotalMaxNum);		// 位置点编号
	m_vbPosValid.resize(RobotTeachPosTotalMaxNum);		// 位置点是否有效
	m_vrpPosPos.resize(RobotTeachPosTotalMaxNum);		// 位置点的位置信息

	for(i = 0; i < RobotTeachPosTotalMaxNum; i++)
	{
		m_vnPosIndex[i] = m_RobotTeachRangeInfo.m_nRobotTeachPosStartPoint + i;
	}
	m_vbPosValid = m_RobotTeachInfo.m_vbRobotTearchPosValid;
	m_vrpPosPos = m_RobotTeachInfo.m_vrpRobotTeachPos;
	m_nCurMSpeed = m_RobotTeachInfo.m_nMSpeed;
	m_bCurMSpeedValid = TRUE;
	
	// 创建示教点Tab控件及相关对话框
	for(i = 0; i < RobotTeachPosMaxPageNum; i++)
	{
		m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), m_RobotTeachRangeInfo.m_vstrRobotTeachPosPageName[i]);		
	}
	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), m_RobotTeachRangeInfo.m_strRobotTeachSGPageName);

// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_TEACH_POS1);		
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_TEACH_POS2);	
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_TEACH_POS3);		
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_TEACH_POS4);
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_TEACH_POS5);
// 	m_tabSheet.InsertItem(m_tabSheet.GetItemCount(), TABSHEET_ITEM_TEACH_POS6);

	CRect rcTabSheet;
	m_tabSheet.GetClientRect(rcTabSheet);
	rcTabSheet.top	  += 22;   
	rcTabSheet.bottom -= 4;
	rcTabSheet.left	  += 2;
	rcTabSheet.right  -= 4;

	m_vpDlgRobotTeachPos.resize(RobotTeachPosMaxPageNum, NULL);
	for (i = 0; i < RobotTeachPosMaxPageNum; i++)
	{
		m_vpDlgRobotTeachPos[i] = new CDlgRobotTeachPos;
		m_vpDlgRobotTeachPos[i]->m_pParent = this;
		m_vpDlgRobotTeachPos[i]->m_nPageIndex = i;
		m_vpDlgRobotTeachPos[i]->SetSysInfoStringsRef(m_psaSysInfoStrings);
		m_vpDlgRobotTeachPos[i]->Create(IDD_DLG_ROBOT_TEACH_POS, &m_tabSheet);
		m_vpDlgRobotTeachPos[i]->MoveWindow(rcTabSheet);
		m_vpDlgRobotTeachPos[i]->EnableDlgItem(FALSE);
	}

	m_pDlgRobotTeachSG = new CDlgRobotTeachSG;
	m_pDlgRobotTeachSG->m_pParent = this;
	m_pDlgRobotTeachSG->m_nPageIndex = i;
	m_pDlgRobotTeachSG->SetSysInfoStringsRef(m_psaSysInfoStrings);
	m_pDlgRobotTeachSG->Create(IDD_DLG_ROBOT_TEACH_SG, &m_tabSheet);
	m_pDlgRobotTeachSG->MoveWindow(rcTabSheet);
	m_pDlgRobotTeachSG->EnableDlgItem(FALSE);
	m_pDlgRobotTeachSG->EnableSysKeyboard(m_bSysKeyboardEnabled);

	// 设置Edit控件字体显示
	SetItemFont();


	m_vstrPosX.push_back(&m_strPosX1);
	m_vstrPosY.push_back(&m_strPosY1);
	m_vstrPosR.push_back(&m_strPosR1);
	m_vstrPosZ.push_back(&m_strPosZ1);

	m_vstrPosX.push_back(&m_strPosX2);
	m_vstrPosY.push_back(&m_strPosY2);
	m_vstrPosR.push_back(&m_strPosR2);
	m_vstrPosZ.push_back(&m_strPosZ2);

	m_vstrPosX.push_back(&m_strPosX3);
	m_vstrPosY.push_back(&m_strPosY3);
	m_vstrPosR.push_back(&m_strPosR3);
	m_vstrPosZ.push_back(&m_strPosZ3);

	m_vstrPosX.push_back(&m_strPosX4);
	m_vstrPosY.push_back(&m_strPosY4);
	m_vstrPosR.push_back(&m_strPosR4);
	m_vstrPosZ.push_back(&m_strPosZ4);

	m_vstrPosX.push_back(&m_strPosX5);
	m_vstrPosY.push_back(&m_strPosY5);
	m_vstrPosR.push_back(&m_strPosR5);
	m_vstrPosZ.push_back(&m_strPosZ5);

	if (!m_pVisionASM->IsCommOpen())
	{
		m_pVisionASM->OpenComm();
	}


	m_labelNote.SetBkColor(RGB(0,255,255));
	m_labelNote.SetFontSize(10);
	m_labelNote.SetFontBold(TRUE);
//	m_labelNote.SetText(_T("注:请确保各拍照位置的平台θ轴位置一致！！！"));


	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	int nPartWidth[3] = {20, 300, -1};
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	m_pStatusBar->SetParts(3, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);

	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	UpdateStatusBar();

	// 	SetWindowText((m_nType == 0) ? _T("目标Mark手动选点") : _T("对象Mark手动选点"));
	// 	GetDlgItem(IDC_STATIC_CH0_CAP)->SetWindowText((m_nType == 0) ? _T("CH0") : _T("CH2"));
	// 	GetDlgItem(IDC_STATIC_CH1_CAP)->SetWindowText((m_nType == 0) ? _T("CH1") : _T("CH3"));

	BOOL bRepaint = FALSE;
	m_btnOK.SetThemeHelper(&m_themeHelper);	
	// 	// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
	// 	// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 
	m_btnCancel.SetThemeHelper(&m_themeHelper);
	// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
	// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);

	m_btnGrabImage.SetThemeHelper(&m_themeHelper);	
	// 	m_btnGrabImage.SetIcon(IDI_ICON_GRAB_40);
	// 	m_btnGrabImage.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnSearchObject0Pos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchObject1Pos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchObject0Pos1.SetThemeHelper(&m_themeHelper);
	m_btnSearchObject1Pos1.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObject0Pos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObject1Pos0.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObject0Pos1.SetThemeHelper(&m_themeHelper);
	m_btnSearchVirtualObject1Pos1.SetThemeHelper(&m_themeHelper);
	m_btnCalcuVirtualObject.SetThemeHelper(&m_themeHelper);
	m_btnCommGetPlatformAxisPos.SetThemeHelper(&m_themeHelper);

	m_btnPosXAdd.SetThemeHelper(&m_themeHelper);
	m_btnPosXSub.SetThemeHelper(&m_themeHelper);
	m_btnPosYAdd.SetThemeHelper(&m_themeHelper);
	m_btnPosYSub.SetThemeHelper(&m_themeHelper);
	m_btnPosRAdd.SetThemeHelper(&m_themeHelper);
	m_btnPosRSub.SetThemeHelper(&m_themeHelper);
	m_btnPosZAdd.SetThemeHelper(&m_themeHelper);
	m_btnPosZSub.SetThemeHelper(&m_themeHelper);

	//	m_btnAssistSearch.SetIcon(IDI_ICON_EXTRACT_POINT_48);
	//	m_btnAssistSearch.SetAlign(CButtonST::ST_ALIGN_VERT);

//	int i = 0;
	for (i=0; i<m_vpGuiDisplay.size(); i++)
	{
		delete m_vpGuiDisplay[i];
		m_vpGuiDisplay[i] = NULL;
	}
	m_vpGuiDisplay.clear();
	m_vpGuiInteractiveContainer.clear();

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<m_pVisionASM->GetPosNum(); i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i, this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			pGUIDisplay->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			pGUIDisplay->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			pGUIDisplay->SetLanguage(1);
			break;
		}
		m_pVisionASM->SetPosDisplay(i, pGUIDisplay);

		m_vpGuiDisplay.push_back(pGUIDisplay);
		m_vpGuiDisplay.at(i)->ShowWindow(SW_SHOW);
		pGUIDisplay = NULL;
	}

	if (m_pVisionASM->GetPosNum() < 3)
	{
		for (i=0; i<m_pVisionASM->GetPosNum(); i++)
		{			
			CRect rcDisp;
			m_vpGuiDisplay.at(i)->GetWindowRect(rcDisp);
			ScreenToClient(rcDisp);
			rcDisp.bottom = rcDisp.top + rcDisp.Height()*2;
			m_vpGuiDisplay.at(i)->MoveWindow(rcDisp);
		}
	}

// 
// 	for (i=0; i<m_pVisionASM->GetPosNum()/2; i++)
// 	{
// 		pGUIDisplay = new scGuiDisplay;
// 		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i+m_pVisionASM->GetPosNum()/2, this,WS_VISIBLE);
// 		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
// 
// 		m_pVisionASM->SetPosDisplay(i+m_pVisionASM->GetPosNum()/2, pGUIDisplay);
// 		m_vpGuiTargetDisplay.push_back(pGUIDisplay);
// 		m_vpGuiTargetDisplay.at(i)->ShowWindow(SW_SHOW);
// 		pGUIDisplay = NULL;
// 	}

	scGuiStaticContainer *pGuiSC = NULL;
	scGuiCross* pGuiCoordCross = NULL;
	scGuiText *pGuiText = NULL;
	CString strKey;
	CString strInfo;
	int nMovePosIndex = 0;
	for(i = 0;  i<m_pVisionASM->GetPosNum(); i++)
	{

		pGuiCoordCross = new scGuiCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));

		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CENTER)/*_T("中心%d")*/,i);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(FALSE);
		m_vpGuiCross.push_back(pGuiCoordCross);

		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_CPFAST_STR_POS),i+1);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiText.push_back(pGuiText);

		pGuiSC = m_vpGuiDisplay.at(i)->GetStaticContainer();

		strKey.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CENTER)/*_T("中心%d")*/,i);
		pGuiSC->AddItem(m_vpGuiCross.at(i), strKey);

		strKey.Format(_T("GuiText%d"),i);
		pGuiSC->AddItem(m_vpGuiText.at(i), strKey);

		m_vpGuiDisplay.at(i)->SetStaticContainer(pGuiSC);
	}

// 
// 	// 目标
// 	for(i = 0;  i<m_pVisionASM->GetPosNum()/2; i++)
// 	{
// 		pGuiCoordCross = new scGuiCoordCross;
// 		pGuiCoordCross->SetInteractive(FALSE);
// 		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
// 		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
// 		pGuiCoordCross->SetRulerVisible(FALSE);
// 		pGuiCoordCross->SetLineColor(RGB(255,0,0));
// 
// 		nMovePosIndex = 0;
// 		strInfo.Format(_T("移动位置%d"),nMovePosIndex+1);
// 		pGuiCoordCross->SetLabel(strInfo);
// 		pGuiCoordCross->SetLabelVisible(TRUE);
// 		m_vpGuiAlnTargetImageMarkPos0.push_back(pGuiCoordCross);
// 
// 
// 		pGuiCoordCross = new scGuiCoordCross;
// 		pGuiCoordCross->SetInteractive(FALSE);
// 		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
// 		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
// 		pGuiCoordCross->SetRulerVisible(FALSE);
// 		pGuiCoordCross->SetLineColor(RGB(255,0,0));
// 
// 		nMovePosIndex = 1;
// 		strInfo.Format(_T("移动位置%d"),nMovePosIndex+1);
// 		pGuiCoordCross->SetLabel(strInfo);
// 		pGuiCoordCross->SetLabelVisible(TRUE);
// 		m_vpGuiAlnTargetImageMarkPos1.push_back(pGuiCoordCross);
// 
// 		strInfo.Format(_T("目标位置%d"),i + 1 + m_pVisionASM->GetPosNum()/2);
// 		pGuiText = new scGuiText;
// 		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
// 		pGuiText->SetLineColor(RGB(255,0,0));
// 		pGuiText->SetVisible(TRUE);
// 		m_vpGuiTargetText.push_back(pGuiText);
// 
// 		pGuiSC = m_vpGuiTargetDisplay.at(i)->GetStaticContainer();
// 
// 		nMovePosIndex = 0;
// 		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
// 		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos0.at(i), strKey);
// 
// 		nMovePosIndex = 1;
// 		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
// 		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos1.at(i), strKey);
// 
// 		strKey.Format(_T("GuiTargetText%d"),i);
// 		pGuiSC->AddItem(m_vpGuiTargetText.at(i), strKey);
// 
// 		m_vpGuiTargetDisplay.at(i)->SetStaticContainer(pGuiSC);
// 	}

	m_vbCalibTargetPosSuccess.resize(m_pVisionASM->GetPosNum(), FALSE);


	UpdateDlgItem(FALSE);


	m_tabSheet.SetCurSel(0);
	m_nPrevTabIndex = 0;
	m_vpDlgRobotTeachPos[0]->ShowWindow(SW_SHOW);
}

void CDlgRobotTeach::UpdateDlgItem(BOOL bEnable)
{
// 	GetDlgItem(IDC_BTN_SERIAL_GRAB)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_RADIO_CALIB_TARGET_POS0)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_RADIO_CALIB_TARGET_POS1)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_RADIO_CALIB_TARGET_POS2)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_COMM_GET_PLATFORM_AXIS_POS)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_OBJECT0_MOVE_POS0)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS0)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_OBJECT0_MOVE_POS1)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS1)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS1)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->EnableWindow(bEnable);


	GetDlgItem(IDC_BTN_GET_POS_MSPEED)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_GET_TEACH_INFO)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_TEACH_INFO_TO_ROBOT)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDIT_MSPEED)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SET_MSPEED)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_SAVE_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS3)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS4)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE_POS5)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_MOVE_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_MOVE_POS2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_MOVE_POS3)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_MOVE_POS4)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_MOVE_POS5)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_POS_X_SUB)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_POS_X_ADD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_POS_Y_SUB)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_POS_Y_ADD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_POS_Z_SUB)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_POS_Z_ADD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_POS_R_SUB)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_POS_R_ADD)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_ETX)->EnableWindow(bEnable);

	GetDlgItem(IDOK)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_APPLY)->EnableWindow(bEnable);

	for(int i = 0; i < RobotTeachPosMaxPageNum; i++)
	{
		m_vpDlgRobotTeachPos[i]->EnableDlgItem(bEnable);
	}

	m_pDlgRobotTeachSG->EnableDlgItem(bEnable);
}


void CDlgRobotTeach::UpdateDlgData(BOOL bSave)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);
	}
	else
	{
		// 将产品数据上传到界面信息中

		// 
		if (m_eCurRobotMod != eRmManual)
		{
			UpdateDlgItem(FALSE);
		}
		else
		{
			UpdateDlgItem(TRUE);
		}

		// 手动移动速度
		if (m_bCurMSpeedValid)
		{
			m_strMSpeed.Format(_T("%d"), m_nCurMSpeed);
		}
		else
		{
			m_strMSpeed = _T("");
		}

		// 当前位置
		if (m_bCurPosValid)
		{
			m_strCurPosX.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosX);
			m_strCurPosY.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosY);
			m_strCurPosR.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosR);
			m_strCurPosZ.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosZ);
		}
		else
		{
			m_strCurPosX = _T("");
			m_strCurPosY = _T("");
			m_strCurPosR = _T("");
			m_strCurPosZ = _T("");
		}

		// 示教位置
		int i = 0; 
		for(i = 0; i < RobotTeachPosMaxPageNum; i++)
		{
			m_vpDlgRobotTeachPos[i]->UpdateDlgData(FALSE);
		}

		m_pDlgRobotTeachSG->UpdateDlgData(FALSE);
// 
// 		// 示教位置
// 		CString strInfo;
// 		int i = 0; 
// 		for(i = 0; i < RobotTeachPosMaxNumPerPage; i++)
// 		{
// 			if (m_vbPosValid[i] == TRUE)
// 			{
// 				strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosX);
// 				*m_vstrPosX.at(i) = strInfo;
// 				strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosY);
// 				*m_vstrPosY.at(i) = strInfo;
// 				strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosR);
// 				*m_vstrPosR.at(i) = strInfo;
// 				strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosZ);
// 				*m_vstrPosZ.at(i) = strInfo;
// 
// 			}
// 			else
// 			{
// 				strInfo = _T("");
// 				*m_vstrPosX.at(i) = strInfo;
// 				strInfo = _T("");
// 				*m_vstrPosY.at(i) = strInfo;
// 				strInfo = _T("");
// 				*m_vstrPosR.at(i) = strInfo;
// 				strInfo = _T("");
// 				*m_vstrPosZ.at(i) = strInfo;
// 			}
// 		}

		UpdateData(FALSE);
	}
}

// 更新机器人工作模式
void CDlgRobotTeach::UpdateDlgDataMod(BOOL bSave)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);
	}
	else
	{
		// 将产品数据上传到界面信息中

		// 
		if (m_eCurRobotMod != eRmManual)
		{
			UpdateDlgItem(FALSE);
		}
		else
		{
			UpdateDlgItem(TRUE);
		}

		UpdateData(FALSE);
	}
}

// 更新手动速度
void CDlgRobotTeach::UpdateDlgDataMSeed(BOOL bSave)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);
	}
	else
	{
		// 将产品数据上传到界面信息中

		// 手动移动速度
		if (m_bCurMSpeedValid)
		{
			m_strMSpeed.Format(_T("%d"), m_nCurMSpeed);
		}
		else
		{
			m_strMSpeed = _T("");
		}

		UpdateData(FALSE);
	}
}	

// 更新当前位置
void CDlgRobotTeach::UpdateDlgDataCurPos(BOOL bSave)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);
	}
	else
	{
		// 将产品数据上传到界面信息中

		// 当前位置
		if (m_bCurPosValid)
		{
			m_strCurPosX.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosX);
			m_strCurPosY.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosY);
			m_strCurPosR.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosR);
			m_strCurPosZ.Format(m_strFormatStringFloat, m_rpCurPos.m_dPosZ);
		}
		else
		{
			m_strCurPosX = _T("");
			m_strCurPosY = _T("");
			m_strCurPosR = _T("");
			m_strCurPosZ = _T("");
		}


		UpdateData(FALSE);
	}
}

// 更新示教点位置
void CDlgRobotTeach::UpdateDlgDataTeachPosPos(BOOL bSave, int nPosIndex)
{
	if (bSave == TRUE)
	{
		// 将界面信息保存到产品数据中
		UpdateData(TRUE);
	}
	else
	{
		// 将产品数据上传到界面信息中

		if (nPosIndex < 0 || nPosIndex >= RobotTeachPosMaxNumPerPage)
		{
			return ;
		}

		// 示教位置
		CString strInfo;

		int i = 0; 
		i = nPosIndex;
		if (m_vbPosValid[i] == TRUE)
		{
			strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosX);
			*m_vstrPosX.at(i) = strInfo;
			strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosY);
			*m_vstrPosY.at(i) = strInfo;
			strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosR);
			*m_vstrPosR.at(i) = strInfo;
			strInfo.Format(m_strFormatStringFloat, m_vrpPosPos[i].m_dPosZ);
			*m_vstrPosZ.at(i) = strInfo;

		}
		else
		{
			strInfo = _T("");
			*m_vstrPosX.at(i) = strInfo;
			strInfo = _T("");
			*m_vstrPosY.at(i) = strInfo;
			strInfo = _T("");
			*m_vstrPosR.at(i) = strInfo;
			strInfo = _T("");
			*m_vstrPosZ.at(i) = strInfo;
		}
	
		UpdateData(FALSE);
	}
}


void CDlgRobotTeach::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}


void CDlgRobotTeach::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_bExitThread = TRUE;// 使线程自动退出
	//m_pVisionASM->CloseComm();
	m_pVisionASM->StopReceiveComm();
	if (WaitForSingleObject(m_hEventReceiveThreadExit, WAIT_THREAD_QUIT_TIME) == WAIT_TIMEOUT)
	{
		TerminateThread(m_pThreadReceiveData->m_hThread, 0);
	}
	CloseHandle(m_hEventReceiveThreadExit);

	if (m_pStatusBar != NULL)
	{
		delete m_pStatusBar;
		m_pStatusBar = NULL;
	}
	if (m_hIconSucceed != NULL)
	{
		DestroyIcon(m_hIconSucceed);
		m_hIconSucceed = NULL;
	}
	if (m_hIconFailed != NULL)
	{
		DestroyIcon(m_hIconFailed);
		m_hIconFailed = NULL;
	}

	if (m_pVisionASM != NULL)
	{
		if (m_bIsGrabbing)
		{
			m_pVisionASM->FreezeAllCameras();
		}		

		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplays.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}


}


LRESULT CDlgRobotTeach::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);
		}
		break;
	case WM_UPDATE_STATUSBAR_INFO:
		{
			m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1,m_strStatusBarInfo2);
			UpdateStatusBar();
		}
		break;
	case WM_UPDATE_COMM_ROBOT_COMMAND_INFO:
		{
			UpdateLogCommCommand();
		}
		break;
	case WM_UPDATE_ROBOT_UPDATE_DLG_DATA:
		{
			UpdateDlgData(FALSE);
		}
		break;
	case WM_UPDATE_ROBOT_MOD:
		{
			UpdateDlgDataMod(FALSE);
		}
		break;
	case WM_UPDATE_ROBOT_MSPEED:
		{
			UpdateDlgDataMSeed(FALSE);
		}
		break;
	case WM_UPDATE_ROBOT_CUR_POS:
		{
			UpdateDlgDataCurPos(FALSE);
		}
		break;
	case WM_UPDATE_ROBOT_TEACH_POS:
		{
			int nPosIndex = (int)wParam;
			int nPageIndex = nPosIndex/RobotTeachPosMaxNumPerPage;
			m_vpDlgRobotTeachPos[nPageIndex]->UpdateDlgDataTeachPosPos(FALSE, nPosIndex);
//			UpdateDlgDataTeachPosPos(FALSE, nPosIndex);
		}
		break;
	case WM_UPDATE_ROBOT_SGI:
		{
			int nPosIndex = (int)wParam;
			m_pDlgRobotTeachSG->UpdateDlgDataTeachSGI(FALSE, nPosIndex);
		}
		break;
	case WM_UPDATE_ROBOT_SGR:
		{
			int nPosIndex = (int)wParam;
			m_pDlgRobotTeachSG->UpdateDlgDataTeachSGR(FALSE, nPosIndex);
		}
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}



BOOL CDlgRobotTeach::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_X_ADD)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaX;
			m_eCurInchType = eRitAdd;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_X_SUB)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaX;
			m_eCurInchType = eRitSub;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_Y_ADD)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaY;
			m_eCurInchType = eRitAdd;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_Y_SUB)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaY;
			m_eCurInchType = eRitSub;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_R_ADD)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaR;
			m_eCurInchType = eRitAdd;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_R_SUB)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaR;
			m_eCurInchType = eRitSub;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_Z_ADD)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaZ;
			m_eCurInchType = eRitAdd;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_POS_Z_SUB)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			m_eCurInchAxis = eRiaZ;
			m_eCurInchType = eRitSub;
			m_nEvent = eRobotInch;
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			m_nEvent = eRobotGetPosWHRXY;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgRobotTeach::UpdateBtnLiveGrab()
{
	// 更新“连续采集/停止采集”按钮 
	int nCamNum = m_pVisionASM->GetPosNum();
	nCamNum = (nCamNum == 4) ? 2 : nCamNum;

	BOOL bSucceed = TRUE;
	PTTriggerMode mode = ePTNotTrigger;
	for (int i=0; i<nCamNum; i++)
	{
		if (m_pVisionASM->IsPosCameraOnline(i+(m_nType<<1)))
		{
			//			bSucceed &= m_pVisionAligner->GetCameraTriggerMode(mode, i+(m_nType<<1));
			if (mode != ePTNotTrigger)
			{
				//				m_pVisionAligner->SetCameraTriggerMode(ePTNotTrigger, i+(m_nType<<1));		// 连续采集
			}
		}

		if (m_pVisionASM->IsPosCameraGrabbing(i+(m_nType<<1)))
		{
			GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")

			GetDlgItem(IDOK)->EnableWindow(FALSE);
			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			//			GetDlgItem(IDC_BUTTON_ASSIST_SEARCH)->EnableWindow(FALSE);

			// 			m_bDisplayCross = TRUE;
			// 			UpdateGuiDisplay();
		}
		else
		{
			if (m_pVisionASM->GrabCamera(i+(m_nType<<1)))
			{
				GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
				GetDlgItem(IDOK)->EnableWindow(FALSE);
				GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
				//				GetDlgItem(IDC_BUTTON_ASSIST_SEARCH)->EnableWindow(FALSE);

				// 				m_bDisplayCross = FALSE;
				// 				UpdateGuiDisplay();
			}
			else
			{
				GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
				//				GetDlgItem(IDC_BUTTON_ASSIST_SEARCH)->EnableWindow(TRUE);

				// 				m_bDisplayCross = TRUE;
				// 				UpdateGuiDisplay();
			}
		}
	}
}

void CDlgRobotTeach::UpdateShutterDisplay()
{
	// 	// 更新曝光时间显示
	// 	int nCamNum = m_pVisionAligner->GetCameraNum();
	// 	nCamNum = (nCamNum == 4) ? 2 : nCamNum;
	// 
	// 	for (int i=0; i<nCamNum; i++)
	// 	{
	// 		// 获取当前曝光时间并设置曝光时间
	// 		double dCurrent, dMin, dMax;
	// 		if (m_pVisionAligner->GetCameraShutter(i+(m_nType<<1), dCurrent, dMin, dMax))
	// 		{	
	// 			m_vdPreShutter.at(i) = dCurrent;
	// 			if (i == 0)
	// 			{
	// 				m_sliderShutter0.EnableWindow(TRUE);
	// 				m_sliderShutter0.SetRange(ceil(dMin/0.01), floor(dMax/0.01), TRUE);
	// 				m_sliderShutter0.SetPos( (int) ((dCurrent+0.005) / 0.01) );
	// 				CString strVal;
	// 				strVal.Format(_T("%.2fms"), dCurrent);
	// 				GetDlgItem(IDC_STATIC_SHUTTER_VAL_CH0)->SetWindowText(strVal);
	// 			}
	// 
	// 			if (i == 1)
	// 			{
	// 				m_sliderShutter1.EnableWindow(TRUE);
	// 				m_sliderShutter1.SetRange(ceil(dMin/0.01), floor(dMax/0.01), TRUE);
	// 				m_sliderShutter1.SetPos( (int) ((dCurrent+0.005) / 0.01) );
	// 				CString strVal;
	// 				strVal.Format(_T("%.2fms"), dCurrent);
	// 				GetDlgItem(IDC_STATIC_SHUTTER_VAL_CH1)->SetWindowText(strVal);
	// 			}
	// 
	// 			if (i == 2)
	// 			{
	// 			}
	// 		}
	// 		else
	// 		{
	// 			if (i == 0)
	// 			{
	// 				m_sliderShutter0.EnableWindow(FALSE);
	// 			}
	// 
	// 			if (i == 1)
	// 			{
	// 				m_sliderShutter1.EnableWindow(FALSE);
	// 			}
	// 
	// 			if (i == 2)
	// 			{
	// 			}
	// 		}
	// 	}
}




BOOL CDlgRobotTeach::SetGUI()
{
	int nCamNum = m_pVisionASM->GetCameraNum();
	nCamNum = (nCamNum == 4) ? 2 : nCamNum;

	m_vpGuiCross.resize(nCamNum);

	int i = 0;
	for (i=0; i<m_vpGuiCross.size(); i++)
	{
		m_vpGuiCross.at(i) = new scGuiCross;

		m_vpGuiCross.at(i)->SetLineColor(RGB(255, 0, 0));
		m_vpGuiCross.at(i)->SetCenterXYWidthHeight(100, 200);
	}

	for (i=0; i<nCamNum; i++)
	{
		m_vpGuiInteractiveContainer.at(i) = m_vpGuiDisplay.at(i)->GetInteractiveContainer();

		CString strInfo;
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHANNEL_TARGET_MARK)/*_T("CH%d通道目标Mark点")*/, i+(m_nType<<1));
		m_vpGuiCross.at(i)->SetLabel(strInfo);
		m_vpGuiCross.at(i)->SetLabelVisible(TRUE);

		strInfo.Format(_T("ManualTargetCross%d"), i+(m_nType<<1));
		m_vpGuiInteractiveContainer.at(i)->AddItem(m_vpGuiCross.at(i), strInfo);

		m_vpGuiDisplay.at(i)->SetInteractiveContainer(m_vpGuiInteractiveContainer.at(i));
	}

	return TRUE;	
}

// 更新训练Gui控件显示
bool CDlgRobotTeach::UpdateGuiDisplay()
{
	// 更新
	int nCamNum = m_pVisionASM->GetCameraNum();
	nCamNum = (nCamNum == 4) ? 2 : nCamNum;
	for (int i=0; i<nCamNum; i++)
	{
		m_vpGuiCross.at(i)->SetVisible(m_bDisplayCross);
		m_vpGuiDisplay.at(i)->MyInvalidate();
	}

	return true;
}	

void CDlgRobotTeach::OnBnClickedBtnSerialGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	int i=0;
	BOOL bSucceed = TRUE;
	int nCamNum = m_pVisionASM->GetPosNum(); 
	std::vector<BOOL> vbSucceed;
	if (m_bIsGrabbing)
	{
		for (i=0; i<nCamNum; i++)
		{
			vbSucceed.push_back(m_pVisionASM->FreezePosCamera(i));
		}
		for (i=0; i<nCamNum; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机停止失败，则恢复所有成功停止的相机为采集状态）
			for (i=0; i<nCamNum; i++)	
			{
				if (vbSucceed.at(i))
				{
					m_pVisionASM->GrabPosCamera(i);
				}
				else
				{
					strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_CPFAST_STR_POS), i+1);
					strNote += strInfo;
				}
			}
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
				strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			UpdateStatusBar();
			return;
		}

		// 		GetDlgItem(IDC_BTN_SERIAL_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));
		// 		GetDlgItem(IDOK)->EnableWindow(TRUE);
		// 		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_BTN_SEARCH_OBJECT_POS0)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_BTN_SEARCH_OBJECT_POS1)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS0)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS1)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT)->EnableWindow(TRUE);
		// 		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		m_bIsGrabbing = FALSE;
	}
	else
	{
		for (i=0; i<nCamNum; i++)
		{
			vbSucceed.push_back(m_pVisionASM->GrabPosCamera(i));
		}
		for (i=0; i<nCamNum; i++)
		{
			bSucceed &= vbSucceed.at(i);
		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机采集失败，则恢复所有成功采集的相机为停止状态）
			for (i=0; i<nCamNum; i++)	
			{
				if (vbSucceed.at(i))
				{
					m_pVisionASM->FreezePosCamera(i);
				}
				else
				{
					strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_CPFAST_STR_POS), i+1);
					strNote += strInfo;
				}
			}
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
				strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			UpdateStatusBar();
			return;
		}

		// 		GetDlgItem(IDC_BTN_SERIAL_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_FAILED));
		// 		GetDlgItem(IDOK)->EnableWindow(FALSE);
		// 		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		// 		GetDlgItem(IDC_BTN_SEARCH_OBJECT_POS0)->EnableWindow(FALSE);
		// 		GetDlgItem(IDC_BTN_SEARCH_OBJECT_POS1)->EnableWindow(FALSE);
		// 		GetDlgItem(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS0)->EnableWindow(FALSE);
		// 		GetDlgItem(IDC_BTN_SEARCH_VIRTUAL_OBJECT_POS1)->EnableWindow(FALSE);
		// 		GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT)->EnableWindow(FALSE);
		m_bIsGrabbing = TRUE;
	}
}


void CDlgRobotTeach::OnBnClickedBtnSearchObject0Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 1; // 对象移动位置1
}


void CDlgRobotTeach::OnBnClickedBtnSearchObject1Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 1; // 对象移动位置1
}


void CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject0Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 2; // 目标移动位置1
}


void CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject1Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 2; // 目标移动位置1
}

void CDlgRobotTeach::OnBnClickedBtnSearchObject0Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 3; // 对象移动位置2
}

void CDlgRobotTeach::OnBnClickedBtnSearchObject1Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 3; // 对象移动位置2
}


void CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject0Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 4; // 目标移动位置2
}


void CDlgRobotTeach::OnBnClickedBtnSearchVirtualObject1Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 4; // 目标移动位置2
}



void CDlgRobotTeach::OnBnClickedBtnCommGetPlatformAxisPos()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 5; // 获取平台基准位置（标定位置、接料位置、对位拍照基准位置）
}


void CDlgRobotTeach::OnBnClickedBtnCalcuVirtualObject()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nRadioCalibTargetPos = 0;

	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (!m_bCommGetPlatformAxisPosSuccess)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
		m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_GET_AXIS_POS);//.Format(_T("请获取平台轴位置！"));

		UpdateStatusBar();

		return;
	}

	if (m_nRadioCalibTargetPos == 0)			// 同时标定目标位置
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Empty();
		m_strStatusBarInfo2.Empty();

		CString strTempInfo1;
		CString strTempInfo2;
		int nPosIndex = 0;
		for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
		{
			if (TRUE == m_pVisionASM->ExecuteCalibrate(m_vpmpCalibMarkImagePos, nPosIndex + m_pVisionASM->GetPosNum()/2))
			{
				m_vbCalibTargetPosSuccess.at(nPosIndex) = TRUE;
			}
			else
			{
				m_vbCalibTargetPosSuccess.at(nPosIndex) = FALSE;
			}
		}

		m_bStatusBarFlag = m_vbCalibTargetPosSuccess.at(0);
		for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
		{
			m_bStatusBarFlag = m_bStatusBarFlag && m_vbCalibTargetPosSuccess.at(nPosIndex);
			if (TRUE == m_vbCalibTargetPosSuccess.at(nPosIndex))
			{
				strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_SUCCESS)/*_T("目标位置%d标定成功！")*/, nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
			}
			else
			{
				strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_FAILE)/*_T("目标位置%d标定失败！")*/, nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
			}
			m_strStatusBarInfo2 += strTempInfo2;
		}

		if (m_bStatusBarFlag)
		{
			m_strStatusBarInfo1= GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_SUCCESS);//.Format(_T("标定成功"));
		}
		else
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
		}
		UpdateStatusBar();

	}
	else if (m_nRadioCalibTargetPos > 0)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Empty();
		m_strStatusBarInfo2.Empty();

		CString strTempInfo1;
		CString strTempInfo2;
		int nPosIndex = 0;

		nPosIndex = m_nRadioCalibTargetPos - 1;
		if (TRUE == m_pVisionASM->ExecuteCalibrate(m_vpmpCalibMarkImagePos, nPosIndex + m_pVisionASM->GetPosNum()/2))
		{
			m_vbCalibTargetPosSuccess.at(nPosIndex) = TRUE;
		}
		else
		{
			m_vbCalibTargetPosSuccess.at(nPosIndex) = FALSE;
		}

		m_bStatusBarFlag = m_vbCalibTargetPosSuccess.at(nPosIndex);
		if (TRUE == m_vbCalibTargetPosSuccess.at(nPosIndex))
		{
			strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_SUCCESS)/*_T("目标位置%d标定成功！")*/, nPosIndex + m_pVisionASM->GetPosNum()/2);
		}
		else
		{
			strTempInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_TARGET_POS_CALI_FAILE)/*_T("目标位置%d标定失败！")*/, nPosIndex + m_pVisionASM->GetPosNum()/2);
		}
		m_strStatusBarInfo2 += strTempInfo2;


		if (m_bStatusBarFlag)
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_SUCCESS);//.Format(_T("标定成功"));
		}
		else
		{
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
		}
		UpdateStatusBar();
	}

}



void CDlgRobotTeach::OnBnClickedBtnApply()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pVisionASM != NULL)
	{

		// 获取机器人的示教信息
		m_RobotTeachInfo.m_nMSpeed = m_nCurMSpeed;
		m_RobotTeachInfo.m_vbRobotTearchPosValid = m_vbPosValid;
		m_RobotTeachInfo.m_vrpRobotTeachPos = m_vrpPosPos;
		m_RobotTeachInfo.m_vbRobotTeachSGIntValid = m_vbSGIntValid;
		m_RobotTeachInfo.m_vnRobotTeachSGInt = m_vnSGInt;
		m_RobotTeachInfo.m_vbRobotTeachSGRealValid = m_vbSGRealValid;
		m_RobotTeachInfo.m_vdRobotTeachSGReal = m_vdCurTempSGReal;

		// 获取当前产品索引 
		int nCurProductIndex = 0;
		CTotalProductData *pTotalProduct = m_pVisionASM->GetTotalProuductData();
		if (pTotalProduct != NULL)
		{
			nCurProductIndex = pTotalProduct->GetCurProductIndex();
		}
		CProductData *pProductData = pTotalProduct->GetItemPtr(nCurProductIndex);

		if (pProductData != NULL)
		{
			pProductData->m_RobotTeachInfo = m_RobotTeachInfo;
			m_pVisionASM->SaveProductDataInfo(nCurProductIndex);
		}

		CString strInfo;
		strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_APPLY_SAVE_SUCCESS);//.Format(_T("应用保存成功"));
		AddCommCommandInfo(strInfo);

	}
	else
	{
		CString strInfo;
		strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_APPLY_SAVE_FAILE);//.Format(_T("应用保存失败"));
		AddCommCommandInfo(strInfo);
	}
	CString strLog;
	strLog.Format(_T("点击[应用]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int nMBReturn = IDNO;
	CString strInfo;
	strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_LABLE4);//.Format(_T("确定保存示教信息到配置文件并退出吗？"));
	nMBReturn = AfxMessageBox(strInfo, MB_YESNO);
	if (nMBReturn == IDNO )
	{
		return;
	}
	else
	{
		if (m_pVisionASM != NULL)
		{
	
			// 获取机器人的示教信息
			m_RobotTeachInfo.m_nMSpeed = m_nCurMSpeed;
			m_RobotTeachInfo.m_vbRobotTearchPosValid = m_vbPosValid;
			m_RobotTeachInfo.m_vrpRobotTeachPos = m_vrpPosPos;
			m_RobotTeachInfo.m_vbRobotTeachSGIntValid = m_vbSGIntValid;
			m_RobotTeachInfo.m_vnRobotTeachSGInt = m_vnSGInt;
			m_RobotTeachInfo.m_vbRobotTeachSGRealValid = m_vbSGRealValid;
			m_RobotTeachInfo.m_vdRobotTeachSGReal = m_vdSGReal;


			// 获取当前产品索引 
			int nCurProductIndex = 0;
			CTotalProductData *pTotalProduct = m_pVisionASM->GetTotalProuductData();
			if (pTotalProduct != NULL)
			{
				nCurProductIndex = pTotalProduct->GetCurProductIndex();
			}
			CProductData *pProductData = pTotalProduct->GetItemPtr(nCurProductIndex);

			if (pProductData != NULL)
			{
				pProductData->m_RobotTeachInfo = m_RobotTeachInfo;
				m_pVisionASM->SaveProductDataInfo(nCurProductIndex);
			}
				
		}

	}


// 	if (m_pVisionASM == NULL)
// 	{
// 		m_bStatusBarFlag = FALSE;
// 		m_strStatusBarInfo1.Format(_T("保存失败"));
// 		m_strStatusBarInfo2.Format(_T(""));
// 		UpdateStatusBar();
// 	}
// 
// 	m_bCalibTargetPosSuccess = FALSE;
// 	int nPosIndex = 0;
// 	m_bCalibTargetPosSuccess = m_vbCalibTargetPosSuccess.at(nPosIndex);
// 	for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
// 	{
// 		m_bCalibTargetPosSuccess = m_bCalibTargetPosSuccess && m_vbCalibTargetPosSuccess.at(nPosIndex);	
// 	}
// 
// 	if (m_bCalibTargetPosSuccess)
// 	{
// 
// 		// 获取系统标定后的所有信息
// 		std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
// 		vpAllCalibratedInfo = m_pVisionASM->GetAllCalibratedInfo();	
// 
// 		/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
// 		m_pVisionASM->SetCurCalibratedInfo(vpAllCalibratedInfo);	
// 
// 		// 保存标定后信息到当前产品中及配置文件中
// 		m_pVisionASM->SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);
// 
// 		m_bStatusBarFlag = TRUE;
// 		m_strStatusBarInfo1.Format(_T("保存成功"));
// 		m_strStatusBarInfo2.Format(_T(""));
// 		UpdateStatusBar();
// 	}
// 	else
// 	{
// 		m_bStatusBarFlag = FALSE;
// 		m_strStatusBarInfo1.Format(_T("保存失败"));
// 		m_strStatusBarInfo2.Format(_T(""));
// 		UpdateStatusBar();
// 
// 		return;
// 	}

	CString strLog;
	strLog.Format(_T("[保存退出]机器人示教\n"));
	g_opLogFile.SaveLogText(strLog,1);
	CDialog::OnOK();
}


void CDlgRobotTeach::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nMBReturn = IDNO;
	CString strInfo;
	strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_EXIT2);//.Format(_T("确定不保存退出吗？"));
	nMBReturn = AfxMessageBox(strInfo, MB_YESNO);
	if (nMBReturn == IDNO )
	{
		return;
	}
	CString strLog;
	strLog.Format(_T("[不保存退出]机器人示教\n"));
	g_opLogFile.SaveLogText(strLog,1);
	CDialog::OnCancel();
}



UINT CDlgRobotTeach::ThreadReceiveData( LPVOID pParam )
{
	CDlgRobotTeach *pOwner = (CDlgRobotTeach*)pParam;
	if (pOwner != NULL)
	{
		pOwner->ThreadReceiveFunc();
	}

	return 0;
}

// typedef enum enumRobotEvent
// {
// 	eRobotIdle = 0,
// 	eRobotAutoControlRun = 1,		// 1）自动模式运行
// 	eRobotSetModManual,				// 2）手动模式；
// 	eRobotGetTeachInfo,				// 3）查询所有示教信息；
// 	eRobotSetMSpeed,				// 4）机器人手动移动速度设定（mspeed）；
// 	eRobotInch,						// 5）机器人移动命令（inch X、Y、Z等）；
// 	eRobotGetPosWHRXY,				// 6）机器人当前位置查询（WHRXY）；	
// 	eRobotTeachPos,					// 7）设置机器人示教点位（TEACH 200）；
// 	eRobotReadPos,					// 9）机器人点位查询（Read P200）；
// 	eRobotMovePos,					// 10）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）；
// } RobotEvent;

void CDlgRobotTeach::ThreadReceiveFunc()
{
	int nInchIndex = 0;
	int nGetPosInchIndex = 50;
	CString strReceived;
	while (!m_bExitThread)
	{
		switch(m_nEvent)
		{
		case eRobotAutoControlRun:		// 1）自动模式运行
			{
				RobotSetAutoControlRun();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotSetModManual:		// 2）手动模式；
			{
				RobotSetModManual();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotGetTeachInfo:		// 3）查询所有示教信息；
			{
				RobotGetTeachInfo();
				m_nEvent = eRobotIdle;
			}
			break;	
		case eRobotSetMSpeed:		// 4）机器人手动移动速度设定（mspeed）；
			{
				RobotSetMSpeed();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotInch:		// 5）机器人移动命令（inch X、Y、Z等）；
			{
				if (!RobotInch())
				{
					nInchIndex = 0;
					m_nEvent = eRobotIdle;
				}
				else
				{
					//RobotGetPosWHRXY();
					nInchIndex++;
					if (nInchIndex > nGetPosInchIndex)
					{
						RobotGetPosWHRXY();
						nInchIndex = 0;
					}
				}
			}
			break;
		case eRobotGetPosWHRXY:		// 6）机器人当前位置查询（WHRXY）；
			{				
				RobotGetPosWHRXY();
				m_nEvent = eRobotIdle;

			}
			break;
		case eRobotTeachPos:		// 7）设置机器人示教点位（TEACH 200）；
			{
				RobotTeachPos();
				RobotReadPos();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotReadPos:		// 9）机器人点位查询（Read P200）；
			{
				RobotReadPos();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotMovePos:	// 10）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）；
			{
				RobotMovePos();
				RobotGetPosWHRXY();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotETX:
			{
				RobotETX();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotSaveTeachInfo:		// 12）保存所有示教信息到机器人；
			{
				RobotSaveTeachInfo();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotSetPos:		// 12）保存所有示教信息到机器人；
			{
				RobotSetPos();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotGetPosWHRXYMSpeed:		// 6）机器人当前位置查询（WHRXY）和手动速度(MSPEED)；
			{				
				RobotGetPosMSpeed();
				m_nEvent = eRobotIdle;

			}
			break;
		case eRobotGetSGISGR:		// 15）设置机器人示教点位（SGI、SGR）；
			{
				RobotGetSGISGR();
				m_nEvent = eRobotIdle;
			}
			break;
		case eRobotSetSGISGR:		// 15）查询机器人示教点位（SGI、SGR）；
			{
				RobotSetSGISGR();
				m_nEvent = eRobotIdle;
			}
			break;
		default:
			{
				Sleep(1);
			}

		}
	//	Sleep(20);
	}

	SetEvent(m_hEventReceiveThreadExit);
}

void CDlgRobotTeach::SearchObjectPos(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return;
	}
	int nMovePosIndex = 0;
	m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetVisible(FALSE);

	if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
	{
		m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

		if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
			&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

			m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(45),100,100);
			m_vpGuiAlnObjectImageMarkPos0.at(nPosIndex)->SetVisible(TRUE);
		}
	}
}


void CDlgRobotTeach::SearchObjectPos1(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return;
	}
	int nMovePosIndex = 1;
	m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetVisible(FALSE);

	if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
	{
		m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

		if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
			&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

			m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(45),100,100);
			m_vpGuiAlnObjectImageMarkPos1.at(nPosIndex)->SetVisible(TRUE);
		}
	}
}

void CDlgRobotTeach::SearchTargetPos(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return;
	}
	int nMovePosIndex = 0;
	m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);
	if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
	{
		m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

		if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
			&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

			m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(0),100,100);
			m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
		}
	}
	else
	{
		return;
	}
}



void CDlgRobotTeach::SearchTargetPos1(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return;
	}
	int nMovePosIndex = 1;
	m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(FALSE);
	if (TRUE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
	{
		m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

		if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
			&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

			m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(0),100,100);
			m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->SetVisible(TRUE);
		}
	};
}

void CDlgRobotTeach::CommGetPlatformAxisPos()
{
	m_bCommGetPlatformAxisPosSuccess = FALSE;
	if (m_pVisionASM == NULL)
	{
		return;
	}

	// 通信获取平台轴位置
	m_bCommGetPlatformAxisPosSuccess = m_pVisionASM->GetCalibAlignerTargetPlatformAxisPos();

}



void CDlgRobotTeach::AddCommCommandInfo( CString strCmd )
{
	// 获取时间	
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	CString strTime;
	strTime.Format(_T("%02d-%02d-%03d "),tm.wMinute, tm.wSecond, tm.wMilliseconds);


	EnterCriticalSection(&m_csResourceComm);

	if (m_saCommCommandDetail.size() > 500)
	{
		m_saCommCommandDetail.pop_front();
	}

	CString strNewCmd = strCmd;
	m_saCommCommandDetail.push_back(strTime + strNewCmd);


	LeaveCriticalSection(&m_csResourceComm);

	PostMessage(WM_UPDATE_COMM_ROBOT_COMMAND_INFO, 0, 0);	
}

void CDlgRobotTeach::UpdateLogCommCommand()
{
	EnterCriticalSection(&m_csResourceComm);
	int i = 0;
	m_strLogCommandDetail.Empty();
	for(i = 0; i < m_saCommCommandDetail.size(); i++)
	{
		m_strLogCommandDetail += m_saCommCommandDetail.at(i) + _T("\r\n");
	}
	LeaveCriticalSection(&m_csResourceComm);

	m_editLog.SetWindowText(m_strLogCommandDetail);
	m_editLog.LineScroll(m_editLog.GetLineCount());

	UpdateData(FALSE);
}

// 获取示教信息
BOOL CDlgRobotTeach::RobotGetTeachInfo()
{
	// 1）机器人工作模式查询
	RobotGetMod();

	// 2）机器人手动移动速度查询
	RobotGetMSpeed();

	// 3）机器人当前位置查询（WHRXY）
	RobotGetPosWHRXY();

	// 4）机器人点位查询；
	int i = 0;
	for(i = 0; i < RobotTeachPosTotalMaxNum; i++)
	{
		m_nCurPosIndex = i;
		RobotReadPos();
	}

	// UpdateDlgData(FALSE);

	// 5)机器人常量查询
	RobotGetSGISGR();

	PostMessage(WM_UPDATE_ROBOT_UPDATE_DLG_DATA, 0, 0);		

	return TRUE;
}

BOOL CDlgRobotTeach::RobotGetPosMSpeed()
{
	// 2）机器人手动移动速度查询
	RobotGetMSpeed();	

	// 3）机器人当前位置查询（WHRXY）
	RobotGetPosWHRXY();

	return TRUE;
}

// 启动手动模式
BOOL CDlgRobotTeach::RobotSetModManual()
{
	// 1）机器人工作模式查询
	RobotGetMod();

	// Stop
	m_eCurAutoControl = eRacStop;
	RobotSetAutoControl();

	// 手动模式
	m_eCurTempRobotMod = eRmManual;
	RobotSetMod();

	if (m_eCurRobotMod == eRmManual)
	{
		UpdateDlgItem(TRUE);
	}
	else
	{
		UpdateDlgItem(FALSE);
	}

	return TRUE;
}

// 启动自动运行模式
BOOL CDlgRobotTeach::RobotSetAutoControlRun()
{
	// 1）机器人工作模式查询
	RobotGetMod();

	// Stop
	m_eCurTempAutoControl = eRacStop;
	RobotSetAutoControl();

	// Auto模式
	m_eCurTempRobotMod = eRmAuto;
	RobotSetMod();

	// Reset
	m_eCurTempAutoControl = eRacReset;
	RobotSetAutoControl();

	// Run
	m_eCurTempAutoControl = eRacRun;
	RobotSetAutoControl();

	if (m_eCurRobotMod == eRmManual)
	{
		UpdateDlgItem(TRUE);
	}
	else
	{
		UpdateDlgItem(FALSE);
	}

	return TRUE;

}

// 1）机器人工作模式查询、设定（手动模式manual、自动模式auto等）；
BOOL CDlgRobotTeach::RobotGetMod()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}
	
	RobotMod rmRobotMod = eRmUnknown;
	if (m_pVisionASM->CommunicateRobotGetMod(rmRobotMod))
	{
		m_eCurRobotMod = rmRobotMod;
		
		CString strInfo;
		strInfo.Format(_T("工作模式查询成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("工作模式查询失败"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDlgRobotTeach::RobotSetMod()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_pVisionASM->CommunicateRobotSetMod(m_eCurTempRobotMod))
	{
		m_eCurRobotMod = m_eCurTempRobotMod;

		CString strInfo;
		strInfo.Format(_T("工作模式设置成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("工作模式设置失败"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgRobotTeach::RobotSetAutoControl()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmAuto)
	{
// 		CString strInfo;
// 		strInfo.Format(_T("当前不是自动模式，自动模式操作失败"));
// 		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	if (m_pVisionASM->CommunicateRobotAutoControl(m_eCurTempAutoControl))
	{
		m_eCurAutoControl = m_eCurTempAutoControl;
		CString strInfo;
		strInfo.Format(_T("自动模式操作设置成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("自动模式操作设置失败"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgRobotTeach::RobotGetMSpeed()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		m_bCurMSpeedValid = FALSE;

		return FALSE;
	}

	int nCurMSpeed = 0;
	if (m_pVisionASM->CommunicateRobotGetMSpeed(nCurMSpeed))
	{
		m_nCurMSpeed = nCurMSpeed;
		m_bCurMSpeedValid = TRUE;

		CString strInfo;
		strInfo.Format(_T("手动移动速度查询成功"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_MSPEED, 0, 0);	
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("手动移动速度查询失败"));
		AddCommCommandInfo(strInfo);
		m_bCurMSpeedValid = FALSE;

		return FALSE;
	}



	return TRUE;

}

BOOL CDlgRobotTeach::RobotSetMSpeed()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);

		return FALSE;
	}

	if (m_pVisionASM->CommunicateRobotSetMSpeed(m_nCurTempMSpeed))
	{
		m_nCurMSpeed = m_nCurTempMSpeed;
		CString strInfo;
		strInfo.Format(_T("手动移动速度设置成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("手动移动速度设置失败"));
		AddCommCommandInfo(strInfo);

		return FALSE;
	}

	return TRUE;
}

// 根据机器人当前位置以及预先设定好的范围，判断当前位置是否超出了范围
BOOL CDlgRobotTeach::IsCurPosOutRange()
{
	if (m_RobotTeachRangeInfo.m_bCurRobotPosRangeEnable == FALSE)
	{
		return FALSE;
	}

	RobotPos rpCurPosMax = m_RobotTeachRangeInfo.m_rpCurRobotPosMax;
	RobotPos rpCurPosMin = m_RobotTeachRangeInfo.m_rpCurRobotPosMin;
	if (m_rpCurPos.m_dPosX > rpCurPosMax.m_dPosX
		|| m_rpCurPos.m_dPosX < rpCurPosMin.m_dPosX
		|| m_rpCurPos.m_dPosY > rpCurPosMax.m_dPosY
		|| m_rpCurPos.m_dPosY < rpCurPosMin.m_dPosY
		|| m_rpCurPos.m_dPosR > rpCurPosMax.m_dPosR
		|| m_rpCurPos.m_dPosR < rpCurPosMin.m_dPosR
		|| m_rpCurPos.m_dPosZ > rpCurPosMax.m_dPosZ
		|| m_rpCurPos.m_dPosZ < rpCurPosMin.m_dPosZ
		|| m_rpCurPos.m_dPosA > rpCurPosMax.m_dPosA
		|| m_rpCurPos.m_dPosA < rpCurPosMin.m_dPosA
		|| m_rpCurPos.m_dPosB > rpCurPosMax.m_dPosB
		|| m_rpCurPos.m_dPosB < rpCurPosMin.m_dPosB)
	{
		return TRUE;
	}

	return FALSE;
}

// 根据机器人当前位置以及预先设定好的示教点限制范围，判断当前位置是否超出了示教点限制范围
BOOL CDlgRobotTeach::IsTeachPosOutRange()
{	
	if (m_RobotTeachRangeInfo.m_vbRobotTeachPosRangeEnable[m_nCurPosIndex] == FALSE)
	{
		return FALSE;
	}

	RobotPos rpCurPosMax = m_RobotTeachRangeInfo.m_vrpRobotTeachPosMax[m_nCurPosIndex];
	RobotPos rpCurPosMin = m_RobotTeachRangeInfo.m_vrpRobotTeachPosMin[m_nCurPosIndex];
	if (m_rpCurPos.m_dPosX > rpCurPosMax.m_dPosX
		|| m_rpCurPos.m_dPosX < rpCurPosMin.m_dPosX
		|| m_rpCurPos.m_dPosY > rpCurPosMax.m_dPosY
		|| m_rpCurPos.m_dPosY < rpCurPosMin.m_dPosY
		|| m_rpCurPos.m_dPosR > rpCurPosMax.m_dPosR
		|| m_rpCurPos.m_dPosR < rpCurPosMin.m_dPosR
		|| m_rpCurPos.m_dPosZ > rpCurPosMax.m_dPosZ
		|| m_rpCurPos.m_dPosZ < rpCurPosMin.m_dPosZ
		|| m_rpCurPos.m_dPosA > rpCurPosMax.m_dPosA
		|| m_rpCurPos.m_dPosA < rpCurPosMin.m_dPosA
		|| m_rpCurPos.m_dPosB > rpCurPosMax.m_dPosB
		|| m_rpCurPos.m_dPosB < rpCurPosMin.m_dPosB)
	{
		return TRUE;
	}

	return FALSE;
}

 // 根据机器人当前位置以及预先设定好的范围，判断是否可以点动
BOOL CDlgRobotTeach::IsInchEnable(RobotInchAxis nAxis, RobotInchType nType)
{
	if (m_RobotTeachRangeInfo.m_bCurRobotPosRangeEnable == FALSE)
	{
		return TRUE;
	}

	BOOL bEnable = TRUE;
	RobotPos rpCurPosMax = m_RobotTeachRangeInfo.m_rpCurRobotPosMax;
	RobotPos rpCurPosMin = m_RobotTeachRangeInfo.m_rpCurRobotPosMin;

	if (nAxis == eRiaX)
	{
		if (nType == eRitAdd)
		{
			if (m_rpCurPos.m_dPosX > rpCurPosMax.m_dPosX)
			{
				bEnable = FALSE;
			}
		}
		else if (nType == eRitSub)
		{
			if (m_rpCurPos.m_dPosX < rpCurPosMin.m_dPosX)
			{
				bEnable = FALSE;
			}
		}
	}
	else if (nAxis == eRiaY)
	{
		if (nType == eRitAdd)
		{
			if (m_rpCurPos.m_dPosY > rpCurPosMax.m_dPosY)
			{
				bEnable = FALSE;
			}
		}
		else if (nType == eRitSub)
		{
			if (m_rpCurPos.m_dPosY < rpCurPosMin.m_dPosY)
			{
				bEnable = FALSE;
			}
		}
	}
	else if (nAxis == eRiaR)
	{
		if (nType == eRitAdd)
		{
			if (m_rpCurPos.m_dPosR > rpCurPosMax.m_dPosR)
			{
				bEnable = FALSE;
			}
		}
		else if (nType == eRitSub)
		{
			if (m_rpCurPos.m_dPosR < rpCurPosMin.m_dPosR)
			{
				bEnable = FALSE;
			}
		}
	}
	else if (nAxis == eRiaZ)
	{
		if (nType == eRitAdd)
		{
			if (m_rpCurPos.m_dPosZ > rpCurPosMax.m_dPosZ)
			{
				bEnable = FALSE;
			}
		}
		else if (nType == eRitSub)
		{
			if (m_rpCurPos.m_dPosZ < rpCurPosMin.m_dPosZ)
			{
				bEnable = FALSE;
			}
		}
	}
	else if (nAxis == eRiaA)
	{
		if (nType == eRitAdd)
		{
			if (m_rpCurPos.m_dPosA > rpCurPosMax.m_dPosA)
			{
				bEnable = FALSE;
			}
		}
		else if (nType == eRitSub)
		{
			if (m_rpCurPos.m_dPosA < rpCurPosMin.m_dPosA)
			{
				bEnable = FALSE;
			}
		}
	}
	else if (nAxis == eRiaB)
	{
		if (nType == eRitAdd)
		{
			if (m_rpCurPos.m_dPosB > rpCurPosMax.m_dPosB)
			{
				bEnable = FALSE;
			}
		}
		else if (nType == eRitSub)
		{
			if (m_rpCurPos.m_dPosB < rpCurPosMin.m_dPosB)
			{
				bEnable = FALSE;
			}
		}
	}

	return bEnable;
}

BOOL CDlgRobotTeach::RobotInch()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);

		return FALSE;
	}

	if (!IsInchEnable(m_eCurInchAxis, m_eCurInchType))
	{
		CString strInfo;
		strInfo.Format(_T("点动超出范围,请反向点动！"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	if (m_pVisionASM->CommunicateRobotInch(m_eCurInchAxis, m_eCurInchType))
	{
		CString strInfo;
		strInfo.Format(_T("点动移动成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("点动移动失败"));
		AddCommCommandInfo(strInfo);

		return FALSE;
	}

	return TRUE;
}

BOOL CDlgRobotTeach::RobotGetPosWHRXY()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		m_bCurMSpeedValid = FALSE;
		return FALSE;
	}

	if (m_pVisionASM->CommunicateRobotGetPosWHRXY(m_rpCurPos))
	{
		m_bCurPosValid = TRUE;
		CString strInfo;
		strInfo.Format(_T("获取当前位置成功"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_CUR_POS, 0, 0);	


		if (IsCurPosOutRange())
		{
			CString strInfo;
			strInfo.Format(_T("当前位置超出可移动范围，请移动回允许范围内！"));
			AddCommCommandInfo(strInfo);
		}

	}
	else
	{
		m_bCurPosValid = FALSE;
		CString strInfo;
		strInfo.Format(_T("获取当前位置失败"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_CUR_POS, 0, 0);

		return FALSE;
	}

	return TRUE;
}

BOOL CDlgRobotTeach::RobotTeachPos()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	if (IsTeachPosOutRange())
	{
		CString strInfo;
		strInfo.Format(_T("当前位置超出了示教点限制范围，示教位置失败！"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nPosIndex = 0;
	nPosIndex = m_vnPosIndex[m_nCurPosIndex];
	if (m_pVisionASM->CommunicateRobotTeachPos(nPosIndex))
	{
		CString strInfo;
		strInfo.Format(_T("示教位置成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("示教位置失败"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgRobotTeach::RobotReadPos()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nPosIndex = 0;
	nPosIndex = m_vnPosIndex[m_nCurPosIndex];
	RobotPos rpPos;
	if (m_pVisionASM->CommunicateRobotReadPos(nPosIndex,rpPos))
	{
		m_vrpPosPos[m_nCurPosIndex] = rpPos;
		m_vbPosValid[m_nCurPosIndex] = TRUE;

		CString strInfo;
		strInfo.Format(_T("读取点位置成功"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_TEACH_POS, m_nCurPosIndex, 0);	
	}
	else
	{
		m_vbPosValid[m_nCurPosIndex] = FALSE;

		CString strInfo;
		strInfo.Format(_T("读取点位置失败"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_TEACH_POS, m_nCurPosIndex, 0);	
		return FALSE;
	}



	return TRUE;
}

BOOL CDlgRobotTeach::RobotMovePos()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nPosIndex = 0;
	nPosIndex = m_vnPosIndex[m_nCurPosIndex];
	RobotPos rpPos;
	if (m_pVisionASM->CommunicateRobotMovePos(nPosIndex, m_nCurMSpeed))
	{
		CString strInfo;
		strInfo.Format(_T("移动成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("移动失败"));
		AddCommCommandInfo(strInfo);

		return FALSE;
	}

	return TRUE;
}


BOOL CDlgRobotTeach::RobotETX()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_pVisionASM->CommunicateRobotETX())
	{
		CString strInfo;
		strInfo.Format(_T("中断处理成功"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("中断处理失败"));
		AddCommCommandInfo(strInfo);

		return FALSE;
	}

	return TRUE;
}


// 10）保存示教信息到机器人
BOOL CDlgRobotTeach::RobotSaveTeachInfo()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	BOOL bOK = TRUE;

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	
	// 保存手动速度
	bOK = bOK && RobotSetMSpeed();

	// 保存点位信息
	int i = 0;
	for(i = 0; i < RobotTeachPosTotalMaxNum; i++)
	{
		m_nCurPosIndex = i;
		bOK = bOK && RobotSetPos();
	}

	// 保存常量信息
	bOK = bOK && RobotSetSGISGR();

	if (bOK)
	{
		CString strInfo;
		strInfo.Format(_T("保存示教信息到机器人成功！"));
		AddCommCommandInfo(strInfo);
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("保存示教信息到机器人失败！"));
		AddCommCommandInfo(strInfo);
	}

	return bOK;
}

// 11）设置机器人点位
BOOL CDlgRobotTeach::RobotSetPos()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nPosIndex = 0;	
	RobotPos rpPos;
	BOOL bPosValid = FALSE;
	nPosIndex = m_vnPosIndex[m_nCurPosIndex];
	rpPos = m_vrpPosPos[m_nCurPosIndex];
	bPosValid = m_vbPosValid[m_nCurPosIndex];
	if (bPosValid)
	{
		if (m_pVisionASM->CommunicateRobotSetPos(nPosIndex,rpPos))
		{
			m_vrpPosPos[m_nCurPosIndex] = rpPos;
			m_vbPosValid[m_nCurPosIndex] = TRUE;

			CString strInfo;
			strInfo.Format(_T("保存点位置P%d成功"), nPosIndex);
			AddCommCommandInfo(strInfo);

		}
		else
		{
			m_vbPosValid[m_nCurPosIndex] = FALSE;

			CString strInfo;
			strInfo.Format(_T("保存点位置P%d失败"), nPosIndex);
			AddCommCommandInfo(strInfo);

			return FALSE;
		}
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("无效点位置%d，不保存"), nPosIndex);
		AddCommCommandInfo(strInfo);
	}

	
	return TRUE;
}


// 3）机器人常量查询设置（SGI、SGR）
BOOL CDlgRobotTeach::RobotGetSGI()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nIndex = 0;
	nIndex = m_vnSGIntIndex[m_nCurSGIntIndex];
	int nVal = 0;
	if (m_pVisionASM->CommunicateRobotReadSGI(nIndex,nVal))
	{
		m_vnSGInt[m_nCurSGIntIndex] = nVal;
		m_vbSGIntValid[m_nCurSGIntIndex] = TRUE;

		CString strInfo;
		strInfo.Format(_T("读取整数常量成功"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_SGI, m_nCurSGIntIndex, 0);	
	}
	else
	{
		m_vbSGIntValid[m_nCurSGIntIndex] = FALSE;

		CString strInfo;
		strInfo.Format(_T("读取整数常量失败"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_SGI, m_nCurSGIntIndex, 0);	
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDlgRobotTeach::RobotSetSGI()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nIndex = 0;	
	int nSGInt = 0;
	BOOL bSGIntValid = FALSE;
	nIndex = m_vnSGIntIndex[m_nCurSGIntIndex];
	nSGInt = m_vnCurTempSGInt[m_nCurSGIntIndex];
	bSGIntValid = m_vbSGIntValid[m_nCurSGIntIndex];
	if (bSGIntValid)
	{
		if (m_pVisionASM->CommunicateRobotWriteSGI(nIndex,nSGInt))
		{
			m_vnSGInt[m_nCurSGIntIndex] = nSGInt;
			m_vbSGIntValid[m_nCurSGIntIndex] = TRUE;

			CString strInfo;
			strInfo.Format(_T("保存整数常量%d成功"), nIndex);
			AddCommCommandInfo(strInfo);

		}
		else
		{
			m_vbSGIntValid[m_nCurSGIntIndex] = FALSE;

			CString strInfo;
			strInfo.Format(_T("保存整数常量%d失败"), nIndex);
			AddCommCommandInfo(strInfo);

			return FALSE;
		}
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("无效整数常量%d，不保存"), nIndex);
		AddCommCommandInfo(strInfo);
	}


	return TRUE;
}



// 3）机器人常量查询设置（SGI、SGR）
BOOL CDlgRobotTeach::RobotGetSGR()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nIndex = 0;
	nIndex = m_vnSGRealIndex[m_nCurSGRealIndex];
	double dVal = 0;
	if (m_pVisionASM->CommunicateRobotReadSGR(nIndex,dVal))
	{
		m_vdSGReal[m_nCurSGRealIndex] = dVal;
		m_vbSGRealValid[m_nCurSGRealIndex] = TRUE;

		CString strInfo;
		strInfo.Format(_T("读取实数常量成功"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_SGR, m_nCurSGRealIndex, 0);	
	}
	else
	{
		m_vbSGRealValid[m_nCurSGRealIndex] = FALSE;

		CString strInfo;
		strInfo.Format(_T("读取实数常量失败"));
		AddCommCommandInfo(strInfo);

		PostMessage(WM_UPDATE_ROBOT_SGR, m_nCurSGRealIndex, 0);	
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgRobotTeach::RobotSetSGR()
{
	if (m_bStopNow == TRUE)
	{
		return FALSE;
	}

	if (m_eCurRobotMod != eRmManual)
	{
		CString strInfo;
		strInfo.Format(_T("当前不是手动模式"));
		AddCommCommandInfo(strInfo);
		return FALSE;
	}

	int nIndex = 0;	
	double dSGReal = 0;
	BOOL bSGRealValid = FALSE;
	nIndex = m_vnSGRealIndex[m_nCurSGRealIndex];
	dSGReal = m_vdCurTempSGReal[m_nCurSGRealIndex];
	bSGRealValid = m_vbSGRealValid[m_nCurSGRealIndex];
	if (bSGRealValid)
	{
		if (m_pVisionASM->CommunicateRobotWriteSGR(nIndex,dSGReal))
		{
			m_vdSGReal[m_nCurSGRealIndex] = dSGReal;
			m_vbSGRealValid[m_nCurSGRealIndex] = TRUE;

			CString strInfo;
			strInfo.Format(_T("保存实数常量%d成功"), nIndex);
			AddCommCommandInfo(strInfo);

		}
		else
		{
			m_vbSGRealValid[m_nCurSGRealIndex] = FALSE;

			CString strInfo;
			strInfo.Format(_T("保存实数常量%d失败"), nIndex);
			AddCommCommandInfo(strInfo);

			return FALSE;
		}
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("无效实数常量%d，不保存"), nIndex);
		AddCommCommandInfo(strInfo);
	}


	return TRUE;
}


BOOL CDlgRobotTeach::RobotGetSGISGR()
{
	// 整型常量
	int i = 0;
	BOOL bOK = TRUE;
	for(i = 0; i < RobotTeachSGIntMaxNum; i++)
	{
		m_nCurSGIntIndex = i;
		bOK = bOK && RobotGetSGI();
	}

	// 实数常量
	for(i = 0; i < RobotTeachSGRealMaxNum; i++)
	{
		m_nCurSGRealIndex = i;
		bOK = bOK && RobotGetSGR();
	}
	
	return bOK;
}

BOOL CDlgRobotTeach::RobotSetSGISGR()
{
	// 整型常量
	int i = 0;
	BOOL bOK = TRUE;
	for(i = 0; i < RobotTeachSGIntMaxNum; i++)
	{
		m_nCurSGIntIndex = i;
		bOK = bOK && RobotSetSGI();
	}

	// 实数常量
	for(i = 0; i < RobotTeachSGRealMaxNum; i++)
	{
		m_nCurSGRealIndex = i;
		bOK = bOK && RobotSetSGR();
	}

	return bOK;
}



void CDlgRobotTeach::OnBnClickedBtnSetMannualMod()
{
	// TODO: 在此添加控件通知处理程序代码

	// 设置机器人为手动模式
	m_nEvent = eRobotSetModManual;	
	CString strLog;
	strLog.Format(_T("点击[启动机器人手动模式]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnSetAutoRun()
{
	// TODO: 在此添加控件通知处理程序代码

	// 设置机器人为自动运行模式
	m_nEvent = eRobotAutoControlRun;	
	CString strLog;
	strLog.Format(_T("点击[启动机器人自动运行]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnSaveTeachInfoToRobot()
{
	// TODO: 在此添加控件通知处理程序代码
	int nMBReturn = IDNO;
	CString strInfo;
	strInfo.Format(_T("确定将示教信息写入机器人吗？"));
	nMBReturn = AfxMessageBox(strInfo, MB_YESNO);
	if (nMBReturn == IDNO )
	{
		return;
	}

	m_nEvent = eRobotSaveTeachInfo;

	CString strLog;
	strLog.Format(_T("点击[保存示教信息到机器人]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}

void CDlgRobotTeach::OnBnClickedBtnGetTeachInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = eRobotGetTeachInfo;
	CString strLog;
	strLog.Format(_T("点击[查询机器人示教信息]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}



void CDlgRobotTeach::OnBnClickedBtnGetPosMspeed()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = eRobotGetPosWHRXYMSpeed;
	CString strLog;
	strLog.Format(_T("点击[查询当前位置速度]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}




void CDlgRobotTeach::OnBnClickedBtnSetMspeed()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_nCurTempMSpeed = atoi(m_strMSpeed);
	CString strLog;
	strLog.Format(_T("点击[设定]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	m_nEvent = eRobotSetMSpeed;
}


void CDlgRobotTeach::OnBnClickedBtnSavePos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 0;
	m_nEvent = eRobotTeachPos;
}


void CDlgRobotTeach::OnBnClickedBtnSavePos2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 1;
	m_nEvent = eRobotTeachPos;
}


void CDlgRobotTeach::OnBnClickedBtnSavePos3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 2;
	m_nEvent = eRobotTeachPos;
}


void CDlgRobotTeach::OnBnClickedBtnSavePos4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 3;
	m_nEvent = eRobotTeachPos;
}


void CDlgRobotTeach::OnBnClickedBtnSavePos5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 4;
	m_nEvent = eRobotTeachPos;
}


void CDlgRobotTeach::OnBnClickedBtnMovePos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 0;
	m_nEvent = eRobotMovePos;
}


void CDlgRobotTeach::OnBnClickedBtnMovePos2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 1;
	m_nEvent = eRobotMovePos;
}


void CDlgRobotTeach::OnBnClickedBtnMovePos3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 2;
	m_nEvent = eRobotMovePos;
}


void CDlgRobotTeach::OnBnClickedBtnMovePos4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 3;
	m_nEvent = eRobotMovePos;
}


void CDlgRobotTeach::OnBnClickedBtnMovePos5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurPosIndex = 4;
	m_nEvent = eRobotMovePos;
}


void CDlgRobotTeach::OnBnClickedBtnPosXSub()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[X-]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnPosXAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[X+]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnPosYSub()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[Y-]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnPosYAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[Y+]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnPosRSub()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[R-]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnPosZSub()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[Z-]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnPosZAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[Z+]\n"));
	g_opLogFile.SaveLogText(strLog,2);

}


BOOL CDlgRobotTeach::IsStringNumerical( LPCTSTR lpszVal )
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i=0; i<nLen; i++)
	{
		ch = strVal.GetAt(i);
		if ((ch > '9') || ((ch < '0') && (ch != '.')))
		{
			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgRobotTeach::IsStringOverMSpeed(LPCTSTR lpszVal)
{
	CString strVal = lpszVal;
	if (atoi(strVal) > 100 ||  atoi(strVal) < 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CDlgRobotTeach::OnEnSetfocusEditMspeed()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MSPEED)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MSPEED, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_MSPEED, strOutput);
			CString strLog;
			strLog.Format(_T("修改[手动移动速度]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditMspeed();
	}	
}


void CDlgRobotTeach::OnEnKillfocusEditMspeed()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_MSPEED, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMSpeed(strVal))
		{
			SetDlgItemText(IDC_EDIT_MSPEED, m_strMSpeed);
		}
		else
		{
			m_strMSpeed.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_MSPEED, m_strMSpeed);
		}
	}	
}
// 重置通信
void CDlgRobotTeach::ReOpenComm()
{
	if(m_pVisionASM == NULL)
	{
		CString strInfo;
		strInfo.Format(_T("重置通信失败！"));
		AddCommCommandInfo(strInfo);
	}

	m_pVisionASM->StopNow(TRUE);
	m_bStopNow = TRUE;

	if (m_pVisionASM->CloseComm())
	{
	}
	Sleep(10);
	int i = 0;
	while(m_nEvent != eRobotIdle)
	{
		Sleep(10);
		i++;
		if (i > 1000)
		{
			break;
		}
	}

	m_nEvent = eRobotIdle;
	if (m_pVisionASM->OpenComm())
	{
		CString strInfo;
		strInfo.Format(_T("重置通信成功！"));
		AddCommCommandInfo(strInfo);
		m_pVisionASM->StopNow(FALSE);
		m_bStopNow = FALSE;
	}
	else
	{
		if (m_pVisionASM->OpenComm())
		{
			CString strInfo;
			strInfo.Format(_T("重置通信成功！"));
			AddCommCommandInfo(strInfo);
			m_pVisionASM->StopNow(FALSE);
			m_bStopNow = FALSE;
		}
		else
		{
			CString strInfo;
			strInfo.Format(_T("重置通信失败！"));
			AddCommCommandInfo(strInfo);
		}
	}
}

void CDlgRobotTeach::OnBnClickedBtnEtx()
{
	// TODO: 在此添加控件通知处理程序代码
	ReOpenComm();
	
	m_nEvent = eRobotETX;

	CString strLog;
	strLog.Format(_T("点击[停止]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnBnClickedBtnReopenComm()
{
	// TODO: 在此添加控件通知处理程序代码
	ReOpenComm();
	CString strLog;
	strLog.Format(_T("点击[重置通信]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}


void CDlgRobotTeach::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	if (1 == nIDEvent)
// 	{
// 		RobotETX();
// 
// 		KillTimer(1);
// 	}

	CDialog::OnTimer(nIDEvent);
}


void CDlgRobotTeach::OnTcnSelchangeTabTeachPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	int nCurSel = m_tabSheet.GetCurSel();
	if (nCurSel == m_nPrevTabIndex)
	{
		return;
	}

	CString strItem;
	TCITEM tcItem;
	CHAR chChar[256];

	//  Get the current tab item text.
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = chChar;
	tcItem.cchTextMax = 256;

	m_tabSheet.GetItem(m_nPrevTabIndex, &tcItem);
	strItem = tcItem.pszText;

	int i = 0;

	for(i = 0; i < RobotTeachPosMaxPageNum; i++)
	{
		if (m_RobotTeachRangeInfo.m_vstrRobotTeachPosPageName[i] == strItem)
		{
			m_vpDlgRobotTeachPos[i]->ShowWindow(SW_HIDE);
			break;
		}
	}

	if (m_RobotTeachRangeInfo.m_strRobotTeachSGPageName == strItem)
	{
		m_pDlgRobotTeachSG->ShowWindow(SW_HIDE);
	}


	//  Get the current tab item text.
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = chChar;
	tcItem.cchTextMax = 256;
	m_tabSheet.GetItem(nCurSel, &tcItem);
	strItem = tcItem.pszText;

	for(i = 0; i < RobotTeachPosMaxPageNum; i++)
	{
		if (m_RobotTeachRangeInfo.m_vstrRobotTeachPosPageName[i] == strItem)
		{
			m_vpDlgRobotTeachPos[i]->ShowWindow(SW_SHOW);
			break;
		}
	}

	if (m_RobotTeachRangeInfo.m_strRobotTeachSGPageName == strItem)
	{
		m_pDlgRobotTeachSG->ShowWindow(SW_SHOW);
	}

	m_nPrevTabIndex = nCurSel;

	*pResult = 0;
}

void CDlgRobotTeach::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	vcXMLConfigurator xml;
	CString strXMLPath;

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle")); 
	xml.EnterElem();

	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));

		if(_T("IDD_DLG_ROBOT_TEACH") == strDlgID)
		{
			int nID;
			CString strValue = xml.GetAttribute(_T("Value"));
			SetWindowText(strValue);

			xml.EnterElem();	   
			while(xml.FindElem())
			{
				xml.GetAttribute(_T("Id"),  nID);
				strValue = xml.GetAttribute(_T("Value"));

				if(GetDlgItem(nID) != NULL)
				{
					GetDlgItem(nID)->SetWindowText(strValue);
				}	  
			}
		}  
	}
	xml.LeaveElem();
}


void CDlgRobotTeach::OnBnClickedBtnPosRAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[R+]\n"));
	g_opLogFile.SaveLogText(strLog,2);
}
