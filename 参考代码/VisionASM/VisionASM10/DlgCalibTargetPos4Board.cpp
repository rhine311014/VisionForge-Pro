// DlgCalibTargetPos3456.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibTargetPos4Board.h"
#include "afxdialogex.h"
#include "DlgCalibTargetDmCodeParam.h"
#include "DlgCalibTargetShutterSetting.h"
#include "DlgSearch.h"
// CDlgCalibTargetPos4Board 对话框

IMPLEMENT_DYNAMIC(CDlgCalibTargetPos4Board, CDialog)


CDlgCalibTargetPos4Board::CDlgCalibTargetPos4Board(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibTargetPos4Board::IDD, pParent)
	, m_nRadioCalibTargetPos(0)
	, m_strCalibBoardTaget3PosX0(_T(""))
	, m_strCalibBoardTaget3PosY0(_T(""))
	, m_strCalibBoardTaget3PosX1(_T(""))
	, m_strCalibBoardTaget3PosY1(_T(""))
	, m_strCalibBoardTaget4PosX0(_T(""))
	, m_strCalibBoardTaget4PosY0(_T(""))
	, m_strCalibBoardTaget4PosX1(_T(""))

	, m_strCalibBoardSizeX(_T(""))
	, m_strCalibBoardSizeY(_T(""))
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
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;

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

	m_bIsDmCodeSearch = FALSE;

	for(int it=0;it<2;it++)
	{
		dShutterList[it] = 0.0;
	}
	m_bStopUpdateDisplayImage = FALSE;
}

CDlgCalibTargetPos4Board::~CDlgCalibTargetPos4Board()
{
	int i=0;

	if (m_bIsDmCodeSearch)
	{
		for (i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->RemoveGuiCalibrateGraphicsFromDisplay(i,m_vpGuiDisplay.at(i));
		}
		for (i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->RemoveGuiCalibrateGraphicsFromDisplay(i+m_pVisionASM->GetPosNum()/2,m_vpGuiTargetDisplay.at(i));
		}
	}

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


	for(i = 0; i<m_vpmpCalibMarkBoardPos.size();i++)
	{
		if (m_vpmpCalibMarkBoardPos.at(i) != NULL)
		{
			delete m_vpmpCalibMarkBoardPos.at(i);
			m_vpmpCalibMarkBoardPos.at(i) = NULL;
		}
	}

	m_pVisionASM = NULL;
}

void CDlgCalibTargetPos4Board::DoDataExchange(CDataExchange* pDX)
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
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, m_strCalibBoardTaget3PosX0);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, m_strCalibBoardTaget3PosY0);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, m_strCalibBoardTaget3PosX1);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, m_strCalibBoardTaget3PosY1);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, m_strCalibBoardTaget4PosX0);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, m_strCalibBoardTaget4PosY0);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, m_strCalibBoardTaget4PosX1);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, m_strCalibBoardTaget4PosY1);

	DDX_Text(pDX, IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, m_strCalibBoardSizeX);
	DDX_Text(pDX, IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, m_strCalibBoardSizeY);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboExProductIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalibTargetPos4Board, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SERIAL_GRAB, &CDlgCalibTargetPos4Board::OnBnClickedBtnSerialGrab)
	ON_BN_CLICKED(IDCANCEL, &CDlgCalibTargetPos4Board::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgCalibTargetPos4Board::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT0_MOVE_POS0, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject0Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT1_MOVE_POS0, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject1Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT0_MOVE_POS1, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject0Pos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject1Pos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET0_MOVE_POS0, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject0Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET1_MOVE_POS0, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject1Pos0)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET0_MOVE_POS1, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject0Pos1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET1_MOVE_POS1, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject1Pos1)
	ON_BN_CLICKED(IDC_BTN_CALIB_TARGET_POS, &CDlgCalibTargetPos4Board::OnBnClickedBtnCalcuVirtualObject)
	ON_BN_CLICKED(IDC_BTN_COMM_GET_PLATFORM_AXIS_POS, &CDlgCalibTargetPos4Board::OnBnClickedBtnCommGetPlatformAxisPos)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosX0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosX0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosY0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosY0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosX1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosX1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosY1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosY1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosX0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosX0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosY0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosY0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosX1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosX1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosY1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosY1)
	ON_BN_CLICKED(IDC_BTN_SEARCH_TARGET_ALL, &CDlgCalibTargetPos4Board::OnBnClickedBtnSearchTargetAll)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibBoardSizeX)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibBoardSizeX)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, &CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibBoardSizeY)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, &CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibBoardSizeY)
	ON_BN_CLICKED(IDC_RADIO_CALIB_TARGET_POS0, &CDlgCalibTargetPos4Board::OnBnClickedRadioCalibTargetPos0)
	ON_BN_CLICKED(IDC_RADIO_CALIB_TARGET_POS2, &CDlgCalibTargetPos4Board::OnBnClickedRadioCalibTargetPos0)
	ON_BN_CLICKED(IDC_RADIO_CALIB_TARGET_POS1, &CDlgCalibTargetPos4Board::OnBnClickedRadioCalibTargetPos0)
	ON_BN_CLICKED(IDC_BTN_SET_DMCODE_PARAM, &CDlgCalibTargetPos4Board::OnBnClickedBtnSetDmcodeParam)
	
	ON_BN_CLICKED(IDC_BUTTON_POS1_CAMERA_SHUTTER, &CDlgCalibTargetPos4Board::OnBnClickedSetCalibTargetShutter1)
	ON_BN_CLICKED(IDC_BUTTON_POS2_CAMERA_SHUTTER, &CDlgCalibTargetPos4Board::OnBnClickedSetCalibTargetShutter2)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)

END_MESSAGE_MAP()


// CDlgCalibTargetPos4Board 消息处理程序

void CDlgCalibTargetPos4Board::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
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

BOOL CDlgCalibTargetPos4Board::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	BOOL bEnable = m_bSysKeyboardEnabled;
	m_bSysKeyboardEnabled = FALSE;

	// 初始化界面
	// 	if (m_pVisionASM->GetCameraNum() != 4)
	// 	{
	// 		m_nType = 0;
	// 	}

	InitDlgItem();		

	// 初始化GUI
	//	SetGUI();

	//	UpdateBtnLiveGrab();

	//	UpdateShutterDisplay();

	//	m_bDisplayCross = TRUE;
	//	UpdateGuiDisplay();

