// DlgCalcuVirtualObject.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgManualSearchCalib.h"
#include "afxdialogex.h"


// CDlgCalcuVirtualObject 对话框

IMPLEMENT_DYNAMIC(CDlgManualSearchCalib, CDialog)

CDlgManualSearchCalib::CDlgManualSearchCalib(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgManualSearchCalib::IDD, pParent)
	, m_strStepLength(_T(""))
{
	m_nType = 0;
	m_pVAPrevMainWnd = NULL;

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;

	m_vpGuiDisplay = NULL;

	m_vpPrevGuiDisplays = NULL;
	m_pVisionASM = NULL;

	m_psaSysInfoStrings = NULL;

	//m_vpGuiCross=NULL;

	m_bDisplayCross = FALSE;

	// 状态栏相关
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");

	m_vpGuiInteractiveContainer=NULL;


	m_vMarkCorrdPos.clear();
	m_vdPreShutter.clear();
	m_vPrevGrabStatus.clear();
	m_vPrevTriggerMode.clear();


	m_bIsGrabbing = FALSE;
	m_dMarkX = 0;
	m_dMarkY = 0;
	m_dStepLength = 0.5;

	m_nCurPosIndex = 0;
	m_nCurPlatformIndex = 0;


	m_nStepLen = 2;
}

CDlgManualSearchCalib::~CDlgManualSearchCalib()
{
	int i=0;


	{
		if (m_vpGuiDisplay!=NULL)
		{
			delete m_vpGuiDisplay;
			m_vpGuiDisplay = NULL;
		}
	}
	
	for(i=0; i< m_vpGuiCross.size();i++)
	{
		if (m_vpGuiCross.at(i) != NULL)
		{
			delete m_vpGuiCross.at(i);
			m_vpGuiCross.at(i) = NULL;
		}
	}


	//{
	//	if (m_vpGuiCross!=NULL)
	//	{
	//		delete m_vpGuiCross;
	//		m_vpGuiCross = NULL;
	//	}
	//}

	//m_pVisionASM = NULL;
}

void CDlgManualSearchCalib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SERIAL_GRAB, m_btnGrabImage);
	DDX_Control(pDX, IDC_STATIC_NOTE, m_labelNote);
	DDX_Text(pDX, IDC_EDIT_STEP_LENGTH, m_strStepLength);
	DDX_Control(pDX, IDC_BTN_UP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btnMoveLeft);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btnMoveRight);
	DDX_Radio(pDX, IDC_RADIO_025, m_nStepLen);
	DDV_MinMaxInt(pDX, m_nStepLen, 0, 3);
}


BEGIN_MESSAGE_MAP(CDlgManualSearchCalib, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SERIAL_GRAB, &CDlgManualSearchCalib::OnBnClickedBtnSerialGrab)
	ON_BN_CLICKED(IDCANCEL, &CDlgManualSearchCalib::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgManualSearchCalib::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_UP, &CDlgManualSearchCalib::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CDlgManualSearchCalib::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDC_BTN_LEFT, &CDlgManualSearchCalib::OnBnClickedBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, &CDlgManualSearchCalib::OnBnClickedBtnRight)
	ON_EN_SETFOCUS(IDC_EDIT_STEP_LENGTH, &CDlgManualSearchCalib::OnEnSetfocusEditStepLength)
	ON_MESSAGE(WM_SV_DISPLAY_DRAGSTOP,OnGuiDisplayDrapStop)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK,OnGuiDisplayDoubleClick)
	ON_MESSAGE(WM_SV_DISPLAY_MOUSEMOVE,OnGuiDisplayMouseMove)
END_MESSAGE_MAP()


// CDlgCalcuVirtualObject 消息处理程序