//	UpdateData(FALSE);

	UpdateDlgData(FALSE);

	OnBnClickedBtnSerialGrab();

	m_pThreadReceiveData = AfxBeginThread(ThreadReceiveData, this);
	m_hEventReceiveThreadExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	m_bSysKeyboardEnabled = bEnable;

	UpdateDialogLanguage();

	if(m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);

		double dMin = 0, dMax = 0,dShutter = 0.0;
		for(int nPos=0;nPos<2 && nPos<m_pVisionASM->GetPosNum();nPos++)
		{

			if (m_pVisionASM->GetPosCameraShutter(nPos+m_pVisionASM->GetObjectPosNum() , dShutter,dMin,dMax))
			{
				dShutterList[nPos] = dShutter;
			}
			int nCam = m_pVisionASM->GetPosCameraIndex(nPos+m_pVisionASM->GetObjectPosNum());
			if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
			{
				double dCalibTargetShutter = vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter;
				// 没有设置关联曝光时;设置为系统曝光值
				if(dCalibTargetShutter<=0.01)
				{
					vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter = dShutter;
					dCalibTargetShutter = dShutter;
				}
				m_pVisionASM->SwitchToSearchShutter(nPos+m_pVisionASM->GetObjectPosNum(), dCalibTargetShutter);
			}
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCalibTargetPos4Board::InitDlgItem()
{
	// 	// 	CRect cr;
	// 	// 	GetClientRect(&cr);//获取对话框客户区域大小
	// 	// 	ClientToScreen(&cr);//转换为荧幕坐标
	// 	int x= GetSystemMetrics(SM_CXSCREEN);//获取荧幕坐标的宽度，单位为像素
	// 	int y= GetSystemMetrics(SM_CYSCREEN);//获取荧幕坐标的高度，单位为像素
	// 	MoveWindow(40, 30, (x - 80), (y - 60));//设置窗口显示在屏幕中间

	if (!m_pVisionASM->IsCommOpen())
	{
		m_pVisionASM->OpenComm();
	}

	if (eDmCodeCalib == m_pVisionASM->GetCalibTargetMarkPatternMode())
	{
		m_bIsDmCodeSearch = TRUE;
	}

	m_labelNote.SetBkColor(RGB(0,255,255));
	m_labelNote.SetFontSize(10);
	m_labelNote.SetFontBold(TRUE);
	m_labelNote.SetText(_T("注:请确保各拍照位置的平台θ轴位置一致！！！"));


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

	//	m_btnAssistSearch.SetIcon(IDI_ICON_EXTRACT_POINT_48);
	//	m_btnAssistSearch.SetAlign(CButtonST::ST_ALIGN_VERT);

	int i = 0;
	for (i=0; i<m_vpGuiDisplay.size(); i++)
	{
		delete m_vpGuiDisplay[i];
		m_vpGuiDisplay[i] = NULL;
	}
	m_vpGuiDisplay.clear();
	m_vpGuiInteractiveContainer.clear();


	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<m_pVisionASM->GetObjectPosNum(); i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i, this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		//pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		m_pVisionASM->SetPosDisplay(i, pGUIDisplay);

		m_vpGuiDisplay.push_back(pGUIDisplay);
		m_vpGuiDisplay.at(i)->ShowWindow(SW_HIDE);
		pGUIDisplay = NULL;
	}

	for (i=0; i<m_pVisionASM->GetTargetPosNum(); i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_CH0_DISPLAY+i+m_pVisionASM->GetObjectPosNum(), this,WS_VISIBLE);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		m_pVisionASM->SetPosDisplay(i+m_pVisionASM->GetObjectPosNum(), pGUIDisplay);
		m_vpGuiTargetDisplay.push_back(pGUIDisplay);
		m_vpGuiTargetDisplay.at(i)->ShowWindow(SW_SHOW);
		pGUIDisplay = NULL;
	}

	scGuiStaticContainer *pGuiSC = NULL;
	scGuiCoordCross* pGuiCoordCross = NULL;
	scGuiText *pGuiText = NULL;
	// 对象
	CString strKey;
	CString strInfo;
	int nMovePosIndex = 0;
	for(i = 0;  i<m_pVisionASM->GetObjectPosNum(); i++)
	{

		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));

		nMovePosIndex = 0;
		strInfo.Format(_T("移动位置%d"),nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnObjectImageMarkPos0.push_back(pGuiCoordCross);

		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));

		nMovePosIndex = 1;
		strInfo.Format(_T("移动位置%d"),nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnObjectImageMarkPos1.push_back(pGuiCoordCross);

		strInfo.Format(_T("对象位置%d"),i+1);
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiText.push_back(pGuiText);

		pGuiSC = m_vpGuiDisplay.at(i)->GetStaticContainer();

		nMovePosIndex = 0;
		strKey.Format(_T("ObjectPos%d_MovePosIndex%d"),i, nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos0.at(i), strKey);

		nMovePosIndex = 1;
		strKey.Format(_T("ObjectPos%d_MovePosIndex%d"),i, nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnObjectImageMarkPos1.at(i), strKey);

		strKey.Format(_T("GuiText%d"),i);
		pGuiSC->AddItem(m_vpGuiText.at(i), strKey);

		m_vpGuiDisplay.at(i)->SetStaticContainer(pGuiSC);
	}


	// 目标
	for(i = 0;  i<m_pVisionASM->GetTargetPosNum(); i++)
	{
		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));

		nMovePosIndex = 0;
		strInfo.Format(_T("移动位置%d"),nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnTargetImageMarkPos0.push_back(pGuiCoordCross);


		pGuiCoordCross = new scGuiCoordCross;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(255,0,0));

		nMovePosIndex = 1;
		strInfo.Format(_T("移动位置%d"),nMovePosIndex+1);
		pGuiCoordCross->SetLabel(strInfo);
		pGuiCoordCross->SetLabelVisible(TRUE);
		m_vpGuiAlnTargetImageMarkPos1.push_back(pGuiCoordCross);

		strInfo.Format(_T("目标位置%d"),i + 1 + m_pVisionASM->GetObjectPosNum());
		pGuiText = new scGuiText;
		pGuiText->SetXYRotationText(sc2Vector(10,10),scRadian(0),strInfo);
		pGuiText->SetLineColor(RGB(255,0,0));
		pGuiText->SetVisible(TRUE);
		m_vpGuiTargetText.push_back(pGuiText);

		pGuiSC = m_vpGuiTargetDisplay.at(i)->GetStaticContainer();

		nMovePosIndex = 0;
		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos0.at(i), strKey);

		nMovePosIndex = 1;
		strKey.Format(_T("TargetPos%d_MovePosIndex%d"),i,nMovePosIndex);
		pGuiSC->AddItem(m_vpGuiAlnTargetImageMarkPos1.at(i), strKey);

		strKey.Format(_T("GuiTargetText%d"),i);
		pGuiSC->AddItem(m_vpGuiTargetText.at(i), strKey);

		m_vpGuiTargetDisplay.at(i)->SetStaticContainer(pGuiSC);
	}

	m_vbCalibTargetPosSuccess.resize(m_pVisionASM->GetTargetPosNum(), FALSE);

	CMarkImagePos *pMarkBoardPos;
	for (i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		pMarkBoardPos = new CMarkImagePos;
		pMarkBoardPos->SetMarkImagePosNum(m_pVisionASM->GetPosNum());
		m_vpmpCalibMarkBoardPos.push_back(pMarkBoardPos);
	}

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	if (sysPlatformInfo.m_bTargetObjectCamSeparate && sysPlatformInfo.m_nPositionNum == 4)
	{
		UpdateDlgItem(TRUE);
	}
	else
	{
		UpdateDlgItem(FALSE);
	}
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		for (i=0; i<sysPlatformInfo.m_nMultiCalibExtensionMaxNum+1; i++)
		{
			CString strExProductIndex;
			strExProductIndex.Format(_T("%d"), i+1);
			m_comboExProductIndex.InsertString(i, strExProductIndex);
		}
		m_comboExProductIndex.SetCurSel(0);

		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
	}
	if (m_bIsDmCodeSearch)
	{
		for (int i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->AddGuiCalibrateGraphicsToDisplay(i,m_vpGuiDisplay.at(i));
		}
		for (int i=0;i<m_pVisionASM->GetPosNum()/2;i++)
		{
			m_pVisionASM->AddGuiCalibrateGraphicsToDisplay(i+m_pVisionASM->GetPosNum()/2,m_vpGuiTargetDisplay.at(i));
		}


		CTotalProductData *pTotalProductData;
		pTotalProductData = m_pVisionASM->GetTotalProuductData();
		int nCurProductIdx = pTotalProductData->GetCurProductIndex();
		// 获取当前产品对应的标定数据
		CProductData* pProductData;
		pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		scDMCodeParam DmCodeParam;
		for (int i=0;i<m_pVisionASM->GetPosNum();i++)
		{
			if(optionInfo.m_bEnableSearchShutter)
			{
				DmCodeParam = pProductData->m_vDMcodeCalib.at(i);
				if (DmCodeParam.m_dShutter >= 0.001 )
				{
					m_pVisionASM->SwitchToSearchShutter(i, DmCodeParam.m_dShutter);
				}
			}	
		}
	}


	// 	// 调整GUI显示
	// 	// 	scGuiDisplay *pGUIDisplay = NULL;
	// 	int nCamNum = m_pVisionASM->GetCameraNum();
	// 	nCamNum = (nCamNum == 4) ? 2 : nCamNum;
	// 	for (i=0; i<nCamNum; i++)
	// 	{
	// 		// 		pGUIDisplay = new scGuiDisplay;
	// 		m_vpGuiDisplay.at(i)->Create(IDC_STATIC_CH0_DISPLAY+i, this, WS_BORDER);
	// 		// 		m_vpGuiDisplay.push_back(pGUIDisplay);
	// 		// 		pGUIDisplay = NULL;
	// 
	// 		m_vpGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 	}

	// 	// 更新“连续采集/停止采集”按钮 
	// 	for (i=0; i<2; i++)
	// 	{
	// 		if (m_pVisionAligner->IsCameraGrabbing(i))
	// 		{
	// 			GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
	// 
	// 			GetDlgItem(IDOK)->EnableWindow(FALSE);
	// 			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	// 
	// // 			m_bDisplayCross = TRUE;
	// // 			UpdateGuiDisplay();
	// 		}
	// 		else
	// 		{
	// 			if (m_pVisionAligner->GrabCamera(i))
	// 			{
	// 				GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
	// 				GetDlgItem(IDOK)->EnableWindow(FALSE);
	// 				GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	// 
	// // 				m_bDisplayCross = FALSE;
	// // 				UpdateGuiDisplay();
	// 			}
	// 			else
	// 			{
	// 				GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
	// 				GetDlgItem(IDOK)->EnableWindow(TRUE);
	// 				GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	// 
	// // 				m_bDisplayCross = TRUE;
	// // 				UpdateGuiDisplay();
	// 			}
	// 		}
	// 	}
}

void CDlgCalibTargetPos4Board::UpdateDlgItem(BOOL bEnable)
{
	GetDlgItem(IDC_BTN_SERIAL_GRAB)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_CALIB_TARGET_POS0)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_CALIB_TARGET_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_CALIB_TARGET_POS2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_COMM_GET_PLATFORM_AXIS_POS)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_OBJECT0_MOVE_POS0)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS0)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_OBJECT0_MOVE_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_OBJECT1_MOVE_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEARCH_TARGET_ALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->EnableWindow(bEnable);
	GetDlgItem(IDOK)->EnableWindow(bEnable);

	if (m_bIsDmCodeSearch)
	{
		m_bitMap.LoadBitmap(IDB_BITMAP_DMCODE);
		((CStatic*)GetDlgItem(IDC_STATIC_BOARD_IMAGE))->SetBitmap(HBITMAP(m_bitMap));
		GetDlgItem(IDC_STATIC_GRP_GRID_SIZE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GRID_SIZE_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GRID_SIZE_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GRP_CALIBRATION_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET3_POS0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET3_POS1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET4_POS0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET4_POS1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET_POS_X2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET_POS_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET_POS_X4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALIBRATION_TARGET_POS_X3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SEARCH_TARGET_ALL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_SET_DMCODE_PARAM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_COMM_GET_PLATFORM_AXIS_POS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->ShowWindow(SW_SHOW);
	}
}



void CDlgCalibTargetPos4Board::UpdateDlgData(BOOL bSaveAndValidate/* = TRUE*/)
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (bSaveAndValidate)
	{
		// 将界面的数据跟新保存到配置数据中
		UpdateData(TRUE);

		int i = 0;
		int nCalibIndex = 0;
		CCoordPos pos;


		// 靶标实际物理位置
		nCalibIndex = 0;
		i = 0;
		pos.m_dPosX = 0;
		pos.m_dPosY = 0;		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);
		i = 1;
		pos.m_dPosX = 0;
		pos.m_dPosY = 0;		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);

		nCalibIndex = 1;
		i = 0;
		pos.m_dPosX = 0;
		pos.m_dPosY = 0;		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);
		i = 1;
		pos.m_dPosX = 0;
		pos.m_dPosY = 0;		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);

		nCalibIndex = 2;
		i = 0;
		pos.m_dPosX = atof(m_strCalibBoardTaget3PosX0);
		pos.m_dPosY = atof(m_strCalibBoardTaget3PosY0);		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);
		i = 1;
		pos.m_dPosX = atof(m_strCalibBoardTaget3PosX1);
		pos.m_dPosY = atof(m_strCalibBoardTaget3PosY1);		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);

		nCalibIndex = 3;
		i = 0;
		pos.m_dPosX = atof(m_strCalibBoardTaget4PosX0);
		pos.m_dPosY = atof(m_strCalibBoardTaget4PosY0);		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);
		i = 1;
		pos.m_dPosX = atof(m_strCalibBoardTaget4PosX1);
		pos.m_dPosY = atof(m_strCalibBoardTaget4PosY1);		
		m_vpmpCalibMarkBoardPos.at(i)->SetMarkImagePos(nCalibIndex,TRUE,pos);

		double dSizeX = atof(m_strCalibBoardSizeX);
		double dSizeY = atof(m_strCalibBoardSizeY);
		m_vCalibBoardSize = sc2Vector(dSizeX, dSizeY);
	}
	else
	{
		// 将配置数据更新到界面上
		CTotalProductData* pTotalProductData = m_pVisionASM->GetTotalProuductData();
		CTotalCalibData* pTotalCalibData = m_pVisionASM->GetTotalCalibData();
		if (pTotalProductData == NULL || pTotalCalibData == NULL)
		{
			return;
		}
		CCalibData* pCalibData = pTotalCalibData->GetItemPtr(pTotalProductData->GetCurProductIndex());
		if (pCalibData == NULL)
		{
			return;
		}
		int i = 0;
		int nCalibIndex = 0;

		CCoordPos	cpCalibBoardMarkImgCoordPos[CALIBBOARDMARKPOSNUM]; // 用靶标关联映射时，标定板上的Mark位置图像坐标
		CCoordPos	cpCalibBoardMarkBoardCoordPos[CALIBBOARDMARKPOSNUM]; // 用靶标

		
		nCalibIndex = 2;
		if (nCalibIndex < pCalibData->m_vpCalibratedInfo.size() 
			&& pCalibData->m_vpCalibratedInfo.at(nCalibIndex) != NULL)
		{
			pCalibData->m_vpCalibratedInfo.at(nCalibIndex)->GetCalibBoardMarkBoardCoordPos(cpCalibBoardMarkBoardCoordPos);

			i = 0;
			m_strCalibBoardTaget3PosX0.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosX());
			m_strCalibBoardTaget3PosY0.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosY());
			i = 1;
			m_strCalibBoardTaget3PosX1.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosX());
			m_strCalibBoardTaget3PosY1.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosY());

			sc2Vector vCalibBoardSize;
			pCalibData->m_vpCalibratedInfo.at(nCalibIndex)->GetCalibBoardSize(vCalibBoardSize);
			m_strCalibBoardSizeX.Format(_T("%.4f"), vCalibBoardSize.GetX());
			m_strCalibBoardSizeY.Format(_T("%.4f"), vCalibBoardSize.GetY());

		}
		else
		{
			i = 0;
			m_strCalibBoardTaget3PosX0.Format(_T("0.0000"));
			m_strCalibBoardTaget3PosY0.Format(_T("0.0000"));
			i = 1;
			m_strCalibBoardTaget3PosX1.Format(_T("0.0000"));
			m_strCalibBoardTaget3PosY1.Format(_T("0.0000"));

			m_strCalibBoardSizeX.Format(_T("1.0"));
			m_strCalibBoardSizeY.Format(_T("1.0"));


		}


		nCalibIndex = 3;
		if (nCalibIndex < pCalibData->m_vpCalibratedInfo.size() 
			&& pCalibData->m_vpCalibratedInfo.at(nCalibIndex) != NULL)
		{
			pCalibData->m_vpCalibratedInfo.at(nCalibIndex)->GetCalibBoardMarkBoardCoordPos(cpCalibBoardMarkBoardCoordPos);

			i = 0;
			m_strCalibBoardTaget4PosX0.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosX());
			m_strCalibBoardTaget4PosY0.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosY());
			i = 1;
			m_strCalibBoardTaget4PosX1.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosX());
			m_strCalibBoardTaget4PosY1.Format(_T("%.4f"), cpCalibBoardMarkBoardCoordPos[i].GetPosY());
		}
		else
		{
			i = 0;
			m_strCalibBoardTaget4PosX0.Format(_T("0.0000"));
			m_strCalibBoardTaget4PosY0.Format(_T("0.0000"));
			i = 1;
			m_strCalibBoardTaget4PosX1.Format(_T("0.0000"));
			m_strCalibBoardTaget4PosY1.Format(_T("0.0000"));
		}


		UpdateData(FALSE);
	}
}

BOOL CDlgCalibTargetPos4Board::IsStringNumerical( LPCTSTR lpszVal )
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


void CDlgCalibTargetPos4Board::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}


void CDlgCalibTargetPos4Board::OnDestroy()
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


LRESULT CDlgCalibTargetPos4Board::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			//m_pVisionASM->UpdateDisplayImage((int)wParam);
			if(m_bStopUpdateDisplayImage==FALSE)
			{
				m_pVisionASM->UpdateDisplayImage((int)wParam);
			}
		}
		break;
	case WM_UPDATE_STATUSBAR_INFO:
		{
			m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1,m_strStatusBarInfo2);
			UpdateStatusBar();
		}
		break;
	case WM_UPDATE_CALIBRATE_TARGET_GRAPHIC:
		{
			//m_pVisionASM->UpdateAlnObjectGraphics((int)wParam, (BOOL)lParam);
			m_pVisionASM->ShowGuiCalibrateGraphics((int)wParam, (BOOL)lParam);
			int nPosIndex = (int)wParam;
			if ( nPosIndex < m_pVisionASM->GetPosNum()/2)
			{
				m_vpGuiDisplay.at(nPosIndex)->Invalidate();
			}
			else
			{
				m_vpGuiTargetDisplay.at(nPosIndex - m_pVisionASM->GetPosNum()/2)->Invalidate();
			}

		}
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}