void CDlgManualSearchCalib::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
{
	if (m_pVisionASM != NULL)
	{

		{
			m_pVisionASM->SetPosDisplay(m_nCurPosIndex, m_vpPrevGuiDisplays);
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}

	m_pVisionASM = (vcXYDVisionAlign*)pVisionAligner;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay

		{
			m_vpPrevGuiDisplays = m_pVisionASM->GetPosDisplay(m_nCurPosIndex);
			m_pVisionASM->SetPosDisplay(m_nCurPosIndex, m_vpGuiDisplay);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
	}
}

BOOL CDlgManualSearchCalib::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    UpdateDialogLanguage();
	InitDlgItem();		

	// 初始化GUI

	SetGUI();
	
	OnBnClickedBtnSerialGrab();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgManualSearchCalib::InitDlgItem()
{
	// 	// 	CRect cr;
	// 	// 	GetClientRect(&cr);//获取对话框客户区域大小
	// 	// 	ClientToScreen(&cr);//转换为荧幕坐标
	// 	int x= GetSystemMetrics(SM_CXSCREEN);//获取荧幕坐标的宽度，单位为像素
	// 	int y= GetSystemMetrics(SM_CYSCREEN);//获取荧幕坐标的高度，单位为像素
	// 	MoveWindow(40, 30, (x - 80), (y - 60));//设置窗口显示在屏幕中间

	// m_pVisionASM->OpenComm();

	m_labelNote.SetBkColor(RGB(0,255,255));
	m_labelNote.SetFontSize(10);
	m_labelNote.SetFontBold(TRUE);
	m_labelNote.SetText(_T(""));

	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
// 	int nPartWidth[3] = {20, 300, -1};
// 	m_pStatusBar->SetParts(3, nPartWidth);
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	int nPartWidth[4] = {20, 400, 750, -1};
	m_pStatusBar->SetParts(4, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);

	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = _T("");
	m_strStatusBarInfo2 = _T("");
	UpdateStatusBar();

	CString strTitle;
	if (m_nType==0)
	{
		strTitle.Format("平台%d 目标Mark手动选点",m_nCurPlatformIndex);
	}
	else
	{
		strTitle.Format("平台%d 对象Mark手动选点",m_nCurPlatformIndex);
	}
	
 	SetWindowText(strTitle);
// 	GetDlgItem(IDC_STATIC_CH0_CAP)->SetWindowText((m_nType == 0) ? _T("CH0") : _T("CH2"));
// 	GetDlgItem(IDC_STATIC_CH1_CAP)->SetWindowText((m_nType == 0) ? _T("CH1") : _T("CH3"));

	BOOL bRepaint = FALSE;
	m_btnOK.SetThemeHelper(&m_themeHelper);	
// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	m_btnOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);

	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);
	// 	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, CButtonST::BTNST_COLOR_BK_OUT, bRepaint);

	m_btnGrabImage.SetThemeHelper(&m_themeHelper);	
// 	m_btnGrabImage.SetIcon(IDI_ICON_GRAB_40);
// 	m_btnGrabImage.SetAlign(CButtonST::ST_ALIGN_VERT);

//	m_btnAssistSearch.SetIcon(IDI_ICON_EXTRACT_POINT_48);
//	m_btnAssistSearch.SetAlign(CButtonST::ST_ALIGN_VERT);

	int i = 0;
// 	for (i=0; i<m_vpGuiDisplay.size(); i++)
// 	{
// 	 	delete m_vpGuiDisplay[i];
// 	 	m_vpGuiDisplay[i] = NULL;
// 	}
// 	m_vpGuiDisplay.clear();
// 	m_vpGuiInteractiveContainer.clear();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	CString strSelectPos;
	switch (m_nCurPosIndex)
	{
		case 0:
			strSelectPos.Format(_T("搜索对象%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		case 1:
			strSelectPos.Format(_T("搜索对象%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		case 2:
			strSelectPos.Format(_T("搜索对象%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		case 3:
			strSelectPos.Format(_T("搜索对象%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		case 4:
			strSelectPos.Format(_T("搜索目标%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		case 5:
			strSelectPos.Format(_T("搜索目标%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		case 6:
			strSelectPos.Format(_T("搜索目标%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		case 7:
			strSelectPos.Format(_T("搜索目标%d移动位置%d"),m_nCurPosIndex+1,nMoveIndex+1);
			break;
		default:
			strSelectPos.Format(_T("移动十字光标选择显示"));
			break;
	}

	{
	 	m_vpGuiDisplay = new scGuiDisplay;
	 	m_vpGuiDisplay->Create(IDC_STATIC_CH0_DISPLAY, this,WS_VISIBLE);
		m_vpGuiDisplay->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		m_vpGuiDisplay->RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DRAGSTOP|SV_DISPLAY_DBLCLICK);

		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_vpGuiDisplay->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_vpGuiDisplay->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			m_vpGuiDisplay->SetLanguage(1);
			break;
		}

		m_vGuiCrossVar.SetFixText(strSelectPos);
		scGuiStaticContainer* pGuiConstContainer = NULL;
		pGuiConstContainer = m_vpGuiDisplay->GetConstContainer();
		pGuiConstContainer->AddItem(m_vGuiCrossVar.m_pGuiTextShow, "GuiTextShow");
		m_vpGuiDisplay->SetConstContainer(pGuiConstContainer);


		m_pVisionASM->SetPosDisplay(m_nCurPosIndex, m_vpGuiDisplay);
		m_vpGuiDisplay->ShowWindow(SW_SHOW);
		//pGUIDisplay = NULL;
	}
	


	CString strInfo;
	m_dStepLength = 0.5;
	strInfo.Format(_T("%f"),m_dStepLength);
	GetDlgItem(IDC_EDIT_STEP_LENGTH)->SetWindowText(strInfo);

	m_btnMoveUp.SetThemeHelper(&m_themeHelper);
	m_btnMoveUp.SetIcon(IDI_ICON_MOVE_UP_32);
	m_btnMoveDown.SetThemeHelper(&m_themeHelper);
	m_btnMoveDown.SetIcon(IDI_ICON_MOVE_DOWN_32);
	m_btnMoveLeft.SetThemeHelper(&m_themeHelper);
	m_btnMoveLeft.SetIcon(IDI_ICON_MOVE_LEFT_32);
	m_btnMoveRight.SetThemeHelper(&m_themeHelper);
	m_btnMoveRight.SetIcon(IDI_ICON_MOVE_RIGHT_32);
}

void CDlgManualSearchCalib::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}


void CDlgManualSearchCalib::OnDestroy()
{
	CDialog::OnDestroy();

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
		if (!m_bIsGrabbing)
		{
			m_pVisionASM->GrabCamera(m_nCurPosIndex);
		}		

		// 还原旧的GuiDisplay
		//int nNum = m_pVisionASM->GetPosNum();

		{
			m_pVisionASM->SetPosDisplay(m_nCurPosIndex, m_vpPrevGuiDisplays);
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}


}


LRESULT CDlgManualSearchCalib::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgManualSearchCalib::UpdateBtnLiveGrab()
{
	// 更新“连续采集/停止采集”按钮 
// 	int nCamNum = m_pVisionASM->GetPosNum();
// 	nCamNum = (nCamNum == 4) ? 2 : nCamNum;

	BOOL bSucceed = TRUE;
	PTTriggerMode mode = ePTNotTrigger;
	for (int i=0; i<1; i++)
	{
		if (m_pVisionASM->IsPosCameraOnline(m_nCurPosIndex))
		{
//			bSucceed &= m_pVisionAligner->GetCameraTriggerMode(mode, i+(m_nType<<1));
			if (mode != ePTNotTrigger)
			{
//				m_pVisionAligner->SetCameraTriggerMode(ePTNotTrigger, i+(m_nType<<1));		// 连续采集
			}
		}

		if (m_pVisionASM->IsPosCameraGrabbing(m_nCurPosIndex))
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
			if (m_pVisionASM->GrabCamera(m_nCurPosIndex))
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

BOOL CDlgManualSearchCalib::SetGUI()
{
	// 十字
	m_vGuiCrossVar.SetDisplay(m_vpGuiDisplay);
	m_vGuiCrossVar.SetCenterXYWidthHeight(100,100,40,40);
	m_vpGuiInteractiveContainer = m_vpGuiDisplay->GetInteractiveContainer();

	m_vpGuiDisplay->SetInteractiveContainer(m_vpGuiInteractiveContainer);


	return TRUE;	
}

void CDlgManualSearchCalib::OnBnClickedBtnSerialGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	int i=0;
	BOOL bSucceed = TRUE;
	int nCamNum = m_pVisionASM->GetPosNum();

	if (m_bIsGrabbing)
	{		
		if (m_pVisionASM->IsCameraOnline(m_nCurPosIndex))
		{
			bSucceed = m_pVisionASM->FreezeCamera(m_nCurPosIndex);
			GetDlgItem(IDC_BTN_SERIAL_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")

		}
		if (!bSucceed)
		{
			CString strInfo, strNote;
			// roll back（如果一个相机停止失败，则恢复所有成功停止的相机为采集状态）
			m_pVisionASM->GrabPosCamera(m_nCurPosIndex);
	
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
				strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB));
			UpdateStatusBar();
			return;
		}
		m_bIsGrabbing = FALSE;
	}
	else
	{
		if (m_pVisionASM->IsCameraOnline(m_nCurPosIndex))
		{
			bSucceed = m_pVisionASM->GrabCamera(m_nCurPosIndex);
			GetDlgItem(IDC_BTN_SERIAL_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")

		}

		if (!bSucceed)
		{
			CString strInfo, strNote;

			m_pVisionASM->FreezeCamera(m_nCurPosIndex);

			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			m_strStatusBarInfo2.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_SSS_SSS), 
			strNote.Left(strNote.GetLength()), GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_START_GRAB));
			UpdateStatusBar();
			return;
		}
		m_bIsGrabbing = TRUE;
	}
}


void CDlgManualSearchCalib::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	m_mpMarkImagePos.SetMarkImagePosNum(1);
	{
		CCoordPos MarkCorrdPos;
		
		//m_vpGuiCross.at(0)->GetCenterXY(MarkCorrdPos.m_dPosX, MarkCorrdPos.m_dPosY);
		for (int i = 0;i<m_nMarkNum;i++)
		{
			if ( 0 == nMoveIndex)
			{
// 				sc2Vector vCenter;
// 				vCenter = m_vpGuiCross.at(i)->GetCenter();
				m_vGuiCrossVar.GetCenter();
				sc2Vector vCenter = m_vGuiCrossVar.GetCenter();
				if (i ==0)
				{
					MarkCorrdPos.m_dPosX = vCenter.GetX();
					MarkCorrdPos.m_dPosY = vCenter.GetY();
				}
				MarkCorrdPos.m_vdAuxiliaryPosX.at(i) = vCenter.GetX();
				MarkCorrdPos.m_vdAuxiliaryPosY.at(i) = vCenter.GetY();
			}
			else if(1 ==nMoveIndex)
			{
				m_vGuiCrossVar.GetCenter();
				sc2Vector vCenter = m_vGuiCrossVar.GetCenter();
				if (i ==0)
				{
					MarkCorrdPos.m_dPosX = vCenter.GetX();
					MarkCorrdPos.m_dPosY = vCenter.GetY();
				}
				MarkCorrdPos.m_vdAuxiliaryPosX.at(i+2) = vCenter.GetX();
				MarkCorrdPos.m_vdAuxiliaryPosY.at(i+2) = vCenter.GetY();
			}
			
		}
		m_mpMarkImagePos.SetMarkImagePos(0,TRUE,MarkCorrdPos);
	}
	CDialog::OnOK();
}


void CDlgManualSearchCalib::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}



void CDlgManualSearchCalib::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		OnUp();		
	}
	else if (nIDEvent == 2)
	{
		OnDown();	
	}
	else if (nIDEvent == 3)
	{
		OnLeft();	
	}
	else if (nIDEvent == 4)
	{
		OnRight();	
	}
	CDialog::OnTimer(nIDEvent);
}


void CDlgManualSearchCalib::SetMarkType(int ntype)
{
	m_nType = ntype;
}


void CDlgManualSearchCalib::OnBnClickedBtnUp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_vpGuiDisplay->GetImageCP()->IsValid() && m_vGuiCrossVar.m_vGuiCoorCross!=NULL)
	{			
		if (m_vGuiCrossVar.m_vGuiCoorCross->IsSelected())
		{
			vdMarkCenter = m_vGuiCrossVar.GetCenter();
			vdMarkCenter.SetY(vdMarkCenter.GetY() - m_dStepLength);
			m_vGuiCrossVar.SetCrossCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}			
	}	
}


void CDlgManualSearchCalib::OnBnClickedBtnDown()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_vpGuiDisplay->GetImageCP()->IsValid() && m_vGuiCrossVar.m_vGuiCoorCross!=NULL)
	{			
		if (m_vGuiCrossVar.m_vGuiCoorCross->IsSelected())
		{
			vdMarkCenter = m_vGuiCrossVar.GetCenter();
			vdMarkCenter.SetY(vdMarkCenter.GetY() + m_dStepLength);
			m_vGuiCrossVar.SetCrossCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}			
	}	
}


void CDlgManualSearchCalib::OnBnClickedBtnLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_vpGuiDisplay->GetImageCP()->IsValid() && m_vGuiCrossVar.m_vGuiCoorCross!=NULL)
	{			
		if (m_vGuiCrossVar.m_vGuiCoorCross->IsSelected())
		{
			vdMarkCenter = m_vGuiCrossVar.GetCenter();
			vdMarkCenter.SetX(vdMarkCenter.GetX() - m_dStepLength);
			m_vGuiCrossVar.SetCrossCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}			
	}	
}


void CDlgManualSearchCalib::OnBnClickedBtnRight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_vpGuiDisplay->GetImageCP()->IsValid() && m_vGuiCrossVar.m_vGuiCoorCross!=NULL)
	{			
		if (m_vGuiCrossVar.m_vGuiCoorCross->IsSelected())
		{
			vdMarkCenter = m_vGuiCrossVar.GetCenter();
			vdMarkCenter.SetX(vdMarkCenter.GetX() + m_dStepLength);
			m_vGuiCrossVar.SetCrossCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}			
	}	
}

//连续移动在定时器中实现
void CDlgManualSearchCalib::OnUp() 
{
	if (FALSE == CheckMoveStepValid())
		return ;
	if (m_vpGuiDisplay->GetImageCP()->IsValid() && m_vGuiCrossVar.m_vGuiCoorCross!=NULL)
	{			
		if (m_vGuiCrossVar.m_vGuiCoorCross->IsSelected())
		{
			vdMarkCenter = m_vGuiCrossVar.GetCenter();
			vdMarkCenter.SetY(vdMarkCenter.GetY() - m_dStepLength);
			m_vGuiCrossVar.SetCrossCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}			
	}
	for (int i=0;i<m_vpGuiCross.size();i++)
	{
		if (m_vpGuiCross.at(i)->IsSelected())
		{
			//m_vpGuiCross.at(i)->GetCenterXY(m_dMarkX, m_dMarkY);
			//m_dMarkY -= m_dStepLength;
			//m_vpGuiCross.at(i)->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,150,150);
			vdMarkCenter = m_vpGuiCross.at(i)->GetCenter();
			vdMarkCenter.SetY(vdMarkCenter.GetY()- m_dStepLength);
			m_vpGuiCross.at(i)->SetCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}
	}	
}