void CDlgCalibTargetPos4Board::UpdateBtnLiveGrab()
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

void CDlgCalibTargetPos4Board::UpdateShutterDisplay()
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




BOOL CDlgCalibTargetPos4Board::SetGUI()
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
		strInfo.Format(_T("CH%d通道目标Mark点"), i+(m_nType<<1));
		m_vpGuiCross.at(i)->SetLabel(strInfo);
		m_vpGuiCross.at(i)->SetLabelVisible(TRUE);

		strInfo.Format(_T("ManualTargetCross%d"), i+(m_nType<<1));
		m_vpGuiInteractiveContainer.at(i)->AddItem(m_vpGuiCross.at(i), strInfo);

		m_vpGuiDisplay.at(i)->SetInteractiveContainer(m_vpGuiInteractiveContainer.at(i));
	}

	return TRUE;	
}

// 更新训练Gui控件显示
bool CDlgCalibTargetPos4Board::UpdateGuiDisplay()
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

void CDlgCalibTargetPos4Board::OnBnClickedBtnSerialGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	int i=0;
	BOOL bSucceed = TRUE;
	int nCamNum = m_pVisionASM->GetPosNum(); 
	std::vector<BOOL> vbSucceed;
	if (m_bIsGrabbing)
	{
		CString strLog;
		strLog.Format(_T("点击[连续采集]\n"));
		g_opLogFile.SaveLogText(strLog, 2);
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
// 					strInfo.Format(_T("位置%d、"), i+1);
// 					strNote += strInfo;
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
		CString strLog;
		strLog.Format(_T("点击[停止采集]\n"));
		g_opLogFile.SaveLogText(strLog, 2);
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
// 					strInfo.Format(_T("位置%d、"), i+1);
// 					strNote += strInfo;

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


void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchTargetAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nEvent = 6; // 全部移动位置1
	CString strLog;
	strLog.Format(_T("点击[搜索目标位置]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}

void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject0Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 1; // 对象移动位置1
	CString strLog;
	strLog.Format(_T("点击[搜索目标5位置1]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject1Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 1; // 对象移动位置1
	CString strLog;
	strLog.Format(_T("点击[搜索目标6位置1]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject0Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 2; // 目标移动位置1
	CString strLog;
	strLog.Format(_T("点击[搜索目标3位置1]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject1Pos0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 2; // 目标移动位置1
	CString strLog;
	strLog.Format(_T("点击[搜索目标4位置1]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}

void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject0Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 3; // 对象移动位置2
	CString strLog;
	strLog.Format(_T("点击[搜索目标5位置2]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}

void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchObject1Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 3; // 对象移动位置2
	CString strLog;
	strLog.Format(_T("点击[搜索目标6位置2]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject0Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 1;
	m_nEvent = 4; // 目标移动位置2
	CString strLog;
	strLog.Format(_T("点击[搜索目标3位置2]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos4Board::OnBnClickedBtnSearchVirtualObject1Pos1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nRadioCalibTargetPos = 2;
	m_nEvent = 4; // 目标移动位置2
	CString strLog;
	strLog.Format(_T("点击[搜索目标4位置2]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}



void CDlgCalibTargetPos4Board::OnBnClickedBtnCommGetPlatformAxisPos()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("请确认平台轴当前位置为对位拍照基准位置（平台对象拍照位置）!"));

	m_nEvent = 5; // 获取平台基准位置（标定位置、接料位置、对位拍照基准位置）
	CString strLog;
	strLog.Format(_T("点击[获取平台基准位置]\n"));
	g_opLogFile.SaveLogText(strLog, 2);
}


void CDlgCalibTargetPos4Board::OnBnClickedBtnCalcuVirtualObject()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[标定目标位置]\n"));
	g_opLogFile.SaveLogText(strLog, 2);

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	if (!m_bIsDmCodeSearch)
	{
		UpdateDlgData(TRUE);
	}
	int nExProductIndex = 0;
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		nExProductIndex = m_comboExProductIndex.GetCurSel();
	}
	m_nRadioCalibTargetPos = 0;

	if (m_pVisionASM == NULL)
	{
		return;
	}

	if (!m_bCommGetPlatformAxisPosSuccess)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T("标定失败"));
		m_strStatusBarInfo2.Format(_T("请获取平台轴位置！"));

		UpdateStatusBar();

		return;
	}

	if (m_nRadioCalibTargetPos == 0)			// 同时标定目标位置
	{
		if (m_bIsDmCodeSearch)
		{
			for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size()/2;i++)
			{
				if (FALSE == m_pVisionASM->m_vDmCodeCalibTool.at(i+m_pVisionASM->m_vDmCodeCalibTool.size()/2).IsSearchValid())
				{
					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_CALIBRATE_FAILE);// .Format(_T("标定失败"));
					m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_GET_OBJECT_AND_TARGET_POS);//.Format(_T("请获取对象、目标位置！"));

					UpdateStatusBar();
					return;
				}
			}
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Empty();
			m_strStatusBarInfo2.Empty();
			std::vector<sc2Vector> vImagePos;
			std::vector<sc2Vector> vBoardPos;

			CString strTempInfo1;
			CString strTempInfo2;
			int nPosIndex = 0;
			for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
			{
				int nMarkType = 1;
				m_pVisionASM->m_vDmCodeCalibTool.at(nPosIndex + m_pVisionASM->GetPosNum()/2).GetCornersImagePos(vImagePos);
				m_pVisionASM->m_vDmCodeCalibTool.at(nPosIndex + m_pVisionASM->GetPosNum()/2).GetCornersLabelPos(vBoardPos);
				if (TRUE == m_pVisionASM->ExecuteCalibrateDmCodeBoard(nPosIndex + m_pVisionASM->GetPosNum()/2+m_pVisionASM->GetPosNum()*nExProductIndex,vImagePos,vBoardPos,nMarkType))
				{
					m_vbCalibTargetPosSuccess.at(nPosIndex) = TRUE;
				}
				else
				{
					m_vbCalibTargetPosSuccess.at(nPosIndex) = FALSE;
				}
			}

			m_bStatusBarFlag = m_vbCalibTargetPosSuccess.at(0);
			for(nPosIndex =  0; nPosIndex < m_pVisionASM->GetPosNum()/2; nPosIndex++)
			{
				m_bStatusBarFlag = m_bStatusBarFlag && m_vbCalibTargetPosSuccess.at(nPosIndex);
				if (TRUE == m_vbCalibTargetPosSuccess.at(nPosIndex))
				{
					strTempInfo2.Format(_T("目标位置%d标定成功！"), nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
				}
				else
				{
					strTempInfo2.Format(_T("目标位置%d标定失败！"), nPosIndex + 1 + m_pVisionASM->GetPosNum()/2);
				}
				m_strStatusBarInfo2 += strTempInfo2;
			}

			if (m_bStatusBarFlag)
			{
				m_strStatusBarInfo1.Format(_T("标定成功"));
			}
			else
			{
				m_strStatusBarInfo1.Format(_T("标定失败"));
			}
			UpdateStatusBar();
		}
		else
		{
			{			
				if (m_vpmpCalibMarkImagePos.size() < 2)
				{
					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1.Format(_T("标定失败"));
					m_strStatusBarInfo2.Format(_T("请获取对象、目标位置！"));

					UpdateStatusBar();

					return;
				}

				int nPosIndex = 0;
				int nMovePosIndex = 0;
				for (nMovePosIndex = 0; nMovePosIndex < m_vpmpCalibMarkImagePos.size(); nMovePosIndex++)
				{
					for (nPosIndex = m_pVisionASM->GetObjectPosNum(); nPosIndex < m_pVisionASM->GetPosNum(); nPosIndex++)
					{
						if (FALSE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
						{
							m_bStatusBarFlag = FALSE;
							m_strStatusBarInfo1.Format(_T("标定失败"));
							m_strStatusBarInfo2.Format(_T("请获取目标位置的标记位置！"));

							UpdateStatusBar();
							return;
						}
					}	
				}
			}

			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Empty();
			m_strStatusBarInfo2.Empty();
			sc2Vector vBoardSize;
			vBoardSize = sc2Vector(m_vCalibBoardSize.GetX(), m_vCalibBoardSize.GetY());

			CString strTempInfo1;
			CString strTempInfo2;
			int nPosIndex = 0;
			for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetTargetPosNum(); nPosIndex++)
			{
				int nMarkType = 1;
				if (TRUE == m_pVisionASM->ExecuteCalibrateBoard(m_vpmpCalibMarkImagePos, m_vpmpCalibMarkBoardPos, vBoardSize, nPosIndex + m_pVisionASM->GetObjectPosNum()+m_pVisionASM->GetPosNum()*nExProductIndex, nMarkType))
				{
					m_vbCalibTargetPosSuccess.at(nPosIndex) = TRUE;
				}
				else
				{
					m_vbCalibTargetPosSuccess.at(nPosIndex) = FALSE;
				}
			}

			m_bStatusBarFlag = m_vbCalibTargetPosSuccess.at(0);
			for(nPosIndex =  0; nPosIndex < m_pVisionASM->GetTargetPosNum(); nPosIndex++)
			{
				m_bStatusBarFlag = m_bStatusBarFlag && m_vbCalibTargetPosSuccess.at(nPosIndex);
				if (TRUE == m_vbCalibTargetPosSuccess.at(nPosIndex))
				{
					strTempInfo2.Format(_T("目标位置%d标定成功！"), nPosIndex + 1 + m_pVisionASM->GetObjectPosNum());
				}
				else
				{
					strTempInfo2.Format(_T("目标位置%d标定失败！"), nPosIndex + 1 + m_pVisionASM->GetObjectPosNum());
				}
				m_strStatusBarInfo2 += strTempInfo2;
			}

			if (m_bStatusBarFlag)
			{
				m_strStatusBarInfo1.Format(_T("标定成功"));
			}
			else
			{
				m_strStatusBarInfo1.Format(_T("标定失败"));
			}
			UpdateStatusBar();
		}
	}
	else if (m_nRadioCalibTargetPos > 0)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Empty();
		m_strStatusBarInfo2.Empty();

		sc2Vector vBoardSize;
		vBoardSize = sc2Vector(1.0, 1.0);

		CString strTempInfo1;
		CString strTempInfo2;
		int nPosIndex = 0;

		nPosIndex = m_nRadioCalibTargetPos - 1;
		int nMarkType = 1;
		if (TRUE == m_pVisionASM->ExecuteCalibrateBoard(m_vpmpCalibMarkImagePos, m_vpmpCalibMarkBoardPos, vBoardSize, nPosIndex + m_pVisionASM->GetObjectPosNum()+m_pVisionASM->GetPosNum()*nExProductIndex, nMarkType))
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
			strTempInfo2.Format(_T("目标位置%d标定成功！"), nPosIndex + 1 + m_pVisionASM->GetObjectPosNum());
		}
		else
		{
			strTempInfo2.Format(_T("目标位置%d标定失败！"), nPosIndex + 1 + m_pVisionASM->GetObjectPosNum());
		}
		m_strStatusBarInfo2 += strTempInfo2;


		if (m_bStatusBarFlag)
		{
			m_strStatusBarInfo1.Format(_T("标定成功"));
		}
		else
		{
			m_strStatusBarInfo1.Format(_T("标定失败"));
		}
		UpdateStatusBar();
	}

}


void CDlgCalibTargetPos4Board::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[保存退出]标定目标设置\n"));
	g_opLogFile.SaveLogText(strLog, 1);
	if (m_pVisionASM == NULL)
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T("保存失败"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();
	}

	m_bCalibTargetPosSuccess = FALSE;
	int nPosIndex = 0;
	m_bCalibTargetPosSuccess = m_vbCalibTargetPosSuccess.at(nPosIndex);
	for(nPosIndex = 0; nPosIndex < m_pVisionASM->GetTargetPosNum(); nPosIndex++)
	{
		m_bCalibTargetPosSuccess = m_bCalibTargetPosSuccess && m_vbCalibTargetPosSuccess.at(nPosIndex);	
	}

	if (m_bCalibTargetPosSuccess)
	{
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		//执行复制标定
		if(optionInfo.m_bAutoCopyCalibData)
		{
			SysPlatformInfo sysPlatformInfo;
			m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
			int nExProductIndex = 0;
			if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
			{
				nExProductIndex = m_comboExProductIndex.GetCurSel();
			}
			m_pVisionASM->ExecuteCopyCalibrateData(2,nExProductIndex);
		}
		else
		{
			// 获取系统标定后的所有信息
			std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
			vpAllCalibratedInfo = m_pVisionASM->GetAllCalibratedInfo();	

			/// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
			m_pVisionASM->SetCurCalibratedInfo(vpAllCalibratedInfo);	

			// 保存标定后信息到当前产品中及配置文件中
			m_pVisionASM->SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);

		}

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1.Format(_T("保存成功"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();

		if (/*sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE 
			&& m_pVisionASM->m_pSystempOptionConfig->m_bEnableCommMultiCalibExtension == TRUE
			&&*/ optionInfo.m_bSaveCalibData)
		{
			if (m_bIsDmCodeSearch)
			{
				int nCurExProductIndex = 0;

				nCurExProductIndex = m_comboExProductIndex.GetCurSel();

				// 保存畸变校正图片
				int nCurProdcutIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
				CString strImageFileDirt;
				char ch = '\\';
				strImageFileDirt.Format(m_pVisionASM->GetDataRecordDirectory() + _T("\\CalibRecordData\\ProdcutIndex%d\\GuanLian\\Ex%d"),nCurProdcutIndex,nCurExProductIndex);
				CreateMultipleDirectory(strImageFileDirt,ch);

				for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
				{
					//1.保存图片
					CString strImageName;
					strImageName.Format(_T(" Pos%d_ImageInput.bmp"),i);
					SaveCalibSearchImage(strImageFileDirt,strImageName,m_pVisionASM->m_vDmCodeCalibTool.at(i).m_imageInput);

					//2.保存角点
					std::vector<sc2Vector> vCornersImagePos;
					m_pVisionASM->m_vDmCodeCalibTool.at(i).GetCornersImagePos(vCornersImagePos);

					SYSTEMTIME sys;
					GetLocalTime(&sys);
					CString strTmp;
					strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
					CString strCornersPosFileName;
					strCornersPosFileName.Format(_T("\\%s Pos%d_InterCrossPoint.txt"),strTmp,i);
					CString strImagePointData;
					strImagePointData.Format(_T("\n%d\n"),vCornersImagePos.size());
					for(int nPointIndex = 0 ;nPointIndex < vCornersImagePos.size();nPointIndex ++)
					{
						strImagePointData.AppendFormat(_T("%0.2lf,%0.2lf\n"),vCornersImagePos[nPointIndex].GetX(),vCornersImagePos[nPointIndex].GetY());
					}
					m_pVisionASM->m_vDmCodeCalibTool.at(i).VisionAlignLogRecord(strImageFileDirt+strCornersPosFileName,strImagePointData);
				}
				//3.关联结果
				CString strSaveData;
				GetCalibResultCString(strSaveData);
				SaveCSVFile(strImageFileDirt,_T("GuanLian_Result"), strSaveData);
			}
		}

		if (m_bIsDmCodeSearch)
		{
			for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
			{
				m_pVisionASM->m_vDmCodeCalibTool.at(i).SetSearchValid(FALSE);
				m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearSearchResult();
				m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearRelationResult();
			}
		}

	}
	else
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1.Format(_T("保存失败"));
		m_strStatusBarInfo2.Format(_T(""));
		UpdateStatusBar();

		return;
	}

	ResetCameraShutter();

	CDialog::OnOK();
}


void CDlgCalibTargetPos4Board::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[不保存退出]标定目标设置\n"));
	g_opLogFile.SaveLogText(strLog, 1);

	if (m_bIsDmCodeSearch)
	{
		for (int i=0;i<m_pVisionASM->m_vDmCodeCalibTool.size();i++)
		{
			m_pVisionASM->m_vDmCodeCalibTool.at(i).SetSearchValid(FALSE);
			m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearSearchResult();
			m_pVisionASM->m_vDmCodeCalibTool.at(i).ClearRelationResult();
		}
	}

	ResetCameraShutter();

	CDialog::OnCancel();
}



UINT CDlgCalibTargetPos4Board::ThreadReceiveData( LPVOID pParam )
{
	CDlgCalibTargetPos4Board *pOwner = (CDlgCalibTargetPos4Board*)pParam;
	CString strReceived;
	while (!pOwner->m_bExitThread)
	{
		if (pOwner->m_nEvent == 1)		// 对象移动位置1
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;
			if (pOwner->m_nRadioCalibTargetPos == 0)
			{
				for(nPosIndex = 4; nPosIndex < pOwner->m_pVisionASM->GetPosNum(); nPosIndex++)
				{
					pOwner->SearchTargetPos(nPosIndex);
				}
			}
			else if (pOwner->m_nRadioCalibTargetPos > 0)
			{
				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1 + 4;
				pOwner->SearchTargetPos(nPosIndex);
			}
			else
			{
			}

		}		
		else if (pOwner->m_nEvent == 2)	// 目标移动位置1
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;
			if (pOwner->m_nRadioCalibTargetPos == 0)
			{
				for(nPosIndex = 0; nPosIndex < pOwner->m_pVisionASM->GetTargetPosNum(); nPosIndex++)
				{
					pOwner->SearchTargetPos(nPosIndex + pOwner->m_pVisionASM->GetObjectPosNum());
				}
			}
			else if (pOwner->m_nRadioCalibTargetPos > 0)
			{
				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1;
				if (pOwner->m_bIsDmCodeSearch)
				{
					pOwner->DmCodeSearch(nPosIndex+pOwner->m_pVisionASM->GetPosNum()/2);
				}
				else
				{
					pOwner->SearchTargetPos(nPosIndex + pOwner->m_pVisionASM->GetObjectPosNum());
				}
				
			}
			else
			{
			}
		}
		else if (pOwner->m_nEvent == 3)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;
			if (pOwner->m_nRadioCalibTargetPos == 0)
			{
				for(nPosIndex = 4; nPosIndex < pOwner->m_pVisionASM->GetPosNum(); nPosIndex++)
				{
					pOwner->SearchTargetPos1(nPosIndex);
				}
			}
			else if (pOwner->m_nRadioCalibTargetPos > 0)
			{
				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1 + 4;
				pOwner->SearchTargetPos1(nPosIndex);
			}
			else
			{
			}
		}
		else if (pOwner->m_nEvent == 4)
		{
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;
			if (pOwner->m_nRadioCalibTargetPos == 0)
			{
				for(nPosIndex = 0; nPosIndex < pOwner->m_pVisionASM->GetTargetPosNum(); nPosIndex++)
				{
					pOwner->SearchTargetPos1(nPosIndex + pOwner->m_pVisionASM->GetObjectPosNum());
				}
			}
			else if (pOwner->m_nRadioCalibTargetPos > 0)
			{
				nPosIndex = pOwner->m_nRadioCalibTargetPos - 1;
				pOwner->SearchTargetPos1(nPosIndex + pOwner->m_pVisionASM->GetObjectPosNum());
			}
			else
			{
			}
		}
		else if (pOwner->m_nEvent == 5) // 获取平台轴位置
		{
			pOwner->m_nEvent = 0;
			pOwner->CommGetPlatformAxisPos();
		}
		else if (pOwner->m_nEvent == 6)
		{						
			pOwner->m_nEvent = 0;
			int nPosIndex = 0;				
			for(nPosIndex = pOwner->m_pVisionASM->GetObjectPosNum(); nPosIndex < pOwner->m_pVisionASM->GetPosNum(); nPosIndex++)
			{
				// 目标移动位置1
				pOwner->SearchTargetPos(nPosIndex);
				// 目标移动位置2
				pOwner->SearchTargetPos1(nPosIndex);
			}				
		}
		Sleep(100);
	}

	SetEvent(pOwner->m_hEventReceiveThreadExit);
	return 0;
}

void CDlgCalibTargetPos4Board::SearchObjectPos(int nPosIndex)
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

	if (TRUE == m_pVisionASM->SingleCameraCalibBoardAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
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


void CDlgCalibTargetPos4Board::SearchObjectPos1(int nPosIndex)
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

	if (TRUE == m_pVisionASM->SingleCameraCalibBoardAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
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

void CDlgCalibTargetPos4Board::SearchTargetPos(int nPosIndex)
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
	m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(FALSE);
	if (TRUE == m_pVisionASM->SingleCameraCalibBoardAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
	{
		m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

		if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
			&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

			m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(0),100,100);
			m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineColor(RGB(255,0,0));
			m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineWidth(1);
			m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(TRUE);
		}
	}
	else
	{
		return;
	}
}



void CDlgCalibTargetPos4Board::SearchTargetPos1(int nPosIndex)
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
	m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(FALSE);
	if (TRUE == m_pVisionASM->SingleCameraCalibBoardAlignerTargetSearchExFTGT(nPosIndex, nMovePosIndex))
	{
		m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();

		if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
			&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
		{
			CCoordPos pos;
			pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

			m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetCenterRotationLengths(
				sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(0),100,100);
			m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineColor(RGB(255,0,0));
			m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineWidth(1);
			m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(TRUE);
		}
	};
}

void CDlgCalibTargetPos4Board::CommGetPlatformAxisPos()
{
	m_bCommGetPlatformAxisPosSuccess = FALSE;
	if (m_pVisionASM == NULL)
	{
		return;
	}

	// 通信获取平台轴位置
	m_bCommGetPlatformAxisPosSuccess = m_pVisionASM->GetCalibAlignerTargetPlatformAxisPos();

}

void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosX0()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标3位置1][列号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget3PosX0();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosX0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, m_strCalibBoardTaget3PosX0);
		}
		else
		{
			m_strCalibBoardTaget3PosX0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X0, m_strCalibBoardTaget3PosX0);
		}
	}	
}