void CDlgManualSearchCalib::OnDown() 
{
	//if (FALSE == CheckMoveStepValid())
	//	return ;

	for (int i=0;i<m_vpGuiCross.size();i++)
	{
		if (m_vpGuiCross.at(i)->IsSelected())
		{
			//m_vpGuiCross.at(i)->GetCenterXY(m_dMarkX, m_dMarkY);
			//m_dMarkY += m_dStepLength;
			//m_vpGuiCross.at(i)->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,150,150);
			vdMarkCenter = m_vpGuiCross.at(i)->GetCenter();
			vdMarkCenter.SetY(vdMarkCenter.GetY()+ m_dStepLength);
			m_vpGuiCross.at(i)->SetCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}
	}
}

void CDlgManualSearchCalib::OnLeft() 
{
	if (FALSE == CheckMoveStepValid())
		return ;

	for (int i=0;i<m_vpGuiCross.size();i++)
	{
		if (m_vpGuiCross.at(i)->IsSelected())
		{
			//m_vpGuiCross.at(i)->GetCenterXY(m_dMarkX, m_dMarkY);
			//m_dMarkX -= m_dStepLength;
			//m_vpGuiCross.at(i)->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,150,150);
			vdMarkCenter = m_vpGuiCross.at(i)->GetCenter();
			vdMarkCenter.SetX(vdMarkCenter.GetX()- m_dStepLength);
			m_vpGuiCross.at(i)->SetCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}
	}	
}

void CDlgManualSearchCalib::OnRight() 
{
	if (FALSE == CheckMoveStepValid())
		return ;

	for (int i=0;i<m_vpGuiCross.size();i++)
	{
		if (m_vpGuiCross.at(i)->IsSelected())
		{
			//m_vpGuiCross.at(i)->GetCenterXY(m_dMarkX, m_dMarkY);
			//m_dMarkX += m_dStepLength;
			//m_vpGuiCross.at(i)->SetCenterXYWidthHeight(m_dMarkX, m_dMarkY,150,150);
			vdMarkCenter = m_vpGuiCross.at(i)->GetCenter();
			vdMarkCenter.SetX(vdMarkCenter.GetX()+ m_dStepLength);
			m_vpGuiCross.at(i)->SetCenter(vdMarkCenter);
			m_vpGuiDisplay->MyInvalidate();
		}
	}
}

BOOL CDlgManualSearchCalib::CheckMoveStepValid()
{
	UpdateData(TRUE);
	int nLength = m_strStepLength.GetLength();
	if (0 == nLength)
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INPUT_VALID_STEP_LENGTH));	/*_T("请输入有效的微调步长！")*/
		GetDlgItem(IDC_EDIT_STEP_LENGTH)->SetFocus();

		return FALSE;
	}

	TCHAR everyChar;
	BOOL bIsIuputOK;
	for (int i=0; i<nLength; i++)
	{
		everyChar = m_strStepLength.GetAt(i);
		bIsIuputOK = ((everyChar>='0')&&(everyChar<='9'))||(everyChar=='.');
		if (FALSE == bIsIuputOK)
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INPUT_VALID_STEP_LENGTH));	/*_T("请输入有效的微调步长！")*/
			GetDlgItem(IDC_EDIT_STEP_LENGTH)->SetFocus();

			return FALSE;
		}
	}

	int iNum = 0;
	for (int j=0; j<nLength; j++)
	{
		everyChar = m_strStepLength.GetAt(j);
		if (everyChar=='.') iNum++;
	}
	if (iNum>=2)
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INPUT_VALID_STEP_LENGTH));	/*_T("请输入有效的微调步长！")*/
		GetDlgItem(IDC_EDIT_STEP_LENGTH)->SetFocus();

		return FALSE;
	}
	m_dStepLength = atof(m_strStepLength);

	if (m_nStepLen == 0)
	{
		m_dStepLength = 0.25;
	}
	else if (m_nStepLen == 1)
	{
		m_dStepLength = 0.5;
	}
	else if (m_nStepLen == 2)
	{
		m_dStepLength = 1;
	}
	else
	{
		m_dStepLength = 5;
	}
	
	return TRUE;
}