void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosY0()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标3位置1][行号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget3PosY0();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosY0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, m_strCalibBoardTaget3PosY0);
		}
		else
		{
			m_strCalibBoardTaget3PosY0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y0, m_strCalibBoardTaget3PosY0);
		}
	}	
}


void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosX1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标3位置2][列号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget3PosX1();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosX1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, m_strCalibBoardTaget3PosX1);
		}
		else
		{
			m_strCalibBoardTaget3PosX1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_X1, m_strCalibBoardTaget3PosX1);
		}
	}	
}

void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget3PosY1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标3位置2][行号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget3PosY1();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget3PosY1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, m_strCalibBoardTaget3PosY1);
		}
		else
		{
			m_strCalibBoardTaget3PosY1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET3_POS_Y1, m_strCalibBoardTaget3PosY1);
		}
	}	
}


void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosX0()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标4位置1][列号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget4PosX0();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosX0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, m_strCalibBoardTaget4PosX0);
		}
		else
		{
			m_strCalibBoardTaget4PosX0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X0, m_strCalibBoardTaget4PosX0);
		}
	}	
}

void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosY0()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标4位置1][行号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget4PosY0();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosY0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, m_strCalibBoardTaget4PosY0);
		}
		else
		{
			m_strCalibBoardTaget4PosY0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y0, m_strCalibBoardTaget4PosY0);
		}
	}	
}


void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosX1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标4位置2][列号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget4PosX1();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosX1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, m_strCalibBoardTaget4PosX1);
		}
		else
		{
			m_strCalibBoardTaget4PosX1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_X1, m_strCalibBoardTaget4PosX1);
		}
	}	
}

void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibrationTarget4PosY1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标4位置2][行号]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibrationTarget4PosY1();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibrationTarget4PosY1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, m_strCalibBoardTaget4PosY1);
		}
		else
		{
			m_strCalibBoardTaget4PosY1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIBRATION_TARGET4_POS_Y1, m_strCalibBoardTaget4PosY1);
		}
	}	
}


void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibBoardSizeX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, strOutput);
			CString strLog;
			strLog.Format(_T("修改[靶标尺寸间隔设置][X]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog, 2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibBoardSizeX();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibBoardSizeX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, m_strCalibBoardSizeX);
		}
		else
		{
			m_strCalibBoardSizeX.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_X, m_strCalibBoardSizeX);
		}
	}
}


void CDlgCalibTargetPos4Board::OnEnSetfocusEditValCalibBoardSizeY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[靶标尺寸间隔设置][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog, 2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCalibBoardSizeY();
	}	
}


void CDlgCalibTargetPos4Board::OnEnKillfocusEditValCalibBoardSizeY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, m_strCalibBoardSizeY);
		}
		else
		{
			m_strCalibBoardSizeY.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CALIB_BOARD_SIZE_Y, m_strCalibBoardSizeY);
		}
	}
}

void CDlgCalibTargetPos4Board::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

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

		if(_T("IDD_DLG_CALIB_TARGET_POS4_BOARD") == strDlgID)
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
			xml.LeaveElem();
		}  
	}
	xml.LeaveElem();
	if(m_bIsDmCodeSearch)
	{
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->SetWindowTextA(_T("Target position3"));
			GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->SetWindowTextA(_T("Target position4"));
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置3"));
			GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置4"));
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			GetDlgItem(IDC_BTN_SEARCH_TARGET0_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置3"));
			GetDlgItem(IDC_BTN_SEARCH_TARGET1_MOVE_POS0)->SetWindowTextA(_T("搜索目标位置4"));
			break;
		}
	}
}