BOOL CDlgManualSearchCalib::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialog::PreTranslateMessage(pMsg);

	if (pMsg->hwnd == GetDlgItem(IDC_BTN_UP)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			SetTimer(1,25, 0);
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			KillTimer(1);
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DOWN)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			SetTimer(2,25, 0);
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			KillTimer(2);
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_LEFT)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			SetTimer(3,25, 0);
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			KillTimer(3);
		}
	}
	else if (pMsg->hwnd == GetDlgItem(IDC_BTN_RIGHT)->GetSafeHwnd())
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			SetTimer(4,25, 0);
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			KillTimer(4);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgManualSearchCalib::OnEnSetfocusEditStepLength()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_STEP_LENGTH)->GetWindowRect(rcWnd);
		GetDlgItemText(IDC_EDIT_STEP_LENGTH, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_STEP_LENGTH, strOutput);
		}
	}
}

void CDlgManualSearchCalib::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_MANUAL_SEARCH") == strDlgID)
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
}

LRESULT CDlgManualSearchCalib::OnGuiDisplayDrapStop(WPARAM wParam, LPARAM lParam)
{
	if (wParam == m_vpGuiDisplay->GetID())
	{
		if (m_vpGuiDisplay->GetImageCP() != NULL)
		{
			if (m_vpGuiDisplay->GetImageCP()->IsValid())
			{	
				//m_vGuiCrossVar.GuiCrossToLineseg();
				m_vGuiCrossVar.Dragging();			
			}
		}	
	}
	return 0;
}