void CDlgCalibTargetPos4Board::OnBnClickedRadioCalibTargetPos0()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRadioCalibTargetPosOld = m_nRadioCalibTargetPos;
	UpdateData(TRUE);
	CString strLog;
	if (nRadioCalibTargetPosOld != m_nRadioCalibTargetPos)
	{
		CString strVal[] = {_T("标定所有目标位置"),_T("标定目标位置3(左)"),_T("标定目标位置4(右)")};
		strLog.Format(_T("修改[标定目标位置]由%s到%s\n"), strVal[nRadioCalibTargetPosOld],strVal[m_nRadioCalibTargetPos]);
		g_opLogFile.SaveLogText(strLog,2);
	}
}

BOOL CDlgCalibTargetPos4Board::DmCodeSearch(int nPosIndex)
{
	if (m_pVisionASM == NULL)
	{
		return FALSE;
	}
	if (nPosIndex < 0 || nPosIndex >= m_pVisionASM->GetPosNum())
	{
		return FALSE;
	}
	//m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(FALSE);
	//m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(FALSE);
	if (FALSE == m_pVisionASM->SingleCameraCalibAlignerTargetSearchExDmCode(nPosIndex))
	{
		return FALSE;
	}

	//UpdateDmCodeSearchResult(nPosIndex);

	//int nMovePosIndex = 0;
	//{

	//	if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
	//		&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
	//	{
	//		CCoordPos pos;
	//		pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

	//		m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetCenterRotationLengths(
	//			sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(45),100,100);
	//		m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineColor(RGB(0,255,255));
	//		m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineWidth(3);
	//		m_vpGuiAlnTargetImageMarkPos0.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(TRUE);
	//	}
	//}
	//nMovePosIndex = 1;
	//{

	//	if (nMovePosIndex < m_vpmpCalibMarkImagePos.size()
	//		&& TRUE == m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetIsMarkImagePosOK(nPosIndex))
	//	{
	//		CCoordPos pos;
	//		pos =  m_vpmpCalibMarkImagePos.at(nMovePosIndex)->GetMarkImagePos(nPosIndex);

	//		m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetCenterRotationLengths(
	//			sc2Vector(pos.GetPosX(),pos.GetPosY()),scDegree(45),100,100);
	//		m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineColor(RGB(255,0,255));
	//		m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetLineWidth(2);
	//		m_vpGuiAlnTargetImageMarkPos1.at(nPosIndex - m_pVisionASM->GetObjectPosNum())->SetVisible(TRUE);
	//	}
	//}
	return TRUE;
}

void CDlgCalibTargetPos4Board::UpdateDmCodeSearchResult(int nPosIndex)
{
	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pVisionASM->GetTotalProuductData();
	int nCurProductIdx = pTotalProductData->GetCurProductIndex();
	CProductData* pProductData;
	pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);
	scDMCodeParam DmCodeParam;
	DmCodeParam = pProductData->m_vDMcodeCalib.at(nPosIndex);
	m_vCalibBoardSize.SetX(DmCodeParam.m_FindCornerPara.m_dGridSize);
	m_vCalibBoardSize.SetY(DmCodeParam.m_FindCornerPara.m_dGridSize);

	std::vector<sc2Vector> tmpCornersImagePos;
	m_pVisionASM->m_vDmCodeCalibTool.at(nPosIndex).GetCornersImagePos(tmpCornersImagePos);
	std::vector<sc2Vector> tmpCornersLabelPos;
	m_pVisionASM->m_vDmCodeCalibTool.at(nPosIndex).GetCornersLabelPos(tmpCornersLabelPos);

	sc2Vector vMarkImagePos0 = tmpCornersImagePos.at(tmpCornersImagePos.size()/2);
	sc2Vector vMarkLabelPos0 = tmpCornersLabelPos.at(tmpCornersImagePos.size()/2)/DmCodeParam.m_FindCornerPara.m_dGridSize;
	int nMaxDistanceIndex = 0;
	double maxDistance = 0;
	for (int i=0;i<tmpCornersImagePos.size();i++)
	{
		double distance = vMarkImagePos0.Distance(tmpCornersImagePos.at(i));
		if ( distance > maxDistance)
		{
			nMaxDistanceIndex = i;
			maxDistance = distance;
		}
	}
	sc2Vector vMarkImagePos1 = tmpCornersImagePos.at(nMaxDistanceIndex);
	sc2Vector vMarkLabelPos1 = tmpCornersLabelPos.at(nMaxDistanceIndex)/DmCodeParam.m_FindCornerPara.m_dGridSize;

	m_vpmpCalibMarkImagePos = m_pVisionASM->GetCalibMarkImagPos();
	CCoordPos pos;
	pos.SetPos(vMarkImagePos0.GetX(),vMarkImagePos0.GetY());
	m_vpmpCalibMarkImagePos.at(0)->SetMarkImagePos(nPosIndex,TRUE,pos);
	pos.SetPos(vMarkLabelPos0.GetX(),vMarkLabelPos0.GetY());
	m_vpmpCalibMarkBoardPos.at(0)->SetMarkImagePos(nPosIndex,TRUE,pos);

	pos.SetPos(vMarkImagePos1.GetX(),vMarkImagePos1.GetY());
	m_vpmpCalibMarkImagePos.at(1)->SetMarkImagePos(nPosIndex,TRUE,pos);
	pos.SetPos(vMarkLabelPos1.GetX(),vMarkLabelPos1.GetY());
	m_vpmpCalibMarkBoardPos.at(1)->SetMarkImagePos(nPosIndex,TRUE,pos);
}

void CDlgCalibTargetPos4Board::OnBnClickedBtnSetDmcodeParam()
{
	// TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pVisionASM->GetTotalProuductData();
	if (pTotalProductData->GetCount()<= 0)
	{
		return;
	}

	CString strLog;
	strLog.Format(_T("点击[二维码参数设置]进入[二维码参数设置界面]\n"));
	g_opLogFile.SaveLogText(strLog,2);
	OnBnClickedBtnSerialGrab();
	CDlgCalibTargetDmcodeParam dlg;
	dlg.SetVisionASMRef(m_pVisionASM);	
	dlg.SetIsBoardCalibrate(true);
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlg.SetLogLevel(3);
	dlg.DoModal();

	OnBnClickedBtnSerialGrab();
}

/************************************************************************/
/* 
功能：
	创建多级目录
参数：
	CString& szPath	文件路径名
	char split		分割字符，根据路径设置
*/
/************************************************************************/
bool CDlgCalibTargetPos4Board::CreateMultipleDirectory(CString& szPath, char split)
{
	CString strDir(szPath);
	//	检测路径是否以 结尾
	if (strDir.GetAt(strDir.GetLength()-1) != split)
	{
		strDir.AppendChar(split);
	}
	std::vector<CString> vPath;	//	存放每一层目录字符串
	CString strTemp;			//	临时变量,存放目录字符串
	bool bSuccess = false;		//	成功标志
	// 遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != split) // 如果当前字符不是 split('\','//')
		{
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else //如果当前字符是'\\'
		{
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(split);
		}
	}

	//	遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//	如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

//获得关联结果
bool CDlgCalibTargetPos4Board::GetCalibResultCString(CString & strCalibResult)
{

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	int nPosNum = sysPlatformInfo.m_nPositionNum;

	std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
	vpAllCalibratedInfo = m_pVisionASM->GetAllCalibratedInfo();	

	CString strTemp = _T("");
	CString strOut = _T("");
	CCalibratedInfo* pCalibratedInfo;
	BOOL bIsOK = TRUE;  // 关联结果是否OK
	int nExProductIndex = 0;
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
	{
		nExProductIndex = m_comboExProductIndex.GetCurSel();
	}
	for (int nPosIndex = nPosNum/2;nPosIndex<nPosNum;nPosIndex++)
	{

		if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE )
		{
			pCalibratedInfo = vpAllCalibratedInfo.at(nPosIndex+nExProductIndex*nPosNum);
		}
		else
		{
			pCalibratedInfo = vpAllCalibratedInfo.at(nPosIndex);
		}



		if (pCalibratedInfo == NULL)
		{
			return false;
		}

		strTemp.Format(_T("%d,"),nPosIndex+1);strOut += strTemp;

		// 3*3矩阵
		double h[9];
		memset(h, 0.0, sizeof(double)*9);
		h[0] = 1.0;
		h[4] = 1.0;
		h[8] = 1.0;
		pCalibratedInfo->GetPlatformTransferH(h);
		strTemp.Format(_T("%.6f,"), h[0]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[1]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[2]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[3]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[4]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[5]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[6]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[7]);strOut += strTemp;
		strTemp.Format(_T("%.6f,"), h[8]);strOut += strTemp;

		if (sysPlatformInfo.m_bTargetObjectCamSeparate)
		{

			if (m_pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point ||
				m_pVisionASM->GetCalibTargetMarkPatternMode() == eDmCodeCalib)
			{
				double dMaxError = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX();
				strTemp.Format(_T("%.3f,"),dMaxError);
				strOut += strTemp;
			}
			else
			{
				bIsOK =TRUE;
			}
		}
		strTemp.Format(_T("\n"));
		strOut += strTemp;
		strOut += _T(",");
	}

	strCalibResult = strOut;
	return true;
}

// 保存CVS 文件
bool CDlgCalibTargetPos4Board::SaveCSVFile(CString strImageFileDirt,CString strName,CString& strSaveData )
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString strFileName;
	CString strTmp;
	CString strOut;

	strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
	strFileName.Format("%s\\%s.csv", strImageFileDirt, strName);

	//判断文件是否存在
	CStdioFile file;
	CFileStatus rStatus;
	BOOL bRes = CFile::GetStatus(strFileName, rStatus);

	if (!file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return false;
	}
	file.SeekToEnd();

	if(!bRes)//绘制表头
	{
		file.WriteString(_T("关联时间,位置,h0,h1,h2,h3,h4,h5,h6,h7,h8,MaxError\n"));
	}

	strTmp.Format("%04d-%02d-%02d-%02d-%02d-%02d-%03d,", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	strOut += strTmp;
	strOut += strSaveData;

	strOut+=_T("\n");
	file.WriteString(strOut);
	file.Close();
	return true;
}

// 保存畸变校正前的图片
void CDlgCalibTargetPos4Board::SaveCalibSearchImage(CString strImageFileDirt,CString strImageName,cpImage & imageSave)
{
	CString strTmp = strImageName;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString strFileName;
	CString strOut;

	strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
	strTmp.Append(strImageName);
	strFileName.Format("%s\\%s", strImageFileDirt, strTmp);


	scDIB scDibTmp;
	scDibTmp.Init(imageSave);
	scDibTmp.Write(strFileName);

}

void CDlgCalibTargetPos4Board::OnBnClickedSetCalibTargetShutter1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCalibTargetShutter(2);
}

void CDlgCalibTargetPos4Board::OnBnClickedSetCalibTargetShutter2()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCalibTargetShutter(3);
}

void CDlgCalibTargetPos4Board::ResetCameraShutter()
{
	if(m_pVisionASM)
	{
		for(int nPos=0;nPos<2 && nPos<m_pVisionASM->GetPosNum();nPos++)
		{
			if(m_pVisionASM->IsCameraFirstPosIndex(nPos+m_pVisionASM->GetObjectPosNum()))
			{
				m_pVisionASM->SwitchToSearchShutter(nPos+m_pVisionASM->GetObjectPosNum() , dShutterList[nPos]);
			}
		}
	}
}

void CDlgCalibTargetPos4Board::SetCalibTargetShutter(int nPosIndex)
{
	if(m_pVisionASM== NULL)
	{
		return;
	}

	if(nPosIndex>=m_pVisionASM->GetPosNum())
	{
		return;
	}


	CameraParam para;
	double dMin = 0, dMax = 0,dShutter = 0.0;
	if (!m_pVisionASM->GetPosCameraShutter(nPosIndex, dShutter,dMin,dMax))
	{
		AfxMessageBox(_T("获取相机曝光错误！"));
		return;
	}

	CVisionASMConfig *vsConfig;
	m_pVisionASM->GetVSconfig(vsConfig);

	CDlgCalibTargetShutterSetting dlg;
	dlg.m_nPosIndex = nPosIndex;
	dlg.SetVisionAlignerRef(m_pVisionASM);
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);


	dlg.m_dEnvironmentShutter = dShutter;
	double dCalibTargetShutter = dShutter;
	int nCam = m_pVisionASM->GetPosCameraIndex(nPosIndex);
	if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
	{
		dCalibTargetShutter = vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter;
	}


	if (dlg.DoModal() == IDOK)
	{
		int nCam = m_pVisionASM->GetPosCameraIndex(nPosIndex);
		if (NULL !=vsConfig && nCam<vsConfig->m_vCameraParam.size())
		{
			vsConfig->m_vCameraParam.at(nCam).m_dCalibTargetShutter = dlg.m_dDmCodeShutter;
			m_pVisionASM->SaveCameraInfo();
			m_pVisionASM->SaveJobCameraInfo();
		}
	}
	else
	{
		m_pVisionASM->SwitchToSearchShutter(nPosIndex, dCalibTargetShutter);
	}
}

void CDlgCalibTargetPos4Board::CallDlgSearch(int nPosIndex)
{
	if(m_pVisionASM== NULL)
	{
		return;
	}

	int nProductIdx= 0;

	if(m_pVisionASM->GetTotalProuductData())
	{
		nProductIdx = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	}
	CBaseSearchTool *pSearchTool = NULL;
	pSearchTool = m_pVisionASM->GetRepoAlnCalibTargetSearchTool(nProductIdx,nPosIndex,0);

	if (m_pVisionASM->m_pSystemStatusInfo!=NULL)
	{
		double dUsedRate = 0.0;

		CString strModuelDir;
		GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
		strModuelDir.ReleaseBuffer();
		CString StrTmp = strModuelDir.Left(strModuelDir.ReverseFind('\\'));   

		m_pVisionASM->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

		if ((m_pVisionASM->IsSysMemoryUsedRateWarning() ||  m_pVisionASM->IsDiskUsedRateWarning(StrTmp)) && pSearchTool==NULL)
		{

			CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
			CString strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);
			m_pVisionASM->SetStatusBarInfo(FALSE, strStatusBarInfo1, strStatusBarInfo2 + _T("\n无法新建模板！"));
			m_pVisionASM->PostMessageUpdateStatusBarInfo();

			CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
			m_pVisionASM->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！！"));
			// 避免后续无法保存的情况
			return;
		}
	}

	CString strTmpTitle = _T("定位模板设置");
	strTmpTitle.Format(_T("位置%d关联模板设置"),nPosIndex+1);

	CDlgSearch dlgSearch(this, pSearchTool);
	dlgSearch.m_nCurPositionIdx = nPosIndex;
	dlgSearch.m_strTitleName = strTmpTitle;
	dlgSearch.SetVisionASMRef(m_pVisionASM, nPosIndex);
	dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);


	if (dlgSearch.DoModal() == IDOK)
	{
		pSearchTool = dlgSearch.GetSearchTool();
		if(pSearchTool)
		{
			m_pVisionASM->SetRepoAlnCalibTargetSearchTool(nProductIdx, nPosIndex, 0, pSearchTool);
			m_pVisionASM->SaveAlnCalibTargetSearchToolRepository(nProductIdx);
		}
	}

}

void CDlgCalibTargetPos4Board::SetCalibTargetSearchTool(int nPosIndex)
{
	if(m_pVisionASM== NULL)
	{
		return;
	}

	if(nPosIndex<0 || nPosIndex>= m_pVisionASM->GetPosNum())
	{
		return;
	}

	m_pVisionASM->FreezeCamera(m_pVisionASM->GetPosCameraIndex(nPosIndex));
	// 停止刷新图像，避免dlgSearch 图像刷新
	m_bStopUpdateDisplayImage = TRUE;
	CallDlgSearch(nPosIndex);
	m_pVisionASM->GrabCamera(m_pVisionASM->GetPosCameraIndex(nPosIndex));
	m_bStopUpdateDisplayImage = FALSE;

}

LRESULT CDlgCalibTargetPos4Board::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{

	if(NULL== m_pVisionASM)
	{
		return S_OK;
	}

	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	if (eDmCodeCalib == m_pVisionASM->GetCalibTargetMarkPatternMode() || eDoublePatternSingleMark == m_pVisionASM->GetCalibTargetMarkPatternMode())
	{
		return S_OK;
	}


	//for(int it=0;it<m_vpGuiDisplay.size();it++)
	//{
	//	if(m_vpGuiDisplay.at(it) && m_vpGuiDisplay.at(it)->GetID()==wParam)
	//	{
	//		int nPos = it;
	//		SetCalibTargetSearchTool(nPos);
	//	}
	//}

	for(int it=0;it<m_vpGuiTargetDisplay.size();it++)
	{
		if(m_vpGuiTargetDisplay.at(it) && m_vpGuiTargetDisplay.at(it)->GetID()==wParam)
		{
			int nPos = it+m_pVisionASM->GetPosNum()/2;
			SetCalibTargetSearchTool(nPos);
		}
	}

	return S_OK;
}