//双击十字转换成直线暂时不用
LRESULT CDlgManualSearchCalib::OnGuiDisplayDoubleClick(WPARAM wParam, LPARAM lParam)
{
	if (wParam == m_vpGuiDisplay->GetID())
	{
		if (m_vpGuiDisplay->GetImageCP() != NULL)
		{
			if (m_vpGuiDisplay->GetImageCP()->IsValid() && m_vGuiCrossVar.m_vGuiCoorCross!=NULL)
			{			
				if (m_vGuiCrossVar.m_vGuiCoorCross->IsSelected())
				{
					//m_vGuiCrossVar.GuiCrossToLineseg();
				}			
			}
		}	
	}
	return 0;
}

LRESULT CDlgManualSearchCalib::OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam)
{
	CString strStatus = " ";

	if (wParam == m_vpGuiDisplay->GetID())
	{
		if (m_vpGuiDisplay->GetImageCP() != NULL)
		{
			if (m_vpGuiDisplay->GetImageCP()->IsValid())
			{
				long nWidth  = m_vpGuiDisplay->GetImageCP()->Width();
				long nHeight = m_vpGuiDisplay->GetImageCP()->Height();

				// 获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标,低16位存储的是X坐标
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
				double dImageX, dImageY;
				m_vpGuiDisplay->TransformPos(x,y,dImageX,dImageY);

				// 获取对应像素灰度值
				BYTE R,G,B;
				m_vpGuiDisplay->GetImagePixelValue(LONG(dImageX),LONG(dImageY),R,G,B);
				if(dImageX<0||dImageY<0||dImageY>=nHeight||dImageX>=nWidth)
				{
					R = 0;
					G = 0;
					B = 0;
				}
				//m_svGuiCrossVar.SetDisplay(&m_GuiDisplay);
				//m_svGuiCrossVar.SetFixText("十字坐标显示");
				//m_svGuiCrossVar.m_pGuiTextShow->SetVisible(TRUE);
				strStatus.Format("(%.0f,%.0f)  (%d,%d,%d)", dImageX, dImageY, R, G, B);
			}
		}	
	}
	m_pStatusBar->SetText(strStatus,3,0);
	return 0 ;
}
